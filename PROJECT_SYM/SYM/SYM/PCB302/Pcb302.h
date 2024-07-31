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

		/// <summary>
		/// This is the enumeration of the detected component.
		/// </summary>
		/// 
		/// The Component can be one of the following device:
		/// + PATIENT PROTECTION IN THE RIGHT POSITION;
		/// + PATIENT PROTECTION SHIFTED;
		/// + Magnifier Device with 1.5x factor;
		/// + Magnifier Device with 1.8x factor;
		/// + Magnifier Device with 2x factor;
		/// 
		enum class  ComponentCode {
			SHORT_CIRCUIT = 0,
			PATIENT_PROTECTION_SHITED,
			PATIENT_PROTECTION_POSITIONED,
			MAGNIFIER_DEVICE_15x,
			MAGNIFIER_DEVICE_18x,
			MAGNIFIER_DEVICE_20x,
			UNDETECTED_COMPONENT
		};

		ref class StatusRegister {
		public:

			enum class register_index {
				SYSTEM_REGISTER = 0, //!> This is the System Status register index
				SPARE_REGISTER,
				PADDLE_REGISTER,
				RAW_PADDLE_REGISTER
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

				device_fault = sys->d1 & 0x80;

				// Byte 2
				compression_ena = sys->d2 & 0x1;
				compression_on = sys->d2 & 0x2;

				smart_target = sys->d2 & 0x8;
				force_target = sys->d2 & 0x10;
				limit_compression = sys->d2 & 0x20;

				// Byte 3
				if (sys->d3 >= (unsigned char)ComponentCode::UNDETECTED_COMPONENT) component = ComponentCode::UNDETECTED_COMPONENT;
				else component = (ComponentCode)sys->d3;
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

				if (device_fault) sys->d1 |= 0x80;

				// Byte 2
				if (compression_ena) sys->d2 |= 0x1;
				if (compression_on) sys->d2 |= 0x2;

				if (smart_target)sys->d2 |= 0x8;
				if (force_target) sys->d2 |= 0x10;
				if (limit_compression)sys->d2 |= 0x20;

				// Byte 3
				sys->d3 = (unsigned char)component;

				// Returns the formatted register
				return sys;
			}



			static bool decodePaddleRegister(Register^ pad) {
				if (pad == nullptr) return false;
				paddle_position = (int)pad->d0 + 256 * (int)(pad->d1 & 0x0f);
				paddle_force = (int)((pad->d1 & 0xF0) >> 4) + 16 * (int)pad->d2;
				paddle_tag = pad->d3;
				return true;
			}

			Register^ encodePaddleRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ pad = gcnew Register;

				pad->d0 = (unsigned char)(paddle_position & 0xFF);
				pad->d1 = (unsigned char)((paddle_position >> 8) & 0x0f);
				pad->d1 |= ((unsigned char)(paddle_force & 0x0F) << 4);
				pad->d2 = (unsigned char)((paddle_force >> 4) & 0xff);
				pad->d3 = paddle_tag;

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
				pad->d1 |= ((unsigned char)(paddle_raw_force & 0x0F) >> 4);
				pad->d2 = (unsigned char)((paddle_raw_force >> 4) & 0xff);
				pad->d3 = paddle_raw_code;

				// Returns the formatted register
				return pad;
			}


			static bool manual_servo_up = false; //!< The Manual Servo Up button is activated
			static bool manual_servo_down = false;//!< The Manual Servo Down button is activated
			static bool manual_pedal_up = false;//!< The Manual Compression Pedal Up button is activated
			static bool manual_pedal_down = false;//!< The Manual Compression Pedal Down button is activated

			static bool idle_status = false;//!< The Compressor is in Idle (no activation is pending)
			static bool manual_activation = false;//!< The Servo manual activation is executing
			static bool pedal_activation = false;//!< The Pedal manual activation is executing
			static bool command_activation = false;//!< The Protocol command  activation is executing
			static bool upward_direction = false;//!< The paddle is activated Upward
			static bool downward_direction = false;//!< The paddle is activated Downward

			static bool device_fault = false;//!< The device is in fault condition (see the Error register)

			static bool compression_ena = false;//!< The compressor Ena hardware input is detected
			static bool compression_on = false;	//!< A valid compression is detected (Compression-On)

			static bool smart_target = false;//!< The Smart uPress has been detected
			static bool force_target = false;//!< The Target compression is detected
			static bool limit_compression = false;//!< The Limit compression is detected

			static int paddle_position = 0; //! Current calibrated paddle position
			static int paddle_force = 0;//! Current calibrated paddle force
			static unsigned char paddle_tag = 0;//! Current detected paddle tag

			static int paddle_raw_position = 0;//! Current sensor position value
			static int paddle_raw_force = 0;//! Current sensor force value
			static unsigned char paddle_raw_code = 0;//! Current paddle code

			static ComponentCode component = ComponentCode::UNDETECTED_COMPONENT; //!< Current detected Component code

		};


		ref class DataRegister {
		public:

			enum class register_index {
				HOLDER_LIMITS = 0,
				COMPRESSOR_LIMITS,
				PADDLE_WEIGHT
			};

			static bool decodeHolderLimitsRegister(Register^ reg) {
				if (reg == nullptr) return false;
				max_position = reg->d0 + 256 * reg->d1;
				min_position = reg->d2 + 256 * reg->d3;
				return true;
			}

			Register^ encodeHolderLimitsRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ out = gcnew Register;

				out->d0 = (unsigned char)max_position;
				out->d1 = (unsigned char)(max_position >> 8);
				out->d2 = (unsigned char)min_position;
				out->d3 = (unsigned char)(min_position >> 8);


				// Returns the formatted register
				return out;
			}

			static bool decodeCompressorLimitsRegister(Register^ reg) {
				if (reg == nullptr) return false;
				limit_compression = reg->d0;
				target_compression = reg->d1;
				return true;
			}


			Register^ encodeCompressorLimitsRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ out = gcnew Register;

				out->d0 = (unsigned char)limit_compression;
				out->d1 = (unsigned char)target_compression;

				// Returns the formatted register
				return out;
			}

			static bool decodePaddleWeightRegister(Register^ reg) {
				if (reg == nullptr) return false;
				absolute_arm_angle = reg->d0;
				paddle_weight = reg->d1;
				paddle_distance_from_plane = (unsigned short)reg->d2 + 256 * (unsigned short)reg->d3;
				return true;
			}

			Register^ encodePaddleWeightRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ out = gcnew Register;

				out->d0 = absolute_arm_angle;
				out->d1 = paddle_weight;
				out->d2 = (unsigned char)paddle_distance_from_plane;
				out->d3 = (unsigned char)(paddle_distance_from_plane >> 8);

				// Returns the formatted register
				return out;
			}


			static unsigned char  absolute_arm_angle = 0;//!< Current detected Arm angle
			static unsigned char  paddle_weight = 0;//!< Current detected paddle weight

			static unsigned char limit_compression = 200;//!< Current limit compression
			static unsigned char target_compression = 150;//!< Current target compression

			static unsigned short max_position = 0; //!< Max Holder position
			static unsigned short min_position = 0; //!< Min holder position

			static unsigned short paddle_distance_from_plane = 0; //!< Distance from the paddle plane to the compression plane


		};

		ref class ParameterRegister {
		public:
			enum class register_index {
				HOLDER_CALIB = 0,
				COMPRESSION_CALIB
			};

			static bool decodeHolderCalibRegister(Register^ reg) {
				if (reg == nullptr) return false;
				Kp = reg->d0;
				Op = reg->d1 + 256 * reg->d2;
				return true;
			}

			Register^ encodeHolderCalibRegister(void) {
				unsigned char d0 = Kp;
				unsigned char d1 = (unsigned char)Op;
				unsigned char d2 = (unsigned char)(Op >> 8);
				unsigned char d3 = 0;
				return gcnew Register(d0, d1, d2, d3);
			}

			static bool decodeCompressionCalibRegister(Register^ reg) {
				if (reg == nullptr) return false;

				return true;
			}

			Register^ encodeCompressionCalibRegister(void) {

				unsigned char d0 = 0;
				unsigned char d1 = 0;
				unsigned char d2 = 0;
				unsigned char d3 = 0;
				return gcnew Register(d0, d1, d2, d3);
			}


			static unsigned char Kp;
			static unsigned short Op;

		};

		ref class Commands {
		public:
			enum class command_index {
				ABORT_COMMAND = 0, //!< Abort Command (mandatory as for device protocol)
				SET_TRIMMERS_COMMAND,
				SET_COMPRESSION,
				SET_UNLOCK,
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
			
			breast_thickness = 50;
			current_force = 0;
			paddle_holder_position = 300;
			current_paddle_code = paddleCodes::PADDLE_NOT_DETECTED;

		}
		
		int breast_thickness;		//!< Sets the breast dimension put on the plane
		int paddle_holder_position; //!< Current calibrated paddle holder
		int current_force;			//!< Current sensor force
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

