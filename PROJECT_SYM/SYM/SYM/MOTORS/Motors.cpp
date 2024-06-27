#include "Motors.h"
#include "pd4_od.h"
#include "canInterface.h"
#include <thread>
#include "mutex"




#define MOTOR_LOOP_TIME_MS 10
#define OD_1001_IDX 0 // General Error Register
#define OD_1003_01_IDX 1 // Specific error
#define OD_2034_IDX 16 // Upper Voltage Warning Level
#define OD_2035_IDX 17 // Lower Voltage Warning Level

#define OD_2062_IDX 27 // Speed denominator (numerator always 1)

#define OD_6040_IDX 76
#define OD_6041_IDX 77
#define OD_6060_IDX 83
#define OD_6064_IDX 84 // Current Encoder Position
#define OD_607A_IDX	89 // Target Position
#define OD_607C_IDX	92 // Home Offset

#define OD_607D_01_IDX	93 // Min position
#define OD_607D_02_IDX	94 // Max position

#define OD_6081_IDX	96 // Speed
#define OD_6098_IDX 105 // Homing method



Motors::Motors(unsigned char id) {

	devId = id;
	active = false;
	fault_condition = false;
	rot_to_unit = 1;

	// Creates the Object Dictionary
	object_dictionary = gcnew cli::array<ODRegister^>(114);
	unsigned char index = 0;
	object_dictionary[index++] = gcnew ODRegister(OD_1001_00); // 0
	object_dictionary[index++] = gcnew ODRegister(OD_1003_01); // 1
	object_dictionary[index++] = gcnew ODRegister(OD_1010_01);
	object_dictionary[index++] = gcnew ODRegister(OD_1010_02);
	object_dictionary[index++] = gcnew ODRegister(OD_1010_03);
	object_dictionary[index++] = gcnew ODRegister(OD_1010_04);
	object_dictionary[index++] = gcnew ODRegister(OD_1010_05);
	object_dictionary[index++] = gcnew ODRegister(OD_1010_06);
	object_dictionary[index++] = gcnew ODRegister(OD_1029_01);
	object_dictionary[index++] = gcnew ODRegister(OD_1029_02);
	object_dictionary[index++] = gcnew ODRegister(OD_1F50_02);
	object_dictionary[index++] = gcnew ODRegister(OD_1F51_02);
	object_dictionary[index++] = gcnew ODRegister(OD_1F57_02);
	object_dictionary[index++] = gcnew ODRegister(OD_2031_00); 
	object_dictionary[index++] = gcnew ODRegister(OD_2032_00);
	object_dictionary[index++] = gcnew ODRegister(OD_2033_00);
	object_dictionary[index++] = gcnew ODRegister(OD_2034_00); // 16
	object_dictionary[index++] = gcnew ODRegister(OD_2035_00); // 17
	object_dictionary[index++] = gcnew ODRegister(OD_2036_00);
	object_dictionary[index++] = gcnew ODRegister(OD_2037_00);
	object_dictionary[index++] = gcnew ODRegister(OD_203B_01);
	object_dictionary[index++] = gcnew ODRegister(OD_203B_02);
	object_dictionary[index++] = gcnew ODRegister(OD_203B_03);
	object_dictionary[index++] = gcnew ODRegister(OD_203B_04);
	object_dictionary[index++] = gcnew ODRegister(OD_203B_05);
	object_dictionary[index++] = gcnew ODRegister(OD_2056_00);
	object_dictionary[index++] = gcnew ODRegister(OD_2061_00);
	object_dictionary[index++] = gcnew ODRegister(OD_2062_00); // 27
	object_dictionary[index++] = gcnew ODRegister(OD_2063_00);
	object_dictionary[index++] = gcnew ODRegister(OD_2064_00);
	object_dictionary[index++] = gcnew ODRegister(OD_2065_00);
	object_dictionary[index++] = gcnew ODRegister(OD_2066_00);
	object_dictionary[index++] = gcnew ODRegister(OD_2300_00);
	object_dictionary[index++] = gcnew ODRegister(OD_2301_00);
	object_dictionary[index++] = gcnew ODRegister(OD_2302_00);
	object_dictionary[index++] = gcnew ODRegister(OD_2500_01);
	object_dictionary[index++] = gcnew ODRegister(OD_2700_01);
	object_dictionary[index++] = gcnew ODRegister(OD_2700_02);
	object_dictionary[index++] = gcnew ODRegister(OD_2700_03);
	object_dictionary[index++] = gcnew ODRegister(OD_2700_04);
	object_dictionary[index++] = gcnew ODRegister(OD_2700_05);
	object_dictionary[index++] = gcnew ODRegister(OD_2700_06);
	object_dictionary[index++] = gcnew ODRegister(OD_2700_07);
	object_dictionary[index++] = gcnew ODRegister(OD_2700_08);
	object_dictionary[index++] = gcnew ODRegister(OD_2700_09);
	object_dictionary[index++] = gcnew ODRegister(OD_3202_00);
	object_dictionary[index++] = gcnew ODRegister(OD_320B_01);
	object_dictionary[index++] = gcnew ODRegister(OD_320B_01);
	object_dictionary[index++] = gcnew ODRegister(OD_320B_02);
	object_dictionary[index++] = gcnew ODRegister(OD_320B_03);
	object_dictionary[index++] = gcnew ODRegister(OD_320B_04);
	object_dictionary[index++] = gcnew ODRegister(OD_3210_01);
	object_dictionary[index++] = gcnew ODRegister(OD_3210_02);
	object_dictionary[index++] = gcnew ODRegister(OD_3210_05);
	object_dictionary[index++] = gcnew ODRegister(OD_3210_06);
	object_dictionary[index++] = gcnew ODRegister(OD_3220_01);
	object_dictionary[index++] = gcnew ODRegister(OD_3221_00);
	object_dictionary[index++] = gcnew ODRegister(OD_3240_01);
	object_dictionary[index++] = gcnew ODRegister(OD_3240_02);
	object_dictionary[index++] = gcnew ODRegister(OD_3240_03);
	object_dictionary[index++] = gcnew ODRegister(OD_3240_04);
	object_dictionary[index++] = gcnew ODRegister(OD_3240_05);
	object_dictionary[index++] = gcnew ODRegister(OD_3240_06);
	object_dictionary[index++] = gcnew ODRegister(OD_3241_01);
	object_dictionary[index++] = gcnew ODRegister(OD_3241_02);
	object_dictionary[index++] = gcnew ODRegister(OD_3241_03);
	object_dictionary[index++] = gcnew ODRegister(OD_3241_04);
	object_dictionary[index++] = gcnew ODRegister(OD_3250_02);
	object_dictionary[index++] = gcnew ODRegister(OD_3250_03);
	object_dictionary[index++] = gcnew ODRegister(OD_3250_04);
	object_dictionary[index++] = gcnew ODRegister(OD_3250_08);
	object_dictionary[index++] = gcnew ODRegister(OD_3700_00);
	object_dictionary[index++] = gcnew ODRegister(OD_4013_01);
	object_dictionary[index++] = gcnew ODRegister(OD_4014_01);
	object_dictionary[index++] = gcnew ODRegister(OD_4014_02);
	object_dictionary[index++] = gcnew ODRegister(OD_4014_03);
	object_dictionary[index++] = gcnew ODRegister(OD_6040_00); // 76
	object_dictionary[index++] = gcnew ODRegister(OD_6041_00); // 77
	object_dictionary[index++] = gcnew ODRegister(OD_605A_00);
	object_dictionary[index++] = gcnew ODRegister(OD_605B_00);
	object_dictionary[index++] = gcnew ODRegister(OD_605C_00);
	object_dictionary[index++] = gcnew ODRegister(OD_605D_00);
	object_dictionary[index++] = gcnew ODRegister(OD_605E_00);
	object_dictionary[index++] = gcnew ODRegister(OD_6060_00); // 83
	object_dictionary[index++] = gcnew ODRegister(OD_6064_00); // 84
	object_dictionary[index++] = gcnew ODRegister(OD_6065_00);
	object_dictionary[index++] = gcnew ODRegister(OD_6066_00);
	object_dictionary[index++] = gcnew ODRegister(OD_6067_00);
	object_dictionary[index++] = gcnew ODRegister(OD_6068_00);
	object_dictionary[index++] = gcnew ODRegister(OD_607A_00); // 89
	object_dictionary[index++] = gcnew ODRegister(OD_607B_01);
	object_dictionary[index++] = gcnew ODRegister(OD_607B_02);
	object_dictionary[index++] = gcnew ODRegister(OD_607C_00); // 92
	object_dictionary[index++] = gcnew ODRegister(OD_607D_01); // 93
	object_dictionary[index++] = gcnew ODRegister(OD_607D_02); // 94
	object_dictionary[index++] = gcnew ODRegister(OD_607E_00);
	object_dictionary[index++] = gcnew ODRegister(OD_6081_00); // 96
	object_dictionary[index++] = gcnew ODRegister(OD_6082_00);
	object_dictionary[index++] = gcnew ODRegister(OD_6083_00);
	object_dictionary[index++] = gcnew ODRegister(OD_6084_00);
	object_dictionary[index++] = gcnew ODRegister(OD_6085_00);
	object_dictionary[index++] = gcnew ODRegister(OD_608F_01);
	object_dictionary[index++] = gcnew ODRegister(OD_608F_02);
	object_dictionary[index++] = gcnew ODRegister(OD_6091_01);
	object_dictionary[index++] = gcnew ODRegister(OD_6091_02);
	object_dictionary[index++] = gcnew ODRegister(OD_6098_00); // 105
	object_dictionary[index++] = gcnew ODRegister(OD_6099_01);
	object_dictionary[index++] = gcnew ODRegister(OD_6099_02);
	object_dictionary[index++] = gcnew ODRegister(OD_609A_00);
	object_dictionary[index++] = gcnew ODRegister(OD_60C5_00);
	object_dictionary[index++] = gcnew ODRegister(OD_60C6_00);
	object_dictionary[index++] = gcnew ODRegister(OD_60F2_00);
	object_dictionary[index++] = gcnew ODRegister(OD_60FD_00);
	object_dictionary[index++] = gcnew ODRegister(OD_60FE_01);

	profile_positioning_init = true;
	profile_homing_init = true;
	reset_event = true;
	current_encoder_position = 0;
	object_dictionary[OD_6064_IDX]->data = 0; // Encoder position

	// connect the reception handler
	canInterface::canrx_canopen_sdo_event += gcnew canInterface::rxData_slot(this, &Motors::canrx_canopen_sdo_event);
	canInterface::canrx_canopen_nmt_event += gcnew canInterface::rxData_slot(this, &Motors::canrx_canopen_nmt_event);
	canInterface::connection_event += gcnew canInterface::connection_slot(this, &Motors::handle_connection);


	// Start the reception thread
	running_thread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &Motors::threadWork));
	running_thread->Name = "Motor Interface thread";
	running_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
	running_thread->Start();

}

Motors::ODRegister^ Motors::findRegister(unsigned short index, unsigned char sub_index) {
	int size_array = object_dictionary->Length;

	int i = size_array / 2;
	int low = 0;
	int high = size_array - 1;
	static int iterazioni = 0;

	iterazioni = 0;
	while (true) {
		iterazioni++;
		if ((object_dictionary[i]->index == index) && (object_dictionary[i]->subindex == sub_index)) {
			return object_dictionary[i];
		}

		if (index > object_dictionary[i]->index) {
			low = i + 1;
		}
		else if (index < object_dictionary[i]->index) {
			high = i - 1;
		}
		else {
			if (sub_index > object_dictionary[i]->subindex) {
				low = i + 1;
			}
			else  {
				high = i - 1;
			}
		}

		// No match
		if (low > high) {
			return nullptr;
		}
		i = (low + high) / 2;
	}
}

int Motors::findIndex(unsigned short index, unsigned char sub_index) {
	int size_array = object_dictionary->Length;

	int i = size_array / 2;
	int low = 0;
	int high = size_array - 1;
	static int iterazioni = 0;

	iterazioni = 0;
	while (true) {
		iterazioni++;
		if ((object_dictionary[i]->index == index) && (object_dictionary[i]->subindex == sub_index)) {
			return i;
		}

		if (index > object_dictionary[i]->index) {
			low = i + 1;
		}
		else if (index < object_dictionary[i]->index) {
			high = i - 1;
		}
		else {
			if (sub_index > object_dictionary[i]->subindex) {
				low = i + 1;
			}
			else {
				high = i - 1;
			}
		}

		// No match
		if (low > high) {
			return -1;
		}
		i = (low + high) / 2;
	}
}

void Motors::canrx_canopen_sdo_event(void) {
	cli::array<Byte>^ buffer = gcnew cli::array<Byte>(8);

	if (canInterface::canId != devId + 0x600) return;
	if (canInterface::datalen != 8) return;

	ODRegister::SDOCommandCodes dt = (ODRegister::SDOCommandCodes)canInterface::canDataBuffer[0];
	unsigned short index = canInterface::canDataBuffer[1] + 256 * canInterface::canDataBuffer[2];
	unsigned char subindex = canInterface::canDataBuffer[3];
	
	// Inexistent object
	int i = findIndex(index, subindex);
	if (i == -1) {
		buffer[0] = 0x80;
		buffer[1] = canInterface::canDataBuffer[1];
		buffer[2] = canInterface::canDataBuffer[2];
		buffer[3] = canInterface::canDataBuffer[3];
		buffer[4] = 0;
		buffer[5] = 0;
		buffer[6] = 0x02;
		buffer[7] = 0x06;
		canInterface::send(devId + 0x580, 8, buffer);
		return;
	}

	// Data len
	if (((unsigned char)dt & 0xF0) == (unsigned char)ODRegister::SDOCommandCodes::WRCMD) {
		// Write command
		if (((unsigned char)dt & 0xF) != (unsigned char)object_dictionary[i]->data_dim) {
			buffer[0] = 0x80;
			buffer[1] = canInterface::canDataBuffer[1];
			buffer[2] = canInterface::canDataBuffer[2];
			buffer[3] = canInterface::canDataBuffer[3];
			buffer[4] = 0x12;
			buffer[5] = 0;
			buffer[6] = 0x07;
			buffer[7] = 0x06;
			canInterface::send(devId + 0x580, 8, buffer);
			return;
		}

		// Writes the content
		object_dictionary[i]->data = ODRegister::getVal(canInterface::canDataBuffer);
		buffer[0] = 0x60;
		buffer[1] = canInterface::canDataBuffer[1];
		buffer[2] = canInterface::canDataBuffer[2];
		buffer[3] = canInterface::canDataBuffer[3];
		buffer[4] = 0;
		buffer[5] = 0;
		buffer[6] = 0;
		buffer[7] = 0;
		canInterface::send(devId + 0x580, 8, buffer);

	}
	else if (((unsigned char)dt & 0xF0) == (unsigned char)ODRegister::SDOCommandCodes::RDCMD) {
		buffer[0] = 0x40 | (unsigned char) object_dictionary[i]->data_dim;
		buffer[1] = canInterface::canDataBuffer[1];
		buffer[2] = canInterface::canDataBuffer[2];
		buffer[3] = canInterface::canDataBuffer[3];
		buffer[4] = (unsigned char) (object_dictionary[i]->data & 0xFF);
		buffer[5] = (unsigned char)((object_dictionary[i]->data >>8) & 0xFF);
		buffer[6] = (unsigned char)((object_dictionary[i]->data >> 16) & 0xFF);
		buffer[7] = (unsigned char)((object_dictionary[i]->data >> 24) & 0xFF);
		canInterface::send(devId + 0x580, 8, buffer);
	}

	return;


}

void Motors::send_bootup_message(void) {
	cli::array<Byte>^ buffer = gcnew cli::array<Byte>(8);
	for (int i = 0; i < 7; i++) buffer[i] = 0;
	canInterface::send(devId + 0x700, 1, buffer);

}
void Motors::canrx_canopen_nmt_event(void) {

	// Reset NMT code
	if (canInterface::canDataBuffer[0] == 0x81) {
		if ((canInterface::canDataBuffer[1] == 0) || (canInterface::canDataBuffer[1] == devId)) {
			reset_event = true;
		}
	}	
	return;
}
void Motors::handle_connection(bool stat) {

}

float Motors::getRotationFromEncoder(int encoder) {
	long lval = ((long) encoder * 10) / 2000;
	return (float) lval / 10.0;
}

float Motors::getUserUnitFromEncoder(int encoder) {
	float rot = getRotationFromEncoder(encoder);
	return rot / rot_to_unit ;
}



void Motors::threadWork(void) {
	
	// After reset the motor starts in not ready to 
	CiA_current_status = _CiA402Status::CiA402_NotReadyToSwitchOn;


	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(MOTOR_LOOP_TIME_MS));

		if (reset_event) {
			reset_event = false;
			for (int i = 0; i < object_dictionary->Length; i++) object_dictionary[i]->data = object_dictionary[i]->default_data;
			current_encoder_position = (int)object_dictionary[OD_6064_IDX]->data;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			// If the connection is not present waits here
			while(!canInterface::isConnected()) std::this_thread::sleep_for(std::chrono::milliseconds(100));

			// Send the bootup message
			send_bootup_message();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}
		
		object_dictionary[OD_6064_IDX]->data = (unsigned int)current_encoder_position;
		
		// Voltage condition
		if ((voltage_supply_mV < object_dictionary[OD_2035_IDX]->data) || (voltage_supply_mV > object_dictionary[OD_2034_IDX]->data)) {
			
			fault_condition = true;
			object_dictionary[OD_1001_IDX]->data = 0x4; // Voltage error
			if (voltage_supply_mV < object_dictionary[OD_2035_IDX]->data)	object_dictionary[OD_1003_01_IDX]->data = (0x00003100 | 0x00040000 | 0x01000000); // low voltage code
			else object_dictionary[OD_1003_01_IDX]->data = (0x00003100 | 0x00040000 | 0x01000000); // high voltage code
			
		}
		else {
			fault_condition = false;
			object_dictionary[OD_1001_IDX]->data = 0;
			object_dictionary[OD_1003_01_IDX]->data = 0;

		}
		
		// Activates the fault reaction
		if ( (fault_condition) && ((CiA_current_status != _CiA402Status::CiA402_Fault) && (CiA_current_status != _CiA402Status::CiA402_FaultReactionActive))) {
			CiA_current_status = _CiA402Status::CiA402_FaultReactionActive;
		}
			

		switch (CiA_current_status) {
		case _CiA402Status::CiA402_NotReadyToSwitchOn: not_ready_to_switch_on_callback(); break;
		case _CiA402Status::CiA402_SwitchOnDisabled: switched_on_disabled_callback(); break;
		case _CiA402Status::CiA402_ReadyToSwitchOn: ready_to_switch_on_callback(); break;
		case _CiA402Status::CiA402_SwitchedOn: switched_on_callback(); break;
		case _CiA402Status::CiA402_OperationEnabled: operation_enabled_callback(); break;
		case _CiA402Status::CiA402_QuickStopActive: quick_stop_active_callback(); break;
		case _CiA402Status::CiA402_FaultReactionActive: fault_reaction_active_callback(); break;
		case _CiA402Status::CiA402_Fault: fault_active_callback(); break;

		}
		
	}
}


void Motors::not_ready_to_switch_on_callback() {
	
	object_dictionary[OD_6041_IDX]->data &= ~0x4F;
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	CiA_current_status = _CiA402Status::CiA402_SwitchOnDisabled;
	return;
}

void Motors::switched_on_disabled_callback() {
	object_dictionary[OD_6041_IDX]->data &= ~0x4F;
	object_dictionary[OD_6041_IDX]->data |= 0x40;

	// Evaluates the change status on the 6040
	unsigned int val = object_dictionary[OD_6040_IDX]->data & 0x87;
	if (val == 6) {
		CiA_current_status = _CiA402Status::CiA402_ReadyToSwitchOn;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return;
	}

}
void Motors::ready_to_switch_on_callback() {
	object_dictionary[OD_6041_IDX]->data &= ~0x6F;
	object_dictionary[OD_6041_IDX]->data |= 0x21;

	// Evaluates the change status on the 6040
	unsigned int val = object_dictionary[OD_6040_IDX]->data & 0x8F;
	if (val == 7) {
		CiA_current_status = _CiA402Status::CiA402_SwitchedOn;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return;
	}

	val = object_dictionary[OD_6040_IDX]->data & 0x82;
	if (val == 0) {
		CiA_current_status = _CiA402Status::CiA402_SwitchOnDisabled;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return;
	}
}


void Motors::switched_on_callback() {
	object_dictionary[OD_6041_IDX]->data &= ~0x6F;
	object_dictionary[OD_6041_IDX]->data |= 0x23;

	unsigned int val = object_dictionary[OD_6040_IDX]->data & 0x82;
	if (val == 0) {
		CiA_current_status = _CiA402Status::CiA402_SwitchOnDisabled;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return;
	}

	val = object_dictionary[OD_6040_IDX]->data & 0x87;
	if (val == 6) {
		CiA_current_status = _CiA402Status::CiA402_ReadyToSwitchOn;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return;
	}

	val = object_dictionary[OD_6040_IDX]->data & 0x8F;
	if (val == 0xF) {
		CiA_current_status = _CiA402Status::CiA402_OperationEnabled;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return;
	}

	// Evaluates the profile management
	unsigned int profile = object_dictionary[OD_6060_IDX]->data;
	if (profile == 6) {
		if(object_dictionary[OD_6098_IDX]->data == 35) profile_homing_management();		
	}

}
void Motors::operation_enabled_callback() {
	object_dictionary[OD_6041_IDX]->data &= ~0x6F;
	object_dictionary[OD_6041_IDX]->data |= 0x27;

	unsigned int val = object_dictionary[OD_6040_IDX]->data & 0x8F;
	if (val == 7) {
		CiA_current_status = _CiA402Status::CiA402_SwitchedOn;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		profile_positioning_init = true;
		profile_homing_init = true;
		return;
	}

	val = object_dictionary[OD_6040_IDX]->data & 0x86;
	if (val == 2) {
		CiA_current_status = _CiA402Status::CiA402_QuickStopActive;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		profile_positioning_init = true;
		profile_homing_init = true;
		return;
	}

	val = object_dictionary[OD_6040_IDX]->data & 0x87;
	if (val == 6) {
		CiA_current_status = _CiA402Status::CiA402_ReadyToSwitchOn;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		profile_positioning_init = true;
		profile_homing_init = true;
		return;
	}

	val = object_dictionary[OD_6040_IDX]->data & 0x82;
	if (val == 0) {
		CiA_current_status = _CiA402Status::CiA402_SwitchOnDisabled;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		profile_positioning_init = true;
		profile_homing_init = true;
		return;
	}

	// Evaluates the profile management
	unsigned int profile = object_dictionary[OD_6060_IDX]->data;
	if(profile == 1) profile_positioning_management();
	else if(profile == 6) profile_homing_management();

}
void Motors::quick_stop_active_callback() {
	object_dictionary[OD_6041_IDX]->data &= ~0x6F;
	object_dictionary[OD_6041_IDX]->data |= 0x07;

	unsigned int val = object_dictionary[OD_6040_IDX]->data & 0x82;	
	if (val == 0) {
		CiA_current_status = _CiA402Status::CiA402_SwitchOnDisabled;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return;
	}
}

void Motors::fault_reaction_active_callback() {
	object_dictionary[OD_6041_IDX]->data &= ~0x4F;
	object_dictionary[OD_6041_IDX]->data |= 0xF;

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	CiA_current_status = _CiA402Status::CiA402_Fault;


}
void Motors::fault_active_callback() {
	object_dictionary[OD_6041_IDX]->data &= ~0x4F;
	object_dictionary[OD_6041_IDX]->data |= 0x8;

	if (fault_condition) return;

	// If the fault condition should no more present, the remote can try to change the Cia Status
	unsigned int val = object_dictionary[OD_6040_IDX]->data & 0x80;
	if (val == 0x80) {
		CiA_current_status = _CiA402Status::CiA402_SwitchOnDisabled;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return;
	}
}


void Motors::profile_positioning_management(void) {
	
	if (profile_positioning_init) {		

		// Initializes the 6040 register:
		od_6040_reg = object_dictionary[OD_6040_IDX]->data;
		position_started = false;
		target_ok = false;
		profile_positioning_init = false;
	}

	
	
	if (!position_started) {
		if ((object_dictionary[OD_6040_IDX]->data & 0x10) && (!(od_6040_reg & 0x10))) {
			
			// Initializes the 6040 register
			object_dictionary[OD_6040_IDX]->data &= ~0x400; // Bit 10: Target Ok
			object_dictionary[OD_6040_IDX]->data &= ~0x800; // Bit 11: Limits
			object_dictionary[OD_6040_IDX]->data &= ~0x1000; // Bit 12: Set Point
			object_dictionary[OD_6040_IDX]->data &= ~0x2000; // Bit 13: Following error

			position_started = true;
			current_encoder_position = (int) object_dictionary[OD_6064_IDX]->data;

			// Target solution definition (0 Absolute) (1 Relative)
			if (object_dictionary[OD_6040_IDX]->data & 0x40) {
				// Relative target = current position + target(OD_607A_IDX)
				target_position = current_encoder_position + (int) object_dictionary[OD_607A_IDX]->data;
			}
			else {
				// Absolute Target: target = OD_607A_IDX
				target_position = (int) object_dictionary[OD_607A_IDX]->data;
			}

			if (target_position > current_encoder_position) increment_direction = true;
			else increment_direction = false;

			// Speed * deltaT
			int speed_denominator = object_dictionary[OD_2062_IDX]->data;
			delta_count = (object_dictionary[OD_6081_IDX]->data * MOTOR_LOOP_TIME_MS * 2 / speed_denominator); 
			target_ok = false;			
		
		}
		else {
			// Wait the 0 to 1 transition
			od_6040_reg = object_dictionary[OD_6040_IDX]->data;			
		}
		return;
	}
	od_6040_reg = object_dictionary[OD_6040_IDX]->data;

	current_encoder_position = (int) object_dictionary[OD_6064_IDX]->data;

	if (!target_ok) {

		if (current_encoder_position > (int)object_dictionary[OD_607D_02_IDX]->data) {
			current_encoder_position = (int)object_dictionary[OD_607D_02_IDX]->data;
			target_ok = true;
			object_dictionary[OD_6040_IDX]->data |= 0x400; // Bit 10: Target Ok
			object_dictionary[OD_6040_IDX]->data |= 0x800; // Bit 11: Limits
		}else if (current_encoder_position < (int)object_dictionary[OD_607D_01_IDX]->data) {
			current_encoder_position = (int)object_dictionary[OD_607D_01_IDX]->data;
			target_ok = true;
			object_dictionary[OD_6040_IDX]->data |= 0x400; // Bit 10: Target Ok
			object_dictionary[OD_6040_IDX]->data |= 0x800; // Bit 11: Limits

		}else if (current_encoder_position == target_position)
		{
			// Target detected!
			target_ok = true;
			object_dictionary[OD_6040_IDX]->data |= 0x400; // Bit 10: Target Ok
			object_dictionary[OD_6040_IDX]->data |= 0x1000; // Bit 12: Set Point

		}
		else {
			if (increment_direction) {
				current_encoder_position += delta_count;
				if (current_encoder_position > target_position) current_encoder_position = target_position;
			}
			else {
				current_encoder_position -= delta_count;
				if (current_encoder_position < target_position) current_encoder_position = target_position;
			}
		}


		object_dictionary[OD_6064_IDX]->data = (unsigned int)current_encoder_position;
		return;
	}

	// Target reached
	profile_positioning_init = true;
}

void Motors::profile_homing_management(void) {
	

	if (profile_homing_init) {		
		// Initializes the 6040 register:
		profile_started = false;
		target_ok = false;
		profile_homing_init = false;
	}


	if (!profile_started) {
		if (object_dictionary[OD_6040_IDX]->data & 0x10)  {
			profile_started = true;

			// Initializes the 6040 register		
			object_dictionary[OD_6040_IDX]->data |= 0x400; // Bit 10
			object_dictionary[OD_6040_IDX]->data &= ~0x1000; // Bit 12
			object_dictionary[OD_6040_IDX]->data &= ~0x2000; // Bit 13

			// Zero setting done
			current_encoder_position = (int) object_dictionary[OD_607C_IDX]->data;
			object_dictionary[OD_6064_IDX]->data = (unsigned int) current_encoder_position;
			object_dictionary[OD_6040_IDX]->data |= 0x400; // Bit 10
			object_dictionary[OD_6040_IDX]->data |= 0x1000; // Bit 12
			object_dictionary[OD_6040_IDX]->data &= ~0x2000; // Bit 13
		}		
		
	}
	
	if (!target_ok) {
		if (object_dictionary[OD_6040_IDX]->data & 0x10) return;
		target_ok = true;
	}
	
	
	// Homing completed
	profile_homing_init = true;
	return;
}