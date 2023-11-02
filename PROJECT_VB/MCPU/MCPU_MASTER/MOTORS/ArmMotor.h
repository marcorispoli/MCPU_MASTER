#pragma once
#include "CanOpenMotor.h"
#include "gantry_global_status.h"

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
    static ProjectionOptions() {
        ProjectionsIcons->Add("", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\NO_PROJ.PNG"));
        ProjectionsIcons->Add("UNDEF", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\NO_PROJ.PNG"));
        ProjectionsIcons->Add("LCC", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\LCC.PNG"));
        ProjectionsIcons->Add("LFB", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\LFB.PNG"));
        ProjectionsIcons->Add("LISO", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\LISO.PNG"));
        ProjectionsIcons->Add("LLM", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\LLM.PNG"));
        ProjectionsIcons->Add("LLMO", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\LLMO.PNG"));
        ProjectionsIcons->Add("LML", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\LML.PNG"));
        ProjectionsIcons->Add("LMLO", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\LMLO.PNG"));
        ProjectionsIcons->Add("LSIO", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\LSIO.PNG"));
        ProjectionsIcons->Add("RCC", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\RCC.PNG"));
        ProjectionsIcons->Add("RFB", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\RFB.PNG"));
        ProjectionsIcons->Add("RISO", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\RISO.PNG"));
        ProjectionsIcons->Add("RLM", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\RLM.PNG"));
        ProjectionsIcons->Add("RLMO", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\RLMO.PNG"));
        ProjectionsIcons->Add("RML", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\RML.PNG"));
        ProjectionsIcons->Add("RMLO", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\RMLO.PNG"));
        ProjectionsIcons->Add("RSIO", Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\prjFFDM\\RSIO.PNG"));


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

    static GantryStatusRegisters::enumType<options>^ Value = gcnew GantryStatusRegisters::enumType<options>(tags);
    static Dictionary<System::String^, Image^>^ ProjectionsIcons = gcnew Dictionary<System::String^, Image^>();

};

ref class ArmMotor : public CANOPEN::CanOpenMotor
{
public:
	ArmMotor(void);

    // Exposure acceptable conditions
public: 
    bool isTarget(void) { return ((current_uposition >= selected_target - 1) && (current_uposition <= selected_target + 1));}
    bool isValidTarget(void) { return valid_target; }
    bool isInRange(void) { return ((current_uposition >= allowed_low) && (current_uposition <= allowed_high)); }
    bool isValidPosition(void) { return (valid_target && isInRange()); }

    // Arm activation section
public:
    delegate void delegate_target_change_callback(int id, int target_position);
    static event delegate_target_change_callback^ target_change_event;
    
    bool setTarget(int pos, int low, int high, System::String^ proj, int id) {       
        if (projections->Value->indexOf(proj) < 0) return false;

        // Assignes the projection
        projections->Value->setCode(proj);

        // Assignes the target data
        allowed_low = low;
        allowed_high = high;
        valid_target = true;
        selected_target = pos;
        
        target_change_event(id, pos); // For the Window Form update state
        
        // Activate an Isocentric C-ARM rotation
        return activateAutomaticPositioning(id, pos, 1000,200,200,true);
    }

    delegate void delegate_target_abort_callback(void);
    static event delegate_target_abort_callback^ target_abort_event;
    static void abortTarget(void) {
        if (!valid_target) return;
        valid_target = false;
        projections->Value->clearCode();
        target_abort_event();
    }


    // Projection selection section
public:
    static inline ProjectionOptions^ getProjectionsList() { return projections; }
    static System::String^ getSelectedProjection(void) { return projections->Value->getTag(); }
    
    delegate void delegate_abort_projection_request_callback(void);
    static event delegate_abort_projection_request_callback^ abort_projection_request_event;
    static void abortProjectionRequest(void) { if (valid_target) abort_projection_request_event(); };

    delegate void delegate_projection_request_callback(System::String^ str);
    static event delegate_projection_request_callback^ projection_request_event;
    static void projectionRequest(System::String^ projection) { if (projections->Value->isPresent(projection)) projection_request_event(projection); }

protected:
    bool initializeSpecificObjectDictionary(void) override; //!< Sets specific registers for the Arm activation
    void setCommandCompletedCode(MotorCompletedCodes error) override; //!< Override the basic class to handle the Virtual isocentric function

private:
    static ProjectionOptions^ projections = gcnew ProjectionOptions;  //!< This is the current selected projection
    static int         allowed_low = 0;            //!< Lower acceptable angle (°)
    static int         allowed_high = 0;           //!< Higher acceptable angle (°)
    static bool        valid_target = false;       //!< True if the target is a valid target
    static int         selected_target;            //!< Automatic selected target  
};

