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
				POSITION_REGISTER,
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

			static bool decodePositionRegister(Register^ sys) {
				if (sys == nullptr) return false;

				X = (int) sys->d0 + 256 * (int) sys->d1;
				Y = (int)sys->d2;
				Z = (int)sys->d3;
				return true;
			}

			Register^ encodePositionRegister(void) {

				// Creates a register with all bytes set to 0
				CanDeviceProtocol::Register^ sys = gcnew CanDeviceProtocol::Register;


				sys->d0 = (unsigned char) (X & 0xFF);
				sys->d1 = (unsigned char)((X>>8) & 0xFF);
				sys->d2 = (unsigned char)(Y);
				sys->d3 = (unsigned char)(Z);

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
				NUM_REGISTERS = 0,
			};

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
				CMD_MOVE_XYZ,                
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
			CanDeviceCommand^ encodeMoveXYZModeCommand(int X, int Y, int Z) {
				System::Byte d0 = (System::Byte)(X & 0xFF);
				System::Byte d1 = (System::Byte)((X>>8) & 0xFF);
				System::Byte d2 = (System::Byte)(Y);
				System::Byte d3 = (System::Byte)(Z);
				return gcnew CanDeviceCommand((unsigned char)command_index::CMD_MOVE_XYZ, d0, d1, d2, d3);
			}

		};

		static StatusRegister status_register;
		static DataRegister data_register;
		static ParameterRegister parameter_register;
		static Commands command;
	};


	static bool isBiopsyConnected(void) { return biopsy_connected; }
	static bool activateCycleTest(void);

protected: 	
	void runningLoop(void) override;
	bool configurationLoop(void) override;//!< This is the Base Class override function to handle the device configuration

private:
	static ProtocolStructure protocol; //!< This is the structure with the Status register info
	static bool biopsy_connected = false;
	static bool service_cycle_test = false;
};

