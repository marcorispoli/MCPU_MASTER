#include "CalibrationConfig.h"
#include "Notify.h"
#include "VerticalMotor.h"
#include "pd4_od.h"
#include "PCB301.h"


#define ROT_PER_MM ((double) 1/ (double) 1) //!< Defines the position user units in 1 mm

#define MAX_POSITION 6500
#define MIN_POSITION -6500
#define HOMING_ON_METHOD 22
#define HOMING_OFF_METHOD 21

bool VerticalMotor::initializeSpecificObjectDictionaryCallback(void) {

    
    if (!blocking_writeOD(OD_3202_00, 0x41)) return false; 	// Motor Drive Submode Select: 6:BLDC 3:CurRed 2:Brake 1:VoS 0: 1=CLOSED_LOOP/O = OPEN_LOOP

    if (!blocking_writeOD(OD_2031_00, 5000)) return false;  // Peak current
    if (!blocking_writeOD(OD_2032_00, 5000)) return false;  // Maximum Speed
    
    // I2t Parameters
    if (!blocking_writeOD(OD_203B_01, 50000)) return false;	// Nominal Current
    if (!blocking_writeOD(OD_203B_02, 1000)) return false;	// Maximum Duration Of Peak Current
    if (!blocking_writeOD(OD_203B_03, 0)) return false;	    // Threshold
    if (!blocking_writeOD(OD_203B_04, 0)) return false;	    // CalcValue
    if (!blocking_writeOD(OD_203B_05, 5000)) return false; // LimitedCurrent
    if (!blocking_writeOD(OD_2056_00, 500)) return false;	// Limit Switch Tolerance Band

    // Max Absolute Acceleration and Deceleration
    if (!blocking_writeOD(OD_60C5_00, 5000)) return false;  // Max Acceleration
    if (!blocking_writeOD(OD_60C6_00, 5000)) return false;  // Max Deceleration


    // Software Position Limit
    if (!blocking_writeOD(OD_607D_01, convert_User_To_Encoder(MIN_POSITION))) return false;	// Min Position Limit
    if (!blocking_writeOD(OD_607D_02, convert_User_To_Encoder(MAX_POSITION))) return false;	// Max Position Limit

    // Set the input setting
    if (!blocking_writeOD(OD_3240_01, 0x4)) return false; // Input control special: I3 = HOMING
    if (!blocking_writeOD(OD_3240_02, 0)) return false;   // Function Inverted: not inverted
    if (!blocking_writeOD(OD_3240_03, 0)) return false;   // Force Enable = false
    if (!blocking_writeOD(OD_3240_06, 0)) return false;   // Input Range Select: threshold = 5V;

    return true;
}

VerticalMotor::VerticalMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::VERTICAL_ID, L"MOTOR_VERTICAL", ROT_PER_MM)
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

    this->home_initialized = homing_initialized;
    this->encoder_initial_value = init_position;

    // Activate a warning condition is the motor should'n be initialized
    if (!home_initialized) Notify::activate(Notify::messages::ERROR_VERTICAL_MOTOR_HOMING, false);
    
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
    if (device->home_initialized) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_VERTICAL, MotorConfig::PARAM_CURRENT_POSITION, device->current_eposition.ToString());
        MotorConfig::Configuration->storeFile();
    }

    // if is an isocentric activation needs to notify the AWS
    if(iso_activation_mode) device->command_completed_event(command_id, (int) error);
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
    static PCB301::vertical_activation_options vertical_request = PCB301::vertical_activation_options::VERTICAL_NO_ACTIVATION;

    int speed, acc, dec;

    // Handle the Safety condition 
    
    // Handle a Manual activation mode
    if (PCB301::getVerticalActivationStatus() != vertical_request) {
        vertical_request = PCB301::getVerticalActivationStatus();
        if (manual_activation_enabled) {
            speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_MANUAL_SPEED]);
            acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_MANUAL_ACC]);
            dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_MANUAL_DEC]);

            switch (vertical_request) {
            case PCB301::vertical_activation_options::VERTICAL_UP_ACTIVATION:
                device->activateManualPositioning(MAX_POSITION, speed, acc, dec);
                manual_up_direction = true;
                break;
            case PCB301::vertical_activation_options::VERTICAL_DOWN_ACTIVATION:
                device->activateManualPositioning(MIN_POSITION, speed, acc, dec);
                manual_up_direction = false;
                break;

            }
        } // Enabled command
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
    
    if (device->home_initialized) {
        // Set the position in the configuration file and clear the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_VERTICAL, MotorConfig::PARAM_CURRENT_POSITION, device->current_eposition.ToString());
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
    PCB301::vertical_activation_options vertical_request = PCB301::getVerticalActivationStatus();
    if (vertical_request == PCB301::vertical_activation_options::VERTICAL_NO_ACTIVATION) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    else if( (manual_up_direction) && (vertical_request == PCB301::vertical_activation_options::VERTICAL_DOWN_ACTIVATION)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    else if ((!manual_up_direction) && (vertical_request == PCB301::vertical_activation_options::VERTICAL_UP_ACTIVATION)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;

    // Proceeds with the manual activation
    return MotorCompletedCodes::COMMAND_PROCEED;
}
