#include "Pcb303.h"
void PCB303::device_workflow_callback(void) {

	// Decode the DATA registers ..
	
	// Decode Parameter register
	
	// Front/Back Left/Right target parameters
	for (int i = 0; i < NUM_COLLIMATION_SLOTS; i++) {
		protocol.parameter_register.decodeFBCollimationSlotRegister(parameter_registers[i], i);
		protocol.parameter_register.decodeLRCollimationSlotRegister(parameter_registers[i], i);
	}
		
	// Trap parameters
	for (int i = 0; i < NUM_COLLIMATION_SLOTS/2; i++) {
		protocol.parameter_register.decodeTrapCollimationSlotRegister(parameter_registers[i + NUM_COLLIMATION_SLOTS], i);
	}


	
}

void PCB303::device_reset_callback(void) {
	device.init();
	inputs.init();
	outputs.init();
}