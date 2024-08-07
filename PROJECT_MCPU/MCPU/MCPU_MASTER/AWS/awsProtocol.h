#pragma once


using namespace System::Collections::Generic;

#include "TcpIpServerCLI.h"
#include "Generator.h"

/// <summary>
/// This is the module implementing the communication protocol with the AWS software.
/// 
/// \defgroup awsModule AWS Interface Module
/// 
/// # Protocol general Description
/// 
/// This module implements the details of the AWS protocol specification document.
/// 
/// The communication takes place with two ethernet channels:
/// - Command channel: the channel where the AWS sends command to the Gantry;
/// - Event channel: the channel where the Gantry sends Status change notify frame to the AWS software.
/// 
/// Both channels are Tcp/Ip based connections where the Gantry acts as Server of the communication 
/// with the following communication parameters:
/// - Command channel: IP = 127.0.0.1, PORT 10000;
/// - Event channel: IP = 127.0.0.1, PORT 10001;
/// 
///		>NOTE: the actual address and port can be changed in the SystemInit.cnf configuration file.
/// 
/// The data frame format are equivalent for both channels:
/// 
/// + COMMAND FRAME: 
///		+ **<ID % EXEC_xxx Param ... %>** for sequence execution commands;
///		+ **<ID % SET_xxx Param ... %>** for setting status commands;
///		+ **<ID % GET_xxx Param ... %>** for getting status commands;
/// 
/// + EVENT FRAME: 
///		+ <ID % EVENT_xxx Param ... %> for status Events notification;
///		+ <ID % EXECUTED OK optional-Param ... %> for execution command successfully completed notifications;
///		+ <ID % EXECUTED NOK optional-Param ... %> for execution command terminated in error notifications;
/// 
/// Only for COMMANDS Gantry will acknowledge with three possible frames:
/// - OK frame: <ID % OK optional-params..>, a command has been successfully executed;
/// - NOK frame:<ID % NOK errcode error_string>, a command has been rejected because of errors;
/// - EXECUTING: <ID % EXECUTING > , a command is executing and will be further notified the command completion (see the EVENTS) 
/// 
///  
/// 
/// 
/// 
/// </summary>

/// <summary>
/// This is the class implementing the AWS Interface Module
/// 
/// \ingroup awsModule
/// 
/// @{
/// </summary>
ref class awsProtocol
{
public:
	/// \defgroup awsErrors AWS Command Error Codes
	/// \ingroup awsModule
	/// @{ 
	
	/// <summary>
	/// The module implements a standard set of error codes that 
	/// can be returned (NOK frames) to the AWS in executing a Command request.
	///  
	/// </summary>
	enum class return_errors {
		AWS_RET_WRONG_PARAMETERS = 1,		//!< [1] The number of the command parameters is invalid
		AWS_RET_WRONG_OPERATING_STATUS,		//!< [2] The current operating status is not valid for the command
		AWS_RET_SYSTEM_ERRORS,				//!< [3] The command cannot be executed with active system errors
		AWS_RET_SYSTEM_WARNINGS,			//!< [4] There are active System warnings
		AWS_RET_INVALID_PARAMETER_FORMAT,	//!< [5] A parameter is not in the expected format
		AWS_RET_INVALID_PARAMETER_VALUE,	//!< [6] A parameter is not in the expected range
		AWS_RET_DATA_NOT_ALLOWED,			//!< [7] The current system setting is not ready to accept the command
		AWS_RET_DEVICE_BUSY,				//!< [8] The target device cannot be activated
		AWS_RET_DEVICE_ERROR,				//!< [9] The Device signaled an error condition in executing the command
	};

	/// @}
	
	/// \defgroup awsProtoApi AWS Protocol Api 
	/// \ingroup awsModule
	/// @{ 


	/// This is the class constructor	
	awsProtocol();
	static awsProtocol^ device = gcnew awsProtocol(); //!< Auto generation declaration

	/// <summary>
	/// This function returns true in case both Command and Event channels are connected.
	/// 
	/// </summary>
	/// <param name="code"></param>
	static inline bool isConnected(void) {
		if (!device->command_server) return false;
		if (!device->event_server) return false;
		return (device->command_server->isConnected() && device->event_server->isConnected());
	}


	/// @}

	/// \defgroup awsProtoEvents AWS Protocol Events
	/// \ingroup awsModule
	/// @{ 
	

	/// This is the event for command terminated notification
	static void EVENT_Executed(int id, int error);

	/// This is the EVENT the gantry sends to AWS to request a projection selection
	static void EVENT_SelectProjection(System::String^ proj);

	/// This is the EVENT the gantry sends to AWS to request a projection abort
	static void EVENT_AbortProjection(void);

	/// This is the EVENT the gantry sends to AWS to notify about its internal operating status
	static void EVENT_GantryStatus(void);

	/// This is the EVENT the gantry sends to AWS to notify about compressor data change
	static void EVENT_Compressor(void);

	/// This is the EVENT the gantry sends to AWS to notify about component data change
	static void EVENT_Components(void);

	/// This is the EVENT the gantry sends to AWS to notify about the current Ready for exposure status
	static void EVENT_ReadyForExposure(bool ready, unsigned short code);

	/// This is the EVENT the gantry sends to AWS to notify about the X-RAY push button activation
	static void EVENT_XrayPushButton(bool status);

	/// This is the EVENT the gantry sends to AWS to notify about the X-RAY sequence terminated
	static void EVENT_XraySequenceCompleted(void);

	/// This is the EVENT the gantry sends to AWS to notify about a x-ray  pulse termination
	static void EVENT_exposurePulseCompleted(unsigned char npulse) ;
	
	static void EVENT_Poweroff(void) ;

	static void EVENT_operatingStatusChange(void) {};

	/// @}

	
private:


	///  This structure stores the decoded command
	ref struct aws_decoded_frame_t {
		bool valid; //!< The decoded frame is valid
		unsigned short ID;//!< This is the command identifier

		System::String^ command;//!< This is the command code
		List<System::String^>^ parameters;//!< This is the list of command's parameters

		int errcode;//!< This is the error code assigned to the command
		System::String^ errstr;//!<This is the error string assigned to the command

		/// <summary>
		/// This function returns the number of decode parameters
		/// 
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		bool Count(void) { if (parameters == nullptr) return 0; else return parameters->Count; }

	};


	TcpIpServerCLI^ event_server; //!< This is the event-server handler of the tcp/ip server implementation class
	TcpIpServerCLI^ command_server;//!< This is the command-server handler of the tcp/ip server implementation class

	void command_rx_handler(cli::array<System::Byte>^ buffer, int rc);//!< This is the command-server reception handler
	void event_rx_handler(cli::array<System::Byte>^ buffer, int rc);//!< This is the event-server reception handler

	bool findNextParam(int* i, System::String^ sFrame, System::String^% result, bool* completed);//!< This function finds the next item in a command frame
	int decodeFrame(cli::array<System::Byte>^ buffer, int size, aws_decoded_frame_t^% pDecoded);//!< This function decodes an incoming buffer from the tcp/ip channel
	aws_decoded_frame_t^ pDecodedFrame;//!< This is the decoded command
	unsigned short event_counter;//!< This is the sequence counter of the events

	delegate void command_callback(void); //!< This is the delegate of the function pointer for the command execution functions
	Dictionary<System::String^, command_callback^>^ commandExec; //!< This is the dictionary of the command that are implemented


	void ackNa(void);//!< This is the Not Implemented command acknowledge function
	void ackOk(void);	//!< This is the immediate command executed OK
	void ackOk(List<System::String^>^ params);//!< This is the immediate command executed OK with parameters
	void ackNok(void);	//!< This is the immediate command NOK
	void ackExecuting(void);//!< This is the immediate command EXECUTING
	void eventExecutedOk(unsigned short id);//!< This is the Command Executed OK event
	void eventExecutedOk(unsigned short id, List<System::String^>^ params);//!< This is the Command Executed OK event with parameters
	void eventExecutedNok(unsigned short id, unsigned short errcode);//!< This is the Command Executed NOK event  with an error code
	void eventExecutedNok(unsigned short id, unsigned short errcode, System::String^ errorstr);//!< This is the Command Executed NOK event  with an error code and error string


	/// \defgroup awsProtoCommands AWS Protocol Command set
	/// \ingroup awsModule
	/// @{ 
	
	void EXEC_TestCommand(void);
	void EXEC_OpenStudy(void);
	void EXEC_CloseStudy(void);


	void SET_ProjectionList(void);
	void EXEC_ArmPosition(void);
	void EXEC_AbortProjection(void);
	void EXEC_TrxPosition(void);
	void SET_TomoConfig(void);
	void SET_ExposureMode(void);
	void SET_ExposureData(void);
	void SET_EnableXrayPush(void);
	void GET_ReadyForExposure(void);
	void EXEC_StartXraySequence(void);

	void GET_Compressor(void);
	void GET_Components(void);
	void GET_Trx(void);
	void GET_Arm(void);
	void GET_TubeTemperature(void);

	void SET_Language(void);
	void EXEC_PowerOff(void) { ackNa(); };
	void AWS_NotRecognizedCommand(void) { ackNa(); };

	/// @}


};


