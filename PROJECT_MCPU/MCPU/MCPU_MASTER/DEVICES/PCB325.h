#pragma once
#include "CanDeviceProtocol.h"

/// <summary>
/// \defgroup PCB326_Module PCB326 Module: Obstacle device module controller
/// 
/// \ingroup CanDevice_Module  
/// </summary>
ref class PCB325 : public CanDeviceProtocol
{


public:


	PCB325() : CanDeviceProtocol(0x15, L"BIOPSY_DEVICE")
	{

	}
	static PCB325^ device = gcnew PCB325();

	/// <summary>
	/// This class implement the protocol data structure as described in the protocol specification.
	/// </summary>
	ref class ProtocolStructure {
	public:


		ref class StatusRegister {
		public:

			enum class register_index {
				MODE_REGISTER = 0, 
				POSITION_XY_REGISTER,
				POSITION_Z_REGISTER,
				NUM_REGISTERS
			};

			enum class working_mode {
				DISABLE_MODE = 0,
				CALIB_MODE,
				COMMAND_MODE,
				SERVICE_MODE

			};

			

			static bool decodeModeRegister(Register^ sys) {
				if (sys == nullptr) return false;

				mode = (working_mode) sys->d0;
				general_enable = (sys->d0 & 0x1);
				keyboard_enable = (sys->d0 & 0x2);
				enable_feedback = (sys->d0 & 0x4);
				disable_needle_feedback = (sys->d0 & 0x8);
				return true;
			}

			Register^ encodeModeRegister(void) {

				// Creates a register with all bytes set to 0
				CanDeviceProtocol::Register^ sys = gcnew CanDeviceProtocol::Register;

				
				sys->d0 = (unsigned char)mode;
				
				sys->d1 = 0;
				if (general_enable) sys->d1 |= 0x1;
				if (keyboard_enable) sys->d1 |= 0x2;
				if (enable_feedback) sys->d1 |= 0x4;
				if (disable_needle_feedback) sys->d1 |= 0x8;

				sys->d2 = 0;
				sys->d3 = 0;

				// Returns the formatted register
				return sys;
			}

			static bool decodePositionXYRegister(Register^ sys) {
				if (sys == nullptr) return false;

				X = (int) sys->d0 + 256 * (int) sys->d1;
				Y = (int)sys->d2 + 256 * (int)sys->d3;
				return true;
			}

			Register^ encodePositionXYRegister(void) {

				// Creates a register with all bytes set to 0
				CanDeviceProtocol::Register^ sys = gcnew CanDeviceProtocol::Register;


				sys->d0 = (unsigned char) (X & 0xFF);
				sys->d1 = (unsigned char)((X>>8) & 0xFF);
				sys->d2 = (unsigned char)(Y & 0xFF);
				sys->d3 = (unsigned char)((Y >> 8) & 0xFF);

				// Returns the formatted register
				return sys;
			}
			static bool decodePositionZRegister(Register^ sys) {
				if (sys == nullptr) return false;

				Z = (int)sys->d0 + 256 * (int)sys->d1;
				
				return true;
			}

			Register^ encodePositionZRegister(void) {

				// Creates a register with all bytes set to 0
				CanDeviceProtocol::Register^ sys = gcnew CanDeviceProtocol::Register;


				sys->d0 = (unsigned char)(Z & 0xFF);
				sys->d1 = (unsigned char)((Z >> 8) & 0xFF);
				sys->d2 = 0;
				sys->d3 = 0;

				// Returns the formatted register
				return sys;
			}

			static working_mode mode = working_mode::DISABLE_MODE;
			static bool general_enable = false;
			static bool keyboard_enable = false;
			static bool enable_feedback = false;
			static bool disable_needle_feedback = false;
			static int X = 0;
			static int Y = 0;
			static int Z = 0;
		};


		ref class DataRegister {
		public:

			enum class register_index {
				GENERAL_REGISTER = 0,
				NUM_REGISTERS
			};

			static bool decodeGeneralRegister(Register^ sys) {
				if (sys == nullptr) return false;

				light_status = sys->d0 & 0x1;
				step_keyb_ena = sys->d0 & 0x2;

				light_duty = sys->d1;

				return true;
			}

			Register^ encodeGeneralRegister(void) {

				// Creates a register with all bytes set to 0
				CanDeviceProtocol::Register^ sys = gcnew CanDeviceProtocol::Register;


				sys->d0 = 0;
				if (light_status) sys->d0 |= 0x1;
				if (step_keyb_ena) sys->d0 |= 0x2;

				sys->d1 = light_duty;
				sys->d2 = 0;
				sys->d3 = 0;

				// Returns the formatted register
				return sys;
			}

			static bool step_keyb_ena = false;
			static bool light_status = false;
			
			static int  light_duty = 20;


		};

		ref class ParameterRegister {
		public:

			enum class register_index {
				NUM_REGISTERS = 0,
			};

			
		};

		ref class Commands {
		public:
			enum class command_index {
				CMD_ABORT = 0,          
				CMD_DISABLE_MODE,
				CMD_COMMAND_MODE,
				CMD_SERVICE_MODE,
				CMD_CALIB_MODE,
				CMD_MOVE_X,
				CMD_MOVE_Y,
				CMD_MOVE_Z,
				CMD_SERVICE_TEST_CYCLE      
			};

			CanDeviceCommand^ encodeDisableModeCommand(void) {				
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_DISABLE_MODE, 0, 0, 0, 0);
			}
			CanDeviceCommand^ encodeCommandModeCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_COMMAND_MODE, 0, 0, 0, 0);
			}
			CanDeviceCommand^ encodeServiceModeCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_SERVICE_MODE, 0, 0, 0, 0);
			}
			CanDeviceCommand^ encodeCalibModeCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_CALIB_MODE, 0, 0, 0, 0);
			}
			CanDeviceCommand^ encodeTestCycleModeCommand(void) {
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_SERVICE_TEST_CYCLE, 0, 0, 0, 0);
			}
			CanDeviceCommand^ encodeMoveXCommand(int X) {			
				System::Byte d0 = (System::Byte)(X & 0xFF);
				System::Byte d1 = (System::Byte)((X>>8) & 0xFF);
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_MOVE_X, d0, d1, 0, 0);
			}
			CanDeviceCommand^ encodeMoveYCommand(int Y) {
				System::Byte d0 = (System::Byte)(Y & 0xFF);
				System::Byte d1 = (System::Byte)((Y >> 8) & 0xFF);
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_MOVE_Y, d0, d1, 0, 0);
			}
			CanDeviceCommand^ encodeMoveZCommand(int Z) {
				System::Byte d0 = (System::Byte)(Z & 0xFF);
				System::Byte d1 = (System::Byte)((Z >> 8) & 0xFF);
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_MOVE_Z, d0, d1,0, 0);
			}

		};

		static StatusRegister status_register;
		static DataRegister data_register;
		static ParameterRegister parameter_register;
		static Commands command;
	};


	static bool isBiopsyConnected(void) { return biopsy_connected; }
	static bool activateCycleTest(void);
	static void setLight(bool status, int duty) { protocol.data_register.light_status = status;  protocol.data_register.light_duty = duty; }
	static void moveHome(void) { command_home_request = true; }

protected: 	
	void runningLoop(void) override;
	bool configurationLoop(void) override;//!< This is the Base Class override function to handle the device configuration

private:
	static ProtocolStructure protocol; //!< This is the structure with the Status register info
	static bool biopsy_connected = false;
	static bool service_cycle_test = false;
	static bool command_home_request = false;
	static int sequence = 0;
};

