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

    PCB304::syncGeneratorOff();

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
    
    
    // 3D Pulse Preparation
    System::String^ exposure_data_str = ExpName + " ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR TYPE: " + detector_param; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR MAX 3D INTEGRATION TIME: " + exposure_time; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "FPS:" + Exposures::getTomoExposure()->tomo_fps; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter:" + Exposures::getExposurePulse(0)->filter.ToString(); LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Samples:" + Exposures::getTomoExposure()->tomo_samples; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Skips:" + Exposures::getTomoExposure()->tomo_skip; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Position Home:" + Exposures::getTomoExposure()->tomo_home; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Position End:" + Exposures::getTomoExposure()->tomo_end; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Speed:" + Exposures::getTomoExposure()->tomo_speed; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Ramp-Acc:" + Exposures::getTomoExposure()->tomo_acc; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Ramp-Dec:" + Exposures::getTomoExposure()->tomo_acc; LogClass::logInFile(exposure_data_str);
    error = (exposure_completed_errors)generator3DPulsePreparation(ExpName, Exposures::getExposurePulse(0)->kV, Exposures::getExposurePulse(0)->mAs, Exposures::getTomoExposure()->tomo_samples, Exposures::getTomoExposure()->tomo_skip, large_focus, 25, exposure_time);
    if (error != Exposures::exposure_completed_errors::XRAY_NO_ERRORS) return error;



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
       
    }
   
    // Checks the filter in position: the filter has been selected early in the generator procedure    
    if (!PCB315::waitForValidFilter()) {
        return exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    if (!demo) {

        // Longer Timeout: the generator checks for the correct seqeunce here
        error = (exposure_completed_errors)generatorExecutePulseSequence(ExpName, 40000);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!!
        if (large_focus) setExposedData(1, (unsigned char)0, getExposurePulse(0)->filter, 1);
        else setExposedData(1, (unsigned char)0, getExposurePulse(0)->filter, 0);

    }
    else {
        // Simulation of the preparation:
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Starts the Tomo scan 
        if (!TiltMotor::setTarget(TiltMotor::target_options::TOMO_E, 0)) return exposure_completed_errors::XRAY_POSITIONING_ERROR;

        // Demo pulse implementation
        exposure_pulse^ epulse = Exposures::getExposurePulse(0);

        // Activate the Buzzer in manual mode
        PCB301::setBuzzerManualMode(true);

        // Activates the procedure to generate buzzer pulses at the rate of the current tomo 
        PCB301::CanDeviceCommandResult^ result = PCB301::activateManualBuzzerTomoMode(getTomoExposure()->tomo_samples, getTomoExposure()->tomo_fps, 200, this);
        if (result->error_code != PCB301::CommandRegisterErrors::COMMAND_NO_ERROR) {
            LogClass::logInFile(ExpName + "activateBuzzerDemo command failed! ");
            setExposedPulse(0, gcnew exposure_pulse(epulse->getKv(), 0, epulse->getFilter()));
            error = exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
        }
        else {
            if (result->result0 != getTomoExposure()->tomo_samples) {
                LogClass::logInFile(ExpName + "activateBuzzerDemo early terminated ");
                float mAs = epulse->getmAs() * result->result0 / getTomoExposure()->tomo_samples;
                setExposedPulse(0, gcnew exposure_pulse(epulse->getKv(), mAs, epulse->getFilter()));
                error = exposure_completed_errors::XRAY_BUTTON_RELEASE;
            }
            else {
                setExposedPulse(0, gcnew exposure_pulse(epulse->getKv(), epulse->getmAs(), epulse->getFilter()));
                error = exposure_completed_errors::XRAY_NO_ERRORS;
            }
        }
        

        // Monitor the 
        PCB301::setBuzzerManualMode(false);

    }
    
    return error;

};


Exposures::exposure_completed_errors Exposures::aec_3d_exposure_procedure(bool demo) {
    // Set thecommunication error code as general case
    return exposure_completed_errors::XRAY_NO_ERRORS;

};
