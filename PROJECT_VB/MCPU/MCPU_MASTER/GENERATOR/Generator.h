#pragma once
#include "TcpIpClientCLI.h"
#include "R2CP_Eth.h"


/// <summary>
/// This Module class implements the communication and control of the the SHFD Device.
/// 
/// </summary>
/// 
/// This module implements the R2CP protocol interface in order to 
/// communicate and control the Generator Device through the external Smart Hub
/// software.
/// 
/// The Feature provided by the module are following described:
/// + Simulator Mode: the module can be activated in simulation mode when required;
///	
/// \ingroup Exposure_Module  
ref class Generator: public TcpClientCLI
{
public:
	Generator(void);
	

	enum class generator_errors {
		GEN_NO_ERRORS = 0,			//!< No error code
		GEN_INVALID_PROCEDURE,		//!< A not valid procedure has been requested
		GEN_INVALID_PARAMS,			//!< A non valid exposure parameter has been detected
		GEN_INVALID_STATUS,			//!< The generator is in a not expected status		
		GEN_COMMUNICATION_ERROR,	//!< A generator command is failed
		GEN_INTERNAL_ERROR,			//!< The generator activated internal error messages
		GEN_BUTTON_RELEASE,			//!< The X-Ray Button has been released 
		GEN_TIMEOUT,			    //!< Timeout generator sequence	
		
		GEN_LAST_ERRCODE
	};

	static void startNormalMode(void);
	static void startSimulatorMode(void);
	static bool isSmartHubConnected(void);
	static bool isGeneratorConnected(void);
	static bool isServiceToolConnected(void);
	inline static bool isGeneratorSetupCompleted(void) { return setup_completed; }
	inline static bool isGeneratorIdle(void) { return idle_status; }
	inline static bool isReadyForExosure(void) { return ready_for_exposure; }
	
	
protected:
	void rxData(cli::array<Byte>^ receiveBuffer, int rc) override;
	virtual void  setXrayEnable(bool stat) {};
	virtual void  exposureManagementLoop(bool demo) ;
	static System::String^ getGeneratorErrorString(System::String^ errstr);

	bool handleCommandProcessedState(unsigned char* code);
	bool clearSystemMessages(void);
	bool updateGeneratorStatus(void);
	inline static unsigned char getGeneratorStatus(void) {return current_generator_status;}
	generator_errors generatorPulseSequence(System::String^ ExpName, int ms_timeout);
	generator_errors generatorSet3PointDatabank(unsigned char dbId, bool large_focus, float KV, float MAS, int long_pulse, int min_pulse, int max_pulse);
	static Generator^ device;// = gcnew Generator(); //! Self module generation 

private:
	

	static bool simulator_mode = false; //!< This is the current real or simulating mode flag
	Thread^ running_thread;				//!< This is the module worker thread handler
	static CR2CP_Eth* R2CP_Eth;			//!< This is the Tcp/Ip client connection handler with the smart-hub external software

	static bool setup_completed = false; //!< The Setup process has been terminated
	static bool idle_status = false;	  //!< The Generator is in IDLE mode
	static bool ready_for_exposure = false; //!< Ready for exposure
	
	static unsigned char current_generator_status = 255; //!< This is the current generator status 


	static short sendCR2CPData(unsigned char* pMessage, unsigned short  datalength);
    void threadWork(void);
	void simulatorWork();
	void errorLoop(void);
	void serviceToolLoop(void);
	bool generatorIdleLoop(void);
	bool generatorErrorMessagesLoop(void);
	bool connectionTest(void);
	bool generatorInitialization(void);
	bool generatorSetup(void);	
	static float getR10Time(float ms, bool next);
		
};

