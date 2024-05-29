#pragma once
#include "CanDeviceProtocol.h"

/// <summary>
/// \defgroup PCB301_Module PCB301 Module: power service board module control
/// This Module implements the PCB301 device protocol communication.
/// 
/// </summary>
/// \ingroup CanDevice_Module 
/// This module subclasses the CanDeviceProtocol base module in order to inherit the 
/// protocol implementation basics.
/// 
/// This module implements the specific functionalities related to the PCB301 device.
/// 
/// See the PCB301 specification documents for details.
/// 
/// # General Description
/// 
/// The PCB301 device board is responsible to handle the following system management:
/// 
/// + Remote device initialization: see the @ref PCB301Protocol section;
/// + System Power Supply management: see the @ref PCB301PowerSupply section;
/// + System Safety management: see the @ref PCB301Safety section;
/// + System Motor Activation Input Management: see the @ref  PCB301MotorActivation section;
/// + System Exposure Tools: see @ref PCB301Exposure section;
/// 
/// # Module Initialization and Setup
/// 
/// This module executes the remote device initial setting always in the following scenarios: 
/// - at the system startup (application start);
/// - in case of remote device reset event (as for communication protocol specification);
/// 
/// During the module initialization all the protocol Parameters are uploaded to the device.
/// 
/// As soon as this fase completes, the running workflow will start normally.
///  
/// 
/// # Alarm And Warning activation
/// 
/// The module every about 20ms gets the PCB301 Status registers.
/// 
/// The Status Registers provide the Module with the current status of the board Inputs
/// and diagnostic information as described in the Communication protocol sepcification.
/// 
/// Some of the Input data are used by this module to direct activate/deactivate system messages
/// as following described:
/// 
/// - Notify::messages::ERROR_POWER_DOWN_ERROR: when the powerdown condition is detected;
/// - Notify::messages::ERROR_CABINET_SAFETY: when the Safety Cabinet Input line is detected activated;
/// - Notify::messages::ERROR_EMERGENCY_BUTTON: when the Emergency Input line is detected activated;
/// - Notify::messages::INFO_MOTOR_48V_SAFETY_LINE_OFF: when the solid state relay is deactivated;
/// - Notify::messages::INFO_MOTOR_48V_POWER_SUPPLY_OFF: when the 48VDC power supply is off;
/// - Notify::messages::INFO_BURNING_JUMPER_PRESENT: when the Burning jumper is detected;
/// - Notify::messages::INFO_BATTERY_DISABLED: when the Batteries are disabled;
/// - Notify::messages::ERROR_BATTERY_LOW_ERROR: when the Batteries are at low level;
/// - Notify::messages::WARNING_DOOR_STUDY_OPEN: when the Study Door is detected Open;
/// 
/// 
/// # Keep Alive Module Implementation
/// 
/// The PCB301 board implements a safety function that disables the 
/// 48DVC line and the Solid State relay in case the communication with this module should 
/// drop for almost 1 second.
/// 
/// This Module toggles the proper bit in the Protocol Data Register to keeps the board Alive.
/// 
/// # Input and Output Update
/// 
/// The Module gets the Input Status from the PCB301 board and sets the Outputs 
/// every about 20ms in a polling ways.
///  
/// 


/// <summary>
/// \defgroup PCB301PowerSupply PCB301 API to control the System Power Supply
/// This section provides methods to monitor and control the system power supply.
/// 
/// </summary>
/// \ingroup PCB301_Module 
/// 
/// 



/// <summary>
/// \defgroup PCB301Safety PCB301 API to control the Module's Safety
/// 
/// This section provides methods to monitor and control the Safety tools 
/// implemented in the PCB301 board.
/// 
/// </summary>
/// \ingroup PCB301_Module 
/// 
/// The PCB301 board implements hardware circuits in order to provides the system  with a level of safety related to:
/// 
/// + Emergency Input Line
/// + Motor Rotation protection;
/// + X-RAY execution protection;
/// + Compressor activation;
/// 
/// 
/// # Emergency Input
/// 
/// The PCB301 board provides a dedicated hardware output to control the coil of the Main Contactor.\n
/// The MainContactor is a power switch providing the power supply to the X-RAY generator and Motors.
/// 
/// The output is controlled with a series of dedicated inputs connected with the 
/// generator device and the Emergency button that can disable the coil activation in the following scenarios:
/// + Emergency Push Button activation: if the operator should press one of predisposed emergency buttons
/// the contactor is hardware disabled;
/// + Generator initialization fase: during the initialization, the generator disables the AC power supply 
/// to prevent damages to its internal power circuit;
/// 
/// The status of the inputs line are monitored by the PCB301 and passed to this module
/// in order to be properly handled. 
/// 
/// # Open Cabinet Input 
/// 
/// The PCB301 board provides a dedicated hardware input line that, in case it should be open,
/// disables the 48DVC power supply (used to power the motors).
/// 
/// The input line should be connected to a series of cabinet switches that should prevent the 
/// unwanted motor activation when one of motor protection should be removed.
/// 
/// The status of the input is monitored by the PCB301 and passed to this module
/// in order to be properly handled.  
///  
/// # Compression Detected Input
///
/// The PCB301 board provides a dedicated hardware input line that, in case it should be activated,
/// disables the Safety Solid State Relay 48DVC power supply (used to power a subset of motors);
/// 
/// The input line should be connected with the Compressor board (PCB302) that is responsible
/// to detect the compression. 
/// 
/// When the compression is present some motor shall be disabled to prevent 
/// a breast injury.
/// 
/// The status of the input is monitored by the PCB301 and passed to this module
/// in order to be properly handled.  
/// 
/// # Open Study Input
/// 
/// This is a dedicated hardware input line that, in case it should be activated,
/// prevents to activate the X-RAY enable signal of the Generator device and,
/// consequently, preventing to activate the X-RAY's.
/// 
/// This line should be connected to the Door of the Exposure Room to prevent 
/// unexpected entrance of persons during an exposure sequence.
/// 
/// The status of the input is monitored by the PCB301 and passed to this module
/// in order to be properly handled.  
/// 
/// # Compressor Enable Outputs
/// 
/// The PCB301 board controls two dedicated output hardware lines to enable 
/// the compressor board (PCB302) to activate a compression on a breast or to calibrate the force measurement.
/// 
/// + Compressor Enable Line: this line enables/disables  the compressor motor  to be activated; 
/// + Compressor Calibration Line: this line enables/disables  the calibration procedure on the compressor device;
/// 
/// The compression activation should be disabled in the following conditions:
/// + During an exposure to prevent to loose the current compression;
/// + In case of system fault, to prevent an invalid compression activation;
/// 
/// The Calibration line should be activated only in service mode.
/// 
/// This module provides API's to control those output lines.
/// 
/// # Burning Jumper Input
/// 
/// The PCB301 allows to make use of an hardware jumper enabling the software 
/// activation of the X-RAY push Button line.
/// 
/// This feature is useful in production to activate special test procedures that can be 
/// executed in automatic mode without the operator. 
/// 
/// This jumper however shall be removed when the System is set in operating condition
/// for safety reason. If the jumper should be detected the System should prevent to 
/// enter in an operating mode.
/// 
/// In order to prevent an exposure event, this module activates a proper WARNING 
/// messagge when the jumper should be detected. 
/// 
/// This module provides API to check the current status of the jumper detection status.
/// 
/// 




/// <summary>
/// \defgroup PCB301MotorActivation PCB301 API to control the Motor activation
/// This section provides methods to monitor and control the system motor activation.
/// 
/// </summary>
/// \ingroup PCB301_Module 
/// 
/// The PCB301 board handle several hardware input lines 
/// dedicated to request a particolar motor activation.
/// 
/// NOTE: the pcb301 board nor this module directly activate any motorization.
///  
/// # External Pedalboard Activation Inputs
/// 
/// The PCB301 board provides four inputs coming from the external pedalboard:
/// + Pedal Up: see PCB301::get_pedal_up_stat();
/// + Pedal Down: see PCB301::get_pedal_down_stat();
/// + Compression Up: see PCB301::get_cmp_up_stat();
/// + Compression Down: see PCB301::get_cmp_down_stat();
/// 
/// # Manual Button Keypad 
/// 
/// The system provides a set of manual buttons to request a motor activation.
/// Those buttons are mounted into a keypad in a cross geometry:
/// + Vertical disposition: Manual-Up and Manual-Down buttons;
/// + Horizontal disposition: Manual-CCW and Manual-CW buttons;
/// 
/// The PCB301 board reads those input luines and provide the current status 
/// to this module:
///  
/// + Manual-CW: see PCB301::get_button_cw_stat();
/// + Manual-CCW: see PCB301::get_button_ccw_stat();
/// + Manual-Up: see PCB301::get_button_up_stat();
/// + Manual-Down: see PCB301::get_button_down_stat();
///  
/// The Keypad device provides a status LED that can be activated:
/// + Keypad-LED: see PCB301::set_keypad_led();
/// 
/// 
/// # Manual Slide Motor Activation
/// 
/// The system provides dedicated hardware input lines to control the 
/// manual Slide Motor activation:
/// + Slide Up: PCB301::get_button_slide_up_stat();
/// + Slide Down: PCB301::get_button_slide_down_stat();
/// 
/// 
/// # Manual Body Motor Activation
/// 
/// The system provides dedicated hardware input lines to control the 
/// manual Body Motor activation:
/// + Body CW: PCB301::get_button_body_cw();
/// + Body CCW: PCB301::get_button_body_ccw();
/// 

/// <summary>
/// \defgroup PCB301Exposure PCB301 API for Exposure handling
/// This section provides methods useful during the Exposure workflow.
/// 
/// </summary>
/// \ingroup PCB301_Module 
/// 
/// 


ref class PCB301 :  public CanDeviceProtocol
{

	
	/// <summary>
	///	This is the Device STATUS Register implementation 
	/// </summary>
	private:enum class StatusRegisters {
		SYSTEM_STATUS_REGISTER = 0, //!> This is the System Status register index
		BATTERY_STATUS_REGISTER,		//!> This is the Battery level Status register index		
	};

	#define PCB301_GET_SYSTEM_ERROR_STATUS(reg)			(bool)	(reg->d0 & 0x01)
	#define PCB301_GET_SYSTEM_EMERGENCY_STATUS(reg)		(bool)	(reg->d0 & 0x02)
	#define PCB301_GET_SYSTEM_POWERDOWN(reg)			(bool)	(reg->d0 & 0x04)
	#define PCB301_GET_SYSTEM_CABINET_SAFETY(reg)		(bool)	(reg->d0 & 0x08)
	#define PCB301_GET_SYSTEM_POWER_OFF_REQ(reg)		(bool)	(reg->d0 & 0x10)
	#define PCB301_GET_SYSTEM_BATT1LOW(reg)				(bool)	(reg->d0 & 0x20)
	#define PCB301_GET_SYSTEM_BATT2LOW(reg)				(bool)	(reg->d0 & 0x40)
	#define PCB301_GET_SYSTEM_BATTENA(reg)				(bool)	(reg->d0 & 0x80)
	
	#define PCB301_GET_BUTTON_VERTICAL_UP(reg)			(bool)	(reg->d1 & 0x1)
	#define PCB301_GET_BUTTON_VERTICAL_DOWN(reg)		(bool)	(reg->d1 & 0x2)
	#define PCB301_GET_BUTTON_SLIDE_UP(reg)				(bool)	(reg->d1 & 0x4)
	#define PCB301_GET_BUTTON_SLIDE_DOWN(reg)			(bool)	(reg->d1 & 0x8)
	#define PCB301_GET_BUTTON_ARM_CW(reg)				(bool)	(reg->d1 & 0x10)
	#define PCB301_GET_BUTTON_ARM_CCW(reg)				(bool)	(reg->d1 & 0x20)
	#define PCB301_GET_BUTTON_BODY_CW(reg)				(bool)	(reg->d1 & 0x40)
	#define PCB301_GET_BUTTON_BODY_CCW(reg)				(bool)	(reg->d1 & 0x80)
	
	#define PCB301_GET_SYSTEM_MOTOR_OK(reg)				(bool) (reg->d2 & 0x01)
	#define PCB301_GET_SYSTEM_MOTOR_SWITCH(reg)			(bool) (reg->d2 & 0x02)
	#define PCB301_GET_SYSTEM_COMPRESSION(reg)			(bool) (reg->d2 & 0x04)
	#define PCB301_GET_XRAY_PUSH_BUTTON(reg)			(bool) (reg->d2 & 0x08)
	#define PCB301_GET_SYSTEM_CLOSEDOOR(reg)			(bool) (reg->d2 & 0x10)
	#define PCB301_GET_SYSTEM_BURNING_JMP(reg)			(bool) (reg->d2 & 0x20)
	#define PCB301_GET_SYSTEM_POWER_LOCK(reg)			(bool) (reg->d2 & 0x40)
	
	#define PCB301_GET_PEDAL_VERTICAL_UP(reg)			(bool) (reg->d3 & 0x1)
	#define PCB301_GET_PEDAL_VERTICAL_DOWN(reg)			(bool) (reg->d3 & 0x2)
	
	// This is the BATTERY SECTION
	#define PCB301_GET_BATTERY_VBATT1(reg) (unsigned char) (reg->d0)
	#define PCB301_GET_BATTERY_VBATT2(reg) (unsigned char) (reg->d1)


	

	/// <summary>
	///	 This is the Device DATA Register implementation 
	/// </summary>
	private:enum class DataRegisters {
		OUTPUTS_DATA_REGISTER = 0, //!> This is the Outputs Data register index
	};

	#define PCB301_OUTPUTS_DATA_POWER_LOCK(reg,stat)				reg->D0(stat, 0x1) 
	#define PCB301_OUTPUTS_DATA_MOTOR_POWER_SUPPLY_ENABLE(reg,stat)	reg->D0(stat, 0x2)
	#define PCB301_OUTPUTS_DATA_MOTOR_SWITCH_ENABLE(reg,stat)		reg->D0(stat, 0x4)
	#define PCB301_OUTPUTS_DATA_COMPRESSOR_ENABLE(reg,stat)			reg->D0(stat, 0x8)
	#define PCB301_OUTPUTS_DATA_COMPRESSOR_CALIBRATION(reg,stat)	reg->D0(stat, 0x10)
	#define PCB301_OUTPUTS_DATA_XRAY_ENA(reg,stat)					reg->D0(stat, 0x20)
	

	#define PCB301_OUTPUTS_DATA_BURNING_STAT(reg,stat)	reg->D1(stat, 0x1) 
	#define PCB301_OUTPUTS_DATA_BUZZER_STAT(reg,stat)	reg->D1(stat, 0x2) 
	#define PCB301_OUTPUTS_DATA_MANUAL_BUZZER(reg,stat)	reg->D1(stat, 0x4) 
	#define PCB301_OUTPUTS_DATA_XRAY_LED(reg,stat)		reg->D1(stat, 0x8) 
	#define PCB301_OUTPUTS_DATA_XRAY_LAMP1(reg,stat)	reg->D1(stat, 0x10) 
	#define PCB301_OUTPUTS_DATA_XRAY_LAMP2(reg,stat)	reg->D1(stat, 0x20) 
	#define PCB301_OUTPUTS_DATA_MAN_ROT_LED(reg,stat)	reg->D1(stat, 0x40) 
	
	#define PCB301_OUTPUTS_DATA_POWER_OFF(reg,stat)		reg->D3(stat, 0x40) 
	#define PCB301_GET_OUTPUTS_DATA_POWER_OFF(reg)		(reg->d3 & 0x40) 
	#define PCB301_OUTPUTS_DATA_KEEP_ALIVE(reg,stat)	reg->D3(stat, 0x80)

	/// <summary>	
	/// This enumeration class defines the Indexes of the Command Execution
	///
	/// </summary>
	private:enum class Commandregister {
		ABORT_COMMAND = 0, //!< Abort Command (mandatory as for device protocol)
		ACTIVATE_SOFT_POWEROFF,//!< Soft power off activation
		ACTIVATE_DEMO_TOMO,		//!< Buzzer pulse for tomo		
	};
	#define PCB301_SET_DEMO_TOMO_COMMAND(samples,fps) (System::Byte) Commandregister::ACTIVATE_DEMO_TOMO, (System::Byte) (samples),(System::Byte) fps,(System::Byte) 0,(System::Byte) 0 
			   
public:

	PCB301(): CanDeviceProtocol(0x10, L"POWER_SERVICE")
	{

	}
	static PCB301^ device = gcnew PCB301();


	public: enum class door_options {
		CLOSED_DOOR = 0,
		OPEN_DOOR
	};

public:	
	
	
	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This function returns the PowerDown condition status
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: a power down condition has been detected</returns>
	static inline bool getPowerdown(void) { return power_down_status; }

	
	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This function returns the current status of the 48DVC power supply activation status.
	/// </summary>
	/// 
	/// The 48VDC status is read directly from the power supply device.
	/// 
	/// The Reason to have a 48DVC status Off are:
	/// + The Application requested to disable the Power Supply for safety reason ( see PCB301::setMotorPowerSupply() );
	/// + The Emergency button has been activated: the pcb301 board disables the power supply via hardware;
	/// + The Safety Cabinet hardware input is active:  the pcb301 board disables the power supply via hardware;
	/// + A short is present on the load side and the Power Supply is in protection;
	/// 
	/// 
	/// <param name=""></param>
	/// <returns>True: the 48DVC power supply is active at the nominal level (48VDC) </returns>
	static inline bool getMotorPowerSupply(void) { return motor_48V_ok; }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method requests the board to disable the 48DVC power supply. 
	/// </summary>
	/// 
	/// The Application can request the pcb301 board to disable the 48VDC power Supply.
	/// 
	/// If the Application should set the Power Supply status to True (power presence) 
	/// the effective status can depend by other internal hardware conditions that 
	/// may prevent to activate the Power Supply (see PCB301::getMotorPowerSupply )
	/// 
	/// The Application can monitor the effective status with the method PCB301::getMotorPowerSupply()
	/// 
	/// 
	/// <param name="stat">The requested activation status </param>
	static inline void setMotorPowerSupply(bool stat) { PCB301_OUTPUTS_DATA_MOTOR_POWER_SUPPLY_ENABLE(outputs_data_register, stat); }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method requests the current status of the safety external solid state relay
	/// </summary>
	/// 
	/// The 48VDC output line is partially protected with a solid state relay 
	/// for a subset of load devices that shall be momentary disabled for safety condition.
	/// 
	/// The Application gets the actual status of this relay with this method.
	/// 
	/// The reason of a False activation status can be one of:
	/// + The Application requested to disable this relay with the PCB301::setMotorSwitch(false) method;
	/// + The Safety Cabinet hardware input is active:  the pcb301 board disables the power supply via hardware;
	/// + Compression hardware input is active:  the pcb301 board disables the power supply via hardware;
	/// 
	/// <param name=""></param>
	/// <returns></returns>
	static inline bool getMotorSwitch(void) { return motor_safety_switch; }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method requests the board to disable the 48VDC safety solid state relay. 
	/// </summary>
	/// 
	/// The Application can request the pcb301 board to disable the safety solid state relay
	/// for safety reason.
	/// 
	/// Although the Application should request to activate the solid state relay, 
	/// the effective status can depend by other internal hardware conditions that 
	/// may prevent the activation. See the PCB301::getMotorSwitch() for details
	/// 
	/// 
	/// <param name="stat">The requested activation status </param>
	static inline void setMotorSwitch(bool stat) { PCB301_OUTPUTS_DATA_MOTOR_SWITCH_ENABLE(outputs_data_register, stat); }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method gets the current status of the Battery Enable External button.
	/// </summary>
	/// 
	/// The system provides an external Button to hardware enable the presence and usage 
	/// of the backup batteries.
	/// 
	/// If this button should set to disable the Backup batteries, in case of powerdown condition 
	/// the system will switche off.
	/// 
	/// The Application can monitor the status of the button with this method.
	/// 
	/// <param name=""></param>
	/// <returns>True: the batteries are enabled</returns>
	static inline bool getBatteryEna(void) { return battery_enabled_status; }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the info about a possible battery malfunction.
	/// </summary>
	/// 
	/// The pcb301 board monitors the current voltage of the batteries (if present and enabled).
	/// 
	/// If the level of the batteries drops below an alarm level, the board signals the 
	/// malfunction.
	/// 
	/// The Application gets the actual status of a possible fault condition with this method. 
	/// 
	/// See also:
	/// + PCB301::getBatt1Alarm();
	/// + PCB301::getBatt2Alarm();
	///
	/// <param name=""></param>
	/// <returns>True: one or both the batteries are in alarm condition</returns>
	static inline bool getBatteryAlarm(void) { return (batt1_low_alarm || batt2_low_alarm); }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the info about a possible battery 1 malfunction.
	/// </summary>
	/// <param name=""></param>
	/// <returns>True: the battery 1 is in alarm condition</returns>
	static inline bool getBatt1Alarm(void) { return batt1_low_alarm; }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the info about a possible battery 2 malfunction.
	/// </summary>
	/// <param name=""></param>
	/// <returns>True: the battery 2 is in alarm condition</returns>
	static inline bool getBatt2Alarm(void) { return batt2_low_alarm; }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the info about the battery 1 voltage level in 0.1V units .
	/// </summary>
	/// 
	/// The battery is a 12VDC nominal battery with a full charge level of 13.8V (138 units)
	/// <param name=""></param>
	/// <returns></returns>
	static inline unsigned char getVoltageBatt1(void) { return voltage_batt1; }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the info about the battery 2 voltage level in 0.1V units .
	/// </summary>
	/// 
	/// The battery is a 12VDC nominal battery with a full charge level of 13.8V (138 units)
	/// <param name=""></param>
	/// <returns></returns>
	static inline unsigned char getVoltageBatt2(void) { return voltage_batt2; }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the status of the current soft power off request coming from the pcb301 board
	/// </summary>
	/// 
	/// The PCB301 board monitors a dedicated external hardware input button 
	/// used to request a system power off.
	/// 
	/// A short time pression detection causes a soft power off request.
	/// 
	/// The Application may ignore this request if there are not the right conditions to 
	/// switch off the system. In case the Application should proceed with the power off sequence,
	/// the PCB301::setPowerOff(true) method should be used.
	/// 
	/// See  PCB301::setPowerOff() for details.
	/// 
	/// <param name=""></param>
	/// <returns></returns>
	static inline bool getSoftPowerOffRequest(void) { return soft_power_off_request; }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the Application to initiate a soft power off sequence.
	/// </summary>
	/// 
	/// When the Application should initiate a soft power off seqeunce, 
	/// it should use the PCB301::setPowerOff(true) method: 
	/// + the pcb301 will wait for a given time to allow the system to properly 
	/// shutdown the operating system and all the system devices.
	/// 
	/// If the Application should abort the sequence, it is enough to set again the method with a False parameter:
	/// + PCB301::setPowerOff(false);
	/// 
	/// NOTE: The Power Off sequence can be Aborted ONLY if the waiting timr is not yest expired!
	/// 
	/// <param name="stat"></param>
	static inline void setPowerOff(bool stat) { PCB301_OUTPUTS_DATA_POWER_OFF(outputs_data_register, stat); }


	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the current status of the Programming Power Lock 
	/// </summary>
	/// 
	/// The Programming power lock is a special hardware circuit that 
	/// locks, when activated, the 24VDC power supply output to be active 
	/// irrspective of the pcb301 microcontroller activity.
	/// 
	/// This feature is necessary to allow the system to upgrade the 
	/// pcb301 board controller firmware by mean of a software procedure 
	/// that runs on the system PC. Because during the firmware upload the 
	/// board microcontroller resets, without this circuit, the entire system 
	/// will switch off in case of reprogramming attempt.
	/// 
	/// NOTE: when this circuit should be activated (see PCB301::setPowerLock()) 
	/// the only method to switch off the system is to remove the Main AC power supply and 
	/// to disable the backup Batteries!!
	/// 
	/// The Application should signal a Warning condition in case it should detect this circuit active
	/// in an Operating status.
	///  
	/// <param name=""></param>
	/// <returns></returns>
	static inline bool getPowerLockStatus(void) { return power_lock_status; }
	
	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the Application to activate the Programming Power Lock circuit.
	/// </summary>
	/// 
	/// See the PCB301::getPowerLockStatus() for details about this circuit.
	/// 
	/// <param name="stat"></param>
	static inline void setPowerLock(bool stat) { PCB301_OUTPUTS_DATA_POWER_LOCK(outputs_data_register, stat); }
	
	

	/// <summary>
	/// \ingroup PCB301Safety
	/// This function returns the Emergency Input circuit line activation status.
	/// </summary>
	/// 
	/// When this input is activated, the external power contactor is switched off 
	/// toghether with the 48VDC power supply.
	/// 
	/// The Application should use this method to properly handle the operativity
	/// and to provide the correct information to the operator.	
	/// 
	/// <param name=""></param>
	/// <returns>True: the emergency input has been detected activated</returns>
	static inline bool getEmergency(void) { return emergency_status; }

	/// <summary>
	/// \ingroup PCB301Safety
	/// This function returns the Cabinet Safety Input circuit line activation status.
	/// </summary>
	/// 
	/// When this input is activated, the external 48VDC power supply is disabled
	/// for safety reasons.
	/// 
	/// The Application should use this method to properly handle the operativity
	/// and to provide the correct information to the operator.	
	/// 
	/// <param name=""></param>
	/// <returns>True: the cabinet input circuit is detected active</returns>
	static inline bool getCabinetSafety(void) { return cabinet_safety_status; }

	/// <summary>
	/// \ingroup PCB301Safety
	/// This function returns the Compression Pending Status Input circuit line activation status.
	/// </summary>
	/// 
	/// When this input is activated, the external safety solid state relay is disabled
	/// and the Motor powered by this line cannot be activated for safety reasons.
	/// 
	/// The Application should use this method to properly handle the operativity
	/// and to provide the correct information to the operator.	
	/// 
	/// <param name=""></param>
	/// <returns>True: the compression line is detected active</returns>
	static inline bool getCompressionStatus(void) { return compression_on_status; }
	
	/// <summary>
	/// \ingroup PCB301Safety
	/// This function returns the Door Study Status Input circuit line activation status.
	/// </summary>
	/// 
	/// When this input is activated, the X-RAY enable signal is disabled 
	/// and no X-RAY Radiation can be executed by the Generator device.
	/// 
	/// The Application should use this method to properly handle the operativity
	/// and to provide the correct information to the operator.	
	/// 
	/// <param name=""></param>
	/// <returns>True: the study's door is detected open</returns>
	static door_options getDoorStatus(void) { return door_status; }

	/// <summary>
	/// \ingroup PCB301Safety
	/// This method sets the Compressor Enable Output line on the PCB301 board.
	/// </summary>
	/// 
	/// The Compression Enable output line enables/disables the motor of the compressor device
	/// preventing any unwanted motor activation.
	///  
	/// <param name="stat"></param>
	inline static void SetCompressorEna(bool stat) { PCB301_OUTPUTS_DATA_COMPRESSOR_ENABLE(outputs_data_register, stat); }

	/// <summary>
	/// \ingroup PCB301Safety
	/// This method sets the Compressor Calibration Output line on the PCB301 board.
	/// </summary>
	/// 
	/// The Compression Calibration output line enables/disables the activation of the 
	/// force sensor of the copressor device.
	///  
	/// <param name="stat"></param>
	inline static void SetCompressorCalib(bool stat) { PCB301_OUTPUTS_DATA_COMPRESSOR_CALIBRATION(outputs_data_register, stat); }

	/// <summary>
	/// \ingroup PCB301Safety
	/// This function returns the status of the Burning Jumper detected presence
	/// </summary>
	/// 
	/// The jumper for the burning reduces the safety level of the Gantry.
	/// 
	/// The jumper cannot be present in an operative session so the Application shall
	/// detect its presence for safety reason.
	/// 
	/// <param name=""></param>
	/// <returns>True if the Jumper is detected present</returns>
	inline static bool getBurningJumper(void) { return burning_jumper_present; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Pedal Board Vertical-Up Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_pedal_up_stat(void) { return pedal_up_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Pedal Board Vertical-Down Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_pedal_down_stat(void) { return pedal_down_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Pedal Board Compression-Up Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_cmp_up_stat(void) { return cmp_up_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Pedal Board Compression-Down Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_cmp_down_stat(void) { return cmp_down_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Keypad CW Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_cw_stat(void) { return button_arm_cw_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Keypad CCW Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_ccw_stat(void) { return button_arm_ccw_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Keypad Up Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_up_stat(void) { return button_up_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Keypad Down Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_down_stat(void) { return button_down_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Body-CW Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_body_cw(void) { return button_body_cw; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Body-CCW Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_body_ccw(void) { return button_body_ccw; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Slide-Up Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_slide_up_stat(void) { return button_slide_up_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Slide-Down Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_slide_down_stat(void) { return button_slide_down_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function controls the status of the Keypad led 
	/// </summary>
	/// 
	/// The Keypad device provides a green led that can be activated whenever needed.
	/// 
	/// Usually this led should be activated whenever a manual motor activation can be requested 
	/// by the operator.
	/// 
	/// <param name="stat">True: led is switched ON</param>
	inline static void set_keypad_led(bool stat) { PCB301_OUTPUTS_DATA_MAN_ROT_LED(outputs_data_register, stat); }


	/// <summary>
	/// \ingroup PCB301Exposure
	/// This method returns the current status of the X-RAY push button activation status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool getXrayPushButtonStat(void) { return xray_push_button; }

	/// <summary>
	/// \ingroup PCB301Exposure
	/// This method requests to activate the x-ray enable hardware signal on the generator external interface.
	/// </summary>
	/// 
	/// The X-RAY enable signal, during an exposure sequence, enables the Generator to proceeds with the x-ray activation.
	/// 
	/// The Application shall set this signal during an exposure workflow.
	/// 
	/// The actual status of the signals depends by other hardware signals in the pcb301 board:
	/// + Closed Door Input hardware line;
	/// + Xray Push Button hardware Input line;
	/// 
	/// 
	/// <param name="stat">True: requests to activate the external X-RAY enable (or Generator Xray request)</param>
	inline static void setXrayEna(bool stat) { PCB301_OUTPUTS_DATA_XRAY_ENA(outputs_data_register, stat); }

	/// <summary>
	/// \ingroup PCB301Exposure
	/// This method sets the working mode of the Buzzer in Manual Mode or Automatic Mode
	/// </summary>
	/// 
	/// The PCB301 mounts a local Buzzer device that works in two ways:
	/// + Manual Mode;
	/// + Automatic Mode;
	/// 
	/// When in Automatic mode the PCB301 activates the Buzzer toghether with the Hardware input line HV-ON.
	/// 
	/// When in Manual Mode 
	/// 
	/// <param name="stat"></param>
	inline static void setBuzzerManualMode(bool stat) { PCB301_OUTPUTS_DATA_MANUAL_BUZZER(outputs_data_register,stat); }

	/// <summary>
	/// \ingroup PCB301Exposure
	/// This method, when the buzzer is set in manual mode, sets the current status of the Buzzer device.
	/// </summary>
	/// 
	/// This function has no effect if the Buzzer is set to Automatic Mode.
	/// <param name="stat"></param>
	inline static void activationManualBuzzer(bool stat) { PCB301_OUTPUTS_DATA_BUZZER_STAT(outputs_data_register, stat); }


	/// <summary>
	/// \ingroup PCB301Exposure
	/// This method, when the buzzer is set in manual mode, activates the buzzer in Tomo mode, 
	/// providing a given number of consecutive pulses at a given rate.
	/// </summary>
	/// 
	/// This method helps the application to implement a simulated Tomo sequence when 
	/// the generator device is not present for Demo applications.
	/// 
	/// The calling thread remains locked to the call until one of the following condition are true:
	/// + all the pulses are generated;
	/// + the x-ray push button is released;
	/// + the Timeout expires
	/// 
	/// This function fails and immediatelly returns if the method is called into the PCB301 Module!!
	/// 
	/// 
	/// <param name="samples">Number of Pulses</param>
	/// <param name="fps">Pulse for seconds</param>
	/// <param name="tmo">Timout command in ms</param>
	/// <param name="device">Handle of the calling thread</param>
	/// 
	/// <returns>
	/// The result of the executed command:
	/// + CanDeviceCommandResult::error_code:
	///		+ error_code == COMMAND_NO_ERROR command success;
	///		+ error_code != COMMAND_NO_ERROR command failed;
	/// 
	/// + CanDeviceCommandResult::result_0: numer of actually executed pulses.
	/// 
	/// NOTE: if the x-ray push button should be released during the pulse generation
	/// the procedure successfully termines (no error_code) but the result_0 
	/// will provide a number of executed pulses lower than expected.
	/// 
	/// </returns>
	inline static CanDeviceCommandResult^ activateManualBuzzerTomoMode(int samples, int fps, int tmo, Object^ device) { return PCB301::device->commandWaitCompletion(PCB301_SET_DEMO_TOMO_COMMAND(samples, fps), tmo, device); }
	
	
	
private:
	

	static Register^ outputs_data_register = gcnew Register(); 
	static door_options door_status = door_options::OPEN_DOOR; //!< This is the current status of the Study door
	
	static bool power_down_status = false;			//!< Current Powerdown Status 
	static bool emergency_status = false;			//!< Current Emergency Status
	static bool cabinet_safety_status = false;		//!< The Cabinet safety input status 
	static bool motor_safety_switch = false;		//!< Safety switch of the 48V status
	static bool motor_48V_ok = false;				//!< Feedback from the motor power supply
	static bool compression_on_status = false;		//!< Actual compression signal
	static bool burning_jumper_present = false;		//!< Burning jumper present in the system
	static bool soft_power_off_request = false;		//!< A power Off sequence is requested
	static bool power_lock_status = false;			//!< The power supply lock condition (for programming)


	// Battery management
	static bool battery_enabled_status = false; //!< Battery enabled system button status
	static bool batt1_low_alarm = false;		//!< Low voltage of battery 1
	static bool batt2_low_alarm = false;		//!< Low voltage of battery 2
	static unsigned char voltage_batt1 = 0;		//!< 10 * voltage level of battery 1
	static unsigned char voltage_batt2 = 0;		//!< 10 * voltage level of battery 2

	// X-RAY push button handling
	static bool xray_push_button = false; //!> This is the current X-RAY status 
	
	static bool pedal_up_stat = false;		//!> This is the current status of the Pedal Board - Vertical Up input line 
	static bool pedal_down_stat = false;	//!> This is the current status of the Pedal Board - Vertical Down input line 
	static bool cmp_up_stat = false;		//!> This is the current status of the Pedal Board - Compressor Up input line 
	static bool cmp_down_stat = false;		//!> This is the current status of the Pedal Board - Compressor Down input line
	static bool button_arm_cw_stat = false;	//!> This is the current status of the Manual Keypad - Arm CW input line
	static bool button_arm_ccw_stat = false;//!> This is the current status of the Manual Keypad - Arm CCW input line
	static bool button_up_stat = false;		//!> This is the current status of the Manual Keypad - Vertical Up input line
	static bool button_down_stat = false;	//!> This is the current status of the Manual Keypad - Vertical Down input line
	static bool button_body_cw = false;		//!> This is the current status of the Manual Body-CW input line
	static bool button_body_ccw = false;	//!> This is the current status of the Manual Body-CCW input line
	static bool button_slide_up_stat = false;//!> This is the current status of the Manual Slide-Up input line
	static bool button_slide_down_stat = false;//!> This is the current status of the Manual Slide-Down input line
	

protected: 	
	    void runningLoop(void) override;	//!< This is the Running Loop overriden procedure
		void demoLoop(void) override;		//!< This is the Demo Loop overriden procedure

private:
		void handleSystemStatusRegister(void);
		void handleBatteryStatusRegister(void);
		void toggleKeepalive(void);
		void evaluateEvents(void);

};

