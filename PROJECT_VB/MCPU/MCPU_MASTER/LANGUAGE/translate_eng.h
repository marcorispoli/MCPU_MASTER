
/* -------- ERRORS ---------------------------*/

"ERROR", "E00001", "XRAY_PUSH_FAULT", "Xray push button fault", "Xray push button locked On",
"ERROR", "E00002", "MOTOR_PEDALS_UD_FAULT", "Motor Pedals Fault", "Up/Down pedals locked On",
"ERROR", "E00003", "MOTOR_PEDALS_COMP_FAULT", "Motor Pedals Fault", "Compressor pedals locked On",
"ERROR", "E00004", "MOTOR_BUTTON_CARM_FAULT", "Motor Buttons Fault", "C-Arm rotation buttons locked On",
"ERROR", "E00005", "MOTOR_BUTTON_UD_FAULT", "Motor Buttons Fault", "Up/Down buttons locked On",
"ERROR", "E00006", "CONFIGURATION_FAULT", "Software Configuration Fault", "Wrong software or firmware revision detected",
"ERROR", "E00007", "TUBE_TEMP_FAULT", "Tube Temperature Fault", "The xray tube temperature is detected high",
"ERROR", "E00008", "DOOR_STUDY_OPEN", "Exposure Preparation", "The Door Study is open!\nThe Exposure cannot proceed in this status",
"ERROR", "E00009", "POWER_DOWN_ERROR", "System Power Monitoring", "Power down condition detected!",
"ERROR", "E00010", "BATTERY_LOW_ERROR", "System Power Monitoring", "Battery voltage monitoring error!\nThe battery voltage level is too low.",

/* -------- WARNINGS ---------------------------*/

"WARNING", "W00001", "BATTERY_DISABLED_WARNING", "System Power Monitoring", "The battery usage has been disabled!\nThe use of batteries is mandatory to activate an exposure sequence.",
"WARNING", "W00002", "DOOR_STUDY_OPEN_WARNING", "Safety Monitoring", "The Study's door is detected Open!\nThe study's door shall be closed to activate an exposure sequence.",

/* -------- LABELS ---------------------------*/

"LABEL", "L00001", "ERROR_WINDOW_PANEL", "Error Window Panel", "-",
"LABEL", "L00002", "ERROR", "ERR", "-",
"LABEL", "L00003", "WARNING", "WRN", "-",
"LABEL", "L00004", "INFO", "INFO", "-",