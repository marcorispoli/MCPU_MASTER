#include "Errors.h"
#include "BodyMotor.h"
#include "pd4_od.h"
#include <thread>


#define ROT_PER_DEGREE ((double) 80 / (double) 3600) //!< Defines the internal unit in 0.1 degrees for unit

#define BRAKE_INPUT_MASK(x) (x & 0x00080000) //!< Not in the Special region [00II][0000]
#define OUPUT1_OUT_MASK     0x00010000 //!< Not in the Special region [00II][0000]
#define OUPUT2_OUT_MASK     0x00020000 //!< Not in the Special region [00II][0000]

/// <summary>
/// This is the class constructor.
/// </summary>
/// 
/// The Constructor initializes the base class and assignes the module can open address and 
/// user unit ratio.
/// 
/// <param name=""></param>
BodyMotor::BodyMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::BODY_ID, L"MOTOR_BODY", ROT_PER_DEGREE)
{
    
}

/// <summary>
/// This function initializes Input and Output digital registers 
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
bool BodyMotor::initializeSpecificObjectDictionary(void) {

    // Sets the Output setting
    if (!blocking_writeOD(OD_3250_02, 0)) return false; // Output control not inverted
    if (!blocking_writeOD(OD_3250_03, 0)) return false; // Force Enable = false
    if (!blocking_writeOD(OD_3250_08, 0)) return false; // Routing Enable = false
    if (!blocking_writeOD(OD_60FE_01, 0)) return false; // Set All outputs to 0

    // Set the input setting
    if (!blocking_writeOD(OD_3240_01, 0x4)) return false; // Input control special: I3 = HOMING
    if (!blocking_writeOD(OD_3240_02, 0x3)) return false; // Function Inverted: I1 and 2 inverted
    if (!blocking_writeOD(OD_3240_03, 0)) return false;   // Force Enable = false
    if (!blocking_writeOD(OD_3240_06, 0)) return false;   // Input Range Select: threshold = 5V;

    // Check if the BRAKE input is OFF. In case it should be ON, a relevant alarm shall be activated
    if (!blocking_readOD(OD_60FD_00)) return false; // Reads the Inpuyts
    if (BRAKE_INPUT_MASK(rxSdoRegister->data)) {
        brake_alarm = true;
        Debug::WriteLine("BodyMotor: Failed test output off, off");
        Notify::activate("BODY_MOTOR_BRAKE_FAULT", false);
        return true;
    }

    // Sets the OUTPUT2 ON and OUTPUT1 OFF and test again the Brake input
    if (!blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK)) return false;
    if (!blocking_readOD(OD_60FD_00)) return false; // Reads the Inpuyts
    if (BRAKE_INPUT_MASK(rxSdoRegister->data)) {
        brake_alarm = true;
        Debug::WriteLine("BodyMotor: Failed test output off, on");
        Notify::activate("BODY_MOTOR_BRAKE_FAULT", false);

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
        Notify::activate("BODY_MOTOR_BRAKE_FAULT", false);

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
        Notify::activate("BODY_MOTOR_BRAKE_FAULT", false);

        // Clear the OUTPUTS
        blocking_writeOD(OD_60FE_01, 0);
        return true;
    }

    return true;
}

/// <summary>
/// The BodyMotor class override this function in order to 
/// handle the IDLE activities
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool BodyMotor::idleCallback(void) {

    // Check if the BRAKE input is OFF. In case it should be ON, a relevant alarm shall be activated
    if (!brake_alarm) {
        if (!blocking_readOD(OD_60FD_00)) return false;

        if (BRAKE_INPUT_MASK(rxSdoRegister->data)) {
            brake_alarm = true;
            Debug::WriteLine("BodyMotor: Failed test brake input in IDLE");
            Notify::activate("BODY_MOTOR_BRAKE_FAULT", false);
            blocking_writeOD(OD_60FE_01, 0); // Set All outputs to 0
            return false;
        }

        // All things are OK
        return true;
    }

    // Alarm condition
    blocking_writeOD(OD_60FE_01, 0); // Set All outputs to 0
    return false;
    
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
bool BodyMotor::automaticPositioningPreparation(void) {

    // Unlock the Brake device
    if (!blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK | OUPUT1_OUT_MASK)) {

        // Failed to receive the command answer: in any case switch off the brake
        blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK);
        return false;
    }

    // wait a while before to proceed
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Test if the brake is actually activated
    if (!blocking_readOD(OD_60FD_00)) return false; // Reads the Inputs
    if (!BRAKE_INPUT_MASK(rxSdoRegister->data)) {

        // Failed the brake power activation detected
        brake_alarm = true;
        Debug::WriteLine("BodyMotor: Failed to unlock");
        Notify::activate("BODY_MOTOR_BRAKE_FAULT", false);

        // Clear the OUTPUTS
        blocking_writeOD(OD_60FE_01, 0);
        return false;
    }

    // OK. Brake device correctly powered
    return true;
}

/// <summary>
/// The BodyMotor class override this function in order to 
/// activate the motor brake as soon as the activation command termines.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
void BodyMotor::automaticPositioningCompletion(void) {

    // Lock the brake device
    blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return;
}
