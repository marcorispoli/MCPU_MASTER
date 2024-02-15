#pragma once

#include "math.h"
#include "ValuePopup.h"



using namespace System::Collections::Generic;
using namespace System::Drawing;

ref class Gantry
{
public:
    
    Gantry();
    static Gantry^ gantry_handle = gcnew Gantry();
    static void initialize(void);

    

   
    // Monitor coordinates
    static int monitor_X0;//!< Pointer to the Monitor X0 position
    static int monitor_Y0;//!< Pointer to the Monitor Y0 position
    static System::String^ applicationResourcePath; //!< This is the current application resource path
    
    // Forms
    static Object^ pIdleForm; //!< Pointer to the IdleForm 
    static Object^ pOperatingForm; //!< Pointer to the OperatingForm 
    static Object^ pServiceForm; //!< Pointer to the ServiceForm 
    
    enum class operating_status_options {
        GANTRY_STARTUP = 0, //!< Gantry is in the Startup operating mode
        GANTRY_IDLE,        //!< Gantry is in the Idle operating mode
        GANTRY_OPERATING,  //!< Gantry is in the Operating operating mode
        GANTRY_SERVICE,     //!< Gantry is in the Service operating mode       
    };
    static const cli::array<System::String^>^ operating_status_tags = gcnew cli::array<System::String^>   { "GANTRY_STARTUP", "GANTRY_IDLE", "GANTRY_OPEN_STUDY", "GANTRY_SERVICE", "UNDEF" };//!< This is the option-tags static array

    enum class manual_rotation_options {
        GANTRY_MANUAL_ROTATION_DISABLED = 0,    //!< The Manual rotation is globally disabled
        GANTRY_IDLE_MANUAL_ROTATION,        //!< Arm rotation and Vertical activation with buttons and pedals
        GANTRY_OPERATING_STATUS_MANUAL_ROTATION,        //!< Arm rotation and Vertical activation with buttons and pedals
        GANTRY_BODY_MANUAL_ROTATION,            //!< Pedals and Buttons for Body rotation
        GANTRY_SLIDE_MANUAL_ROTATION,           //!< Pedals and Buttons for Slide
        GANTRY_TILT_MANUAL_ROTATION,            //!< Pedals and Buttons for Slide
        GANTRY_ARM_MANUAL_ROTATION,             //!< Pedals and Buttons for Arm
    };

    enum class safety_rotation_conditions {
        GANTRY_SAFETY_OK,                       //!< No safety issues are detected
        GANTRY_SAFETY_EMERGENCY_BUTTON,         //!< The Emergency push button is detected
        GANTRY_SAFETY_POWER_DOWN,               //!< The Power down is detected
        GANTRY_SAFETY_COMPRESSION,              //!< The Compression is present
        GANTRY_SAFETY_CABINET,                  //!< The Cabinet is open
        GANTRY_SAFETY_POWER_SUPPLY,                  //!< The Cabinet is open
    };

    static manual_rotation_options manual_rotation_mode = manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED;
    static void setManualRotationMode(manual_rotation_options mode);

    static bool getObstacleRotationStatus(int addr);
    static safety_rotation_conditions getSafetyRotationStatus(int addr);
    static bool getManualRotationIncrease(int addr);
    static bool getManualRotationDecrease(int addr);

    static System::String^ getPatientName(void) { return patient_name; }
    static bool setOpenStudy(System::String^ patient);
    static bool setCloseStudy(void);

    static System::String^ getOperatingStatusName(void) { return operating_status_tags[(int)current_operating_status]; }
    static bool isSERVICE(void) { return (current_operating_status == operating_status_options::GANTRY_SERVICE); }
    static bool isIDLE(void) { return (current_operating_status == operating_status_options::GANTRY_IDLE); }
    static bool isOPERATING(void) { return (current_operating_status == operating_status_options::GANTRY_OPERATING); }
    static bool isSTARTUP(void) { return (current_operating_status == operating_status_options::GANTRY_STARTUP); }

    static bool setIdle(void);
    static void setStartup(void);
    static bool setOperating(void);
    static bool setService(void);
    static inline ValuePopupForm^ getValuePopupWindow() { return valuePopupWindow; }
    
    static inline bool isOperatingDemo(void) { return operating_demo_status; }
    static inline bool isPcb301Demo(void) { return pcb301_demo; }
    static inline bool isPcb302Demo(void) { return pcb302_demo; }
    static inline bool isPcb303Demo(void) { return pcb303_demo; }
    static inline bool isPcb304Demo(void) { return pcb304_demo; }
    static inline bool isPcb315Demo(void) { return pcb315_demo; }
    static inline bool isPcb326Demo(void) { return pcb326_demo; }
    static inline bool isMotorTiltDemo(void) { return motor_tilt_demo; }
    static inline bool isMotorArmDemo(void) { return motor_arm_demo; }
    static inline bool isMotorBodyDemo(void) { return motor_body_demo; }
    static inline bool isMotorVerticalDemo(void) { return motor_vertical_demo; }
    static inline bool isMotorSlideDemo(void) { return motor_slide_demo; }
    static inline bool isGeneratorDemo(void) { return generator_demo; }

    // Motor Activation
    static inline bool isMotorsActive(void);

private:
    static operating_status_options current_operating_status = operating_status_options::GANTRY_STARTUP;
    static ValuePopupForm^ valuePopupWindow;
    static System::String^ patient_name;
    
    static bool operating_demo_status = false;
    static bool pcb301_demo = false;
    static bool pcb302_demo = false;
    static bool pcb303_demo = false;
    static bool pcb304_demo = false;
    static bool pcb315_demo = false;
    static bool pcb326_demo = false;
    static bool motor_vertical_demo = false;
    static bool motor_tilt_demo = false;
    static bool motor_arm_demo = false;
    static bool motor_body_demo = false;
    static bool motor_slide_demo = false;
    static bool generator_demo = false;

    
};



ref class Biopsy {
public:
    static bool isBiopsy(void) { return biopsy_detected; }
    static bool biopsy_detected = false;
    
};







