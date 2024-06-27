#include "Pcb303.h"

/*
*
	#define PCB301_GET_SYSTEM_ERROR_STATUS(reg)			(bool)	(reg->d0 & 0x01)
	#define PCB301_GET_SYSTEM_EMERGENCY_STATUS(reg)		(bool)	(reg->d0 & 0x02)
	#define PCB301_GET_SYSTEM_POWERDOWN(reg)			(bool)	(reg->d0 & 0x04)
	#define PCB301_GET_SYSTEM_CABINET_SAFETY(reg)		(bool)	(reg->d0 & 0x08)
	#define PCB301_GET_SYSTEM_POWER_OFF_REQ(reg)		(bool)	(reg->d0 & 0x10)
	#define PCB301_GET_SYSTEM_BATT1LOW(reg)				(bool)	(reg->d0 & 0x20)
	#define PCB301_GET_SYSTEM_BATT2LOW(reg)				(bool)	(reg->d0 & 0x40)
	#define PCB301_GET_SYSTEM_BATTENA(reg)				(bool)	(reg->d0 & 0x80)

	#define PCB301_GET_BUTTON_VERTICAL_UP(reg)			(bool)	(reg->d1 & 0x1)
	#define PCB301_GET_BUTTON_VERTICAL_DOWN(reg)		(bool)	(reg->d1 & 0x2)
	#define PCB301_GET_BUTTON_SLIDE_UP(reg)				(bool)	(reg->d1 & 0x4)
	#define PCB301_GET_BUTTON_SLIDE_DOWN(reg)			(bool)	(reg->d1 & 0x8)
	#define PCB301_GET_BUTTON_ARM_CW(reg)				(bool)	(reg->d1 & 0x10)
	#define PCB301_GET_BUTTON_ARM_CCW(reg)				(bool)	(reg->d1 & 0x20)
	#define PCB301_GET_BUTTON_BODY_CW(reg)				(bool)	(reg->d1 & 0x40)
	#define PCB301_GET_BUTTON_BODY_CCW(reg)				(bool)	(reg->d1 & 0x80)

	#define PCB301_GET_SYSTEM_MOTOR_OK(reg)				(bool) (reg->d2 & 0x01)
	#define PCB301_GET_SYSTEM_MOTOR_SWITCH(reg)			(bool) (reg->d2 & 0x02)
	#define PCB301_GET_SYSTEM_COMPRESSION(reg)			(bool) (reg->d2 & 0x04)
	#define PCB301_GET_XRAY_PUSH_BUTTON(reg)			(bool) (reg->d2 & 0x08)
	#define PCB301_GET_SYSTEM_CLOSEDOOR(reg)			(bool) (reg->d2 & 0x10)
	#define PCB301_GET_SYSTEM_BURNING_JMP(reg)			(bool) (reg->d2 & 0x20)
	#define PCB301_GET_SYSTEM_POWER_LOCK(reg)			(bool) (reg->d2 & 0x40)

	#define PCB301_GET_PEDAL_VERTICAL_UP(reg)			(bool) (reg->d3 & 0x1)
	#define PCB301_GET_PEDAL_VERTICAL_DOWN(reg)			(bool) (reg->d3 & 0x2)

	// This is the BATTERY SECTION
	#define PCB301_GET_BATTERY_VBATT1(reg) (unsigned char) (reg->d0)
	#define PCB301_GET_BATTERY_VBATT2(reg) (unsigned char) (reg->d1)


*/
void PCB303::device_workflow_callback(void) {

}

void PCB303::device_reset_callback(void) {
	// Init of the status register
	//status_registers[0] = setRegval(0x80, 0, 0x1 | 0x2 | 0x10, 0);
	//status_registers[1] = setRegval(12, 12, 0, 0);
}