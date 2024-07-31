#include "Pcb302.h"


void PCB302::device_workflow_callback(void) {
	static int paddle_down_sequence = 0;
	static bool paddle_down_status = false;

	// Decode the DATA registers ..
	protocol.data_register.decodeHolderLimitsRegister(data_registers[(Byte)ProtocolStructure::DataRegister::register_index::HOLDER_LIMITS]);
	protocol.data_register.decodeCompressorLimitsRegister(data_registers[(Byte)ProtocolStructure::DataRegister::register_index::COMPRESSOR_LIMITS]);
	protocol.data_register.decodePaddleWeightRegister(data_registers[(Byte)ProtocolStructure::DataRegister::register_index::PADDLE_WEIGHT]);
	
	/*
	*		
			static unsigned char  absolute_arm_angle = 0;
			static unsigned char  paddle_weight = 0;

			static unsigned char limit_compression = 200;
			static unsigned char target_compression = 150;

			static unsigned short max_position = 0;
			static unsigned short min_position = 0;
	*/

	// Feedback compression ena
	protocol.status_register.compression_ena = inputs.compression_ena;

	// Driver Compression Enable 
	if (!inputs.compression_ena) {
		// No activation is permitted
		protocol.status_register.downward_direction = false;
		protocol.status_register.upward_direction = false;
		protocol.status_register.idle_status = true;
	}
	else if (inputs.compression_up && inputs.compression_down) {
		// Unlock  
		protocol.status_register.downward_direction = false;
		if (device.current_force) {			
			device.paddle_holder_position++;
			if (device.paddle_holder_position > protocol.data_register.max_position) {
				device.paddle_holder_position = protocol.data_register.max_position;
				protocol.status_register.upward_direction = false;
				protocol.status_register.idle_status = true;
			}
			else {
				protocol.status_register.upward_direction = true;
				protocol.status_register.idle_status = false;
			}
		}
		else {
			protocol.status_register.upward_direction = false;
			protocol.status_register.idle_status = true;
		}

	}
	else if (inputs.compression_up || inputs.paddle_up) {
		// Upward direction  
		device.paddle_holder_position++;
		protocol.status_register.downward_direction = false;

		if (device.paddle_holder_position > protocol.data_register.max_position) {
			protocol.status_register.upward_direction = false;
			protocol.status_register.idle_status = true;
			device.paddle_holder_position = protocol.data_register.max_position;
		}
		else {
			protocol.status_register.upward_direction = true;
			protocol.status_register.idle_status = false;
		}
		
	}
	else if ((inputs.compression_down) || (inputs.paddle_down)) {
		protocol.status_register.upward_direction = false;
		

		// Select what is the current sequence
		if (paddle_down_sequence == 0) {
			if (device.current_force >= protocol.data_register.target_compression) {
				// Target compression check
				device.current_force = protocol.data_register.target_compression;
				protocol.status_register.downward_direction = false;
				protocol.status_register.idle_status = true;
				protocol.status_register.force_target = true;
			}
			else {
				protocol.status_register.downward_direction = true;
				device.current_force += 5;
				if (device.current_force > protocol.data_register.target_compression) {
					device.current_force = protocol.data_register.target_compression;
					protocol.status_register.force_target = true;
					protocol.status_register.idle_status = true;
				}
			}
		}
		else {
			if (device.current_force >= protocol.data_register.limit_compression) {
				// Limit compression check
				device.current_force = protocol.data_register.limit_compression;
				protocol.status_register.downward_direction = false;
				protocol.status_register.limit_compression = true;
				protocol.status_register.idle_status = true;
			}
			else {
				protocol.status_register.downward_direction = true;
				protocol.status_register.idle_status = false;
				device.current_force += 5;
				if (device.current_force > protocol.data_register.limit_compression) {
					device.current_force = protocol.data_register.limit_compression;
					protocol.status_register.limit_compression = true;
				}
			}
		}


	}
	else {
		protocol.status_register.downward_direction = false;
		protocol.status_register.upward_direction = false;
		protocol.status_register.idle_status = true;
	}
	
	// Force calculation based on the protocol.data_register.paddle_distance_from_plane
	if (protocol.data_register.paddle_distance_from_plane >= device.breast_thickness) device.current_force = 0;
	else {
		device.current_force = (device.breast_thickness - protocol.data_register.paddle_distance_from_plane) * (200 / 10);
	}

	// Resets the compression sequence 
	if ((device.current_force == 0) && (protocol.status_register.idle_status)) {
		paddle_down_sequence = 0;
		paddle_down_status = false;
		protocol.status_register.force_target = false;
		protocol.status_register.limit_compression = true;
	}
		
	
	// Set the external compression detected signal
	if (device.current_force) outputs.compression_detected = true;
	else outputs.compression_detected = false;
	
	protocol.status_register.compression_on = outputs.compression_detected;

	// Paddle register
	protocol.status_register.paddle_position = device.paddle_holder_position;
	protocol.status_register.paddle_force = device.current_force;
	protocol.status_register.paddle_tag = (unsigned char) device.current_paddle_code;

	// Raw values are not implemented here
	protocol.status_register.paddle_raw_position = device.paddle_holder_position;
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