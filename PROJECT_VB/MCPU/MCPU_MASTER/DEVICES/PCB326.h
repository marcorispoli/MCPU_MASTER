#pragma once
#include "CanDeviceProtocol.h"

ref class PCB326 : public CanDeviceProtocol
{
public:



	PCB326() : CanDeviceProtocol(0x16, L"OBSTACLE_DEVICE")
	{

	}
	static PCB326^ device = gcnew PCB326();

protected: 	
	virtual void runningLoop(void) override;
private:
	

};

