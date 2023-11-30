#pragma once


using namespace System::Collections::Generic;

#include "TcpIpServerCLI.h"
#include "Generator.h"

/// <summary>
/// This is the module implementing the communication protocol with the AWS.
/// 
/// \defgroup awsModule AWS Interface Module
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
	/// Thi is the enumeration class fo rthe command returned error codes
	/// </summary>
	enum class return_errors {
		AWS_RET_WRONG_PARAMETERS = 1,	//!< The number of the command parameters is invalid
		AWS_RET_WRONG_OPERATING_STATUS, //!< The current operating status is not valid for the command
		AWS_RET_SYSTEM_ERRORS,			//!< The command cannot be executed with active system errors
		AWS_RET_SYSTEM_WARNINGS,			//!< There are active System warnings
		AWS_RET_INVALID_PARAMETER_FORMAT,		//!< A parameter is not in the expected format
		AWS_RET_INVALID_PARAMETER_VALUE,		//!< A parameter is not in the expected range
		AWS_RET_DATA_NOT_ALLOWED,				//!< The current system setting is not ready to accept the command
		AWS_RET_DEVICE_BUSY,				//!< The target device cannot be activated
		AWS_RET_DEVICE_ERROR,				//!< The Device signaled an error condition in executing the command
	};

	/// @}
	
	/// \defgroup awsProtoApi AWS Protocol Api 
	/// \ingroup awsModule
	/// @{ 


	/// This is the class constructor	
	awsProtocol();
	static awsProtocol^ device = gcnew awsProtocol(); //!< Auto generation declaration

	static inline bool isConnected(void) {
		return (device->command_server->isConnected() && device->event_server->isConnected());
	}


	/// @}

	/// \defgroup awsProtoEvents AWS Protocol Events
	/// \ingroup awsModule
	/// @{ 
	

	/// This is the protocol activation completed Event when a delayd command termines
	static void EVENT_ActivationCompleted(int id, int error);

	/// This function shall be connected to the user select projection event
	static void EVENT_SelectProjection(System::String^ proj);

	/// This function shall be connected to the abort projection event
	static void EVENT_AbortProjection(void);

	/// This function shall be connected to the Gantry status change
	static void EVENT_GantryStatus(void);

	/// This function shall be connected to the Compressor data change events
	static void EVENT_Compressor(void);

	/// This function shall be connected to the system component's change events
	static void EVENT_Components(void);

	static void EVENT_ReadyForExposure(bool ready, unsigned short code);

	/// This function shall be connected to the push button status change event
	static void EVENT_XrayPushButton(void);


	/// This function shall be connected to the xray completed event
	static void EVENT_XraySequenceCompleted(Generator::exposure_completed_options code);

	/// This function shall be connected to the xray pulse completed event
	static void EVENT_exposurePulseCompleted(unsigned char npulse) {};

	/// This function shall be connected to the operating status change event
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


