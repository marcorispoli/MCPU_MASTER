#include "CalibrationConfig.h"
#include "Notify.h"
#include "BodyMotor.h"
#include "CanOpenMotor.h"
#include "PCB301.h"
#include "pd4_od.h"
#include "..\gantry_global_status.h"
#include <thread>
#include "Log.h"




#define ROT_PER_DEGREE ((double) 80 / (double) 3600) //!< Defines the position user units in 0.1 degree
#define EXTERNAL_K ((double) 2.929) //!< Defines the external pot resolution per 0.1 degree

#define BRAKE_INPUT_MASK(x) (x & PD4_MOTOR_DI4) //!< Not in the Special region [00II][0000]

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

BodyMotor::BodyMotor(void): CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::BODY_ID, L"MOTOR_BODY", MotorConfig::PARAM_BODY, Notify::messages::ERROR_BODY_MOTOR_HOMING, MIN_ROTATION_ANGLE, MAX_ROTATION_ANGLE, ROT_PER_DEGREE, EXTERNAL_K, false)
{
    // Sets +/- 0.2 ° as the acceptable target range
    setTargetRange(2, 2);
   
}

bool BodyMotor::serviceAutoPosition(int pos) {

    return device->activateAutomaticPositioning(0, pos * 10, true);

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
#define BODY_OD_CODE 0x0001
unsigned short BodyMotor::initializeSpecificObjectDictionaryCallback(void) {
    
    // Motor Drive Parameter Set
    while (!blocking_writeOD(OD_3210_01, 10000)); // 50000 Position Loop, Proportional Gain (closed Loop)
    while (!blocking_writeOD(OD_3210_02, 5));	 // 10  Position Loop, Integral Gain (closed Loop)

    // Position Range Limit
    while (!blocking_writeOD(OD_607B_01, convert_User_To_Encoder(MIN_ROTATION_ANGLE - 50))); 	// Min Position Range Limit
    while (!blocking_writeOD(OD_607B_02, convert_User_To_Encoder(MAX_ROTATION_ANGLE + 50)));	// Max Position Range Limit

    // Software Position Limit
    if (!blocking_writeOD(OD_607D_01, convert_User_To_Encoder(MIN_ROTATION_ANGLE))) return 0;	// Min Position Limit
    if (!blocking_writeOD(OD_607D_02, convert_User_To_Encoder(MAX_ROTATION_ANGLE))) return 0;	// Max Position Limit

    // Sets the Output setting
    if (!blocking_writeOD(OD_3250_02, 0)) return 0; // Output control not inverted
    if (!blocking_writeOD(OD_3250_03, 0)) return 0; // Force Enable = false
    if (!blocking_writeOD(OD_3250_08, 0)) return 0; // Routing Enable = false
    if (!blocking_writeOD(OD_60FE_01, 0)) return 0; // Set All outputs to 0

    // Set the input setting
    if (!blocking_writeOD(OD_3240_01, 0x4)) return 0; // Input control special: I3 = HOMING
    if (!blocking_writeOD(OD_3240_02, 0)) return 0;   // Function Inverted: not inverted
    if (!blocking_writeOD(OD_3240_03, 0)) return 0;   // Force Enable = false
    if (!blocking_writeOD(OD_3240_06, 0)) return 0;   // Input Range Select: threshold = 5V;

    // Setup the Analog Input
    while (!blocking_writeOD(OD_3221_00, 0));     // 0 , Voltage, 1, Current


    // Writes 0s of both outputs    
    if (!blocking_writeOD(OD_60FE_01, 0)) return 0;

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
        LogClass::logInFile("BodyMotor: Failed test output off, off");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT);
        
        // Clear the OUTPUTS
        blocking_writeOD(OD_60FE_01, 0);
        return BODY_OD_CODE;
    }

    if (!activateBrake()) {
        LogClass::logInFile("BodyMotor: Failed test output on, on");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT);
        return BODY_OD_CODE;
    }

    if (!deactivateBrake()) {
        LogClass::logInFile("BodyMotor: Failed test output off, on");
        Notify::activate(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT);        
    }

   

    return BODY_OD_CODE;
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


    return ret_code;
    
}

BodyMotor::MotorCompletedCodes BodyMotor::runningCallback(MotorCommands current_command, int current_position, int target_position) {
    MotorCompletedCodes ret_code = MotorCompletedCodes::COMMAND_PROCEED;
    return ret_code;

    
}



/// <summary>
/// This function activates the External Sensor calibration Homing procedure
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool BodyMotor::startAutoHoming(void) {

    // Gets the Speed and Acceleration from the configuration file
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_HOME_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_BODY)[MotorConfig::PARAM_HOME_ACC]);
    return device->activateAutomaticHoming(HOMING_ON_METHOD, HOMING_OFF_METHOD, speed, acc);

    
}

bool BodyMotor::startManualHoming(int target_position) {
    if(device->isPositionFromExternalSensor()) return device->activateExternalHoming(target_position);
    else return device->activateManualHoming(target_position);
}

bool BodyMotor::brakeCallback(void){
    deactivateBrake();
    return true;
}

bool BodyMotor::unbrakeCallback(void) {

    // Unlock the Brake device
    if (!activateBrake()) {
        LogClass::logInFile("BodyMotor: Activation failed to unlock the brake device");
        return false;
    }

    return true;
}

void BodyMotor::faultCallback(bool errstat, bool data_changed, unsigned int error_class, unsigned int error_code) {

    if (errstat == false) {
        Notify::deactivate(Notify::messages::INFO_BODY_DRIVER);
        return;
    }


    if (data_changed) {
        //System::String^ driver_error = "CL:" + error_class.ToString() + " CD:" + error_code.ToString();
        System::String^ driver_error = CanOpenMotor::getErrorCode1003(error_code);
        Notify::activate(Notify::messages::INFO_BODY_DRIVER, driver_error);
    }


}
