#include "SystemConfig.h"
#include "Generator.h"
#include "gantry_global_status.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB304.h"
#include "Notify.h"
#include <thread>

bool Generator::generatorDemoIdle(void) {

    while (true) {

        // Evaluates if there is a request for the X-Ray exposure
        bool procedure_ret_code = false;
        bool dose_is_released = false;
        xray_exposure_error = exposure_completed_errors::XRAY_NO_ERRORS;

        if (xray_processing) {

            // Every Xray procedure return True if the seqeunce is completed if success.
            // In case of false returned (error condition), the   xray_exposure_error is set properly
            switch (xray_exposure_type) {
            case ExposureModule::exposure_type_options::MAN_2D: procedure_ret_code = man_2d_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::AEC_2D: procedure_ret_code = aec_2d_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::MAN_3D: procedure_ret_code = man_3d_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::AEC_3D: procedure_ret_code = aec_3d_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::MAN_COMBO: procedure_ret_code = man_combo_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::AEC_COMBO: procedure_ret_code = aec_combo_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::MAN_AE: procedure_ret_code = man_ae_exposure_procedure_demo(); break;
            case ExposureModule::exposure_type_options::AEC_AE: procedure_ret_code = aec_ae_exposure_procedure_demo(); break;
            default:
                procedure_ret_code = false;
                xray_exposure_error = exposure_completed_errors::XRAY_INVALID_PROCEDURE;
            }

            // Fills the ExposureModule::exposed[] with the effettive pulse data retrived from the proper Databanks
            // The function returns true if a dose has been released.
            bool dose_is_released = setExecutedPulseDemo(xray_exposure_type);

            // Debug Prints
            if (procedure_ret_code)  Debug::WriteLine("GENERATOR EXPOSURE: Successfully completed \n");
            else  if (!dose_is_released)    Debug::WriteLine("GENERATOR EXPOSURE ERROR (NO DOSE): Error code = " + xray_exposure_error.ToString() + "\n");
            else  Debug::WriteLine("GENERATOR EXPOSURE ERROR (PARTIAL DOSE): Error code = " + xray_exposure_error.ToString() + "\n");

            // Unlock the compressor if requested
            if (ExposureModule::getCompressorMode() == ExposureModule::compression_mode_option::CMP_RELEASE) PCB302::setCompressorUnlock();

            // The X-Ray procedure termines here: the complete event is generated
            // In case of sequence not completed (partial or total) is App to the application 
            // retrive the error code (xray_exposure_error).
            if (procedure_ret_code)  xray_complete_event(exposure_completed_options::XRAY_SUCCESS);
            else if (dose_is_released) xray_complete_event(exposure_completed_options::XRAY_PARTIAL_DOSE);
            else xray_complete_event(exposure_completed_options::XRAY_NO_DOSE);

            // Waits for the X-RAY button release
            if (PCB301::getXrayPushButtonStat()) {
                Debug::WriteLine("GENERATOR EXPOSURE WAITING BUTTON RELEASE\n");
                while (PCB301::getXrayPushButtonStat()) std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            Debug::WriteLine("GENERATOR EXPOSURE TERMINATED\n");
            xray_processing = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return true;
}

bool Generator::setExecutedPulseDemo(ExposureModule::exposure_type_options xray_exposure_type) {
    return false;

    ExposureModule::exposure_pulse^ exposure_pulse;
    double kV;
    double mAs;
    PCB315::filterMaterialCodes filter;
    bool is_released_dose = false;

    // reset of the pulses
    ExposureModule::setExposedPulse(0, gcnew ExposureModule::exposure_pulse());
    ExposureModule::setExposedPulse(1, gcnew ExposureModule::exposure_pulse());
    ExposureModule::setExposedPulse(2, gcnew ExposureModule::exposure_pulse());
    ExposureModule::setExposedPulse(3, gcnew ExposureModule::exposure_pulse());

    switch (xray_exposure_type) {

    case ExposureModule::exposure_type_options::MAN_2D:
       
        exposure_pulse = ExposureModule::getExposurePulse(0);
        filter = exposure_pulse->getFilter();
        kV = exposure_pulse->getKv();
        mAs = exposure_pulse->getmAs();

        if (mAs > 1) is_released_dose = true;
        ExposureModule::setExposedPulse(0, gcnew ExposureModule::exposure_pulse(kV, mAs, filter)); // This is the only used

        break;
    case ExposureModule::exposure_type_options::AEC_2D:

        break;
    case ExposureModule::exposure_type_options::MAN_3D:  break;
    case ExposureModule::exposure_type_options::AEC_3D:  break;
    case ExposureModule::exposure_type_options::MAN_COMBO:  break;
    case ExposureModule::exposure_type_options::AEC_COMBO:  break;
    case ExposureModule::exposure_type_options::MAN_AE:  break;
    case ExposureModule::exposure_type_options::AEC_AE:  break;
    default:
        break;
    }

    return is_released_dose;

}


bool Generator::man_2d_exposure_procedure_demo(void) { 

    Debug::WriteLine("DEMO EXPOSURE 2D MANUAL STARTED\n");

    // Simulation of the preparation:
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Xray push button test
    if (!PCB301::getXrayPushButtonStat()) {
        xray_exposure_error = exposure_completed_errors::XRAY_BUTTON_RELEASE;
        return false;
    }

    // Generation of the single pulse
    Debug::WriteLine("DEMO EXPOSURE 2D MANUAL: PULSE\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Exposure completed
    Debug::WriteLine("DEMO EXPOSURE 2D MANUAL: COMPLETED\n");
    
    return true; 
}


bool Generator::aec_2d_exposure_procedure_demo(void) { return true; }
bool Generator::man_ae_exposure_procedure_demo(void) { return true; }
bool Generator::aec_ae_exposure_procedure_demo(void) { return true; }
bool Generator::man_3d_exposure_procedure_demo(void) { return true; }
bool Generator::aec_3d_exposure_procedure_demo(void) { return true; }
bool Generator::man_combo_exposure_procedure_demo(void) { return true; }
bool Generator::aec_combo_exposure_procedure_demo(void) { return true; }