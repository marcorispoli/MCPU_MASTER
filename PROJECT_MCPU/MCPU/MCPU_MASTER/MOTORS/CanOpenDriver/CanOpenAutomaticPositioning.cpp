#include "CanDriver.h"
#include "Notify.h"
#include "CanOpenMotor.h"
#include "ConfigurationFiles.h"
#include "gantry_global_status.h"
#include "pd4_od.h"
#include <thread>
#include "Log.h"



/// <summary>
/// This is the API function to initiate an Automatic positioning.
/// </summary>
/// 
/// The Automatic positioning is a command to move the motor 
/// from the current position to a target defined position.
/// 
/// The following conditions shall be true in order to execute the command:
/// + the motor shall be in Ready status condition (use isReady() to check it);
/// + the encoder shall be initialized (use isEncoderInitialized()) 
/// 
/// The command return true if it can be executed. 
/// In case it should return false:
/// + use getCommandCompletedCode() to get the error reason;
///  
/// The Application (or the subclass) can monitor the command execution status:
/// + polling the running status with the isReady() function;
/// + handling the command_completed_event() callback;
/// 
/// The Subclass may override the automaticPositioningCompletedCallback()
/// in order to handling differently the command termination event.
/// 
/// The command execution is based on thre different Steps:
/// + Command Preparation: the motor driver is in a non powered state;
/// + Command Execution: the motor is powered and moving;
/// + Command Termination: the activation is terminated (successfully or with error).
/// 
/// The Subclass can implement specific actions that may be executed in those steps,\n
/// overriding the following functions:
/// + automaticPositioningPreparationCallback(): this is called during the preparation fase;
/// + automaticPositioningRunningCallback(): this is called during the running phase;
/// + automaticPositioningCompletedCallback(): this is called after the motor has been stopped;
/// 
/// 
/// <param name="id">This is the ID code assigned by the application</param>
/// <param name="target">This is the target position in Application units</param>
/// <param name="speed">This is the speed in the Application units</param>
/// <param name="acc">This is the Acceleration rate in Application units</param>
/// <param name="dec">This is the Deceleration rate in Application units</param>
/// <param name="autostart">set to true to automatically start the activation (bit4 of control word) </param>
/// <returns>true if the command can be executed</returns>
bool CanOpenMotor::activateAutomaticPositioning(int id, int target, int speed, int acc, int dec, bool autostart) {

    // Test the target range
    if ((target < min_position) || (target > max_position)) {
        LogClass::logInFile("Motor <" + device_id.ToString() + "> error: target out of range > " );
        command_completed_code = MotorCompletedCodes::ERROR_TARGET_OUT_OF_RANGE;
        return false;
    }

    // If the safety condition prevent the command execution it is immediatelly aborted
    Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device_id);
    if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
        LogClass::logInFile("Motor <" + device_id.ToString() + ">: safety condition error > " + safety.ToString());
        command_completed_code = MotorCompletedCodes::ERROR_SAFETY;
        return false;
    }
    
    // Only withthe homing executed or initialized can be activated
    if (!home_initialized) {
        command_completed_code = MotorCompletedCodes::ERROR_MISSING_HOME;
        return false;
    }

    // Command already in execution
    if (!isReady()) {
        command_completed_code = MotorCompletedCodes::ERROR_MOTOR_BUSY;
        return false;
    }

    command_id = id;
    command_target = target;
    command_acc = acc;
    command_dec = dec;
    command_speed = speed;
    request_command = MotorCommands::MOTOR_AUTO_POSITIONING;
    autostart_mode = autostart;

    return true;
}

bool CanOpenMotor::activateAutomaticPositioning(int id, int target, bool autostart) {

    // Test the target range
    if ((target < min_position) || (target > max_position)) {
        LogClass::logInFile("Motor <" + device_id.ToString() + "> error: target out of range > ");
        command_completed_code = MotorCompletedCodes::ERROR_TARGET_OUT_OF_RANGE;
        return false;
    }

    // If the safety condition prevent the command execution it is immediatelly aborted
    Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device_id);
    if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
        LogClass::logInFile("Motor <" + device_id.ToString() + ">: safety condition error > " + safety.ToString());
        command_completed_code = MotorCompletedCodes::ERROR_SAFETY;
        return false;
    }

    // Only withthe homing executed or initialized can be activated
    if (!home_initialized) {
        command_completed_code = MotorCompletedCodes::ERROR_MISSING_HOME;
        return false;
    }

    // Command already in execution
    if (!isReady()) {
        command_completed_code = MotorCompletedCodes::ERROR_MOTOR_BUSY;
        return false;
    }

    command_speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_AUTO_SPEED]);
    command_acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_AUTO_ACC]);
    command_dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_AUTO_DEC]);

    command_id = id;
    command_target = target;    
    request_command = MotorCommands::MOTOR_AUTO_POSITIONING;
    autostart_mode = autostart;
    return true;
}


bool  CanOpenMotor::activateRelativePositioning(int id, int delta_target, int speed, int acc, int dec) {
    
    // Set the taret position
    int target = getCurrentPosition() + delta_target;    
    return activateAutomaticPositioning(id, target, speed, acc, dec, true);
}

bool  CanOpenMotor::activateRelativePositioning(int id, int delta_target) {

    // Set the taret position
    int target = getCurrentPosition() + delta_target;
    return activateAutomaticPositioning(id, target, true);
}


/// <summary>
/// This function executes the Automatic positioning of the motor at the expected target position.
/// 
/// The procedure executes the following steps for preparation:
/// - sets the expected timeout in ms;
/// - verifies if the current position should be already in target (termines immediatelly);
/// - upload the running parameters: speed, acceleration deceleration;
/// - Activate the Power to the motor wires;
/// - Calls the automaticPositioningPreparationCallback();
/// - stores the encoder current_position (before to move) in the previous_position variable;
/// 
/// The procedure executes the following steps for the running management:
/// 
/// - updates the current position;
/// - checks for the Abort request (from the application);
/// - verifies if the current CiA status should be changed: in this case termines with error;
/// - verifies if the activation timeout should expires. In this case termines with error;
/// - calls the automaticPositioningRunningCallback() for the subclass management;
/// - verifies if the target is detected in three possible ways:
///     - The motor signals automatically the target (usually it should);
///     - The current position matches with the target position (less probable);
///     - in case of timeout if the position should be in the acceptable range (recovery strategy);
/// 
/// The procedure executes the following steps for the command termination:
/// 
/// - updates the current position;
/// - calls the automaticPositioningCompletedCallback() for the subclass management;
/// - exits form the Powered mode to the Idle status (if not in CiA-Fault);
/// 
/// 
/// </summary>
/// <param name=""></param>
void CanOpenMotor::manageAutomaticPositioning(void) {
    MotorCompletedCodes termination_code;
    bool error_condition;
    bool motor_started = false;
    unsigned int ctrlw;

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
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING COMPLETED (ALREADY IN POSITION)");        
        setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);
        return;
    }
    
   
    // Sets the Speed activation: if the callback returns false, the speed is set here internally with the predefined parameters
    // This is useful for critical activations where the speeds and ramps can changes following the activation position and direction.
    // The function should change the command_speed, command_acc and command_dec
    motionParameterCallback(MotorCommands::MOTOR_AUTO_POSITIONING, encoder_uposition, command_target);

    // Gets the extimation about the transit time
    command_ms_tmo = getActivationTimeout(command_speed, command_acc, command_dec, command_target);

    LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) +
        "> INIT AUTO POSITIONING: From " + System::Convert::ToString(encoder_uposition) +
        " To " + System::Convert::ToString(command_target) +
        " Expected In " + System::Convert::ToString((int)((double)command_ms_tmo)) + " (ms)");


    // Assignes the Positioning parameters
    error_condition = false;
    if (!blocking_writeOD(OD_6081_00, convert_UserSec_To_Speed(command_speed))) error_condition = true; // Sets the Rotation speed 
    if (!blocking_writeOD(OD_6083_00, convert_UserSec_To_Speed(command_acc))) error_condition = true; // Sets the Rotation Acc
    if (!blocking_writeOD(OD_6084_00, convert_UserSec_To_Speed(command_dec))) error_condition = true; // Sets the Rotation Dec
    if (!blocking_writeOD(OD_607A_00, convert_User_To_Encoder(command_target))) error_condition = true; // Set the target position
    if (!blocking_writeOD(OD_6060_00, 1)) error_condition = true;// Set the Activation mode to Profile Positioning Mode

    // Select the positioning activationn mode
    if (!writeControlWord(POSITION_SETTING_CTRL_INIT_MASK, POSITION_SETTING_CTRL_INIT_VAL)) error_condition = true;// Sets the Position control bits in the control word

    if (error_condition) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN AUTO POSITIONING PREPARATION");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }

    // Tries to activate the Operation Enabled
    if (!CiA402_activateOperationEnable()) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN AUTO POSITIONING OPERATION ENABLE SETTING");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }

    // Assignes the current motor direction
    if (command_target > encoder_uposition) motor_direction = motor_rotation_activations::MOTOR_INCREASE;
    else motor_direction = motor_rotation_activations::MOTOR_DECREASE;
    
    // Allows the application to prepare for the motor activation
    MotorCompletedCodes preparation_error = preparationCallback(MotorCommands::MOTOR_AUTO_POSITIONING, encoder_uposition, command_target);
    if (preparation_error != MotorCompletedCodes::COMMAND_PROCEED) {
        setCommandCompletedCode(preparation_error);
        return;
    }

    // Invalidate the stored encoder position
    MotorConfig::Configuration->setParam(config_param, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
    MotorConfig::Configuration->storeFile();
    
    // Unbrake 
    if (!unbrakeCallback()) {
        brakeCallback();
        setCommandCompletedCode(MotorCompletedCodes::ERROR_BRAKE_DEVICE);
        return;
    }
    
    // Set the start bit (BIT4) in the control word register for autostart mode
    if (autostart_mode) {
        if(!startRotation()){
            brakeCallback();
            setCommandCompletedCode(MotorCompletedCodes::ERROR_ACCESS_REGISTER);
            return;
        }
    }
    
    // Every 1000 ms checks the distance from the target
    command_ms_tmo = 5000;
    bool timeout = false;
    int delta_target = abs(encoder_uposition - command_target);
    
    // Measuring the actuation time
    typedef std::chrono::high_resolution_clock clock;
    clock::time_point start ;

    while (true) {
        // Read the current position 
        updateCurrentPosition();

        if (!motor_started) {
            command_ms_tmo = 5000;
            delta_target = abs(encoder_uposition - command_target);
            timeout = false;

            if (readControlWord(&ctrlw)) {
                if (ctrlw & 0x0010) {
                    motor_started = true;
                    start = clock::now();      
                    LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ROTATION STARTED!");
                }
           }
        }
        else {
            if (command_ms_tmo <= 0) {
                if (abs(encoder_uposition - command_target) >= delta_target) timeout = true;
                else {
                    delta_target = abs(encoder_uposition - command_target);
                    command_ms_tmo = 5000;
                }
            }
        }


        // Test the abort request flag
        if (abort_request) {
            abort_request = false;

            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ABORT REQUEST!");
            termination_code = MotorCompletedCodes::ERROR_COMMAND_ABORTED;
            break;
        }

        // Test the Timeout condition
        if ((timeout) && (!isTarget())) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING TIMEOUT ERROR");
            termination_code = MotorCompletedCodes::ERROR_TIMOUT;
            break;
        }

        MotorCompletedCodes termination_condition = runningCallback(MotorCommands::MOTOR_AUTO_POSITIONING, encoder_uposition, command_target);
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

        // Gets the obstacle condition
        if (Gantry::getObstacleRotationStatus(device_id, motor_direction)) {
            LogClass::logInFile("Motor <" + device_id.ToString() + ">: obstacle condition error" );
            termination_code = MotorCompletedCodes::ERROR_OBSTACLE_DETECTED;
            break;
        }


        // Reads the status to be sure that it is still Operation Enabled 
        unsigned int statw;
        if (!readStatusWord(&statw)) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING ERROR READING STATUS REGISTER");
            termination_code = MotorCompletedCodes::ERROR_ACCESS_REGISTER;
            break;
        }

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING ERROR POSSIBLE FAULT");
            termination_code = MotorCompletedCodes::ERROR_INTERNAL_FAULT;
            break;
        }

        
        if (((statw & 0x1400) == 0x1400) || (encoder_uposition == command_target) || (timeout) || (termination_condition == MotorCompletedCodes::COMMAND_MANUAL_TERMINATION)) {

            // Calculates the effective duration time
            typedef std::chrono::milliseconds milliseconds;
            milliseconds ms = std::chrono::duration_cast<milliseconds>(clock::now() - start);

            if (timeout) {
                LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING COMPLETED (TMO) - Time =  " + System::Convert::ToString(ms.count()) + " (ms)");
            }
            else if (termination_condition == MotorCompletedCodes::COMMAND_MANUAL_TERMINATION) {
                LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING COMPLETED (MANUAl) - Time =  " + System::Convert::ToString(ms.count()) + " (ms)");
            }
            else {
                LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSIITONING COMPLETED actually In =  " + System::Convert::ToString(ms.count()) + " (ms)");
            }

            termination_code = MotorCompletedCodes::COMMAND_SUCCESS;


            // resets the OMS bit of the control word
            writeControlWord(0x0270, 0);

            // set the cia Switched On status
            writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);

            break;
        }

        command_ms_tmo -= 50;
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


