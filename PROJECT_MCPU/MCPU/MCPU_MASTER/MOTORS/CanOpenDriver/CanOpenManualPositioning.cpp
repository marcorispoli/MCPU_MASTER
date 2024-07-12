#include "CanDriver.h"
#include "Notify.h"
#include "CanOpenMotor.h"
#include "CalibrationConfig.h"
#include "gantry_global_status.h"
#include "pd4_od.h"
#include <thread>
#include "Log.h"

bool CanOpenMotor::activateManualService(bool increase) {

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


    command_id = 0;

    if(increase) command_target = 0x7FFFFFFF;
    else command_target = 0x80000000;
    
    command_speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_MANUAL_SPEED])/2;
    command_acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_MANUAL_ACC]);
    command_dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_MANUAL_DEC]);

    command_stop = false;
    request_command = MotorCommands::MOTOR_MANUAL_SERVICE;
    return true;
}

bool CanOpenMotor::activateManualPositioning(int target, int speed, int acc, int dec) {

    // If the safety condition prevent the command execution it is immediatelly aborted
    Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device_id);
    if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
        LogClass::logInFile("Motor <" + device_id.ToString() + ">: safety condition error > " + safety.ToString());
        command_completed_code = MotorCompletedCodes::ERROR_SAFETY;
        return false;
    }
    
    // Only with the homing executed or initialized can be activated
    if (!home_initialized) {
        command_completed_code = MotorCompletedCodes::ERROR_MISSING_HOME;
        return false;
    }

    // Command already in execution
    if (!isReady()) {
        command_completed_code = MotorCompletedCodes::ERROR_MOTOR_BUSY;
        return false;
    }


    command_id = 0;
    command_target = target;
    command_acc = acc;
    command_dec = dec;
    command_speed = speed;
    command_stop = false;
    request_command = MotorCommands::MOTOR_MANUAL_POSITIONING;
    return true;
}

bool CanOpenMotor::activateManualPositioning(int target) {

    // If the safety condition prevent the command execution it is immediatelly aborted
    Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device_id);
    if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
        LogClass::logInFile("Motor <" + device_id.ToString() + ">: safety condition error > " + safety.ToString());
        command_completed_code = MotorCompletedCodes::ERROR_SAFETY;
        return false;
    }

    // Only with the homing executed or initialized can be activated
    if (!home_initialized) {
        command_completed_code = MotorCompletedCodes::ERROR_MISSING_HOME;
        return false;
    }

    // Command already in execution
    if (!isReady()) {
        command_completed_code = MotorCompletedCodes::ERROR_MOTOR_BUSY;
        return false;
    }


    command_speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_MANUAL_SPEED]);
    command_acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_MANUAL_ACC]);
    command_dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_MANUAL_DEC]);

    command_id = 0;
    command_target = target;
    command_stop = false;
    request_command = MotorCommands::MOTOR_MANUAL_POSITIONING;
    return true;
}


void CanOpenMotor::manageManualServicePositioning(void) {
    bool error_condition;
    MotorCompletedCodes termination_code;

    // repeat the configuration in case of error
    configuration_command = true;

    int i, repeat = 20;
    for (i = repeat; i > 0; i--) {
        
        // With simulation adds 50ms every attempt
        if(simulator_mode) std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // Position Range Limit
        if (!blocking_writeOD(OD_607B_01, 0x80000000)) continue; // Min Position Range Limit 
        if (!blocking_writeOD(OD_607B_02, 0x7FFFFFFF)) continue; // Max Position Range Limit
        if (!blocking_writeOD(OD_607D_01, 0x80000000)) continue; // Min Position  Limit 
        if (!blocking_writeOD(OD_607D_02, 0x7FFFFFFF)) continue; // Max Position  Limit

        if (!blocking_writeOD(OD_6081_00, convert_UserSec_To_Speed(command_speed))) continue; // Sets the Rotation speed 
        if (!blocking_writeOD(OD_6083_00, 0)) continue; // Sets the Rotation Acc
        if (!blocking_writeOD(OD_6084_00, 0)) continue; // Sets the Rotation Dec
        if (!blocking_writeOD(OD_607A_00, command_target)) continue; // Set the target position
        if (!blocking_writeOD(OD_6060_00, 1)) continue;// Set the Activation mode to Profile Positioning Mode

        break;
    }

    if (!i) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: manageManualServicePositioning() - ERROR IN MANUAL POSITIONING PREPARATION");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }
    
    if (!writeControlWord(POSITION_SETTING_CTRL_INIT_MASK, POSITION_SETTING_CTRL_INIT_VAL)){
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: manageManualServicePositioning() - ERROR IN MANUAL POSITIONING PREPARATION");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);                
        return;
    }

    // Tries to activate the Operation Enabled      
    if (!CiA402_activateOperationEnable()) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: manageManualServicePositioning() - ERROR IN MANUAL POSITIONING OPERATION ENABLE SETTING");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }


    // Assignes the current motor direction
    if (command_target == 0x7FFFFFFF) motor_direction = motor_rotation_activations::MOTOR_INCREASE;
    else motor_direction = motor_rotation_activations::MOTOR_DECREASE;

    
    // Unbrake 
    if (!unbrakeCallback()) {
        brakeCallback();
        setCommandCompletedCode(MotorCompletedCodes::ERROR_BRAKE_DEVICE);
        return;
    }

    // Set the start bit (BIT4) in the control word register
    if (!writeControlWord(POSITION_SETTING_START_MASK, POSITION_SETTING_START_VAL)) {
        brakeCallback();
        setCommandCompletedCode(MotorCompletedCodes::ERROR_ACCESS_REGISTER);
        return;
    }

    while (true) {

        // Test the abort request flag
        if (abort_request) {
            abort_request = false;

            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ABORT REQUEST!");
            termination_code = MotorCompletedCodes::ERROR_COMMAND_ABORTED;
            break;
        }

        MotorCompletedCodes termination_condition = MotorCompletedCodes::COMMAND_PROCEED;

        // If the safety condition prevent the command execution it is immediatelly aborted
        Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device_id);
        if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
            LogClass::logInFile("Motor <" + device_id.ToString() + ">: safety condition error > " + safety.ToString());
            termination_condition = MotorCompletedCodes::ERROR_SAFETY;
        }

        // Evaluates the manual termination condition
        if (termination_condition == MotorCompletedCodes::COMMAND_PROCEED) {
            // handle the manual hardware inputs
            motor_rotation_activations manual_req = Gantry::getManualActivationRequestState(device_id);
            if (motor_direction != manual_req) termination_condition = MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
        }

        if (termination_condition >= MotorCompletedCodes::MOTOR_ERRORS) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: Application terminated early for error detected");
            termination_code = termination_condition;
            break;
        }

        // Reads the status to be sure that it is still Operation Enabled 
        unsigned int statw;
        if (!readStatusWord(&statw)) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING ERROR READING STATUS REGISTER");
            termination_code = MotorCompletedCodes::ERROR_ACCESS_REGISTER;
            break;
        }

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING ERROR POSSIBLE FAULT");
            termination_code = MotorCompletedCodes::ERROR_INTERNAL_FAULT;
            break;
        }

        if (((statw & 0x1400) == 0x1400)  || (termination_condition == MotorCompletedCodes::COMMAND_MANUAL_TERMINATION)) {

            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL SERVICE COMPLETED ");
            termination_code = MotorCompletedCodes::COMMAND_SUCCESS;

            // resets the OMS bit of the control word
            writeControlWord(0x0270, 0);

            if (termination_condition == MotorCompletedCodes::COMMAND_MANUAL_TERMINATION) {
                // Quick Stop to slowly stop
                writeControlWord(OD_6040_00_QUICKSTOP_MASK, OD_6040_00_QUICKSTOP_VAL);
            }
            else {
                // Immediate stop set the cia Switched On status
                writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);
            }
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    } // End of main controlling loop


    // resets the OMS bit of the control word
    writeControlWord(0x0270, 0);

    // Waits in case a Quick Stop should be active
    unsigned int statw;
    while (true) {
        if (readStatusWord(&statw)) {
            if (getCiAStatus(statw) != _CiA402Status::CiA402_QuickStopActive) break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // Activates the brakes
    brakeCallback();

    // Read the current position 
    updateCurrentPosition();
    if (external_position_mode) {
        update_external_position();
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: CURRENT ENCODER UPOSITION = " + encoder_uposition.ToString() + ", CURRENT EXTERNAL UPOSITION = " + external_uposition.ToString());
    }
    else {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: CURRENT POSITION = " + encoder_uposition.ToString());
    }


    // set the cia Switched On status
    writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);
    setCommandCompletedCode(termination_code);


    // Position Range Limit reconfiguration
    repeat = 10;
    for (i = repeat; i > 0; i--) {
        if (simulator_mode) std::this_thread::sleep_for(std::chrono::milliseconds(50));

        if (!blocking_writeOD(OD_607B_01, convert_Absolute_User_To_Encoder(min_position - 10))) continue; 	// Min Position Range Limit
        if (!blocking_writeOD(OD_607B_02, convert_Absolute_User_To_Encoder(max_position + 10))) continue;	// Max Position Range Limit

        // Software Position Limit
        if (!blocking_writeOD(OD_607D_01, convert_Absolute_User_To_Encoder(min_position))) continue; 	// Min Position Limit
        if (!blocking_writeOD(OD_607D_02, convert_Absolute_User_To_Encoder(max_position))) continue; 	    // Max PositionLimit
        break;
    }

    if (!i) return;

    // All right: no reconfiguration is requested!
    configuration_command = false;
    return;

}



void CanOpenMotor::manageManualPositioning(void) {
    bool error_condition;
    MotorCompletedCodes termination_code;

    if (external_position_mode) {
        // If this is the external source, befor to proceed initializes the encoder at 
        // the current external position
        update_external_position();
        int init_eposition = convert_User_To_Encoder(external_uposition);

        if (!setEncoderCommand(init_eposition)) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: manageAutomaticPositioning() ERROR IN RESETTING THE ENCODER DURING PREPARATION");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
            return;
        }
    }
    else updateCurrentPosition();
    previous_uposition = encoder_uposition;

    // Test if the actual position is already in target position
    if (isTarget()) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING COMPLETED (ALREADY IN POSITION)");
        setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);
        return;
    }

    // Sets the Speed activation: if the callback returns false, the speed is set here internally with the predefined parameters
    // This is useful for critical activations where the speeds and ramps can changes following the activation position and direction.
    // The function should change the command_speed, command_acc and command_dec
    motionParameterCallback(MotorCommands::MOTOR_MANUAL_POSITIONING, encoder_uposition, command_target);

    int i, repeat = 10;
    for (i = repeat; i > 0; i--)
    {
        if (simulator_mode) std::this_thread::sleep_for(std::chrono::milliseconds(50));

        if (!blocking_writeOD(OD_6081_00, convert_UserSec_To_Speed(command_speed))) continue; // Sets the Rotation speed 
        if (!blocking_writeOD(OD_6083_00, convert_UserSec_To_Speed(command_acc))) continue;   // Sets the Rotation Acc
        if (!blocking_writeOD(OD_6084_00, convert_UserSec_To_Speed(command_dec))) continue;   // Sets the Rotation Dec
        if (!blocking_writeOD(OD_607A_00, convert_User_To_Encoder(command_target))) continue; // Set the target position
        if (!blocking_writeOD(OD_6060_00, 1)) continue;// Set the Activation mode to Profile Positioning Mode
        break;
    }
    if (!i) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN MANUAL POSITIONING PREPARATION");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }

    if (!writeControlWord(POSITION_SETTING_CTRL_INIT_MASK, POSITION_SETTING_CTRL_INIT_VAL)) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN MANUAL POSITIONING PREPARATION");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }

    // Tries to activate the Operation Enabled      
    if (!CiA402_activateOperationEnable()) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN MANUAL POSITIONING OPERATION ENABLE SETTING");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }


    // Assignes the current motor direction
    if (command_target > encoder_uposition) motor_direction = motor_rotation_activations::MOTOR_INCREASE;
    else motor_direction = motor_rotation_activations::MOTOR_DECREASE;

    // Allows the application to prepare for the motor activation
    MotorCompletedCodes preparation_error = preparationCallback(MotorCommands::MOTOR_MANUAL_POSITIONING, encoder_uposition, command_target);
    if (preparation_error != MotorCompletedCodes::COMMAND_PROCEED) {
        setCommandCompletedCode(preparation_error);
        return;
    }

    
    // Invalidate the stored encoder position
    MotorConfig::Configuration->setParam(config_param, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
    MotorConfig::Configuration->storeFile();

    // Update the previous position
    previous_uposition = encoder_uposition;

    // Unbrake 
    if (!unbrakeCallback()) {
        brakeCallback();
        setCommandCompletedCode(MotorCompletedCodes::ERROR_BRAKE_DEVICE);
        return;
    }

    // Set the start bit (BIT4) in the control word register
    if (!writeControlWord(POSITION_SETTING_START_MASK, POSITION_SETTING_START_VAL)) {
        brakeCallback();
        setCommandCompletedCode(MotorCompletedCodes::ERROR_ACCESS_REGISTER);
        return;
    }

    while (true) {

        // Test the abort request flag
        if (abort_request) {
            abort_request = false;

            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ABORT REQUEST!");
            termination_code = MotorCompletedCodes::ERROR_COMMAND_ABORTED;
            break;
        }

        // Read the current position 
        updateCurrentPosition();

        MotorCompletedCodes termination_condition = runningCallback(MotorCommands::MOTOR_MANUAL_POSITIONING, encoder_uposition, command_target);

        // If the safety condition prevent the command execution it is immediatelly aborted
        Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device_id);
        if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
            LogClass::logInFile("Motor <" + device_id.ToString() + ">: safety condition error > " + safety.ToString());
            termination_condition = MotorCompletedCodes::ERROR_SAFETY;
        }

        // Evaluates the manual termination condition
        if (termination_condition == MotorCompletedCodes::COMMAND_PROCEED) {
            // handle the manual hardware inputs
            motor_rotation_activations manual_req = Gantry::getManualActivationRequestState(device_id);
            if(motor_direction != manual_req) termination_condition = MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;         
        }

        if (termination_condition >= MotorCompletedCodes::MOTOR_ERRORS) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: Application terminated early for error detected");
            termination_code = termination_condition;
            break;
        }

        // Reads the status to be sure that it is still Operation Enabled 
        unsigned int statw;
        if (!readStatusWord(&statw)) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING ERROR READING STATUS REGISTER");
            termination_code = MotorCompletedCodes::ERROR_ACCESS_REGISTER;
            break;
        }

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING ERROR POSSIBLE FAULT");
            termination_code = MotorCompletedCodes::ERROR_INTERNAL_FAULT;
            break;
        }
        
        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING ERROR POSSIBLE FAULT");
            termination_code = MotorCompletedCodes::ERROR_INTERNAL_FAULT;
            break;
        }

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING ERROR POSSIBLE FAULT");
            termination_code = MotorCompletedCodes::ERROR_INTERNAL_FAULT;
            break;
        }
       
        if (((statw & 0x1400) == 0x1400) || (encoder_uposition == command_target) || (termination_condition == MotorCompletedCodes::COMMAND_MANUAL_TERMINATION)) {

            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING COMPLETED ");
            termination_code = MotorCompletedCodes::COMMAND_SUCCESS;

            // resets the OMS bit of the control word
            writeControlWord(0x0270, 0);

            if (termination_condition == MotorCompletedCodes::COMMAND_MANUAL_TERMINATION) {
                // Quick Stop to slowly stop
                writeControlWord(OD_6040_00_QUICKSTOP_MASK, OD_6040_00_QUICKSTOP_VAL);
            }
            else {
                // Immediate stop set the cia Switched On status
                writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);
            }
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    } // End of main controlling loop

  
    // resets the OMS bit of the control word
    writeControlWord(0x0270, 0);

    // Waits in case a Quick Stop should be active
    unsigned int statw;
    while (true) {
        if (readStatusWord(&statw)) {
            if (getCiAStatus(statw) != _CiA402Status::CiA402_QuickStopActive) break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    // Activates the brakes
    brakeCallback();

    // Read the current position 
    updateCurrentPosition();

    if (external_position_mode) {
        update_external_position();
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: CURRENT ENCODER UPOSITION = " + encoder_uposition.ToString() + ", CURRENT EXTERNAL UPOSITION = " + external_uposition.ToString());
    }
    else {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: CURRENT POSITION = " + encoder_uposition.ToString());
    }


    // set the cia Switched On status
    writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);

    setCommandCompletedCode(termination_code);
    return;

}
