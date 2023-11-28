#pragma once

#include <math.h>
#include "ConfigFile.h"
#include "Errors.h"





using namespace System::Collections::Generic;
using namespace System::Drawing;

ref class GlobalObjects
{
public:
    
    #define pIDLEFORM  ((IdleForm^) GlobalObjects::pIdleForm)
    #define pOPERFORM  ((OperatingForm^) GlobalObjects::pOperatingForm)

    #define pAWS ((awsProtocol^) GlobalObjects::pAws)
    
    #define pMSHIFT  ((CanOpenMotor^) GlobalObjects::pMotShift)
   



    static Object^ pAws = nullptr; //!< Pointer to the AWS interface    
    static Object^ pMotShift = nullptr; //!< Pointer to the Shift Motor
    

  
    // Monitor coordinates
    static int monitor_X0;//!< Pointer to the Monitor X0 position
    static int monitor_Y0;//!< Pointer to the Monitor Y0 position
    static System::String^ applicationResourcePath; //!< This is the current application resource path
    
    // Forms
    static Object^ pIdleForm = nullptr; //!< Pointer to the IdleForm 
    static Object^ pOperatingForm = nullptr; //!< Pointer to the OperatingForm 
    
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

    static void dataLock(void);
    static void dataUnlock(void);

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
        enumType(const cli::array<String^>^% tagsvar) {
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

        void addToList(T item) {
            if (list_code->Contains(item)) return;
            list_code->Add(item);
        }
        void inline clearList(void) { list_code->Clear(); }

    private:
        T enum_code;
        List<T>^ list_code;
        cli::array<String^>^ array_tags;
    };


    //static test<options>^ core = gcnew test<options> (tags);




    
    
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
        static const cli::array<String^>^ tags = gcnew cli::array<String^> { "PAD_24x30", "PAD_18x24_C", "PAD_18x24_L", "PAD_18x24_R", "PADCOLLI_9x21", "PAD_10x24", "PAD_D75", "PAD_BIOP2D", "PAD_BIOP3D", "PAD_TOMO", "PAD_9x9", "PAD_UNDETECTED", "PAD_UNLOCKED", "PAD_UNDETECTED" };//!< This is the option-tags static array
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

        static const cli::array<String^>^ tags = gcnew cli::array<String^>  {"OFF", "ON", "UNDEF"}; //!< Definition of the register tags

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
        static const cli::array<String^>^ tags_conf = gcnew cli::array<String^>  { "TOMO_1F", "TOMO_2F", "TOMO_3F", "TOMO_4F", "TOMO_5F", "UNDEF" }; //!< This is the config id option-tags static array

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
        static const cli::array<String^>^ tags_seq = gcnew cli::array<String^>  { "N", "I", "W", "SW", "UNDEF" }; //!< This is the sequence option-tags static array



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
        static const cli::array<String^>^ tags = gcnew cli::array<String^>   { "GANTRY_STARTUP", "GANTRY_IDLE", "GANTRY_OPEN_STUDY", "GANTRY_SERVICE", "UNDEF" };//!< This is the option-tags static array


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
            if (operating_status != options::GANTRY_IDLE) {
                operating_status = options::GANTRY_IDLE;
                operating_status_change_event();
            }            
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

        static bool isProtection3Dpresent(void) {
            return (Value->getCode() == options::COMPONENT_PROTECTION_3D);
        }

        static unsigned char getMagFactor(void) {
            if (Value->getCode() == options::COMPONENT_MAG15) return 15;
            else if (Value->getCode() == options::COMPONENT_MAG18) return 18;
            else if (Value->getCode() == options::COMPONENT_MAG20) return 20;
            else return 10;
        }

        static const cli::array<String^>^ tags = gcnew cli::array<String^> { "PROTECTION_3D", "MAG_15", "MAG_18", "MAG_20", "BIOPSY", "COMPONENT_UNDETECTED"};//!< This is the option-tags static array        
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
        static const cli::array<String^>^ tags = gcnew cli::array<String^> { "PROTECTION_2D", "SHIFTED_PROTECTION", "LEAD_SCREEN", "SPECIMEN", "COLLI_COMPONENT_UNDETECTED" };//!< This is the option-tags static array
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
        static inline bool isPaddle(void) { return (paddle->Value->getCode() < PaddleOption::options::PAD_UNDETECTED); }
    private:
        static unsigned short breast_thickness = 0;  //!< Compressed breast thickness in mm
        static unsigned short compression_force = 0; //!< Compression force in N
        static PaddleOption^ paddle = gcnew PaddleOption;

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
        static const cli::array<String^>^ tags = gcnew cli::array<String^> { "SCOUT", "BP_R", "BP_L", "TOMO_H", "TOMO_E", "TRX_UNDEF"}; //!< This is the tags array
        static cli::array<int>^ targets = gcnew cli::array<int> {0, 1500, -1500, 2700, -2700, 0 }; //!< This is the current value of the target angles


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
        static const cli::array<String^>^ tags = gcnew cli::array<String^>  { "ITA", "FRA", "ENG", "PRT", "RUS", "ESP", "LTU", "UNDEF"}; //!< This is the option-tags static array
        static enumType<options>^ Value = gcnew enumType<options>(tags);
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






