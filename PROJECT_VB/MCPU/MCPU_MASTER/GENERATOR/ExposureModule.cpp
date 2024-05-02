#include "R2CP_Eth.h"
#include "CaDataDicGen.h"
#include "SystemConfig.h"
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

bool ExposureModule::startExposure(void) {

    // test if the Exposure mode is correctly set
    if (getExposureMode() == exposure_type_options::EXP_NOT_DEFINED) return false;

    // Test if the pulse 0 is actually valid
    if (!getExposurePulse(0)->validated) return false;

    // Test if the Generator is Ready to start a new exposure
    if (!isReadyForExosure()) return false; 
    
    // Test if a pending exposure is requested
    if (xray_processing) return false; 
    xray_processing = true;

    // Clear the previous completed code 
    clearXrayCompleted();

    // Invalidates the Pulse 0 because it is now consumed.
    getExposurePulse(0)->validated = false;
    return true;
}

void ExposureModule::setXrayEnable(bool stat)
{
    PCB301::set_xray_ena(stat); 
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
void ExposureModule::exposureManagementLoop(bool demo) {
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
    else if (PCB315::isTubeAlarm()) {
        // Checks for the Tube temperature
        exposure_err_code = exposure_completed_errors::XRAY_TUBE_TEMPERATURE;
    }
    else {

        // Preliminary selection of the filter without waiting the command completion. The given seqeunce will check for completion
        PCB315::setFilterAutoMode(getExposurePulse(0)->filter, false);

        // Every Xray procedure return True if the seqeunce is completed if success.
        // In case of false returned (error condition), the   xray_exposure_error is set properly
        switch (getExposureMode()) {
        case exposure_type_options::TEST_2D: exposure_err_code = test_exposure_procedure(demo);  break;
        case exposure_type_options::MAN_2D: exposure_err_code = man_2d_exposure_procedure(demo); break;
        case exposure_type_options::AEC_2D: exposure_err_code = aec_2d_exposure_procedure(demo); break;
        case exposure_type_options::MAN_3D: exposure_err_code = man_3d_exposure_procedure(demo); break;
        case exposure_type_options::AEC_3D: exposure_err_code = aec_3d_exposure_procedure(demo); break;
        case exposure_type_options::MAN_COMBO: exposure_err_code = man_combo_exposure_procedure(demo); break;
        case exposure_type_options::AEC_COMBO: exposure_err_code = aec_combo_exposure_procedure(demo); break;
        case exposure_type_options::MAN_AE: exposure_err_code = man_ae_exposure_procedure(demo); break;
        case exposure_type_options::AEC_AE: exposure_err_code = aec_ae_exposure_procedure(demo); break;
        default:
            exposure_err_code = ExposureModule::exposure_completed_errors::XRAY_INVALID_PROCEDURE;
        }
    }

    // Removes the X-RAY ena signal 
    PCB301::set_xray_ena(false);

    ExposureModule::setCompletedError(exposure_err_code);

    // The X-Ray procedure termines here: the complete event is generated
    // In case of sequence not completed (partial or total) is App to the application 
    // retrive the error code (xray_exposure_error).
    if (exposure_err_code == ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS)  ExposureModule::setCompletedCode(ExposureModule::exposure_completed_options::XRAY_SUCCESS);// xray_complete_event(exposure_completed_options::XRAY_SUCCESS);
    else if (ExposureModule::getExposedPulse(0)->getmAs()) ExposureModule::setCompletedCode(ExposureModule::exposure_completed_options::XRAY_PARTIAL_DOSE);
    else ExposureModule::setCompletedCode(ExposureModule::exposure_completed_options::XRAY_NO_DOSE);
    LogClass::logInFile("GENERATOR EXPOSURE RESULT:" + ExposureModule::getExposureCompletedCode().ToString() + "-" + exposure_err_code.ToString());


    // De-synch the grid device
    PCB304::synchGridWithGenerator(false);

    // Only in operating mode
    if (ExposureModule::getExposureMode() != ExposureModule::exposure_type_options::TEST_2D) {

        // Unlock the compressor if requested
        if (ExposureModule::getCompressorMode() == ExposureModule::compression_mode_option::CMP_RELEASE) 
            PCB302::setCompressorUnlock();

        // Notify the AWS about the XRAY completed event
        awsProtocol::EVENT_XraySequenceCompleted();

        // Disable the Xray Button
        ExposureModule::enableXrayPushButtonEvent(false);

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
    ExposureModule::setXrayCompletedFlag(); // The Exposure is completed and the data are available
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
void ExposureModule::setExposedData(unsigned char databank_index, unsigned char pulse_seq, PCB315::filterMaterialCodes ft, unsigned char fc) {
    if (R2CP::CaDataDicGen::GetInstance()->executed_pulses[databank_index].samples) {
        setExposedPulse(pulse_seq, gcnew ExposureModule::exposure_pulse(
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
