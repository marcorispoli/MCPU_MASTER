
/* -------- ERRORS ---------------------------*/

"ERROR", "E00001", "XRAY_PUSH_FAULT", "Xray push button fault", "Xray push button locked On",
"ERROR", "E00002", "MOTOR_PEDALS_UD_FAULT", "Motor Pedals Fault", "Up/Down pedals locked On",
"ERROR", "E00003", "MOTOR_PEDALS_COMP_FAULT", "Motor Pedals Fault", "Compressor pedals locked On",
"ERROR", "E00004", "MOTOR_BUTTON_CARM_FAULT", "Motor Buttons Fault", "C-Arm rotation buttons locked On",
"ERROR", "E00005", "MOTOR_BUTTON_UD_FAULT", "Motor Buttons Fault", "Up/Down buttons locked On",
"ERROR", "E00006", "CONFIGURATION_FAULT", "Software Configuration Fault", "Wrong software or firmware revision detected",
"ERROR", "E00008", "DOOR_STUDY_OPEN", "Exposure Preparation", "The Door Study is open!\nThe Exposure cannot proceed in this status",
"ERROR", "E00009", "POWER_DOWN_ERROR", "System Power Monitoring", "Power down condition detected!",
"ERROR", "E00010", "BATTERY_LOW_ERROR", "System Power Monitoring", "Battery voltage monitoring error!\nThe battery voltage level is too low.",
"ERROR", "E00011", "GENERATOR_ERROR_SH", "Generator Status", "Smart Hub not connected",
"ERROR", "E00012", "GENERATOR_ERROR_GEN", "Generator Status", "Generator device not connected",


/* -------- WARNINGS ---------------------------*/
"WARNING", "W00001", "MISSING_COMPRESSION_WARNING", "Compressor Monitoring", "Missing compression force.",
"WARNING", "W00002", "MISSING_PATIENT_PROTECTION_WARNING", "Component Monitoring", "Undetected patient protection component.",
"WARNING", "W00003", "WRONG_ARM_POSITION_WARNING", "Arm Position Monitoring", "C-ARM angle out of range.\nRepeat the C-ARM positioning.",
"WARNING", "W00004", "WRONG_PADDLE_WARNING", "Compressor Monitoring", "Wrong Paddle model detected.",
"WARNING", "W00005", "MISSING_EXPOSURE_MODE_WARNING", "Exposure Parameters Monitoring", "Missing exposure mode selection.",
"WARNING", "W00006", "MISSING_EXPOSURE_DATA_WARNING", "Exposure Parameters Monitoring", "Missing exposure data selection.",
"WARNING", "W00007", "XRAY_BUTTON_DISABLED_WARNING", "X-Ray Button Monitoring", "The X-Ray button is not enabled.",
"WARNING", "W00008", "DOOR_STUDY_OPEN_WARNING", "Safety Monitoring", "The Study's door is detected Open.",
"WARNING", "W00010", "GENERATOR_INIT_WARNING", "Generator Status Monitoring", "The Generator device is not ready for exposures.",
"WARNING", "W00011", "TUBE_TEMP_WARNING", "X-Ray Tube Monitoring", "The X-Ray Tube cumulated energy is too high",

/* -------- INFO ---------------------------*/

"INFO", "I00001", "BATTERY_DISABLED_INFO", "System Power Monitoring", "The backup batteries are detected Disabled!",
"INFO", "I00002", "PROJECTION_CONFIRMATION_INFO", "Projection Confirmation Action", "The C-ARM may be activated",
"INFO", "I00003", "PROJECTION_ABORT_INFO", "Abort Projection Request", "Proceed to Abort the current projection?",

/* -------- LABELS ---------------------------*/

"LABEL", "L00001", "ERROR_WINDOW_PANEL", "Error Window Panel", "-",
"LABEL", "L00002", "PROJECTION_SELECTION_PANEL", "Projection Selection", "-",
"LABEL", "L00003", "ERROR", "ERR", "-",
"LABEL", "L00004", "WARNING", "WRN", "-",
"LABEL", "L00005", "INFO", "INF", "-",
"LABEL", "L00006", "NOT-READY-FOR-EXPOSURE", "STAND-BY", "-",
"LABEL", "L00007", "READY-FOR-EXPOSURE", "READY", "-",
"LABEL", "L00008", "X-RAY-ON", "X-RRAY ON", "-",
