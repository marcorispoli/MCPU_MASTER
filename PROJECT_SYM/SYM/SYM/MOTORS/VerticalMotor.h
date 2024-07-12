#pragma once
#include "Motors.h"
using namespace System;


ref class VerticalMotor : public Motors
{
public:


	VerticalMotor(void);
	static VerticalMotor^ device = gcnew VerticalMotor();
	static void initialize(void) {};

private:

};

