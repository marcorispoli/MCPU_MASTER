#include "gantry_global_status.h"
#include "IdleForm.h"
#include "OperatingForm.h"
#include "ServiceForm.h"
#include "SystemConfig.h"
#include "Notify.h"
#include <mutex>
#include "PCB301.h"
#include "PCB302.h"
#include "CanOpenMotor.h"
#include "Exposuremodule.h"
#include "awsProtocol.h"
#include "Log.h"
#include "ArmMotor.h"
#include "TiltMotor.h"
#include "SlideMotor.h"
#include "BodyMotor.h"
#include "VerticalMotor.h"



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
        LogClass::logInFile("SELECTED MONITOR: X0 = " + monitor_X0.ToString() + ", Y0 = " + monitor_Y0.ToString());
         

        
    
}


void Gantry::initialize(void) {
    
   
    // Initializes the Operating Demo status
    if (SystemConfig::Configuration->getParam(SystemConfig::PARAM_DEMO_MODE)[SystemConfig::PARAM_DEMO_MODE_STATUS] == "1")
        operating_demo_status = true;
    else  operating_demo_status = false;

    // Force some process to be in demo status when the operating is in demo
    if (operating_demo_status) {       
        generator_simulator = true;
    }
    generator_simulator = true;
    
    can_driver_simulator = false;   
    
    if (System::Convert::ToByte(SystemConfig::Configuration->getParam(SystemConfig::PARAM_RUNNING_MODE)[SystemConfig::PARAM_RUNNING_MODE_PCB301]) == 1)
        pcb301_simulator = false;
    else pcb301_simulator = true;

    if (System::Convert::ToByte(SystemConfig::Configuration->getParam(SystemConfig::PARAM_RUNNING_MODE)[SystemConfig::PARAM_RUNNING_MODE_PCB302]) == 1)
        pcb302_simulator = false;
    else pcb302_simulator = true;

    if (System::Convert::ToByte(SystemConfig::Configuration->getParam(SystemConfig::PARAM_RUNNING_MODE)[SystemConfig::PARAM_RUNNING_MODE_PCB303]) == 1)
        pcb303_simulator = false;
    else pcb303_simulator = true;

    if (System::Convert::ToByte(SystemConfig::Configuration->getParam(SystemConfig::PARAM_RUNNING_MODE)[SystemConfig::PARAM_RUNNING_MODE_PCB304]) == 1)
        pcb304_simulator = false;
    else pcb304_simulator = true;

    if (System::Convert::ToByte(SystemConfig::Configuration->getParam(SystemConfig::PARAM_RUNNING_MODE)[SystemConfig::PARAM_RUNNING_MODE_PCB315]) == 1)
        pcb315_simulator = false;
    else pcb315_simulator = true;

    if (System::Convert::ToByte(SystemConfig::Configuration->getParam(SystemConfig::PARAM_RUNNING_MODE)[SystemConfig::PARAM_RUNNING_MODE_PCB326]) == 1)
        pcb326_simulator = false;
    else pcb326_simulator = true;

    if( System::Convert::ToByte(SystemConfig::Configuration->getParam(SystemConfig::PARAM_RUNNING_MODE)[SystemConfig::PARAM_RUNNING_MODE_TILT]) == 1)
        motor_tilt_simulator = false;
    else motor_tilt_simulator = true;

    if (System::Convert::ToByte(SystemConfig::Configuration->getParam(SystemConfig::PARAM_RUNNING_MODE)[SystemConfig::PARAM_RUNNING_MODE_ARM]) == 1)
        motor_arm_simulator = false;
    else motor_arm_simulator = true;

    if (System::Convert::ToByte(SystemConfig::Configuration->getParam(SystemConfig::PARAM_RUNNING_MODE)[SystemConfig::PARAM_RUNNING_MODE_SLIDE]) == 1)
        motor_slide_simulator = false;
    else motor_slide_simulator = true;

    if (System::Convert::ToByte(SystemConfig::Configuration->getParam(SystemConfig::PARAM_RUNNING_MODE)[SystemConfig::PARAM_RUNNING_MODE_BODY]) == 1)
        motor_body_simulator = false;
    else motor_body_simulator = true;

    if (System::Convert::ToByte(SystemConfig::Configuration->getParam(SystemConfig::PARAM_RUNNING_MODE)[SystemConfig::PARAM_RUNNING_MODE_VERTICAL]) == 1)
        motor_vertical_simulator = false;
    else motor_vertical_simulator = true;

   

    // Set the current language for messages and GUI
    Notify::setLanguage("ENG");

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
    awsProtocol::EVENT_GantryStatus();
    return true;
}

bool Gantry::setOperating() {
    if (current_operating_status == operating_status_options::GANTRY_OPERATING) return true;
    if (current_operating_status == operating_status_options::GANTRY_SERVICE) return false;
    if (current_operating_status == operating_status_options::GANTRY_IDLE) ((IdleForm^)pIdleForm)->close();

    current_operating_status = operating_status_options::GANTRY_OPERATING;
    ((OperatingForm^)pOperatingForm)->open();
    awsProtocol::EVENT_GantryStatus();
    return true;
}

bool Gantry::setService() {
    if (current_operating_status == operating_status_options::GANTRY_OPERATING) return false;
    if (current_operating_status == operating_status_options::GANTRY_SERVICE) return true;
    
    if (current_operating_status == operating_status_options::GANTRY_IDLE) ((IdleForm^)pIdleForm)->close();

    current_operating_status = operating_status_options::GANTRY_SERVICE;
    ((ServiceForm^)pServiceForm)->open();
    awsProtocol::EVENT_GantryStatus();
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
    ExposureModule::reset();// Reset all the modalities
    ((OperatingForm^)pOperatingForm)->evaluateReadyWarnings(true); // Reset the Warnings of the ready conditions

    return setIdle();
}

 void Gantry::setManualRotationMode(manual_rotation_options mode) {
    manual_rotation_mode = mode;
    if (manual_rotation_mode == manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED) PCB301::set_manual_rot_ena_led(false);
    else PCB301::set_manual_rot_ena_led(true);
}

bool Gantry::getObstacleRotationStatus(int addr, CANOPEN::CanOpenMotor::motor_rotation_activations direction) {
    switch ((CANOPEN::MotorDeviceAddresses)addr) {
    case CANOPEN::MotorDeviceAddresses::ARM_ID:
        if(direction == CANOPEN::CanOpenMotor::motor_rotation_activations::MOTOR_INCREASE) return PCB326::isSensorActive(SENS_CS1 );
        else    return PCB326::isSensorActive(SENS_CS2);
        break;

    case CANOPEN::MotorDeviceAddresses::TILT_ID:
        break;
    case CANOPEN::MotorDeviceAddresses::BODY_ID:
        return PCB326::isSensorActive(SENS_CS1 | SENS_CS2 | SENS_CS3 | SENS_CS4);
        break;

    case CANOPEN::MotorDeviceAddresses::SLIDE_ID:
        return PCB326::isSensorActive(SENS_CS3 | SENS_CS4);
        break;
    case CANOPEN::MotorDeviceAddresses::VERTICAL_ID:
        break;
    }

    return false;

}


Gantry::safety_rotation_conditions Gantry::getSafetyRotationStatus(int addr) {

    // High priority over all 
    if (PCB301::getCabinetSafety()) return safety_rotation_conditions::GANTRY_SAFETY_CABINET;    // No activation with the cabinet open
    if (PCB301::getPowerdown()) return safety_rotation_conditions::GANTRY_SAFETY_POWER_DOWN;        // No activation in power down condition
    if (PCB301::getEmergency()) return safety_rotation_conditions::GANTRY_SAFETY_EMERGENCY_BUTTON;        // No activation in Emergency condition

    // The TILT activation can be performed now
    if ((CANOPEN::MotorDeviceAddresses)addr == CANOPEN::MotorDeviceAddresses::TILT_ID) return safety_rotation_conditions::GANTRY_SAFETY_OK;

    // The rest of motorizations it is requested more conditions

    if (PCB302::getForce()) return safety_rotation_conditions::GANTRY_SAFETY_COMPRESSION; // No activation with the compression force detected
    if (PCB301::getCompressionStatus()) return safety_rotation_conditions::GANTRY_SAFETY_COMPRESSION; // No activation with the hardware compression input activated
    if (!PCB301::getMotorSwitch()) return safety_rotation_conditions::GANTRY_SAFETY_POWER_SUPPLY;// No activation with the hardware safety switch disabled;

    // Ok safety
    return safety_rotation_conditions::GANTRY_SAFETY_OK;

}

bool Gantry::getManualRotationIncrease(int addr) {
    if (manual_rotation_mode == manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED) return false;
  


    switch ((CANOPEN::MotorDeviceAddresses)addr) {
    case CANOPEN::MotorDeviceAddresses::ARM_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_STATUS_MANUAL_ROTATION) {
            if (PCB301::get_button_arm_cw_stat()) return true;
        }else if (manual_rotation_mode == manual_rotation_options::GANTRY_ARM_MANUAL_ROTATION) {
            if (PCB301::get_button_arm_cw_stat()) return true;
            if (PCB301::get_button_up_stat()) return true;
            if (PCB301::get_pedal_up_stat()) return true;
        }
        break;

    case CANOPEN::MotorDeviceAddresses::TILT_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_TILT_MANUAL_ROTATION) {
            if (PCB301::get_button_arm_cw_stat()) return true;
            if (PCB301::get_pedal_up_stat()) return true;
        }
        break;

    case CANOPEN::MotorDeviceAddresses::BODY_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_STATUS_MANUAL_ROTATION) {
            if (PCB301::get_button_body_cw()) return true;
        }else if (manual_rotation_mode == manual_rotation_options::GANTRY_BODY_MANUAL_ROTATION) {
            if (PCB301::get_button_body_cw()) return true;
            if (PCB301::get_pedal_up_stat()) return true;
            if (PCB301::get_button_arm_cw_stat()) return true;
        }
        break;

    case CANOPEN::MotorDeviceAddresses::SLIDE_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_STATUS_MANUAL_ROTATION) {
            if (PCB301::get_button_slide_up_stat()) return true;
        }else if (manual_rotation_mode == manual_rotation_options::GANTRY_SLIDE_MANUAL_ROTATION) {
            if (PCB301::get_button_slide_up_stat()) return true;
            if (PCB301::get_button_down_stat()) return true;
            if (PCB301::get_pedal_down_stat()) return true;
        }

        break;

    case CANOPEN::MotorDeviceAddresses::VERTICAL_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_STATUS_MANUAL_ROTATION) {
            if (PCB301::get_button_up_stat()) return true;
            if (PCB301::get_pedal_up_stat()) return true;
        }
        else if (manual_rotation_mode == manual_rotation_options::GANTRY_VERTICAL_MANUAL_ROTATION) {
            if (PCB301::get_button_up_stat()) return true;
            if (PCB301::get_pedal_up_stat()) return true;
        }

        return false;
        break;
    }

    return false;
}

bool Gantry::getManualRotationDecrease(int addr) { 
    if (manual_rotation_mode == manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED) return false;

    switch ((CANOPEN::MotorDeviceAddresses)addr) {
    case CANOPEN::MotorDeviceAddresses::ARM_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_STATUS_MANUAL_ROTATION) {
            if (PCB301::get_button_arm_ccw_stat()) return true;
        }else if (manual_rotation_mode == manual_rotation_options::GANTRY_ARM_MANUAL_ROTATION) {
            if (PCB301::get_button_arm_ccw_stat()) return true;
            if (PCB301::get_button_down_stat()) return true;
            if (PCB301::get_pedal_down_stat()) return true;
        }

        break;
    case CANOPEN::MotorDeviceAddresses::TILT_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_TILT_MANUAL_ROTATION) {
            if (PCB301::get_button_arm_ccw_stat()) return true;
            if (PCB301::get_pedal_down_stat()) return true;
        }
        break;
    case CANOPEN::MotorDeviceAddresses::BODY_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_STATUS_MANUAL_ROTATION) {
            if (PCB301::get_button_body_ccw()) return true;
        }
        else if (manual_rotation_mode == manual_rotation_options::GANTRY_BODY_MANUAL_ROTATION) {
            if (PCB301::get_button_body_ccw()) return true;
            if (PCB301::get_pedal_down_stat()) return true;
            if (PCB301::get_button_arm_ccw_stat()) return true;
            
        }
        break;
    case CANOPEN::MotorDeviceAddresses::SLIDE_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_STATUS_MANUAL_ROTATION) {
            if (PCB301::get_button_slide_down_stat()) return true;
        }
        else if (manual_rotation_mode == manual_rotation_options::GANTRY_SLIDE_MANUAL_ROTATION) {
            if (PCB301::get_button_slide_down_stat()) return true;
            if (PCB301::get_button_up_stat()) return true; // Makes use of Up notation to drive upward
            if (PCB301::get_pedal_up_stat()) return true;  // Makes use of Up notation to drive upward     
        }

        break;
    case CANOPEN::MotorDeviceAddresses::VERTICAL_ID:
        if (manual_rotation_mode == manual_rotation_options::GANTRY_STANDARD_STATUS_MANUAL_ROTATION) {
            if (PCB301::get_button_down_stat()) return true;
            if (PCB301::get_pedal_down_stat()) return true;
        }
        else if (manual_rotation_mode == manual_rotation_options::GANTRY_VERTICAL_MANUAL_ROTATION) {
            if (PCB301::get_button_down_stat()) return true;
            if (PCB301::get_pedal_down_stat()) return true;
        }
        break;
    }

    return false;

}

CANOPEN::CanOpenMotor::motor_rotation_activations Gantry::getManualActivationRequestState(int device_id) {
    bool increase = getManualRotationIncrease(device_id);
    bool decrease = getManualRotationDecrease(device_id);
    if (increase && decrease) return CANOPEN::CanOpenMotor::motor_rotation_activations::MOTOR_UNDEFINED;
    if (!increase && !decrease) return CANOPEN::CanOpenMotor::motor_rotation_activations::MOTOR_NO_ACTIVATION;
    if(increase) return CANOPEN::CanOpenMotor::motor_rotation_activations::MOTOR_INCREASE;
    return CANOPEN::CanOpenMotor::motor_rotation_activations::MOTOR_DECREASE;

}

bool Gantry::isMotorsActive(void) {
    return (
        ArmMotor::device->isBusy() ||
        TiltMotor::device->isBusy() ||
        BodyMotor::device->isBusy() ||
        SlideMotor::device->isBusy() ||
        VerticalMotor::device->isBusy()
        );
}