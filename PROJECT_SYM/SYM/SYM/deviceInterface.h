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

public:ref class CanDeviceCommand {
public:

	CanDeviceCommand(unsigned char cd) {
		code = cd;
		d0 = d1 = d2 = d3 = 0;
	}
	CanDeviceCommand(unsigned char cd, unsigned char id0, unsigned char id1, unsigned char id2, unsigned char id3) {
		code = cd;
		d0 = id0;
		d1 = id1;
		d2 = id2;
		d3 = id3;
	}

	unsigned char code;
	unsigned char d0, d1, d2, d3;
};

	  /// <summary>
	  /// This class provides a data structure to handle a command result.
	  /// 
	  /// \ingroup commanControl
	  /// </summary>
	public:ref class CanDeviceCommandResult {
	public:

		/// <summary>
		/// Creates a result structure assigning all the internal data.
		/// </summary>
		/// <param name="err">The error code</param>
		/// <param name="ris0">The result data 0</param>
		/// <param name="ris1">The result data 1</param>
		CanDeviceCommandResult(CommandRegisterErrors err, unsigned char ris0, unsigned char ris1) {
			error_code = err;
			result0 = ris0;
			result1 = ris1;
		}

		/// <summary>
		/// Creates a result structure assigning only the error code.
		/// </summary>
		/// <param name="err">The error code</param>		
		CanDeviceCommandResult(CommandRegisterErrors err) {
			error_code = err;
			result0 = 0;
			result1 = 0;
		}

		/// <summary>
		/// Creates a result structure assigning the NO ERROR code to the error field.
		/// </summary>
		/// <param name=""></param>
		CanDeviceCommandResult(void) {
			error_code = CommandRegisterErrors::COMMAND_NO_ERROR;
			result0 = 0;
			result1 = 0;
		}

		CommandRegisterErrors error_code; //|< Command completed code
		unsigned char result0; //!< Result D0 code 
		unsigned char result1;	//!< Result D1 code 
	};

	public:ref class Register {
	public:
		enum class exceptions {
			INDEX_OUT_OF_RANGE = 1,
			NULLPTR_REGISTER = 2,
		};

		// Used to detect an exception
		ref struct regException : public System::Exception {
		public:
			regException(exceptions val) { code = val; }
			exceptions code;
		};

		/// <summary>
		/// Creates a register assigning all the internal data
		/// </summary>
		/// <param name="pd0"></param>
		/// <param name="pd1"></param>
		/// <param name="pd2"></param>
		/// <param name="pd3"></param>
		Register(unsigned char pd0, unsigned char pd1, unsigned char pd2, unsigned char pd3) {
			d0 = pd0; d1 = pd1; d2 = pd2; d3 = pd3;
		}

		Register(unsigned int val) {

			d0 = (Byte)(val & 0xFF);
			d1 = (Byte)((val>>8) & 0xFF);
			d2 = (Byte)((val >> 16) & 0xFF);
			d3 = (Byte)((val >> 24) & 0xFF);
		}

		/// <summary>
		/// Creates a Register with all the internal data set to 0.
		/// </summary>
		/// <param name=""></param>
		Register(void) {
			d0 = d1 = d2 = d3 = 0;
		}

		unsigned char d0; //!< Internal D0 data
		unsigned char d1; //!< Internal D1 data
		unsigned char d2; //!< Internal D2 data
		unsigned char d3; //!< Internal D3 data

		/// <summary>
		/// This method assignes a bitfield value with a bit-mask to  the internal data d0.
		/// </summary>
		/// <param name="stat"></param>
		/// <param name="mask"></param>
		void D0(bool stat, unsigned char mask) {
			if (stat) d0 |= mask;
			else d0 &= ~mask;
		}

		/// <summary>
		/// This method assignes a bitfield value with a bit-mask to  the internal data d1.
		/// </summary>
		/// <param name="stat">this is a byte value</param>
		/// <param name="mask">this is a byte mask</param>
		void D1(bool stat, unsigned char mask) {
			if (stat) d1 |= mask;
			else d1 &= ~mask;
		}

		/// <summary>
		/// This method assignes a bitfield value with a bit-mask to  the internal data d2.
		/// </summary>
		/// <param name="stat">this is a byte value</param>
		/// <param name="mask">this is a byte mask</param>
		void D2(bool stat, unsigned char mask) {
			if (stat) d2 |= mask;
			else d2 &= ~mask;
		}

		/// <summary>
		/// This method assignes a bitfield value with a bit-mask to  the internal data d3.
		/// </summary>
		/// <param name="stat">this is a byte value</param>
		/// <param name="mask">this is a byte mask</param>
		void D3(bool stat, unsigned char mask) {
			if (stat) d3 |= mask;
			else d3 &= ~mask;
		}

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
		command_register = gcnew Register((Byte)cmd, (Byte)result->status, (Byte)result->d0, (Byte)result->error);
		return result;
	}

	// Register section
	Register^ command_register;
	Register^ error_register;
	Register^ revision_register;


	cli::array<Register^>^ status_registers;
	cli::array<Register^>^ data_registers;
	cli::array<Register^>^ parameter_registers;


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

