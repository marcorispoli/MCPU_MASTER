#include "Pcb304.h"


void PCB304::device_workflow_callback(void) {
	
	// Decode DATA regsiter
	protocol.data_register.decodeGridRegister(data_registers[(Byte)ProtocolStructure::DataRegister::register_index::GRID_REGISTER]);
	protocol.data_register.decodeDisplayRegister(data_registers[(Byte)ProtocolStructure::DataRegister::register_index::DISPLAY_REGISTER]);

	
	// Display On Management
	if (protocol.data_register.display_on) {

		// Handle the keepalive
		if (device.display_keepalive_stat != protocol.data_register.display_keep_alive) device.display_keepalive_count = 0;
		else device.display_keepalive_count++;
		device.display_keepalive_stat = protocol.data_register.display_keep_alive;

		// Keepalive Timeout: display switched Off
		if (device.display_keepalive_count >= ProtocolStructure::display_keep_alive_tmo) {
			device.display_keepalive_count = ProtocolStructure::display_keep_alive_tmo;

			device.display_on = false;
			device.blink_count = 0;
			device.display_intensity = 0;
		}
		else {
			device.display_intensity = protocol.data_register.display_intensity;
			if (protocol.data_register.display_blink) {
				device.blink_count++;
				if (device.blink_count > hardware_device::blink_tmo) {
					device.blink_count = 0;
					device.display_on = !device.display_on;
				}
			}
			else {
				device.display_on = true;
				device.blink_count = 0;
			}

			// Sets the Value and the decimal points
			signed short dval = (signed short) protocol.data_register.display_value;
			if (protocol.data_register.dot_position == 0) {
				device.display_value = (float)dval;
			}
			else if (protocol.data_register.dot_position == 1) {
				device.display_value = (float)dval /10;
			}
			else if (protocol.data_register.dot_position == 2) {
				device.display_value = (float)dval / 100;
			}
			else if (protocol.data_register.dot_position == 3) {
				device.display_value = (float)dval / 1000;
			}else device.display_value = (float)dval;
		}
		
	}
	else {
		device.display_on = false;
		device.blink_count = 0;
		device.display_intensity = 0;
	}

	
	// Grid Management
	if (protocol.data_register.InOutAutoEnable) {

		if ((protocol.data_register.InOutStatus) && (device.grid_InOut_stat != hardware_device::IN_FIELD)) {

			// In Field Activation
			if (device.grid_InOut_stat == hardware_device::INOUT_UNDEF) {
				device.grid_InOut_stat = hardware_device::INOUT_RUN;
				device.grid_Translation_stat = hardware_device::TRANSLATION_CENTER;
				device.inout_run_count = 10;
			}
			else if (device.grid_InOut_stat == hardware_device::INOUT_RUN) {
				device.inout_run_count--;
				if (device.inout_run_count == 0) {
					device.grid_InOut_stat = hardware_device::IN_FIELD;
					device.grid_Translation_stat = hardware_device::TRANSLATION_HOME;
				}
			}
			else if (device.grid_InOut_stat == hardware_device::OUT_FIELD) {
				device.grid_InOut_stat = hardware_device::INOUT_UNDEF;
			}
		}
		else if ((!protocol.data_register.InOutStatus) && (device.grid_InOut_stat != hardware_device::OUT_FIELD)) {
			// Out Field Activation
			if (device.grid_InOut_stat == hardware_device::INOUT_UNDEF) {
				device.grid_InOut_stat = hardware_device::INOUT_RUN;
				device.grid_Translation_stat = hardware_device::TRANSLATION_CENTER;
				device.inout_run_count = 10;
			}
			else if (device.grid_InOut_stat == hardware_device::INOUT_RUN) {
				device.inout_run_count--;
				if (device.inout_run_count == 0) {
					device.grid_InOut_stat = hardware_device::OUT_FIELD;
					device.grid_Translation_stat = hardware_device::TRANSLATION_CENTER;
				}
			}
			else if (device.grid_InOut_stat == hardware_device::IN_FIELD) {
				device.grid_InOut_stat = hardware_device::INOUT_UNDEF;
			}			
		}
	}

	// In Out Executing bit
	if (device.grid_InOut_stat == hardware_device::INOUT_RUN) protocol.status_register.inout_executing = true;
	else protocol.status_register.inout_executing = false;

	// In Out Position 
	if (device.grid_InOut_stat == hardware_device::OUT_FIELD) {
		protocol.status_register.inField = false;
		protocol.status_register.outField = true;
	}else if (device.grid_InOut_stat == hardware_device::IN_FIELD) {
		protocol.status_register.inField = true;
		protocol.status_register.outField = false;
	}
	else {
		protocol.status_register.inField = false;
		protocol.status_register.outField = false;
	}

	if (device.grid_Translation_stat == hardware_device::TRANSLATION_RUN) protocol.status_register.transversal_executing = true;
	else protocol.status_register.transversal_executing = false;


	if (device.grid_Translation_stat == hardware_device::TRANSLATION_HOME) {
		protocol.status_register.home = true;
		protocol.status_register.center = false;
	}else if (device.grid_Translation_stat == hardware_device::TRANSLATION_CENTER) {
		protocol.status_register.home = false;
		protocol.status_register.center = true;
	}
	else {
		protocol.status_register.home = false;
		protocol.status_register.center = false;
	}
	// Encode STATUS registers
	status_registers[(Byte)ProtocolStructure::StatusRegister::register_index::GRID] = protocol.status_register.encodeGridRegister();
}

void PCB304::device_reset_callback(void) {
	device.init();
	inputs.init();
	outputs.init();
}