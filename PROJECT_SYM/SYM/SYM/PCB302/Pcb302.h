#pragma once
#include "deviceInterface.h"

using namespace System;

#define NUM_STATUS (Byte) 4
#define NUM_DATA (Byte) 2
#define NUM_PARAM (Byte) 5



ref class PCB302: public deviceInterface
{
public:
	PCB302(void) :deviceInterface((unsigned short)0x11, setRegVal(1, 0, 0, 0), NUM_STATUS, NUM_DATA, NUM_PARAM) {

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

};

