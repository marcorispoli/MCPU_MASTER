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

	literal System::Byte  NUM_COLLIMATION_SLOTS = 20; //!< Max number of available collimation slots
	literal System::Byte  CUSTOM_SLOT = NUM_COLLIMATION_SLOTS - 1; //!< The slot assigned to the custom format collimation
	

	/// <summary>
	/// This class implement the protocol data structure as described in the protocol specification.
	/// </summary>
	ref class ProtocolStructure {
	public:
		ref class format_collimation_data {
		public:
			format_collimation_data() {
				front = 0;
				back = 0;
				left = 0;
				right = 0;
				trap = 0;
			}

			unsigned short front;
			unsigned short back;
			unsigned short left;
			unsigned short right;
			unsigned short trap;

		};

		ref class StatusRegister {
		public:

			enum class register_index {
				SYSTEM_REGISTER = 0, //!> This is the System Status register index
				TUBE_REGISTER,	
				NUM_REGISTERS
			};

			enum class action_code {
				STAT_UNDEFINED = 0, //!> The position of the device is undefined
				STAT_POSITIONING,	//!> A positioning command is executing
				STAT_POSITIONED		//!> The position of the device is undefined
			};

			enum class mirror_target_code {
				OUT_FIELD = 0,	//!> Out of the X-RAY field
				IN_FIELD,		//!> In of the X-RAY field
			};

			enum class light_target_code {
				LIGHT_OFF = 0,	//!> Light switched Off
				LIGHT_ON,		//!> Light Switched On
			};

			

			static bool decodeSystemRegister(Register^ sys) {
				if (sys == nullptr) return false;

				// Byte 0: Collimator byte
				collimation_action_status = (action_code) (sys->d0 & 0x7);
				collimation_target_index = (sys->d0  >> 3);

				// Byte 1: Filter byte
				filter_action_status = (action_code)(sys->d1 & 0x7);
				filter_target_index = (sys->d1 >> 3);

				// Byte 2: Mirror byte
				mirror_action_status = (action_code)(sys->d2 & 0x7);

				if (sys->d2 & 0x8) mirror_target_index = mirror_target_code::IN_FIELD;
				else mirror_target_code::OUT_FIELD;

				if (sys->d2 & 0x10) light_status = light_target_code::LIGHT_ON;
				else light_status = light_target_code::LIGHT_OFF;

				return true;
			}

			Register^ encodeSystemRegister(void) {

				// Creates a register with all bytes set to 0
				CanDeviceProtocol::Register^ sys = gcnew CanDeviceProtocol::Register;

				// Byte 0 of the register
				sys->d0 = ((unsigned char) collimation_action_status & 0x7);
				sys->d0 |= ((unsigned char) collimation_target_index >> 3);

				// Byte 1 of the register
				sys->d1 = ((unsigned char)filter_action_status & 0x7);
				sys->d1 |= ((unsigned char)filter_target_index >> 3);

				// Byte 2 of the register
				sys->d2 = ((unsigned char)mirror_action_status & 0x7);
				
				if (mirror_target_index == mirror_target_code::IN_FIELD) sys->d2 |= 0x8;
				else sys->d2 &= ~0x8;

				if (light_status == light_target_code::LIGHT_ON) sys->d2 |= 0x10;
				else sys->d2 &= ~0x10;

				// Returns the formatted register
				return sys;
			}


			static bool decodeTubeRegister(Register^ sys) {
				if (sys == nullptr) return false;

				// Byte 0: Stator
				stator_temp = sys->d0;

				// Byte 1: Bulb Byte
				bulb_temp = sys->d1;
				return true;
			}

			Register^ encodeTubeRegister(void) {

				// Creates a register with all bytes set to 0
				CanDeviceProtocol::Register^ sys = gcnew CanDeviceProtocol::Register;

				// Byte 0 of the register
				sys->d0 = stator_temp;

				// Byte 1 of the register
				sys->d1 = bulb_temp;

				// Returns the formatted register
				return sys;
			}

			static action_code collimation_action_status;
			static unsigned char collimation_target_index;

			static action_code filter_action_status;
			static unsigned char filter_target_index;

			static action_code mirror_action_status;
			static mirror_target_code mirror_target_index;

			static light_target_code light_status;

			static unsigned char stator_temp;
			static unsigned char bulb_temp;

		};


		ref class DataRegister {
		public:

			enum class register_index {
				NUM_REGISTERS = 0,
			};

		};

		ref class ParameterRegister {
		public:

			enum class register_index {
				FB_FORMAT_SLOT_IDX = 0,
				LR_FORMAT_SLOT_IDX = FB_FORMAT_SLOT_IDX + NUM_COLLIMATION_SLOTS,
				TR_FORMAT_SLOT_IDX = LR_FORMAT_SLOT_IDX + NUM_COLLIMATION_SLOTS,
				FILTER_SLOT_IDX = TR_FORMAT_SLOT_IDX + NUM_COLLIMATION_SLOTS / 2,
				MIRROR_SLOT_IDX = FILTER_SLOT_IDX + 3,
				NUM_REGISTERS
			};

			ParameterRegister() {
				format_collimation = gcnew cli::array< format_collimation_data^>(NUM_COLLIMATION_SLOTS);
				filter_slots = gcnew cli::array< unsigned short>(6);

				for (int i = 0; i < NUM_COLLIMATION_SLOTS; i++) {
					format_collimation[i] = gcnew format_collimation_data();
				}
				for (int i = 0; i < 6; i++) {
					filter_slots[i] = 0;
				}
				mirror_slot = 0;
			}

			static bool decodeFBCollimationSlotRegister(Register^ reg, int idx) {
				if (reg == nullptr) return false;
				if (idx >= format_collimation->Length) return false;

				format_collimation[idx]->front = reg->d0 + 256 * reg->d1;
				format_collimation[idx]->back = reg->d2 + 256 * reg->d3;
				return true;
			}

			Register^ encodeFBCollimationSlotRegister(int idx) {
				if (idx >= format_collimation->Length) return nullptr;

				unsigned char d0 = format_collimation[idx]->front & 0xFF;
				unsigned char d1 = (format_collimation[idx]->front >> 8) & 0xFF;

				unsigned char d2 = format_collimation[idx]->back & 0xFF;
				unsigned char d3 = (format_collimation[idx]->back >> 8) & 0xFF;

				return gcnew Register(d0, d1, d2, d3);
			}

			static bool decodeLRCollimationSlotRegister(Register^ reg, int idx) {
				if (reg == nullptr) return false;
				if (idx >= format_collimation->Length) return false;

				format_collimation[idx]->left = reg->d0 + 256 * reg->d1;
				format_collimation[idx]->right = reg->d2 + 256 * reg->d3;
				return true;
			}

			Register^ encodeLRCollimationSlotRegister(int idx) {
				if (idx >= format_collimation->Length) return nullptr;

				unsigned char d0 = format_collimation[idx]->left & 0xFF;
				unsigned char d1 = (format_collimation[idx]->left >> 8) & 0xFF;

				unsigned char d2 = format_collimation[idx]->right & 0xFF;
				unsigned char d3 = (format_collimation[idx]->right >> 8) & 0xFF;

				return gcnew Register(d0, d1, d2, d3);
			}

			static bool decodeTrapCollimationSlotRegister(Register^ reg, int idx) {
				if (reg == nullptr) return false;
				if ((idx * 2) >= format_collimation->Length) return false;

				int idxl = idx * 2;
				int idxh = idxl + 1;

				format_collimation[idxl]->trap = reg->d0 + 256 * reg->d1;
				format_collimation[idxh]->trap = reg->d2 + 256 * reg->d3;
				return true;
			}

			Register^ encodeTrapCollimationSlotRegister(int idx) {
				if ((idx*2) >= format_collimation->Length) return nullptr;

				int idxl = idx * 2;
				int idxh = idxl + 1;
				
				unsigned char d0 = format_collimation[idxl]->left & 0xFF;
				unsigned char d1 = (format_collimation[idxl]->left >> 8) & 0xFF;

				unsigned char d2 = format_collimation[idxh]->right & 0xFF;
				unsigned char d3 = (format_collimation[idxh]->right >> 8) & 0xFF;

				return gcnew Register(d0, d1, d2, d3);
			}

			static bool decodeFilterSlotRegister(Register^ reg, int idx) {
				if (reg == nullptr) return false;
				if ((idx * 2) >= filter_slots->Length) return false;

				int idxl = idx * 2;
				int idxh = idxl + 1;

				filter_slots[idxl] = reg->d0 + 256 * reg->d1;
				filter_slots[idxh] = reg->d2 + 256 * reg->d3;
				return true;
			}

			Register^ encodeFilterSlotRegister(int idx) {
				if ((idx * 2) >= filter_slots->Length) return nullptr;

				int idxl = idx * 2;
				int idxh = idxl + 1;

				unsigned char d0 = filter_slots[idxl] & 0xFF;
				unsigned char d1 = (filter_slots[idxl] >> 8) & 0xFF;

				unsigned char d2 = filter_slots[idxh] & 0xFF;
				unsigned char d3 = (filter_slots[idxh] >> 8) & 0xFF;

				return gcnew Register(d0, d1, d2, d3);
			}

			static bool decodeMirroRegister(Register^ reg) {
				if (reg == nullptr) return false;

				mirror_slot = reg->d0 + 256 * reg->d1;
				return true;
			}

			Register^ encodeMirrorRegister(void) {
				unsigned char d0 = mirror_slot & 0xFF;
				unsigned char d1 = (mirror_slot >> 8) & 0xFF;

				unsigned char d2 = 0;
				unsigned char d3 = 0;

				return gcnew Register(d0, d1, d2, d3);
			}

			static cli::array< format_collimation_data^>^ format_collimation;
			static cli::array< unsigned short>^ filter_slots;
			static unsigned short mirror_slot;
		};

		ref class Commands {
		public:
			enum class command_index {
				ABORT_COMMAND = 0, //!< Abort Command (mandatory as for device protocol)
				SET_FORMAT,
				SET_FILTER,
				SET_MIRROR,
				SET_LIGHT,
			};

			CanDeviceCommand^ encodeSetFormatCommand(unsigned char target_format_slot) {
				// if the index is out of range, the collimatin will be st to OPEN in the device
				if (target_format_slot >= NUM_COLLIMATION_SLOTS) target_format_slot = 0;
				return gcnew CanDeviceCommand((unsigned char)command_index::SET_FORMAT, target_format_slot, 0, 0, 0);
			}

			CanDeviceCommand^ encodeSetFilterCommand(unsigned char target_filter_slot) {
				if (target_filter_slot >= 5) return nullptr;
				return gcnew CanDeviceCommand((unsigned char)command_index::SET_FILTER, target_filter_slot, 0, 0, 0);
			}

			CanDeviceCommand^ encodeSetMirrorCommand(System::Byte target_mirror) {
				if (target_mirror > 1) return nullptr;
				return gcnew CanDeviceCommand((unsigned char)command_index::SET_MIRROR, target_mirror, 0, 0, 0);
			}

			CanDeviceCommand^ encodeSetLightCommand(System::Byte target_light) {
				if (target_light > 1) return nullptr;
				return gcnew CanDeviceCommand((unsigned char)command_index::SET_LIGHT, target_light, 0, 0, 0);
			}

		};

		static StatusRegister status_register;
		static DataRegister data_register;
		static ParameterRegister parameter_register;
		static Commands command;
	};

	
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

	
	private:	
		void formatManagement(void);
		void filterManagement(void);
		void mirrorManagement(void);
		

	///@}
	
	/// <summary>
	///  This enumeration class describes the possible collimation modes
	/// the Application can select
	///  \ingroup PCB303_Internal
	/// </summary>
	enum class  collimationModeEnum{
		AUTO_COLLIMATION = 0, //!< The collimator is in Auto mode: the paddle model defines the current collimation format
		CUSTOM_COLLIMATION,   //!< The collimator is in Custom mode: the collimation format is set by the operator		
		OPEN_MODE,			  //!< The collimator is in Open mode: the collimation format set OPEN
		CALIBRATION_MODE,	  //!< The collimator is in Calibration mode: the collimation format is set manually by the service software
		TOMO_MODE			  //!< The collimator is in Tomo mode: the collimation format is dinamically set by the tomo pulse sequence
	};

	

	/// \ingroup PCB303_Internal
	///@{
	private: 
		static ProtocolStructure protocol; //!< This is the structure with the Status register info

		static collimationModeEnum collimationMode;				//!< Sets the current collimation mode 
		static System::Byte selected_format = 0;					//!< This is the current selected format 
		static System::Byte current_auto_format = 0;	//!< Auo Collimation format
		static System::Byte current_custom_format = 0; //!< Manual Collimation format
		static bool valid_collimation_format = false; //!< This flag is set when the collimation format is correct and coherent with the collimationMode register
		static int format_collimation_attempt = 0; //!< This register counts the attempt to exit from a fault condition

		static System::Byte selected_filter = 0;
		static bool valid_filter_format = false;
		static int filter_attempt = 0; 

		static ProtocolStructure::StatusRegister::mirror_target_code selected_mirror = ProtocolStructure::StatusRegister::mirror_target_code::OUT_FIELD;
		static bool valid_mirror_format = false;
		static int mirror_attempt = 0; 

		static ProtocolStructure::StatusRegister::light_target_code selected_light = ProtocolStructure::StatusRegister::light_target_code::LIGHT_OFF;
		static bool retrigger_light_on_command = false;

	///@}

public:
	/// \ingroup PCB303_Interface
	///@{
	static void setAutoCollimationMode(void); //!< This function sets the format collimation to AUTO mode
	static void setOpenCollimationMode(void); //!< This function sets the format collimation to OPEN mode	
	
	static void setCustomCollimationMode(System::Byte format_index);//!< This function sets the format collimation to CUSTOM mode
	static void setCollimationLight(bool stat);
	

	static void resetFaults(void);//!< In case of collimation fault condition, this function starts a new collimation attempt.
	inline static bool isValidCollimationFormat(void) { return valid_collimation_format; }
	///@}
};


