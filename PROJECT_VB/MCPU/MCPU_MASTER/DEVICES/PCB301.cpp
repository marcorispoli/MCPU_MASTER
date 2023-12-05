#include "PCB301.h"
#include "Notify.h"
#include "awsProtocol.h"
#include "../gantry_global_status.h"
#include <thread>


void PCB301::runningLoop(void) {
    
    static bool study_door_closed = false;

    bool bval, b1val;

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    //__________________________________________________________________________________________________________________ REGISTER STATUS
    while (!send(PCB301_GET_STATUS_SYSTEM_REGISTER)) ;

    // Power Down Condition Monitor
    power_down_status = PCB301_GET_SYSTEM_POWERDOWN(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    if(power_down_status) Notify::activate(Notify::messages::ERROR_POWER_DOWN_ERROR, false);
    else Notify::deactivate(Notify::messages::ERROR_POWER_DOWN_ERROR);
    
    // Monitor of the Battery Enable input
    battery_enabled_status = PCB301_GET_SYSTEM_BATTENA(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    if (battery_enabled_status == false) Notify::activate(Notify::messages::INFO_BATTERY_DISABLED, false);
    else Notify::deactivate(Notify::messages::INFO_BATTERY_DISABLED);
             
    // Monitor of the Battery Low error condition
    if (battery_enabled_status == false) {
        // If the Batteries are not enabled, the low level is not monitored
        batt1_low_alarm = false;
        batt2_low_alarm = false;
    }
    else {
        batt1_low_alarm = PCB301_GET_SYSTEM_BATT1LOW(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
        batt2_low_alarm = PCB301_GET_SYSTEM_BATT2LOW(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    }

    // Handles the battery alarm
    if (batt1_low_alarm || batt2_low_alarm) Notify::activate(Notify::messages::ERROR_BATTERY_LOW_ERROR, false);
    else Notify::deactivate(Notify::messages::ERROR_BATTERY_LOW_ERROR);
    
    // Monitor of the Study door input
    if (PCB301_GET_SYSTEM_CLOSEDOOR(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6)) {
        Notify::deactivate(Notify::messages::WARNING_DOOR_STUDY_OPEN);
        door_status = door_options::CLOSED_DOOR;
    }
    else {
        door_status = door_options::OPEN_DOOR;
        Notify::activate(Notify::messages::WARNING_DOOR_STUDY_OPEN, false);
    }
        
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

    // handles the X-RAY puss button
    if ((PCB301_GET_XRAY_PUSH_BUTTON(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6) != xray_push_button)) {
        xray_push_button = PCB301_GET_XRAY_PUSH_BUTTON(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
        if(xray_push_button_event_enable) awsProtocol::EVENT_XrayPushButton(xray_push_button);
    }


    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //__________________________________________________________________________________________________________________ REGISTER BATTERY
    while (!send(PCB301_GET_STATUS_BATTERY_REGISTER));
    voltage_batt1 = PCB301_GET_BATTERY_VBATT1(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    voltage_batt2 = PCB301_GET_BATTERY_VBATT2(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    

    return;
}
