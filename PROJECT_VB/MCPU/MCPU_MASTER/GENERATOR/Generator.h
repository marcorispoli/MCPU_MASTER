#pragma once
#include "TcpIpClientCLI.h"
#include "ExposureModule.h"
#include "R2CP_Eth.h"

/// <summary>
/// \defgroup Generator_Module Generator module controller
///   
/// </summary>
 
/// <summary>
/// \ingroup Generator_Module  
/// </summary>
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

	static bool startExposure(void);

	
protected:
	static bool simulator_mode = false;
	
	static short sendCR2CPData(unsigned char* pMessage, unsigned short  datalength);
	static CR2CP_Eth* R2CP_Eth;

	void rxData(cli::array<Byte>^ receiveBuffer, int rc) override;
	Thread^ running_thread;//!< This is the worker thread handler
    void threadWork(void);//!< This is the worker thread for the workflow
	void simulatorWork();
	void errorLoop(void);
	void serviceToolLoop(void);


	bool handleCommandProcessedState(unsigned char* code);
	bool connectionTest(void);
	bool generatorInitialization(void);
	bool generatorSetup(void);
	bool clearSystemMessages(void);
	bool generatorIdle(void);
	bool generatorSimulatorIdle(void);
	bool generatorErrorMessagesLoop(void);
	bool logCurrentStatus(unsigned char status);
	ExposureModule::exposure_completed_errors commonExposurePulseSequence(System::String^ ExpName, bool pre_pulse);
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
	

	static bool setup_completed = false; //!< The Setup process has been terminated
	static bool idle_status = false;	  //!< The Generator is in IDLE mode
	static bool ready_for_exposure = false; //!< Ready for exposure
	static bool xray_processing = false; //!< The Generator is processing a X-RAY sequence

	
	/**
	* @brief This function returns the closed lower or higher time from the reinard R10 scale
	*
	* The Function returns the next R10 value or the Previous R10 value \n
	* from the requested parameter.
	*
	* @param ms: this is the nominal time value
	* @param next: true = next value, false = previous value
	* @return the requested time of the R10 scale
	* @return 0: value not found
	*/
    static float getR10Time(float ms, bool next) {
        static float r10[] = { 1,1.25,1.6,2,2.5,3.2,4,5,6.3,8,10 };
        float mult = 1;

        if (ms >= 10000) mult = 10000;
        else if (ms >= 1000) mult = 1000;
        else if (ms >= 100) mult = 100;
        else if (ms >= 10) mult = 10;
        ms = ms / mult;

        for (int i = 9; i >= 0; i--) {
            if (ms == r10[i]) return r10[i] * mult;
            else if (ms > r10[i]) {
                if (next) return r10[i + 1] * mult;
                else return r10[i] * mult;
            }
        }
        return 0;
    }
	static void setExposedData(unsigned char databank_index, unsigned char pulse_seq, PCB315::filterMaterialCodes ft, unsigned char fc);
		
};

