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

   
    // Position Range Limit
    if (!blocking_writeOD(OD_607B_01, 0x80000000)) error_condition = true; // Min Position Range Limit 
    if (!blocking_writeOD(OD_607B_02, 0x7FFFFFFF)) error_condition = true; // Max Position Range Limit
    if (!blocking_writeOD(OD_607D_01, 0x80000000)) error_condition = true; // Min Position  Limit 
    if (!blocking_writeOD(OD_607D_02, 0x7FFFFFFF)) error_condition = true; // Max Position  Limit


    // Sets the Speed activation
    error_condition = false;
    if (!blocking_writeOD(OD_6081_00, convert_UserSec_To_Speed(command_speed))) error_condition = true; // Sets the Rotation speed 
    if (!blocking_writeOD(OD_6083_00, 0)) error_condition = true; // Sets the Rotation Acc
    if (!blocking_writeOD(OD_6084_00, 0)) error_condition = true; // Sets the Rotation Dec
    if (!blocking_writeOD(OD_607A_00, command_target)) error_condition = true; // Set the target position
    if (!blocking_writeOD(OD_6060_00, 1)) error_condition = true;// Set the Activation mode to Profile Positioning Mode
    if (!writeControlWord(POSITION_SETTING_CTRL_INIT_MASK, POSITION_SETTING_CTRL_INIT_VAL)) error_condition = true;// Sets the Position control bits in the control word
    if (error_condition) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: manageManualServicePositioning() - ERROR IN MANUAL POSITIONING PREPARATION");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);                
        return;
    }


    // Tries to activate the Operation Enabled 
    error_condition = true;
    for (int i = 0; i < 5; i++) {

        if (!writeControlWord(OD_6040_00_ENABLEOP_MASK, OD_6040_00_ENABLEOP_VAL)) continue;

        // Read the status word
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (!blocking_readOD(OD_6041_00)) continue;
        if (getCiAStatus(rxSdoRegister->data) == _CiA402Status::CiA402_OperationEnabled) {
            error_condition = false;
            break;
        }

    }

    if (error_condition) {
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

        // Reads the status to be sure that it is still Operation Enabled 
        if (!blocking_readOD(OD_6041_00)) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING ERROR READING STATUS REGISTER");
            termination_code = MotorCompletedCodes::ERROR_ACCESS_REGISTER;
            break;
        }

        unsigned short statw = rxSdoRegister->data;

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING ERROR POSSIBLE FAULT");
            termination_code = MotorCompletedCodes::ERROR_INTERNAL_FAULT;
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


    // Position Range Limit
    error_condition = false;
    if(!blocking_writeOD(OD_607B_01, convert_User_To_Encoder(min_position - 10))) error_condition = true; 	// Min Position Range Limit
    if (!blocking_writeOD(OD_607B_02, convert_User_To_Encoder(max_position + 10))) error_condition = true;	// Max Position Range Limit

    // Software Position Limit
    if (!blocking_writeOD(OD_607D_01, convert_User_To_Encoder(min_position))) error_condition = true; 	// Min Position Limit
    if (!blocking_writeOD(OD_607D_02, convert_User_To_Encoder(max_position))) error_condition = true; 	    // Max PositionLimit

    if (error_condition) return;
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

    // Sets the Speed activation
    error_condition = false;
    if (!blocking_writeOD(OD_6081_00, convert_UserSec_To_Speed(command_speed))) error_condition = true; // Sets the Rotation speed 
    if (!blocking_writeOD(OD_6083_00, convert_UserSec_To_Speed(command_acc))) error_condition = true; // Sets the Rotation Acc
    if (!blocking_writeOD(OD_6084_00, convert_UserSec_To_Speed(command_dec))) error_condition = true; // Sets the Rotation Dec
    if (!blocking_writeOD(OD_607A_00, convert_User_To_Encoder(command_target))) error_condition = true; // Set the target position
    if (!blocking_writeOD(OD_6060_00, 1)) error_condition = true;// Set the Activation mode to Profile Positioning Mode
    if (!writeControlWord(POSITION_SETTING_CTRL_INIT_MASK, POSITION_SETTING_CTRL_INIT_VAL)) error_condition = true;// Sets the Position control bits in the control word
    if (error_condition) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN MANUAL POSITIONING PREPARATION");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }


    // Tries to activate the Operation Enabled 
    error_condition = true;
    for (int i = 0; i < 5; i++) {

        if (!writeControlWord(OD_6040_00_ENABLEOP_MASK, OD_6040_00_ENABLEOP_VAL)) continue;

        // Read the status word
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (!blocking_readOD(OD_6041_00)) continue;
        if (getCiAStatus(rxSdoRegister->data) == _CiA402Status::CiA402_OperationEnabled) {
            error_condition = false;
            break;
        }

    }

    if (error_condition) {
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

        // Reads the status to be sure that it is still Operation Enabled 
        if (!blocking_readOD(OD_6041_00)) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING ERROR READING STATUS REGISTER");
            termination_code = MotorCompletedCodes::ERROR_ACCESS_REGISTER;
            break;
        }

        unsigned short statw = rxSdoRegister->data;

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING ERROR POSSIBLE FAULT");
            termination_code = MotorCompletedCodes::ERROR_INTERNAL_FAULT;            
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
