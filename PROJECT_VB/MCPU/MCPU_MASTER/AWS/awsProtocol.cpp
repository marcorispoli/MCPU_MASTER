
#include "../gantry_global_status.h"
#include "awsProtocol.h"
#include "ArmMotor.h"
#include "PCB315.h"
#include "PCB302.h"
#include "PCB303.h"

using namespace GantryStatusRegisters;
using namespace System::Diagnostics;

/// <summary>
/// This is the class constructor.
/// 
/// This function initializes the module.
/// 
/// </summary>
/// <param name="ip">IP4 Address (String format) for the servers</param>
/// <param name="command_port">Command server port address</param>
/// <param name="event_port">Event server port address</param>
awsProtocol::awsProtocol(String^ ip, int command_port, int event_port) {
    pDecodedFrame = gcnew aws_decoded_frame_t;

    // Creates the TcpIp based Servers
    command_server = gcnew TcpIpServerCLI(ip, command_port);
    event_server = gcnew TcpIpServerCLI(ip, event_port);
    event_counter = 1;

    // Connect the reception Events 
    command_server->rxData_event += gcnew TcpIpServerCLI::rxData_slot(this, &awsProtocol::command_rx_handler);
    event_server->rxData_event += gcnew TcpIpServerCLI::rxData_slot(this, &awsProtocol::event_rx_handler);

    // Create the dictionary
    commandExec = gcnew Dictionary<String^, command_callback^>;
    commandExec->Add("EXEC_OpenStudy", gcnew command_callback(this, &awsProtocol::EXEC_OpenStudy));
    commandExec->Add("EXEC_CloseStudy", gcnew command_callback(this, &awsProtocol::EXEC_CloseStudy));
    commandExec->Add("SET_ProjectionList", gcnew command_callback(this, &awsProtocol::SET_ProjectionList));
    commandExec->Add("EXEC_ArmPosition", gcnew command_callback(this, &awsProtocol::EXEC_ArmPosition));
    commandExec->Add("EXEC_AbortProjection", gcnew command_callback(this, &awsProtocol::EXEC_AbortProjection));
    commandExec->Add("EXEC_TrxPosition", gcnew command_callback(this, &awsProtocol::EXEC_TrxPosition));
    commandExec->Add("SET_TomoConfig", gcnew command_callback(this, &awsProtocol::SET_TomoConfig));
    commandExec->Add("SET_ExposureMode", gcnew command_callback(this, &awsProtocol::SET_ExposureMode));
    commandExec->Add("SET_ExposureData", gcnew command_callback(this, &awsProtocol::SET_ExposureData));
    commandExec->Add("SET_EnableXrayPush", gcnew command_callback(this, &awsProtocol::SET_EnableXrayPush));
    commandExec->Add("GET_ReadyForExposure", gcnew command_callback(this, &awsProtocol::GET_ReadyForExposure));
    commandExec->Add("EXEC_StartXraySequence", gcnew command_callback(this, &awsProtocol::EXEC_StartXraySequence));
    commandExec->Add("GET_Compressor", gcnew command_callback(this, &awsProtocol::GET_Compressor));
    commandExec->Add("GET_Components", gcnew command_callback(this, &awsProtocol::GET_Components));
    commandExec->Add("GET_Trx", gcnew command_callback(this, &awsProtocol::GET_Trx));
    commandExec->Add("GET_Arm", gcnew command_callback(this, &awsProtocol::GET_Arm));
    commandExec->Add("GET_TubeTemperature", gcnew command_callback(this, &awsProtocol::GET_TubeTemperature));
    commandExec->Add("SET_Language", gcnew command_callback(this, &awsProtocol::SET_Language));
    commandExec->Add("EXEC_PowerOff", gcnew command_callback(this, &awsProtocol::EXEC_PowerOff));
    commandExec->Add("EXEC_TestCommand", gcnew command_callback(this, &awsProtocol::EXEC_TestCommand));

    // Connects the Global register callbacks to the local Events
    XrayPushButtonRegister::activationStatus->value_change_event += gcnew delegate_void_callback(this, &awsProtocol::xrayPushbuttonStatusChangeCallback);
    ExposureDataRegister::exposure_completed_event += gcnew delegate_void_callback(this, &awsProtocol::exposureSequenceCompletedCallback);
    ArmMotor::command_completed_event += gcnew ArmMotor::delegate_command_completed_callback(this, &awsProtocol::activationCompletedCallback);
    ArmMotor::projection_request_event += gcnew ArmMotor::delegate_projection_request_callback(this, &awsProtocol::selectProjectionCallback);
    ArmMotor::abort_projection_request_event += gcnew ArmMotor::delegate_abort_projection_request_callback(this, &awsProtocol::abortProjectionCallback);

}

/// <summary>
/// This function retrive the next item in the current decoding frame.
/// </summary>
/// <param name="i">This is the character index of the received buffer</param>
/// <param name="sFrame">This is the frame received</param>
/// <param name="result">This is the decoded item</param>
/// <param name="completed">This flag is set if the frame is completed</param>
/// <returns>true if the item is successfully detected</returns>
bool awsProtocol::findNextParam(int* i, String^ sFrame, String^% result, bool* completed) {

    // General pointer test
    if (result == nullptr) return false;
    if (sFrame == nullptr) return false;
    if (i == nullptr) return false;
    if (completed == nullptr) return false;
    if (*i == sFrame->Length) return false;

    // Initialize the result
    result = "";

    // Clear spaces
    while ((sFrame[++(*i)] == L' ') && (*i < sFrame->Length));
    if (*i == sFrame->Length) return false;

    // Found the frame completed character
    if (sFrame[*i] == L'%') {
        *completed = true;
        return true;
    }

    // Finds the String between the " .. "
    if (sFrame[*i] == L'"') {
        (*i)++;
        while (*i < sFrame->Length)
        {
            if (sFrame[*i] == L'"') return true;
            if (sFrame[*i] == L'%') return false;   // Invalid end command
            result += sFrame[*i];
            (*i)++;
        }
        return false;
    }

    // Creates the parameter to be returned
    while (*i < sFrame->Length)
    {
        if (sFrame[*i] == L' ') return true; // End command limiter
        if (sFrame[*i] == L'%') {
            *completed = true;
            return true;
        }
        result += sFrame[*i];
        (*i)++;
    }

    return false;

}


/// <summary>
/// This is the decode frame of the received byte array from the AWS interface
/// </summary>
/// 
/// <param name="buffer">This is the incoming byte streaming </param>
/// <param name="size">This is the size of the received buffer</param>
/// <param name="pDecoded">This is the handler of the decoded result</param>
/// <returns>This is the error code if <0</returns>
int awsProtocol::decodeFrame(cli::array<Byte>^ buffer, int size, aws_decoded_frame_t^% pDecoded) {
    bool frame_complete = false;
    int i;
    String^ param;

    if (pDecoded == nullptr) return -1;
    pDecoded->valid = false;
    pDecoded->errcode = 0;
    pDecoded->errstr = "";

    if (buffer == nullptr) return -2;

    // Decode with Unicode the byte array into a string
    String^ sFrame = System::Text::UTF8Encoding::UTF8->GetString(buffer, 0, size);

    // Finds the init character: '<'
    for (i = 0; i < sFrame->Length; i++) if (sFrame[i] == L'<') break;

    // No initiator matched
    if (i == sFrame->Length) return -3;

    // Clear spaces    
    while ((sFrame[++i] == L' ') && (i < sFrame->Length));

    // Find the ID: ->i points to the first test character
    param = "";
    while (1)
    {
        if (sFrame[i] == L' ') break;
        if (sFrame[i] == L'%') break;
        if ((sFrame[i] > L'9') || (sFrame[i] < L'0')) return -6;
        param += sFrame[i];
        i++;
        if (i == sFrame->Length) return -7;
    }

    // In the standard message format the ID is assigned here
    pDecoded->ID = Convert::ToUInt16(param);

    // Find the '%'
    // <i> is not previously incremented because the character % could be already present
    while (i < sFrame->Length)
    {
        if (sFrame[i] == L'%') break;
        if (sFrame[i] == L' ') i++; // No spaces
        else return -8;
    }
    if (i == sFrame->Length) return -9;

    // Get the next field in the frame
    if (!findNextParam(&i, sFrame, param, &frame_complete)) return -10;

    if (param == L"") return -11;

    // Assignes the command field
    pDecoded->command = param;

    // Create the parameter  list field
    pDecoded->parameters = gcnew List<String^>;
    while (!frame_complete) {
        if (!findNextParam(&i, sFrame, param, &frame_complete)) return -12;
        if (param->Length != 0) pDecoded->parameters->Add(param);
    }

    // Verifies the frame end character: simbolo '>'
    while (++i < sFrame->Length)
    {
        if (sFrame[i] == L' ') continue;
        if (sFrame[i] == L'>') {
            pDecoded->valid = true;
            return 0;
        }

        return -13; // No other symbols are admitted
    }

    // The > symbol has not been found!
    return -14;
}

/// <summary>
/// This is the callback assigned to the command_server buffer reception
/// </summary>
/// <param name="buffer">This is the received byte array</param>
/// <param name="rc">This is the length of the received buffer</param>
void awsProtocol::command_rx_handler(cli::array<Byte>^ buffer, int rc) {
    Debug::WriteLine("Command channel: Buffer Received!\n");

    // Decodes the content of the received frame
    pDecodedFrame->errcode = decodeFrame(buffer, rc, pDecodedFrame);
    if (!pDecodedFrame->valid) return;

    try {
        // Executes the matched function with the command string
        commandExec[pDecodedFrame->command]();
    }
    catch (KeyNotFoundException^) {
        // If the command is not found a NA ack is sent back        
        ackNa();
    }


}

/// <summary>
/// This is the event_server reception callback.
/// 
/// The AWS should never send any data on this channel!
/// </summary>
/// <param name="buffer">This is the received byte array</param>
/// <param name="rc">This is the length of the received buffer</param>
void awsProtocol::event_rx_handler(cli::array<Byte>^ receivbufeBuffer, int rc) {
    Debug::WriteLine("Event channel: Buffer Received!\n");
}



/// <summary>
/// This is the function to acknowledge the AWS command with the NA code 
/// 
/// The NA code is sent back to AWS when a command is not implemented.
/// </summary>
/// <param name=""></param>
void awsProtocol::ackNa(void) {
    String^ answer = "<" + pDecodedFrame->ID.ToString() + " %NA %>";
    command_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    return;
}

/// <summary>
/// This is the function to acknowledge the AWS command with the OK code 
/// 
/// The OK code is sent back to AWS when a command has been successfully executed
/// </summary>
/// <param name=""></param>
void awsProtocol::ackOk(void) {
    String^ answer = "<" + pDecodedFrame->ID.ToString() + " %OK %>";
    command_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

/// <summary>
/// This is the function to acknowledge the AWS command with the OK code 
/// 
/// The OK code is sent back to AWS when a command has been successfully executed
/// </summary>
/// <param name="params">This is a list of optionals parameters</param>
void awsProtocol::ackOk(List<String^>^ params) {
    if (params == nullptr) {
        ackOk();
        return;
    }

    String^ answer = "<" + pDecodedFrame->ID.ToString() + " %OK ";

    for (int i = 0; i < params->Count; i++) {
        answer += params[i] + " ";
    }

    answer += " %>";
    command_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    return;
}

/// <summary>
/// This is the function to acknowledge the AWS command with the NOK code 
/// 
/// The NOK code is sent back to AWS when a command has been aborted.\n
/// The function returns a frame with an error cede and an optional error string.
/// </summary>
/// <param name=""></param>
void awsProtocol::ackNok(void) {

    String^ answer = "<" + pDecodedFrame->ID.ToString() + " %NOK " + pDecodedFrame->errcode.ToString() + " " + pDecodedFrame->errstr + " %>";
    command_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    return;
}

/// <summary>
/// This is the function to acknowledge AWS that the command is executing
/// </summary>
/// <param name=""></param>
void awsProtocol::ackExecuting(void) {
    String^ answer = "<" + pDecodedFrame->ID.ToString() + " %EXECUTING %>";
    command_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    return;
}

/// <summary>
/// This is the function to send an event to AWS to notify a successfully command completion.
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::eventExecutedOk(unsigned short id) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EXECUTED  " + id.ToString() + " OK %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

/// <summary>
/// This is the function to send an event to AWS to notify a successfully command completion.
/// 
/// </summary>
/// <param name="params">This is a list of optionals parameters</param>
void awsProtocol::eventExecutedOk(unsigned short id, List<String^>^ params) {
    event_counter++;
    if (params == nullptr) {
        eventExecutedOk(id);
        return;
    }

    String^ answer = "<" + event_counter.ToString() + " %EXECUTED  " + id.ToString() + " OK ";

    for (int i = 0; i < params->Count; i++) {
        answer += params[i] + " ";
    }

    answer += " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    return;
}

/// <summary>
/// This is the function to send an event to AWS notifying the failed command completion.
/// 
/// The function send an error code to the AWS as a parameter
/// </summary>
/// <param name="errcode">This is the error code notified to the AWS</param>
void awsProtocol::eventExecutedNok(unsigned short id, unsigned short errcode) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EXECUTED  " + id.ToString() + " NOK " + errcode.ToString() + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

/// <summary>
/// This is the function to send an event to AWS notifying the failed command completion.
/// 
/// The function send an error code and an error string to the AWS as parameters
/// </summary>
/// <param name="errcode">This is the error code notified to the AWS</param>
/// <param name="errorstr">This is an error string describing the error event</param>
void awsProtocol::eventExecutedNok(unsigned short id, unsigned short errcode, String^ errorstr) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EXECUTED  " + id.ToString() + " NOK " + errcode.ToString() + " " + errorstr + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}


/// <summary>
/// This function Open the study.
/// 
/// The patient name is assigned and the global gantryOperatingStatusOptions is changed.
/// 
/// The gantryOperatingStatusOptions::status_change_event() is generated 
/// 
/// </summary>
/// <param name=""></param>
void  awsProtocol::EXEC_OpenStudy(void) {
    Debug::WriteLine("EXEC_OpenStudy COMMAND MANAGEMENT");

    if (pDecodedFrame->Count() != 1) { pDecodedFrame->errcode = 2; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }

    // Open the study and assignes the patient name !!!
    if (!OperatingStatusRegister::setOpenStudy(pDecodedFrame->parameters[0])) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "NOT_IN_IDLE_MODE"; ackNok(); return; }

    // With the OPEN Study, the collimator is automatically set to AUTO mode.
    PCB303::setAutoCollimationMode();

    ackOk();
}

/// <summary>
/// This command executes the Close Study command.
/// 
/// The study is closed, and the gantryOperatingStatusOptions::status_change_event() is generated.
///  
/// </summary>
/// <param name=""></param>
void  awsProtocol::EXEC_CloseStudy(void) {
    Debug::WriteLine("EXEC_CloseStudy COMMAND MANAGEMENT");
    if (!OperatingStatusRegister::setCloseStudy()) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    ackOk();

}

/// <summary>
/// This command populate the available projection list.
/// 
/// The projectionOptions::list is updated;
/// 
/// The projectionOptions::lista_change_event() is generated;
/// 
/// </summary>
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
    if ((!pMARM) || (!pMARM->isReady())) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "ARM_BUSY"; ackNok(); return; }
    if (ArmMotor::getProjectionsList()->Value->indexOf(pDecodedFrame->parameters[0]) < 0) { pDecodedFrame->errcode = 4; pDecodedFrame->errstr = "WRONG_PROJECTION"; ackNok(); return; }

    // Parameter 0: Projection code  
    // This function causes the ARM activation. See the ArmStatus::setTarget definition 
    // Parameter 1: target Angle
    // Parameter 2: Low Angle
    // Parameter 3: High Angle
    if (!pMARM->setTarget(
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
/// </summary>
/// <param name=""></param>
void   awsProtocol::SET_ExposureMode(void) {
    Debug::WriteLine("SET_ExposureMode COMMAND MANAGEMENT");

    if (pDecodedFrame->parameters->Count != 6) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (!OperatingStatusRegister::isOPEN()) { pDecodedFrame->errcode = 2; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    // Parameter 0: Exposure type
    if (!ExposureModeRegister::exposureType->Value->setCode(pDecodedFrame->parameters[0])) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_EXPOSURE_TYPE"; ackNok(); return; }

    // Parameter 1: Detector type
    if (!ExposureModeRegister::detector->Value->setCode(pDecodedFrame->parameters[1])) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_DETECTOR_TYPE"; ackNok(); return; }

    // Parameter 2: Compression Mode
    if (!ExposureModeRegister::compressorMode->Value->setCode(pDecodedFrame->parameters[2])) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_COMPRESSION_MODE"; ackNok(); return; }

    // Parameter 3: Collimation Mode
    if (pDecodedFrame->parameters[3] == "COLLI_AUTO") {
        PCB303::setAutoCollimationMode();

    }else if (pDecodedFrame->parameters[3] == "COLLI_CUSTOM") {

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
    
    
    // Parameter 4: Protection Mode
    if (!ExposureModeRegister::protectionMode->Value->setCode(pDecodedFrame->parameters[4])) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_PATIENT_PROTECTION_MODE"; ackNok(); return; }

    // Parameter 5: Arm Mode
    if (!ExposureModeRegister::armMode->Value->setCode(pDecodedFrame->parameters[5])) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_ARM_MODE"; ackNok(); return; }

    ackOk();

    return;
}

/// <summary>
/// This command assignes the exposure parameters for the next Exposure pulse
/// </summary>
/// <param name=""></param>
void   awsProtocol::SET_ExposureData(void) {
    Debug::WriteLine("SET_ExposureData COMMAND MANAGEMENT");

    if (pDecodedFrame->parameters->Count != 4) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (!OperatingStatusRegister::isOPEN()) { pDecodedFrame->errcode = 2; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    // Parameter 0: Pulse sequence (unsigned char)
    // Parameter 1: kV value (double)
    // Parameter 2: mAs value (double)
    // Parameter 3: filter value (FilterOption)
    unsigned char seq = Convert::ToByte(pDecodedFrame->parameters[0]);
    if (ExposureDataRegister::getPulse(seq) == nullptr) { pDecodedFrame->errcode = 3; pDecodedFrame->errstr = "PULSE_SEQ_OUT_OF_RANGE"; ackNok(); return; }

    System::Globalization::CultureInfo^ myInfo = gcnew  System::Globalization::CultureInfo("en-US", false);
    Double kV = Convert::ToDouble(pDecodedFrame->parameters[1], myInfo);
    Double mAs = Convert::ToDouble(pDecodedFrame->parameters[2], myInfo);
    
    // Test the Filter option value
    PCB315::filterMaterialCodes filter = PCB315::getFilterFromTag(pDecodedFrame->parameters[3]);

    if (filter == PCB315::filterMaterialCodes::FILTER_INVALID) {
        pDecodedFrame->errcode = 1; 
        pDecodedFrame->errstr = "INVALID_FILTER"; 
        ackNok(); 
        return;
    }

    if (!ExposureDataRegister::getPulse(seq)->set(kV, mAs, filter)) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_PARAMETERS"; ackNok(); return; }

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

    // Ready for exposure
    ackOk();
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
    if(pMARM)  lista->Add(pMARM->getCurrentPosition().ToString());
    else lista->Add("-");

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
/// This is the EVENT requesting the selection of a given projection.
/// 
/// The projection shall belong to the projections list set by the AWS through 
/// the command SET_ProjectionList.
/// 
/// </summary>
/// <param name="projname">This is the Tag of the requested projection</param>
void awsProtocol::EVENT_SelectProjection(String^ projname) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EVENT_SelectProjection  " + projname + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_AbortProjection(void) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EVENT_AbortProjection %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_GantryStatus(String^ status) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EVENT_GantryStatus  " + status + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_Compressor(unsigned short thick, unsigned short force) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EVENT_Compressor  " + thick.ToString() + "  " + force.ToString() + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_Components(String^ component, String^ paddle, String^ colli_component) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EVENT_Components  " + component + " " + paddle + " " + colli_component + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}
void awsProtocol::EVENT_ReadyForExposure(bool ready, unsigned short code) {
    event_counter++;
    String^ answer;
    if (ready) {
        answer = "<" + event_counter.ToString() + " %EVENT_ReadyForExposure OK 0 %>";
    }
    else {
        answer = "<" + event_counter.ToString() + " %EVENT_ReadyForExposure NOK " + code.ToString() + " %>";
    }

    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_XrayPushButton(String^ status) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EVENT_XrayPushButton  " + status + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_XraySequenceCompleted(String^ result,
    double kv0, double mas0, String^ filter0,
    double kv1, double mas1, String^ filter1,
    double kv2, double mas2, String^ filter2,
    double kv3, double mas3, String^ filter3) {
    event_counter++;
    String^ answer;

    System::Globalization::CultureInfo^ myInfo = gcnew  System::Globalization::CultureInfo("en-US", false);

    answer = "<" + event_counter.ToString() + " %EVENT_XraySequenceCompleted  " + result;
    answer += " " + kv0.ToString(myInfo) + " " + mas0.ToString(myInfo) + " " + filter0;
    answer += " " + kv1.ToString(myInfo) + " " + mas1.ToString(myInfo) + " " + filter1;
    answer += " " + kv2.ToString(myInfo) + " " + mas2.ToString(myInfo) + " " + filter2;
    answer += " " + kv3.ToString(myInfo) + " " + mas3.ToString(myInfo) + " " + filter3;
    answer += " %>";

    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));

}




void awsProtocol::activationCompletedCallback(int id, int error) {
    if (error) eventExecutedNok(id, error);
    else eventExecutedOk(id);
}

void awsProtocol::selectProjectionCallback(String^ proj) {
    EVENT_SelectProjection(proj);
};

void awsProtocol::abortProjectionCallback(void) {
    EVENT_AbortProjection();
};

void awsProtocol::gantryStatusChangeCallback(void) {
    EVENT_GantryStatus(OperatingStatusRegister::getStatus());
}

void awsProtocol::compressorDataChangeCallback(void) {
    EVENT_Compressor(CompressorRegister::getThickness(), CompressorRegister::getForce());
}

void awsProtocol::componentDataChangeCallback(void) {
    EVENT_Components(ComponentRegister::Value->getTag(), CompressorRegister::getPaddle()->Value->getTag(), CollimatorComponentRegister::Value->getTag());
}


void awsProtocol::xrayPushbuttonStatusChangeCallback(void) {
    if (XrayPushButtonRegister::isEnabled()) {
        EVENT_XrayPushButton(XrayPushButtonRegister::activationStatus->getTag());
    }
}

void awsProtocol::exposureSequenceCompletedCallback(void) {
    ExposureCompletedOptions^ exposure = ExposureDataRegister::getExposureComplete();
    String^ UNDEF = PCB315::getTagFromFilter( PCB315::filterMaterialCodes::FILTER_INVALID);

    if (exposure->exposed_pulses == nullptr) {
        EVENT_XraySequenceCompleted(exposure->Value->getTag(),
            0, 0, UNDEF,
            0, 0, UNDEF,
            0, 0, UNDEF,
            0, 0, UNDEF);
        return;
    }

    EVENT_XraySequenceCompleted(exposure->Value->getTag(),
        exposure->exposed_pulses[0]->getKv(), exposure->exposed_pulses[0]->getmAs(), PCB315::getTagFromFilter(exposure->exposed_pulses[0]->getFilter()),
        exposure->exposed_pulses[1]->getKv(), exposure->exposed_pulses[1]->getmAs(), PCB315::getTagFromFilter(exposure->exposed_pulses[1]->getFilter()),
        exposure->exposed_pulses[2]->getKv(), exposure->exposed_pulses[2]->getmAs(), PCB315::getTagFromFilter(exposure->exposed_pulses[2]->getFilter()),
        exposure->exposed_pulses[3]->getKv(), exposure->exposed_pulses[3]->getmAs(), PCB315::getTagFromFilter(exposure->exposed_pulses[3]->getFilter())
    );

}

/// <summary>
/// This command requests for the exposure start 
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::EXEC_TestCommand(void) {
   

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