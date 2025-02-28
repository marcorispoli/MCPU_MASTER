#include "Pcb325.h"

/// <summary>
/// Updates all the Status registers when required
/// </summary>
/// <param name=""></param>
void PCB325::updateStatusRegisters(void) {

	// Encode the Status registers 
	status_registers[(Byte)ProtocolStructure::StatusRegister::register_index::SYSTEM_REGISTER] = protocol.status_register.encodeSystemRegister();
	status_registers[(Byte)ProtocolStructure::StatusRegister::register_index::BATTERY_REGISTER] = protocol.status_register.encodeBatteryRegister();
}


void PCB325::device_workflow_callback(void) {
	/*
	// Decode the DATA registers ..
	protocol.data_register.decodeOutputRegister(data_registers[(Byte)ProtocolStructure::DataRegister::register_index::OUTPUTS]);
		
	if (protocol.data_register.power_lock) device.powerlock_stat = true;

	// Keep alive not implemented
	
	
	// Sets the current Motor Power Supply status
	if (
		(protocol.data_register.motor_power_supply_ena) &&
		(!inputs.emergency) &&
		(!inputs.cabinet_safety) &&
		(!inputs.powerdown)
		) outputs.power_48VDC_stat = true;
	else outputs.power_48VDC_stat = false;

	// Sets the current Power Switch status
	if (
		(protocol.data_register.motor_power_switch_ena) &&
		(!inputs.cabinet_safety) &&
		(!inputs.compression_detected)
		) outputs.power_48SW_stat = true;
	else outputs.power_48SW_stat = false;

	// Rotation Led
	if (protocol.data_register.button_rotation_led) outputs.rotation_led = true;
	else outputs.rotation_led = false;

	// Buzzer
	if (protocol.data_register.manual_buzzer_mode) {
		device.buzzer_stat = protocol.data_register.manual_buzzer_status;
	}

	// Feedback of the power supply status
	protocol.status_register.motor_48V_ok = outputs.power_48VDC_stat;
	
	// Feedback of the power switch status
	protocol.status_register.motor_safety_switch = outputs.power_48SW_stat;

	// Evaluates the Battery voltage for the alarmn status
	protocol.status_register.batt1_low_alarm = (device.vbatt1 < 10);
	protocol.status_register.batt2_low_alarm = (device.vbatt2 < 10);

	// X-RAY button request
	protocol.status_register.xray_push_button = ((inputs.xray_button_input) || (protocol.data_register.burning_activation && inputs.burning_jumper));

	// Generator X-RAY enable output
	if (
		(protocol.status_register.xray_push_button) &&
		(inputs.closed_door) &&
		(protocol.data_register.xray_enable)
		) outputs.generator_xray_ena = true;
	else outputs.generator_xray_ena = false;

	// Compression on 
	protocol.status_register.compression_on_status = inputs.compression_detected;

	// Compression ena
	if (protocol.data_register.compression_enable) outputs.compression_ena = true;
	else outputs.compression_ena = false;

	// Calibration ena
	if (protocol.data_register.compression_calibration) outputs.calibration_ena = true;
	else outputs.calibration_ena = false;

	// Pedalboard Compression Up
	outputs.pedalboard_compression_up = inputs.pedalboard_cmp_up;
	outputs.pedalboard_compression_down = inputs.pedalboard_cmp_down;

	// Handle The button power on
	static int power_off_count = 0;	
	if (inputs.power_off_req) {
		power_off_count++;
		if (power_off_count > 30) System::Windows::Forms::Application::Exit();
	}
	else power_off_count = 0;

	protocol.status_register.system_error = false;
	protocol.status_register.system_emergency = inputs.emergency;
	protocol.status_register.system_power_down = inputs.powerdown;
	protocol.status_register.cabinet_safety_alarm = inputs.cabinet_safety;
	protocol.status_register.soft_power_off_request = inputs.power_off_req;
	protocol.status_register.battery_enabled = inputs.battery_enable;
	protocol.status_register.button_up_stat = inputs.keypad_up;
	protocol.status_register.button_down_stat = inputs.keypad_down;
	protocol.status_register.button_slide_up_stat = inputs.manual_slide_up;
	protocol.status_register.button_slide_down_stat = inputs.manual_slide_down;
	protocol.status_register.button_cw_stat = inputs.keypad_cw;
	protocol.status_register.button_ccw_stat = inputs.keypad_ccw;
	protocol.status_register.button_body_cw = inputs.manual_body_cw;
	protocol.status_register.button_body_ccw = inputs.manual_body_ccw;
	protocol.status_register.closed_door = inputs.closed_door;
	protocol.status_register.burning_jumper_present = inputs.burning_jumper;
	protocol.status_register.power_lock_status = device.powerlock_stat;
	protocol.status_register.pedal_up_stat = inputs.pedalboard_up;
	protocol.status_register.pedal_down_stat = inputs.pedalboard_down;
	protocol.status_register.pedal_cmp_up_stat = inputs.pedalboard_cmp_up;
	protocol.status_register.pedal_cmp_down_stat = inputs.pedalboard_cmp_down;

	protocol.status_register.voltage_batt1 = device.vbatt1;
	protocol.status_register.voltage_batt2 = device.vbatt2;
	*/

	// Encode the Status registers SYSTEM_REGISTER = 0,	//!> This is the System Status register index
	updateStatusRegisters();

}

void PCB325::device_reset_callback(void) {
	device.init();
	inputs.init();
	outputs.init();
	updateStatusRegisters();
}

PCB325::commandResult^ PCB325::device_command_callback(unsigned char cmd, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3) {

	//IMPORTANT!!!! Before to return it is necessary to update the status registers in the case they should be modified.

	/*
	switch ((ProtocolStructure::Commands::command_index)cmd) {
	case ProtocolStructure::Commands::command_index::ABORT_COMMAND:
		break;

	case ProtocolStructure::Commands::command_index::SET_FILTER:
		if (d0 > 5) return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM);
		device.filter_index = d0;

		if (device.filter_position == protocol.parameter_register.filter_slots[d0]) device.filter_action_command = ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED;
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

		}
		else if (d0 == (System::Byte)ProtocolStructure::StatusRegister::mirror_target_code::OUT_FIELD) {
			device.mirror_index = ProtocolStructure::StatusRegister::mirror_target_code::OUT_FIELD;
			if (device.mirror_position != 0)	device.mirror_action_command = ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING;
			else device.mirror_action_command = ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED;

			device.power_light = ProtocolStructure::StatusRegister::light_target_code::LIGHT_OFF;
			device.power_light_timer = 0;

		}
		else return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM);

		protocol.status_register.mirror_target_index = device.mirror_index;
		protocol.status_register.mirror_action_status = device.mirror_action_command;
		protocol.status_register.light_status = device.power_light;
		break;

	default:
		return gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED);
	}
	*/

	updateStatusRegisters();
	commandResult^ result = gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_NO_ERROR);
	return result;
}