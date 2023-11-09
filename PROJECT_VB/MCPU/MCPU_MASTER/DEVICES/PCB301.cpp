#include "PCB301.h"
#include "../gantry_global_status.h"
#include <thread>


void PCB301::runningLoop(void) {
    static bool powerdown = false;
    static bool batt_ena = false;
    static bool batt_low = false; 
    static bool study_door_closed = false;

    bool bval;

    std::this_thread::sleep_for(std::chrono::microseconds(10000));

    //__________________________________________________________________________________________________________________ REGISTER STATUS
    while (!send(PCB301_GET_STATUS_SYSTEM_REGISTER)) ;

    // Power Down Condition Monitor
    bval = PCB301_GET_SYSTEM_POWERDOWN(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    if (bval != powerdown) {
        powerdown = bval;

        // Sets the general error
        if (powerdown) Notify::activate("POWER_DOWN_ERROR", false);
        else Notify::deactivate("POWER_DOWN_ERROR");
    }

    // Monitor of the Battery Enable input
    bval = PCB301_GET_SYSTEM_BATTENA(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    if (bval != batt_ena) {
        batt_ena = bval;
        if (batt_ena == false) {
            Notify::activate("BATTERY_DISABLED_INFO", false);
        }else Notify::deactivate("BATTERY_DISABLED_INFO");
    }
    
    // Monitor of the Battery Low error condition
    bool batt1_low, batt2_low;
    if (batt_ena == false) {
        // If the Batteries are not enabled, the low level is not monitored
        batt1_low = false;
        batt2_low = false;
    }
    else {
        batt1_low = PCB301_GET_SYSTEM_BATT1LOW(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
        batt2_low = PCB301_GET_SYSTEM_BATT2LOW(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    }
    if ((batt1_low || batt2_low) != batt_low) {
        batt_low = (batt1_low || batt2_low);
        if(batt_low) Notify::activate("BATTERY_LOW_ERROR", false);
        else  Notify::deactivate("BATTERY_LOW_ERROR");
    }

    // Monitor of the Study door input
    bval = PCB301_GET_SYSTEM_CLOSEDOOR(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    if (bval != study_door_closed) {
        study_door_closed = bval;

        if(study_door_closed) Notify::deactivate("DOOR_STUDY_OPEN_WARNING");
        else Notify::activate("DOOR_STUDY_OPEN_WARNING", false);
    }
    GantryStatusRegisters::SafetyStatusRegister::setCloseDoor(study_door_closed);


    std::this_thread::sleep_for(std::chrono::microseconds(10000));
    //__________________________________________________________________________________________________________________ REGISTER BATTERY
    while (!send(PCB301_GET_STATUS_BATTERY_REGISTER));
    unsigned char vbatt1 = PCB301_GET_BATTERY_VBATT1(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    unsigned char vbatt2 = PCB301_GET_BATTERY_VBATT2(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    GantryStatusRegisters::PowerStatusRegister::setBatteryData(batt_ena, batt1_low, batt2_low, vbatt1, vbatt2);
    

    return;
}
