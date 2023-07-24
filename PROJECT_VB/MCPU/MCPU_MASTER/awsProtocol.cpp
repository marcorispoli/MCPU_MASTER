#include "pch.h"
#include "awsProtocol.h"

using namespace GantryStatusRegisters;

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
    
    
}

/// <summary>
/// This function retrive the next item in the current decoding frame.
/// </summary>
/// <param name="i">This is the character index of the received buffer</param>
/// <param name="sFrame">This is the frame received</param>
/// <param name="result">This is the decoded item</param>
/// <param name="completed">This flag is set if the frame is completed</param>
/// <returns>true if the item is successfully detected</returns>
bool awsProtocol::findNextParam(int* i, String^ sFrame, String^ %result, bool* completed) {

    // General pointer test
    if (result == nullptr) return false;
    if (sFrame == nullptr) return false;
    if (i == nullptr) return false;
    if (completed == nullptr) return false;
    if (*i == sFrame->Length) return false;

    // Initialize the result
    result="";

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
            if (sFrame[*i]  == L'"') return true;
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
int awsProtocol::decodeFrame(array<Byte>^ buffer, int size, aws_decoded_frame_t^ %pDecoded) {
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
    while ((sFrame[++i] == L' ') && (i < sFrame->Length))  ;

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

    if(param == L"") return -11;

    // Assignes the command field
    pDecoded->command = param;
    
    // Create the parameter  list field
    pDecoded->parameters = gcnew List<String^>;
    while (!frame_complete) {
        if (!findNextParam(&i, sFrame, param, &frame_complete)) return -12;
        if (param->Length !=0) pDecoded->parameters->Add(param);
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
void awsProtocol::command_rx_handler(array<Byte>^ buffer, int rc) {
	Debug::WriteLine("Command channel: Buffer Received!\n");
    
    // Decodes the content of the received frame
    pDecodedFrame->errcode = decodeFrame(buffer, rc, pDecodedFrame);
    if (!pDecodedFrame->valid) return;
    
    try {
        // Executes the matched function with the command string
        commandExec[pDecodedFrame->command]();
    }
    catch (KeyNotFoundException^ ) {
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
void awsProtocol::event_rx_handler(array<Byte>^ receivbufeBuffer, int rc) {
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
    String^ answer =  "<" + pDecodedFrame->ID.ToString() + " %OK %>";
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
void awsProtocol::eventExecutedOk(void) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EXECUTED  " + pDecodedFrame->ID.ToString() + " OK %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

/// <summary>
/// This is the function to send an event to AWS to notify a successfully command completion.
/// 
/// </summary>
/// <param name="params">This is a list of optionals parameters</param>
void awsProtocol::eventExecutedOk(List<String^>^ params) {
    event_counter++;
    if (params == nullptr) {
        eventExecutedOk();
        return;
    }

    String^ answer = "<" + event_counter.ToString() + " %EXECUTED  " + pDecodedFrame->ID.ToString() + " OK ";

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
void awsProtocol::eventExecutedNok(unsigned short errcode) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EXECUTED  " + pDecodedFrame->ID.ToString() + " NOK " + pDecodedFrame->errcode.ToString()  + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

/// <summary>
/// This is the function to send an event to AWS notifying the failed command completion.
/// 
/// The function send an error code and an error string to the AWS as parameters
/// </summary>
/// <param name="errcode">This is the error code notified to the AWS</param>
/// <param name="errorstr">This is an error string describing the error event</param>
void awsProtocol::eventExecutedNok(unsigned short errcode, String^ errorstr) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EXECUTED  " + pDecodedFrame->ID.ToString() + " NOK " + pDecodedFrame->errcode.ToString() + " "  + pDecodedFrame->errstr + " %>";
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
 
    if (!OperatingStatusRegister::isIDLE()) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "NOT_IN_IDLE_MODE"; ackNok(); return; }
    if (!ErrorRegister::isError()) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "SYSTEM_IN_ERROR_CONDITION"; ackNok(); return; }
    if (pDecodedFrame->Count() != 1) { pDecodedFrame->errcode = 2; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }

    // Sets the patient name: set it before to set the gantry status !!!
    PatientDataRegister::setName(pDecodedFrame->parameters[0]);

    // Open study initialization
    OperatingStatusRegister::setCode(OperatingStatusRegister::options::GANTRY_OPEN_STUDY);

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

    if (!OperatingStatusRegister::isOPEN()) { pDecodedFrame->errcode = 0; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    
    OperatingStatusRegister::setCode(OperatingStatusRegister::options::GANTRY_IDLE);
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
    
    // Create the projection list
    List<int>^ lista = gcnew List<int>;

    for (int i = 0; i < pDecodedFrame->parameters->Count; i++) {
        lista->Add(Convert::ToInt16(pDecodedFrame->parameters[i]));
    }
    if(!ProjectionRegister::setList(lista)) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_PROJECTION_IN_THE_LIST"; ackNok(); return; }

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
    if (ArmStatusRegister::isBusy()) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "ARM_BUSY"; ackNok(); return; }

    // Parameter 0: Projection code 
    if (!ProjectionRegister::setCode(pDecodedFrame->parameters[0])) { pDecodedFrame->errcode = 4; pDecodedFrame->errstr = "WRONG_PROJECTION"; ackNok(); return; }

    // This function causes the ARM activation. See the ArmStatus::setTarget definition 
    // Parameter 1: target Angle
    // Parameter 2: Low Angle
    // Parameter 3: High Angle
    if(!ArmStatusRegister::setTarget(
        Convert::ToInt16(pDecodedFrame->parameters[1]), 
        Convert::ToInt16(pDecodedFrame->parameters[2]), 
        Convert::ToInt16(pDecodedFrame->parameters[3]), 
        pDecodedFrame->ID)) { pDecodedFrame->errcode = 3; pDecodedFrame->errstr = "WRONG_DATA"; ackNok(); return;
    }

    if (ArmStatusRegister::isBusy()) ackExecuting();
    else ackOk();

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
    ArmStatusRegister::validate(false);
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
    if (ArmStatusRegister::isBusy()) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "TRX_BUSY"; ackNok(); return; }
    
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
    if (!ExposureModeRegister::setCode(pDecodedFrame->parameters[0])) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_EXPOSURE_TYPE"; ackNok(); return; }
    
    // Parameter 1: Detector type
    if (!DetectorTypeRegister::setCode(pDecodedFrame->parameters[1])) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_DETECTOR_TYPE"; ackNok(); return; }

    // Parameter 2: Compression Mode
    if (!CompressionModeRegister::setCode(pDecodedFrame->parameters[2])) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_COMPRESSION_MODE"; ackNok(); return; }

    // Parameter 3: Collimation Mode
    if (!CollimationModeRegister::setCode(pDecodedFrame->parameters[3])) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_COLLIMATION_MODE"; ackNok(); return; }

    // Parameter 4: Protection Mode
    if (!PatientProtectionRegister::setCode(pDecodedFrame->parameters[4])) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_PATIENT_PROTECTION_MODE"; ackNok(); return; }

    // Parameter 5: Arm Mode
    if (!ArmStatusRegister::setMode(pDecodedFrame->parameters[4])) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_PATIENT_PROTECTION_MODE"; ackNok(); return; }

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
    double kV = Convert::ToDouble(pDecodedFrame->parameters[1]);
    double mAs = Convert::ToDouble(pDecodedFrame->parameters[2]);
    String^ filter = pDecodedFrame->parameters[3];

    if (!ExposureDataRegister::setPulse(seq, kV, mAs, filter)) { pDecodedFrame->errcode = 1; pDecodedFrame->errstr = "INVALID_PARAMETERS"; ackNok(); return; }

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

    // Parameter 0: Xray Push enable status
    String^ xray_pushena_stat = pDecodedFrame->parameters[0];

    if (!XrayPushButtonRegister::setEnableCode(xray_pushena_stat)) { pDecodedFrame->errcode = 2; pDecodedFrame->errstr = "INVALID_PARAMETERS"; ackNok(); return; }

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
    
    pDecodedFrame->errcode = ReadyForExposureRegister::getNotReadyCode();
    if (pDecodedFrame->errcode) {  pDecodedFrame->errstr = "GANTRY_NOT_READY"; ackNok(); return; }

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

    if (!ReadyForExposureRegister::requestStartExposure()) {
        pDecodedFrame->errcode = ReadyForExposureRegister::getNotReadyCode();
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
    lista->Add(ComponentRegister::getTag());
    lista->Add(PaddleRegister::getTag());
    lista->Add(CollimatorComponentRegister::getTag());


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
    lista->Add(TrxStatusRegister::getPositionTag());
    lista->Add(TrxStatusRegister::getAngle().ToString());


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
    lista->Add(ProjectionRegister::getTag());
    lista->Add(ArmStatusRegister::getAngle().ToString());


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
    lista->Add(TubeDataRegister::getAnode().ToString());
    lista->Add(TubeDataRegister::getBulb().ToString());
    lista->Add(TubeDataRegister::getStator().ToString());
    lista->Add(TubeDataRegister::getExposures().ToString());

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
    if (!LanguageRegister::setCode(pDecodedFrame->parameters[0])) { pDecodedFrame->errcode = 2; pDecodedFrame->errstr = "INVALID_LANGUAGE"; ackNok(); return; }

    ackOk();
    return;
}


