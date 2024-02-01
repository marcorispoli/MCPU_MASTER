#include "PCB301.h"
#include "./DEMO/PCB301_Demo.h"

#include "Notify.h"
#include "awsProtocol.h"
#include "../gantry_global_status.h"
#include <thread>

void PCB301::evaluateEvents(void) {
    static bool init = true;
    static bool power_down = false;
    static bool emergency = false;
    static bool cabinet = false;
    static bool motor_switch = false;
    static bool motor_supply = false;
    static bool burning_jumper = false;
    static bool battery_enabled = false;
    static bool batt_alarm = false;
    static door_options door = door_options::CLOSED_DOOR;
    static bool xray_push = false;

    // Initializes the events
    if (init) {
        init = false;
        Notify::deactivate(Notify::messages::ERROR_POWER_DOWN_ERROR);
        Notify::deactivate(Notify::messages::ERROR_CABINET_SAFETY);
        Notify::deactivate(Notify::messages::ERROR_EMERGENCY_BUTTON);
        Notify::deactivate(Notify::messages::INFO_MOTOR_48V_SAFETY_LINE_OFF);
        Notify::deactivate(Notify::messages::INFO_MOTOR_48V_POWER_SUPPLY_OFF);
        Notify::deactivate(Notify::messages::INFO_BURNING_JUMPER_PRESENT);
        Notify::deactivate(Notify::messages::INFO_BATTERY_DISABLED);
        Notify::deactivate(Notify::messages::ERROR_BATTERY_LOW_ERROR);
        Notify::deactivate(Notify::messages::WARNING_DOOR_STUDY_OPEN);
        return;
    }

    // Power Down Condition Monitor Event
    if (power_down != power_down_status) {
        power_down = power_down_status;
        if (power_down_status) Notify::activate(Notify::messages::ERROR_POWER_DOWN_ERROR);
        else Notify::deactivate(Notify::messages::ERROR_POWER_DOWN_ERROR);
    }

    // Emergency
    if (emergency != emergency_status) {
        emergency = emergency_status;

        if (emergency_status) Notify::activate(Notify::messages::ERROR_EMERGENCY_BUTTON);
        else Notify::deactivate(Notify::messages::ERROR_EMERGENCY_BUTTON);
    }

    // Cabinet safety
    if (cabinet != cabinet_safety_status) {
        cabinet = cabinet_safety_status;

        if (cabinet_safety_status) Notify::activate(Notify::messages::ERROR_CABINET_SAFETY);
        else Notify::deactivate(Notify::messages::ERROR_CABINET_SAFETY);
    }

    // Motor Switch
    if (motor_switch != motor_safety_switch) {
        motor_switch = motor_safety_switch;

        if (!motor_safety_switch) Notify::activate(Notify::messages::INFO_MOTOR_48V_SAFETY_LINE_OFF);
        else Notify::deactivate(Notify::messages::INFO_MOTOR_48V_SAFETY_LINE_OFF);
    }

    // Motor Power Supply
    if (motor_supply != motor_48V_ok) {
        motor_supply = motor_48V_ok;

        if (!motor_48V_ok) Notify::activate(Notify::messages::INFO_MOTOR_48V_POWER_SUPPLY_OFF);
        else Notify::deactivate(Notify::messages::INFO_MOTOR_48V_POWER_SUPPLY_OFF);
    }

    // Burning Jumper presence
    if (burning_jumper != burning_jumper_present) {
        burning_jumper != burning_jumper_present;

        if (burning_jumper_present) Notify::activate(Notify::messages::INFO_BURNING_JUMPER_PRESENT);
        else Notify::deactivate(Notify::messages::INFO_BURNING_JUMPER_PRESENT);
    }

    // Monitor of the Battery Enable input
    if (battery_enabled != battery_enabled_status) {
        battery_enabled != battery_enabled_status;

        if (battery_enabled_status == false) Notify::activate(Notify::messages::INFO_BATTERY_DISABLED);
        else Notify::deactivate(Notify::messages::INFO_BATTERY_DISABLED);
    }

    // Monitor of the Battery Low error condition
    if (battery_enabled_status == false) {
        if (batt_alarm) {
            batt_alarm = false;
            Notify::deactivate(Notify::messages::ERROR_BATTERY_LOW_ERROR);
        }
    }
    else {
        if ((batt1_low_alarm || batt2_low_alarm) != batt_alarm) {
            batt_alarm = (batt1_low_alarm || batt2_low_alarm);
            if (batt_alarm) Notify::activate(Notify::messages::ERROR_BATTERY_LOW_ERROR);
            else Notify::deactivate(Notify::messages::ERROR_BATTERY_LOW_ERROR);
        }
    }

    // Monitor of the Study door input
    if (door != door_status) {
        door = door_status;

        if (door_status == door_options::CLOSED_DOOR) Notify::deactivate(Notify::messages::WARNING_DOOR_STUDY_OPEN);
        else Notify::activate(Notify::messages::WARNING_DOOR_STUDY_OPEN);
    }


    // handles the X-RAY push button event
    if (xray_push != xray_push_button) {
        xray_push = xray_push_button;
        if (ExposureModule::getXrayPushButtonEvent()) awsProtocol::EVENT_XrayPushButton(xray_push_button);
    }
}

void PCB301::handleSystemStatusRegister(void) {
    
    Register^ system_status_register = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_STATUS_REGISTER);
    if (system_status_register == nullptr) return;

    // Power Down Condition Monitor
    power_down_status = PCB301_GET_SYSTEM_POWERDOWN(system_status_register);
    emergency_status = PCB301_GET_SYSTEM_EMERGENCY_STATUS(system_status_register);
    cabinet_safety_status = PCB301_GET_SYSTEM_CABINET_SAFETY(system_status_register);
    motor_safety_switch = PCB301_GET_SYSTEM_MOTOR_SWITCH(system_status_register);
    motor_48V_ok = PCB301_GET_SYSTEM_MOTOR_OK(system_status_register);
    compression_on_status = PCB301_GET_SYSTEM_COMPRESSION(system_status_register);
    burning_jumper_present = PCB301_GET_SYSTEM_BURNING_JMP(system_status_register);
    soft_power_off_request = PCB301_GET_SYSTEM_POWER_OFF_REQ(system_status_register);
    battery_enabled_status = PCB301_GET_SYSTEM_BATTENA(system_status_register);
    batt1_low_alarm = PCB301_GET_SYSTEM_BATT1LOW(system_status_register);
    batt2_low_alarm = PCB301_GET_SYSTEM_BATT2LOW(system_status_register);

    if (Gantry::isOperatingDemo()) {
        door_status = door_options::CLOSED_DOOR;
    }
    else {
        if (PCB301_GET_SYSTEM_CLOSEDOOR(system_status_register)) door_status = door_options::CLOSED_DOOR;
        else door_status = door_options::OPEN_DOOR;
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

     xray_push_button = PCB301_GET_XRAY_PUSH_BUTTON(system_status_register);
    
}

void PCB301::handleBatteryStatusRegister(void) {
    Register^ battery_status_register = readStatusRegister((unsigned char) StatusRegisters::BATTERY_STATUS_REGISTER);
    if (battery_status_register == nullptr) return;

    voltage_batt1 = PCB301_GET_BATTERY_VBATT1(battery_status_register);
    voltage_batt2 = PCB301_GET_BATTERY_VBATT2(battery_status_register);

}

void PCB301::toggleKeepalive(void) {
    static bool stat = false;

    if (stat) stat = false;
    else stat = true;

    PCB301_OUTPUTS_DATA_KEEP_ALIVE(outputs_data_register, stat);
    

}
void PCB301::runningLoop(void) {
    static int count = 0;
    static bool commerr = false;

    // Test the communication status
    if (commerr != isCommunicationError()) {
        commerr = isCommunicationError();
        if (isCommunicationError()) {
            Notify::activate(Notify::messages::ERROR_PCB301_COMMUNICATION_ERROR);
        }
        else {
            Notify::deactivate(Notify::messages::ERROR_PCB301_COMMUNICATION_ERROR);
        }
    }

    handleSystemStatusRegister();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    handleBatteryStatusRegister();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Toggles the keepalive bit to keep the board alive!
    toggleKeepalive();

    // Refresh the Data register
    writeDataRegister((unsigned char)DataRegisters::OUTPUTS_DATA_REGISTER, outputs_data_register);
   
    evaluateEvents();

    return;
}




void PCB301::demoLoop(void) {
    if (!DemoPcb301::Configuration->loadFile()) {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        return;
    }
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    

    // Power Down Condition Monitor
    if (DemoPcb301::Configuration->getParam(DemoPcb301::PARAM_POWERDOWN_STAT)[0] == "1") power_down_status = true;
    else power_down_status = false;
   
    // Emergency
    if (DemoPcb301::Configuration->getParam(DemoPcb301::PARAM_EMERGENCY_STAT)[0] == "1") emergency_status = true;
    else emergency_status = false;
   

    // Cabinet safety
    if (DemoPcb301::Configuration->getParam(DemoPcb301::PARAM_CABINET_STAT)[0] == "1") cabinet_safety_status = true;
    else cabinet_safety_status = false;

    // Door closed
    if (DemoPcb301::Configuration->getParam(DemoPcb301::PARAM_DOOR_STAT)[0] == "1") door_status = door_options::OPEN_DOOR;
    else door_status = door_options::CLOSED_DOOR;

    // Compressor On
    if (DemoPcb301::Configuration->getParam(DemoPcb301::PARAM_CMP_ON_STAT)[0] == "1") compression_on_status = true;
    else compression_on_status = false;
    
    
    // handles the X-RAY push button    
    if (DemoPcb301::Configuration->getParam(DemoPcb301::PARAM_XRAY_BUTTON_STAT)[0] == "1") xray_push_button = true;
    else xray_push_button = false;

    // Motor 48V
    if (emergency_status || power_down_status) {
        motor_48V_ok = false;
    }else motor_48V_ok = true;

    // Motor safety swittch
    if (compression_on_status || cabinet_safety_status) {
        motor_safety_switch = false;
    }else motor_safety_switch = true;

    // Pedal Up/Down
    if (DemoPcb301::Configuration->getParam(DemoPcb301::PARAM_PEDAL_UP_STAT)[0] == "1") pedal_up_stat = true;
    else pedal_up_stat = false;
    if (DemoPcb301::Configuration->getParam(DemoPcb301::PARAM_PEDAL_DOWN_STAT)[0] == "1") pedal_down_stat = true;
    else pedal_down_stat = false;


    burning_jumper_present = false;
    soft_power_off_request = false;
    battery_enabled_status = false;
    batt1_low_alarm = false;
    batt2_low_alarm = false;
 
    button_up_stat = false;
    button_down_stat = false;    
    button_body_cw = false;
    button_body_ccw = false;
    button_arm_cw_stat = false;
    button_arm_ccw_stat = false;
    button_slide_up_stat = false;
    button_slide_down_stat = false;
    voltage_batt1 = 120;
    voltage_batt2 = 120;

    evaluateEvents();
}