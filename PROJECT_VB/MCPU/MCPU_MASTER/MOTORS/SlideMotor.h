#pragma once
#include "CanOpenMotor.h"
#include "CalibrationConfig.h"

ref class SlideMotor : public CANOPEN::CanOpenMotor
{
public:
	SlideMotor(void);
	static SlideMotor^ device = gcnew SlideMotor();        
    static bool startHoming(void);
    static inline void setManualEnable(bool status) { manual_activation_enabled = status; } //!< Enables / Disables the manual activation mode

protected:
    bool initializeSpecificObjectDictionaryCallback(void) override; //!< Sets specific registers for the Arm activation    
    bool idleCallback(void) override;
    void automaticPositioningCompletedCallback(MotorCompletedCodes error) override; //!< Override the basic class to handle the Virtual isocentric function    
    void automaticHomingCompletedCallback(MotorCompletedCodes error) override;
    void manualPositioningCompletedCallback(MotorCompletedCodes error) override;
    
    MotorCompletedCodes manualPositioningRunningCallback(void) override;

private:
    static bool manual_activation_enabled = false; //!< This is the flag activating the body manual activation
    static bool manual_up_direction = false; //!< Sets true if the UP manual command is executing, false if the DOWN manual activation is executing

};

