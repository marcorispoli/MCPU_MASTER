#include "ExposureModule.h"
#include "awsProtocol.h"
#include "Generator.h"
#include "R2CP_Eth.h"
#include "CaDataDicGen.h"
#include "PCB304.h"
#include "PCB301.h"
#include "PCB303.h"
#include <thread>
#include "Log.h"

using namespace System::Diagnostics;
using namespace System::Collections::Generic;

Exposures::exposure_completed_errors Exposures::man_2d_exposure_procedure(bool demo) {
    System::String^ ExpName;    
    bool large_focus;
    bool detector_synch = true;
    bool grid_synch = true;
    System::String^ detector_param;
    int exposure_time;

    if (demo) ExpName = "Demo Exposure 2D Manual>";
    else ExpName = "Exposure 2D Manual>";

    try {
        detector_param = getDetectorType().ToString();
        exposure_time = System::Convert::ToInt16(DetectorConfig::Configuration->getParam(detector_param)[DetectorConfig::PARAM_MAX_2D_INTEGRATION_TIME]);
    }
    catch (...) {
        LogClass::logInFile(ExpName + "Invalid Detector Parameter ");
        return exposure_completed_errors::XRAY_INVALID_2D_PARAMETERS;
    }

    // Set the filter selected is the expected into the pulse(0). No wait for positioning here
    if (!PCB315::setFilterAutoMode(getExposurePulse(0)->filter, false)) {
        return exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    // Sets the Grid On Field (if not yet) : wait for the ready condition   
    PCB304::synchGridWithGenerator(true);
    if (!PCB304::setGridOnField(true)) {
        return exposure_completed_errors::XRAY_GRID_ERROR;
    }

    // Determines if the Focus is Small or large based on the presence of the Magnifier device
    if (PCB304::isMagnifierDeviceDetected()) large_focus = false;
    else large_focus = true;

    
    // The format collimation should be already configured
    if (!PCB303::isValidCollimationFormat()) {
        return exposure_completed_errors::XRAY_COLLI_FORMAT_ERROR;
    }
    
    // Load Data Bank For pulse    
    float pulse_mA;

    if (!demo) {
        R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pulse, large_focus, getExposurePulse(0)->kV, getExposurePulse(0)->mAs, exposure_time);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Generator_Set_2D_Databank error");
            return exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        // Gets the Anodic current selected
        pulse_mA = ((float)R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pulse].mA100.value) / 100;

    }
    else pulse_mA = 150;
    
   
    System::String^ exposure_data_str = ExpName + " ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR TYPE: " + detector_param; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR MAX INTEGRATION TIME: " + exposure_time; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "kV:" + Exposures::getExposurePulse(0)->kV; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "mAs:" + Exposures::getExposurePulse(0)->mAs; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Anodic-mA:" + pulse_mA; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter:" + Exposures::getExposurePulse(0)->filter.ToString(); LogClass::logInFile(exposure_data_str);
    if (large_focus) { exposure_data_str = "Focus: LARGE";  LogClass::logInFile(exposure_data_str); }
    else { exposure_data_str = "Focus: SMALL";  LogClass::logInFile(exposure_data_str); }

    
    if (!demo) {
        
        // Procedure activation
        LogClass::logInFile("GENERATOR MAN 2D EXPOSURE: procedure activation");
        R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DProcedurePulse(detector_synch, grid_synch);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Patient_Activate2DProcedurePulse error");
            return exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        // Clear the active system messages
        if (!clearSystemMessages()) {
            LogClass::logInFile(ExpName + "SystemMessages_Clear_Message error");
            return exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        // Disables the safety disable RX message    
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_SetDisableRx(false);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "SystemMessages_SetDisableRx error");
            return exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }
        
        // Status not in StandBy
        updateGeneratorStatus();
        if (getGeneratorStatus() != R2CP::Stat_Standby) {
            LogClass::logInFile(ExpName + "generator not in standby error");
            return exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
        }

    }

    // Checks the filter in position: the filter has been selected early in the generator procedure    
    if (!PCB315::waitForValidFilter()) {
        return Exposures::exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    // Activate the X-RAY Enable Interface signal on the PCB301 board
    PCB301::set_xray_ena(true);

    exposure_completed_errors  error;
    if (!demo) {
        error = (exposure_completed_errors)generatorPulseSequence(ExpName, 15000);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!!
        if (large_focus) setExposedData(1, (unsigned char)0, getExposurePulse(0)->filter, 1);
        else setExposedData(1, (unsigned char)0, getExposurePulse(0)->filter, 0);

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

Exposures::exposure_completed_errors Exposures::aec_2d_exposure_procedure(bool demo) {
    System::String^ ExpName;
    bool large_focus;
    bool detector_synch = true;
    bool grid_synch = true;

    System::String^ detector_param;
    int exposure_time;

    if (demo) ExpName = "Demo Exposure 2D + AEC>";
    else ExpName = "Exposure 2D + AEC>";

    try {
        detector_param = Exposures::getDetectorType().ToString();
        exposure_time = System::Convert::ToInt16(DetectorConfig::Configuration->getParam(detector_param)[DetectorConfig::PARAM_MAX_2D_PRE_INTEGRATION_TIME]);
    }
    catch (...) {
        LogClass::logInFile(ExpName + "Invalid Detector Parameter ");
        return exposure_completed_errors::XRAY_INVALID_2D_PARAMETERS;
    }

    // Set the filter selected is the expected into the pulse(0). No wait for positioning here
    if (!PCB315::setFilterAutoMode(Exposures::getExposurePulse(0)->filter, false)) {
        return Exposures::exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    // Sets the Grid On Field (if not yet) : wait for the ready condition   
    PCB304::synchGridWithGenerator(true);
    if (!PCB304::setGridOnField(true)) {
        return Exposures::exposure_completed_errors::XRAY_GRID_ERROR;
    }

    // Determines if the Focus is Small or large based on the presence of the Magnifier device
    if (PCB304::isMagnifierDeviceDetected()) large_focus = false;
    else large_focus = true;


    // Verifies that a valid the format collimation is present
    if (!PCB303::isValidCollimationFormat()) {
        return Exposures::exposure_completed_errors::XRAY_COLLI_FORMAT_ERROR;
    }


    // Load Data Bank For pre-pulse 
    float pulse_mA;
    if (!demo) {
        R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pre, large_focus, Exposures::getExposurePulse(0)->kV, Exposures::getExposurePulse(0)->mAs, exposure_time);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Generator_Set_2D_Databank error");
            return Exposures::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        // Gets the Anodic current selected
        pulse_mA = ((float)R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pre].mA100.value) / 100;
    }
    else pulse_mA = 150;
    
    
    System::String^ exposure_data_str = ExpName + " PRE-PULSE DATA ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR TYPE: " + detector_param; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR MAX AEC INTEGRATION TIME: " + exposure_time; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "kV Pre:" + Exposures::getExposurePulse(0)->kV; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "mAs Pre:" + Exposures::getExposurePulse(0)->mAs; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Anodic-mA:" + pulse_mA; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter Pre:" + Exposures::getExposurePulse(0)->filter.ToString(); LogClass::logInFile(exposure_data_str);
    if (large_focus) { exposure_data_str = "Focus: LARGE";  LogClass::logInFile(exposure_data_str); }
    else { exposure_data_str = "Focus: SMALL";  LogClass::logInFile(exposure_data_str); }

    if (!demo) {
        // Procedure activation
        LogClass::logInFile(ExpName + "procedure activation");
        R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DAecProcedurePre();
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Patient_Activate2DAecProcedurePre error");
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
    

    // Checks the filter in position: the filter has been selected early in the generator procedure    
    if (!PCB315::waitForValidFilter()) {
        return Exposures::exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    // Activate the X-RAY Enable Interface signal on the PCB301 board
    PCB301::set_xray_ena(true);

    exposure_completed_errors  error;
    if (!demo) {
        // Sequence for the AEC: only the Standby is admitted as returned code: the WaitFootRelease is not admitted here
        error = (exposure_completed_errors) generatorPulseSequence(ExpName, 15000);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!!
        if (large_focus) setExposedData(1, (unsigned char)0, getExposurePulse(0)->filter, 1);
        else setExposedData(1, (unsigned char)0, getExposurePulse(0)->filter, 0);
        if (getGeneratorStatus() != R2CP::Stat_Standby) error = exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
    }
    else {
        // Demo pre-pulse implementation

        // Activate the Buzzer in manual mode
        PCB301::set_manual_buzzer(true);

        // Simulation of the preparation:
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // Xray push button test
        if (!PCB301::getXrayPushButtonStat()) {
            return exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }

        // Generation of the single pulse of 300ms
        PCB301::set_activation_buzzer(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        PCB301::set_activation_buzzer(false);

        exposure_pulse^ epulse = Exposures::getExposurePulse(0);
        setExposedPulse(0, gcnew exposure_pulse(epulse->getKv(), epulse->getmAs(), epulse->getFilter()));
        error = exposure_completed_errors::XRAY_NO_ERRORS;
    }
    
    
    // If the sequence should return an error condition termines here
    if (error != exposure_completed_errors::XRAY_NO_ERRORS) {
        return error;
    }

    // Sends the pulse-completed event to the AWS
    awsProtocol::EVENT_exposurePulseCompleted(0);

    // Waits for the Validated Pulse from the acquisition software
    int timeout = 100; // 10 seconds timeout
    while (timeout--) {
        if (getExposurePulse(1)->isValid()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (!timeout) {
        return exposure_completed_errors::XRAY_TIMEOUT_AEC;
    }
    getExposurePulse(1)->validated = false;

    // Set/verify the filter selected is the expected into the pulse(1)
    if (!PCB315::setFilterAutoMode(getExposurePulse(1)->filter, true)) {
        return exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    try {        
        exposure_time = System::Convert::ToInt16(DetectorConfig::Configuration->getParam(detector_param)[DetectorConfig::PARAM_MAX_2D_INTEGRATION_TIME]);
    }
    catch (...) {
        LogClass::logInFile(ExpName + "Invalid Detector Parameter ");
        return exposure_completed_errors::XRAY_INVALID_2D_PARAMETERS;
    }

    // Load Data Bank For pulse       
    if (!demo) {
        R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pulse, large_focus, getExposurePulse(1)->kV, getExposurePulse(1)->mAs, exposure_time);
        getExposurePulse(1)->validated = false;
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Generator_Set_2D_Databank(pulse) error");
            return exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        pulse_mA = ((float)R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pulse].mA100.value) / 100;
    }
    else pulse_mA = 150;
    
    exposure_data_str = ExpName + " PULSE DATA ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR TYPE: " + detector_param; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR MAX PULSE INTEGRATION TIME: " + exposure_time; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "kV:" + Exposures::getExposurePulse(1)->kV; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "mAs:" + Exposures::getExposurePulse(1)->mAs; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Anodic-mA:" + pulse_mA; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter:" + Exposures::getExposurePulse(1)->filter.ToString(); LogClass::logInFile(exposure_data_str);
    if (large_focus) { exposure_data_str = "Focus: LARGE";  LogClass::logInFile(exposure_data_str); }
    else { exposure_data_str = "Focus: SMALL";  LogClass::logInFile(exposure_data_str); }

    if (!demo) {
        // Procedure activation
        LogClass::logInFile(ExpName + "procedure activation");
        R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DAecProcedurePulse();
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Patient_Activate2DAecProcedurePulse error");
            return exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        error = (exposure_completed_errors) generatorPulseSequence(ExpName, 15000);
        
        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!! 
        if (large_focus) setExposedData(2, (unsigned char)0, getExposurePulse(1)->filter, 1);
        else setExposedData(2, (unsigned char)0, getExposurePulse(1)->filter, 0);
    }
    else {
        // Demo pulse implementation

        // Activate the Buzzer in manual mode
        PCB301::set_manual_buzzer(true);

        // Simulation of the preparation:
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // Xray push button test
        if (!PCB301::getXrayPushButtonStat()) {
            return exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }

        // Generation of the single pulse of 300ms
        PCB301::set_activation_buzzer(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        PCB301::set_activation_buzzer(false);

        exposure_pulse^ epulse = Exposures::getExposurePulse(1);
        setExposedPulse(0, gcnew exposure_pulse(epulse->getKv(), epulse->getmAs(), epulse->getFilter()));
        error = exposure_completed_errors::XRAY_NO_ERRORS;
    }
    
    return error;
    
};
