#pragma once
#include "CanDeviceProtocol.h"
ref class PCB301 :  public CanDeviceProtocol
{
public:
	
	#define GET_STATUS_SYSTEM_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
	#define GET_SYSTEM_BATTENA(d0,d1,d2,d3) (bool) (d0 & 0x80)
	#define GET_SYSTEM_BATT1LOW(d0,d1,d2,d3) (bool) (d0 & 0x20)
	#define GET_SYSTEM_BATT2LOW(d0,d1,d2,d3) (bool) (d0 & 0x40)
	#define GET_SYSTEM_POWERDOWN(d0,d1,d2,d3) (bool) (d0 & 0x04)
	#define GET_SYSTEM_CLOSEDOOR(d0,d1,d2,d3) (bool) (d2 & 0x10)


	#define GET_STATUS_BATTERY_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
	#define GET_BATTERY_VBATT1(d3,d4,d5,d6) (unsigned char) (d3)
	#define GET_BATTERY_VBATT2(d3,d4,d5,d6) (unsigned char) (d4)


	PCB301(): CanDeviceProtocol(0x10, L"POWER_SERVICE")
	{

	}

	protected: 	virtual void runningLoop(void) override;

};

