#include "CanDriver.h"
#include "Notify.h"
#include "CanOpenMotor.h"
#include "gantry_global_status.h"
#include "pd4_od.h"
#include <thread>
#include "Log.h"
#include "ConfigurationFiles.h"

bool CanOpenMotor::activateExternalHoming(int current_uposition) {

    // Thi is a valid command for the external sensor
    if (!external_position_mode) {
        command_completed_code = MotorCompletedCodes::ERROR_INVALID_COMMAND;
        return false;
    }

    // Command already in execution
    if (!isReady()) {
        command_completed_code = MotorCompletedCodes::ERROR_MOTOR_BUSY;
        return false;
    }

    command_target = current_uposition;
    request_command = MotorCommands::MOTOR_EXTERNAL_HOMING;
    return true;
}

bool CanOpenMotor::activateManualHoming(int current_uposition) {

    // Command already in execution
    if (!isReady()) {
        command_completed_code = MotorCompletedCodes::ERROR_MOTOR_BUSY;
        return false;
    }

    command_target = current_uposition;
    request_command = MotorCommands::MOTOR_MANUAL_HOMING;
    return true;
}


bool CanOpenMotor::activateAutomaticHoming(int method_on, int method_off, int speed, int acc) {
    
    // In simulation mode only manual mode is possible
    if (simulator_mode) {
        return activateManualHoming(0);
    }

    // Command not valid for the external position mode
    if (external_position_mode) {
        command_completed_code = MotorCompletedCodes::ERROR_INVALID_COMMAND;
        return false;
    }

    // If the safety condition prevent the command execution it is immediatelly aborted
    Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device_id);
    if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
        LogClass::logInFile("Motor <" + device_id.ToString() + ">: safety condition error > " + safety.ToString());
        command_completed_code = MotorCompletedCodes::ERROR_SAFETY;
        return false;
    }

    // Command already in execution
    if (!isReady()) {
       command_completed_code = MotorCompletedCodes::ERROR_MOTOR_BUSY;
        return false;
    }
   
    command_homing_on_method = method_on;
    command_homing_off_method = method_off;
    command_acc = acc;
    command_speed = speed;
    request_command = MotorCommands::MOTOR_AUTO_HOMING;
    return true;
}

#define ZERO_INPUT_MASK(x) (x & 0x00040000) //!< Not in the Special region [00II][0000]
void CanOpenMotor::manageAutomaticHoming(void) {
    bool error_condition;
    MotorCompletedCodes termination_code;

    home_initialized = false;
    
    // Reset the position in the configuration file and reactivate the alarm
    MotorConfig::Configuration->setParam(config_param, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
    MotorConfig::Configuration->storeFile();
    Notify::activate(error_homing);
    
    // Sets the Speed activation
    error_condition = false;

    // Read the inputs before to proceed
    bool current_homing_input = false;
    if (blocking_readOD(OD_60FD_00)) {
        if (ZERO_INPUT_MASK(rxSdoRegister->data)) current_homing_input = true;
    }

    // Sets the Speed activation: if the callback returns false, the speed is set here internally with the predefined parameters
    // This is useful for critical activations where the speeds and ramps can changes following the activation position and direction.
    // The function should change the command_speed, command_acc and command_dec
    motionParameterCallback(MotorCommands::MOTOR_AUTO_HOMING, encoder_uposition, 0);

    if (!blocking_writeOD(OD_6099_01, convert_UserSec_To_Speed(command_speed))) error_condition = true; // Homing Speed to switch
    if (!blocking_writeOD(OD_6099_02, convert_UserSec_To_Speed(command_speed))) error_condition = true; // Homing Speed to reference
    if (!blocking_writeOD(OD_609A_00, convert_UserSec_To_Speed(command_acc))) error_condition = true; // Sets the Rotation Acc    
    

    // Read the status of the input zero to determines witch algorithm shall be used
    blocking_readOD(OD_60FD_00);
    unsigned char homing;
    if (ZERO_INPUT_MASK(rxSdoRegister->data)) homing = command_homing_on_method;
    else homing = command_homing_off_method;
    if (!blocking_writeOD(OD_6098_00, homing)) error_condition = true; // Write the Homing method

    if (!blocking_writeOD(OD_6060_00, OD_6060_00_PROFILE_HOMING)) error_condition = true; // Write the operating mode to HOMING   
    if (!blocking_writeOD(OD_607C_00, 0)) error_condition = true; // Write the Position offset register 


    if (error_condition) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN AUTO HOMING PREPARATION");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }


    // Tries to activate the Operation Enabled      
    if (!CiA402_activateOperationEnable()) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN AUTO HOMING OPERATION ENABLE SETTING");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }
    

    // Allows the application to prepare for the motor activation
    MotorCompletedCodes preparation_error = preparationCallback(MotorCommands::MOTOR_AUTO_HOMING, encoder_uposition, 0);
    if (preparation_error != MotorCompletedCodes::COMMAND_PROCEED) {
        setCommandCompletedCode(preparation_error);
        return;
    }

    // Update the previous position
    previous_uposition = encoder_uposition;

    // Unbrake 
    if (!unbrakeCallback()) {
        brakeCallback();
        setCommandCompletedCode(MotorCompletedCodes::ERROR_BRAKE_DEVICE);
        return;
    }

    // Set the start bit (BIT4) in the control word register
    if (!writeControlWord(0x10, 0x10)) {
        brakeCallback();
        setCommandCompletedCode(MotorCompletedCodes::ERROR_ACCESS_REGISTER);
        return;
    }

    // Measuring the actuation time
    typedef std::chrono::high_resolution_clock clock;
    clock::time_point start = clock::now();

    command_ms_tmo = 60000; // Sets the timout to 60 seconds for all devices
    LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">:  HOMING INIT POSITION = " + encoder_uposition.ToString());

    while (true) {

        // Test the abort request flag
        if (abort_request) {
            abort_request = false;

            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ABORT REQUEST!");
            termination_code = MotorCompletedCodes::ERROR_COMMAND_ABORTED;
            break;
        }

        // Reads the status to be sure that it is still Operation Enabled 
        if (!blocking_readOD(OD_6041_00)) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING ERROR READING STATUS REGISTER");
            termination_code = MotorCompletedCodes::ERROR_ACCESS_REGISTER;
            break;
        }

        unsigned short statw = rxSdoRegister->data;

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING ERROR POSSIBLE FAULT");
            termination_code = MotorCompletedCodes::ERROR_INTERNAL_FAULT;
            break;
        }

        // Test the Timeout condition
        if (command_ms_tmo <= 0) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING TIMEOUT ERROR");
            termination_code = MotorCompletedCodes::ERROR_TIMOUT;
            break;
        }

        MotorCompletedCodes termination_condition = runningCallback(MotorCommands::MOTOR_AUTO_HOMING, encoder_uposition, 0);
        if (termination_condition >= MotorCompletedCodes::MOTOR_ERRORS) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: Application terminated early for error detected");
            termination_code = termination_condition;
            break;
        }
        

        // If the safety condition prevent the command execution it is immediatelly aborted
        Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device_id);
        if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
            LogClass::logInFile("Motor <" + device_id.ToString() + ">: safety condition error > " + safety.ToString());
            termination_code = MotorCompletedCodes::ERROR_SAFETY;
            break;
        }

        unsigned short zero_stat = statw & 0x3400;

        bool completed = false;
        bool error = false;

        switch (zero_stat) {

        case 0x1000: // CONFIRMED
        case 0: // PERFORMED
            break;

        case 0x2400: // ERROR IDLE
        case 0x2000: // ERROR
        case 0x0400: // INTERRUPTED
            error = true;
            break;


        case 0x1400: // COMPLETED
            completed = true;
            break;


        }

        if (completed) {

            // Calculates the effective duration time
            typedef std::chrono::milliseconds milliseconds;
            milliseconds ms = std::chrono::duration_cast<milliseconds>(clock::now() - start);
            home_initialized = true;
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING SUCCESSFULLY COMPLETED");
            termination_code = MotorCompletedCodes::COMMAND_SUCCESS;         
            break;
        }

        if (error) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING ERROR  FAULT");
            termination_code = MotorCompletedCodes::ERROR_INTERNAL_FAULT;
            break;

        }

        command_ms_tmo -= 50;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    } // End of main controlling loop


    // Read the current position 
    updateCurrentPosition();
    LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">:  HOMING FINAL POSITION = " + encoder_uposition.ToString());

    // resets the OMS bit of the control word
    writeControlWord(0x0270, 0);

    // Read the status word
    while (true) {
        if (!blocking_readOD(OD_6041_00)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
        if (getCiAStatus(rxSdoRegister->data) != _CiA402Status::CiA402_QuickStopActive) break;
    }

    // Activates the brakes
    brakeCallback();

    // set the cia Switched On status
    writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);

    if (home_initialized) {
        // Set the position in the configuration file and clear the alarm
        MotorConfig::Configuration->setParam(config_param, MotorConfig::PARAM_CURRENT_POSITION, getCurrentUposition().ToString());
        MotorConfig::Configuration->storeFile();
        Notify::deactivate(error_homing);
    }

    setCommandCompletedCode(termination_code);
    return;

}

void CanOpenMotor::manageManualHoming(int zero_position) {

    int initial_eposition = convert_User_To_Encoder(zero_position);

    home_initialized = false;
    
    // Reset the position in the configuration file and reactivate the alarm
    MotorConfig::Configuration->setParam(config_param, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
    MotorConfig::Configuration->storeFile();
    Notify::activate(error_homing);

    if (!setEncoderCommand(initial_eposition)) {
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }

    home_initialized = true;
    
    // Set the position in the configuration file and clear the alarm
    MotorConfig::Configuration->setParam(config_param, MotorConfig::PARAM_CURRENT_POSITION, getCurrentUposition().ToString());
    MotorConfig::Configuration->storeFile();
    Notify::deactivate(error_homing);

    setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);
    return ;

}

void CanOpenMotor::manageExternalHoming(int zero_position) {
    
    home_initialized = false;
    
    // Reset the position in the configuration file and reactivate the alarm
    MotorConfig::Configuration->setParam(config_param, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
    MotorConfig::Configuration->setParam(config_param, MotorConfig::PARAM_EXTERNAL_POSITION, MotorConfig::MOTOR_EXTERNAL_UNDEFINED_POSITION);
    MotorConfig::Configuration->storeFile();
    Notify::activate(error_homing);

    // Potentiometer position acquisition
    if (!blocking_readOD(OD_3220_01)) {
        setCommandCompletedCode(MotorCompletedCodes::ERROR_ACCESS_REGISTER);
        return;
    }
    external_raw_position = (unsigned short)getRxReg()->data;

    // Calcs the equivalent zero position
    float f = (float) zero_position / external_k_coeff;
    external_zero_setting = (unsigned short)((float) external_raw_position - f);
    
    update_external_position();
    int initial_eposition = convert_User_To_Encoder(external_uposition);

    // Stores the zero position in the configuration file    
    MotorConfig::Configuration->setParam(config_param, MotorConfig::PARAM_EXTERNAL_POSITION, external_zero_setting.ToString());

    if (!setEncoderCommand(initial_eposition)) {
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }

    // Set the position in the configuration file and clear the alarm
    MotorConfig::Configuration->setParam(config_param, MotorConfig::PARAM_CURRENT_POSITION, getCurrentUposition().ToString());
    MotorConfig::Configuration->storeFile();
    Notify::deactivate(error_homing);
    home_initialized = true;

    // Command completed
    setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);
    return;
}



/// <summary>
/// This function reset the Encoder internal position to a predefined value.
/// </summary>
/// 
/// + The function sets the  variable "home_initialized" to TRUE in case of success;
/// + The function sets the  variable "home_initialized" to FALSE in case of failure;
/// 
/// NOTE: the function returns FALSE only if a communication error happen.
/// 
/// <param name=""></param>
/// <returns>true: no communcation errors</returns>
bool CanOpenMotor::setEncoderCommand(int initial_eposition) {
    

    // Already reset in this position
    updateCurrentPosition();
    if (encoder_eposition == initial_eposition) return true;
   
    // Write the Homing method to 35: current position method
    if (!blocking_writeOD(OD_6098_00, 35)) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: setEncoderCommand() - ERROR WRITING IDX 6098");
        return false;
    }

    // Write the Position offset register
    if (!blocking_writeOD(OD_607C_00, initial_eposition)) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: setEncoderCommand() - ERROR WRITING IDX 607C");
        return false;
    }

    // Write the operating mode to HOMING
    if (!blocking_writeOD(OD_6060_00, OD_6060_00_PROFILE_HOMING)) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: setEncoderCommand() - ERROR WRITING IDX 6060 TO HOMING");
        return false;
    }

    // Set the start bit (BIT4) in the control word register
    if (!writeControlWord(0x10, 0x10)) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: setEncoderCommand() - ERROR WRITING CW BIT 4");
        return false;
    }

    if(simulator_mode)  std::this_thread::sleep_for(std::chrono::milliseconds(500));
    else  std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Clear the start bit (BIT4) in the control word register
    if (!writeControlWord(0x10, 0)) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: setEncoderCommand() -  ERROR CLEARING CW BIT 4");
        return false;
    }

    // Write the operating mode to PROFILE POSITIONING
    if (!blocking_writeOD(OD_6060_00, OD_6060_00_PROFILE_POSITIONING)) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: setEncoderCommand() - ERROR WRITING IDX 6060 TO POSITIONING");
        return false;
    }

    updateCurrentPosition();

    LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: encoder_requested = " + convert_Encoder_To_User(initial_eposition).ToString() + "Position = " + encoder_uposition.ToString());
    if (encoder_eposition != initial_eposition) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: setEncoderCommand() - ENCODER NOT IN THE INITIAL ASSIGNED VALUE");
        return false;
    }

    LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: setEncoderCommand() -  ENCODER INITIALIZATION SUCCESS, Position = " + encoder_uposition.ToString());
    return true;

}


