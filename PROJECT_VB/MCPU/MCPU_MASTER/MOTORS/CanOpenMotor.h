#pragma once
#include <Windows.h>

using namespace System::Diagnostics;
using namespace System::Threading;

/// <summary>
/// This is the module implementing the control 
/// for motor device based on the CANOpen protocol.
/// 
/// \defgroup CanOpenModule CanOPEN based motor devices 
/// 
/// 
/// </summary>
/// 
namespace CANOPEN {

	#define SPEED_DENOMINATOR       120 //!< This is the Speed conversion denominator unit 

	/// <summary>
	/// This is the enumeration of the Application Motor device addresses
	/// 
	/// \ingroup CanOpenModule
	/// </summary>
	enum class MotorDeviceAddresses {
		TILT_ID = 0x3,	//!< Address of the Tilt Motor controller 
		ARM_ID = 0x4,	//!< Address of the C-ARM Motor controller 
		SLIDE_ID = 0x5, //!< Address of the SLIDE Motor controller 
		BODY_ID = 0x6,	//!< Address of the BODY Motor controller 
		VERTICAL_ID = 0x7,//!< Address of the Vertical Motor controller 
	};

	/// <summary>
	/// This is the class implementating the CanOPEN SDO object register.
	/// 
	/// The CanOPEN device declares a set of the so called Object registers,
	/// a collection of internal registers that determines the workflow of the motor.
	/// 
	/// An Object register is defined by:
	/// - Index: a 2 bytes (little endian encoding) main address;
	/// - Sub-index: a 1 byte sub address;
	/// - Data dimension: 1 Byte defining the dimension of the register value;
	/// - Data value: 1 to 4 byte register value in little endian format; 
	/// 
	/// This Class implements the content of the Object Register and also 
	/// provides tools to encode/decode a SDO Can frame for reading and writing a register.
	/// 
	///  
	/// \ingroup CanOpenModule
	/// </summary>
	ref class ODRegister {
	public:

		/// <summary>
		/// Returns a string defining the Ack Error in an SDO transation.
		/// 
		/// </summary>
		/// <param name="frame"></param>
		/// <returns></returns>
		System::String^ getError(unsigned char* frame) {
			unsigned long code = frame[5] + 256 * frame[5] + 256 * 256 * frame[6] + 256 * 256 * 256 * frame[7];
			unsigned short index = frame[1] + 256 * frame[2];
			unsigned char subindex = frame[3];

			
			switch (code) {
			case 0x05030000: return "ERR: IDX:" + index.ToString()+" SUB:"+subindex.ToString()+" > Toggle bit notchanged";
			case 0x05040001: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > Command specifier unknown";
			case 0x06010000: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > Unsupported access";
			case 0x06010002: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > Read only entry";
			case 0x06020000: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > Object not existing";
			case 0x06040041: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > Object cannot be PDO mapped";
			case 0x06040042: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > Mapped Pdo exceed PDO";
			case 0x06070012: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > parameter lenght too long";
			case 0x06070013: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > parameter lenght too short";
			case 0x06090011: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > Sub index not existing";
			case 0x06090031: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > Value too great";
			case 0x06090032: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > Value too small";
			case 0x08000000: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > General error";
			case 0x08000022: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > Data cannot be read or stored in this state";
			default: return "ERR: IDX:" + index.ToString() + " SUB:" + subindex.ToString() + " > " + code.ToString();
			}
		}

		enum class SDOCommandCodes {
			WRCMD = 0x20,	//!< SDO Write command for frame to the remote device
			WRANSW = 0x60,  //!< SDO Write command ack code from the remote device
			RDCMD = 0x40,   //!< SDO Read command for frame to the remote device
			RDANSW = 0x40,	//!< SDO Read command ack code from the remote device
			ERRACK = 0x80,  //!< SDO Error ack code from the remote device
		};

		enum class SDODataDimension {
			R0B = 0,	//!< SDO null dimension (for nanoj upload procedure)
			R1B = 0x0F, //!< SDO 1 Byte data dimension
			R2B = 0x0B, //!< SDO 2 Byte data dimension
			R3B = 0x07, //!< SDO 3 Byte data dimension
			R4B = 0x03, //!< SDO 4 Byte data dimension
		};

		/// <summary>
		/// This is the constructor of a void register.
		/// 
		/// The created register doesn't have any informatioon about the 
		/// phisical target register.
		/// 
		/// All the internal parameters are cleared.
		/// The register is set as not valid.
		/// 
		/// </summary>
		ODRegister() {
			index = 0;
			subindex = 0;
			data = 0;
			valid = false;			
		}

		/// <summary>
		/// Instance of a initialized register.
		/// 
		/// This constructor creates an object register 
		/// pointingto a phisical register (with address, data data type) 
		/// and initalizes its data value.
		/// 
		/// </summary>
		/// <param name="index">index of the register</param>
		/// <param name="subidx">sub index of the register</param>
		/// <param name="dim">dimension of the data</param>
		/// <param name="dt">default value</param>
		ODRegister(unsigned short idx, unsigned char sub, SDODataDimension dim, unsigned int dt) {
			index = idx;
			subindex = sub;
			data_dim = dim;
			data = dt;
			valid = false;
		}

		/// <summary>
		/// Instance of a non initialized register
		/// 
		/// This constructor creates an object register 
		/// pointing to a phisical register (with address, data data type) 
		/// but without initializing its content.
		/// 
		/// </summary>
		/// <param name="index">index of the register</param>
		/// <param name="subidx">sub index of the register</param>
		/// <param name="dim">dimension of the data</param>
		ODRegister(unsigned short idx, unsigned char sub, SDODataDimension dim) {
			index = idx;
			subindex = sub;
			data_dim = dim;
			valid = false;
		}

		


		/// <summary>
		/// Encode a CAN buffer frame for a read command of the register 
		/// 
		/// This command is used to encode a can frame to be used to read 
		/// an Object Register from the motor device.
		/// 
		/// The register shall contains the index and Subindex. 
		/// The application can use a constructor without value initialization
		/// and then using this function to retrive the can data buffer to be used 
		/// in the communication with the remote motor device.
		/// 
		/// </summary>
		/// <param name="buffer"></param>
		/// <returns>true  if success</returns>
		bool getReadBuffer(unsigned char* buffer) {
			if (buffer == nullptr) return false;

			buffer[0] = (unsigned char) SDOCommandCodes::RDCMD;
			buffer[1] = (unsigned char) index & 0xFF;
			buffer[2] = (unsigned char) (index >> 8) & 0xFF;
			buffer[3] = subindex;

			buffer[4] = 0;
			buffer[5] = 0;
			buffer[6] = 0;
			buffer[7] = 0;
			return true;

		}

		/// <summary>
		/// Encode a CAN buffer frame for a write command of the register 
		/// 
		/// This command is used to encode a can frame to be used to write 
		/// an Object Register to the motor device.
		/// 
		/// The register shall contains the Index, Subindex, data format and data value.
		///  
		/// The application can use a constructor with value initialization
		/// and then using this function to retrive the can data buffer to be used 
		/// in the communication with the remote motor device.
		///
		/// </summary>
		/// <param name="buffer"></param>
		/// <returns>true  if success</returns>
		bool getWriteBuffer(unsigned char* buffer) {
			if (buffer == nullptr) return false;
			
			buffer[0] = (unsigned char) SDOCommandCodes::WRCMD | (unsigned char) data_dim;
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

		/// <summary>
		/// Gets the command content form a frame
		/// </summary>
		/// <param name="frame">can frame</param>
		/// <returns>command code</returns>
		inline unsigned char  getCmd(unsigned char* frame) { return frame[0] & 0xF0; }

		/// <summary>
		/// Gets the data dimension content from a frame
		/// </summary>
		/// <param name="frame">can frame</param>
		/// <returns>data dimension code</returns>
		inline unsigned char  getDataDim(unsigned char* frame) { return frame[0] & 0x0F;}

		/// <summary>
		/// Gets the Index address from a can frame
		/// </summary>
		/// <param name="frame">can frame</param>
		/// <returns>Index register</returns>
		inline unsigned short getIdx(unsigned char* frame) { return frame[1] + 256 * frame[2]; }

		/// <summary>
		/// Gets the Sub Index address from a can frame
		/// </summary>
		/// <param name="frame">can frame</param>
		/// <returns>Sub Index register</returns>
		inline unsigned char  getSub(unsigned char* frame) { return frame[3] ; }

		/// <summary>
		/// Gets the Value from a can frame.
		/// 
		/// The value depends of the data dimension encoded into the frame.
		/// 
		/// </summary>
		/// <param name="frame">can frame</param>
		/// <returns>Value</returns>
		inline unsigned int  getVal(unsigned char* frame) { 

			switch (frame[0] & 0x0F) {
			case (unsigned char) SDODataDimension::R1B:
				return (unsigned int) frame[4];
			case (unsigned char)SDODataDimension::R2B:
				return ((unsigned int) frame[4] + 0x100 * (unsigned int) frame[5]);
			case (unsigned char)SDODataDimension::R3B:
				return (unsigned int) frame[4] + 0x100 * (unsigned int) frame[5] + 0x10000 * (unsigned int) frame[6];
			case (unsigned char)SDODataDimension::R4B:
				return (unsigned int)frame[4] + 0x100 * (unsigned int)frame[5] + 0x10000 * (unsigned int) frame[6] + 0x1000000 * (unsigned int) frame[7];
			}
			return 0;
		}

		/// <summary>
		/// This function is used to validate an incoming can frame as a result
		/// of a read or write commands. 
		/// 
		/// Usually this function is used in the an Reception callback to 
		/// accept and validate a SDO command acknowledge form the remmote device;
		/// 
		/// The validation process requires the following tests:
		/// - The incoming Index shall match the register Index;
		/// - The incoming Sub-Index shall match the register Sub-Index;
		/// - The incoming command answer shall match with the register expected command;
		/// 
		/// If the incoming frame matches with the Register Address and command, 
		/// the register value is updated with the incoming value:
		/// - In case of a Read command, the incoming value is the content of the remote register;
		/// - In case of Write command, the incoming value should match with the value written;
		/// 
		/// </summary>
		/// <param name="frame">incoming can data frame</param>
		/// <returns>true if the data are valid</returns>
		bool validateSdo(unsigned char* frame) {
			valid = false;

			if (frame == nullptr) {
				Debug::WriteLine("NULL");
				return false;
			}
		
			if (ODRegister::getIdx(frame) != index) {
				Debug::WriteLine("INVALID INDEX");
				return false;
			}
			if (ODRegister::getSub(frame) != subindex) {
				Debug::WriteLine("INVALID SUB-INDEX");
				return false;
			}

			unsigned char received_cmd = getCmd(frame);
			if (received_cmd == (unsigned char)SDOCommandCodes::ERRACK) {
				if(cmd == SDOCommandCodes::RDCMD) Debug::WriteLine("READ: " + getError(frame));
				else Debug::WriteLine("WRITE: " + getError(frame));
				return false;
			}

			if (cmd == SDOCommandCodes::RDCMD)  {
				if (received_cmd != (unsigned char) SDOCommandCodes::RDANSW) {

					System::String^ stringa = " ";
					for (int i = 0; i < 8; i++) stringa += System::Convert::ToString(frame[i]) + " ";
					Debug::WriteLine(stringa);
					return false;
				}
				if (ODRegister::getDataDim(frame) != (unsigned char) data_dim) {
					Debug::WriteLine("INVALID DATADIM");
					return false;
				}
				
			}
			else {
				if (received_cmd != (unsigned char)SDOCommandCodes::WRANSW) {
					Debug::WriteLine("WRANSW");
					return false;
				}
			}

			// The data is valid: fills the read SDO content into the register			
			data = ODRegister::getVal(frame);
			valid = true;
			return true;
		}

		
		
		SDOCommandCodes cmd;		//!< Command Code assigned to a register for a pending Read or Write command
		SDODataDimension data_dim;	//!< Data dimension assigned for write commands or received by read commands
		unsigned short index;	//!< Register Index address
		unsigned char subindex;	//!< Regiter SubIndex address 
		unsigned int  data;		//!< 4 Bytes Data content register: assigned ffor write commands, received by read commands
		bool valid;				//!< Result of a validation process (in the reception can frame evaluation)
		
	};
	
	/// <summary>
	/// This is the Class implementing the CanOPEN Motor Device control protocol.
	/// 
	/// This class implements the full control of a  Motor device based on the CanOPEN protocol.
	/// The class provides the following features:
	/// 
	/// - Handles the register initialization and configuration; 
	/// - Handles the nanoj program uploading and activation;
	/// - Handles the CiA status machine management;
	/// - Handles the Positioning commands;
	/// 
	/// # Device initialization
	/// 
	/// When the Application calls the activateConfiguration() or in case of remote device reset,
	/// the module starts the remote device configuration. 
	/// 
	/// During this configuration fase the module:
	/// - Upload if present the Nano-J prgram;
	/// - Uploads the object registers with the application object register values.
	/// 
	/// ## Custom register initialization
	/// 
	/// 
	/// # CiA Status machine management
	/// 
	/// The CanOPEN based device internal status changes in a set of standard predefined 
	/// status called CiA_ Status machine. 
	/// 
	/// Any status is characterized by a specific motor conditions. 
	/// It is not possible to arbitrary change from any status to any status but a specific change status sequence 
	/// shall be followed: the Module implements automatically all the steps to drive 
	/// the internal status into the operating status: CiA_SwitchedOn status.
	/// 
	/// # Nano-J program handling
	/// 
	/// The module allows to upload a special program running on the PD4/PD6 series 
	/// of the nanotec motors. In order to upload a Nanoj program, the Application shall subclass
	/// this module declaring a vector of the program data pointer and passing it to the module 
	/// in the Subclass constructor with the command  setNanoJPtr();
	/// 
	/// During the initialization fase the program is then uploaded to the device. 
	/// 
	/// Specific commands are provided to starts/stop the program into the remote device.
	///  
	/// # User Units and Motor Units
	/// 
	/// When the module shall Read or Write the activation registers like the encoder position,
	/// the target position register, the Speed register, the Acceleration register or deceleration register,
	/// the units are internal and depends by several internal registers. 
	/// 
	/// To help the application in handling a more readable units, the user units, the module provides 
	/// two functions to read/write the position registers and one function to set 
	/// the Speed/Acceleration/Deceleration registers:
	/// - convert_Encoder_To_User(): converts the motor position units to User units;
	/// - convert_User_To_Encoder(): converts the User position units to motor position units;
	/// - convert_UserSec_To_Speed(): converts the user per seconds units to Motor unit per seconds;
	/// 
	/// For example, if the Application should need to read the actual Motor position,
	/// the Application should read the Position Register and use the  convert_Encoder_To_User(position-register-content)
	/// to get the information about the position in user units.
	/// 
	/// If the Application should set the activation speed register, it can assign to the register 
	/// to the  convert_UserSec_To_Speed(user-speed) value.
	/// 
	/// In order to defines the User units the Application shall pass a	special parameter
	/// to the class constructor: 
	/// - rot_to_unit: this is the number of motor round corresponding to 1 unit.
	/// 
	/// This number clearly depends by the gearbox ratio from the motor axe and the activation point.  
	///  
	/// # Activation Commands
	/// 
	/// When the module is in the CiA_SwitchedOn status a motor activation can be performed.
	/// The module provides the following commands for the activation:
	/// 
	/// -  activateAutomaticPositioning(): starts the motor at the target position;
	///  
	/// 
	//// \ingroup CanOpenModule
	/// </summary>
	ref class CanOpenMotor 
	{
		public:delegate void delegate_fault_callback(int code);
		public:static event delegate_fault_callback^ fault_event; //!< Event generated at the command completion


		public:delegate void delegate_command_completed_callback(int id, int code);
		public:static event delegate_command_completed_callback^ command_completed_event; //!< Event generated at the command completion

		public:bool activateAutomaticPositioning(int id, int target, int speed, int acc, int dec);	//!< This function starts an automatic positioning		
		public:bool activateAutomaticHoming(int method_on, int method_off, int speed, int acc);	//!< This function starts the automatic homing procedure
		protected: inline int getPreviousPosition(void) { return previous_uposition; }

		public:CanOpenMotor(unsigned char devid, LPCWSTR motorname, double gear);

		public:enum class  status_options {
				MOTOR_NOT_CONNECTED = 0,
				MOTOR_CONFIGURATION,
				MOTOR_READY,
				MOTOR_BUSY,
				MOTOR_FAULT
			};

			/// <summary>
			/// This enumeration class defines the Command codes
			/// </summary>
			public:enum class MotorCommands {
				MOTOR_IDLE = 0,			//!< No command are presents				
				MOTOR_HOMING,			//!< Homing procedure for automatic zero setting
				MOTOR_AUTO_POSITIONING, //!< Motor Automatic activation to target
			};

			public:enum class MotorCompletedCodes {
				COMMAND_SUCCESS = 0,
				COMMAND_PROCEED = 0,
				ERROR_MOTOR_BUSY,
				ERROR_INITIALIZATION,
				ERROR_UNEXPECTED_STATUS,
				ERROR_LIMIT_SWITCH,
				ERROR_BRAKE_DEVICE,
				ERROR_INVALID_COMMAND_INITIAL_CONDITION,
				ERROR_TIMOUT,
				ERROR_INTERNAL_FAULT,
				ERROR_ACTIVATION_REGISTER,
				ERROR_MISSING_HOME,
			};

		public: inline System::String^ getInternalStatusStr(void) { return status_tags[(int)internal_status]; }
		public: inline status_options getInternalStatus(void) { return internal_status; }
		
		public: bool activateConfiguration(void);
		public: inline bool isConfigurating(void) { return configuration_command; }
		public: inline bool isODConfigured(void) { return od_initialized; }
		public: inline bool isNanojConfigured(void) { return nanoj_initialized; }
		public: inline bool isReady(void) { return ((internal_status == status_options::MOTOR_READY) && (request_command == MotorCommands::MOTOR_IDLE)); }
		public: inline int getCurrentPosition(void) { return current_uposition; }

		

		// Device Initialization __________________________________________________________________
		
		protected:bool			initializeObjectDictionary(void);		
		protected:void			setNanoJPtr(const unsigned char* ptr, int size) { pNanoj = ptr; nanojSize = size; }
		protected: bool			initResetEncoderCommand(int initial_position);

		// Device Status _________________________________________________________________________
		static const cli::array<System::String^>^ status_tags = gcnew cli::array<System::String^> { "NOT CONNECTED", "CONFIGURATION", "READY", "BUSY", "FAULT"};
		private:status_options internal_status;
		protected: int encoder_initial_value; //! This is the value that shall be assigne to the encoder at the startup
		protected: int current_eposition;		//!< Current Encoder position
		protected: int current_uposition;		//!< Current User position 
		protected: int previous_uposition;			//!< This is the last target position for non coordinate activations
		private: int target_range_h;			//!< This is the acceptable target range (positive limit)
		private: int target_range_l;			//!< This is the acceptable target range (negative limit)
		protected: void setTargetRange(int h, int l) { target_range_h = h; target_range_l = l; }

		protected: bool od_initialized;    //!< Object dictionary has been intialized
		protected: bool nanoj_initialized; //!< Nano-J program has been intialized
		protected: bool home_initialized;  //!< The device has executed the homing procedure
		
		// Device Unit Definition ________________________________________________________________
		private: double rot_per_unit; //!< This is Rotation/units
		protected: int convert_Encoder_To_User(int x);
		protected: int convert_User_To_Encoder(int x);
		protected: int convert_UserSec_To_Speed(int x);

		// CanOpen protocol section ______________________________________________________________
		private: unsigned char device_id;
		private: HANDLE rxSDOEvent;			//!< Event object signaled by the SDO receiving callback
		private:bool sdo_rx_pending;		//!< A SDO reception fdata is pending 
		private:bool nanoj_rx_pending;		//!< A SDO reception fdata is pending 
		protected:ODRegister^ rxSdoRegister;	//!< SDO receiving data
		private:unsigned char rxNanojAck;
		private:bool rxNanojAckValid;

		protected:bool blocking_writeOD(unsigned short index, unsigned char sub, ODRegister::SDODataDimension dim, int val);
		private:void write_resetNode(void);
		protected:bool blocking_readOD(unsigned short index, unsigned char sub, ODRegister::SDODataDimension dim);
		private:bool writeControlWord(unsigned int mask, unsigned int val);
	
		private:void thread_canopen_rx_sdo_callback(unsigned short canid, unsigned char* data, unsigned char len);
		private: void thread_canopen_bootup_callback(unsigned short canid, unsigned char* data, unsigned char len);


		// CanOPEN CiA status management __________________________________________________________________
		private:Thread^ main_thread;
		private:void mainWorker(void);

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

		private: _CiA402Status CiA_current_status;     //!< CiA current detected status
		private: System::String^ getErrorClass1001(unsigned int val);
		private: System::String^ getErrorClass1003(unsigned int val);
		private: System::String^ getErrorCode1003(unsigned int val);

		private: _CiA402Status getCiAStatus(int regval);
		private: System::String^ getCiAStatusString(_CiA402Status status);
		private: void CiA402_QuickStopActiveCallback(void);
		private: void CiA402_SwitchOnDisabledCallback(void);
		private: void CiA402_ReadyToSwitchOnCallback(void);
		private: void CiA402_SwitchedOnCallback(void);
		private: void CiA402_OperationEnabledCallback(void);
		private: void CiA402_FaultCallback(void);
		private: bool error_condition;
		private: unsigned int error_class;
		private: unsigned int error_code;

		// Device Configuration _____________________________________________________________________________
		private: bool initNanojDataRegister(void);
		private: bool nanojWrite1024Block(int index, int size);
		private: bool uploadNanojProgram(void);
		private: bool configuration_command;
		private: const unsigned char* pNanoj;
		private: int nanojSize;
		

		// Device Activation Management Section ____________________________________________________	
		private:	void setCommandCompletedCode(MotorCompletedCodes error); //!< This function 	
		private:	void updateCurrentPosition(void);
		private:	void setActivationTimeout(int speed, int acc, int dec, int target);
		private:	bool isTarget(void) { return ((current_uposition <= command_target + target_range_h) && (current_uposition >= command_target - target_range_h)); }

		private:	void manageAutomaticPositioning(void);
		private:	void manageAutomaticHoming(void);


		private: MotorCommands request_command; //!< Application request command code
		private: MotorCommands current_command; //!< Current executing command code
		private:MotorCompletedCodes command_completed_code;//!< Activation result
		
		protected:int command_id;				//!< ID code of the requested command
		private:int command_target;			//!< Target position in user units
		private:int command_acc;			//!< Acceleration in user/s2
		private:int command_dec;			//!< Deceleration in user/s2
		private:int command_speed;			//!< Speed in user/s
		private:int command_ms_tmo;			//!< Timoeut activation in ms
		private:int command_homing_on_method;  //!< Homing method whith zero photocell starting in ON status
		private:int command_homing_off_method;  //!< Homing method whith zero photocell starting in OFF status

		// Subclass Overridable callbacks ____________________________________________________
		
		
		protected:	virtual MotorCompletedCodes automaticPositioningPreparationCallback(void) { return MotorCompletedCodes::COMMAND_PROCEED; } //!< This function is called just before to Power the motor phases
		protected:	virtual void automaticPositioningCompletedCallback(MotorCompletedCodes error) { return; }  //!< This function is called when the command is terminated
		
		protected:	virtual MotorCompletedCodes automaticHomingPreparationCallback(void) { return MotorCompletedCodes::COMMAND_PROCEED; } //!< This function is called just before to Power the motor phases
		protected:	virtual void automaticHomingCompletedCallback(MotorCompletedCodes error) { return; }  //!< This function is called when the command is terminated

		protected:	virtual bool idleCallback(void) { return true; }
		protected:	virtual bool initializeSpecificObjectDictionaryCallback(void) { return true; } //!< Override this function to initialize specific registers of the target Motor Device

	};

}; 