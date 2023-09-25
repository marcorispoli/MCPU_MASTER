#pragma once
#include "CanOpenMotor.h"
ref class TiltMotor : public CANOPEN::CanOpenMotor
{
public:
	TiltMotor(unsigned char devid);


};

