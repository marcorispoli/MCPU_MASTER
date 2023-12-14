#include "CanDriver.h"
#include "Notify.h"
#include "CanOpenMotor.h"
#include "pd4_od.h"
#include <thread>


/// <summary>
/// This is the function to activate an Automatic positioning.
/// 
/// This function can be used by the application in order to request 
/// the execution of a positioning.
/// 
/// The function return immediatelly if the motor shouldn't in the right status 
/// to execute a command (busy or not homing).
/// 
/// When a command should be accepted the Application shall wait for the 
/// command_completed_event() callback to detect the command completion.
/// 
/// In case the command should be an Isocentric command (iso == true) 
/// the internal iso_position register shall not be modified.
/// The Isocentric mode is used for those motor activation that needs to be syncronized
/// to keep a stable isocentric position. 
/// 
/// NOTE: the command will start as soon as the module main thread fetch the command request.
/// 
/// </summary>
/// <param name="id">This is the ID code assigned by the applicaiton</param>
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
/// This function executes the Automatic positioning of the motor at the expected target position.
/// 
/// The function executes the following steps for preparation:
/// - sets the expected timeout in ms;
/// - verifies if the current position should be already in target;
/// - upload the running parameters: speed, acceleration deceleration;
/// - sets the Positioning profile mode in the motor;
/// - activates the Start bit in the control register to start the rotation;
/// 
/// During the rotation:
/// - verifies if the current status should be changed. In this case termines with error;
/// - verifies if the activation timeout should expires. In this case termines with error;
/// - verifies if the target is detected in three possible ways:
///     - The motor signals automatically the target (usually it should);
///     - The current position matches with the target position (less probable);
///     - in case of timeout if the position should be in the acceptable range (recovery strategy);
/// 
/// When the command completes (successfully or in error) 
/// the CiA status mode is set to SwitchedOn status (or in a fault status if it should be actually active)
/// 
/// With the command completion, the following event is generated:
/// - command_completed_event(command_id, termination_code);
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
        setCommandCompletedCode(MotorCompletedCodes::ERROR_ACTIVATION_REGISTER);
        return;
    }

    // Measuring the actuation time
    typedef std::chrono::high_resolution_clock clock;
    clock::time_point start = clock::now();


    while (true) {
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
            setCommandCompletedCode(MotorCompletedCodes::ERROR_ACTIVATION_REGISTER);
            break;
        }

        unsigned short statw = rxSdoRegister->data;

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING ERROR POSSIBLE FAULT");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_INTERNAL_FAULT);
            break;
        }

        // Read the current position 
        updateCurrentPosition();

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


