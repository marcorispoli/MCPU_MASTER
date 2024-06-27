#pragma once
#include "canInterface.h"

using namespace System;


ref class deviceInterface
{
public:
	bool active;

	enum class CommandRegisterErrors {
		COMMAND_NO_ERROR = 0,		//!< Protocol Reserved code for the command successfully completed 
		COMMAND_ERROR_BUSY,			//!< Protocol Reserved code for the command busy
		COMMAND_ERROR_INVALID_PARAM,//!< Protocol Reserved code for the command with invalid parameter
		COMMAND_ERROR_MOMENTARY_DISABLED,//!< Protocol Reserved code for the command momentary disabled

		COMMAND_INVALID_DEVICE = 253,		//!< Command requested from an invalid thread
		COMMAND_COMMUNICATION_ERROR = 254,//!< Command failed due to communication error
		COMMAND_DEVICE_TMO = 255		//!< Command execution timeout
	};

	enum class CommandReturnStatus {
		COMMAND_RESERVED = 0,
		COMMAND_PROCESSING,
		COMMAND_SUCCESS,
		COMMAND_ERROR,
	};

	ref class commandResult {
	public:
		commandResult(CommandRegisterErrors err) {
			error = err;
			status = CommandReturnStatus::COMMAND_ERROR;
			d0 = 0;
			d1 = 0;
		}
		
		commandResult(Byte R0, Byte R1) {
			error = CommandRegisterErrors::COMMAND_NO_ERROR;
			status = CommandReturnStatus::COMMAND_SUCCESS;
			d0 = R0;
			d1 = R1;
		}

		commandResult(void) {
			error = CommandRegisterErrors::COMMAND_NO_ERROR;
			status = CommandReturnStatus::COMMAND_PROCESSING;
			d0 = 0;
			d1 = 0;
		}

		CommandReturnStatus status;
		Byte d0;
		Byte d1;
		CommandRegisterErrors error;
	};

	enum class ProtocolBootloaderCode {
		BOOTLOADER_RESERVED = 0,
		BOOTLOADER_GET_INFO,
		BOOTLOADER_START,
		BOOTLOADER_EXIT,
	};
	
	enum class ProtocolFrameCode {
	FRAME_ERROR = 0,	// Device detected an invalid access to the registers
	FRAME_READ_REVISION,//!< Read Revision register frame command code
	FRAME_READ_ERRORS,//!< Read Error register frame command code
	FRAME_READ_COMMAND,//!< Read Command register frame command code
	FRAME_READ_STATUS,//!< Read Status register frame command code
	FRAME_READ_DATA,//!< Read Data register frame command code
	FRAME_READ_PARAM,//!< Read Parameter register frame command code
	FRAME_WRITE_DATA,//!< Write Data register frame command code
	FRAME_WRITE_PARAM,//!< Write Parameter register frame command code
	FRAME_STORE_PARAMS,//!< Store Parameters register frame command code
	FRAME_COMMAND_EXEC,//!< Execute Command frame command code
	FRAME_DEVICE_RESET,//!< Device Reset Code
	};

	deviceInterface(unsigned short devid, unsigned int revision, unsigned char status, unsigned char data, unsigned char parameters);
		

	static unsigned int setRegVal(Byte d0, Byte d1, Byte d2, Byte d3) {
		return (unsigned int)d0 + (((unsigned int)d1) << 8) + (((unsigned int)d2) << 16) + (((unsigned int)d3) << 24);
	}
	static unsigned int setRegD0(unsigned int reg, Byte d) {return (reg & 0xFFFFFF00) + (unsigned int) d;}
	static unsigned int setRegD1(unsigned int reg, Byte d) { return (reg & 0xFFFF00FF) + ((unsigned int) d >> 8); }
	static unsigned int setRegD2(unsigned int reg, Byte d) { return (reg & 0xFF00FFFF) + ((unsigned int)d >> 16); }
	static unsigned int setRegD3(unsigned int reg, Byte d) { return (reg & 0x00FFFFFF) + ((unsigned int)d >> 24); }

	static  unsigned int setRegBit(unsigned int val, unsigned int mask, bool stat ) { 
		if (stat) {
			return (val |= mask);
		}
		else {
			return (val &=~mask);
		}		
	}

	static  unsigned char getRegD0(unsigned int val) {return (unsigned char)val & 0xFF;}
	static  unsigned char getRegD1(unsigned int val) { return (unsigned char) ((val >> 8) & 0xFF); }
	static  unsigned char getRegD2(unsigned int val) { return (unsigned char)((val >> 16) & 0xFF); }
	static  unsigned char getRegD3(unsigned int val) { return (unsigned char)((val >> 24) & 0xFF); }


protected:
	virtual void device_reset_callback(void) {
	}

	virtual void device_workflow_callback(void) {
	}

	virtual commandResult^ device_command_callback(unsigned char cmd, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3) {
		commandResult^ result = gcnew commandResult(deviceInterface::CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED);
		command_register = deviceInterface::setRegVal((Byte)cmd, (Byte)result->status, (Byte)result->d0, (Byte)result->error);
		return result;
	}

	// Register section
	unsigned int command_register;
	unsigned int error_register;
	unsigned int revision_register;


	cli::array<unsigned int>^ status_registers;
	cli::array<unsigned int>^ data_registers;
	cli::array<unsigned int>^ parameter_registers;


private:
	void threadWork(void);//!< This is the worker thread for the connection management	
	Thread^ running_thread;//!< This is the worker thread handler
	
	unsigned short devId;
	unsigned char  num_status;
	unsigned char  num_data;
	unsigned char  num_parameters;

	
	void canrx_device_event(void);
	void handle_connection(bool stat);
	void handle_bootloader_frame(void);
	void handle_normal_frame(void);

	// Received frame data content
	bool frame_bootloader;
	unsigned char frame_seq;
	unsigned char frame_cmd;
	unsigned char frame_index;
	unsigned char frame_d0;
	unsigned char frame_d1;
	unsigned char frame_d2;
	unsigned char frame_d3;
	unsigned char frame_crc;
	bool device_reset_request;
	
	
};

