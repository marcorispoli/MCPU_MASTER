#include "Pcb326.h"

void PCB326::device_workflow_callback(void) {

}

void PCB326::device_reset_callback(void) {
	// Init of the status register
	//status_registers[0] = setRegval(0x80, 0, 0x1 | 0x2 | 0x10, 0);
	//status_registers[1] = setRegval(12, 12, 0, 0);
}