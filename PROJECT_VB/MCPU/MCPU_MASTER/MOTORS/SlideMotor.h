#pragma once
#include "CanOpenMotor.h"
#include "CalibrationConfig.h"

ref class SlideMotor : public CANOPEN::CanOpenMotor
{
public:
	SlideMotor(void);
	static SlideMotor^ device = gcnew SlideMotor();        
    static bool startHoming(void);
    static bool serviceAutoPosition(int pos);
    static bool isoAutoPosition(int pos);
    static bool setIdlePosition(void);

    static bool isAllowedPosition(void) {
        if (!device->isEncoderInitialized()) return false;
        return ((device->getCurrentPosition() < 2000) || (device->getCurrentPosition() > 8000));
    }

protected:
    bool iso_activation_mode; //!< Setting this flag, causes the Vertical motor activation at the Arm rotation completion

    bool initializeSpecificObjectDictionaryCallback(void) override; //!< Sets specific registers for the Arm activation    
    void completedCallback(int id, MotorCommands current_command, int current_position, MotorCompletedCodes term_code) override;
    

private:
    static bool manual_up_direction = false; //!< Sets true if the UP manual command is executing, false if the DOWN manual activation is executing
    double init_angolo;
    double last_angolo;
    double H0;
    double H1;
    static bool idle_positioning = false;
};

