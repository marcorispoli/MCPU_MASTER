#pragma once
#include "CanOpenMotor.h"

/// <summary>
/// \defgroup BodyMotor_Module Body Module controller
/// \ingroup CanOpenModule 
/// 
/// This module implements the Body rotation requirements.
/// 
/// </summary>
/// 
///  
/// # Motor Mechanical Assembly setup
/// 
/// + The Motor gear ratio is 80-1: 80 motor turns equal to 360 degree body rotation;
/// + When the motor rotates clockwise, the Body rotates counter_clockwise (viewed from the Body Top);
/// + The Zero setting photocell: 
///		+ when it is ON (on light) the body has to rotate clockwise (the motor rotate counter_clockwise) to find the center;
///		+ when it is OFF (obscured) the body has to rotate counter_clockwise  (the motor rotate clockwise) to find the center;
/// 
/// + Limit switch photocell OFF (obscured): 
///		+ with Zero setting ON: the limit is the counter-clockwise;
///		+ with Zero setting OFF: the limit is the clockwise;
///
/// 
/// # Motor GPIO assignment
/// 
/// ## Digital Inputs
///  
/// + DIGITAL INPUT 1 and 2: Assigned to the limit switch photocell;
/// + DIGITAL INPUT 3: Homing photocell;
/// + DIGITAL INPUT 4: Brake activation status;
/// 
/// The Input1 and Input 2 are assigned to the same photocell: the limit switch.\n
/// The Limit switch is an active low: when 0 means that the limit switch has been detected.
/// 
/// The Input3 shal be assigned to the Homing Driver process.
/// 
/// The Input4 is a Positive logic assigned to the current brake status:
///		- ON: the brake is powered;
///		- OFF: the brake is not powered;
/// 
///		NOTE: the Brake device is a normally closed brake, 
///		so when it should be powered, it releases the braking force!
/// 
/// ## Digital Outputs
/// 
/// + DIGITAL OUTPUT 1 and 2: brake activation outputs
/// 
/// The Output are tw open collector output that 
/// shall be set as Normally closed (OFF = 0V output).
/// 
/// In order to power the Brake device both Outpuyt shall be set ON.
/// 
/// The use of the Output should be:
/// - OUTPUT2 shall be set normally ON: ift should be set OFF only in case of brake malfunction;
/// - OUTPUT1 shall be set ON/OFF to control the actual brake status;
/// 
///		NOTE: The OUPUT2 should be kept ON because this status should guarantee 
///		that one of the power Mosfet controlling the Brake remains working 
///		in case, for any reason, the other Mosfet should be broken.
/// 
/// # Position unit definition
/// 
/// + The module provides the position readout in the range of +/-180 degree; 
/// + The allowed activation is in the range of +/- 90° with a precision of 0.1 degree;
/// + The positive angle is the counter-clockwise direction (from the TOP View);
/// 
/// # Brake Diagnostic Test
/// 
/// The Brake activation is tested at the beginning during the 
/// motor initialization, and during the Motor IDLE status.
/// 
/// ## Brake Test at the Initialization
/// 
/// During the Motor Initialization, both the OUTPUT1 and OUTPUT2 shall be set to OFF,\n 
/// and the Input4 should be OFF:
/// - In case Input4 should be ON, a non resettable fault condition is generated.
/// 
/// ## Brake Test in IDLE
/// 
/// During the IDLE mode, the OUTPUT 2 is kept ONand the OUTPUT1 is set OFF:
/// - In case the Input4 should be detected ON,  a non resettable fault condition is generated.
/// 
/// ## Brake Fault Condition
/// 
/// In case a Brake device should be detected in Fault, 
/// the OUTPUT1 and OUTPUT2 will both be set OFF and a relevant non resettable alarm will be generated.
/// 
/// 

/// <summary>
/// This is the Body Module class  implementation
/// \ingroup BodyMotor_Module
/// </summary>
ref class BodyMotor : public CANOPEN::CanOpenMotor
{
public:
	BodyMotor(void);
	static BodyMotor^ device = gcnew BodyMotor();
	static bool startHoming(void);
	static inline void setManualEnable(bool status) { manual_activation_enabled = status; } //!< Enables / Disables the manual activation mode

protected:
	bool initializeSpecificObjectDictionaryCallback(void) override;
	MotorCompletedCodes automaticPositioningPreparationCallback(void) override;
	void automaticPositioningCompletedCallback(MotorCompletedCodes error) override;


	MotorCompletedCodes idleCallback(void) override;

	MotorCompletedCodes automaticHomingPreparationCallback(void) override;
	void automaticHomingCompletedCallback(MotorCompletedCodes error) override;

	MotorCompletedCodes manualPositioningPreparationCallback(void) override;
	void manualPositioningCompletedCallback(MotorCompletedCodes error) override;
	MotorCompletedCodes manualPositioningRunningCallback(void) override;

private:
	static bool brake_alarm = false; //!< This is the current brake malfunction alarm
	static bool manual_activation_enabled = false; //!< This is the flag activating the body manual activation
	static bool manual_cw_direction = false; //!< Sets true if the CW manual command is executing, false if the CCW manual activation is executing

};

		