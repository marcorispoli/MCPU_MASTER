#pragma once
#include "CanDeviceProtocol.h"

ref class PCB315 : public CanDeviceProtocol
{
public:

#define GET_STATUS_SYSTEM_REGISTER (Byte) 0, (Byte) 4,(Byte) 0, (Byte) 0,(Byte) 0,(Byte) 0, (Byte) 0,(Byte) 0, false
#define GET_SYSTEM_FILTER(d0,d1,d2,d3) (unsigned char) (d0)
#define GET_SYSTEM_STATOR(d0,d1,d2,d3) (unsigned char) (d1)
#define GET_SYSTEM_BULB(d0,d1,d2,d3) (unsigned char) (d2)


	PCB315() : CanDeviceProtocol(0x13, L"FILTER_DEVICE")
	{

	}

protected: 	virtual void runningLoop(void) override;

};

