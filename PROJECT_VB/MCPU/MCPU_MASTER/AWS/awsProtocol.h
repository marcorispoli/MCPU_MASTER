#pragma once

using namespace System::Collections::Generic;

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

	/// \defgroup awsProtoApi AWS Protocol Api 
	/// \ingroup awsModule
	/// @{ 


	/// This is the class constructor
	awsProtocol(String^ ip, int command_port, int event_port);

	/// This function returns the command server TcpIpServerCLI class handler
	TcpIpServerCLI^ getCommmandHandler(void) { return command_server; };

	/// This function returns the event server TcpIpServerCLI class handler
	TcpIpServerCLI^ getEventHandler(void) { return event_server; };

	inline bool isConnected(void) {
		return (command_server->isConnected() && event_server->isConnected());
	}


	/// @}

	/// \defgroup awsProtoCallback AWS Protocol Callbacks
	/// 
	/// This section describes the callbacks that allows the module
	/// to cooperate with other modules
	/// 
	/// \ingroup awsModule
	/// @{ 
	/// 

	/// This function shall be connected to command completes events
	void activationCompletedCallback(unsigned short id, int error);

	/// This function shall be connected to the user select projection event
	void selectProjectionCallback(String^ proj);

	/// This function shall be connected to the abort projection event
	void abortProjectionCallback(void);

	/// This function shall be connected to the Gantry status change
	void gantryStatusChangeCallback(void);

	/// This function shall be connected to the Compressor data change events
	void compressorDataChangeCallback(void);

	/// This function shall be connected to the system component's change events
	void componentDataChangeCallback(void);



	/// This function shall be connected to the push button status change event
	void xrayPushbuttonStatusChangeCallback(void);

	/// This function shall be connected to the ready for exposure change event
	void exposureReadyChangeCallback(void);

	/// This function shall be connected to the xray completed event
	void exposureSequenceCompletedCallback(void);

	/// This function shall be connected to the xray pulse completed event
	void exposurePulseCompletedCallback(unsigned char npulse) {};

	/// This function shall be connected to the operating status change event
	void operatingStatusChangeCallback(void) {};



	/// @}

private:


	///  This structure stores the decoded command
	ref struct aws_decoded_frame_t {
		bool valid; //!< The decoded frame is valid
		unsigned short ID;//!< This is the command identifier

		String^ command;//!< This is the command code
		List<String^>^ parameters;//!< This is the list of command's parameters

		int errcode;//!< This is the error code assigned to the command
		String^ errstr;//!<This is the error string assigned to the command

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

	void command_rx_handler(cli::array<Byte>^ buffer, int rc);//!< This is the command-server reception handler
	void event_rx_handler(cli::array<Byte>^ buffer, int rc);//!< This is the event-server reception handler

	bool findNextParam(int* i, String^ sFrame, String^% result, bool* completed);//!< This function finds the next item in a command frame
	int decodeFrame(cli::array<Byte>^ buffer, int size, aws_decoded_frame_t^% pDecoded);//!< This function decodes an incoming buffer from the tcp/ip channel
	aws_decoded_frame_t^ pDecodedFrame;//!< This is the decoded command
	unsigned short event_counter;//!< This is the sequence counter of the events

	delegate void command_callback(void); //!< This is the delegate of the function pointer for the command execution functions
	Dictionary<String^, command_callback^>^ commandExec; //!< This is the dictionary of the command that are implemented


	void ackNa(void);//!< This is the Not Implemented command acknowledge function
	void ackOk(void);	//!< This is the immediate command executed OK
	void ackOk(List<String^>^ params);//!< This is the immediate command executed OK with parameters
	void ackNok(void);	//!< This is the immediate command NOK
	void ackExecuting(void);//!< This is the immediate command EXECUTING
	void eventExecutedOk(unsigned short id);//!< This is the Command Executed OK event
	void eventExecutedOk(unsigned short id, List<String^>^ params);//!< This is the Command Executed OK event with parameters
	void eventExecutedNok(unsigned short id, unsigned short errcode);//!< This is the Command Executed NOK event  with an error code
	void eventExecutedNok(unsigned short id, unsigned short errcode, String^ errorstr);//!< This is the Command Executed NOK event  with an error code and error string


	/// \defgroup awsProtoCommands AWS Protocol Command set
	/// \ingroup awsModule
	/// @{ 

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

	/// \defgroup awsProtoEvents AWS Protocol Events
	/// \ingroup awsModule
	/// @{ 
	/// 
	void EVENT_SelectProjection(String^ projname);
	void EVENT_AbortProjection(void);
	void EVENT_GantryStatus(String^ status);
	void EVENT_Compressor(unsigned short thick, unsigned short force);
	void EVENT_Components(String^ component, String^ paddle, String^ colli_component);
	void EVENT_ReadyForExposure(bool ready, unsigned short code);
	void EVENT_XrayPushButton(String^ status);
	void EVENT_XraySequenceCompleted(String^ result,
		double kv0, double mas0, String^ filter0,
		double kv1, double mas1, String^ filter1,
		double kv2, double mas2, String^ filter2,
		double kv3, double mas3, String^ filter3);

	/// @}

	/*


	virtual void EVENT_XrayPulseCompleted(unsigned char pulse) {};

	*/
	/// @}
};


