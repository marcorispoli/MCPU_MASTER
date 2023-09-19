#include "pch.h"
#include "PCB301.h"

#include <thread>

void PCB301::runningLoop(void) {

    if (!send(GET_STATUS_SYSTEM_REGISTER)) {
        Debug::WriteLine("Device Board <" + Convert::ToString(0x10) + ">: TMO");
        return;
    }

    bool powerdown = GET_SYSTEM_POWERDOWN(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    bool batt_ena = GET_SYSTEM_BATTENA(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    bool batt1_low = GET_SYSTEM_BATT1LOW(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    bool batt2_low = GET_SYSTEM_BATT2LOW(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    bool close_door = GET_SYSTEM_CLOSEDOOR(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);

    std::this_thread::sleep_for(std::chrono::microseconds(10000));

    if (!send(GET_STATUS_BATTERY_REGISTER)) return;

    unsigned char vbatt1 = GET_BATTERY_VBATT1(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    unsigned char vbatt2 = GET_BATTERY_VBATT2(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);


    GantryStatusRegisters::PowerStatusRegister::setPowerdown(powerdown);
    GantryStatusRegisters::PowerStatusRegister::setBatteryData(batt_ena, batt1_low, batt2_low, vbatt1, vbatt2);
    GantryStatusRegisters::SafetyStatusRegister::setCloseDoor(close_door);

    std::this_thread::sleep_for(std::chrono::microseconds(100000));

    return;
}
