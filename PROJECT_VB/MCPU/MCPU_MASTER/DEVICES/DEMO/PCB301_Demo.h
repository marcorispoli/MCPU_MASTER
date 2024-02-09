#pragma once
#include "ConfigFile.h"

/*

cabinet_safety_status = false;

motor_safety_switch = true;
motor_48V_ok = true;



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
*/


ref class DemoPcb301
{
public:
    literal System::String^ FILENAME = "DemoPcb301.cnf";
    literal int     FILE_REVISION = 2;

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_XRAY_BUTTON_STAT = "XRAY_BUTTON";
    literal System::String^ PARAM_XRAY_BUTTON_STAT_COMMENT = "Xray Button Status";
    literal System::String^ PARAM_XRAY_BUTTON_STAT_DEFAULT = "0";
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_POWERDOWN_STAT = "POWER_DOWN";
    literal System::String^ PARAM_POWERDOWN_STAT_COMMENT = "Powerdown Status";
    literal System::String^ PARAM_POWERDOWN_STAT_DEFAULT = "0";
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_EMERGENCY_STAT = "EMERGENCY";
    literal System::String^ PARAM_EMERGENCY_STAT_COMMENT = "Emergency Status";
    literal System::String^ PARAM_EMERGENCY_STAT_DEFAULT = "0";
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_DOOR_STAT = "DOOR";
    literal System::String^ PARAM_DOOR_STAT_COMMENT = "Door Open Status";
    literal System::String^ PARAM_DOOR_STAT_DEFAULT = "0";
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PEDAL_UP_STAT = "PEDAL_UP";
    literal System::String^ PARAM_PEDAL_UP_STAT_COMMENT = "Pedal Up Status";
    literal System::String^ PARAM_PEDAL_UP_STAT_DEFAULT = "0";
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PEDAL_DOWN_STAT = "PEDAL_DOWN";
    literal System::String^ PARAM_PEDAL_DOWN_STAT_COMMENT = "Pedal Down Status";
    literal System::String^ PARAM_PEDAL_DOWN_STAT_DEFAULT = "0";
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_CMP_ON_STAT = "CMP_ON";
    literal System::String^ PARAM_CMP_ON_STAT_COMMENT = "Compression On Status";
    literal System::String^ PARAM_CMP_ON_STAT_DEFAULT = "0";
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_CABINET_STAT = "CABINET_SWITCH";
    literal System::String^ PARAM_CABINET_STAT_COMMENT = "Cabinet Switch Status";
    literal System::String^ PARAM_CABINET_STAT_DEFAULT = "0";


    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_XRAY_BUTTON_STAT, PARAM_XRAY_BUTTON_STAT_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_XRAY_BUTTON_STAT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_EMERGENCY_STAT, PARAM_EMERGENCY_STAT_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_EMERGENCY_STAT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_POWERDOWN_STAT, PARAM_POWERDOWN_STAT_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_POWERDOWN_STAT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_DOOR_STAT, PARAM_DOOR_STAT_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_DOOR_STAT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_PEDAL_UP_STAT, PARAM_PEDAL_UP_STAT_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_PEDAL_UP_STAT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_PEDAL_DOWN_STAT, PARAM_PEDAL_DOWN_STAT_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_PEDAL_DOWN_STAT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_CMP_ON_STAT, PARAM_CMP_ON_STAT_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_CMP_ON_STAT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_CABINET_STAT, PARAM_CABINET_STAT_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_CABINET_STAT_DEFAULT,
            }),

        }
    );
};