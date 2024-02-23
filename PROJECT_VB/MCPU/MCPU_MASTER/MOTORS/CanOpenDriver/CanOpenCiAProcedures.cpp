#include "CanDriver.h"
#include "Notify.h"
#include "CanOpenMotor.h"
#include "gantry_global_status.h"
#include "pd4_od.h"
#include <thread>
#include "Log.h"



using namespace CANOPEN;



/// <summary>
/// This function is internally used to decode the content of the Status register 
/// and returning the current CiA status code.
/// 
/// </summary>
/// <param name="regval">The content of the Status register</param>
/// <returns>The detected CiA status code</returns>
CanOpenMotor::_CiA402Status CanOpenMotor::getCiAStatus(int regval) {
    unsigned char val = (unsigned char)(regval & 0xFF);
    if ((val & 0x4F) == 0) return  _CiA402Status::CiA402_NotReadyToSwitchOn;
    else if ((val & 0x4F) == 0x40) return  _CiA402Status::CiA402_SwitchOnDisabled;
    else if ((val & 0x6F) == 0x21) return  _CiA402Status::CiA402_ReadyToSwitchOn;
    else if ((val & 0x6F) == 0x23) return  _CiA402Status::CiA402_SwitchedOn;
    else if ((val & 0x6F) == 0x27) return  _CiA402Status::CiA402_OperationEnabled;
    else if ((val & 0x6F) == 0x7) return  _CiA402Status::CiA402_QuickStopActive;
    else if ((val & 0x4F) == 0xF) return  _CiA402Status::CiA402_FaultReactionActive;
    else if ((val & 0x4F) == 0x8) return  _CiA402Status::CiA402_Fault;
    return _CiA402Status::CiA402_NotReadyToSwitchOn;
}

/// <summary>
/// This convenient function return a String name of the curent CiA status
/// </summary>
/// <param name="status">the CiA status code</param>
/// <returns>The String name of the CiA status code</returns>
System::String^ CanOpenMotor::getCiAStatusString(_CiA402Status status) {
    switch (status) {
    case _CiA402Status::CiA402_NotReadyToSwitchOn: return "CiA402_NotReadyToSwitchOn";
    case _CiA402Status::CiA402_SwitchOnDisabled: return "CiA402_SwitchOnDisabled";
    case _CiA402Status::CiA402_ReadyToSwitchOn: return "CiA402_ReadyToSwitchOn";
    case _CiA402Status::CiA402_SwitchedOn: return "CiA402_SwitchedOn";
    case _CiA402Status::CiA402_OperationEnabled: return "CiA402_OperationEnabled";
    case _CiA402Status::CiA402_QuickStopActive: return "CiA402_QuickStopActive";
    case _CiA402Status::CiA402_FaultReactionActive: return "CiA402_FaultReactionActive";
    case _CiA402Status::CiA402_Fault: return "CiA402_Fault";
    }

    return "";
}


/// <summary>
/// This function handles the SwitchOnDisabled CiA status.
/// 
/// The module tries to enter the ReadyToSwitchOn from this status
/// </summary>
/// <param name=""></param>
void CanOpenMotor::CiA402_SwitchOnDisabledCallback(void) {
    if (CiA_current_status != _CiA402Status::CiA402_SwitchOnDisabled) {
        CiA_current_status = _CiA402Status::CiA402_SwitchOnDisabled;
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: SWITCH-ON DISABLED STATUS");
    }

    // Try to enter the CiA402_ReadyToSwitchOn
    if (!writeControlWord(OD_6040_00_SHUTDOWN_MASK,OD_6040_00_SHUTDOWN_VAL)) return;
}

/// <summary>
/// This function is called when the CiA402_QuickStopActive status is detected.
/// 
/// The module tries to enter the ReadyToSwitchOn from this status
/// </summary>
/// <param name=""></param>
void CanOpenMotor::CiA402_QuickStopActiveCallback(void) {

    if (CiA_current_status != _CiA402Status::CiA402_QuickStopActive) {
        CiA_current_status = _CiA402Status::CiA402_QuickStopActive;
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: QUICK STOP ACTIVE STATUS");
    }

    // Try to enter the CiA402_ReadyToSwitchOn
    if (!writeControlWord(OD_6040_00_SHUTDOWN_MASK, OD_6040_00_SHUTDOWN_VAL)) return;

    return;
}

/// <summary>
/// This function is called when the CiA402_ReadyToSwitchOn status is detected. 
/// 
/// The module tries to enter the CiA402_SwitchedOn from this status
/// </summary>
/// <param name=""></param>
void CanOpenMotor::CiA402_ReadyToSwitchOnCallback(void) {

    if (CiA_current_status != _CiA402Status::CiA402_ReadyToSwitchOn) {
        CiA_current_status = _CiA402Status::CiA402_ReadyToSwitchOn;
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: READY TO SWITCH-ON STATUS");
    }

    // Try to enter the CiA402_SwitchedOn
    if (!writeControlWord(OD_6040_00_SWITCHON_MASK, OD_6040_00_SWITCHON_VAL)) return;

    return;
}


/// <summary>
/// This function is called when the CiA402_SwitchedOn status is detected.  
/// 
/// This is the Operating status machine. The activation commands can be performed in this status.
/// </summary>
/// <param name=""></param>
void CanOpenMotor::CiA402_SwitchedOnCallback(void) {
    MotorCompletedCodes idle_returned_condition;

    if (CiA_current_status != _CiA402Status::CiA402_SwitchedOn) {
        CiA_current_status = _CiA402Status::CiA402_SwitchedOn;
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: SWITCHED-ON STATUS");        

        
    }

    // The subclass can add extra commands in IDLE and enable the command execution
    idle_returned_condition = idleCallback();

    // Test if the manual activation can be done
    if (idle_returned_condition == MotorCompletedCodes::COMMAND_PROCEED) {

        // If the safety condition prevent the command execution it is immediatelly aborted
        Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device_id);
        if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
            idle_returned_condition = MotorCompletedCodes::ERROR_SAFETY; // Priority over the limit switch
        }

        // Evaluates the manual termination condition
        if (idle_returned_condition == MotorCompletedCodes::COMMAND_PROCEED) {
            // handle the manual hardware inputs
            motor_rotation_activations manual_req = Gantry::getManualActivationRequestState(device_id);
            if (manual_req == motor_rotation_activations::MOTOR_INCREASE) {
                if (!activateManualPositioning(max_position)) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
            }
            else if (manual_req == motor_rotation_activations::MOTOR_DECREASE) {
                if (!activateManualPositioning(min_position)) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
            }
        }

    }
    
    
    // Initiate a requested command
    switch (request_command) {
    case MotorCommands::MOTOR_AUTO_POSITIONING:
        current_command = request_command;
        abort_request = false;
        if(idle_returned_condition == MotorCompletedCodes::COMMAND_PROCEED) manageAutomaticPositioning();              
        else setCommandCompletedCode(idle_returned_condition);
        request_command = MotorCommands::MOTOR_IDLE;
        break;

    case MotorCommands::MOTOR_MANUAL_POSITIONING:
        current_command = request_command;
        abort_request = false;
        if (idle_returned_condition == MotorCompletedCodes::COMMAND_PROCEED) manageManualPositioning();
        else setCommandCompletedCode(idle_returned_condition);
        
        request_command = MotorCommands::MOTOR_IDLE;
        break;

    case MotorCommands::MOTOR_HOMING:
        current_command = request_command;
        abort_request = false;
        if (
            (idle_returned_condition == MotorCompletedCodes::COMMAND_PROCEED) ||
            (idle_returned_condition == MotorCompletedCodes::ERROR_LIMIT_SWITCH) 
            )   manageAutomaticHoming();
        else setCommandCompletedCode(idle_returned_condition);
        
        request_command = MotorCommands::MOTOR_IDLE;
        break;

    case MotorCommands::MOTOR_IDLE:
        abort_request = false;
        break;

    }
        
    return;
}

/// <summary>
/// This function is called when the CiA402_OperationEnabled status is detected.
/// 
/// The Application cannot remains in this status out of a command execution 
/// because the motor wires are powered and a torque is applied.
/// 
/// The module tries to enter the CiA402_SwitchedOn from this status
/// 
/// </summary>
/// <param name=""></param>
void CanOpenMotor::CiA402_OperationEnabledCallback(void) {
    
    if (CiA_current_status != _CiA402Status::CiA402_OperationEnabled) {
        CiA_current_status = _CiA402Status::CiA402_OperationEnabled;
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: OPERATION ENABLED STATUS");
    }

    // The Motor cannot remain in this state without a command execution
    writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);
    return;
}


/// <summary>
/// This function handles the CiA_Fault status
/// 
/// The motor device enters a fault status whenever an intrnal fault condition is detected.
/// 
/// This module read the error code from the device and, 
/// as soon as the error condition expires, try to exit from the CiA_Fault status 
/// to the CiA_SwitchedOn status (operating status).
///
/// When a new error is generated or when the error condition expires, 
/// the module generate the event: fault_event(error_code)
/// 
///  
/// </summary>
/// <param name=""></param>
void CanOpenMotor::CiA402_FaultCallback(void) {
    bool data_changed = false;
    unsigned int ctrlw;
    

    if (CiA_current_status != _CiA402Status::CiA402_Fault) {
        CiA_current_status = _CiA402Status::CiA402_Fault;
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: FAULT STATUS");
    }

    // If a command execution is present the command shall be aborted!
    if (current_command != MotorCommands::MOTOR_IDLE) setCommandCompletedCode(MotorCompletedCodes::ERROR_INTERNAL_FAULT);
    
    // Read the error class register
    if (!blocking_readOD(OD_1001_00)) return;
    if (error_class != rxSdoRegister->data) data_changed = true;
    error_class = rxSdoRegister->data;

    // Read the error code register
    if (!blocking_readOD(OD_1003_01)) return;
    if (error_code != rxSdoRegister->data) data_changed = true;
    error_code = rxSdoRegister->data;

    if (error_class) {
        if(!error_condition) data_changed = true;
        error_condition = true;
        
        if(data_changed){            
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR CLASS = " + getErrorClass1001(error_class));
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR CODE = " + getErrorCode1003(error_code));            
        }       
        faultCallback( error_condition, data_changed, error_class, error_code);

        // If an attempt to activate the manual rotation is generated in fault condition an istant window will appear
        if ((Gantry::getManualActivationRequestState(device_id) == CANOPEN::CanOpenMotor::motor_rotation_activations::MOTOR_INCREASE) ||
            (Gantry::getManualActivationRequestState(device_id) == CANOPEN::CanOpenMotor::motor_rotation_activations::MOTOR_DECREASE))
            Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
        return;
    }

    faultCallback(false, false, (unsigned int)0, (unsigned int)0);

    // Tries to reset the error condition
    if (!blocking_readOD(OD_6040_00)) return;
    ctrlw = rxSdoRegister->data;
    ctrlw |= 0x80;
    if (!blocking_writeOD(OD_6040_00, ctrlw)) return;
    ctrlw &= ~0x80;
    if (!blocking_writeOD(OD_6040_00, ctrlw)) return;    
    return;
}

