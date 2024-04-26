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

ExposureModule::exposure_completed_errors Generator::man_2d_exposure_procedure(void) {
    System::String^ ExpName = "Exposure 2D Manual>";    
    bool large_focus;
    bool detector_synch = true;
    bool grid_synch = true;
    System::String^ detector_param;
    int exposure_time;

    try {
        detector_param = ExposureModule::getDetectorType().ToString();
        exposure_time = System::Convert::ToInt16(DetectorConfig::Configuration->getParam(detector_param)[DetectorConfig::PARAM_MAX_2D_INTEGRATION_TIME]);
    }
    catch (...) {
        LogClass::logInFile(ExpName + "Invalid Detector Parameter ");
        return ExposureModule::exposure_completed_errors::XRAY_INVALID_2D_PARAMETERS;
    }

    // Set the filter selected is the expected into the pulse(0). No wait for positioning here
    if (!PCB315::setFilterAutoMode(ExposureModule::getExposurePulse(0)->filter, false)) {
        return ExposureModule::exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    // Sets the Grid On Field (if not yet) : wait for the ready condition   
    PCB304::synchGridWithGenerator(true);
    if (!PCB304::setGridOnField(true)) {
        return ExposureModule::exposure_completed_errors::XRAY_GRID_ERROR;
    }

    // Determines if the Focus is Small or large based on the presence of the Magnifier device
    if (PCB304::isMagnifierDeviceDetected()) large_focus = false;
    else large_focus = true;

    
    // The format collimation should be already configured
    if (!PCB303::isValidCollimationFormat()) {
        return ExposureModule::exposure_completed_errors::XRAY_COLLI_FORMAT_ERROR;
    }
    
   

    // Load Data Bank For pulse        
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pulse, large_focus, ExposureModule::getExposurePulse(0)->kV, ExposureModule::getExposurePulse(0)->mAs, exposure_time);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Generator_Set_2D_Databank error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }


    // Gets the Anodic current selected
    float pulse_mA = ((float)R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pulse].mA100.value) / 100;

    System::String^ exposure_data_str = ExpName + " ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR TYPE: " + detector_param; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR MAX INTEGRATION TIME: " + exposure_time; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "kV:" + ExposureModule::getExposurePulse(0)->kV; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "mAs:" + ExposureModule::getExposurePulse(0)->mAs; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Anodic-mA:" + pulse_mA; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter:" + ExposureModule::getExposurePulse(0)->filter.ToString(); LogClass::logInFile(exposure_data_str);
    if (large_focus) { exposure_data_str = "Focus: LARGE";  LogClass::logInFile(exposure_data_str); }
    else { exposure_data_str = "Focus: SMALL";  LogClass::logInFile(exposure_data_str); }

    // Procedure activation
    LogClass::logInFile("GENERATOR MAN 2D EXPOSURE: procedure activation");
    R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DProcedurePulse(detector_synch, grid_synch);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Patient_Activate2DProcedurePulse error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Clear the active system messages
    if (!clearSystemMessages()) {
        LogClass::logInFile(ExpName + "SystemMessages_Clear_Message error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
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
    unsigned char current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    // Status not in standby
    if (current_status != R2CP::Stat_Standby) {
        LogClass::logInFile(ExpName + "generator not in standby error");
        return ExposureModule::exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
    }

    // Checks the filter in position: the filter has been selected early in the generator procedure    
    if (!PCB315::waitForValidFilter()) {
        return ExposureModule::exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    // Activate the X-RAY Enable Interface signal on the PCB301 board
    PCB301::set_xray_ena(true);

    ExposureModule::exposure_completed_errors  error = commonExposurePulseSequence(ExpName, false);
    
    // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!!
    if(large_focus) setExposedData(1, (unsigned char)0, ExposureModule::getExposurePulse(0)->filter, 1);
    else setExposedData(1, (unsigned char)0, ExposureModule::getExposurePulse(0)->filter, 0);
    
    return error;
};

ExposureModule::exposure_completed_errors Generator::aec_2d_exposure_procedure(void) {
    System::String^ ExpName = "Exposure 2D + AEC>";
    bool large_focus;
    bool detector_synch = true;
    bool grid_synch = true;

    System::String^ detector_param;
    int exposure_time;

    try {
        detector_param = ExposureModule::getDetectorType().ToString();
        exposure_time = System::Convert::ToInt16(DetectorConfig::Configuration->getParam(detector_param)[DetectorConfig::PARAM_MAX_2D_PRE_INTEGRATION_TIME]);
    }
    catch (...) {
        LogClass::logInFile(ExpName + "Invalid Detector Parameter ");
        return ExposureModule::exposure_completed_errors::XRAY_INVALID_2D_PARAMETERS;
    }

    // Set the filter selected is the expected into the pulse(0). No wait for positioning here
    if (!PCB315::setFilterAutoMode(ExposureModule::getExposurePulse(0)->filter, false)) {
        return ExposureModule::exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    // Sets the Grid On Field (if not yet) : wait for the ready condition   
    PCB304::synchGridWithGenerator(true);
    if (!PCB304::setGridOnField(true)) {
        return ExposureModule::exposure_completed_errors::XRAY_GRID_ERROR;
    }

    // Determines if the Focus is Small or large based on the presence of the Magnifier device
    if (PCB304::isMagnifierDeviceDetected()) large_focus = false;
    else large_focus = true;


    // Verifies that a valid the format collimation is present
    if (!PCB303::isValidCollimationFormat()) {
        return ExposureModule::exposure_completed_errors::XRAY_COLLI_FORMAT_ERROR;
    }


    // Load Data Bank For pre-pulse    
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pre, large_focus, ExposureModule::getExposurePulse(0)->kV, ExposureModule::getExposurePulse(0)->mAs, exposure_time);
    ExposureModule::getExposurePulse(0)->validated = false;
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Generator_Set_2D_Databank error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Gets the Anodic current selected
    float pulse_mA = ((float)R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pre].mA100.value) / 100;
    System::String^ exposure_data_str = ExpName + " PRE-PULSE DATA ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR TYPE: " + detector_param; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR MAX AEC INTEGRATION TIME: " + exposure_time; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "kV Pre:" + ExposureModule::getExposurePulse(0)->kV; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "mAs Pre:" + ExposureModule::getExposurePulse(0)->mAs; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Anodic-mA:" + pulse_mA; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter Pre:" + ExposureModule::getExposurePulse(0)->filter.ToString(); LogClass::logInFile(exposure_data_str);
    if (large_focus) { exposure_data_str = "Focus: LARGE";  LogClass::logInFile(exposure_data_str); }
    else { exposure_data_str = "Focus: SMALL";  LogClass::logInFile(exposure_data_str); }

    // Procedure activation
    LogClass::logInFile(ExpName + "procedure activation");
    R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DAecProcedurePre();
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Patient_Activate2DAecProcedurePre error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Clear the active system messages
    if (!clearSystemMessages()) {
        LogClass::logInFile(ExpName + "SystemMessages_Clear_Message error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
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
    unsigned char current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    // Status not in standby
    if (current_status != R2CP::Stat_Standby) {
        LogClass::logInFile(ExpName + "generator not in standby error");
        return ExposureModule::exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
    }

    // Checks the filter in position: the filter has been selected early in the generator procedure    
    if (!PCB315::waitForValidFilter()) {
        return ExposureModule::exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    // Activate the X-RAY Enable Interface signal on the PCB301 board
    PCB301::set_xray_ena(true);

    // Sequence for the AEC
    ExposureModule::exposure_completed_errors  error = commonExposurePulseSequence(ExpName, true);
       
    // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!!
    if (large_focus) setExposedData(1, (unsigned char)0, ExposureModule::getExposurePulse(0)->filter, 1);
    else setExposedData(1, (unsigned char)0, ExposureModule::getExposurePulse(0)->filter, 0);

    // If the sequence should return an error condition termines here
    if (error != ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS) {
        return error;
    }

    // Sends the pulse-completed event to the AWS
    awsProtocol::EVENT_exposurePulseCompleted(0);

    // Waits for the Validated Pulse from the acquisition software
    int timeout = 100; // 10 seconds timeout
    while (timeout--) {
        if (ExposureModule::getExposurePulse(1)->isValid()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (!timeout) {
        return ExposureModule::exposure_completed_errors::XRAY_TIMEOUT_AEC;
    }
    
    // Set/verify the filter selected is the expected into the pulse(1)
    if (!PCB315::setFilterAutoMode(ExposureModule::getExposurePulse(1)->filter, true)) {
        return ExposureModule::exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    try {        
        exposure_time = System::Convert::ToInt16(DetectorConfig::Configuration->getParam(detector_param)[DetectorConfig::PARAM_MAX_2D_INTEGRATION_TIME]);
    }
    catch (...) {
        LogClass::logInFile(ExpName + "Invalid Detector Parameter ");
        return ExposureModule::exposure_completed_errors::XRAY_INVALID_2D_PARAMETERS;
    }

    // Load Data Bank For pulse       
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pulse, large_focus, ExposureModule::getExposurePulse(1)->kV, ExposureModule::getExposurePulse(1)->mAs, exposure_time);
    ExposureModule::getExposurePulse(1)->validated = false;
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Generator_Set_2D_Databank(pulse) error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }
    
    pulse_mA = ((float)R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pulse].mA100.value) / 100;
    exposure_data_str = ExpName + " PULSE DATA ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR TYPE: " + detector_param; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR MAX PULSE INTEGRATION TIME: " + exposure_time; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "kV:" + ExposureModule::getExposurePulse(1)->kV; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "mAs:" + ExposureModule::getExposurePulse(1)->mAs; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Anodic-mA:" + pulse_mA; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter:" + ExposureModule::getExposurePulse(1)->filter.ToString(); LogClass::logInFile(exposure_data_str);
    if (large_focus) { exposure_data_str = "Focus: LARGE";  LogClass::logInFile(exposure_data_str); }
    else { exposure_data_str = "Focus: SMALL";  LogClass::logInFile(exposure_data_str); }

    // Procedure activation
    LogClass::logInFile(ExpName + "procedure activation");
    R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DAecProcedurePulse();
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Patient_Activate2DAecProcedurePulse error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }
    
    error = commonExposurePulseSequence(ExpName, false);

   
    // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!! 
    if (large_focus) setExposedData(2, (unsigned char)0, ExposureModule::getExposurePulse(1)->filter, 1);
    else setExposedData(2, (unsigned char)0, ExposureModule::getExposurePulse(1)->filter, 0);

    return error;
    
};
