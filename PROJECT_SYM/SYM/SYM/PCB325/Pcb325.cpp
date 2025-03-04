#include "Pcb325.h"

/// <summary>
/// Updates all the Status registers when required
/// </summary>
/// <param name=""></param>
void PCB325::updateStatusRegisters(void) {

	protocol.status_register.motor_working_mode = device.motor_working_mode;
	protocol.status_register.motor_power_sw = device.power_switch_stat;
	protocol.status_register.motor_general_enable = device.general_enable_stat;
	protocol.status_register.motor_keyb_enable = device.keyboard_enable_stat;
	protocol.status_register.motor_needle_enable = device.needle_enable_stat;
	protocol.status_register.keystep_mode = device.keystep_mode;
	protocol.status_register.yup_detected = device.Yup_stat;
	protocol.status_register.xscroll_detected = device.XScroll;
	protocol.status_register.needle_detected = device.Needle;
	status_registers[(Byte)ProtocolStructure::StatusRegister::register_index::SYSTEM_STATUS] = protocol.status_register.encodeSystemRegister();

	protocol.status_register.x_position = device.Xposition;
	protocol.status_register.y_position = device.Yposition;
	status_registers[(Byte)ProtocolStructure::StatusRegister::register_index::SYSTEM_XY] = protocol.status_register.encodeXYRegister();

	protocol.status_register.z_position = device.Zposition;
	protocol.status_register.s_position = device.Slider;
	status_registers[(Byte)ProtocolStructure::StatusRegister::register_index::SYSTEM_ZS] = protocol.status_register.encodeZSRegister();

}

void PCB325::motor_disable_workflow(void){
		static int key_timer = (1000 / _DEVICE_TASK_EXECUTION_DELAY_ms);

		if (device.working_mode_init) {
			device.working_mode_init = false;
			key_timer = (1000 / _DEVICE_TASK_EXECUTION_DELAY_ms);
		}

		device.general_enable_stat = false;
		device.keyboard_enable_stat = false;		
		device.keystep_mode = false;


		// Evaluates the chage status with the Keyboard
		if (device.key == hardware_device::keypress::NOT_PRESSED) key_timer = (1000 / _DEVICE_TASK_EXECUTION_DELAY_ms);
		else key_timer--;

		// Change the current working mode to CALIBRATION mode
		if (key_timer == 0) {
			request_motor_working_mode = ProtocolStructure::StatusRegister::motor_mode::MOTOR_CALIBRATION_MODE;
			return;
		}

}

void PCB325::motor_calibration_workflow(void) {
	static int no_activity_timer = (60000 / _DEVICE_TASK_EXECUTION_DELAY_ms);
	static bool keystat = false;

	if (device.working_mode_init) {
		device.working_mode_init = false;
		no_activity_timer = (60000 / _DEVICE_TASK_EXECUTION_DELAY_ms);

		// init of the key transition handling
		if (device.key != hardware_device::keypress::NOT_PRESSED) keystat = true;
		else keystat = false;
	}
	
	// Disables the general enable 
	device.general_enable_stat = true;	
	device.keyboard_enable_stat = true;
	device.keystep_mode = false;

	if (device.key != hardware_device::keypress::NOT_PRESSED) no_activity_timer = (60000 / _DEVICE_TASK_EXECUTION_DELAY_ms);
	else no_activity_timer--;
	
	// Change the current working mode to DISABLE mode
	if (no_activity_timer == 0) {
		request_motor_working_mode = ProtocolStructure::StatusRegister::motor_mode::MOTOR_DISABLE_MODE;
		return;
	}

	// Activate the calibration commands
	if (keystat) {
		if (device.key == hardware_device::keypress::NOT_PRESSED) keystat = false; // button release
	}
	else {
		if (device.key != hardware_device::keypress::NOT_PRESSED) {
			keystat = true; // button pression

			// Command identification
			if (device.key == hardware_device::keypress::XM) {
				device.Xtarget = 0;
				device.motor_command = hardware_device::motor_activation::MOTOR_X;
				return;
			}

			if (device.key == hardware_device::keypress::XP) {
				if (device.Xtarget == 1290) device.Xtarget = 2580;
				else device.Xtarget = 1290;
				device.motor_command = hardware_device::motor_activation::MOTOR_X;
				return;
			}

			if (device.key == hardware_device::keypress::YM) {
				device.Ytarget = 0;
				device.motor_command = hardware_device::motor_activation::MOTOR_Y;
				return;
			}

			if (device.key == hardware_device::keypress::YP) {
				device.Ytarget = 700;
				device.motor_command = hardware_device::motor_activation::MOTOR_Y;
				return;
			}

			if (device.key == hardware_device::keypress::ZM) {
				device.Ztarget = 0;
				device.motor_command = hardware_device::motor_activation::MOTOR_Z;
				return;
			}

			if (device.key == hardware_device::keypress::ZP) {
				device.Ztarget = 1000;
				device.motor_command = hardware_device::motor_activation::MOTOR_Z;
				return;
			}
		}
	}

}
void PCB325::motor_command_workflow(void) {
	static bool keystat = false;

	if (device.working_mode_init) {
		device.working_mode_init = false;
		
		// init of the key transition handling
		if (device.key != hardware_device::keypress::NOT_PRESSED) keystat = true;
		else keystat = false;
	}

	// Always the general enable ON
	device.general_enable_stat = true;
	
	// Keyboard only with the keystep mode activated
	if(device.keystep_mode) device.keyboard_enable_stat = true;
	else device.keyboard_enable_stat = false;
	
	// Handle the KeyStep mode
	// Activate the calibration commands
	if (keystat) {
		if (device.key == hardware_device::keypress::NOT_PRESSED) keystat = false; // button release
	}
	else {
		if (device.key != hardware_device::keypress::NOT_PRESSED) {
			keystat = true; // button pression
			if (!device.keystep_mode) return;

			// Command identification
			if (device.key == hardware_device::keypress::XM) {
				if (device.Xposition < 10) device.Xtarget = 0;
				else device.Xtarget = device.Xposition - 10;
				
				device.motor_command = hardware_device::motor_activation::MOTOR_X;
				return;
			}

			if (device.key == hardware_device::keypress::XP) {
				if (device.Xposition > 2570) device.Xtarget = 2580;
				else device.Xtarget = device.Xposition + 10;

				device.motor_command = hardware_device::motor_activation::MOTOR_X;
				return;
			}

			if (device.key == hardware_device::keypress::YM) {
				if (device.Yposition < 10) device.Ytarget = 0;
				else device.Ytarget = device.Yposition - 10;
				device.motor_command = hardware_device::motor_activation::MOTOR_Y;
				return;
			}

			if (device.key == hardware_device::keypress::YP) {
				if (device.Yposition > 690) device.Ytarget = 700;
				else device.Ytarget = device.Yposition + 10;
				device.motor_command = hardware_device::motor_activation::MOTOR_Y;
				return;
			}

			if (device.key == hardware_device::keypress::ZM) {
				if (device.Zposition < 10) device.Ztarget = 0;
				else device.Ztarget = device.Zposition - 10;

				device.motor_command = hardware_device::motor_activation::MOTOR_Z;
				return;
			}

			if (device.key == hardware_device::keypress::ZP) {
				if (device.Zposition > 1390) device.Ztarget = 1400;
				else device.Ztarget = device.Zposition + 10;
				device.motor_command = hardware_device::motor_activation::MOTOR_Z;
				return;
			}
		}
	}

}
void PCB325::motor_service_workflow(void) {

}

bool PCB325::evaluate_power_switch_stat(void) {
	
	// In case of no general enable 
	if (!device.general_enable_stat) return false;

	// In case of keboard activation with the keyboard enabled to activate..
	if ((device.key != hardware_device::keypress::NOT_PRESSED) && (device.keyboard_enable_stat)) return true;

	// In here it depends only by the needle identification
	return device.needle_enable_stat;

}

void PCB325::motor_activation_completed(bool result) {
	if(result) commandCompleted(0,0);
	device.motor_command = hardware_device::motor_activation::MOTOR_NOT_ACTIVATED;
	return;
}


// 50ms task execution (_DEVICE_TASK_EXECUTION_DELAY_ms)
void PCB325::device_workflow_callback(void) {

	// No DATA registers need to be decoded before to evaluate the engine.
	if (request_motor_working_mode != device.motor_working_mode) {
		device.motor_working_mode = request_motor_working_mode;
		device.working_mode_init = true;
		commandCompleted(0,0);
	}

	// Working mode management
	switch (device.motor_working_mode) {
	case ProtocolStructure::StatusRegister::motor_mode::MOTOR_DISABLE_MODE:     PCB325::motor_disable_workflow(); break;
	case ProtocolStructure::StatusRegister::motor_mode::MOTOR_CALIBRATION_MODE: PCB325::motor_calibration_workflow(); break;
	case ProtocolStructure::StatusRegister::motor_mode::MOTOR_COMMAND_MODE:     PCB325::motor_command_workflow(); break;
	case ProtocolStructure::StatusRegister::motor_mode::MOTOR_SERVICE_MODE:     PCB325::motor_service_workflow(); break;

	}
	

	// Pointer presence and needle enable evaluation 
	if (device.pointer_present) {
		if(device.Needle == ProtocolStructure::StatusRegister::needle::NEEDLE_NOT_PRESENT) device.needle_enable_stat = true;
		else device.needle_enable_stat = false;
	}
	else {
		device.needle_enable_stat = true;
	}

	// Evaluation of the safety power switch status
	device.power_switch_stat = evaluate_power_switch_stat();
	
	// Evaluates a possible impact event when Y is not up
	int xmax_pointer, xmin_pointer, xmax_base, xmin_base;
	bool possible_crash = false;

	if (!device.Yup_stat) {
		xmax_pointer = device.Xposition + (Y_WIDTH / 2);
		xmin_pointer = device.Xposition - (Y_WIDTH / 2);

		if (device.XScroll == ProtocolStructure::StatusRegister::xscroll::SCROLL_CENTER) {
			xmin_base = 1290 - (Z_WIDTH / 2);
			xmax_base = 1290 + (Z_WIDTH / 2);
		} else if (device.XScroll == ProtocolStructure::StatusRegister::xscroll::SCROLL_RIGHT) {
			xmin_base = 2580 - Z_WIDTH;
			xmax_base = 2580;

		}else if (device.XScroll == ProtocolStructure::StatusRegister::xscroll::SCROLL_RIGHT) {
			xmin_base = 0;
			xmax_base = (Z_WIDTH);
		}

		if ((xmin_pointer <= xmax_base) && (xmin_pointer >= xmin_base)) device.crash_event = true;
		else if ((xmax_pointer <= xmax_base) && (xmax_pointer >= xmin_base)) device.crash_event = true;
		else device.crash_event = false;
	}else device.crash_event = false;
	
	// Motor activation management
	switch (device.motor_command) {
	
	case hardware_device::motor_activation::MOTOR_NOT_ACTIVATED:
		
		break;
	case hardware_device::motor_activation::MOTOR_X:
		if (!device.power_switch_stat) {
			motor_activation_completed(false);
			break;
		}
		if (request_abort_command) {
			motor_activation_completed(false);
			break;
		}
		if(device.crash_event) {
			motor_activation_completed(false);
			break;
		}

		if ( ((device.Xposition - device.Xtarget) < 20) && ((device.Xposition - device.Xtarget) > -20)) {
			// Target position 
			device.Xposition = device.Xtarget;
			motor_activation_completed(true);
		}
		else if (device.Xposition > device.Xtarget) device.Xposition -= 10;
		else if (device.Xposition < device.Xtarget) device.Xposition += 10;

		break;
	case hardware_device::motor_activation::MOTOR_Y:
		if (!device.power_switch_stat) {
			motor_activation_completed(false);
			break;
		}
		if (request_abort_command) {
			motor_activation_completed(false);
			break;
		}
		if (device.crash_event) {
			motor_activation_completed(false);
			break;
		}

		if (((device.Yposition - device.Ytarget) < 20) && ((device.Yposition - device.Ytarget) > -20)) {
			// Target position 
			device.Yposition = device.Ytarget;
			motor_activation_completed(true);
		}
		else if (device.Yposition > device.Ytarget) device.Yposition -= 10;
		else if (device.Yposition < device.Ytarget) device.Yposition += 10;

		break;
	case hardware_device::motor_activation::MOTOR_Z:
		if (!device.power_switch_stat) {
			motor_activation_completed(false);
			break;
		}
		if (request_abort_command) {
			motor_activation_completed(false);
			break;
		}
		if (device.crash_event) {
			motor_activation_completed(false);
			break;
		}

		if (((device.Zposition - device.Ztarget) < 20) && ((device.Zposition - device.Ztarget) > -20)) {
			// Target position 
			device.Zposition = device.Ztarget;
			motor_activation_completed(true);
		}
		else if (device.Zposition > device.Ztarget) device.Zposition -= 10;
		else if (device.Zposition < device.Ztarget) device.Zposition += 10;
		break;

	}

	if (request_abort_command) {
		request_abort_command = false;
		commandCompleted(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_ABORT);
	}

	// Updates the Status registers of the device
    updateStatusRegisters();

}

void PCB325::device_reset_callback(void) {
	device.init();
	inputs.init();
	outputs.init();
	updateStatusRegisters();
}

PCB325::commandResult^ PCB325::device_command_callback(unsigned char cmd, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3) {
	unsigned short pos;

	//IMPORTANT!!!! Before to return it is necessary to update the status registers in the case they should be modified.

	switch ((ProtocolStructure::Commands::command_index)cmd) {
	
	case ProtocolStructure::Commands::command_index::CMD_ABORT:
		
		request_abort_command = true;
		return gcnew commandResult(); // Processing		
		break;

	case ProtocolStructure::Commands::command_index::CMD_CALIB_MODE:
		request_motor_working_mode = ProtocolStructure::StatusRegister::motor_mode::MOTOR_CALIBRATION_MODE;
		return gcnew commandResult(); // Processing	
		break;

	case ProtocolStructure::Commands::command_index::CMD_COMMAND_MODE:
		request_motor_working_mode = ProtocolStructure::StatusRegister::motor_mode::MOTOR_COMMAND_MODE;
		return gcnew commandResult(); // Processing	
		break;

	case ProtocolStructure::Commands::command_index::CMD_SERVICE_MODE:
		request_motor_working_mode = ProtocolStructure::StatusRegister::motor_mode::MOTOR_SERVICE_MODE;
		return gcnew commandResult(); // Processing	
		break;

	case ProtocolStructure::Commands::command_index::CMD_DISABLE_MODE:
		request_motor_working_mode = ProtocolStructure::StatusRegister::motor_mode::MOTOR_DISABLE_MODE;
		return gcnew commandResult(); // Processing	
		break;

	case ProtocolStructure::Commands::command_index::CMD_MOVE_X:
		pos = d0 + 256 * d1;
		
		if (pos == device.Xposition) 
			return gcnew commandResult(d0, d1); // Already in position
		
		if(pos > 2580) 
			return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM);

		if (device.motor_working_mode != ProtocolStructure::StatusRegister::motor_mode::MOTOR_COMMAND_MODE)
			return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED);

		if (!device.power_switch_stat)
			return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED);

		if(device.motor_command != hardware_device::motor_activation::MOTOR_NOT_ACTIVATED)
			return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_BUSY);

		device.Xtarget = pos;
		device.motor_command = hardware_device::motor_activation::MOTOR_X;
		return gcnew commandResult(); // Processing

		break;
	case ProtocolStructure::Commands::command_index::CMD_MOVE_Y:

		pos = d0 + 256 * d1;

		if (pos == device.Yposition)
			return gcnew commandResult(d0, d1); // Already in position

		if (pos > 700)
			return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM);

		if (device.motor_working_mode != ProtocolStructure::StatusRegister::motor_mode::MOTOR_COMMAND_MODE)
			return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED);

		if (!device.power_switch_stat)
			return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED);

		if (device.motor_command != hardware_device::motor_activation::MOTOR_NOT_ACTIVATED)
			return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_BUSY);

		device.Ytarget = pos;
		device.motor_command = hardware_device::motor_activation::MOTOR_Y;
		return gcnew commandResult(); // Processing
		break;

	case ProtocolStructure::Commands::command_index::CMD_MOVE_Z:
		pos = d0 + 256 * d1;

		if (pos == device.Zposition)
			return gcnew commandResult(d0, d1); // Already in position

		if (pos > 1350)
			return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM);

		if (device.motor_working_mode != ProtocolStructure::StatusRegister::motor_mode::MOTOR_COMMAND_MODE)
			return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED);

		if (!device.power_switch_stat)
			return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED);

		if (device.motor_command != hardware_device::motor_activation::MOTOR_NOT_ACTIVATED)
			return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_BUSY);

		device.Ztarget = pos;
		device.motor_command = hardware_device::motor_activation::MOTOR_Z;
		return gcnew commandResult(); // Processing
		break;

	case ProtocolStructure::Commands::command_index::CMD_ENABLE_KEYSTEP:
		if (device.motor_working_mode == ProtocolStructure::StatusRegister::motor_mode::MOTOR_COMMAND_MODE) {
			if (d0) device.keystep_mode = true;
			else device.keystep_mode = false;
		}else device.keystep_mode = false;
		
		return gcnew commandResult(0,0); // Immediate
		break;



	default:
		return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED);
	}
	
	return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED);

}

/*
switch(motorMoveX((int) d0 + (int) d1 * 256)){

				

				

				

				/// \addtogroup CANPROT
				/// \test Busy : ImmediateError(\ref MET_CAN_COMMAND_BUSY)
				case MOTOR_ERROR_BUSY:
					MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_BUSY);
					break;

				/// \addtogroup CANPROT
				/// \test invalid-return_code (software bug): ImmediateError(\ref MET_CAN_COMMAND_WRONG_RETURN_CODE)
				default:
					MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_WRONG_RETURN_CODE);
			}
			break;

*/