#pragma once
#include "deviceInterface.h"

using namespace System;


ref class PCB302: public deviceInterface
{
public:
	
	PCB302(void) :deviceInterface((unsigned short)0x11, setRegVal(1, 0, 0, 0), (Byte)ProtocolStructure::StatusRegister::register_index::NUM_REGISTER, (Byte)ProtocolStructure::DataRegister::register_index::NUM_REGISTER, (Byte)ProtocolStructure::ParameterRegister::register_index::NUM_REGISTER) {

	}
	static void  initialize(void) {};
	static PCB302^ board = gcnew PCB302;
	

	enum class  paddleCodes {
		PADDLE_PROSTHESIS = 0, //!< Paddle PROSTHESIS format
		PADDLE_BIOP2D, //!< Paddle BIOPSY 2D format
		PADDLE_BIOP3D, //!< Paddle BIOPSY STEREO format
		PADDLE_TOMO, //!< Paddle TOMO 24x30 format
		PADDLE_24x30_CONTACT, //!< Paddle 24x30 format
		PADDLE_18x24_C_CONTACT, //!< Paddle 18x24 CENTER format
		PADDLE_18x24_L_CONTACT, //!< Paddle 18x24 LEFT format
		PADDLE_18x24_R_CONTACT, //!< Paddle 18x24 RIGHT format
		PADDLE_10x24_CONTACT, //!< Paddle 10x24 format
		PADDLE_9x21_MAG, //!< Paddle 9x21(MAG) format
		PADDLE_9x9_MAG, //!< Paddle TOMO 9x9(MAG) format
		PADDLE_D75_MAG, //!< Paddle D75(MAG) format
		PADDLE_LEN,
		PADDLE_NOT_DETECTED = PADDLE_LEN
	};

	ref class ProtocolStructure {
	public:

		ref class StatusRegister {
		public:

			enum class register_index {
				SYSTEM_REGISTER = 0, //!> This is the System Status register index
				SPARE_REGISTER,
				PADDLE_REGISTER,
				RAW_PADDLE_REGISTER,
				NUM_REGISTER
			};

			static bool decodeSystemRegister(Register^ sys) {
				if (sys == nullptr) return false;

				// Byte 0 of the register
				manual_servo_up = sys->d0 & 0x1;
				manual_servo_down = sys->d0 & 0x2;
				manual_pedal_up = sys->d0 & 0x4;
				manual_pedal_down = sys->d0 & 0x8;

				// Byte 1
				idle_status = sys->d1 & 0x1;
				manual_activation = sys->d1 & 0x2;
				pedal_activation = sys->d1 & 0x4;
				command_activation = sys->d1 & 0x8;
				upward_direction = sys->d1 & 0x10;
				downward_direction = sys->d1 & 0x20;
				unlock_activation = sys->d1 & 0x40;
				device_fault = sys->d1 & 0x80;

				// Byte 2
				compression_ena = sys->d2 & 0x1;
				compression_on = sys->d2 & 0x2;
				zero_compression_mode = sys->d2 & 0x4;
				smart_target = sys->d2 & 0x8;
				force_target = sys->d2 & 0x10;
				limit_compression = sys->d2 & 0x20;

				// Byte 3
				// Not implemented
				return true;
			}
			Register^ encodeSystemRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ sys = gcnew Register;

				// Byte 0 of the register
				if (manual_servo_up) sys->d0 |= 0x1;
				if (manual_servo_down) sys->d0 |= 0x2;
				if (manual_pedal_up)sys->d0 |= 0x4;
				if (manual_pedal_down)sys->d0 |= 0x8;

				// Byte 1
				if (idle_status)sys->d1 |= 0x1;
				if (manual_activation)sys->d1 |= 0x2;
				if (pedal_activation) sys->d1 |= 0x4;
				if (command_activation) sys->d1 |= 0x8;
				if (upward_direction) sys->d1 |= 0x10;
				if (downward_direction) sys->d1 |= 0x20;
				if (unlock_activation) sys->d1 |= 0x40;
				if (device_fault) sys->d1 |= 0x80;

				// Byte 2
				if (compression_ena) sys->d2 |= 0x1;
				if (compression_on) sys->d2 |= 0x2;
				if (zero_compression_mode) sys->d2 |= 0x4;
				if (smart_target)sys->d2 |= 0x8;
				if (force_target) sys->d2 |= 0x10;
				if (limit_compression)sys->d2 |= 0x20;

				// Returns the formatted register
				return sys;
			}

			static bool decodePaddleRegister(Register^ pad) {
				if (pad == nullptr) return false;
				paddle_position = (int)pad->d0 + 256 * (int)(pad->d1 & 0x0f);
				paddle_force = (int)((pad->d1 & 0xF0) >> 4) + 16 * (int)pad->d2;
				paddle_code = pad->d3;
				return true;
			}
			Register^ encodePaddleRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ pad = gcnew Register;

				pad->d0 = (unsigned char)(paddle_position & 0xFF);
				pad->d1 = (unsigned char)((paddle_position >> 8) & 0x0f);
				pad->d1 |= ((unsigned char)(paddle_force & 0x0F) << 4);
				pad->d2 = (unsigned char)((paddle_force >> 4) & 0xff);
				pad->d3 = paddle_code;

				// Returns the formatted register
				return pad;
			}

			static bool decodeRawPaddleRegister(Register^ rpad) {
				if (rpad == nullptr) return false;

				paddle_raw_position = (int)rpad->d0 + 256 * (int)(rpad->d1 & 0x0f);
				paddle_raw_force = (int)((rpad->d1 & 0xF0) >> 4) + 16 * (int)rpad->d2;
				paddle_raw_code = rpad->d3;
				return true;
			}

			Register^ encodeRawPaddleRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ pad = gcnew Register;

				pad->d0 = (unsigned char)(paddle_raw_position & 0xFF);
				pad->d1 = (unsigned char)((paddle_raw_position >> 8) & 0x0f);
				pad->d1 |= ((unsigned char)(paddle_raw_force & 0x0F) << 4 );
				pad->d2 = (unsigned char)((paddle_raw_force >> 4) & 0xff);
				pad->d3 = paddle_raw_code;

				// Returns the formatted register
				return pad;
			}


			static bool manual_servo_up = false;
			static bool manual_servo_down = false;
			static bool manual_pedal_up = false;
			static bool manual_pedal_down = false;

			static bool idle_status = false;
			static bool manual_activation = false;
			static bool pedal_activation = false;
			static bool command_activation = false;
			static bool upward_direction = false;
			static bool downward_direction = false;
			static bool unlock_activation = false;
			static bool device_fault = false;

			static bool compression_ena = false;
			static bool compression_on = false;
			static bool zero_compression_mode = false;
			static bool smart_target = false;
			static bool force_target = false;
			static bool limit_compression = false;


			static int paddle_position = 0;
			static int paddle_force = 0;
			static unsigned char paddle_code = 0;

			static int paddle_raw_position = 0;
			static int paddle_raw_force = 0;
			static unsigned char paddle_raw_code = 0;

		};


		ref class DataRegister {
		public:

			enum class register_index {
				POSITION_LIMIT_REGISTER = 0,
				OPTIONS_REGISTER,
				NUM_REGISTER
			};

			Register^ encodePositionLimitRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ out = gcnew Register;

				out->d0 = position_limit & 0xFF;
				out->d1 = (unsigned char)(position_limit >> 8) & 0xFF;

				// Returns the formatted register
				return out;
			}
			static bool decodePositionLimitRegister(Register^ reg) {
				if (reg == nullptr) return false;
				position_limit = reg->d0 + 256 * reg->d1;
				return true;
			}

			Register^ encodeOptionsRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ out = gcnew Register;

				if (position_calibration) out->d0 |= 0x1;
				if (force_calibration) out->d0 |= 0x2;
				if (master_enable) out->d0 |= 0x4;
				if (zero_compression_mode) out->d0 |= 0x8;


				// Returns the formatted register
				return out;
			}
			static bool decodeOptionsRegister(Register^ reg) {
				if (reg == nullptr) return false;

				position_calibration = reg->d0 & 0x1;
				force_calibration = reg->d0 & 0x2;
				master_enable = reg->d0 & 0x4;
				zero_compression_mode = reg->d0 & 0x8;
				return true;
			}

			static unsigned int  position_limit = 0;
			static bool position_calibration = false;
			static bool force_calibration = false;
			static bool master_enable = false;
			static bool zero_compression_mode = false;


		};

		ref class ParameterRegister {
		public:
			enum class register_index {
				POSITION_PARAM_REGISTER = 0,
				FORCE_CALIBRATION_PARAM_REGISTER,
				COMPRESSION_PARAM_REGISTER,
				UPWARD_PARAM_REGISTER,
				DOWNWARD_PARAM_REGISTER,
				NUM_REGISTER
			};

			//writeParamRegister(unsigned char idx, Register^ reg)

			Register^ encodePositionParamRegister(unsigned int offset, unsigned int gain) {
				unsigned char d0 = (unsigned char)offset;
				unsigned char d1 = (unsigned char)(offset >> 8);
				unsigned char d2 = (unsigned char)gain;
				unsigned char d3 = (unsigned char)(gain >> 8);

				return gcnew Register(d0, d1, d2, d3);
			}

			Register^ encodeForceParamRegister(unsigned int offset, unsigned int gain) {
				unsigned char d0 = (unsigned char)offset;
				unsigned char d1 = (unsigned char)(offset >> 8);
				unsigned char d2 = (unsigned char)gain;
				unsigned char d3 = (unsigned char)(gain >> 8);
				return gcnew Register(d0, d1, d2, d3);
			}

			Register^ encodeCompressionParamRegister(unsigned char limit, unsigned char target) {
				unsigned char d0 = (unsigned char)limit;
				unsigned char d1 = (unsigned char)target;
				unsigned char d2 = 0;
				unsigned char d3 = 0;
				return gcnew Register(d0, d1, d2, d3);
			}


		};

		ref class Commands {
		public:
			enum class command_index {
				ABORT_COMMAND = 0, //!< Abort Command (mandatory as for device protocol)
				SET_TRIMMERS_COMMAND,
				SET_COMPRESSION,
				SET_UNLOCK,
				NUM_COMMANDS
			};

			CanDeviceCommand^ encodeSetUnlockCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::SET_UNLOCK, 0, 0, 0, 0);
			}

		};

		static StatusRegister status_register;
		static DataRegister data_register;
		static ParameterRegister parameter_register;
		static Commands command;
	};

	ref class hardware_device {
	public:
		hardware_device() { init(); }
		void init() {
			target_force = 150;
			target_thickness = 50;
			current_force = 0;
			current_thickness = 0;
			current_paddle_code = paddleCodes::PADDLE_NOT_DETECTED;

		}

		int target_force;
		int target_thickness;
		int current_force;
		int current_thickness;
		paddleCodes current_paddle_code;
	
	};

	ref class hardware_inputs {
	public:
		hardware_inputs() { init(); }
		void init() {
			compression_ena = false; //!< This signal should be set by the PCB301 board (bus hardware)
			calibration_ena = false; //!< This signal should be set by the PCB301 board (bus hardware)
			compression_up = false; //!< This signal should be set by the PCB301 board (bus hardware)
			compression_down = false; //!< This signal should be set by the PCB301 board (bus hardware)
			paddle_up = false;
			paddle_down = false;
		}

		bool compression_ena; 
		bool calibration_ena;
		bool compression_up;
		bool compression_down; 
		bool paddle_up;
		bool paddle_down;
		
	};

	ref class hardware_outputs {
	public:
		hardware_outputs() { init(); }
		void init() {
			compression_detected = false;
		}

		bool compression_detected;

	};

	void device_workflow_callback(void) override;
	void device_reset_callback(void) override;

	static hardware_device device;
	static hardware_inputs inputs;
	static hardware_outputs outputs;
	static ProtocolStructure protocol;
};

