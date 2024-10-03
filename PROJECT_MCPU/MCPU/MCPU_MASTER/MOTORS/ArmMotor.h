#pragma once
#include "CanOpenMotor.h"
#include "gantry_global_status.h"

/**
    \addtogroup MOTDESC
    <div style="page-break-after: always;"></div>


    \section CARM C-ARM Rotation Description

    ## Abstract 

    This section describes the feature of the C-ARM motor control.
    The section describes in detail the performances, the activation modes,
    the performances and the limitations.


    ## Overview

    The Gantry provides the C-ARM rotation tool mainly to allow 
    the operator to select a projection for an incoming exposure.

    The projection selection is a mandatory condition to proceed with 
    an exposure. 

    The only method to select a projection and rotating the C-ARM to the angle 
    assigned to that projection is through the AWS command (see the AWS protocol documentation \ref EXEC_ArmPosition).

    Although the C-ARM shall be activated only by the AWS protocol, a manual activation can always be allowed
    in OpenStudy mode: the manual positioning may be used to change the standard 
    projection angle, but only in a limited range, the range selected by the AWS software.

    When the C-ARM is activated in automatic mode (AWS command), the Vertical motor 
    is activated subsequently to keep the Detector plane at the same level
    befor the C-ARM rotation: this feature is called _Virtual Isocentric Rotation_.

        NOTE: the Virtual isocentric correction is activated only with the AWS command; 
        the manual position doesn't change the Vertical ARM position.

    The current ARM angle and selected projection, when allowed, is displayed on the Gantry Display.

    A Safety startegy is iplemented in order to prevent accidental activation or impacts.

     
    ## Performances

    The C-ARM is controlled with a precision of 0.01 degree.
    Although the precision is 1/100 of the degree, an Automatic activation with the AWS protocol command
    can set the angle only with 1 degree unit.

    The AWS command can select the angle in a range from -180 to 180.

    The actual rotation speed can be changed in the \ref MotorConfig file:
    + The Automatic Acceleration, Deceleration and Speed can be set;
    + The Manual Acceleration, Deceleration and Speed can be set;
    + The Service Acceleration and Speed(for zero setting procedure) can be set;

    ## Position Calibration

    The C-ARM is equipped with a photocell for the Automatic zero setting procedure.
    
    After the system istallation or in the case of malfunction, the absolute position may be lost:
    + In this case a Warning message will be active in the Gantry.
    + No more activation can be performed in this state;
    + The Study cannot be Open;

    In the service panel, the position can be recalibrated, activating the automatic procedure or the manual procedure.
    See the Service Panel / Zero Setting section for details.


    ## Motor activation details

    ### Automatic Positioning

    When the Gantry is set in _Open Study_ operating mode, 
    the C-ARM can be automatic positioned to a target angle with the proper protocol command (\ref EXEC_ArmPosition).

    The Command will set:
    + The projection name;
    + The target Angle;
    + The acceptable position range;

    The Projection name refers to one of the possible selectable projections availables.

    Follows a table of possible projection names:

    PROJECTION FOR 2D|PROJECTION FOR 3D|PROJECTION FOR CESM|PROJECTION FOR COMBO|PROJECTION FOR VI|
    |:--|:--|:--|:--|:--|
    |LCC|LCCT|LCCD|LCCB|LCCM|
    |LFB|LFBT|LFBD|LFBB|LFBM|
    |LISO|LISOT|LISOD|LISOB|LISOM|
    |LLM|LLMT|LLMD|LLMB|LLMM|
    |LLMO|LLMOT|LLMOD|LLMOB|LLMOM|
    |LML|LMLT|LMLD|LMLB|LMLM|
    |LMLO|LMLOT|LMLOD|LMLOB|LMLOM|
    |LSIO|LSIOT|LSIOD|LSIOB|LSIOM|
    |RCC|RCCT|RCCD|RCCB|RCCM|
    |RFB|RFBT|RFBD|RFBB|RFBM|
    |RISO|RISOT|RISOD|RISOB|RISOM|
    |RLM|RLMT|RLMD|RLMB|RLMM|
    |RML|RLMOT|RLMOD|RLMOB|RLMOM|
    |RMLO|RMLT|RMLD|RMLB|RMLM|
    |RSIO|RMLOT|RMLOD|RMLOB|RMLOM|

    The valid projection, however, shall be one present in a list that the AWS should have previously uploaded into Gantry
    with the command \ref SET_ProjectionList.

        NOTE: the target angle and the projection name are arbitrary paired by the AWS application.
        Usually they are related to the ACR views, but this is not a general rule for Gantry.

    The projection selection is mandatory to enable an exposure.
        NOTE: after any exposure, the current projection is invalidated and the AWS shall set it ones more in order to proceed
        with a new exposure.

    Toghether with the target angle and the projection name, the AWS will provide a position _acceptable range_ that Gantry 
    will check in order to validate the current C-ARM position:
    + if the current angle should be out of the acceptable range, the exposure will not be enabled.

    The acceptable range allows the operator to modify the standard projection angle with manual adjustment in a limited range
    without invalidating the current projection. 

    When an Automatic activation is requested, at the end of the C-ARM rotation, 
    the Vertical Motor will adjust (if necessary) the position of the Detector plane 
    in order to keep the compression plane unchanged. 
    This featurew, called <b>Virtual Isocentricity</b>,  allows the operator to set the right position for the patient one time only, 
    for different projection exposures.

    During the C-ARM rotation, no other motorizations can be activated:
        NOTE: only one motor at a time can be activated!
    
    During the Automatic activation, the current rotation angle is displayed on the Gantry displays emphasized.
    

    ### Manual Positioning

    ### Safety Strategy

    The following safety strategy protects against injuries:
    + In Idle state (not in Open Study) the C-ARM rotation is disabled at all.  
        NOTE: In Idle state, the Gantry may be left unattended and consequently may be accidentally activated;

    + During a compression, the C-ARM activation is disabled;
    + In case of driver faut, The C-ARM activation is disabled;
    + During an Automatic rotation (AWS) every manual button or pedalboard activation stops the rotation;
    + During an Automatic rotation (AWS) the obstacle detection stops the rotation;
    + A picture with the projection Icon will result well visible on the Screen,
    remembering to the operator what is the current selected projection, and preventing a wrong breast placement;



*/ 


/// <summary>
/// \defgroup ArmMotor_Module C-ARM Rotation Module Implementation
/// \ingroup MOTIMPL 
/// 
/// This module describes the C-ARM rotation implementation
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
        UNDEF = LEN,
        RESERVED_FOR_INIT
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
    static bool isTarget(void) { return ((device->getCurrentUposition() >= selected_target - 1) && (device->getCurrentUposition() <= selected_target + 1)); }
    static bool isValidTarget(void) { return valid_target; }
    static bool isInRange(void) { return ((device->getCurrentUposition() >= allowed_low) && (device->getCurrentUposition() <= allowed_high)); }
    static bool isValidPosition(void) { return (valid_target && isInRange()); }

    // Arm activation section
    static bool startManualHoming(int target_position);
    static bool startAutoHoming();
    
    static int setTarget(int pos, int low, int high, System::String^ proj, int id);

    static void abortTarget(void);
    static bool serviceAutoPosition(int pos);
    static bool setIdlePosition(void);

public:
    static inline ProjectionOptions^ getProjectionsList() { return projections; }
    static System::String^ getSelectedProjection(void) { return projections->getCurrentProjectionName(); }
    static ProjectionOptions::options getSelectedProjectionCode(void) { return projections->getCurrentProjectionCode(); }

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
    void completedCallback(int id, MotorCommands current_command, int current_position, MotorCompletedCodes term_code) override; 
    unsigned short initializeSpecificObjectDictionaryCallback(void) override; //!< Sets specific registers for the Arm activation    
    void faultCallback(bool errstat, bool data_changed, unsigned int error_class, unsigned int error_code) override;
   

private:
    static ProjectionOptions^ projections = gcnew ProjectionOptions;  //!< This is the current selected projection
    static int         allowed_low = 0;            //!< Lower acceptable angle (°)
    static int         allowed_high = 0;           //!< Higher acceptable angle (°)
    static bool        valid_target = false;       //!< True if the target is a valid target
    static int         selected_target;            //!< Automatic selected target  
   
    
};

