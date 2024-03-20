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
    System::String^ ExpName = "Exposure Test>";
    unsigned char focus = ExposureModule::getExposurePulse(0)->getFocus();
    bool large_focus = ExposureModule::getExposurePulse(0)->isLargeFocus();
    bool detector_synch = false;
    bool grid_synch = ExposureModule::getExposurePulse(0)->useGrid();
    PCB315::filterMaterialCodes filter;
    unsigned char current_status;
    int timeout;
  
    // Checks if the X-RAY push button is pressed
    if (!PCB301::getXrayPushButtonStat()) {
        LogClass::logInFile(ExpName + "push button early released");
        return ExposureModule::exposure_completed_errors::XRAY_BUTTON_RELEASE;
    }

    // Checks for the Tube temperature
    if (PCB315::isTubeAlarm()) {
        LogClass::logInFile(ExpName + "tube temperature monitoring error");
        return ExposureModule::exposure_completed_errors::XRAY_TUBE_TEMPERATURE;
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

    timeout = 50;
    while (timeout) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (timeout) {
            timeout--;
            if (!timeout) {
                LogClass::logInFile(ExpName + "timeout waiting for the grid");
                return ExposureModule::exposure_completed_errors::XRAY_GRID_ERROR;
            }
        }

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

    // Filter selection 
    filter = ExposureModule::getExposurePulse(0)->filter;
    LogClass::logInFile(ExpName + "filter:" + filter.ToString());

    PCB315::setFilterManualMode(filter);
    timeout = 50;
    while (timeout--){
        if (PCB315::isValidFilterSelected()) break;
        if (PCB315::isFilterFault()) {
            return ExposureModule::exposure_completed_errors::XRAY_FILTER_ERROR;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (!timeout) {
        LogClass::logInFile(ExpName + "timeout waiting the filter selection");
        return ExposureModule::exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    // Load Data Bank For pulse    
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pulse, large_focus, ExposureModule::getExposurePulse(0)->kV, ExposureModule::getExposurePulse(0)->mAs, 5000);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Generator_Set_2D_Databank error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Procedure activation    
    R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DProcedurePulse(detector_synch, grid_synch);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Patient_Activate2DProcedurePulse error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Clear the active system messages
    for (int i = 0; i < R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size(); i++) {
        unsigned int id = R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList[i];
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_Clear_Message(id);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "SystemMessages_Clear_Message error");
            return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }
    }

    // Read again the messages
    R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "SystemMessages_Get_AllMessages error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Test if the messages have been removed
    if (R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size()) {
        LogClass::logInFile(ExpName + "system messages error");
        return ExposureModule::exposure_completed_errors::XRAY_GENERATOR_ERROR;
    }

    // Disables the safety disable RX message    
    R2CP::CaDataDicGen::GetInstance()->SystemMessages_SetDisableRx(false);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "SystemMessages_SetDisableRx error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Gets the current generator status
    R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Generator_Get_StatusV6 error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }
    current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    // Status not in standby
    if (current_status != R2CP::Stat_Standby) {
        LogClass::logInFile(ExpName + "generator not in standby error");
        return ExposureModule::exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
    }

   
    // Activate the X-RAY Enable Interface signal on the PCB301 board
    PCB301::set_xray_ena(true);

    // Wait to exit from standby
    timeout = 100;
    while (timeout--) {
        if (!timeout) {
            LogClass::logInFile(ExpName + "timeout waiting generator executing the xray sequence");
            return ExposureModule::exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
        }

        // Gets the current generator status
        R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Generator_Get_StatusV6 error");
            return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
        if (current_status != R2CP::Stat_Standby) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }


    logCurrentStatus(255);

    // Exposure execution
    bool exit_condition = false;    
    ExposureModule::exposure_completed_errors error;
    timeout = 200;
    while (!exit_condition) {

        // No more than10 seconds for the entire sequence
        if (timeout) {
            timeout--;
            if (!timeout) {
                LogClass::logInFile(ExpName + "generator sequence timeout error");
                error = ExposureModule::exposure_completed_errors::XRAY_TIMEOUT;
                exit_condition = true;
                continue;
            }
        }

        // Gets the current generator status
        R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Generator_Get_StatusV6 error");
            error = ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
            exit_condition = true;
            continue;
        }
        current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

        // Checks for the XRAY button
        if (!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus) { 
            LogClass::logInFile(ExpName + "push button release error");
            error = ExposureModule::exposure_completed_errors::XRAY_BUTTON_RELEASE;
            exit_condition = true;
            continue;
        }

        // Checks for error messages
        if (R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active) {            
            LogClass::logInFile(ExpName + "generator system messages during exposure error");
            error = ExposureModule::exposure_completed_errors::XRAY_GENERATOR_ERROR;
            exit_condition = true;
            continue;
        }

        if (logCurrentStatus(current_status)) {
            // Waits for the Exposure complete event
            switch (current_status) {

            case R2CP::Stat_WaitFootRelease: // Exposure terminated
            case R2CP::Stat_Standby:
                LogClass::logInFile(ExpName + "sequence successfully completed");
                error = ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS;
                exit_condition = true;
                break;

            case R2CP::Stat_Error:          
                LogClass::logInFile(ExpName + "generator R2CP::Stat_Error error ");
                error = ExposureModule::exposure_completed_errors::XRAY_GENERATOR_ERROR;
                exit_condition = true;
                break;

            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:   
                LogClass::logInFile(ExpName + current_status.ToString() + " status error ");
                error = ExposureModule::exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
                exit_condition = true;
                break;

            default:
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    // Updates the Exposed pulses  
    unsigned char databank_index = 1;      
    setExposedData(databank_index, (unsigned char) 0, filter, focus);
    return error;

};
