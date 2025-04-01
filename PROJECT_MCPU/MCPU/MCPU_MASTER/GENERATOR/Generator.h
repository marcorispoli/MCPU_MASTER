#pragma once
#include "TcpIpClientCLI.h"
#include "R2CP_Eth.h"


/// <summary>
/// This Module class implements the communication and control of the the SHFD Device.
/// 
/// \todo Implementazione comando per il power down: email (R: Pending issue SHFR Mamo -26/03/2025 Juan Miguel Andujar Morgado JuanMiguel.Andujar@sedecal.com)
/// \todo Implementazione re-inizializzazione se lo stato del gneratore cambia a seguito di powerdown o emergency o altro)
/// 
/// </summary>
/// \ingroup Exposure_Module
/// 
/// 
/// # Module Dependencies
/// 
/// ## R2CP Installation and setup
/// 
/// The Module runs with the following R2CP settings:
/// 
/// - Smart Hub Installed into C:/OEM;
/// - SQLite database for system messages installed into C:/OEM/AppData/system_messages.sqlite;
/// - SmartHub IP/Port connection: 192.6.1.201 @ 10000;
/// - Smart Hub Node Id: 1;
/// - Generator Node Id: 6;
/// - Service Tool Node Id: 9;
/// - Application Node Id: 17;
/// - Direct Workstation: 1
/// - Detector Workstation: 5
/// 
/// ## Application libraries
/// 
/// The Module uses the SQLite dll files:
/// - The Dll shall be installed into the {$ApplicationExecutable}/SQLITE;
/// - The following dll shall be present into the SQLITE subdir:
///		- SQLite.Designer.dll
///		- SQLite.Interop.dll
///		- System.Data.SQLite.dll
///		- System.Data.SQLite.EF6.dll
///		- System.Data.SQLite.Linq.dll
/// 
/// # Module Usage
/// 
/// The Application shall subclasses this module in order to implement the specific exposure workflow.
/// 
/// The Application shall override the following callbacks:
/// -  setXrayEnable(): this function is called by the base class when the xray_enable hardware signal needs to be set properly;
/// -  exposureManagementLoop(): the base class calls this fuction every 100ms when in ready for the exposure internal status;
/// 
/// The setXrayEnable() should be implemented so that the Application can properly set the external hardware signal;
/// 
/// In the exposureManagementLoop() the Application should implement the:
/// + Exposure Initialization process: detect when the application request a given exposure procedure;
/// + Exposure Data Prepapration: preparing and validating the exposures data;
/// + Exposure Implementation: implementation of a given exposure procedures; 
/// + Exposure Termination: handling the exposure termination;
/// 
/// # Generator Anodic Current and mAs handling
/// 
/// ## Integration Time
/// The Dose is controlled integrating a given current for a time.
/// The integration time is set in discrete time slots belonging to the R20 table.
/// + R20 Slots: 12.5ms, 16ms,20ms,25ms,32ms,40ms,50ms,64ms,80ms,100ms,125ms .....
/// 
/// ## Anodic Current contol
/// 
/// The Generator device accuratelly controls the Anodic current level during the exposure.
/// Based on the R20 discrete time slots, an XRAY tube load curve is discretized getting the maximum 
/// anodic current that can be used for a given integration time.
/// 
/// ## mAs integration in two point exposure control
/// 
/// In two point exposure control mode, the application selects the kV and the mAs.
/// Because the Generator doesn't measure mAs directly, but measure current and time,
/// and because the integration time is not arbitrary  but executed in discrete temporal slots,
/// the generator finds the best combination from time slot, maximum current that provides the 
/// Time * I value most closed to the requested mAs.  
/// 
///  
///	## mAs integration in three point exposure control
/// 
/// In three point exposure, the application selects the anodic current,
/// the integration time and the kV. The mAs in this scenario is a consequence 
/// of the selection.
/// 
/// ## Anodic Calculator Tool
/// 
/// The Generator module provides a convenient function that can be invoked 
/// for service. This function provides, in two point tech, the whole table 
/// of anodic current related to kV and mAs. 
/// 
/// The function generates a text file with one coloumn for any kV and 640 rows (one row for any mAs) 
/// with the anodic current at the selected kV and mAs.
/// 
/// The Function takes several seconds to complete as polls the generator about 640 * 29 
/// different combinations.
/// 
/// # Generator and the Emergency button management 
/// 
/// ## Generator Bootup Scenario
/// 
/// When the generator boots up, the internal status is set to Initialization.
/// During this phase the generator Opens the Power Contactor coil switch until the boot up is completed.
/// When the boot up completes, the generator closes the switch.
/// 
/// If the DC bus should be at low level or absent, the generator activates the alarm 1404xx and quickly opens the coil switch.
/// This scenario can be replicated even if the Emergency button should be Open during the Generator startup.
/// 
/// \important The only exit strategy in this error scenario is to Warn the operator to check the Emergency Buttons 
/// and Restart the Unit. Actually a real electric problem may keep down the Bus so this scenario cannot be self resetted by software.
/// 
/// ## Generator in Standby
/// 
/// If the system should startup with the Emergency buttons not activated, and no dc bus issues are detected,
/// the generator terminates its startup properly. The Standby internal status is then activated.
/// 
/// In the case a DC bus issue should be detected, whetehr is a true dc bus problem or the Emergency Button activation,
/// the Generator doesn't open the power contactor coil switch.
/// 
/// In this scenario, ence, the Application can properly detect the emergency button activation 
/// monitoring the coil circuit (PCB301).
/// 
/// The Application may reset the generator error condition as soon as the Emergency button 
/// should be detected closed again, resetting the system error.
///  
/// 
/// # Module General description
/// 
/// This module implements the R2CP protocol interface in order to 
/// communicate and control the Generator Device through the external Smart Hub
/// software.
/// 
/// The Feature provided by this module are following described:
/// + Simulator Mode/ Exposure Mode: the module provides the simulation mode activation.\n
/// In simulation mode the presence of the Generator device is not necessary (nor the Smart Hub software);
/// + Connection with the Smart Hub: the module keeps the connection alive and handle possible disconnections;
/// + External Service Tool presence detection: the module handles the presence of the service tool software;
/// + System Message Handling: the module handles the presence of the persistent system messages;
/// 
/// # Internal Module Status
/// 
/// The Generator module can be in one of the following internal status:
/// + Idle Status: the generator completed succesfully the initialization.
///		+ The communication with the Smart Hub and the Generator device is working;
///	+ Ready For Exposure: the generator is ready to init a pulse sequence.
///		+ The Ready for exposure is set when the Generator is in Idle and no system message are presents
/// 
/// # External Service Tool sotware activation detection
/// 
/// The Module detects the presence of the Service Tool software activation.
/// 
/// When the software is detected, the module enters into a running loop with 
/// no action, until the software is closed.
/// 
/// The communication with the Generator is reinitialized as soon as the Service Tool 
/// is disconnected and the normal working mode is ripristinated.
/// 
/// # Communication initialization
/// 
/// The communication is initialized:
/// + after system startup;
/// + after a communication error during the normal mode (idle);
/// + after the service tool software is closed;
/// 
/// During the initialization, the module:
/// + tests the communication with the Smart Hub and the Generator;
/// + initializes the Databanks and the Procedures used;
/// + resets all the system messages should pending in the generator;
/// 
/// When the Initialization should successfully completes the 
/// module enters the Idle mode.
/// 
/// # System Messages handling
/// 
/// The module periodically checks the presence of the system messages 
/// in the generator device. 
/// 
/// The module resets all the messages and stores the non resettable 
/// (persistent) messages.
/// 
/// The persistent messages can originate a System Error message 
/// with the description of the currnt persisten message.
/// 
/// The Description of the message is retrived by the SQLite 
/// archive that shall be installed into the /OEM/AppData/system_messages.sqlite database.
/// 
/// ## RX DISABLE MESSAGE handling
/// 
/// A special message is treated apart: GENERAL_RX_DISABLE_MSG_ID	= 500074;
/// 
/// This message is not stored in the current persistent message queue because the module 
/// activate it to prevent an unexpected exposure.
/// 
/// The module activates it:
/// + after the system startup;
/// + in Idle after any exposure sequence;
/// 
/// The module clears it:
/// + at the beginning of the first pulse of a sequence.
/// 
/// The Status of this message is not visible out of the Generator module. 
///  
/// # Exposure management
/// 
/// The module provides the control of four exposure types:
/// + 2D Single Pulse exposure;
/// + 2D AEC sequence (pre-pulse + pulse);
/// + 3D Single Pulse exposure;
/// + 3D AEC sequence (pre-pulse + pulse);
///
/// The 2D single pulse exposure can be activated with or without the 
/// use of the Grid and Detector synchronization signal.
/// 
/// The Application can implement an arbitrary exposure workflow
/// using an arbitrary sequence of the basic exposure types.
/// 
/// In order to implement such generic workflow, the application, subclassing the Generator class, 
/// shall override the exposureManagementLoop(). This callback is called by the generator class
/// ones for 100ms when in Ready For Exposure mode.
/// 
/// 
/// 
/// ## 2D Single Pulse Exposure implementation
/// 
/// The application shall call the following procedures in order to 
/// activate a generator Single Pulse sequence:
/// 
/// - generator2DPulsePreparation() : this routine prepares the Generator device for the procedure activation.\n
/// With this routine the application sets the exposure parameters to be used during the single pulse exposure.
/// 
/// - generatorExecutePulseSequence(): this procedure handles the internal generator status machine that activates the x-rays.
///  
/// ## 2D AEC Exposure implementation
/// 
/// The application shall call the following procedures in order to 
/// activate a generator 2D-AEC sequence:
/// 
/// - generator2DAecPrePreparation() : this routine prepares the Generator device for the Aec-Pre procedure activation.\n
/// With this routine the application sets the pre-pulse exposure parameters to be used .
/// 
/// - generatorExecutePulseSequence(): this procedure handles the internal generator status machine that activates the x-rays
/// for the pre-pulse procedure;
///
/// - generator2DAecPulsePreparation() : this routine prepares the Generator device for the Aec-Pulse procedure activation.\n
/// With this routine the application sets the main-pulse exposure parameters to be used .
/// 
/// - generatorExecutePulseSequence(): this procedure handles the internal generator status machine that activates the x-rays
/// for the pulse procedure;
/// 
/// ## 3D Single Pulse Exposure implementation
/// 
/// The application shall call the following procedures in order to 
/// activate a generator 3D Single Pulse sequence:
/// 
/// - generator3DPulsePreparation() : this routine prepares the Generator device for the procedure activation.\n
/// With this routine the application sets the exposure parameters to be used during the single pulse exposure.
/// 
/// - generatorExecutePulseSequence(): this procedure handles the internal generator status machine that activates the x-rays.
///	
/// ## 3D AEC Exposure implementation
/// 
/// The application shall call the following procedures in order to 
/// activate a generator 3D-AEC sequence:
/// 
/// - generator3DAecPrePreparation() : this routine prepares the Generator device for the 3D Aec-Pre procedure activation.\n
/// With this routine the application sets the pre-pulse exposure parameters to be used .
/// 
/// - generatorExecutePulseSequence(): this procedure handles the internal generator status machine that activates the x-rays
/// for the pre-pulse procedure;
///
/// - generator3DAecPulsePreparation() : this routine prepares the Generator device for the 3D Aec-Pulse procedure activation.\n
/// With this routine the application sets the main-pulse exposure parameters to be used .
/// 
/// - generatorExecutePulseSequence(): this procedure handles the internal generator status machine that activates the x-rays
/// for the pulse procedure; 
/// 
/// 
/// 
///   
ref class Generator: public TcpClientCLI
{
public:
	Generator(void);
	
	/// <summary>
	/// This enumeration class provides a standard set of possible error codes during a pulse exposure.
	/// </summary>
	enum class generator_errors {
		GEN_NO_ERRORS = 0,			//!< No error code
		GEN_INVALID_PROCEDURE,		//!< A not valid procedure has been requested
		GEN_INVALID_PARAMS,			//!< A non valid exposure parameter has been detected
		GEN_INVALID_STATUS,			//!< The generator is in a not expected status		
		GEN_COMMUNICATION_ERROR,	//!< A generator command is failed
		GEN_INTERNAL_ERROR,			//!< The generator activated internal error messages
		GEN_BUTTON_RELEASE,			//!< The X-Ray Button has been released 
		GEN_TIMEOUT,			    //!< Timeout generator sequence			
		GEN_LAST_ERRCODE			//!< This code shall be used by the Application Subclass to enhance the error code list during the exposures	
	};

	static void startNormalMode(void);
	static void startSimulatorMode(void);
	static bool isSmartHubConnected(void);
	static bool isGeneratorConnected(void);
	static bool isServiceToolConnected(void);
	static bool isFatalError(void) { return dc_bus_error_initialization; }

	inline static bool isGeneratorIdle(void) { return idle_status; }
	inline static bool isReadyForExposure(void) { return ready_for_exposure; }
	inline static bool isSimulatorMode(void) { return simulator_mode; }
	
	bool generateAnodicCurrentTable(bool large_focus);

protected:
	virtual void  setXrayEnable(bool stat) {};
	virtual bool  getXrayPushButton(void) { return false; };
	virtual void  exposureManagementLoop(bool demo) {};

	static inline  Generator^ getDevice(void) { return device; }
	void rxData(cli::array<Byte>^ receiveBuffer, int rc) override;

	
	static System::String^ getGeneratorErrorString(System::String^ errstr);
	inline static unsigned char getGeneratorStatus(void) {return current_generator_status;}
	generator_errors generatorExecutePulseSequence(System::String^ ExpName, int ms_timeout);	
	generator_errors generator2DPulsePreparation(System::String^ exp_name, float kV, float mAs, bool islargefocus, bool det_sync, bool grid_sync, int exp_time);	
	generator_errors generator2DAecPrePulsePreparation(System::String^ exp_name, float kV, float mAs, bool islargefocus, int exp_time);
	generator_errors generator2DAecPulsePreparation(System::String^ exp_name, float kV, float mAs, bool islargefocus, int exp_time);
	generator_errors generator3DPulsePreparation(System::String^ exp_name, float kV, float mAs, int tomo_samples, int tomo_skip, bool islargefocus, int min_exp_time, int max_exp_time);
	inline static float getSelectedAnodeCurrent(void) { return selected_anode_current; }

private:
	
	static Generator^ device;	//!< This is the Handle of the Generator class instance 
	static  cli::array< cli::array<int>^>^ anodicMap;

	static bool simulator_mode = false; //!< This is the current real or simulating mode flag
	Thread^ running_thread;				//!< This is the module worker thread handler
	static CR2CP_Eth* R2CP_Eth;			//!< This is the Tcp/Ip client connection handler with the smart-hub external software
	
	static bool dc_bus_error_initialization = false;   //!< The DC Bus error during initialization fase detected
	static bool idle_status = false;		//!< The Generator is in IDLE mode
	static bool ready_for_exposure = false; //!< The Generator is in Ready for exposure mode
	static bool disable_rx_message = false; //!< This is the status of the disable rx message
	static float selected_anode_current = 100;   //!< This is the current selected anodic current from the pulse preparation

	bool handleCommandProcessedState(unsigned char* code);
	bool clearSystemMessages(void);
	bool testDCBusError(void);

	bool updateGeneratorStatus(void);
	static unsigned char current_generator_status = 255; //!< This is the current generator status 

	bool setDisableRxMessage(bool stat);

	static short sendCR2CPData(unsigned char* pMessage, unsigned short  datalength);
    
	void threadWork(void);
	void simulatorWork();

	void connectSmarthHubAndGenerator(void);
	bool waitGeneratorInitialization(void);

	
	void serviceToolLoop(void);
	bool generatorIdleLoop(void);
	bool generatorErrorMessagesLoop(void);
	bool connectionTest(void);
	
	bool generatorSetup(void);	
	static float getR10Time(float ms, bool next);
	generator_errors generatorSet3PointDatabank(unsigned char dbId, bool large_focus, float KV, float MAS, int long_pulse, int min_pulse, int max_pulse);

};

