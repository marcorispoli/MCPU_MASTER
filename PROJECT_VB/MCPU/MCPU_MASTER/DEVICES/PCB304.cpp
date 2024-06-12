
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
    Register^ system_status_register = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_STATUS_REGISTER);
    if (system_status_register) {
        grid_sync_ready = PCB304_GET_GRID_SYNC_STATUS(system_status_register);
    }

    // To be completed
    if (grid_sync_ready) {
        grid_on_field_ready = true;
        grid_off_field_ready = false;
    }
    else {
        grid_on_field_ready = false;
        grid_off_field_ready = true;
    }
    

    // Always toggles the keepalive to keep the display ON
    if(keepalive)   PCB304_DISPLAY_KEEPALIVE(display_data_register, true); 
    else  PCB304_DISPLAY_KEEPALIVE(display_data_register, false); 
    keepalive = !keepalive;

    // Sets the display outputs
    PCB304_DISPLAY_ON(display_data_register, display_on);
    PCB304_DISPLAY_BLINK(display_data_register, display_blink);
    PCB304_DISPLAY_DOT_POSITION(display_data_register, display_decimals);
    PCB304_DISPLAY_INTENSITY(display_data_register, display_intensity);
    
    short val = display_val;
    PCB304_DISPLAY_CONTENT_LOW(display_data_register, val);
    PCB304_DISPLAY_CONTENT_HIGH(display_data_register, val);
 
    // Udates the DISPLAY DATA REGISTER
    writeDataRegister((unsigned char)DataRegisters::DISPLAY_DATA_REGISTER, display_data_register);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Udates the DISPLAY GRID REGISTER
    writeDataRegister((unsigned char)DataRegisters::GRID_DATA_REGISTER, grid_data_register);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return;
}




bool PCB304::setGridOnField(bool wait_completion) { 
    grid_on_field = true; 

    // Returns immediatelly if the function shall not wait for the command completion
    if (!wait_completion) return true;

    int timeout = 50;
    while (timeout--) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        if (PCB304::isError()) {
            LogClass::logInFile("PCB304: error during On field grid positioning");
            return false;
        }

        if (!PCB304::isGridOnFieldReady()) continue;
        
        // Grin On field
        return true;
    }

    LogClass::logInFile("PCB304: timeout waiting for the grid On Field");
    return false;


}

bool PCB304::setGridOffField(bool wait_completion) {
    grid_on_field = false; 

    // Returns immediatelly if the function shall not wait for the command completion
    if (!wait_completion) return true;

    int timeout = 50;
    while (timeout--) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        if (PCB304::isError()) {
            LogClass::logInFile("PCB304: error during OFF field grid positioning");
            return false;
        }

        if (!PCB304::isGridOffFieldReady()) continue;

        // Grin Off field
        return true;
    }

    LogClass::logInFile("PCB304: timeout waiting for the grid OFF Field");
    return false;

}

void PCB304::demoLoop(void) {

    if (grid_on_field) grid_on_field_ready = true;
    else grid_off_field_ready = true;

    grid_sync_ready = true;
    error = false;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return;
}
