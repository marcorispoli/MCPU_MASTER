#pragma once
#include "CanDeviceProtocol.h"

ref class PCB326 : public CanDeviceProtocol
{
public:

//#define GET_STATUS_SYSTEM_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
//#define GET_SYSTEM_FILTER(d0,d1,d2,d3) (unsigned char) (d0)
//#define GET_SYSTEM_STATOR(d0,d1,d2,d3) (unsigned char) (d1)
//#define GET_SYSTEM_BULB(d0,d1,d2,d3) (unsigned char) (d2)


	PCB326() : CanDeviceProtocol(0x16, L"OBSTACLE_DEVICE")
	{

	}

protected: 	virtual void runningLoop(void) override;

};

