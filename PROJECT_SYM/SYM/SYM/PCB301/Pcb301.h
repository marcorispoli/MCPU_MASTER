#pragma once
#include "deviceInterface.h"

using namespace System;

#define NUM_STATUS (Byte) 2
#define NUM_DATA (Byte) 1
#define NUM_PARAM (Byte) 1



ref class PCB301: public deviceInterface
{
public:

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

	PCB301(void) :deviceInterface((unsigned short)0x10, setRegVal(1, 0, 0, 0), NUM_STATUS, NUM_DATA, NUM_PARAM) {

	}
	static void  initialize(void) {};
	static PCB301^ board = gcnew PCB301;
	

	void device_workflow_callback(void) override;
	void device_reset_callback(void) override;
	
	static hardware_device device;
	static hardware_inputs inputs;
	static hardware_outputs outputs;
};
