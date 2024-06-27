#include "PCB301.h"
#include "ExposureModule.h"
#include "Notify.h"
#include "awsProtocol.h"
#include "../gantry_global_status.h"
#include "Log.h"
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
    static bool closed_door = false;
    static bool xray_push = false;
    static bool soft_power_off = false;

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
    if (power_down != protocol.status_register.system_power_down) {
        power_down = protocol.status_register.system_power_down;
        if (power_down) Notify::activate(Notify::messages::ERROR_POWER_DOWN_ERROR);
        else Notify::deactivate(Notify::messages::ERROR_POWER_DOWN_ERROR);
    }

    // Emergency
    if (emergency != protocol.status_register.system_emergency) {
        emergency = protocol.status_register.system_emergency;

        if (protocol.status_register.system_emergency) Notify::activate(Notify::messages::ERROR_EMERGENCY_BUTTON);
        else Notify::deactivate(Notify::messages::ERROR_EMERGENCY_BUTTON);
    }

    // Cabinet safety
    if (cabinet != protocol.status_register.cabinet_safety_alarm) {
        cabinet = protocol.status_register.cabinet_safety_alarm;

        if (protocol.status_register.cabinet_safety_alarm) Notify::activate(Notify::messages::ERROR_CABINET_SAFETY);
        else Notify::deactivate(Notify::messages::ERROR_CABINET_SAFETY);
    }

    // Motor Switch
    if (motor_switch != protocol.status_register.motor_safety_switch) {
        motor_switch = protocol.status_register.motor_safety_switch;

        if (!protocol.status_register.motor_safety_switch) Notify::activate(Notify::messages::INFO_MOTOR_48V_SAFETY_LINE_OFF);
        else Notify::deactivate(Notify::messages::INFO_MOTOR_48V_SAFETY_LINE_OFF);
    }

    // Motor Power Supply
    if (motor_supply != protocol.status_register.motor_48V_ok) {
        motor_supply = protocol.status_register.motor_48V_ok;

        if (!protocol.status_register.motor_48V_ok) Notify::activate(Notify::messages::INFO_MOTOR_48V_POWER_SUPPLY_OFF);
        else Notify::deactivate(Notify::messages::INFO_MOTOR_48V_POWER_SUPPLY_OFF);
    }

    // Burning Jumper presence
    if (burning_jumper != protocol.status_register.burning_jumper_present) {
        burning_jumper = protocol.status_register.burning_jumper_present;

        if (protocol.status_register.burning_jumper_present) Notify::activate(Notify::messages::INFO_BURNING_JUMPER_PRESENT);
        else Notify::deactivate(Notify::messages::INFO_BURNING_JUMPER_PRESENT);
    }

    // Monitor of the Battery Enable input
    if (battery_enabled != protocol.status_register.battery_enabled) {
        battery_enabled = protocol.status_register.battery_enabled;

        if (protocol.status_register.battery_enabled == false) Notify::activate(Notify::messages::INFO_BATTERY_DISABLED);
        else Notify::deactivate(Notify::messages::INFO_BATTERY_DISABLED);
    }

    // Monitor of the Battery Low error condition
    if (protocol.status_register.battery_enabled == false) {
        if (batt_alarm) {
            batt_alarm = false;
            Notify::deactivate(Notify::messages::ERROR_BATTERY_LOW_ERROR);
        }
    }
    else {
        if ((protocol.status_register.batt1_low_alarm || protocol.status_register.batt2_low_alarm) != batt_alarm) {
            batt_alarm = (protocol.status_register.batt1_low_alarm || protocol.status_register.batt2_low_alarm);
            if (batt_alarm) Notify::activate(Notify::messages::ERROR_BATTERY_LOW_ERROR);
            else Notify::deactivate(Notify::messages::ERROR_BATTERY_LOW_ERROR);
        }
    }

    // Monitor of the Study door input
    if (closed_door != protocol.status_register.closed_door) {
        closed_door = protocol.status_register.closed_door;

        if (protocol.status_register.closed_door) Notify::deactivate(Notify::messages::WARNING_DOOR_STUDY_OPEN);
        else Notify::activate(Notify::messages::WARNING_DOOR_STUDY_OPEN);
    }


    // handles the X-RAY push button event
    if (xray_push != protocol.status_register.xray_push_button) {
        xray_push = protocol.status_register.xray_push_button;
        if (Exposures::getXrayPushButtonEvent()) awsProtocol::EVENT_XrayPushButton(protocol.status_register.xray_push_button);
    }

    // handles the soft_power_off_request event
    if (protocol.status_register.soft_power_off_request != soft_power_off) {
        soft_power_off = protocol.status_register.soft_power_off_request;
        awsProtocol::EVENT_Poweroff();        
    }

    
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

    // Reads the System and battery registers from the device
    protocol.status_register.decodeSystemRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::SYSTEM_REGISTER));
    
    // Modifyes some of the status in the case of demo to hel the Demo Installation
    if (Gantry::isOperatingDemo()) {
        protocol.status_register.closed_door = true;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    protocol.status_register.decodeBatteryRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::BATTERY_REGISTER));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Refresh the Data register
    // Toggles the keepalive bit to keep the board alive!
    protocol.data_register.keep_alive = !protocol.data_register.keep_alive;    
    writeDataRegister((unsigned char)ProtocolStructure::DataRegister::register_index::OUTPUTS, protocol.data_register.encodeOutputRegister());
   
    evaluateEvents();
    return;
}



/// <summary>
/// This callback is called whenever the Symulator device should not be connected.
/// </summary>
/// <param name=""></param>
void PCB301::demoLoop(void) {
   
    protocol.status_register.system_power_down = false;
    protocol.status_register.system_emergency = false;
    protocol.status_register.cabinet_safety_alarm = false;

    protocol.status_register.motor_safety_switch = true;
    protocol.status_register.motor_48V_ok = true;
    protocol.status_register.compression_on_status = false;
    protocol.status_register.burning_jumper_present = false;
    protocol.status_register.soft_power_off_request = false;
    protocol.status_register.battery_enabled = false;
    protocol.status_register.batt1_low_alarm = false;
    protocol.status_register.batt2_low_alarm = false;
    protocol.status_register.closed_door = true;
    protocol.status_register.button_up_stat = false;
    protocol.status_register.button_down_stat = false;
    protocol.status_register.button_cw_stat = false;
    protocol.status_register.button_ccw_stat = false;

    protocol.status_register.pedal_up_stat = false;
    protocol.status_register.pedal_down_stat = false;
    protocol.status_register.button_body_cw = false;
    protocol.status_register.button_body_ccw = false;
    protocol.status_register.button_slide_up_stat = false;
    protocol.status_register.button_slide_down_stat = false;
    protocol.status_register.xray_push_button = false;

    protocol.status_register.voltage_batt1 = 12;
    protocol.status_register.voltage_batt2 = 12;

    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    evaluateEvents();
}
