#pragma once
#include "TcpIpClientCLI.h"
#include "ExposureModule.h"
#include "R2CP_Eth.h"


/// <summary>
/// \defgroup Generator_Module Generator Module
/// This Module class implements the commands and working thread to 
/// control the Generator Device.
/// </summary>
/// 
/// This module implements the R2CP protocol interface in order to 
/// communicate and control the Generator Device through the external Smart Hub
/// software.
/// 
/// The Feature provided by the module are following described:
/// + Simulator Mode: the module can be activated in simulation mode when required;
///	
/// \ingroup Generator_Module  
ref class Generator: public TcpClientCLI
{
public:
	Generator(void);
	static Generator^ device = gcnew Generator(); //! Self module generation 

	static void startNormalMode(void);
	static void startSimulatorMode(void);

	static bool isSmartHubConnected(void);
	static bool isGeneratorConnected(void);
	static bool isServiceToolConnected(void);
	inline static bool isGeneratorSetupCompleted(void) { return setup_completed; }
	inline static bool isGeneratorIdle(void) { return idle_status; }


	static System::String^ getGeneratorErrorString(System::String^ errstr);	
	static bool startExposure(void);

	
protected:
	void rxData(cli::array<Byte>^ receiveBuffer, int rc) override;

private:
	static bool simulator_mode = false; //!< This is the current real or simulating mode flag
	Thread^ running_thread;				//!< This is the module worker thread handler
	static CR2CP_Eth* R2CP_Eth;			//!< This is the Tcp/Ip client connection handler with the smart-hub external software

	static bool setup_completed = false; //!< The Setup process has been terminated
	static bool idle_status = false;	  //!< The Generator is in IDLE mode
	static bool ready_for_exposure = false; //!< Ready for exposure
	static bool xray_processing = false; //!< The Generator is processing a X-RAY sequence
	static unsigned char current_status = 255; //!< This is the current generator status 


	static short sendCR2CPData(unsigned char* pMessage, unsigned short  datalength);
    void threadWork(void);
	void simulatorWork();
	void errorLoop(void);
	void serviceToolLoop(void);
	bool generatorIdleLoop(void);
	bool generatorSimulatorIdle(void);
	bool generatorErrorMessagesLoop(void);

	bool handleCommandProcessedState(unsigned char* code);
	bool clearSystemMessages(void);
	bool getGeneratorStatus(void);

	bool connectionTest(void);
	bool generatorInitialization(void);
	bool generatorSetup(void);
	

	ExposureModule::exposure_completed_errors pulseSequence(System::String^ ExpName, int ms_timeout);
	ExposureModule::exposure_completed_errors set3PointDatabank(unsigned char dbId, bool large_focus, float KV, float MAS, int long_pulse, int min_pulse, int max_pulse);

	ExposureModule::exposure_completed_errors man_2d_exposure_procedure(void);
	ExposureModule::exposure_completed_errors aec_2d_exposure_procedure(void);
	ExposureModule::exposure_completed_errors man_ae_exposure_procedure(void);
	ExposureModule::exposure_completed_errors aec_ae_exposure_procedure(void);
	ExposureModule::exposure_completed_errors man_3d_exposure_procedure(void);
	ExposureModule::exposure_completed_errors aec_3d_exposure_procedure(void);
	ExposureModule::exposure_completed_errors man_combo_exposure_procedure(void);
	ExposureModule::exposure_completed_errors aec_combo_exposure_procedure(void);
	ExposureModule::exposure_completed_errors man_2d_exposure_procedure_demo(void);
	ExposureModule::exposure_completed_errors aec_2d_exposure_procedure_demo(void);
	ExposureModule::exposure_completed_errors man_ae_exposure_procedure_demo(void);
	ExposureModule::exposure_completed_errors aec_ae_exposure_procedure_demo(void);
	ExposureModule::exposure_completed_errors man_3d_exposure_procedure_demo(void);
	ExposureModule::exposure_completed_errors aec_3d_exposure_procedure_demo(void);
	ExposureModule::exposure_completed_errors man_combo_exposure_procedure_demo(void);
	ExposureModule::exposure_completed_errors aec_combo_exposure_procedure_demo(void);
	ExposureModule::exposure_completed_errors test_exposure_procedure(void);
	void handleExposureActivation(void);
	void setExposedData(unsigned char databank_index, unsigned char pulse_seq, PCB315::filterMaterialCodes ft, unsigned char fc);
	static float getR10Time(float ms, bool next);
		
};

