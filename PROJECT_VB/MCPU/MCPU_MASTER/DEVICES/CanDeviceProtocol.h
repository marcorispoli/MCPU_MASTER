#pragma once
#include <Windows.h>

/// <summary>
/// \defgroup CanDevice_Module Device board modules based on CAN bus protocol
/// 
/// This module implements the genera device communication protocol based on the CAN bus
/// 
/// </summary>

/// <summary>
/// \defgroup moduleControl Public Module's Control API
/// 
/// This group provides API to control the current Module operating status.
/// 
/// 
/// 
/// </summary>
/// \ingroup CanDevice_Module 
/// 
/// The Module provides methods to:
/// + Start the module in Normal mode;
/// + Start the module in Simulation mode;
/// + Get the current module operating status;
/// + Get the connected remote device firmware and bootloader revision;
/// 

/// <summary>
/// \defgroup commanControl Public Module's Command Execution API
/// 
/// This group provides API to send protocol commands to the remote device.
/// </summary>
/// 
/// \ingroup CanDevice_Module 
/// 
/// # General description
/// 
/// The Module provides methods to:
/// + Send a command to the device;
/// + Wait for the result;
/// + Test the resulting code and errors;
/// 
/// # Command Management description
/// 
/// The Can Protocol specification defines two types of Commands:
/// + Immediate Commands: a command that is immediatelly exeuted by the remote device;
/// + Delayed Commands: a command that takes more than 2ms to be completed.
///  
/// With the immediate command, the remote device provides the execution result 
/// in the Command Answer frame packet.
/// 
/// With the Delayed command, the remote device acknowledge the Command request
/// and further update the special command register with the status of the execution.
/// 
/// The Module handles both the command execution types in a transparent way
/// so that the caller thread shall only to makes use of the API in order to 
/// detect the command completion, wheather it is delayed or immediate command type.
/// 
/// There are two types of command API for the application:
/// 
/// + CanDeviceProtocol::commandWaitCompletion(): This is a non blocking method to send a Command frame to the remote device;
/// + CanDeviceProtocol::commandWaitCompletion(): This is a blocking method to send a Command frame to the remote device;
/// 
/// ## commandNoWaitCompletion()
/// 
/// This is a non blocking method to send a Command frame to the remote device.
/// 
///	The caller thread shall poll the command completion status using the following methods:
///			+ CanDeviceProtocol::isCommandCompleted(): checks if the command is terminated;
/// 		+ CanDeviceProtocol::isCommandError(): checks if the command has been terminated with error condition;
/// 
/// The caller thread shall rerive the command result with the following methods:
///			+ CanDeviceProtocol::getCommandResult0(): returns the result byte 0;
///			+ CanDeviceProtocol::getCommandResult1(): returns the result byte 1;
///			+ CanDeviceProtocol::getCommandError():   returns the error code;
/// 
/// ## commandWaitCompletion()
/// 
/// This is a blocking method to send a Command frame to the remote device.
/// 
/// Using this method the calling thread gets locked until the command completion
/// is detected by the Base module class. The result is then available 
/// in the returned structure of the method.
/// 
/// WARNING! This method cannot be called inside the RunningLoop of the 
/// CanDevice Subclass module because it will generate a dead lock condition. 
/// 
/// For that reason the method needs a pointer to the calling thread as input parameter
/// to check if the condition should be violated.
/// 
///  
/// 
/// 



/// <summary>
/// \defgroup dataAccess Protected Module's Data Access API
/// This group provides API to access remote register in read write modes
/// 
/// </summary> 
/// \ingroup CanDevice_Module 
/// 
/// Since the access to the remote device should be made only by the 
/// class handling the communication with that module, the API for the data access 
/// shall be limited to the Subclass module implementing the communication with the device.
/// 
/// 
/// This group provides API to access remote register in read write modes:
/// - Read the STATUS registers;
/// - Write the DATA registers
/// - Write the parameter registers;
/// - Read the Special registers.
/// 


/// <summary>
/// \defgroup protocolModule Private Module's communication protocol API
/// This section provides methods and data structure to implement the communication protocol features.
/// 
/// </summary>
/// \ingroup CanDevice_Module 
/// 
/// This section provides all the internal methods and structures implementing 
/// the device module control. Those API are not accessible by the Application 
/// or the Subclass because is part of the communication protocol core implementation, 
/// shared with all the device specific modules.
/// 
/// 
/// 

/// <summary>
/// \defgroup subclassModule Protected Subclasses methods
/// 
/// This section provides the method and structures to subclass the CanDeviceProtocol base class .
/// 
/// </summary>
/// \ingroup CanDevice_Module 
/// 
/// 
/// 
/// 

/// <summary>
/// \defgroup debugModule Public Debug Methods
/// 
/// This section provides debug feature to monitor the communication with the 
/// remote device and in general internal module performances.
/// 
/// </summary>
///  \ingroup CanDevice_Module 
/// 
/// The available tools are:
/// + CanDeviceCommunicationMonitor: this is a dedicated class that makes statistics 
/// on the data exchanges with the remote device to monitor the quality of the communication.
/// 
/// 
/// 


using namespace System::Collections::Generic;
using namespace System::Threading;

/// <summary>
/// \ingroup CanDevice_Module 
/// 
/// </summary>
ref class CanDeviceProtocol //: public System::Windows::Forms::Form
{
public:

	CanDeviceProtocol(unsigned char devid, LPCWSTR devname); //! Standard constructor

	/// <summary>
	/// \ingroup protocolModule
	/// 
	/// This is the content of the Special Command Status register,
	/// reporting the execuion status of a pending command.
	/// 
	/// </summary>
	private:enum class CommandRegisterStatus {
		COMMAND_EXECUTING = 1,  //!< The Command is still in execution 
		COMMAND_TERMINATED,		//!< The Command is successfully terminated 
		COMMAND_ERROR,			//!< The Command is terminated with an error condition
	};

	/// <summary>
	/// \ingroup commanControl
	/// This is the enumeration of the possible command executed  errors.
	/// 
	/// NOTE: 
	/// 
	/// </summary>
	public:enum class CommandRegisterErrors {
		COMMAND_NO_ERROR = 0,		//!< Protocol Reserved code for the command successfully completed 
		COMMAND_ERROR_BUSY,			//!< Protocol Reserved code for the command busy
		COMMAND_ERROR_INVALID_PARAM,//!< Protocol Reserved code for the command with invalid parameter
		COMMAND_ERROR_MOMENTARY_DISABLED,//!< Protocol Reserved code for the command momentary disabled
		
		COMMAND_INVALID_DEVICE=253,		//!< Command requested from an invalid thread
		COMMAND_COMMUNICATION_ERROR=254,//!< Command failed due to communication error
		COMMAND_DEVICE_TMO = 255		//!< Command execution timeout
	};

	/// <summary>
	/// This is the command Code of the protocol frames
	///  \ingroup protocolModule
	/// 
	/// </summary>
	private:enum class ProtocolFrameCode {
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

	// Bootloader commands
	#define GET_BOOTLOADER_INFO (System::Byte) 0x1, (System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,true
	#define GET_COMMAND_REGISTER (System::Byte) 0, (System::Byte) 3,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,false

	/// <summary>
	/// \ingroup moduleControl
	/// 
	/// This is the enumeration of the Module's internal operating status.\n
	/// See the getModuleStatus() method.
	/// </summary>
	/// 
	public:enum class status_options {
		WAITING_CAN_DRIVER_CONNECTION = 0,	//!< The Device is waiting the Can Driver connection		
		WAITING_REVISION,					//!< The module is waiting for the revision acquisition
		DEVICE_CONFIGURATION,				//!< The module is uploading the device parameters
		DEVICE_RUNNING,						//!< The module is Running		
		DEVICE_SIMULATOR,					//!< Device in Simulation mode
		LEN,
		UNDEF = LEN
	};


	/// <summary>
	/// \ingroup moduleControl
	/// 
	/// This is the enumeration calss defining the current status of the remote bootloader activity.
	/// </summary>
	public:enum class bootloader_options {
		BOOTLOADER_NOT_PRESENT = 0, //<! The Bootloader is not present in the remote device
		BOOTLOADER_RUNNING = 1,//<! The Bootloader is present and running in the remote device
		BOOTLOADER_PRESENT = 2,//<! The Bootloader is present and not running in the remote device
		BOOTLOADER_UNCKNOWN_STAT//<! Invalid bootloader status
	};


	/// <summary>
	/// This is the data structure handling the protocol device register data.
	/// 
	/// \ingroup dataAccess
	/// </summary>	
	protected:ref class Register {
	public:
		
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

		/// <summary>
		/// Creates a Register with all the internal data set to 0.
		/// </summary>
		/// <param name=""></param>
		Register(void) {
			d0 = d1 = d2 = d3 =0; 
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

	/// <summary>
	/// This class provides a data structure for the can device frame communication protocol.
	/// 
	/// \ingroup dataAccess
	/// 
	/// 
	/// </summary>
	protected: ref class CanDeviceRegister {
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

		void set(unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, unsigned char d4, unsigned char d5, unsigned char d6, unsigned char d7, bool bootl) {
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

	/// <summary>
	/// This class provides an interface to monitor the communication performance with the device.
	/// 
	/// \ingroup debugModule
	/// </summary>
	/// 
	/// Every frame is sent and after every message received, 
	/// the internal statistic is updated with the following infos:
	/// + Tx-Rx mean time: this is the mean time of the time elapsed from the Tx event to the Rx event;
	/// + Perc5: this is the percentage of elapsed time less than 5ms;
	/// + Perc10: this is the percentage of elapsed time less than 10ms;
	///	+ Perc15: this is the percentage of elapsed time less than 15ms;
	/// + Perc20: this is the percentage of elapsed time less than 20ms;
	/// + Perc25: this is the percentage of elapsed time less than 25ms;
	/// + Perc30: this is the percentage of elapsed time less than 30ms;
	/// + Percxx: this is the percentage of elapsed time exceeding 30ms;
	/// 
	/// The Statistics are updated every 100 messages.
	/// 
	protected: ref class CanDeviceCommunicationMonitor {
	public:

		/// <summary>
		/// Initializes the internal caounters
		/// </summary>
		/// <param name=""></param>
		CanDeviceCommunicationMonitor(void) {
			
			sent_messages = 0;
			sent_5 = 0;
			sent_10 = 0;
			sent_15 = 0;
			sent_20 = 0;
			sent_25 = 0;
			sent_30 = 0;
			sent_xx = 0;

			
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

				meanTime = 0;
				sent_5 = 0;
				sent_10 = 0;
				sent_15 = 0;
				sent_20 = 0;
				sent_25 = 0;
				sent_30 = 0;
				sent_xx = 0;
				sent_messages = 0;

			};
		}

		/// <summary>
		/// Returns a formatted string with the calculated statistics
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		System::String^ getStatFormattedString(void) {
			log_string += " [T]:" + ((int)percMeanTime).ToString();
			log_string += " [5]:" + ((int)perc5).ToString();
			log_string += " [10]:" + ((int)perc10).ToString();
			log_string += " [15]:" + ((int)perc15).ToString();
			log_string += " [20]:" + ((int)perc20).ToString();
			log_string += " [25]:" + ((int)perc25).ToString();
			log_string += " [30]:" + ((int)perc30).ToString();
			log_string += " [>30]:" + ((int)percXX).ToString();
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

		double perc5;	//!< Percent of messages with less than 5ms of tx-rx time 
		double perc10;	//!< Percent of messages with less than 10ms of tx-rx time 
		double perc15;  //!< Percent of messages with less than 15ms of tx-rx time 
		double perc20;  //!< Percent of messages with less than 20ms of tx-rx time 
		double perc25;	//!< Percent of messages with less than 25ms of tx-rx time 
		double perc30;	//!< Percent of messages with less than 30ms of tx-rx time 
		double percXX;  //!< Percent of messages exceeding 30ms of tx-rx time 

		double meanTime;	//!< Total Rx-Tx Mean time 
		double percMeanTime;//!< Percent of the Tx-Rx mean time

		unsigned long unreceived_messages; //!< Number of unreceived messages
		System::String^ log_string;//!< Pre formatted string statistic
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

public:

	/// <summary>
	/// This function activate the Module in Real mode.
	/// </summary>
	/// \ingroup moduleControl
	/// <param name=""></param>
	void runMode(void) { simulator_mode = false; run = true; }

	/// <summary>
	/// This function activate the Module in Simulation mode
	/// </summary>
	/// \ingroup moduleControl
	/// 
	/// <param name=""></param>
	void simulMode(void) { simulator_mode = true; run = true; }

	/// <summary>
	/// This function tests if the module is running in simulation mode
	/// </summary>
	/// \ingroup moduleControl
	/// 
	/// <param name=""></param>
	/// <returns>return true if the moduleis in simulation mode</returns>
	bool inline isSimulatorMode(void) { return simulator_mode; }

	/// <summary>
	/// This function tests if the module is in communication error condition
	/// </summary>
	/// \ingroup moduleControl
	/// 
	/// <param name=""></param>
	/// <returns></returns>	
	inline bool isCommunicationError(void) { return communication_error; }

	/// <summary>
	/// This function returns the Module operating status
	/// </summary>
	/// \ingroup moduleControl
	/// 
	/// <param name=""></param>
	/// <returns>return the current module operating status. See the status_options status enumeration</returns>
	inline status_options getModuleStatus(void) { return internal_status; }

	/// <summary>
	/// This function returns the current status of the bootloader activities on the remote device.
	/// </summary>
	/// \ingroup moduleControl
	/// 
	/// <param name=""></param>
	/// <returns></returns>
	inline bootloader_options getBootStatus(void) { return (bootloader_options) bootloader_status; }

	/// <summary>
	/// This function returns a string with the current detected Bootloader revision code.
	/// </summary>
	/// \ingroup moduleControl
	/// 
	/// The bootloader revision code is available only after the module startup 
	/// when the module initialization is completed.
	/// 
	/// <param name=""></param>
	/// <returns></returns>
	inline System::String^ getBootRevision(void) { return boot_maj.ToString() + "." + boot_min.ToString() + "." + boot_sub.ToString(); }

	/// <summary>
	/// This function returns a string with the current detected remote firmware revision code.
	/// </summary>
	/// \ingroup moduleControl
	/// 
	/// The firmware revision code is available only after the module startup 
	/// when the module initialization is completed.
	/// 
	/// <param name=""></param>
	/// <returns></returns>
	inline System::String^ getAppRevision(void) { return app_maj.ToString() + "." + app_min.ToString() + "." + app_sub.ToString(); }
	

	
	
	/// <summary>
	///  This is a non blocking thread safe function sending a command to the remote device.
	/// 
	/// </summary>
	///  \ingroup commanControl
	/// 
	/// 
	/// The function prepares the data structures to allow the device module to 
	/// handle the command execution.
	/// 
	///	The calling thread shall poll the command completion status using the following methods:
	///			+ CanDeviceProtocol::isCommandCompleted(): checks if the command is terminated;
	/// 		+ CanDeviceProtocol::isCommandError(): checks if the command has been terminated with error condition;
	/// 
	/// The calling thread shall rerive the command result with the following methods:
	///			+ CanDeviceProtocol::getCommandResult0(): returns the result byte 0;
	///			+ CanDeviceProtocol::getCommandResult1(): returns the result byte 1;
	///			+ CanDeviceProtocol::getCommandError():   returns the error code;	
	///  
	/// 
	/// <param name="code"> This is the protocol command code</param>
	/// <param name="d0">this is the protocol data 0 of the command</param>
	/// <param name="d1">this is the protocol data 1 of the command</param>
	/// <param name="d2">this is the protocol data 2 of the command</param>
	/// <param name="d3">this is the protocol data 3 of the command</param>
	/// <param name="tmo">this is the timeout in milliseconds for the command completion</param>
	/// <returns>true if the command can be executed</returns>
	bool commandNoWaitCompletion(unsigned char code, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, int tmo);
	
	
	/// <summary>
	/// This is a blocking thread safe function sending a command to the remote device.
	/// 
	/// </summary>
	/// \ingroup commanControl
	/// 
	/// 
	/// IMPORTANT: This function cannot be called inside the running loop of the 
	/// sublcass thread handling the communication with the remote device!
	/// 
	/// This function is thrade safe so it can be called by more different threads,
	/// because it is protected by a mutex.
	/// 
	/// The function:
	/// + prepares for the command execution;
	/// + waits for the command completion;
	/// + returns the result in a proper data structure;
	/// 
	/// NOTE: The calling thread remain locked during the command completion.
	/// 
	/// <param name="code">This is the protocol command code</param>
	/// <param name="d0">This is the data-0 passed to the command</param>
	/// <param name="d1">This is the data-1 passed to the command</param>
	/// <param name="d2">This is the data-2 passed to the command</param>
	/// <param name="d3">This is the data-3 passed to the command</param>
	/// <param name="tmo">This is the timeout in milliseconds</param>
	/// <param name="src">This is the device handler calling the procedure</param>
	/// <returns>The handler to the result class</returns>
	CanDeviceCommandResult^ commandWaitCompletion(unsigned char code, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, int tmo, Object^ src);

	/// <summary>
	/// This function returns the current status of the pending command.
	/// </summary>
	/// 
	/// <param name=""></param>
	/// \ingroup commanControl
	/// 
	/// NOTE: the command is completed event when it completes with an error condition.
	/// 
	/// <returns>true if the command is completed</returns>
	bool isCommandCompleted(void) { return !command_executing; }

	/// <summary>
	/// This function returns true if the command is terminated with an error condition.
	/// </summary>
	/// \ingroup commanControl
	/// 
	/// This function should be called only when the command is terminated. See isCommandCompleted()
	/// 
	/// <param name=""></param>
	/// <returns>true if the command has been completed with an error condition</returns>
	bool isCommandError(void) { return (command_error != (unsigned char)CommandRegisterErrors::COMMAND_NO_ERROR); }
	
	/// <summary>
	/// In case of command completed without errors, this function returns the command returned data-0.
	/// </summary>
	/// \ingroup commanControl
	/// 
	/// <param name=""></param>
	/// <returns>Command completed returned data-0 code</returns>
	inline unsigned char getCommandResult0(void) { return command_ris0; }

	/// <summary>
	/// In case of command completed without errors, this function returns the command returned data-1.
	/// </summary>
	/// \ingroup commanControl
	/// 
	/// <param name=""></param>
	/// <returns>Command completed returned data-1 code</returns>
	inline unsigned char getCommandResult1(void) { return command_ris1; }

	/// <summary>
	/// This function returns the last error condition of an executed command.
	/// 
	/// </summary>
	/// \ingroup commanControl
	/// 
	/// <param name=""></param>
	/// <returns>Command completed error code</returns>
	inline CommandRegisterErrors getCommandError(void) { return (CommandRegisterErrors) command_error; }

protected:
	
	CanDeviceCommunicationMonitor communication; //!< This is the debug class 

	/// <summary>
	/// This function executes the read of the special device command register.
	/// </summary>
	/// \ingroup dataAccess
	/// 
	/// <param name=""></param>
	/// <returns>The device command register content or nullptr in case of communication error.</returns>
	Register^ readCommandRegister(void);

	/// <summary>
	/// This function executes the read of the special device error register.
	/// </summary>
	/// \ingroup dataAccess
	/// 
	/// <param name=""></param>
	/// <returns>The device error register content or nullptr in case of communication error</returns>
	Register^ readErrorRegister(void);

	/// <summary>
	///  This function executes the read of the STATUS register of given INDEX.
	/// </summary>
	/// \ingroup dataAccess
	/// 
	/// <param name="index">Index of the  target Status register</param>
	/// <returns>The device STATUS Register(index) content or nullptr in case of communication error</returns>
	Register^ readStatusRegister(unsigned char index);

	/// <summary>
	/// This function executes the read of the DATA register of given INDEX.
	/// </summary>
	/// \ingroup dataAccess
	/// 
	/// <param name="index">Index of the  target Data register</param>
	/// <returns>The device STATUS Data(index) content or nullptr in case of communication error</returns>
	Register^ readDataRegister(unsigned char index);

	/// <summary>
	/// This function executes the write of the PARAMETER(index).
	/// </summary>
	/// \ingroup dataAccess
	/// 
	/// The device will store in EEPROM the parameter if it should be changed.
	/// 
	/// <param name="index">Index of the  target Parameter register</param>
	/// <param name="d0">D0 to be written</param>
	/// <param name="d1">D1 to be written</param>
	/// <param name="d2">D2 to be written</param>
	/// <param name="d3">D3 to be written</param>
	/// <returns>true in case of success</returns>
	bool writeParamRegister(unsigned char index, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3);

	/// <summary>
	/// This function executes the write of the PARAMETER(index).
	/// </summary>
	/// \ingroup dataAccess
	/// 
	/// The device will store in EEPROM the parameter if it should be changed.
	/// 
	/// <param name="idx">Index of the  target Parameter register</param>
	/// <param name="reg">Register structure to be stored</param>
	/// <returns></returns>
	inline bool writeParamRegister(unsigned char idx, Register^ reg) { return writeParamRegister(idx, reg->d0, reg->d1, reg->d2, reg->d3); }

	/// <summary>
	/// This function executes the write of the DATA(index).
	/// </summary>
	/// \ingroup dataAccess
	/// 
	/// 
	/// 
	/// <param name="index">Index of the  target Data register</param>
	/// <param name="d0">D0 to be written</param>
	/// <param name="d1">D1 to be written</param>
	/// <param name="d2">D2 to be written</param>
	/// <param name="d3">D3 to be written</param>
	/// <returns>true in case of success</returns>
	bool writeDataRegister(unsigned char index, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3);

	/// <summary>
	/// This function executes the write of the DATA(index).
	/// </summary>
	/// \ingroup dataAccess
	/// 	
	/// 
	/// <param name="idx">Index of the  target Data register</param>
	/// <param name="reg">Register structure to be stored</param>
	/// <returns></returns>
	inline bool writeDataRegister(unsigned char idx, Register^ reg) { return writeDataRegister(idx, reg->d0, reg->d1, reg->d2, reg->d3); }
	
	/// <summary>
	/// This function returns the last read register.
	/// 
	/// </summary>
	/// \ingroup dataAccess
	/// 
	/// <param name=""></param>
	/// <returns>The last valid read register content</returns>
	inline CanDeviceProtocol::CanDeviceRegister^ getRxRegister(void) { return rx_register; }
	
	/// <summary>
	/// This routine shall be overridden by the subclass module in order to 
	/// inplement the specific module features.
	///  
	/// </summary>
	/// \ingroup subclassModule
	/// 
	/// <param name=""></param>
	virtual void runningLoop(void) ;

	/// <summary>
	/// This routine shall be overridden by the subclass module in order to
	/// implement the simulated features.
	///  
	/// </summary>
	/// \ingroup subclassModule
	/// 
	/// <param name=""></param>
	virtual void demoLoop(void) { return; };

	/// <summary>
	/// This routine shall be overridden by the subclass module in order to
	/// implement the specific operations in case of the remote device reset detection.
	///  
	/// </summary>
	/// \ingroup subclassModule
	/// 
	/// <param name=""></param>
	virtual void resetLoop(void);

	/// <summary>
	/// This routine shall be overridden by the subclass module in order to
	/// implement the specific operations during the device configuration fase
	///  
	/// </summary>
	/// \ingroup subclassModule
	/// 
	/// <param name=""></param>
	virtual bool configurationLoop(void);

	

private:
	
	/// <summary>
	/// This function sends a frame the the remote device
	/// </summary>
	/// \ingroup protocolModule
	/// 
	/// The function sends a frame to the remote device and waits for a valid answer or timeout condition.
	/// 
	/// <param name="d0">Frame Data 0</param>
	/// <param name="d1">Frame Data 1</param>
	/// <param name="d2">Frame Data 2</param>
	/// <param name="d3">Frame Data 3</param>
	/// <param name="d4">Frame Data 4</param>
	/// <param name="d5">Frame Data 5</param>
	/// <param name="d6">Frame Data 6</param>
	/// <param name="d7">Frame Data 7</param>
	/// <param name="bootl">True if the frame is destinated to the bootloader.</param>
	/// <returns>true if the frame is successfully sent</returns>
	bool send(unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, unsigned char d4, unsigned char d5, unsigned char d6, unsigned char d7, bool bootl);
	
	/// <summary>
	/// This is the callback to be connected to the CAN reception event.
	/// </summary>
	/// \ingroup protocolModule
	/// 
	/// This callback is called in a different thread than this module:
	/// precautions shall be implemented in order to be thread safe.
	/// 
	/// <param name="canid">This is the canId identifier address</param>
	/// <param name="data">Thi is the pointer to the received data frame (8 bytes max) </param>
	/// <param name="len">This is the lenght of the buffer</param>
	void thread_can_rx_callback(unsigned short canid, unsigned char* data, unsigned char len);
	
	Thread^ main_thread; //!< Module internal main thread handle
	void mainWorker(void);//!< Main Thread routine activated by the Module's Thread
	void InternalRunningLoop(void); //!< Internal Loop routine called when the device result configured and correctly running

	unsigned short device_id; //!< Device ID assigned by the Subclass to handle a target remote device
	bool simulator_mode;	  //!< True if the module has been activated in simulation mode
	bool run;				  //!< True if the module has been activated in real mode

	bool register_access_fault; //!< True if a series of attempt to send frames to the device should be failed
	int register_access_fault_counter;//!< Counter variable to count the consecutive failed attempt to send frame to the remote device

	static unsigned char rx_sequence; //!< Current frame sequence number
	HANDLE rxEvent; //!< Event object signaled by the receiving callback
	bool rx_pending; //!< A reception data is pending 
	bool device_reset;//!< A Device reset code has been received
	CanDeviceRegister^ tx_register; //!< Last Transmitted  register
	CanDeviceRegister^ rx_register; //!< Last Received register

	status_options internal_status; //!> This is the current module internal running status

	// Target device revision set
	unsigned char boot_maj; //!< Received Bootloader Major Revision code
	unsigned char boot_min; //!< Received Bootloader Minor Revision code
	unsigned char boot_sub; //!< Received Bootloader Sub Revision code
	unsigned char app_maj;  //!< Received Application Major Revision code
	unsigned char app_min;	//!< Received Application Minor Revision code
	unsigned char app_sub;	//!< Received Application Sub Revision code
	unsigned char bootloader_status;//!< Received Bootloader running status

	bool communication_error;//!< The communication is set to error condition
	bool rxOk;			     //!< A frame has been successfullly received
	int attempt;			 //!< Number of transmission attempts

	// Command execution section
	bool		  command_executing; //!< A command is executing
	int			  command_tmo;		 //!< 100ms command timeout
	unsigned char command_code;		 //!< Command executing code
	unsigned char command_d0;		 //!< Command Executing D0 data
	unsigned char command_d1;		 //!< Command Executing D1 data
	unsigned char command_d2;	     //!< Command Executing D2 data
	unsigned char command_d3;        //!< Command Executing D3 data
	unsigned char command_ris0;      //!< Command Executed Result-0 data
	unsigned char command_ris1;      //!< Command Executed Result-1 data
	unsigned char command_error;    //!< Command Executed error data



};

