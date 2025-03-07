#pragma once
#include "CanDeviceProtocol.h"

/// <summary>
/// \defgroup PCB326_Module PCB326 Module: Obstacle device module controller
/// 
/// \ingroup CanDevice_Module  
/// </summary>
ref class PCB325 : public CanDeviceProtocol
{


public:

	literal unsigned short BIOPSY_TARGET_X_RANGE = 5; //!< Minimum distance from the acceptable target  
	literal unsigned short BIOPSY_TARGET_Y_RANGE = 5; //!< Minimum distance from the acceptable target  
	literal unsigned short BIOPSY_TARGET_Z_RANGE = 5; //!< Minimum distance from the acceptable target  
	literal unsigned short BIOPSY_MAX_X_POSITION = 2580; //!< Maximum axes position  
	literal unsigned short BIOPSY_MAX_Y_POSITION = 700; //!< Maximum axes position  
	literal unsigned short BIOPSY_MAX_Z_POSITION = 1350; //!< Maximum axes position  

	PCB325() : CanDeviceProtocol(0x15, L"BIOPSY_DEVICE")
	{

	}
	static PCB325^ device = gcnew PCB325();

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
				motor_working_mode = (motor_mode)sys->d0;

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
				sys->d0 = (unsigned char)motor_working_mode;

				// Byte 1
				if (motor_power_sw) sys->d1 |= 0x1;
				if (motor_general_enable)  sys->d1 |= 0x2;
				if (motor_keyb_enable) sys->d1 |= 0x4;
				if (motor_needle_enable)  sys->d1 |= 0x8;

				// Byte 2
				if (keystep_mode)  sys->d2 |= 0x1;
				if (yup_detected)  sys->d2 |= 0x2;
				sys->d2 |= (((unsigned char)xscroll_detected) & 0x3) << 2;
				sys->d2 |= (((unsigned char)needle_detected) & 0xF) << 4;

				return sys;
			}

			static bool decodeXYRegister(Register^ sys) {
				if (sys == nullptr) return false;

				// Byte 0 + 1
				x_position = (unsigned short)sys->d0 + 256 * (unsigned short)sys->d1;

				// Byte 2 + 3
				y_position = (unsigned short)sys->d2 + 256 * (unsigned short)sys->d3;

				return true;
			}

			static Register^ encodeXYRegister(void) {
				Register^ sys = gcnew Register;

				// Byte 0
				sys->d0 = (unsigned char)x_position;
				sys->d1 = (unsigned char)(x_position >> 8);
				sys->d2 = (unsigned char)y_position;
				sys->d3 = (unsigned char)(y_position >> 8);

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
				NUM_REGISTERS = 0,
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
				System::Byte d1 = (System::Byte)((X>>8) & 0xFF);
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
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_MOVE_Z, d0, d1,0, 0);
			}
			CanDeviceCommand^ encodeKeyStepEnableCommand(bool enable) {
				
				System::Byte d0 = (enable) ? 1 : 0;
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_ENABLE_KEYSTEP, d0, 0, 0, 0);
			}

		};

		static StatusRegister status_register;
		static DataRegister data_register;
		static ParameterRegister parameter_register;
		static Commands command;
	};


	static bool isBiopsyConnected(void) { return biopsy_connected; }
	static void setBiopsyFreeMode(void) { request_working_mode = working_mode::BIOPSY_FREE_MODE; }
	static void setBiopsyDisableMode(void) { request_working_mode = working_mode::BIOPSY_DISABLE_MODE; }
	static void setBiopsyCalibrationMode(void) { request_working_mode = working_mode::BIOPSY_CALIBRATION_MODE; }
	static void setBiopsyCommandMode(void) { request_working_mode = working_mode::BIOPSY_COMMAND_MODE; }
	static void setBiopsyServiceMode(void) { request_working_mode = working_mode::BIOPSY_SERVICE_MODE; }
	static void setKeyStepMode(bool ena) { keystep_mode = ena; }
	static unsigned short getX(void) { return protocol.status_register.x_position; }
	static unsigned short getY(void) { return protocol.status_register.y_position; }
	static unsigned short getZ(void) { return protocol.status_register.z_position; }
	static unsigned short getS(void) { return protocol.status_register.s_position; }

	static bool isPointerMoving(void) {
		if (motor_command_result == 0) return true;
	}
	static bool isPointerSuccessfullyMoved(void) {
		if (motor_command_result == 1) return true;
	}
	static bool moveX(unsigned short x) { 
		if (!command_mode_ready) return false;
		if (request_motor_command != waiting_motor_command) return false;

		request_motor_command = motor_command::MOTOR_X;
		request_x = x;
		motor_command_result = 0;	
		return true;
	}

	static bool moveY(unsigned short y) {
		if (!command_mode_ready) return false;
		if (request_motor_command != waiting_motor_command) return false;

		request_motor_command = motor_command::MOTOR_Y;
		request_y = y;
		motor_command_result = 0;
		return true;
	}

	static bool moveZ(unsigned short z) {
		if (!command_mode_ready) return false;
		if (request_motor_command != waiting_motor_command) return false;

		request_motor_command = motor_command::MOTOR_Z;
		request_z = z;
		motor_command_result = 0;
		return true;
	}

	static bool isXtarget(unsigned short tg) {
		if (tg > protocol.status_register.x_position) {
			if (tg > protocol.status_register.x_position + BIOPSY_TARGET_X_RANGE) return false;
			return true;
		}
		else {
			if (protocol.status_register.x_position < BIOPSY_TARGET_X_RANGE) return true;
			if (tg < protocol.status_register.x_position - BIOPSY_TARGET_X_RANGE) return false;
			return true;
		}		
	}

	static bool isYtarget(unsigned short tg) {
		if (tg > protocol.status_register.y_position) {
			if (tg > protocol.status_register.y_position + BIOPSY_TARGET_Y_RANGE) return false;
			return true;
		}
		else {
			if (protocol.status_register.y_position < BIOPSY_TARGET_Y_RANGE) return true;
			if (tg < protocol.status_register.y_position - BIOPSY_TARGET_Y_RANGE) return false;
			return true;
		}
	}
	static bool isZtarget(unsigned short tg) {
		if (tg > protocol.status_register.z_position) {
			if (tg > protocol.status_register.z_position + BIOPSY_TARGET_Z_RANGE) return false;
			return true;
		}
		else {
			if (protocol.status_register.z_position < BIOPSY_TARGET_Z_RANGE) return true;
			if (tg < protocol.status_register.z_position - BIOPSY_TARGET_Z_RANGE) return false;
			return true;
		}
	}

	static bool isYUp(void) { return protocol.status_register.yup_detected; }

	// Scroll position iniquiry
	static bool isXC(void) { return (protocol.status_register.xscroll_detected == ProtocolStructure::StatusRegister::xscroll::SCROLL_CENTER); }
	static bool isXL(void) { return (protocol.status_register.xscroll_detected == ProtocolStructure::StatusRegister::xscroll::SCROLL_LEFT); }
	static bool isXR(void) { return (protocol.status_register.xscroll_detected == ProtocolStructure::StatusRegister::xscroll::SCROLL_RIGHT); }
	static bool isXU(void) { return (protocol.status_register.xscroll_detected == ProtocolStructure::StatusRegister::xscroll::SCROLL_UNDEF); }

	// Group Y position evaluation respect the Z group
	static bool isGroupYleftThanBody() { return true; }
	static bool isGroupYrightThanBody() { return false; }
	static bool moveXHomeLeft(void) { return true; }
	static bool moveXHomeRight(void) { return true; }


	static bool activateCycleTest(void);
	
	//static void setLight(bool status, int duty) { protocol.data_register.light_status = status;  protocol.data_register.light_duty = duty; }
	static void moveHome(void) { command_home_request = true; }

protected: 	
	void runningLoop(void) override;
	bool configurationLoop(void) override;//!< This is the Base Class override function to handle the device configuration

private:
	static ProtocolStructure protocol; //!< This is the structure with the Status register info
	static bool biopsy_connected = false;
	static bool keystep_mode = false;
	static bool command_mode_ready = false;

	static bool service_cycle_test = false;
	static bool command_home_request = false;
	static int sequence = 0;

	
	enum class working_mode {
		BIOPSY_FREE_MODE = 0,			//!< The Biopsy device is left operating in self defined mode (calibration/disable)
		BIOPSY_DISABLE_MODE,		//!< The Biopsy is forced to be in disable mode
		BIOPSY_CALIBRATION_MODE,	//!< The Biopsy is forced to be in calibration mode
		BIOPSY_COMMAND_MODE,		//!< The Biopsy is forced to be in command mode
		BIOPSY_SERVICE_MODE			//!< The Biopsy is forced to be in service mode
	};

	static working_mode request_working_mode = working_mode::BIOPSY_FREE_MODE;
	static working_mode biopsy_working_mode = working_mode::BIOPSY_FREE_MODE;

	enum class motor_command {
		NO_COMMAND = 0,
		MOTOR_X,
		MOTOR_Y,
		MOTOR_Z,
	};

	static motor_command request_motor_command;
	static motor_command waiting_motor_command;
	static unsigned short request_x, request_y, request_z;
	static int motor_command_result;

	void run_free_mode_workflow(void); 
	void run_disable_mode_workflow(void);
	void run_calibration_mode_workflow(void);
	void run_command_mode_workflow(void);
	void run_service_mode_workflow(void);
	void resetPositioningErrorMessages(void);
	
};

