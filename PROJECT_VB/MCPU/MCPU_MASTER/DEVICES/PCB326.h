#pragma once
#include "CanDeviceProtocol.h"

/// <summary>
/// \defgroup PCB326_Module PCB326 Module: Obstacle device module controller
/// 
/// \ingroup CanDevice_Module  
/// </summary>
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

