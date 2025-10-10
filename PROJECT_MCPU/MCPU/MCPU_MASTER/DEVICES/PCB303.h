#pragma once
#include "CanDeviceProtocol.h"
#include "ConfigurationFiles.h"

/**
	\defgroup PCB303DESC Collimation Management Description
	\ingroup APPDOC


	# Abstract

	This section describes the function and the performances of the Collimator module.


	# Overview

	The Gantry system makes use of the PCB303 board to implement the collimation functions:
	+ X-RAY 2D collimation: the x-ray field exiting the X-Ray tube shall be geometrically limited to a well defined ispection area;
	+ X-RAY 3D collimation: the x-ray field exiting the X-Ray tube during a Tomo sequence shall be confined to a well defined area, through a dinamic collimation process;
	+ X-RAY verification: before to activate the x-rays, the collimator device shall project a light wich field geometry shall equals the x-ray collimated area;
	+ X-RAY filtering: the collimator shall provide a set of selectable filters to shape the x-ray field spectra;
	+ Tube temperature monmitoring: due to its proximity with the X-RAY Tube, the Tube temperature is collected during the exposures to prevent overheating;

	The format collimation is performed by setting in the collimator device the position of five collimation blades:
	+ Frontal blade: limits the x-ray field from the patient side;
	+ Back blade: limits the x-ray field from the gantry side;
	+ left blade: limits the x-ray field from the left side (patient point of view);
	+ right blade: limits the x-ray field from the right side (patient point of view);
	+ trap blade: is a special blade positioned on the back side, to correct the trasversal dispersion during a tomo scan;

	The X-Ray verification is performed activating a power light and positioning a reflecting Mirror 
	in a position where the virtual light focus is coincident with the X-RAY focus.

	The X-Ray filtering is performed controlling the position of a rotating selector 
	where the filters are mounted in.

	The Tube temperature is performed reading two sensors inside the X-Ray tube:
	+ The stator sensor;
	+ The Bulb sensor;

	# Communication with the PCB303 board

	The application implements the general Can Device Communication protocol (\ref CanDeviceProtocol class)
	in order to communicate and control the Collimator device.

	The Application implements the specific protocol feature described in the PCB303 protocol specification document.  

	Based on that protocol, the following common activities are implemented:
	+ Communication startup management:
		+ Connection fase;
		+ Remote Bootloader management;
		+ Revision acquisition;
		+ Device Register Configuration;
		+ Board reset handling;

	+ Register Set and Get commands;
	+ Execution Commands workflow;

	# 2D Format Collimation

	## General Description

	The Application implements a 2D Format collimation applicable to the 2D exposures.

		A Dinamic format collimation is then reserved for the Tomo exposures instead.

	The Application allows to select the following collimation formats:
	+ The OPEN collimation format: is a special format where the blades are set to 0 position, out of the x-ray field;
	+ Automatic format collimation (typical): the format collimation is assigned automatically by the Application
	based on the current detected compressor paddle (see the \ref PCB302DESC for details);
	+ Custom collimation: the current collimation format can be selected into a list of the available formats:
		+ The list is composed by the whole set of paddle's formats and an extra format that can be arbitrary calibrated;

	## Format setting and Paddle assignement

	The module allows to configure up to 20 different collimation format slots:
	+ Each collimation slot sets the value of the collimation blades;
	+ The Slots are defined into the CollimatorCalibration.cnf (see \ref CollimatorConfig)
	+ The Slot #0 is assigned to the so called "OPEN" format;
	+ The Slot #19 is assigned to the so called "CUSTOM" format;
	+ The Slot #1 to #18 can be assigned to any compressor paddles.

	The assignement of a collimation slot to a compression paddle is executed 
	into the PaddleCalibration.cnf (see \ref PaddleConfig):
	+ the assignement is made setting the slot number to the proper paddle parameter.

		NOTE: different paddles can share the same format collimation slot if necessary.


	## Format Selection Rules
	
	### Closed-Study operating status

	The collimation is always set to OPEN.

	See the Idle Status descripion for details

	### Open-Study operating status

	In Open Study the Acquisition software (AWS) controls the collimation format selection.

	The AWS can select two possible collimation methods:
	+ Automatic Mode;
	+ Manual Mode;

	When the Automatic Mode is selected, the application automatically assignes the collimation format
	based on the detected paddle, following the PaddleConfiguration.cnf setting.

	When the Manual Mode is selected, the AWS selects the Paddle format to be used or an extra format called "CUSTOM".

		NOTE: The Open collimation format is not allowed in Open-Study			

	See the Open-Study Status descripion for details.

	### Service Operating status

	The collimation format is set to OPEN if not differently set by a specific service 
	module that should handle the collimator for testy or calibration.

	See the Service Status descripion for details.

	# Collimation Light Management

	The Application controls a power light and a mirror device in the collimator device, 
	in order to implement the collimation light function.

	The function of the collimation light is:
	+ Help the Operator in the breast positioning during the breast compression;
	+ Verify the current collimation field.

	When the collimation light position and the mirror position are well calibrated, the
	position of the light focus will coincide with the position of the X-RAY focus: 
	+ In this condition, the field projected by the light is collimated as it should be the x-ray focus;
	+ The projected light footprint on the compression plane is then equivalent (with proper tollerances) with the x-ray footprint;
	+ The Operator can check the accuracy of the collimation before to activate the x-rays.

	When the collimation light is requested:
	+ The Mirror is positioned in the In-Field position, at the calibrated position (see the \ref CollimatorConfig );
	+ The light is activated for a limited time (usually 20 seconds);
	+ The light can be retriggered at the occurrence;

	The Mirror will be removed from the In-Field position (and the light switched Off)  before to activate the X-RAY sequence.

	## Collimation Light in Closed Study status

	When the Application is in Closed Study (Idle) the Mirror is always set Out-Field
	and the light switched Off.

	## Collimation Light in Open Study status

	When in Open Study, the Mirror will be set In-Field and the Light switched On as soon as the Compressor paddle 
	is activated downward, even if not yet in compression.

	The Operator can manually activate the Light at the occurrence, pressing a graphical button on the GUI.

	The Light remains On for a limited time after the compressor paddle activation termines, usually 20 seconds.
	+ The Time the light shall remains activated can be configured into the \ref CollimatorConfig ;
	+ In case the paddle should reactivated, the light will be reactivated and the timer will be reset;

		NOTE: The Mirror will not be removed from the In-Field when the light switches Off, to reduce the mechanical stress and noise.

	The Mirror will be removed from thr In-Field (set to Out-Field) and the Light will be switched Off 
	if a X-Ray procedure should start.

	See the Open Study description for details.

	## Collimation Light in Service status

	The Mirror is removed for the In-Field and Light switched Off in Service if no specific service 
	function should calibrate or test the light collimation function.

	# Filter Selection

	# Tube Temperature Monitoring



*/


/// <summary>
/// \defgroup PCB303IMPL Collimator Module Implementation
/// \ingroup DEVIMPL 
/// 
/// This module describes the PCB303 Collimator module implementation
/// 
/// </summary>
/// 
///  
/// 


/// <summary>
/// This class implements the functions of the PCB303 Module
/// \ingroup PCB303IMPL
/// 
/// </summary>
ref class PCB303 : public CanDeviceProtocol
{
public:

	literal System::Byte  NUM_COLLIMATION_SLOTS = 20; //!< Max number of available collimation slots
	literal System::Byte  CUSTOM_SLOT = NUM_COLLIMATION_SLOTS - 1; //!< The slot assigned to the custom format collimation
	
	/// <summary>
	/// This is the enumeration class for the available filters
	/// </summary>
	enum class filter_index {
		FILTER_RH = 0, 
		FILTER_AG,
		FILTER_AL,
		FILTER_CU,
		FILTER_MO,
		FILTER_LD,
	};

	static System::String^  getTagFromFilter(filter_index flt) {
		if (flt == filter_index::FILTER_RH) return "Rh";
		if (flt == filter_index::FILTER_AG) return "Ag";
		if (flt == filter_index::FILTER_AL) return "Al";
		if (flt == filter_index::FILTER_CU) return "Cu";
		if (flt == filter_index::FILTER_MO) return "Mo";
		if (flt == filter_index::FILTER_LD) return "Ld";

		return "";
	}

	static filter_index getFilterFromTag(System::String^ tag) {
		if (
			(tag->Equals("ag")) ||
			(tag->Equals("aG")) ||
			(tag->Equals("Ag")) ||
			(tag->Equals("AG"))
			) return filter_index::FILTER_AG;
			
		if (
			(tag->Equals("al")) ||
			(tag->Equals("aL")) ||
			(tag->Equals("Al")) ||
			(tag->Equals("AL"))
			) return filter_index::FILTER_AL;

		if (
			(tag->Equals("rh")) ||
			(tag->Equals("rH")) ||
			(tag->Equals("Rh")) ||
			(tag->Equals("RH"))
			) return filter_index::FILTER_RH;

		if (
			(tag->Equals("cu")) ||
			(tag->Equals("cU")) ||
			(tag->Equals("Cu")) ||
			(tag->Equals("CU"))
			) return filter_index::FILTER_CU;

		if (
			(tag->Equals("mo")) ||
			(tag->Equals("mO")) ||
			(tag->Equals("Mo")) ||
			(tag->Equals("MO"))
			) return filter_index::FILTER_MO;

		if (
			(tag->Equals("ld")) ||
			(tag->Equals("lD")) ||
			(tag->Equals("Ld")) ||
			(tag->Equals("LD"))
			) return filter_index::FILTER_LD;

		// As default returns the Rh
		return filter_index::FILTER_RH;
	}

	/// <summary>
	/// This class implements and read the device configuration file 
	/// </summary>
	ref class ConfigurationStructure {
	public:

		static cli::array<unsigned short>^ left_blade_formats = gcnew  cli::array<unsigned short> (CollimatorConfig::TOTAL_BLADE_FORMAT_NUMBERS);
		static cli::array<unsigned short>^ right_blade_formats = gcnew  cli::array<unsigned short> (CollimatorConfig::TOTAL_BLADE_FORMAT_NUMBERS);
		static cli::array<unsigned short>^ front_blade_formats = gcnew  cli::array<unsigned short> (CollimatorConfig::TOTAL_BLADE_FORMAT_NUMBERS);
		static cli::array<unsigned short>^ back_blade_formats = gcnew  cli::array<unsigned short> (CollimatorConfig::TOTAL_BLADE_FORMAT_NUMBERS);		
		static cli::array<unsigned short>^ trap_blade_formats = gcnew  cli::array<unsigned short>(CollimatorConfig::TOTAL_BLADE_FORMAT_NUMBERS);
		static cli::array<unsigned short>^ filter_slot_steps = gcnew  cli::array<unsigned short> (CollimatorConfig::TOTAL_FILTER_SLOT_NUMBERS);
		static unsigned char  rh_assigned_slot = 0;
		static unsigned char  ag_assigned_slot = 0;
		static unsigned char  al_assigned_slot = 0;
		static unsigned char  cu_assigned_slot = 0;
		static unsigned char  mo_assigned_slot = 0;
		static unsigned char  ld_assigned_slot = 0;
		static unsigned short mirror_steps;

		static void reload(void) {

			// Upload in the internal structurs the format blades values
			for (int index = 0; index < CollimatorConfig::TOTAL_BLADE_FORMAT_NUMBERS; index++) {
				System::String^ Param = "COLLI_STANDARD_FORMAT_" + index.ToString();
				left_blade_formats[index] = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_LEFT]);
				right_blade_formats[index] = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_RIGHT]);
				front_blade_formats[index] = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_FRONT]);
				back_blade_formats[index] = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_BACK]);
				trap_blade_formats[index] = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_TRAP]);
			}

			// Upload the filter positions from the configuration file
			filter_slot_steps[0] = System::Convert::ToUInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_POSITION)[CollimatorConfig::PARAM_FILTER_POSITION_0]);
			filter_slot_steps[1] = System::Convert::ToUInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_POSITION)[CollimatorConfig::PARAM_FILTER_POSITION_1]);
			filter_slot_steps[2] = System::Convert::ToUInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_POSITION)[CollimatorConfig::PARAM_FILTER_POSITION_2]);
			filter_slot_steps[3] = System::Convert::ToUInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_POSITION)[CollimatorConfig::PARAM_FILTER_POSITION_3]);
			filter_slot_steps[4] = System::Convert::ToUInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_POSITION)[CollimatorConfig::PARAM_FILTER_POSITION_4]);

			/// Updates the slot assignment to the filter materials
			rh_assigned_slot = System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_RH_SLOT]);
			ag_assigned_slot = System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_AG_SLOT]);
			al_assigned_slot = System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_AL_SLOT]);
			cu_assigned_slot = System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_CU_SLOT]);
			mo_assigned_slot = System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_MO_SLOT]);
			ld_assigned_slot = System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_LD_SLOT]);

			// Load the steps for the mirror when in field
			mirror_steps = System::Convert::ToUInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_MIRROR)[CollimatorConfig::MIRROR_INFIELD]);
		}

		ConfigurationStructure(){
			reload();
		}

	};


	
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
				else mirror_target_index = mirror_target_code::OUT_FIELD;

				if (sys->d2 & 0x10) light_status = light_target_code::LIGHT_ON;
				else light_status = light_target_code::LIGHT_OFF;

				return true;
			}

			Register^ encodeSystemRegister(void) {

				// Creates a register with all bytes set to 0
				CanDeviceProtocol::Register^ sys = gcnew CanDeviceProtocol::Register;

				// Byte 0 of the register
				sys->d0 = ((unsigned char) collimation_action_status & 0x7);
				sys->d0 |= ((unsigned char) collimation_target_index << 3);

				// Byte 1 of the register
				sys->d1 = ((unsigned char)filter_action_status & 0x7);
				sys->d1 |= ((unsigned char)filter_target_index << 3);

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
				
				unsigned char d0 = format_collimation[idxl]->trap & 0xFF;
				unsigned char d1 = (format_collimation[idxl]->trap >> 8) & 0xFF;

				unsigned char d2 = format_collimation[idxh]->trap & 0xFF;
				unsigned char d3 = (format_collimation[idxh]->trap >> 8) & 0xFF;

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
		collimationMode = collimationModeEnum::DISABLED;
		filterMode = filterModeEnum::DISABLED_FILTER_MODE;
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
		void tubeManagement(void);
		

	///@}
	
	/// <summary>
	///  This enumeration class describes the possible collimation modes
	/// the Application can select
	///  \ingroup PCB303_Internal
	/// </summary>
	public: enum class  collimationModeEnum{
		DISABLED = 0,	//!< The collimator is disabled
		AUTO,			//!< The collimator is in Auto mode: the paddle model defines the current collimation format
		CUSTOM,			//!< The collimator is in Custom mode: the collimation format is set by the operator		
		OPEN,					//!< The collimator is in Open mode: the collimation format set OPEN		
		TOMO,					//!< The collimator is in Tomo mode: the collimation format is dinamically set by the tomo pulse sequence
		ITEMS=TOMO
	};

	/// <summary>
	///  This enumeration class describes the possible filter modes the Application can select
	///  \ingroup PCB303_Internal
	/// </summary>
	public: enum class  filterModeEnum {
		DISABLED_FILTER_MODE = 0,	//!< The Filter activation is not enabled
		ACTIVE_MODE,				//!< The Filter is selectable
	};

	/// \ingroup PCB303_Internal
	///@{
	private: 
		static ProtocolStructure protocol; //!< This is the structure with the Status register info
		static ConfigurationStructure^ config = gcnew ConfigurationStructure; //!< This is the configuration file content

		static collimationModeEnum collimationMode;				//!< Sets the current collimation mode 
		static int selected_format = -1;						//!< This is the current selected format 
		static int current_auto_format = -1;					//!< Auo Collimation format
		static int current_custom_format = -1;					//!< Manual Collimation format
		static int current_service_format = -1;					//!< Service Collimation format

		static bool valid_collimation_format = false; //!< This flag is set when the collimation format is correct and coherent with the collimationMode register
		static int format_collimation_attempt = 0; //!< This register counts the attempt to exit from a fault condition

		static filterModeEnum filterMode;
		static filter_index selected_filter = filter_index::FILTER_LD;
		static int selected_slot = 0;

		static bool valid_filter_format = false;
		static bool filter_error = false;
		static int filter_attempt = 0; 
		

		static ProtocolStructure::StatusRegister::mirror_target_code selected_mirror = ProtocolStructure::StatusRegister::mirror_target_code::OUT_FIELD;
		static bool valid_mirror_format = false;
		static int mirror_attempt = 0; 

		static ProtocolStructure::StatusRegister::light_target_code selected_light = ProtocolStructure::StatusRegister::light_target_code::LIGHT_OFF;
		static bool retrigger_light_on_command = false;

		static int bulb_temperature_perc = 0;
		static int stator_temperature_perc = 0;
		static int max_temperature_perc = 0;

	///@}

public:
	/// \ingroup PCB303_Interface
	///@{
	
	static ConfigurationStructure^ getConfig(void) { return config; }
	static void setFormatCollimationMode(collimationModeEnum mode, unsigned char format_index);

	// Filter Commands	
	static void setFilterMode(filterModeEnum mode);
	static bool selectFilter(filter_index filter);
	

	static int  getFilterSlot(filter_index filter);
	static bool isValidFilter(void) { return valid_filter_format; }
	static bool isFilterInError(void) { return filter_error; }
	static bool	waitFilterCompleted(void);

	static void setCollimationLight(bool stat);
	static bool getPowerLightStatus(void) { return (protocol.status_register.light_status == ProtocolStructure::StatusRegister::light_target_code::LIGHT_ON) ? true : false; }
	
	static int getBulbPerc(void) { return bulb_temperature_perc; }
	static int getStatorPerc(void) { return stator_temperature_perc; }
	static int getMaxTubePerc(void) { return max_temperature_perc; }

	static bool isTubeAlarm() { return false; }

	static void resetFaults(void);//!< In case of collimation fault condition, this function starts a new collimation attempt.

	inline static bool isValidCollimationFormat(void) { return valid_collimation_format; }
	inline static int getFormatIndex(void) { return (valid_collimation_format) ? selected_format : (int)-1; }
	inline static collimationModeEnum getFormatCollimationMode(void) { return collimationMode; }
	///@}
};


