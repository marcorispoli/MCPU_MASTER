#include "CalibrationConfig.h"
#include "ArmMotor.h"
#include "verticalMotor.h"
#include "Notify.h"
#include "../gantry_global_status.h"



#define ROT_PER_CDEGREE ((double) 112/ (double) 36000) //!< Defines the position user units in 0.01 degree
#define COMPRESSION_PLANE_MM ((double) 100) //!< Distance from the compression plane and the C-ARM rotation axe for the Isocentric calculation

#define MAX_ROTATION_ANGLE 18000
#define MIN_ROTATION_ANGLE -18000
#define HOMING_ON_METHOD 19
#define HOMING_OFF_METHOD 20


ArmMotor::ArmMotor(void) :CANOPEN::CanOpenMotor((unsigned char) CANOPEN::MotorDeviceAddresses::ARM_ID, L"MOTOR_ARM", ROT_PER_CDEGREE, false)
{
    // Sets +/- 0.2 ° as the acceptable target range
    setTargetRange(20, 20);

    // Gets the initial position of the encoder. If the position is a valid position the oming is not necessary
    bool homing_initialized = false;
    int  init_position = 0;
    if (MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_CURRENT_POSITION] != MotorConfig::MOTOR_UNDEFINED_POSITION) {
        homing_initialized = true;
        init_position = System::Convert::ToInt32(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_CURRENT_POSITION]);
    }

    setEncoderInitStatus(homing_initialized);
    setEncoderInitialUvalue(init_position);

    // Activate a warning condition is the motor should'n be initialized
    if (!isEncoderInitialized()) Notify::activate(Notify::messages::ERROR_ARM_MOTOR_HOMING, false);


}

/// <summary>
/// The ArmMotor override this function in order to initialize specific motor registers
/// 
/// </summary>
/// 
/// 
/// <param name=""></param>
/// <returns>true if the initialization termines successfully</returns>

bool ArmMotor::initializeSpecificObjectDictionaryCallback(void) {

    
    return true;
}

/// <summary>
/// The ArmMotor class override this function in order to 
/// handle the Isocentric automatic activation code
/// 
/// </summary>
/// <param name=LABEL_ERROR></param>
void ArmMotor::automaticPositioningCompletedCallback(MotorCompletedCodes error) {

    // Sets the current Vertical position
    if (isEncoderInitialized()) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_ARM, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
    }

    // If the C-ARM activation is not a Isocentric mode (or is terminated in error) then the command termines here
    // end the command_completed event is generated 
    if ((!iso_activation_mode) || (error != MotorCompletedCodes::COMMAND_SUCCESS)) {
        command_completed_event(getCommandId(), (int)error);
        return;
    }

    // Activates the Vertical Motor for the isocentric correction
    // The C-ARM position is expressed in cents of degrees;
    // The Vertical Arm position is espressed in millimeters

    double init_h = COMPRESSION_PLANE_MM * cos((double) getPreviousPosition() * 3.14159 / (double)18000);
    double end_h = COMPRESSION_PLANE_MM * cos((double)getCurrentEncoderUposition() * 3.14159 / (double)18000);
    int delta_h =  (int) init_h - (int) end_h;
    VerticalMotor::activateIsocentricCorrection(getCommandId(), delta_h);
    return;
}



/// <summary>
/// The ArmMotor class override this function in order to 
/// handle the IDLE activities
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
ArmMotor::MotorCompletedCodes ArmMotor::idleCallback(void) {

   
    return MotorCompletedCodes::COMMAND_PROCEED;

}

void ArmMotor::abortTarget(void) {
    if (!valid_target) return;
    valid_target = false;

    projections->clrProjection();
    target_abort_event();
}

bool ArmMotor::setTarget(int pos, int low, int high, System::String^ proj, int id) {

    // Checks the validity of the requested projection
    if (!projections->isValidProjection(proj) ) return false;

    // Assignes the projection
    projections->setProjection(proj);

    // Assignes the target data
    allowed_low = low;
    allowed_high = high;
    valid_target = true;
    selected_target = pos;

    target_change_event(id, pos); // For the Window Form update state

    // Activate an Isocentric C-ARM rotation
    device->iso_activation_mode = true;

    // Activate the command
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_AUTO_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_AUTO_ACC]);
    int dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_AUTO_DEC]);
    return device->activateAutomaticPositioning(id, pos, speed, acc, dec);

}

/// <summary>
/// This function activates the Automatic Homing procedure
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool ArmMotor::startHoming(void) {

    // Gets the Speed and Acceleration from the configuration file
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_HOME_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_HOME_ACC]);
    return device->activateAutomaticHoming(HOMING_ON_METHOD, HOMING_OFF_METHOD, speed, acc);
}




/// <summary>
/// The ArmMotor class override this function in order to 
/// handle the homing completion process.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
void ArmMotor::automaticHomingCompletedCallback(MotorCompletedCodes error) {
   
    if (isEncoderInitialized()) {
        // Set the position in the configuration file and clear the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_ARM, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
        Notify::deactivate(Notify::messages::ERROR_ARM_MOTOR_HOMING);
    }
    else {
        // Reset the position in the configuration file and reactivate the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_ARM, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
        MotorConfig::Configuration->storeFile();
        Notify::activate(Notify::messages::ERROR_ARM_MOTOR_HOMING, false);
    }

    // Notify the command termination event
    device->command_completed_event((int)0, (int)error);
}


void ArmMotor::manualPositioningCompletedCallback(MotorCompletedCodes error) {

  

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
ArmMotor::MotorCompletedCodes  ArmMotor::manualPositioningRunningCallback(void) {

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