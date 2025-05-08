#include "PCB325.h"
#include "Notify.h"
#include "Log.h"
#include <thread>
#include "ConfigurationFiles.h"



/// <summary>
/// This is the configuration loop routine executed at the beginning of the 
/// device connection, before to execute the runningLoop() routine.
/// 
/// 
/// </summary>
/// <param name=""></param>
/// <returns>true if the configuration success</returns>
bool PCB325::configurationLoop(void) {
    LogClass::logInFile("PCB325: CONFIGURED");
    return true;
}


void PCB325::runningLoop(void) {
    
    // The Biopsy device is not yet connected
    if (isCommunicationError()) {
        biopsy_connected = false;

        // Waits the connection with the biopsy polling ones per second the mode register
        protocol.status_register.decodeSystemRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::SYSTEM_STATUS));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return;
    }

    // The Biopsy is connected to the Gantry
    biopsy_connected = true;
    
    // The Status registers are download
    protocol.status_register.decodeSystemRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::SYSTEM_STATUS));
    protocol.status_register.decodeXYRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::SYSTEM_XY));
    protocol.status_register.decodeZSRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::SYSTEM_ZS));
    
    // Verifies if a change working mode is requested
    if (request_working_mode != biopsy_working_mode) {
        
        // a pending command shall terminate before
        if (!device->isCommandCompleted()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            return;
        }            

        biopsy_working_mode = request_working_mode;
        command_mode_ready = false;
        resetPositioningErrorMessages();
    }
    
    // The working mode of the Biopsy device can be forced to be one of the following
    switch (biopsy_working_mode) {
    case working_mode::BIOPSY_FREE_MODE: run_free_mode_workflow(); break;
    case working_mode::BIOPSY_DISABLE_MODE: run_disable_mode_workflow(); break;
    case working_mode::BIOPSY_CALIBRATION_MODE: run_calibration_mode_workflow(); break;
    case working_mode::BIOPSY_COMMAND_MODE: run_command_mode_workflow(); break;
    case working_mode::BIOPSY_SERVICE_MODE: run_service_mode_workflow(); break;
    }


}

void PCB325::resetPositioningErrorMessages(void) {
    Notify::deactivate(Notify::messages::ERROR_BIOPSY_X_POSITIONING);
    Notify::deactivate(Notify::messages::ERROR_BIOPSY_Y_POSITIONING);
    Notify::deactivate(Notify::messages::ERROR_BIOPSY_Z_POSITIONING);
    
}

bool PCB325::activateCycleTest(void) {
    service_cycle_test = true;
    return true;
}

void PCB325::run_free_mode_workflow(void) {

    // If the actual biopsy working mode is in disable or calibration mode
    // no more action shall be performed here.
    if (protocol.status_register.motor_working_mode == ProtocolStructure::StatusRegister::motor_mode::MOTOR_DISABLE_MODE) return;
    if (protocol.status_register.motor_working_mode == ProtocolStructure::StatusRegister::motor_mode::MOTOR_CALIBRATION_MODE) return;

    // If a command is executing wait the completion event
    if (!device->isCommandCompleted()) return;

    // If a different working mode is present, the MOTOR_DISABLE_MODE is activated 
    commandNoWaitCompletion(protocol.command.encodeDisableModeCommand(), 30);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return;
}

void PCB325::run_disable_mode_workflow(void) {
    // If the actual biopsy working mode is in disable mode
    // no more action shall be performed here.
    if (protocol.status_register.motor_working_mode == ProtocolStructure::StatusRegister::motor_mode::MOTOR_DISABLE_MODE) return;

    // If a command is executing wait the completion event
    if (!device->isCommandCompleted()) return;

    // If a different working mode is present, the MOTOR_DISABLE_MODE is activated 
    commandNoWaitCompletion(protocol.command.encodeDisableModeCommand(), 30);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return;
}

void PCB325::run_calibration_mode_workflow(void) {
    // If the actual biopsy working mode is in calibration mode
    // no more action shall be performed here.   
    if (protocol.status_register.motor_working_mode == ProtocolStructure::StatusRegister::motor_mode::MOTOR_CALIBRATION_MODE) return;

    // If a command is executing wait the completion event
    if (!device->isCommandCompleted()) return;

    // If a different working mode is present, the MOTOR_DISABLE_MODE is activated 
    commandNoWaitCompletion(protocol.command.encodeCalibModeCommand(), 30);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return;
}
void PCB325::run_command_mode_workflow(void) {

    if (!device->isCommandCompleted()) return;

    // The procedure forces the biopsy command mode to be activated
    if (protocol.status_register.motor_working_mode != ProtocolStructure::StatusRegister::motor_mode::MOTOR_COMMAND_MODE){    
        command_mode_ready = false;       
        commandNoWaitCompletion(protocol.command.encodeCommandModeCommand(), 30);   
        request_motor_command = motor_command::NO_COMMAND;
        waiting_motor_command = request_motor_command;
        if (motor_command_result == 0) {
            motor_command_result = -1;
            LogClass::logInFile("BIOPSY MOTOR ACTIVATION ABORTED DUE TO WORKING MODE CHANGE");
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return;
    }

    // Upgrades the Keystep mode
    if (keystep_mode != protocol.status_register.keystep_mode) {
        command_mode_ready = false;
        commandNoWaitCompletion(protocol.command.encodeKeyStepEnableCommand(keystep_mode), 30);
        request_motor_command = motor_command::NO_COMMAND;
        waiting_motor_command = request_motor_command;
        if (motor_command_result == 0) {
            motor_command_result = -1;
            LogClass::logInFile("BIOPSY MOTOR ACTIVATION ABORTED DUE TO WORKING MODE CHANGE");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return;
    }
    
    command_mode_ready = true;

    // Command execution handling
    if (request_motor_command != motor_command::NO_COMMAND) {
        waiting_motor_command = request_motor_command;
        

        if(request_motor_command == motor_command::MOTOR_X)
            commandNoWaitCompletion(protocol.command.encodeMoveXCommand(request_x), 200);
        else if (request_motor_command == motor_command::MOTOR_Y)
            commandNoWaitCompletion(protocol.command.encodeMoveYCommand(request_y), 200);
        else 
            commandNoWaitCompletion(protocol.command.encodeMoveZCommand(request_z), 200);

        request_motor_command = motor_command::NO_COMMAND;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return;
    }

    // Command verification
    if (waiting_motor_command != motor_command::NO_COMMAND) {
        if (device->isCommandError()) {
            motor_command_result = -1;
            LogClass::logInFile("BIOPSY MOTOR ACTIVATION FAILED");
            if(waiting_motor_command == motor_command::MOTOR_X) 
                Notify::activate(Notify::messages::ERROR_BIOPSY_X_POSITIONING);
            else if (waiting_motor_command == motor_command::MOTOR_Y)
                Notify::activate(Notify::messages::ERROR_BIOPSY_Y_POSITIONING);
            else if (waiting_motor_command == motor_command::MOTOR_Z)
                Notify::activate(Notify::messages::ERROR_BIOPSY_Z_POSITIONING);

        }
        else {
            motor_command_result = 1;
            LogClass::logInFile("BIOPSY MOTOR ACTIVATION SUCCESS");
            if (waiting_motor_command == motor_command::MOTOR_X)
                Notify::deactivate(Notify::messages::ERROR_BIOPSY_X_POSITIONING);
            else if (waiting_motor_command == motor_command::MOTOR_Y)
                Notify::deactivate(Notify::messages::ERROR_BIOPSY_Y_POSITIONING);
            else if (waiting_motor_command == motor_command::MOTOR_Z)
                Notify::deactivate(Notify::messages::ERROR_BIOPSY_Z_POSITIONING);
        }

        
        waiting_motor_command = motor_command::NO_COMMAND;
    }

}

void PCB325::run_service_mode_workflow(void) {

    // The procedure forces the biopsy service mode to be activated
    if (protocol.status_register.motor_working_mode != ProtocolStructure::StatusRegister::motor_mode::MOTOR_SERVICE_MODE) {
        if (!device->isCommandCompleted()) return;

        commandNoWaitCompletion(protocol.command.encodeServiceModeCommand(), 30);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return;
    }


}
