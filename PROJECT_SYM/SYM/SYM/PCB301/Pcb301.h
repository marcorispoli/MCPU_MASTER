#pragma once
#include "deviceInterface.h"

using namespace System;

ref class PCB301: public deviceInterface
{
public:
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
				if (system_error)			sys->d0 |= 0x1;
				if (system_emergency)		sys->d0 |= 0x2;
				if (system_power_down)		sys->d0 |= 0x4;
				if (cabinet_safety_alarm)	sys->d0 |= 0x8;
				if (soft_power_off_request) sys->d0 |= 0x10;
				if (batt1_low_alarm)		sys->d0 |= 0x20;
				if (batt2_low_alarm)		sys->d0 |= 0x40;
				if (battery_enabled)		sys->d0 |= 0x80;

				// Byte 1 of the register
				if (button_up_stat) sys->d1 |= 0x1;
				if (button_down_stat)  sys->d1 |= 0x2;
				if (button_slide_up_stat)  sys->d1 |= 0x4;
				if (button_slide_down_stat) sys->d1 |= 0x8;
				if (button_cw_stat)  sys->d1 |= 0x10;
				if (button_ccw_stat)  sys->d1 |= 0x20;
				if (button_body_cw)  sys->d1 |= 0x40;
				if (button_body_ccw)  sys->d1 |= 0x80;

				// Byte 2
				if (motor_48V_ok) sys->d2 |= 0x1;
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
				NUM_COMMANDS
			};

			CanDeviceCommand^ encodeActivateDemoCommand(unsigned char samples, unsigned char fps) {
				return gcnew CanDeviceCommand((unsigned char)command_index::ACTIVATE_SOFT_POWEROFF, samples, fps, 0, 0);
			}

		};

		static StatusRegister status_register;
		static DataRegister data_register;
		static Commands command;
	};

	ref class hardware_device {
	public:
		hardware_device() { init(); }
		void init() {
			buzzer_stat = false;			
			powerlock_stat = false;
			vbatt1 = 12;
			vbatt2 = 12;
		}

		bool buzzer_stat;		
		bool powerlock_stat;
		int  vbatt1;
	    int  vbatt2;
	};

	ref class hardware_inputs {
	public:
		hardware_inputs() { init(); }		
		void init() {
			emergency = false;
			powerdown = false;
			cabinet_safety = false;
			closed_door = true;
			burning_jumper = false;
			battery_enable = false;

			xray_button_input = false;
			compression_detected = false; 
			keypad_up = false;
			keypad_down = false;
			keypad_cw = false;
			keypad_ccw = false;

			manual_slide_up = false;
			manual_slide_down = false;
			manual_body_cw = false;
			manual_body_ccw = false;

			pedalboard_up = false;
			pedalboard_down = false;
			pedalboard_cmp_up = false;
			pedalboard_cmp_down = false;

			power_off_req = false;
		}

		bool emergency;
		bool powerdown ;
		bool cabinet_safety;
		bool closed_door;
		bool burning_jumper;
		bool battery_enable;
		
		bool xray_button_input;
		bool compression_detected;

		bool keypad_up;
		bool keypad_down;
		bool keypad_cw;
		bool keypad_ccw;

		bool manual_slide_up;
		bool manual_slide_down;
		bool manual_body_cw;
		bool manual_body_ccw;

		bool  pedalboard_up;
		bool  pedalboard_down;
		bool pedalboard_cmp_up;
		bool pedalboard_cmp_down;

		bool power_off_req ;
	};

	ref class hardware_outputs {
	public:
		hardware_outputs() {init();}
		void init() {
			compression_ena = false;
			calibration_ena = false;
			pedalboard_compression_up = false;
			pedalboard_compression_down = false;
			generator_xray_ena = false;
			rotation_led = false;
			power_48VDC_stat = false;
			power_48SW_stat = false;

			xray_led = false;
			xray_lamp1 = false;
			xray_lamp2 = false;
		}

		bool compression_ena; 
		bool calibration_ena; 
		bool pedalboard_compression_up; 
		bool pedalboard_compression_down; 
		bool generator_xray_ena;
		bool rotation_led;
		bool power_48VDC_stat;
		bool power_48SW_stat;
		
		bool xray_led;
		bool xray_lamp1;
		bool xray_lamp2;

	};

	PCB301(void) :deviceInterface((unsigned short)0x10, setRegVal(1, 0, 0, 0), (Byte) ProtocolStructure::StatusRegister::register_index::NUM_REGISTER, (Byte)ProtocolStructure::DataRegister::register_index::NUM_REGISTER, (Byte)ProtocolStructure::ParameterRegister::register_index::NUM_REGISTER) {

	}
	static void  initialize(void) {};
	static PCB301^ board = gcnew PCB301;
	

	void device_workflow_callback(void) override;
	void device_reset_callback(void) override;
	
	static hardware_device device;
	static hardware_inputs inputs;
	static hardware_outputs outputs;
	static ProtocolStructure protocol;

};
