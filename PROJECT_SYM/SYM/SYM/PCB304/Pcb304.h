#pragma once
#include "deviceInterface.h"

using namespace System;

#define NUM_STATUS (Byte) 10
#define NUM_DATA (Byte) 10
#define NUM_PARAM (Byte) 10



ref class PCB304: public deviceInterface
{
public:
	PCB304(void) :deviceInterface((unsigned short)0x14, setRegVal(1, 0, 0, 0), NUM_STATUS, NUM_DATA, NUM_PARAM) {

	}
	static void  initialize(void) {};
	static PCB304^ board = gcnew PCB304;
	

	void device_workflow_callback(void) override;
	void device_reset_callback(void) override;
	

};

