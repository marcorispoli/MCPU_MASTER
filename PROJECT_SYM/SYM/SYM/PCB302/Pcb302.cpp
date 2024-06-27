#include "Pcb302.h"


#define PCB302_SET_SYSTEM_MANUAL_SERVO_UP(reg)			status_registers[0] = setRegBit(status_registers[0], 0x1, (reg))
#define PCB302_SET_SYSTEM_MANUAL_SERVO_DWN(reg)			status_registers[0] = setRegBit(status_registers[0], 0x2, (reg))
#define PCB302_SET_SYSTEM_PEDAL_UP(reg)					status_registers[0] = setRegBit(status_registers[0], 0x4, (reg))
#define PCB302_SET_SYSTEM_PEDAL_DWN(reg)				status_registers[0] = setRegBit(status_registers[0], 0x8, (reg))

#define PCB302_SET_SYSTEM_IDLE(reg)						status_registers[0] = setRegBit(status_registers[0], 0x100, (reg))
#define PCB302_SET_SYSTEM_MANUAL_ACTIVATION(reg)		status_registers[0] = setRegBit(status_registers[0], 0x200, (reg))
#define PCB302_SET_SYSTEM_PEDAL_ACTIVATION(reg)			status_registers[0] = setRegBit(status_registers[0], 0x400, (reg))
#define PCB302_SET_SYSTEM_COMMAND_ACTIVATION(reg)		status_registers[0] = setRegBit(status_registers[0], 0x800, (reg))
#define PCB302_SET_SYSTEM_UPWARD_DIRECTION(reg)			status_registers[0] = setRegBit(status_registers[0], 0x1000, (reg))
#define PCB302_SET_SYSTEM_DOWNWARD_DIRECTION(reg)		status_registers[0] = setRegBit(status_registers[0], 0x2000, (reg))
#define PCB302_SET_SYSTEM_UNLOCK_ACTIVATION(reg)		status_registers[0] = setRegBit(status_registers[0], 0x4000, (reg))
#define PCB302_SET_SYSTEM_FAULT(reg)					status_registers[0] = setRegBit(status_registers[0], 0x8000, (reg))

#define PCB302_SET_SYSTEM_CMP_ENA(reg)					status_registers[0] = setRegBit(status_registers[0], 0x10000, (reg))
#define PCB302_SET_SYSTEM_CMP_ON(reg)					status_registers[0] = setRegBit(status_registers[0], 0x20000, (reg))
#define PCB302_SET_SYSTEM_ZERO_COMPRESSION(reg)			status_registers[0] = setRegBit(status_registers[0], 0x40000, (reg))
#define PCB302_SET_SYSTEM_SMARTUP_TARGET(reg)			status_registers[0] = setRegBit(status_registers[0], 0x80000, (reg))
#define PCB302_SET_SYSTEM_FORCE_TARGET(reg)				status_registers[0] = setRegBit(status_registers[0], 0x100000, (reg))
#define PCB302_SET_SYSTEM_LIMIT_COMPRESSION(reg)		status_registers[0] = setRegBit(status_registers[0], 0x200000, (reg))


void PCB302::device_workflow_callback(void) {

	// Feedback compression ena
	PCB302_SET_SYSTEM_CMP_ENA(inputs.compression_ena);

	// Handle the compression event
	if ((inputs.compression_ena) && (device.current_paddle_code != paddleCodes::PADDLE_NOT_DETECTED)) {

		// Unlock event
		if (inputs.compression_up && inputs.compression_down) {
			device.current_force = 0;
			PCB302_SET_SYSTEM_DOWNWARD_DIRECTION(false);
			PCB302_SET_SYSTEM_UPWARD_DIRECTION(false);
			PCB302_SET_SYSTEM_UNLOCK_ACTIVATION(true);
			PCB302_SET_SYSTEM_IDLE(false);
		}
		else {
			if ((inputs.compression_up) || (inputs.paddle_up)) {
				PCB302_SET_SYSTEM_DOWNWARD_DIRECTION(false);
				PCB302_SET_SYSTEM_IDLE(false);

				if (device.current_force == 0) {					
					PCB302_SET_SYSTEM_UPWARD_DIRECTION(false);					
				}
				else {
					PCB302_SET_SYSTEM_UPWARD_DIRECTION(true);
					device.current_force -= 5;
					if (device.current_force < 0) device.current_force = 0;
				}				
			}
			else if ((inputs.compression_down) || (inputs.paddle_down)) {
				PCB302_SET_SYSTEM_UPWARD_DIRECTION(false);
				PCB302_SET_SYSTEM_IDLE(false);

				if (device.current_force >= device.target_force) {					
					PCB302_SET_SYSTEM_DOWNWARD_DIRECTION(false);
				}
				else {
					PCB302_SET_SYSTEM_DOWNWARD_DIRECTION(true);
					device.current_force += 5;
					if (device.current_force > device.target_force) device.current_force = device.target_force;
				}
				
			}
			else {
				PCB302_SET_SYSTEM_DOWNWARD_DIRECTION(false);
				PCB302_SET_SYSTEM_UPWARD_DIRECTION(false);
				PCB302_SET_SYSTEM_UNLOCK_ACTIVATION(false);
				PCB302_SET_SYSTEM_IDLE(true);
			}
		}
	}
	else {
		PCB302_SET_SYSTEM_DOWNWARD_DIRECTION(false);
		PCB302_SET_SYSTEM_UPWARD_DIRECTION(false);
		PCB302_SET_SYSTEM_UNLOCK_ACTIVATION(false);
		PCB302_SET_SYSTEM_IDLE(true);
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

	if (outputs.compression_detected) PCB302_SET_SYSTEM_CMP_ON(true);
	else PCB302_SET_SYSTEM_CMP_ON(false);

	// Set the actual compression thicknes
	device.current_thickness = (int)((float)device.target_thickness * ((float)device.current_force / (float)device.target_force));
	if (device.current_thickness > 255) device.current_thickness = 255;
	

	// Sets the Paddle, Compression and Force register
	Byte D0 = (Byte) (device.current_thickness & 0xFF);
	Byte D1 = (Byte) ((device.current_thickness >> 8) & 0x0F);
	D1 += (Byte)((device.current_force & 0x0F) << 4);
	Byte D2 = (Byte)((device.current_force >> 4) & 0xFF);
	Byte D3 = (Byte) device.current_paddle_code;
	status_registers[2] = setRegVal(D0, D1, D2, D3);


}

void PCB302::device_reset_callback(void) {
	device.init();
	inputs.init();
	outputs.init();
}