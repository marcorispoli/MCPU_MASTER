#pragma once
#include "Motors.h"
using namespace System;


ref class TiltMotor : public Motors
{
public:


	TiltMotor(void);
	static TiltMotor^ device = gcnew TiltMotor();
	static void initialize(void) {};

private:

};

