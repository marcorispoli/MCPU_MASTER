#include "CanDriver.h"
#include "Notify.h"
#include "CanOpenMotor.h"
#include "pd4_od.h"
#include <thread>


bool CanOpenMotor::activateManualPositioning(int target, int speed, int acc, int dec) {

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



void CanOpenMotor::manageManualPositioning(void) {
    bool error_condition;
    MotorCompletedCodes termination_code;

    // Get the actual encoder position 
    updateCurrentPosition();

    // Test if the actual position is already in target position
    if (isTarget()) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING COMPLETED (ALREADY IN POSITION)");
        setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);
        return;
    }


    // Sets the Speed activation
    error_condition = false;
    if (!blocking_writeOD(OD_6081_00, convert_UserSec_To_Speed(command_speed))) error_condition = true; // Sets the Rotation speed 
    if (!blocking_writeOD(OD_6083_00, convert_UserSec_To_Speed(command_acc))) error_condition = true; // Sets the Rotation Acc
    if (!blocking_writeOD(OD_6084_00, convert_UserSec_To_Speed(command_dec))) error_condition = true; // Sets the Rotation Dec
    if (!blocking_writeOD(OD_607A_00, convert_User_To_Encoder(command_target))) error_condition = true; // Set the target position
    if (!blocking_writeOD(OD_6060_00, 1)) error_condition = true;// Set the Activation mode to Profile Positioning Mode
    if (!writeControlWord(POSITION_SETTING_CTRL_INIT_MASK, POSITION_SETTING_CTRL_INIT_VAL)) error_condition = true;// Sets the Position control bits in the control word
    if (error_condition) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN MANUAL POSITIONING PREPARATION");
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
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN MANUAL POSITIONING OPERATION ENABLE SETTING");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }

    // Allows the application to prepare for the motor activation
    MotorCompletedCodes preparation_error = manualPositioningPreparationCallback();
    if (preparation_error != MotorCompletedCodes::COMMAND_PROCEED) {
        setCommandCompletedCode(preparation_error);
        return;
    }

    // Update the previous position
    previous_uposition = current_uposition;

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

            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ABORT REQUEST!");
            termination_code = MotorCompletedCodes::ERROR_COMMAND_ABORTED;
            break;
        }

        // Reads the status to be sure that it is still Operation Enabled 
        if (!blocking_readOD(OD_6041_00)) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING ERROR READING STATUS REGISTER");
            termination_code = MotorCompletedCodes::ERROR_ACCESS_REGISTER;
            break;
        }

        unsigned short statw = rxSdoRegister->data;

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING ERROR POSSIBLE FAULT");
            termination_code = MotorCompletedCodes::ERROR_INTERNAL_FAULT;            
            break;
        }

        // Read the current position 
        updateCurrentPosition();

        // The Application can early terminate the activation
        MotorCompletedCodes termination_condition = manualPositioningRunningCallback();
        if (termination_condition >= MotorCompletedCodes::MOTOR_ERRORS) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: Application terminated early for error detected");
            termination_code = termination_condition;
            break;
        }

       
        if (((statw & 0x1400) == 0x1400) || (current_uposition == command_target) || (termination_condition == MotorCompletedCodes::COMMAND_MANUAL_TERMINATION)) {

            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING COMPLETED ");
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

    // Read the current position 
    updateCurrentPosition();

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

    setCommandCompletedCode(termination_code);
    return;

}
