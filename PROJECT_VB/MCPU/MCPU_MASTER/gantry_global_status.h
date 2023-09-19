#pragma once

#include <math.h>
#include "ConfigFile.h"

using namespace System;
using namespace System::Collections::Generic;


ref class GlobalObjects
{
public:

    #define pIDLEFORM  ((IdleForm^) GlobalObjects::pIdleForm)
    #define pAWS ((awsProtocol^) GlobalObjects::pAws)
    #define pCAN ((CanDriver^) GlobalObjects::pCan)
    #define pFW301 ((PCB301^) GlobalObjects::pFw301)
    
    #define pFW302 ((CanDeviceProtocol^) GlobalObjects::pFw302)
    #define pFW303 ((CanDeviceProtocol^) GlobalObjects::pFw303)
    #define pFW304 ((CanDeviceProtocol^) GlobalObjects::pFw304)
    #define pFW315 ((CanDeviceProtocol^) GlobalObjects::pFw315)
    #define pMBODY ((CanOpenMotor^) GlobalObjects::pMotBody)
    #define pMTILT ((TiltMotor^) GlobalObjects::pMotTilt)
    #define pMARM  ((CanOpenMotor^) GlobalObjects::pMotArm)
    #define pMSHIFT  ((CanOpenMotor^) GlobalObjects::pMotShift)
    #define pMVERT  ((CanOpenMotor^) GlobalObjects::pMotVert)

    static Object^ pAws = nullptr; //!< Pointer to the AWS interface
    static Object^ pCan = nullptr; //!< Pointer to the Can Driver

    // Motor pointers
    static Object^ pMotTilt = nullptr; //!< Pointer to the Tilt Motor
    static Object^ pMotArm = nullptr;  //!< Pointer to the Arm Motor
    static Object^ pMotBody = nullptr; //!< Pointer to the Body Motor
    static Object^ pMotShift = nullptr; //!< Pointer to the Shift Motor
    static Object^ pMotVert = nullptr;//!< Pointer to the Up/Down Motor

    static Object^ pFw301 = nullptr; //!< Pointer to the FW301 board
    static Object^ pFw302 = nullptr; //!< Pointer to the FW302 board
    static Object^ pFw303 = nullptr; //!< Pointer to the FW303 board
    static Object^ pFw304 = nullptr; //!< Pointer to the FW304 board
    static Object^ pFw315 = nullptr; //!< Pointer to the FW315 board

    // Monitor coordinates
    static int monitor_X0;//!< Pointer to the Monitor X0 position
    static int monitor_Y0;//!< Pointer to the Monitor Y0 position
    
    // Forms
    static Object^ pIdleForm = nullptr; //!< Pointer to the IdleForm 
    
};

/// <summary>
/// This is the module implementing the Gantry Global Status registers.
/// 
/// \defgroup globalModule Gantry Global Status Registers
/// 
/// The Gantry internal status is defined by several data 
/// that represent the current status of some device item or 
/// system setting.
/// 
/// Every register status is implemented with a dedicated static class
/// with the following commmon characteristics:
/// - It is define an enumeration code;
/// - It is defined an array of strings rapresenting the tags of every code;
/// - It is defined a set of functions to access the internal data;
/// - It is defined one or more event associated to the internal data changing.
/// 
/// The class is commonly defined with:
/// - some_class::options::<enumeration_value>
/// - some_class::tags[] ={"tag0",..,"tagn"}
/// - some_class::getCode(String^ tag)
/// - some_class::getTag(some_class::options::<enum value>)
/// - some_class::setCode(String^ tag)
/// - some_class::setTag(some_class::options::<enum value>)
/// - some_class::isValid(String^ tag)
/// 
/// </summary>

/// <summary>
/// This is the class implementing the AWS Interface Module
/// 
/// \ingroup globalModule
/// 
/// </summary>
namespace GantryStatusRegisters {
    using namespace System;
    using namespace System::Collections::Generic;

    delegate void delegate_void_callback(void); //!< This is the delegate of the function with a void as parameter();
    delegate void delegate_string_callback(String^ str); //!< This is the delegate of the function with a string as parameter();
    delegate void delegate_int_callback(int i); //!< This is the delegate of the function with an integer as parameter();
    delegate void delegate_activation_completed(unsigned short id, int error);//!< This is the delegate of the activation_completed_event();

    template <class T>
    ref class enumType {
    public:

        /// <summary>
        /// This is the constructor of the template class.
        /// 
        /// The constructor assignes the tags vector to the internal pointer
        /// and initializes the code to a UNDEF
        /// 
        /// The class template provides an event for the internal data change:
        /// + value_change_event();
        /// 
        /// </summary>
        /// <param name="tagsvar"></param>
        enumType(const array<String^>^% tagsvar) {
            array_tags = tagsvar;
            enum_code = T::UNDEF;
            list_code = gcnew List<T> {};
        }

        /// <summary>
        /// This event is generated when the value changes
        ///        
        /// </summary>
        event delegate_void_callback^ value_change_event;

        /// <summary>
        /// This event is generated if the list of the T elemnts changes
        /// 
        /// </summary>
        event delegate_void_callback^ lista_change_event;

        /// <summary>
        /// Returns the String tag from the enum code
        /// </summary>
        /// <param name="code"></param>
        /// <returns></returns>
        String^ ToTag(int code) {
            if (code >= (int)T::LEN) return array_tags[(int)T::UNDEF];
            return array_tags[(int)code];
        }

        /// <summary>
        /// Returns the option code of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the option code </returns>
        T getCode(void) { return enum_code; }

        /// <summary>
        /// Returns the option tag of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        String^ getTag(void) { return array_tags[(int)enum_code]; }

        /// <summary>
        /// This function verifies if  a string is part of the enumearation tags
        /// 
        /// </summary>
        /// <param name="str">String tag to be tested</param>
        /// <returns>
        /// + index of a tag [0:LEN];
        /// + -1 if the string doesn't belong to the enumerated tags</returns>
        int indexOf(String^ str) {
            for (int i = 0; i < (int)T::LEN; i++) {
                if (str == array_tags[i]) return i;
            }
            return -1;
        }

        /// <summary>
       /// This function set the register with the option code (and related tag).
       /// 
       /// The register is updated with the option code and related tag and, in case of value change,
       /// the status_change_event() is generated.
       /// 
       /// </summary>
       /// <param name="val">this is the option-code to be set</param>
       /// <returns>true if success or false if the code doesn't exist</returns>
        bool setCode(T val) {

            // If val is not supported into the enumeration, the UNDEF is used 
            if (val >= T::LEN) {
                if (enum_code != T::UNDEF) {
                    enum_code = T::UNDEF;
                    value_change_event();
                }
                return false;
            }

            // Rises an event in case of changes
            if (val != enum_code) {
                enum_code = val;
                value_change_event();
            }

            return true;
        }

        /// This function set the register with the option tag (and related code).
        /// 
        /// The register is updated with the tag and the related option code and, in case of value change,
        /// the status_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the tag to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        bool setCode(String^ val) {
            int i = indexOf(val);
            if (i == -1) {
                if (enum_code != T::UNDEF) {
                    enum_code = T::UNDEF;
                    value_change_event();
                }
                return false;
            }
            return setCode((T)i);
        }

        /// <summary>
        /// This function set the internal code to UNDEF
        /// 
        /// </summary>
        /// <param name=""></param>
        bool clearCode(void) {
            return setCode(T::UNDEF);
        }

        /// <summary>
        /// This function verifies if the string is part of the list of items T
        /// 
        /// </summary>
        /// <param name="s">String to be verified</param>
        /// <returns>true if the string is in the internal list</returns>
        bool isPresent(String^ s) {
            for (int i = 0; i < (int)list_code->Count; i++) {
                if (array_tags[(int)list_code[i]] == s) return true;
            }
            return false;
        }

        /// <summary>
        /// This function return the Tag string of an item  in the list
        /// </summary>
        /// <param name="i">index of the element</param>
        /// <returns>The Tag string of the item </returns>
        String^ getItem(unsigned char i) {
            if (i >= list_code->Count) return "";
            return array_tags[(int)list_code[i]];
        }

        /// <summary>
        /// This function returns the count elements of the internal list
        /// </summary>
        /// <param name=""></param>
        /// <returns></returns>
        int Count(void) { return list_code->Count; }


        /// <summary>
        /// This function set the list of the enumerations.
        /// 
        /// In case of list change, the list_change_event() is generated
        /// </summary>
        /// 
        /// <param name="lista">This is the list</param>
        /// <returns>true if the list is a valid list</returns>
        bool setList(List<String^>^ l) {
            if (l->Count >= (int)T::LEN) return false;

            // Checks the items in the list
            for (int i = 0; i < l->Count; i++) {
                if (indexOf(l[i]) < 0) return false;
            }

            // Updates the list
            list_code->Clear();
            for (int i = 0; i < l->Count; i++) {
                list_code->Add((T)indexOf(l[i]));
            }

            lista_change_event();
            return true;
        }

        void inline clearList(void) { list_code->Clear(); }

    private:
        T enum_code;
        List<T>^ list_code;
        array<String^>^ array_tags;
    };


    //static test<options>^ core = gcnew test<options> (tags);


    /// <summary>
    /// This class implements the Filter selection interface.
    /// 
    /// The class is declared by registers that need to store a filter status info.
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class FilterOptions {
    public:

        /// <summary>
        /// This is the enumeration option code 
        /// </summary>
        enum class options {
            FILTER_Ag = 0, //!< Ag filter seletion option
            FILTER_Al, //!< Al filter seletion option
            FILTER_Mo, //!< Mo filter seletion option
            FILTER_Rh, //!< Rh filter seletion option
            FILTER_Cu, //!< Cu filter seletion option
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^> { "Ag", "Al", "Mo", "Rh", "Cu", "UNDEF"}; //!< This is the tags array

        enumType<options>^ Value = gcnew enumType<options>(tags);
    };


    /// <summary>
   /// This class handles the option for the Collimation behavior during the exposure.
   /// 
   /// 
   /// \ingroup globalModule 
   /// </summary>
    ref class CollimationModeOption {
    public:

        /// <summary>
        /// This is the enumeration option code 
        /// </summary>
        enum class options {
            COLLI_AUTO = 0, //!< The Exposure uses the Automatic collimation set by the current detected Paddle
            COLLI_24x30, //!< Force the 24x30 format
            COLLI_18x24_C, //!< Force the 18x24 CENTER format
            COLLI_18x24_L, //!< Force the 18x24 LEFT format
            COLLI_18x24_R, //!< Force the 18x24 RIGHT format
            COLLI_9x21, //!< Force the 9x21(MAG) format
            COLLI_10x24, //!< Force the 10x24 format
            COLLI_PROSTHESIS, //!< Force the PROSTHESIS format
            COLLI_D75, //!< Force the D75(MAG) format
            COLLI_BIOP2D, //!< Force the BIOPSY 2D format
            COLLI_BIOP3D, //!< Force the BIOPSY STEREO format
            COLLI_TOMO, //!< Force the TOMO 24x30 format
            COLLI_9x9, //!< Force the TOMO 9x9(MAG) format
            COLLI_CUSTOM, //!< Force the custom format
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^>  { "COLLI_AUTO", "COLLI_24x30", "COLLI_18x24_C", "COLLI_18x24_L", \
            "COLLI_18x24_R", "COLLI_9x21", "COLLI_10x24", "COLLI_PROSTHESIS", \
            "COLLI_D75", "COLLI_BIOP2D", "COLLI_BIOP3D", "COLLI_TOMO", \
            "COLLI_9x9", "COLLI_CUSTOM", "UNDEF"}; //!< This is the option-tags static array


        enumType<options>^ Value = gcnew enumType<options>(tags);
    };

    /// <summary>
    /// This class handles the Compression behavior during the exposure.
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class CompressionModeOption {
    public:

        /// <summary>
        /// This is the enumeration option code 
        /// </summary>
        enum class options {
            CMP_KEEP = 0, //!< Keeps the compression after exposure;
            CMP_RELEASE,  //!< Releases the compression after exposure;
            CMP_DISABLE, //!< Disables the Compression check (for exposures without the compression);
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^>  { "CMP_KEEP", "CMP_RELEASE", "CMP_DISABLE", "UNDEF"}; //!< This is the option-tags static array
        enumType<options>^ Value = gcnew enumType<options>(tags);

    };

    /// <summary>
    /// This class handles the exposure type status.
    ///     
    /// \ingroup globalModule 
    /// </summary>
    ref class ExposureTypeOption {
    public:

        /// <summary>
        /// This is the enumeration option code 
        /// </summary>
        enum class options {
            MAN_2D = 0, //!< The next exposure is a 2D manual mode
            AEC_2D, //!< The next exposure is a 2D with AEC
            MAN_3D, //!< The next exposure is a Tomo 3D in manual mode
            AEC_3D, //!< The next exposure is a Tomo 3D with AEC
            MAN_COMBO, //!< The next exposure is a Manual Combo
            AEC_COMBO, //!< The next exposure is a Combo with AEC
            MAN_AE, //!< The next exposure is a Dual energy exposure in manual mode
            AEC_AE, //!< The next exposure is a Dual energy with AEC
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^>  { "MAN_2D", "AEC_2D", "MAN_3D", "AEC_3D", "MAN_COMBO", "AEC_COMBO", "MAN_AE", "AEC_AE", "UNDEF"}; //!< This is the option-tags static array
        enumType<options>^ Value = gcnew enumType<options>(tags);
    };

    /// <summary>
    /// This class handles the Detector model selection.
    /// 
    /// The Detector model influences the behavior of the exposure sequence
    /// due to possibly differences in the synch signals and or timing.
    /// \ingroup globalModule 
    /// </summary>
    ref class DetectorTypeOption {
    public:

        /// <summary>
        /// This is the enumeration option code 
        /// </summary>
        enum class options {
            LMAM2V2 = 0, //!< Analogic LMAM2 V2
            LMAM2FDIV2,  //!< Analogic LMAM2 FDI-V2            
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^>  { "LMAM2V2", "LMAM2FDIV2", "UNDEF"}; //!< This is the option-tags static array
        enumType<options>^ Value = gcnew enumType<options>(tags);
    };

    /// <summary>
    /// This class provides the Arm Mode options handling.
    /// 
    /// The Arm mode options enable/disable the check of the Arm position 
    /// in order to validate the exposure.
    /// 
    /// </summary>
    ref class ArmModeOption {
    public:
        /// <summary>
        /// This is the enumeration option code for the Arm Mode option        
        /// 
        /// </summary>
        enum class options {
            ARM_ENA = 0, //!< Enables the Angle range check during exposure;
            ARM_DIS, //!< Disables the Angle range check during the exposure;            
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^>  { "ARM_ENA", "ARM_DIS", "UNDEF"}; //!< This is the option-tags static array
        enumType<options>^ Value = gcnew enumType<options>(tags);
    };

    /// <summary>
    /// This class handles the Usage of the patient protection during the exposure.
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class PatientProtectionMode {
    public:

        /// <summary>
        /// This is the enumeration option code 
        /// </summary>
        enum class options {
            PROTECTION_ENA = 0, //!< Enables the Patient protection check;
            PROTECTION_DIS, //!< Disables the Patient protection check;            
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^>  { "PROTECTION_ENA", "PROTECTION_DIS", "UNDEF"}; //!< This is the option-tags static array

        /// <summary>
        /// This function tests if the use of the Patient protection is enabled
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>true if the use of the protection is enabled</returns>
        inline bool useProtection(void) { return Value->getCode() == options::PROTECTION_ENA; }
        enumType<options>^ Value = gcnew enumType<options>(tags);
    };

    /// <summary>
    /// This is the projection handling  class.
    /// 
    /// This class defines the availabe projection options,
    /// store the current selected projection and the projection selectable list.
    /// 
    /// The class provides events related to the data change 
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class ProjectionOptions {
    public:

        /// <summary>
        ///  This is the enumeration option code of the selectable projections.
        /// 
        /// </summary>
        enum class options {
            LCC = 0,
            LFB,
            LISO,
            LLM,
            LLMO,
            LML,
            LMLO,
            LSIO,
            RCC,
            RFB,
            RISO,
            RLM,
            RLMO,
            RML,
            RMLO,
            RSIO,
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^> { "LCC", "LFB", "LISO", "LLM", "LLMO", "LML", "LMLO", "LSIO", "RCC", "RFB", "RISO", "RLM", "RLMO", "RML", "RMLO", "RSIO", "UNDEF"};//!< This is the option-tags static array
        enumType<options>^ Value = gcnew enumType<options>(tags);

    };

    /// <summary>
    /// This register handles the Paddles identifiable by the compressor device
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class PaddleOption {
    public:

        enum class options {
            PAD_24x30 = 0,
            PAD_18x24_C,
            PAD_18x24_L,
            PAD_18x24_R,
            PAD_9x21,
            PAD_10x24,
            PAD_D75,
            PAD_BIOP2D,
            PAD_BIOP3D,
            PAD_TOMO,
            PAD_9x9,
            PAD_UNDETECTED,
            PAD_UNLOCKED,
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^> { "PAD_24x30", "PAD_18x24_C", "PAD_18x24_L", "PAD_18x24_R", "PADCOLLI_9x21", "PAD_10x24", "PAD_D75", "PAD_BIOP2D", "PAD_BIOP3D", "PAD_TOMO", "PAD_9x9", "PAD_UNDETECTED", "PAD_UNLOCKED", "PAD_UNDETECTED" };//!< This is the option-tags static array
        enumType<options>^ Value = gcnew enumType<options>(tags);
    };







    /// <summary>
    /// This class handles the Xray Push Button activation 
    /// status and activities
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class XrayPushButtonRegister {
    public:


        ///  Definition of the xray push button enable status
        enum class enable_options {
            XRAYPUSH_DISABLED = 0, //!< XRAY Push Button is enabled
            XRAYPUSH_ENABLED = 1,//!< XRAY Push Button is disabled
            LEN,
            UNDEF = LEN
        };

        ///  Definition of the xray push button current status
        enum class activation_options {
            XRAYPUSH_RELEASED = 0,//!< XRAY Push Button is released
            XRAYPUSH_PRESSED = 1, //!< XRAY Push Button is pressed
            LEN,
            UNDEF = LEN
        };

        static const array<String^>^ tags = gcnew array<String^>  {"OFF", "ON", "UNDEF"}; //!< Definition of the register tags

        static inline bool isEnabled(void) { return enableStatus->getCode() == enable_options::XRAYPUSH_ENABLED; }
        static inline bool isDisabled(void) { return enableStatus->getCode() != enable_options::XRAYPUSH_ENABLED; }
        static inline bool setEnable(void) { return enableStatus->setCode(enable_options::XRAYPUSH_ENABLED); }
        static inline bool setDisable(void) { return enableStatus->setCode(enable_options::XRAYPUSH_DISABLED); }
        static inline void setPressed(void) { activationStatus->setCode(activation_options::XRAYPUSH_PRESSED); }
        static inline void setReleased(void) { activationStatus->setCode(activation_options::XRAYPUSH_RELEASED); }

        static enumType<enable_options>^ enableStatus = gcnew enumType<enable_options>(tags);
        static enumType<activation_options>^ activationStatus = gcnew enumType<activation_options>(tags);

    };



    /// <summary>
    /// This class implements the Tomo configuration selection options.
    /// 
    /// The Tomomo configuration file is stored in the Disk.
    /// 
    /// There are several predefined configuration files 
    /// and in each of the configuration files there are a set of predefined 
    /// sequence description with its unit descriptor identifier.
    /// 
    /// The configuration file identifier is named confid;
    /// The sequnce descriptor is named seqid;
    /// The selection is achieved with the confid + seqid codes.
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class TomoConfigRegister {
    public:

        /// <summary>
        /// This is the enumeration option code for file configuration 
        /// </summary>
        enum class options_conf {
            TOMO_1F = 0, //!< Tomo configuration file 1F
            TOMO_2F, //!< Tomo configuration file 2F
            TOMO_3F, //!< Tomo configuration file 3F
            TOMO_4F, //!< Tomo configuration file 4F
            TOMO_5F, //!< Tomo configuration file 5F
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags_conf = gcnew array<String^>  { "TOMO_1F", "TOMO_2F", "TOMO_3F", "TOMO_4F", "TOMO_5F", "UNDEF" }; //!< This is the config id option-tags static array

        /// <summary>
        /// This is the enumeration option code for the particolar sequence in the configuration file  
        /// </summary>
        enum class options_seq {
            NARROW = 0,         //!< Narrow sequence
            INTERMEDIATE,   //!< Intermediate sequence
            WIDE,           //!< Wide seqeunce
            SUPERWIDE,      //!< Super-wide sequence
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags_seq = gcnew array<String^>  { "N", "I", "W", "SW", "UNDEF" }; //!< This is the sequence option-tags static array



        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: TomoConfigRegister::selection_change_event += gcnew delegate_void_callback(&some_class, some_class::func)
        /// </summary>
        static event delegate_void_callback^ selection_change_event;

        /// <summary>
        /// This function selects a Tomo configuration sequence 
        /// based on the confid and seqid identifiers.
        /// 
        /// If the selection changes, the selection_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="conftag">this is the configuration file identifier tag</param>
        /// <param name="seqtag">this is the sequence identifier tag</param>
        /// <returns> tue if the confid + seqid exists</returns>
        static bool select(String^ conftag, String^ seqtag) {
            int i;

            options_conf curconf = confid;
            options_seq curseq = seqid;

            for (i = 0; i < (int)options_conf::LEN; i++) {
                if (conftag == tags_conf[i]) {
                    confid = (options_conf)i;
                    break;
                }
            }
            if (i == (int)options_conf::LEN) {
                confid = options_conf::UNDEF;
                seqid = options_seq::UNDEF;
                return false;
            }

            for (i = 0; i < (int)options_seq::LEN; i++) {
                if (seqtag == tags_seq[i]) {
                    seqid = (options_seq)i;

                    // In case of change, calls the delegate
                    if ((confid != curconf) || (seqid != curseq))
                        selection_change_event();

                    return true;
                }
            }

            confid = options_conf::UNDEF;
            seqid = options_seq::UNDEF;
            return false;

        }

        /// <summary>
        /// This function returns the Tomo Configuration file name
        /// </summary>
        /// <param name=""></param>
        /// <returns>Tomo Configuration file name</returns>
        static inline String^ getConfig(void) { return tags_conf[(int)confid]; }

        /// <summary>
        /// This function returns the Tomo Sequence name
        /// </summary>
        /// <param name=""></param>
        /// <returns>Tomo Sequence name</returns>
        static inline String^ getSequence(void) { return tags_seq[(int)seqid]; }

        /// <summary>
        /// This function returns true if the Tomo configuration has been selected
        /// </summary>
        /// <param name=""></param>
        /// <returns>true if a valid configuration is selected</returns>
        static inline bool isValid(void) { return ((confid != options_conf::UNDEF) && (seqid != options_seq::UNDEF)); }

        /// <summary>
        /// This function returns the Home position of a selected configuration
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>Tomo home position in 0.01°</returns>
        static inline int getTomoHome(void) { return tomo_home; }

        /// <summary>
       /// This function returns the End position of a selected configuration
       /// 
       /// </summary>
       /// <param name=""></param>
       /// <returns>Tomo End position in 0.01°</returns>
        static inline int getTomoEnd(void) { return tomo_end; }

        /// <summary>
       /// This function returns the Acceleration of a selected Tomo configuration
       /// 
       /// </summary>
       /// <param name=""></param>
       /// <returns>Acceleration in 0.01°/ss </returns>
        static inline int getTomoAcc(void) { return tomo_acc; }

        /// <summary>
      /// This function returns the Deceleration of a selected Tomo configuration
      /// 
      /// </summary>
      /// <param name=""></param>
      /// <returns>Deceleration in 0.01°/ss </returns>
        static inline int getTomoDec(void) { return tomo_dec; }

        /// <summary>
      /// This function returns the Speed of a selected Tomo configuration
      /// 
      /// </summary>
      /// <param name=""></param>
      /// <returns>Speed in 0.01°/s </returns>
        static inline int getTomoRun(void) { return tomo_run; }

        /// <summary>
        /// This function returns the number of valid sample of a selected Tomo configuration
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>Number of sequence samples </returns>
        static inline int getTomoSmp(void) { return tomo_smp; }

        /// <summary>
        /// This function returns the number of initial skips of a selected Tomo configuration
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>Number of initial skips </returns>
        static inline int getTomoSkp(void) { return tomo_skp; }


    private:
        static options_conf confid = options_conf::UNDEF; //!< This is the register conf_id 
        static options_seq seqid = options_seq::UNDEF; //!< This is the register sequence_id 
        static int tomo_home;//!< This is the Home position in 0.01°
        static int tomo_end;//!< This is the Tomo End position in 0.01°
        static int tomo_acc;//!< This is the acceleration in 0.01°/ss
        static int tomo_dec;//!< This is the deceleration in 0.01°/ss
        static int tomo_run;//!< This is the speed in 0.01°/s
        static int tomo_smp;//!<  This is the number of valid samples
        static int tomo_skp;//!<  This is the number of initial skip pulses
    };

    /// <summary>
    /// This is the Gantry operating status register definition.
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class OperatingStatusRegister {
    public:

        /// <summary>
        /// This is the enumeration option code 
        /// </summary>
        enum class options {
            GANTRY_STARTUP = 0, //!< Gantry is in the Startup operating mode
            GANTRY_IDLE,        //!< Gantry is in the Idle operating mode
            GANTRY_OPEN_STUDY,  //!< Gantry is in the Open-Study operating mode
            GANTRY_SERVICE,     //!< Gantry is in the Service operating mode
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^>   { "GANTRY_STARTUP", "GANTRY_IDLE", "GANTRY_OPEN_STUDY", "GANTRY_SERVICE", "UNDEF" };//!< This is the option-tags static array


        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: OperatingStatusRegister::operating_status_change_event += gcnew delegate_void_callback(&some_class, some_class::func)
        /// </summary>
        static event delegate_void_callback^ operating_status_change_event;


        static String^ getPatientName(void) { return patientName; }

        static bool setOpenStudy(String^ patient_name) {
            if (operating_status != options::GANTRY_IDLE) return false;
            operating_status = options::GANTRY_OPEN_STUDY;
            patientName = patient_name;
            operating_status_change_event();
            return true;
        }

        static bool setCloseStudy(void) {
            if (operating_status != options::GANTRY_OPEN_STUDY) return false;
            operating_status = options::GANTRY_IDLE;
            patientName = "";
            operating_status_change_event();
            return true;
        }

        static bool setService(void) {
            if (operating_status != options::GANTRY_IDLE) return false;
            operating_status = options::GANTRY_SERVICE;
            operating_status_change_event();
            return true;
        }

        static bool setIdle(void) {
            if (operating_status != options::GANTRY_SERVICE) return false;
            operating_status = options::GANTRY_IDLE;
            operating_status_change_event();
            return true;
        }

        static bool setStartup(void) {
            if (operating_status != options::GANTRY_STARTUP) return false;
            operating_status = options::GANTRY_STARTUP;
            operating_status_change_event();
            return true;
        }


        static String^ getStatus(void) { return tags[(int)operating_status]; }
        static bool isSERVICE(void) { return (operating_status == options::GANTRY_SERVICE); }
        static bool isIDLE(void) { return (operating_status == options::GANTRY_IDLE); }
        static bool isOPEN(void) { return (operating_status == options::GANTRY_OPEN_STUDY); }
        static bool isCLOSE(void) { return isIDLE(); }

    private:
        static options operating_status = options::GANTRY_STARTUP; //!< This is the register option-code 
        static String^ patientName = "";
    };



    /// <summary>
    /// This register handle the Component that can be used during the exposure
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class ComponentRegister {
    public:

        enum class options {
            COMPONENT_PROTECTION_3D = 0,
            COMPONENT_MAG15,
            COMPONENT_MAG18,
            COMPONENT_MAG20,
            COMPONENT_BIOPSY,
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^> { "PROTECTION_3D", "MAG_15", "MAG_18", "MAG_20", "BIOPSY", "COMPONENT_UNDETECTED"};//!< This is the option-tags static array        
        static enumType<options>^ Value = gcnew enumType<options>(tags);
    };

    /// <summary>
    /// This register handles the component applicable to the collimator
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class CollimatorComponentRegister {
    public:

        enum class options {
            COLLI_COMPONENT_PROTECTION_2D = 0,
            COLLI_COMPONENT_PROTECTION_SHIFTED,
            COLLI_COMPONENT_LEAD_SCREEN,
            COLLI_COMPONENT_SPECIMEN,
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^> { "PROTECTION_2D", "SHIFTED_PROTECTION", "LEAD_SCREEN", "SPECIMEN", "COLLI_COMPONENT_UNDETECTED" };//!< This is the option-tags static array
        static enumType<options>^ Value = gcnew enumType<options>(tags);

    };



    /// <summary>
    /// This is the register handling the compression force and thickness.
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class CompressorRegister {
    public:


        /// <summary>
        /// This event is generated when the force changes
        /// 
        /// </summary>
        static event delegate_void_callback^ force_change_event;

        /// <summary>
        /// This event is generated when the thickness changes
        /// 
        /// </summary>
        static event delegate_void_callback^ thickness_change_event;

        /// <summary>
        /// This function returns the current breast compression Force
        /// </summary>
        /// <param name=""></param>
        /// <returns>The breast compression force in Newtons</returns>
        static unsigned short getForce(void) {
            return compression_force;
        }

        /// <summary>
        /// This function returns the breast compression Thickness.
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>The compressed breast thickness in mm</returns>
        static unsigned short getThickness(void) {
            if (compression_force) return breast_thickness;
            else return 0;
        }

        /// <summary>
        /// This function updates the current compression Force.
        /// 
        /// If the force is changed then the compressor_change_event() is generated.
        /// </summary>
        /// <param name="force"></param>
        static void setForce(unsigned short force) {
            if (compression_force != force) {
                compression_force = force;
                force_change_event();
            }
            return;
        }

        /// <summary>
        /// This function updates the current compression Thickness.
        /// 
        /// If the Thickness is changed then the compressor_change_event() is generated.
        /// </summary>
        /// <param name="thick"></param>
        static void setThickness(unsigned short thick) {
            if (breast_thickness != thick) {
                breast_thickness = thick;
                thickness_change_event();
            }
            return;
        }

        static inline PaddleOption^ getPaddle(void) { return paddle; }

    private:
        static unsigned short breast_thickness = 0;  //!< Compressed breast thickness in mm
        static unsigned short compression_force = 0; //!< Compression force in N
        static PaddleOption^ paddle = gcnew PaddleOption;

    };


    /// <summary>
    /// This is the Error handling register
    /// \ingroup globalModule 
    ///
    /// Work in Progress .....
    /// </summary>
    ref class ErrorRegister {
    public:
        static bool isError(void) { return error_status; }

    private:
        static bool        error_status = false;
        static array<int>^ error_code_list = gcnew array<int> {};
        static array<String^>^ error_string_list = gcnew array<String^> {""};
    };





    /// <summary>
    /// This class handles the exposure type status.
    /// 
    /// The Exposure type status determines what kind of exposure 
    /// will take place. 
    /// \ingroup globalModule 
    /// </summary>
    ref class ExposureModeRegister {
    public:

        static ExposureTypeOption^ exposureType = gcnew ExposureTypeOption;
        static CollimationModeOption^ collimationMode = gcnew CollimationModeOption;
        static ArmModeOption^ armMode = gcnew ArmModeOption;
        static CompressionModeOption^ compressorMode = gcnew CompressionModeOption;
        static PatientProtectionMode^ protectionMode = gcnew PatientProtectionMode;
        static DetectorTypeOption^ detector = gcnew DetectorTypeOption;
    };





    /// <summary>
    /// This is the status register of the Arm motorization.
    /// 
    /// There are several features that this register allow to handle:
    /// - Arm Angle verification option: enables or disables the test on the angle validity during exposure;
    /// - Arm target angle and projection setting: allow to handles the arm positioning
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class ArmStatusRegister {
    public:


        /// <summary>
        /// This event is generated whenever a projection request command is called.
        /// 
        /// Usage: ArmStatusRegister::projection_request_event += gcnew delegate_string_callback(&some_class, some_class::func)
        /// </summary>        
        static event delegate_string_callback^ projection_request_event;

        /// <summary>
        /// This function generate the projection_request_event() event
        /// </summary>
        /// <param name="projection">Tag of the requested projection</param>
        static void projectionRequest(String^ projection) {

            // Checks if the projection actually is in the available list
            if (!projections->Value->isPresent(projection)) return;
            projection_request_event(projection);
        }



        /// <summary>
        /// This event is generated whenever a projection request command is called.
        /// 
        /// Usage: ProjectionRegister::abort_projection_request_event += gcnew delegate_void_callback(&some_class, some_class::func)
        /// </summary>        
        static event delegate_void_callback^ abort_projection_request_event;

        /// <summary>
        /// This function generate the projection_request_event() event
        /// </summary>
        /// <param name="projection">Tag of the requested projection</param>

        /// <summary>
        /// This function generates the abort_projection_request_event() if a projection 
        /// is currently valid
        /// 
        /// </summary>
        /// <param name=""></param>
        static void abortProjectionRequest(void) {

            // Checks if the Arm data are valid or not. Only for valid data is alowed to request an abort projection
            if (!isValid()) return;
            abort_projection_request_event();
        }


        /// <summary>
        /// This event is generated whenver the arm target is changed.
        /// 
        /// Usage: ArmStatusRegister::target_change_event += gcnew delegate_void_callback(&some_class, some_class::func)
        /// </summary>
        static event delegate_void_callback^ target_change_event;


        /// <summary>
        /// This event is generated whenver the arm target parameters are validated.
        /// 
        /// Usage: ArmStatusRegister::validate_change_event += gcnew delegate_void_callback(&some_class, some_class::func)
        /// </summary>
        static event delegate_void_callback^ validate_change_event;

        /// <summary>
        /// This method validates or invalidates the Arm parameters.
        /// 
        /// 
        /// When the validation status changes, the validate_change_event() is generated.
        /// 
        /// 
        /// </summary>
        /// <param name="stat"></param>
        static void validate(bool stat) {

            if (position_validated != stat) {
                position_validated = stat;
                validate_change_event();
            }

            projections->Value->clearCode();
        }

        /// <summary>
        /// This funtion returns the Arm activation status.
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>true if the Arm is executing a command</returns>
        static bool isBusy(void) { return executing; }

        /// <summary>
        /// This funtion returns the current data valid flag.
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>true if the Arm data are valid</returns>
        static bool isValid(void) { return position_validated; }

        /// <summary>
        /// This function set the new target for the Arm position.
        /// 
        /// If the target should change the event validate_change_event()
        /// and the event target_change_event()  are generated.
        /// 
        /// The target is automatically validated!
        /// 
        /// </summary>
        /// <param name="pos">target position in (°)</param>
        /// <param name="low">lower acceptable range (°)</param>
        /// <param name="high">higher acceptable range (°)</param>
        /// <param name="id">identifier of the AWS command</param>
        /// <param name="proj"> This is the projection name assigned to the target</param>
        /// <returns>true if the target is validated</returns>
        static bool setTarget(int pos, int low, int high, String^ proj, int id) {
            if (executing) return false;
            if (pos > 180) return false;
            if (pos < -180) return false;
            if ((pos > high) || (pos < low)) return false;
            if (projections->Value->indexOf(proj) < 0) return false;

            // Assignes the projection
            projections->Value->setCode(proj);

            // Assignes the target data
            position_validated = true;
            position_target = pos;
            allowed_low = low;
            allowed_high = high;

            // Verifies if the target is changed
            if (pos == current_angle) {
                executing_id = 0;
                return true;
            }

            // If the target is changed, a Arm activation command should be invoked
            executing_id = id;
            executing = true;

            validate_change_event();
            target_change_event();
            return true;
        }


        /// <summary>
        /// This event is generated whenver the arm position is updated.
        /// This is not the activation completion event!
        /// 
        /// Usage: ArmStatusRegister::position_change_event += gcnew delegate_void_callback(&some_class, some_class::func)
        /// </summary>
        static event delegate_void_callback^ position_change_event;

        /// <summary>
        /// This function updates the current angle value.
        /// 
        /// If the angle changes, the position_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="angle">This is the new angle in (°)</param>
        static void  updateCurrentPosition(int angle) {
            if (current_angle != angle) {
                current_angle = angle;
                position_change_event();
            }
        }

        /// <summary>
        /// This event is generated whenver the arm completes an activation!
        /// 
        /// Usage: ArmStatusRegister::activation_completed_event += gcnew delegate_activation_completed(&some_class, some_class::func)
        /// </summary>
        static event delegate_activation_completed^ activation_completed_event;

        /// <summary>
        /// This function shall be called when the activation termines
        /// 
        /// 
        /// </summary>
        /// <param name="angle">THis is the final angle</param>
        /// <param name="error">This is an error code</param>
        static void  activationCompleted(int angle, int error) {
            if (!executing) return;
            executing = false;

            unsigned short  id = executing_id;
            executing_id = 0;

            updateCurrentPosition(angle);
            activation_completed_event(id, error);

        }


        /// <summary>
        /// This function returns the current ARM angle, even if the command is not yet terminated.
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>The current angle (°) units</returns>
        static int getAngle(void) {
            return current_angle;
        }

        /// <summary>
        /// This function returns the current ARM target
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>The current target (°) units</returns>
        static int getTarget(void) {
            return position_target;
        }

        /// <summary>
        /// This function returns the current ARM low target
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>The current low target (°) units</returns>
        static int getLow(void) {
            return allowed_low;
        }

        /// <summary>
        /// This function returns the current ARM high target
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>The current high target (°) units</returns>
        static int getHigh(void) {
            return allowed_high;
        }

        /// <summary>
        /// Returns the current selected projection
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns></returns>
        static String^ getProjection(void) {
            return projections->Value->getTag();
        }

        /// <summary>
        /// This function verifies if the current angle is into the expected acceptable range
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>true if the angle is acceptable</returns>
        static bool isValidAngle(void) {
            if (!position_validated) return false;
            if (current_angle > allowed_high) return false;
            if (current_angle < allowed_low) return false;
            if (executing) return false;

            return true;
        }
        static inline ProjectionOptions^ getProjections() { return projections; }

    private:


        static bool         position_validated = false; //!< Validation status
        static int          position_target = 0;        //!< Validated target position
        static int          current_angle;              //!< Current Arm position

        static int          allowed_low = 0;            //!< Lower acceptable angle (°)
        static int          allowed_high = 0;           //!< Higher acceptable angle (°)
        static bool         executing = false;          //!< Command is in execution
        static unsigned short  executing_id = 0;        //!< AWS command Id
        static ProjectionOptions^ projections = gcnew ProjectionOptions;  //!< This is the current selected projection


    };


    /// <summary>
    /// This class handles the Trx motorization
    /// 
    /// The Trx can be activate only in discrete positions
    /// \ingroup globalModule 
    /// </summary>
    ref class TrxStatusRegister {
    public:

        /// <summary>
        /// This is the enumeration of the possible Trx target positions
        /// </summary>
        enum class target_options {
            TRX_SCOUT = 0, //!< TRX in scout position
            TRX_BP_R,       //!< TRX in Biopsy Right position
            TRX_BP_L,       //!< TRX in Biopsy Left position
            TRX_TOMO_H,     //!< TRX in Tomo Home position
            TRX_TOMO_E,     //!< TRX in Tomo End position       
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^> { "SCOUT", "BP_R", "BP_L", "TOMO_H", "TOMO_E", "TRX_UNDEF"}; //!< This is the tags array
        static array<int>^ targets = gcnew array<int> {0, 1500, -1500, 2700, -2700, 0 }; //!< This is the current value of the target angles


        /// <summary>
        /// This event is generated whenver the trx target is changed.
        /// 
        /// Usage: TrxStatusRegister::target_change_event += gcnew delegate void delegate_void_callback(unsigned short id, int error);//!< This is the delegate of the activation_completed_event();(&some_class, some_class::func)
        /// </summary>
        static event delegate_void_callback^ target_change_event;

        /// <summary>
        /// This funtion returns the Trx activation status.
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>true if the Trx is executing a command</returns>
        static bool isBusy(void) { return executing; }

        /// <summary>
        /// This function sets a new target for the Trx.
        /// 
        /// If the target_angle-current_angle exceeds sensitivity, the target_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="tg">this is the target option code</param>
        /// <param name="id">this is the aws command identifier</param>
        /// <returns>true if the target is successfully set</returns>
        static bool setTarget(target_options tg, int id) {
            if (executing) return false;
            if (tg >= target_options::LEN) return false;

            switch (tg) {
            case target_options::TRX_SCOUT:
                target_angle = 0;
                break;
            case target_options::TRX_BP_R:
                target_angle = 1500;
                break;
            case target_options::TRX_BP_L:
                target_angle = -1500;
                break;
            case target_options::TRX_TOMO_H:
                if (!TomoConfigRegister::isValid()) return false;
                target_angle = TomoConfigRegister::getTomoHome();
                break;
            case target_options::TRX_TOMO_E:
                if (!TomoConfigRegister::isValid()) return false;
                target_angle = TomoConfigRegister::getTomoEnd();
                break;

            }

            if (abs(current_angle - target_angle) >= sensitivity) {
                executing_id = id;
                executing = true;
                target_change_event();
            }

            return true;
        }

        /// <summary>
        /// This function sets a new target for the Trx.
        /// 
        /// If the target_angle-current_angle exceeds sensitivity, the target_change_event() is generated.
        /// 
        /// 
        /// </summary>
        /// <param name="tag">Trx Target Tag</param>
        /// <param name="id">this is the aws command identifier</param>
        /// <returns>true if the target is successfully set</returns>
        static bool setTarget(String^ tag, int id) {
            if (executing) return false;

            for (int i = 0; i < (int)target_options::LEN; i++) {
                if (tags[i] == tag) {
                    return setTarget((target_options)i, id);
                }
            }
            return false;
        }



        /// <summary>
        /// This event is generated whenver the trx position is updated.
        /// This is not the activation completion event!
        /// 
        /// Usage: TrxStatusRegister::position_change_event += gcnew delegate_void_callback(&some_class, some_class::func)
        /// </summary>
        static event delegate_void_callback^ position_change_event;

        /// <summary>
        /// This function updates the current angle value.
        /// 
        /// If the variation of the angle with respect the last value updated with position_change_event() is greater than sensitivity,
        /// than a new position_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="angle"></param>
        /// <param name="sensitivity"></param>
        static void  updateCurrentPosition(int angle) {
            static int last_val_signaled = 0;
            current_angle = angle;
            if (abs(last_val_signaled - angle) >= sensitivity) {
                last_val_signaled = angle;
                position_change_event();
            }
        }

        /// <summary>
        /// This function returns the target TRX angle
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>The current angle in 0.01° units</returns>
        static int getTargetAngle(void) {
            return target_angle;
        }

        /// <summary>
        /// This function returns the code related to the angle 
        /// based on the targets currently set.
        /// 
        /// </summary>
        /// <param name="angle">This is the requested angle</param>
        /// <returns>The target option corresponding to the target angle angle</returns>
        static target_options getTargetCode(void) {

            for (int i = 0; i < (int)target_options::LEN; i++) {
                if (abs(target_angle - targets[i]) < sensitivity) {
                    return (target_options)i;
                }
            }
            return target_options::UNDEF;
        }

        /// <summary>
        /// This function returns the current target tag in term of target symbols not angle.
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>The target Tag </returns>
        static String^ getTargetTag(void) {
            return tags[(int)getTargetCode()];
        }

        /// <summary>
        /// This function returns the current TRX angle
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>The current angle in 0.01° units</returns>
        static int getCurrentAngle(void) {
            return current_angle;
        }

        /// <summary>
         /// This function returns the code related to the angle 
         /// based on the targets currently set.
         /// 
         /// </summary>
         /// <param name="angle">This is the requested angle</param>
         /// <returns>The target corresponding to the angle</returns>
        static target_options getCurrentCode(void) {

            for (int i = 0; i < (int)target_options::LEN; i++) {
                if (abs(current_angle - targets[i]) < sensitivity) {
                    return (target_options)i;
                }
            }
            return target_options::UNDEF;
        }

        /// <summary>
        /// This function returns the current position tag in term of target symbols not angle.
        /// 
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>The current position </returns>
        static String^ getCurrentTag(void) {
            return tags[(int)getCurrentCode()];
        }



        /// <summary>
        /// This event is generated whenver the trx completes an activation!
        /// 
        /// Usage: TrxStatusRegister::activation_completed_event += gcnew delegate_activation_completed(&some_class, some_class::func)
        /// </summary>
        static event delegate_activation_completed^ activation_completed_event;

        /// <summary>
        /// This function shall be called when the activation termines
        /// 
        /// 
        /// </summary>
        /// <param name="angle">THis is the final angle</param>
        /// <param name="error">This is an error code</param>
        static void  activationCompleted(int angle, int error) {
            if (!executing) return;
            executing = false;

            unsigned short  id = executing_id;
            executing_id = 0;

            current_angle = angle;
            position_change_event();
            activation_completed_event(id, error);

        }
    private:
        static int      target_angle = 0;    //!< This is the Trx target position in 0.01° units          
        static int      current_angle = 0;  //! The Trx angle in 0.01° units
        static bool     executing = false;                //!< Command is in execution
        static unsigned short  executing_id = 0;              //!< AWS command Id
        static int sensitivity = 50; //!< Sets the maximum difference from current_ange and position_target 
    };



    /// <summary>
    /// Internal class uses to group the exposure data pulses
    /// 
    /// </summary>
    ref class exposurePulse {
    public:


        /// <summary>
        /// This function sets the pulse exposure data content
        /// 
        /// 
        /// </summary>
        /// <param name="filter_tag"> This is the tag of the filter used in the next exposure pulse</param>
        /// <param name="kV">kV value of the next exposure pulse [20:640] </param>
        /// <param name="mAs">mAs value of the next exposure pulse [0:640] </param>
        /// <returns>true if data are accepted</returns>
        bool set(double kv, double mas, String^ filter_tag) {
            if ((kv > 49.0) || (kv < 20.0)) return false;
            if ((mas > 640) || (mas < 0)) return false;

            // The pulse shall be consumed before to reuse it.
            if (validated) return false;

            // Assignes the filter
            if (!filter->Value->setCode(filter_tag)) return false;

            kV = kv;// Assignes the kV
            mAs = mas;// Assignes the mAs
            validated = true;
            return true;
        }

        /// <summary>
        /// This function sets the exposure data of a given exposure pulse number.
        /// 
        /// There are a maximum of 4 possible exposure pulses.
        /// 
        /// </summary>
        /// <param name="filter_code"> This is the code of the filter used in the next exposure pulse</param>
        /// <param name="kV">kV value of the next exposure pulse [20:640] </param>
        /// <param name="mAs">mAs value of the next exposure pulse [0:640] </param>
        /// <returns>true if data are accepted</returns>
        bool set(double kV, double mAs, FilterOptions::options filter_code) {
            return set(kV, mAs, filter->Value->ToTag((int)filter_code));
        }

        /// <summary>
        /// This function invalidate the usage of exposure data
        /// </summary>
        /// <param name=""></param>
        inline void invalidate(void) { validated = false; }


        /// <summary>
        /// This is the creator of the pulse data class.
        /// </summary>
        /// <param name=""></param>
        exposurePulse(void) {


            // The handle of the filter selection is created
            filter = gcnew FilterOptions;

            // When created the pulse is invalidated for safety
            validated = false;
        };

        /// <summary>
        /// Returns the Pulse data Validation status
        /// </summary>
        /// <param name=""></param>
        /// <returns>true if data are valid</returns>
        inline bool isValid(void) { return validated; }

        /// <summary>
        /// Returns the kV of the pulse
        /// </summary>
        /// <returns>kV value</returns>
        inline double getKv() { return kV; }

        /// <summary>
        /// Returns the mAs of the pulse
        /// </summary>
        /// <returns>mAs value</returns>
        inline double getmAs() { return mAs; }

        /// <summary>
        /// Returnrn the filter option of the pulse
        /// </summary>
        /// <returns>The filter option</returns>
        inline FilterOptions^ getFilter() { return filter; }

    private:
        FilterOptions^ filter; //!< This is the assigned filter handle
        double kV; //!< This is the selected kV value
        double mAs; //!< This is the selected mAs value
        bool   validated; //!< This is the flag that validate the use of thhis pulse in a sequence
    };

    /// <summary>
    /// This class implements the XRAY completed status register.
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class ExposureCompletedOptions {
    public:


        ///  Definition of the register enumeration codes        
        enum class options {
            XRAY_COMPLETED_OK = 0, //!< XRAY sequence successfully completed
            XRAY_COMPLETED_PARTIAL_DOSE = 1,//!< XRAY sequence partially completed
            XRAY_COMPLETED_NO_DOSE = 2,//!< XRAY sequence aborted without dose
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^>  {"OK", "PARTIAL", "NOK", "UNDEF"}; //!< Definition of the register tags        
        enumType<options>^ Value = gcnew enumType<options>(tags);

        static array<exposurePulse^>^ exposed_pulses = nullptr;
    };

    /// <summary>
    /// This register handle the Exposure pulse data info.
    /// 
    /// The Exposure data of the exposure sequence is stored into this register.
    /// \ingroup globalModule  
    /// </summary>
    ref class ExposureDataRegister {
    public:

        /// <summary>
        /// This event is generated when the value changes
        ///        
        /// </summary>
        static event delegate_void_callback^ exposure_completed_event;

        static inline ExposureCompletedOptions^ getExposureComplete(void) { return exposure_complete; }
        static inline exposurePulse^ getPulse(int i) { if (i < pulses->Length) return pulses[i]; else return nullptr; }
        static inline void setExposurePartial(ExposureCompletedOptions::options, double kV, double mAs, FilterOptions::options filter_code) {
            exposure_complete->exposed_pulses = pulses;
            for (int i = 0; i < 4; i++) {
                if (exposure_complete->exposed_pulses[3 - i]->isValid()) {
                    exposure_complete->exposed_pulses[3 - i]->set(kV, mAs, filter_code);
                    break;
                }
            }
            clear();
            exposure_complete->Value->setCode(ExposureCompletedOptions::options::XRAY_COMPLETED_PARTIAL_DOSE);
            exposure_completed_event();
        }
        static inline void setExposureCompleted(void) {
            exposure_complete->exposed_pulses = pulses;
            clear();
            exposure_complete->Value->setCode(ExposureCompletedOptions::options::XRAY_COMPLETED_OK);
            exposure_completed_event();

        }
        static inline void setExposureNoDose(void) {
            exposure_complete->exposed_pulses = nullptr;
            clear();
            exposure_complete->Value->setCode(ExposureCompletedOptions::options::XRAY_COMPLETED_NO_DOSE);
            exposure_completed_event();
        }


        static inline void clear(void) {
            pulses = gcnew array<exposurePulse^>{gcnew exposurePulse(), gcnew exposurePulse(), gcnew exposurePulse(), gcnew exposurePulse()};
        }

    private:
        static array<exposurePulse^>^ pulses = gcnew array<exposurePulse^>{gcnew exposurePulse(), gcnew exposurePulse(), gcnew exposurePulse(), gcnew exposurePulse()};//!< Stores the array of data pulses
        static ExposureCompletedOptions^ exposure_complete = gcnew ExposureCompletedOptions;
    };

    /// <summary>
    /// This class handles the Xray Tybe data and setting.
    /// 
    /// \ingroup globalModule  
    /// </summary>
    ref class TubeDataRegister {
    public:

        /// <summary>
        /// This function returns the cumulated anode Hu
        /// </summary>
        /// <param name=""></param>
        /// <returns>Anode Hu in % respect the maximum</returns>
        static unsigned char getAnode(void) { return anodeHu; }

        /// <summary>
        /// This function returns the cumulated bulb's heat
        /// </summary>
        /// <param name=""></param>
        /// <returns>Bulb's heat in % respect the maximum</returns>
        static unsigned char getBulb(void) { return bulbHeat; }

        /// <summary>
        /// This function returns the cumulated stator's heat
        /// </summary>
        /// <param name=""></param>
        /// <returns>Stator's heat in % respect the maximum</returns>
        static unsigned char getStator(void) { return statorHeat; }

        /// <summary>
        /// This function returns the available number of exposure 
        /// that it ishould be possible before to stop for temperature issues
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>Number of available exposures</returns>
        static unsigned short getExposures(void) { return availableExposure; }


        static unsigned char  anodeHu = 0;     //!< Cumulated Anode HU %
        static unsigned char  bulbHeat = 0;    //!< Cumulated Bulb Heat %
        static unsigned char  statorHeat = 0;  //!< Cumulated Stator Heat %
        static unsigned short availableExposure = 0xFFFF; //!< Estimated number of available exposure 
    };


    /// <summary>
    /// This class handles the Interface language
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class LanguageRegister {
    public:

        /// <summary>
        /// This is the enumeration option code 
        /// </summary>
        enum class options {
            LNG_ITA = 0,        //!< Italian Language
            LNG_FRA,        //!< French Language
            LNG_ENG,        //!< English Language
            LNG_PRT,        //!< Portuguese 
            LNG_RUS,        //!< Russian Language
            LNG_ESP,        //!< Espanol Language
            LNG_LTU,        //!< Lituanian Language
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^>  { "ITA", "FRA", "ENG", "PRT", "RUS", "ESP", "LTU", "UNDEF"}; //!< This is the option-tags static array
        static enumType<options>^ Value = gcnew enumType<options>(tags);
    };

    /// <summary>
    /// This class register handles the REady for exposure conditions.
    /// 
    /// 
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class ReadyForExposureRegister {
    public:

        ///  Definition of the not-ready conditions
        enum class options {
            READY_FOR_EXPOSURE = 0, //!< The Gantry is ready to activate an exposure
            SYSTEM_ERROR = 0x1,         //!< The Gantry is in error condition
            MISSING_COMPRESSION = 0x2,  //!< The compression force is not detected
            MISSING_PATIENT_PROTECTION = 0x4,  //!< The Patient protection is missing
            WRONG_ARM_POSITION = 0x8, //!< The Arm position is out of the expected range
            WRONG_PADDLE_DETECTED = 0x10, //!< The Paddle is not the expected for the exposure mode
            MISSING_VALID_EXPOSURE_MODE = 0x20, //!< The Exposure mode is not selected
            MISSING_VALID_EXPOSURE_DATA = 0x40, //!< The Exposure data are not present
            XRAY_PUSHBUTTON_DISABLED = 0x80, //!< The Xray push button is not enabled
            STUDY_CLOSED = 0x100, //!< The study is closed
            NOT_READY_FOR_EXPOSURE = 0x8000 //!< Used to initialize the status  to a non ready condition
        };


        /// <summary>
        /// This event is generated whenver the ready for exposure changes its status
        /// 
        /// Usage: ReadyForExposureRegister::ready_change_event += gcnew delegate_void_callback(&some_class, some_class::func)
        /// </summary>
        static event delegate_void_callback^ ready_change_event;

        /// <summary>
   /// This event is generated whenver the ready for exposure changes its status
   /// 
   /// Usage: ReadyForExposureRegister::start_exposure_event += gcnew delegate_void_callback(&some_class, some_class::func)
   /// </summary>
        static event delegate_void_callback^ start_exposure_event;

        /// <summary>
        /// This function requests for the start exposure.
        /// If the system is in ready for exposure, the event start_exposure_event() is generated.
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>true if the system is in ready for exposure</returns>
        static bool requestStartExposure(void) {
            if (code != options::READY_FOR_EXPOSURE) return false;
            start_exposure_event();
            return true;
        }

        /// <summary>
        /// Returns the not ready for exposure bit-wise variable.
        /// 
        /// If the variable is 0 it means that the Gantry is Ready!
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the not ready condition (bit-wise) </returns>
        static unsigned short getNotReadyCode(void) { return (unsigned short)code; }

        /// <summary>
        /// This function shall be called by every action to the 
        /// system variables that affect the current ready condition
        /// 
        /// Whwnever the ready status changes, the ready_change_event() is generated 
        /// </summary>
        /// <param name=""></param>
        static unsigned short evaluateReadyForExposure(void) {
            options initial_status = code;

            unsigned short notready = 0;
            if (ErrorRegister::isError()) notready |= (unsigned short)options::SYSTEM_ERROR;


            if ((ExposureModeRegister::compressorMode->Value->getCode() != CompressionModeOption::options::CMP_DISABLE)) {
                if (CompressorRegister::getForce() == 0) notready |= (unsigned short)options::MISSING_COMPRESSION;
            }


            if ((ExposureModeRegister::protectionMode->Value->getCode() != PatientProtectionMode::options::PROTECTION_DIS)) {
                if ((ComponentRegister::Value->getCode() != ComponentRegister::options::COMPONENT_PROTECTION_3D) &&
                    (CollimatorComponentRegister::Value->getCode() != CollimatorComponentRegister::options::COLLI_COMPONENT_PROTECTION_2D))
                    notready |= (unsigned short)options::MISSING_PATIENT_PROTECTION;
            }

            if ((ExposureModeRegister::armMode->Value->getCode() != ArmModeOption::options::ARM_DIS)) {
                if (!ArmStatusRegister::isValidAngle()) notready |= (unsigned short)options::WRONG_ARM_POSITION;
            }


            if ((ExposureModeRegister::compressorMode->Value->getCode() != CompressionModeOption::options::CMP_DISABLE)) {
                if ((CompressorRegister::getPaddle()->Value->getCode() == PaddleOption::options::PAD_UNDETECTED) ||
                    (CompressorRegister::getPaddle()->Value->getCode() == PaddleOption::options::PAD_UNLOCKED) ||
                    (CompressorRegister::getPaddle()->Value->getCode() == PaddleOption::options::UNDEF)) notready |= (unsigned short)options::WRONG_PADDLE_DETECTED;
            }

            if (ExposureModeRegister::exposureType->Value->getCode() == ExposureTypeOption::options::UNDEF) notready |= (unsigned short)options::MISSING_VALID_EXPOSURE_MODE;

            if (!ExposureDataRegister::getPulse(0)->isValid()) notready |= (unsigned short)options::MISSING_VALID_EXPOSURE_DATA;

            if (XrayPushButtonRegister::isDisabled()) notready |= (unsigned short)options::XRAY_PUSHBUTTON_DISABLED;

            code = (options)notready;

            if (code != initial_status) ready_change_event();

            return (unsigned short)code;
        }

    private:
        static options code = options::NOT_READY_FOR_EXPOSURE; //!< This is the register option-code 

    };

    ref class PowerStatusRegister {
    public:

        static void setBatteryData(bool ena, bool batt1low, bool batt2low, unsigned char vbatt1, unsigned char vbatt2);
        static void getBatteryData(bool* ena, bool* batt1low, bool* batt2low, unsigned char* vbatt1, unsigned char* vbatt2);
        static void setPowerdown(bool stat);
        static bool getPowerdown(void);

    private:
        
        // Powerdown
        static bool power_down = false;

        // Battery management
        static bool battery_enabled = false;//!< Battery enabled
        static bool batt1_low = false;      //!< Low voltage of battery 1
        static bool batt2_low = false;      //!< Low voltage of battery 2
        static unsigned char voltage_batt1 = 0; //!< 10 * voltage level of battery 1
        static unsigned char voltage_batt2 = 0; //!< 10 * voltage level of battery 2

    };

    ref class SafetyStatusRegister {
    public:

        static void setCloseDoor(bool stat);
        static bool getCloseDoor(void);

    private:

        static bool closed_door = false;

        
    };

}






