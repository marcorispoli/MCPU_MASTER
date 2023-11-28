
#include "../gantry_global_status.h"
#include "awsProtocol.h"
#include "ArmMotor.h"
#include "BodyMotor.h"
#include "PCB315.h"
#include "PCB302.h"
#include "PCB303.h"
#include "ExposureModule.h"
#include "Generator.h"


using namespace GantryStatusRegisters;
using namespace System::Diagnostics;


/// <summary>
/// This function Opens the study.
/// </summary>
///  
/// Parameters of the command:
/// + PARAM 0: patient name;
/// 
/// The Operating mode is activated (Open Study) and the patient name is assigned.\n
/// The Collimation mode is set in AUTO mode
/// 
/// NOK Code returned:
/// 
/// + 0: Wrong number of parameters received;
/// + 1: The Gantry is not in IDLE status;
/// 
/// <param name=""></param>
void  awsProtocol::EXEC_OpenStudy(void) {
    Debug::WriteLine("EXEC_OpenStudy COMMAND MANAGEMENT");

    if (pDecodedFrame->Count() != 1) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }

    String^ patient_name = pDecodedFrame->parameters[0];

    // Open the study and assignes the patient name !!!
    if (!OperatingStatusRegister::setOpenStudy(patient_name)) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "NOT_IN_IDLE_MODE"; ackNok(); return; }

    // With the OPEN Study, the collimator is automatically set to AUTO mode.
    PCB303::setAutoCollimationMode();

    ackOk();
}

/// <summary>
/// This command executes the Close Study command.
///  
/// </summary>
/// 
/// Parameters of the command: no parameters
/// 
/// The study is closed, and the Gantry returns in IDLE status.\n
/// 
/// NOK Code returned:
/// 
/// + 0: The Gantry is not in Open Study mode;
/// 
/// <param name=""></param>
void  awsProtocol::EXEC_CloseStudy(void) {
    Debug::WriteLine("EXEC_CloseStudy COMMAND MANAGEMENT");
    if (!OperatingStatusRegister::setCloseStudy()) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    ackOk();

}

/// <summary>
/// This command populates the available projection list.
/// 
/// </summary>
/// 
/// Parameters of the command:
/// - List of projections names
/// 
/// The list of available projections is updated;
/// 
/// NOK Code returned:
/// 
/// + 0: The Gantry is not in Open Study mode;
/// + 1: one or more arguments of the projection list are invalid.
/// 
/// 
/// <param name=""></param>
void awsProtocol::SET_ProjectionList(void) {
    Debug::WriteLine("SET_ProjectionList COMMAND MANAGEMENT");
    if (!OperatingStatusRegister::isOPEN()) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    if (!ArmMotor::getProjectionsList()->Value->setList(pDecodedFrame->parameters)) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_PROJECTION_IN_THE_LIST"; ackNok(); return; }

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
void awsProtocol::EXEC_ArmPosition(void) {
    Debug::WriteLine("EXEC_ArmPosition COMMAND MANAGEMENT");

    if (!OperatingStatusRegister::isOPEN()) { pDecodedFrame->errcode = 5; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    if (pDecodedFrame->parameters->Count != 4) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if ((!ArmMotor::device->isReady())) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "ARM_BUSY"; ackNok(); return; }
    if (ArmMotor::getProjectionsList()->Value->indexOf(pDecodedFrame->parameters[0]) < 0) { pDecodedFrame->errcode = 4; pDecodedFrame->errstr = "WRONG_PROJECTION"; ackNok(); return; }

    // Parameter 0: Projection code  
    // This function causes the ARM activation. See the ArmStatus::setTarget definition 
    // Parameter 1: target Angle
    // Parameter 2: Low Angle
    // Parameter 3: High Angle
    if (!ArmMotor::setTarget(
        Convert::ToInt16(pDecodedFrame->parameters[1]) ,
        Convert::ToInt16(pDecodedFrame->parameters[2]) ,
        Convert::ToInt16(pDecodedFrame->parameters[3]) ,
        pDecodedFrame->parameters[0], // Projection code
        pDecodedFrame->ID)) {
        pDecodedFrame->errcode = 3; pDecodedFrame->errstr = "WRONG_DATA"; ackNok(); return;
    }

    // Always deference the answer 
    ackExecuting();    
    return;

}


/// <summary>
/// This command invalidate any selected projection.
/// 
/// The ARM is not activated with this command!
/// 
/// The ArmStatus::validate_change_event() is generated.
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::EXEC_AbortProjection(void) {
    Debug::WriteLine("EXEC_AbortProjection COMMAND MANAGEMENT");

    if (!OperatingStatusRegister::isOPEN()) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    ArmMotor::abortTarget();
    ackOk();

}

/// <summary>
/// This command executes the TRX activation.
/// 
/// If the TRX target is changed, the TrxStatus::target_change_event() is generated,
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::EXEC_TrxPosition(void) {
    Debug::WriteLine("EXEC_TrxPosition COMMAND MANAGEMENT");

    if (!OperatingStatusRegister::isOPEN()) { pDecodedFrame->errcode = 4; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    if (pDecodedFrame->parameters->Count != 1) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (TrxStatusRegister::isBusy()) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "TRX_BUSY"; ackNok(); return; }

    // Parameter 0: Target TRX 
    if (!TrxStatusRegister::setTarget(pDecodedFrame->parameters[0], pDecodedFrame->ID)) { pDecodedFrame->errcode = 3; pDecodedFrame->errstr = "INVALID_TARGET"; ackNok(); return; }

    if (TrxStatusRegister::isBusy()) ackExecuting();
    else ackOk();

    return;
}

/// <summary>
/// This command select the current Tomo sequence
/// from the Tomo Configuration file.
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::SET_TomoConfig(void) {
    Debug::WriteLine("SET_TomoConfig COMMAND MANAGEMENT");

    if (pDecodedFrame->parameters->Count != 2) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (!OperatingStatusRegister::isOPEN()) { pDecodedFrame->errcode = 3; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    // Parameter 0: Tomo config file id;
    // Parameter 1: Sequence id;
    if (!TomoConfigRegister::select(pDecodedFrame->parameters[0], pDecodedFrame->parameters[1])) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_CONFID_SEQID"; ackNok(); return; }

    ackOk();

    return;
}

/// <summary>
/// This command select the Exposure Mode for the incoming Exposure sequence
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::SET_ExposureMode(void) {
    Debug::WriteLine("SET_ExposureMode COMMAND MANAGEMENT");


    if (pDecodedFrame->parameters->Count != 6) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (!OperatingStatusRegister::isOPEN()) { pDecodedFrame->errcode = 2; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    String^ exposure_type = pDecodedFrame->parameters[0];
    String^ detector_type = pDecodedFrame->parameters[1];
    String^ compression_mode = pDecodedFrame->parameters[2];
    String^ collimation_mode = pDecodedFrame->parameters[3];
    String^ protection_mode = pDecodedFrame->parameters[4];
    String^ arm_mode = pDecodedFrame->parameters[5];

    // Setes the next exposure mode
    if (exposure_type == "MAN_2D") ExposureModule::setExposureMode(ExposureModule::exposure_type_options::MAN_2D);
    else if (exposure_type == "AEC_2D") ExposureModule::setExposureMode(ExposureModule::exposure_type_options::AEC_2D);
    else if (exposure_type == "MAN_3D") ExposureModule::setExposureMode(ExposureModule::exposure_type_options::MAN_3D);
    else if (exposure_type == "AEC_3D") ExposureModule::setExposureMode(ExposureModule::exposure_type_options::AEC_3D);
    else if (exposure_type == "MAN_COMBO") ExposureModule::setExposureMode(ExposureModule::exposure_type_options::MAN_COMBO);
    else if (exposure_type == "AEC_COMBO") ExposureModule::setExposureMode(ExposureModule::exposure_type_options::AEC_COMBO);
    else if (exposure_type == "MAN_AE") ExposureModule::setExposureMode(ExposureModule::exposure_type_options::MAN_AE);
    else if (exposure_type == "AEC_AE") ExposureModule::setExposureMode(ExposureModule::exposure_type_options::AEC_AE);
    else { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_EXPOSURE_TYPE"; ackNok(); return; }

    // Sets the detector used for the exposure
    if (detector_type == "LMAM2V2") ExposureModule::setDetectorType(ExposureModule::detector_model_option::LMAM2V2);
    else if (detector_type == "FDIV2") ExposureModule::setDetectorType(ExposureModule::detector_model_option::FDIV2);
    else { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_DETECTOR_TYPE"; ackNok(); return; }

    // Sets the Compression Mode used for the exposure
    if (compression_mode == "CMP_KEEP") ExposureModule::setCompressorMode(ExposureModule::compression_mode_option::CMP_KEEP);
    else if (compression_mode == "CMP_RELEASE") ExposureModule::setCompressorMode(ExposureModule::compression_mode_option::CMP_RELEASE);
    else if (compression_mode == "CMP_DISABLE") ExposureModule::setCompressorMode(ExposureModule::compression_mode_option::CMP_DISABLE);
    else { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_COMPRESSION_MODE"; ackNok(); return; }
    
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
        int paddle = PCB302::getPaddleCode(pDecodedFrame->parameters[3]);
        if (paddle == -1){  
            // The Paddle is not a valid paddle
            pDecodedFrame->errcode = 1; 
            pDecodedFrame->errstr = "INVALID_PADDLE"; 
            ackNok(); 
            return;
        }

        // Gets the collimation format associated to the paddle code
        int format = PCB302::getPaddleCollimationFormatIndex(paddle);
        if (format <1) {
            // The paddle is not associated to a valid format
            pDecodedFrame->errcode = 1;
            pDecodedFrame->errstr = "INVALID_COLLIMATION_FORMAT";
            ackNok();
            return;
        }

        // Activate the custom collimation mode
        PCB303::setCustomCollimationMode((PCB303::ColliStandardSelections) format);
    }

    // Patient protection usage 
    if (protection_mode == "PROTECTION_ENA") ExposureModule::setProtectionMode(ExposureModule::patient_protection_option::PROTECTION_ENA);
    else if (protection_mode == "PROTECTION_DIS") ExposureModule::setProtectionMode(ExposureModule::patient_protection_option::PROTECTION_DIS);
    else { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_PATIENT_PROTECTION_MODE"; ackNok(); return; }

    // Arm Mode setting  
    if (arm_mode == "ARM_ENA") ExposureModule::setArmMode(ExposureModule::arm_mode_option::ARM_ENA);
    else if (arm_mode == "ARM_DIS") ExposureModule::setArmMode(ExposureModule::arm_mode_option::ARM_DIS);
    else { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_ARM_MODE"; ackNok(); return; }


    ackOk();
    return;
}

/// <summary>
/// This command assignes the exposure parameters for the next Exposure pulse
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::SET_ExposureData(void) {
    Debug::WriteLine("SET_ExposureData COMMAND MANAGEMENT");

    if (pDecodedFrame->parameters->Count != 4) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (!OperatingStatusRegister::isOPEN()) { pDecodedFrame->errcode = 2; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    String^ pulse_param = pDecodedFrame->parameters[0];
    String^ kv_param = pDecodedFrame->parameters[1];
    String^ mAs_param = pDecodedFrame->parameters[2];
    String^ filter_param = pDecodedFrame->parameters[3];

    System::Globalization::CultureInfo^ myInfo = gcnew  System::Globalization::CultureInfo("en-US", false);
    unsigned char pulse_seq = Convert::ToByte(pulse_param);
    Double kV = Convert::ToDouble(kv_param, myInfo);
    Double mAs = Convert::ToDouble(mAs_param, myInfo);
    
    if (!ExposureModule::setExposurePulse(pulse_seq, gcnew ExposureModule::exposure_pulse(kV, mAs, PCB315::getFilterFromTag(filter_param)))) {
        pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_PARAMETERS"; ackNok(); return;
    }
   
    ackOk();
    return;
}

/// <summary>
/// This command assignes the exposure parameters for the next Exposure pulse
/// </summary>
/// <param name=""></param>
void   awsProtocol::SET_EnableXrayPush(void) {
    Debug::WriteLine("SET_EnableXrayPush COMMAND MANAGEMENT");

    if (pDecodedFrame->parameters->Count != 1) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (!OperatingStatusRegister::isOPEN()) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    if (!XrayPushButtonRegister::enableStatus->setCode(pDecodedFrame->parameters[0])) { pDecodedFrame->errcode = 2; pDecodedFrame->errstr = "INVALID_PARAMETERS"; ackNok(); return; }

    ackOk();

    return;
}

/// <summary>
/// This command request for the current status of the ready for exposure
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_ReadyForExposure(void) {
    Debug::WriteLine("GET_ReadyForExposure COMMAND MANAGEMENT");

    //pDecodedFrame->errcode = ReadyForExposureRegister::getNotReadyCode();
    if(Notify::isError()) pDecodedFrame->errcode = 1;
    else if (Notify::isWarning()) pDecodedFrame->errcode = 2;
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
    Debug::WriteLine("EXEC_StartXraySequence COMMAND MANAGEMENT");

    if ((Notify::isError()) || (Notify::isWarning())) {
        pDecodedFrame->errcode = 1;
        pDecodedFrame->errstr = "GANTRY_NOT_READY";
        ackNok();
        return;
    }

    
    // Tries to start the sequence
    if (Generator::startExposure()) ackOk();
    else ackNok();

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
    Debug::WriteLine("GET_Compressor COMMAND MANAGEMENT");

    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;
    lista->Add(CompressorRegister::getThickness().ToString());
    lista->Add(CompressorRegister::getForce().ToString());

    ackOk(lista);
    return;
}

/// <summary>
/// This command requests the component identified by the system.
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_Components(void) {
    Debug::WriteLine("GET_Components COMMAND MANAGEMENT");

    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;
    lista->Add(ComponentRegister::Value->getTag());
    lista->Add(CompressorRegister::getPaddle()->Value->getTag());
    lista->Add(CollimatorComponentRegister::Value->getTag());


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
    Debug::WriteLine("GET_Trx COMMAND MANAGEMENT");

    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;
    lista->Add(TrxStatusRegister::getCurrentTag());
    lista->Add(TrxStatusRegister::getCurrentAngle().ToString());


    ackOk(lista);
    return;
}

/// <summary>
/// This command provides the current ARM position
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_Arm(void) {
    Debug::WriteLine("GET_Arm COMMAND MANAGEMENT");

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
    Debug::WriteLine("GET_TubeTemperature COMMAND MANAGEMENT");

    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;
    // lista->Add(PCB315::getAnode().ToString());
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
    Debug::WriteLine("SET_Language COMMAND MANAGEMENT");

    if (OperatingStatusRegister::isOPEN()) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "NOT_IN_CLOSE_MODE"; ackNok(); return; }
    if (pDecodedFrame->parameters->Count != 1) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (!LanguageRegister::Value->setCode(pDecodedFrame->parameters[0])) { pDecodedFrame->errcode = 2; pDecodedFrame->errstr = "INVALID_LANGUAGE"; ackNok(); return; }

    ackOk();
    return;
}

/// <summary>
/// This command requests for the exposure start 
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::EXEC_TestCommand(void) {
   
    // Reset test
    BodyMotor::device->activateResetEncoderCommand((int) 1);
    return;

    // Body test
    if (pDecodedFrame->parameters->Count == 1) {
        int pos = System::Convert::ToInt16(pDecodedFrame->parameters[0]);
        BodyMotor::device->activateAutomaticPositioning(1, pos, 50, 10, 10);
    }
    return;

    // Filter test
    Debug::WriteLine("EXEC_TestCommand:FILTER");
    if (pDecodedFrame->parameters->Count == 1) {
        if (pDecodedFrame->parameters[0] == "AG") {
            PCB315::setFilterManualMode(PCB315::filterMaterialCodes::FILTER_AG);
        }
        else if (pDecodedFrame->parameters[0] == "AL") {
            PCB315::setFilterManualMode(PCB315::filterMaterialCodes::FILTER_AL);
        }
        else if (pDecodedFrame->parameters[0] == "CU") {
            PCB315::setFilterManualMode(PCB315::filterMaterialCodes::FILTER_CU);
        }
        else if (pDecodedFrame->parameters[0] == "RH") {
            PCB315::setFilterManualMode(PCB315::filterMaterialCodes::FILTER_RH);
        }
        else if (pDecodedFrame->parameters[0] == "MO") {
            PCB315::setFilterManualMode(PCB315::filterMaterialCodes::FILTER_MO);
        }
        else if (pDecodedFrame->parameters[0] == "MIRROR") {
            PCB315::setMirrorMode(true);
        }


    }
    return;

    // Collimator test
    Debug::WriteLine("EXEC_TestCommand: COLLIMATION");
    if (pDecodedFrame->parameters->Count == 1) {
        if (pDecodedFrame->parameters[0] == "OPEN") {
            Debug::WriteLine("COLLI OPEN COMMAND MANAGEMENT");
            PCB303::setOpenCollimationMode();
            ackOk();
            return;
        }else if(pDecodedFrame->parameters[0] == "AUTO") {
            Debug::WriteLine("COLLI OPEN COMMAND MANAGEMENT");
            PCB303::setAutoCollimationMode();
            ackOk();
            return;
        }else if (pDecodedFrame->parameters[0] == "STANDARD1") {
            Debug::WriteLine("COLLI OPEN COMMAND MANAGEMENT");
            PCB303::setCustomCollimationMode(PCB303::ColliStandardSelections::COLLI_STANDARD1);            
            ackOk();
            return;
        }else if (pDecodedFrame->parameters[0] == "STANDARD2") {
            Debug::WriteLine("COLLI OPEN COMMAND MANAGEMENT");
            PCB303::setCustomCollimationMode(PCB303::ColliStandardSelections::COLLI_STANDARD2);
            ackOk();
            return;
        }else if (pDecodedFrame->parameters[0] == "STANDARD3") {
            Debug::WriteLine("COLLI OPEN COMMAND MANAGEMENT");
            PCB303::setCustomCollimationMode(PCB303::ColliStandardSelections::COLLI_STANDARD3);
            ackOk();
            return;
        }

        
        
    }
    else
    {
        if (pDecodedFrame->parameters[0] == "CALIB") {
            unsigned short front = System::Convert::ToUInt16(pDecodedFrame->parameters[1]);
            unsigned short back = System::Convert::ToUInt16(pDecodedFrame->parameters[2]);
            unsigned short left = System::Convert::ToUInt16(pDecodedFrame->parameters[3]);
            unsigned short right = System::Convert::ToUInt16(pDecodedFrame->parameters[4]);            
            unsigned short trap = System::Convert::ToUInt16(pDecodedFrame->parameters[5]);
            PCB303::setCalibrationCollimationMode(gcnew PCB303::formatBlades(front, back, left, right, trap));
            ackOk();
            return;
        }
    }

    return;
}