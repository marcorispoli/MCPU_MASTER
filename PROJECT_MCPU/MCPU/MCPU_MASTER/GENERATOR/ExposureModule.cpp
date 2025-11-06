#include "R2CP_Eth.h"
#include "CaDataDicGen.h"
#include "ConfigurationFiles.h"
#include "Generator.h"
#include "gantry_global_status.h"
#include "ExposureModule.h"
#include "Notify.h"
#include "awsProtocol.h"
#include "ArmMotor.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB304.h"
#include <thread>
#include "Log.h"

bool Exposures::startExposure(void) {

    // test if the Exposure mode is correctly set
    if (getExposureType() == exposure_type_options::EXP_NOT_DEFINED) return false;

    // Test if the pulse 0 is actually valid
    if (!getExposurePulse(0)->validated) return false;

    // Test if the Generator is Ready to start a new exposure
    if (!isReadyForExposure()) return false; 
    
    // Test if a pending exposure is requested
    if (xray_processing) return false; 
    xray_processing = true;

    // Clear the previous completed code 
    clearXrayCompleted();

    // Invalidates the Pulse 0 because it is now consumed.
    getExposurePulse(0)->validated = false;
    return true;
}

void Exposures::setXrayEnable(bool stat)
{
    PCB301::setXrayEna(stat); 
}

bool Exposures::getXrayPushButton(void) {
    return PCB301::getXrayPushButtonStat();
}

/// <summary>
/// This function is called in the Generator Idle status in order to handle an Exposure procedure.
/// 
/// </summary>
/// 
/// The Procedure handle the activation and the completion of any exposure procedure:
/// 
/// The application should have used the dedicated API routine to activate an exposure sequence:
/// the ExposureModule class provides all the API to prrepare and select the further exposure procedure.
/// 
/// This function:
/// + Resets the internal and external Exposed-Pulse data structures:
///     + Those structure will be filled, after the exposure completion,  with the actual exposed parameters like kV, mAs,..
/// + Checks for the XRAY-push button activation;
/// + Checks for the Tube temperature validity;
/// + Pre-select the current filter that will be used;
/// + Starts the dedicated exsposure procedure;
/// + Evaluates the exposure completion code;
/// + If requested, interact with the Acquisition software to signal the exposure completion event;
/// + Handles the final status of the compressor device (keep compression or release);
/// 
/// 
/// <param name=""></param>
void Exposures::exposureManagementLoop(bool demo) {
    if (!xray_processing) return;
    getExposurePulse(0)->validated = false;

    exposure_completed_errors exposure_err_code;

    // Resets of the Internal Generator Module exposed pulses for all data-banks    
    if(!demo) R2CP::CaDataDicGen::GetInstance()->resetExecutedPulse();

    // Reset of the Application Exposure Pulses
    setExposedPulse(0, gcnew exposure_pulse());
    setExposedPulse(1, gcnew exposure_pulse());
    setExposedPulse(2, gcnew exposure_pulse());
    setExposedPulse(3, gcnew exposure_pulse());

    // Common preparation Tests
    exposure_err_code = exposure_completed_errors::XRAY_NO_ERRORS;

    if (!PCB301::getXrayPushButtonStat()) {
        // Checks if the X-RAY push button is pressed
        exposure_err_code = exposure_completed_errors::XRAY_BUTTON_RELEASE;
    }
    else if (PCB303::isTubeAlarm()) {
        // Checks for the Tube temperature
        exposure_err_code = exposure_completed_errors::XRAY_TUBE_TEMPERATURE;
    }
    else {

        
        // Every Xray procedure return True if the seqeunce is completed if success.
        // In case of false returned (error condition), the   xray_exposure_error is set properly
        switch (getExposureType()) {
        case exposure_type_options::TEST_2D: exposure_err_code = test_exposure_procedure(demo);  break;
        case exposure_type_options::MAN_2D: exposure_err_code = man_2d_exposure_procedure(demo); break;
        case exposure_type_options::AEC_2D: exposure_err_code = aec_2d_exposure_procedure(demo); break;
        case exposure_type_options::MAN_3D: 
            if (tomo_selection_mode == tomo_mode_selection_index::TOMO_CALIB) exposure_err_code = man_3d_static_exposure_procedure(demo);
            else exposure_err_code = man_3d_exposure_procedure(demo); 
            break;
        case exposure_type_options::AEC_3D: exposure_err_code = aec_3d_exposure_procedure(demo); break;
        case exposure_type_options::MAN_3D_STATIC: exposure_err_code = man_3d_static_exposure_procedure(demo); break;
        case exposure_type_options::MAN_COMBO: exposure_err_code = man_combo_exposure_procedure(demo); break;
        case exposure_type_options::AEC_COMBO: exposure_err_code = aec_combo_exposure_procedure(demo); break;
        case exposure_type_options::MAN_AE: exposure_err_code = man_ae_exposure_procedure(demo); break;
        case exposure_type_options::AEC_AE: exposure_err_code = aec_ae_exposure_procedure(demo); break;
        default:
            exposure_err_code = Exposures::exposure_completed_errors::XRAY_INVALID_PROCEDURE;
        }
    }

    // Removes the X-RAY ena signal 
    Exposures::setXrayEnable(false);

    Exposures::setCompletedError(exposure_err_code);

    // The X-Ray procedure termines here: the complete event is generated
    // In case of sequence not completed (partial or total) is App to the application 
    // retrive the error code (xray_exposure_error).
    if (exposure_err_code == Exposures::exposure_completed_errors::XRAY_NO_ERRORS)  Exposures::setCompletedCode(Exposures::exposure_completed_options::XRAY_SUCCESS);// xray_complete_event(exposure_completed_options::XRAY_SUCCESS);
    else if (Exposures::getExposedPulse(0)->getmAs()) Exposures::setCompletedCode(Exposures::exposure_completed_options::XRAY_PARTIAL_DOSE);
    else Exposures::setCompletedCode(Exposures::exposure_completed_options::XRAY_NO_DOSE);
    LogClass::logInFile("GENERATOR EXPOSURE RESULT:" + Exposures::getExposureCompletedCode().ToString() + "-" + exposure_err_code.ToString());
   

    // Only in operating mode
    if (Exposures::getExposureType() != Exposures::exposure_type_options::TEST_2D) {

        // Unlock the compressor if requested
        if (Exposures::getCompressorMode() == Exposures::compression_mode_option::CMP_RELEASE) 
            PCB302::setCompressorUnlock();

        // Notify the AWS about the XRAY completed event
        awsProtocol::EVENT_XraySequenceCompleted();

        // Disable the Xray Button
        Exposures::enableXrayPushButtonEvent(false);

        // Invalidate the current projection
        ArmMotor::abortTarget();
    }


    // Waits for the X-RAY button release
    if (PCB301::getXrayPushButtonStat()) {
        LogClass::logInFile("GENERATOR EXPOSURE WAITING BUTTON RELEASE");
        while (PCB301::getXrayPushButtonStat()) std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    LogClass::logInFile("GENERATOR EXPOSURE COMPLETED");
    xray_processing = false;
    Exposures::setXrayCompletedFlag(); // The Exposure is completed and the data are available
}

/// <summary>
    /// This function gets the data pulses received by the generator during the single Pulse sequence
    /// and store it into the exposure pulse sequence number data structure.
    /// </summary>
    /// 
    /// Every Exposure sequence can be composed with a max of four singlePulses.
    /// Those pulses are referred in the application as pulse-0 to pulse-3.
    /// 
    /// The application gets the executed pulse exposure data (kv, mAs, ..) reading 
    /// the data in the class ExposureModule::exposed after the exposure completed.
    /// 
    /// The actual exposed data are collected internally by the genberator module
    /// into the  R2CP::CaDataDicGen::GetInstance()->executed_pulses[i] vector 
    /// where i is the databank index passed to the Procedure definition.
    /// 
    /// This function is called after every single-pulse sequence 
    /// to properly upload the Application data structure with the internal collected
    /// exposure data.
    /// 
    /// 
    /// <param name="databank_index">
    /// This is the index assigned to the current data-bank in the procedure\n
    /// The index is not the number of the databank (i.e. R2CP::DB_Pre or R2CP::DB_Pulse)\n
    /// The index is the number assigned to it when the databank is assigne to a procedure
    /// 
    /// Example:\n
    /// R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Standard_Mammography_2D, 1);
    ///     - in this example 1 is the index to be used!    
    /// 
    /// </param>
    /// <param name="pulse_seq">This is the current pulse sequence (0 to 3) </param>
    /// <param name="ft">Filter used in the exposure</param>
    /// <param name="fc">This is the focus used in the exposure</param>
void Exposures::setExposedData(unsigned char databank_index, unsigned char pulse_seq, PCB303::filter_index ft, unsigned char fc) {
    if (R2CP::CaDataDicGen::GetInstance()->executed_pulses[databank_index].samples) {
        setExposedPulse(pulse_seq, gcnew Exposures::exposure_pulse(
            R2CP::CaDataDicGen::GetInstance()->executed_pulses[databank_index].kV,
            R2CP::CaDataDicGen::GetInstance()->executed_pulses[databank_index].mAs,
            ft,
            R2CP::CaDataDicGen::GetInstance()->executed_pulses[databank_index].mA,
            R2CP::CaDataDicGen::GetInstance()->executed_pulses[databank_index].ms,
            fc,
            R2CP::CaDataDicGen::GetInstance()->executed_pulses[databank_index].samples
        ));
    }
}


float Exposures::demo2DPulse(float mAs, float current) {
    
    
    // Calculates the time based on the exposure parameters
    int demo_pulse;
    try {
        demo_pulse = (mAs * 1000 / current);
        if (demo_pulse < 100) demo_pulse = 100;
    }
    catch (...) {
        demo_pulse = 100;
    }

    // Simulation of the preparation:
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Xray push button test
    if (!PCB301::getXrayPushButtonStat()) return 0;

    // Generation of the single pulse of 1 second       
    PCB301::activationManualBuzzer(true);
    int time_pulse = demo_pulse / 50;
    while (time_pulse--) {        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (!PCB301::getXrayPushButtonStat()) {
            PCB301::activationManualBuzzer(false);
            return -1 * ((demo_pulse - (time_pulse * 50)) * current) / 1000;
        }
    }

    // Success 
    PCB301::activationManualBuzzer(false);
    return 1;
}

float Exposures::demo3DPulses(float mAs, int samples, int fps) {

    // Simulation of the preparation:
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Calculates the time based on the exposure parameters
    float mas_for_sample = mAs / samples;
    int time_pulse = 1000 / fps;
    if (time_pulse > 100) time_pulse -= 100;
    if (time_pulse < 20) time_pulse = 0;

    if (!PCB301::getXrayPushButtonStat()) return 0;

    float tmas = 0;
    for (int i = 0; i < samples; i++) {
        PCB301::activationManualBuzzer(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        PCB301::activationManualBuzzer(false);
        if(time_pulse) std::this_thread::sleep_for(std::chrono::milliseconds(time_pulse));
        tmas += mas_for_sample;

        if (!PCB301::getXrayPushButtonStat()) {            
            return -1 * tmas;
        }
    }

    // Success 
    return 1;
}
