#include "PCB301.h"
#include "Notify.h"
#include "awsProtocol.h"
#include "../gantry_global_status.h"
#include <thread>


void PCB301::handleSystemStatusRegister(void) {
    
    Register^ system_status_register = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_STATUS_REGISTER);
    if (system_status_register == nullptr) return;

    // Power Down Condition Monitor
    if (PCB301_GET_SYSTEM_POWERDOWN(system_status_register) != power_down_status) {
        power_down_status = PCB301_GET_SYSTEM_POWERDOWN(system_status_register);

        if (power_down_status) Notify::activate(Notify::messages::ERROR_POWER_DOWN_ERROR, false);
        else Notify::deactivate(Notify::messages::ERROR_POWER_DOWN_ERROR);
    }

    // Monitor of the Battery Enable input
    if (PCB301_GET_SYSTEM_BATTENA(system_status_register) != battery_enabled_status) {
        battery_enabled_status = PCB301_GET_SYSTEM_BATTENA(system_status_register);
        if (battery_enabled_status == false) Notify::activate(Notify::messages::INFO_BATTERY_DISABLED, false);
        else Notify::deactivate(Notify::messages::INFO_BATTERY_DISABLED);
    }
    
   
    // Monitor of the Battery Low error condition
    if (battery_enabled_status == false) {

        bool batt_alarm = batt1_low_alarm || batt2_low_alarm;
        if (batt_alarm) {
            batt1_low_alarm = false;
            batt2_low_alarm = false;
            Notify::deactivate(Notify::messages::ERROR_BATTERY_LOW_ERROR);
        }
        
    }
    else {
        bool batt_alarm = batt1_low_alarm || batt2_low_alarm;
        batt1_low_alarm = PCB301_GET_SYSTEM_BATT1LOW(system_status_register);
        batt2_low_alarm = PCB301_GET_SYSTEM_BATT2LOW(system_status_register);
        if ((batt1_low_alarm || batt2_low_alarm) != batt_alarm) {
            if(batt1_low_alarm || batt2_low_alarm) Notify::activate(Notify::messages::ERROR_BATTERY_LOW_ERROR, false);
            else Notify::deactivate(Notify::messages::ERROR_BATTERY_LOW_ERROR);
        }
    }
    

    // Monitor of the Study door input
    if (Gantry::isDemo()) {
        if (door_status != door_options::CLOSED_DOOR) {
            door_status = door_options::CLOSED_DOOR;
            Notify::deactivate(Notify::messages::WARNING_DOOR_STUDY_OPEN);
        }
        
    }else {   
        door_options door_closed_status;
        if (PCB301_GET_SYSTEM_CLOSEDOOR(system_status_register)) door_closed_status = door_options::CLOSED_DOOR;
        else door_closed_status = door_options::OPEN_DOOR;

        if (door_closed_status != door_status) {
            door_status = door_closed_status;
            if (door_status == door_options::CLOSED_DOOR) Notify::deactivate(Notify::messages::WARNING_DOOR_STUDY_OPEN);
            else Notify::activate(Notify::messages::WARNING_DOOR_STUDY_OPEN, false);
        }
    }

    

    // Handles the motor manual inputs
    button_up_stat = PCB301_GET_BUTTON_VERTICAL_UP(system_status_register);
    button_down_stat = PCB301_GET_BUTTON_VERTICAL_DOWN(system_status_register);
    pedal_up_stat = PCB301_GET_PEDAL_VERTICAL_UP(system_status_register);
    pedal_down_stat = PCB301_GET_PEDAL_VERTICAL_DOWN(system_status_register);

    button_body_cw = PCB301_GET_BUTTON_BODY_CW(system_status_register);
    button_body_ccw = PCB301_GET_BUTTON_BODY_CCW(system_status_register);

    button_arm_cw_stat = PCB301_GET_BUTTON_ARM_CW(system_status_register);
    button_arm_ccw_stat = PCB301_GET_BUTTON_ARM_CCW(system_status_register);

    button_slide_up_stat = PCB301_GET_BUTTON_SLIDE_UP(system_status_register);
    button_slide_down_stat = PCB301_GET_BUTTON_SLIDE_DOWN(system_status_register);

    // handles the X-RAY push button
    if ((PCB301_GET_XRAY_PUSH_BUTTON(system_status_register) != xray_push_button)) {
        xray_push_button = PCB301_GET_XRAY_PUSH_BUTTON(system_status_register);
        if (xray_push_button_event_enable) awsProtocol::EVENT_XrayPushButton(xray_push_button);
    }

}

void PCB301::handleBatteryStatusRegister(void) {
    Register^ battery_status_register = readStatusRegister((unsigned char) StatusRegisters::BATTERY_STATUS_REGISTER);
    if (battery_status_register == nullptr) return;

    voltage_batt1 = PCB301_GET_BATTERY_VBATT1(battery_status_register);
    voltage_batt2 = PCB301_GET_BATTERY_VBATT2(battery_status_register);

}

void PCB301::runningLoop(void) {
    static int count = 0;

   

    handleSystemStatusRegister();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    handleBatteryStatusRegister();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Refresh the Data register
    writeDataRegister((unsigned char)DataRegisters::OUTPUTS_DATA_REGISTER, outputs_data_register);
   
    return;
}
