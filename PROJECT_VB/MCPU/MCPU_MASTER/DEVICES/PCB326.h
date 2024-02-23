#pragma once
#include "CanDeviceProtocol.h"

/// <summary>
/// \defgroup PCB326_Module PCB326 Module: Obstacle device module controller
/// 
/// \ingroup CanDevice_Module  
/// </summary>
ref class PCB326 : public CanDeviceProtocol
{


	/// <summary>
	///		This is the Device STATUS Register implementation 
	/// </summary>
	private: enum class StatusRegisters {
		SYSTEM_STATUS_REGISTER = 0, //!> This is the System Status register index		
	};

	#define PCB326_GET_SYSTEM_CS_STATUS(reg)			(unsigned char) reg->d0
	#define PCB326_GET_SYSTEM_CS1_STATUS(reg)			(bool)	(reg->d0 & 0x01)
	#define PCB326_GET_SYSTEM_CS2_STATUS(reg)			(bool)	(reg->d0 & 0x02)
	#define PCB326_GET_SYSTEM_CS3_STATUS(reg)			(bool)	(reg->d0 & 0x04)
	#define PCB326_GET_SYSTEM_CS4_STATUS(reg)			(bool)	(reg->d0 & 0x08)
	#define PCB326_GET_SYSTEM_CS5_STATUS(reg)			(bool)	(reg->d0 & 0x10)
	#define PCB326_GET_SYSTEM_CS6_STATUS(reg)			(bool)	(reg->d0 & 0x20)
	#define PCB326_GET_SYSTEM_CS7_STATUS(reg)			(bool)	(reg->d0 & 0x40)
	#define PCB326_GET_SYSTEM_CS8_STATUS(reg)			(bool)	(reg->d0 & 0x80)

	#define PCB326_GET_SYSTEM_RESET_STATUS(reg)			(bool)	(reg->d1 & 0x1)
	#define PCB326_GET_SYSTEM_CONFIG_STATUS(reg)		(bool)	(reg->d1 & 0x2)
	#define PCB326_GET_SYSTEM_CALIB_STATUS(reg)			(bool)	(reg->d1 & 0x4)
	#define PCB326_GET_SYSTEM_RUN_STATUS(reg)			(bool)	(reg->d1 & 0x8)

	/// <summary>
	///	 This is the Device DATA Register implementation 
	/// </summary>
	private: enum class DataRegisters {
		CONTROL_DATA_REGISTER = 0, //!> This is the enable sensor data register 
		
	};
	
	private: static inline Register^ setControlDataRegister(bool reset, bool config) {
		unsigned char d0 = 0;
		if (reset) d0 |= 1;
		if (config) d0 |= 2;
		return gcnew Register((unsigned char) d0, (unsigned char)0, (unsigned char)0, (unsigned char)0);
	}

	
	/// <summary>
	///	 This is the Device Parameters Register implementation 
	/// </summary>
	private: enum class ParamRegisters {
		SENSITIVITY_PARAMETER_REGISTER = 0, //!> This is the Ou
		CS1_4_THRESHOLD_PARAMETER_REGISTER,
		CS5_8_THRESHOLD_PARAMETER_REGISTER,
	};

	#define SENS_CS1 0x1
	#define SENS_CS2 0x2
	#define SENS_CS3 0x4
	#define SENS_CS4 0x8
	#define SENS_CS5 0x10
	#define SENS_CS6 0x20
	#define SENS_CS7 0x40
	#define SENS_CS8 0x80
	
	public: enum class GeneralSensOption {
		GEN_1x = 0,		//!> This is the 1x General gain
		GEN_2x,			//!> This is the 2x General gain
		GEN_4x,			//!> This is the 4x General gain
		GEN_8x,			//!> This is the 8x General gain
	};

	public: enum class SensorSensOption {
		SENS_128x = 0,	//!> This is the 128x Sensor gain
		SENS_64x,		//!> This is the 64x Sensor gain
		SENS_32x,		//!> This is the 32x Sensor gain
		SENS_16x,		//!> This is the 16x Sensor gain
		SENS_8x,		//!> This is the 8x Sensor gain
		SENS_4x,		//!> This is the 4x Sensor gain
		SENS_2x,		//!> This is the 2x Sensor gain
		SENS_1x,		//!> This is the 1x Sensor gain
	};

	public: enum class RecalibrationOption {
		RECAL_560ms = 0,	
		RECAL_840ms,
		RECAL_1120ms,
		RECAL_1400ms,
		RECAL_1680ms,
		RECAL_2240ms,
		RECAL_2800ms,
		RECAL_3360ms,
		RECAL_3920ms,
		RECAL_4480ms,
		RECAL_5600ms,
		RECAL_6720ms,
		RECAL_7840ms,
		RECAL_8906ms,
		RECAL_10080ms,
		RECAL_11200ms,

	};

	private: static inline Register^ setSensitivitRegister(GeneralSensOption gen_gain, SensorSensOption sensor_sens, RecalibrationOption recalib_time, unsigned char sensors_ena) {
		return gcnew Register((unsigned char)gen_gain, (unsigned char)sensor_sens, (unsigned char)recalib_time, (unsigned char) sensors_ena);
	}

	
	/// <summary>	
	/// This enumeration class defines the Indexes of the Command Execution
	///
	/// </summary>
	private: enum class CommandRegister {
		ABORT_COMMAND = 0,		//!< Reserved for the protocol
		RESET_COMMAND,			//!< Executes the reset of the CP1188 device
		CONFIG_COMMAND,			//!< Executes the reconfiguration of the CP1188 device		
		CALIBRATE_COMMAND,		//!< Executes the recalibration of the CP1188 device
		NO_COMMAND 
	};

	private: inline bool resetCommand(void) { return command((unsigned char)CommandRegister::RESET_COMMAND, 0, 0, 0, 0, 10); }
	private: inline bool configCommand(void) { return command((unsigned char)CommandRegister::CONFIG_COMMAND, 0, 0, 0, 0, 10); }
	private: inline bool calibrateCommand(void) { return command((unsigned char)CommandRegister::CALIBRATE_COMMAND, 0, 0, 0, 0, 10); }

public:


	PCB326() : CanDeviceProtocol(0x16, L"OBSTACLE_DEVICE")
	{

	}
	static PCB326^ device = gcnew PCB326();

	static bool executeObstacleReset(void) {
		if (current_command != CommandRegister::NO_COMMAND) return false;
		current_command = CommandRegister::RESET_COMMAND;
		return true;
	}

	static bool executeObstacleConfig(GeneralSensOption gen_gain, SensorSensOption sensor_sens, RecalibrationOption recalib_time, unsigned char sensors_ena) {
		if (current_command != CommandRegister::NO_COMMAND) return false;
		selected_gen_sens = gen_gain;
		selected_sensor_sens = sensor_sens;
		selected_recalib_time = recalib_time;
		selected_sensor_ena = sensors_ena;

		current_command = CommandRegister::CONFIG_COMMAND;
		return true;
	}

	static bool executeObstacleCalibrate(void) {
		if (current_command != CommandRegister::NO_COMMAND) return false;
		current_command = CommandRegister::CALIBRATE_COMMAND;
		return true;
	}

	static bool isSensorActive(unsigned char mask) {
		if (cs_status & mask) return true;
		return false;
	}


protected: 	
	void runningLoop(void) override;
	bool configurationLoop(void) override;//!< This is the Base Class override function to handle the device configuration

private:
	static Register^ outputs_data_register = gcnew Register();

	void handleSystemStatusRegister(void);
	
	static GeneralSensOption selected_gen_sens = GeneralSensOption::GEN_1x;
	static SensorSensOption  selected_sensor_sens = SensorSensOption::SENS_2x;
	static  RecalibrationOption selected_recalib_time = RecalibrationOption::RECAL_560ms;
	static unsigned char selected_sensor_ena = SENS_CS1;

	static CommandRegister current_command = CommandRegister::NO_COMMAND;
	static unsigned char cs_status = 0;

	static unsigned char cs1_th;
	static unsigned char cs2_th;
	static unsigned char cs3_th;
	static unsigned char cs4_th;
	static unsigned char cs5_th;
	static unsigned char cs6_th;
	static unsigned char cs7_th;
	static unsigned char cs8_th;


};

