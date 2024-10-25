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
        
    
    if (!isCommunicationError()) {
        biopsy_connected = true;
        protocol.status_register.decodeModeRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::MODE_REGISTER));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Writes the data General register
        writeDataRegister((unsigned char)ProtocolStructure::DataRegister::register_index::GENERAL_REGISTER, protocol.data_register.encodeGeneralRegister());

        
    }
    else {
        biopsy_connected = false;
        // Waits the connection with the biopsy polling ones per second the mode register
        protocol.status_register.decodeModeRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::MODE_REGISTER));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        
    }    

    if (service_cycle_test) {
        if (protocol.status_register.mode != ProtocolStructure::StatusRegister::working_mode::SERVICE_MODE) {
            commandNoWaitCompletion(protocol.command.encodeServiceModeCommand(), 30);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return;
        }

        commandNoWaitCompletion(protocol.command.encodeTestCycleModeCommand(), 30);
        service_cycle_test = false;       
    }

  
    if (command_home_request) {
        if (protocol.status_register.mode != ProtocolStructure::StatusRegister::working_mode::COMMAND_MODE) {
            commandNoWaitCompletion(protocol.command.encodeCommandModeCommand(), 30);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            sequence = 0;
            return;
        }

        switch (sequence) {
        case 0:
            
            commandNoWaitCompletion(protocol.command.encodeMoveYCommand(System::Convert::ToInt16(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_PARK)[BiopsyConfig::PARAM_PARK_Y])), 30);
            sequence++;
            break;
        case 1:
            if (device->isCommandError()) {
                sequence = 0;
                command_home_request = 0;
            }
            else if (device->isCommandCompleted()) {
                sequence++;
                commandNoWaitCompletion(protocol.command.encodeMoveXCommand(System::Convert::ToInt16(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_PARK)[BiopsyConfig::PARAM_PARK_X])), 30);
            }
            break;
        case 2:
            if (device->isCommandError()) {
                sequence = 0;
                command_home_request = 0;
            }
            else if (device->isCommandCompleted()) {
                sequence++;
                commandNoWaitCompletion(protocol.command.encodeMoveZCommand(System::Convert::ToInt16(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_PARK)[BiopsyConfig::PARAM_PARK_Z])), 30);
            }
            break;
        case 3:
            if (device->isCommandError()) {
                sequence = 0;
                command_home_request = 0;
            }
            else if (device->isCommandCompleted()) {
                sequence = 0;
                command_home_request = 0;
            }

            commandNoWaitCompletion(protocol.command.encodeDisableModeCommand(), 30);
            break;
        }
        

       
    }
    
    return;
}

bool PCB325::activateCycleTest(void) {
    service_cycle_test = true;
    return true;
}
