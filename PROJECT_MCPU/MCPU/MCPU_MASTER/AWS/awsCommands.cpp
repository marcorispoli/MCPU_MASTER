
#include "../gantry_global_status.h"
#include "awsProtocol.h"
#include "ArmMotor.h"
#include "VerticalMotor.h"
#include "TiltMotor.h"
#include "BodyMotor.h"
#include "SlideMotor.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB304.h"
#include "PCB315.h"
#include "PCB326.h"
#include "ExposureModule.h"
#include "Generator.h"
#include "Notify.h"
#include "Log.h"


using namespace System::Diagnostics;


/// <summary>
/// This command shall be sent by AWS to request to Open the Study.
/// 
/// </summary>
/// 
/// The Open Study is necessary to enter the Operating Status.
/// 
/// Frame format:\n
/// <ID % EXEC_OpenStudy "patient_name">
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |patient_name|String|Study's patient name| 
/// 
/// 
/// Possibly returned error codes:
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |return_errors::AWS_RET_SYSTEM_ERRORS| "SYSTEM_ERRORS" | system error condition are presents|
/// |return_errors::AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"| wrong number of parameters (it should be 1)|
/// |return_errors::AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_IDLE_MODE"| the Gantry is not in IDLE status|
/// 
/// <param name=""></param>
void  awsProtocol::EXEC_OpenStudy(void) {

    // Not in error condition !!!
    if (Notify::isError()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_SYSTEM_ERRORS; pDecodedFrame->errstr = "SYSTEM_ERRORS"; ackNok(); return; }

    if (pDecodedFrame->Count() != 1) { pDecodedFrame->errcode = (int) return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    String^ patient_name = pDecodedFrame->parameters[0];

    // Open the study and assignes the patient name !!!
    if (!Gantry::setOpenStudy(patient_name)) { pDecodedFrame->errcode = (int) return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_IDLE_MODE"; ackNok(); return; }

 
    // With the OPEN Study, the collimator is automatically set to AUTO mode.
    PCB303::setAutoCollimationMode();

    TiltMotor::setIdlePosition();

    ackOk();
}

/// <summary>
/// This command shall be sent by AWS to Close a current study and set the Gantry in IDLE operating status.
/// </summary>
/// 
/// Frame format:\n
/// <ID % EXEC_CloseStudy >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-| 
/// 
/// Possibly returned error codes:
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |return_errors::AWS_RET_WRONG_OPERATING_STATUS| "NOT_IN_OPEN_MODE" | The gantry is not in Open Status mode|
/// 
/// <param name=""></param>
void  awsProtocol::EXEC_CloseStudy(void) {
    if (!Gantry::setCloseStudy()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    TiltMotor::setIdlePosition();
    ackOk();

}


/// <summary>
/// This command shall be sent by AWS to set the gantry selectable projections.
/// 
/// </summary>
/// 
/// The AWS provides a set of projection that the operator can select\n
/// from the local displays.
/// 
/// Frame format:\n
/// <ID % SET_ProjectionList proj1, proj2, .. , proj-n>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |proj1..proj-n|Projection name|List of the selectable projections| 
/// 
/// 
/// Possibly returned error codes:
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |return_errors::AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_OPEN_MODE"| the Gantry is not in Open Study operating status|
/// |return_errors::AWS_RET_INVALID_PARAMETER_VALUE|"INVALID_PROJECTION_IN_THE_LIST"|a projection name in the list is not valid|
/// 
/// <param name=""></param>
void awsProtocol::SET_ProjectionList(void) {
    if (!Gantry::isOPERATING()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    if (!ArmMotor::getProjectionsList()->setList(pDecodedFrame->parameters)) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_PROJECTION_IN_THE_LIST"; ackNok(); return; }

    ackOk();
    return;

}

/// <summary>
/// This function activate the ARM to a target position;
/// 
/// The target data in the ArmStatus are updated;
/// 
/// if the target angle is different of more than 1° then the
/// ArmStatus::target_change_event() is generated and the 
/// ARM activation initiates.
/// 
/// </summary>
/// <param name=""></param>

/// <summary>
/// This command shall be sent by AWS to activate the C-ARM to a given projection.
/// 
/// </summary>
/// 
/// In operating mode (Open Study) the AWS controls the ARM angle position using this command. 
/// 
/// 
/// Frame format:\n
/// <ID % EXEC_ArmPosition projection Angle Min Max>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |projection|String|Name of the projection the AWS is selecting| 
/// |Angle|Integer|Degree of the target angle| 
/// |Min|Integer|Degree of the minimum acceptable angle|
/// |Max|Integer|Degree of the maximum acceptable angle|
/// 
/// - Projection name: it shall be present in the list of the selectable projections (see the "SET_ProjectionList" command);
/// - Angle: is the target Angle the AWS assign to the projection. Is up to the AWS to decide what is the right angle.
/// - The Min and the Max value define the acceptable range in the case the operator should manually change the projection angle:
///     - if the actual ARM angle should be < Min or > Max the gantry will reject the Exposure activation;
/// 
/// NOTE: 
/// + the Min shall be < Ange;
/// + the Max shall be > Ange;
/// 
/// Possibly returned error codes:
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |return_errors::AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_OPEN_MODE"| the Gantry is not in Open Study operating status|
/// |return_errors::AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"|the number of parameters is not correct (it should be 4)|
/// |return_errors::AWS_RET_DEVICE_BUSY|"MOTORS_BUSY"|One of the motors is running|
/// |return_errors::AWS_RET_DEVICE_BUSY|"ARM_NOT_READY"|The ARM is not ready to execute an activation|
/// |return_errors::AWS_RET_DATA_NOT_ALLOWED | "WRONG_PROJECTION" | The projection name is not valid or it isn't in the list of selectable projections |
/// |return_errors::AWS_RET_INVALID_PARAMETER_VALUE |  "WRONG_TARGET_DATA" | One of the angle parameter is not correct or out of range |
///
/// The Command always returns a <ID % EXECUTING%> frame in case of success, because 
/// the ARM requires some time to be positioned, even if the ARM should be already in the target position.
///  
/// <param name=""></param>
void awsProtocol::EXEC_ArmPosition(void) {
    if (!Gantry::isOPERATING()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    if (pDecodedFrame->parameters->Count != 4) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if(Gantry::isMotorsActive()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_BUSY; pDecodedFrame->errstr = "MOTORS_BUSY"; ackNok(); return; }
    if ((!ArmMotor::device->isReady())) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_BUSY; pDecodedFrame->errstr = "ARM_NOT_READY"; ackNok(); return; }
    if ((!TiltMotor::isScoutPosition())) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_ERROR; pDecodedFrame->errstr = "TILT_NOT_IN_SCOUT"; ackNok(); return; }

    
    if (!ArmMotor::getProjectionsList()->isValidProjection(pDecodedFrame->parameters[0])) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DATA_NOT_ALLOWED; pDecodedFrame->errstr = "WRONG_PROJECTION"; ackNok(); return; }

    if (!ArmMotor::setTarget(
        Convert::ToInt16(pDecodedFrame->parameters[1]) ,
        Convert::ToInt16(pDecodedFrame->parameters[2]) ,
        Convert::ToInt16(pDecodedFrame->parameters[3]) ,
        pDecodedFrame->parameters[0], // Projection code
        pDecodedFrame->ID)) {
        pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_ERROR; pDecodedFrame->errstr = "DEVICE_ERROR"; ackNok(); return;
    }

    // Always deference the answer 
    ackExecuting();    
    return;

}

/// <summary>
/// This command shall be sent by AWS to invalidate any selected projection.
/// </summary>
/// 
/// When the Gantry executes this command, the current projection is 
/// invalidated:
/// + The exposure cannot be further initiated until a new projection is selected.
/// 
/// The ARM remains in the current position when the projection is invalidated,
/// but the system display will remove the projection icon from the panel.
/// 
/// Frame format:\n
/// <ID % EXEC_AbortProjection >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-| 
/// 
/// Possibly returned error codes:
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |return_errors::AWS_RET_WRONG_OPERATING_STATUS| "NOT_IN_OPEN_MODE" | The gantry is not in Open Status mode|
/// 
/// <param name=""></param>
void awsProtocol::EXEC_AbortProjection(void) {
    if (!Gantry::isOPERATING()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    ArmMotor::abortTarget();
    ackOk();

}

/// <summary>
/// This command shall be sent by AWS to activate the Tilting.
/// 
/// </summary>
/// 
/// 
/// Frame format:\n
/// <ID % EXEC_TrxPosition trx_target>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |trx_target|String|the TRX target position. See table below| 
/// 
/// |TRX TARGET VALUE|DESCRIPTION|
/// |:--|:--|
/// |"SCOUT"| Scout Position |
/// |"BP_R"| Biopsy Right Position |
/// |"BP_L"| Biopsy left Position |
/// |"TOMO_H"| Tomo Home Position |
/// |"TOMO_E"| Tomo Final Position |
/// 
/// Possibly returned error codes:
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |return_errors::AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_OPEN_MODE"| the Gantry is not in Open Study operating status|
/// |return_errors::AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"|the number of parameters is not correct (it should be 1)|
/// |return_errors::AWS_RET_DEVICE_BUSY|"MOTORS_BUSY"|One of the motors is running|
/// |return_errors::AWS_RET_DEVICE_BUSY|"TRX_NOT_READY"|The TRX is not ready to execute an activation|
/// |return_errors::AWS_RET_INVALID_PARAMETER_VALUE |  "INVALID_TARGET" | One of the angle parameter is not correct or out of range |
/// |return_errors::AWS_RET_DEVICE_ERROR |  "DEVICE_ERROR" | The Tilt Device cannot activate the command for an internal reason |
/// 
/// Gantry always returns <ID % EXECUTING%> frame: the TRX start executing;
///  
/// <param name=""></param>
void awsProtocol::EXEC_TrxPosition(void) {
    if (!Gantry::isOPERATING()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    if (pDecodedFrame->parameters->Count != 1) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (Gantry::isMotorsActive()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_BUSY; pDecodedFrame->errstr = "MOTORS_BUSY"; ackNok(); return; }
    if (!TiltMotor::device->isReady()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_BUSY; pDecodedFrame->errstr = "TRX_NOT_READY"; ackNok(); return; }
    TiltMotor::target_options target = TiltMotor::getTargetCode(pDecodedFrame->parameters[0]);
    if(target == TiltMotor::target_options::UNDEF) { pDecodedFrame->errcode = (int) return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_TARGET"; ackNok(); return; }
    if (!TiltMotor::setTarget(target, pDecodedFrame->ID)) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_ERROR; pDecodedFrame->errstr = "DEVICE_ERROR"; ackNok(); return; }

    ackExecuting();
    
    return;
}

/// <summary>
/// This command select the current Tomo sequence
/// from the Tomo Configuration file.
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::SET_TomoConfig(void) {
    if (pDecodedFrame->parameters->Count != 1) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (!Gantry::isOPERATING()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    // Parameter 0: Tomo configiguration selection;
    if(!Exposures::getTomoExposure()->set(pDecodedFrame->parameters[0])) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "WRONG_CONFIGURATION_ID"; ackNok(); return; }
    ackOk();

    return;
}

/// <summary>
/// This command select the Exposure Mode for the incoming Exposure sequence
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::SET_ExposureMode(void) {
    if (pDecodedFrame->parameters->Count != 6) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (!Gantry::isOPERATING()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    String^ exposure_type = pDecodedFrame->parameters[0];
    String^ detector_type = pDecodedFrame->parameters[1];
    String^ compression_mode = pDecodedFrame->parameters[2];
    String^ collimation_mode = pDecodedFrame->parameters[3];
    String^ protection_mode = pDecodedFrame->parameters[4];
    String^ arm_mode = pDecodedFrame->parameters[5];

    // Setes the next exposure mode
    if (exposure_type == "MAN_2D") Exposures::setExposureMode(Exposures::exposure_type_options::MAN_2D);
    else if (exposure_type == "AEC_2D") Exposures::setExposureMode(Exposures::exposure_type_options::AEC_2D);
    else if (exposure_type == "MAN_3D") Exposures::setExposureMode(Exposures::exposure_type_options::MAN_3D);
    else if (exposure_type == "AEC_3D") Exposures::setExposureMode(Exposures::exposure_type_options::AEC_3D);
    else if (exposure_type == "MAN_COMBO") Exposures::setExposureMode(Exposures::exposure_type_options::MAN_COMBO);
    else if (exposure_type == "AEC_COMBO") Exposures::setExposureMode(Exposures::exposure_type_options::AEC_COMBO);
    else if (exposure_type == "MAN_AE") Exposures::setExposureMode(Exposures::exposure_type_options::MAN_AE);
    else if (exposure_type == "AEC_AE") Exposures::setExposureMode(Exposures::exposure_type_options::AEC_AE);
    else { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_EXPOSURE_TYPE"; ackNok(); return; }

    // Sets the detector used for the exposure
    bool detector_found = false;
    for (int i = 0; i < (int)DetectorConfig::detector_model_option::DETECTOR_LIST_SIZE; i++) {
        if (((DetectorConfig::detector_model_option)i).ToString() == detector_type) {
            Exposures::setDetectorType((DetectorConfig::detector_model_option)i);
            detector_found = true;
            break;
        }
    }
    if (!detector_found) {
        Exposures::setDetectorType(DetectorConfig::detector_model_option::GENERIC);
        pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_DETECTOR_TYPE"; ackNok(); return;
    }

    
    // Sets the Compression Mode used for the exposure
    if (compression_mode == "CMP_KEEP") Exposures::setCompressorMode(Exposures::compression_mode_option::CMP_KEEP);
    else if (compression_mode == "CMP_RELEASE") Exposures::setCompressorMode(Exposures::compression_mode_option::CMP_RELEASE);
    else if (compression_mode == "CMP_DISABLE") Exposures::setCompressorMode(Exposures::compression_mode_option::CMP_DISABLE);
    else { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_COMPRESSION_MODE"; ackNok(); return; }
    
    // Collimation Mode
    if (collimation_mode == "COLLI_AUTO") {
        PCB303::setAutoCollimationMode();
    }else if (collimation_mode == "COLLI_CUSTOM") {
        // The PCB303::ColliStandardSelections::COLLI_STANDARD20 is reserved for the custom.
        PCB303::setCustomCollimationMode(PCB303::ColliStandardSelections::COLLI_STANDARD20);
    }
    else {
        // The AWS sets a format related to a given Paddle 
        
        // Gets the code of the paddle to be used as collimation format
        PCB302::paddleCodes paddle = PCB302::getPaddleCode(pDecodedFrame->parameters[3]);
        if (paddle == PCB302::paddleCodes::PADDLE_NOT_DETECTED){
            // The Paddle is not a valid paddle
            pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE;
            pDecodedFrame->errstr = "INVALID_PADDLE"; 
            ackNok(); 
            return;
        }

        // Gets the collimation format associated to the paddle code
        PCB303::ColliStandardSelections format = (PCB303::ColliStandardSelections) PCB302::getPaddleCollimationFormatIndex((int) paddle);
        if (format == PCB303::ColliStandardSelections::COLLI_INVALID_FORMAT) {
            // The paddle is not associated to a valid format
            pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE;
            pDecodedFrame->errstr = "INVALID_COLLIMATION_FORMAT";
            ackNok();
            return;
        }

        // Activate the custom collimation mode
        PCB303::setCustomCollimationMode(format);
    }

    // Patient protection usage 
    if (protection_mode == "PROTECTION_ENA") Exposures::setProtectionMode(Exposures::patient_protection_option::PROTECTION_ENA);
    else if (protection_mode == "PROTECTION_DIS") Exposures::setProtectionMode(Exposures::patient_protection_option::PROTECTION_DIS);
    else { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_PATIENT_PROTECTION_MODE"; ackNok(); return; }

    // Arm Mode setting  
    if (arm_mode == "ARM_ENA") Exposures::setArmMode(Exposures::arm_mode_option::ARM_ENA);
    else if (arm_mode == "ARM_DIS") Exposures::setArmMode(Exposures::arm_mode_option::ARM_DIS);
    else { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_ARM_MODE"; ackNok(); return; }


    ackOk();
    return;
}

/// <summary>
/// This command assignes the exposure parameters for the next Exposure pulse
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::SET_ExposureData(void) {
    if (pDecodedFrame->parameters->Count != 4) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (!Gantry::isOPERATING()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    String^ pulse_param = pDecodedFrame->parameters[0];
    String^ kv_param = pDecodedFrame->parameters[1];
    String^ mAs_param = pDecodedFrame->parameters[2];
    String^ filter_param = pDecodedFrame->parameters[3];

    System::Globalization::CultureInfo^ myInfo = gcnew  System::Globalization::CultureInfo("en-US", false);
    unsigned char pulse_seq = Convert::ToByte(pulse_param);
    Double kV = Convert::ToDouble(kv_param, myInfo);
    Double mAs = Convert::ToDouble(mAs_param, myInfo);
    
    if (!Exposures::setExposurePulse(pulse_seq, gcnew Exposures::exposure_pulse(kV, mAs, PCB315::getFilterFromTag(filter_param)))) {
        pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_PARAMETERS"; ackNok(); return;
    }
   
    ackOk();
    return;
}

/// <summary>
/// This command assignes the exposure parameters for the next Exposure pulse
/// </summary>
/// <param name=""></param>
void   awsProtocol::SET_EnableXrayPush(void) {
    if (pDecodedFrame->parameters->Count != 1) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (!Gantry::isOPERATING()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    
    if (pDecodedFrame->parameters[0] == "ON") Exposures::enableXrayPushButtonEvent(true);
    else  Exposures::enableXrayPushButtonEvent(false);

    ackOk();

    return;
}

/// <summary>
/// This command request for the current status of the ready for exposure
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_ReadyForExposure(void) {    
    if(Notify::isError()) pDecodedFrame->errcode = (int) return_errors::AWS_RET_SYSTEM_ERRORS;
    else if (Notify::isWarning()) pDecodedFrame->errcode = (int)return_errors::AWS_RET_SYSTEM_WARNINGS;
    else    pDecodedFrame->errcode = 0;

    if (pDecodedFrame->errcode) { pDecodedFrame->errstr = "GANTRY_NOT_READY"; ackNok(); return; }

    // Ready for exposure
    ackOk();
    return;
}

/// <summary>
/// This command requests for the exposure start 
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::EXEC_StartXraySequence(void) {
    if (Notify::isError()) pDecodedFrame->errcode = (int)return_errors::AWS_RET_SYSTEM_ERRORS;
    else if (Notify::isWarning()) pDecodedFrame->errcode = (int)return_errors::AWS_RET_SYSTEM_WARNINGS;
    else pDecodedFrame->errcode = 0;

    if (pDecodedFrame->errcode) {
        pDecodedFrame->errstr = "GANTRY_NOT_READY";
        ackNok();
        return;
    }

    
    // Tries to start the sequence
    if (Exposures::startExposure()) ackOk();
    else {
        pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_ERROR;
        pDecodedFrame->errstr = "GENERATOR_ERROR";
        ackNok();
    }

    return;
}


/// <summary>
/// This command requests the Compressor data:
/// 
/// + Compressor Thickness;
/// + Compressore Force;
///  
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_Compressor(void) {
    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;
    lista->Add(PCB302::getThickness().ToString());
    lista->Add(PCB302::getForce().ToString());

    ackOk(lista);
    return;
}

/// <summary>
/// This command requests the component identified by the system.
/// 
/// </summary>
/// 
/// + Potter-Type, Mag Factor, ComprPaddle, ProtectionType, CollimationTool
/// <param name=""></param>
void   awsProtocol::GET_Components(void) {
    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;

    // Potter Type parameter
    if (Biopsy::isBiopsy()) {
        lista->Add("BIOPSY");
    }else  if (PCB302::getMagnifierFactor() != 10) {
        lista->Add("MAGNIFIER");
    }else  {
        lista->Add("POTTER");
    }

    // Magnification factor
    lista->Add(PCB302::getMagnifierFactor().ToString());

    // Compressor paddle
    lista->Add(PCB302::getDetectedPaddleCode().ToString());

    // Protection Type
    if (PCB302::getPatientProtection() == PCB302::PatientProtection::POSITIONED) {
        lista->Add("PROTECTION_POSITIONED");
    }else if (PCB302::getPatientProtection() == PCB302::PatientProtection::SHIFTED) {
        lista->Add("PROTECTION_SHIFTED");
    }else lista->Add("UNDETECTED_PROTECTION");

    // Collimation Tool
    if (PCB315::getComponent() == PCB315::component_options::LEAD_SCREEN)  lista->Add("LEAD_SCREEN");
    else if (PCB315::getComponent() == PCB315::component_options::SPECIMEN)  lista->Add("SPECIMEN");
    else lista->Add("UNDETECTED_COLLIMATOR");


    ackOk(lista);
    return;
}

/// <summary>
/// This command provides the current TRX position:
/// + The Symbolic position;
/// + The actual angle position;
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_Trx(void) {
    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;

    lista->Add(TiltMotor::getTargetPosition().ToString());
    lista->Add(TiltMotor::device->getCurrentPosition().ToString());


    ackOk(lista);
    return;
}

/// <summary>
/// This command provides the current ARM position
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_Arm(void) {
    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;
    lista->Add(ArmMotor::getSelectedProjection());
    lista->Add(ArmMotor::device->getCurrentPosition().ToString());
    
    ackOk(lista);
    return;
}


/// <summary>
/// This command returns the Tube cumeulated energy 
/// for the Anode and the internal Filament and Stator device.
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_TubeTemperature(void) {
    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;

    lista->Add("0"); // To Be Done ..
    lista->Add(PCB315::getBulb().ToString());
    lista->Add(PCB315::getStator().ToString());

    ackOk(lista);
    return;
}

/// <summary>
/// This command sets the GUI language.
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::SET_Language(void) {
    if (Gantry::isOPERATING()) { pDecodedFrame->errcode = pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_CLOSE_MODE"; ackNok(); return; }
    if (pDecodedFrame->parameters->Count != 1) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (!Notify::setLanguage(pDecodedFrame->parameters[0])) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_LANGUAGE"; ackNok(); return; }

    ackOk();
    return;
}

/// <summary>
/// This command requests for the exposure start 
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::EXEC_TestCommand(void) {
    if (pDecodedFrame->parameters[0] == "TILT") {
        LogClass::logInFile("TEST ON TILT MOTOR");
        int pos = System::Convert::ToInt16(pDecodedFrame->parameters[1]);
        int speed = System::Convert::ToInt16(pDecodedFrame->parameters[2]);
        int acc = System::Convert::ToInt16(pDecodedFrame->parameters[3]);
        int dec = System::Convert::ToInt16(pDecodedFrame->parameters[4]);
        TiltMotor::device->activateAutomaticPositioning(0, pos, speed, acc, dec, true);

    }
    else if (pDecodedFrame->parameters[0] == "TOMO") {
        LogClass::logInFile("TEST ON TOMO TILT MOTOR");
        int pos = System::Convert::ToInt16(pDecodedFrame->parameters[1]);
        int speed = System::Convert::ToInt16(pDecodedFrame->parameters[2]);
        int acc = System::Convert::ToInt16(pDecodedFrame->parameters[3]);
        int dec = System::Convert::ToInt16(pDecodedFrame->parameters[4]);
        TiltMotor::activateTomoScan(pos, speed, acc, dec);
    }
    else if (pDecodedFrame->parameters[0] == "SLIDE") {
        LogClass::logInFile("TEST ON SLIDE MOTOR");
        int pos = System::Convert::ToInt16(pDecodedFrame->parameters[1]);
        int speed = System::Convert::ToInt16(pDecodedFrame->parameters[2]);
        int acc = System::Convert::ToInt16(pDecodedFrame->parameters[3]);
        int dec = System::Convert::ToInt16(pDecodedFrame->parameters[4]);
        SlideMotor::device->activateAutomaticPositioning(0, pos, speed, acc, dec, true);
    }
    else if (pDecodedFrame->parameters[0] == "ARM") {
        LogClass::logInFile("TEST ON ARM MOTOR");
        int pos = System::Convert::ToInt16(pDecodedFrame->parameters[1]);
        int speed = System::Convert::ToInt16(pDecodedFrame->parameters[2]);
        int acc = System::Convert::ToInt16(pDecodedFrame->parameters[3]);
        int dec = System::Convert::ToInt16(pDecodedFrame->parameters[4]);
        ArmMotor::device->activateAutomaticPositioning(0, pos, speed, acc, dec, true);
    }
    else if (pDecodedFrame->parameters[0] == "BODY") {
        LogClass::logInFile("TEST ON BODY MOTOR");
        int pos = System::Convert::ToInt16(pDecodedFrame->parameters[1]);
        int speed = System::Convert::ToInt16(pDecodedFrame->parameters[2]);
        int acc = System::Convert::ToInt16(pDecodedFrame->parameters[3]);
        int dec = System::Convert::ToInt16(pDecodedFrame->parameters[4]);
        BodyMotor::device->activateAutomaticPositioning(0, pos, speed, acc, dec, true);
    }
    else if (pDecodedFrame->parameters[0] == "VERTICAL") {
        LogClass::logInFile("TEST ON VERTICAL MOTOR");
        int pos = System::Convert::ToInt16(pDecodedFrame->parameters[1]);
        int speed = System::Convert::ToInt16(pDecodedFrame->parameters[2]);
        int acc = System::Convert::ToInt16(pDecodedFrame->parameters[3]);
        int dec = System::Convert::ToInt16(pDecodedFrame->parameters[4]);
        VerticalMotor::device->activateAutomaticPositioning(0, pos, speed, acc, dec, true);
    }

    return;
}