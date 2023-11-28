#include "ArmMotor.h"
#include "verticalMotor.h"
#include "../gantry_global_status.h"



#define ROT_PER_CDEGREE ((double) 112/ (double) 36000)
#define COMPRESSION_PLANE_MM ((double) 100) //!< Distance from the compression plane and the C-ARM rotation axe

ArmMotor::ArmMotor(void) :CANOPEN::CanOpenMotor((unsigned char) CANOPEN::MotorDeviceAddresses::ARM_ID, L"MOTOR_ARM", ROT_PER_CDEGREE)
{
    // Sets +/- 0.2 ° as the acceptable target range
    setTargetRange(20, 20);
}
bool ArmMotor::initializeSpecificObjectDictionary(void) {

    // Motor Drive Parameter Set
    // if (!blocking_writeOD(OD_3210_01, 50000)) return false; // Position Loop, Proportional Gain (closed Loop)
    // if (!blocking_writeOD(OD_3210_02, 10)) return false;	 // Position Loop, Integral Gain (closed Loop)

    // Quick Stop Option Code
    //if (!blocking_writeOD(OD_605A_00, 2)) return false;   // 0 Immediate stop  // 1 Braking with "slow down ramp"  // 2 Braking with "quick stop ramp"

    // Fault Option Code
    //if (!blocking_writeOD(OD_605E_00, 0)) return false;  // 0 Immediate stop  // 1 Braking with "slow down ramp"   // 2 Braking with "quick stop ramp"

    // Software Position Limit
    //if (!blocking_writeOD(OD_607D_01, convert_cGRAD_TO_EncoderUnit(-18000))) return false;	// Min Position Limit
    //if (!blocking_writeOD(OD_607D_02, convert_cGRAD_TO_EncoderUnit(18000))) return false;	// Max Position Limit

    // Homing registers
    //if (!blocking_writeOD(OD_6098_00, 21)) return false;  // Homing method 21
    //if (!blocking_writeOD(OD_607C_00, 0)) return false;
    return true;
}

void ArmMotor::setCommandCompletedCode(MotorCompletedCodes error) {

    // If the C-ARM activation is not a Isocentric mode (or is terminated in error) then the command termines here
    if ((!iso_activation_mode) || (error != MotorCompletedCodes::COMMAND_SUCCESS)) {
        CanOpenMotor::setCommandCompletedCode(error);
        return;
    }

    // Activates the Vertical Motor for the isocentric correction
    // The C-ARM position is expressed in cents of degrees;
    // The Vertical Arm position is espressed in millimeters

    double init_h = COMPRESSION_PLANE_MM * cos((double) getPreviousPosition() * 3.14159 / (double)18000);
    double end_h = COMPRESSION_PLANE_MM * cos((double)current_uposition * 3.14159 / (double)18000);
    int target = VerticalMotor::device->getCurrentPosition() + (int) init_h - (int) end_h;

    VerticalMotor::device->activateAutomaticPositioning(command_id, target, 50, 10, 10);
    return;
}