#include "PCB301.h"
#include "ExposureModule.h"
#include "Simulator.h"
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
    static door_options door = door_options::CLOSED_DOOR;
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
        burning_jumper = burning_jumper_present;

        if (burning_jumper_present) Notify::activate(Notify::messages::INFO_BURNING_JUMPER_PRESENT);
        else Notify::deactivate(Notify::messages::INFO_BURNING_JUMPER_PRESENT);
    }

    // Monitor of the Battery Enable input
    if (battery_enabled != battery_enabled_status) {
        battery_enabled = battery_enabled_status;

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
        if (Exposures::getXrayPushButtonEvent()) awsProtocol::EVENT_XrayPushButton(xray_push_button);
    }

    // handles the soft_power_off_request event
    if (soft_power_off_request != soft_power_off) {
        soft_power_off = soft_power_off_request;        
        awsProtocol::EVENT_Poweroff();        
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
    if (button_up_stat != PCB301_GET_BUTTON_VERTICAL_UP(system_status_register)) {
        button_up_stat = PCB301_GET_BUTTON_VERTICAL_UP(system_status_register);
        if (button_up_stat) LogClass::logInFile("PCB301: MANUAL BUTTON UP ACTIVATED");
        else LogClass::logInFile("PCB301: MANUAL BUTTON UP CLEARED");
    }

    if (button_down_stat != PCB301_GET_BUTTON_VERTICAL_DOWN(system_status_register)) {
        button_down_stat = PCB301_GET_BUTTON_VERTICAL_DOWN(system_status_register);
        if (button_down_stat) LogClass::logInFile("PCB301: MANUAL BUTTON DOWN ACTIVATED");
        else LogClass::logInFile("PCB301: MANUAL BUTTON DOWN CLEARED");
    }

    if (button_arm_cw_stat != PCB301_GET_BUTTON_ARM_CW(system_status_register)) {
        button_arm_cw_stat = PCB301_GET_BUTTON_ARM_CW(system_status_register);
        if (button_arm_cw_stat) LogClass::logInFile("PCB301: MANUAL CW ACTIVATED");
        else LogClass::logInFile("PCB301: MANUAL CW CLEARED");
    }

    if (button_arm_ccw_stat != PCB301_GET_BUTTON_ARM_CCW(system_status_register)) {
        button_arm_ccw_stat = PCB301_GET_BUTTON_ARM_CCW(system_status_register);
        if (button_arm_ccw_stat) LogClass::logInFile("PCB301: MANUAL CCW ACTIVATED");
        else LogClass::logInFile("PCB301: MANUAL CCW CLEARED");
    }
    

    pedal_up_stat = PCB301_GET_PEDAL_VERTICAL_UP(system_status_register);
    pedal_down_stat = PCB301_GET_PEDAL_VERTICAL_DOWN(system_status_register);
    button_body_cw = PCB301_GET_BUTTON_BODY_CW(system_status_register);
    button_body_ccw = PCB301_GET_BUTTON_BODY_CCW(system_status_register);
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

    
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    

    // Power Down Condition Monitor
    if(from_simulator[(int) simul_rx_struct::POWERDOWN_STAT]) power_down_status = true;
    else power_down_status = false;


    // Emergency
    if (from_simulator[(int)simul_rx_struct::EMERGENCY_STAT]) emergency_status = true;
    else emergency_status = false;


    // Cabinet safety
    if (from_simulator[(int)simul_rx_struct::CABINET_STAT]) cabinet_safety_status = true;
    else cabinet_safety_status = false;

    // Door closed
    if (from_simulator[(int)simul_rx_struct::DOOR_STAT]) door_status = door_options::CLOSED_DOOR;
    else door_status = door_options::OPEN_DOOR;

    // Compressor On
    if (from_simulator[(int)simul_rx_struct::COMPRESSION_ON_STAT]) compression_on_status = true;
    else compression_on_status = false;
        
    // handles the X-RAY push button    
    if (from_simulator[(int)simul_rx_struct::XRAY_PUSH_BUTTON_STAT]) xray_push_button = true;
    else xray_push_button = false;

    // Motor 48V
    if (from_simulator[(int)simul_rx_struct::V48_OK_STAT ])   motor_48V_ok  = true;
    else motor_48V_ok = false;

    // Motor safety swittch
    if (from_simulator[(int)simul_rx_struct::V48SW_STAT])   motor_safety_switch = true;
    else motor_safety_switch = false;

    // Pedal Up/Down
    if (from_simulator[(int)simul_rx_struct::PEDALBOARD_UP_STAT])   pedal_up_stat = true;
    else pedal_up_stat = false;
    if (from_simulator[(int)simul_rx_struct::PEDALBOARD_DOWN_STAT])   pedal_down_stat = true;
    else pedal_down_stat = false;

    // Burning Jumper
    if (from_simulator[(int)simul_rx_struct::BURNING_JUMPER_STAT])   burning_jumper_present = true;
    else burning_jumper_present = false;

    // Soft Power Off
    if (from_simulator[(int)simul_rx_struct::SOFT_POWEROFF_STAT])   soft_power_off_request = true;
    else soft_power_off_request = false;

    // Battery Enable
    if (from_simulator[(int)simul_rx_struct::BATT_ENA_STAT])   battery_enabled_status = true;
    else battery_enabled_status = false;

    // BAttery Alarm
    if (from_simulator[(int)simul_rx_struct::BATT1_LOW_STAT])   batt1_low_alarm = true;
    else batt1_low_alarm = false;
    if (from_simulator[(int)simul_rx_struct::BATT2_LOW_STAT])   batt2_low_alarm = true;
    else batt2_low_alarm = false;

    // Keypad 
    if (from_simulator[(int)simul_rx_struct::KEYPAD_UP_STAT])   button_up_stat = true;
    else button_up_stat = false;
    if (from_simulator[(int)simul_rx_struct::KEYPAD_DOWN_STAT])   button_down_stat = true;
    else button_down_stat = false;
    if (from_simulator[(int)simul_rx_struct::KEYPAD_CW_STAT])   button_arm_cw_stat = true;
    else button_arm_cw_stat = false;
    if (from_simulator[(int)simul_rx_struct::KEYPAD_CCW_STAT])   button_arm_ccw_stat = true;
    else button_arm_ccw_stat = false;

    if (from_simulator[(int)simul_rx_struct::BODY_CW_STAT])   button_body_cw = true;
    else button_body_cw = false;
    if (from_simulator[(int)simul_rx_struct::BODY_CCW_STAT])   button_body_ccw = true;
    else button_body_ccw = false;

    if (from_simulator[(int)simul_rx_struct::SLIDE_UP_STAT])   button_slide_up_stat = true;
    else button_slide_up_stat = false;
    if (from_simulator[(int)simul_rx_struct::SLIDE_DOWN_STAT])   button_slide_down_stat = true;
    else button_slide_down_stat = false;

    
    voltage_batt1 = from_simulator[(int)simul_rx_struct::VBATT1];
    voltage_batt1 = from_simulator[(int)simul_rx_struct::VBATT2];

    evaluateEvents();

    toggleKeepalive();

    // Data Outputs

    if (outputs_data_register->d0 & 0x1) to_simulator[(int)simul_tx_struct::PROGRAMMING_POWER_LOCK] = 1;
    else to_simulator[(int)simul_tx_struct::PROGRAMMING_POWER_LOCK] = 0;

    if (outputs_data_register->d0 & 0x2) to_simulator[(int)simul_tx_struct::MOTOR_POWER_ENA] = 1;
    else to_simulator[(int)simul_tx_struct::MOTOR_POWER_ENA] = 0;

    if (outputs_data_register->d0 & 0x4) to_simulator[(int)simul_tx_struct::MOTOR_SW_ENA] = 1;
    else to_simulator[(int)simul_tx_struct::MOTOR_SW_ENA] = 0;

    if (outputs_data_register->d0 & 0x8) to_simulator[(int)simul_tx_struct::COMPRESSOR_ENA] = 1;
    else to_simulator[(int)simul_tx_struct::COMPRESSOR_ENA] = 0;

    if (outputs_data_register->d0 & 0x10) to_simulator[(int)simul_tx_struct::CALIBRATION_ENA] = 1;
    else to_simulator[(int)simul_tx_struct::CALIBRATION_ENA] = 0;

    if (outputs_data_register->d0 & 0x20) to_simulator[(int)simul_tx_struct::XRAY_ENA] = 1;
    else to_simulator[(int)simul_tx_struct::XRAY_ENA] = 0;


    if (outputs_data_register->d1 & 0x01) to_simulator[(int)simul_tx_struct::BURNING_STAT] = 1;
    else to_simulator[(int)simul_tx_struct::BURNING_STAT] = 0;

    if (outputs_data_register->d1 & 0x02) to_simulator[(int)simul_tx_struct::BUZZER_STAT] = 1;
    else to_simulator[(int)simul_tx_struct::BUZZER_STAT] = 0;

    if (outputs_data_register->d1 & 0x04) to_simulator[(int)simul_tx_struct::MANUAL_BUZZER_STAT] = 1;
    else to_simulator[(int)simul_tx_struct::MANUAL_BUZZER_STAT] = 0;

    if (outputs_data_register->d1 & 0x08) to_simulator[(int)simul_tx_struct::XLED_STAT] = 1;
    else to_simulator[(int)simul_tx_struct::XLED_STAT] = 0;

    if (outputs_data_register->d1 & 0x10) to_simulator[(int)simul_tx_struct::XLAMP1_STAT] = 1;
    else to_simulator[(int)simul_tx_struct::XLAMP1_STAT] = 0;

    if (outputs_data_register->d1 & 0x20) to_simulator[(int)simul_tx_struct::XLAMP2_STAT] = 1;
    else to_simulator[(int)simul_tx_struct::XLAMP2_STAT] = 0;

    if (outputs_data_register->d1 & 0x40) to_simulator[(int)simul_tx_struct::ROTATION_LED] = 1;
    else to_simulator[(int)simul_tx_struct::ROTATION_LED] = 0;

    if (outputs_data_register->d3 & 0x40) to_simulator[(int)simul_tx_struct::POWER_OFF_STAT] = 1;
    else to_simulator[(int)simul_tx_struct::POWER_OFF_STAT] = 0;

    if (outputs_data_register->d3 & 0x80) to_simulator[(int)simul_tx_struct::KEEP_ALIVE_STAT] = 1;
    else to_simulator[(int)simul_tx_struct::KEEP_ALIVE_STAT] = 0;


    // Evaluates i fthe data has been changed to update the Simulator     
    bool changed = false;
    for (int i = 0; i < to_simulator->Length; i++) {
        if (to_simulator[i] != to_simulator_previous[i]) {
            changed = true;
            to_simulator_previous[i] = to_simulator[i];
        }
    }

    if (changed) simulSend();

    // Evaluates the soft power-off bit
    if (PCB301_GET_OUTPUTS_DATA_POWER_OFF(outputs_data_register)) {
        LogClass::logInFile("PCB301 DEMO POWER OFF COMMAND");
        Application::Exit();
    }
}

void PCB301::simulRx(cli::array<System::Byte>^ receiveBuffer, int index, int rc) {
    if (rc != (int) simul_rx_struct::BUFLEN) return;
    
    for (int i = 0; i < rc; i++) device->from_simulator[i] = receiveBuffer[index + i];
}

void PCB301::simulSend(void) {

    to_simulator[0] = 0x3;
    to_simulator[1] = (int) simul_tx_struct::BUFLEN;
    to_simulator[(int)simul_tx_struct::DEVICE_ID] = PCB301_DEVID;
    to_simulator[(int)simul_tx_struct::ENDFRAME] = 0x2;

    // Sends the buffer
    ((Simulator^) Gantry::pSimulator)->send(to_simulator);
}

void PCB301::simulInit(void) {

    // Create the Simulator structure if shuld be  necessary
    from_simulator = gcnew cli::array<System::Byte>( (int)PCB301::simul_rx_struct::BUFLEN);
    to_simulator = gcnew cli::array<System::Byte>((int)PCB301::simul_tx_struct::BUFLEN);
    to_simulator_previous = gcnew cli::array<System::Byte>((int)PCB301::simul_tx_struct::BUFLEN);


    from_simulator[(int)simul_rx_struct::POWERDOWN_STAT] = 0;
    from_simulator[(int)simul_rx_struct::EMERGENCY_STAT] = 0;
    from_simulator[(int)simul_rx_struct::CABINET_STAT] = 0;
    from_simulator[(int)simul_rx_struct::DOOR_STAT] = 1;
    from_simulator[(int)simul_rx_struct::COMPRESSION_ON_STAT] = 0;
    from_simulator[(int)simul_rx_struct::XRAY_PUSH_BUTTON_STAT] = 0;
    from_simulator[(int)simul_rx_struct::V48_OK_STAT] = 1;
    from_simulator[(int)simul_rx_struct::V48SW_STAT] = 1;
    from_simulator[(int)simul_rx_struct::PEDALBOARD_UP_STAT] = 0;
    from_simulator[(int)simul_rx_struct::PEDALBOARD_DOWN_STAT] = 0;
    from_simulator[(int)simul_rx_struct::BURNING_JUMPER_STAT] = 0;
    from_simulator[(int)simul_rx_struct::SOFT_POWEROFF_STAT] = 0;
    from_simulator[(int)simul_rx_struct::BATT_ENA_STAT] = 0;
    from_simulator[(int)simul_rx_struct::BATT1_LOW_STAT] = 0;
    from_simulator[(int)simul_rx_struct::BATT2_LOW_STAT] = 0;
    from_simulator[(int)simul_rx_struct::KEYPAD_UP_STAT] = 0;
    from_simulator[(int)simul_rx_struct::KEYPAD_DOWN_STAT] = 0;
    from_simulator[(int)simul_rx_struct::KEYPAD_CW_STAT] = 0;
    from_simulator[(int)simul_rx_struct::KEYPAD_CCW_STAT] = 0;
    from_simulator[(int)simul_rx_struct::BODY_CW_STAT] = 0;
    from_simulator[(int)simul_rx_struct::BODY_CCW_STAT] = 0;
    from_simulator[(int)simul_rx_struct::SLIDE_UP_STAT] = 0;
    from_simulator[(int)simul_rx_struct::SLIDE_DOWN_STAT] = 0;
    from_simulator[(int)simul_rx_struct::VBATT1] = 12;
    from_simulator[(int)simul_rx_struct::VBATT2] = 12;


    // Connects the reception event
    ((Simulator^) Gantry::pSimulator)->pcb301_rx_event += gcnew Simulator::rxData_slot(&PCB301::simulRx);
    
}