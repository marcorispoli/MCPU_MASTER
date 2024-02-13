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


ArmMotor::ArmMotor(void) :CANOPEN::CanOpenMotor((unsigned char) CANOPEN::MotorDeviceAddresses::ARM_ID, L"MOTOR_ARM", ROT_PER_CDEGREE, true)
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
    if (!isEncoderInitialized()) Notify::activate(Notify::messages::ERROR_ARM_MOTOR_HOMING);


}

void ArmMotor::resetCallback(void) {

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

    // Activates the configuration of the device
    activateConfiguration();
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
/// The module overrides this function in order to handle the automatic activation process.
/// </summary>
/// 
/// During the automatic activation, the module checks:
/// 
/// - the system enabled abort input: the current enabled abort inputs activation shall quickly stop the activation; 
/// 
/// <param name=""></param>
/// <returns></returns>
ArmMotor::MotorCompletedCodes  ArmMotor::automaticPositioningRunningCallback(void) {

    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::ARM_ID)) {
        return MotorCompletedCodes::ERROR_SAFETY;
    }

    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getObstacleRotationStatus((int)CANOPEN::MotorDeviceAddresses::ARM_ID)) {
        return MotorCompletedCodes::ERROR_OBSTACLE_DETECTED;
    }

    // Proceeds with the manual activation
    return MotorCompletedCodes::COMMAND_PROCEED;
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



/// <summary>
/// The ArmMotor class override this function in order to 
/// handle the IDLE activities
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
ArmMotor::MotorCompletedCodes ArmMotor::idleCallback(void) {
    int speed, acc, dec;
    MotorCompletedCodes ret_code = MotorCompletedCodes::COMMAND_PROCEED;

    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::ARM_ID)) {
        ret_code = MotorCompletedCodes::ERROR_SAFETY; // Priority over the limit switch
    }

    // Handle a Manual activation mode
    bool man_increase = Gantry::getManualRotationIncrease((int)CANOPEN::MotorDeviceAddresses::ARM_ID);
    bool man_decrease = Gantry::getManualRotationDecrease((int)CANOPEN::MotorDeviceAddresses::ARM_ID);
    if (man_increase || man_decrease) {
        if (!manual_activation_enabled) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_MANUAL_DISABLE);
        else if (ret_code == MotorCompletedCodes::ERROR_SAFETY) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_SAFETY_DISABLE);
        else if (ret_code != MotorCompletedCodes::COMMAND_PROCEED) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
        else {
            speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_MANUAL_SPEED]);
            acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_MANUAL_ACC]);
            dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_MANUAL_DEC]);

            if (man_increase) {
                manual_increment_direction = true;
                if (!device->activateManualPositioning(MAX_ROTATION_ANGLE, speed, acc, dec)) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
            }
            else {
                manual_increment_direction = false;
                if (!device->activateManualPositioning(MIN_ROTATION_ANGLE, speed, acc, dec)) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
            }
        }
    }

    return ret_code;
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

    // Activate the command
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_AUTO_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_AUTO_ACC]);
    int dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_AUTO_DEC]);

    // Assignes the projection
    if (device->activateAutomaticPositioning(id, pos * 100, speed, acc, dec,true)) {
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

    // Activate the command
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_AUTO_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_AUTO_ACC]);
    int dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_AUTO_DEC]);
    valid_target = false;
    return device->activateAutomaticPositioning(0, pos * 100, speed, acc, dec, true);

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
        Notify::activate(Notify::messages::ERROR_ARM_MOTOR_HOMING);
    }

    // Notify the command termination event
    device->command_completed_event((int)0, (int)error);
}




/// <summary>
/// The ArmMotor class override this function in order to handle the manual activation process.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
ArmMotor::MotorCompletedCodes  ArmMotor::manualPositioningRunningCallback(void) {

    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::ARM_ID)) {
        return MotorCompletedCodes::ERROR_SAFETY;
    }

    
    // handle the manual hardware inputs
    bool man_increase = Gantry::getManualRotationIncrease((int)CANOPEN::MotorDeviceAddresses::ARM_ID);
    bool man_decrease = Gantry::getManualRotationDecrease((int)CANOPEN::MotorDeviceAddresses::ARM_ID);
    if ((!man_increase) && (!man_decrease)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    if ((!man_increase) && (manual_increment_direction)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    if ((!man_decrease) && (!manual_increment_direction)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;

    // Proceeds with the manual activation
    return MotorCompletedCodes::COMMAND_PROCEED;
}

void ArmMotor::manualPositioningCompletedCallback(MotorCompletedCodes error) {

    // Sets the current position
    if (isEncoderInitialized()) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_ARM, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
    }
    return;

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

/// <summary>
/// This function is called at the beginning of the automatic activation
/// </summary>
/// 
/// The function invalidate the current encoder position in the case, 
/// during the activation, the software should be killed before to update the current encoder position.
/// 
/// <param name=""></param>
/// <returns></returns>
ArmMotor::MotorCompletedCodes ArmMotor::automaticPositioningPreparationCallback(void) {

    // Invalidate the position: if the command should completes the encoder position will lbe refresh 
    // with the current valid position
    MotorConfig::Configuration->setParam(MotorConfig::PARAM_ARM, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
    MotorConfig::Configuration->storeFile();
    return MotorCompletedCodes::COMMAND_PROCEED;
}

/// <summary>
/// This function is called at the beginning of the automatic activation
/// </summary>
/// 
/// See the automaticPositioningPreparationCallback()
/// 
/// <param name=""></param>
/// <returns></returns>
ArmMotor::MotorCompletedCodes ArmMotor::manualPositioningPreparationCallback(void) {
    return automaticPositioningPreparationCallback();
}