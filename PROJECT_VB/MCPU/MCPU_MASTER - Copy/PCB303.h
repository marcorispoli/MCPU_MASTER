#pragma once
#include "CanDeviceProtocol.h"

ref class PCB303 : public CanDeviceProtocol
{
public:

#define GET_STATUS_SYSTEM_REGISTER (Byte) 0, (Byte) 4,(Byte) 0, (Byte) 0,(Byte) 0,(Byte) 0, (Byte) 0,(Byte) 0, false



	PCB303() : CanDeviceProtocol(0x12, L"COLLIMATOR_DEVICE")
	{

	}

protected: 	virtual void runningLoop(void) override;

};


