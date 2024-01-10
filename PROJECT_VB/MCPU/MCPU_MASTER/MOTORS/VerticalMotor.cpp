#include "CalibrationConfig.h"
#include "Notify.h"
#include "VerticalMotor.h"
#include "pd4_od.h"
#include "gantry_global_status.h"
#include "PCB301.h"


#define ROT_PER_MM ((double) 1/ (double) 1) //!< Defines the position user units in 1 mm

#define MAX_POSITION 6500
#define MIN_POSITION -6500
#define HOMING_ON_METHOD 22
#define HOMING_OFF_METHOD 21

bool VerticalMotor::initializeSpecificObjectDictionaryCallback(void) {

    // Polarity inversion to have positive activation to ward Up
    //while (!blocking_writeOD(OD_607E_00, 0x80));	// b7:1-> inverse rotation

    while (!blocking_writeOD(OD_3202_00, 0x41)) ; 	// Motor Drive Submode Select: 6:BLDC 3:CurRed 2:Brake 1:VoS 0: 1=CLOSED_LOOP/O = OPEN_LOOP

    while (!blocking_writeOD(OD_2031_00, 15000)) ;  // Peak current
    while (!blocking_writeOD(OD_2032_00, 5000)) ;  // Maximum Speed
    
    // I2t Parameters
    while (!blocking_writeOD(OD_203B_01, 50000)) ;	// Nominal Current
    while (!blocking_writeOD(OD_203B_02, 2000)) ;	// Maximum Duration Of Peak Current
    while (!blocking_writeOD(OD_203B_03, 0)) ;	    // Threshold
    while (!blocking_writeOD(OD_203B_04, 0)) ;	    // CalcValue
    while (!blocking_writeOD(OD_203B_05, 15000)); // LimitedCurrent
    while (!blocking_writeOD(OD_2056_00, 500)) ;	// Limit Switch Tolerance Band

    // Motor Drive Parameter Set
    while (!blocking_writeOD(OD_3210_01, 10000)); // 50000 Position Loop, Proportional Gain (closed Loop)
    while (!blocking_writeOD(OD_3210_02, 5));	 // 10  Position Loop, Integral Gain (closed Loop)

    // Max Absolute Acceleration and Deceleration
    while (!blocking_writeOD(OD_60C5_00, 5000)) ;  // Max Acceleration
    while (!blocking_writeOD(OD_60C6_00, 5000)) ;  // Max Deceleration


    // Software Position Limit
    while (!blocking_writeOD(OD_607D_01, convert_User_To_Encoder(MIN_POSITION))) ;	// Min Position Limit
    while (!blocking_writeOD(OD_607D_02, convert_User_To_Encoder(MAX_POSITION))) ;	// Max Position Limit

    // Set the input setting
    while (!blocking_writeOD(OD_3240_01, 0x4)) ; // Input control special: I3 = HOMING
    while (!blocking_writeOD(OD_3240_02, 0)) ;   // Function Inverted: not inverted
    while (!blocking_writeOD(OD_3240_03, 0)) ;   // Force Enable = false
    while (!blocking_writeOD(OD_3240_06, 0)) ;   // Input Range Select: threshold = 5V;

    return true;
}

VerticalMotor::VerticalMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::VERTICAL_ID, L"MOTOR_VERTICAL", ROT_PER_MM, true)
{
    // Sets +/- 5mm as the acceptable target range
    setTargetRange(5, 5);

    // Gets the initial position of the encoder. If the position is a valid position the oming is not necessary
    bool homing_initialized = false;
    int  init_position = 0;
    if (MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_CURRENT_POSITION] != MotorConfig::MOTOR_UNDEFINED_POSITION) {
        homing_initialized = true;
        init_position = System::Convert::ToInt32(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_CURRENT_POSITION]);
    }

    setEncoderInitStatus(homing_initialized);
    setEncoderInitialUvalue(init_position);

    // Activate a warning condition is the motor should'n be initialized
    if (!isEncoderInitialized()) Notify::activate(Notify::messages::ERROR_VERTICAL_MOTOR_HOMING, false);
    
}

bool VerticalMotor::activateIsocentricCorrection(int id, int delta_h)
{
    // Set the taret position
    int target = VerticalMotor::device->getCurrentPosition() + delta_h;

    // Activate the automatic positioning for isocentric correction
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_AUTO_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_AUTO_ACC]);
    int dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_AUTO_DEC]);
    iso_activation_mode = true;
    return device->activateAutomaticPositioning(id, target, speed, acc, dec);    
}

/// <summary>
/// The VerticalMotor class override this function in order to 
/// handle the Isocentric automatic activation code
/// 
/// </summary>
/// <param name=LABEL_ERROR></param>
void VerticalMotor::automaticPositioningCompletedCallback(MotorCompletedCodes error) {
    
    // Sets the current Vertical position
    if (isEncoderInitialized()) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_VERTICAL, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
    }

    // if is an isocentric activation needs to notify the AWS
    if (iso_activation_mode) {
        // Alwais answer with success for isocentric correction
        // because a fail in the isocentric correction shall not prevent to proceed with the exposure.
        device->command_completed_event(getCommandId(), (int)MotorCompletedCodes::COMMAND_SUCCESS);
    }
    return;
}



/// <summary>
/// The VerticalMotor class override this function in order to  handle the IDLE activities
/// 
/// </summary>
/// 
/// # Safety Management
/// 
/// # Manual Activation Feature    
/// 
/// The function polls the status of the hardware inputs of the PCB301 board 
/// in order to detect a request for a manual activation.
/// 
/// If the request should change state from No Activation 
/// to Up or Down activation code, the manual activation procedure is started.
/// 
/// This procedure shall be enabled in order to be executed. (see setManualEnable() )
/// 
/// <param name=""></param>
/// <returns></returns>
CanOpenMotor::MotorCompletedCodes VerticalMotor::idleCallback(void) {
    static bool manaul_activation_request = false;
    int speed, acc, dec;

    // Handle the Safety condition 
    
    // Handle a Manual activation mode
    if (!manaul_activation_request) {
        if ((Gantry::getVerticalManualActivationIncrease()) || (Gantry::getVerticalManualActivationDecrease())) {
            manaul_activation_request = true;

            if (manual_activation_enabled) {
                speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_MANUAL_SPEED]);
                acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_MANUAL_ACC]);
                dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_MANUAL_DEC]);

                if (Gantry::getVerticalManualActivationIncrease()) {
                    manual_increment_direction = true;
                    device->activateManualPositioning(MAX_POSITION, speed, acc, dec);
                }
                else {
                    manual_increment_direction = false;
                    device->activateManualPositioning(MIN_POSITION, speed, acc, dec);
                }
            } // Enabled command
        }

    }
    else {
        if ((!Gantry::getBodyManualActivationIncrease()) && (!Gantry::getBodyManualActivationDecrease())) {
            manaul_activation_request = false;
        }
    }




    // Allows to proceed with commands
    return MotorCompletedCodes::COMMAND_PROCEED;

}

/// <summary>
/// This function activates the Automatic Homing procedure
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool VerticalMotor::startHoming(void) {

    // Gets the Speed and Acceleration from the configuration file
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_HOME_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_HOME_ACC]);
    return device->activateAutomaticHoming(HOMING_ON_METHOD, HOMING_OFF_METHOD, speed, acc);
}


/// <summary>
/// The VerticalMotor class override this function in order to 
/// handle the homing completion process.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
void VerticalMotor::automaticHomingCompletedCallback(MotorCompletedCodes error) {
    
    if (isEncoderInitialized()) {
        // Set the position in the configuration file and clear the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_VERTICAL, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
        Notify::deactivate(Notify::messages::ERROR_VERTICAL_MOTOR_HOMING);
    }
    else {
        // Reset the position in the configuration file and reactivate the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_VERTICAL, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
        MotorConfig::Configuration->storeFile();
        Notify::activate(Notify::messages::ERROR_VERTICAL_MOTOR_HOMING, false);
    }
}

/// <summary>
/// The VerticalMotor class override this function in order to 
/// handle the manual activation process.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VerticalMotor::MotorCompletedCodes  VerticalMotor::manualPositioningRunningCallback(void) {
    
    // Handles the enable condition
    if (!manual_activation_enabled) {
        return MotorCompletedCodes::ERROR_COMMAND_DISABLED;
    }

    // Handle the limit switches

    // Handle the safety

    // handle the manual hardware inputs
    if ((!Gantry::getVerticalManualActivationIncrease()) && (!Gantry::getVerticalManualActivationDecrease())) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    if ((!Gantry::getVerticalManualActivationIncrease()) && (manual_increment_direction)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    if ((!Gantry::getVerticalManualActivationDecrease()) && (!manual_increment_direction)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;

    // Proceeds with the manual activation
    return MotorCompletedCodes::COMMAND_PROCEED;
}
