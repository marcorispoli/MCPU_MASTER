#include "deviceInterface.h"
#include "canInterface.h"
#include <thread>
#include "mutex"

deviceInterface::deviceInterface(unsigned short deviceid,unsigned int revision, unsigned char status, unsigned char data, unsigned char parameters) {

	devId = deviceid;
	active = false;
	operating = false;

	num_status = status;
	num_data = data;
	num_parameters = parameters;

	device_reset_request = false;
	
	command_register = gcnew Register();
	error_register = gcnew Register();
	revision_register = gcnew Register(revision);

	// Register creation
	status_registers = gcnew cli::array<Register^>(num_status);
	for (int i = 0; i < num_status; i++) status_registers[i] = gcnew Register();
	
	data_registers = gcnew cli::array<Register^>(num_data);
	for (int i = 0; i < num_data; i++) data_registers[i] = gcnew Register();

	parameter_registers = gcnew cli::array<Register^>(num_parameters);
	for (int i = 0; i < num_parameters; i++) parameter_registers[i] = gcnew Register();

	// connect the reception handler
	canInterface::canrx_device_event += gcnew canInterface::rxData_slot(this, &deviceInterface::canrx_device_event);
	canInterface::connection_event += gcnew canInterface::connection_slot(this, &deviceInterface::handle_connection);

	// Start the reception thread
	running_thread = gcnew Thread(gcnew ThreadStart(this, &deviceInterface::threadWork));
	running_thread->Name = "Device Interface thread";
	running_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
	running_thread->Start();

}

void deviceInterface::threadWork(void) {
	
	device_reset_callback();
	
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(_DEVICE_TASK_EXECUTION_DELAY_ms));
		device_workflow_callback();
	}
}

void deviceInterface::handle_connection(bool stat) {

}

void deviceInterface::canrx_device_event(void) {

	// Se non è ancora operativo non risponde
	if (!operating) return;

	// Bootloader / normal frame detection
	if (canInterface::canId == 0x140 + devId) frame_bootloader = false;
	else if (canInterface::canId == 0x100 + devId) frame_bootloader = true;
	else return;

	if (canInterface::datalen != 8) return; // Only 8 byte admitted


	// Data decoding
	frame_seq = canInterface::canDataBuffer[0];
	frame_cmd = canInterface::canDataBuffer[1];
	frame_index = canInterface::canDataBuffer[2];
	frame_d0 = canInterface::canDataBuffer[3];
	frame_d1 = canInterface::canDataBuffer[4];
	frame_d2 = canInterface::canDataBuffer[5];
	frame_d3 = canInterface::canDataBuffer[6];
	frame_crc = canInterface::canDataBuffer[7];
	

	if (frame_bootloader) handle_bootloader_frame();
	else handle_normal_frame();
}

void deviceInterface::handle_bootloader_frame(void){
	cli::array<Byte>^ buffer = gcnew cli::array<Byte>(8);

	buffer[0] = frame_seq;
	if (frame_seq == (Byte)ProtocolBootloaderCode::BOOTLOADER_GET_INFO) {
		buffer[1] = 2; // Bootloader present but not running
		buffer[2] = 1;
		buffer[3] = 0;
		buffer[4] = 0;
		buffer[5] = (Byte) revision_register->d0; // App maj
		buffer[6] = (Byte) revision_register->d1; // App min
		buffer[7] = (Byte) revision_register->d2; // App sub
		canInterface::send(devId + 0x100, 8, buffer);
	}
}

void deviceInterface::handle_normal_frame(void) {
	//unsigned char canId, unsigned char dlen, cli::array<Byte>^ buffer
	cli::array<Byte>^ buffer = gcnew cli::array<Byte>(8);

	// Handles the device reset event
	if (device_reset_request) {
		device_reset_request = false;
		buffer[0] = (Byte)ProtocolFrameCode::FRAME_DEVICE_RESET;
		unsigned char crc = 0;
		for (int i = 0; i < 7; i++) crc ^= buffer[i];
		buffer[7] = crc;
		canInterface::send(devId + 0x140, 8, buffer);
		return;
	}

	buffer[0] = frame_seq;
	buffer[1] = frame_cmd;
	buffer[2] = frame_index;

	switch(frame_cmd) {
	
	case (Byte)ProtocolFrameCode::FRAME_READ_REVISION:
		buffer[3] = (Byte)(revision_register->d0);
		buffer[4] = (Byte)(revision_register->d1);
		buffer[5] = (Byte)(revision_register->d2);
		buffer[6] = (Byte)(revision_register->d3);
		break;

	case (Byte) ProtocolFrameCode::FRAME_READ_ERRORS:
		buffer[3] = (Byte)(error_register->d0);
		buffer[4] = (Byte)(error_register->d1);
		buffer[5] = (Byte)(error_register->d2);
		buffer[6] = (Byte)(error_register->d3);
		break;

	case (Byte)ProtocolFrameCode::FRAME_READ_COMMAND:
		buffer[3] = (Byte)(command_register->d0);
		buffer[4] = (Byte)(command_register->d1);
		buffer[5] = (Byte)(command_register->d2);
		buffer[6] = (Byte)(command_register->d3);
		break;

	case (Byte)ProtocolFrameCode::FRAME_READ_STATUS:
		if (frame_index >= num_status) {
			buffer[3] = 0;
			buffer[4] = frame_cmd;
			buffer[5] = (Byte)CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM;
			buffer[6] = 0;
		}
		else {
			buffer[3] = (Byte)(status_registers[frame_index]->d0);
			buffer[4] = (Byte)(status_registers[frame_index]->d1);
			buffer[5] = (Byte)(status_registers[frame_index]->d2);
			buffer[6] = (Byte)(status_registers[frame_index]->d3);
		}
		break;

	case (Byte)ProtocolFrameCode::FRAME_READ_DATA:
		if (frame_index >= num_data) {
			buffer[3] = 0;
			buffer[4] = frame_cmd;
			buffer[5] = (Byte)CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM;
			buffer[6] = 0;
		}
		else {
			buffer[3] = (Byte)(data_registers[frame_index]->d0);
			buffer[4] = (Byte)(data_registers[frame_index]->d1);
			buffer[5] = (Byte)(data_registers[frame_index]->d2);
			buffer[6] = (Byte)(data_registers[frame_index]->d3);
		}
		break;

	case (Byte)ProtocolFrameCode::FRAME_WRITE_DATA:
		if (frame_index >= num_data) {
			buffer[3] = 0;
			buffer[4] = frame_cmd;
			buffer[5] = (Byte)CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM;
			buffer[6] = 0;
		}
		else {
			
			data_registers[frame_index]->d0 = (unsigned int)frame_d0;
			data_registers[frame_index]->d1 = (unsigned int)frame_d1;
			data_registers[frame_index]->d2 = (unsigned int)frame_d2;
			data_registers[frame_index]->d3 = (unsigned int)frame_d3;
			
			buffer[3] = (Byte)(data_registers[frame_index]->d0);
			buffer[4] = (Byte)(data_registers[frame_index]->d0);
			buffer[5] = (Byte)(data_registers[frame_index]->d2);
			buffer[6] = (Byte)(data_registers[frame_index]->d3);
		}
		break;

	case (Byte)ProtocolFrameCode::FRAME_READ_PARAM:
		if (frame_index >= num_parameters) {
			buffer[3] = 0;
			buffer[4] = frame_cmd;
			buffer[5] = (Byte)CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM;
			buffer[6] = 0;
		}
		else {
			buffer[3] = (Byte)(parameter_registers[frame_index]->d0);
			buffer[4] = (Byte)(parameter_registers[frame_index]->d1);
			buffer[5] = (Byte)(parameter_registers[frame_index]->d2);
			buffer[6] = (Byte)(parameter_registers[frame_index]->d3);
		}
		break;

	
	case (Byte)ProtocolFrameCode::FRAME_WRITE_PARAM:
		if (frame_index >= num_parameters) {
			buffer[3] = 0;
			buffer[4] = frame_cmd;
			buffer[5] = (Byte)CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM;
			buffer[6] = 0;
		}
		else {
			
			parameter_registers[frame_index]->d0 = (unsigned int)frame_d0;
			parameter_registers[frame_index]->d1 = (unsigned int)frame_d1;
			parameter_registers[frame_index]->d2 = (unsigned int)frame_d2;
			parameter_registers[frame_index]->d3 = (unsigned int)frame_d3;
			
			buffer[3] = (Byte)(parameter_registers[frame_index]->d0);
			buffer[4] = (Byte)(parameter_registers[frame_index]->d1);
			buffer[5] = (Byte)(parameter_registers[frame_index]->d2);
			buffer[6] = (Byte)(parameter_registers[frame_index]->d3);
		}
		break;

	case (Byte)ProtocolFrameCode::FRAME_STORE_PARAMS:
		if (frame_index >= num_parameters) {
			buffer[3] = 0;
			buffer[4] = frame_cmd;
			buffer[5] = (Byte)CommandRegisterErrors::COMMAND_ERROR_INVALID_PARAM;
			buffer[6] = 0;
		}
		else {

			buffer[3] = (Byte)(parameter_registers[frame_index]->d0);
			buffer[4] = (Byte)(parameter_registers[frame_index]->d1);
			buffer[5] = (Byte)(parameter_registers[frame_index]->d2);
			buffer[6] = (Byte)(parameter_registers[frame_index]->d3);
		}
		break;

	case (Byte)ProtocolFrameCode::FRAME_DEVICE_RESET:
		device_reset_request = true;
		device_reset_callback();
		return; // no answer

	case (Byte)ProtocolFrameCode::FRAME_COMMAND_EXEC:
		
		// Busy condition
		if ((command_executing) && (frame_index != 0)) {
			// The command_register shall not be modified!	
			buffer[3] = (Byte)(CommandReturnStatus::COMMAND_ERROR);
			buffer[4] = (Byte)(0);
			buffer[5] = (Byte)(0);
			buffer[6] = (Byte)(CommandRegisterErrors::COMMAND_ERROR_BUSY);
			break;
		}

		commandResult^ result = device_command_callback(frame_index, frame_d0, frame_d1, frame_d2, frame_d3);
		command_register = gcnew Register((Byte)result->status, (Byte)result->d0, (Byte)result->d1, (Byte)result->error);
		buffer[3] = (Byte)(result->status);
		buffer[4] = (Byte)(result->d0);
		buffer[5] = (Byte)(result->d1);
		buffer[6] = (Byte)(result->error);
		
		if (result->status == CommandReturnStatus::COMMAND_PROCESSING) command_executing = true;
		break;

	}

	// Sends the frame 
	unsigned char crc = 0;
	for (int i = 0; i < 7; i++) crc ^= buffer[i];
	buffer[7] = crc;
	canInterface::send(devId + 0x140, 8, buffer);
	return;
}

