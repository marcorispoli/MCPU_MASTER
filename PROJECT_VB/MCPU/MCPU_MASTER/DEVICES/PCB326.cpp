#include "PCB326.h"
#include "Notify.h"
#include <thread>

void PCB326::runningLoop(void) {
    static bool commerr = false;

    // Test the communication status
    if (commerr != isCommunicationError()) {
        commerr = isCommunicationError();
        if (isCommunicationError()) {
            Notify::activate(Notify::messages::ERROR_PCB326_COMMUNICATION_ERROR);
        }
        else {
            Notify::deactivate(Notify::messages::ERROR_PCB326_COMMUNICATION_ERROR);
        }
    }

    /*
    while (!send(GET_STATUS_SYSTEM_REGISTER));

    unsigned char filters =  GET_SYSTEM_FILTER(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    unsigned char stator = GET_SYSTEM_STATOR(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    unsigned char bulb = GET_SYSTEM_BULB(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    GantryStatusRegisters::TubeDataRegister::setTubeTemp(stator, bulb);

    */
    std::this_thread::sleep_for(std::chrono::microseconds(10000));

    return;
}
