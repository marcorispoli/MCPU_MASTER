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
/// + The Motor Rotation conversion ratio is 1 round -> 1mm;
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
/// # General module Specification
/// 
/// ## Position control
/// 
/// + The position is set in mm/unit;
/// + The speed is set in mm/sec;
/// + The Acceleration and deceleration is set in mm/sec^2;
/// + After the first system startup, the current position is invalidated:
///		+ the Gantry shall execute a Service/Zero setting procedure to validate the actual position;
/// + The current position will be further invalidated in the case the limit switch should be detected;
/// + The current position is stored in the Motor Configuration File after any activation;
/// + After a subsequent startup, the current position will be retrived from the Motor Configuration File without activate a zero settoing procedure;
///
/// ## Limit Switch detection
/// 
/// The module monitors the current limit switch photocell status.
/// 
/// In Idle status:
/// - if the limit switch should be detected active, the current position will be invalidated 
///   and a relevant alarm is generated. Further activation will be rejected until the zero setting procedure will 
///	  be successfully  executed.
/// 
/// During the activation:
/// - if the limit switch should be detected active, the activation is quickly aborted.
///	  The position will be then invalidated (see Idle status)
/// 
/// ## Safety condition detection
/// 
/// The module handles the following safety conditions:
/// - Obstacle detection: Gantry::getObstacleStatus();
/// - Compression activation: Gantry::getCompressionStatus();
/// - Open Cabinet: Gantry::getOpenCabinetStatus();
/// 
///		NOTE: All those conditions are under the control of the Gantry module ant this module makes use of 
///		external functions to get the related status.
/// 
/// For all the previous conditions:
/// - Any activation is rejected;
/// - The motor activation is immediatelly aborted if the safety condition should be activated after the command activation.
///  
/// ## Manual Motor activation
/// 
/// + The module allows tio manually activate the motor Upwards or Downwards in the allowed activation range;
/// + If the activation should reach the upper or lower position, the module softly stops the activation before to reach the Limit position;
/// + The Gantry module determines what are the current system inputs combinations that determines the manual activation upwards or downwards;
///		+ Gantry::manualMotorActivationIncrease(): causes the activation upward;
/// 	+ Gantry::manualMotorActivationDecrease(): causes the activation downward;
/// 
/// See the Gantry module description for the input used in the different operating scenarios.
/// 
/// 
///
/// # Module Initialization 
/// 
/// + The module overrides the initializeSpecificObjectDictionaryCallback() in order to initialize
/// specific registers of the Vertical Motor control.
/// 
/// # Manual Activation 
/// 
/// 
/// The Module implements the manual activation, inheriting the Manual Activation feature from the CanOpenMotor Base Class.
/// The module override the following callbacks:
/// - manualPositioningRunningCallback(): 
///	  during the manual activation the activation inputs, the limit swithc and the safety condition are monitored;
/// - manualPositioningCompletedCallback(): at the command completion the encoder position is stored into the configuration file.
/// 
/// The Module 
///  
/// # Automatic activation
/// 
/// # Zero Setting 
/// 
/// # Safety 
/// 
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
	static VerticalMotor^ device = gcnew VerticalMotor();

	static bool activateIsocentricCorrection(int id, int delta_target); //!< This command activates the isocentric correction
	static bool startHoming(void);
	static inline void setManualEnable(bool status) { manual_activation_enabled = status; } //!< Enables / Disables the manual activation mode

private:

	static bool manual_activation_enabled = true; //!< This is the flag to enable the manual activation
	static bool manual_increment_direction = false; //!< Sets true if the increment manual command is executing, false if the decrement manual activation is executing

protected:
	bool initializeSpecificObjectDictionaryCallback(void) override; //!< Sets specific registers for the Arm activation
	bool testLimitSwitch(void); //!< True if the limit switch is engaged

	MotorCompletedCodes automaticPositioningPreparationCallback(void) override;
	MotorCompletedCodes automaticPositioningRunningCallback(void) override;
	void automaticPositioningCompletedCallback(MotorCompletedCodes error) override; //!< Override the basic class to handle the Virtual isocentric function    	

	MotorCompletedCodes manualPositioningPreparationCallback(void) override;
	MotorCompletedCodes manualPositioningRunningCallback(void) override;
	void manualPositioningCompletedCallback(MotorCompletedCodes error) override;

	void automaticHomingCompletedCallback(MotorCompletedCodes error) override;
	
	MotorCompletedCodes idleCallback(void) override;
	void faultCallback(bool errstat, bool data_changed,  unsigned int error_class, unsigned int error_code) override;

	void resetCallback(void) override;



};

