#include "PCB301.h"
#include "../gantry_global_status.h"
#include <thread>


void PCB301::runningLoop(void) {
    static bool powerdown = false;
    static bool batt_ena = false;
    static bool batt_low = false; 
    static bool study_door_closed = false;

    bool bval, b1val;

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    //__________________________________________________________________________________________________________________ REGISTER STATUS
    while (!send(PCB301_GET_STATUS_SYSTEM_REGISTER)) ;

    // Power Down Condition Monitor
    bval = PCB301_GET_SYSTEM_POWERDOWN(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    if (bval != powerdown) {
        powerdown = bval;

        // Sets the general error
        if (powerdown) Notify::activate(Notify::messages::ERROR_POWER_DOWN_ERROR, false);
        else Notify::deactivate(Notify::messages::ERROR_POWER_DOWN_ERROR);
    }

    // Monitor of the Battery Enable input
    bval = PCB301_GET_SYSTEM_BATTENA(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    if (bval != batt_ena) {
        batt_ena = bval;
        if (batt_ena == false) {
            Notify::activate(Notify::messages::INFO_BATTERY_DISABLED, false);
        }else Notify::deactivate(Notify::messages::INFO_BATTERY_DISABLED);
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
        if(batt_low) Notify::activate(Notify::messages::ERROR_BATTERY_LOW_ERROR, false);
        else  Notify::deactivate(Notify::messages::ERROR_BATTERY_LOW_ERROR);
    }

    // Monitor of the Study door input
    bval = PCB301_GET_SYSTEM_CLOSEDOOR(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    if (bval != study_door_closed) {
        study_door_closed = bval;

        if(study_door_closed) Notify::deactivate(Notify::messages::WARNING_DOOR_STUDY_OPEN);
        else Notify::activate(Notify::messages::WARNING_DOOR_STUDY_OPEN, false);
    }
    GantryStatusRegisters::SafetyStatusRegister::setCloseDoor(study_door_closed);

    // Handles the manual vertical activation request
    bool up_stat = PCB301_GET_VERTICAL_UP(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    bool down_stat = PCB301_GET_VERTICAL_DOWN(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);

    if (up_stat && down_stat) vertical_activation_status = vertical_activation_options::VERTICAL_INVALID_CODE;
    else if ((up_stat) && (!down_stat)) vertical_activation_status = vertical_activation_options::VERTICAL_UP_ACTIVATION;
    else if ((!up_stat) && (down_stat)) vertical_activation_status = vertical_activation_options::VERTICAL_DOWN_ACTIVATION;
    else vertical_activation_status = vertical_activation_options::VERTICAL_NO_ACTIVATION;

    // Handles the manual body activation request
    bool cw_stat = PCB301_GET_BODY_CW(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    bool ccw_stat = PCB301_GET_BODY_CCW(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);

    if (cw_stat && ccw_stat) body_activation_status = body_activation_options::BODY_INVALID_CODE;
    else if ((cw_stat) && (!ccw_stat)) body_activation_status = body_activation_options::BODY_CW_ACTIVATION;
    else if ((!cw_stat) && (ccw_stat)) body_activation_status = body_activation_options::BODY_CCW_ACTIVATION;
    else body_activation_status = body_activation_options::BODY_NO_ACTIVATION;


    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //__________________________________________________________________________________________________________________ REGISTER BATTERY
    while (!send(PCB301_GET_STATUS_BATTERY_REGISTER));
    unsigned char vbatt1 = PCB301_GET_BATTERY_VBATT1(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    unsigned char vbatt2 = PCB301_GET_BATTERY_VBATT2(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    GantryStatusRegisters::PowerStatusRegister::setBatteryData(batt_ena, batt1_low, batt2_low, vbatt1, vbatt2);
    

    return;
}
