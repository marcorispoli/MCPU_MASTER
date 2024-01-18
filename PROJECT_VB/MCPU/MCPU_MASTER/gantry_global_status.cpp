#include "gantry_global_status.h"
#include "WINDOWS/IdleForm.h"
#include "WINDOWS/OperatingForm.h"
#include "WINDOWS/ServiceForm.h"
#include "CONFIGURATION/SystemConfig.h"

#include "Notify.h"
#include <mutex>
#include "PCB301.h"
#include "PCB302.h"
#include "CanOpenMotor.h"


using namespace System;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;

Gantry::Gantry() {
 
        // Sets the resource path
        applicationResourcePath = System::IO::Directory::GetCurrentDirectory() + "\\RESOURCES\\";

        // Monitor Identification
        int monitors = Screen::AllScreens->Length;

        // Gets the Monitors position
        for (int i = 0; i < monitors; i++) {
            
            int H = Screen::AllScreens[i]->Bounds.Height;
            int W = Screen::AllScreens[i]->Bounds.Width;

            if (W == 600) {
                monitor_X0 = Screen::AllScreens[i]->Bounds.Left;
                monitor_Y0 = Screen::AllScreens[i]->Bounds.Top;                
            }
        }
        Debug::WriteLine("SELECTED MONITOR: X0 = " + monitor_X0.ToString() + ", Y0 = " + monitor_Y0.ToString());
         

        
    
}

void Gantry::initialize(void) {
    if (SystemConfig::Configuration->getParam(SystemConfig::PARAM_DEMO_MODE)[SystemConfig::PARAM_DEMO_MODE_STATUS] == "1") {
        demo_status = true;
    }
    else demo_status = false;


    // Set the current language for messages and GUI
    Notify::setLanguage("ENG");

    pcb304_demo = true;
    pcb302_demo = true;
    pcb326_demo = true;
    motor_arm_demo = true;
    motor_tilt_demo = true;
    motor_slide_demo = true;

    // Creates the status Windows
    pIdleForm = gcnew IdleForm();
    pOperatingForm = gcnew OperatingForm();
    pServiceForm = gcnew ServiceForm();
    valuePopupWindow = gcnew ValuePopupForm();

}
bool Gantry::setIdle() {
    if (current_operating_status == operating_status_options::GANTRY_IDLE) return true;
    if (current_operating_status == operating_status_options::GANTRY_OPERATING) ((OperatingForm^)pOperatingForm)->close();
    if (current_operating_status == operating_status_options::GANTRY_SERVICE) ((ServiceForm^)pServiceForm)->close();

    current_operating_status = operating_status_options::GANTRY_IDLE;
    ((IdleForm^)pIdleForm)->open();
    return true;
}

bool Gantry::setOperating() {
    if (current_operating_status == operating_status_options::GANTRY_OPERATING) return true;
    if (current_operating_status == operating_status_options::GANTRY_SERVICE) return false;
    if (current_operating_status == operating_status_options::GANTRY_IDLE) ((IdleForm^)pIdleForm)->close();

    current_operating_status = operating_status_options::GANTRY_OPERATING;
    ((OperatingForm^)pOperatingForm)->open();
    return true;
}

bool Gantry::setService() {
    if (current_operating_status == operating_status_options::GANTRY_OPERATING) return false;
    if (current_operating_status == operating_status_options::GANTRY_SERVICE) return true;
    
    if (current_operating_status == operating_status_options::GANTRY_IDLE) ((IdleForm^)pIdleForm)->close();

    current_operating_status = operating_status_options::GANTRY_SERVICE;
    ((ServiceForm^)pServiceForm)->open();
    return true;
}


void Gantry::setStartup(void) {
    current_operating_status = operating_status_options::GANTRY_STARTUP;
}

bool Gantry::setOpenStudy(System::String^ patient) {    
    patient_name = patient;
    return setOperating();    
}

bool Gantry::setCloseStudy(void) {    
    patient_name = "";
    return setIdle();
}






bool Gantry::getObstacleRotationStatus(int addr) { 
    switch ((CANOPEN::MotorDeviceAddresses)addr) {
    case CANOPEN::MotorDeviceAddresses::ARM_ID:
        break;
    case CANOPEN::MotorDeviceAddresses::TILT_ID:
        break;
    case CANOPEN::MotorDeviceAddresses::BODY_ID:
        break;
    case CANOPEN::MotorDeviceAddresses::SLIDE_ID:
        break;
    case CANOPEN::MotorDeviceAddresses::VERTICAL_ID:
        break;
    }

    return false;

}
bool Gantry::getSafetyRotationStatus(int addr) { 

    // High priority over all 
    if (PCB301::getCabinetSafety()) return true;
    //if (PCB301::getPowerdown()) return true;
    //if (PCB301::getEmergency()) return true;
    
    if ((CANOPEN::MotorDeviceAddresses) addr == CANOPEN::MotorDeviceAddresses::TILT_ID) {

    }
    else {
        if (PCB302::getForce()) return true;
        if (PCB301::getCompressionStatus()) return true;
        if (!PCB301::getMotorSwitch()) return true;
    }

    return false;

}

bool Gantry::getManualRotationIncrease(int addr) {
    switch ((CANOPEN::MotorDeviceAddresses)addr) {
    case CANOPEN::MotorDeviceAddresses::ARM_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_MANUAL_ROTATION) {
            if (PCB301::get_button_arm_cw_stat()) return true;
        }
        else if (manual_rotation_mode == manual_rotation_options::GANTRY_ARM_MANUAL_ROTATION) {
            if (PCB301::get_button_arm_cw_stat()) return true;
            if (PCB301::get_button_up_stat()) return true;
            if (PCB301::get_pedal_up_stat()) return true;
        }

        break;
    case CANOPEN::MotorDeviceAddresses::TILT_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_TILT_MANUAL_ROTATION) {
            if (PCB301::get_button_arm_cw_stat()) return true;
        }

        break;
    case CANOPEN::MotorDeviceAddresses::BODY_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_MANUAL_ROTATION) {
            if (PCB301::get_button_body_cw()) return true;
        }
        else if (manual_rotation_mode == manual_rotation_options::GANTRY_BODY_MANUAL_ROTATION) {
            if (PCB301::get_button_body_cw()) return true;
            if (PCB301::get_button_up_stat()) return true;
            if (PCB301::get_pedal_up_stat()) return true;
            if (PCB301::get_button_arm_cw_stat()) return true;
        }
        break;
    case CANOPEN::MotorDeviceAddresses::SLIDE_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_MANUAL_ROTATION) {
            if (PCB301::get_button_slide_up_stat()) return true;
        }
        else if (manual_rotation_mode == manual_rotation_options::GANTRY_SLIDE_MANUAL_ROTATION) {
            if (PCB301::get_button_slide_up_stat()) return true;
            if (PCB301::get_button_up_stat()) return true;
            if (PCB301::get_pedal_up_stat()) return true;
            if (PCB301::get_button_arm_cw_stat()) return true;
        }

        break;
    case CANOPEN::MotorDeviceAddresses::VERTICAL_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_MANUAL_ROTATION) {
            if (PCB301::get_button_up_stat()) return true;
            if (PCB301::get_pedal_up_stat()) return true;
        }

        return false;
        break;
    }

    return false;
}

bool Gantry::getManualRotationDecrease(int addr) { 
    switch ((CANOPEN::MotorDeviceAddresses)addr) {
    case CANOPEN::MotorDeviceAddresses::ARM_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_MANUAL_ROTATION) {
            if (PCB301::get_button_arm_ccw_stat()) return true;
        }
        else if (manual_rotation_mode == manual_rotation_options::GANTRY_ARM_MANUAL_ROTATION) {
            if (PCB301::get_button_arm_ccw_stat()) return true;
            if (PCB301::get_button_down_stat()) return true;
            if (PCB301::get_pedal_down_stat()) return true;
        }

        break;
    case CANOPEN::MotorDeviceAddresses::TILT_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_TILT_MANUAL_ROTATION) {
            if (PCB301::get_button_arm_ccw_stat()) return true;
        }
        break;
    case CANOPEN::MotorDeviceAddresses::BODY_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_MANUAL_ROTATION) {
            if (PCB301::get_button_body_ccw()) return true;
        }
        else if (manual_rotation_mode == manual_rotation_options::GANTRY_BODY_MANUAL_ROTATION) {
            if (PCB301::get_button_body_ccw()) return true;
            if (PCB301::get_button_down_stat()) return true;
            if (PCB301::get_pedal_down_stat()) return true;
            if (PCB301::get_button_arm_ccw_stat()) return true;
        }
        break;
    case CANOPEN::MotorDeviceAddresses::SLIDE_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_MANUAL_ROTATION) {
            if (PCB301::get_button_slide_down_stat()) return true;
        }
        else if (manual_rotation_mode == manual_rotation_options::GANTRY_SLIDE_MANUAL_ROTATION) {
            if (PCB301::get_button_slide_down_stat()) return true;
            if (PCB301::get_button_down_stat()) return true;
            if (PCB301::get_pedal_down_stat()) return true;
            if (PCB301::get_button_arm_ccw_stat()) return true;
        }

        break;
    case CANOPEN::MotorDeviceAddresses::VERTICAL_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_MANUAL_ROTATION) {
            if (PCB301::get_button_down_stat()) return true;
            if (PCB301::get_pedal_down_stat()) return true;
        }
        break;
    }

    return false;

}

