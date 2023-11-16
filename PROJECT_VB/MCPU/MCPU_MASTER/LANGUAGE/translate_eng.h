
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
"ERROR", "E00013", "COLLIMATION_SELECTION_ERROR", "Collimator Monitoring", "The Collimator selector seams to be blocked",
"ERROR", "E00013", "FILTER_SELECTION_ERROR", "Filter Monitoring", "The Filter selector seams to be blocked",
"ERROR", "E00015", "STATOR_SENSOR_LOW", "Tube Temperature Monitoring", "The Tube Stator sensor connection input\nseams to be broken",
"ERROR", "E00016", "STATOR_SENSOR_SHORT", "Tube Temperature Monitoring", "The Tube Stator sensor connection input\nseams to be shorted",
"ERROR", "E00017", "BULB_SENSOR_LOW", "Tube Temperature Monitoring", "The Tube Bulb sensor connection input\nseams to be broken",
"ERROR", "E00018", "BULB_SENSOR_SHORT", "Tube Temperature Monitoring", "The Tube Bulb sensor connection input\nseams to be shorted",
"ERROR", "E00019", "PCB315_COMMUNICATION_ERROR", "PCB315 Communication", "The PCB315 board no longer communicate with the MCPU",
"ERROR", "E00020", "PCB303_COMMUNICATION_ERROR", "PCB303 Communication", "The PCB303 board no longer communicate with the MCPU",


/* -------- WARNINGS ---------------------------*/
"WARNING", "W00001", "MISSING_COMPRESSION_WARNING", "Compressor Monitoring", "Missing compression force.",
"WARNING", "W00002", "MISSING_PATIENT_PROTECTION_WARNING", "Component Monitoring", "Undetected patient protection component.",
"WARNING", "W00003", "WRONG_ARM_POSITION_WARNING", "Arm Position Monitoring", "C-ARM angle out of range.\nRepeat the C-ARM positioning.",
"WARNING", "W00004", "WRONG_PADDLE_WARNING", "Compressor Monitoring", "Wrong Paddle model detected.",
"WARNING", "W00005", "MISSING_EXPOSURE_MODE_WARNING", "Exposure Parameters Monitoring", "Missing exposure mode selection.",
"WARNING", "W00006", "MISSING_EXPOSURE_DATA_WARNING", "Exposure Parameters Monitoring", "Missing exposure data selection.",
"WARNING", "W00007", "XRAY_BUTTON_DISABLED_WARNING", "X-Ray Button Monitoring", "The X-Ray button is not enabled.",
"WARNING", "W00008", "DOOR_STUDY_OPEN_WARNING", "Safety Monitoring", "The Study's door is detected Open.",
"WARNING", "W00009", "GENERATOR_INIT_WARNING", "Generator Status Monitoring", "The Generator device is not ready for exposures.",
"WARNING", "W00010", "TUBE_TEMP_WARNING", "X-Ray Tube Monitoring", "The X-Ray Tube cumulated energy is too high",
"WARNING", "W00011", "COLLI_LIGHT_ACTIVATION_WARNING", "Collimator Light Command Failed", "The Collimation Light cannot be activated now.\nTry again.",
"WARNING", "W00012", "STATOR_SENSOR_HIGH", "Tube Temperature Monitoring", "The Tube Stator temperature is too high",
"WARNING", "W00013", "BULB_SENSOR_HIGH", "Tube Temperature Monitoring", "The Tube Bulb temperature is too high",
"WARNING", "W00014", "ANODE_TEMPERATURE_HIGH", "Tube Temperature Monitoring", "The Anode temperature is too high",
"WARNING", "W00015", "FILTER_OUT_OF_POSITION", "Filter Monitoring", "The Filter selector is not in a valid position",
"WARNING", "W00016", "COLLIMATOR_OUT_OF_POSITION", "Collimator Monitoring", "An invalid collimation format is detected",

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
