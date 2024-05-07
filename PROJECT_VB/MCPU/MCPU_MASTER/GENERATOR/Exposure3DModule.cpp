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


using namespace System::Collections::Generic;


Exposures::exposure_completed_errors Exposures::man_3d_exposure_procedure(bool demo) {
    System::String^ ExpName ;
    bool large_focus = true;
    bool detector_synch = true;
    int timeout;
    exposure_completed_errors error;

    if (demo) ExpName = "Demo Exposure 3D Manual>";
    else ExpName = "Exposure 3D Manual>";

    // Gets the Detector used in the exposure sequence:
    // The Detector determines the maximum integration time allowd and 
    // the maximum FPS the detector can support.
    System::String^ detector_param = Exposures::getDetectorType().ToString();
    int max_fps;
    int exposure_time;

    try
    {
        max_fps = System::Convert::ToInt16(DetectorConfig::Configuration->getParam(detector_param)[DetectorConfig::PARAM_MAX_3D_FPS]);

        // Tomo not enabled for this detector
        if (max_fps < 1) {
            LogClass::logInFile(ExpName + "Invalid Tomo activation for this detector");
            return exposure_completed_errors::XRAY_INVALID_TOMO_PARAMETERS;
        }

        if (max_fps > DetectorConfig::MAX_TOMO_FPS) {
            LogClass::logInFile(ExpName + "Invalid Tomo Configuration file");
            return exposure_completed_errors::XRAY_INVALID_TOMO_PARAMETERS;
        }

        if (Exposures::getTomoExposure()->tomo_fps > max_fps) {
            LogClass::logInFile(ExpName + "Invalid Tomo FPS");
            return exposure_completed_errors::XRAY_INVALID_TOMO_PARAMETERS;
        }

        exposure_time = System::Convert::ToInt16(DetectorConfig::Configuration->getParam(detector_param)[DetectorConfig::PARAM_MAX_3D_INTEGRATION_TIME_1FPS + (Exposures::getTomoExposure()->tomo_fps - 1)]);
    }
    catch (...) {
        LogClass::logInFile(ExpName + "Invalid Detector Parameter ");
        return exposure_completed_errors::XRAY_INVALID_TOMO_PARAMETERS;
    }

   

    // Set the filter selected is the expected into the pulse(0). No wait for positioning here
    if (!PCB315::setFilterAutoMode(getExposurePulse(0)->filter, false)) {
        return exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    // Tilt preparation in Home
    if (!getTomoExposure()->valid) {
        return exposure_completed_errors::XRAY_INVALID_TOMO_PARAMETERS;
    }

    // 20s waits for ready condition
    timeout = 200;
    while (!TiltMotor::device->isReady()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        timeout--;
        if(!timeout) return exposure_completed_errors::XRAY_TIMEOUT_TILT_IN_HOME;
    }
    
    // Starts the Tilt positioning in Home. Further in the code the Home position is checked 
    if (!TiltMotor::setTarget(TiltMotor::target_options::TOMO_H, 0)) {
        return exposure_completed_errors::XRAY_ERROR_TILT_IN_HOME;
    }


    // Sets the Grid On Field (if not yet) : wait for the ready condition   
    //PCB304::synchGridWithGenerator(false);
    //if (!PCB304::setGridOffField(true)) {
    //    return ExposureModule::exposure_completed_errors::XRAY_GRID_ERROR;
    //}

    // ------------------------------------------------------------------------------------ > Dynamic collimation mode
    float pulse_mA;
    if (!demo) {

        //-------- Setup the 3D pulse using the 3 point approach procedure ---------------------------------------------           
        error = (exposure_completed_errors)generatorSet3PointDatabank(R2CP::DB_Pulse, true, Exposures::getExposurePulse(0)->kV, (Exposures::getExposurePulse(0)->mAs / Exposures::getTomoExposure()->tomo_samples), Exposures::getTomoExposure()->tomo_samples, 25, exposure_time);
        if (error != exposure_completed_errors::XRAY_NO_ERRORS) {
            LogClass::logInFile(ExpName + "set3PointDatabank() error");
            return error;
        }

        // Gets the Anodic current selected
        pulse_mA = ((float)R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pulse].mA100.value) / 100;
    }
    else pulse_mA = 200;
    

    System::String^ exposure_data_str = ExpName + " ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR TYPE: " + detector_param; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR MAX 3D INTEGRATION TIME: " + exposure_time; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "FPS:" + Exposures::getTomoExposure()->tomo_fps; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "kV:" + Exposures::getExposurePulse(0)->kV; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "mAs:" + Exposures::getExposurePulse(0)->mAs; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Anodic-mA:" + pulse_mA; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter:" + Exposures::getExposurePulse(0)->filter.ToString(); LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Samples:" + Exposures::getTomoExposure()->tomo_samples; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Skips:" + Exposures::getTomoExposure()->tomo_skip; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Position Home:" + Exposures::getTomoExposure()->tomo_home; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Position End:" + Exposures::getTomoExposure()->tomo_end; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Speed:" + Exposures::getTomoExposure()->tomo_speed; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Ramp-Acc:" + Exposures::getTomoExposure()->tomo_acc; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Ramp-Dec:" + Exposures::getTomoExposure()->tomo_acc; LogClass::logInFile(exposure_data_str);

    if (!demo) {
        // Setup the 3D Databank for Tomo skip pulses
        R2CP::CaDataDicGen::GetInstance()->Generator_Set_SkipPulse_Databank(R2CP::DB_SkipPulse, getTomoExposure()->tomo_skip);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Generator_Set_SkipPulse_Databank error");
            return Exposures::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        // Setup the 3D Mammography procedure    
        R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Standard_Mammography_3D, getTomoExposure()->tomo_samples);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Patient_SetupProcedureV6 error");
            return Exposures::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        // Assignes the Skip-Pulse Databank to the Standard 3D Mammography procedure    
        R2CP::CaDataDicGen::GetInstance()->Generator_Assign_SkipPulse_Databank(R2CP::ProcId_Standard_Mammography_3D, R2CP::DB_SkipPulse);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Generator_Assign_SkipPulse_Databank error");
            return Exposures::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        // Assignes the Pulse Databank to the Index 2 of the Standard 3D Mammography with AEC procedure    
        R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Standard_Mammography_3D, 1);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Generator_AssignDbToProc error");
            return Exposures::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }

        // Procedure activation
        LogClass::logInFile(ExpName + " procedure activation");
        R2CP::CaDataDicGen::GetInstance()->Patient_Activate3DProcedurePulse();
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(ExpName + "Patient_Activate3DProcedurePulse error");
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
    }
   

    // Waits the Tilt completion
    timeout = 200;
    while (!TiltMotor::device->isReady()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        timeout--;
        if (!timeout) return Exposures::exposure_completed_errors::XRAY_TIMEOUT_TILT_IN_HOME;
    }

    // Verify if the Tilt has been successfully completed
    if (TiltMotor::device->getCommandCompletedCode() != TiltMotor::MotorCompletedCodes::COMMAND_SUCCESS) {
        return Exposures::exposure_completed_errors::XRAY_ERROR_TILT_IN_HOME;
    }

    if (!demo) {

        // Activate the Tube Tomo Scan
        if (!TiltMotor::activateTomoScan(Exposures::getTomoExposure()->tomo_end, getTomoExposure()->tomo_speed, getTomoExposure()->tomo_acc, getTomoExposure()->tomo_dec)) {
            return Exposures::exposure_completed_errors::XRAY_ERROR_TILT_IN_HOME;
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
        return exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    // Activate the X-RAY Enable Interface signal on the PCB301 board
    PCB301::set_xray_ena(true);

    if (!demo) {
        error = (exposure_completed_errors)generatorPulseSequence(ExpName, 15000);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!!
        if (large_focus) setExposedData(1, (unsigned char)0, getExposurePulse(0)->filter, 1);
        else setExposedData(1, (unsigned char)0, getExposurePulse(0)->filter, 0);

    }
    else {
        // Simulation of the preparation:
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Starts the Tomo scan 
        if (!TiltMotor::setTarget(TiltMotor::target_options::TOMO_E, 0)) return exposure_completed_errors::XRAY_POSITIONING_ERROR;

        int deltat = 1000 / getTomoExposure()->tomo_fps;
        if (deltat > 200) deltat -= 200;
        else deltat = 100;

        for (int i = 0; i < getTomoExposure()->tomo_samples; i++) {

            // Xray push button test
            if (!PCB301::getXrayPushButtonStat())  return exposure_completed_errors::XRAY_BUTTON_RELEASE;

            PCB301::set_activation_buzzer(true);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            PCB301::set_activation_buzzer(false);
            std::this_thread::sleep_for(std::chrono::milliseconds(deltat));
        }

        exposure_pulse^ epulse = Exposures::getExposurePulse(0);
        setExposedPulse(0, gcnew exposure_pulse(epulse->getKv(), epulse->getmAs(), epulse->getFilter()));
        error = exposure_completed_errors::XRAY_NO_ERRORS;
    }
    
    return error;

};

Exposures::exposure_completed_errors Exposures::aec_3d_exposure_procedure(bool demo) {
    // Set thecommunication error code as general case
    return exposure_completed_errors::XRAY_NO_ERRORS;

};
