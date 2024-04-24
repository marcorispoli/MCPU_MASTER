#include "ExposureModule.h"
#include "awsProtocol.h"
#include "Generator.h"
#include "R2CP_Eth.h"
#include "CaDataDicGen.h"
#include "PCB304.h"
#include "PCB301.h"
#include "PCB303.h"
#include "TiltMotor.h"
#include <thread>
#include "Log.h"

using namespace System::Diagnostics;
using namespace System::Collections::Generic;



ExposureModule::exposure_completed_errors Generator::man_3d_exposure_procedure(void) {
    System::String^ ExpName = "Exposure 3D Manual>";
    bool large_focus = true;
    bool detector_synch = true;
    int timeout;
    ExposureModule::exposure_completed_errors error;

    System::String^ exposure_data_str = ExpName + " ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "kV:" + ExposureModule::getExposurePulse(0)->kV; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "mAs:" + ExposureModule::getExposurePulse(0)->mAs; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "pulses:" + ExposureModule::getTomoExposure()->tomo_samples; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "skip-pulses:" + ExposureModule::getTomoExposure()->tomo_skip; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "speed:" + ExposureModule::getTomoExposure()->tomo_speed; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "acc:" + ExposureModule::getTomoExposure()->tomo_acc; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "dec:" + ExposureModule::getTomoExposure()->tomo_acc; LogClass::logInFile(exposure_data_str);
    

    // Set the filter selected is the expected into the pulse(0). No wait for positioning here
    if (!PCB315::setFilterAutoMode(ExposureModule::getExposurePulse(0)->filter, false)) {
        return ExposureModule::exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    // Tilt preparation in Home
    if (!ExposureModule::getTomoExposure()->valid) {
        return ExposureModule::exposure_completed_errors::XRAY_INVALID_TOMO_PARAMETERS;
    }

    // 20s waits for ready condition
    timeout = 200;
    while (!TiltMotor::device->isReady()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        timeout--;
        if(!timeout) return ExposureModule::exposure_completed_errors::XRAY_TIMEOUT_TILT_IN_HOME;
    }
    
    // Starts the Tilt positioning in Home. Further in the code the Home position is checked 
    if (!TiltMotor::setTarget(TiltMotor::target_options::TOMO_H, 0)) {
        return ExposureModule::exposure_completed_errors::XRAY_ERROR_TILT_IN_HOME;
    }


    // Sets the Grid On Field (if not yet) : wait for the ready condition   
    //PCB304::synchGridWithGenerator(false);
    //if (!PCB304::setGridOffField(true)) {
    //    return ExposureModule::exposure_completed_errors::XRAY_GRID_ERROR;
    //}

    // ------------------------------------------------------------------------------------ > Dynamic collimation mode
    

    //-------- Setup the 3D pulse using the 3 point approach procedure ---------------------------------------------    
    #define MIN_TOMO_PULSE_TIME 25 // ms
    #define MAX_TOMO_PULSE_TIME 150 // ms

    error = set3PointDatabank(R2CP::DB_Pulse, true, ExposureModule::getExposurePulse(0)->kV, (ExposureModule::getExposurePulse(0)->mAs / ExposureModule::getTomoExposure()->tomo_samples), 5000, 25, 150);
    if (error != ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS) {
        LogClass::logInFile(ExpName + "set3PointDatabank() error");
        return error;
    }

    // Setup the 3D Databank for Tomo skip pulses
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_SkipPulse_Databank(R2CP::DB_SkipPulse, ExposureModule::getTomoExposure()->tomo_skip);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Generator_Set_SkipPulse_Databank error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Setup the 3D Mammography procedure    
    R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Standard_Mammography_3D, ExposureModule::getTomoExposure()->tomo_samples);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Patient_SetupProcedureV6 error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Assignes the Skip-Pulse Databank to the Standard 3D Mammography procedure    
    R2CP::CaDataDicGen::GetInstance()->Generator_Assign_SkipPulse_Databank(R2CP::ProcId_Standard_Mammography_3D, R2CP::DB_SkipPulse);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Generator_Assign_SkipPulse_Databank error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Assignes the Pulse Databank to the Index 2 of the Standard 3D Mammography with AEC procedure    
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Standard_Mammography_3D, 1);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Generator_AssignDbToProc error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Procedure activation
    LogClass::logInFile(ExpName + " procedure activation");
    R2CP::CaDataDicGen::GetInstance()->Patient_Activate3DProcedurePulse();
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Patient_Activate3DProcedurePulse error");
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

    // Waits the Tilt completion
    timeout = 200;
    while (!TiltMotor::device->isReady()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        timeout--;
        if (!timeout) return ExposureModule::exposure_completed_errors::XRAY_TIMEOUT_TILT_IN_HOME;
    }

    // Verify if the Tilt has been successfully completed
    if (TiltMotor::device->getCommandCompletedCode() != TiltMotor::MotorCompletedCodes::COMMAND_SUCCESS) {
        return ExposureModule::exposure_completed_errors::XRAY_ERROR_TILT_IN_HOME;
    }

    // Activate the Tube Tomo Scan
    if (!TiltMotor::activateTomoScan(ExposureModule::getTomoExposure()->tomo_end, ExposureModule::getTomoExposure()->tomo_speed, ExposureModule::getTomoExposure()->tomo_acc, ExposureModule::getTomoExposure()->tomo_dec))  {
        return ExposureModule::exposure_completed_errors::XRAY_ERROR_TILT_IN_HOME;
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

    error = commonExposurePulseSequence(ExpName, false);

    // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!!
    if (large_focus) setExposedData(1, (unsigned char)0, ExposureModule::getExposurePulse(0)->filter, 1);
    else setExposedData(1, (unsigned char)0, ExposureModule::getExposurePulse(0)->filter, 0);

    return error;

};

ExposureModule::exposure_completed_errors Generator::aec_3d_exposure_procedure(void) {
    // Set thecommunication error code as general case
    return ExposureModule::exposure_completed_errors::XRAY_INVALID_PROCEDURE;

};
