#pragma once
#include "ExposureModule.h"
#include "ConfigurationFiles.h"
#include "CanOpenMotor.h"


/**
    \addtogroup MOTDESC
    <div style="page-break-after: always;"></div>


    \section MOTTILT Tube Arm Description

    ## Abstract

    This section describes the feature of the Tube Arm motor control, or most commonly defined Tilt Motor.
    
    The section describes in detail the performances, the activation modes, and the limitations.


    ## Overview

    The Gantry provides the Tilt rotation for the following system operating modes:
    + Tomography exposures;
    + Stereotaxis's Biopsy; 


    ## Performances

    + The Tilt rotation is controlled with a precision of 0.01 degree
    + The allowed rotation is in he -27:+27 degrees;
    + The rotation speed varies depending by the usage:
        + In biopsy or in the pre positioning before to start a Tomo exposure the speed is set into the \ref MotorConfig;
        + The speed used during a Tomo exposure is set into the \ref TomoConfig and depends by the current Tomo sequence selected;
        + During the service and the zero setting procedure, the speed can be reduced to help the calibration quality;
    + The maximum allowed speed is 8 degrees per second in the most heavy conditions;


    ## Position Calibration

    The current position of the Tilt is provided by an high resolution encoder,
    embedded into the intelligent motor driver:
    + The Application reads the \ref MotorConfig file at the startup, retriving the last valid position;  
    + The motor internal encoder is then initialized duing the system startup;
    + After every activation, the current position is stored into the \ref MototConfig so that it can be further used to reinitialize the internal encoder;

    In the event of data lost or corrupted position,
    the correct position can be restored with an automatic or manual calibration procedure:
    + The Automatic calibration position makes use of a zero position photocell;
    + The manual procedure allows to manually move the Tilt to the correct mechanical zero position;

    \note
    After the first istallation it is mandatory to execute the zero position calibration
    before to make the Tilt available for the automatic activations. In this case a WARNING message
    is generated by the Application.

    ## Stationary Electromagnetic Brake 

    The Tilt motor makes use of an EM brake to keep the Arm stable 
    when the motor torque is removed (Idle state).

    The brake activation is indirectly controlled by the Application
    by mean of the motor GPIOs:
    + the motor used to rotate the Tilt provides several GPIOs.
    + one of the available output has been connected to the brake switch;
    + the Applicaiton set the outp state properly during the activation fase;

    During the Application startup fase, the application checks 
    the effectiveness of the brake toggling the controlling output and reading back the 
    brake activation status.

    In case the test should fail, the Gantry willl be set out of service.


    ## Safety Speed Control Circuit Monitoring

    The speed of the Tilt is constantly controlled by an external circuit 
    that, in the case of extra speed, activates the stationary brake.
    
    This circuit provides a fault signal that the Application 
    can read through the motor GPIO inputs:
    + In case of fault the Application will put the Gantry Out Of Service.


    ## Automatic Tilt Positioning for Biopsy or Service scopes

    The AWS can activate the Tilt to move it to a predefined set of positions (\ref EXEC_TrxPosition):
    + +15� for Biopsy;
    + -15� for Biopsy;
    + +25� for Biopsy;
    + -25� for Biopsy;
    + To Predefined Pre positioning (so called Home positions) for the current selected Tomo exposure;

    \note
    The AWS is not enabled to select an arbitrary Tilt angle.

    In a proper Service panel of the Application the Tilt can be activated to 
    a se of representative angles:
    + +/- 15�;
    + +/- 20�;
    + +/- 25�;

    \note
    Although the module implementing the Tilt control provides activation commands 
    to arbitrary positions, with 0.01� of resolution,  those activation are reserved 
    only for the Tomo exposures.
    

    ## Automatic Tilt Positioning for Tomo exposures

    The Application allows to control the Tilt with a special procedure that 
    is required for tomo exposures.

    During a Tomo sequence, the Tilt activation needs to be synchronized with 
    the Detector in order to guarantee that every exposure pulse will be 
    executed to a predefined Tilt angle.

    In order to achive this result, the Tilt activation cannot be performed via software, 
    to prevent an unacceptable jitter. Instead, a special program is started into the 
    motor memory user program: this program reads the synchronization digital signal
    coming from the Detector by mean of a motor GPIO and starts the rotation at the first synch edge.

    The Application uploads this special program during the startup fase, togheher with  the 
    configuration registers.


    ### Tomo Configuration File

    The Tilt activation during a Tomo exposure is fully defined into the  \ref TomoConfig.

    This configuration file, for every Tomo sequence, defines:
    + The Initial and Final positions;
    + The Acceleration, deceleration and travel speed;

    The Initial and Final positions can be set with 0.01� precision;
    The travel speed can be set with 0.01�/s precision;
    The Acceleration and Deceleration can be set with 0.01�/s^2 precision;

    See the \ref TomoConfig file for details.

    ## Manual Tilt Positioning

    \important
    The Tilt cannot be manually moved in Operating mode.

    The Application however implements such manual activation in Service mode,
    with a dedicated Service panel: in this case the manual activation button will 
    be used to start the Tilt rotation.

    ## Zero Setting Calibration

    The zero setting procedure is necessary when:
    + After the first installation in Factory;
    + In case the Tilt should be mechanically removed or rotated for service;
    + In case the Application should be reinstalled and the \ref MotorConfig should be cancelled or overridden;
    + In case the configuraiton file should be corrupted;

    The missing of the correct Tilt position cannot be detected by the system automatically
    and may be visible by an evident discrepance from the expected position (usually the zero position)
    and the real position.

    In the case a zero setting procedure should be executed, a service tool is provided.

    There are two possible zero setting procedures:
    + The automatic procedure;
    + The manual procedure;

    When the automatic zero setting procedure is activated:
    + The motor driver executes a slow rotation until a photocell intercepts the mechanical zero point;
    + The Internal encoder of the motor is then reset to zero;
    + The \ref MotorConfig file is updated;

    The manual zero setting position, differently by the automatic zero setting procedure,
    requires the manual activation of the Tilt by mean of the enabled buttons.

    The operator shall activate the rotation until the Tilt position reaches the expected mechanical zero point:
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
    + In Idle state (not in Open Study) the Tilt rotation is disabled at all.

    \note
    In Idle state, the Gantry may be left unattended and consequently may be accidentally activated by
    non competent operators.

    + In case of driver faut, the Tilt activation is disabled;
    + During an Automatic rotation (AWS) every manual button or pedalboard activation stops the rotation;
    + During an Automatic rotation (AWS) the obstacle detection stops the rotation;
    
    ## Motor Configuration process

    During the Application startup fase, after the communication with the motor driver
    is achieved and before to enter in an operating mode (where the motor can be activated),
    the application uploads the motor driver with all the necessary registers in order to
    tune the motor performances as expected.

    \note
    Not all the registers are modified during the startup: some register is runtime modified
    when the motor is activated by the application.

    Because the Tilt module inherits the Base Motor Module,
    most of the registers that this module uploads are the default of the base module.
    See the \ref MOTCONFIG for details

    Here are reported only those registers  that are specific for the Tilt:

    |Register Address|Value|Description|
    |:--|:--|:--|
    |0x3210:01|10000| Position Loop, Proportional Gain (closed Loop)|
    |0x3210:02|5| Position Loop, Integral Gain (closed Loop)|
    |0x3250:02|0| Output GPIO Inverted = False |
    |0x3250:03|0| Force Enabled = False |
    |0x3250:08|0| Routing Enabled = False |
    |0x60Fe:01|1| Sets the All the Output = 0 |
    |0x3240:01|0x4| Input I3 assigned to zero setting |
    |0x3250:02|0| Not inverted input|
    |0x3250:03|0| Force Enabled = False |
    |0x60Fe:06|1| Iput voltage threshold = 5V |


*/


/// <summary>
/// \defgroup TiltMotor_Module Tilt Rotation Module Implementation
/// 
/// \ingroup MOTIMPL 
/// 
/// This module describes the Tilt rotation implementation.
/// 
/// See the \ref MOTDESC for module functional description details.
/// 
/// </summary>
/// 
///  
///


/// <summary>
/// This is the Tilt Module class  implementation
/// \ingroup TiltMotor_Module
/// </summary>
ref class TiltMotor : public CANOPEN::CanOpenMotor
{
public:


	TiltMotor(void);

    /// <summary>
    /// This is the static reference to the TiltMotor module.
    /// 
    /// The first use of the static reference in the Application 
    /// will causes an automatic auto instancing of the module
    /// 
    /// The application can have access to the non static members of this module 
    /// (for example the methods inherited by the base  class)  by using the TiltMotor::device-> reference.
    /// 
    /// 
    /// </summary>
	static TiltMotor^ device = gcnew TiltMotor(); //!< This is the pointer to the Base Class functions

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

    /// <summary>
    /// This function activates the procedure for the Tomo exposure.
    /// 
    /// </summary>    
    static bool activateTomoScan(int pos, int speed, int acc, int dec);
    
    /// <summary>
    /// This function activates an automatic Tilt rotation with an arbitrary target.
    /// </summary>    
    static bool serviceAutoPosition(int pos);
    
    /// <summary>
    /// This is the method to be called by the Application to activate the Tilt chained Idle command.
    /// </summary> 
    static bool setIdlePosition(void);

    literal int SCOUT_POSITION = 0;     //!< This is the Scout angle position
    literal int BP_R_POSITION = 1500;   //!< This is the Biopsy Right position
    literal int BP_L_POSITION = -1500;  //!< This is the Biopsy left position

    /// <summary>
    /// This is the enumeration class defining the target position external interface
    /// </summary>
    enum class target_options {
        SCOUT = 0,  //!< TRX in scout position
        BP_R,       //!< TRX in Biopsy Right position
        BP_L,       //!< TRX in Biopsy Left position
        TOMO_H,     //!< TRX in Tomo Home position
        TOMO_E,     //!< TRX in Tomo End position
        UNDEF       //!< TRX in not predefined target
    };    
    
    /// <summary>
    /// This function shall be called by the Application to request an Automatic Tilt rotation to the predefined angles.
    /// </summary> 
    static bool setTarget(target_options tg, int id);

    /// <summary>
    /// This function checks if the current Tilt angle is in a range of +/-1 �.
    /// 
    /// </summary>    
    static bool isScoutPosition(void) {
        if (!device->isEncoderInitialized()) return false;
        return ((device->getCurrentPosition() < 100) && (device->getCurrentPosition() > -100));
    }

    /// <summary>
    /// This function returns the current Tilt target position.
    /// </summary>
    /// 
    /// \note
    /// The target position is not necessarily the current angle 
    /// but it is the target angle of the last call of the method setTarget().
    /// 
    /// <param name=""></param>
    /// <returns></returns>
    static inline target_options getTargetPosition(void) { 
        if (!device->isEncoderInitialized()) return target_options::UNDEF;
        return current_target; 
    }

    /// <summary>
    /// This function returns the target code rapresented by its string name.
    /// </summary>
    /// 
    /// This function has been introduced to help the
    /// identification of the target position in the AWS protocol command \ref EXEC_TrxPosition.
    /// 
    /// <param name="strtg">String name of the predefined target positionb (see \ref target_options)</param>
    /// <returns>The code related to the target name</returns>
    static target_options getTargetCode(System::String^ strtg) {
        for (int i = 0; i < (int) target_options::UNDEF; i++) {
            if (((target_options) i).ToString() == strtg) return (target_options)i;
        }
        return target_options::UNDEF;
    }
    
    

protected:

    /// <summary>
    /// This is the override callback called during the initialization fase.    
    /// </summary>
    unsigned short initializeSpecificObjectDictionaryCallback(void) override;
    
    /// <summary>
    /// This callback is called during the activation preparation fase.
    /// </summary>
    MotorCompletedCodes preparationCallback(MotorCommands current_command, int current_position, int target_position) override;

    /// <summary>
    /// The module overrides this callback iin order to update the module status at the command completion.
    /// </summary>
    void completedCallback(int id, MotorCommands current_command, int current_position, MotorCompletedCodes term_code) override;
    

    /// <summary>
    /// This callback is periodically called by the base class when the motor driver is ready 
    /// to execute an activation command.
    /// </summary>
    MotorCompletedCodes idleCallback(void) override;
    
    /// <summary>
    /// This is the overriden callback called in case of Motor Device reset event.
    /// </summary>
    void resetCallback(void) override;

    /// <summary>
    /// This callback is called by the base class when the brake device shall be locked.
    /// </summary>
    bool brakeCallback(void) override;

    /// <summary>
    /// This callback is called by the base class when the brake device shall be unlocked.
    /// </summary>
    bool unbrakeCallback(void) override;

private:

    static bool idle_positioning = false;

    static bool brake_alarm = false; //!< This is the current brake malfunction alarm
    static bool manual_increment_direction = false; //!< Sets true if the increment manual command is executing, false if the decrement manual activation is executing
    static bool tomo_scan = false;
    static target_options current_target = target_options::UNDEF;
    static target_options pending_target = target_options::UNDEF;

    bool unlockBrake(void);
    bool lockBrake(void);
    
};

