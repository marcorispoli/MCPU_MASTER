#pragma once
#include "CanDeviceProtocol.h"

ref class PCB315 : public CanDeviceProtocol
{
public:

// Status registers
#define GET_STATUS_SYSTEM_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
#define GET_SYSTEM_FILTER(d0,d1,d2,d3) (unsigned char) (d0)
#define GET_SYSTEM_STATOR(d0,d1,d2,d3) (unsigned char) (d1)
#define GET_SYSTEM_BULB(d0,d1,d2,d3) (unsigned char) (d2)

// Parameters
#define WRITE_FILTER1_REGISTER(val) (System::Byte) 0, (System::Byte) 8,(System::Byte) 1,(System::Byte) (val&0xFF), (System::Byte) ((val>>8)&0xFF),(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
#define WRITE_FILTER2_REGISTER(val) (System::Byte) 0, (System::Byte) 8,(System::Byte) 2,(System::Byte) (val&0xFF), (System::Byte) ((val>>8)&0xFF),(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
#define WRITE_FILTER3_REGISTER(val) (System::Byte) 0, (System::Byte) 8,(System::Byte) 3,(System::Byte) (val&0xFF), (System::Byte) ((val>>8)&0xFF),(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
#define WRITE_FILTER4_REGISTER(val) (System::Byte) 0, (System::Byte) 8,(System::Byte) 4,(System::Byte) (val&0xFF), (System::Byte) ((val>>8)&0xFF),(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
#define WRITE_MIRROR_REGISTER(val) (System::Byte) 0, (System::Byte) 8,(System::Byte) 5,(System::Byte) (val&0xFF), (System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false


	PCB315() : CanDeviceProtocol(0x13, L"FILTER_DEVICE")
	{

	}

protected: 	void runningLoop(void) override;
protected: 	bool configurationLoop(void) override;

};

