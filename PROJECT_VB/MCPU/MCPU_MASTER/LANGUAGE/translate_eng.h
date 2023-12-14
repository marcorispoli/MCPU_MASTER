
/* -------- ERRORS ---------------------------*/


"E00001", Notify::messages::ERROR_PCB301_RESET, "PCB301 Communication", "The Board has been reset",
"E00002", Notify::messages::ERROR_PCB302_RESET, "PCB302 Communication", "The Board has been reset",
"E00003", Notify::messages::ERROR_PCB303_RESET, "PCB303 Communication", "The Board has been reset",
"E00004", Notify::messages::ERROR_PCB304_RESET, "PCB304 Communication", "The Board has been reset",
"E00005", Notify::messages::ERROR_PCB315_RESET, "PCB315 Communication", "The Board has been reset",
"E00006", Notify::messages::ERROR_PCB326_RESET, "PCB326 Communication", "The Board has been reset",
"E00007", Notify::messages::ERROR_PCB301_COMMUNICATION_ERROR, "PCB301 Communication", "The board no longer communicate with the MCPU",
"E00008", Notify::messages::ERROR_PCB302_COMMUNICATION_ERROR, "PCB302 Communication", "The board no longer communicate with the MCPU",
"E00009", Notify::messages::ERROR_PCB303_COMMUNICATION_ERROR, "PCB303 Communication", "The board no longer communicate with the MCPU",
"E00010", Notify::messages::ERROR_PCB304_COMMUNICATION_ERROR, "PCB304 Communication", "The board no longer communicate with the MCPU",
"E00011", Notify::messages::ERROR_PCB315_COMMUNICATION_ERROR, "PCB315 Communication", "The board no longer communicate with the MCPU",
"E00012", Notify::messages::ERROR_PCB326_COMMUNICATION_ERROR, "PCB326 Communication", "The board no longer communicate with the MCPU",
"E00013", Notify::messages::ERROR_XRAY_PUSH_FAULT, "Xray push button fault", "Xray push button locked On",
"E00014", Notify::messages::ERROR_MOTOR_PEDALS_UD_FAULT, "Motor Pedals Fault", "Up/Down pedals locked On",
"E00003", Notify::messages::ERROR_MOTOR_PEDALS_COMP_FAULT, "Motor Pedals Fault", "Compressor pedals locked On",
"E00004", Notify::messages::ERROR_MOTOR_BUTTON_CARM_FAULT, "Motor Buttons Fault", "C-Arm rotation buttons locked On",
"E00005", Notify::messages::ERROR_MOTOR_BUTTON_UD_FAULT, "Motor Buttons Fault", "Up/Down buttons locked On",
"E00006", Notify::messages::ERROR_CONFIGURATION_FAULT, "Software Configuration Fault", "Wrong software or firmware revision detected",
"E00009", Notify::messages::ERROR_POWER_DOWN_ERROR, "System Power Monitoring", "Power down condition detected!",
"E00010", Notify::messages::ERROR_BATTERY_LOW_ERROR, "System Power Monitoring", "Battery voltage monitoring error! The battery voltage level is too low.",
"E00012", Notify::messages::ERROR_GENERATOR_ERROR_CONNECTION, "Generator Status", "Generator device not connected",
"E00013", Notify::messages::ERROR_COLLIMATION_SELECTION_ERROR, "Collimator Monitoring", "The Collimator selector seams to be blocked",
"E00013", Notify::messages::ERROR_FILTER_SELECTION_ERROR, "Filter Monitoring", "The Filter selector seams to be blocked",
"E00015", Notify::messages::ERROR_STATOR_SENSOR_LOW, "Tube Temperature Monitoring", "The Tube Stator sensor connection input seams to be broken",
"E00016", Notify::messages::ERROR_STATOR_SENSOR_SHORT, "Tube Temperature Monitoring", "The Tube Stator sensor connection input seams to be shorted",
"E00017", Notify::messages::ERROR_BULB_SENSOR_LOW, "Tube Temperature Monitoring", "The Tube Bulb sensor connection input seams to be broken",
"E00018", Notify::messages::ERROR_BULB_SENSOR_SHORT, "Tube Temperature Monitoring", "The Tube Bulb sensor connection input seams to be shorted",
"E00019", Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT, "Body Motor Monitoring", "The brake device is in Fault condition",
"E00019", Notify::messages::ERROR_TILT_MOTOR_BRAKE_FAULT, "Tilt Motor Monitoring", "The brake device is in Fault condition",
"E00020", Notify::messages::ERROR_VERTICAL_MOTOR_HOMING, "Vertical Motor Monitoring", "The encoder is not initialized.",
"E00021", Notify::messages::ERROR_ARM_MOTOR_HOMING, "ARM Motor Monitoring", "The encoder is not initialized.",
"E00022", Notify::messages::ERROR_SLIDE_MOTOR_HOMING, "Slide Motor Monitoring", "The encoder is not initialized.",
"E00023", Notify::messages::ERROR_TILT_MOTOR_HOMING, "Tilt Motor Monitoring", "The encoder is not initialized.",
"E00024", Notify::messages::ERROR_BODY_MOTOR_HOMING, "Body Motor Monitoring", "The encoder is not initialized.",

/* -------- X-RAY SEQUENCE ERRROS ---------------------------*/
"E00900", Notify::messages::ERROR_X_RAY_BUTTON_RELEASED, "X-RAY Sequence Error", "X-Ray push button early released",
"E00901", Notify::messages::ERROR_X_RAY_LOW_ANODIC, "X-RAY Sequence Error", "Lower Anodic Current",
"E00902", Notify::messages::ERROR_X_RAY_LOW_KV, "X-RAY Sequence Error", "Lower kV than expected",
"E00903", Notify::messages::ERROR_X_RAY_TIMEOUT, "X-RAY Sequence Error", "Exposure Timeout",
"E00904", Notify::messages::ERROR_X_RAY_STARTER, "X-RAY Sequence Error", "HS Starter Error",


/* -------- WARNINGS ---------------------------*/
"W00001", Notify::messages::WARNING_MISSING_COMPRESSION, "Compressor Monitoring", "Missing compression force.",
"W00002", Notify::messages::WARNING_MISSING_PATIENT_PROTECTION, "Component Monitoring", "Undetected patient protection component.",
"W00003", Notify::messages::WARNING_ARM_POSITION_WARNING, "Arm Position Monitoring", "C-ARM angle out of range. Repeat the C-ARM positioning.",
"W00004", Notify::messages::WARNING_WRONG_PADDLE, "Compressor Monitoring", "Wrong Paddle model detected.",
"W00005", Notify::messages::WARNING_MISSING_EXPOSURE_MODE, "Exposure Parameters Monitoring", "Missing exposure mode selection.",
"W00006", Notify::messages::WARNING_MISSING_EXPOSURE_DATA, "Exposure Parameters Monitoring", "Missing exposure data selection.",
"W00007", Notify::messages::WARNING_XRAY_BUTTON_DISABLED, "X-Ray Button Monitoring", "The X-Ray button is not enabled.",
"W00008", Notify::messages::WARNING_DOOR_STUDY_OPEN, "Safety Monitoring", "The Study's door is detected Open.",
"W00009", Notify::messages::WARNING_GENERATOR_INIT, "Generator Status Monitoring", "The Generator device is initializing.",
"W00010", Notify::messages::WARNING_GENERATOR_NOT_READY, "Generator Status Monitoring", "The Generator is not ready for exposure.",
"W00011", Notify::messages::WARNING_GENERATOR_MESSAGE, "Generator Device Messages", "There are messages from generator:",
"W00012", Notify::messages::WARNING_TUBE_TEMPERATURE, "X-Ray Tube Monitoring", "The X-Ray Tube cumulated energy is too high",
"W00013", Notify::messages::WARNING_COLLI_LIGHT_ACTIVATION, "Collimator Light Command Failed", "The Collimation Light cannot be activated now. Try again.",
"W00014", Notify::messages::WARNING_STATOR_SENSOR_HIGH, "Tube Temperature Monitoring", "The Tube Stator temperature is too high",
"W00015", Notify::messages::WARNING_BULB_SENSOR_HIGH, "Tube Temperature Monitoring", "The Tube Bulb temperature is too high",
"W00016", Notify::messages::WARNING_ANODE_TEMPERATURE_HIGH, "Tube Temperature Monitoring", "The Anode temperature is too high",
"W00017", Notify::messages::WARNING_FILTER_OUT_OF_POSITION, "Filter Monitoring", "The Filter selector is not in a valid position",
"W00018", Notify::messages::WARNING_COLLIMATOR_OUT_OF_POSITION, "Collimator Monitoring", "An invalid collimation format is detected",

/* -------- INFO ---------------------------*/

"I00001", Notify::messages::INFO_BATTERY_DISABLED, "System Power Monitoring", "The backup batteries are detected Disabled!",
"I00002", Notify::messages::INFO_PROJECTION_CONFIRMATION, "Projection Confirmation Action", "The C-ARM may be activated",
"I00003", Notify::messages::INFO_PROJECTION_ABORT, "Abort Projection Request", "Proceed to Abort the current projection?",

/* -------- LABELS ---------------------------*/

"L00001", Notify::messages::LABEL_ERROR_WINDOW_PANEL, "Error Window Panel", "-",
"L00002", Notify::messages::LABEL_PROJECTION_SELECTION_PANEL, "Projection Selection", "-",
"L00003", Notify::messages::LABEL_ZERO_SETTING_PANEL_TITLE, "Zero Setting Panel", "-",
"L00004", Notify::messages::LABEL_ERROR, "ERR", "-",
"L00005", Notify::messages::LABEL_WARNING, "WRN", "-",
"L00006", Notify::messages::LABEL_INFO, "INF", "-",
"L00007", Notify::messages::LABEL_NOT_READY_FOR_EXPOSURE, "STAND-BY", "-",
"L00008", Notify::messages::LABEL_READY_FOR_EXPOSURE, "READY", "-",
"L00009", Notify::messages::LABEL_X_RAY_ON, "X-RAY ON", "-",

"L00010", Notify::messages::LABEL_MOTOR_COMMAND_SUCCESS, "Command successfully completed", "-",
"L00011", Notify::messages::LABEL_MOTOR_COMMAND_MANUAL_TERMINATION, "Command manually terminated", "-",
"L00012", Notify::messages::LABEL_MOTOR_ERROR_OBSTACLE_DETECTED, "Obstacle detected error", "-",
"L00013", Notify::messages::LABEL_MOTOR_ERROR_MOTOR_BUSY, "Motor busy error", "-",
"L00014", Notify::messages::LABEL_MOTOR_ERROR_INITIALIZATION, "Command initialization error", "-",
"L00015", Notify::messages::LABEL_MOTOR_ERROR_UNEXPECTED_STATUS, "Unexpected internal status error", "-",
"L00016", Notify::messages::LABEL_MOTOR_ERROR_LIMIT_SWITCH, "Limit switches activation error", "-",
"L00017", Notify::messages::LABEL_MOTOR_ERROR_BRAKE_DEVICE, "Brake device activation error", "-",
"L00018", Notify::messages::LABEL_MOTOR_ERROR_INVALID_COMMAND_INITIAL_CONDITION, "Command initialization error", "-",
"L00019", Notify::messages::LABEL_MOTOR_ERROR_TIMOUT, "Timeout activation error", "-",
"L00020", Notify::messages::LABEL_MOTOR_ERROR_INTERNAL_FAULT, "Driver internal error", "-",
"L00021", Notify::messages::LABEL_MOTOR_ERROR_ACTIVATION_REGISTER, "Driver access register error", "-",
"L00022", Notify::messages::LABEL_MOTOR_ERROR_MISSING_HOME, "Missing zero setting error", "-",
"L00023", Notify::messages::LABEL_MOTOR_ERROR_COMMAND_DISABLED, "Motor activation disabled error", "-",
"L00024", Notify::messages::LABEL_MOTOR_ERROR_COMMAND_ABORTED, "Motor activation aborted", "-",





