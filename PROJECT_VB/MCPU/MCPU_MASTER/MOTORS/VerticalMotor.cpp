#include "CalibrationConfig.h"
#include "Notify.h"
#include "VerticalMotor.h"
#include "pd4_od.h"
#include "gantry_global_status.h"



#define ROT_PER_MM ((double) 1/ (double) 1) //!< Defines the position user units in 1 mm

#define LIMIT_INPUT_MASK(x) (x & PD4_MOTOR_DI1) //!< Sets the limit photocell switch input
#define ZERO_INPUT_MASK(x) (x & PD4_MOTOR_DI3) //!< Sets the Zero photocell switch input

#define MAX_POSITION 585    //!< Defines the Maximum software position respect the zero setting point
#define MIN_POSITION -355   //!< Defines the Minimum software position respect the zero setting point

#define HOMING_ON_METHOD 22 //!< Zero setting approaching method starting with the Zero photocell ON
#define HOMING_OFF_METHOD 21//!< Zero setting approaching method starting with the Zero photocell OFF

/// <summary>
/// This function test if the limit switch is activated
/// </summary>
/// 
/// The function reads the Input register before to test the related input.
/// 
/// The Limit switch activation status is determined when the photocell is not in light condition.
/// 
/// <param name=""></param>
/// <returns>true if the limit photocell is detected activated</returns>
bool VerticalMotor::testLimitSwitch(void) {
    if (!blocking_readOD(OD_60FD_00)) return false; // reads the Motor GPIO inputs

    if (LIMIT_INPUT_MASK(getRxReg()->data)) return false; // Photocell in Light condition: limit swicth not activated
    else return true;
}

/// <summary>
/// The module override this function in order to initialize specific motor registers
/// 
/// </summary>
/// 
/// 
/// <param name=""></param>
/// <returns>true if the initialization termines successfully</returns>
bool VerticalMotor::initializeSpecificObjectDictionaryCallback(void) {
    
    while (!blocking_writeOD(OD_3202_00, 0x41)) ; 	// Motor Drive Submode Select: 6:BLDC 3:CurRed 2:Brake 1:VoS 0: 1=CLOSED_LOOP/O = OPEN_LOOP

    while (!blocking_writeOD(OD_2031_00, 15000)) ;  // Peak current
    while (!blocking_writeOD(OD_2032_00, 5000)) ;  // Maximum Speed
    
    // I2t Parameters
    while (!blocking_writeOD(OD_203B_01, 50000)) ;	// Nominal Current
    while (!blocking_writeOD(OD_203B_02, 2000)) ;	// Maximum Duration Of Peak Current
    while (!blocking_writeOD(OD_203B_03, 0)) ;	    // Threshold
    while (!blocking_writeOD(OD_203B_04, 0)) ;	    // CalcValue
    while (!blocking_writeOD(OD_203B_05, 15000));   // LimitedCurrent
    while (!blocking_writeOD(OD_2056_00, 500)) ;	// Limit Switch Tolerance Band

    // Motor Drive Parameter Set
    while (!blocking_writeOD(OD_3210_01, 10000));   // 50000 Position Loop, Proportional Gain (closed Loop)
    while (!blocking_writeOD(OD_3210_02, 5));	    // 10  Position Loop, Integral Gain (closed Loop)

    // Max Absolute Acceleration and Deceleration
    while (!blocking_writeOD(OD_60C5_00, 5000)) ;   // Max Acceleration
    while (!blocking_writeOD(OD_60C6_00, 5000)) ;   // Max Deceleration

    // Position Range Limit
    while (!blocking_writeOD(OD_607B_01, convert_User_To_Encoder(MIN_POSITION - 10))); 	// Min Position Range Limit
    while (!blocking_writeOD(OD_607B_02, convert_User_To_Encoder(MAX_POSITION + 10)));	// Max Position Range Limit

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

/// <summary>
/// This is the class constructor.
/// </summary>
/// 
/// The Constructor:
/// - Initializes the CanOpenMotor base class:
///     - Set the motor address;
///     - Set The module name;
///     - Set The unit conversion coefficient
/// 
/// - Set the target acceptable precision range;
/// - Initializes the encoder initial position from the configuration file;
/// 
/// <param name=""></param>
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
    if (!isEncoderInitialized()) Notify::activate(Notify::messages::ERROR_VERTICAL_MOTOR_HOMING);
    
}

/// <summary>
/// This function activates the Isocentric correction procedure.
/// </summary>
/// 
/// The Isocentric procedure corrects the actual vertical position of the ARM,
/// when a C-ARM rotation command is executed.
/// 
/// The command activates an automatic positioning where the target is expressed
/// in terms of position variation and not as an absolute target.
/// 
/// The activation command makes use of the following activation parameters:
/// - Speed: MotorConfig::PARAM_AUTO_SPEED;
/// - Acceleration: MotorConfig::PARAM_AUTO_ACC;
/// - Deceleration: MotorConfig::PARAM_AUTO_DEC;
/// 
/// <param name="id"> the requesting command ID to be finally signaled </param>
/// <param name="delta_h">position variation</param>
/// <returns></returns>
bool VerticalMotor::activateIsocentricCorrection(int id, int delta_h)
{   
    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::VERTICAL_ID)) {
        device->setCommandCompleted(CanOpenMotor::MotorCompletedCodes::ERROR_SAFETY);
        return false;
    }

    
    // Activate the automatic positioning for isocentric correction
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_AUTO_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_AUTO_ACC]);
    int dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_AUTO_DEC]);
 
    return device->activateRelativePositioning(id, delta_h, speed, acc, dec);
}

/// <summary>
/// The module override this function in order to handle the automatic positioning comnpleted activities.
/// 
/// </summary>
/// 
/// When an automatic activation completes, the current final encoder positionn is stored
/// in order to be used at the further system power on.
/// 
/// The command_completed_event() is then signaled to the application.
/// 
/// <param name=LABEL_ERROR></param>
void VerticalMotor::automaticPositioningCompletedCallback(MotorCompletedCodes error) {
    
    // Sets the current Vertical position
    if (isEncoderInitialized()) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_VERTICAL, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
    }

    // Always answer with success to the remote command because an error in Vertical activation should not prevent to 
    // proceed with the Exposure workflow!
    device->command_completed_event(getCommandId(), (int)MotorCompletedCodes::COMMAND_SUCCESS);    

    return;
}

/// <summary>
/// The modjule override this function in order to  handle the IDLE activities
/// 
/// </summary>
/// 
/// In Idle status the following activities are implemented:
/// - test of the limit switch activation:
///     - If the limit switch shouild be detected, an alarm is activated and the 
///       encoder position is invalidated. The Gantry shall rest this condition only 
///       with a zero setting procedure.
/// - test of the manual activation:
///     - The Gantry::getVerticalManualActivationIncrease() and Gantry::getVerticalManualActivationDecrease() 
///        functions returns the system triggers for the manual activation.
/// - test of the safety activation condition:
///     - If the safety condition are not meet the motr cannot be activated.
/// 
/// <param name=""></param>
/// <returns>
/// - MotorCompletedCodes::COMMAND_PROCEED: a command can be processed;
/// - Other values: a command cannot be processed due to a number of the reason.
/// 
/// </returns>
CanOpenMotor::MotorCompletedCodes VerticalMotor::idleCallback(void) {
    static bool error_limit_switch = false;
    bool limit_status;
    int speed, acc, dec;
    MotorCompletedCodes ret_code = MotorCompletedCodes::COMMAND_PROCEED;

    // If a limit switch should be engaged then the activation shall be disabled
    limit_status = testLimitSwitch();
    if (limit_status != error_limit_switch) {
        error_limit_switch = limit_status;
        if (error_limit_switch) {
            Notify::activate(Notify::messages::ERROR_VERTICAL_LIMIT_SWITCH);
            Notify::activate(Notify::messages::ERROR_VERTICAL_MOTOR_HOMING);

            // Remove the zero condition
            if (isEncoderInitialized()) {
                setEncoderInitStatus(false);
                MotorConfig::Configuration->setParam(MotorConfig::PARAM_VERTICAL, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
                MotorConfig::Configuration->storeFile();
            }
           
        }
        else  Notify::deactivate(Notify::messages::ERROR_VERTICAL_LIMIT_SWITCH);
    }
    if (error_limit_switch) ret_code = MotorCompletedCodes::ERROR_LIMIT_SWITCH;

    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::VERTICAL_ID)) {
        ret_code = MotorCompletedCodes::ERROR_SAFETY; // Priority over the limit switch
    }

    // Handle a Manual activation mode
    bool man_increase = Gantry::getManualRotationIncrease((int)CANOPEN::MotorDeviceAddresses::VERTICAL_ID);
    bool man_decrease = Gantry::getManualRotationDecrease((int)CANOPEN::MotorDeviceAddresses::VERTICAL_ID);
    if (man_increase || man_decrease) {
        if (!manual_activation_enabled) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_MANUAL_DISABLE);
        else if(ret_code == MotorCompletedCodes::ERROR_SAFETY) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_SAFETY_DISABLE);
        else if(ret_code != MotorCompletedCodes::COMMAND_PROCEED) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
        else {
            speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_MANUAL_SPEED]);
            acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_MANUAL_ACC]);
            dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_MANUAL_DEC]);

            if (man_increase) {
                manual_increment_direction = true;
                if(!device->activateManualPositioning(MAX_POSITION, speed, acc, dec)) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
            }
            else {
                manual_increment_direction = false;
                if(!device->activateManualPositioning(MIN_POSITION, speed, acc, dec)) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
            }
        }
    }

    return ret_code;

}

/// <summary>
/// This function activates the Automatic Homing procedure
/// 
/// </summary>
/// 
/// The procedure makes use of the following activation parameters:
/// - Speed:  MotorConfig::PARAM_HOME_SPEED;
/// - Acceleration and Deceleration: MotorConfig::PARAM_HOME_ACC;
/// 
/// <param name=""></param>
/// <returns>true: the command is processing</returns>
bool VerticalMotor::startHoming(void) {
    
    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::VERTICAL_ID)) {
        device->setCommandCompleted(CanOpenMotor::MotorCompletedCodes::ERROR_SAFETY);
        return false;
    }

    // Gets the Speed and Acceleration from the configuration file
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_HOME_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_HOME_ACC]);
    return device->activateAutomaticHoming(HOMING_ON_METHOD, HOMING_OFF_METHOD, speed, acc);
}

/// <summary>
/// The module overrides this function in order to handle the automatic activation process.
/// </summary>
/// 
/// During the automatic activation, the module checks:
/// - the limit switch activation: the activation is istantly terminated;
/// - the system enabled abort input: the current enabled abort inputs activation shall quickly stop the activation; 
/// 
/// <param name=""></param>
/// <returns></returns>
VerticalMotor::MotorCompletedCodes  VerticalMotor::automaticPositioningRunningCallback(void) {

    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::VERTICAL_ID)) {        
        return MotorCompletedCodes::ERROR_SAFETY;
    }

    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getObstacleRotationStatus((int)CANOPEN::MotorDeviceAddresses::VERTICAL_ID)) {
        return MotorCompletedCodes::ERROR_OBSTACLE_DETECTED;
    }

    // Test the limit switch to early stop the activation
    if(testLimitSwitch()) return MotorCompletedCodes::ERROR_LIMIT_SWITCH;

    // Proceeds with the manual activation
    return MotorCompletedCodes::COMMAND_PROCEED;
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
        Notify::activate(Notify::messages::ERROR_VERTICAL_MOTOR_HOMING);
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
   
 
    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::VERTICAL_ID)) {
        return MotorCompletedCodes::ERROR_SAFETY;
    }

    // Test the limit switch to early stop the activation
    if (testLimitSwitch()) return MotorCompletedCodes::ERROR_LIMIT_SWITCH;

    // Handle the limit switches

    // Handle the safety

    // handle the manual hardware inputs
    bool man_increase = Gantry::getManualRotationIncrease((int)CANOPEN::MotorDeviceAddresses::VERTICAL_ID);
    bool man_decrease = Gantry::getManualRotationDecrease((int)CANOPEN::MotorDeviceAddresses::VERTICAL_ID);
    if ((!man_increase) && (!man_decrease)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    if ((!man_increase) && (manual_increment_direction)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    if ((!man_decrease) && (!manual_increment_direction)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;

    // Proceeds with the manual activation
    return MotorCompletedCodes::COMMAND_PROCEED;
}

/// <summary>
/// The VerticalMotor class override this function in order to 
/// handle the manual activation completed
/// 
/// </summary>
/// <param name=LABEL_ERROR></param>
void VerticalMotor::manualPositioningCompletedCallback(MotorCompletedCodes error) {

    // Sets the current Vertical position
    if (isEncoderInitialized()) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_VERTICAL, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
    }
    return;
}

void VerticalMotor::faultCallback(bool errstat, bool data_changed, unsigned int error_class, unsigned int error_code) {
    if (errstat == false) {
        Notify::deactivate(Notify::messages::WARNING_VERTICAL_DRIVER);
        return;
    }


    if (data_changed){
        //System::String^ driver_error = "CL:" + error_class.ToString() + " CD:" + error_code.ToString();
        System::String^ driver_error = CanOpenMotor::getErrorCode1003(error_code);
        Notify::activate(Notify::messages::WARNING_VERTICAL_DRIVER, driver_error);        
    }

    bool man_increase = Gantry::getManualRotationIncrease((int)CANOPEN::MotorDeviceAddresses::VERTICAL_ID);
    bool man_decrease = Gantry::getManualRotationDecrease((int)CANOPEN::MotorDeviceAddresses::VERTICAL_ID);
    if (man_increase || man_decrease) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);

}
