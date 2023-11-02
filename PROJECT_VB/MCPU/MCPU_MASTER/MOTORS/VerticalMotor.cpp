#include "VerticalMotor.h"
#include "pd4_od.h"

#define ROT_PER_MM ((double) 1/ (double) 1)

bool VerticalMotor::initializeSpecificObjectDictionary(void) {

    // Motor Drive Parameter Set
    // if (!blocking_writeOD(OD_3210_01, 50000)) return false; // Position Loop, Proportional Gain (closed Loop)
    // if (!blocking_writeOD(OD_3210_02, 10)) return false;	 // Position Loop, Integral Gain (closed Loop)

    // Quick Stop Option Code
    //if (!blocking_writeOD(OD_605A_00, 2)) return false;   // 0 Immediate stop  // 1 Braking with "slow down ramp"  // 2 Braking with "quick stop ramp"

    // Fault Option Code
    //if (!blocking_writeOD(OD_605E_00, 0)) return false;  // 0 Immediate stop  // 1 Braking with "slow down ramp"   // 2 Braking with "quick stop ramp"
    if (!blocking_writeOD(OD_3202_00, 0x41)) return false; 	// Motor Drive Submode Select: 6:BLDC 3:CurRed 2:Brake 1:VoS 0: 1=CLOSED_LOOP/O = OPEN_LOOP

    if (!blocking_writeOD(OD_2031_00, 5000)) return false;  // Peak current
    if (!blocking_writeOD(OD_2032_00, 5000)) return false;  // Maximum Speed
    
    // I2t Parameters
    if (!blocking_writeOD(OD_203B_01, 50000)) return false;	// Nominal Current
    if (!blocking_writeOD(OD_203B_02, 1000)) return false;	// Maximum Duration Of Peak Current
    if (!blocking_writeOD(OD_203B_03, 0)) return false;	    // Threshold
    if (!blocking_writeOD(OD_203B_04, 0)) return false;	    // CalcValue
    if (!blocking_writeOD(OD_203B_05, 5000)) return false; // LimitedCurrent
    if (!blocking_writeOD(OD_2056_00, 500)) return false;	// Limit Switch Tolerance Band

    // Max Absolute Acceleration and Deceleration
    if (!blocking_writeOD(OD_60C5_00, 5000)) return false;  // Max Acceleration
    if (!blocking_writeOD(OD_60C6_00, 5000)) return false;  // Max Deceleration


    // Software Position Limit
    if (!blocking_writeOD(OD_607D_01, convert_User_To_Encoder(-6500))) return false;	// Min Position Limit
    if (!blocking_writeOD(OD_607D_02, convert_User_To_Encoder(6500))) return false;	// Max Position Limit

    // Homing registers
    //if (!blocking_writeOD(OD_6098_00, 21)) return false;  // Homing method 21
    //if (!blocking_writeOD(OD_607C_00, 0)) return false;
    return true;
}
VerticalMotor::VerticalMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::VERTICAL_ID, L"MOTOR_VERTICAL", ROT_PER_MM)
{
    // Sets +/- 5mm as the acceptable target range
    setTargetRange(5, 5);
}