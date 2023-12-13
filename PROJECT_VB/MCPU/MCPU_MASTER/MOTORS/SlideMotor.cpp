#include "SlideMotor.h"
#include "Notify.h"

#define GEAR_RATIO (double) 352.2

#define MAX_ROTATION_ANGLE 900
#define MIN_ROTATION_ANGLE 0
#define HOMING_ON_METHOD 19
#define HOMING_OFF_METHOD 20

/// <summary>
/// The SlideMotor override this function in order to initialize specific motor registers
/// 
/// </summary>
/// 
/// 
/// <param name=""></param>
/// <returns>true if the initialization termines successfully</returns>

bool SlideMotor::initializeSpecificObjectDictionaryCallback(void) {


    return true;
}

SlideMotor::SlideMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::SLIDE_ID, L"MOTOR_SLIDE", GEAR_RATIO)
{
    
    // Gets the initial position of the encoder. If the position is a valid position the oming is not necessary
    bool homing_initialized = false;
    int  init_position = 0;

    if (MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_CURRENT_POSITION] != MotorConfig::MOTOR_UNDEFINED_POSITION) {
        homing_initialized = true;
        init_position = System::Convert::ToInt32(MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_CURRENT_POSITION]);
    }

    this->home_initialized = homing_initialized;
    this->encoder_initial_value = init_position;

    // Activate a warning condition is the motor should'n be initialized
    if (!home_initialized) Notify::activate(Notify::messages::ERROR_SLIDE_MOTOR_HOMING, false);

}


/// <summary>
/// The SlideMotor class override this function in order to 
/// handle the Isocentric automatic activation code
/// 
/// </summary>
/// <param name=LABEL_ERROR></param>
void SlideMotor::automaticPositioningCompletedCallback(MotorCompletedCodes error) {

    // Sets the current Vertical position
    if (home_initialized) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_SLIDE, MotorConfig::PARAM_CURRENT_POSITION, device->current_eposition.ToString());
        MotorConfig::Configuration->storeFile();
    }

    // If requested, rise the command completed event
    device->command_completed_event(command_id, (int) error);

    return;
}



/// <summary>
/// The SlideMotor class override this function in order to 
/// handle the IDLE activities
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool SlideMotor::idleCallback(void) {


    return true;

}



/// <summary>
/// This function activates the Automatic Homing procedure
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool SlideMotor::startHoming(void) {

    // Gets the Speed and Acceleration from the configuration file
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_HOME_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_HOME_ACC]);
    return device->activateAutomaticHoming(HOMING_ON_METHOD, HOMING_OFF_METHOD, speed, acc);
}




/// <summary>
/// The SlideMotor class override this function in order to 
/// handle the homing completion process.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
void SlideMotor::automaticHomingCompletedCallback(MotorCompletedCodes error) {

    if (device->home_initialized) {
        // Set the position in the configuration file and clear the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_SLIDE, MotorConfig::PARAM_CURRENT_POSITION, device->current_eposition.ToString());
        MotorConfig::Configuration->storeFile();
        Notify::deactivate(Notify::messages::ERROR_SLIDE_MOTOR_HOMING);
    }
    else {
        // Reset the position in the configuration file and reactivate the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_SLIDE, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
        MotorConfig::Configuration->storeFile();
        Notify::activate(Notify::messages::ERROR_SLIDE_MOTOR_HOMING, false);
    }

    // Notify the command termination event
    device->command_completed_event((int)0, (int)error);
}

void SlideMotor::manualPositioningCompletedCallback(MotorCompletedCodes error) {



    // Notify the command termination event
    device->command_completed_event((int)0, (int)error);

    return;
}

/// <summary>
/// The ArmMotor class override this function in order to handle the manual activation process.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
SlideMotor::MotorCompletedCodes  SlideMotor::manualPositioningRunningCallback(void) {

    /*
    // Handles the enable condition
    if (!manual_activation_enabled) {
        return MotorCompletedCodes::ERROR_COMMAND_DISABLED;
    }

    // Handle the limit switches

    // Handle the safety

    // handle the manual hardware inputs
    PCB301::body_activation_options body_request = PCB301::getBodyActivationStatus();
    if (body_request == PCB301::body_activation_options::BODY_NO_ACTIVATION) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    else if ((manual_cw_direction) && (body_request == PCB301::body_activation_options::BODY_CCW_ACTIVATION)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    else if ((!manual_cw_direction) && (body_request == PCB301::body_activation_options::BODY_CW_ACTIVATION)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;

    */
    // Proceeds with the manual activation
    return MotorCompletedCodes::COMMAND_PROCEED;
}