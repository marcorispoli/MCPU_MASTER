#include "SystemConfig.h"
#include "Generator.h"
#include "gantry_global_status.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB304.h"
#include "TiltMotor.h"
#include "ArmMotor.h"
#include "Notify.h"
#include "awsProtocol.h"
#include <thread>
#include "Log.h"


bool Generator::generatorDemoIdle(void) {
    ExposureModule::exposure_completed_errors exposure_err_code;

    while (true) {

        
        if (xray_processing) {
            
            // Activate the Buzzer in manual mode
            PCB301::set_manual_buzzer(true); 

            // reset of the pulses
            ExposureModule::setExposedPulse(0, gcnew ExposureModule::exposure_pulse());
            ExposureModule::setExposedPulse(1, gcnew ExposureModule::exposure_pulse());
            ExposureModule::setExposedPulse(2, gcnew ExposureModule::exposure_pulse());
            ExposureModule::setExposedPulse(3, gcnew ExposureModule::exposure_pulse());
            
            // Every Xray procedure return True if the seqeunce is completed if success.
            // In case of false returned (error condition), the   xray_exposure_error is set properly
            switch (ExposureModule::getExposureMode()) {
            case ExposureModule::exposure_type_options::MAN_2D: exposure_err_code = man_2d_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::AEC_2D: exposure_err_code = aec_2d_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::MAN_3D: exposure_err_code = man_3d_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::AEC_3D: exposure_err_code = aec_3d_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::MAN_COMBO: exposure_err_code = man_combo_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::AEC_COMBO: exposure_err_code = aec_combo_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::MAN_AE: exposure_err_code = man_ae_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::AEC_AE: exposure_err_code = aec_ae_exposure_procedure_demo(); break;
            default:
                exposure_err_code = ExposureModule::exposure_completed_errors::XRAY_INVALID_PROCEDURE;
            }
            
            PCB301::set_manual_buzzer(false); // Set the buzzer in Auto mode

            ExposureModule::setCompletedError(exposure_err_code);

            // The X-Ray procedure termines here: the complete event is generated
            // In case of sequence not completed (partial or total) is App to the application 
            // retrive the error code (xray_exposure_error).
            if (exposure_err_code == ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS)  ExposureModule::setCompletedCode(ExposureModule::exposure_completed_options::XRAY_SUCCESS);// xray_complete_event(exposure_completed_options::XRAY_SUCCESS);
            else if (ExposureModule::getExposedPulse(0)->getmAs()) ExposureModule::setCompletedCode(ExposureModule::exposure_completed_options::XRAY_PARTIAL_DOSE);
            else ExposureModule::setCompletedCode(ExposureModule::exposure_completed_options::XRAY_NO_DOSE);
            LogClass::logInFile("GENERATOR EXPOSURE RESULT:" + ExposureModule::getExposureCompletedCode().ToString() + "-" + exposure_err_code.ToString());

            // Debug Prints
            if (exposure_err_code == ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS)  LogClass::logInFile("GENERATOR EXPOSURE: Successfully completed \n");
            else  if (ExposureModule::getExposedPulse(0)->getmAs() == 0)    LogClass::logInFile("GENERATOR EXPOSURE ERROR (NO DOSE): Error code = " + exposure_err_code.ToString() + "\n");
            else  LogClass::logInFile("GENERATOR EXPOSURE ERROR (PARTIAL DOSE): Error code = " + exposure_err_code.ToString() + "\n");

            // Unlock the compressor if requested
            if (ExposureModule::getCompressorMode() == ExposureModule::compression_mode_option::CMP_RELEASE) PCB302::setCompressorUnlock();
             
            // Notify the AWS about the XRAY completed event
            awsProtocol::EVENT_XraySequenceCompleted();
           
            // Disable the Xray Button
            ExposureModule::enableXrayPushButtonEvent(false);

            // Invalidate the current projection
            ArmMotor::abortTarget();

            // Waits for the X-RAY button release
            if (PCB301::getXrayPushButtonStat()) {
                LogClass::logInFile("GENERATOR EXPOSURE WAITING BUTTON RELEASE\n");
                while (PCB301::getXrayPushButtonStat()) std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            xray_processing = false;
            ExposureModule::setXrayCompletedFlag(); // The Exposure is completed and the data are available
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return true;
}


ExposureModule::exposure_completed_errors Generator::man_2d_exposure_procedure_demo(void) {
    ExposureModule::exposure_pulse^ exposure_pulse;
    double kV;
    double mAs;
    PCB315::filterMaterialCodes filter;

    LogClass::logInFile("DEMO EXPOSURE 2D MANUAL STARTED\n");

    // Simulation of the preparation:
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Xray push button test
    if (!PCB301::getXrayPushButtonStat()) {
        return ExposureModule::exposure_completed_errors::XRAY_BUTTON_RELEASE;
    }

    PCB301::set_activation_buzzer(true);

    // Generation of the single pulse
    LogClass::logInFile("DEMO EXPOSURE 2D MANUAL: PULSE\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    PCB301::set_activation_buzzer(false);

    exposure_pulse = ExposureModule::getExposurePulse(0);
    filter = exposure_pulse->getFilter();
    kV = exposure_pulse->getKv();
    mAs = exposure_pulse->getmAs();
    ExposureModule::setExposedPulse(0, gcnew ExposureModule::exposure_pulse(kV, mAs, filter)); // This is the only used

    // Exposure completed
    LogClass::logInFile("DEMO EXPOSURE 2D MANUAL: COMPLETED\n");
    
    return ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS;
}


ExposureModule::exposure_completed_errors Generator::aec_2d_exposure_procedure_demo(void) {
    ExposureModule::exposure_pulse^ exposure_pulse;
    double kV;
    double mAs;
    PCB315::filterMaterialCodes filter;

    LogClass::logInFile("DEMO EXPOSURE 2D AEC STARTED\n");

    // Simulation of the preparation:
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Xray push button test
    if (!PCB301::getXrayPushButtonStat()) {
        return ExposureModule::exposure_completed_errors::XRAY_BUTTON_RELEASE;
    }

    PCB301::set_activation_buzzer(true);
    // Generation of the Aec  pulse
    LogClass::logInFile("DEMO EXPOSURE 2D AEC: PRE-PULSE\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    PCB301::set_activation_buzzer(false);

    exposure_pulse = ExposureModule::getExposurePulse(0);
    filter = exposure_pulse->getFilter();
    kV = exposure_pulse->getKv();
    mAs = exposure_pulse->getmAs();
    ExposureModule::setExposedPulse(0, gcnew ExposureModule::exposure_pulse(kV, mAs, filter)); // This is the only used


    // Notifies the AWS about the pulse termination
    awsProtocol::EVENT_exposurePulseCompleted((unsigned char) 0);

    // Waits for the Main Pulse Data valid    
    for (int i = 0; i < 100; i++) {
        if (!PCB301::getXrayPushButtonStat()) {
            return ExposureModule::exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }

        if (ExposureModule::getExposurePulse(1)->isValid()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!ExposureModule::getExposurePulse(1)->isValid()) {
        return ExposureModule::exposure_completed_errors::XRAY_TIMEOUT_AEC;
    }
    
    // Invalidates the Pulse 1 because it is consumed.
    ExposureModule::getExposurePulse(1)->validated = false;

    PCB301::set_activation_buzzer(true);
    // Generation of the Aec  pulse
    LogClass::logInFile("DEMO EXPOSURE 2D AEC: MAIN-PULSE\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    PCB301::set_activation_buzzer(false);

    exposure_pulse = ExposureModule::getExposurePulse(1);
    filter = exposure_pulse->getFilter();
    kV = exposure_pulse->getKv();
    mAs = exposure_pulse->getmAs();
    ExposureModule::setExposedPulse(1, gcnew ExposureModule::exposure_pulse(kV, mAs, filter)); // This is the only used

    // Exposure completed
    LogClass::logInFile("DEMO EXPOSURE 2D AEC: COMPLETED\n");
    return ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS;
}

ExposureModule::exposure_completed_errors Generator::man_ae_exposure_procedure_demo(void) { return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR; }
ExposureModule::exposure_completed_errors Generator::aec_ae_exposure_procedure_demo(void) { return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR; }

ExposureModule::exposure_completed_errors Generator::man_3d_exposure_procedure_demo(void) { 
    ExposureModule::exposure_pulse^ exposure_pulse;
    double kV;
    double mAs;
    PCB315::filterMaterialCodes filter;

    LogClass::logInFile("DEMO EXPOSURE 3D MANUAL STARTED\n");

    // Position the Tube in HOME
    while(!TiltMotor::device->isReady()){ 
        if (!PCB301::getXrayPushButtonStat()) return ExposureModule::exposure_completed_errors::XRAY_BUTTON_RELEASE;
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
    }
    if(!TiltMotor::setTarget(TiltMotor::target_options::TOMO_H, 0)) return ExposureModule::exposure_completed_errors::XRAY_POSITIONING_ERROR;

    while (!TiltMotor::device->isReady()) { 
        if (!PCB301::getXrayPushButtonStat()) return ExposureModule::exposure_completed_errors::XRAY_BUTTON_RELEASE;
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
    }

    // Simulation of the preparation:
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Starts the Tomo scan 
    if(!TiltMotor::setTarget(TiltMotor::target_options::TOMO_E, 0)) return ExposureModule::exposure_completed_errors::XRAY_POSITIONING_ERROR;
     
    int deltat = 1000 / ExposureModule::getTomoExposure()->tomo_fps;
    if (deltat > 200) deltat -= 200;
    else deltat = 100;

    for (int i = 0; i < ExposureModule::getTomoExposure()->tomo_samples; i++) {
        
        // Xray push button test
        if (!PCB301::getXrayPushButtonStat())  return ExposureModule::exposure_completed_errors::XRAY_BUTTON_RELEASE;

        PCB301::set_activation_buzzer(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        PCB301::set_activation_buzzer(false);
        std::this_thread::sleep_for(std::chrono::milliseconds(deltat));
    }


    exposure_pulse = ExposureModule::getExposurePulse(0);
    filter = exposure_pulse->getFilter();
    kV = exposure_pulse->getKv();
    mAs = exposure_pulse->getmAs();
    ExposureModule::setExposedPulse(0, gcnew ExposureModule::exposure_pulse(kV, mAs, filter)); // This is the only used

    // Exposure completed
    LogClass::logInFile("DEMO EXPOSURE 3D MANUAL: COMPLETED\n");

    return ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS;
}

ExposureModule::exposure_completed_errors Generator::aec_3d_exposure_procedure_demo(void) { return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR; }
ExposureModule::exposure_completed_errors Generator::man_combo_exposure_procedure_demo(void) { return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR; }
ExposureModule::exposure_completed_errors Generator::aec_combo_exposure_procedure_demo(void) { return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR; }