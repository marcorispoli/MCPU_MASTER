#pragma once
#include "CanDeviceProtocol.h"

ref class PCB303 : public CanDeviceProtocol
{
public:

#define GET_STATUS_SYSTEM_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false



	PCB303() : CanDeviceProtocol(0x12, L"COLLIMATOR_DEVICE")
	{

	}

protected: 	virtual void runningLoop(void) override;

};


