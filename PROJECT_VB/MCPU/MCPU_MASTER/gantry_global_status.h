#pragma once

#include "math.h"

using namespace System::Collections::Generic;
using namespace System::Drawing;

ref class Gantry
{
public:
    
    Gantry();
    static Gantry^ gantry_handle = gcnew Gantry();
    static void initialize(void) {}; //!< Causes the Gantry auto creation

    #define pMSHIFT  ((CanOpenMotor^) Gantry::pMotShift)
   

    //static Object^ pAws = nullptr; //!< Pointer to the AWS interface    
    static Object^ pMotShift = nullptr; //!< Pointer to the Shift Motor
    

    // Monitor coordinates
    static int monitor_X0;//!< Pointer to the Monitor X0 position
    static int monitor_Y0;//!< Pointer to the Monitor Y0 position
    static System::String^ applicationResourcePath; //!< This is the current application resource path
    
    // Forms
    static Object^ pIdleForm; //!< Pointer to the IdleForm 
    static Object^ pOperatingForm; //!< Pointer to the OperatingForm 
    
    enum class operating_status_options {
        GANTRY_STARTUP = 0, //!< Gantry is in the Startup operating mode
        GANTRY_IDLE,        //!< Gantry is in the Idle operating mode
        GANTRY_OPEN_STUDY,  //!< Gantry is in the Open-Study operating mode
        GANTRY_SERVICE,     //!< Gantry is in the Service operating mode       
    };
    static const cli::array<System::String^>^ operating_status_tags = gcnew cli::array<System::String^>   { "GANTRY_STARTUP", "GANTRY_IDLE", "GANTRY_OPEN_STUDY", "GANTRY_SERVICE", "UNDEF" };//!< This is the option-tags static array




    static System::String^ getPatientName(void) { return patient_name; }
    static bool setOpenStudy(System::String^ patient);
    static bool setCloseStudy(void);

    static System::String^ getOperatingStatusName(void) { return operating_status_tags[(int)current_operating_status]; }
    static bool isSERVICE(void) { return (current_operating_status == operating_status_options::GANTRY_SERVICE); }
    static bool isIDLE(void) { return (current_operating_status == operating_status_options::GANTRY_IDLE); }
    static bool isOPEN(void) { return (current_operating_status == operating_status_options::GANTRY_OPEN_STUDY); }
    static bool isCLOSE(void) { return isIDLE(); }

    static void setIdle(void);
    static void setStartup(void);
    static void setOperating(void);

private:
    static operating_status_options current_operating_status = operating_status_options::GANTRY_STARTUP;
    static System::String^ patient_name;
};



ref class Biopsy {
public:
    static bool isBiopsy(void) { return biopsy_detected; }
    static bool biopsy_detected = false;

};







