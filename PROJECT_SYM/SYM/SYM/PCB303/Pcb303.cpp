#include "Pcb303.h"
void PCB303::device_workflow_callback(void) {

	// Decode the DATA registers ..
	
	// Decode Parameter register
	
	// Decode the Front Back Registers
	for (int i = 0; i < NUM_COLLIMATION_SLOTS; i++) {
		protocol.parameter_register.decodeFBCollimationSlotRegister(parameter_registers[i + (System::Byte) ProtocolStructure::ParameterRegister::register_index::FB_FORMAT_SLOT_IDX], i);
		protocol.parameter_register.decodeLRCollimationSlotRegister(parameter_registers[i + (System::Byte)ProtocolStructure::ParameterRegister::register_index::LR_FORMAT_SLOT_IDX], i);
	}
		
	// Trap parameters
	for (int i = 0; i < NUM_COLLIMATION_SLOTS/2; i++) {
		protocol.parameter_register.decodeLRCollimationSlotRegister(parameter_registers[i + (System::Byte)ProtocolStructure::ParameterRegister::register_index::TR_FORMAT_SLOT_IDX], i);
	}

	// Filter parameters
	for (int i = 0; i < 3; i++) {
		protocol.parameter_register.decodeFilterSlotRegister(parameter_registers[i + (System::Byte)ProtocolStructure::ParameterRegister::register_index::FILTER_SLOT_IDX], i);
	}

	// Mirror parameters
	protocol.parameter_register.decodeMirroRegister(parameter_registers[(System::Byte)ProtocolStructure::ParameterRegister::register_index::MIRROR_SLOT_IDX]);
	

	// Manage the Power Light: ligth on for a timeout time
	if (device.power_light_timer) {
		device.power_light_timer--;
		if (!device.power_light_timer) device.power_light = ProtocolStructure::StatusRegister::light_target_code::LIGHT_OFF;
	}

	// Manage Blades positioning
	if (device.format_action_command == ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING) {
		device.format_data.back = protocol.parameter_register.format_collimation[device.format_index]->back;
		device.format_data.front = protocol.parameter_register.format_collimation[device.format_index]->front;
		device.format_data.left = protocol.parameter_register.format_collimation[device.format_index]->left;
		device.format_data.right = protocol.parameter_register.format_collimation[device.format_index]->right;
		device.format_data.trap = protocol.parameter_register.format_collimation[device.format_index]->trap;
		device.format_action_command = ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED;
	}

	// Manage Filter positioning
	if (device.filter_action_command == ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING) {
		device.filter_position = protocol.parameter_register.filter_slots[device.filter_index];
		device.filter_action_command = ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED;
	}

	// Manage Mirror positioning
	if (device.mirror_action_command == ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING) {
		if (device.mirror_index == ProtocolStructure::StatusRegister::mirror_target_code::IN_FIELD) {
			device.mirror_position = protocol.parameter_register.mirror_slot;
		}
		else {
			device.mirror_position = 0;
		}
		device.mirror_action_command = ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED;
	}



	protocol.status_register.collimation_target_index = device.format_index;
	protocol.status_register.collimation_action_status = device.format_action_command;

	protocol.status_register.filter_target_index = device.filter_index;
	protocol.status_register.filter_action_status = device.filter_action_command;
	
	protocol.status_register.mirror_target_index = device.mirror_index;
	protocol.status_register.mirror_action_status = device.mirror_action_command;
	
	protocol.status_register.light_status = device.power_light;

	protocol.status_register.bulb_temp = device.temp_bulb;
	protocol.status_register.stator_temp = device.temp_stator;

	// Encode the Status registers
	status_registers[(Byte)ProtocolStructure::StatusRegister::register_index::SYSTEM_REGISTER] = protocol.status_register.encodeSystemRegister();
	status_registers[(Byte)ProtocolStructure::StatusRegister::register_index::TUBE_REGISTER] = protocol.status_register.encodeTubeRegister();


}


void PCB303::device_reset_callback(void) {
	device.init();
	inputs.init();
	outputs.init();
}

PCB303::commandResult^ PCB303::device_command_callback(unsigned char cmd, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3) {
	
	
	switch ((ProtocolStructure::Commands::command_index) cmd) {
	case ProtocolStructure::Commands::command_index::ABORT_COMMAND:		
		break;

	case ProtocolStructure::Commands::command_index::SET_FILTER:
		if(d0 > 5) return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM);
		device.filter_index = d0;	

		if(device.filter_position == protocol.parameter_register.filter_slots[d0]) device.filter_action_command = ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED;
		else device.filter_action_command = ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING;
		
		protocol.status_register.filter_target_index = device.filter_index;
		protocol.status_register.filter_action_status = device.filter_action_command;
		break;

	case ProtocolStructure::Commands::command_index::SET_FORMAT:
		if (d0 >= NUM_COLLIMATION_SLOTS) return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM);
		device.format_index = d0;
		device.format_action_command = ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING;

		protocol.status_register.collimation_target_index = device.format_index;
		protocol.status_register.collimation_action_status = device.format_action_command;
		break;

	case ProtocolStructure::Commands::command_index::SET_LIGHT:
		if ((ProtocolStructure::StatusRegister::light_target_code)d0 == ProtocolStructure::StatusRegister::light_target_code::LIGHT_ON) {
			device.power_light = ProtocolStructure::StatusRegister::light_target_code::LIGHT_ON;
			device.power_light_timer = 200;
		}
		else {
			device.power_light = ProtocolStructure::StatusRegister::light_target_code::LIGHT_OFF;
			device.power_light_timer = 0;
		}		

		protocol.status_register.light_status = device.power_light;
		break;

	case ProtocolStructure::Commands::command_index::SET_MIRROR:


		if (d0 == (System::Byte)ProtocolStructure::StatusRegister::mirror_target_code::IN_FIELD) {

			device.mirror_index = ProtocolStructure::StatusRegister::mirror_target_code::IN_FIELD;
			if (device.mirror_position != protocol.parameter_register.mirror_slot)	device.mirror_action_command = ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING;
			else device.mirror_action_command = ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED;

			device.power_light = ProtocolStructure::StatusRegister::light_target_code::LIGHT_ON;
			device.power_light_timer = 200; 

		}else if (d0 == (System::Byte)ProtocolStructure::StatusRegister::mirror_target_code::OUT_FIELD) {
			device.mirror_index = ProtocolStructure::StatusRegister::mirror_target_code::OUT_FIELD;
			if (device.mirror_position != 0)	device.mirror_action_command = ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING;
			else device.mirror_action_command = ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED;

			device.power_light = ProtocolStructure::StatusRegister::light_target_code::LIGHT_OFF;
			device.power_light_timer = 0;

		}else return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM);

		protocol.status_register.mirror_target_index = device.mirror_index;
		protocol.status_register.mirror_action_status = device.mirror_action_command;
		protocol.status_register.light_status = device.power_light;
		break;

	default:
		return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED);
	}

	commandResult^ result = gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_NO_ERROR);
	return result;
}