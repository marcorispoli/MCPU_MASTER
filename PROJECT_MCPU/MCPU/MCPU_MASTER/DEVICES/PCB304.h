#pragma once
#include "CanDeviceProtocol.h"

/// <summary>
/// \defgroup PCB304_Module PCB304 Module: Grid device module controller
/// \ingroup CanDevice_Module  
/// </summary>
ref class PCB304 : public CanDeviceProtocol
{
public:
	/// <summary>
	/// This class implement the protocol data structure as described in the protocol specification.
	/// </summary>
	ref class ProtocolStructure {
	public:

		ref class StatusRegister {
		public:

			enum class register_index {
				GRID = 0,	//!> This is the Status register with the Grid internal status				
				NUM_REGISTER
			};

			static bool decodeGridRegister(Register^ sys) {
				if (sys == nullptr) return false;
				
				inField =				sys->d0 & 0x1;
				outField =				sys->d0 & 0x2;
				inout_executing =	sys->d0 & 0x4;
				home =				sys->d0 & 0x8;
				center =			sys->d0 & 0x10;
				transversal_executing = sys->d0 & 0x20;
				xray_disable =		sys->d0 & 0x40;
				grid_start =		sys->d0 & 0x80;

				error =				sys->d1 & 0x1;

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

				if(display_on) out->d0 |= 0x1;
				if (display_blink) out->d0 |= 0x2;
				if (display_reverse) out->d0 |= 0x4;
				if (display_keep_alive) out->d0 |= 0x8;
				out->d0 |= ((dot_position & 0x0f) << 4);

				out->d1 = (display_intensity & 0x0f);
				
				unsigned short val = (unsigned short) display_value;
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
				if(InField)	return gcnew CanDeviceCommand((unsigned char)command_index::SET_INOUT, 1, 0, 0, 0);
				else	return gcnew CanDeviceCommand((unsigned char)command_index::SET_INOUT, 2, 0, 0, 0);
			}

			CanDeviceCommand^ encodeActivateFullTestCommand(unsigned char cycles, unsigned char nfast, unsigned char nslow) {
				return gcnew CanDeviceCommand((unsigned char)command_index::ACTIVATE_GRID_TEST, (System::Byte)command_test_codes::FULL_TEST,cycles, nfast, nslow);
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

	PCB304() : CanDeviceProtocol(0x14, L"POTTER_DEVICE")
	{
		// Status initializtion
		protocol.data_register.GeneralEnable = false;
		protocol.data_register.InOutAutoEnable = false;
		protocol.data_register.EnableStartGrid = false;
		protocol.data_register.ManualXrayDisableEnable = false;
	}
	static PCB304^ device = gcnew PCB304();

	literal int max_num_error_resets = 5; //!< Maximum number of attempts to reset a pending error condition

	static void setGridInOutTest(void) { test_grid_in_out = true; };
	static void setAutoGridInField(void) { protocol.data_register.GeneralEnable = true; protocol.data_register.InOutAutoEnable = true; protocol.data_register.InOutStatus = true; };
	static void setAutoGridOutField(void) { protocol.data_register.GeneralEnable = true; protocol.data_register.InOutAutoEnable = true; protocol.data_register.InOutStatus = false; };
	static void syncGeneratorOn(void) { protocol.data_register.ManualXrayDisableEnable = false; protocol.data_register.EnableStartGrid = true; }
	static void syncGeneratorOff(void) { protocol.data_register.ManualXrayDisableEnable = true; protocol.data_register.ManualXrayDisableStatus = true; protocol.data_register.EnableStartGrid = false; }
	inline static bool isGridOnFieldReady(void) { return protocol.status_register.inField; }
	inline static bool isGridOffFieldReady(void) { return protocol.status_register.outField;}
	inline static void enableStartGrid(bool status) { protocol.data_register.EnableStartGrid = status; }

	inline static bool isError(void) { return protocol.status_register.error; }
	inline static void setDisplay(bool on_off) {protocol.data_register.display_on = on_off;}

	static void setDisplay(short val, unsigned char decimals, bool blink, unsigned char intensity) {
		protocol.data_register.display_on = true;
		protocol.data_register.display_blink = blink;
		protocol.data_register.display_value = val;
		protocol.data_register.dot_position = decimals;
		protocol.data_register.display_intensity = intensity;
	}

protected: 
	void runningLoop(void) override;
	void demoLoop(void) override;

private:
	static bool test_grid_in_out = false;
	static ProtocolStructure protocol; // This is the structure with the Status register info
		
};

