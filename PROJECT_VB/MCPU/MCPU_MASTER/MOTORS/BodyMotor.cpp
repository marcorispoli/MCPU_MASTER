#include "CalibrationConfig.h"
#include "Notify.h"
#include "BodyMotor.h"
#include "PCB301.h"
#include "pd4_od.h"
#include <thread>


#define ROT_PER_DEGREE ((double) 80 / (double) 3600) //!< Defines the position user units in 0.1 degree

#define BRAKE_INPUT_MASK(x) (x & 0x00080000) //!< Not in the Special region [00II][0000]
#define OUPUT1_OUT_MASK     0x00010000 //!< Not in the Special region [00II][0000]
#define OUPUT2_OUT_MASK     0x00020000 //!< Not in the Special region [00II][0000]

#define MAX_ROTATION_ANGLE 900
#define MIN_ROTATION_ANGLE -900
#define HOMING_ON_METHOD 19
#define HOMING_OFF_METHOD 20

/// <summary>
/// This is the class constructor.
/// </summary>
/// 
/// The Constructor initializes the base class and assignes the module can open address and 
/// user unit ratio.
/// 
/// <param name=""></param>
BodyMotor::BodyMotor(void): CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::BODY_ID, L"MOTOR_BODY", ROT_PER_DEGREE)
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
 
    this->home_initialized = homing_initialized;
    this->encoder_initial_value = init_position;

    // Activate a warning condition is the motor should'n be initialized
    if (!home_initialized) Notify::activate(Notify::messages::ERROR_BODY_MOTOR_HOMING, false);
    
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

    // Check if the BRAKE input is OFF. In case it should be ON, a relevant alarm shall be activated
    if (!blocking_readOD(OD_60FD_00)) return false; // Reads the Inpuyts
    if (BRAKE_INPUT_MASK(rxSdoRegister->data)) {
        brake_alarm = true;
        Debug::WriteLine("BodyMotor: Failed test output off, off");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT, false);
        return true;
    }

    // Sets the OUTPUT2 ON and OUTPUT1 OFF and test again the Brake input
    if (!blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK)) return false;
    if (!blocking_readOD(OD_60FD_00)) return false; // Reads the Inpuyts
    if (BRAKE_INPUT_MASK(rxSdoRegister->data)) {
        brake_alarm = true;
        Debug::WriteLine("BodyMotor: Failed test output off, on");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT, false);

        // Clear the OUTPUTS
        blocking_writeOD(OD_60FE_01, 0);
        return true;
    }

    // Sets the OUTPUT2 ON and OUTPUT1 ON and test again the Brake input
    if (!blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK | OUPUT1_OUT_MASK)) return false;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (!blocking_readOD(OD_60FD_00)) return false; // Reads the Inputs
    if (!BRAKE_INPUT_MASK(rxSdoRegister->data)) {
        brake_alarm = true;
        Debug::WriteLine("BodyMotor: Failed test output on, on");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT, false);

        // Clear the OUTPUTS
        blocking_writeOD(OD_60FE_01, 0);
        return true;
    }

    // Sets the OUTPUT2 ON and OUTPUT1 OFF and test again the Brake input
    if (!blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK)) return false;
    if (!blocking_readOD(OD_60FD_00)) return false; // Reads the Inpuyts
    if (BRAKE_INPUT_MASK(rxSdoRegister->data)) {
        brake_alarm = true;
        Debug::WriteLine("BodyMotor: Failed test last output off, off ");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT, false);

        // Clear the OUTPUTS
        blocking_writeOD(OD_60FE_01, 0);
        return true;
    }

    return true;
}

/// <summary>
/// The BodyMotor class override this function in order to handle the IDLE activities
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
CanOpenMotor::MotorCompletedCodes BodyMotor::idleCallback(void) {
    static PCB301::body_activation_options body_request = PCB301::body_activation_options::BODY_NO_ACTIVATION;
    int speed, acc, dec;

    // With the brake alarm present, no more action can be executed
    if (brake_alarm) {

        // Alarm condition
        blocking_writeOD(OD_60FE_01, 0); // Set All outputs to 0
        return MotorCompletedCodes::ERROR_BRAKE_DEVICE;
    }

    // Check if the BRAKE input is OFF. In case it should be ON, a relevant alarm shall be activated
    if (!blocking_readOD(OD_60FD_00)) return MotorCompletedCodes::ERROR_ACTIVATION_REGISTER;
    if (BRAKE_INPUT_MASK(rxSdoRegister->data)) {

        brake_alarm = true;
        Debug::WriteLine("BodyMotor: Failed test brake input in IDLE");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT, false);
        blocking_writeOD(OD_60FE_01, 0); // Set All outputs to 0
        return MotorCompletedCodes::ERROR_BRAKE_DEVICE;
    }

    // Handle the Safety condition 

    // Handle a Manual activation mode
    if (PCB301::getBodyActivationStatus() != body_request) {
        body_request = PCB301::getBodyActivationStatus();

        if (manual_activation_enabled) {
            speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_MANUAL_SPEED]);
            acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_MANUAL_ACC]);
            dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_MANUAL_DEC]);

            switch (body_request) {
            case PCB301::body_activation_options::BODY_CW_ACTIVATION:
                device->activateManualPositioning(MAX_ROTATION_ANGLE, speed, acc, dec);
                manual_cw_direction = true;
                break;
            case PCB301::body_activation_options::BODY_CCW_ACTIVATION:
                device->activateManualPositioning(MIN_ROTATION_ANGLE, speed, acc, dec);
                manual_cw_direction = false;
                break;

            }
        } // Enabled command
    }

   
    return MotorCompletedCodes::COMMAND_PROCEED;
    
}

/// <summary>
/// The BodyMotor class override this function in order to 
/// deactivate the motor brake before to start the motor in the target direction.
/// 
/// </summary>
/// 
/// This function shall unlock the e.m brake to allow the rotation.
/// 
/// The function test the brake unlocked condition befor to 
/// enable the command execution.
/// 
/// In case of fault condition, a non resettable alarm is activated.
/// 
/// <param name=""></param>
/// <returns></returns>
CanOpenMotor::MotorCompletedCodes BodyMotor::automaticPositioningPreparationCallback(void) {

    // Unlock the Brake device
    if (!blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK | OUPUT1_OUT_MASK)) {

        // Failed to receive the command answer: in any case switch off the brake
        blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK);
        return MotorCompletedCodes::ERROR_BRAKE_DEVICE;
    }

    // wait a while before to proceed
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Test if the brake is actually activated
    if (!blocking_readOD(OD_60FD_00)) return MotorCompletedCodes::ERROR_BRAKE_DEVICE; // Reads the Inputs
    if (!BRAKE_INPUT_MASK(rxSdoRegister->data)) {

        // Failed the brake power activation detected
        brake_alarm = true;
        Debug::WriteLine("BodyMotor: Failed to unlock");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT, false);

        // Clear the OUTPUTS
        blocking_writeOD(OD_60FE_01, 0);
        return MotorCompletedCodes::ERROR_BRAKE_DEVICE;
    }

    // OK. Brake device correctly powered
    return MotorCompletedCodes::COMMAND_PROCEED;
}

CanOpenMotor::MotorCompletedCodes BodyMotor::automaticHomingPreparationCallback(void) {
    return automaticPositioningPreparationCallback();
}

CanOpenMotor::MotorCompletedCodes BodyMotor::manualPositioningPreparationCallback(void) {
    return automaticPositioningPreparationCallback();
}


/// <summary>
/// The BodyMotor class override this function in order to update the current position
/// and the command termination
/// 
/// </summary>
/// <param name=LABEL_ERROR></param>
void BodyMotor::automaticPositioningCompletedCallback(MotorCompletedCodes error) {
    
    // Lock the brake device
    // The control of the brake status is done in the IDLE status
    blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (device->home_initialized) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_BODY, MotorConfig::PARAM_CURRENT_POSITION, device->current_eposition.ToString());
        MotorConfig::Configuration->storeFile();
    }

    // Notify the command termination event
    device->command_completed_event(command_id, (int) error);

    return;
}

void BodyMotor::manualPositioningCompletedCallback(MotorCompletedCodes error) {

    // Lock the brake device
    // The control of the brake status is done in the IDLE status
    blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Notify the command termination event
    device->command_completed_event((int) 0, (int)error);

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

    // Lock the brake device
    // The control of the brake status is done in the IDLE status
    blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (device->home_initialized) {
        // Set the position in the configuration file and clear the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_BODY, MotorConfig::PARAM_CURRENT_POSITION, device->current_eposition.ToString());
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

    // Handle the limit switches

    // Handle the safety

    // handle the manual hardware inputs
    PCB301::body_activation_options body_request = PCB301::getBodyActivationStatus();
    if (body_request == PCB301::body_activation_options::BODY_NO_ACTIVATION) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    else if ((manual_cw_direction) && (body_request == PCB301::body_activation_options::BODY_CCW_ACTIVATION)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    else if ((!manual_cw_direction) && (body_request == PCB301::body_activation_options::BODY_CW_ACTIVATION)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;

    // Proceeds with the manual activation
    return MotorCompletedCodes::COMMAND_PROCEED;
}