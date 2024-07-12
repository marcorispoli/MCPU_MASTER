#pragma once
#include "Motors.h"
using namespace System;


ref class BodyMotor : public Motors
{
public:


	BodyMotor(void);
	static BodyMotor^ device = gcnew BodyMotor();
	static void initialize(void) {};

private:

};

