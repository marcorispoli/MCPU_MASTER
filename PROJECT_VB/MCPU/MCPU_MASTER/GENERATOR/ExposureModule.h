#pragma once
#include <Windows.h>
#include "PCB315.h"
#include "CalibrationConfig.h"


using namespace System::Collections::Generic;
using namespace System::Threading;

ref class ExposureModule 
{
public:
	
    ref class tomo_data {
    public:

        tomo_data(void) {
            valid = false;
        }

        System::String^ configName; //!< Name of the selected configuration;
        bool valid; //!< A valid selection is present
        int tomo_home;
        int tomo_end;
        int tomo_skip;
        int tomo_samples;
        int tomo_speed;
        int tomo_acc;
        int tomo_dec;
        int tomo_fps;

        void invalidate(void) { valid = false; }

        bool set(System::String^ cfg) {
            for (int i = 0; i < (int)TomoConfig::tomo_id::TOMO_ID_NUM; i++) {
                if (cfg == ((TomoConfig::tomo_id)i).ToString()) {
                    valid = true;
                    tomo_home = System::Convert::ToInt16(TomoConfig::Configuration->getParam(cfg)[TomoConfig::PARAM_TOMO_HOME]);
                    tomo_end = System::Convert::ToInt16(TomoConfig::Configuration->getParam(cfg)[TomoConfig::PARAM_TOMO_END]);
                    tomo_skip = System::Convert::ToInt16(TomoConfig::Configuration->getParam(cfg)[TomoConfig::PARAM_TOMO_SKIP]);
                    tomo_samples = System::Convert::ToInt16(TomoConfig::Configuration->getParam(cfg)[TomoConfig::PARAM_TOMO_SAMPLES]);
                    tomo_speed = System::Convert::ToInt16(TomoConfig::Configuration->getParam(cfg)[TomoConfig::PARAM_TOMO_SPEED]);
                    tomo_acc = System::Convert::ToInt16(TomoConfig::Configuration->getParam(cfg)[TomoConfig::PARAM_TOMO_ACC]);
                    tomo_dec = System::Convert::ToInt16(TomoConfig::Configuration->getParam(cfg)[TomoConfig::PARAM_TOMO_DEC]);
                    tomo_fps = System::Convert::ToInt16(TomoConfig::Configuration->getParam(cfg)[TomoConfig::PARAM_TOMO_FPS]);                    
                    return true;
                }
            }
            valid = false;
            return false;
        }
    };

    ref class exposure_pulse 
    {
    public:


        /// <summary>
        /// This is the creator of the pulse data class.
        /// 
        /// </summary>
        /// <param name=""></param>
        exposure_pulse(void) {

            // The handle of the filter selection is created
            filter = PCB315::filterMaterialCodes::FILTER_INVALID;
            kV = 0;
            mAs = 0;

            // When created the pulse is invalidated for safety
            validated = false;
        };

        exposure_pulse(double kv, double mas, PCB315::filterMaterialCodes flt) {
            
            validated = false;

            if ((kv > 49.0) || (kv < 20.0)) return ;
            if ((mas > 640) || (mas < 0)) return;
            if (flt == PCB315::filterMaterialCodes::FILTER_INVALID) return;

            filter = flt;
            kV = kv;// Assignes the kV
            mAs = mas;// Assignes the mAs
            validated = true;
        };

        inline double getKv() { return kV; }
        inline double getmAs() { return mAs; }
        inline PCB315::filterMaterialCodes getFilter() { return filter; }
        inline bool isValid() { return validated; }

        PCB315::filterMaterialCodes filter; //!< This is the assigned filter 
        double kV; //!< This is the selected kV value
        double mAs; //!< This is the selected mAs value
        bool   validated; //!< This is the flag that validate the use of thhis pulse in a sequence
    };

	

    /// <summary>
    /// This is the enumeration class defining all the possible exposures 
    /// 
    /// </summary>
    enum class exposure_type_options {
        MAN_2D = 0, //!< The next exposure is a 2D manual mode
        AEC_2D, //!< The next exposure is a 2D with AEC
        MAN_3D, //!< The next exposure is a Tomo 3D in manual mode
        AEC_3D, //!< The next exposure is a Tomo 3D with AEC
        MAN_COMBO, //!< The next exposure is a Manual Combo
        AEC_COMBO, //!< The next exposure is a Combo with AEC
        MAN_AE, //!< The next exposure is a Dual energy exposure in manual mode
        AEC_AE, //!< The next exposure is a Dual energy with AEC        
        EXP_NOT_DEFINED
    };
    

    /// <summary>
    /// This is the enumeration of the possible Compression modes.
    /// 
    /// + CMP_KEEP: The commpressor paddle is not released after exposure;
    /// + CMP_RELEASE: The coompressor paddle is released after exposure;
    /// + CMP_DISABLE: The compression is not necessary (but can be applied) for the current exposure.
    /// 
    /// </summary>
    enum class compression_mode_option {
        CMP_KEEP = 0, //!< Keeps the compression after exposure;
        CMP_RELEASE,  //!< Releases the compression after exposure;
        CMP_DISABLE, //!< Disables the Compression check (for exposures without the compression);

    };
    // static const cli::array<String^>^ tags = gcnew cli::array<String^>  { "CMP_KEEP", "CMP_RELEASE", "CMP_DISABLE", "UNDEF"}; //!< This is the option-tags static array

    /// <summary>
    /// This is the enumeration of the possible ARM usage during exposure
    /// 
    /// + ARM_ENA: The ARM shall be in a position defined by the Acquisition software;
    /// + ARM_DIS: The ARM can be in a arbitrary position;
    /// 
    /// </summary>
    enum class arm_mode_option {
        ARM_ENA = 0, //!< Enables the Angle range check during exposure;
        ARM_DIS, //!< Disables the Angle range check during the exposure;                    
    };
    //static const cli::array<String^>^ tags = gcnew cli::array<String^>  { "ARM_ENA", "ARM_DIS", "UNDEF"}; //!< This is the option-tags static array

    /// <summary>
    /// This is the enumeration of the possible Patient Protection usage during exposure
    /// 
    /// + PROTECTION_ENA: The Patient protection shall be present during exposure;
    /// + PROTECTION_DIS: The patient protection can be removed during exposure;
    /// 
    /// </summary>
    enum class patient_protection_option {
        PROTECTION_ENA = 0, //!< Enables the Patient protection check;
        PROTECTION_DIS, //!< Disables the Patient protection check;                   
    };
    //static const cli::array<String^>^ tags = gcnew cli::array<String^>  { "PROTECTION_ENA", "PROTECTION_DIS", "UNDEF"}; //!< This is the option-tags static array

    /// <summary>
    /// This is the enumeration of the possible Detectors used for exposures
    /// 
    /// </summary>
    enum class detector_model_option {
        LMAM2V2 = 0, //!< Analogic LMAM2 V2
        FDIV2,  //!< Analogic FDI-V2            
    };
    // static const cli::array<String^>^ tags = gcnew cli::array<String^>  { "LMAM2V2", "FDIV2", "UNDEF"}; //!< This is the option-tags static array

    /// <summary>
    /// This Enumeration class describes the possible results of an exposure.
    /// 
    /// + XRAY_SUCCESS: All the expected exposure's pulses has been successfully executed;
    /// + XRAY_PARTIAL_DOSE: the exposure has been partially executed.
    /// + XRAY_NO_DOSE: The exposure is early terminated without dose released to the patient.
    /// + XRAY_EXECUTING: The X-RAY sequence is still running
    /// 
    /// </summary>
    enum class exposure_completed_options {
        XRAY_SUCCESS = 0, //!< XRAY sequence successfully completed
        XRAY_PARTIAL_DOSE = 1,//!< XRAY sequence partially completed
        XRAY_NO_DOSE = 2,//!< XRAY sequence aborted without dose       
    };

    /// <summary>
    /// This class enumerates all the possible x-ray error reasons
    /// 
    /// </summary>
    enum class exposure_completed_errors {
        XRAY_NO_ERRORS = 0,			//!< No error code
        XRAY_INVALID_PROCEDURE,		//!< A not valid procedure has been requested
        XRAY_COMMUNICATION_ERROR,	//!< A generator command is failed
        XRAY_GENERATOR_ERROR,		//!< The generator activated internal error messages
        XRAY_BUTTON_RELEASE,		//!< The X-Ray Button has been released 
        XRAY_FILAMENT_ERROR,		//!< The generator detected a Filament error
        XRAY_KV_ERROR,				//!< The generator detected a kV error
        XRAY_STARTER_ERROR,			//!< The generator detected an Anode Starter error condition
        XRAY_GRID_ERROR,			//!< The Grid device is in error condition
        XRAY_TIMEOUT_AEC,			//!< Timeout waiting the Main Pulse data after an AEC pre pulse
        XRAY_POSITIONING_ERROR,     //!< Error in ARM or Tilt positioning
    };

    
    static void inline setExposureMode(exposure_type_options mode) { exposure_type = mode; }
    static exposure_type_options inline getExposureMode(void) { return exposure_type; }

    static void inline setDetectorType(detector_model_option detector) { detector_model = detector; }
    static detector_model_option inline getDetectorType(void) { return detector_model; }

    static void inline setCompressorMode(compression_mode_option mode) { compressor_mode = mode; }
    static compression_mode_option inline getCompressorMode(void) { return compressor_mode; }

    static void inline setProtectionMode(patient_protection_option mode) { protection_mode = mode; }
    static patient_protection_option inline getProtectionMode(void) { return protection_mode; }

    static void inline setArmMode(arm_mode_option mode) { arm_mode = mode; }
    static arm_mode_option inline getArmMode(void) { return arm_mode; }

    static bool setExposurePulse(unsigned char seq, exposure_pulse^ pls) {
        if (seq >= pulse->Length) return false;
        if (!pls->validated) return false;
        pulse[seq] = pls;
        return true;
    }

    static bool setExposedPulse(unsigned char seq, exposure_pulse^ pls) {
        if (seq >= exposed->Length) return false;
        exposed[seq] = pls;
        exposed[seq]->validated = true;
        return true;
    }

    static exposure_pulse^ getExposedPulse(unsigned char seq) {
        if (seq >= exposed->Length) return gcnew exposure_pulse();
        return  exposed[seq];        
    }

    static exposure_pulse^ getExposurePulse(unsigned char seq) {
        if (seq >= pulse->Length) return gcnew exposure_pulse();
        return  pulse[seq];
    }

    inline static bool isXrayCompleted() { return xray_completed; }
    inline static bool isXrayRunning() { return !xray_completed; }
    inline static void clearXrayCompleted() { xray_completed = false; }
    inline static void setXrayCompletedFlag() { xray_completed = true; }

    inline static exposure_completed_errors getExposureCompletedError(void) { return xray_exposure_error; }
    inline static exposure_completed_options getExposureCompletedCode(void) { return xray_completed_code; }
    inline static void setCompletedError(exposure_completed_errors err) { xray_exposure_error = err; }
    inline static void setCompletedCode(exposure_completed_options code) { xray_completed_code = code; }
    inline static tomo_data^ getTomoExposure(void) { return tomo_exposure; }
    inline static void enableXrayPushButtonEvent(bool  stat) { xray_event_ena = stat; }
    inline static bool getXrayPushButtonEvent(void) { return xray_event_ena; }

    static void reset(void) {
        pulse = gcnew array<exposure_pulse^> {gcnew exposure_pulse(), gcnew exposure_pulse(), gcnew exposure_pulse(), gcnew exposure_pulse() };
        exposed = gcnew array<exposure_pulse^> {gcnew exposure_pulse(), gcnew exposure_pulse(), gcnew exposure_pulse(), gcnew exposure_pulse() };
        exposure_type = exposure_type_options::EXP_NOT_DEFINED;
        arm_mode = arm_mode_option::ARM_ENA;
        compressor_mode = compression_mode_option::CMP_KEEP;
        protection_mode = patient_protection_option::PROTECTION_ENA;
        xray_exposure_type = exposure_type_options::EXP_NOT_DEFINED;
        tomo_exposure = gcnew tomo_data;
        xray_completed = true;
        xray_exposure_error = exposure_completed_errors::XRAY_NO_ERRORS;
        xray_completed_code = exposure_completed_options::XRAY_NO_DOSE;
        xray_event_ena = false;
    }

private:

    static cli::array<exposure_pulse^>^ pulse = gcnew array<exposure_pulse^> {gcnew exposure_pulse (),gcnew exposure_pulse(), gcnew exposure_pulse(), gcnew exposure_pulse() };
    static cli::array<exposure_pulse^>^ exposed = gcnew array<exposure_pulse^> {gcnew exposure_pulse(), gcnew exposure_pulse(), gcnew exposure_pulse(), gcnew exposure_pulse() };

    static exposure_type_options exposure_type = exposure_type_options::EXP_NOT_DEFINED;
    static arm_mode_option arm_mode = arm_mode_option::ARM_ENA;
    static compression_mode_option compressor_mode = compression_mode_option::CMP_KEEP;
    static patient_protection_option protection_mode = patient_protection_option::PROTECTION_ENA;
    static detector_model_option detector_model = detector_model_option::LMAM2V2;
    static exposure_type_options xray_exposure_type = exposure_type_options::EXP_NOT_DEFINED;
    static tomo_data^ tomo_exposure = gcnew tomo_data;
    static bool xray_event_ena = false;

    // X-Ray completed section    
    static bool xray_completed = true;
    static exposure_completed_options xray_completed_code = exposure_completed_options::XRAY_NO_DOSE;
    static exposure_completed_errors xray_exposure_error = exposure_completed_errors::XRAY_NO_ERRORS;

};

