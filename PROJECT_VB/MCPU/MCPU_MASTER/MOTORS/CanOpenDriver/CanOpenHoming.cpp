#include "CanDriver.h"
#include "Notify.h"
#include "CanOpenMotor.h"
#include "pd4_od.h"
#include <thread>




bool CanOpenMotor::activateAutomaticHoming(int method_on, int method_off, int speed, int acc) {

    // Command already in execution
    if (!isReady()) {
        command_completed_code = MotorCompletedCodes::ERROR_MOTOR_BUSY;
        return false;
    }

    command_homing_on_method = method_on;
    command_homing_off_method = method_off;
    command_acc = acc;
    command_speed = speed;
    request_command = MotorCommands::MOTOR_HOMING;

    return true;
}

#define ZERO_INPUT_MASK(x) (x & 0x00040000) //!< Not in the Special region [00II][0000]
void CanOpenMotor::manageAutomaticHoming(void) {
    bool error_condition;
    home_initialized = false;



    // Sets the Speed activation
    error_condition = false;

    // Read the inputs before to proceed
    bool current_homing_input = false;
    if (blocking_readOD(OD_60FD_00)) {
        if (ZERO_INPUT_MASK(rxSdoRegister->data)) current_homing_input = true;
    }

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
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN AUTO HOMING PREPARATION");
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
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN AUTO HOMING OPERATION ENABLE SETTING");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }


    // Allows the application to prepare for the motor activation
    MotorCompletedCodes preparation_error = automaticHomingPreparationCallback();
    if (preparation_error != MotorCompletedCodes::COMMAND_PROCEED) {
        setCommandCompletedCode(preparation_error);
        return;
    }

    // Update the previous position
    previous_uposition = current_uposition;

    // Set the start bit (BIT4) in the control word register
    if (!writeControlWord(0x10, 0x10)) {
        setCommandCompletedCode(MotorCompletedCodes::ERROR_ACTIVATION_REGISTER);
        return;
    }

    // Measuring the actuation time
    typedef std::chrono::high_resolution_clock clock;
    clock::time_point start = clock::now();

    command_ms_tmo = 60000; // Sets the timout to 60 seconds for all devices

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
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING ERROR READING STATUS REGISTER");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_ACTIVATION_REGISTER);
            break;
        }

        unsigned short statw = rxSdoRegister->data;

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING ERROR POSSIBLE FAULT");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_INTERNAL_FAULT);
            break;
        }

        // Test the Timeout condition
        if (command_ms_tmo <= 0) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING TIMEOUT ERROR");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_TIMOUT);
            break;
        }

        // The Application can early terminate the activation
        MotorCompletedCodes termination_condition = automaticHomingRunningCallback();
        if (termination_condition >= MotorCompletedCodes::MOTOR_ERRORS) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: Application terminated early for error detected");
            setCommandCompletedCode(termination_condition);
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
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING SUCCESSFULLY COMPLETED");

            setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);

            // resets the OMS bit of the control word
            writeControlWord(0x0270, 0);

            // set the cia Switched On status
            writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);

            break;
        }

        if (error) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING ERROR  FAULT");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_INTERNAL_FAULT);
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


bool CanOpenMotor::initResetEncoderCommand(int initial_eposition) {

    updateCurrentPosition();

    // Already reset in this position
    if (current_eposition == initial_eposition) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ENCODER INITIALIZATION SUCCESS, Position = " + current_uposition.ToString());
        return true;
    }

    // Write the Homing method to 35: current position method
    if (!blocking_writeOD(OD_6098_00, 35)) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR WRITING IDX 6098");
        return false;
    }

    // Write the Position offset register
    if (!blocking_writeOD(OD_607C_00, initial_eposition)) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR WRITING IDX 607C");
        return false;
    }


    // Write the operating mode to HOMING
    if (!blocking_writeOD(OD_6060_00, OD_6060_00_PROFILE_HOMING)) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR WRITING IDX 6060 TO HOMING");
        return false;
    }

    // Set the start bit (BIT4) in the control word register
    if (!writeControlWord(0x10, 0x10)) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR WRITING CW BIT 4");
        return false;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Clear the start bit (BIT4) in the control word register
    if (!writeControlWord(0x10, 0)) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR CLEARING CW BIT 4");
        return false;
    }

    // Write the operating mode to PROFILE POSITIONING
    if (!blocking_writeOD(OD_6060_00, OD_6060_00_PROFILE_POSITIONING)) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR WRITING IDX 6060 TO POSITIONING");
        return false;
    }

    updateCurrentPosition();
    if (current_eposition != initial_eposition) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ENCODER NOT IN THE INITIAL ASSIGNED VALUE");
        return false;
    }


    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ENCODER INITIALIZATION SUCCESS, Position = " + current_uposition.ToString());
    return true;


}
