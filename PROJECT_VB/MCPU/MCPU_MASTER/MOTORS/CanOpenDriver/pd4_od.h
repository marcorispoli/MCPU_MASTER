#pragma once
#include "CanOpenMotor.h"

using namespace CANOPEN;

// _______________________ DIGITAL IO MASK ________________________________________

#define PD4_MOTOR_DI1  0x00010000
#define PD4_MOTOR_DI2  0x00020000
#define PD4_MOTOR_DI3  0x00040000
#define PD4_MOTOR_DI4  0x00080000
#define PD4_MOTOR_DI5  0x00100000
#define PD4_MOTOR_DI6  0x00200000

#define PD4_MOTOR_DO1  0x00010000
#define PD4_MOTOR_DO2  0x00020000

// _______________________ ZERO SETTING OBJECTS ________________________________________


#define ZERO_SETTING_CTRL_INIT  0x0270,0x0000                   // OMS BITS RESET
#define ZERO_SETTING_START      0x0270,0x0010                   // OMS BIT4 = ON
#define ZERO_SETTING_STOP       0x0270,0x0010                   // OMS BIT4 = OFF

#define ZERO_STAT_PERFORMED     0x3400,0x0000
#define ZERO_STAT_INTERRUPTED   0x3400,0x0400
#define ZERO_STAT_CONFIRMED     0x3400,0x1000
#define ZERO_STAT_COMPLETED     0x3400,0x1400
#define ZERO_STAT_ERROR_RUN     0x3400,0x2000
#define ZERO_STAT_ERROR_IDL     0x3400,0x2400


//_______________________ POSITIONING SETTING OBJECTS ________________________________________
#define POSITION_SETTING_CTRL_INIT_MASK		0x0370
#define POSITION_SETTING_CTRL_INIT_VAL		0x0020

#define POSITION_TRIGGER_SETTING_CTRL_INIT  0x0370, 0x0000
#define POSITION_SETTING_START_MASK			0x0370
#define POSITION_SETTING_START_VAL			0x0030


#define OD_1001_00 0x1001, 0x00, ODRegister::SDODataDimension::R1B  // ERROR REGISTER
#define OD_1001_00_GENERAL_ERROR   0x1
#define OD_1001_00_I_ERROR         0x2
#define OD_1001_00_VOLTAGE_ERROR   0x4
#define OD_1001_00_TEMP_ERROR      0x8
#define OD_1001_00_COM_ERROR       0x10
#define OD_1001_00_PROFILE_ERROR   0x20
#define OD_1001_00_RESERVED_ERROR  0x40
#define OD_1001_00_MANUFACT_ERROR  0x80

#define OD_1003_01 0x1003, 0x01, ODRegister::SDODataDimension::R4B  // SPECIFIC ERROR REGISTER

//_______________ STORE PARAMETER REGISTER ________________________________________________
#define OD_1010_01 0x1010, 0x01, ODRegister::SDODataDimension::R4B  // Save All Parameters To Non-volatile Memory
#define OD_1010_02 0x1010, 0x02, ODRegister::SDODataDimension::R4B  // Save Communication Parameters To Non-volatile Memory
#define OD_1010_03 0x1010, 0x03, ODRegister::SDODataDimension::R4B  // Save Application Parameters To Non-volatile Memory
#define OD_1010_04 0x1010, 0x04, ODRegister::SDODataDimension::R4B  // Save Customer Parameters To Non-volatile Memory
#define OD_1010_05 0x1010, 0x05, ODRegister::SDODataDimension::R4B  // Save Drive Parameters To Non-volatile Memory
#define OD_1010_06 0x1010, 0x06, ODRegister::SDODataDimension::R4B  // Save Tuning Parameters To Non-volatile Memory
#define OD_SAVE_CODE 0x65766173
#define OD_SAVE_ENABLED 0x1
#define OD_SAVE_DISABLED 0

//_______________ User Internal data ____________________________________________________________________________________________________________________
#define OD_2700_01 0x2700, 0x01 ,ODRegister::SDODataDimension::R1B   // User data control word: SET to 1 to save User content. reset autom. to 0 when finished
#define OD_2700_02 0x2700, 0x02 ,ODRegister::SDODataDimension::R2B   // USER #0 - RESERVED FOR THE CONFIG VECTOR CONFIGURATION CODE
#define OD_2700_03 0x2700, 0x03 ,ODRegister::SDODataDimension::R2B   // USER #1 - RESERVED FOR THE NANOJ CONFIGURATION CODE
#define OD_2700_04 0x2700, 0x04 ,ODRegister::SDODataDimension::R2B   // USER #2 - RESERVED FOR RESET SENSING
#define OD_2700_05 0x2700, 0x05 ,ODRegister::SDODataDimension::R2B   // USER #3
#define OD_2700_06 0x2700, 0x06 ,ODRegister::SDODataDimension::R2B   // USER #4
#define OD_2700_07 0x2700, 0x07 ,ODRegister::SDODataDimension::R2B   // USER #5
#define OD_2700_08 0x2700, 0x08 ,ODRegister::SDODataDimension::R2B   // USER #6
#define OD_2700_09 0x2700, 0x09 ,ODRegister::SDODataDimension::R2B   // USER #7


#define CONFIG_USER_PARAM OD_2700_02
#define NANOJ_USER_PARAM OD_2700_03
#define RESET_USER_PARAM OD_2700_04
#define SAVE_USER_PARAM OD_2700_01
#define RESET_CODE  0xAAAA


//_______________ NANOJ ________________________________________________
#define OD_1F51_02 0x1F51, 0x02 ,ODRegister::SDODataDimension::R1B   // VMM FLASH CONTROL WORD:
#define VMM_DELETE 0x10
#define VMM_INIT 0x11
#define VMM_WRITE 0x12

#define OD_1F50_02  0x1F50, 0x02 ,ODRegister::SDODataDimension::R4B   // VMM FLASH DATA CONTENT:
#define OD_1F50_02_INIT 0x20     // VMM FLASH DATA CONTENT:

#define OD_1F57_02  0x1F57, 0x02 ,ODRegister::SDODataDimension::R4B  // VMM FLASH STATUS:
#define OD_1029_01  0x1029, 0x01 ,ODRegister::SDODataDimension::R1B // NMT Behavior in case of fault
#define OD_1029_02  0x1029, 0x02,ODRegister::SDODataDimension::R1B
#define OD_2031_00  0x2031, 0x00, ODRegister::SDODataDimension::R4B // Maximum Current
#define OD_2032_00  0x2032, 0x00,ODRegister::SDODataDimension::R4B  // Maximum Speed
#define OD_2033_00  0x2033, 0x00,ODRegister::SDODataDimension::R4B   // Plunger Block
#define OD_2034_00  0x2034, 0x00 ,ODRegister::SDODataDimension::R4B  // Upper Voltage Warning Level
#define OD_2035_00  0x2035, 0x00 ,ODRegister::SDODataDimension::R4B  // Lower Voltage Warning Level
#define OD_2036_00  0x2036, 0x00 ,ODRegister::SDODataDimension::R4B  // Open Loop Current Reduction Idle Time
#define OD_2037_00  0x2037, 0x00 ,ODRegister::SDODataDimension::R4B  // Open Loop Current Reduction Value/factor


// I2t Parameters
#define OD_203B_01  0x203B, 0x01 ,ODRegister::SDODataDimension::R4B  // Nominal Current
#define OD_203B_02  0x203B, 0x02 ,ODRegister::SDODataDimension::R4B  // Maximum Duration Of Peak Current
#define OD_203B_03  0x203B, 0x03 ,ODRegister::SDODataDimension::R4B  // Threshold
#define OD_203B_04  0x203B, 0x04 ,ODRegister::SDODataDimension::R4B  // CalcValue
#define OD_203B_05  0x203B, 0x05 ,ODRegister::SDODataDimension::R4B  // LimitedCurrent
#define OD_2056_00  0x2056, 0x00 ,ODRegister::SDODataDimension::R4B  // Limit Switch Tolerance Band


// user unitS
#define OD_2061_00  0x2061, 0x00 ,ODRegister::SDODataDimension::R4B  // Velocity Numerator
#define OD_2062_00  0x2062, 0x00 ,ODRegister::SDODataDimension::R4B  // Velocity Denominator
#define OD_2063_00  0x2063, 0x00 ,ODRegister::SDODataDimension::R4B  // Acceleration Numerator
#define OD_2064_00  0x2064, 0x00 ,ODRegister::SDODataDimension::R4B  // Acceleration Denominator
#define OD_2065_00  0x2065, 0x00 ,ODRegister::SDODataDimension::R4B  // Jerk Numerator
#define OD_2066_00  0x2066, 0x00 ,ODRegister::SDODataDimension::R4B  // Jerk Denominator
#define OD_3202_00  0x3202, 0x00 ,ODRegister::SDODataDimension::R4B  // Motor Drive Submode Select


// Motor Drive Sensor Display Closed Loop
#define OD_320B_01  0x320B, 0x01 ,ODRegister::SDODataDimension::R4B  // Commutation
#define OD_320B_02  0x320B, 0x02 ,ODRegister::SDODataDimension::R4B  // Torque
#define OD_320B_03  0x320B, 0x03 ,ODRegister::SDODataDimension::R4B  // Velocity
#define OD_320B_04  0x320B, 0x04 ,ODRegister::SDODataDimension::R4B  // Position

// Motor Drive Parameter Set
#define OD_3210_01  0x3210, 0x01 ,ODRegister::SDODataDimension::R4B  // Position Loop, Proportional Gain (closed Loop)
#define OD_3210_02  0x3210, 0x02 ,ODRegister::SDODataDimension::R4B  // Position Loop, Integral Gain (closed Loop)

// Analogue Inputs Control
#define OD_3221_00  0x3221, 0x00 ,ODRegister::SDODataDimension::R4B  // Analogue Inputs Control: 0 , Voltage, 1, Current

// Digital Inputs Control
#define OD_3240_01  0x3240, 0x01 ,ODRegister::SDODataDimension::R4B  // Special Function Enable (b2:ZS, b1:PL, b0:Nl)
#define OD_3240_02  0x3240, 0x02 ,ODRegister::SDODataDimension::R4B  // Function Inverted (0,NO; 1,NC)
#define OD_3240_03  0x3240, 0x03 ,ODRegister::SDODataDimension::R4B  // Force Enable
#define OD_3240_04  0x3240, 0x04 ,ODRegister::SDODataDimension::R4B  // Force Value
#define OD_3240_05  0x3240, 0x05 ,ODRegister::SDODataDimension::R4B  // Input Raw Value
#define OD_3240_06  0x3240, 0x06 ,ODRegister::SDODataDimension::R4B  // Input Range Select (0,threshold,5V, 1: threshold,24V)

#define OD_60FD_00  0x60FD, 0x00 ,ODRegister::SDODataDimension::R4B  // Input Read Status register

// Digital Input Capture
#define OD_3241_01  0x3241, 0x01 ,ODRegister::SDODataDimension::R4B  // Control (0:off, 1:RE, 2:FE, 3:RE+FE)
#define OD_3241_02  0x3241, 0x02 ,ODRegister::SDODataDimension::R4B  // Capture Count
#define OD_3241_03  0x3241, 0x03 ,ODRegister::SDODataDimension::R4B  // Encoder user units
#define OD_3241_04  0x3241, 0x04 ,ODRegister::SDODataDimension::R4B  // Encoder Raw Value


// Digital Outputs Control
#define OD_3250_02  0x3250, 0x02 ,ODRegister::SDODataDimension::R4B  // Function Inverted
#define OD_3250_03  0x3250, 0x03 ,ODRegister::SDODataDimension::R4B  // Force Enable
#define OD_3250_04  0x3250, 0x04 ,ODRegister::SDODataDimension::R4B  // Force Value
#define OD_3250_08  0x3250, 0x08 ,ODRegister::SDODataDimension::R4B  // Routing Enable

#define OD_60FE_01  0x60FE, 0x01 ,ODRegister::SDODataDimension::R4B  // Output write register


// Change status reaction:-1 No reaction,0 Immediate stop,1 Braking with "slow down ramp",2 Braking with "quick stop ramp"
#define OD_3700_00  0x3700, 0x00 ,ODRegister::SDODataDimension::R2B  // Following Error Option Code
#define OD_605A_00  0x605A, 0x00 ,ODRegister::SDODataDimension::R2B  // Quick Stop Option Code
#define OD_605B_00  0x605B, 0x00 ,ODRegister::SDODataDimension::R2B  // Shutdown Option Code
#define OD_605C_00  0x605C, 0x00 ,ODRegister::SDODataDimension::R2B  // Disable Option Code
#define OD_605D_00  0x605D, 0x00 ,ODRegister::SDODataDimension::R2B  // Halt Option Code
#define OD_605E_00  0x605E, 0x00 ,ODRegister::SDODataDimension::R2B  // Fault Option Code

// CiA 402 status word
#define OD_6041_00 0x6041, 0x00, ODRegister::SDODataDimension::R2B
#define OD_6041_00_NotReadyToSwitchOn   0x004F,0x0000
#define OD_6041_00_SwitchOnDisabled     0x004F,0x0040
#define OD_6041_00_ReadyToSwitchOn      0x006F,0x0021
#define OD_6041_00_SwitchedOn           0x006F,0x0023
#define OD_6041_00_OperationEnabled     0x006F,0x0027
#define OD_6041_00_QuickStopActive      0x006F,0x0007
#define OD_6041_00_FaultReactionActive  0x004F,0x000F
#define OD_6041_00_Fault                0x004F,0x0008

// CiA Control Word & Status change flags
#define OD_6040_00 0x6040, 0x00, ODRegister::SDODataDimension::R2B
#define OD_6040_00_SHUTDOWN_MASK                    0x0087
#define OD_6040_00_SHUTDOWN_VAL                     0x0006
#define OD_6040_00_SWITCHON_MASK                    0x008F
#define OD_6040_00_SWITCHON_VAL                     0x0007
#define OD_6040_00_ENABLEOP_MASK                    0x008F
#define OD_6040_00_ENABLEOP_VAL                     0x000F
#define OD_6040_00_DISABLEOP_MASK                   0x008F
#define OD_6040_00_DISABLEOP_VAL                    0x0007
#define OD_6040_00_QUICKSTOP_MASK                   0x0087
#define OD_6040_00_QUICKSTOP_VAL                    0x0002

#define OD_6040_00_DISVOLTAGE                  0x0082, 0x0000


#define OD_6040_00_RESET_OMS                   0x0270, 0x0000

//___________ OPERATING MODE PROFILE ______________________________________________________
#define OD_6060_00 0x6060, 0x00, ODRegister::SDODataDimension::R1B  // OPERATING MODE CONROL WORD
#define OD_6060_00_NO_PROFILE                  0
#define OD_6060_00_PROFILE_HOMING              6
#define OD_6060_00_PROFILE_POSITIONING         1

// Position control characteristics
#define OD_6064_00  0x6064, 0x00 ,ODRegister::SDODataDimension::R4B  // Actual position encoder value (user units)
#define OD_607A_00  0x607A, 0x00, ODRegister::SDODataDimension::R4B  // Target Position user unit (relative or absolute)

#define OD_6065_00  0x6065, 0x00 ,ODRegister::SDODataDimension::R4B  // Following Error Window
#define OD_6066_00  0x6066, 0x00 ,ODRegister::SDODataDimension::R2B  // Following Error Time
#define OD_6067_00  0x6067, 0x00 ,ODRegister::SDODataDimension::R4B  // Position Target Window + time
#define OD_6068_00  0x6068, 0x00 ,ODRegister::SDODataDimension::R2B  // Position Target Time
#define OD_607B_01  0x607B, 0x01 ,ODRegister::SDODataDimension::R4B  // Min Position Range Limit
#define OD_607B_02  0x607B, 0x02 ,ODRegister::SDODataDimension::R4B  // Max Position Range Limit
#define OD_607D_01  0x607D, 0x01 ,ODRegister::SDODataDimension::R4B  // Min Position Limit
#define OD_607D_02  0x607D, 0x02 ,ODRegister::SDODataDimension::R4B  // Max Position Limit
#define OD_607E_00  0x607E, 0x00 ,ODRegister::SDODataDimension::R1B  // Rotation Polarity
#define OD_6081_00  0x6081, 0x00 ,ODRegister::SDODataDimension::R4B  // Maximum travel speed
#define OD_6082_00  0x6082, 0x00 ,ODRegister::SDODataDimension::R4B  // End Velocity
#define OD_6083_00  0x6083, 0x00 ,ODRegister::SDODataDimension::R4B  // Maximum acceleration
#define OD_6084_00  0x6084, 0x00 ,ODRegister::SDODataDimension::R4B  // Maximum Deceleration
#define OD_6085_00  0x6085, 0x00 ,ODRegister::SDODataDimension::R4B  // Quick Stop Deceleration

#define OD_60F2_00  0x60F2, 0x00 ,ODRegister::SDODataDimension::R2B  // Positioning Option Code

// Position Encoder Resolution: EncInc/MotRev
#define OD_608F_01  0x608F, 0x01 ,ODRegister::SDODataDimension::R4B  // Encoder Increments
#define OD_608F_02  0x608F, 0x02 ,ODRegister::SDODataDimension::R4B  // Motor Revolutions

// Gear Ratio
#define OD_6091_01  0x6091, 0x01 ,ODRegister::SDODataDimension::R4B  // Motor Revolutions
#define OD_6091_02  0x6091, 0x02 ,ODRegister::SDODataDimension::R4B  // Shaft Revolutions

// Max Absolute Acceleration and Deceleration
#define OD_60C5_00  0x60c5, 0x00 ,ODRegister::SDODataDimension::R4B  // Max Acceleration
#define OD_60C6_00  0x60c6, 0x00 ,ODRegister::SDODataDimension::R4B  // Max Deceleration

// Homing profile registers
#define OD_6098_00 0x6098, 0x00 ,ODRegister::SDODataDimension::R1B   // Homing method
#define OD_607C_00 0x607C, 0x00 ,ODRegister::SDODataDimension::R4B   // Offset to be applied to the zero setting
#define OD_6099_01 0x6099, 0x01 ,ODRegister::SDODataDimension::R4B   // Homing Speed to switch
#define OD_6099_02 0x6099, 0x02 ,ODRegister::SDODataDimension::R4B   // Homing Speed to reference
#define OD_609A_00 0x609A, 0x00 ,ODRegister::SDODataDimension::R4B   // Homing Acceleration

#define OD_4013_01 0x4013, 0x01 ,ODRegister::SDODataDimension::R4B   // Hardware Setup: 1= VCC LOGIC ON


#define OD_2300_00 0x2300, 0x00 ,ODRegister::SDODataDimension::R4B   // NanoJ Program execution: 1= RUN, 0=STOP
#define OD_2301_00 0x2301, 0x00 ,ODRegister::SDODataDimension::R4B   // NanoJ Status --- ERR,RES,RUN
#define OD_2302_00 0x2302, 0x00 ,ODRegister::SDODataDimension::R4B   // NanoJ Error code

#define OD_4014_01 0x4014, 0x01 ,ODRegister::SDODataDimension::R4B   // Voltage Power (mV)
#define OD_4014_02 0x4014, 0x02 ,ODRegister::SDODataDimension::R4B   // Voltage Logic (mV)
#define OD_4014_03 0x4014, 0x03 ,ODRegister::SDODataDimension::R4B   // Temperature (dC°)


#define OD_2500_01 0x2500, 0x01 ,ODRegister::SDODataDimension::R4B   // Base USER RAM NANO-J (1:32)

