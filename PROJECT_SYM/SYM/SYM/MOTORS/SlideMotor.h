#pragma once
#include "Motors.h"
using namespace System;


ref class SlideMotor : public Motors
{
public:


	SlideMotor(void);
	static SlideMotor^ device = gcnew SlideMotor();
	static void initialize(void) {};

private:

};

