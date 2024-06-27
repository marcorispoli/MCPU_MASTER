#include "Pcb302.h"


void PCB302::device_workflow_callback(void) {

	// Decode the DATA registers ..
	protocol.data_register.decodeOptionsRegister(data_registers[(Byte)ProtocolStructure::DataRegister::register_index::OPTIONS_REGISTER]);
	protocol.data_register.decodePositionLimitRegister(data_registers[(Byte)ProtocolStructure::DataRegister::register_index::POSITION_LIMIT_REGISTER]);

	// Feedback compression ena
	protocol.status_register.compression_ena = inputs.compression_ena;

	// Handle the compression event
	if ((inputs.compression_ena) && (device.current_paddle_code != paddleCodes::PADDLE_NOT_DETECTED)) {

		// Unlock event
		if (inputs.compression_up && inputs.compression_down) {
			device.current_force = 0;

			protocol.status_register.downward_direction = false;
			protocol.status_register.upward_direction = false;
			protocol.status_register.unlock_activation = true;
			protocol.status_register.idle_status = false;
		}
		else {
			if ((inputs.compression_up) || (inputs.paddle_up)) {
				protocol.status_register.downward_direction = false;
				protocol.status_register.idle_status = false;

				if (device.current_force == 0) {
					protocol.status_register.upward_direction = false;
				}
				else {
					protocol.status_register.upward_direction = true;
					device.current_force -= 5;
					if (device.current_force < 0) device.current_force = 0;
				}
			}else if ((inputs.compression_down) || (inputs.paddle_down)) {
				protocol.status_register.upward_direction = false;
				protocol.status_register.idle_status = false;

				if (device.current_force >= device.target_force) {	
					protocol.status_register.downward_direction = false;
				}
				else {
					protocol.status_register.downward_direction = true;					
					device.current_force += 5;
					if (device.current_force > device.target_force) device.current_force = device.target_force;
				}
				
			}
			else {
				protocol.status_register.downward_direction = false;
				protocol.status_register.upward_direction = false;
				protocol.status_register.unlock_activation = true;
				protocol.status_register.idle_status = true;
			}
		}
	}
	else {
		protocol.status_register.downward_direction = false;
		protocol.status_register.upward_direction = false;
		protocol.status_register.unlock_activation = true;
		protocol.status_register.idle_status = true;
	}

	// In case the paddle should not been detected the force and the thickness is reset
	if ((device.current_paddle_code == paddleCodes::PADDLE_NOT_DETECTED)) {
		device.current_force = 0;
		device.current_thickness = 0;
	}

	// Set the external compression detected signal
	if (device.current_force) outputs.compression_detected = true;
	else outputs.compression_detected = false;
	if (device.current_force > 255) device.current_force = 255;

	protocol.status_register.compression_on = outputs.compression_detected;

	// Set the actual compression thicknes
	device.current_thickness = (int)((float)device.target_thickness * ((float)device.current_force / (float)device.target_force));
	if (device.current_thickness > 255) device.current_thickness = 255;
	
	protocol.status_register.paddle_position = device.current_thickness;
	protocol.status_register.paddle_force = device.current_force;
	protocol.status_register.paddle_code = (unsigned char) device.current_paddle_code;

	// Raw values are not implemented here
	protocol.status_register.paddle_raw_position = device.current_thickness;
	protocol.status_register.paddle_raw_force = device.current_force;
	protocol.status_register.paddle_raw_code = (unsigned char)device.current_paddle_code;


	// Encode the Status registers
	status_registers[(Byte)ProtocolStructure::StatusRegister::register_index::SYSTEM_REGISTER] = protocol.status_register.encodeSystemRegister();
	status_registers[(Byte)ProtocolStructure::StatusRegister::register_index::PADDLE_REGISTER] = protocol.status_register.encodePaddleRegister();
	status_registers[(Byte)ProtocolStructure::StatusRegister::register_index::RAW_PADDLE_REGISTER] = protocol.status_register.encodeRawPaddleRegister();
}

void PCB302::device_reset_callback(void) {
	device.init();
	inputs.init();
	outputs.init();
}