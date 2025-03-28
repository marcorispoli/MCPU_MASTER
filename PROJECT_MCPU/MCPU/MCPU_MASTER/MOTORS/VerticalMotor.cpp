#include "ConfigurationFiles.h"
#include "Notify.h"
#include "VerticalMotor.h"
#include "pd4_od.h"
#include "gantry_global_status.h"



#define ROT_PER_MM ((double) 1/ (double) 0.714 ) //!< Defines the position user units in mm

#define LIMIT_INPUT_MASK(x) (x & PD4_MOTOR_DI1) //!< Sets the limit photocell switch input
#define UP_LIMIT_INPUT_MASK(x) !(x & PD4_MOTOR_DI1) //!< Sets the limit photocell switch input
#define DOWN_LIMIT_INPUT_MASK(x) !(x & PD4_MOTOR_DI4) //!< Sets the limit photocell switch input
#define ZERO_INPUT_MASK(x) !(x & PD4_MOTOR_DI3) //!< Sets the Zero photocell switch input

#define MAX_POSITION 634    //!< Defines the Maximum software position respect the zero setting point
#define MIN_POSITION 0   //!< Defines the Minimum software position respect the zero setting point
#define ABSOLUTE_MAX_POSITION (MAX_POSITION+50)     //!< Defines the Absolute Maximum mechanical position (invalid the zero position at the restart)
#define ABSOLUTE_MIN_POSITION (MIN_POSITION-50)    //!< Defines the Absolute Minimum mechanical position (invalid the zero position at the restart)

#define HOMING_ON_METHOD 20 //!< Zero setting approaching method starting with the Zero photocell ON
#define HOMING_OFF_METHOD 19//!< Zero setting approaching method starting with the Zero photocell OFF

/// <summary>
/// This function test high and low limit switches.
/// </summary>
/// 
/// The status of the internal variables high_photocell and low_photocell
/// sre assigned after the function is called.
/// 
/// <param name=""></param>
/// <returns></returns>
void VerticalMotor::testLimitSwitch(void) {
    

    if (!blocking_readOD(OD_60FD_00)) return ; // reads the Motor GPIO inputs

    // Not used for prototype
    high_photocell = false;
//    if (UP_LIMIT_INPUT_MASK(getRxReg()->data)) high_photocell = true;
//    else high_photocell = false;

    if (DOWN_LIMIT_INPUT_MASK(getRxReg()->data)) low_photocell = true;
    else low_photocell = false;
 

    if (ZERO_INPUT_MASK(getRxReg()->data)) zero_photocell = true;
    else zero_photocell = false;

    return;
}


/// <summary>
/// 
/// 
/// </summary>
/// 
/// The following registers will be uploaded here:
/// 
/// | Register Address | Value | Description|
/// |:--| : --| : --|
/// |0x3202 : 00 | 0x41 | Sets the BLDC motor model|
/// |0x2031 : 00 | 15000 | Pick current = 15Amp|
/// |0x2032 : 00 | 5000 | Maximum speed|
/// |0x203B : 01 | 50000 | Nominal current |
/// |0x203B : 02 | 2000 | Maximum duration of peak current |
/// |0x203B : 03 | 0 | Threshold|
/// |0x203B : 04 | 0 | Calc Value|
/// |0x203B : 05 | 15000 | Limit current|
/// |0x2056 : 00 | 500 | Limit swithc tollerance Band|
/// |0x3210 : 01 | 10000 | P of Pid|
/// |0x3210 : 02 | 5 | I of Pid|
/// |0x60C5 : 00 | 5000 | Max acceleration|
/// |0x60C6 : 00 | 5000 | Max deceleration|
/// |0x3240 : 01 | 4 | Input I3 = homing|
/// |0x3240 : 02 | 0 | Not inverted inputs|
/// |0x3240 : 03 | 0 | Force Enable = false|
/// |0x3240 : 06 | 0 | Input voltage threshold = 5V |
/// 
#define VERTICAL_OD_CODE 0x0001
unsigned short VerticalMotor::initializeSpecificObjectDictionaryCallback(void) {
    
    // Position Range Limit
    while (!blocking_writeOD(OD_607B_01, convert_Absolute_User_To_Encoder(ABSOLUTE_MIN_POSITION))); // Min Position Range Limit
    while (!blocking_writeOD(OD_607B_02, convert_Absolute_User_To_Encoder(ABSOLUTE_MAX_POSITION)));	// Max Position Range Limit

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

      
    // Set the input setting
    while (!blocking_writeOD(OD_3240_01, 0x4)) ; // Input control special: I3 = HOMING
    while (!blocking_writeOD(OD_3240_02, 0)) ;   // Function Inverted: not inverted
    while (!blocking_writeOD(OD_3240_03, 0)) ;   // Force Enable = false
    while (!blocking_writeOD(OD_3240_06, 0)) ;   // Input Range Select: threshold = 5V;

    return VERTICAL_OD_CODE;
}

/// <summary>
/// 
/// </summary>
/// 
/// The Constructor:
/// - Initializes the CanOpenMotor base class:
///     - Set the motor address;
///     - Set The module name;
///     - Set The unit conversion coefficient
/// 
/// - Set the target acceptable precision range to +/-5mm;
/// - Initializes the encoder initial position from the configuration file;
/// 
/// <param name=""></param>
VerticalMotor::VerticalMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::VERTICAL_ID, L"MOTOR_VERTICAL", MotorConfig::PARAM_VERTICAL, Notify::messages::ERROR_VERTICAL_MOTOR_HOMING, MIN_POSITION, MAX_POSITION, ROT_PER_MM, 1, false)
{
    // Sets +/- 5mm as the acceptable target range
    setTargetRange(5, 5);    
     
    // initializes the value of the detected photocells
    high_photocell = false;
    low_photocell = false;
}

/// <summary>
/// 
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
/// <returns>
/// + true: the activation started successfully;
/// +false: the activation is rejected;
/// </returns>
bool VerticalMotor::activateIsocentricCorrection(int id, int delta_h)
{   
    
    // If the safety condition prevent the command execution it is immediatelly aborted
    Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device->device_id);
    if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
        LogClass::logInFile("Motor <" + device->device_id.ToString() + ">: safety condition error > " + safety.ToString());
        device->setCommandCompleted(CanOpenMotor::MotorCompletedCodes::ERROR_SAFETY);
        return false;
    }
    
    return device->activateRelativePositioning(id, delta_h);
}


/// <summary>
/// 
/// 
/// </summary>
/// 
/// In idle state, the module test limit switches 
/// verifying that both cannot be in active status at the same time. 
///
/// <param name=""></param>
/// <returns>
/// - MotorCompletedCodes::COMMAND_PROCEED: a command can be processed;
/// - Other values: a command cannot be processed due to a number of the reason.
/// 
/// </returns>
VerticalMotor::MotorCompletedCodes VerticalMotor::idleCallback(void) {
    static bool error_limit_switch = false;
   
    
    MotorCompletedCodes ret_code = MotorCompletedCodes::COMMAND_PROCEED;
    
    // Update the value of the fotocells
    // Notify::activate(Notify::messages::ERROR_VERTICAL_LIMIT_SWITCH);
    // Notify::activate(Notify::messages::ERROR_VERTICAL_MOTOR_HOMING);
    if (!simulator_mode) {
        testLimitSwitch();
        if (high_photocell && low_photocell) {
            if(!error_limit_switch) Notify::activate(Notify::messages::ERROR_VERTICAL_LIMIT_SWITCH);
            error_limit_switch = true;
        }
    }
    if (error_limit_switch) ret_code = MotorCompletedCodes::ERROR_LIMIT_SWITCH;

    return ret_code;

}

/// <summary>
/// 
/// 
/// </summary>
/// 
/// The automatic zero setting procedure activates the motor rotation 
/// until a photocell dedicate to the zero setting procedure intercepts 
/// the mechanical zero setting point.
/// 
/// This method should be called by the application for service,
/// in case a automatic zero setting should be executed.
/// 
/// This methods is not a blocking method for the caller thread:
/// + the encoder reset may takes time after this method returns.
/// 
/// \note:
/// + the Application shall call the method CanOpenMotor::isRunning() to checks if the 
/// manual command is terminated;
/// + the application shall call the CanOpenMotor::getCommandCompletedCode() to get the result 
/// of the command as soon as the CanOpenMotor::isRunning() should return false.
/// 
/// <param name=""></param>
/// <returns>
/// + true: the zero setting process is actually started;
/// + false: the zero setting cannot start for some reason (CanOpenMotor::getCommandCompletedCode() for gets the error code) 
/// </returns>
bool VerticalMotor::startAutoHoming(void) {
    
    // If the safety condition prevent the command execution it is immediatelly aborted
    Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device->device_id);
    if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
        LogClass::logInFile("Motor <" + device->device_id.ToString() + ">: safety condition error > " + safety.ToString());
        device->setCommandCompleted(CanOpenMotor::MotorCompletedCodes::ERROR_SAFETY);
        return false;
    }

    // Gets the Speed and Acceleration from the configuration file
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_HOME_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_VERTICAL)[MotorConfig::PARAM_HOME_ACC]);
    return device->activateAutomaticHoming(HOMING_ON_METHOD, HOMING_OFF_METHOD, speed, acc);
}

/// <summary>
/// 
/// 
/// </summary>
/// 
/// The manual zero setting procedure is a procedure that doesn't require
/// any motor activation. The motor sets its internal encoder to the value
/// manually passed by this module.
/// 
/// This method should be called by the application for service,
/// in case a position manual zero setting should be executed.
/// 
/// This methods is not a blocking method for the caller thread:
/// + the encoder reset may takes time after this method returns.
/// 
/// \note:
/// + the Application shall call the method CanOpenMotor::isRunning() to checks if the 
/// manual command is terminated;
/// + the application shall call the CanOpenMotor::getCommandCompletedCode() to get the result 
/// of the command as soon as the CanOpenMotor::isRunning() should return false.
/// 
/// <param name="target_position">This is the current effective position in 1mm units</param>
/// <returns>
/// + true: the zero setting process is actually started;
/// + false: the zero setting cannot start
/// </returns>
bool VerticalMotor::startManualHoming(int target_position) {
   
    if (device->isPositionFromExternalSensor()) return device->activateExternalHoming(target_position);
    else return device->activateManualHoming(target_position);
}

/// <summary>
/// This callback is called during the activation preparation fase.
/// </summary>
/// 
/// The module override this callback in order to handle the status of the limit switches 
/// before to init the activation.
/// 
/// <param name="current_command">the base class passes the code of the current command that is excuting</param>
/// <param name="current_position">the base class passes the current position</param>
/// <param name="target_position">the base class passes the target command position</param>
/// <returns>
/// + MotorCompletedCodes::COMMAND_PROCEED: the command can proceed;
/// + MotorCompletedCodes::ERROR_LIMIT_SWITCH: a limit switch is detected active;
/// </returns>
VerticalMotor::MotorCompletedCodes VerticalMotor::preparationCallback(MotorCommands current_command, int current_position, int target_position) {
    if(current_command == MotorCommands::MOTOR_AUTO_HOMING) return MotorCompletedCodes::COMMAND_PROCEED;
    if (simulator_mode) return MotorCompletedCodes::COMMAND_PROCEED;

    // test the limit switches: prevents to start in a wrong direction if the related photocell is activated
    if ((getMotorDirection() == motor_rotation_activations::MOTOR_INCREASE) && high_photocell) return MotorCompletedCodes::ERROR_LIMIT_SWITCH;
    if ((getMotorDirection() == motor_rotation_activations::MOTOR_DECREASE) && low_photocell) return MotorCompletedCodes::ERROR_LIMIT_SWITCH;
    
    return MotorCompletedCodes::COMMAND_PROCEED;

}

/// <summary>
/// 
/// </summary>
/// 
/// The module override this function in order to test the status of the limit switches during the 
/// motor activation.
/// 
/// <param name="current_command">the base class passes the code of the current command that is excuting</param>
/// <param name="current_position">the base class passes the current position</param>
/// <param name="target_position">the base class passes the target command position</param>
/// <returns>
/// + MotorCompletedCodes::COMMAND_PROCEED: the command can proceed;
/// + MotorCompletedCodes::ERROR_LIMIT_SWITCH: a limit switch is detected active;
/// </returns>
VerticalMotor::MotorCompletedCodes VerticalMotor::runningCallback(MotorCommands current_command, int current_position, int target_position) {
    if(simulator_mode) return MotorCompletedCodes::COMMAND_PROCEED;
    
    // Test the limit switch to early stop the activation
    testLimitSwitch();
    if ((getMotorDirection() == motor_rotation_activations::MOTOR_INCREASE) && high_photocell) return MotorCompletedCodes::ERROR_LIMIT_SWITCH;
    if ((getMotorDirection() == motor_rotation_activations::MOTOR_DECREASE) && low_photocell) return MotorCompletedCodes::ERROR_LIMIT_SWITCH;

    // Proceeds with the manual activation
    return MotorCompletedCodes::COMMAND_PROCEED;
}

/// <summary>
/// 
/// </summary>
/// 
/// The module overrides this callback in order to handle the Automatic activation termination:
/// + in case of automatic activation (Isocentric) the module shall send the \ref EVENT_Executed
/// event to the AWS.
/// 
/// <param name="id">the identifier of the AWS command;</param>
/// <param name="current_command">the current executed command code;</param>
/// <param name="current_position">the final Vertical activation</param>
/// <param name="term_code">the command_completed code of the activation;</param>
void VerticalMotor::completedCallback(int id, MotorCommands current_command, int current_position, MotorCompletedCodes term_code) {
    if (current_command != MotorCommands::MOTOR_AUTO_POSITIONING) return;

    if(id) command_completed_event(id, (int)term_code);
    return;

}


/// <summary>
/// The module overrrides this callback in order to activate a specific error message
/// </summary>
/// 
/// 
/// <param name="errstat"></param>
/// <param name="data_changed"></param>
/// <param name="error_class"></param>
/// <param name="error_code"></param>
void VerticalMotor::faultCallback(bool errstat, bool data_changed, unsigned int error_class, unsigned int error_code) {
    if (errstat == false) {
        Notify::deactivate(Notify::messages::INFO_VERTICAL_DRIVER);
        return;
    }


    if (data_changed){
        //System::String^ driver_error = "CL:" + error_class.ToString() + " CD:" + error_code.ToString();
        System::String^ driver_error = CanOpenMotor::getErrorCode1003(error_code);
        Notify::activate(Notify::messages::INFO_VERTICAL_DRIVER, driver_error);        
    }

}
