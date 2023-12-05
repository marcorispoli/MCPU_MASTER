#pragma once
#include "CanOpenMotor.h"
#include "gantry_global_status.h"

/// <summary>
/// \defgroup ArmMotor_Module C-ARM Module controller
/// \ingroup CanOpenModule 
/// 
/// This module implements the Body rotation requirements.
/// 
/// </summary>
/// 
///  
/// 

/// <summary>
/// 
/// \ingroup ArmMotor_Module
/// </summary>


ref class ProjectionOptions {
public:
    ProjectionOptions() {

        // Initialize the selected projection
        clrProjection();

        // Create a void list
        projection_list = gcnew List<options>();

        ProjectionsIcons = gcnew Dictionary<System::String^, Image^>();
        ProjectionsIcons->Add("", Image::FromFile(Gantry::applicationResourcePath + "Projections\\NO_PROJ.PNG"));
        ProjectionsIcons->Add("UNDEF", Image::FromFile(Gantry::applicationResourcePath + "Projections\\NO_PROJ.PNG"));
        ProjectionsIcons->Add("LCC", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\LCC.PNG"));
        ProjectionsIcons->Add("LFB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\LFB.PNG"));
        ProjectionsIcons->Add("LISO", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\LISO.PNG"));
        ProjectionsIcons->Add("LLM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\LLM.PNG"));
        ProjectionsIcons->Add("LLMO", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\LLMO.PNG"));
        ProjectionsIcons->Add("LML", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\LML.PNG"));
        ProjectionsIcons->Add("LMLO", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\LMLO.PNG"));
        ProjectionsIcons->Add("LSIO", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\LSIO.PNG"));
        ProjectionsIcons->Add("RCC", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\RCC.PNG"));
        ProjectionsIcons->Add("RFB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\RFB.PNG"));
        ProjectionsIcons->Add("RISO", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\RISO.PNG"));
        ProjectionsIcons->Add("RLM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\RLM.PNG"));
        ProjectionsIcons->Add("RLMO", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\RLMO.PNG"));
        ProjectionsIcons->Add("RML", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\RML.PNG"));
        ProjectionsIcons->Add("RMLO", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\RMLO.PNG"));
        ProjectionsIcons->Add("RSIO", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjFFDM\\RSIO.PNG"));


        //ProjectionOptions::Value->addToList(options::LCC);
    }

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

    static const cli::array<System::String^>^ tags = gcnew cli::array<System::String^> {
        "LCC", "LFB", "LISO", "LLM", "LLMO", "LML", "LMLO", "LSIO", "RCC", "RFB", "RISO", "RLM", "RLMO", "RML", "RMLO", "RSIO",
            "UNDEF"
    };//!< This is the option-tags static array

    static options getCode(System::String^ tg) {
        for (int i = 0; i < (int)options::LEN; i++) {
            if (tags[i] == tg) return (options)i;
        }
        return options::UNDEF;
    }

    void clrProjection(void) {
        current_projection_code = options::UNDEF;
        current_projection_name = tags[(int)current_projection_code];
    }

    bool setProjection(System::String^ tag) {
        for (int i = 0; i < (int)options::UNDEF; i++) {
            if (tags[i] == tag) return setProjection((options)i);
        }
        return false;
    }

    bool isValidProjection(System::String^ tag) {
        for (int i = 0; i < (int)options::UNDEF; i++) {
            if (tags[i] == tag) {
                if (projection_list->Contains((options)i)) return true;
                else return false;
            }
        }
        return false;
    }

    bool setProjection(options code) {
        if ((int)code >= (int)options::UNDEF) return false;
        if (!projection_list->Contains(code)) return false;
        current_projection_code = code;
        current_projection_name = tags[(int)code];
        return true;
    }

    bool setList(List<System::String^>^ list) {
        projection_list->Clear();

        for (int i = 0; i < list->Count; i++) {
            options code = getCode(list[i]);
            if (code == options::UNDEF) {
                projection_list->Clear();
                return false;
            }
            projection_list->Add(code);
        }
        return true;
    }

    System::String^ getCurrentProjectionName(void) { return current_projection_name; }
    options getCurrentProjectionCode(void) { return current_projection_code; }
    Image^ getCurrentProjectionIcon(void) { return ProjectionsIcons[current_projection_name]; }
    List<options>^ getCurrentProjectionList(void) { return projection_list; }

    static System::String^ getProjectionName(options code) { return tags[(int)code]; }
    static Image^ getProjectionIcon(System::String^ tag) { return ProjectionsIcons[tag]; }
    static Image^ getProjectionIcon(options code) { return ProjectionsIcons[tags[(int)code]]; }


private:
    options current_projection_code;
    System::String^ current_projection_name;
    static Dictionary<System::String^, Image^>^ ProjectionsIcons;
    List<options>^ projection_list;
};


/// <summary>
/// 
/// \ingroup ArmMotor_Module
/// </summary>
ref class ArmMotor : public CANOPEN::CanOpenMotor
{
public:
    

public:
	ArmMotor(void);
    static ArmMotor^ device = gcnew ArmMotor();
    
    // Exposure acceptable conditions
    static bool isTarget(void) { return ((device->current_uposition >= selected_target - 1) && (device->current_uposition <= selected_target + 1));}
    static bool isValidTarget(void) { return valid_target; }
    static bool isInRange(void) { return ((device->current_uposition >= allowed_low) && (device->current_uposition <= allowed_high)); }
    static bool isValidPosition(void) { return (valid_target && isInRange()); }

    // Arm activation section
    static bool startHoming(void);

    delegate void delegate_target_change_callback(int id, int target_position);
    static event delegate_target_change_callback^ target_change_event;
    static bool setTarget(int pos, int low, int high, System::String^ proj, int id);

    delegate void delegate_target_abort_callback(void);
    static event delegate_target_abort_callback^ target_abort_event;
    static void abortTarget(void);


public:
    static inline ProjectionOptions^ getProjectionsList() { return projections; }
    static System::String^ getSelectedProjection(void) { return projections->getCurrentProjectionName(); }
    
    delegate void delegate_abort_projection_request_callback(void);
    static event delegate_abort_projection_request_callback^ abort_projection_request_event;
    static void abortProjectionRequest(void) { if (valid_target) abort_projection_request_event(); };

    delegate void delegate_projection_request_callback(System::String^ str);
    static event delegate_projection_request_callback^ projection_request_event;
    static void projectionRequest(System::String^ projection) {
        if (projections->isValidProjection(projection)) projection_request_event(projection); 
    }

protected:
    bool initializeSpecificObjectDictionaryCallback(void) override; //!< Sets specific registers for the Arm activation
    void automaticPositioningCompletedCallback(MotorCompletedCodes error) override; //!< Override the basic class to handle the Virtual isocentric function    
    bool idleCallback(void) override;
    void automaticHomingCompletedCallback(MotorCompletedCodes error) override;

    bool iso_activation_mode; //!< Setting this flag, causes the Vertical motor activation at the Arm rotation completion

private:
    static ProjectionOptions^ projections = gcnew ProjectionOptions;  //!< This is the current selected projection
    static int         allowed_low = 0;            //!< Lower acceptable angle (°)
    static int         allowed_high = 0;           //!< Higher acceptable angle (°)
    static bool        valid_target = false;       //!< True if the target is a valid target
    static int         selected_target;            //!< Automatic selected target  
};

