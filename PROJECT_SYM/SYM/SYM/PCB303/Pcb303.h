#pragma once
#include "deviceInterface.h"

using namespace System;




ref class PCB303: public deviceInterface
{
public:
	PCB303(void) :deviceInterface((unsigned short)0x12, setRegVal(1, 0, 0, 0), (Byte)ProtocolStructure::StatusRegister::register_index::NUM_REGISTERS, (Byte)ProtocolStructure::DataRegister::register_index::NUM_REGISTERS, (Byte)ProtocolStructure::ParameterRegister::register_index::NUM_REGISTERS) {
		filter_slot_assignment = gcnew cli::array<System::String^>(6);
		filter_slot_assignment[0] = "-";
		filter_slot_assignment[1] = "-";
		filter_slot_assignment[2] = "-";
		filter_slot_assignment[3] = "-";
		filter_slot_assignment[4] = "-";
	}

	static void  initialize(void) {};
	static PCB303^ board = gcnew PCB303;

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
				collimation_action_status = (action_code)(sys->d0 & 0x7);
				collimation_target_index = (sys->d0 >> 3);

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
				Register^ sys = gcnew Register;

				// Byte 0 of the register
				sys->d0 = ((unsigned char)collimation_action_status & 0x7);
				sys->d0 |= ((unsigned char)collimation_target_index << 3);

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
				Register^ sys = gcnew Register;

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

			literal unsigned int  FB_FORMAT_SLOT_IDX = 0;
			literal unsigned int  LR_FORMAT_SLOT_IDX = FB_FORMAT_SLOT_IDX + NUM_COLLIMATION_SLOTS;
			literal unsigned int  TR_FORMAT_SLOT_IDX = LR_FORMAT_SLOT_IDX + NUM_COLLIMATION_SLOTS;
			literal unsigned int  FILTER_SLOT_IDX = TR_FORMAT_SLOT_IDX + NUM_COLLIMATION_SLOTS / 2;
			literal unsigned int  MIRROR_SLOT_IDX = FILTER_SLOT_IDX + 3;



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
				if ((idx * 2) >= format_collimation->Length) return nullptr;

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


	void device_workflow_callback(void) override;
	void device_reset_callback(void) override;
	commandResult^ device_command_callback(unsigned char cmd, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3) override;

	

	ref class hardware_device {
	public:
		hardware_device() { init(); }
		void init() {
			temp_stator = 0;
			temp_bulb = 0;
			
			format_index = 0;
			format_action_command = ProtocolStructure::StatusRegister::action_code::STAT_UNDEFINED;
			format_data.back = 0;
			format_data.front = 0;
			format_data.left = 0;
			format_data.right = 0;
			format_data.trap = 0;
			
			filter_index = 0;
			filter_action_command = ProtocolStructure::StatusRegister::action_code::STAT_UNDEFINED;
			filter_position = 0;

			mirror_index = ProtocolStructure::StatusRegister::mirror_target_code::OUT_FIELD;
			mirror_action_command = ProtocolStructure::StatusRegister::action_code::STAT_UNDEFINED;
			mirror_position = 0;

			power_light_timer = 0;
			power_light = ProtocolStructure::StatusRegister::light_target_code::LIGHT_OFF;

			FAN = false;

		}

		int temp_stator;
		int temp_bulb;	

		// Format Collimation 
		int format_index;
		ProtocolStructure::StatusRegister::action_code format_action_command;
		ProtocolStructure::format_collimation_data format_data;

		// Filter Selection
		int filter_index;
		ProtocolStructure::StatusRegister::action_code filter_action_command;
		int filter_position;

		ProtocolStructure::StatusRegister::mirror_target_code mirror_index;
		ProtocolStructure::StatusRegister::action_code mirror_action_command;
		int mirror_position;

		int  power_light_timer;
		ProtocolStructure::StatusRegister::light_target_code power_light;

		bool FAN;

	};

	ref class hardware_inputs {
	public:
		hardware_inputs() { init(); }
		void init() {
			
		}

		

	};

	ref class hardware_outputs {
	public:
		hardware_outputs() { init(); }
		void init() {
			
		}

		

	};

	
	static hardware_device device;
	static hardware_inputs inputs;
	static hardware_outputs outputs;
	static ProtocolStructure protocol;
	static cli::array<System::String^>^ filter_slot_assignment;
};

