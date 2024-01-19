
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
    send(PCB304_GET_STATUS_SYSTEM_REGISTER);
    std::this_thread::sleep_for(std::chrono::microseconds(10000));
    return;
}
