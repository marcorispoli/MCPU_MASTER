#include "CalibrationConfig.h"
#include "ArmMotor.h"
#include "VerticalMotor.h"
#include "TiltMotor.h"
#include "pd4_od.h"
#include "Notify.h"
#include "../gantry_global_status.h"



#define ROT_PER_CDEGREE ((double) 112/ (double) 36000) //!< Defines the position user units in 0.01 degree
#define COMPRESSION_PLANE_MM ((double) 100) //!< Distance from the compression plane and the C-ARM rotation axe for the Isocentric calculation

#define MAX_ROTATION_ANGLE 18000
#define MIN_ROTATION_ANGLE -18000
#define HOMING_ON_METHOD 19
#define HOMING_OFF_METHOD 20


ArmMotor::ArmMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::ARM_ID, L"MOTOR_ARM", MotorConfig::PARAM_ARM, Notify::messages::ERROR_ARM_MOTOR_HOMING, MIN_ROTATION_ANGLE, MAX_ROTATION_ANGLE, ROT_PER_CDEGREE, 1, true)
{
    // Sets +/- 0.2 ° as the acceptable target range
    setTargetRange(20, 20);
   
}


/// <summary>
/// The ArmMotor override this function in order to initialize specific motor registers
/// 
/// </summary>
/// 
/// 
/// <param name=""></param>
/// <returns>true if the initialization termines successfully</returns>
#define ARM_OD_CODE 0x0001
unsigned short ArmMotor::initializeSpecificObjectDictionaryCallback(void) {

    // Motor Drive Parameter Set
    while (!blocking_writeOD(OD_3210_01, 50000)); // Position Loop, Proportional Gain (closed Loop)
    while (!blocking_writeOD(OD_3210_02, 10));	 // Position Loop, Integral Gain (closed Loop)

  
    return ARM_OD_CODE;
}


void ArmMotor::completedCallback(int id, MotorCommands current_command, int current_position, MotorCompletedCodes term_code) {
    if (current_command != MotorCommands::MOTOR_AUTO_POSITIONING) return;

    // Invalidate the target if the command should fails
    if (term_code != MotorCompletedCodes::COMMAND_SUCCESS) valid_target = false;


    // If the C-ARM activation is not a Isocentric mode (or is terminated in error) then the command termines here
    // end the command_completed event is generated 
    if ((!iso_activation_mode) || (term_code != MotorCompletedCodes::COMMAND_SUCCESS)) {
        command_completed_event(id, (int) term_code);
        return;
    }

    // Activates the Vertical Motor for the isocentric correction
    // The C-ARM position is expressed in cents of degrees;
    // The Vertical Arm position is espressed in millimeters

    double init_h = -1 * COMPRESSION_PLANE_MM * cos((double)getPreviousPosition() * 3.14159 / (double)18000);
    double end_h = -1 * COMPRESSION_PLANE_MM * cos((double)getCurrentUposition() * 3.14159 / (double)18000);
    int delta_h = (int)init_h - (int)end_h;
    if (!VerticalMotor::activateIsocentricCorrection(id, delta_h)) {
        // The target is not invalidated because the rotation angle is still valid!
        device->command_completed_event(id, (int)MotorCompletedCodes::COMMAND_SUCCESS);
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
bool ArmMotor::startAutoHoming(void) {
    

    // Gets the Speed and Acceleration from the configuration file
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_HOME_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_HOME_ACC]);
    return device->activateAutomaticHoming(HOMING_ON_METHOD, HOMING_OFF_METHOD, speed, acc);
}

bool ArmMotor::startManualHoming(int target_position) {
    if (device->isPositionFromExternalSensor()) return device->activateExternalHoming(target_position);
    else return device->activateManualHoming(target_position);
}

void ArmMotor::faultCallback(bool errstat, bool data_changed, unsigned int error_class, unsigned int error_code) {
    

    if (errstat == false) {
        Notify::deactivate(Notify::messages::INFO_ARM_DRIVER);
        return;
    }


    if (data_changed) {
        //System::String^ driver_error = "CL:" + error_class.ToString() + " CD:" + error_code.ToString();
        System::String^ driver_error = CanOpenMotor::getErrorCode1003(error_code);
        Notify::activate(Notify::messages::INFO_ARM_DRIVER, driver_error);
    }

   

}

