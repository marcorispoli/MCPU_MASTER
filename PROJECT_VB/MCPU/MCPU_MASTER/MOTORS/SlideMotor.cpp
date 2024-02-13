#include "SlideMotor.h"
#include "Notify.h"
#include "pd4_od.h"
#include "VerticalMotor.h"
#include "TiltMotor.h"
#include "math.h"

// User Units are in 0.01°
#define GEAR_RATIO ((double) 1 / (double) 29.1) // 1 turn == 0.291° 

#define MAX_ROTATION_ANGLE 9000
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

    // Motor Drive Parameter Set
    while (!blocking_writeOD(OD_3210_01, 10000)); // 50000 Position Loop, Proportional Gain (closed Loop)
    while (!blocking_writeOD(OD_3210_02, 10));	 // 10  Position Loop, Integral Gain (closed Loop)


    // Position Range Limit
    while (!blocking_writeOD(OD_607B_01, convert_User_To_Encoder(MIN_ROTATION_ANGLE - 200))); 	// Min Position Range Limit
    while (!blocking_writeOD(OD_607B_02, convert_User_To_Encoder(MAX_ROTATION_ANGLE + 200)));	// Max Position Range Limit

    // Software Position Limit
    if (!blocking_writeOD(OD_607D_01, convert_User_To_Encoder(MIN_ROTATION_ANGLE))) return false;	// Min Position Limit
    if (!blocking_writeOD(OD_607D_02, convert_User_To_Encoder(MAX_ROTATION_ANGLE))) return false;	// Max Position Limit


    return true;
}

SlideMotor::SlideMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::SLIDE_ID, L"MOTOR_SLIDE", GEAR_RATIO, false)
{
    
    // Gets the initial position of the encoder. If the position is a valid position the oming is not necessary
    bool homing_initialized = false;
    int  init_position = 0;

    if (MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_CURRENT_POSITION] != MotorConfig::MOTOR_UNDEFINED_POSITION) {
        homing_initialized = true;
        init_position = System::Convert::ToInt32(MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_CURRENT_POSITION]);
    }

    setEncoderInitStatus(homing_initialized);
    setEncoderInitialUvalue(init_position);

    // Activate a warning condition is the motor should'n be initialized
    if (!isEncoderInitialized()) Notify::activate(Notify::messages::ERROR_SLIDE_MOTOR_HOMING);

}

bool SlideMotor::serviceAutoPosition(int pos) {
    if ((!TiltMotor::isScoutPosition())) {
        LogClass::logInFile("SlideMotor::serviceAutoPosition() - command: error, tilt not in scout ");
        return false;

    }

    device->iso_activation_mode = false;

    // Activate the command
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_AUTO_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_AUTO_ACC]);
    int dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_AUTO_DEC]);

    return device->activateAutomaticPositioning(0, pos * 100, speed, acc, dec, true);

}

bool SlideMotor::isoAutoPosition(int pos) {
    if ((!TiltMotor::isScoutPosition())) {
        LogClass::logInFile("SlideMotor::isoAutoPosition() - command: error, tilt not in scout ");
        return false;

    }

    device->iso_activation_mode = true;

    // Activate the command
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_AUTO_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_AUTO_ACC]);
    int dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_AUTO_DEC]);

    return device->activateAutomaticPositioning(0, pos, speed, acc, dec, true);

}


void SlideMotor::resetCallback(void) {

    // Gets the initial position of the encoder. If the position is a valid position the oming is not necessary
    bool homing_initialized = false;
    int  init_position = 0;

    if (MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_CURRENT_POSITION] != MotorConfig::MOTOR_UNDEFINED_POSITION) {
        homing_initialized = true;
        init_position = System::Convert::ToInt32(MotorConfig::Configuration->getParam(MotorConfig::PARAM_SLIDE)[MotorConfig::PARAM_CURRENT_POSITION]);
    }

    setEncoderInitStatus(homing_initialized);
    setEncoderInitialUvalue(init_position);

    // Activate a warning condition is the motor should'n be initialized
    if (!isEncoderInitialized()) Notify::activate(Notify::messages::ERROR_SLIDE_MOTOR_HOMING);

    // Activates the configuration of the device
    activateConfiguration();
}


/// <summary>
/// The SlideMotor class override this function in order to 
/// handle the Isocentric automatic activation code
/// 
/// </summary>
/// <param name=LABEL_ERROR></param>
void SlideMotor::automaticPositioningCompletedCallback(MotorCompletedCodes error) {

    // Sets the current Vertical position
    if (isEncoderInitialized()) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_SLIDE, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
    }


    // If Slide activation is not a Isocentric mode (or is terminated in error) then the command termines here
    // end the command_completed event is generated 
    if ((!iso_activation_mode) || (error != MotorCompletedCodes::COMMAND_SUCCESS)) {
        command_completed_event(getCommandId(), (int)error);
        return;
    }

    // Activates the Vertical Motor for the isocentric correction
    // The Slide position is expressed in cents of degrees;
    // The Vertical Arm position is espressed in millimeters
    double L = 412; // millimeters from the rotation center
    double A = 30 * 3.14159  / 180; // Alfa of the potter at the 0 slide degree

    init_angolo = A + ( (double)getPreviousPosition() * 3.14159 / (double)18000);
    last_angolo = A + ((double)getCurrentEncoderUposition() * 3.14159 / (double)18000);
    H0 = L * cos(init_angolo);
    H1 = L * cos(last_angolo);
    
    if (!VerticalMotor::activateIsocentricCorrection(getCommandId(), (H1-H0))) {
        // The target is not invalidated because the rotation angle is still valid!
        device->command_completed_event(getCommandId(), (int)MotorCompletedCodes::COMMAND_SUCCESS);
        //command_completed_event(getCommandId(), (int) VerticalMotor::device->getCommandCompletedCode());
    }

    return;
}



/// <summary>
/// The SlideMotor class override this function in order to 
/// handle the IDLE activities
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
SlideMotor::MotorCompletedCodes SlideMotor::idleCallback(void) {


    return MotorCompletedCodes::COMMAND_PROCEED;

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

    if (isEncoderInitialized()) {
        // Set the position in the configuration file and clear the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_SLIDE, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
        Notify::deactivate(Notify::messages::ERROR_SLIDE_MOTOR_HOMING);
    }
    else {
        // Reset the position in the configuration file and reactivate the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_SLIDE, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
        MotorConfig::Configuration->storeFile();
        Notify::activate(Notify::messages::ERROR_SLIDE_MOTOR_HOMING);
    }

    // Notify the command termination event
    device->command_completed_event((int)0, (int)error);
}

void SlideMotor::manualPositioningCompletedCallback(MotorCompletedCodes error) {



    // Notify the command termination event
    device->command_completed_event((int)0, (int)error);

    return;
}

SlideMotor::MotorCompletedCodes  SlideMotor::automaticPositioningRunningCallback(void) {
    return MotorCompletedCodes::COMMAND_PROCEED;
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


/// <summary>
/// This function is called at the beginning of the automatic activation
/// </summary>
/// 
/// The function invalidate the current encoder position in the case, 
/// during the activation, the software should be killed before to update the current encoder position.
/// 
/// <param name=""></param>
/// <returns></returns>
SlideMotor::MotorCompletedCodes SlideMotor::automaticPositioningPreparationCallback(void) {

    // Invalidate the position: if the command should completes the encoder position will lbe refresh 
    // with the current valid position
    MotorConfig::Configuration->setParam(MotorConfig::PARAM_SLIDE, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
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
SlideMotor::MotorCompletedCodes SlideMotor::manualPositioningPreparationCallback(void) {
    return automaticPositioningPreparationCallback();
}