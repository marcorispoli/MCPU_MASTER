#pragma once
#include "ExposureModule.h"
#include "CalibrationConfig.h"
#include "CanOpenMotor.h"

/// <summary>
/// \defgroup TiltMotor_Module Tilt Motor Module controller
/// \ingroup CanOpenModule 
/// This module implements the Tilt activation requirements.
/// 
/// </summary>
///   
/// # Motor Mechanical Assembly setup
/// 
/// + The Motor Rotation conversion ratio is 1 round -> 1.2° rotation;
/// + The Motor assembly is equipped with a brake device.
/// 
/// 
/// # Motor GPIO assignment
/// 
/// ## Digital Inputs
///  
/// + DIGITAL INPUT 1: assigned to the Exp-Win signal;
/// + DIGITAL INPUT 2: assigned to the feedback of the brake device activation status;
/// + DIGITAL INPUT 3: Homing photocell;
/// + DIGITAL INPUT 4: Safety circuit activation feedback;
///
/// 
/// ## Digital Outputs
/// 
/// + DIGITAL OUTPUT 1: assigned to the brake unlock;
/// 
/// |OUTPUT STATUS|BRAKE DEVICE STATUS|
/// |:---:|:---:|
/// |1|LOCKED|
/// |0|UNLOCKED|
/// 
/// # General module Specification
/// 
/// ## Position control
/// 
/// + The position is set in 0.01°/units;
/// + The speed is set in 0.01°/sec;
/// + The Acceleration and deceleration is set in 0.01°/sec^2;
/// + After the first system startup, the current position is invalidated:
///		+ the Gantry shall execute a Service/Zero setting procedure to validate the actual position;
/// + The current position is stored in the Motor Configuration File after any activation;
///     + the current position will be retrived from the Motor Configuration File at the system startup (if the zero setting has been executed almost ones)
///
/// + The position can be activated in Automatic mode or in Tomo mode:
///     + In Automatic mode the motor moves the arm to a defined target;
///     + In Tomo mode the motor moves the arm to a defined target starting when the Digital Input 1 is triggered;
/// 
/// The modules upload a Nano-J program into the Motor driver to handle the Tomo activation mode.
/// 
/// ## Manual Motor activation
/// 
/// + The module allows to manually activate the motor;
/// + The Gantry module determines what are the proper input selected for the manual activation:
///		+ Gantry::manualMotorActivationIncrease(): causes the activation upward;
/// 	+ Gantry::manualMotorActivationDecrease(): causes the activation downward;
/// 
/// The manual activation is monitored in Idle status.
/// 
/// See the Gantry module description for details.
/// 
/// ## Safety condition detection
/// 
/// The module during the activation monitors the system safety conditions.
/// The safety conditions are monitored calling the Gantry::getSafetyRotationStatus();
/// 
/// See the Gantry module description for details.
/// 
/// ## Obstacle detection condition 
/// 
/// The module during the activation monitors the possible obstacle detection
/// calling the Gantry::getObstacleRotationStatus();
/// 
/// See the Gantry module description for details.
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

	static TiltMotor^ device = gcnew TiltMotor(); //!< This is the pointer to the Base Class functions
    static bool startHoming(void);
    static inline void setManualEnable(bool status) { manual_activation_enabled = status; } //!< Enables / Disables the manual activation mode
    static bool activateTomoScan(int pos, int speed, int acc, int dec);
    static bool serviceAutoPosition(int pos);

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
    /// This function sets a new target for the Trx.
    /// 
    /// If the target_angle-current_angle exceeds sensitivity, the target_change_event() is generated.
    /// 
    /// </summary>
    /// <param name="tg">this is the target option code</param>
    /// <param name="id">this is the aws command identifier</param>
    /// <returns>true if the target is successfully set</returns>
    static bool setTarget(target_options tg, int id) {
        int angle = 0;
        int speed, acc, dec;

        pending_target = target_options::UNDEF;

        switch (tg) {
        case target_options::SCOUT:            
            angle = 0;
            speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_SPEED]);
            acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_ACC]);
            dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_DEC]);
            break;

        case target_options::BP_R:
            angle = 1500;
            speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_SPEED]);
            acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_ACC]);
            dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_DEC]);
            break;
        case target_options::BP_L:
            angle = -1500;
            speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_SPEED]);
            acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_ACC]);
            dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_DEC]);
            break;

        case target_options::TOMO_H:
            if (!ExposureModule::getTomoExposure()->valid) return false;
            angle = ExposureModule::getTomoExposure()->tomo_home;
            speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_SPEED]);
            acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_ACC]);
            dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_DEC]);
            break;

        case target_options::TOMO_E:
            if (!ExposureModule::getTomoExposure()->valid) return false;
            angle = ExposureModule::getTomoExposure()->tomo_end;
            speed = ExposureModule::getTomoExposure()->tomo_speed;
            acc = ExposureModule::getTomoExposure()->tomo_acc;
            dec = ExposureModule::getTomoExposure()->tomo_dec;
            break;

        default:
            return false;

        }

        // Activate the command        
        pending_target = tg;
        return device->activateAutomaticPositioning(id, angle, speed, acc, dec,true);
    }

    static inline target_options getTargetPosition(void) { return current_target; }
    static target_options getTargetCode(System::String^ strtg) {
        for (int i = 0; i < (int) target_options::UNDEF; i++) {
            if (((target_options) i).ToString() == strtg) return (target_options)i;
        }
        return target_options::UNDEF;
    }
    
    

protected:
    bool initializeSpecificObjectDictionaryCallback(void) override;

    MotorCompletedCodes automaticPositioningPreparationCallback(void) override;
    MotorCompletedCodes automaticPositioningRunningCallback(void) override;
    void automaticPositioningCompletedCallback(MotorCompletedCodes error) override; //!< Override the basic class to handle the Virtual isocentric function    	

  
    MotorCompletedCodes manualPositioningRunningCallback(void) override;
    void manualPositioningCompletedCallback(MotorCompletedCodes error) override;


    MotorCompletedCodes idleCallback(void) override;

    
    void automaticHomingCompletedCallback(MotorCompletedCodes error) override;

    
    void resetCallback(void) override;
    bool brakeCallback(void) override;
    bool unbrakeCallback(void) override;

private:
    static bool brake_alarm = false; //!< This is the current brake malfunction alarm
    static bool manual_activation_enabled = true; //!< This is the flag to enable the manual activation
    static bool manual_increment_direction = false; //!< Sets true if the increment manual command is executing, false if the decrement manual activation is executing
    static bool tomo_scan = false;
    static target_options current_target = target_options::UNDEF;
    static target_options pending_target = target_options::UNDEF;

    bool unlockBrake(void);
    bool lockBrake(void);
    
};

