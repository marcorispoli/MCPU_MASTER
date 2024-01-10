#include "ExposureModule.h"
#include "Generator.h"
#include "R2CP_Eth.h"
#include "CaDataDicGen.h"
#include "PCB304.h"
#include "PCB301.h"
#include <thread>

using namespace System::Diagnostics;
using namespace System::Collections::Generic;

bool Generator::man_2d_exposure_procedure(void) {
    bool large_focus;
    bool detector_synch = true;
    bool grid_synch = true;

    // Set thecommunication error code as general case
    xray_exposure_error = exposure_completed_errors::XRAY_COMMUNICATION_ERROR;

    // Checks if the X-RAY push button is pressed
    if (!PCB301::getXrayPushButtonStat()) {
        xray_exposure_error = exposure_completed_errors::XRAY_BUTTON_RELEASE;
        return false;
    }

    // Sets the Grid On Field (if not yet) : wait for the ready condition
    PCB304::setGridOnField();
    while (true) {
        if (PCB304::isError()) {
            xray_exposure_error = exposure_completed_errors::XRAY_GRID_ERROR;
            return false;
        }
        if (PCB304::isGridOnFieldReady()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Synch the Grid with the generator
    PCB304::synchGridWithGenerator(true);
    while (true) {
        if (PCB304::isError()) {
            xray_exposure_error = exposure_completed_errors::XRAY_GRID_ERROR;
            return false;
        }
        if (PCB304::isGridSynchReady()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }


    // Determines if the Focus is Small or large based on the presence of the Magnifier device
    if (PCB304::isMagnifierDeviceDetected()) large_focus = false;
    else large_focus = true;

    // Filter in Auto Mode: the current filter is the one in the pulse(0)
    PCB315::setFilterAutoMode(ExposureModule::getExposurePulse(0)->filter);

    // The format collimation should be already configured

    // Starter activation: requires a special command from Sedecal

    // Load Data Bank For pulse
    Debug::WriteLine("GENERATOR MAN 2D EXPOSURE: Setup Databank for Pulse");
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pulse, large_focus, ExposureModule::getExposurePulse(0)->kV, ExposureModule::getExposurePulse(0)->mAs, 5000);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Procedure activation
    Debug::WriteLine("GENERATOR MAN 2D EXPOSURE: procedure activation");
    R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DProcedurePulse(detector_synch, grid_synch);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Checks if a system message should be present before to proceed
    Debug::WriteLine("GENERATOR MAN 2D EXPOSURE: check internal error messages");
    for (int i = 0; i < R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size(); i++) {
        unsigned int id = R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList[i];
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_Clear_Message(id);
        if (!handleCommandProcessedState(nullptr)) return false;
    }

    // Read again the messages
    R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();
    if (!handleCommandProcessedState(nullptr)) return false;

    // Test if the messages have been removed
    if (R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size()) {
        xray_exposure_error = exposure_completed_errors::XRAY_GENERATOR_ERROR;
        return false;
    }

    // Disables the safety disable RX message
    Debug::WriteLine("GENERATOR MAN 2D EXPOSURE: reset the Disable-RX message");
    R2CP::CaDataDicGen::GetInstance()->SystemMessages_SetDisableRx(false);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Activate the X-RAY Enable Interface signal on the PCB301 board
    PCB301::setXrayEnable(true);

    // Wait for the Exposure in progress
    bool exit_condition = false;
    unsigned char current_status;

    while (!exit_condition) {
        
        // Gets the current generator status
        R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
        if (!handleCommandProcessedState(nullptr)) return false;
        current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;        
        
        // Checks for error messages
        if (R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active) {
            xray_exposure_error = exposure_completed_errors::XRAY_GENERATOR_ERROR;
            return false;
        }
        
        switch (current_status) {
        case R2CP::Stat_Standby: continue;
        case R2CP::Stat_Error:
            Debug::WriteLine("GENERATOR PREPARATION ERROR: Internal error condition");
            xray_exposure_error = exposure_completed_errors::XRAY_GENERATOR_ERROR;
            return false;

        case R2CP::Stat_WaitFootRelease:
        case R2CP::Stat_GoigToShutdown:
        case R2CP::Stat_Service:
        case R2CP::Stat_Initialization:
            Debug::WriteLine("GENERATOR PREPARATION ERROR: Wrong generator status");
            xray_exposure_error = exposure_completed_errors::XRAY_GENERATOR_ERROR;
            return false;

        // The Generator is in preparation: proceed with the sequence
        case R2CP::Stat_ExpInProgress:
        case R2CP::Stat_Ready:
            exit_condition = true;
            break;
        }
    }
    

    // Exposure execution
    exit_condition = false;
    while (!exit_condition) {

        // Gets the current generator status
        R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
        if (!handleCommandProcessedState(nullptr)) return false;
        current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

        // Checks for error messages
        if (R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active) {
            xray_exposure_error = exposure_completed_errors::XRAY_GENERATOR_ERROR;
            return false;
        }
        
        // Checks for the XRAY button
        if (!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus) {
            xray_exposure_error = exposure_completed_errors::XRAY_BUTTON_RELEASE;
            return false;
        }

        // Waits for the Exposure complete event
        switch (current_status) {
            
        case R2CP::Stat_WaitFootRelease: // Exposure terminated
        case R2CP::Stat_Standby:
            exit_condition = true;
            break;

        case R2CP::Stat_Error:
        case R2CP::Stat_GoigToShutdown:
        case R2CP::Stat_Service:
        case R2CP::Stat_Initialization:
            Debug::WriteLine("GENERATOR EXECUTION ERROR: Wrong generator status");
            xray_exposure_error = exposure_completed_errors::XRAY_GENERATOR_ERROR;
            return false;

        default:
            break;
        }

    }

    // Waits for Standby condition
    exit_condition = false;
    while (!exit_condition) {
        
        // Gets the current generator status
        R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
        if (!handleCommandProcessedState(nullptr)) return false;
        current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

        if (current_status == R2CP::Stat_Standby) exit_condition = true;

    }


    return false;
};

bool Generator::aec_2d_exposure_procedure(void) {
    // Set thecommunication error code as general case
    xray_exposure_error = exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    return false;

};
