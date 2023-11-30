#pragma once
#include "CanOpenMotor.h"
ref class TiltMotor : public CANOPEN::CanOpenMotor
{
public:
	TiltMotor(void);
	static TiltMotor^ device = gcnew TiltMotor();

};

