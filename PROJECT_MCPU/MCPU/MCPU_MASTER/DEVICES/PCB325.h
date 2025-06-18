#pragma once
#include "CanDeviceProtocol.h"



/// <summary>
/// \defgroup  PCB325Module PCB325 -Biopsy Device- Module
/// \ingroup CanDevice_Module  
/// This section describes the module controlling the Bipsy device.
///
/// </summary>
/// 
/// # General Information
/// 
/// + Device Address: 0x15
/// + Device Bootloader Address:0x115
/// + Cane Device Handle: PCB325::device
/// 
/// # Module Workflow Description
/// 
/// 
/// 

/// <summary>
/// \defgroup BiopModuleDescription Device Description
/// \ingroup PCB325Module
/// 
/// This section describes the Biopsy device behavior 
/// </summary>
/// 
/// 

///  <summary>
/// \ingroup PCB325Module
/// This is the class implementing the biopsy module controller
/// 
/// </summary>
/// 
/// # Module Workflow Description
/// 

ref class PCB325 : public CanDeviceProtocol
{


public:

	PCB325() : CanDeviceProtocol(0x15, L"BIOPSY_DEVICE")
	{

	}
	static PCB325^ device = gcnew PCB325();

	/// <summary>
	/// \defgroup BiopConstant Module Constants Definition
	/// \ingroup PCB325Module
	/// 
	/// This section defines constants variables of the module
	/// </summary>
	/// 
	

	/// 
	/// <summary>
	/// \ingroup BiopConstant
	/// Device Module Constant Definitions
	/// </summary>
	ref class ConstantDefinition {
	public:
		literal unsigned short BIOPSY_TARGET_X_RANGE = 5; //!< Minimum distance from the acceptable target  
		literal unsigned short BIOPSY_TARGET_Y_RANGE = 5; //!< Minimum distance from the acceptable target  
		literal unsigned short BIOPSY_TARGET_Z_RANGE = 5; //!< Minimum distance from the acceptable target  
		literal unsigned short BIOPSY_MAX_X_POSITION = 2580; //!< Maximum axes position  
		literal unsigned short BIOPSY_MAX_Y_POSITION = 700; //!< Maximum axes position  
		literal unsigned short BIOPSY_MAX_Z_POSITION = 1350; //!< Maximum axes position  
		literal unsigned short BIOPSY_ZBODY_WIDTH = 1000;
		literal unsigned short BIOPSY_YBODY_WIDTH = 300;
	};


	/// <summary>
	/// \defgroup BiopProtocol Can Device Protocol Description
	/// \ingroup PCB325Module
	/// 
	/// This class implements the protocol data structure as described in the Biopsy Device Protocol Specification.
	/// </summary>
	/// 
	/// The Protocol class provides the access to the device registers.
	/// 
	
	/// <summary>
	/// \ingroup BiopProtocol
	/// </summary>
	ref class ProtocolStructure {
	public:

		/// <summary>
		/// Status Register Definition
		/// </summary>
		ref class StatusRegister {
		public:

			/// <summary>
			///  This is the enumeration of the status register available INDEXES
			/// </summary>
			enum class register_index {
				SYSTEM_STATUS = 0,	//!> Internal statuis flag register 
				SYSTEM_XY,			//!> Current XY pointer position
				SYSTEM_ZS,			//!> Current Z pointer and Slider position
				NUM_REGISTER
			};

			/// <summary>
			///  This is the enumeration of the current device working mode.\n
			///  see \ref StatusRegister::device_working_mode
			/// 
			/// </summary>
			enum class device_mode {
				DEVICE_DISABLE_MODE = 0, //!> The Device is in Disable mode
				DEVICE_CALIBRATION_MODE, //!> The Device is in Calibration mode
				DEVICE_COMMAND_MODE,	 //!> The Device is in Command mode
				DEVICE_SERVICE_MODE		 //!> The Device is in Service mode	
			};

			/// <summary>
			///  This is the enumeration of the xscroll status.\n
			///  see \ref StatusRegister::xscroll_detected
			/// 
			/// </summary>
			enum class xscroll_code {
				XSCROLL_UNDEF = 0,	//!> The xscroll position is undefined
				XSCROLL_CENTER,		//!> The xscroll position is CENTER
				XSCROLL_LEFT,		//!> The xscroll position is LEFT
				XSCROLL_RIGHT,		//!> The xscroll position is RIGHT

			};

			/// <summary>
			///  This is the enumeration of the needle detected status.\n
			///  see \ref StatusRegister::needle_detected
			/// 
			/// </summary>
			enum class needle_code {
				NEEDLE_A = 0,	//!> The detected needle is type A
				NEEDLE_B,		//!> The detected needle is type B
				NEEDLE_C,		//!> The detected needle is type C
				NEEDLE_NOT_PRESENT	//!> The needle is not detetected
			};


			/// <summary>
			/// This is the method to decode the content of the register_index::SYSTEM_STATUS  downloaded from the device.
			/// 
			/// </summary>
			/// 
			/// The MCPU module shall call the following method to update the SYSTEM_STATUS register:
			/// + protocol.status_register.decodeSystemRegister(readStatusRegister((unsigned char) ProtocolStructure::StatusRegister::register_index::SYSTEM_STATUS));
			/// 
			/// The following data will be decoded:
			/// + \ref device_working_mode;
			/// + \ref motor_power_sw;
			/// + \ref motor_general_enable;
			/// + \ref motor_keyb_enable;
			/// + \ref motor_needle_enable;
			/// + \ref keystep_mode;
			/// + \ref yup_detected;
			/// + \ref xscroll_detected;
			/// + \ref needle_detected;
			/// 
			/// <param name="sys"></param>
			/// <returns>true: success</returns>
			static bool decodeSystemRegister(Register^ sys) {
				if (sys == nullptr) return false;

				// Byte 0 
				device_working_mode = (device_mode) sys->d0;

				// Byte 1
				motor_power_sw = sys->d1 & 0x1;
				motor_general_enable = sys->d1 & 0x2;
				motor_keyb_enable = sys->d1 & 0x4;
				motor_needle_enable = sys->d1 & 0x8;

				// Byte 2
				keystep_mode = sys->d2 & 0x1;
				yup_detected = sys->d2 & 0x2;
				xscroll_detected = (xscroll_code) ((sys->d2 >> 2) & 0x3);
				needle_detected = (needle_code) ((sys->d2 >> 4) & 0xF);

				return true;
			}

			
			/// <summary>
			/// This method shall be called by the SIMULATOR to encode the SYSTEM_STATUS register in a Can Device Frame Register.
			/// </summary>
			/// 
			/// <param name=""></param>
			/// The following data will be encode:
			/// + \ref device_working_mode;
			/// + \ref motor_power_sw;
			/// + \ref motor_general_enable;
			/// + \ref motor_keyb_enable;
			/// + \ref motor_needle_enable;
			/// + \ref keystep_mode;
			/// + \ref yup_detected;
			/// + \ref xscroll_detected;
			/// + \ref needle_detected;
			/// 
			/// <returns>The Register^ with the encoded data</returns>
			static Register^ encodeSystemRegister(void) {
				Register^ sys = gcnew Register;

				// Byte 0
				sys->d0 = (unsigned char) device_working_mode;

				// Byte 1
				if (motor_power_sw) sys->d1 |= 0x1;
				if (motor_general_enable)  sys->d1 |= 0x2;
				if (motor_keyb_enable) sys->d1 |= 0x4;
				if (motor_needle_enable)  sys->d1 |= 0x8;

				// Byte 2
				if (keystep_mode)  sys->d2 |= 0x1;
				if (yup_detected)  sys->d2 |= 0x2;
				sys->d2 |= (((unsigned char)xscroll_detected) & 0x3) << 2;
				sys->d2 |= (((unsigned char)needle_detected) & 0xF) << 4;

				return sys;
			}

			/// <summary>
			/// This is the method to decode the content of the register_index::SYSTEM_XY  downloaded from the device.
			/// 
			/// </summary>
			/// 
			/// The MCPU module shall call the following method to update the SYSTEM_XY register:
			/// + protocol.status_register.decodeXYRegister(readStatusRegister((unsigned char) ProtocolStructure::StatusRegister::register_index::SYSTEM_XY));
			/// 
			/// The following data will be decoded:
			/// + \ref x_position;
			/// + \ref y_position;			
			/// 
			/// <param name="sys"></param>
			/// <returns>true: success</returns>
			static bool decodeXYRegister(Register^ sys) {
				if (sys == nullptr) return false;

				// Byte 0 + 1
				x_position = (unsigned short)sys->d0 + 256 * (unsigned short)sys->d1;

				// Byte 2 + 3
				y_position = (unsigned short)sys->d2 + 256 * (unsigned short)sys->d3;

				return true;
			}

			/// <summary>
			/// This method shall be called by the SIMULATOR to encode the SYSTEM_XY register in a Can Device Frame Register.
			/// </summary>
			/// 
			/// <param name=""></param>
			/// 
			/// The following data will be encoded:
			/// + \ref x_position;
			/// + \ref y_position;			
			/// 
			/// <returns>The Register^ with the encoded data</returns>
			static Register^ encodeXYRegister(void) {
				Register^ sys = gcnew Register;

				// Byte 0
				sys->d0 = (unsigned char)x_position;
				sys->d1 = (unsigned char)(x_position >> 8);
				sys->d2 = (unsigned char)y_position;
				sys->d3 = (unsigned char)(y_position >> 8);

				return sys;
			}

			/// <summary>
			/// This is the method to decode the content of the register_index::SYSTEM_ZS  downloaded from the device.
			/// 
			/// </summary>
			/// 
			/// The MCPU module shall call the following method to update the SYSTEM_ZS register:
			/// + protocol.status_register.decodeZSRegister(readStatusRegister((unsigned char) ProtocolStructure::StatusRegister::register_index::SYSTEM_ZS));
			/// 
			/// The following data will be decoded:
			/// + \ref z_position;
			/// + \ref s_position;			
			/// 
			/// <param name="sys"></param>
			/// <returns>true: success</returns>
			static bool decodeZSRegister(Register^ sys) {
				if (sys == nullptr) return false;

				// Byte 0 + 1
				z_position = (unsigned short)sys->d0 + 256 * (unsigned short)sys->d1;

				// Byte 2 + 3
				s_position = (unsigned short)sys->d2 + 256 * (unsigned short)sys->d3;

				return true;
			}

			/// <summary>
			/// This method shall be called by the SIMULATOR to encode the SYSTEM_ZS register in a Can Device Frame Register.
			/// </summary>
			/// 
			/// <param name=""></param>
			/// 
			/// The following data will be encoded:
			/// + \ref x_position;
			/// + \ref y_position;			
			/// 
			/// <returns>The Register^ with the encoded data</returns>
			static Register^ encodeZSRegister(void) {
				Register^ sys = gcnew Register;

				// Byte 0
				sys->d0 = (unsigned char)z_position;
				sys->d1 = (unsigned char)(z_position >> 8);
				sys->d2 = (unsigned char)s_position;
				sys->d3 = (unsigned char)(s_position >> 8);

				return sys;
			}


			
			static device_mode device_working_mode; //!< SYSTEM_STATUS: this is the device current working mode
			static xscroll_code xscroll_detected;	//!< SYSTEM_STATUS: this is the device current x-scroll position
			static needle_code  needle_detected;	//!< SYSTEM_STATUS: this is the device current needle detected code
			
			/// <summary>
			/// SYSTEM_STATUS:motor_power_sw: this is the status of the power relay 
			/// </summary>
			/// 
			/// \note The Power Switch logic activation is as follows:
			/// + SWITCH = motor_general_enable && [( motor_keyb_enable || motor_needle_enable)]
			/// 
			static bool motor_power_sw;				
			
			/// <summary>
			/// SYSTEM_STATUS:motor_general_enable: this is the status of the general enable flag 
			/// </summary>
			/// 
			/// \note The Power Switch logic activation is as follows:
			/// + SWITCH = motor_general_enable && [( motor_keyb_enable || motor_needle_enable)]
			/// 
			static bool motor_general_enable;		

			/// <summary>
			/// SYSTEM_STATUS:motor_keyb_enable: this is the status of the keyboard motor enable flag
			/// </summary>
			/// 
			/// \note The Power Switch logic activation is as follows:
			/// + SWITCH = motor_general_enable && [( motor_keyb_enable || motor_needle_enable)]
			/// 
			static bool motor_keyb_enable;			

			/// <summary>
			/// SYSTEM_STATUS:motor_needle_enable: this is the status of the needle motor enable flag
			/// </summary>
			/// 
			/// \note The Power Switch logic activation is as follows:
			/// + SWITCH = motor_general_enable && [( motor_keyb_enable || motor_needle_enable)]
			/// 
			static bool motor_needle_enable;		

			/// <summary>
			/// SYSTEM_STATUS:keystep_mode: this is the key_step enabled flag 
			/// </summary>
			/// 
			/// If the keystep_mode )) true, the keyboard is operating in step mode. 
			/// 
			static bool keystep_mode;				

			static bool yup_detected;				//!< SYSTEM_STATUS: this is the YUP detected flag
			

			// XY Status
			static unsigned short x_position;	//!< SYSTEM_XY: this is the x axis position (0.1 mm)
			static unsigned short y_position;   //!< SYSTEM_XY: this is the y axis position (0.1 mm)

			// ZS Status
			static unsigned short z_position;	//!< SYSTEM_ZS: this is the z axis position (0.1 mm)
			static unsigned short s_position;	//!< SYSTEM_ZS: this is the sh axis position (0.1 mm)

		};


		/// <summary>
		/// Data Register Definition: no data register are defined for this device
		/// </summary>
		ref class DataRegister {
		public:

			enum class register_index {
				NUM_REGISTER
			};


		};

		/// <summary>
		/// Param Register Definition: no  parameters are defined for this device
		/// </summary>
		ref class ParameterRegister {
		public:

			enum class register_index {
				NUM_REGISTERS = 0,
			};

			
		};

		/// <summary>
		/// Protocol Command Definition
		/// </summary>
		ref class Commands {
		public:
			/// <summary>
			/// This is the command index enumeration
			/// </summary>
			enum class command_index {
				CMD_ABORT = 0,          //!< Abort Activation Command
				CMD_DISABLE_MODE,		//!< Disable Mode Activation Command
				CMD_COMMAND_MODE,		//!< Command Mode Activation Command
				CMD_SERVICE_MODE,		//!< Service Mode Activation Command
				CMD_CALIB_MODE,			//!< Calibration Mode Activation Command
				CMD_MOVE_X,				//!< Move X axis command
				CMD_MOVE_Y,				//!< Move Y axis command
				CMD_MOVE_Z,				//!< Move Z axis command
				CMD_ENABLE_KEYSTEP,		//!< Enable keyboard step mode
				CMD_SERVICE_TEST_CYCLE	//!< Cycle Test Command Activation
			};

			/// <summary>
			/// This function encode the data for the  CMD_DISABLE_MODE
			/// </summary>
			/// 
			/// Use the commandNoWaitCompletion(protocol.command.encodeDisableModeCommand(), Timeout);
			/// to send the command to the device.
			/// 
			/// <param name=""></param>
			/// <returns>The encoded device command</returns>
			CanDeviceCommand^ encodeDisableModeCommand(void) {				
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_DISABLE_MODE, 0, 0, 0, 0);
			}

			/// <summary>
			/// This function encode the data for the  CMD_COMMAND_MODE
			/// </summary>
			/// 
			/// Use the commandNoWaitCompletion(protocol.command.encodeCommandModeCommand(), Timeout);
			/// to send the command to the device.
			/// 
			/// <param name=""></param>
			/// <returns>The encoded device command</returns>
			CanDeviceCommand^ encodeCommandModeCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_COMMAND_MODE, 0, 0, 0, 0);
			}

			/// <summary>
			/// This function encode the data for the  CMD_SERVICE_MODE
			/// </summary>
			/// 
			/// Use the commandNoWaitCompletion(protocol.command.encodeServiceModeCommand(), Timeout);
			/// to send the command to the device.
			/// 
			/// <param name=""></param>
			/// <returns>The encoded device command</returns>
			CanDeviceCommand^ encodeServiceModeCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_SERVICE_MODE, 0, 0, 0, 0);
			}

			/// <summary>
			/// This function encode the data for the  CMD_CALIB_MODE
			/// </summary>
			/// 
			/// Use the commandNoWaitCompletion(protocol.command.encodeCalibModeCommand(), Timeout);
			/// to send the command to the device.
			/// 
			/// <param name=""></param>
			/// <returns>The encoded device command</returns>
			CanDeviceCommand^ encodeCalibModeCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_CALIB_MODE, 0, 0, 0, 0);
			}

			/// <summary>
			/// This function encode the data for the  CMD_SERVICE_TEST_CYCLE
			/// </summary>
			/// 
			/// Use the commandNoWaitCompletion(protocol.command.encodeTestCycleModeCommand(), Timeout);
			/// to send the command to the device.
			/// 
			/// <param name=""></param>
			/// <returns>The encoded device command</returns>
			CanDeviceCommand^ encodeTestCycleModeCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_SERVICE_TEST_CYCLE, 0, 0, 0, 0);
			}

			/// <summary>
			/// This function encode the data for the  CMD_MOVE_X(x)
			/// </summary>
			/// 
			/// Use the commandNoWaitCompletion(protocol.command.encodeMoveXCommand(X), Timeout);
			/// to send the command to the device.
			/// 
			/// <param name="X">This is the X coordinate (0.1mm units)</param>
			/// <returns>The encoded device command</returns>			
			CanDeviceCommand^ encodeMoveXCommand(int X) {			
				System::Byte d0 = (System::Byte)(X & 0xFF);
				System::Byte d1 = (System::Byte)((X>>8) & 0xFF);
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_MOVE_X, d0, d1, 0, 0);
			}

			/// <summary>
			/// This function encode the data for the  CMD_MOVE_Y(Y)
			/// </summary>
			/// 
			/// Use the commandNoWaitCompletion(protocol.command.encodeMoveYCommand(Y), Timeout);
			/// to send the command to the device.
			/// 
			/// <param name="Y">This is the Y coordinate (0.1mm units)</param>
			/// <returns>The encoded device command</returns>			
			CanDeviceCommand^ encodeMoveYCommand(int Y) {
				System::Byte d0 = (System::Byte)(Y & 0xFF);
				System::Byte d1 = (System::Byte)((Y >> 8) & 0xFF);
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_MOVE_Y, d0, d1, 0, 0);
			}

			/// <summary>
			/// This function encode the data for the  CMD_MOVE_Z(Z)
			/// </summary>
			/// 
			/// Use the commandNoWaitCompletion(protocol.command.encodeMoveZCommand(Z), Timeout);
			/// to send the command to the device.
			/// 
			/// <param name="Z">This is the Z coordinate (0.1mm units)</param>
			/// <returns>The encoded device command</returns>			
			CanDeviceCommand^ encodeMoveZCommand(int Z) {
				System::Byte d0 = (System::Byte)(Z & 0xFF);
				System::Byte d1 = (System::Byte)((Z >> 8) & 0xFF);
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_MOVE_Z, d0, d1,0, 0);
			}

			/// <summary>
			/// This function encode the data for the  CMD_ENABLE_KEYSTEP(enable)
			/// </summary>
			/// 
			/// Use the commandNoWaitCompletion(protocol.command.encodeKeyStepEnableCommand(enable), Timeout);
			/// to send the command to the device.
			/// 
			/// <param name="enable">true: activate the keyboard in step mode</param>
			/// <returns>The encoded device command</returns>			
			CanDeviceCommand^ encodeKeyStepEnableCommand(bool enable) {
				
				System::Byte d0 = (enable) ? 1 : 0;
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_ENABLE_KEYSTEP, d0, 0, 0, 0);
			}

		};

		static StatusRegister status_register; //!< This is the Status register structure content
		static DataRegister data_register;//!< This is the Data register structure content
		static ParameterRegister parameter_register;//!< This is the Parameter register structure content
		static Commands command;//!< This is the Command register structure content
	};

	/// <summary>
	/// \defgroup BiopCommandWoorkingModeApi Device Working Mode Commands
	/// \ingroup PCB325Module
	/// 
	/// This section provides methods to control the device working mode
	/// </summary>
	/// @{ 
	

	/// <summary>
	/// This method request the activation of the Device FREE WORKING MODE
	/// </summary>
	/// <param name=""></param>
	static void setBiopsyFreeMode(void) {
		request_working_mode = working_mode::BIOPSY_FREE_MODE; 
	}

	/// <summary>
	/// This method request the activation of the Device Disable Mode
	/// </summary>
	/// <param name=""></param>
	static void setBiopsyDisableMode(void) {
		request_working_mode = working_mode::BIOPSY_DISABLE_MODE; 
	}

	/// <summary>
	/// This method request the activation of the Device Calibration Mode
	/// </summary>
	/// <param name=""></param>
	static void setBiopsyCalibrationMode(void) {
		request_working_mode = working_mode::BIOPSY_CALIBRATION_MODE; 
	}

	/// <summary>
	/// This method request the activation of the Device Command Mode
	/// </summary>
	/// <param name=""></param>
	static void setBiopsyCommandMode(void) {
		request_working_mode = working_mode::BIOPSY_COMMAND_MODE; 
	}

	/// <summary>
	/// This method request the activation of the Device Service Mode
	/// </summary>
	/// <param name=""></param>
	static void setBiopsyServiceMode(void) {
		request_working_mode = working_mode::BIOPSY_SERVICE_MODE; 
	}

	/// <summary>
	/// This method Enable Disables the Keyboard Step Mode
	/// </summary>
	/// <param name=""></param>
	static void setKeyStepMode(bool ena) { 
		keystep_mode = ena; 
	}
	/// @}

	/// <summary>
	/// \defgroup BiopCommandMovingApi Axis Activation Commands
	/// \ingroup PCB325Module
	/// 
	/// This section provides methods to activate the Axis to target positions
	/// </summary>
	/// @{ 
	
	
	/// <summary>
	/// This is the command to activate the X axis to a target.
	/// </summary>
	/// 
	/// This method requests to the module the executiion of the command.
	/// The command will be sent to the device as soon as the module's working thread
	/// should get in execution.
	/// 
	/// <param name="x">the target X coordinate (0.1mm units)</param>
	/// <returns>true: the command has been accepted</returns>
	static bool moveX(unsigned short x) {
		if (!command_mode_ready) return false;
		if (request_motor_command != waiting_motor_command) return false;

		request_motor_command = motor_command::MOTOR_X;
		request_x = x;
		motor_command_result = 0;
		return true;
	}

	/// <summary>
	/// This is the command to activate the Y axis to a target.
	/// </summary>
	/// 
	/// This method requests to the module the execution of the command.
	/// The command will be sent to the device as soon as the module's working thread
	/// should get in execution.
	/// 
	/// <param name="x">the target Y coordinate (0.1mm units)</param>
	/// <returns>true: the command has been accepted</returns>
	static bool moveY(unsigned short y) {
		if (!command_mode_ready) return false;
		if (request_motor_command != waiting_motor_command) return false;

		request_motor_command = motor_command::MOTOR_Y;
		request_y = y;
		motor_command_result = 0;
		return true;
	}

	/// <summary>
	/// This is the command to activate the Z axis to a target.
	/// </summary>
	/// 
	/// This method requests to the module the execution of the command.
	/// The command will be sent to the device as soon as the module's working thread
	/// should get in execution.
	/// 
	/// <param name="x">the target Z coordinate (0.1mm units)</param>
	/// <returns>true: the command has been accepted</returns>
	static bool moveZ(unsigned short z) {
		if (!command_mode_ready) return false;
		if (request_motor_command != waiting_motor_command) return false;

		request_motor_command = motor_command::MOTOR_Z;
		request_z = z;
		motor_command_result = 0;
		return true;
	}

	/// <summary>
	/// This command checks if an axis is moving.
	/// </summary>
	/// <param name=""></param>
	/// <returns>true: a moving command is executing</returns>
	static bool isPointerMoving(void) {
		return (motor_command_result == 0);
	}

	/// <summary>
	/// This command checks if the last executed command terminated with success.
	/// </summary>
	/// 
	/// \note this command shall be used only after isPointerMoving() should return true. 
	/// <param name=""></param>
	/// <returns>true: the last command terminated with success</returns>
	static bool isPointerSuccessfullyMoved(void) {
		return (motor_command_result == 1);
	}

	/// @}
	
	/// <summary>
	/// This command returns the current position of the Shift axis in 0.1mm units
	/// </summary>
	/// <param name=""></param>
	/// <returns>The Shift axis in 0.1mm units</returns>
	static unsigned short getS(void) { 
		return protocol.status_register.s_position; 
	}
	
	/// <summary>
	/// Returns the current connection status with the Biopsy device
	/// </summary>
	/// <param name=""></param>
	/// <returns>true: connected</returns>
	static bool isBiopsyConnected(void) {
		return biopsy_connected;
	}
	
	/// <summary>
	/// \defgroup BiopPositionAxisApi Position Control Commands
	/// \ingroup PCB325Module
	/// 
	/// This section provides methods to control the Axes position
	/// </summary>
	/// @{ 

	/// <summary>
	/// This function returns the current X coordinate
	/// </summary>
	/// <param name=""></param>
	/// <returns>X coordinates (0.1mm units)</returns>
	static unsigned short getX(void) {
		return protocol.status_register.x_position;
	}

	/// <summary>
	/// This function returns the current Y coordinate
	/// </summary>
	/// <param name=""></param>
	/// <returns>Y coordinates (0.1mm units)</returns>
	static unsigned short getY(void) {
		return protocol.status_register.y_position;
	}

	/// <summary>
	/// This function returns the current Z coordinate
	/// </summary>
	/// <param name=""></param>
	/// <returns>Z coordinates (0.1mm units)</returns>
	static unsigned short getZ(void) {
		return protocol.status_register.z_position;
	}

	/// <summary>
	/// This method checks if the current X axis should be already in the selected target
	/// </summary>
	/// 
	/// <param name="tg">coordinate of the target (0.1mm)</param>
	/// <returns>true: the axis is in the target range</returns>
	static bool isXtarget(unsigned short tg) {
		if (tg > protocol.status_register.x_position) {
			if (tg > protocol.status_register.x_position + ConstantDefinition::BIOPSY_TARGET_X_RANGE) return false;
			return true;
		}
		else {
			if (protocol.status_register.x_position < ConstantDefinition::BIOPSY_TARGET_X_RANGE) return true;
			if (tg < protocol.status_register.x_position - ConstantDefinition::BIOPSY_TARGET_X_RANGE) return false;
			return true;
		}		
	}

	/// <summary>
	/// This method checks if the current Y axis should be already in the selected target
	/// </summary>
	/// 
	/// <param name="tg">coordinate of the target (0.1mm)</param>
	/// <returns>true: the axis is in the target range</returns>
	static bool isYtarget(unsigned short tg) {
		if (tg > protocol.status_register.y_position) {
			if (tg > protocol.status_register.y_position + ConstantDefinition::BIOPSY_TARGET_Y_RANGE) return false;
			return true;
		}
		else {
			if (protocol.status_register.y_position < ConstantDefinition::BIOPSY_TARGET_Y_RANGE) return true;
			if (tg < protocol.status_register.y_position - ConstantDefinition::BIOPSY_TARGET_Y_RANGE) return false;
			return true;
		}
	}

	/// <summary>
	/// This method checks if the current Z axis should be already in the selected target
	/// </summary>
	/// 
	/// <param name="tg">coordinate of the target (0.1mm)</param>
	/// <returns>true: the axis is in the target range</returns>
	static bool isZtarget(unsigned short tg) {
		if (tg > protocol.status_register.z_position) {
			if (tg > protocol.status_register.z_position + ConstantDefinition::BIOPSY_TARGET_Z_RANGE) return false;
			return true;
		}
		else {
			if (protocol.status_register.z_position < ConstantDefinition::BIOPSY_TARGET_Z_RANGE) return true;
			if (tg < protocol.status_register.z_position - ConstantDefinition::BIOPSY_TARGET_Z_RANGE) return false;
			return true;
		}
	}

	
	
	/// <summary>
	/// This method checks if the Y is rotated Upward
	/// </summary>
	/// <param name=""></param>
	/// <returns>true: Y is Upward</returns>
	static bool isYUp(void) { return protocol.status_register.yup_detected; }

	/// <summary>
	/// This method checks if the X-SCROLL is in Center Position
	/// </summary>
	/// <param name=""></param>
	/// <returns>true: is CENTER position</returns>
	static bool isXC(void) { 
		return (protocol.status_register.xscroll_detected == ProtocolStructure::StatusRegister::xscroll_code::XSCROLL_CENTER); 
	}

	/// <summary>
	/// This method checks if the X-SCROLL is in Left Position
	/// </summary>
	/// <param name=""></param>
	/// <returns>true: is LEFT position</returns>
	static bool isXL(void) {
		return (protocol.status_register.xscroll_detected == ProtocolStructure::StatusRegister::xscroll_code::XSCROLL_LEFT); 
	}

	/// <summary>
	/// This method checks if the X-SCROLL is in Right Position
	/// </summary>
	/// <param name=""></param>
	/// <returns>true: is RIGHT position</returns>
	static bool isXR(void) {
		return (protocol.status_register.xscroll_detected == ProtocolStructure::StatusRegister::xscroll_code::XSCROLL_RIGHT); 
	}

	/// <summary>
	/// This method checks if the X-SCROLL is in Undefined
	/// </summary>
	/// <param name=""></param>
	/// <returns>true: is UNDEFINED </returns>
	static bool isXU(void) {
		return (protocol.status_register.xscroll_detected == ProtocolStructure::StatusRegister::xscroll_code::XSCROLL_UNDEF); 
	}
	
	/// @}


	// Group Y position evaluation respect the Z group
	static bool isGroupYleftThanBody() { 
		
		int xmax_pointer, xmin_pointer, xmax_base, xmin_base;

		// Position of the Y body in X coordinate
		xmax_pointer = protocol.status_register.x_position + (ConstantDefinition::BIOPSY_YBODY_WIDTH / 2);
		xmin_pointer = protocol.status_register.x_position - (ConstantDefinition::BIOPSY_YBODY_WIDTH / 2);

		// Find the coordinates of the ZBODY in X coordinate
		if (isXC()) {
			xmin_base = ConstantDefinition::BIOPSY_MAX_X_POSITION / 2 - (ConstantDefinition::BIOPSY_ZBODY_WIDTH / 2);
			xmax_base = ConstantDefinition::BIOPSY_MAX_X_POSITION / 2 + (ConstantDefinition::BIOPSY_ZBODY_WIDTH / 2);
		}
		else if (isXL()) {
			xmin_base = ConstantDefinition::BIOPSY_MAX_X_POSITION - ConstantDefinition::BIOPSY_ZBODY_WIDTH;
			xmax_base = ConstantDefinition::BIOPSY_MAX_X_POSITION;
		}
		else if (isXR() ) {			
			xmin_base = 0;
			xmax_base = (ConstantDefinition::BIOPSY_ZBODY_WIDTH);
		}
		if (xmin_pointer > xmax_base) return true;
		return false;

		/*
		if ((xmin_pointer <= xmax_base) && (xmin_pointer >= xmin_base)) device.crash_event = true;
		else if ((xmax_pointer <= xmax_base) && (xmax_pointer >= xmin_base)) device.crash_event = true;
		else device.crash_event = false;
		*/
	}
	
	

	static bool isGroupYrightThanBody()  {

		int xmax_pointer, xmin_pointer, xmax_base, xmin_base;

		// Position of the Y body in X coordinate
		xmax_pointer = protocol.status_register.x_position + (ConstantDefinition::BIOPSY_YBODY_WIDTH / 2);
		xmin_pointer = protocol.status_register.x_position - (ConstantDefinition::BIOPSY_YBODY_WIDTH / 2);

		// Find the coordinates of the ZBODY in X coordinate
		if (isXC()) {
			xmin_base = ConstantDefinition::BIOPSY_MAX_X_POSITION / 2 - (ConstantDefinition::BIOPSY_ZBODY_WIDTH / 2);
			xmax_base = ConstantDefinition::BIOPSY_MAX_X_POSITION / 2 + (ConstantDefinition::BIOPSY_ZBODY_WIDTH / 2);
		}
		else if (isXL()) {
			xmin_base = ConstantDefinition::BIOPSY_MAX_X_POSITION - ConstantDefinition::BIOPSY_ZBODY_WIDTH;
			xmax_base = ConstantDefinition::BIOPSY_MAX_X_POSITION;

		}
		else if (isXR()) {
			xmin_base = 0;
			xmax_base = (ConstantDefinition::BIOPSY_ZBODY_WIDTH);
		}
		if (xmax_pointer < xmin_base) return true;
		return false;
		/*
		if ((xmin_pointer <= xmax_base) && (xmin_pointer >= xmin_base)) device.crash_event = true;
		else if ((xmax_pointer <= xmax_base) && (xmax_pointer >= xmin_base)) device.crash_event = true;
		else device.crash_event = false;
		*/
	}
	
	static bool testXPossibleCrash(int target_x) {
		
		// No crash if the Y is UP
		if (isYUp()) return false;

		int xmax_pointer, xmin_pointer, xmax_base, xmin_base;

		// Position of the Y body in X coordinate
		xmax_pointer = protocol.status_register.x_position + (ConstantDefinition::BIOPSY_YBODY_WIDTH / 2);
		xmin_pointer = protocol.status_register.x_position - (ConstantDefinition::BIOPSY_YBODY_WIDTH / 2);

		// Find the coordinates of the ZBODY in X coordinate
		if (isXC()) {
			xmin_base = ConstantDefinition::BIOPSY_MAX_X_POSITION / 2 - (ConstantDefinition::BIOPSY_ZBODY_WIDTH / 2);
			xmax_base = ConstantDefinition::BIOPSY_MAX_X_POSITION / 2 + (ConstantDefinition::BIOPSY_ZBODY_WIDTH / 2);
		}
		else if (isXL()) {
			xmin_base = ConstantDefinition::BIOPSY_MAX_X_POSITION - ConstantDefinition::BIOPSY_ZBODY_WIDTH;
			xmax_base = ConstantDefinition::BIOPSY_MAX_X_POSITION;

		}
		else if (isXR()) {
			xmin_base = 0;
			xmax_base = (ConstantDefinition::BIOPSY_ZBODY_WIDTH);
		}

		if (xmax_pointer < xmin_base) {
			// Y is Right of the Body 
			if (target_x < protocol.status_register.x_position) return false;
			if ((target_x - protocol.status_register.x_position) > (xmin_base - xmax_pointer)) return true; // crash
			return false;
		}
		else if (xmin_pointer > xmax_base) {
			// Y is Left of the Body 
			if (target_x > protocol.status_register.x_position) return false;
			if ((protocol.status_register.x_position - target_x ) > (xmin_pointer - xmax_base)) return true; // crash
			return false;

		}
		
		return true; // Crash 
		
	}

	static bool moveXHomeLeft(void) { return moveX(ConstantDefinition::BIOPSY_MAX_X_POSITION); }
	static bool moveXHomeRight(void) { return moveX(0); }


	static bool activateCycleTest(void);
	
	//static void setLight(bool status, int duty) { protocol.data_register.light_status = status;  protocol.data_register.light_duty = duty; }
	static void moveHome(void) { command_home_request = true; }
	/// @}

protected: 	
	void runningLoop(void) override;
	bool configurationLoop(void) override;//!< This is the Base Class override function to handle the device configuration

private:
	static ProtocolStructure protocol; //!< This is the structure with the Status register info
	static bool biopsy_connected = false;
	static bool keystep_mode = false;
	static bool command_mode_ready = false;

	static bool service_cycle_test = false;
	static bool command_home_request = false;
	static int sequence = 0;

	
	enum class working_mode {
		BIOPSY_FREE_MODE = 0,			//!< The Biopsy device is left operating in self defined mode (calibration/disable)
		BIOPSY_DISABLE_MODE,		//!< The Biopsy is forced to be in disable mode
		BIOPSY_CALIBRATION_MODE,	//!< The Biopsy is forced to be in calibration mode
		BIOPSY_COMMAND_MODE,		//!< The Biopsy is forced to be in command mode
		BIOPSY_SERVICE_MODE			//!< The Biopsy is forced to be in service mode
	};

	static working_mode request_working_mode = working_mode::BIOPSY_FREE_MODE;
	static working_mode biopsy_working_mode = working_mode::BIOPSY_FREE_MODE;

	enum class motor_command {
		NO_COMMAND = 0,
		MOTOR_X,
		MOTOR_Y,
		MOTOR_Z,
	};

	static motor_command request_motor_command;
	static motor_command waiting_motor_command;
	static unsigned short request_x, request_y, request_z;
	static int motor_command_result;

	void run_free_mode_workflow(void); 
	void run_disable_mode_workflow(void);
	void run_calibration_mode_workflow(void);
	void run_command_mode_workflow(void);
	void run_service_mode_workflow(void);
	void resetPositioningErrorMessages(void);
	
};

