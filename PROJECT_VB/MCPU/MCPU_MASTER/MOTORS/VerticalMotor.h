#pragma once
#include "CanOpenMotor.h"
ref class VerticalMotor : public CANOPEN::CanOpenMotor
{
public:
	VerticalMotor(void);

protected:
	bool initializeSpecificObjectDictionary(void) override; //!< Sets specific registers for the Vertical activation

};

