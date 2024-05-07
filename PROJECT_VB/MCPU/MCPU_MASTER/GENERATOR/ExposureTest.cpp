#include "ExposureModule.h"
#include "Generator.h"
#include "R2CP_Eth.h"
#include "CaDataDicGen.h"
#include "PCB304.h"
#include "PCB301.h"
#include <thread>
#include "Log.h"


using namespace System::Collections::Generic;

Exposures::exposure_completed_errors Exposures::test_exposure_procedure(bool demo) {
    System::String^ ExpName ;
    unsigned char focus = Exposures::getExposurePulse(0)->getFocus();
    bool large_focus = Exposures::getExposurePulse(0)->isLargeFocus();
    bool detector_synch = Exposures::getExposurePulse(0)->useDetector();
    bool grid_synch = Exposures::getExposurePulse(0)->useGrid();
    PCB315::filterMaterialCodes filter;
    unsigned char current_status;
    int timeout;
  
    if (demo) ExpName = "Demo Exposure Test >";
    else ExpName = "Exposure Test >";

    // Sets the Grid On/Off Field and wait for the ready condition
    if (grid_synch) {        
        PCB304::synchGridWithGenerator(true);
        if (!PCB304::setGridOnField(true)) {
            return exposure_completed_errors::XRAY_GRID_ERROR;
        }       
    }
    else {        
        //PCB304::synchGridWithGenerator(false);
        //if (!PCB304::setGridOffField(true)) {
        //    return ExposureModule::exposure_completed_errors::XRAY_GRID_ERROR;
        //}        
    }

    // Filter selection 
    filter = getExposurePulse(0)->filter;
    
    if (!PCB315::setFilterAutoMode(filter, true)) {
        return exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    float pulse_mA;

    if (!demo) {
        // Load Data Bank For pulse    
        R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pulse, large_focus, Exposures::getExposurePulse(0)->kV, Exposures::getExposurePulse(0)->mAs, 5000);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Generator_Set_2D_Databank error");
            return Exposures::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        pulse_mA = ((float)R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pulse].mA100.value) / 100;
    }
    else pulse_mA = 150;
    
    System::String^ exposure_data_str = ExpName + " PULSE DATA ---------------- "; LogClass::logInFile(exposure_data_str);   
    exposure_data_str = "kV:" + Exposures::getExposurePulse(0)->kV; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "mAs:" + Exposures::getExposurePulse(0)->mAs; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Anodic-mA:" + pulse_mA; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter:" + Exposures::getExposurePulse(0)->filter.ToString(); LogClass::logInFile(exposure_data_str);
    if (large_focus) { exposure_data_str = "Focus: LARGE";  LogClass::logInFile(exposure_data_str); }
    else { exposure_data_str = "Focus: SMALL";  LogClass::logInFile(exposure_data_str); }

    if (!demo) {
        if (detector_synch) grid_synch = true; // Procedure activation 
        R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DProcedurePulse(detector_synch, grid_synch);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Patient_Activate2DProcedurePulse error");
            return Exposures::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        // Clear the active system messages
        if (!clearSystemMessages()) {
            LogClass::logInFile(ExpName + "SystemMessages_Clear_Message error");
            return Exposures::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }


        // Disables the safety disable RX message    
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_SetDisableRx(false);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "SystemMessages_SetDisableRx error");
            return Exposures::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        // Status not in StandBy
        updateGeneratorStatus();
        if (getGeneratorStatus() != R2CP::Stat_Standby) {
            LogClass::logInFile(ExpName + "generator not in standby error");
            return exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
        }

    }
    
    // Activate the X-RAY Enable Interface signal on the PCB301 board
    PCB301::set_xray_ena(true);

    
    exposure_completed_errors  error;
    if (!demo) {
        error = (exposure_completed_errors)generatorPulseSequence(ExpName, 15000);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!! 
        setExposedData(1, (unsigned char)0, filter, focus);
    }
    else {
        // Demo pulse implementation

        // Activate the Buzzer in manual mode
        PCB301::set_manual_buzzer(true);

        // Simulation of the preparation:
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Xray push button test
        if (!PCB301::getXrayPushButtonStat()) {
            return exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }

        // Generation of the single pulse of 1 second       
        PCB301::set_activation_buzzer(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        PCB301::set_activation_buzzer(false);

        exposure_pulse^ epulse = Exposures::getExposurePulse(0);
        setExposedPulse(0, gcnew exposure_pulse(epulse->getKv(), epulse->getmAs(), epulse->getFilter()));

        error = exposure_completed_errors::XRAY_NO_ERRORS;
    }
    
    return error;

};
