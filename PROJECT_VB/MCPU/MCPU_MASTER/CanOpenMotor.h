#pragma once

namespace CANOPEN {

	#define SPEED_DENOMINATOR       120

	ref class Register {
	public:

		Register() {
			index = 0;
			data = 0;
			valid = false;
			default = false;
		}

		/// <summary>
		/// Instance of a initialized register
		/// 
		/// This register is initialized at the connection 
		/// with the default value passed as argument.
		/// 
		/// </summary>
		/// <param name="index">index of the register</param>
		/// <param name="subidx">sub index of the register</param>
		/// <param name="dim">dimension of the data</param>
		/// <param name="dt">default value</param>
		Register(unsigned short idx, unsigned char subidx, unsigned char dim, int dt) {
			index = idx;
			data = dt;
			valid = false;
			default = true;
		}

		/// <summary>
		/// Instance of a non initialized register
		/// 
		/// This register is not initialized at the connection. 
		/// 
		/// </summary>
		/// <param name="index">index of the register</param>
		/// <param name="subidx">sub index of the register</param>
		/// <param name="dim">dimension of the data</param>
		Register(unsigned short idx, unsigned char subidx, unsigned char dim) {
			index = idx;
			valid = false;
			default = false;
		}

		void update(unsigned char* frame, unsigned char len) {
			if (len < 8) {
				index = 0;
				data = 0;
				valid = false;
				default = false;
				return;
			}

			cmd = frame[0] & 0xF0;
			data_dim = frame[0] & 0x0F;
			index = frame[1] + 256 * frame[2];
			subindex = frame[3];
			data = getVal(frame);
			valid = false;
			default = false;
		}


		static inline unsigned short getIdx(unsigned char* frame) { return frame[1] + 256 * frame[2]; }
		static inline unsigned char  getSub(unsigned char* frame) { return frame[3] ; }
		static inline			int  getVal(unsigned char* frame) { 
			switch (frame[0] & 0x0F) {
			case R1B: return (int) frame[4];
			case R2B: return (int) frame[4] + 256 * (int) frame[5];
			case R3B: return (int)frame[4] + 0xFF * (int) frame[5] + 0xFFFF * (int) frame[6];
			case R4B: 
			case 0:return (int)frame[4] + 0xFF * (int)frame[5] + 0xFFFF * (int)frame[6] + +0xFFFFFF * (int)frame[7];
			}
			return 0;
		}



		

		static const unsigned char WRCMD = 0x20;
		static const unsigned char RDCMD = 0x40;
		static const unsigned char ERRACK = 0x80;

		static const unsigned char R1B = 0x0F;
		static const unsigned char R2B = 0x0B;
		static const unsigned char R3B = 0x07;
		static const unsigned char R4B = 0x03;

		
		unsigned char cmd;
		unsigned char data_dim;
		unsigned short index;
		unsigned char subindex;
		int  data;			//!< 4 Bytes data
		int  default_data;	//!< 4 Bytes data
		bool valid;
		bool default;
	};


	ref class CanOpenMotor : public System::Windows::Forms::Form
	{
	public:
		CanOpenMotor(unsigned char devid, double gear);

		/// <summary>
		/// This is the callback to be connected to the CANOPEN SDO reception event.
		/// </summary>
		/// 
		/// This callback is called in a different thread than this module:
		/// prrecutions shall be implemented in order to be thread safe.
		/// 
		/// <param name="canid">This is the canId identifier address</param>
		/// <param name="data">Thi is the pointer to the received data frame (8 bytes max) </param>
		/// <param name="len">This is the lenght of the buffer</param>
		void thread_canopen_rx_sdo_callback(unsigned short canid, unsigned char* data, unsigned char len);
		
		/// <summary>
		/// This function converts a speed from c_grad/s  to rotation/min,
		/// where c_grad: is expressed in 0.01° unit;
		/// 
		/// </summary>
		/// <param name="x">is the speed in c_grad/s</param>
		/// <returns></returns>
		int convert_cGRADsec_TO_ROT_min(int x);

		/// <summary>
		/// This function converts a position from c_grad/s to motor encoder units,
		/// where c_grad: is expressed in 0.01° unit;
		/// 
		/// </summary>
		/// <param name="x"></param>
		/// <returns>is the position in 0.01°unit</returns>
		int convert_cGRAD_TO_EncoderUnit(int x);

		/// <summary>
		/// This function converts a position from encoder units to c_grad/s,
		/// where c_grad: is expressed in 0.01° unit;
		/// 
		/// </summary>
		/// <param name="x"></param>
		/// <returnsmotor encoder units</returns>
		int convert_EncoderUnit_TO_cGRAD(int x);


	protected: 
		
		/// <summary>
		/// This is the callback to be connected to the CANOPEN SDO reception event.
		/// </summary>
		/// <param name=""></param>
		virtual void createObjectDictionary(void) ;
		
		/// <summary>
		/// This add a register in the object dictionary that shall be initialized
		/// at the startup.
		/// 
		/// </summary>
		/// <param name="idx">register index</param>
		/// <param name="sub">register sub-index</param>
		/// <param name="dim">register register data dimension </param>
		/// <param name="val">initialization value</param>
		void DECLARE_INIT_REGISTER(unsigned short idx, unsigned char sub, unsigned char dim, int val) { object_dictionary->Add(gcnew Register(idx, sub, dim, val)); }
		

		/// <summary>
		/// This add a register in the object dictionary that shall not be initialized
		/// at the startup.
		/// 
		/// </summary>
		/// <param name="idx">register index</param>
		/// <param name="sub">register sub-index</param>
		/// <param name="dim">register register data dimension </param>
		void DECLARE_NOINIT_REGISTER(unsigned short idx, unsigned char sub, unsigned char dim) { object_dictionary->Add(gcnew Register(idx, sub, dim)); }

	private:
		/// <summary>
		/// This function creates the list of working object dictionaries
		/// that shall not initialized and shall be used to control the 
		/// motor activation.
		/// 
		/// </summary>
		/// <param name=""></param>
		void createWorkingDictionary(void);

		 
		/// <summary>
		/// This function returns the index of the register with the index and sub-index 
		/// passed as arguments.
		/// 
		/// </summary>
		/// <param name="index">Register Index Address</param>
		/// <param name="sub">Register Sub-Index </param>
		/// <returns>
		/// + The index of register in the Object Dictionary array
		/// + -1 in case of register not present </returns>
		int findOd(unsigned short index, unsigned short sub);

		bool blocking_writeOD(unsigned short index, unsigned char sub, unsigned char dim, int val);
		bool blocking_readOD(unsigned short index, unsigned char sub, unsigned char dim);

	private:
		void startupThread(void);
		HWND hwnd;
		unsigned short device_id;
		Thread^ startup_thread;

		double gear_ratio;
		List<Register^>^ object_dictionary;
		HANDLE rxEvent;
		
		unsigned short wait_index;
		unsigned char wait_subindex;
		Register^ rxRegister;

	};

};