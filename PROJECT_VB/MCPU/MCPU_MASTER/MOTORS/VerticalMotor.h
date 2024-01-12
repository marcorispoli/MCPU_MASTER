#pragma once
#include "CanOpenMotor.h"


/// <summary>
/// \defgroup VerticalMotor_Module Vertical Module controller
/// \ingroup CanOpenModule 
/// This module implements the Vertical activation requirements.
/// 
/// </summary>
///   
/// # Motor Mechanical Assembly setup
/// 
/// + The Motor Rotation conversion ratio is 1g - 1mm: 1 motor round equals  1mm vertical position change;
/// + When the motor rotates clockwise, the ARM moves downward;
/// 
/// + Zero setting photocell: 
///		+ ON  (Light) the motor has to rotate clockwise (Arm downward) to find the center;
///		+ OFF (Dark) the motor has to rotate counter-clockwise (Arm upward) to find the center;
/// 
/// + Limit switch photocell:
///		+ ON  (Light): the limit switch is not detected
///		+ OFF (Dark): the limit switch is detected
/// 
/// When the limit position photocell is engaged (OFF status), the actual limit position \n
/// is determined evaluating the status of both Limit and Zero photocells as \n
/// the following Truth table:
/// 
/// |Zero Photocell|Limit Photocell|Position|
/// |:--|:--|:--|
/// |x|ON|travel range|
/// |ON|OFF|Upper position|
/// |OFF|OFF|Lower position|
/// 
/// 
/// # Motor GPIO assignment
/// 
/// ## Digital Inputs
///  
/// + DIGITAL INPUT 1 and 2: Assigned to the limit switch photocell;
/// + DIGITAL INPUT 3: Homing photocell;
///
/// The Input1 and Input2 are assigned to the same photocell: the limit switch.\n
/// The Input3 shall be assigned to the special function Homing switch.
/// 
/// ## Digital Outputs
/// Not used  
/// 
/// 

/// <summary>
/// This is the Vertical Module class  implementation
/// \ingroup globalModule
/// </summary>
ref class VerticalMotor : public CANOPEN::CanOpenMotor
{
public:
	VerticalMotor(void);
	
	static bool activateIsocentricCorrection(int id, int delta_target); //!< This command activates the isocentric correction
	static VerticalMotor^ device = gcnew VerticalMotor();
	static bool startHoming(void);
	static inline void setManualEnable(bool status) { manual_activation_enabled = status; } //!< Enables / Disables the manual activation mode
private:

	static bool manual_activation_enabled = true; //!< This is the flag to enable the manual activation
	static bool manual_increment_direction = false; //!< Sets true if the increment manual command is executing, false if the decrement manual activation is executing

protected:
	bool initializeSpecificObjectDictionaryCallback(void) override; //!< Sets specific registers for the Arm activation
	bool testLimitSwitch(void); //!< True if the limit switch is engaged

	MotorCompletedCodes automaticPositioningRunningCallback(void) override;
	void automaticPositioningCompletedCallback(MotorCompletedCodes error) override; //!< Override the basic class to handle the Virtual isocentric function    	
	void automaticHomingCompletedCallback(MotorCompletedCodes error) override;
	MotorCompletedCodes idleCallback(void) override;

	MotorCompletedCodes manualPositioningRunningCallback(void) override;
	void manualPositioningCompletedCallback(MotorCompletedCodes error) override;
};

