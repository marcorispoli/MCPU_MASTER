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

        // ----------------------- FFDM SELECTIONS ------------------------------------------------------------------- //
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

        // ----------------------- TOMO SELECTIONS ------------------------------------------------------------------- //       
        ProjectionsIcons->Add("LCCT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\LCCT.PNG"));
        ProjectionsIcons->Add("LFBT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\LFBT.PNG"));
        ProjectionsIcons->Add("LISOT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\LISOT.PNG"));
        ProjectionsIcons->Add("LLMT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\LLMT.PNG"));
        ProjectionsIcons->Add("LLMOT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\LLMOT.PNG"));
        ProjectionsIcons->Add("LMLT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\LMLT.PNG"));
        ProjectionsIcons->Add("LMLOT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\LMLOT.PNG"));
        ProjectionsIcons->Add("LSIOT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\LSIOT.PNG"));
        ProjectionsIcons->Add("RCCT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\RCCT.PNG"));
        ProjectionsIcons->Add("RFBT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\RFBT.PNG"));
        ProjectionsIcons->Add("RISOT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\RISOT.PNG"));
        ProjectionsIcons->Add("RLMT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\RLMT.PNG"));
        ProjectionsIcons->Add("RLMOT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\RLMOT.PNG"));
        ProjectionsIcons->Add("RMLT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\RMLT.PNG"));
        ProjectionsIcons->Add("RMLOT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\RMLOT.PNG"));
        ProjectionsIcons->Add("RSIOT", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjTOMO\\RSIOT.PNG"));

        // ----------------------- CESM SELECTIONS ------------------------------------------------------------------- //       
        ProjectionsIcons->Add("LCCD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\LCCD.PNG"));
        ProjectionsIcons->Add("LFBD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\LFBD.PNG"));
        ProjectionsIcons->Add("LISOD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\LISOD.PNG"));
        ProjectionsIcons->Add("LLMD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\LLMD.PNG"));
        ProjectionsIcons->Add("LLMOD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\LLMOD.PNG"));
        ProjectionsIcons->Add("LMLD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\LMLD.PNG"));
        ProjectionsIcons->Add("LMLOD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\LMLOD.PNG"));
        ProjectionsIcons->Add("LSIOD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\LSIOD.PNG"));
        ProjectionsIcons->Add("RCCD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\RCCD.PNG"));
        ProjectionsIcons->Add("RFBD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\RFBD.PNG"));
        ProjectionsIcons->Add("RISOD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\RISOD.PNG"));
        ProjectionsIcons->Add("RLMD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\RLMD.PNG"));
        ProjectionsIcons->Add("RLMOD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\RLMOD.PNG"));
        ProjectionsIcons->Add("RMLD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\RMLD.PNG"));
        ProjectionsIcons->Add("RMLOD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\RMLOD.PNG"));
        ProjectionsIcons->Add("RSIOD", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCESM\\RSIOD.PNG"));

        // ----------------------- COMBO SELECTIONS ------------------------------------------------------------------- //       
        ProjectionsIcons->Add("LCCB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\LCCB.PNG"));
        ProjectionsIcons->Add("LFBB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\LFBB.PNG"));
        ProjectionsIcons->Add("LISOB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\LISOB.PNG"));
        ProjectionsIcons->Add("LLMB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\LLMB.PNG"));
        ProjectionsIcons->Add("LLMOB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\LLMOB.PNG"));
        ProjectionsIcons->Add("LMLB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\LMLB.PNG"));
        ProjectionsIcons->Add("LMLOB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\LMLOB.PNG"));
        ProjectionsIcons->Add("LSIOB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\LSIOB.PNG"));
        ProjectionsIcons->Add("RCCB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\RCCB.PNG"));
        ProjectionsIcons->Add("RFBB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\RFBB.PNG"));
        ProjectionsIcons->Add("RISOB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\RISOB.PNG"));
        ProjectionsIcons->Add("RLMB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\RLMB.PNG"));
        ProjectionsIcons->Add("RLMOB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\RLMOB.PNG"));
        ProjectionsIcons->Add("RMLB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\RMLB.PNG"));
        ProjectionsIcons->Add("RMLOB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\RMLOB.PNG"));
        ProjectionsIcons->Add("RSIOB", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjCOMBO\\RSIOB.PNG"));
      
        // ----------------------- VI SELECTIONS ------------------------------------------------------------------- //       
        ProjectionsIcons->Add("LCCM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\LCCM.PNG"));
        ProjectionsIcons->Add("LFBM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\LFBM.PNG"));
        ProjectionsIcons->Add("LISOM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\LISOM.PNG"));
        ProjectionsIcons->Add("LLMM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\LLMM.PNG"));
        ProjectionsIcons->Add("LLMOM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\LLMOM.PNG"));
        ProjectionsIcons->Add("LMLM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\LMLM.PNG"));
        ProjectionsIcons->Add("LMLOM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\LMLOM.PNG"));
        ProjectionsIcons->Add("LSIOM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\LSIOM.PNG"));
        ProjectionsIcons->Add("RCCM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\RCCM.PNG"));
        ProjectionsIcons->Add("RFBM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\RFBM.PNG"));
        ProjectionsIcons->Add("RISOM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\RISOM.PNG"));
        ProjectionsIcons->Add("RLMM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\RLMM.PNG"));
        ProjectionsIcons->Add("RLMOM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\RLMOM.PNG"));
        ProjectionsIcons->Add("RMLM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\RMLM.PNG"));
        ProjectionsIcons->Add("RMLOM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\RMLOM.PNG"));
        ProjectionsIcons->Add("RSIOM", Image::FromFile(Gantry::applicationResourcePath + "Projections\\prjVI\\RSIOM.PNG"));
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

        LCCT,
        LFBT,
        LISOT,
        LLMT,
        LLMOT,
        LMLT,
        LMLOT,
        LSIOT,
        RCCT,
        RFBT,
        RISOT,
        RLMT,
        RLMOT,
        RMLT,
        RMLOT,
        RSIOT,

        LCCD,
        LFBD,
        LISOD,
        LLMD,
        LLMOD,
        LMLD,
        LMLOD,
        LSIOD,
        RCCD,
        RFBD,
        RISOD,
        RLMD,
        RLMOD,
        RMLD,
        RMLOD,
        RSIOD,

        LCCB,
        LFBB,
        LISOB,
        LLMB,
        LLMOB,
        LMLB,
        LMLOB,
        LSIOB,
        RCCB,
        RFBB,
        RISOB,
        RLMB,
        RLMOB,
        RMLB,
        RMLOB,
        RSIOB,

        LCCM,
        LFBM,
        LISOM,
        LLMM,
        LLMOM,
        LMLM,
        LMLOM,
        LSIOM,
        RCCM,
        RFBM,
        RISOM,
        RLMM,
        RLMOM,
        RMLM,
        RMLOM,
        RSIOM,

        LEN,
        UNDEF = LEN
    };

    static const cli::array<System::String^>^ tags = gcnew cli::array<System::String^> {
        "LCC", "LFB", "LISO", "LLM", "LLMO", "LML", "LMLO", "LSIO", "RCC", "RFB", "RISO", "RLM", "RLMO", "RML", "RMLO", "RSIO",
         "LCCT", "LFBT", "LISOT", "LLMT", "LLMOT", "LMLT", "LMLOT", "LSIOT", "RCCT", "RFBT", "RISOT", "RLMT", "RLMOT", "RMLT", "RMLOT", "RSIOT",
         "LCCD", "LFBD", "LISOD", "LLMD", "LLMOD", "LMLD", "LMLOD", "LSIOD", "RCCD", "RFBD", "RISOD", "RLMD", "RLMOD", "RMLD", "RMLOD", "RSIOD",
         "LCCB", "LFBB", "LISOB", "LLMB", "LLMOB", "LMLB", "LMLOB", "LSIOB", "RCCB", "RFBB", "RISOB", "RLMB", "RLMOB", "RMLB", "RMLOB", "RSIOB",
         "LCCM", "LFBM", "LISOM", "LLMM", "LLMOM", "LMLM", "LMLOM", "LSIOM", "RCCM", "RFBM", "RISOM", "RLMM", "RLMOM", "RMLM", "RMLOM", "RSIOM",
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

    void clrList(void) {
        projection_list->Clear();
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
    static bool isTarget(void) { return ((device->getCurrentEncoderUposition() >= selected_target - 1) && (device->getCurrentEncoderUposition() <= selected_target + 1)); }
    static bool isValidTarget(void) { return valid_target; }
    static bool isInRange(void) { return ((device->getCurrentEncoderUposition() >= allowed_low) && (device->getCurrentEncoderUposition() <= allowed_high)); }
    static bool isValidPosition(void) { return (valid_target && isInRange()); }

    // Arm activation section
    static bool startHoming(void);
    static inline void setManualEnable(bool status) { manual_activation_enabled = status; } //!< Enables / Disables the manual activation mode

    static bool setTarget(int pos, int low, int high, System::String^ proj, int id);
    static void abortTarget(void);
    static bool serviceAutoPosition(int pos);


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

    bool iso_activation_mode; //!< Setting this flag, causes the Vertical motor activation at the Arm rotation completion

    bool initializeSpecificObjectDictionaryCallback(void) override; //!< Sets specific registers for the Arm activation
    

    MotorCompletedCodes automaticPositioningPreparationCallback(void) override;
    MotorCompletedCodes automaticPositioningRunningCallback(void) override;
    void automaticPositioningCompletedCallback(MotorCompletedCodes error) override; //!< Override the basic class to handle the Virtual isocentric function    	

    MotorCompletedCodes manualPositioningPreparationCallback(void) override;
    MotorCompletedCodes manualPositioningRunningCallback(void) override;
    void manualPositioningCompletedCallback(MotorCompletedCodes error) override;
    
    void automaticHomingCompletedCallback(MotorCompletedCodes error) override;

    MotorCompletedCodes idleCallback(void) override;
    void faultCallback(bool errstat, bool data_changed, unsigned int error_class, unsigned int error_code) override;
    void resetCallback(void) override;

private:
    static ProjectionOptions^ projections = gcnew ProjectionOptions;  //!< This is the current selected projection
    static int         allowed_low = 0;            //!< Lower acceptable angle (°)
    static int         allowed_high = 0;           //!< Higher acceptable angle (°)
    static bool        valid_target = false;       //!< True if the target is a valid target
    static int         selected_target;            //!< Automatic selected target  

    static bool manual_activation_enabled = true; //!< This is the flag to enable the manual activation
    static bool manual_increment_direction = false; //!< Sets true if the increment manual command is executing, false if the decrement manual activation is executing

};

