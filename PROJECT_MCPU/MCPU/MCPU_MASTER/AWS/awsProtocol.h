#pragma once

#define PROTOCOL_REVISION_CODE "1.0.0"


/**
	\defgroup AWSProtocolModule AWS Protocol Communication Implementation
	\ingroup APPIMPL
	\internal

	This section describes the implementation of the protocol communication with the AWS software.
*/


/**
\defgroup AWSProtocolDescription AWS Protocol Communication  Description
\ingroup APPDOC

   <b>PROTOCOL REVISION: 1.0.0</b>

   # Change Log
   
   _Revision 1.0.0_ 

   <b>Added the \ref GET_ProtocolRevision command</b>

   A protocol revision code is added in order to check the current implementation running on Gantry.\n
   The current revision code is also reported in the protocol documentation.\n
   The command \ref GET_ProtocolRevision command is then added to the protocol.

   <b>Added the command \ref GET_TomoInfo command</b>

   The command returns the parameters of a given Tomo sequence.

   <b>Create a list of standard returned error codes</b>

   A Discrete list of the error returned code has been completed.\n
   Now every command can return only one of the predefined error code.
   See the \ref ERRTABLE

   <b>Changed the \ref GET_Components command</b>

   See the command description for details.

   <b>Reviewed the list of the available Detectors</b>

   The list now is:
   - GENERIC;
   - LMAM2V2;
   - FDIV2;
   - DRTECH;
   - VAREX;

   If the AWS should select a different Detector, the GENERIC Detector is automatically selected
   and no error is then generated.
   
	<b>Modified the \ref EXEC_ArmPosition command</b>

	The command now checks if the current position is already at the requested target.
	In the case of already in Target, the command is istantly accepted.

   <div style="page-break-after: always;"></div>
   # Abstract

   This document describes in detail the communication protocol with the AWS software.

   

   # Index

   ## Implemented Commands 

   _Generic Commands_
   +	\ref GET_ProtocolRevision
   +	\ref GET_Compressor
   +	\ref GET_Components
   +	\ref GET_TubeTemperature
   +	\ref SET_Language

   _Study Control_
   +	\ref EXEC_OpenStudy
   +	\ref EXEC_CloseStudy
   
   _Projection and Tube-Arm Control_
   +	\ref SET_ProjectionList
   +	\ref EXEC_ArmPosition
   +	\ref EXEC_AbortProjection
   +    \ref EXEC_TrxPosition
   +	\ref GET_Trx
   +	\ref GET_Arm

   _Exposure Control_
   +	\ref GET_TomoInfo
   +	\ref SET_TomoConfig
   +	\ref SET_ExposureMode
   +	\ref SET_ExposureData
   +	\ref SET_EnableXrayPush
   +	\ref GET_ReadyForExposure
   +	\ref EXEC_StartXraySequence

   ## Implemented Events

   +	\ref EVENT_SelectProjection
   +	\ref EVENT_AbortProjection
   +	\ref EVENT_GantryStatus
   +	\ref EVENT_Compressor
   +	\ref EVENT_Components
   +	\ref EVENT_ReadyForExposure
   +	\ref EVENT_XrayPushButton
   +	\ref EVENT_exposurePulseCompleted
   +	\ref EVENT_XraySequenceCompleted
   +	\ref EVENT_Poweroff


   <div style="page-break-after: always;"></div>

	# Protocol connection description
 
	The communication takes place on two ethernet channels:
	- Command channel: the channel where the AWS sends command to the Gantry;
	- Event channel: the channel where the Gantry sends Status change notify frames to the AWS software.
 
	Both channels are Tcp/Ip based connections where the Gantry acts as Server of the communication.

	The IP and Port address of the sockets are set into the SystemIni.cnf file:
	- The default Command Channel address is: IP = 127.0.0.1, PORT 10000;
	- The default Event Channel address is: IP = 127.0.0.1, PORT 10001;
 
		
	# Protocol frame description

	The frame formats are equivalent for both channels:
	+ Command Frames: frame sent from AWS to Gantry;
	+ Event Frames: frame sents by Gantry to AWS;
 
	## Command Frame Description

	This is the frame the AWS sends to the application to request a service:
			+ <b><ID % EXEC_xxx Param ... %></b> for sequence execution commands;
			+ <b><ID % SET_xxx Param ... %></b> for setting status commands;
			+ <b><ID % GET_xxx Param ... %></b> for getting status commands;
 
	The application acknowledges the COMMAND frames with three possible answer frames:
		 - *OK* frame: <ID % OK optional-params..>, a command has been successfully executed;
		 - *NOK* frame:<ID % NOK errcode error_string>, a command has been rejected because of errors;
		 - *EXECUTING*: <ID % EXECUTING > , a command is executing and will be further notified the command completion (see the EVENTS)

	## Event Frame Description
	
	This is the frame the Gantry sends to the AWS to notify its internal status change or to notify a command completion event.

	+ <b><ID % EVENT_xxx Param ... %></b> for status Events notification;
	+ <b><ID % EXECUTED  CMDID OK optional-Param ... %></b> for execution command successfully completed notifications;
	+ <b><ID % EXECUTED  CMDID NOK optional-Param ... %></b> for execution command terminated in error notifications;
	
	Where:
	+ CMDID: is the Id of the executing command that is terminated;

	The AWS shall not answer to Event frames!
	
	 <div style="page-break-after: always;"></div>
	# Error codes 

	In case the application should answer to a COMMAND with a NOK frame, or in case the Application 
	should send a NOK EVENT, an error code is provided.

	There are a list of possible error codes that the Application can answer:

	\subsection ERRTABLE Error Table 

	|ERROR CODE|VALUE|DESCRIPTION|AWS-ACTION|
	|:--|:--|:--|:--|
	|AWS_RET_WRONG_PARAMETERS|1|The number of the command parameters is invalid|AWS Bug: check the command implementation|
	|AWS_RET_WRONG_OPERATING_STATUS|2|The current operating status is not valid for the command|AWS Bug: check the current operating status|
	|AWS_RET_SYSTEM_ERRORS|3|The command cannot be executed with active system errors|The AWS should handle a system error condition|
	|AWS_RET_SYSTEM_WARNINGS|4|There are active System warnings|The AWS shall check the right condition before to send the command|
	|AWS_RET_INVALID_PARAMETER_FORMAT|5|A parameter is not in the expected format|AWS Bug: check the command implementation|
	|AWS_RET_INVALID_PARAMETER_VALUE|6|A parameter is not in the expected range|AWS Bug: check the command implementation|
	|AWS_RET_DATA_NOT_ALLOWED|7|The current system setting is not ready to accept the command|check the current operating status|
	|AWS_RET_DEVICE_BUSY|8|The target device cannot be activated|The AWS should wait the previous command completioin before to send a new command|
	|AWS_RET_DEVICE_ERROR|9|The Device signaled an error condition in executing the command|The AWS should abort the current workflow|

	\section ProtoProjection Current Selectable Projection Codes

	The AWS can select one of the possible projections of the following table:

	|PROJECTION FOR 2D|PROJECTION FOR 3D|PROJECTION FOR CESM|PROJECTION FOR COMBO|PROJECTION FOR VI|
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
	

*/


using namespace System::Collections::Generic;

#include "TcpIpServerCLI.h"
#include "Generator.h"

/// <summary>
/// This is the class implementing the AWS Interface Module
/// 
/// \ingroup AWSProtocolModule
/// 
/// @{
/// </summary>
ref class awsProtocol
{
public:
	/// \defgroup awsErrors AWS Command Error Codes
	/// \ingroup AWSProtocolModule
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
	/// \ingroup AWSProtocolModule
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
	/// \ingroup AWSProtocolModule
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
	static void EVENT_ReadyForExposure(void);

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
	/// \ingroup AWSProtocolModule
	/// @{ 
	
	void EXEC_TestCommand(void);
	void EXEC_OpenStudy(void);
	void EXEC_CloseStudy(void);


	void SET_ProjectionList(void);
	void EXEC_ArmPosition(void);
	void EXEC_AbortProjection(void);
	void EXEC_TrxPosition(void);
	void SET_TomoConfig(void);
	void GET_TomoInfo(void);
	
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
	void GET_ProtocolRevision(void);

	void SET_Language(void);
	void EXEC_PowerOff(void) { ackNa(); };
	void AWS_NotRecognizedCommand(void) { ackNa(); };

	/// @}


};


