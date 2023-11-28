#pragma once
#include "TcpIpClientCLI.h"
#include "ExposureModule.h"
#include "R2CP_Eth.h"


ref class Generator: public TcpClientCLI
{
public:
	Generator();
	
	/// <summary>
	/// This Enumeration class describes the possible results of an exposure.
	/// 
	/// + XRAY_SUCCESS: All the expected exposure's pulses has been successfully executed;
	/// + XRAY_PARTIAL_DOSE: the exposure has been partially executed.
	/// + XRAY_NO_DOSE: The exposure is early terminated without dose released to the patient.
	/// + XRAY_EXECUTING: The X-RAY sequence is still running
	/// 
	/// </summary>
	enum class exposure_completed_options {
		XRAY_SUCCESS = 0, //!< XRAY sequence successfully completed
		XRAY_PARTIAL_DOSE = 1,//!< XRAY sequence partially completed
		XRAY_NO_DOSE = 2,//!< XRAY sequence aborted without dose       
	};

	/// <summary>
	/// This class enumerates all the possible x-ray error reasons
	/// 
	/// </summary>
	enum class exposure_completed_errors {
		XRAY_NO_ERRORS = 0,			//!< No error code
		XRAY_INVALID_PROCEDURE,		//!< A not valid procedure has been requested
		XRAY_COMMUNICATION_ERROR,	//!< A generator command is failed
		XRAY_GENERATOR_ERROR,		//!< The generator activated internal error messages
		XRAY_BUTTON_RELEASE,		//!< The X-Ray Button has been released 
		XRAY_FILAMENT_ERROR,		//!< The generator detected a Filament error
		XRAY_KV_ERROR,				//!< The generator detected a kV error
		XRAY_STARTER_ERROR,			//!< The generator detected an Anode Starter error condition
		XRAY_GRID_ERROR,			//!< The Grid device is in error condition
	};

	static bool isSmartHubConnected(void);
	static bool isGeneratorConnected(void);
	static bool startExposure(void);

	inline static bool isGeneratorSetupCompleted(void) { return setup_completed; }
	inline static bool isGeneratorIdle(void) { return idle_status; }
	inline static exposure_completed_errors getExposureError(void) { return xray_exposure_error; }

	delegate void delegate_xray_complete_callback(exposure_completed_options code); //!< This is the delegate of the xr-ray complete event 
	static event delegate_xray_complete_callback^ xray_complete_event;
	
protected:
	static Generator^ generator = gcnew Generator(); //! Self module generation 
	static short sendCR2CPData(unsigned char* pMessage, unsigned short  datalength);
	static CR2CP_Eth* R2CP_Eth;

	void rxData(cli::array<Byte>^ receiveBuffer, int rc) override;
	Thread^ running_thread;//!< This is the worker thread handler
    void threadWork(void);//!< This is the worker thread for the workflow
	
	bool handleCommandProcessedState(unsigned char* code);
	bool connectionTest(void);
	bool generatorInitialization(void);
	bool generatorSetup(void);
	bool clearSystemMessages(void);
	bool generatorIdle(void);
	bool generatorErrorMessagesLoop(void);
	bool setExecutedPulse(ExposureModule::exposure_type_options xray_exposure_type);

	bool man_2d_exposure_procedure(void);
	bool aec_2d_exposure_procedure(void);
	bool man_ae_exposure_procedure(void);
	bool aec_ae_exposure_procedure(void);
	bool man_3d_exposure_procedure(void);
	bool aec_3d_exposure_procedure(void);
	bool man_combo_exposure_procedure(void);
	bool aec_combo_exposure_procedure(void);

	static bool setup_completed = false; //!< The Setup process has been terminated
	static bool idle_status = false;	  //!< The Generator is in IDLE mode
	static bool ready_for_exposure = false; //!< Ready for exposure
	static bool xray_processing = false; //!< The Generator is processing a X-RAY sequence

	static ExposureModule::exposure_type_options xray_exposure_type;
	static exposure_completed_errors xray_exposure_error = exposure_completed_errors::XRAY_NO_ERRORS;

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

};

