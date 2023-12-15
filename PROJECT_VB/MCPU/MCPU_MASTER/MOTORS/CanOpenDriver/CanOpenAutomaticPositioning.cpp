#include "CanDriver.h"
#include "Notify.h"
#include "CanOpenMotor.h"
#include "pd4_od.h"
#include <thread>


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
/// <returns>true if the command can be executed</returns>
bool CanOpenMotor::activateAutomaticPositioning(int id, int target, int speed, int acc, int dec) {

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
    return true;
}

/// <summary>
/// This function is called by the Base class at the command preparation. 
/// </summary>
/// 
/// The Subclass should override this function in order to prepare the 
/// activation with specif requirements (brake device activation, diagnostic checks).
/// 
/// + The function shall return the MotorCompletedCodes::COMMAND_PROCEED code to 
/// let the command to continue;
/// + The function shall return any other error code in case of command reject;
/// 
/// <param name=""></param>
/// <returns></returns>
CanOpenMotor::MotorCompletedCodes CanOpenMotor::automaticPositioningPreparationCallback(void) {
    return MotorCompletedCodes::COMMAND_PROCEED; 
}

/// <summary>
/// This function is called by the Base class during the motor activation,
/// every 50ms (approximatelly).
/// 
/// </summary>
/// 
/// The Subclass should override this function in order to monitor the positioning
/// and eventually to stop it in case of specific conditions should be true.
/// 
/// + The function shall return the MotorCompletedCodes::COMMAND_PROCEED code to 
/// let the command to continue;
/// + The function shall return any other error code in case of command abort;
/// 
/// <param name=""></param>
/// <returns></returns>
CanOpenMotor::MotorCompletedCodes CanOpenMotor::automaticPositioningRunningCallback(void) {
    return MotorCompletedCodes::COMMAND_PROCEED; 
}

/// <summary>
/// This function is called by the Base class at the command completion.
/// 
/// </summary>
/// 
/// The function only calls the command_completion_event(). 
/// 
/// The Subclass should override this function in order to implement specific behaviors.
/// 
/// <param name="completed_code"></param>
void CanOpenMotor::automaticPositioningCompletedCallback(MotorCompletedCodes completed_code) {
    command_completed_event(command_id, (int)  completed_code);
    return; 
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
    bool error_condition;

    // Get the actual encoder position 
    updateCurrentPosition();

    // Set the timeout for the activation based on the actual speed and target
    setActivationTimeout(command_speed, command_acc, command_dec, command_target);

    // Test if the actual position is already in target position
    if (isTarget()) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING COMPLETED (ALREADY IN POSITION)");
        setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);
        return;
    }

    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) +
        "> INIT AUTO POSITIONING: From " + System::Convert::ToString(current_uposition) +
        " To " + System::Convert::ToString(command_target) +
        " In " + System::Convert::ToString((int)((double)command_ms_tmo / 1.5)) + " (ms)");


    // Sets the Speed activation
    error_condition = false;
    if (!blocking_writeOD(OD_6081_00, convert_UserSec_To_Speed(command_speed))) error_condition = true; // Sets the Rotation speed 
    if (!blocking_writeOD(OD_6083_00, convert_UserSec_To_Speed(command_acc))) error_condition = true; // Sets the Rotation Acc
    if (!blocking_writeOD(OD_6084_00, convert_UserSec_To_Speed(command_dec))) error_condition = true; // Sets the Rotation Dec
    if (!blocking_writeOD(OD_607A_00, convert_User_To_Encoder(command_target))) error_condition = true; // Set the target position
    if (!blocking_writeOD(OD_6060_00, 1)) error_condition = true;// Set the Activation mode to Profile Positioning Mode
    if (!writeControlWord(POSITION_SETTING_CTRL_INIT_MASK, POSITION_SETTING_CTRL_INIT_VAL)) error_condition = true;// Sets the Position control bits in the control word
    if (error_condition) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN AUTO POSITIONING PREPARATION");
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
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN AUTO POSITIONING OPERATION ENABLE SETTING");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }

    // Allows the application to prepare for the motor activation
    MotorCompletedCodes preparation_error = automaticPositioningPreparationCallback();
    if (preparation_error != MotorCompletedCodes::COMMAND_PROCEED) {
        setCommandCompletedCode(preparation_error);
        return;
    }

    // Update the previous position
    previous_uposition = current_uposition;


    // Set the start bit (BIT4) in the control word register
    if (!writeControlWord(POSITION_SETTING_START_MASK, POSITION_SETTING_START_VAL)) {
        setCommandCompletedCode(MotorCompletedCodes::ERROR_ACCESS_REGISTER);
        return;
    }

    // Measuring the actuation time
    typedef std::chrono::high_resolution_clock clock;
    clock::time_point start = clock::now();


    while (true) {
        
        // Read the current position 
        updateCurrentPosition();

        // Test the abort request flag
        if (abort_request) {
            abort_request = false;

            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ABORT REQUEST!");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_COMMAND_ABORTED);
            break;
        }

        // Reads the status to be sure that it is still Operation Enabled 
        if (!blocking_readOD(OD_6041_00)) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING ERROR READING STATUS REGISTER");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_ACCESS_REGISTER);
            break;
        }

        unsigned short statw = rxSdoRegister->data;

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING ERROR POSSIBLE FAULT");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_INTERNAL_FAULT);
            break;
        }

        

        // Test the Timeout condition
        if ((command_ms_tmo <= 0) && (!isTarget())) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING TIMEOUT ERROR");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_TIMOUT);
            break;
        }

        // The Application can early terminate the activation
        MotorCompletedCodes termination_condition = automaticPositioningRunningCallback();
        if (termination_condition >= MotorCompletedCodes::MOTOR_ERRORS) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: Application terminated early for error detected");
            setCommandCompletedCode(termination_condition);
            break;
        }

        if (((statw & 0x1400) == 0x1400) || (current_uposition == command_target) || ((command_ms_tmo <= 0) && isTarget()) || (termination_condition == MotorCompletedCodes::COMMAND_MANUAL_TERMINATION)) {

            // Calculates the effective duration time
            typedef std::chrono::milliseconds milliseconds;
            milliseconds ms = std::chrono::duration_cast<milliseconds>(clock::now() - start);

            if (((command_ms_tmo <= 0) && isTarget())) {
                Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING COMPLETED (MANUALLY) - Time =  " + System::Convert::ToString(ms.count()));
            }
            else {
                Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSIITONING COMPLETED actually In =  " + System::Convert::ToString(ms.count()) + " (ms)");
            }

            setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);

            // resets the OMS bit of the control word
            writeControlWord(0x0270, 0);

            // set the cia Switched On status
            writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);

            break;
        }

        command_ms_tmo -= 50;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    } // End of main controlling loop

    // Read the current position 
    updateCurrentPosition();

    // resets the OMS bit of the control word
    writeControlWord(0x0270, 0);


    // set the cia Switched On status
    writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);

    // Clears motor command
    current_command = MotorCommands::MOTOR_IDLE;

    return;

}


