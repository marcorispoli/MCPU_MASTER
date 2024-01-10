#include "CalibrationConfig.h"
#include "Notify.h"
#include "BodyMotor.h"
#include "PCB301.h"
#include "pd4_od.h"
#include "..\gantry_global_status.h"
#include <thread>


#define ROT_PER_DEGREE ((double) 80 / (double) 3600) //!< Defines the position user units in 0.1 degree

#define BRAKE_INPUT_MASK(x) (x & 0x00080000) //!< Not in the Special region [00II][0000]
#define LIMIT_INPUT_MASK(x) (x & 0x00020000) //!< Not in the Special region [00II][0000]
#define ZERO_INPUT_MASK(x) (x & 0x00040000) //!< Not in the Special region [00II][0000]

#define OUPUT1_OUT_MASK     0x00010000 //!< Not in the Special region [00II][0000]
#define OUPUT2_OUT_MASK     0x00020000 //!< Not in the Special region [00II][0000]

#define MAX_ROTATION_ANGLE  880
#define MIN_ROTATION_ANGLE  -880
#define HOMING_ON_METHOD 20
#define HOMING_OFF_METHOD 19

/// <summary>
/// This is the class constructor.
/// </summary>
/// 
/// The Constructor initializes the base class and assignes the module can open address and 
/// user unit ratio.
/// 
/// <param name=""></param>
BodyMotor::BodyMotor(void): CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::BODY_ID, L"MOTOR_BODY", ROT_PER_DEGREE, false)
{
    // Sets +/- 0.2 ° as the acceptable target range
    setTargetRange(2, 2);
    
    // Gets the initial position of the encoder. If the position is a valid position the oming is not necessary
    bool homing_initialized = false;
    int  init_position = 0;

    if (MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_CURRENT_POSITION] != MotorConfig::MOTOR_UNDEFINED_POSITION) {
        homing_initialized = true;
        init_position = System::Convert::ToInt32(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_CURRENT_POSITION]);
    }
    setEncoderInitStatus(homing_initialized);
    setEncoderInitialUvalue(init_position);

    // Activate a warning condition is the motor should'n be initialized
    if (!isEncoderInitialized()) Notify::activate(Notify::messages::ERROR_BODY_MOTOR_HOMING, false);
    
}

/// <summary>
/// The BodyMotor override this function in order to initialize specific motor registers
/// 
/// </summary>
/// 
/// The modules set the Digital Io as follows:
/// + Output1 and output2: controls the e.m. brake activation. They are normally closed output;
/// + Input1 and Input2: used for limit switches detection. They are active low inputs, 5V threshold;
/// + Input3: homing.  It is not inverted input, with 5V threshold
/// + Input4: read back the actual brake activation status. It is not inverted input, with 5V threshold
/// 
/// The module makes a test on the Brake control.
/// 
/// The sequence of the test is as follows:
/// - Test OFF OFF;
/// - Test OFF ON;
/// - Test ON ON;
/// - Test OFF OFF;
/// 
/// If the test should fail, a relevant non resettable alarm is generated 
/// and no more activations can be executed.
/// 
/// >NOTE: the function returns false only if a communicaton issue should prevent to set or read the registers.
/// 
/// 
/// <param name=""></param>
/// <returns>true if the initialization termines successfully</returns>
bool BodyMotor::initializeSpecificObjectDictionaryCallback(void) {
    
    // Motor Drive Parameter Set
    while (!blocking_writeOD(OD_3210_01, 10000)); // 50000 Position Loop, Proportional Gain (closed Loop)
    while (!blocking_writeOD(OD_3210_02, 5));	 // 10  Position Loop, Integral Gain (closed Loop)

    // Software Position Limit
    if (!blocking_writeOD(OD_607D_01, convert_User_To_Encoder(MIN_ROTATION_ANGLE))) return false;	// Min Position Limit
    if (!blocking_writeOD(OD_607D_02, convert_User_To_Encoder(MAX_ROTATION_ANGLE))) return false;	// Max Position Limit

    // Sets the Output setting
    if (!blocking_writeOD(OD_3250_02, 0)) return false; // Output control not inverted
    if (!blocking_writeOD(OD_3250_03, 0)) return false; // Force Enable = false
    if (!blocking_writeOD(OD_3250_08, 0)) return false; // Routing Enable = false
    if (!blocking_writeOD(OD_60FE_01, 0)) return false; // Set All outputs to 0

    // Set the input setting
    if (!blocking_writeOD(OD_3240_01, 0x4)) return false; // Input control special: I3 = HOMING
    if (!blocking_writeOD(OD_3240_02, 0)) return false;   // Function Inverted: not inverted
    if (!blocking_writeOD(OD_3240_03, 0)) return false;   // Force Enable = false
    if (!blocking_writeOD(OD_3240_06, 0)) return false;   // Input Range Select: threshold = 5V;

    // Writes 0s of both outputs    
    if (!blocking_writeOD(OD_60FE_01, 0)) return false;

    bool brake_activated = true;
    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!blocking_readOD(OD_60FD_00)) continue; // Reads the Inputs

        if (!BRAKE_INPUT_MASK(getRxReg()->data)) {
            brake_activated = false;
            break;
        }
    }

    if (brake_activated) {
        brake_alarm = true;
        Debug::WriteLine("BodyMotor: Failed test output off, off");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT, false);
        
        // Clear the OUTPUTS
        blocking_writeOD(OD_60FE_01, 0);
        return true;
    }

    if (!activateBrake()) {
        Debug::WriteLine("BodyMotor: Failed test output on, on");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT, false);
        return true;
    }

    if (!deactivateBrake()) {
        Debug::WriteLine("BodyMotor: Failed test output off, on");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT, false);        
    }

    return true;
}


bool BodyMotor::activateBrake(void) {

    // Sets the OUTPUT2 ON and OUTPUT1 ON and test again the Brake input
    if (!blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK | OUPUT1_OUT_MASK)) {
        blocking_writeOD(OD_60FE_01, 0);
        return false;
    }

    // Test the input feedback to detect the effective activation
    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!blocking_readOD(OD_60FD_00)) continue; // Reads the Inputs

        if (BRAKE_INPUT_MASK(getRxReg()->data)) return true;
    }

    // Error in detecting the brake activation
    blocking_writeOD(OD_60FE_01, 0);
    brake_alarm = true;
    return false;
}

bool  BodyMotor::deactivateBrake(void) {

    // Sets the OUTPUT2 ON and OUTPUT1 OFF and test again the Brake input   
    if (!blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK)) {
        blocking_writeOD(OD_60FE_01, 0);
        return false;
    }

    // Test the input feedback to detect the effective deactivation
    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!blocking_readOD(OD_60FD_00)) continue; // Reads the Inputs
        if (!BRAKE_INPUT_MASK(getRxReg()->data)) return true;
    }

    // Error in detecting the brake activation
    blocking_writeOD(OD_60FE_01, 0);
    brake_alarm = true;
    return false;
}


/// <summary>
/// The BodyMotor class override this function in order to handle the IDLE activities
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
CanOpenMotor::MotorCompletedCodes BodyMotor::idleCallback(void) {
    static bool manual_activation_request = false;
    static bool error_limit_switch = false;

    int speed, acc, dec;
    bool brake_unlocking_status, limit_status;
    

    // With the brake alarm present, no more action can be executed
    if (brake_alarm) {
        
        // Keeps the alarm ON
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT, false);

        // Alarm condition
        blocking_writeOD(OD_60FE_01, 0); // Set All outputs to 0
        return MotorCompletedCodes::ERROR_BRAKE_DEVICE;
    }

    // reads the Motor GPIO inputs
    if (!blocking_readOD(OD_60FD_00)) return MotorCompletedCodes::ERROR_ACCESS_REGISTER;
    
    if (BRAKE_INPUT_MASK(getRxReg()->data)) brake_unlocking_status = true; 
    else brake_unlocking_status = false;

    if (LIMIT_INPUT_MASK(getRxReg()->data)) limit_status = true; // Photocell in Light condition: limit swicth activated
    else limit_status = false;


    // If a limit switch should be engaged then the activation shall be disabled
    if (limit_status != error_limit_switch) {
        error_limit_switch = limit_status;
        if (error_limit_switch) {
            Notify::activate(Notify::messages::ERROR_BODY_LIMIT_SWITCH, false);
            Notify::activate(Notify::messages::ERROR_BODY_MOTOR_HOMING, false);

            // Remove the zero condition
            if (isEncoderInitialized()) {
                setEncoderInitStatus(false);
                MotorConfig::Configuration->setParam(MotorConfig::PARAM_BODY, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
                MotorConfig::Configuration->storeFile();
            }
        }
        else  Notify::deactivate(Notify::messages::ERROR_BODY_LIMIT_SWITCH);
    }

    if (brake_unlocking_status) {
        brake_alarm = true;
        Debug::WriteLine("BodyMotor: Failed test brake input in IDLE");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT, false);
        blocking_writeOD(OD_60FE_01, 0); // Set All outputs to 0
        return MotorCompletedCodes::ERROR_BRAKE_DEVICE;
    }

    if(error_limit_switch) return MotorCompletedCodes::ERROR_LIMIT_SWITCH;
    
    
    // Handle a Manual activation mode
    if (!manual_activation_request) {
        if ((Gantry::getBodyManualActivationIncrease()) || (Gantry::getBodyManualActivationDecrease())) {

           
            manual_activation_request = true;

            if (manual_activation_enabled) {
                speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_MANUAL_SPEED]);
                acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_MANUAL_ACC]);
                dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_MANUAL_DEC]);

                if (Gantry::getBodyManualActivationIncrease()) {
                    manual_increment_direction = true;
                    device->activateManualPositioning(MAX_ROTATION_ANGLE, speed, acc, dec);
                }
                else {
                    manual_increment_direction = false;
                    device->activateManualPositioning(MIN_ROTATION_ANGLE, speed, acc, dec);
                }
            } // Enabled command
        }
        
    }
    else {
        if ((!Gantry::getBodyManualActivationIncrease()) && (!Gantry::getBodyManualActivationDecrease())) {
            manual_activation_request = false;
        }
    }

    return MotorCompletedCodes::COMMAND_PROCEED;
    
}

/// <summary>
/// This callback is called by the base class during the motor activation.
/// </summary>
/// 
/// The function will checks for the limit switch activation condition.
/// 
/// <param name=""></param>
/// <returns></returns>
BodyMotor::MotorCompletedCodes  BodyMotor::automaticPositioningRunningCallback(void) {

    // Checks for the limit switch activation     
    if (blocking_readOD(OD_60FD_00)) {
        // Limit switch activation
        if (LIMIT_INPUT_MASK(getRxReg()->data)) return MotorCompletedCodes::ERROR_LIMIT_SWITCH;
    }

    // Proceeds with the manual activation
    return MotorCompletedCodes::COMMAND_PROCEED;
}

/// <summary>
/// The BodyMotor class override this function in order to update the current position
/// and the command termination
/// 
/// </summary>
/// <param name=LABEL_ERROR></param>
void BodyMotor::automaticPositioningCompletedCallback(MotorCompletedCodes error) {
    if (isEncoderInitialized()) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_BODY, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
    }

    // Notify the command termination event
    device->command_completed_event(getCommandId(), (int)error);
    return;
}

void BodyMotor::manualPositioningCompletedCallback(MotorCompletedCodes error) {
    if (isEncoderInitialized()) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_BODY, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
    }

    device->command_completed_event((int) 0, (int) error);
    return;
}

/// <summary>
/// The BodyMotor class override this function in order to 
///handle the homing completion process.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
void BodyMotor::automaticHomingCompletedCallback(MotorCompletedCodes error) {


    if (isEncoderInitialized()) {
        // Set the position in the configuration file and clear the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_BODY, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
        Notify::deactivate(Notify::messages::ERROR_BODY_MOTOR_HOMING);
    }
    else {
        // Reset the position in the configuration file and reactivate the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_BODY, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
        MotorConfig::Configuration->storeFile();
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_HOMING, false);
    }

    // Notify the command termination event
    device->command_completed_event((int)0, (int)error);
}


/// <summary>
/// This function activates the Automatic Homing procedure
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool BodyMotor::startHoming(void) {

    // Gets the Speed and Acceleration from the configuration file
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_HOME_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_HOME_ACC]);
    return device->activateAutomaticHoming(HOMING_ON_METHOD, HOMING_OFF_METHOD, speed, acc);
}



/// <summary>
/// The BodyMotor class override this function in order to handle the manual activation process.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BodyMotor::MotorCompletedCodes  BodyMotor::manualPositioningRunningCallback(void) {

    // Handles the enable condition
    if (!manual_activation_enabled) {
        return MotorCompletedCodes::ERROR_COMMAND_DISABLED;
    }

    // Checks for the limit switch activation     
    if (blocking_readOD(OD_60FD_00)) {
        // Limit switch activation
        if (LIMIT_INPUT_MASK(getRxReg()->data)) return MotorCompletedCodes::ERROR_LIMIT_SWITCH;
    }


    // Handle the safety

    // handle the manual hardware inputs
    if ((!Gantry::getBodyManualActivationIncrease()) && (!Gantry::getBodyManualActivationDecrease())) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    if ((!Gantry::getBodyManualActivationIncrease()) && (manual_increment_direction)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    if ((!Gantry::getBodyManualActivationDecrease()) && (!manual_increment_direction)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;

    // Proceeds with the manual activation
    return MotorCompletedCodes::COMMAND_PROCEED;
}

bool BodyMotor::brakeCallback(void){
    deactivateBrake();
    return true;
}

bool BodyMotor::unbrakeCallback(void) {

    // Unlock the Brake device
    if (!activateBrake()) {
        Debug::WriteLine("BodyMotor: Activation failed to unlock the brake device");
        return false;
    }

    return true;
}
