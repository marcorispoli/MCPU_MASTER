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
        protocol.status_register.decodePositionRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::POSITION_REGISTER));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
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

   
    return;
}

bool PCB325::activateCycleTest(void) {
    service_cycle_test = true;
    return true;
}
