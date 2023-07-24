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
	
	void command_rx_handler(array<Byte>^ buffer, int rc);//!< This is the command-server reception handler
	void event_rx_handler(array<Byte>^ buffer, int rc);//!< This is the event-server reception handler

	bool findNextParam(int* i, String^ sFrame, String^ %result, bool* completed);//!< This function finds the next item in a command frame
	int decodeFrame(array<Byte>^ buffer, int size, aws_decoded_frame_t^ %pDecoded);//!< This function decodes an incoming buffer from the tcp/ip channel
	aws_decoded_frame_t^ pDecodedFrame;//!< This is the decoded command
	unsigned short event_counter;//!< This is the sequence counter of the events

	delegate void command_callback(void); //!< This is the delegate of the function pointer for the command execution functions
	Dictionary<String^, command_callback^>^ commandExec; //!< This is the dictionary of the command that are implemented
	
	
	void ackNa(void);//!< This is the Not Implemented command acknowledge function
	void ackOk(void);	//!< This is the immediate command executed OK
	void ackOk(List<String^>^ params);//!< This is the immediate command executed OK with parameters
	void ackNok(void);	//!< This is the immediate command NOK
	void ackExecuting(void);//!< This is the immediate command EXECUTING
	void eventExecutedOk(void);//!< This is the Command Executed OK event
	void eventExecutedOk(List<String^>^ params);//!< This is the Command Executed OK event with parameters
	void eventExecutedNok(unsigned short errcode);//!< This is the Command Executed NOK event  with an error code
	void eventExecutedNok(unsigned short errcode, String^ errorstr);//!< This is the Command Executed NOK event  with an error code and error string


	/// \defgroup awsProtoCommands AWS Protocol Command set
	/// \ingroup awsModule
	/// @{ 
	
	void EXEC_OpenStudy(void);
	void EXEC_CloseStudy(void);

	
	void SET_ProjectionList(void);
	void EXEC_ArmPosition(void) ;
	void EXEC_AbortProjection(void) ;
	void EXEC_TrxPosition(void) ;
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

	void SET_Language(void) ;
	void EXEC_PowerOff(void) { ackNa(); };
	void AWS_NotRecognizedCommand(void) { ackNa(); };
	/// @}

	/*
	virtual void EVENT_XrayPushButton(bool status) {};
	virtual void EVENT_ReadyForExposure(bool ready, unsigned short code) {};
	virtual void EVENT_XrayPulseCompleted(unsigned char pulse) {};
	virtual void EVENT_XraySequenceCompleted(_xray_completed_status_e result, float kv0, float mAs0, _filter_table_e flt0,
		float kv1 = 0, float mAs1 = 0, _filter_table_e flt1 = FILTER_NA,
		float kv2 = 0, float mAs2 = 0, _filter_table_e flt2 = FILTER_NA,
		float kv3 = 0, float mAs3 = 0, _filter_table_e flt3 = FILTER_NA,
		float kv4 = 0, float mAs4 = 0, _filter_table_e flt4 = FILTER_NA) {};
	virtual void EVENT_GantryStatus(_gantry_operating_status_e status) {};
	virtual void EVENT_SelectProjection(_projection_list_e projname) {};
	virtual void EVENT_AbortProjection(void) {};
	virtual void EVENT_Compressor(unsigned short thick, unsigned short force) {};
	virtual void EVENT_Components(_components_options_e component, _compressor_paddles_e paddle, _colli_components_options_e colli_component) {};
	*/
	/// @}
};

