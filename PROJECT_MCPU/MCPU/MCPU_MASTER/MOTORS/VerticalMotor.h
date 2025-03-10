#pragma once
#include "CanOpenMotor.h"


/**
    \addtogroup MOTDESC
    <div style="page-break-after: always;"></div>


    \section MOTVERTICAL Vertical Motor Description

    ## Abstract

    This section describes the feature of the Vertical motor control, or most commonly known as Up/Down Motor.

    The section describes in detail the performances, the activation modes, and the limitations.


    ## Overview

    The Gantry provides the Vertical linear motorization in order to adjust the height of the compression plane.

    There are two activation modes:
    + the manual activation: the operatore adjust manually the height of the compression plane;
    + automatic activation: this is an automatic activation reserved for the "Virtual Isocentric Correction"; 
    

    ## Performances

    + The Vertical position can be controlled with 1mm precision;
    + The allowed travel is from 0 (Lower position) to 1000 mm (Upper position);
    + The travel speed is set to 50 mm/s and can be adjusted in the \ref MotorConfig;
    + The maximum speed depends by the tube load;


    ## Position Calibration

    The current position of the Vertical motor is provided by an high resolution encoder,
    embedded into the intelligent motor driver:
    + The Application reads the \ref MotorConfig file at the startup, retriving the last valid position;
    + The motor internal encoder is then initialized during the system startup;
    + After every activation, the current position is stored into the \ref MototConfig so that it can be further used to reinitialize the internal encoder;

    In the event of data lost or corrupted position,
    the correct position can be restored with an automatic or manual calibration procedure:
    + The Automatic calibration position makes use of a zero position photocell;
    + The manual procedure allows to manually move the Vertical motor to the correct mechanical zero position;

    \note
    After the first istallation it is mandatory to execute the zero position calibration
    before to make the Vertical motor available for the automatic activations or manual activations. 
    In this case a WARNING message  is generated by the Application.


    ## Safety Position Limits Monitoring

    The Vertical motor is provided with two photocells that detects a non allowed transit in the limit positions:
    + An Upper photocell detects the Upper limit position;
    + a Lower photocell detects the lower limit position;

    The Photocell status is read by mean of the Motor GPIO and their status are reported to the Application
    controller module.

    In case of a limit photocell should be detected active:    
    + The current activation is immediatelly aborted;
    + A Warning message will be activated: no exposure can be further initiated;
    + The current encoder position is invalidated and the zero setting procedure shall be required to restore the normal operations;
    + No more activation can be initiated until the zero setting procedure will be successfully executed.

    \important
    + The breast compression is disabled for safety reasons;

    ## Automatic Vertical Positioning for the Isocentric Correction 

    The Application allows to activate Vertical motor in automatic mode only 
    for the Isocentric correction.

    The Isocentric correction is activated only when:
    + the C-ARM rotation is activated  by the AWS with the \ref EXEC_ArmPositioning command;
    + the Slide motor is activated for the Detector tilting funtion in operating mode;

    \note
    Non the C-ARM nor the Slide manual activations can trigger the Vertical Isocentric correction.

    ## Manual Vertical Positioning

    The Application provides several manual triggers point to manualy activate the Vertical motor:
    + In operating mode: the pedalboard and the manual keyboard provides buttons to start the manual VErtical activation;
    + In serice mode: the pedalboard and the manual keyboard provides buttons to start the manual VErtical activation;

    \note
    When Gantry is in Idle mode the Vertical activation is not permitted.


    ## Zero Setting Calibration

    The zero setting procedure is necessary when:
    + After the first installation in Factory;
    + In case the Vertical should be mechanically removed or the position is mechanically modified;
    + In the case a limit photocell (upper or lower) should be activated;
    + In case the Application should be reinstalled and the \ref MotorConfig should be cancelled or overridden;
    + In case the configuraiton file should be corrupted;
    
    The Application provides a dedicated service panel to proceed with the zero setting calibration.

    There are two possible zero setting procedures:
    + The automatic procedure;
    + The manual procedure;

    When the automatic zero setting procedure is activated:
    + The motor driver moves toward the zero setting photocell;
    + When the photocell is detected activated the motor stops the activation and reset its internal encoder;   
    + The \ref MotorConfig file is then updated;

    The manual zero setting position, differently by the automatic zero setting procedure,
    requires the manual activation of the Vertical motor by mean of the enabled buttons.

    The operator shall activate the rotation until the Vertical position reaches the expected mechanical zero point:
    + The Internal encoder of the motor is then reset to zero;
    + The \ref MotorConfig file is then updated;

    \note
    the speed in the manual mode is reduced allowing to a fine zero positioning.

    ## Motor Fault Management

    The motor driver handles an internal Diagnostics in order to detect malfunctions.

    There are several fault conditions:
    + some of theme are repristinable;
    + other malfunctions, for their inner nature, causes a permanent fault;

    In case the Motor driver should detect a fault condition, the MCPU application
    will activate an ERROR message with the description of the signaled fault.

    \attention
    When the motor driver is in fault condition it internally disables the motor activation.\n
    The Application only monitors the status changing and, as soon as the fault condition should
    reset, it clears the error message.\n
    There is however an exception:
    + if the signaled fault is due to a thermal protection, the MCPU will remove the power to the
    motor and put the Gantry out of service.


    ## Safety Strategy

    The following safety strategy protects against injuries:
    + In Idle state (not in Open Study) the VErtical rotation is disabled at all.
    + In case of driver faut, the Vertical activation is disabled;
    + In case of limit photocells activated, the Vertical motor and the compressor device are disabled;
    

    \note
    In Idle state, the Gantry may be left unattended and consequently may be accidentally activated by
    non competent operators.

    \important
    the obstacle detection with the Vertical activation is not applicable;

    ## Motor Configuration process

    During the Application startup fase, after the communication with the motor driver
    is achieved and before to enter in an operating mode (where the motor can be activated),
    the application uploads the motor driver with all the necessary registers in order to
    tune the motor performances as expected.

    \note
    Not all the registers are modified during the startup: some register is runtime modified
    when the motor is activated by the application.

    Because the Vertical module inherits the Base Motor Module,
    most of the registers that this module uploads are the default of the base module.
    See the \ref MOTCONFIG for details

    Here are reported only those registers  that are specific for the Tilt:

    |Register Address|Value|Description|
    |:--|:--|:--|
    |0x3202:00|0x41| Sets the BLDC motor model|
    |0x2031:00|15000| Pick current = 15Amp|
    |0x2032:00|5000| Maximum speed|
    |0x203B:01|50000| Nominal current |
    |0x203B:02|2000| Maximum duration of peak current |
    |0x203B:03|0|Threshold|
    |0x203B:04|0|Calc Value|
    |0x203B:05|15000|Limit current|
    |0x2056:00|500|Limit swithc tollerance Band|
    |0x3210:01|10000|P of Pid|
    |0x3210:02|5|I of Pid|
    |0x60C5:00|5000|Max acceleration|
    |0x60C6:00|5000|Max deceleration|
    |0x3240:01|4|Input I3 = homing|
    |0x3240:02|0|Not inverted inputs|
    |0x3240:03|0|Force Enable = false|
    |0x3240:06|0|Input voltage threshold = 5V|


*/



/// <summary>
/// \defgroup VerticalMotor_Module Vertical Activation Module Implementation
/// 
/// \ingroup MOTIMPL 
/// 
/// This module describes the Vertical activation implementation.
/// 
/// See the \ref MOTDESC for module functional description details.
/// 
/// </summary>
/// 
///  
///


/// <summary>
/// This is the Vertical Module class  implementation
/// \ingroup VerticalMotor_Module
/// </summary>
ref class VerticalMotor : public CANOPEN::CanOpenMotor
{
public:
	
    /// <summary>
    /// This is the VerticalMotor class constructor
    /// 
    /// </summary>
    VerticalMotor(void);

    /// <summary>
    /// This is the static reference to the VerticalMotor module.
    /// 
    /// The first use of the static reference in the Application 
    /// will causes an automatic auto instancing of the module
    /// 
    /// The application can have access to the non static members of this module 
    /// (for example the methods inherited by the base  class)  by using the TiltMotor::device-> reference.
    /// 
    /// 
    /// </summary>
	static VerticalMotor^ device = gcnew VerticalMotor();

	/// <summary>
	/// This function activates the Vertical motor in auto mode for the isocentric correction
	/// </summary>	
	static bool activateIsocentricCorrection(int id, int delta_target); //!< This command activates the isocentric correction

    /// <summary>
    /// This function activates the manual zero setting procedure.
    /// 
    /// </summary>   
	static bool startManualHoming(int target_position);

    /// <summary>
    /// This function activates the automatic zero setting procedure.
    /// 
    /// </summary>  
    static bool startAutoHoming();
	
private:

	static bool manual_increment_direction = false; //!< Sets true if the increment manual command is executing, false if the decrement manual activation is executing

protected:

    /// <summary>
    /// The VerticalMotor overrides this function in order to initialize specific motor registers during the startup fase.
    /// </summary>
	unsigned short initializeSpecificObjectDictionaryCallback(void) override; //!< Sets specific registers for the Arm activation
	
    /// <summary>
    /// This callback is called during the activation preparation fase.
    /// </summary>
	MotorCompletedCodes preparationCallback(MotorCommands current_command, int current_position, int target_position) override;
	
    /// <summary>
    /// This callback is called during the travel of the motor.
    /// </summary>   
    MotorCompletedCodes runningCallback(MotorCommands current_command, int current_position, int target_position) override;
	
    /// <summary>
    /// The module overrides this function in order to  handle the IDLE activities
    /// 
    /// </summary>
    MotorCompletedCodes idleCallback(void) override;
	
    /// <summary>
    /// The module overrrides this callback in order to activate a specific error message
    /// </summary>
    /// 
    void faultCallback(bool errstat, bool data_changed,  unsigned int error_class, unsigned int error_code) override;
	
    /// <summary>
    /// The module overrides this callback in order to update the module status at the command completion.
    /// </summary>
    /// 
    void completedCallback(int id, MotorCommands current_command, int current_position, MotorCompletedCodes term_code) override;
	

	bool high_photocell; //!< This is the current detected upper limit photocell status;
	bool low_photocell; //!< This is the current detected lower limit photocell status;
	bool zero_photocell; //!< This is the current detected zero photocell status;
	void testLimitSwitch(void); //!< True if the limit switch is engaged
};

