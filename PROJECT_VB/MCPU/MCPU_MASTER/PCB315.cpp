#include "pch.h"
#include "PCB315.h"

#include <thread>

void PCB315::runningLoop(void) {

    while (!send(GET_STATUS_SYSTEM_REGISTER));

    unsigned char filters =  GET_SYSTEM_FILTER(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    unsigned char stator = GET_SYSTEM_STATOR(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    unsigned char bulb = GET_SYSTEM_BULB(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    GantryStatusRegisters::TubeDataRegister::setTubeTemp(stator, bulb);


    std::this_thread::sleep_for(std::chrono::microseconds(10000));

    return;
}
