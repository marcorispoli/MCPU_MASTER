#include "Pcb302.h"


void PCB302::device_workflow_callback(void) {
	static int paddle_down_sequence = 0;
	static bool paddle_down_status = false;
	static unsigned char holder_speed = 0;

	// Decode the DATA registers ..
	protocol.data_register.decodeHolderLimitsRegister(data_registers[(Byte)ProtocolStructure::DataRegister::register_index::HOLDER_LIMITS]);
	protocol.data_register.decodeCompressorLimitsRegister(data_registers[(Byte)ProtocolStructure::DataRegister::register_index::COMPRESSOR_LIMITS]);
	protocol.data_register.decodePaddleWeightRegister(data_registers[(Byte)ProtocolStructure::DataRegister::register_index::PADDLE_WEIGHT]);
	
	// Decode Parameter register: the calibration is not used here	
	protocol.parameter_register.decodeHolderCalibRegister(parameter_registers[(Byte)ProtocolStructure::ParameterRegister::register_index::HOLDER_CALIB]);


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
		
		if (!paddle_down_status) {
			paddle_down_status = true;
			if (!protocol.status_register.force_target) paddle_down_sequence = 0;
			else paddle_down_sequence = 1;
		}

		// Possible compression Targets detected 
		if (((paddle_down_sequence == 0) && (protocol.status_register.force_target)) || ((paddle_down_sequence == 1) && (protocol.status_register.limit_compression))) {
			protocol.status_register.downward_direction = false;
			protocol.status_register.idle_status = true;
		}
		else {
			if (device.paddle_holder_position <= protocol.data_register.min_position) {
				protocol.status_register.idle_status = true;
				protocol.status_register.downward_direction = false;
			}
			else {				
				if (device.current_force) {
					if (!holder_speed) device.paddle_holder_position--;
					holder_speed ++;
					if (holder_speed > 7) holder_speed = 0;
				}
				else {
					device.paddle_holder_position--;
					holder_speed = 0;
				}
				
				protocol.status_register.downward_direction = true;
				protocol.status_register.idle_status = false;
			}
		}


	}
	else {
		protocol.status_register.downward_direction = false;
		protocol.status_register.upward_direction = false;
		protocol.status_register.idle_status = true;
		paddle_down_status = false;
	}
	
	// Force calculation based on the protocol.data_register.paddle_distance_from_plane
	int paddle_distance = device.paddle_holder_position - protocol.data_register.paddle_offset - protocol.data_register.magnifier_offset;
	if (paddle_distance >= device.breast_thickness) device.current_force = 0;
	else {
		device.current_force = (device.breast_thickness - paddle_distance) * (200 / 20);

		if (device.current_force > protocol.data_register.limit_compression) {
			paddle_distance = device.breast_thickness - (protocol.data_register.limit_compression * 20 / 200);
			device.paddle_holder_position = paddle_distance + protocol.data_register.paddle_offset + protocol.data_register.magnifier_offset;
			if (device.paddle_holder_position > protocol.data_register.max_position) device.paddle_holder_position = protocol.data_register.max_position;
		}
	}

	// Sets the targets
	if (device.current_force >= protocol.data_register.target_compression) protocol.status_register.force_target = true;
	if (device.current_force >= protocol.data_register.limit_compression) protocol.status_register.limit_compression = true;

	// Resets the compression sequence 
	if ((device.current_force == 0) && (protocol.status_register.idle_status)) {
		paddle_down_sequence = 0;
		paddle_down_status = false;
		protocol.status_register.force_target = false;
		protocol.status_register.limit_compression = false;
	}
		
	
	// Set the external compression detected signal
	if (device.current_force) outputs.compression_detected = true;
	else outputs.compression_detected = false;
	
	protocol.status_register.compression_on = outputs.compression_detected;

	// Paddle register
	protocol.status_register.paddle_position = device.paddle_holder_position;
	protocol.status_register.paddle_force = device.current_force;
	protocol.status_register.paddle_tag = (unsigned char) device.current_paddle_tag;

	// Raw values are not implemented here
	protocol.status_register.paddle_raw_position = device.paddle_holder_position;
	protocol.status_register.paddle_raw_force = device.current_force;
	protocol.status_register.paddle_raw_code = (unsigned char)device.current_paddle_tag;

	// Sets the detected component code
	protocol.status_register.component = (ProtocolStructure::ComponentCode) device.current_component_code;

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