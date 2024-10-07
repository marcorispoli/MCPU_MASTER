#pragma once
#include "CanDeviceProtocol.h"

/// <summary>
/// \defgroup PCB303_Module PCB303 Module: format collimation
/// \ingroup CanDevice_Module 
/// This module implements the functions of the Collimator device
/// 
/// - 2D Format collimation;
/// - Tomo Dynamic Collimation;
/// 
/// The module implements the Device Communication protocol 
/// to communicate with the PCB303 device through the CAN bus.
/// 
/// In order to implement the protocol, the module inherits the 
/// CanDeviceProtocol class as the base class.
/// 
/// The module overrides the configurationLoop() routine of the Base class 
/// in order to writes the initialization Parameters and Data Registers as needed.\n 
/// The routine is automatically called by the Base class after system startup or always after a device reset event.
/// 
/// This module overrides the runningLoop() rotine of the base class in order 
/// to implement the device specific functions.
/// 
/// As soon as the module class is Instantiated, the constructor will call the 
/// Base class constructor.\n
/// The Base class constructor, through the CAN Driver module, tries to connect the 
/// Board Device getting the Bootloader and Application revision codes.
/// 
/// When the Device connection is established and the Board Device firmware revision is acquired,\n
/// the Base class calls the configurationLoop() routine (overridden in this module),\n
/// in order to let the module to upload and store the Device Parameters and the deviice DATA registers.\n
/// In detail, the moudule in the configurationLoop() routine:
/// 
/// - Uploads the Standard Collimation format parameters;
/// - Uploads the Tomo collimation format parameters;
/// 
/// When the configuration fase termines, the module runs the runningLoop() routine (overridden from the Base class),\n
/// to handle the normal workflow.
/// 
///		NOTE: as soon as the startup termines, the Module automatically sets the OPEN collimation mode.
/// 
/// In the normal module workflow - runningLoop() - the module controls the Collimator device insuring that the current \n
/// Application requested collimation format is selected and operating in the collimator device.
/// 
/// In case of the actual collimation shouldn't corrispond with the requested collimation,\n
/// the module automatically tries for a maximum of 5 times to set the correct collimation.\n
/// In case the number of attempts should expire, the module will disable any further attempt and\n
/// an error condition is activated (is up to the application to handle the fault condition).
/// 
///		NOTE: In case of error, the Application can call the resetFault() routine to restart the collimation attempts.
/// 
///  
/// # Application Interface 
/// 
/// The module provides the following Application interface:
/// 
/// - setAutoCollimationMode() : This function sets the format collimation to AUTO mode;
/// - setOpenCollimationMode(): This function sets the format collimation to OPEN mode;
/// - setCalibrationCollimationMode():  This function sets the format collimation to CALIBRATION mode;
/// - setCustomCollimationMode(): This function sets the format collimation to CUSTOM mode; 
/// - resetFaults(): This function allows the Module to attempt ones more to execute a collimation, in case of fault condition;
/// - getError(): This function returns an error code in case of the collimator should be in error condition;
/// 
/// 
/// # 2D Collimation Format Selection specification
/// 
/// The module allows the Application to select one of the following 2D collimation modes:
/// 
/// - OPEN collimation mode;
/// - AUTO collimation mode; 
/// - CUSTOM collimation mode;
/// - CALIBRATION collimation mode; 
/// 
/// ## OPEN Collimation Mode
/// 
/// If the Application should select the OPEN collimation mode,\n
/// the Module will set the Collimator blades to 0. This is a so called Open \n
/// position where the collimator doesn't act any collimation on the X-RAYS.\n
/// 
/// This collimation mode should be used for test or calibration purpose.
/// 
///		NOTE: this collimation mode is automatically set by the Module after the system startup. 
///
/// ## AUTO Collimation Mode
/// 
/// If the Application should select the AUTO collimation mode,\n
///	the Module sets the current collimation format based on the current detected compressor Paddle.
///  
/// The Paddle is assigned to a given collimation format into the PaddleCalibration file. 
/// 
/// This collimation mode should be set by the Application in the Operating workflow.
/// 
/// ## CUSTOM Collimation Mode
/// 
/// If the Application should select the CUSTOM collimation mode,\n
///	the Module sets the current collimation format equal to the format directly 
/// set by the Application with the proper interface command - setCustomCalibraitonMode(customFormat) -.
/// 
/// The custom collimation format can be selected from the 20 predefined collimation formats.
/// 
/// ## CALIBRATION Collimation Mode
/// 
/// If the Application should select the CALIBRATION collimation mode,\n
/// the module will force the current blades position to get a non standard value imposed by the application.
/// 
/// The Application requests this Non Standard collimation using the proper\n 
/// interface function passing the value of the target blades positons as parameter.
///	
/// 
/// # Collimator Calibration file
/// 
/// The Application can calibrate a maximum of 20 possibles standard 2D collimation formats.\n
/// All those formats shares the same Trap and Front blade positions because they should not change\n 
/// along with the paddle geometry. 
/// 
/// See the CollimatorConfig class for details.
/// 
/// 
/// # Format collimation errors
/// 
/// In case the collimator should fail in setting the requested collimation mode,
/// the module automatically tries to repeat the collimation command for a max of five times.
/// 
/// If the command repetion should fail, the Module disables further collimation selection attempts
/// and a persistent error condition will be notified in the system.
/// 
/// - The Application can check an error condition presence calling the getError() function;
/// 
/// - In case of Error, the Application may restart the collimation attempts calling the resetFault() function; 
/// 
/// </summary>

/// <summary>
/// \defgroup PCB303_Protocol Protocol implementation
/// \ingroup PCB303_Module
/// This section implements the internal module data and structures in order to 
/// implement the protocol detail specifications related to the PCB303 board. 
/// 
/// 
/// </summary>



/// \defgroup PCB303_Internal Internal implementation section
/// \ingroup PCB303_Module

/// \defgroup PCB303_Interface Application Interface
/// \ingroup PCB303_Module

/// <summary>
/// This class implements the functions of the PCB303 Module
/// \ingroup PCB303_Module
/// 
/// </summary>
ref class PCB303 : public CanDeviceProtocol
{
private:

	/// <summary>
/// This class implement the protocol data structure as described in the protocol specification.
/// </summary>
	ref class ProtocolStructure {
	public:

		

		ref class StatusRegister {
		public:

			enum class register_index {
				SYSTEM_REGISTER = 0, //!> This is the System Status register index
				TUBE_REGISTER,				
			};

			enum class status_code {
				STAT_UNDEFINED = 0, //!> The position of the device is undefined
				STAT_POSIIONING,	//!> A positioning command is executing
				STAT_POSITIONED		//!> The position of the device is undefined
			};

			static status_code collimation_status;
			static unsigned char collimation_index;
			static status_code filter_status;
			static status_code mirror_status;
			static bool light_status;

			unsigned char stator_temp;
			unsigned char bulb_temp;
			bool stator_high;
			bool stator_fault;
			bool bulb_high;
			bool bulb_fault;

			static bool decodeSystemRegister(Register^ sys) {
				if (sys == nullptr) return false;

				// Byte 0 of the register
				collimation_status = (status_code) (sys->d0 & 0x7);
				collimation_index = (sys->d0  >> 3);

				manual_servo_up = sys->d0 & 0x1;
				manual_servo_down = sys->d0 & 0x2;
				manual_pedal_up = sys->d0 & 0x4;
				manual_pedal_down = sys->d0 & 0x8;

				// Byte 1
				idle_status = sys->d1 & 0x1;
				manual_activation = sys->d1 & 0x2;
				pedal_activation = sys->d1 & 0x4;
				command_activation = sys->d1 & 0x8;
				upward_direction = sys->d1 & 0x10;
				downward_direction = sys->d1 & 0x20;

				device_fault = sys->d1 & 0x80;

				// Byte 2
				compression_ena = sys->d2 & 0x1;
				compression_on = sys->d2 & 0x2;

				smart_target = sys->d2 & 0x8;
				force_target = sys->d2 & 0x10;
				limit_compression = sys->d2 & 0x20;

				// Byte 3
				if (sys->d3 >= (unsigned char)ComponentCode::UNDETECTED_COMPONENT) component = ComponentCode::UNDETECTED_COMPONENT;
				else component = (ComponentCode)sys->d3;
				return true;
			}

			Register^ encodeSystemRegister(void) {

				// Creates a register with all bytes set to 0
				CanDeviceProtocol::Register^ sys = gcnew CanDeviceProtocol::Register;

				// Byte 0 of the register
				if (manual_servo_up) sys->d0 |= 0x1;
				if (manual_servo_down) sys->d0 |= 0x2;
				if (manual_pedal_up)sys->d0 |= 0x4;
				if (manual_pedal_down)sys->d0 |= 0x8;

				// Byte 1
				if (idle_status)sys->d1 |= 0x1;
				if (manual_activation)sys->d1 |= 0x2;
				if (pedal_activation) sys->d1 |= 0x4;
				if (command_activation) sys->d1 |= 0x8;
				if (upward_direction) sys->d1 |= 0x10;
				if (downward_direction) sys->d1 |= 0x20;

				if (device_fault) sys->d1 |= 0x80;

				// Byte 2
				if (compression_ena) sys->d2 |= 0x1;
				if (compression_on) sys->d2 |= 0x2;

				if (smart_target)sys->d2 |= 0x8;
				if (force_target) sys->d2 |= 0x10;
				if (limit_compression)sys->d2 |= 0x20;

				// Byte 3
				sys->d3 = (unsigned char)component;

				// Returns the formatted register
				return sys;
			}



			static bool decodePaddleRegister(Register^ pad) {
				if (pad == nullptr) return false;
				paddle_position = (int)pad->d0 + 256 * (int)(pad->d1 & 0x0f);
				paddle_force = (int)((pad->d1 & 0xF0) >> 4) + 16 * (int)pad->d2;
				paddle_tag = pad->d3;
				return true;
			}

			Register^ encodePaddleRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ pad = gcnew Register;

				pad->d0 = (unsigned char)(paddle_position & 0xFF);
				pad->d1 = (unsigned char)((paddle_position >> 8) & 0x0f);
				pad->d1 |= ((unsigned char)(paddle_force & 0x0F) << 4);
				pad->d2 = (unsigned char)((paddle_force >> 4) & 0xff);
				pad->d3 = paddle_tag;

				// Returns the formatted register
				return pad;
			}

			static bool decodeRawPaddleRegister(Register^ rpad) {
				if (rpad == nullptr) return false;

				paddle_raw_position = (int)rpad->d0 + 256 * (int)(rpad->d1 & 0x0f);
				paddle_raw_force = (int)((rpad->d1 & 0xF0) >> 4) + 16 * (int)rpad->d2;
				paddle_raw_code = rpad->d3;
				return true;
			}

			Register^ encodeRawPaddleRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ pad = gcnew Register;

				pad->d0 = (unsigned char)(paddle_raw_position & 0xFF);
				pad->d1 = (unsigned char)((paddle_raw_position >> 8) & 0x0f);
				pad->d1 |= ((unsigned char)(paddle_raw_force & 0x0F) >> 4);
				pad->d2 = (unsigned char)((paddle_raw_force >> 4) & 0xff);
				pad->d3 = paddle_raw_code;

				// Returns the formatted register
				return pad;
			}


			static bool manual_servo_up = false; //!< The Manual Servo Up button is activated
			static bool manual_servo_down = false;//!< The Manual Servo Down button is activated
			static bool manual_pedal_up = false;//!< The Manual Compression Pedal Up button is activated
			static bool manual_pedal_down = false;//!< The Manual Compression Pedal Down button is activated

			static bool idle_status = false;//!< The Compressor is in Idle (no activation is pending)
			static bool manual_activation = false;//!< The Servo manual activation is executing
			static bool pedal_activation = false;//!< The Pedal manual activation is executing
			static bool command_activation = false;//!< The Protocol command  activation is executing
			static bool upward_direction = false;//!< The paddle is activated Upward
			static bool downward_direction = false;//!< The paddle is activated Downward

			static bool device_fault = false;//!< The device is in fault condition (see the Error register)

			static bool compression_ena = false;//!< The compressor Ena hardware input is detected
			static bool compression_on = false;	//!< A valid compression is detected (Compression-On)

			static bool smart_target = false;//!< The Smart uPress has been detected
			static bool force_target = false;//!< The Target compression is detected
			static bool limit_compression = false;//!< The Limit compression is detected

			static int paddle_position = 0; //! Current calibrated paddle position
			static int paddle_force = 0;//! Current calibrated paddle force
			static unsigned char paddle_tag = 0;//! Current detected paddle tag

			static int paddle_raw_position = 0;//! Current sensor position value
			static int paddle_raw_force = 0;//! Current sensor force value
			static unsigned char paddle_raw_code = 0;//! Current paddle code

			static ComponentCode component = ComponentCode::UNDETECTED_COMPONENT; //!< Current detected Component code

		};


		ref class DataRegister {
		public:

			enum class register_index {
				HOLDER_LIMITS = 0,
				COMPRESSOR_LIMITS,
				PADDLE_WEIGHT
			};

			static bool decodeHolderLimitsRegister(Register^ reg) {
				if (reg == nullptr) return false;
				max_position = reg->d0 + 256 * reg->d1;
				min_position = reg->d2 + 256 * reg->d3;
				return true;
			}

			Register^ encodeHolderLimitsRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ out = gcnew Register;

				out->d0 = (unsigned char)max_position;
				out->d1 = (unsigned char)(max_position >> 8);
				out->d2 = (unsigned char)min_position;
				out->d3 = (unsigned char)(min_position >> 8);


				// Returns the formatted register
				return out;
			}

			static bool decodeCompressorLimitsRegister(Register^ reg) {
				if (reg == nullptr) return false;
				limit_compression = reg->d0;
				target_compression = reg->d1;
				return true;
			}


			Register^ encodeCompressorLimitsRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ out = gcnew Register;

				out->d0 = (unsigned char)limit_compression;
				out->d1 = (unsigned char)target_compression;

				// Returns the formatted register
				return out;
			}

			static bool decodePaddleWeightRegister(Register^ reg) {
				if (reg == nullptr) return false;
				paddle_weight = reg->d0;
				paddle_offset = reg->d1;
				absolute_arm_angle = reg->d2;
				magnifier_offset = reg->d3;
				return true;
			}

			Register^ encodePaddleWeightRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ out = gcnew Register;

				out->d0 = paddle_weight;
				out->d1 = paddle_offset;
				out->d2 = absolute_arm_angle;
				out->d3 = magnifier_offset;


				// Returns the formatted register
				return out;
			}


			static unsigned char limit_compression = 200;//!< Current limit compression
			static unsigned char target_compression = 150;//!< Current target compression

			static unsigned short max_position = 0; //!< Max Holder position
			static unsigned short min_position = 0; //!< Min holder position

			static unsigned char paddle_weight = 0;//!< Current detected paddle weight
			static unsigned char magnifier_offset = 0; //!< Offset in case of magnifier
			static unsigned char paddle_offset = 0;	//!< Mechanical offset of the detected paddle
			static unsigned char absolute_arm_angle = 0;//!< Current detected Arm angle

		};

		ref class ParameterRegister {
		public:
			enum class register_index {
				HOLDER_CALIB = 0,
				COMPRESSION_CALIB
			};

			static bool decodeHolderCalibRegister(Register^ reg) {
				if (reg == nullptr) return false;
				Kp = reg->d0;
				Op = reg->d1 + 256 * reg->d2;
				return true;
			}

			Register^ encodeHolderCalibRegister(void) {
				unsigned char d0 = Kp;
				unsigned char d1 = (unsigned char)Op;
				unsigned char d2 = (unsigned char)(Op >> 8);
				unsigned char d3 = 0;
				return gcnew Register(d0, d1, d2, d3);
			}

			static bool decodeCompressionCalibRegister(Register^ reg) {
				if (reg == nullptr) return false;

				return true;
			}

			Register^ encodeCompressionCalibRegister(void) {

				unsigned char d0 = 0;
				unsigned char d1 = 0;
				unsigned char d2 = 0;
				unsigned char d3 = 0;
				return gcnew Register(d0, d1, d2, d3);
			}


			static unsigned char Kp;
			static unsigned short Op;

		};

		ref class Commands {
		public:
			enum class command_index {
				ABORT_COMMAND = 0, //!< Abort Command (mandatory as for device protocol)
				SET_TRIMMERS_COMMAND,
				SET_COMPRESSION,
				SET_UNLOCK,
			};

			CanDeviceCommand^ encodeSetUnlockCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::SET_UNLOCK, 0, 0, 0, 0);
			}

		};

		static StatusRegister status_register;
		static DataRegister data_register;
		static ParameterRegister parameter_register;
		static Commands command;
	};


	
	/// \ingroup PCB303_Protocol	
	/// 
	///@{

	#define PCB303_ERROR_LEFT(reg)	(reg->d2 & 0x1) //!< This is the Left blade collimation error
	#define PCB303_ERROR_RIGHT(reg)	(reg->d2 & 0x2) //!< This is the Right blade collimation error
	#define PCB303_ERROR_FRONT(reg)	(reg->d2 & 0x4) //!< This is the Front blade collimation error
	#define PCB303_ERROR_BACK(reg)	(reg->d2 & 0x8) //!< This is the Back blade collimation error
	#define PCB303_ERROR_TRAP(reg)	(reg->d2 & 0x10) //!< This is the Trap blade collimation error


	#define PCB303_GET_SYSTEM_COLLIMATION_STATUS(reg) (unsigned char) (reg->d0)//!< This macro selects the COLLIMATION_STATUS byte from the received Status Register
	#define PCB303_GET_SYSTEM_FORMAT_INDEX(reg) (unsigned char) (reg->d1)//!< This macro selects the FORMAT_INDEX byte from the received Status register Can Frame
	#define PCB303_GET_SYSTEM_TOMO_PULSE(reg) (unsigned char) (reg->d2)//!< This macro selects the TOMO_PULSE byte from the received Status register Can Frame	
	#define PCB303_GET_SYSTEM_FLAGS(reg) (unsigned char) (reg->d3)//!< This macro selects the SYSTEM_FLAGS byte from the received Status register Can Frame
	#define PCB303_SYSTEM_FLAG_ERRORS 0x1 //!< This macro selects the ERROR_FLAGS from the SYSTEM_FLAGS byte	
	
	#define PCB303_GET_SYSTEM_FB_FRONT(reg) ((unsigned short) reg->d0 + 256 * (unsigned short) reg->d1) //!< This macro selects the Front blade position 
	#define PCB303_GET_SYSTEM_FB_BACK(reg) ((unsigned short) reg->d2 + 256 * (unsigned short) reg->d3) //!< This macro selects the Back blade position 	
	#define PCB303_GET_SYSTEM_LR_LEFT(reg) ((unsigned short) reg->d0 + 256 * (unsigned short) reg->d1) //!< This macro selects the Left blade position 
	#define PCB303_GET_SYSTEM_LR_RIGHT(reg) ((unsigned short) reg->d2 + 256 * (unsigned short) reg->d3) //!< This macro selects the Right blade position 
	#define PCB303_GET_SYSTEM_T_TRAP(reg) ((unsigned short) reg->d0 + 256 * (unsigned short) reg->d1) //!< This macro selects the Trap blade position

	 
	
	/// <summary>	
	/// This enumeration class defines the Indexes of the protocol Status Registers
	/// </summary>
	enum class StatusRegisters {
		SYSTEM_STATUS_REGISTER = 0, //!< System status register index (see StatusDataBytes)
		SYSTEM_CURRENT_FB_REGISTER, //!< This is the current Front and Back position (if the position is valid)
		SYSTEM_CURRENT_LR_REGISTER, //!< This is the current Left and Right position (if the position is valid)
		SYSTEM_CURRENT_T_REGISTER,//!< This is the current Trap position (if the position is valid)
	};
	
	/// <summary>
	/// This enumeration class defines the data content (D0 to D3) of the System Status Register.
	/// </summary>
	enum class StatusDataBytes {
		SYSTEM_COLLIMATION_STATUS = 0, //!> Collimation status data byte see (CollimationStatusCode)
		SYSTEM_FORMAT_INDEX,		//!< Current selected standard collimation format index	(see ColliStandardSelections)
		SYSTEM_TOMO_PULSE,			//!< Current Tomo pulse processing in Tomo dynamic mode
		SYSTEM_SPARE
	};

	/// <summary>
	/// This is the SYSTEM_COLLIMATION_STATUS byte definition of the SYSTEM status register.
	/// </summary>
	enum class CollimationStatusCode {
		COLLI_STATUS_OPEN_FORMAT = 0, //!< The open format is currently selected
		COLLI_STATUS_STANDARD_FORMAT,//!< The standard format is currently selected (see the STATUS FORMAT INDEX in that case)
		COLLI_STATUS_CALIB_FORMAT,  //!< The Calibration format is currently selected
		COLLI_STATUS_TOMO_MODE,		//!< The Tomo mode is currently active
		COLLI_STATUS_OUT_OF_POSITION,//!< Not a valid collimation format is selected
		COLLI_STATUS_2D_EXECUTING,	//!< A 2D collimation format selection is executing
	};

	/// <summary>
	/// This is the SYSTEM_FORMAT_INDEX byte definition of the SYSTEM status register.
	/// This byte reports the current select collimation format.
	/// </summary>
public: enum class ColliStandardSelections {
	COLLI_NOT_STANDARD = 0, //!< When an OPEN format or CALIBRATION format is selected this is reported as a non standard format active
	COLLI_INVALID_FORMAT = COLLI_NOT_STANDARD, //!< This is the code assigned to a Not valid collimatoion format code
	COLLI_STANDARD1, //!< The current selected format is the STANDARD1 
	COLLI_STANDARD_24x30 = COLLI_STANDARD1, //!< The special code is reserved to the 24x30 collimation, assigned to the COLLI_STANDARD1
	COLLI_STANDARD2,//!< The current selected format is the STANDARD2
	COLLI_STANDARD3,//!< The current selected format is the STANDARD3 
	COLLI_STANDARD4,//!< The current selected format is the STANDARD4
	COLLI_STANDARD5,//!< The current selected format is the STANDARD5
	COLLI_STANDARD6,//!< The current selected format is the STANDARD6
	COLLI_STANDARD7,//!< The current selected format is the STANDARD7
	COLLI_STANDARD8,//!< The current selected format is the STANDARD8
	COLLI_STANDARD9,//!< The current selected format is the STANDARD9
	COLLI_STANDARD10,//!< The current selected format is the STANDARD10
	COLLI_STANDARD11,//!< The current selected format is the STANDARD11
	COLLI_STANDARD12,//!< The current selected format is the STANDARD12
	COLLI_STANDARD13,//!< The current selected format is the STANDARD13
	COLLI_STANDARD14,//!< The current selected format is the STANDARD14
	COLLI_STANDARD15,//!< The current selected format is the STANDARD15
	COLLI_STANDARD16,//!< The current selected format is the STANDARD16
	COLLI_STANDARD17,//!< The current selected format is the STANDARD17
	COLLI_STANDARD18,//!< The current selected format is the STANDARD18
	COLLI_STANDARD19,//!< The current selected format is the STANDARD19
	COLLI_STANDARD20,//!< The current selected format is the STANDARD20
	COLLI_STANDARD_LEN

	};
	
public: static ColliStandardSelections getColliFormatIndexFromParam(System::String^ param) {
	if (param == "COLLI_STANDARD1") return ColliStandardSelections::COLLI_STANDARD1;
	else if (param == "COLLI_STANDARD2") return ColliStandardSelections::COLLI_STANDARD2;
	else if (param == "COLLI_STANDARD3") return ColliStandardSelections::COLLI_STANDARD3;
	else if (param == "COLLI_STANDARD4") return ColliStandardSelections::COLLI_STANDARD4;
	else if (param == "COLLI_STANDARD5") return ColliStandardSelections::COLLI_STANDARD5;
	else if (param == "COLLI_STANDARD6") return ColliStandardSelections::COLLI_STANDARD6;
	else if (param == "COLLI_STANDARD7") return ColliStandardSelections::COLLI_STANDARD7;
	else if (param == "COLLI_STANDARD8") return ColliStandardSelections::COLLI_STANDARD8;
	else if (param == "COLLI_STANDARD9") return ColliStandardSelections::COLLI_STANDARD9;
	else if (param == "COLLI_STANDARD10") return ColliStandardSelections::COLLI_STANDARD10;
	else if (param == "COLLI_STANDARD11") return ColliStandardSelections::COLLI_STANDARD11;
	else if (param == "COLLI_STANDARD12") return ColliStandardSelections::COLLI_STANDARD12;
	else if (param == "COLLI_STANDARD13") return ColliStandardSelections::COLLI_STANDARD13;
	else if (param == "COLLI_STANDARD14") return ColliStandardSelections::COLLI_STANDARD14;
	else if (param == "COLLI_STANDARD15") return ColliStandardSelections::COLLI_STANDARD15;
	else if (param == "COLLI_STANDARD16") return ColliStandardSelections::COLLI_STANDARD16;
	else if (param == "COLLI_STANDARD17") return ColliStandardSelections::COLLI_STANDARD17;
	else if (param == "COLLI_STANDARD18") return ColliStandardSelections::COLLI_STANDARD18;
	else if (param == "COLLI_STANDARD19") return ColliStandardSelections::COLLI_STANDARD19;
	else if (param == "COLLI_STANDARD20") return ColliStandardSelections::COLLI_STANDARD20;
	else return ColliStandardSelections::COLLI_INVALID_FORMAT;

}
	/// This is the frame implementation to write the DATA_CALIBRATION_FB register
	#define PCB303_WRITE_DATA_CALIBRATION_FB(front,back) (System::Byte) DataRegisters::DATA_CALIBRATION_FB,(System::Byte) (front&0xFF), (System::Byte) ((front>>8)&0xFF),(System::Byte) (back&0xFF), (System::Byte) ((back>>8)&0xFF)
	
	/// This is the frame implementation to write the DATA_CALIBRATION_LR register
	#define PCB303_WRITE_DATA_CALIBRATION_LR(left,right) (System::Byte) DataRegisters::DATA_CALIBRATION_LR,(System::Byte) (left&0xFF), (System::Byte) ((left>>8)&0xFF),(System::Byte) (right&0xFF), (System::Byte) ((right>>8)&0xFF)
	
	/// This is the frame implementation to write the DATA_CALIBRATION_T register
	#define PCB303_WRITE_DATA_CALIBRATION_T(trap) (System::Byte) DataRegisters::DATA_CALIBRATION_T,(System::Byte) (trap&0xFF), (System::Byte) ((trap>>8)&0xFF),(System::Byte) (trap&0xFF), (System::Byte) ((trap>>8)&0xFF)

	/// <summary>
	/// This is the enumeration class definigs the indexes of the protocol DATA registers	
	/// </summary>
	enum class DataRegisters {
		DATA_CALIBRATION_FB = 0,//!< The Application shall set this register in CALIBRATION to set the Front and Back blades positions
		DATA_CALIBRATION_LR,//!< The Application shall set this register in CALIBRATION to set the Left and Right blades positions
		DATA_CALIBRATION_T,//!< The Application shall set this register in CALIBRATION to set the Trap blade positions
	};
	
	/// This is the frame implementation to write the PARAM_STANDARD_FT Parameter Register		
	#define PCB303_WRITE_PARAM_STANDARD_FT(front,trap) (System::Byte) ParamRegisters::PARAM_STANDARD_FORMAT_FT,(System::Byte) (front&0xFF), (System::Byte) ((front>>8)&0xFF),(System::Byte) (trap&0xFF), (System::Byte) ((trap>>8)&0xFF)
	/// This is the frame implementation to write the PARAM_STANDARD_LR Parameter Register
	#define PCB303_WRITE_PARAM_STANDARD_LR(index, left, right) (System::Byte) (ParamRegisters::PARAM_STANDARD_FORMAT_LR1) + (index-1) * 2, (System::Byte) (left&0xFF), (System::Byte) ((left>>8)&0xFF), (System::Byte) (right&0xFF), (System::Byte) ((right>>8)&0xFF)
	/// This is the frame implementation to write the PARAM_STANDARD_B Parameter Register
	#define PCB303_WRITE_PARAM_STANDARD_B(index, back) (System::Byte) (ParamRegisters::PARAM_STANDARD_FORMAT_B1) + (index-1) * 2 ,(System::Byte) (back&0xFF), (System::Byte) ((back>>8)&0xFF),(System::Byte) (back&0xFF), (System::Byte) ((back>>8)&0xFF)

	/// <summary>	
	/// This enumeration class defines the Indexes of the Parameters Registers
	/// </summary> 
	public: enum class ParamRegisters {
		PARAM_STANDARD_FORMAT_FT = 0,
		PARAM_STANDARD_FORMAT_LR1,
		PARAM_STANDARD_FORMAT_B1,
		PARAM_STANDARD_FORMAT_LR2,
		PARAM_STANDARD_FORMAT_B2,
		PARAM_STANDARD_FORMAT_LR3,
		PARAM_STANDARD_FORMAT_B3,
		PARAM_STANDARD_FORMAT_LR4,
		PARAM_STANDARD_FORMAT_B4,
		PARAM_STANDARD_FORMAT_LR5,
		PARAM_STANDARD_FORMAT_B5,
		PARAM_STANDARD_FORMAT_LR6,
		PARAM_STANDARD_FORMAT_B6,
		PARAM_STANDARD_FORMAT_LR7,
		PARAM_STANDARD_FORMAT_B7,
		PARAM_STANDARD_FORMAT_LR8,
		PARAM_STANDARD_FORMAT_B8,
		PARAM_STANDARD_FORMAT_LR9,
		PARAM_STANDARD_FORMAT_B9,
		PARAM_STANDARD_FORMAT_LR10,
		PARAM_STANDARD_FORMAT_B10,
		PARAM_STANDARD_FORMAT_LR11,
		PARAM_STANDARD_FORMAT_B11,
		PARAM_STANDARD_FORMAT_LR12,
		PARAM_STANDARD_FORMAT_B12,
		PARAM_STANDARD_FORMAT_LR13,
		PARAM_STANDARD_FORMAT_B13,
		PARAM_STANDARD_FORMAT_LR14,
		PARAM_STANDARD_FORMAT_B14,
		PARAM_STANDARD_FORMAT_LR15,
		PARAM_STANDARD_FORMAT_B15,
		PARAM_STANDARD_FORMAT_LR16,
		PARAM_STANDARD_FORMAT_B16,
		PARAM_STANDARD_FORMAT_LR17,
		PARAM_STANDARD_FORMAT_B17,
		PARAM_STANDARD_FORMAT_LR18,
		PARAM_STANDARD_FORMAT_B18,
		PARAM_STANDARD_FORMAT_LR19,
		PARAM_STANDARD_FORMAT_B19,
		PARAM_STANDARD_FORMAT_LR20,
		PARAM_STANDARD_FORMAT_B20,

	};

	/// <summary>	
	/// This enumeration class defines the Indexes of the Command Execution
	///
	/// </summary>
	private: enum class Commandregister {
		ABORT_COMMAND = 0, //!< Abort Command (mandatory as for device protocol)
		SET_OPEN_FORMAT_COMMAND,//!< Selects the Open Collimation
		SET_STANDARD_FORMAT_COMMAND,//!< Selects the Standard Collimation
		SET_CALIBRATION_FORMAT_COMMAND,//!< Selects the Calibration Collimation
		SET_TOMO_MODE_COMMAND//!< Selects the Tomo Collimation
	};
	
	/// This is the frame implementation to Execute ABORT_COMMAND Command
	#define PCB303_COMMAND_ABORT (System::Byte) PCB303::Commandregister::ABORT_COMMAND, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0,(System::Byte) 0
	/// This is the frame implementation to Execute the SET_OPEN_FORMAT_COMMAND Command
	#define PCB303_COMMAND_OPEN_FORMAT (System::Byte) PCB303::Commandregister::SET_OPEN_FORMAT_COMMAND, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0,(System::Byte) 0
	/// This is the frame implementation to Execute the SET_STANDARD_FORMAT_COMMAND collimation Command
	#define PCB303_COMMAND_STANDARD_FORMAT(index) (System::Byte) PCB303::Commandregister::SET_STANDARD_FORMAT_COMMAND, (System::Byte) (index),(System::Byte) 0,(System::Byte) 0,(System::Byte) 0
	/// This is the frame implementation to Execute the SET_CALIBRATION_FORMAT_COMMAND Command
	#define PCB303_COMMAND_CALIBRATION_FORMAT (System::Byte) PCB303::Commandregister::SET_CALIBRATION_FORMAT_COMMAND, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0,(System::Byte) 0
	/// This is the frame implementation to Execute the SET_TOMO_MODE_COMMAND Command
	#define PCB303_COMMAND_ACTIVATE_TOMO (System::Byte) PCB303::Commandregister::SET_TOMO_MODE_COMMAND, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0,(System::Byte) 0

	///@}


	/// <summary>
	/// PCB303 Class constructor.
	/// 
	/// The construction initializes several internal variables.
	/// The constructor call the base class setting the address of the controlled device.
	/// 
	/// As soon as the base class is initialized, the communication process starts
	/// and the device is initialized with configurationLoop() routine.
	/// \ingroup PCB303_Interface
	/// </summary>
	public: PCB303() : CanDeviceProtocol(0x12, L"COLLIMATOR_DEVICE")
	{
		collimationMode = collimationModeEnum::OPEN_MODE;
		valid_collimation_format = false;
		collimator_fault = false;
		format_collimation_attempt = 0;
	}
	static PCB303^ device = gcnew PCB303();

	/// \ingroup PCB303_Internal
	///@{
	/// 
	protected: 	
		void resetLoop(void) override;//!< This is the Base class resetLoop override function
		void runningLoop(void) override;//!< This is the Base Class override function to handle the normal operation workflow 
		bool configurationLoop(void) override;//!< This is the Base Class override function to handle the device configuration
		void demoLoop(void) override;

	private:	bool updateStatusRegister(void); //! Read the status registers and handle the related bytes
	private:	void formatCollimationManagement(void);

	///@}
	
	/// <summary>
	///  This enumeration class describes the possible collimation modes
	/// the Application can select
	///  \ingroup PCB303_Internal
	/// </summary>
	enum class  collimationModeEnum{
		AUTO_COLLIMATION = 0, //!< The collimator is in Auto mode: the paddle model defines the current collimation format
		CUSTOM_COLLIMATION,   //!< The collimator is in Custom mode: the collimation format is set by the operator
		CALIBRATION_MODE,	  //!< The collimator is in Calibration mode: the collimation format is set manually by the service software
		OPEN_MODE,			  //!< The collimator is in Open mode: the collimation format set OPEN
		TOMO_MODE			  //!< The collimator is in Tomo mode: the collimation format is dinamically set by the tomo pulse sequence
	};

	/// <summary>
	/// This class provides a contenitor for a CALIBRATION blade setup
	/// \ingroup PCB303_Interface
	/// </summary>
	public:ref class formatBlades {
	public:

		/// <summary>
		/// This is the constructor to directly assigne the value of the blades positions
		/// </summary>
		/// <param name="ft">front blade</param>
		/// <param name="bk">back blade</param>
		/// <param name="lt">left blade</param>
		/// <param name="rt">right blade</param>
		/// <param name="tp">trap blade</param>
		formatBlades(unsigned short ft, unsigned short bk, unsigned short lt, unsigned short rt, unsigned short tp) {
			front = ft;
			back = bk;
			left = lt;
			right = rt;
			trap = tp;
		}

		/// <summary>
		/// This is the constructor of an OPEN blades
		/// </summary>
		formatBlades() {
			front = back = left = right = trap = 0;
		}

		/// <summary>
		/// This is the function to copy a Blade setting 
		/// </summary>
		/// <param name="b">Handle of a Blade to be copied</param>
		void copy(formatBlades^ b) {
			front = b->front;
			back = b->back;
			left = b->left;
			right = b->right;
			trap = b->trap;
		}

		unsigned short front;//!< Front blade position
		unsigned short back; //!< back blade position
		unsigned short left; //!< Left blade position
		unsigned short right; //!< Right blade position
		unsigned short trap; //!< Trap blade position
	};

	/// \ingroup PCB303_Internal
	///@{
	private: static collimationModeEnum collimationMode; //!< Sets the current collimation mode 
	private: static bool valid_collimation_format; //!< This flag is set when the collimation format is correct and coherent with the collimationMode register
	private: static bool collimator_fault; //!< This flag is set when the collimator is in fault condition
	private: static int format_collimation_attempt; //!< This register counts the attempt to exit from a fault condition
	private: static ColliStandardSelections customStandardSelection;//!< Custom 2D standard format requested (in case of CUSTOM mode)
	private: static ColliStandardSelections getAutomaticStandardFormatIndex(void);//!< Retrives what is the current automatic format to be calibrated
	private: static formatBlades calibrationBlades;
	private: static CollimationStatusCode   collimation_status; //!< last status collimation status read
	private: static ColliStandardSelections format_index;		//!< last status format index read
	private: static unsigned char system_flags;					//!< Last system flags
	///@}

public:
	/// \ingroup PCB303_Interface
	///@{
	static void setAutoCollimationMode(void); //!< This function sets the format collimation to AUTO mode
	static void setOpenCollimationMode(void); //!< This function sets the format collimation to OPEN mode
	static void setCalibrationCollimationMode(formatBlades^ blades) ;//!< This function sets the format collimation to CALIBRATION mode
	static void setTomoCollimationMode(void); //!< This function activates the Tomo collimation mode
	static void setCustomCollimationMode(ColliStandardSelections custom );//!< This function sets the format collimation to CUSTOM mode
	static void resetFaults(void);//!< In case of collimation fault condition, this function starts a new collimation attempt.
	inline static bool isValidCollimationFormat(void) { return valid_collimation_format; }
	///@}
};


