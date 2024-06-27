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
				SYSTEM_REGISTER = 0, //!> This is the System Status register index
			};

			static bool decodeSystemRegister(CanDeviceProtocol::Register^ sys) {
				if (sys == nullptr) return false;

				
				grid_sync_stat = sys->d0 & 0x1;		
				return true;
			}

			static bool grid_sync_stat = false;//!< The grid is in sync mode.
			static bool grid_on_field = false; //!< The grid is on field
			static bool grid_on_field_ready = false; //!< This flag stands for grid correctly positioned in Field
			static bool grid_off_field_ready = false; //!< This flag stands for grid correctly positioned out of Field
		};


		ref class DataRegister {
		public:

			enum class register_index {
				DISPLAY_REGISTER = 0,	//!> This is the Display Data register index
				GRID_REGISTER			//!< This is the control of the Grid status
			};

			CanDeviceProtocol::Register^ encodeDisplayRegister(void) {

				// Creates a register with all bytes set to 0
				CanDeviceProtocol::Register^ out = gcnew CanDeviceProtocol::Register;

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

			CanDeviceProtocol::Register^ encodeGridRegister(void) {

				// Creates a register with all bytes set to 0
				CanDeviceProtocol::Register^ out = gcnew CanDeviceProtocol::Register;

				if (grid_sync) out->d0 |= 0x1;
				
				// Returns the formatted register
				return out;
			}


			
			static bool display_on = false;
			static bool display_blink = false;
			static bool display_reverse = false;
			static bool display_keep_alive = false;
			static unsigned char dot_position = 0;
			static unsigned char display_intensity = 0;
			static short display_value = 0;
			static bool grid_sync = false;
		};

		ref class ParameterRegister {
		public:
			enum class command_index {
				PARAM_REGISTER = 0,				
			};

			//writeParamRegister(unsigned char idx, Register^ reg)

			Register^ encodeParamRegister(void) {
				return gcnew Register(0, 0, 0, 0);
			}

		};

		ref class Commands {
		public:
			enum class command_index {
				ABORT_COMMAND = 0, //!< Abort Command (mandatory as for device protocol)
				
			};

			CanDeviceProtocol::CanDeviceCommand^ encodeAbortCommand(void) {
				return gcnew CanDeviceProtocol::CanDeviceCommand((unsigned char)command_index::ABORT_COMMAND, 0, 0, 0, 0);
			}

		};

		static StatusRegister status_register;
		static DataRegister data_register;
		static ParameterRegister parameter_register;
		static Commands command;
	};

	PCB304() : CanDeviceProtocol(0x14, L"POTTER_DEVICE")
	{
		
	}
	static PCB304^ device = gcnew PCB304();

	
	
	static bool setGridOnField(bool wait_completion);
	static bool setGridOffField(bool wait_completion);
	inline static void synchGridWithGenerator(bool status) { protocol.data_register.grid_sync = status; }
	inline static bool isGridOnFieldReady(void) { return protocol.status_register.grid_on_field_ready; }
	inline static bool isGridOffFieldReady(void) { return protocol.status_register.grid_off_field_ready; }
	inline static bool isGridSynchReady(void) { return protocol.status_register.grid_sync_stat; }
	inline static bool isError(void) { return false; } // TO BE DONE
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

	static ProtocolStructure protocol; // This is the structure with the Status register info
		
};

