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
	private:enum class StatusRegisters {
		SYSTEM_STATUS_REGISTER = 0, //!> This is the System Status register index		
	};

	#define PCB326_GET_SYSTEM_CS1_STATUS(reg)			(bool)	(reg->d0 & 0x01)
	#define PCB326_GET_SYSTEM_CS2_STATUS(reg)			(bool)	(reg->d0 & 0x02)
	#define PCB326_GET_SYSTEM_CS3_STATUS(reg)			(bool)	(reg->d0 & 0x04)
	#define PCB326_GET_SYSTEM_CS4_STATUS(reg)			(bool)	(reg->d0 & 0x08)
	#define PCB326_GET_SYSTEM_CS5_STATUS(reg)			(bool)	(reg->d0 & 0x10)
	#define PCB326_GET_SYSTEM_CS6_STATUS(reg)			(bool)	(reg->d0 & 0x20)
	#define PCB326_GET_SYSTEM_CS7_STATUS(reg)			(bool)	(reg->d0 & 0x40)
	#define PCB326_GET_SYSTEM_CS8_STATUS(reg)			(bool)	(reg->d0 & 0x80)

	#define PCB326_GET_SYSTEM_RESET_STATUS(reg)			(bool)	(reg->d1 & 0x1)
	#define PCB326_GET_SYSTEM_CONFIG_STATUS(reg)			(bool)	(reg->d1 & 0x2)
	#define PCB326_GET_SYSTEM_RUN_STATUS(reg)			(bool)	(reg->d1 & 0x4)

	/// <summary>
	///	 This is the Device DATA Register implementation 
	/// </summary>
	private:enum class DataRegisters {
		OUTPUTS_DATA_REGISTER = 0, //!> This is the Outputs Data register index
	};

	#define PCB326_OUTPUTS_RESET(reg,stat)	reg->D0(stat, 0x1)

public:

	

	PCB326() : CanDeviceProtocol(0x16, L"OBSTACLE_DEVICE")
	{

	}
	static PCB326^ device = gcnew PCB326();

protected: 	
	void runningLoop(void) override;
	bool configurationLoop(void) override;//!< This is the Base Class override function to handle the device configuration

private:
	static Register^ outputs_data_register = gcnew Register();
	void handleSystemStatusRegister(void);

};

