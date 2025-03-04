#pragma once
#include "deviceInterface.h"

using namespace System;

ref class PCB325: public deviceInterface
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
				SYSTEM_STATUS = 0,	//!> Internal statuis flag register 
				SYSTEM_XY,			//!> Current XY pointer position
				SYSTEM_ZS,			//!> Current Z pointer and Slider position
				NUM_REGISTER
			};

			/// <summary>
			///  This is the enumeration of the current motor working mode
			/// </summary>
			enum class motor_mode {
				MOTOR_DISABLE_MODE = 0,
				MOTOR_CALIBRATION_MODE,
				MOTOR_COMMAND_MODE,
				MOTOR_SERVICE_MODE
			};

			enum class xscroll {
				SCROLL_UNDEF = 0,
				SCROLL_CENTER,
				SCROLL_LEFT,
				SCROLL_RIGHT,

			};

			enum class needle {
				NEEDLE_A = 0,
				NEEDLE_B,
				NEEDLE_C,
				NEEDLE_NOT_PRESENT
			};

			

			static bool decodeSystemRegister(Register^ sys) {
				if (sys == nullptr) return false;

				// Byte 0 
				motor_working_mode = (motor_mode) sys->d0;

				// Byte 1
				motor_power_sw = sys->d1 & 0x1;
				motor_general_enable = sys->d1 & 0x2;
				motor_keyb_enable = sys->d1 & 0x4;
				motor_needle_enable = sys->d1 & 0x8;

				// Byte 2
				keystep_mode = sys->d2 & 0x1;
				yup_detected = sys->d2 & 0x2;
				xscroll_detected = (xscroll)((sys->d2 >> 2) & 0x3);
				needle_detected = (needle)((sys->d2 >> 4) & 0xF);

				return true;
			}

			static Register^ encodeSystemRegister(void) {
				Register^ sys = gcnew Register;

				// Byte 0
				sys->d0 = (unsigned char) motor_working_mode;

				// Byte 1
				if(motor_power_sw) sys->d1 |= 0x1;
				if(motor_general_enable)  sys->d1 |= 0x2;
				if(motor_keyb_enable) sys->d1 |= 0x4;
				if(motor_needle_enable)  sys->d1 |= 0x8;

				// Byte 2
				if(keystep_mode)  sys->d2 |= 0x1;
				if(yup_detected)  sys->d2 |= 0x2;
				sys->d2 |= (((unsigned char)xscroll_detected) & 0x3) << 2;
				sys->d2 |= (((unsigned char)needle_detected) & 0xF) << 4;

				return sys;
			}

			static bool decodeXYRegister(Register^ sys) {
				if (sys == nullptr) return false;

				// Byte 0 + 1
				x_position = (unsigned short) sys->d0 + 256 * (unsigned short) sys->d1;

				// Byte 2 + 3
				y_position = (unsigned short)sys->d2 + 256 * (unsigned short)sys->d3;
			
				return true;
			}

			static Register^ encodeXYRegister(void) {
				Register^ sys = gcnew Register;

				// Byte 0
				sys->d0 = (unsigned char)  x_position;
				sys->d1 = (unsigned char) (x_position>>8);
				sys->d2 = (unsigned char) y_position;
				sys->d3 = (unsigned char) (y_position >> 8);

				return sys;
			}

			
			static bool decodeZSRegister(Register^ sys) {
				if (sys == nullptr) return false;

				// Byte 0 + 1
				z_position = (unsigned short)sys->d0 + 256 * (unsigned short)sys->d1;

				// Byte 2 + 3
				s_position = (unsigned short)sys->d2 + 256 * (unsigned short)sys->d3;

				return true;
			}

			static Register^ encodeZSRegister(void) {
				Register^ sys = gcnew Register;

				// Byte 0
				sys->d0 = (unsigned char)z_position;
				sys->d1 = (unsigned char)(z_position >> 8);
				sys->d2 = (unsigned char)s_position;
				sys->d3 = (unsigned char)(s_position >> 8);

				return sys;
			}

			
			// System Status register definition
			static motor_mode motor_working_mode;
			static bool motor_power_sw;
			static bool motor_general_enable;
			static bool motor_keyb_enable;
			static bool motor_needle_enable;
			static bool keystep_mode;
			static bool yup_detected;
			static xscroll xscroll_detected;
			static needle  needle_detected;
			
			// XY Status
			static unsigned short x_position;
			static unsigned short y_position;

			// ZS Status
			static unsigned short z_position;
			static unsigned short s_position;


		};

		ref class DataRegister {
		public:

			enum class register_index {				
				NUM_REGISTER
			};
			

		};

		ref class ParameterRegister {
		public:
			enum class register_index {				
				NUM_REGISTER
			};

		};


		ref class Commands {
		public:
			enum class command_index {
				CMD_ABORT = 0,
				CMD_DISABLE_MODE,
				CMD_COMMAND_MODE,
				CMD_SERVICE_MODE,
				CMD_CALIB_MODE,
				CMD_MOVE_X,
				CMD_MOVE_Y,
				CMD_MOVE_Z,
				CMD_ENABLE_KEYSTEP,
				CMD_SERVICE_TEST_CYCLE
			};

			CanDeviceCommand^ encodeDisableModeCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_DISABLE_MODE, 0, 0, 0, 0);
			}
			CanDeviceCommand^ encodeCommandModeCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_COMMAND_MODE, 0, 0, 0, 0);
			}
			CanDeviceCommand^ encodeServiceModeCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_SERVICE_MODE, 0, 0, 0, 0);
			}
			CanDeviceCommand^ encodeCalibModeCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_CALIB_MODE, 0, 0, 0, 0);
			}
			CanDeviceCommand^ encodeTestCycleModeCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_SERVICE_TEST_CYCLE, 0, 0, 0, 0);
			}
			CanDeviceCommand^ encodeMoveXCommand(int X) {
				System::Byte d0 = (System::Byte)(X & 0xFF);
				System::Byte d1 = (System::Byte)((X >> 8) & 0xFF);
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_MOVE_X, d0, d1, 0, 0);
			}
			CanDeviceCommand^ encodeMoveYCommand(int Y) {
				System::Byte d0 = (System::Byte)(Y & 0xFF);
				System::Byte d1 = (System::Byte)((Y >> 8) & 0xFF);
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_MOVE_Y, d0, d1, 0, 0);
			}
			CanDeviceCommand^ encodeMoveZCommand(int Z) {
				System::Byte d0 = (System::Byte)(Z & 0xFF);
				System::Byte d1 = (System::Byte)((Z >> 8) & 0xFF);
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_MOVE_Z, d0, d1, 0, 0);
			}
			CanDeviceCommand^ encodeKeyStepEnableCommand(bool enable) {

				System::Byte d0 = (enable) ? 1 : 0;
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_ENABLE_KEYSTEP, d0, 0, 0, 0);
			}

		};

		static StatusRegister status_register;
		static DataRegister data_register;
		static ParameterRegister param_register;
		static Commands command;
	};

	ref class hardware_device {
	public:

		

		enum class motor_activation {
			MOTOR_NOT_ACTIVATED = 0,
			MOTOR_X,
			MOTOR_Y,
			MOTOR_Z
		};

		
		enum class keypress {
			XM = 0,
			XP,
			YM,
			YP,
			ZM,
			ZP,
			NOT_PRESSED
		};

		hardware_device() { init(); }

		void init() {
			connected = false;
			motor_working_mode = ProtocolStructure::StatusRegister::motor_mode::MOTOR_DISABLE_MODE;
			power_switch_stat = false;
			general_enable_stat = false;
			needle_enable_stat = false;
			keyboard_enable_stat = false;

			motor_command = motor_activation::MOTOR_NOT_ACTIVATED;
			
			Xtarget = Xposition = 1290;
			Ytarget = Yposition = 0;
			Ztarget = Zposition = 0;
			
			pointer_present = false;
			Slider = 15;

			XScroll = ProtocolStructure::StatusRegister::xscroll::SCROLL_CENTER;
			Yup_stat = true;
			
			
			Needle = ProtocolStructure::StatusRegister::needle::NEEDLE_NOT_PRESENT;
			key = keypress::NOT_PRESSED;
			keystep_mode = false;

			// To initialize the workflow
			working_mode_init = true;
		}

		
		// Motor section
		bool working_mode_init;
		ProtocolStructure::StatusRegister::motor_mode motor_working_mode;
		motor_activation motor_command;
		int  Xposition, Yposition, Zposition;
		int  Xtarget, Ytarget, Ztarget;

		// Safety switch control 
		bool power_switch_stat;
		bool general_enable_stat;
		bool needle_enable_stat;
		bool keyboard_enable_stat;
		
		// Pointer
		bool pointer_present;
		int  Slider;
		ProtocolStructure::StatusRegister::needle  Needle;

		// Axes control
		ProtocolStructure::StatusRegister::xscroll  XScroll;
		bool Yup_stat;
		
		// External keyboard and internal buttons
		keypress key;
		bool keystep_mode;

		bool connected;

		
		


	};

	ref class hardware_inputs {
	public:
		hardware_inputs() { init(); }		
		void init() {
		
		}
		
	};

	ref class hardware_outputs {
	public:
		hardware_outputs() {init();}
		void init() {
		
		}

		
	};

	PCB325(void) :deviceInterface((unsigned short)0x15, setRegVal(1, 0, 0, 0), (Byte) ProtocolStructure::StatusRegister::register_index::NUM_REGISTER, (Byte)ProtocolStructure::DataRegister::register_index::NUM_REGISTER, (Byte)ProtocolStructure::ParameterRegister::register_index::NUM_REGISTER) {
		request_abort_command = false;
		request_motor_working_mode = ProtocolStructure::StatusRegister::motor_mode::MOTOR_DISABLE_MODE;
	}
	static void  initialize(void) {};
	static PCB325^ board = gcnew PCB325;
	

	void device_workflow_callback(void) override;
	void device_reset_callback(void) override;
	commandResult^ device_command_callback(unsigned char cmd, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3) override;

	static hardware_device device;
	static hardware_inputs inputs;
	static hardware_outputs outputs;
	static ProtocolStructure protocol;

	// Internal management
	private:
			bool request_abort_command;
			ProtocolStructure::StatusRegister::motor_mode request_motor_working_mode;
			
			void updateStatusRegisters(void);
		    void motor_disable_workflow(void);
		    void motor_calibration_workflow(void);
		    void motor_command_workflow(void);
		    void motor_service_workflow(void);
			bool evaluate_power_switch_stat(void);
			void motor_activation_completed(bool result);

};
