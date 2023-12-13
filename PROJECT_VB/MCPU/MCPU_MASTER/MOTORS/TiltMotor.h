#pragma once
#include "CanOpenMotor.h"
#include "CalibrationConfig.h"

ref class TiltMotor : public CANOPEN::CanOpenMotor
{
public:
	TiltMotor(void);
	static TiltMotor^ device = gcnew TiltMotor();
    static bool startHoming(void);
    static inline void setManualEnable(bool status) { manual_activation_enabled = status; } //!< Enables / Disables the manual activation mode

    literal int SCOUT_POSITION = 0;
    literal int BP_R_POSITION = 1500;
    literal int BP_L_POSITION = -1500;

    enum class target_options {
        SCOUT = 0,  //!< TRX in scout position
        BP_R,       //!< TRX in Biopsy Right position
        BP_L,       //!< TRX in Biopsy Left position
        TOMO_H,     //!< TRX in Tomo Home position
        TOMO_E,     //!< TRX in Tomo End position
        UNDEF       //!< TRX in not predefined target
    };    
    static const cli::array<System::String^>^ target_tags = gcnew cli::array<System::String^> {"SCOUT", "BP_R", "BP_L", "TOMO_H", "TOMO_E", "UNDEF"};
    

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
        int angle;

        switch (tg) {
        case target_options::SCOUT:
            angle = 0;
            break;

        case target_options::BP_R:
            angle = 1500;
            break;
        case target_options::BP_L:
            angle = -1500;
            break;

        case target_options::TOMO_H:
           // if (!TomoConfigRegister::isValid()) return false;
           // target_angle = TomoConfigRegister::getTomoHome();
            return false;
            break;
        case target_options::TOMO_E:
            //if (!TomoConfigRegister::isValid()) return false;
            //target_angle = TomoConfigRegister::getTomoEnd();
            return false;
            break;

        }

        // Activate the command
        int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_SPEED]);
        int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_ACC]);
        int dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_AUTO_DEC]);
        return device->activateAutomaticPositioning(id, angle, speed, acc, dec);
    }

    static target_options getTargetPosition(void) {
        if ((device->current_uposition < SCOUT_POSITION + 10) && (device->current_uposition > SCOUT_POSITION - 10)) return target_options::SCOUT;
        if ((device->current_uposition < BP_R_POSITION + 10) && (device->current_uposition > BP_R_POSITION - 10)) return target_options::BP_R;
        if ((device->current_uposition < BP_L_POSITION + 10) && (device->current_uposition > BP_L_POSITION - 10)) return target_options::BP_L;

        return target_options::UNDEF;
     }

    static target_options getTargetCode(System::String^ strtg) {
        for (int i = 0; i < (int) target_options::UNDEF; i++) {
            if (target_tags[i] == strtg) return (target_options)i;
        }

        return target_options::UNDEF;
    }

    static System::String^ getTargetName(target_options tg) { return target_tags[(int)tg]; }

protected:
    bool initializeSpecificObjectDictionaryCallback(void) override;
    MotorCompletedCodes automaticPositioningPreparationCallback(void) override;
    void automaticPositioningCompletedCallback(MotorCompletedCodes error) override;


    bool idleCallback(void) override;

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

