#pragma once
#include "CanDeviceProtocol.h"

ref class PCB304 : public CanDeviceProtocol
{
public:

#define PCB304_GET_STATUS_SYSTEM_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false



	PCB304() : CanDeviceProtocol(0x14, L"POTTER_DEVICE")
	{

	}

protected: 	virtual void runningLoop(void) override;

};

