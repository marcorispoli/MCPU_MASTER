#pragma once
#include "CanDeviceProtocol.h"

ref class PCB302 : public CanDeviceProtocol
{
public:

#define GET_STATUS_SYSTEM_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false



	PCB302() : CanDeviceProtocol(0x11, L"COMPRESSOR_DEVICE")
	{

	}

protected: 	virtual void runningLoop(void) override;

};


