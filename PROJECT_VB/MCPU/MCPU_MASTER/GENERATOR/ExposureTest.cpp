#include "ExposureModule.h"
#include "Generator.h"
#include "R2CP_Eth.h"
#include "CaDataDicGen.h"
#include "PCB304.h"
#include "PCB301.h"
#include <thread>
#include "Log.h"

using namespace System::Diagnostics;
using namespace System::Collections::Generic;

ExposureModule::exposure_completed_errors Generator::test_exposure_procedure(void) {
    unsigned char focus = ExposureModule::getExposurePulse(0)->getFocus();
    bool large_focus = ExposureModule::getExposurePulse(0)->isLargeFocus();
    bool detector_synch = false;
    bool grid_synch = ExposureModule::getExposurePulse(0)->useGrid();
    PCB315::filterMaterialCodes filter;
    unsigned char current_status;

    // Set thecommunication error code as general case
    //xray_exposure_error = exposure_completed_errors::XRAY_COMMUNICATION_ERROR;

    // Checks if the X-RAY push button is pressed
    if (!PCB301::getXrayPushButtonStat()) {
        return ExposureModule::exposure_completed_errors::XRAY_BUTTON_RELEASE;
    }

    // Sets the Grid On/Off Field and wait for the ready condition
    if (grid_synch) {
        PCB304::setGridOnField();
        PCB304::synchGridWithGenerator(true);
    }
    else {
        PCB304::setGridOffField();
        PCB304::synchGridWithGenerator(false);
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (PCB304::isError()) {
            return ExposureModule::exposure_completed_errors::XRAY_GRID_ERROR;
        }

        if (grid_synch) {
            if (!PCB304::isGridSynchReady()) continue;
            if (!PCB304::isGridOnFieldReady()) continue;
            
        }
        else  if (!PCB304::isGridOffFieldReady()) continue;        
        break;        
    }


    // Filter in Auto Mode: the current filter is the one in the pulse(0)
    filter = ExposureModule::getExposurePulse(0)->filter;
    PCB315::setFilterAutoMode(filter);

    // The format collimation should be already configured

    // Starter activation: requires a special command from Sedecal

    // Load Data Bank For pulse
    LogClass::logInFile("GENERATOR MAN TEST EXPOSURE: Setup Databank for Pulse");
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pulse, large_focus, ExposureModule::getExposurePulse(0)->kV, ExposureModule::getExposurePulse(0)->mAs, 5000);
    if (!handleCommandProcessedState(nullptr)) return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;

    // Procedure activation
    LogClass::logInFile("GENERATOR MAN TEST EXPOSURE: procedure activation");
    R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DProcedurePulse(detector_synch, grid_synch);
    if (!handleCommandProcessedState(nullptr)) return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;

    // Checks if a system message should be present before to proceed
    LogClass::logInFile("GENERATOR MAN TEST EXPOSURE: check internal error messages");
    for (int i = 0; i < R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size(); i++) {
        unsigned int id = R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList[i];
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_Clear_Message(id);
        if (!handleCommandProcessedState(nullptr)) return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Read again the messages
    R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();
    if (!handleCommandProcessedState(nullptr)) return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;

    // Test if the messages have been removed
    if (R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size()) {
        return ExposureModule::exposure_completed_errors::XRAY_GENERATOR_ERROR;
    }

    // Disables the safety disable RX message
    LogClass::logInFile("GENERATOR MAN TEST EXPOSURE: reset the Disable-RX message");
    R2CP::CaDataDicGen::GetInstance()->SystemMessages_SetDisableRx(false);
    if (!handleCommandProcessedState(nullptr)) return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;

    // Gets the current generator status
    R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
    if (!handleCommandProcessedState(nullptr)) return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    // Status not in standby
    if (current_status != R2CP::Stat_Standby) {
        return ExposureModule::exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
    }

   
    // Activate the X-RAY Enable Interface signal on the PCB301 board
    PCB301::set_xray_ena(true);

    // Wait to exit from standby
    while (true) {
        // Gets the current generator status
        R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
        if (!handleCommandProcessedState(nullptr)) return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
        if (current_status != R2CP::Stat_Standby) break;
    }

   
    logCurrentStatus(255);

    // Exposure execution
    bool exit_condition = false;    
    ExposureModule::exposure_completed_errors error;

    while (!exit_condition) {

        // Gets the current generator status
        R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
        if (!handleCommandProcessedState(nullptr)) {
            error = ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
            exit_condition = true;
            continue;
        }
        current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

        // Checks for the XRAY button
        if (!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus) {            
            error = ExposureModule::exposure_completed_errors::XRAY_BUTTON_RELEASE;
            exit_condition = true;
            continue;
        }

        // Checks for error messages
        if (R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active) {            
            error = ExposureModule::exposure_completed_errors::XRAY_GENERATOR_ERROR;
            exit_condition = true;
            continue;
        }

        if (logCurrentStatus(current_status)) {
            // Waits for the Exposure complete event
            switch (current_status) {

            case R2CP::Stat_WaitFootRelease: // Exposure terminated
            case R2CP::Stat_Standby:
                error = ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS;
                exit_condition = true;
                break;

            case R2CP::Stat_Error:                
                error = ExposureModule::exposure_completed_errors::XRAY_GENERATOR_ERROR;
                exit_condition = true;
                break;

            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:   
                error = ExposureModule::exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
                exit_condition = true;
                break;

            default:
                break;
            }
        }

    }
    
    // Updates the Exposed pulses  
    unsigned char databank_index = 1;      
    setExposedData(databank_index, (unsigned char) 0, filter, focus);
    return error;

};
