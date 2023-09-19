#pragma once

using namespace System;
using namespace System::Collections::Generic;

ref class CanDeviceProtocol : public System::Windows::Forms::Form
{
public:
	
	// Bootloader commands
	#define GET_BOOTLOADER_INFO (Byte) 0x1, (Byte) 0,(Byte) 0, (Byte) 0,(Byte) 0,(Byte) 0, (Byte) 0,(Byte) 0,true


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
		LEN,
		UNDEF = LEN
	};

	enum class bootloader_options {
		BOOTLOADER_NOT_PRESENT = 0,
		BOOTLOADER_RUNNING = 1,
		BOOTLOADER_PRESENT = 2,
		BOOTLOADER_UNCKNOWN_STAT
	};
	static const array<String^>^ bootloader_status_tags = gcnew array<String^> { "NOT PRESENT", "RUNNING", "PRESENT", "UNCKNOWN"}; 

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
	inline String^ getBootRevision(void) { return boot_maj.ToString() + "." + boot_min.ToString() + "." + boot_sub.ToString(); }
	inline String^ getAppRevision(void) { return app_maj.ToString() + "." + app_min.ToString() + "." + app_sub.ToString(); }
	inline String^ getBootStatus(void) { return bootloader_status_tags[ (int) bootloader_status]; }
	
protected:
	virtual void runningLoop(void) ;
private:

	//bool blocking_write(unsigned short index, unsigned char sub, unsigned char dim, int val);
	//bool blocking_read(unsigned short index, unsigned char sub, unsigned char dim);
	
	Thread^ main_thread;
	void mainWorker(void);


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
};

