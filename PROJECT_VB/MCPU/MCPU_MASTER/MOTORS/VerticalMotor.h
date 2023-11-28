#pragma once
#include "CanOpenMotor.h"
ref class VerticalMotor : public CANOPEN::CanOpenMotor
{
public:
	VerticalMotor(void);
	static VerticalMotor^ device = gcnew VerticalMotor();

protected:
	bool initializeSpecificObjectDictionary(void) override; //!< Sets specific registers for the Vertical activation

};

