#include "CalibrationConfig.h"
#include "Notify.h"
#include "BodyMotor.h"
#include "PCB301.h"
#include "pd4_od.h"
#include "..\gantry_global_status.h"
#include <thread>



#define ROT_PER_DEGREE ((double) 80 / (double) 3600) //!< Defines the position user units in 0.1 degree

#define BRAKE_INPUT_MASK(x) (x & PD4_MOTOR_DI4) //!< Not in the Special region [00II][0000]
#define LIMIT_INPUT_MASK(x) (x & PD4_MOTOR_DI2) //!< Not in the Special region [00II][0000]
#define ZERO_INPUT_MASK(x) (x & PD4_MOTOR_DI3) //!< Not in the Special region [00II][0000]

#define OUPUT1_OUT_MASK     PD4_MOTOR_DO1 //!< Not in the Special region [00II][0000]
#define OUPUT2_OUT_MASK     PD4_MOTOR_DO2 //!< Not in the Special region [00II][0000]

#define MAX_ROTATION_ANGLE  880
#define MIN_ROTATION_ANGLE  -880
#define HOMING_ON_METHOD 20
#define HOMING_OFF_METHOD 19

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
    if (!isEncoderInitialized()) Notify::activate(Notify::messages::ERROR_BODY_MOTOR_HOMING);
    
}

void BodyMotor::resetCallback(void) {

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
    if (!isEncoderInitialized()) Notify::activate(Notify::messages::ERROR_BODY_MOTOR_HOMING);

    // Activates the configuration of the device
    activateConfiguration();
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

    // Position Range Limit
    while (!blocking_writeOD(OD_607B_01, convert_User_To_Encoder(MIN_ROTATION_ANGLE - 50))); 	// Min Position Range Limit
    while (!blocking_writeOD(OD_607B_02, convert_User_To_Encoder(MAX_ROTATION_ANGLE + 50)));	// Max Position Range Limit

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
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT);
        
        // Clear the OUTPUTS
        blocking_writeOD(OD_60FE_01, 0);
        return true;
    }

    if (!activateBrake()) {
        Debug::WriteLine("BodyMotor: Failed test output on, on");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT);
        return true;
    }

    if (!deactivateBrake()) {
        Debug::WriteLine("BodyMotor: Failed test output off, on");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT);        
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
    
    static bool error_limit_switch = false;
    MotorCompletedCodes ret_code = MotorCompletedCodes::COMMAND_PROCEED;
    int speed, acc, dec;
    bool brake_unlocking_status, limit_status;
    

    // With the brake alarm present, no more action can be executed
    if (brake_alarm) {
        
        // Keeps the alarm ON
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT);

        // Alarm condition
        blocking_writeOD(OD_60FE_01, 0); // Set All outputs to 0
        return MotorCompletedCodes::ERROR_BRAKE_DEVICE;
    }

    // reads the Motor GPIO inputs
    if (!blocking_readOD(OD_60FD_00)) return MotorCompletedCodes::ERROR_ACCESS_REGISTER;
    
    if (BRAKE_INPUT_MASK(getRxReg()->data)) {        
        brake_alarm = true;
        Debug::WriteLine("BodyMotor: Failed test brake input in IDLE");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT);
        blocking_writeOD(OD_60FE_01, 0); // Set All outputs to 0
        return MotorCompletedCodes::ERROR_BRAKE_DEVICE;        
    }

    // Test the limit switch activation
    if (LIMIT_INPUT_MASK(getRxReg()->data)) limit_status = true; // Photocell in Light condition: limit swicth activated
    else limit_status = false;

    // If a limit switch should be engaged then the activation shall be disabled
    if (limit_status != error_limit_switch) {
        error_limit_switch = limit_status;
        if (error_limit_switch) {
            Notify::activate(Notify::messages::ERROR_BODY_LIMIT_SWITCH);
            Notify::activate(Notify::messages::ERROR_BODY_MOTOR_HOMING);

            // Remove the zero condition
            if (isEncoderInitialized()) {
                setEncoderInitStatus(false);
                MotorConfig::Configuration->setParam(MotorConfig::PARAM_BODY, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
                MotorConfig::Configuration->storeFile();
            }
        }
        else  Notify::deactivate(Notify::messages::ERROR_BODY_LIMIT_SWITCH);
    }

    if(error_limit_switch) ret_code = MotorCompletedCodes::ERROR_LIMIT_SWITCH;
    
    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::BODY_ID)) {
        ret_code = MotorCompletedCodes::ERROR_SAFETY; // Priority over the limit switch
    }

    // Handle a Manual activation mode    
    bool man_increase = Gantry::getManualRotationIncrease((int)CANOPEN::MotorDeviceAddresses::BODY_ID);
    bool man_decrease = Gantry::getManualRotationDecrease((int)CANOPEN::MotorDeviceAddresses::BODY_ID);
    if (man_increase || man_decrease) {
        if (!manual_activation_enabled) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_MANUAL_DISABLE);
        else if (ret_code == MotorCompletedCodes::ERROR_SAFETY) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_SAFETY_DISABLE);
        else if (ret_code != MotorCompletedCodes::COMMAND_PROCEED) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
        else {
            speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_MANUAL_SPEED]);
            acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_MANUAL_ACC]);
            dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_MANUAL_DEC]);

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
    
    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::BODY_ID)) {
        return MotorCompletedCodes::ERROR_SAFETY;
    }

    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getObstacleRotationStatus((int)CANOPEN::MotorDeviceAddresses::BODY_ID)) {
        return MotorCompletedCodes::ERROR_OBSTACLE_DETECTED;
    }


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
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_HOMING);
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

    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::BODY_ID)) {
        device->setCommandCompleted(CanOpenMotor::MotorCompletedCodes::ERROR_SAFETY);
        return false;
    }

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

    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::BODY_ID)) {
        return MotorCompletedCodes::ERROR_SAFETY;
    }

    // Checks for the limit switch activation     
    if (blocking_readOD(OD_60FD_00)) {
        // Limit switch activation
        if (LIMIT_INPUT_MASK(getRxReg()->data)) return MotorCompletedCodes::ERROR_LIMIT_SWITCH;
    }


    // Handle the safety

    // handle the manual hardware inputs
    bool man_increase = Gantry::getManualRotationIncrease((int)CANOPEN::MotorDeviceAddresses::BODY_ID);
    bool man_decrease = Gantry::getManualRotationDecrease((int)CANOPEN::MotorDeviceAddresses::BODY_ID);
    if ((!man_increase) && (!man_decrease)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    if ((!man_increase) && (manual_increment_direction)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    if ((!man_decrease) && (!manual_increment_direction)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;

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

void BodyMotor::faultCallback(bool errstat, bool data_changed, unsigned int error_class, unsigned int error_code) {

    if (errstat == false) {
        Notify::deactivate(Notify::messages::WARNING_BODY_DRIVER);
        return;
    }


    if (data_changed) {
        //System::String^ driver_error = "CL:" + error_class.ToString() + " CD:" + error_code.ToString();
        System::String^ driver_error = CanOpenMotor::getErrorCode1003(error_code);
        Notify::activate(Notify::messages::WARNING_BODY_DRIVER, driver_error);
    }

    bool man_increase = Gantry::getManualRotationIncrease((int)CANOPEN::MotorDeviceAddresses::BODY_ID);
    bool man_decrease = Gantry::getManualRotationDecrease((int)CANOPEN::MotorDeviceAddresses::BODY_ID);
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
BodyMotor::MotorCompletedCodes BodyMotor::automaticPositioningPreparationCallback(void) {

    // Invalidate the position: if the command should completes the encoder position will lbe refresh 
    // with the current valid position
    MotorConfig::Configuration->setParam(MotorConfig::PARAM_BODY, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
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
BodyMotor::MotorCompletedCodes BodyMotor::manualPositioningPreparationCallback(void) {
    return automaticPositioningPreparationCallback();
}