#include "Pcb301.h"

#define PCB301_SET_SYSTEM_ERROR_STATUS(stat) status_registers[0] = setRegBit(status_registers[0], 0x1, (stat))
#define PCB301_SET_SYSTEM_EMERGENCY_STATUS(stat) status_registers[0] = setRegBit(status_registers[0], 0x2, (stat))
#define PCB301_SET_SYSTEM_POWERDOWN(stat) status_registers[0] = setRegBit(status_registers[0], 0x4, (stat))
#define PCB301_SET_SYSTEM_CABINET_SAFETY(stat) status_registers[0] = setRegBit(status_registers[0], 0x8, (stat))
#define PCB301_SET_SYSTEM_POWER_OFF_REQ(stat) status_registers[0] = setRegBit(status_registers[0], 0x10, (stat))
#define PCB301_SET_SYSTEM_BATT1LOW(stat) status_registers[0] = setRegBit(status_registers[0], 0x20, (stat))
#define PCB301_SET_SYSTEM_BATT2LOW(stat) status_registers[0] = setRegBit(status_registers[0], 0x40, (stat))
#define PCB301_SET_SYSTEM_BATTENA(stat) status_registers[0] = setRegBit(status_registers[0], 0x80, (stat))

#define PCB301_SET_BUTTON_VERTICAL_UP(stat) status_registers[0] = setRegBit(status_registers[0], 0x100, (stat))
#define PCB301_SET_BUTTON_VERTICAL_DOWN(stat) status_registers[0] = setRegBit(status_registers[0], 0x200, (stat))
#define PCB301_SET_BUTTON_SLIDE_UP(stat) status_registers[0] = setRegBit(status_registers[0], 0x400, (stat))
#define PCB301_SET_BUTTON_SLIDE_DOWN(stat) status_registers[0] = setRegBit(status_registers[0], 0x800, (stat))
#define PCB301_SET_BUTTON_ARM_CW(stat) status_registers[0] = setRegBit(status_registers[0], 0x1000, (stat))
#define PCB301_SET_BUTTON_ARM_CCW(stat) status_registers[0] = setRegBit(status_registers[0], 0x2000, (stat))
#define PCB301_SET_BUTTON_BODY_CW(stat) status_registers[0] = setRegBit(status_registers[0], 0x4000, (stat))
#define PCB301_SET_BUTTON_BODY_CCW(stat) status_registers[0] = setRegBit(status_registers[0], 0x8000, (stat))

#define PCB301_SET_SYSTEM_MOTOR_OK(stat) status_registers[0] = setRegBit(status_registers[0], 0x10000, stat)
#define PCB301_SET_SYSTEM_MOTOR_SWITCH(stat) status_registers[0] = setRegBit(status_registers[0], 0x20000, (stat))
#define PCB301_SET_SYSTEM_COMPRESSION(stat) status_registers[0] = setRegBit(status_registers[0], 0x40000, (stat))
#define PCB301_SET_XRAY_PUSH_BUTTON(stat) status_registers[0] = setRegBit(status_registers[0], 0x80000, (stat))
#define PCB301_SET_SYSTEM_CLOSEDOOR(stat) status_registers[0] = setRegBit(status_registers[0], 0x100000, (stat))
#define PCB301_SET_SYSTEM_BURNING_JMP(stat) status_registers[0] = setRegBit(status_registers[0], 0x200000, (stat))
#define PCB301_SET_SYSTEM_POWER_LOCK(stat) status_registers[0] = setRegBit(status_registers[0], 0x400000, (stat))

#define PCB301_SET_PEDAL_VERTICAL_UP(stat) status_registers[0] = setRegBit(status_registers[0], 0x1000000, (stat))
#define PCB301_SET_PEDAL_VERTICAL_DOWN(stat) status_registers[0] = setRegBit(status_registers[0], 0x2000000, (stat))

#define PCB301_SET_BATTERY_VBATT(x,y) status_registers[1] = setRegVal(x,y,0,0)


#define PCB301_GET_OUTPUTS_DATA_POWER_LOCK ((getRegD0(data_registers[0]) & 0x1) ? true: false)
#define PCB301_GET_OUTPUTS_DATA_MOTOR_POWER_SUPPLY_ENABLE ((getRegD0(data_registers[0]) & 0x2) ? true: false)
#define PCB301_GET_OUTPUTS_DATA_MOTOR_SWITCH_ENABLE ((getRegD0(data_registers[0]) & 0x4) ? true: false)
#define PCB301_GET_OUTPUTS_DATA_COMPRESSOR_ENABLE ((getRegD0(data_registers[0]) & 0x8) ? true: false)
#define PCB301_GET_OUTPUTS_DATA_COMPRESSOR_CALIBRATION ((getRegD0(data_registers[0]) & 0x10) ? true: false)
#define PCB301_GET_OUTPUTS_DATA_XRAY_ENA ((getRegD0(data_registers[0]) & 0x20) ? true: false)

#define  PCB301_GET_OUTPUTS_DATA_BURNING_STAT ((getRegD1(data_registers[0]) & 0x1) ? true: false)
#define  PCB301_GET_OUTPUTS_DATA_BUZZER_STAT ((getRegD1(data_registers[0]) & 0x2) ? true: false)
#define  PCB301_GET_OUTPUTS_DATA_MANUAL_BUZZER ((getRegD1(data_registers[0]) & 0x4) ? true: false)
#define  PCB301_GET_OUTPUTS_DATA_XRAY_LED ((getRegD1(data_registers[0]) & 0x8) ? true: false)
#define  PCB301_GET_OUTPUTS_DATA_XRAY_LAMP1 ((getRegD1(data_registers[0]) & 0x10) ? true: false)
#define  PCB301_GET_OUTPUTS_DATA_XRAY_LAMP2 ((getRegD1(data_registers[0]) & 0x20) ? true: false)
#define  PCB301_GET_OUTPUTS_DATA_MAN_ROT_LED ((getRegD1(data_registers[0]) & 0x40) ? true: false)

#define  PCB301_GET_OUTPUTS_DATA_POWER_OFF ((getRegD3(data_registers[0]) & 0x40) ? true: false)
#define  PCB301_GET_OUTPUTS_DATA_KEEP_ALIVE ((getRegD3(data_registers[0]) & 0x80) ? true: false)


void PCB301::device_workflow_callback(void) {

	// Keep alive not implemented
	// Sets the current Motor Power Supply status
	if (
		(PCB301_GET_OUTPUTS_DATA_MOTOR_POWER_SUPPLY_ENABLE) &&
		(!inputs.emergency) &&
		(!inputs.cabinet_safety) &&
		(!inputs.powerdown)
		) outputs.power_48VDC_stat = true;
	else outputs.power_48VDC_stat = false;

	// Sets the current Power Switch status
	if (
		(PCB301_GET_OUTPUTS_DATA_MOTOR_SWITCH_ENABLE) &&
		(!inputs.cabinet_safety) &&
		(!inputs.compression_detected)
		) outputs.power_48SW_stat = true;
	else outputs.power_48SW_stat = false;

	// Rotation Led
	if (PCB301_GET_OUTPUTS_DATA_MAN_ROT_LED) outputs.rotation_led = true;
	else outputs.rotation_led = false;

	// Buzzer
	if (PCB301_GET_OUTPUTS_DATA_MANUAL_BUZZER) {
		if (PCB301_GET_OUTPUTS_DATA_BUZZER_STAT) device.buzzer_stat = true;
		else device.buzzer_stat = false;
	}

	// Feedback of the power supply status
	PCB301_SET_SYSTEM_MOTOR_OK(outputs.power_48VDC_stat);
	
	// Feedback of the power switch status
	PCB301_SET_SYSTEM_MOTOR_SWITCH(outputs.power_48SW_stat);

	// Evaluates the Battery voltage for the alarmn status
	PCB301_SET_SYSTEM_BATT1LOW((device.vbatt1 < 10));
	PCB301_SET_SYSTEM_BATT1LOW((device.vbatt2 < 10));

	// X-RAY button request
	PCB301_SET_XRAY_PUSH_BUTTON(((inputs.xray_button_input) || (PCB301_GET_OUTPUTS_DATA_BURNING_STAT && inputs.burning_jumper)));

	// Generator X-RAY enable output
	if (
		(((inputs.xray_button_input) || (PCB301_GET_OUTPUTS_DATA_BURNING_STAT && inputs.burning_jumper))) &&
		(inputs.closed_door) &&
		(PCB301_GET_OUTPUTS_DATA_XRAY_ENA)
		) outputs.generator_xray_ena = true;
	else outputs.generator_xray_ena = false;

	// Compression on 
	PCB301_SET_SYSTEM_COMPRESSION(inputs.compression_detected);

	// Compression ena
	if (PCB301_GET_OUTPUTS_DATA_COMPRESSOR_ENABLE) outputs.compression_ena = true;
	else outputs.compression_ena = false;

	// Calibration ena
	if (PCB301_GET_OUTPUTS_DATA_COMPRESSOR_CALIBRATION) outputs.calibration_ena = true;
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

	
	PCB301_SET_SYSTEM_ERROR_STATUS(false);
	PCB301_SET_SYSTEM_EMERGENCY_STATUS(inputs.emergency);
	PCB301_SET_SYSTEM_POWERDOWN(inputs.powerdown);
	PCB301_SET_SYSTEM_CABINET_SAFETY(inputs.cabinet_safety);
	PCB301_SET_SYSTEM_POWER_OFF_REQ(inputs.power_off_req);
	PCB301_SET_SYSTEM_BATTENA(inputs.battery_enable);

	PCB301_SET_BUTTON_VERTICAL_UP(inputs.keypad_up);
	PCB301_SET_BUTTON_VERTICAL_DOWN(inputs.keypad_down);
	PCB301_SET_BUTTON_SLIDE_UP(inputs.manual_slide_up);
	PCB301_SET_BUTTON_SLIDE_DOWN(inputs.manual_slide_down);
	PCB301_SET_BUTTON_ARM_CW(inputs.keypad_cw);
	PCB301_SET_BUTTON_ARM_CCW(inputs.keypad_ccw);

	PCB301_SET_BUTTON_BODY_CW(inputs.manual_body_cw);
	PCB301_SET_BUTTON_BODY_CCW(inputs.manual_body_ccw);

	PCB301_SET_SYSTEM_CLOSEDOOR(inputs.closed_door);
	PCB301_SET_SYSTEM_BURNING_JMP(inputs.burning_jumper);

	// Power Lock stat: set only once until system swuitched off
	if (PCB301_GET_OUTPUTS_DATA_POWER_LOCK) device.powerlock_stat = true;
	PCB301_SET_SYSTEM_POWER_LOCK(device.powerlock_stat);

	PCB301_SET_PEDAL_VERTICAL_UP(inputs.pedalboard_up);
	PCB301_SET_PEDAL_VERTICAL_DOWN(inputs.pedalboard_down);

	// Battery voltage
	PCB301_SET_BATTERY_VBATT(device.vbatt1, device.vbatt2);
}

void PCB301::device_reset_callback(void) {
	device.init();
	inputs.init();
	outputs.init();
}