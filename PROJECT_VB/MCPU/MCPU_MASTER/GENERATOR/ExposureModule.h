#pragma once
#include <Windows.h>
#include "PCB315.h"
#include "CalibrationConfig.h"
#include "Generator.h"

/// <summary>
/// \defgroup Exposure_Module Exposure Module
/// This Module implements the Application exposure sequence workflows.
/// 
/// </summary>
/// 
/// 
///	
 
using namespace System::Collections::Generic;
using namespace System::Threading;

/// <summary>
/// This Class implements the Exposure data structures and the exposure worflows.
/// </summary>
/// 
/// \ingroup Exposure_Module
ref class Exposures : public Generator
{
public:

    literal unsigned char  FOCUS_LARGE = 0;
    literal unsigned char  FOCUS_SMALL = 1;
    static Exposures^ pExposure = gcnew Exposures(); //! Self module generation
    static inline  Generator^ getDevice(void) { return device; }
	
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

        exposure_pulse(double kv, double mas, PCB315::filterMaterialCodes flt, float ma, float mS, unsigned char fc, int samples) {

            validated = false;
            filter = flt;
            kV = kv;// Assignes the kV
            mAs = mas;// Assignes the mAs
            this->mA = ma;
            this->ms = mS;
            pulse_samples = samples;
            focus = fc; // only for test            
            validated = true;
        };

        exposure_pulse(double kv, double mas, PCB315::filterMaterialCodes flt, unsigned char fc, bool gd, bool sync) {

            validated = false;
            if ((kv > 49.0) || (kv < 20.0)) return;
            if ((mas > 640) || (mas < 0)) return;
            if (flt == PCB315::filterMaterialCodes::FILTER_INVALID) return;

            filter = flt;
            kV = kv;        
            mAs = mas;      

            pulse_samples = 0;
            focus = fc;         
            use_grid = gd;        // only for test
            synch_det = sync;     // only for test
            validated = true;
        };

        inline double getKv() { return kV; }
        inline double getmAs() { return mAs; }
        inline double getmA() { return mA; }
        inline double getms() { return ms; }
        inline double getSamples() { return pulse_samples; }
        inline unsigned char getFocus() { return focus; }
        inline bool isLargeFocus() { return (focus == FOCUS_LARGE); }
        inline bool isSmallFocus() { return (focus == FOCUS_SMALL); }
        inline PCB315::filterMaterialCodes getFilter() { return filter; }
        inline bool useGrid() { return use_grid; }
        inline bool useDetector() { return synch_det; }
        inline bool isValid() { return validated; }


        unsigned char focus; // This is the focus assigned to this pulse
        PCB315::filterMaterialCodes filter; //!< This is the assigned filter 
        bool    use_grid; // Grid shall be used (only test mode)
        bool    synch_det; // Detector sync shall be used (only test mode)
        double  kV; //!< This is the selected kV value
        double  mAs; //!< This is the selected mAs value
        float   mA; //!< anodic current used in mA
        float   ms; //!< pulse endurance in ms
        int     pulse_samples; //!< Number of portion of the pulses received       
        bool    validated; //!< This is the flag that validate the use of thhis pulse in a sequence
    };

	

    /// <summary>
    /// This is the enumeration class defining all the possible exposures 
    /// 
    /// </summary>
    enum class exposure_type_options {
        MAN_2D=0, //!< The next exposure is a 2D manual mode
        AEC_2D, //!< The next exposure is a 2D with AEC
        MAN_3D, //!< The next exposure is a Tomo 3D in manual mode
        AEC_3D, //!< The next exposure is a Tomo 3D with AEC
        MAN_COMBO, //!< The next exposure is a Manual Combo
        AEC_COMBO, //!< The next exposure is a Combo with AEC
        MAN_AE, //!< The next exposure is a Dual energy exposure in manual mode
        AEC_AE, //!< The next exposure is a Dual energy with AEC        
        TEST_2D, //!< This is a test exposure without Detector synch
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
    enum class exposure_completed_errors 
    {
        XRAY_NO_ERRORS = (int) Generator::generator_errors::GEN_NO_ERRORS,			//!< No error code
        XRAY_BUTTON_RELEASE = (int)Generator::generator_errors::GEN_BUTTON_RELEASE, //!< The X-Ray Button has been released  
        XRAY_TIMEOUT = (int) Generator::generator_errors::GEN_TIMEOUT,			    //!< Timeout generator sequence
        XRAY_COMMUNICATION_ERROR = (int)Generator::generator_errors::GEN_COMMUNICATION_ERROR,         //!< A generator command is failed        
        XRAY_INVALID_GENERATOR_STATUS = (int) Generator::generator_errors::GEN_INVALID_STATUS,//!< The generator is in a not expected status	
        XRAY_ERR_CODE = (int)Generator::generator_errors::GEN_LAST_ERRCODE,         //!< Initialize the ExposureModule error codes        
        XRAY_INVALID_2D_PARAMETERS,	//!< The pexposure parameters for 2D are incorrect
        XRAY_INVALID_TOMO_PARAMETERS,	//!< The Tomo parameters has not been validated (selected)        
        XRAY_TIMEOUT_TILT_IN_HOME,	//!< Timeout waiting for the Tilt to be ready for home positioning 
        XRAY_ERROR_TILT_IN_HOME,	//!< Error in positionining the Tilt in Home        		
        XRAY_GRID_ERROR,			//!< The Grid device is in error condition
        XRAY_COLLI_FORMAT_ERROR,	//!< The current collimation format is not valid        
        XRAY_TIMEOUT_AEC,			//!< Timeout waiting the Main Pulse data after an AEC pre pulse
        XRAY_POSITIONING_ERROR,     //!< Error in ARM or Tilt positioning
        XRAY_FILTER_ERROR,          //!< Error in selecting the filter for the exposure
        XRAY_TUBE_TEMPERATURE,      //!< Error the Tube is in fault condition
        XRAY_INVALID_PROCEDURE,     //!< Error non defined procedure has been requested

    };

    
    static void inline setExposureMode(exposure_type_options mode) { exposure_type = mode; }
    static exposure_type_options inline getExposureMode(void) { return exposure_type; }

    static void inline setDetectorType(DetectorConfig::detector_model_option detector) { detector_model = detector; }
    static DetectorConfig::detector_model_option inline getDetectorType(void) { return detector_model; }

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


    static bool startExposure(void);
   
    
protected:
    void exposureManagementLoop(bool demo) override; 
    void setXrayEnable(bool stat) override ;


private:

    static cli::array<exposure_pulse^>^ pulse = gcnew array<exposure_pulse^> {gcnew exposure_pulse (),gcnew exposure_pulse(), gcnew exposure_pulse(), gcnew exposure_pulse() };
    static cli::array<exposure_pulse^>^ exposed = gcnew array<exposure_pulse^> {gcnew exposure_pulse(), gcnew exposure_pulse(), gcnew exposure_pulse(), gcnew exposure_pulse() };

    static exposure_type_options exposure_type = exposure_type_options::EXP_NOT_DEFINED;
    static arm_mode_option arm_mode = arm_mode_option::ARM_ENA;
    static compression_mode_option compressor_mode = compression_mode_option::CMP_KEEP;
    static patient_protection_option protection_mode = patient_protection_option::PROTECTION_ENA;
    static DetectorConfig::detector_model_option detector_model = DetectorConfig::detector_model_option::GENERIC;
    static exposure_type_options xray_exposure_type = exposure_type_options::EXP_NOT_DEFINED;
    static tomo_data^ tomo_exposure = gcnew tomo_data;
    static bool xray_event_ena = false;

    // X-Ray completed section    
    static bool xray_completed = true;
    static bool xray_processing = false; 
    static exposure_completed_options xray_completed_code = exposure_completed_options::XRAY_NO_DOSE;
    static exposure_completed_errors xray_exposure_error = exposure_completed_errors::XRAY_NO_ERRORS;

    exposure_completed_errors man_2d_exposure_procedure(bool demo);
    exposure_completed_errors aec_2d_exposure_procedure(bool demo);
    exposure_completed_errors man_ae_exposure_procedure(bool demo);
    exposure_completed_errors aec_ae_exposure_procedure(bool demo);
    exposure_completed_errors man_3d_exposure_procedure(bool demo);
    exposure_completed_errors aec_3d_exposure_procedure(bool demo);
    exposure_completed_errors man_combo_exposure_procedure(bool demo);
    exposure_completed_errors aec_combo_exposure_procedure(bool demo);    
    exposure_completed_errors test_exposure_procedure(bool demo);

    void setExposedData(unsigned char databank_index, unsigned char pulse_seq, PCB315::filterMaterialCodes ft, unsigned char fc);
        
   
};

