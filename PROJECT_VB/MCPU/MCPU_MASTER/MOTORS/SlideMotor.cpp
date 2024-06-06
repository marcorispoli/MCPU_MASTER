#include "SlideMotor.h"
#include "Notify.h"
#include "pd4_od.h"
#include "VerticalMotor.h"
#include "ArmMotor.h"
#include "TiltMotor.h"
#include "math.h"

// User Units are in 0.01°
#define GEAR_RATIO ((double) 1 / (double) 29.1) // 1 turn == 0.291° 

#define MAX_ROTATION_ANGLE 8900
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
#define SLIDE_OD_CODE 0x0002
unsigned short SlideMotor::initializeSpecificObjectDictionaryCallback(void) {

    
    // Motor Drive Parameter Set
    //while (!blocking_writeOD(OD_3210_01, 10000)); 
    //while (!blocking_writeOD(OD_3210_02, 10));	 

    // PI-POSITION
    while (!blocking_writeOD(OD_3210_01, 10000)); // 2048
    while (!blocking_writeOD(OD_3210_02, 1));   // 0

    // PI-CURRENT
//    while (!blocking_writeOD(OD_3210_05, 420000)); //420000
//    while (!blocking_writeOD(OD_3210_06, 48000));      //12000

    return SLIDE_OD_CODE;
}

SlideMotor::SlideMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::SLIDE_ID, L"MOTOR_SLIDE", MotorConfig::PARAM_SLIDE, Notify::messages::ERROR_SLIDE_MOTOR_HOMING, MIN_ROTATION_ANGLE, MAX_ROTATION_ANGLE, GEAR_RATIO, 1, false)
{
    // Sets +/- 0.1 ° as the acceptable target range
    setTargetRange(20, 20);
    idle_positioning = false;

}

bool SlideMotor::serviceAutoPosition(int pos) {
    if ((!TiltMotor::isScoutPosition())) {
        LogClass::logInFile("SlideMotor::serviceAutoPosition() - command: error, tilt not in scout ");
        return false;

    }

    device->iso_activation_mode = false;
    idle_positioning = false;

    return device->activateAutomaticPositioning(0, pos * 100,  true);

}

bool SlideMotor::setIdlePosition(void) {
    if ((!TiltMotor::isScoutPosition())) {
        LogClass::logInFile("SlideMotor::serviceAutoPosition() - command: error, tilt not in scout ");
        return false;

    }

    device->iso_activation_mode = false;
    idle_positioning = true;

    return device->activateAutomaticPositioning(0, 0, true);

}


bool SlideMotor::isoAutoPosition(int pos) {
    if ((!TiltMotor::isScoutPosition())) {
        LogClass::logInFile("SlideMotor::isoAutoPosition() - command: error, tilt not in scout ");
        return false;

    }

    device->iso_activation_mode = true;
    idle_positioning = false;

    return device->activateAutomaticPositioning(0, pos, true);

}


void SlideMotor::completedCallback(int id, MotorCommands current_command, int current_position, MotorCompletedCodes term_code) {
    if (current_command != MotorCommands::MOTOR_AUTO_POSITIONING) return;

    // Next step of the Idle positioning
    if (idle_positioning) {
        idle_positioning = false;
        if (term_code == MotorCompletedCodes::COMMAND_SUCCESS)  ArmMotor::setIdlePosition();
        return;
    }

    // If Slide activation is not a Isocentric mode (or is terminated in error) then the command termines here
    // end the command_completed event is generated 
    if ((!iso_activation_mode) || (term_code != MotorCompletedCodes::COMMAND_SUCCESS)) {
        command_completed_event(id, (int) term_code);
        return;
    }

    // Activates the Vertical Motor for the isocentric correction
    // The Slide position is expressed in cents of degrees;
    // The Vertical Arm position is espressed in millimeters
    double L = 412; // millimeters from the rotation center
    double A = 30 * 3.14159 / 180; // Alfa of the potter at the 0 slide degree

    init_angolo = A + ((double)getPreviousPosition() * 3.14159 / (double)18000);
    last_angolo = A + ((double)getCurrentUposition() * 3.14159 / (double)18000);
    H0 = L * cos(init_angolo);
    H1 = L * cos(last_angolo);

    if (!VerticalMotor::activateIsocentricCorrection(id, (H1 - H0))) {
        // The target is not invalidated because the rotation angle is still valid!
        device->command_completed_event(id, (int)MotorCompletedCodes::COMMAND_SUCCESS);
    }

    return;

}


/// <summary>
/// This function activates the Automatic Homing procedure
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool SlideMotor::startAutoHoming(void) {

    // Gets the Speed and Acceleration from the configuration file
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_HOME_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_HOME_ACC]);
    return device->activateAutomaticHoming(HOMING_ON_METHOD, HOMING_OFF_METHOD, speed, acc);
}


bool SlideMotor::startManualHoming(int target_position) {
    if (device->isPositionFromExternalSensor()) return device->activateExternalHoming(target_position);
    else return device->activateManualHoming(target_position);
}
