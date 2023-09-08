#pragma once

namespace CANOPEN {

	#define SPEED_DENOMINATOR       120

	ref class Register {
	public:

		Register() {
			index = 0;
			subindex = 0;
			data_dim = 0;
			data = 0;
			valid = false;			
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
		Register(unsigned short idx, unsigned char sub, unsigned char dim, unsigned int dt) {
			index = idx;
			subindex = sub;
			data_dim = dim;
			data = dt;
			valid = false;
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
		Register(unsigned short idx, unsigned char sub, unsigned char dim) {
			index = idx;
			subindex = sub;
			data_dim = dim;
			valid = false;
		}

		bool getReadBuffer(unsigned char* buffer) {
			if (buffer == nullptr) return false;

			buffer[0] = Register::RDCMD;
			buffer[1] = (unsigned char) index & 0xFF;
			buffer[2] = (unsigned char) (index >> 8) & 0xFF;
			buffer[3] = subindex;

			buffer[4] = 0;
			buffer[5] = 0;
			buffer[6] = 0;
			buffer[7] = 0;
			return true;

		}

		bool getWriteBuffer(unsigned char* buffer) {
			if (buffer == nullptr) return false;
			
			buffer[0] = Register::WRCMD | data_dim;
			buffer[1] = (unsigned char)index & 0xFF;
			buffer[2] = (unsigned char)(index >> 8) & 0xFF;
			buffer[3] = subindex;

			unsigned int ival = data;
			buffer[4] = (unsigned char)(ival & 0xFF); ival = ival >> 8;
			buffer[5] = (unsigned char)(ival & 0xFF); ival = ival >> 8;
			buffer[6] = (unsigned char)(ival & 0xFF); ival = ival >> 8;
			buffer[7] = (unsigned char)(ival & 0xFF);

			return true;

		}

		inline unsigned char  getCmd(unsigned char* frame) { return frame[0] & 0xF0; }
	    inline unsigned char  getDataDim(unsigned char* frame) { return frame[0] & 0x0F;}
		inline unsigned short getIdx(unsigned char* frame) { return frame[1] + 256 * frame[2]; }
		inline unsigned char  getSub(unsigned char* frame) { return frame[3] ; }
		inline unsigned int  getVal(unsigned char* frame) { 

			switch (frame[0] & 0x0F) {
			case R1B: 
				return (unsigned int) frame[4];
			case R2B: 
				return ((unsigned int) frame[4] + 0x100 * (unsigned int) frame[5]);
			case R3B: 				
				return (unsigned int) frame[4] + 0x100 * (unsigned int) frame[5] + 0x10000 * (unsigned int) frame[6];
			case R4B: 
				return (unsigned int)frame[4] + 0x100 * (unsigned int)frame[5] + 0x10000 * (unsigned int) frame[6] + 0x1000000 * (unsigned int) frame[7];
			}
			return 0;
		}

		
		bool validateSdo(unsigned char* frame) {

			if (frame == nullptr) {
				Debug::WriteLine("NULL");
				return false;
			}
		
			if (Register::getIdx(frame) != index) {
				Debug::WriteLine("INVALID INDEX");
				return false;
			}
			if (Register::getSub(frame) != subindex) {
				Debug::WriteLine("INVALID SUB-INDEX");
				return false;
			}

			if (cmd == RDCMD)  {
				if (getCmd(frame) != RDANSW) {	

					String^ stringa = " ";
					for (int i = 0; i < 8; i++) stringa += Convert::ToString(frame[i]) + " ";
					Debug::WriteLine(stringa);
					return false;
				}
				if (Register::getDataDim(frame) != data_dim) {
					Debug::WriteLine("INVALID DATADIM");
					return false;
				}
				
			}
			else {
				if (getCmd(frame) != WRANSW) {
					Debug::WriteLine("WRANSW");
					return false;
				}
			}

			// The data is valid: fills the read SDO content into the register			
			data = Register::getVal(frame);
			valid = true;
			return true;
		}


		static const unsigned char WRCMD = 0x20;
		static const unsigned char WRANSW = 0x60;
		static const unsigned char RDCMD = 0x40;
		static const unsigned char RDANSW = 0x40;
		static const unsigned char ERRACK = 0x80;

		static const unsigned char R1B = 0x0F;
		static const unsigned char R2B = 0x0B;
		static const unsigned char R3B = 0x07;
		static const unsigned char R4B = 0x03;

		
		unsigned char cmd;
		unsigned char data_dim;
		unsigned short index;
		unsigned char subindex;
		unsigned int  data;			//!< 4 Bytes data		
		bool valid;
		
	};
	

	ref class CanOpenMotor : public System::Windows::Forms::Form
	{
	public:
		
		enum class  status_options {
			MOTOR_NOT_CONNECTED = 0,
			MOTOR_CONFIGURATION,
			MOTOR_READY,
			MOTOR_BUSY,
			MOTOR_FAULT
		};
		static const array<String^>^ status_tags = gcnew array<String^> { "NOT CONNECTED", "CONFIGURATION", "READY", "BUSY", "FAULT"};
		private:status_options internal_status;
	public: inline String^ getInternalStatusStr(void) { return status_tags[(int)internal_status]; }
	public: inline status_options getInternalStatus(void) { return internal_status; }


	public:
		CanOpenMotor(unsigned char devid, LPCWSTR motorname, double gear);

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
		
		void thread_canopen_bootup_callback(unsigned short canid, unsigned char* data, unsigned char len);

		
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
		virtual bool initializeObjectDictionary(void);
		bool od_initialized;

	private:
		
		bool blocking_writeOD(unsigned short index, unsigned char sub, unsigned char dim, int val);
		bool blocking_readOD(unsigned short index, unsigned char sub, unsigned char dim);
		
		enum class _CiA402Status {
			CiA402_NotReadyToSwitchOn = 0,
			CiA402_SwitchOnDisabled,
			CiA402_ReadyToSwitchOn,
			CiA402_SwitchedOn,
			CiA402_OperationEnabled,
			CiA402_QuickStopActive,
			CiA402_FaultReactionActive,
			CiA402_Fault,
			CiA402_Undefined,
		};

		_CiA402Status CiA_current_status;     //!< CiA current detected status
		String^ getErrorClass1001(unsigned int val);
		String^ getErrorClass1003(unsigned int val);
		String^ getErrorCode1003(unsigned int val);

		_CiA402Status getCiAStatus(int regval);
		String^ getCiAStatusString(_CiA402Status status);
		void CiA402_QuickStopActiveCallback(void);
		void CiA402_SwitchOnDisabledCallback(void);
		void CiA402_ReadyToSwitchOnCallback(void);
		void CiA402_SwitchedOnCallback(void);
		void CiA402_OperationEnabledCallback(void);
		void CiA402_FaultCallback(void);

		Thread^ main_thread;
		void mainWorker(void);
		

		HWND hwnd;
		unsigned short device_id;
		

		HANDLE rxSDOEvent; //!< Event object signaled by the SDO receiving callback
		bool sdo_rx_pending; //!< A SDO reception fdata is pending 
		Register^ rxSdoRegister; //!< SDO receiving data


		double gear_ratio; //!< This is the ratio from the motor axe and the load axe
		
		bool error_condition;
		unsigned int error_class;
		unsigned int error_code;

		bool debug;
	};

}; 