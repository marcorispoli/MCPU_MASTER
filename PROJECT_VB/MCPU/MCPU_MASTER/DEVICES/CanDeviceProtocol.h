#pragma once
#include <Windows.h>

using namespace System::Collections::Generic;
using namespace System::Threading;

ref class CanDeviceProtocol //: public System::Windows::Forms::Form
{
public:

	/// <summary>
	/// This is the command Code of the protocol frames
	/// 
	/// </summary>
	enum class ProtocolFrameCode {
		FRAME_READ_REVISION = 1,//!< Read Revision register frame command code
		FRAME_READ_ERRORS,//!< Read Error register frame command code
		FRAME_READ_COMMAND,//!< Read Command register frame command code
		FRAME_READ_STATUS,//!< Read Status register frame command code
		FRAME_READ_DATA,//!< Read Data register frame command code
		FRAME_READ_PARAM,//!< Read Parameter register frame command code
		FRAME_WRITE_DATA,//!< Write Data register frame command code
		FRAME_WRITE_PARAM,//!< Write Parameter register frame command code
		FRAME_STORE_PARAMS,//!< Store Parameters register frame command code
		FRAME_COMMAND_EXEC,//!< Execute Command frame command code
	};

	// Bootloader commands
	#define GET_BOOTLOADER_INFO (System::Byte) 0x1, (System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,true
	#define GET_COMMAND_REGISTER (System::Byte) 0, (System::Byte) 3,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,false

	enum class CommandRegisterStatus {
		COMMAND_EXECUTING = 1,
		COMMAND_TERMINATED,
		COMMAND_ERROR,
	};

	enum class CommandRegisterErrors {
		COMMAND_NO_ERROR = 0,
		COMMAND_ERROR_BUSY,
		COMMAND_ERROR_INVALID_PARAM,
		COMMAND_ERROR_MOMENTARY_DISABLED,
		COMMAND_DEVICE_TMO = 255
	};

	ref class CanDeviceRegister {
	public:

		CanDeviceRegister(void) {						
			bootloader = false;
		}

		bool decode(unsigned char* data, bool bootl) {
			bootloader = bootl;
			b0 = data[0];
			b1 = data[1];
			b2 = data[2];
			b3 = data[3];
			b4 = data[4];
			b5 = data[5];
			b6 = data[6];
			b7 = data[7];

			if (bootloader) return true;

			unsigned char crc = 0;
			for (int i = 0; i < 8; i++) crc ^= data[i];
			
			if (crc) return false;
			return true;
		}

		void format(unsigned char* data) {
			data[0] = b0;
			data[1] = b1;
			data[2] = b2;
			data[3] = b3;
			data[4] = b4;
			data[5] = b5;
			data[6] = b6;
			data[7] = b7;
			
			crc = 0;
			for (int i = 0; i < 7; i++) crc ^= data[i];

		}

		void set(unsigned char d0 , unsigned char d1, unsigned char d2, unsigned char d3, unsigned char d4, unsigned char d5, unsigned char d6, unsigned char d7, bool bootl) {
			bootloader = bootl;				
			b0 = d0;
			b1 = d1;
			b2 = d2;
			b3 = d3;
			b4 = d4;			
			b5 = d5;
			b6 = d6;
			b7 = d7;
		}

		

		unsigned char b0;
		unsigned char b1;
		unsigned char b2;
		unsigned char b3;
		unsigned char b4;
		unsigned char b5;
		unsigned char b6;
		unsigned char b7;
		unsigned char crc;
		bool bootloader;
	};

	enum class status_options {
		WAITING_CAN_DRIVER_CONNECTION = 0,	//!< The Device is waiting the Can Driver connection		
		WAITING_REVISION,					//!< The module is waiting for the revision acquisition
		DEVICE_CONFIGURATION,				//!< The module is uploading the device parameters
		DEVICE_RUNNING,						//!< The module is Running
		DEVICE_ERROR,						//!< The module is in Error condition
		LEN,
		UNDEF = LEN
	};

	enum class bootloader_options {
		BOOTLOADER_NOT_PRESENT = 0,
		BOOTLOADER_RUNNING = 1,
		BOOTLOADER_PRESENT = 2,
		BOOTLOADER_UNCKNOWN_STAT
	};
	static const cli::array<System::String^>^ bootloader_status_tags = gcnew cli::array<System::String^> { "NOT PRESENT", "RUNNING", "PRESENT", "UNCKNOWN"}; 

	CanDeviceProtocol(unsigned char devid, LPCWSTR devname);

	/// <summary>
	/// This is the callback to be connected to the CAN reception event.
	/// </summary>
	/// 
	/// This callback is called in a different thread than this module:
	/// precautions shall be implemented in order to be thread safe.
	/// 
	/// <param name="canid">This is the canId identifier address</param>
	/// <param name="data">Thi is the pointer to the received data frame (8 bytes max) </param>
	/// <param name="len">This is the lenght of the buffer</param>
	void thread_can_rx_callback(unsigned short canid, unsigned char* data, unsigned char len);

	bool send(unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, unsigned char d4, unsigned char d5, unsigned char d6, unsigned char d7, bool bootl);
	inline bool isTmo(void) { return tmo; }

	inline CanDeviceRegister^ getRxRegister(void) { return rx_register; }
	inline status_options getStatus(void) { return internal_status; }
	inline System::String^ getBootRevision(void) { return boot_maj.ToString() + "." + boot_min.ToString() + "." + boot_sub.ToString(); }
	inline System::String^ getAppRevision(void) { return app_maj.ToString() + "." + app_min.ToString() + "." + app_sub.ToString(); }
	inline System::String^ getBootStatus(void) { return bootloader_status_tags[ (int) bootloader_status]; }
	
	// Command execution section
	bool command(unsigned char code, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, int tmo);
	bool isCommandCompleted(void);
	bool isCommandError(void) { return (command_error != (unsigned char) CommandRegisterErrors::COMMAND_NO_ERROR); }
	
protected:
	virtual void runningLoop(void) ;
	virtual bool errorLoop(void);
	virtual bool configurationLoop(void);


private:

	//bool blocking_write(unsigned short index, unsigned char sub, unsigned char dim, int val);
	//bool blocking_read(unsigned short index, unsigned char sub, unsigned char dim);
	
	Thread^ main_thread;
	void mainWorker(void);
	void InternalRunningLoop(void);

	HWND hwnd;
	unsigned short device_id;

	static unsigned char rx_sequence;
	HANDLE rxEvent; //!< Event object signaled by the receiving callback
	bool rx_pending; //!< A reception data is pending 
	CanDeviceRegister^ tx_register; //!< Received register
	CanDeviceRegister^ rx_register; //!< Received register

	status_options internal_status;

	// Target device revision set
	unsigned char boot_maj;
	unsigned char boot_min;
	unsigned char boot_sub;
	unsigned char app_maj;
	unsigned char app_min;
	unsigned char app_sub;
	unsigned char bootloader_status;

	bool tmo;
	bool rxOk;
	int attempt;

	// Command execution section
	bool		  command_executing;
	int			  command_tmo; // 100ms timeout
	unsigned char command_code;
	unsigned char command_d0;
	unsigned char command_d1;
	unsigned char command_d2;
	unsigned char command_d3;
	unsigned char command_ris0;
	unsigned char command_ris1;
	unsigned char command_error;


};

