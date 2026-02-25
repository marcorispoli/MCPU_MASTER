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
	/// This class implement the protocol data structure as described in the protocol specification.
	/// </summary>
	ref class ProtocolStructure {
	public:

		ref class StatusRegister {
		public:

			enum class register_index {
				SYSTEM_REGISTER = 0,	//!> This is the System Status register index
				BATTERY_REGISTER,		//!> This is the Battery level Status register index		
				NUM_REGISTER
			};

			static bool decodeSystemRegister(Register^ sys) {
				if (sys == nullptr) return false;

				// Byte 0 of the register
				system_error = sys->d0 & 0x1;
				system_emergency = sys->d0 & 0x2;
				system_power_down = sys->d0 & 0x4;
				cabinet_safety_alarm = sys->d0 & 0x8;
				soft_power_off_request = sys->d0 & 0x10;
				batt1_low_alarm = sys->d0 & 0x20;
				batt2_low_alarm = sys->d0 & 0x40;
				battery_enabled = sys->d0 & 0x80;

				// Byte 1 of the register
				button_up_stat = sys->d1 & 0x1;
				button_down_stat = sys->d1 & 0x2;
				button_slide_up_stat = sys->d1 & 0x4;
				button_slide_down_stat = sys->d1 & 0x8;
				button_cw_stat = sys->d1 & 0x10;
				button_ccw_stat = sys->d1 & 0x20;
				button_body_cw = sys->d1 & 0x40;
				button_body_ccw = sys->d1 & 0x80;

				// Byte 2
				motor_48V_ok = sys->d2 & 0x1;
				motor_safety_switch = sys->d2 & 0x2;
				compression_on_status = sys->d2 & 0x4;
				xray_push_button = sys->d2 & 0x8;
				closed_door = sys->d2 & 0x10;
				burning_jumper_present = sys->d2 & 0x20;
				power_lock_status = sys->d2 & 0x40;

				// Byte 3
				pedal_up_stat = sys->d3 & 0x1;
				pedal_down_stat = sys->d3 & 0x2;
				pedal_cmp_up_stat = sys->d3 & 0x4;
				pedal_cmp_down_stat = sys->d3 & 0x8;

				return true;
			}

			static Register^ encodeSystemRegister(void) {
				Register^ sys = gcnew Register;

				// Byte 0 of the register
				if(system_error)			sys->d0 |= 0x1;
				if(system_emergency)		sys->d0 |= 0x2;
				if(system_power_down)		sys->d0 |= 0x4;
				if(cabinet_safety_alarm)	sys->d0 |= 0x8;
				if(soft_power_off_request) sys->d0 |= 0x10;
				if(batt1_low_alarm)		sys->d0 |= 0x20;
				if(batt2_low_alarm)		sys->d0 |= 0x40;
				if(battery_enabled)		sys->d0 |= 0x80;

				// Byte 1 of the register
				if(button_up_stat) sys->d1 |= 0x1;
				if(button_down_stat)  sys->d1 |= 0x2;
				if(button_slide_up_stat)  sys->d1 |= 0x4;
				if(button_slide_down_stat) sys->d1 |= 0x8;
				if(button_cw_stat)  sys->d1 |= 0x10;
				if(button_ccw_stat)  sys->d1 |= 0x20;
				if(button_body_cw)  sys->d1 |= 0x40;
				if(button_body_ccw)  sys->d1 |= 0x80;

				// Byte 2
				if(motor_48V_ok) sys->d2 |= 0x1;
				if (motor_safety_switch) sys->d2 |= 0x2;
				if (compression_on_status) sys->d2 |= 0x4;
				if (xray_push_button) sys->d2 |= 0x8;
				if (closed_door) sys->d2 |= 0x10;
				if (burning_jumper_present) sys->d2 |= 0x20;
				if (power_lock_status)sys->d2 |= 0x40;

				// Byte 3
				if (pedal_up_stat) sys->d3 |= 0x1;
				if (pedal_down_stat) sys->d3 |= 0x2;
				if (pedal_cmp_up_stat) sys->d3 |= 0x4;
				if (pedal_cmp_down_stat) sys->d3 |= 0x8;

				return sys;
			}



			// System Status register definition
			static bool system_error = false;		//!< An error condition is present 
			static bool system_emergency = false;			//!< Current Emergency Status
			static bool system_power_down = false;			//!< Current Powerdown Status 
			static bool cabinet_safety_alarm = false;				//!< The Cabinet safety input status 
			static bool soft_power_off_request = false;		//!< A power Off sequence is requested
			static bool batt1_low_alarm = false;		//!< Low voltage of battery 1
			static bool batt2_low_alarm = false;		//!< Low voltage of battery 2
			static bool battery_enabled = false; //!< Battery enabled system button status
			static bool button_up_stat = false;		//!> This is the current status of the Manual Keypad - Vertical Up input line
			static bool button_down_stat = false;	//!> This is the current status of the Manual Keypad - Vertical Down input line
			static bool button_slide_up_stat = false;//!> This is the current status of the Manual Slide-Up input line
			static bool button_slide_down_stat = false;//!> This is the current status of the Manual Slide-Down input line
			static bool button_cw_stat = false;	//!> This is the current status of the Manual Keypad - Arm CW input line
			static bool button_ccw_stat = false;//!> This is the current status of the Manual Keypad - Arm CCW input line
			static bool button_body_cw = false;		//!> This is the current status of the Manual Body-CW input line
			static bool button_body_ccw = false;	//!> This is the current status of the Manual Body-CCW input line		
			static bool motor_48V_ok = false;				//!< Feedback from the motor power supply
			static bool motor_safety_switch = false;		//!< Safety switch of the 48V status
			static bool compression_on_status = false;		//!< Actual compression signal
			static bool xray_push_button = false;	//!> This is the current X-RAY status 
			static bool closed_door = false;		//!> This is the current closed door status
			static bool burning_jumper_present = false;		//!< Burning jumper present in the system
			static bool power_lock_status = false;			//!< The power supply lock condition (for programming)
			static bool pedal_up_stat = false;		//!> This is the current status of the Pedal Board - Vertical Up input line 
			static bool pedal_down_stat = false;	//!> This is the current status of the Pedal Board - Vertical Down input line 
			static bool pedal_cmp_up_stat = false;		//!> This is the current status of the Pedal Board - Compressor Up input line 
			static bool pedal_cmp_down_stat = false;		//!> This is the current status of the Pedal Board - Compressor Down input line

			static bool decodeBatteryRegister(Register^ sys) {
				if (sys == nullptr) return false;

				// Byte 0 of the register
				voltage_batt1 = sys->d0;

				// Byte 1 of the register
				voltage_batt2 = sys->d1;

				return true;
			}
			static Register^ encodeBatteryRegister(void) {
				Register^ sys = gcnew Register;

				// Byte 0 of the register
				sys->d0 = voltage_batt1;
				sys->d1 = voltage_batt2;
			
				return sys;
			}

			static unsigned char voltage_batt1 = 0;		//!< 10 * voltage level of battery 1
			static unsigned char voltage_batt2 = 0;		//!< 10 * voltage level of battery 2

		};


		ref class DataRegister {
		public:

			enum class register_index {
				OUTPUTS = 0,	//!> This is the System Status register index		
				NUM_REGISTER
			};

			Register^ encodeOutputRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ out = gcnew Register;

				// Assignes the output bit status
				if (power_lock) out->d0 |= 0x1;
				if (motor_power_supply_ena) out->d0 |= 0x2;
				if (motor_power_switch_ena) out->d0 |= 0x4;
				if (compression_enable) out->d0 |= 0x8;
				if (compression_calibration) out->d0 |= 0x10;
				if (xray_enable) out->d0 |= 0x20;

				if (burning_activation) out->d1 |= 0x1;
				if (manual_buzzer_status) out->d1 |= 0x2;
				if (manual_buzzer_mode) out->d1 |= 0x4;
				if (xray_led_status) out->d1 |= 0x8;
				if (xray_lamp1_status) out->d1 |= 0x10;
				if (xray_lamp2_status) out->d1 |= 0x20;
				if (button_rotation_led) out->d1 |= 0x40;


				if (soft_power_off_request) out->d3 |= 0x40;
				if (keep_alive) out->d3 |= 0x80;

				// Returns the formatted register
				return out;
			}

			static bool decodeOutputRegister(Register^ reg) {
				if (reg == nullptr) return false;

				power_lock = reg->d0 & 0x1;
				motor_power_supply_ena = reg->d0 & 0x2;
				motor_power_switch_ena = reg->d0 & 0x4;
				compression_enable = reg->d0 & 0x8;
				compression_calibration = reg->d0 & 0x10;
				xray_enable = reg->d0 & 0x20;

				burning_activation = reg->d1 & 0x1;
				manual_buzzer_status = reg->d1 & 0x2;
				manual_buzzer_mode = reg->d1 & 0x4;
				xray_led_status = reg->d1 & 0x8;
				xray_lamp1_status = reg->d1 & 0x10;
				xray_lamp2_status = reg->d1 & 0x20;
				button_rotation_led = reg->d1 & 0x40;

				soft_power_off_request = reg->d3 & 0x40;
				keep_alive = reg->d3 & 0x80;

				return true;
			}

			// Data 0
			static bool power_lock = false; //!< Activates the power lock status in the remote board
			static bool motor_power_supply_ena = false; //!< Enables the Power supply for all motors
			static bool motor_power_switch_ena = false; //!< Enables the Switch relay on the motor power supply safe line
			static bool compression_enable = false;//!< Enables the compressor-enable bus hardware line
			static bool compression_calibration = false;//!< Enables the calibration bus hardware line
			static bool xray_enable = false; //!< Enables the activation of the X-RAY enable for the generator interface 

			// Data 1
			static bool burning_activation = false; //!< Request to activate the x-ray button via software
			static bool manual_buzzer_status = false; //!< In case of Manual Buzzer Mode active, this is the buzzer status
			static bool manual_buzzer_mode = false; //!< activates the Buzzer manual mode
			static bool xray_led_status = false; //!< activates the xray led output
			static bool xray_lamp1_status = false; //!< activates the xray lamp-1 output
			static bool xray_lamp2_status = false; //!< activates the xray lamp-2 output
			static bool button_rotation_led = false; //!< activates the button_rotation led output

			// Data2
			// Not implemented

			// Data 3
			static bool soft_power_off_request = false; //!< requests the power off sequence activation
			static bool keep_alive = false; //!< keep alive bit to be toggled 

		};

		ref class ParameterRegister {
		public:
			enum class register_index {
				PARAM_REGISTER = 0,
				NUM_REGISTER
			};

			//writeParamRegister(unsigned char idx, Register^ reg)

			Register^ encodeParamRegister(void) {
				return gcnew Register(0, 0, 0, 0);
			}

		};


		ref class Commands {
		public:
			enum class command_index {
				ABORT_COMMAND = 0,		//!< Abort Command (mandatory as for device protocol)
				ACTIVATE_SOFT_POWEROFF,	//!< Soft power off activation
				ACTIVATE_DEMO_TOMO,		//!< Buzzer pulse for tomo		
			};

			CanDeviceCommand^ encodeActivateDemoCommand(unsigned char samples, unsigned char fps) {
				return gcnew CanDeviceCommand((unsigned char)command_index::ACTIVATE_DEMO_TOMO, samples, fps, 0, 0);
			}

		};

		static StatusRegister status_register;
		static DataRegister data_register;
		static Commands command;
	};

	
public:

	PCB301(): CanDeviceProtocol(0x10, L"POWER_SERVICE")
	{
		
	}
	static PCB301^ device = gcnew PCB301();

	
	
	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This function returns the PowerDown condition status
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: a power down condition has been detected</returns>
	static inline bool getPowerdown(void) { return protocol.status_register.system_power_down; }

	
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
	static inline bool getMotorPowerSupply(void) { return protocol.status_register.motor_48V_ok; }

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
	static inline void setMotorPowerSupply(bool stat) { protocol.data_register.motor_power_supply_ena = stat; }

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
	static inline bool getMotorSwitch(void) { return protocol.status_register.motor_safety_switch; }

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
	static inline void setMotorSwitch(bool stat) { protocol.data_register.motor_power_switch_ena = stat; }

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
	static inline bool getBatteryEna(void) { return protocol.status_register.battery_enabled; }

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
	static inline bool getBatteryAlarm(void) { return (protocol.status_register.batt1_low_alarm || protocol.status_register.batt2_low_alarm); }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the info about a possible battery 1 malfunction.
	/// </summary>
	/// <param name=""></param>
	/// <returns>True: the battery 1 is in alarm condition</returns>
	static inline bool getBatt1Alarm(void) { return protocol.status_register.batt1_low_alarm; }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the info about a possible battery 2 malfunction.
	/// </summary>
	/// <param name=""></param>
	/// <returns>True: the battery 2 is in alarm condition</returns>
	static inline bool getBatt2Alarm(void) { return protocol.status_register.batt2_low_alarm; }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the info about the battery 1 voltage level in 0.1V units .
	/// </summary>
	/// 
	/// The battery is a 12VDC nominal battery with a full charge level of 13.8V (138 units)
	/// <param name=""></param>
	/// <returns></returns>
	static inline unsigned char getVoltageBatt1(void) { return protocol.status_register.voltage_batt1; }

	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the info about the battery 2 voltage level in 0.1V units .
	/// </summary>
	/// 
	/// The battery is a 12VDC nominal battery with a full charge level of 13.8V (138 units)
	/// <param name=""></param>
	/// <returns></returns>
	static inline unsigned char getVoltageBatt2(void) { return protocol.status_register.voltage_batt2; }

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
	static inline bool getSoftPowerOffRequest(void) { return protocol.status_register.soft_power_off_request; }

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
	static inline void setPowerOff(bool stat) { protocol.data_register.soft_power_off_request = stat;  }


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
	static inline bool getPowerLockStatus(void) { return protocol.status_register.power_lock_status; }
	
	/// <summary>
	/// \ingroup PCB301PowerSupply
	/// This method provides the Application to activate the Programming Power Lock circuit.
	/// </summary>
	/// 
	/// See the PCB301::getPowerLockStatus() for details about this circuit.
	/// 
	/// <param name="stat"></param>
	static inline void setPowerLock(bool stat) { protocol.data_register.power_lock = stat; }
	
	

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
	static inline bool getEmergency(void) { return protocol.status_register.system_emergency; }

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
	static inline bool getCabinetSafety(void) { return protocol.status_register.cabinet_safety_alarm; }

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
	static inline bool getCompressionStatus(void) { return protocol.status_register.compression_on_status; }
	
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
	static bool isClosedDoor(void) { return protocol.status_register.closed_door; }

	/// <summary>
	/// \ingroup PCB301Safety
	/// This method sets the Compressor Enable Output line on the PCB301 board.
	/// </summary>
	/// 
	/// The Compression Enable output line enables/disables the motor of the compressor device
	/// preventing any unwanted motor activation.
	///  
	/// <param name="stat"></param>
	inline static void SetCompressorEna(bool stat) { protocol.data_register.compression_enable = stat; }

	/// <summary>
	/// \ingroup PCB301Safety
	/// This method sets the Compressor Calibration Output line on the PCB301 board.
	/// </summary>
	/// 
	/// The Compression Calibration output line enables/disables the activation of the 
	/// force sensor of the copressor device.
	///  
	/// <param name="stat"></param>
	inline static void SetCompressorCalib(bool stat) { protocol.data_register.compression_calibration = stat; }

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
	inline static bool getBurningJumper(void) { return protocol.status_register.burning_jumper_present; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Pedal Board Vertical-Up Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_pedal_up_stat(void) { return protocol.status_register.pedal_up_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Pedal Board Vertical-Down Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_pedal_down_stat(void) { return protocol.status_register.pedal_down_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Pedal Board Compression-Up Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_cmp_up_stat(void) { return protocol.status_register.pedal_cmp_up_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Pedal Board Compression-Down Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_cmp_down_stat(void) { return protocol.status_register.pedal_cmp_down_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Keypad CW Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_cw_stat(void) { return protocol.status_register.button_cw_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Keypad CCW Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_ccw_stat(void) { return protocol.status_register.button_ccw_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Keypad Up Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_up_stat(void) { return protocol.status_register.button_up_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Keypad Down Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_down_stat(void) { return protocol.status_register.button_down_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Body-CW Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_body_cw(void) { return protocol.status_register.button_body_cw; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Body-CCW Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_body_ccw(void) { return protocol.status_register.button_body_ccw; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Slide-Up Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_slide_up_stat(void) { return protocol.status_register.button_slide_up_stat; }

	/// <summary>
	/// \ingroup PCB301MotorActivation
	/// This function returns the Manual Slide-Down Hardware Input Line status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool get_button_slide_down_stat(void) { return protocol.status_register.button_slide_down_stat; }

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
	inline static void set_keypad_led(bool stat) { protocol.data_register.button_rotation_led = stat; }


	/// <summary>
	/// \ingroup PCB301Exposure
	/// This method returns the current status of the X-RAY push button activation status.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// <returns>True: the input line is detected active</returns>
	inline static bool getXrayPushButtonStat(void) { return protocol.status_register.xray_push_button; }

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
	inline static void setXrayEna(bool stat) { protocol.data_register.xray_enable = stat; }

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
	inline static void setBuzzerManualMode(bool stat) { protocol.data_register.manual_buzzer_mode = stat; }

	/// <summary>
	/// \ingroup PCB301Exposure
	/// This method, when the buzzer is set in manual mode, sets the current status of the Buzzer device.
	/// </summary>
	/// 
	/// This function has no effect if the Buzzer is set to Automatic Mode.
	/// <param name="stat"></param>
	inline static void activationManualBuzzer(bool stat) { protocol.data_register.manual_buzzer_status = stat;}


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
	inline static CanDeviceCommandResult^ activateManualBuzzerTomoMode(int samples, int fps, int tmo, Object^ device) { return PCB301::device->commandWaitCompletion(protocol.command.encodeActivateDemoCommand(samples, fps), tmo, device); }
	
	
	
protected: 	
	    void runningLoop(void) override;	//!< This is the Running Loop overriden procedure
		void demoLoop(void) override;		//!< This is the Demo Loop overriden procedure

private:
	static ProtocolStructure protocol; // This is the structure with the Status register info
	void evaluateEvents(void);

};

