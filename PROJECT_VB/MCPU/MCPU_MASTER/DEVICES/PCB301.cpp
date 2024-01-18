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

        if (power_down_status) Notify::activate(Notify::messages::ERROR_POWER_DOWN_ERROR);
        else Notify::deactivate(Notify::messages::ERROR_POWER_DOWN_ERROR);
    }

    // Emergency
    if (PCB301_GET_SYSTEM_EMERGENCY_STATUS(system_status_register) != emergency_status) {
        emergency_status = PCB301_GET_SYSTEM_EMERGENCY_STATUS(system_status_register);

        if (emergency_status) Notify::activate(Notify::messages::ERROR_EMERGENCY_BUTTON);
        else Notify::deactivate(Notify::messages::ERROR_EMERGENCY_BUTTON);
    }

    // Cabinet safety
    if (PCB301_GET_SYSTEM_CABINET_SAFETY(system_status_register) != cabinet_safety_status) {
        cabinet_safety_status = PCB301_GET_SYSTEM_CABINET_SAFETY(system_status_register);

        if (cabinet_safety_status) Notify::activate(Notify::messages::ERROR_CABINET_SAFETY);
        else Notify::deactivate(Notify::messages::ERROR_CABINET_SAFETY);
    }

    // Motor Switch
    if (PCB301_GET_SYSTEM_MOTOR_SWITCH(system_status_register) != motor_safety_switch) {
        motor_safety_switch = PCB301_GET_SYSTEM_MOTOR_SWITCH(system_status_register);

        if (!motor_safety_switch) Notify::activate(Notify::messages::INFO_MOTOR_48V_SAFETY_LINE_OFF);
        else Notify::deactivate(Notify::messages::INFO_MOTOR_48V_SAFETY_LINE_OFF);
    }

    // Motor Power Supply
    if (PCB301_GET_SYSTEM_MOTOR_OK(system_status_register) != motor_48V_ok) {
        motor_48V_ok = PCB301_GET_SYSTEM_MOTOR_OK(system_status_register);

        if (!motor_48V_ok) Notify::activate(Notify::messages::INFO_MOTOR_48V_POWER_SUPPLY_OFF);
        else Notify::deactivate(Notify::messages::INFO_MOTOR_48V_POWER_SUPPLY_OFF);
    }

    // Compression status
    if (PCB301_GET_SYSTEM_COMPRESSION(system_status_register) != compression_on_status) {
        compression_on_status = PCB301_GET_SYSTEM_COMPRESSION(system_status_register);
    }

    // Burning Jumper presence
    if (PCB301_GET_SYSTEM_BURNING_JMP(system_status_register) != burning_jumper_present) {
        burning_jumper_present = PCB301_GET_SYSTEM_BURNING_JMP(system_status_register);

        if (burning_jumper_present) Notify::activate(Notify::messages::INFO_BURNING_JUMPER_PRESENT);
        else Notify::deactivate(Notify::messages::INFO_BURNING_JUMPER_PRESENT);
    }

    // Soft Power Off Request
    if (PCB301_GET_SYSTEM_POWER_OFF_REQ(system_status_register) != soft_power_off_request) {
        soft_power_off_request = PCB301_GET_SYSTEM_POWER_OFF_REQ(system_status_register);
    
    }
    

    // Monitor of the Battery Enable input
    if (PCB301_GET_SYSTEM_BATTENA(system_status_register) != battery_enabled_status) {
        battery_enabled_status = PCB301_GET_SYSTEM_BATTENA(system_status_register);
        if (battery_enabled_status == false) Notify::activate(Notify::messages::INFO_BATTERY_DISABLED);
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
            if(batt1_low_alarm || batt2_low_alarm) Notify::activate(Notify::messages::ERROR_BATTERY_LOW_ERROR);
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
            else Notify::activate(Notify::messages::WARNING_DOOR_STUDY_OPEN);
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

    /*
	static bool emergency_status = false;			//!< Current Emergency Status
	static bool enable_48V_power_supply = false;	//!< 48V power supply status
	static bool enable_48V_safety_switch = false;	//!< Safety switch of the 48V
	static bool motor_48V_ok = false;				//!< Feedback from the motor power supply
	static bool compression_on_status = false;		//!< Actual compression signal
	static bool burning_jumper_present = false;		//!< Burning jumper present in the system
	static bool soft_power_off_request = false;		//!< A power Off sequence is requested
    */
}

void PCB301::handleBatteryStatusRegister(void) {
    Register^ battery_status_register = readStatusRegister((unsigned char) StatusRegisters::BATTERY_STATUS_REGISTER);
    if (battery_status_register == nullptr) return;

    voltage_batt1 = PCB301_GET_BATTERY_VBATT1(battery_status_register);
    voltage_batt2 = PCB301_GET_BATTERY_VBATT2(battery_status_register);

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

    // Refresh the Data register
    writeDataRegister((unsigned char)DataRegisters::OUTPUTS_DATA_REGISTER, outputs_data_register);
   
    return;
}
