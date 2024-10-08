#pragma once
#include "CanDeviceProtocol.h"

/// <summary>
/// \defgroup PCB326_Module PCB326 Module: Obstacle device module controller
/// 
/// \ingroup CanDevice_Module  
/// </summary>
ref class PCB325 : public CanDeviceProtocol
{


public:


	PCB325() : CanDeviceProtocol(0x15, L"BIOPSY_DEVICE")
	{

	}
	static PCB325^ device = gcnew PCB325();

	


protected: 	
	void runningLoop(void) override;
	bool configurationLoop(void) override;//!< This is the Base Class override function to handle the device configuration

private:
	

};

