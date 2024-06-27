
#include "PCB304.h"
#include "Notify.h"
#include <thread>

void PCB304::runningLoop(void) {
    static bool commerr = false;
    static bool keepalive = false;
 
    // Test the communication status
    if (commerr != isCommunicationError()) {
        commerr = isCommunicationError();
        if (isCommunicationError()) {
            Notify::activate(Notify::messages::ERROR_PCB304_COMMUNICATION_ERROR);
        }
        else {
            Notify::deactivate(Notify::messages::ERROR_PCB304_COMMUNICATION_ERROR);
        }
    }

    // Read the STATUS register    
    protocol.status_register.decodeSystemRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::SYSTEM_REGISTER));

   
    // To be completed
    if (protocol.status_register.grid_sync_stat) {
        protocol.status_register.grid_on_field_ready = true;
        protocol.status_register.grid_off_field_ready = false;
    }
    else {
        protocol.status_register.grid_on_field_ready = false;
        protocol.status_register.grid_off_field_ready = true;
    }
    

    // Always toggles the keepalive to keep the display ON
    protocol.data_register.display_keep_alive = !protocol.data_register.display_keep_alive;


    // Sets the display outputs
    writeDataRegister((unsigned char)ProtocolStructure::DataRegister::register_index::DISPLAY_REGISTER, protocol.data_register.encodeDisplayRegister());
    std::this_thread::sleep_for(std::chrono::milliseconds(10));


    // Udates the DISPLAY GRID REGISTER
    writeDataRegister((unsigned char)ProtocolStructure::DataRegister::register_index::GRID_REGISTER, protocol.data_register.encodeGridRegister());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return;
}



// To be done
bool PCB304::setGridOnField(bool wait_completion) { 
    protocol.status_register.grid_on_field = true;

    return true;

}

// To be done
bool PCB304::setGridOffField(bool wait_completion) {
    protocol.status_register.grid_on_field = false;

    return true;
}

void PCB304::demoLoop(void) {

    if (protocol.status_register.grid_on_field) protocol.status_register.grid_on_field_ready = true;
    else protocol.status_register.grid_off_field_ready = true;

    protocol.status_register.grid_sync_stat = true;
    

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return;
}
