
#include "../gantry_global_status.h"
#include "awsProtocol.h"
#include "ArmMotor.h"
#include "VerticalMotor.h"
#include "TiltMotor.h"
#include "BodyMotor.h"
#include "SlideMotor.h"
#include "PCB302.h"
#include "PCB303.h"
#include "ExposureModule.h"
#include "Generator.h"
#include "ConfigurationFiles.h"
#include "Notify.h"
#include "Log.h"




using namespace System::Diagnostics;

awsProtocol::awsProtocol(void) {
    System::String^ ip = SystemConfig::Configuration->getParam(SystemConfig::PARAM_AWS_CONNECTIONS)[SystemConfig::PARAM_AWS_CONNECTIONS_IP];
    int cp = System::Convert::ToInt16(SystemConfig::Configuration->getParam(SystemConfig::PARAM_AWS_CONNECTIONS)[SystemConfig::PARAM_AWS_CONNECTIONS_PORT_COMMAND]);
    int ep = System::Convert::ToInt16(SystemConfig::Configuration->getParam(SystemConfig::PARAM_AWS_CONNECTIONS)[SystemConfig::PARAM_AWS_CONNECTIONS_PORT_EVENTS]);
    
    pDecodedFrame = gcnew aws_decoded_frame_t;

    // Creates the TcpIp based Servers
    try {
        command_server = gcnew TcpIpServerCLI(ip, cp);
        event_server = gcnew TcpIpServerCLI(ip, ep);
    }
    catch (...) {
        LogClass::logInFile("awsProtocol: fatal exception in opening the Sockets");
        Notify::activate(Notify::messages::ERROR_AWS_INITIALIZATION);
        return ;
    }
    
    event_counter = 1;

    // Connect the reception Events 
    command_server->rxData_event += gcnew TcpIpServerCLI::rxData_slot(this, &awsProtocol::command_rx_handler);
    event_server->rxData_event += gcnew TcpIpServerCLI::rxData_slot(this, &awsProtocol::event_rx_handler);

    // Create the dictionary
    commandExec = gcnew Dictionary<String^, command_callback^>;
    commandExec->Add("EXEC_OpenStudy", gcnew command_callback(this, &awsProtocol::EXEC_OpenStudy));
    commandExec->Add("EXEC_BiopsyStudy", gcnew command_callback(this, &awsProtocol::EXEC_BiopsyStudy));
    commandExec->Add("EXEC_CloseStudy", gcnew command_callback(this, &awsProtocol::EXEC_CloseStudy));
    commandExec->Add("SET_ProjectionList", gcnew command_callback(this, &awsProtocol::SET_ProjectionList));
    commandExec->Add("EXEC_ArmPosition", gcnew command_callback(this, &awsProtocol::EXEC_ArmPosition));
    commandExec->Add("EXEC_AbortProjection", gcnew command_callback(this, &awsProtocol::EXEC_AbortProjection));
    commandExec->Add("EXEC_TrxPosition", gcnew command_callback(this, &awsProtocol::EXEC_TrxPosition));
    commandExec->Add("SET_TomoConfig", gcnew command_callback(this, &awsProtocol::SET_TomoConfig));
    commandExec->Add("GET_TomoInfo", gcnew command_callback(this, &awsProtocol::GET_TomoInfo));
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
    commandExec->Add("GET_ProtocolRevision", gcnew command_callback(this, &awsProtocol::GET_ProtocolRevision));
    commandExec->Add("SET_Language", gcnew command_callback(this, &awsProtocol::SET_Language));
    commandExec->Add("EXEC_PowerOff", gcnew command_callback(this, &awsProtocol::EXEC_PowerOff));
    commandExec->Add("EXEC_TestCommand", gcnew command_callback(this, &awsProtocol::EXEC_TestCommand));

    commandExec->Add("EXEC_BiopsyHoming", gcnew command_callback(this, &awsProtocol::EXEC_BiopsyHoming));
    commandExec->Add("EXEC_BiopsyParking", gcnew command_callback(this, &awsProtocol::EXEC_BiopsyParking));
    commandExec->Add("EXEC_BiopsyPointing", gcnew command_callback(this, &awsProtocol::EXEC_BiopsyPointing));
    commandExec->Add("SET_BiopsyImage2D", gcnew command_callback(this, &awsProtocol::SET_BiopsyImage2D));
    

    // Connects the Global register callbacks to the local Events        
    ArmMotor::device->command_completed_event += gcnew CANOPEN::CanOpenMotor::delegate_command_completed_callback(&awsProtocol::EVENT_Executed);
    VerticalMotor::device->command_completed_event += gcnew CANOPEN::CanOpenMotor::delegate_command_completed_callback(&awsProtocol::EVENT_Executed);
    TiltMotor::device->command_completed_event += gcnew CANOPEN::CanOpenMotor::delegate_command_completed_callback(&awsProtocol::EVENT_Executed);
    BodyMotor::device->command_completed_event += gcnew CANOPEN::CanOpenMotor::delegate_command_completed_callback(&awsProtocol::EVENT_Executed);
    SlideMotor::device->command_completed_event += gcnew CANOPEN::CanOpenMotor::delegate_command_completed_callback(&awsProtocol::EVENT_Executed);
    ArmMotor::projection_request_event += gcnew ArmMotor::delegate_projection_request_callback(&awsProtocol::EVENT_SelectProjection);
    ArmMotor::abort_projection_request_event += gcnew ArmMotor::delegate_abort_projection_request_callback(&awsProtocol::EVENT_AbortProjection);

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
            LogClass::logInFile("RX-AWS: " + sFrame + "\n");
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
  
    // Decodes the content of the received frame
    pDecodedFrame->errcode = decodeFrame(buffer, rc, pDecodedFrame);
    if (!pDecodedFrame->valid) return;

    // The Id shall be greater then 0!
    if (pDecodedFrame->ID == 0) {
        ackNa();
        return;
    }

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
    LogClass::logInFile("Event channel: Buffer Received!\n");
}



/// <summary>
/// This is the function to acknowledge the AWS command with the NA code 
/// 
/// The NA code is sent back to AWS when a command is not implemented.
/// </summary>
/// <param name=""></param>
void awsProtocol::ackNa(void) {
    if (!command_server) return;
    String^ answer = "<" + pDecodedFrame->ID.ToString() + " %NA %>";
    command_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
    return;
}

/// <summary>
/// This is the function to acknowledge the AWS command with the OK code 
/// 
/// The OK code is sent back to AWS when a command has been successfully executed
/// </summary>
/// <param name=""></param>
void awsProtocol::ackOk(void) {
    if (!command_server) return;
    String^ answer = "<" + pDecodedFrame->ID.ToString() + " %OK %>";
    command_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
}

/// <summary>
/// This is the function to acknowledge the AWS command with the OK code 
/// 
/// The OK code is sent back to AWS when a command has been successfully executed
/// </summary>
/// <param name="params">This is a list of optionals parameters</param>
void awsProtocol::ackOk(List<String^>^ params) {
    if (!command_server) return;

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
    LogClass::logInFile(answer);
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
    if (!command_server) return;

    String^ answer = "<" + pDecodedFrame->ID.ToString() + " %NOK " + pDecodedFrame->errcode.ToString() + " " + pDecodedFrame->errstr + " %>";
    command_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
    return;
}

/// <summary>
/// This is the function to acknowledge AWS that the command is executing
/// </summary>
/// <param name=""></param>
void awsProtocol::ackExecuting(void) {
    if (!command_server) return;
    String^ answer = "<" + pDecodedFrame->ID.ToString() + " %EXECUTING %>";
    command_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
    return;
}

/// <summary>
/// This is the function to send an event to AWS to notify a successfully command completion.
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::eventExecutedOk(unsigned short id) {
    if (!event_server) return;
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EXECUTED  " + id.ToString() + " OK %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
}

/// <summary>
/// This is the function to send an event to AWS to notify a successfully command completion.
/// 
/// </summary>
/// <param name="params">This is a list of optionals parameters</param>
void awsProtocol::eventExecutedOk(unsigned short id, List<String^>^ params) {
    if (!event_server) return;

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
    LogClass::logInFile(answer);
    return;
}

/// <summary>
/// This is the function to send an event to AWS notifying the failed command completion.
/// 
/// The function send an error code to the AWS as a parameter
/// </summary>
/// <param name="errcode">This is the error code notified to the AWS</param>
void awsProtocol::eventExecutedNok(unsigned short id, unsigned short errcode) {
    if (!event_server) return;

    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EXECUTED  " + id.ToString() + " NOK " + errcode.ToString() + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
}

/// <summary>
/// This is the function to send an event to AWS notifying the failed command completion.
/// 
/// The function send an error code and an error string to the AWS as parameters
/// </summary>
/// <param name="errcode">This is the error code notified to the AWS</param>
/// <param name="errorstr">This is an error string describing the error event</param>
void awsProtocol::eventExecutedNok(unsigned short id, unsigned short errcode, String^ errorstr) {
    if (!event_server) return;
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EXECUTED  " + id.ToString() + " NOK " + errcode.ToString() + " " + errorstr + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
}
