#pragma once
#include "CanDeviceProtocol.h"

ref class PCB304 : public CanDeviceProtocol
{
public:

#define GET_STATUS_SYSTEM_REGISTER (Byte) 0, (Byte) 4,(Byte) 0, (Byte) 0,(Byte) 0,(Byte) 0, (Byte) 0,(Byte) 0, false



	PCB304() : CanDeviceProtocol(0x14, L"POTTER_DEVICE")
	{

	}

protected: 	virtual void runningLoop(void) override;

};

