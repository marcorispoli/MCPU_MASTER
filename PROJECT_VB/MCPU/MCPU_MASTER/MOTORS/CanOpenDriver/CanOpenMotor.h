#pragma once
#include <Windows.h>
#include "Log.h"
#include "Notify.h"

using namespace System::Diagnostics;
using namespace System::Threading;

/// <summary>
/// This is the based class module implementing the control of 
/// motor devices based on the CANOpen protocol.
/// 
/// \defgroup CanOpenModule CanOPEN based motor devices 
/// 
/// 
/// </summary>
/// 
namespace CANOPEN {

	#define SPEED_DENOMINATOR       120 //!< This is the Speed conversion denominator unit 
	#define VMM_CTRL_OD OD_1F51_02
	#define VMM_DATA_OD OD_1F50_02
	#define VMM_STATUS_OD OD_1F57_02
	#define DRIVER_POLLING_MS 100
	#define SEND_TMO          100 // 1000 ms waiting the reception


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

			if (frame == nullptr) return false;			
			if (ODRegister::getIdx(frame) != index) return false;			
			if (ODRegister::getSub(frame) != subindex) return false;
			

			unsigned char received_cmd = getCmd(frame);
			if (received_cmd == (unsigned char)SDOCommandCodes::ERRACK) {
				if(cmd == SDOCommandCodes::RDCMD) LogClass::logInFile("READ: " + getError(frame));
				else LogClass::logInFile("WRITE: " + getError(frame));
				return false;
			}

			if (cmd == SDOCommandCodes::RDCMD)  {
				if (received_cmd != (unsigned char) SDOCommandCodes::RDANSW) return false;				
				if (ODRegister::getDataDim(frame) != (unsigned char) data_dim) {
					LogClass::logInFile("INVALID DATADIM");
					return false;
				}				
			}
			else {
				if (received_cmd != (unsigned char)SDOCommandCodes::WRANSW) return false;				
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
	
	ref class CanMotorCommunicationMonitor {
	public:

		/// <summary>
		/// Initializes the internal caounters
		/// </summary>
		/// <param name=""></param>
		CanMotorCommunicationMonitor(void) {

			sent_messages = 0;
			sent_5 = 0;
			sent_10 = 0;
			sent_15 = 0;
			sent_20 = 0;
			sent_25 = 0;
			sent_30 = 0;
			sent_xx = 0;
			repeated_messages = 0;

			meanTime = 0;
			percMeanTime = 0;
			unreceived_messages = 0;
			log_string = "";
		}

		/// <summary>
		/// Updates the communication statistics at the point of the call
		/// </summary>
		/// 
		/// Every 1000 sent messages the internal percentuals statistics are calculated.
		/// 
		/// <param name=""></param>
		void updateStatistics(void) {
			if (txrx_time < 0.005)  sent_5++;
			else if (txrx_time < 0.010)  sent_10++;
			else if (txrx_time < 0.015)  sent_15++;
			else if (txrx_time < 0.020)  sent_20++;
			else if (txrx_time < 0.025)  sent_25++;
			else if (txrx_time < 0.030)  sent_30++;
			else if (txrx_time >= 0.030)  sent_xx++;
			meanTime += txrx_time;

			if (sent_messages == 1000) {
				perc5 = (double)sent_5 * 100 / (double)sent_messages;
				perc10 = (double)sent_10 * 100 / (double)sent_messages;
				perc15 = (double)sent_15 * 100 / (double)sent_messages;
				perc20 = (double)sent_20 * 100 / (double)sent_messages;
				perc25 = (double)sent_25 * 100 / (double)sent_messages;
				perc30 = (double)sent_30 * 100 / (double)sent_messages;
				percXX = (double)sent_xx * 100 / (double)sent_messages;
				percMeanTime = meanTime * 1000 / (double)sent_messages;
				percRepeated = (double)repeated_messages * 100 / (double)sent_messages;



				meanTime = 0;
				sent_5 = 0;
				sent_10 = 0;
				sent_15 = 0;
				sent_20 = 0;
				sent_25 = 0;
				sent_30 = 0;
				sent_xx = 0;
				repeated_messages = 0;
				sent_messages = 0;

			};
		}

		/// <summary>
		/// Returns a formatted string with the calculated statistics
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		System::String^ getStatFormattedString(void) {
			log_string = " [T]:" + ((int)percMeanTime).ToString();
			log_string += " [<5]:" + ((int)perc5).ToString();
			log_string += " [<10]:" + ((int)perc10).ToString();
			log_string += " [<15]:" + ((int)perc15).ToString();
			log_string += " [<20]:" + ((int)perc20).ToString();
			log_string += " [<25]:" + ((int)perc25).ToString();
			log_string += " [<30]:" + ((int)perc30).ToString();
			log_string += " [>30]:" + ((int)percXX).ToString();
			log_string += " [RPT]:" + ((int)percRepeated).ToString();
			return log_string;
		}

		double txrx_time; //!< Last Send - Receive time

		unsigned long sent_messages; //!< Total number of sent frames
		unsigned long sent_5;	//!< Total number of sent frames with a reception time lower than 5ms
		unsigned long sent_10;	//!< Total number of sent frames with a reception time lower than 10ms
		unsigned long sent_15;	//!< Total number of sent frames with a reception time lower than 15ms
		unsigned long sent_20;  //!< Total number of sent frames with a reception time lower than 20ms
		unsigned long sent_25;  //!< Total number of sent frames with a reception time lower than 25ms
		unsigned long sent_30;  //!< Total number of sent frames with a reception time lower than 30ms
		unsigned long sent_xx;  //!< Total number of sent frames with a reception time exceeding 30ms
		unsigned long repeated_messages; //!< A repeated answer message (discarded)  due to a can frame repeatition

		double perc5;	//!< Percent of messages with less than 5ms of tx-rx time 
		double perc10;	//!< Percent of messages with less than 10ms of tx-rx time 
		double perc15;  //!< Percent of messages with less than 15ms of tx-rx time 
		double perc20;  //!< Percent of messages with less than 20ms of tx-rx time 
		double perc25;	//!< Percent of messages with less than 25ms of tx-rx time 
		double perc30;	//!< Percent of messages with less than 30ms of tx-rx time 
		double percXX;  //!< Percent of messages exceeding 30ms of tx-rx time 
		double percRepeated; //!< Percent of repeated messages

		double meanTime;	//!< Total Rx-Tx Mean time 
		double percMeanTime;//!< Percent of the Tx-Rx mean time

		unsigned long unreceived_messages; //!< Number of unreceived messages
		System::String^ log_string;//!< Pre formatted string statistic
	};

	/// <summary>
	/// This is the Class implementing the CanOPEN Motor Device control protocol.
	///
	/// \defgroup CanOpenBaseModule Motor Driver base class module
	/// \ingroup CanOpenModule
	/// </summary>
	/// 
	/// This class implements the full control of a  Motor device based on the CanOPEN protocol.
	/// The class provides the following features:
	/// 
	/// - Handles the register initialization and configuration; 
	/// - Handles the nanoj program uploading and activation;
	/// - Handles the CiA status machine management;
	/// - Handles the Automatic Positioning commands;
	/// - Handles the Manual Positioning commands;
	/// - Handles the Homing commands;
	/// 
	/// # Device initialization
	/// 
	/// When the Application calls the activateConfiguration() or in case of remote device reset,
	/// the module starts the remote device configuration. 
	/// 
	/// During this configuration fase the module:
	/// - Uploads if present the Nano-J program;
	/// - Uploads the object registers with the application object register values.
	/// - In case the registers should be differents, the value is stored into the device.
	/// 
	/// 
	/// 
	/// # CiA Status machine management
	/// 
	/// The CanOPEN based device internal status changesaccording with a standard status called CiA_ Status machine. 
	/// 
	/// - Any status is characterized by a specific motor conditions;
	/// - It is not possible to arbitrary change from any status to any status but a specific change status sequence 
	/// shall be followed; 
	/// - the Module implements automatically all the steps to drive the internal status into the operating status: CiA_SwitchedOn status;
	/// - In case of a fault condition should be present, the module automatically enters a special status called fault,
	/// untile the fault condition should reset.
	/// 
	///		NOTE: the module will drive the motor device in only two possible modes: 
	/// - Switched ON (idle);
	/// - Fault;
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
	/// -  activateAutomaticHoming(): starts automatic zero setting procedure;
	/// -  activateManualPositioning(): starts the manual position procedure;
	/// -  abortActivation(): aborts any pending activation;
	/// 
	/// # Interface callbacks
	/// The module provides a set of overridable callbacks  that a derived module can implement to customize the behavior
	/// of the final motor controller module:
	/// - motionParameterCallback(): this callback is called just before to set the current activation speed, allowing to customize the 
	///   activation paramters;
	/// 
	/// - preparationCallback(): this callback is called just before to power the motor wires;
	/// - runningCallback(): this callback is called several time during the motion;
	/// - completedCallback(): this function is called as soon as the command is completed;
	/// - idleCallback(): this is the callback during the IDEL startus (CiA_Switched_On status);
	/// - faultCallback): this is the callback called during the fault status;
	/// - resetCallback(): this is a callback calle just after the driver reset;
	/// - initializeSpecificObjectDictionaryCallback(): this is the callback called during the initialization fase;
	/// - brakeCallback(): this is a callback called just after the activation is terminated, before to remove the power to the wires.
	/// - unbrakeCallback() : this callback is called at the beginning of the activation, just after the motor wire powering(torque presence)
	/// 
	/// # Encoder Position validation
	/// 
	/// The current encoder posiiton is stored into the MotorCalibration.cnf file;
	/// 
	/// - At the beginning of an activaiton the position is invalidated;
	/// - At the command completion the position is stored into the file;
	/// 
	///		Note: in case of an invalid software procedure should loos the command termination, 
	///		the encoder position will remain invalidated, until a new homing procedure is executed.
	/// 
	/// 
	///		
	/// 
	ref class CanOpenMotor 
	{
	public:

		/// <summary>
		/// \defgroup CanOpenModuleApi Application Interface (API) 
		/// \ingroup CanOpenBaseModule
		/// 
		/// This section describes the API for the Application usage.
		/// 
		/// </summary>
		/// @{
		
		public:CanOpenMotor(unsigned char devid, 
			LPCWSTR motorname, 
			System::String^ parameter, 
			Notify::messages home_err,
			int min_position,
			int max_position,
			double gear, 
			double external_k, 
			bool reverse
		); //!< This is the base class constructor

		void runMode(void) { simulator_mode = false; run = true; }
		void demoMode(void) { simulator_mode = true; run = true; }
		inline bool isSimulatorMode(void) { return simulator_mode; }

		delegate void delegate_fault_callback(int code); //!< Delegate for the callback related to the Fault condition
		event delegate_fault_callback^ fault_event; //!< Event generated when a Driver fault condition is detected
		
		delegate void delegate_command_completed_callback(int id, int code);//!< Delegate for the command completed event		
	    event delegate_command_completed_callback^ command_completed_event; //!< Event generated at the command completion

		bool  activateRelativePositioning(int id, int target, int speed, int acc, int dec); 	//!< This function starts an automatic relative positioning		
		bool  activateRelativePositioning(int id, int target); 	//!< This function starts an automatic relative positioning with predefined motor parameters

		bool  activateAutomaticPositioning(int id, int target, int speed, int acc, int dec, bool autostart);	//!< This function starts an automatic positioning		
		bool  activateAutomaticPositioning(int id, int target, bool autostart); //!< This function starts an automatic positioning with predefined parameters		
		
		bool  activateAutomaticHoming(int method_on, int method_off, int speed, int acc);	//!< This function starts the automatic homing procedure
		bool  activateExternalHoming(int current_uposition);	//!< This function starts the external sensor homing procedure
		bool  activateManualHoming(int current_uposition);	//!< This function starts the external sensor homing procedure

		bool  activateManualPositioning(int target, int speed, int acc, int dec); //!< This command activates the manual mootion		
		bool  activateManualPositioning(int target); //!< This command activates the manual mootion with predefined parameters
		bool  activateManualService(bool increase);

		void setServiceMode(bool stat) { service_mode = stat; }

		void  abortActivation(void); //!< Immediate abort of any activation running

		/// <summary>
		/// This enumeration class descibes the internal status condition
		/// </summary>
		enum class  status_options {
				MOTOR_NOT_CONNECTED = 0,	//!< The Motor is not connected with the CAN bus
				MOTOR_CONFIGURATION,		//!< The module is configuring the driver
				MOTOR_READY,				//!< The driver is ready to execute an activation command
				MOTOR_BUSY,					//!< The driver is executing an acivation command
				MOTOR_FAULT,				//!< The driver is in fault condition				
		};
		
		
		/// <summary>
		/// This function returns the internal module status.
		/// 
		/// </summary>
		/// <param name=""></param>
		/// <returns>The internal Module status</returns>
		inline status_options getInternalStatus(void) { return internal_status; }

		/// <summary>
		/// This function returns a description string of the internal motor status
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		inline System::String^ getInternalStatusStr(void) {	return status_tags[(int)internal_status];}

		/// <summary>
		/// This enumeration class descibes  the Command codes
		/// 
		/// </summary>
		enum class MotorCommands {
			MOTOR_IDLE = 0,			//!< No command are presents				
			MOTOR_AUTO_HOMING,		//!< Automatic Homing procedure for automatic zero setting
			MOTOR_MANUAL_HOMING,	//!< Manual Homing procedure for manual zero setting
			MOTOR_EXTERNAL_HOMING,	//!< Homing for the external position sensor

			MOTOR_AUTO_POSITIONING, //!< Motor Automatic activation to target
			MOTOR_MANUAL_POSITIONING, //!< Motor Manual activation to target
			MOTOR_MANUAL_SERVICE,	//!< Motor Manual activation for service (no position limitation)
		};

		enum class motor_rotation_activations {
			MOTOR_NO_ACTIVATION,    //!< No activation 
			MOTOR_INCREASE,         //!< Activation with encoder increment
			MOTOR_DECREASE,         //!< Activation with encoder decrement
			MOTOR_UNDEFINED,		//!< Undefined condition
		};



		/// <summary>
		/// This enumeration class descibes the command complete codes
		/// </summary>
		/// The enumeration class provoides a set of codes 
		/// used for different purposes:
		/// + COMMAND_SUCCESS: it is used for the command completion status;
		/// + COMMAND_PROCEED: it is used for the authorization in proceed with the command execution
		/// + ERROR_: they are codes reserved for command completion error conditions
		/// 
		enum class MotorCompletedCodes {
			COMMAND_SUCCESS = 0, //!< The Command is successsfully terminated 
			COMMAND_PROCEED = 0, //!< The Command can proceed in the execution (reserved for subclass)
			COMMAND_MANUAL_TERMINATION,//!< The Command has been manually terminated
			MOTOR_ERRORS, //!< First of the reserved Error codes
			ERROR_OBSTACLE_DETECTED = MOTOR_ERRORS, //!< The command has been terminated because of obstacle detected
			ERROR_MOTOR_BUSY,//!< The command cannot be executed because of Busy condition
			ERROR_INITIALIZATION,//!< The command has been aborted during the initialization
			ERROR_UNEXPECTED_STATUS,//!< The command has been aborted due to an unexpected CiA status
			ERROR_LIMIT_SWITCH,//!< The command has been aborted due to limit switch activation
			ERROR_BRAKE_DEVICE,//!< The command has been aborted due to a brake device malfunction
			ERROR_TIMOUT,//!< The command has been aborted due to timeout activation
			ERROR_INTERNAL_FAULT,//!< The command has been aborted due to a driver fault
			ERROR_ACCESS_REGISTER,//!< The command has been aborted due to an error in accessing a driver register
			ERROR_MISSING_HOME,//!< The command has been aborted due to invalid homing (the encoder is not correctly initialized)
			ERROR_TARGET_OUT_OF_RANGE,//!< The target for the activation is lower the minimum or higher then maximum allowed
			ERROR_COMMAND_DISABLED,//!< The command has been aborted because the activation is not enabled
			ERROR_COMMAND_ABORTED,//!< The command has been aborted due to an Abort activation request
			ERROR_COMMAND_DEMO,//!< The command cannot be executed in demo
			ERROR_SAFETY, //!< The command has been aborted due to safety conditions
			ERROR_STARTING_NANOJ, //!< The Nano-J command failed to start
			ERROR_INVALID_COMMAND //!< Command not valuid in the current motor configuration or motor status
		};		
		
		/// <summary>
		/// This function returns the last command termination code
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		inline MotorCompletedCodes getCommandCompletedCode(void) { return command_completed_code; }
		
		/// <summary>
		/// This function activates the Driver configuration fase
		/// </summary>
		/// The Driver configuration will take place only when the 
		/// driver is in internal status READY or FAULT
		/// <param name=""></param>
		/// <returns>true always</returns>
		bool activateConfiguration(void) {
			configuration_command = true; 
			return true;
		} 
		
		inline bool isPositionFromExternalSensor(void) { return external_position_mode; }

		/// <summary>
		/// This function returns the current configuration fase status
		/// </summary>
		/// <param name=""></param>
		/// <returns>true: the configuration is executing</returns>
		inline bool isConfigurating(void) { return configuration_command; }

		/// <summary>
		/// This function returns the status of the Object Dictionary configuration status
		/// </summary>
		/// <param name=""></param>
		/// <returns>true: the object dictionary has benn successfully configured</returns>
		inline bool isODConfigured(void) { return od_initialized; }

		/// <summary>
		/// This function returns the status of the Nano-J programming
		/// </summary>
		/// <param name=""></param>
		/// <returns>true: the program has been successfully uploaded (if required)</returns>
		inline bool isNanojConfigured(void) { return nanoj_initialized; }

		/// <summary>
		/// This function returns true if the module is ready to execute a command.
		/// </summary>
		/// 
		/// If this function should return false, it doesn't mean that a command
		/// is executing. See the isRunning() function for that purpose.
		/// 
		/// A false return code is related to an ointernal code not allowed to execute any command.
		/// 
		/// NOTE: in case of true condition, other external conditions may prevent the command to start execution.
		/// 
		/// <param name=""></param>
		/// <returns>true: the driver is ready to execute a command</returns>
		inline bool isReady(void) { return ((internal_status == status_options::MOTOR_READY) && (request_command == MotorCommands::MOTOR_IDLE)); }

		/// <summary>
		/// This function return true if an executing command is pending
		/// </summary>
		/// 
		/// The function shall be used to know if the motor is activated or a comnmand
		/// is pending (but not yet started)
		/// 
		/// <param name=""></param>
		/// <returns></returns>
		inline bool isBusy(void) { return (request_command != MotorCommands::MOTOR_IDLE); }
		
		/// <summary>
		/// This function return true if a command is executing.
		/// 
		/// </summary>
		/// <param name=""></param>
		/// <returns>true: a command is executing</returns>
		inline bool isRunning(void) { return (current_command != MotorCommands::MOTOR_IDLE); }

		/// <summary>
		/// This function returns the Encoder Zero setting status
		/// </summary>
		/// <param name=""></param>
		/// <returns>true: the encoder has been correctly initialized</returns>
		inline bool isZeroOk(void) { return home_initialized; }

		/// <summary>
		/// This function returns the current encoder position.
		/// </summary>
		/// <param name=""></param>
		/// <returns>The encoder position in user units</returns>
		inline int getCurrentPosition(void) { return getCurrentUposition(); }


		inline unsigned short getExternalSensor(void) { return external_raw_position; }
		inline int getExternalPosition(void) { return external_uposition; }
		inline int getEncoderPosition(void) { return encoder_uposition; }
		
		inline int getMinPosition(void) { return min_position; }
		inline int getMaxPosition(void) { return max_position; }
		
		inline System::String^ getCanCommunicationMonitorString(void) { return can_communication_monitor.getStatFormattedString(); }


		///@} End of the API section ________________________________________________________________________

protected:		

		/// <summary>
		/// \defgroup CanOpenModuleProtected Protected Interface (Subclass reserved) 
		/// \ingroup CanOpenBaseModule
		/// 
		/// This section describes the API for subclass modules.
		/// 
		/// </summary>
		/// @{


		/// <summary>
		/// This function returns the previous position before the last activation execution.
		/// 
		/// </summary>
		/// 
		/// The function provides the application the method to manage
		/// variation in positions.
		/// 
		/// <param name=""></param>
		/// <returns>The previous valid position in user units</returns>
		inline int getPreviousPosition(void) {return previous_uposition;	}
			
		/// <summary>
		/// This function assignes the binary pointer of the nano-j program to be uploaded
		/// during the configuration fase.
		/// </summary>
		/// 
		/// The subclass should set this pointer at the beginning i the subclass constructor in order to 
		/// let the module to upload the program into the driver device.
		/// 
		/// <param name="ptr">this is the pointer of the program byte array</param>
		/// <param name="size">this is the size of the program in bytes</param>
		void setNanoJPtr(const unsigned char* ptr, int size) { pNanoj = ptr; nanojSize = size; }
		
		

		/// <summary>
		/// This function returns the current encoder position in Encoder internal units
		/// </summary>
		/// <param name=""></param>
		/// <returns>The current encoder position in Internal encoder units</returns>
		inline int getCurrentEncoderEposition(void) { return encoder_eposition;}

		inline int getCurrentEncoderUposition(void) { return encoder_uposition;}

		/// <summary>
		/// This function returns the current user defined position
		/// </summary>
		/// 
		/// The user defined position is the position of the external sensor 
		/// if it should be configured.
		/// 
		/// <param name=""></param>
		/// <returns>The current encoder position in user units</returns>
		inline int getCurrentUposition(void) { return (external_position_mode) ? external_uposition : encoder_uposition; }

		/// <summary>
		/// This function set the current acceptable position range.
		/// </summary>
		/// 
		/// The activation command is rejected if the target position should be 
		/// out of the acceptable range.
		/// 
		/// As default, the acceptable range is 0 (no command can be executed)
		/// 
		/// The Subclass should set this range to define the specific activation
		/// range.
		/// 
		/// <param name="h"></param>
		/// <param name="l"></param>
		void setTargetRange(int h, int l) { target_range_h = h; target_range_l = l; }

		/// <summary>
		/// This function returns the current encoder zero setting status
		/// 
		/// </summary>
		/// 
		/// <param name=""></param>
		/// <returns>true if a valid encoder position is present</returns>
		inline bool isEncoderInitialized(void) { return home_initialized; }

		/// <summary>
		/// This function allows the subclass to set the current zero setting status
		/// </summary>
		/// 
		/// The device after startup reset the zero setting flag,
		/// until a zero setting procedure is executed. 
		/// 
		/// Without a valid zero setting, any the activation command is rejected.
		/// 
		/// However the subclass can implement a strategy to recover the actual 
		/// real position without executing a zero setting activation. In this case 
		/// the subclass can set this flag allowing to execute further commands.
		/// 
		/// <param name=""></param>
		/// <returns></returns>
		inline void setEncoderInitStatus(bool val) { home_initialized = val; }

		/// <summary>
		/// This function translate internal encoder position to the unit position value
		/// </summary>
		/// 
		/// This function makes use of the rotation/Unit factor 
		/// the subclass passes to the module constructor in order to 
		/// define what will be the user position unit.
		/// 
		/// <param name="x">the encoder internal position</param>
		/// <returns> the user position </returns>
		int convert_Encoder_To_User(int x);

		/// <summary>
		/// This function translate internal encoder position to the unit position value
		/// </summary>
		/// 
		/// This function makes use of the rotation/Unit factor 
		/// the subclass passes to the module constructor in order to 
		/// define what will be the user position unit.
		///
		/// The value can be positive or negative based on the current direction 
		/// selected in the motor constructor.
		///   
		/// <param name="x">the encoder internal position</param>
		/// <returns> the user position </returns>
		int convert_Absolute_Encoder_To_User(int x);

		/// <summary>
		/// This function translate the user position to the encoder position 
		/// </summary>
		/// 
		/// This function makes use of the rotation/Unit factor 
		/// the subclass passes to the module constructor in order to 
		/// define what will be the user position unit.
		/// 
		/// The value can be positive or negative based on the current direction 
		/// selected in the motor constructor.
		/// 
		/// <param name="x">the user position</param>
		/// <returns> the encoder position </returns>
		int convert_User_To_Encoder(int x);
		
		// <summary>
		/// This function translate the user position to the encoder position
		/// without the direction correction.
		///  
		/// </summary>
		/// 
		/// This function makes use of the rotation/Unit factor 
		/// the subclass passes to the module constructor in order to 
		/// define what will be the user position unit.
		/// 
		/// <param name="x">the user position</param>
		/// <returns> the encoder position </returns>
		int convert_Absolute_User_To_Encoder(int x);
		

		/// <summary>
		/// This function is used to transform the user defined speed or acceleration 
		/// into the SPeed or Acceleration internal encoder units
		/// 
		/// </summary>
		/// <param name="x">this is the user speed or acceleration (units/s or units/ss) </param>
		/// <returns>the encoder unit format</returns>
		int convert_UserSec_To_Speed(int x);

		bool blocking_writeOD(unsigned short index, unsigned char sub, ODRegister::SDODataDimension dim, int val);
		void write_resetNode(void);
		bool blocking_readOD(unsigned short index, unsigned char sub, ODRegister::SDODataDimension dim);
		bool writeControlWord(unsigned int mask, unsigned int val);
		bool readControlWord(unsigned int* ctrlw);
		bool readStatusWord(unsigned int* stw);

		bool startRotation(void);
		bool startNanoj(void);
		bool stopNanoj(void);
			

		static System::String^ getErrorClass1001(unsigned int val);
		static System::String^ getErrorClass1003(unsigned int val);
		static System::String^ getErrorCode1003(unsigned int val);

		virtual void motionParameterCallback(MotorCommands current_command, int current_position, int target_position) { return ; };  //!< This function is called just before to set the speed: if return true, the speed is set to the predefined value
		virtual MotorCompletedCodes preparationCallback(MotorCommands current_command, int current_position, int target_position) { return MotorCompletedCodes::COMMAND_PROCEED; };  //!< This function is called just before to Power the motor phases
		virtual MotorCompletedCodes runningCallback(MotorCommands current_command, int current_position, int target_position) { return MotorCompletedCodes::COMMAND_PROCEED; };  //!< This function is called just before to Power the motor phases
		virtual void completedCallback(int id, MotorCommands current_command, int current_position, MotorCompletedCodes term_code) { return ; };  //!< This function is called just before to Power the motor phases
		virtual MotorCompletedCodes idleCallback(void) { return MotorCompletedCodes::COMMAND_PROCEED; }
		virtual void faultCallback(bool errstat, bool data_change, unsigned int error_class, unsigned int error_code) { return ; }
		virtual void resetCallback(void) { return ; } //!< Called whenever the boot message is received from the device
		virtual unsigned short initializeSpecificObjectDictionaryCallback(void) { return 0x0001; } //!< Override this function to initialize specific registers of the target Motor Device
		virtual bool unbrakeCallback(void) { return true; } //!< Called whenever the optional brake device should be released
		virtual bool brakeCallback(void) { return true; } //!< Called whenever the optional brake device should be reactivated
		
		virtual void demoLoop(void);

		inline void setCommandCompleted(MotorCompletedCodes error) { command_completed_code = error; }

		/// <summary>
		/// This function returns the current command-id.
		/// </summary>
		/// 
		/// The command-id is passed by the application when requesting a command execution.
		/// The command-id is returned in the command_completed_event to the application.
		/// 
		/// <param name=""></param>
		/// <returns>the command_id internal code</returns>
		inline int getCommandId(void) { return command_id; }

		/// <summary>
		/// Returns the pointer of the reception sdo register
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		inline ODRegister^ getRxReg(void) { return rxSdoRegister; }

		///@} End of the protected section _____________________________________________________________


protected:
	bool simulator_mode;
	unsigned char device_id;	//!< This is the target Device Id 
	System::String^ config_param;//!< Pointer to the parameter in the config parameter	
	Notify::messages error_homing;
	
	void inline setSpeed(int val) { command_speed = val; }   //!< Modifies the assigned command speed (to be used into the motionParameterCallback()) 
	void inline setAcc(int val)   { command_acc = val; }     //!< Modifies the assigned command acceleration (to be used into the motionParameterCallback()) 
	void inline setDec(int val)   { command_dec = val; }     //!< Modifies the assigned command deceleration (to be used into the motionParameterCallback()) 
	inline motor_rotation_activations  getMotorDirection(void) { return motor_direction; }
	
	bool fault_activation;
private:
		CanMotorCommunicationMonitor can_communication_monitor; //!< This is the debug class 

		bool run;
		bool reset_node;
		int max_position;			//!< This is the maximum target selectable
		int min_position;			//!< This is the minimum target selectable
		bool service_mode;

		HANDLE rxSDOEvent;			//!< Event object signaled by the SDO receiving callback
		volatile bool sdo_rx_pending;		//!< A SDO reception fdata is pending 
		volatile bool nanoj_rx_pending;		//!< A SDO reception fdata is pending 
		ODRegister^ rxSdoRegister;	//!< SDO receiving data
		unsigned char rxNanojAck;	//!< Nanoj ack byte
		bool rxNanojAckValid;		//!< Nano-j Ack vaild
		static const cli::array<System::String^>^ status_tags = gcnew cli::array<System::String^> { "NOT CONNECTED", "CONFIGURATION", "READY", "BUSY", "FAULT"};

		status_options internal_status; //!< This is the current internal motor status
		
		bool home_initialized;			//!< The device has executed the homing procedure	
	
		bool			external_position_mode;			//! The current position is detected with the analog input from an external source (potentiometer)
		unsigned short  external_raw_position;		//!< Cureent value of the potentiometer;
		unsigned short  external_zero_setting;	//!< Potentiometer value at mechanical zero position
		int				external_uposition;		//!< Current user position calculate from the external source
		float			external_k_coeff;		//!< user_position/external_raw_position coefficient
		bool			update_external_position(void);//!< read the external sensor and fill the external data
		
		
		int encoder_eposition;			//!< Current Encoder position
		int encoder_uposition;			//!< Current User position 

		bool reverse_direction;			//!< Changes the polarity of the position
		int previous_uposition;			//!< This is the last target position for non coordinate activations
		int target_range_h;				//!< This is the acceptable target range in user units (upper limit)
		int target_range_l;				//!< This is the acceptable target range in user units (lower limit)
		bool od_initialized;			//!< Object dictionary has been intialized
		bool nanoj_initialized;			//!< Nano-J program has been intialized
			
		double rot_per_unit;			//!< This is the assigned Rotation/units convertion factor

		
		
	
		// CanOPEN CiA status management __________________________________________________________________
		void thread_canopen_rx_sdo_callback(unsigned short canid, unsigned char* data, unsigned char len); //!< This is the CAN sdo reception callback
		void thread_canopen_bootup_callback(unsigned short canid, unsigned char* data, unsigned char len); //!< This is the CAN boot reception callback
		Thread^ main_thread;
		void mainWorker(void);

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

		_CiA402Status getCiAStatus(int regval);
		System::String^ getCiAStatusString(_CiA402Status status);
		void CiA402_QuickStopActiveCallback(void);
		void CiA402_SwitchOnDisabledCallback(void);
		void CiA402_ReadyToSwitchOnCallback(void);
		void CiA402_SwitchedOnCallback(void);
		void CiA402_OperationEnabledCallback(void);
		void CiA402_FaultCallback(void);
		bool error_condition;
		unsigned int error_class;
		unsigned int error_code;
		

		// Device Configuration _____________________________________________________________________________		
		bool setEncoderCommand(int eposition);
		bool initializeObjectDictionary(void);
		bool initNanojDataRegister(void);
		bool nanojWrite1024Block(int index, int size);
		bool uploadNanojProgram(void);
		bool configuration_command;
		const unsigned char* pNanoj;
		int nanojSize;
		

		// Device Activation Management Section ____________________________________________________	
		void setCommandCompletedCode(MotorCompletedCodes error); //!< This function 	
		void updateCurrentPosition(void);
		int getActivationTimeout(int speed, int acc, int dec, int target);
		bool isTarget(void) { return ((getCurrentUposition() <= command_target + target_range_h) && (getCurrentUposition() >= command_target - target_range_h)); }

		void manageAutomaticPositioning(void);		
		void manageManualPositioning(void);
		void manageManualServicePositioning(void);

		void manageAutomaticHoming(void);
		void manageManualHoming(int zero_position);
		void manageExternalHoming(int zero_position);//!< This function gets the external zero position sensor	   

		bool CiA402_activateOperationEnable(void);

		MotorCommands request_command; //!< Application request command code
		bool abort_request; //!< This flag active causes an immediate command abort

		MotorCommands current_command; //!< Current executing command code
		MotorCompletedCodes command_completed_code;//!< Activation result
		
		int command_id;			//!< ID code of the requested command 
		int command_target;			//!< Target position in user units
		int command_acc;			//!< Acceleration in user/s2
		int command_dec;			//!< Deceleration in user/s2
		int command_speed;			//!< Speed in user/s
		int command_ms_tmo;			//!< Timeout activation in ms
		bool command_stop;			//!< Request to stop the current activation
		int command_homing_on_method;  //!< Homing method whith zero photocell starting in ON status
		int command_homing_off_method;  //!< Homing method whith zero photocell starting in OFF status
		
		bool autostart_mode; //!< Set to tru if the activation ommand is automatically started
		motor_rotation_activations motor_direction; //!< The current direction of the motor activation

	};

}; 