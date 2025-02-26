#pragma once
#include "deviceInterface.h"

using namespace System;

#define NUM_STATUS (Byte) 10
#define NUM_DATA (Byte) 10
#define NUM_PARAM (Byte) 10



ref class PCB304: public deviceInterface
{
public:
	PCB304(void) :deviceInterface((unsigned short)0x14, setRegVal(1, 0, 0, 0), NUM_STATUS, NUM_DATA, NUM_PARAM) {

	}
	static void  initialize(void) {};
	static PCB304^ board = gcnew PCB304;
	void device_workflow_callback(void) override;
	void device_reset_callback(void) override;


	ref class ProtocolStructure {
	public:
		literal int display_keep_alive_tmo = 20;
		ref class StatusRegister {
		public:

			enum class register_index {
				GRID = 0,	//!> This is the Status register with the Grid internal status				
				NUM_REGISTER
			};

			static bool decodeGridRegister(Register^ sys) {
				if (sys == nullptr) return false;

				inField = sys->d0 & 0x1;
				outField = sys->d0 & 0x2;
				inout_executing = sys->d0 & 0x4;
				home = sys->d0 & 0x8;
				center = sys->d0 & 0x10;
				transversal_executing = sys->d0 & 0x20;
				xray_disable = sys->d0 & 0x40;
				grid_start = sys->d0 & 0x80;

				error = sys->d1 & 0x1;

				current_test_cycle = sys->d2;
				return true;
			}

			Register^ encodeGridRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ out = gcnew Register;

				if (inField) out->d0 |= 0x1;
				if (outField) out->d0 |= 0x2;
				if (inout_executing) out->d0 |= 0x4;
				if (home) out->d0 |= 0x8;
				if (center) out->d0 |= 0x10;
				if (transversal_executing) out->d0 |= 0x20;
				if (xray_disable) out->d0 |= 0x40;
				if (grid_start) out->d0 |= 0x80;

				if (error) out->d1 |= 0x1;

				out->d2 = current_test_cycle;

				// Returns the formatted register
				return out;
			}


			static bool inField = false; //!< This is the In-Field position flag
			static bool outField = false; //!< This is the Out-Field position flag
			static bool inout_executing = false; //!< An InOut execution command is pending
			static bool home = false; //!< The current grid transversal position is home
			static bool center = false; //!< The current grid transversal position is center
			static bool transversal_executing = false; //!< A transversal command is executing
			static bool xray_disable = false; //!< Mirror of the current XRay-Disable signal stat
			static bool grid_start = false; //!< Mirror of the current Grid-Start signal stat
			static bool error = false; //!< An error condition is present
			static unsigned char current_test_cycle = 0; //!< In case of a test session it reports the current test cycle
		};


		ref class DataRegister {
		public:

			enum class register_index {
				DISPLAY_REGISTER = 0,	//!> This is the Display Data register index
				GRID_REGISTER,			//!< This is the control of the Grid status
				NUM_REGISTER
			};

			Register^ encodeDisplayRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ out = gcnew Register;

				if (display_on) out->d0 |= 0x1;
				if (display_blink) out->d0 |= 0x2;
				if (display_reverse) out->d0 |= 0x4;
				if (display_keep_alive) out->d0 |= 0x8;
				out->d0 |= ((dot_position & 0x0f) << 4);

				out->d1 = (display_intensity & 0x0f);

				unsigned short val = (unsigned short)display_value;
				out->d2 = (unsigned char)(val & 0xFF);
				out->d3 = (unsigned char)((val >> 8) & 0xFF);

				// Returns the formatted register
				return out;
			}

			static bool decodeDisplayRegister(Register^ sys) {
				if (sys == nullptr) return false;

				display_on = sys->d0 & 0x1;
				display_blink = sys->d0 & 0x2;
				display_reverse = sys->d0 & 0x4;
				display_keep_alive = sys->d0 & 0x8;
				dot_position = (sys->d0 & 0xF0) >> 4;
				display_intensity = sys->d1 & 0x0F;
				display_value = sys->d2 + 256 * sys->d3;
				return true;
			}


			static bool display_on = false;
			static bool display_blink = false;
			static bool display_reverse = false;
			static bool display_keep_alive = false;
			static unsigned char dot_position = 0;
			static unsigned char display_intensity = 0;
			static short display_value = 0;



			Register^ encodeGridRegister(void) {

				// Creates a register with all bytes set to 0
				Register^ out = gcnew Register;

				if (GeneralEnable) out->d0 |= 0x1;
				if (InOutAutoEnable) out->d0 |= 0x2;
				if (InOutStatus) out->d0 |= 0x4;
				if (EnableStartGrid) out->d0 |= 0x8;
				if (ManualXrayDisableEnable) out->d0 |= 0x10;
				if (ManualXrayDisableStatus) out->d0 |= 0x20;

				// Returns the formatted register
				return out;
			}

			static bool decodeGridRegister(Register^ sys) {
				if (sys == nullptr) return false;

				GeneralEnable = sys->d0 & 0x1;
				InOutAutoEnable = sys->d0 & 0x2;
				InOutStatus = sys->d0 & 0x4;
				EnableStartGrid = sys->d0 & 0x8;
				ManualXrayDisableEnable = sys->d0 & 0x10;
				ManualXrayDisableStatus = sys->d0 & 0x20;

				return true;
			}

			static bool GeneralEnable = false; //!< Enable of the grid activities
			static bool InOutAutoEnable = false; //!< Enable of InOut automatic positioning
			static bool InOutStatus = false; //!< InOut automatic target position
			static bool EnableStartGrid = false; //!< Enable the usage of the START-GRID external signal
			static bool ManualXrayDisableEnable = false; //!< XRAY-DISABLE signal manual mode activation
			static bool ManualXrayDisableStatus = false; //!< XRAY-DISABLE signal manual mode target status

		};

		ref class ParameterRegister {
		public:
			enum class register_index {
				PARAM_REGISTER = 0,
				NUM_REGISTER
			};

		};

		ref class Commands {
		public:
			enum class command_index {
				ABORT_COMMAND = 0, //!< Abort Command (mandatory as for device protocol)
				RESET_ERROR, //!< Resets the current error condition
				SET_INOUT,//!< Set the In Out position (only with InOut manual mode)
				ACTIVATE_GRID_TEST, //!< Activate one of the available tests				
			};

			enum class command_test_codes {
				FULL_TEST = 1, //!< Full test Command code
				IN_OUT_TEST, //!< In_Out Test command code
				TRANSLATION_TEST,//!< Translation Test command code						
			};

			CanDeviceCommand^ encodeResetCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::ABORT_COMMAND, 0, 0, 0, 0);
			}

			CanDeviceCommand^ encodeAbortCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::RESET_ERROR, 0, 0, 0, 0);
			}

			CanDeviceCommand^ encodeSetInOutCommand(bool InField) {
				if (InField)	return gcnew CanDeviceCommand((unsigned char)command_index::SET_INOUT, 1, 0, 0, 0);
				else	return gcnew CanDeviceCommand((unsigned char)command_index::SET_INOUT, 0, 0, 0, 0);
			}

			CanDeviceCommand^ encodeActivateFullTestCommand(unsigned char cycles, unsigned char nfast, unsigned char nslow) {
				return gcnew CanDeviceCommand((unsigned char)command_index::ACTIVATE_GRID_TEST, (System::Byte)command_test_codes::FULL_TEST, cycles, nfast, nslow);
			}

			CanDeviceCommand^ encodeActivateInOutTestCommand(unsigned char cycles) {
				return gcnew CanDeviceCommand((unsigned char)command_index::ACTIVATE_GRID_TEST, (System::Byte)command_test_codes::IN_OUT_TEST, cycles, 0, 0);
			}

			CanDeviceCommand^ encodeActivateTranslationTestCommand(unsigned char cycles, unsigned char nfast, unsigned char nslow) {
				return gcnew CanDeviceCommand((unsigned char)command_index::ACTIVATE_GRID_TEST, (System::Byte)command_test_codes::TRANSLATION_TEST, cycles, nfast, nslow);
			}


		};

		static StatusRegister status_register;
		static DataRegister data_register;
		static ParameterRegister parameter_register;
		static Commands command;
	};
	
	ref class hardware_device {
	public:
		literal unsigned char INOUT_UNDEF = 0;
		literal unsigned char IN_FIELD = 1;
		literal unsigned char OUT_FIELD = 2;
		literal unsigned char INOUT_RUN = 3;

		literal unsigned char TRANSLATION_UNDEF = 0;
		literal unsigned char TRANSLATION_HOME = 1;
		literal unsigned char TRANSLATION_CENTER = 2;
		literal unsigned char TRANSLATION_RUN = 3;

		literal unsigned char blink_tmo = 5;
		

		hardware_device() { init(); }
		void init() {
			display_on = false;
			display_keepalive_stat = false;
			display_keepalive_count = 0;			
			display_value = 0;
			blink_count = 0;
			display_intensity = 0;
			
			grid_InOut_stat = INOUT_UNDEF;
			grid_Translation_stat = TRANSLATION_UNDEF;
		}

		// Display Device
		bool display_on;
		bool display_keepalive_stat;
		int display_keepalive_count;

		unsigned char display_intensity;
		int blink_count;
		float display_value;

		// Grid Device
		unsigned char grid_InOut_stat;
		unsigned char inout_run_count;
		unsigned char grid_Translation_stat;
	};

	ref class hardware_inputs {
	public:
		hardware_inputs() { init(); }
		void init() {
			grid_start = false;			
		}

		bool grid_start;

	};

	ref class hardware_outputs {
	public:
		hardware_outputs() { init(); }
		void init() {
			xray_disable = false;
		}

		bool xray_disable;

	};

	static hardware_device device;
	static hardware_inputs inputs;
	static hardware_outputs outputs;
	static ProtocolStructure protocol;

	// Internal management
	private:void updateStatusRegisters(void);
};

