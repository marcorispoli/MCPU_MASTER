#pragma once
#include <Windows.h>
#include "PCB315.h"

using namespace System::Collections::Generic;
using namespace System::Threading;

ref class ExposureModule 
{
public:
	
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
        inline bool getValidated() { return validated; }

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

private:

    static cli::array<exposure_pulse^>^ pulse = gcnew array<exposure_pulse^> {gcnew exposure_pulse (),gcnew exposure_pulse(), gcnew exposure_pulse(), gcnew exposure_pulse() };
    static cli::array<exposure_pulse^>^ exposed = gcnew array<exposure_pulse^> {gcnew exposure_pulse(), gcnew exposure_pulse(), gcnew exposure_pulse(), gcnew exposure_pulse() };

    static exposure_type_options exposure_type = exposure_type_options::EXP_NOT_DEFINED;
    static arm_mode_option arm_mode = arm_mode_option::ARM_ENA;
    static compression_mode_option compressor_mode = compression_mode_option::CMP_KEEP;
    static patient_protection_option protection_mode = patient_protection_option::PROTECTION_ENA;
    static detector_model_option detector_model = detector_model_option::LMAM2V2;
    

    
};

