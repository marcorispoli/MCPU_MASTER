#include "CalibrationConfig.h"
#include "ArmMotor.h"
#include "VerticalMotor.h"
#include "TiltMotor.h"
#include "Notify.h"
#include "../gantry_global_status.h"



#define ROT_PER_CDEGREE ((double) 112/ (double) 36000) //!< Defines the position user units in 0.01 degree
#define COMPRESSION_PLANE_MM ((double) 100) //!< Distance from the compression plane and the C-ARM rotation axe for the Isocentric calculation

#define MAX_ROTATION_ANGLE 18000
#define MIN_ROTATION_ANGLE -18000
#define HOMING_ON_METHOD 19
#define HOMING_OFF_METHOD 20


ArmMotor::ArmMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::ARM_ID, L"MOTOR_ARM", MotorConfig::PARAM_ARM, Notify::messages::ERROR_ARM_MOTOR_HOMING, ROT_PER_CDEGREE, true)
{
    // Sets +/- 0.2 � as the acceptable target range
    setTargetRange(20, 20);
    max_position = MAX_ROTATION_ANGLE;
    min_position = MIN_ROTATION_ANGLE;
    

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
    if (!isEncoderInitialized()) Notify::activate(Notify::messages::ERROR_ARM_MOTOR_HOMING);


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

    if (error != MotorCompletedCodes::COMMAND_SUCCESS) {

        // Invalidate the current target
        valid_target = false;
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

    double init_h = - 1 *COMPRESSION_PLANE_MM * cos((double) getPreviousPosition() * 3.14159 / (double)18000);
    double end_h = -1 * COMPRESSION_PLANE_MM * cos((double)getCurrentEncoderUposition() * 3.14159 / (double)18000);
    int delta_h =  (int) init_h - (int) end_h;
    if (!VerticalMotor::activateIsocentricCorrection(getCommandId(), delta_h)) {
        // The target is not invalidated because the rotation angle is still valid!
        device->command_completed_event(getCommandId(), (int)MotorCompletedCodes::COMMAND_SUCCESS);
        //command_completed_event(getCommandId(), (int) VerticalMotor::device->getCommandCompletedCode());
    }
    return;
}


void ArmMotor::abortTarget(void) {
    if (!valid_target) return;
    valid_target = false;
    projections->clrProjection();
    
}

bool ArmMotor::setTarget(int pos, int low, int high, System::String^ proj, int id) {
    
    if ((!TiltMotor::isScoutPosition())) {
        LogClass::logInFile("ArmMotor::setTarget() - command: error, tilt not in scout ");
        return false;

    }
    

    // Checks the validity of the requested projection
    if (!projections->isValidProjection(proj) ) return false;

    // Activate an Isocentric C-ARM rotation
    device->iso_activation_mode = true;

    // Assignes the projection
    if (device->activateAutomaticPositioning(id, pos * 100, true)) {
        projections->setProjection(proj);

        // Assignes the target data
        allowed_low = low * 100;
        allowed_high = high * 100;
        valid_target = true;
        selected_target = pos;
        return true;
    }    

    valid_target = false;
    return false;
}

bool ArmMotor::serviceAutoPosition(int pos) {
    
    if ((!TiltMotor::isScoutPosition())) {
        LogClass::logInFile("ArmMotor::serviceAutoPosition() - command: error, tilt not in scout ");
        return false;

    }
    device->iso_activation_mode = false;
    valid_target = false;
    return device->activateAutomaticPositioning(0, pos * 100, true);
}

bool ArmMotor::setIdlePosition(void) {
    if ((!TiltMotor::isScoutPosition())) {
        LogClass::logInFile("ArmMotor::serviceAutoPosition() - command: error, tilt not in scout ");
        return false;

    }
    device->iso_activation_mode = false;
    valid_target = false;
    return device->activateAutomaticPositioning(0,0, true);
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



void ArmMotor::faultCallback(bool errstat, bool data_changed, unsigned int error_class, unsigned int error_code) {
    if (errstat == false) {
        Notify::deactivate(Notify::messages::WARNING_ARM_DRIVER);
        return;
    }


    if (data_changed) {
        //System::String^ driver_error = "CL:" + error_class.ToString() + " CD:" + error_code.ToString();
        System::String^ driver_error = CanOpenMotor::getErrorCode1003(error_code);
        Notify::activate(Notify::messages::WARNING_ARM_DRIVER, driver_error);
    }

    // If an attempt to activate the manual rotation is generated in fault condition an istant window will appear
    bool man_increase = Gantry::getManualRotationIncrease((int)CANOPEN::MotorDeviceAddresses::ARM_ID);
    bool man_decrease = Gantry::getManualRotationDecrease((int)CANOPEN::MotorDeviceAddresses::ARM_ID);
    if (man_increase || man_decrease) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);

}

