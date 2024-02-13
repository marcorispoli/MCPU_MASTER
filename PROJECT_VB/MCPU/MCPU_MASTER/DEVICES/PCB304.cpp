
#include "PCB304.h"
#include "Notify.h"

#include <thread>

void PCB304::runningLoop(void) {
    static bool commerr = false;
 
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


   
    // Display setting
    PCB304_DISPLAY_KEEPALIVE(display_data_register, true); // Always true to prevent the auto display off of the device

    PCB304_DISPLAY_ON(display_data_register, display_on);
    PCB304_DISPLAY_BLINK(display_data_register, display_blink);
    PCB304_DISPLAY_DOT_POSITION(display_data_register, display_decimals);
    PCB304_DISPLAY_INTENSITY(display_data_register, display_intensity);
    
    short val = display_val;
    PCB304_DISPLAY_CONTENT_LOW(display_data_register, val);
    PCB304_DISPLAY_CONTENT_HIGH(display_data_register, val);
 
    writeDataRegister((unsigned char)DataRegisters::DISPLAY_DATA_REGISTER, display_data_register);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return;
}
