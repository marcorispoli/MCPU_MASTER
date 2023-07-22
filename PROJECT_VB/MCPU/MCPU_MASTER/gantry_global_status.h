#pragma once

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
            FILTER_Ag, //!< Ag filter seletion option
            FILTER_Al, //!< Al filter seletion option
            FILTER_Mo, //!< Mo filter seletion option
            FILTER_Rh, //!< Rh filter seletion option
            FILTER_Cu, //!< Cu filter seletion option
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^> { "Ag", "Al", "Mo", "Rh", "Cu", "UNDEF"}; //!< This is the tags array

        /// <summary>
        /// Returns the option code of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the option code </returns>
        options getCode(void) { return code; }

        /// <summary>
        /// Returns the option tag of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        String^ getTag(void) { return tag; }


        /// <summary>
        /// This function set the register with the option code (and related tag).
        /// 
        /// The register is updated with the option code and related tag and, in case of value change,
        /// the status_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        bool setCode(options val) {
            if (val >= options::LEN) {

                code = options::UNDEF;
                tag = "UNDEF";
                return false;
            }

            if (val != code) {
                code = val;
                tag = tags[(int)val];
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
            for (int i = 0; i < (int)options::LEN; i++) {
                if (val == tags[i]) {
                    if (tag != val) {
                        tag = val;
                        code = (options)i;
                    }
                    return true;
                }
            }
            code = options::UNDEF;
            tag = "UNDEF";
            return false;

        }

        FilterOptions() {
            code = options::UNDEF;
            tag = "UNDEF";
        }

    private:
        options code; //!< This is the register option-code 
        String^ tag; //!< This is the register tag
    };

    /// <summary>
    /// This class implements the XRAY completed status register.
    /// \ingroup globalModule 
    /// </summary>
    ref class XrayCompletedOptions {
    public:

        
        ///  Definition of the register enumeration codes        
        enum class options {
            XRAY_COMPLETED_OK = 0, //!< XRAY sequence successfully completed
            XRAY_COMPLETED_PARTIAL_DOSE = 1,//!< XRAY sequence partially completed
            XRAY_COMPLETED_NO_DOSE = 2,//!< XRAY sequence aborted without dose
            LEN,
            UNDEF=LEN
        };
        static const array<String^>^ tags = gcnew array<String^>  {"OK", "PARTIAL", "NOK", "UNDEF"}; //!< Definition of the register tags
        

        delegate void delegate_status_change(void); //!< This is the delegate of the status_change_event();

        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: xrayCompletedOptions::status_change_event += gcnew delegate_status_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_status_change^ status_change_event;


        /// <summary>
        /// Returns the option code of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the option code </returns>
        static options getCode(void) { return code; }

        /// <summary>
        /// Returns the option tag of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        static String^ getTag(void) { return tag; }


        /// <summary>
        /// This function set the register with the option code (and related tag).
        /// 
        /// The register is updated with the option code and related tag and, in case of value change,
        /// the status_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        static bool setCode(options val) {
            if (val >= options::LEN) {
                
                code = options::UNDEF;
                tag = "UNDEF";
                status_change_event();
                return false;
            }

            if (val != code) {
                code = val;
                tag = tags[(int)val];
                status_change_event();
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
        static bool setCode(String^ val) {
            for (int i = 0; i < (int)options::LEN; i++) {
                if (val == tags[i]) {
                    if (tag != val) {
                        tag = val;
                        code = (options) i;
                        status_change_event();
                    }
                    return true;
                }
            }
            code = options::UNDEF;
            tag = "UNDEF";
            status_change_event();
            return false;
            
        }

    private:
        static options code = options::UNDEF; //!< This is the register option-code 
        static String^ tag = "UNDEF"; //!< This is the register tag
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
        enum class options_ena {
            XRAYPUSH_DISABLED = 0, //!< XRAY Push Button is enabled
            XRAYPUSH_ENABLED = 1,//!< XRAY Push Button is disabled
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags_ena = gcnew array<String^>  {"OFF", "ON",  "UNDEF"}; //!< Definition of the register tags


        delegate void delegate_status_ena_change(void); //!< This is the delegate of the status_ena_change_event();

        /// <summary>
        /// This event is generated whenver the pushbutton enable status  is changed.
        /// 
        /// Usage: XrayPushButtonRegister::status_ena_change_event += gcnew delegate_status_ena_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_status_ena_change^ status_ena_change_event;


        /// <summary>
        /// Returns the pushbutton enable option code 
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the option code </returns>
        static options_ena getEnableCode(void) { return code_ena; }

        /// <summary>
        /// Returns the pushbutton enable option tag 
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        static String^ getEnableTag(void) { return tag_ena; }


        /// <summary>
        /// This function set the value of the pushbutton enable code (and related tag).
        /// 
        /// The register is updated with the option code and related tag and, in case of value change,
        /// the status_ena_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        static bool setEnableCode(options_ena val) {
            if (val >= options_ena::LEN) {

                code_ena = options_ena::UNDEF;
                tag_ena = "UNDEF";
                status_ena_change_event();
                return false;
            }

            if (val != code_ena) {
                code_ena = val;
                tag_ena = tags_ena[(int)val];
                status_ena_change_event();
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
        static bool setEnableCode(String^ val) {
            for (int i = 0; i < (int)options_ena::LEN; i++) {
                if (val == tags_ena[i]) {
                    if (tag_ena != val) {
                        tag_ena = val;
                        code_ena = (options_ena)i;
                        status_ena_change_event();
                    }
                    return true;
                }
            }
            code_ena = options_ena::UNDEF;
            tag_ena = "UNDEF";
            status_ena_change_event();
            return false;

        }

    private:
        static options_ena code_ena = options_ena::UNDEF; //!< This is the register option-code 
        static String^ tag_ena = "UNDEF"; //!< This is the register tag
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
    /// \ingroup globalModule 
    /// </summary>
    ref class TomoConfigRegister {
    public:

        /// <summary>
        /// This is the enumeration option code for file configuration 
        /// </summary>
        enum class options_conf {
            TOMO_1F, //!< Tomo configuration file 1F
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
            NARROW,         //!< Narrow sequence
            INTERMEDIATE,   //!< Intermediate sequence
            WIDE,           //!< Wide seqeunce
            SUPERWIDE,      //!< Super-wide sequence
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags_seq = gcnew array<String^>  { "N", "I", "W", "SW", "UNDEF" }; //!< This is the sequence option-tags static array


        delegate void delegate_selection_change(void); //!< This is the delegate of the status_change_event();

        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: TomoConfigRegister::selection_change_event += gcnew delegate_selection_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_selection_change^ selection_change_event;

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
                    confid = (options_conf) i;
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
                    if( (confid != curconf) || (seqid != curseq))
                        selection_change_event();
                    
                    return true;
                }
            }

            confid = options_conf::UNDEF;
            seqid = options_seq::UNDEF;
            return false;

        }
       

    private:
        static options_conf confid = options_conf::UNDEF; //!< This is the register conf_id 
        static options_seq seqid = options_seq::UNDEF; //!< This is the register sequence_id 
    };

    /// <summary>
    /// This is the Gantry operating status register definition
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

        delegate void delegate_status_change(void); //!< This is the delegate of the status_change_event();

        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: OperatingStatusRegister::status_change_event += gcnew delegate_status_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_status_change^ status_change_event;

        /// <summary>
        /// Returns the option code of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the option code </returns>
        static options getCode(void) { return code; }

        /// <summary>
        /// Returns the option tag of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        static String^ getTag(void) { return tag; }


        /// <summary>
        /// This function set the register with the option code (and related tag).
        /// 
        /// The register is updated with the option code and related tag and, in case of value change,
        /// the status_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        static bool setCode(options val) {
            if (val >= options::LEN) {

                code = options::UNDEF;
                tag = "UNDEF";
                status_change_event();
                return false;
            }

            if (val != code) {
                code = val;
                tag = tags[(int)val];
                status_change_event();
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
        static bool setCode(String^ val) {
            for (int i = 0; i < (int)options::LEN; i++) {
                if (val == tags[i]) {
                    if (tag != val) {
                        tag = val;
                        code = (options)i;
                        status_change_event();
                    }
                    return true;
                }
            }
            code = options::UNDEF;
            tag = "UNDEF";
            status_change_event();
            return false;

        }

        static bool isIDLE(void) { return (code == options::GANTRY_IDLE); }
        static bool isOPEN(void) { return (code == options::GANTRY_OPEN_STUDY); }
  
private:
        static options code = options::UNDEF; //!< This is the register option-code 
        static String^ tag = "UNDEF"; //!< This is the register tag


    };

    /// <summary>
    /// This is the projection handling option class.
    /// 
    /// This class defines the availabl projection options,
    /// store the current selected projection and the projection selectable list.
    /// \ingroup globalModule 
    /// </summary>
    ref class ProjectionRegister {
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
        
        delegate void delegate_lista_change(void);//!< This is the delegate of the lista_change_event();

        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: ProjectionRegister::lista_change_event += gcnew delegate_lista_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_lista_change^ lista_change_event;


        delegate void delegate_status_change(void); //!< This is the delegate of the status_change_event();

        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: ProjectionRegister::status_change_event += gcnew delegate_status_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_status_change^ status_change_event;

       
        /// <summary>
       /// Returns the option code of the register
       /// </summary>
       /// <param name=""></param>
       /// <returns>the value of the option code </returns>
        static options getCode(void) { return code; }

        /// <summary>
        /// Returns the option tag of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        static String^ getTag(void) { return tag; }


        /// <summary>
        /// This function set the register with the option code (and related tag).
        /// 
        /// The register is updated with the option code and related tag and, in case of value change,
        /// the status_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        static bool setCode(options val) {
            if (val >= options::LEN) {

                code = options::UNDEF;
                tag = "UNDEF";
                status_change_event();
                return false;
            }

            if (val != code) {
                code = val;
                tag = tags[(int)val];
                status_change_event();
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
        static bool setCode(String^ val) {
            for (int i = 0; i < (int)options::LEN; i++) {
                if (val == tags[i]) {
                    if (tag != val) {
                        tag = val;
                        code = (options)i;
                        status_change_event();
                    }
                    return true;
                }
            }
            code = options::UNDEF;
            tag = "UNDEF";
            status_change_event();
            return false;

        }

        /// <summary>
        /// This function set the list of available projections.
        /// 
        /// In case of list change, the list_change_event() is generated
        /// </summary>
        /// <param name="lista">This is the list</param>
        /// <returns>true if the list is a valid list</returns>
        static bool setList(List<int>^ lista) {
            if (lista->Count >= (int)options::LEN) return false;
            for (int i = 0; i < lista->Count; i++) {
                if (lista[i] >= (int)options::LEN) return false;
            }

            list = lista;
            lista_change_event();
            return true;
        }


    private:
        static options code = options::UNDEF; //!< This is the register option-code 
        static String^ tag = "UNDEF"; //!< This is the register tag     
        static List<int>^ list = gcnew List<int> {}; //!< This is the list of the projections

    };

    /// <summary>
    /// This register handle the Component that can be used during the exposure
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class ComponentRegister {
    public:

        enum class options {
            COMPONENT_UNDETECTED = 0,
            COMPONENT_PROTECTION_3D,
            COMPONENT_MAG15,
            COMPONENT_MAG18,
            COMPONENT_MAG20,
            COMPONENT_BIOPSY,
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^> { "COMPONENT_UNDETECTED", "PROTECTION_3D", "MAG_15", "MAG_18", "MAG_20", "BIOPSY", "UNDEF"};//!< This is the option-tags static array
        delegate void delegate_status_change(void); //!< This is the delegate of the status_change_event();

        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: ComponentRegister::status_change_event += gcnew delegate_status_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_status_change^ status_change_event;

        /// <summary>
         /// Returns the option code of the register
         /// </summary>
         /// <param name=""></param>
         /// <returns>the value of the option code </returns>
        static options getCode(void) { return code; }

        /// <summary>
        /// Returns the option tag of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        static String^ getTag(void) { return tag; }


        /// <summary>
        /// This function set the register with the option code (and related tag).
        /// 
        /// The register is updated with the option code and related tag and, in case of value change,
        /// the status_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        static bool setCode(options val) {
            if (val >= options::LEN) {

                code = options::UNDEF;
                tag = "UNDEF";
                status_change_event();
                return false;
            }

            if (val != code) {
                code = val;
                tag = tags[(int)val];
                status_change_event();
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
        static bool setCode(String^ val) {
            for (int i = 0; i < (int)options::LEN; i++) {
                if (val == tags[i]) {
                    if (tag != val) {
                        tag = val;
                        code = (options)i;
                        status_change_event();
                    }
                    return true;
                }
            }
            code = options::UNDEF;
            tag = "UNDEF";
            status_change_event();
            return false;

        }

    private:
        static options code = options::UNDEF; //!< This is the register option-code 
        static String^ tag = "UNDEF"; //!< This is the register tag
    };

    /// <summary>
    /// This register handles the component applicable to the collimator
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class CollimatorComponentRegister {
    public:

        enum class options {
            COLLI_COMPONENT_UNDETECTED = 0,
            COLLI_COMPONENT_PROTECTION_2D,
            COLLI_COMPONENT_PROTECTION_SHIFTED,
            COLLI_COMPONENT_LEAD_SCREEN,
            COLLI_COMPONENT_SPECIMEN,
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^> { "COLLI_COMPONENT_UNDETECTED", "PROTECTION_2D", "SHIFTED_PROTECTION", "LEAD_SCREEN", "SPECIMEN", "UNDEF" };//!< This is the option-tags static array

        delegate void delegate_status_change(void); //!< This is the delegate of the status_change_event();

        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: CollimatorComponentRegister::status_change_event += gcnew delegate_status_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_status_change^ status_change_event;

        /// <summary>
         /// Returns the option code of the register
         /// </summary>
         /// <param name=""></param>
         /// <returns>the value of the option code </returns>
        static options getCode(void) { return code; }

        /// <summary>
        /// Returns the option tag of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        static String^ getTag(void) { return tag; }


        /// <summary>
        /// This function set the register with the option code (and related tag).
        /// 
        /// The register is updated with the option code and related tag and, in case of value change,
        /// the status_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        static bool setCode(options val) {
            if (val >= options::LEN) {

                code = options::UNDEF;
                tag = "UNDEF";
                status_change_event();
                return false;
            }

            if (val != code) {
                code = val;
                tag = tags[(int)val];
                status_change_event();
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
        static bool setCode(String^ val) {
            for (int i = 0; i < (int)options::LEN; i++) {
                if (val == tags[i]) {
                    if (tag != val) {
                        tag = val;
                        code = (options)i;
                        status_change_event();
                    }
                    return true;
                }
            }
            code = options::UNDEF;
            tag = "UNDEF";
            status_change_event();
            return false;

        }

    private:
        static options code = options::UNDEF; //!< This is the register option-code 
        static String^ tag = "UNDEF"; //!< This is the register tag
    };

    /// <summary>
    /// This register handles the Paddles identifiable by the compressor device
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class PaddleRegister {
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
        static const array<String^>^ tags = gcnew array<String^> { "PAD_24x30", "PAD_18x24_C", "PAD_18x24_L", "PAD_18x24_R", "PADCOLLI_9x21", "PAD_10x24", "PAD_D75", "PAD_BIOP2D", "PAD_BIOP3D", "PAD_TOMO", "PAD_9x9", "PAD_UNDETECTED", "PAD_UNLOCKED", "UNDEF" };//!< This is the option-tags static array


        delegate void delegate_status_change(void); //!< This is the delegate of the status_change_event();

        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: PaddleRegister::status_change_event += gcnew delegate_status_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_status_change^ status_change_event;

        /// <summary>
         /// Returns the option code of the register
         /// </summary>
         /// <param name=""></param>
         /// <returns>the value of the option code </returns>
        static options getCode(void) { return code; }

        /// <summary>
        /// Returns the option tag of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        static String^ getTag(void) { return tag; }


        /// <summary>
        /// This function set the register with the option code (and related tag).
        /// 
        /// The register is updated with the option code and related tag and, in case of value change,
        /// the status_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        static bool setCode(options val) {
            if (val >= options::LEN) {

                code = options::UNDEF;
                tag = "UNDEF";
                status_change_event();
                return false;
            }

            if (val != code) {
                code = val;
                tag = tags[(int)val];
                status_change_event();
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
        static bool setCode(String^ val) {
            for (int i = 0; i < (int)options::LEN; i++) {
                if (val == tags[i]) {
                    if (tag != val) {
                        tag = val;
                        code = (options)i;
                        status_change_event();
                    }
                    return true;
                }
            }
            code = options::UNDEF;
            tag = "UNDEF";
            status_change_event();
            return false;

        }

 
    private:
        static options code = options::UNDEF; //!< This is the register option-code 
        static String^ tag = "UNDEF"; //!< This is the register tag
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

        /// <summary>
        /// This is the enumeration option code 
        /// </summary>
        enum class options {
            MAN_2D, //!< The next exposure is a 2D manual mode
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
        static const array<String^>^ tags = gcnew array<String^>  { "MAN_2D", "AEC_2D", "MAN_3D", "AEC_3D", "MAN_COMBO", "AEC_COMBO" , "MAN_AE", "AEC_AE", "UNDEF"}; //!< This is the option-tags static array

        delegate void delegate_status_change(void); //!< This is the delegate of the status_change_event();

        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: ExposureModeRegister::status_change_event += gcnew delegate_status_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_status_change^ status_change_event;


        /// <summary>
        /// Returns the option code of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the option code </returns>
        static options getCode(void) { return code; }

        /// <summary>
        /// Returns the option tag of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        static String^ getTag(void) { return tag; }


        /// <summary>
        /// This function set the register with the option code (and related tag).
        /// 
        /// The register is updated with the option code and related tag and, in case of value change,
        /// the status_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        static bool setCode(options val) {
            if (val >= options::LEN) {

                code = options::UNDEF;
                tag = "UNDEF";
                status_change_event();
                return false;
            }

            if (val != code) {
                code = val;
                tag = tags[(int)val];
                status_change_event();
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
        static bool setCode(String^ val) {
            for (int i = 0; i < (int)options::LEN; i++) {
                if (val == tags[i]) {
                    if (tag != val) {
                        tag = val;
                        code = (options)i;
                        status_change_event();
                    }
                    return true;
                }
            }
            code = options::UNDEF;
            tag = "UNDEF";
            status_change_event();
            return false;

        }

    private:
        static options code = options::UNDEF; //!< This is the register option-code 
        static String^ tag = "UNDEF"; //!< This is the register tag
    };

    /// <summary>
    /// This class handles the Detector model selection.
    /// 
    /// The Detector model influences the behavior of the exposure sequence
    /// due to possibly differences in the synch signals and or timing.
    /// \ingroup globalModule 
    /// </summary>
    ref class DetectorTypeRegister {
    public:

        /// <summary>
        /// This is the enumeration option code 
        /// </summary>
        enum class options {
            LMAM2V2, //!< Analogic LMAM2 V2
            LMAM2FDIV2,  //!< Analogic LMAM2 FDI-V2            
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^>  { "LMAM2V2", "LMAM2FDIV2", "UNDEF"}; //!< This is the option-tags static array

        delegate void delegate_status_change(void); //!< This is the delegate of the status_change_event();

        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: DetectorTypeRegister::status_change_event += gcnew delegate_status_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_status_change^ status_change_event;


        /// <summary>
        /// Returns the option code of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the option code </returns>
        static options getCode(void) { return code; }

        /// <summary>
        /// Returns the option tag of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        static String^ getTag(void) { return tag; }


        /// <summary>
        /// This function set the register with the option code (and related tag).
        /// 
        /// The register is updated with the option code and related tag and, in case of value change,
        /// the status_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        static bool setCode(options val) {
            if (val >= options::LEN) {

                code = options::UNDEF;
                tag = "UNDEF";
                status_change_event();
                return false;
            }

            if (val != code) {
                code = val;
                tag = tags[(int)val];
                status_change_event();
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
        static bool setCode(String^ val) {
            for (int i = 0; i < (int)options::LEN; i++) {
                if (val == tags[i]) {
                    if (tag != val) {
                        tag = val;
                        code = (options)i;
                        status_change_event();
                    }
                    return true;
                }
            }
            code = options::UNDEF;
            tag = "UNDEF";
            status_change_event();
            return false;

        }

    private:
        static options code = options::UNDEF; //!< This is the register option-code 
        static String^ tag = "UNDEF"; //!< This is the register tag
    };

    /// <summary>
    /// This class handles the Compression behavior during the exposure.
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class CompressionModeRegister {
    public:

        /// <summary>
        /// This is the enumeration option code 
        /// </summary>
        enum class options {
            CMP_KEEP, //!< Keeps the compression after exposure;
            CMP_RELEASE,  //!< Releases the compression after exposure;
            CMP_DISABLE, //!< Disables the Compression check (for exposures without the compression);
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^>  { "CMP_KEEP", "CMP_RELEASE", "CMP_DISABLE", "UNDEF"}; //!< This is the option-tags static array

        delegate void delegate_status_change(void); //!< This is the delegate of the status_change_event();

        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: CompressionModeRegister::status_change_event += gcnew delegate_status_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_status_change^ status_change_event;


        /// <summary>
        /// Returns the option code of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the option code </returns>
        static options getCode(void) { return code; }

        /// <summary>
        /// Returns the option tag of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        static String^ getTag(void) { return tag; }


        /// <summary>
        /// This function set the register with the option code (and related tag).
        /// 
        /// The register is updated with the option code and related tag and, in case of value change,
        /// the status_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        static bool setCode(options val) {
            if (val >= options::LEN) {

                code = options::UNDEF;
                tag = "UNDEF";
                status_change_event();
                return false;
            }

            if (val != code) {
                code = val;
                tag = tags[(int)val];
                status_change_event();
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
        static bool setCode(String^ val) {
            for (int i = 0; i < (int)options::LEN; i++) {
                if (val == tags[i]) {
                    if (tag != val) {
                        tag = val;
                        code = (options)i;
                        status_change_event();
                    }
                    return true;
                }
            }
            code = options::UNDEF;
            tag = "UNDEF";
            status_change_event();
            return false;

        }

    private:
        static options code = options::UNDEF; //!< This is the register option-code 
        static String^ tag = "UNDEF"; //!< This is the register tag
    };


    /// <summary>
    /// This class handles the Collimation behavior during the exposure.
    /// 
    /// There are several collimation patterns predefined in the system.
    /// 
    /// This class allows to select the current collimation mode from the 
    /// available list. 
    /// 
    /// This class allows to handle the collimation codes and tag 
    /// for other purpose (visualization). 
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class CollimationModeRegister {
    public:

        /// <summary>
        /// This is the enumeration option code 
        /// </summary>
        enum class options {
            COLLI_AUTO, //!< The Exposure uses the Automatic collimation set by the current detected Paddle
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

        delegate void delegate_status_change(void); //!< This is the delegate of the status_change_event();

        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: CollimationModeRegister::status_change_event += gcnew delegate_status_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_status_change^ status_change_event;


        /// <summary>
        /// Returns the option code of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the option code </returns>
        static options getCode(void) { return code; }

        /// <summary>
        /// Returns the option tag of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        static String^ getTag(void) { return tag; }


        /// <summary>
        /// This function set the register with the option code (and related tag).
        /// 
        /// The register is updated with the option code and related tag and, in case of value change,
        /// the status_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        static bool setCode(options val) {
            if (val >= options::LEN) {

                code = options::UNDEF;
                tag = "UNDEF";
                status_change_event();
                return false;
            }

            if (val != code) {
                code = val;
                tag = tags[(int)val];
                status_change_event();
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
        static bool setCode(String^ val) {
            for (int i = 0; i < (int)options::LEN; i++) {
                if (val == tags[i]) {
                    if (tag != val) {
                        tag = val;
                        code = (options)i;
                        status_change_event();
                    }
                    return true;
                }
            }
            code = options::UNDEF;
            tag = "UNDEF";
            status_change_event();
            return false;

        }

    private:
        static options code = options::UNDEF; //!< This is the register option-code 
        static String^ tag = "UNDEF"; //!< This is the register tag
    };

    /// <summary>
    /// This class handles the Usage of the patient protection during the exposure.
    /// 
    /// \ingroup globalModule 
    /// </summary>
    ref class PatientProtectionRegister {
    public:

        /// <summary>
        /// This is the enumeration option code 
        /// </summary>
        enum class options {
            PROTECTION_ENA, //!< Enables the Patient protection check;
            PROTECTION_DIS, //!< Disables the Patient protection check;            
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^>  { "PROTECTION_ENA", "PROTECTION_DIS", "UNDEF"}; //!< This is the option-tags static array

        delegate void delegate_status_change(void); //!< This is the delegate of the status_change_event();

        /// <summary>
        /// This event is generated whenver the status of the register is changed.
        /// 
        /// Usage: PatientProtectionRegister::status_change_event += gcnew delegate_status_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_status_change^ status_change_event;


        /// <summary>
        /// Returns the option code of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the option code </returns>
        static options getCode(void) { return code; }

        /// <summary>
        /// Returns the option tag of the register
        /// </summary>
        /// <param name=""></param>
        /// <returns>the value of the Tag </returns>
        static String^ getTag(void) { return tag; }


        /// <summary>
        /// This function set the register with the option code (and related tag).
        /// 
        /// The register is updated with the option code and related tag and, in case of value change,
        /// the status_change_event() is generated.
        /// 
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        static bool setCode(options val) {
            if (val >= options::LEN) {

                code = options::UNDEF;
                tag = "UNDEF";
                status_change_event();
                return false;
            }

            if (val != code) {
                code = val;
                tag = tags[(int)val];
                status_change_event();
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
        static bool setCode(String^ val) {
            for (int i = 0; i < (int)options::LEN; i++) {
                if (val == tags[i]) {
                    if (tag != val) {
                        tag = val;
                        code = (options)i;
                        status_change_event();
                    }
                    return true;
                }
            }
            code = options::UNDEF;
            tag = "UNDEF";
            status_change_event();
            return false;

        }

    private:
        static options code = options::UNDEF; //!< This is the register option-code 
        static String^ tag = "UNDEF"; //!< This is the register tag
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
        /// This is the enumeration option code for the Arm Mode option
        /// The Arm Mode Option enables or disables the check of the angle during the exposure.
        /// 
        /// </summary>
        enum class exposure_mode_options {
            ARM_ENA, //!< Enables the Angle range check during exposure;
            ARM_DIS, //!< Disables the Angle range check during the exposure;            
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ exposure_mode_tags = gcnew array<String^>  { "ARM_ENA", "ARM_DIS", "UNDEF"}; //!< This is the option-tags static array

        /// <summary>
        /// This function set the Arm Exposure mode option (and related tag).
        /// 
        /// No change value event is generated
        /// </summary>
        /// <param name="val">this is the option-code to be set</param>
        /// <returns>true if success or false if the code doesn't exist</returns>
        static bool setMode(exposure_mode_options val) {
            if (val >= exposure_mode_options::LEN) {

                exposure_mode = exposure_mode_options::UNDEF;
                exposure_mode_tag = "UNDEF";
                return false;
            }

            if (val != exposure_mode) {
                exposure_mode = val;
                exposure_mode_tag = exposure_mode_tags[(int)val];
            }
            return true;
        }

        /// <summary>
        /// This function set the Arm Exposure mode option tag (and related code).
        /// 
        /// No change value event is generated
        /// </summary>
        /// <param name="val"></param>
        /// <returns></returns>
        static bool setMode(String^ val) {
            for (int i = 0; i < (int)exposure_mode_options::LEN; i++) {
                if (val == exposure_mode_tags[i]) {
                    if (exposure_mode_tag != val) {
                        exposure_mode_tag = val;
                        exposure_mode = (exposure_mode_options)i;
                    }
                    return true;
                }
            }
            exposure_mode = exposure_mode_options::UNDEF;
            exposure_mode_tag = "UNDEF";
            return false;

        }




        delegate void delegate_target_change(void);//!< This is the delegate of the target_change_event();
        
        /// <summary>
        /// This event is generated whenver the arm target is changed.
        /// 
        /// Usage: ArmStatusRegister::target_change_event += gcnew delegate_target_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_target_change^ target_change_event;


        
        delegate void delegate_validate_change(void);//!< This is the delegate of the validate_change_event();

        /// <summary>
        /// This event is generated whenver the arm target parameters are validated.
        /// 
        /// Usage: ArmStatusRegister::validate_change_event += gcnew delegate_validate_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_validate_change^ validate_change_event;
        
        /// <summary>
        /// This method validates or invalidates the Arm parameters.
        /// 
        /// In case the data are validated, the exposure process evaluate the correct position of the ARM. 
        /// In case the data are invalidated the exposure cannot start. 
        /// 
        /// When the validation status changes, the validate_change_event() is generated.
        /// </summary>
        /// <param name="stat"></param>
        static void validate(bool stat) {             
            if (position_validated != stat) {
                position_validated = stat;
                validate_change_event();
            }
        }

        /// <summary>
        /// This funtion returns the Arm activation status.
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns>true if the Arm is executing a command</returns>
        static bool isBusy(void) { return executing; }        
        
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
        /// <returns>true if the target is validated</returns>
        static bool setTarget(int pos, int low, int high, int id) {
            if (executing) return false;
            if (pos > 180) return false;
            if (pos < -180) return false;
            if ((pos > high) || (pos < low)) return false;

            position_validated = true;
            position_target = pos;
            allowed_low = low;
            allowed_high = high;
            
            if ((position < pos + 1) || (position > pos - 1)) {                
                executing_id = 0;
                return true;
            }
            
            executing_id = id;
            executing = true;

            validate_change_event();
            target_change_event();
            return true;
        }

    private:
        static exposure_mode_options exposure_mode = exposure_mode_options::UNDEF; //!< This is the current selected Arm Exposure Mode option code
        static String^ exposure_mode_tag = "UNDEF";//!< This is the current selected Arm Exposure Mode option tag

        static bool         position_validated = false; //!< Validation status
        static int          position_target = 0;        //!< Validated target position
        static int          position;                   //!< Current Arm position
        static int          allowed_low = 0;            //!< Lower acceptable angle (°)
        static int          allowed_high = 0;           //!< Higher acceptable angle (°)
        static bool         executing = false;          //!< Command is in execution
        static unsigned short  executing_id = 0;        //!< AWS command Id
    };

    /// <summary>
    /// This is the class handling the patient name 
    /// \ingroup globalModule 
    /// </summary>
    ref class PatientDataRegister {
        public:
            /// <summary>
            /// This function sets the current patient name
            /// </summary>
            /// <param name="val">The name assigned</param>
            static void setName(String^ val) { patient_name = val; }

            /// <summary>
            /// This function returns the current patient name
            /// </summary>
            /// <returns>The current assigned patient name</returns>
            static String^ getName() { return patient_name; }
        private: 
            static String^ patient_name =  ""; //!< Patient name register
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
        enum class options {
            TRX_SCOUT = 0, //!< TRX in scout position
            TRX_BP_R,       //!< TRX in Biopsy Right position
            TRX_BP_L,       //!< TRX in Biopsy Left position
            TRX_TOMO_H,     //!< TRX in Tomo Home position
            TRX_TOMO_E,     //!< TRX in Tomo End position       
            LEN,
            UNDEF = LEN
        };
        static const array<String^>^ tags = gcnew array<String^> { "SCOUT", "BP_R", "BP_L", "TOMO_H", "TOMO_E"}; //!< This is the tags array

     

        delegate void delegate_target_change(void);//!< This is the delegate of the target_change_event();

        /// <summary>
        /// This event is generated whenver the trx target is changed.
        /// 
        /// Usage: TrxStatusRegister::target_change_event += gcnew delegate_target_change(&some_class, some_class::func)
        /// </summary>
        static event delegate_target_change^ target_change_event;

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
        /// If the target is changed, the target_change_event() is generated
        /// </summary>
        /// <param name="tg">this is the target option code</param>
        /// <param name="id">this is the aws command identifier</param>
        /// <returns>true if the target is successfully set</returns>
        static bool setTarget(options tg, int id) {
            if (executing) return false;
            if (tg >=  options::LEN) return false;
            
            if (tg != position_target) {
                position_target = tg;
                executing_id = id;
                executing = true;
                target_change_event();                
            }
            
            return true;
        }

        /// <summary>
        /// This is the same as the setTarget(options tg, int id)
        /// but with the tag as parameter.
        /// 
        /// </summary>
        /// <param name="tag">Trx Target Tag</param>
        /// <param name="id">this is the aws command identifier</param>
        /// <returns>true if the target is successfully set</returns>
        static bool setTarget(String^ tag, int id) {
            if (executing) return false;
            for (int i = 0; i < (int) options::LEN; i++) {
                if (tags[i] == tag) {
                    if ((i != (int) position_target) ||(current_position != position_target)) {
                        position_target = (options) i;
                        executing_id = id;
                        executing = true;
                        target_change_event();
                        return true;
                    }
                    
                }
            }
            return false;
        }


    private:
        
        static options      position_target = options::UNDEF; //!< This is the Trx target position
        static options      current_position = options::UNDEF;//!< This is the current Trx position        
        static bool         executing = false;          //!< Command is in execution
        static unsigned short  executing_id = 0;        //!< AWS command Id
    };

    

    /// <summary>
    /// This register handle the Exposure pulse data info.
    /// 
    /// The Exposure data of the exposure sequence is stored into this register.
    /// 
    /// </summary>
    ref class ExposureDataRegister {
    public:

        /// <summary>
        /// Internal class uses to group the exposure data pulses
        /// 
        /// </summary>
        ref class exposurePulse {
        public:

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
        
            FilterOptions^ filter; //!< This is the assigned filter handle
            double kV; //!< This is the selected kV value
            double mAs; //!< This is the selected mAs value
            bool   validated; //!< This is the flag that validate the use of thhis pulse in a sequence
        };

        /// <summary>
        /// This function sets the exposure data of a given exposure pulse number.
        /// 
        /// There are a maximum of 4 possible exposure pulses.
        /// 
        /// </summary>
        /// <param name="seq">This is the pulse number, [0:3] </param>
        /// <param name="filter_tag"> This is the tag of the filter used in the next exposure pulse</param>
        /// <param name="kV">kV value of the next exposure pulse [20:640] </param>
        /// <param name="mAs">mAs value of the next exposure pulse [0:640] </param>
        /// <returns>true if data are accepted</returns>
        static bool setPulse(unsigned char seq, double kV, double mAs, String^ filter_tag ) {
            if (seq > pulses->Length) return false;
            if ((kV > 49.0) || (kV < 20.0)) return false;
            if ((mAs > 640) || (mAs < 0)) return false;

            // The pulse shall be consumed before to reuse it.
            if (pulses[seq]->validated) return false;

            if (!pulses[seq]->filter->setCode(filter_tag)) return false;

            pulses[seq]->kV = kV;
            pulses[seq]->mAs = mAs;
            pulses[seq]->validated = true;
            return true;
        }
        
        /// <summary>
        /// This function sets the exposure data of a given exposure pulse number.
        /// 
        /// There are a maximum of 4 possible exposure pulses.
        /// 
        /// </summary>
        /// <param name="seq">This is the pulse number, [0:3] </param>
        /// <param name="filter_code"> This is the code of the filter used in the next exposure pulse</param>
        /// <param name="kV">kV value of the next exposure pulse [20:640] </param>
        /// <param name="mAs">mAs value of the next exposure pulse [0:640] </param>
        /// <returns>true if data are accepted</returns>
        static bool setPulse(unsigned char seq, double kV, double mAs, FilterOptions::options filter_code) {
            if (seq > pulses->Length) return false;
            if ((kV > 49.0) || (kV < 20.0)) return false;
            if ((mAs > 640) || (mAs < 0)) return false;

            // The pulse shall be consumed before to reuse it.
            if (pulses[seq]->validated) return false;       
            
            if (!pulses[seq]->filter->setCode(filter_code)) return false;

            pulses[seq]->kV = kV;
            pulses[seq]->mAs = mAs;
            pulses[seq]->validated = true;
            return true;
        }
 
        /// <summary>
        /// This function retrive the data of a given pulse.
        /// The Pulse is then invalidated (consumed)
        /// </summary>
        /// <param name="seq">This is the pulse sequence number [0:3]</param>
        /// <param name="pulse">This is the reference of a pulse structure receiving the data</param>
        /// <param name="invalidate">This is the action after the call. If true the pulse is consumed</param>
        /// <returns>true if the data are not correct or the pulse is not validated yet</returns>
        static bool getValidatedPulse(unsigned char seq, exposurePulse^ %pulse, bool invalidate) {
            if (seq > pulses->Length) return false;
            if (pulse == nullptr) return false;
            pulse = pulses[seq];
            if (!pulse->validated) return false;

            // If requested, the pulse data is invalidated after the call
            if(invalidate) pulses[seq]->validated = false;
            return true;
        }

    private:
        static array<exposurePulse^>^ pulses = gcnew array<exposurePulse^>{gcnew exposurePulse, gcnew exposurePulse, gcnew exposurePulse, gcnew exposurePulse};//!< Stores the array of data pulses
        
    };


   


}





    
   