#pragma once
#include "Motors.h"
using namespace System;


ref class ArmMotor : public Motors
{
public:


	ArmMotor(void);
	static ArmMotor^ device = gcnew ArmMotor();
	static void initialize(void) {};

	
private:

};

