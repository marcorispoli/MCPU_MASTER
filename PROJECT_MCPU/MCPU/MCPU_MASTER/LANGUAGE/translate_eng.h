
/* -------- ERRORS ---------------------------*/


"E00000", Notify::messages::ERROR_FATAL_COMPILATION, "Compilation Error", "Invalid module condition detected. See the log file",
"E00001", Notify::messages::ERROR_AWS_INITIALIZATION, "AWS Communication", "Fatal error in opening the socket. Check the network parameters.",
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
"E00015", Notify::messages::ERROR_MOTOR_PEDALS_COMP_FAULT, "Motor Pedals Fault", "Compressor pedals locked On",
"E00016", Notify::messages::ERROR_MOTOR_BUTTON_CARM_FAULT, "Motor Buttons Fault", "C-Arm rotation buttons locked On",
"E00017", Notify::messages::ERROR_MOTOR_BUTTON_UD_FAULT, "Motor Buttons Fault", "Up/Down buttons locked On",
"E00018", Notify::messages::ERROR_CONFIGURATION_FAULT, "Software Configuration Fault", "Wrong software or firmware revision detected",
"E00019", Notify::messages::ERROR_POWER_DOWN_ERROR, "System Power Monitoring", "Power down condition detected!",
"E00020", Notify::messages::ERROR_EMERGENCY_BUTTON, "System Power Monitoring", "Emergency button activation detected!",
"E00037", Notify::messages::ERROR_CABINET_SAFETY, "System Safety Monitoring", "Safety Cabinet activation detected!",
"E00021", Notify::messages::ERROR_BATTERY_LOW_ERROR, "System Power Monitoring", "Battery voltage monitoring error! The battery voltage level is too low.",
"E00022", Notify::messages::ERROR_GENERATOR_ERROR_CONNECTION, "Generator Status", "Generator device not connected",
"E00023", Notify::messages::ERROR_COLLIMATION_SELECTION_ERROR, "Collimator Monitoring", "The Collimator selector seams to be blocked",
"E00024", Notify::messages::ERROR_FILTER_SELECTION_ERROR, "Filter Monitoring", "The Filter selector seams to be blocked",
"E00025", Notify::messages::ERROR_STATOR_SENSOR_LOW, "Tube Temperature Monitoring", "The Tube Stator sensor connection input seams to be broken",
"E00026", Notify::messages::ERROR_STATOR_SENSOR_SHORT, "Tube Temperature Monitoring", "The Tube Stator sensor connection input seams to be shorted",
"E00027", Notify::messages::ERROR_BULB_SENSOR_LOW, "Tube Temperature Monitoring", "The Tube Bulb sensor connection input seams to be broken",
"E00028", Notify::messages::ERROR_BULB_SENSOR_SHORT, "Tube Temperature Monitoring", "The Tube Bulb sensor connection input seams to be shorted",
"E00029", Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT, "Body Motor Monitoring", "The brake device is in Fault condition",
"E00030", Notify::messages::ERROR_TILT_MOTOR_BRAKE_FAULT, "Tilt Motor Monitoring", "The brake device is in Fault condition",
"E00031", Notify::messages::ERROR_VERTICAL_MOTOR_HOMING, "Vertical Motor Monitoring", "The encoder is not initialized",
"E00032", Notify::messages::ERROR_ARM_MOTOR_HOMING, "ARM Motor Monitoring", "The encoder is not initialized",
"E00033", Notify::messages::ERROR_SLIDE_MOTOR_HOMING, "Slide Motor Monitoring", "The encoder is not initialized",
"E00034", Notify::messages::ERROR_TILT_MOTOR_HOMING, "Tilt Motor Monitoring", "The encoder is not initialized",
"E00035", Notify::messages::ERROR_BODY_MOTOR_HOMING, "Body Motor Monitoring", "The encoder is not initialized",
"E00036", Notify::messages::ERROR_BODY_LIMIT_SWITCH, "Body Motor Monitoring", "The limit switch is active",
"E00037", Notify::messages::ERROR_BODY_POTENTIOMETER_READ_FAULT, "Body Motor Monitoring", "Unable to read the potentiometer input",
"E00038", Notify::messages::ERROR_VERTICAL_LIMIT_SWITCH, "Vertical Motor Monitoring", "The limit switch is active",
"E00039", Notify::messages::ERROR_GENERATOR_SETUP, "Generator Status Monitoring", "The Generator configuration is failed",
"E00040", Notify::messages::ERROR_GENERATOR_SYS_MESSAGE, "Generator Messages", "There are error messages",
"E00041", Notify::messages::ERROR_POTTER_GRID_POSITION, "Potter Messages", "The Grid device is in fault condition",


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
"W00004", Notify::messages::WARNING_MISSING_PADDLE, "Compressor Monitoring", "A valid paddle is not detected.",
"W00005", Notify::messages::WARNING_WRONG_PADDLE, "Compressor Monitoring", "Wrong Paddle model detected.",
"W00006", Notify::messages::WARNING_MISSING_EXPOSURE_MODE, "Exposure Parameters Monitoring", "Missing exposure mode selection.",
"W00007", Notify::messages::WARNING_MISSING_EXPOSURE_DATA, "Exposure Parameters Monitoring", "Missing exposure data selection.",
"W00008", Notify::messages::WARNING_XRAY_BUTTON_DISABLED, "X-Ray Button Monitoring", "The X-Ray button is not enabled.",
"W00009", Notify::messages::WARNING_DOOR_STUDY_OPEN, "Safety Monitoring", "The Study's door is detected Open.",
"W00010", Notify::messages::WARNING_GENERATOR_NOT_READY, "Generator Status Monitoring", "The Generator is not ready for exposure.",
"W00012", Notify::messages::WARNING_TUBE_TEMPERATURE, "X-Ray Tube Monitoring", "The X-Ray Tube cumulated energy is too high",
"W00013", Notify::messages::WARNING_COLLI_LIGHT_ACTIVATION, "Collimator Light Command Failed", "The Collimation Light cannot be activated now. Try again.",
"W00014", Notify::messages::WARNING_STATOR_SENSOR_HIGH, "Tube Temperature Monitoring", "The Tube Stator temperature is too high",
"W00015", Notify::messages::WARNING_BULB_SENSOR_HIGH, "Tube Temperature Monitoring", "The Tube Bulb temperature is too high",
"W00016", Notify::messages::WARNING_ANODE_TEMPERATURE_HIGH, "Tube Temperature Monitoring", "The Anode temperature is too high",
"W00017", Notify::messages::WARNING_FILTER_OUT_OF_POSITION, "Filter Monitoring", "The Filter selector is not in a valid position",
"W00018", Notify::messages::WARNING_COLLIMATOR_OUT_OF_POSITION, "Collimator Monitoring", "An invalid collimation format is detected",
"W00019", Notify::messages::WARNING_GENERATOR_SERVICE_MODE, "Generator Monitoring", "The Generator is in Service Mode",
"W00020", Notify::messages::WARNING_GRID_OUT_OF_POSITION, "Potter Monitoring", "The grid is not in the expected position",
"W00021", Notify::messages::WARNING_GRID_GENERAL_ENABLE, "Potter Monitoring", "The grid activation is momentary disabled",


/* -------- INFO ---------------------------*/

"I00001", Notify::messages::INFO_BATTERY_DISABLED, "System Power Monitoring", "The backup batteries are detected Disabled!",
"I00002", Notify::messages::INFO_PROJECTION_CONFIRMATION, "Projection Confirmation Action", "The C-ARM may be activated",
"I00003", Notify::messages::INFO_PROJECTION_ABORT, "Abort Projection Request", "Proceed to Abort the current projection?",

"I00004", Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION, "Motor Slide Activation", "Proceed with the Slide activation?",
"I00005", Notify::messages::INFO_BODY_ACTIVATION_CONFIRMATION, "Motor Body Activation", "Proceed with the Body activation?",
"I00006", Notify::messages::INFO_ARM_ACTIVATION_CONFIRMATION, "Motor Arm Activation", "Proceed with the Arm activation?",
"I00007", Notify::messages::INFO_TILT_ACTIVATION_CONFIRMATION, "Motor Tilt Activation", "Proceed with the Tilt activation?",

"I00008", Notify::messages::INFO_SLIDE_ACTIVATION_FAILED, "Motor Slide Activation", "Invalid activation conditions",
"I00009", Notify::messages::INFO_BODY_ACTIVATION_FAILED, "Motor Body Activation", "Invalid activation conditions",
"I00010", Notify::messages::INFO_ARM_ACTIVATION_FAILED, "Motor Arm Activation", "Invalid activation conditions",
"I00011", Notify::messages::INFO_TILT_ACTIVATION_FAILED, "Motor Tilt Activation", "Invalid activation conditions",


"I00012", Notify::messages::INFO_MOTOR_48V_SAFETY_LINE_OFF, "Motor Power Supply Monitoring", "The safety line is off",
"I00013", Notify::messages::INFO_MOTOR_48V_POWER_SUPPLY_OFF, "Motor Power Supply Monitoring", "The power supply is off",
"I00014", Notify::messages::INFO_BURNING_JUMPER_PRESENT, "Power Service Monitoring", "The Burning Jumper is present",
"I00015", Notify::messages::INFO_POWER_LOCK, "Power Service Monitoring", "The power supply lock is active",

"I00016", Notify::messages::INFO_ACTIVATION_MOTOR_MANUAL_DISABLE, "Manual Motor Activation", "The manual activation is disabled in this contest",
"I00017", Notify::messages::INFO_ACTIVATION_MOTOR_SAFETY_DISABLE, "Manual Motor Activation", "The manual activation is disabled for safety",
"I00018", Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE, "Manual Motor Activation", "The manual activation is disabled for pending errors",
"I00018", Notify::messages::INFO_BODY_DRIVER, "Body Motor Monitoring", "The motor driver is in error condition",
"I00018", Notify::messages::INFO_VERTICAL_DRIVER, "Vertical Motor Monitoring", "The motor driver is in error condition",
"I00018", Notify::messages::INFO_TILT_DRIVER, "Tilt Motor Monitoring", "The motor driver is in error condition",
"I00018", Notify::messages::INFO_ARM_DRIVER, "Arm Motor Monitoring", "The motor driver is in error condition",
"I00018", Notify::messages::INFO_SLIDE_DRIVER, "Slide Motor Monitoring", "The motor driver is in error condition",

"I00019", Notify::messages::INFO_POWER_OFF_REQUEST_ACTIVATION, "Power Off Request", "Proceed with the System Power Off?",
"I00020", Notify::messages::INFO_GRID_INOUT_MANUAL_MODE, "Potter Monitoring", "The Grid positioning is set to manual mode (test mode)",




/* -------- LABELS ---------------------------*/

"L00001", Notify::messages::LABEL_ERROR_WINDOW_PANEL, "Error Window Panel", "-",
"L00002", Notify::messages::LABEL_PROJECTION_SELECTION_PANEL, "Projection Selection", "-",
"L00003", Notify::messages::LABEL_ZERO_SETTING_PANEL_TITLE, "Zero Setting Panel", "-",
"L00003", Notify::messages::LABEL_SERVICE_PANEL_TITLE, "Service Panel", "-",
"L00003", Notify::messages::LABEL_CALIBRATION_PANEL_TITLE, "Calibration Panel", "-",
"L00003", Notify::messages::LABEL_ROTATION_TOOL_PANEL_TITLE, "Rotation Tool Panel", "-",


"L00004", Notify::messages::LABEL_ERROR, "ERR", "-",
"L00005", Notify::messages::LABEL_WARNING, "WRN", "-",
"L00006", Notify::messages::LABEL_INFO, "INF", "-",
"L00007", Notify::messages::LABEL_NOT_READY_FOR_EXPOSURE, "Standby", "-",
"L00008", Notify::messages::LABEL_READY_FOR_EXPOSURE, "Ready", "-",
"L00009", Notify::messages::LABEL_X_RAY_ON, "X-Ray Executing", "-",

"L00010", Notify::messages::LABEL_MOTOR_COMMAND_SUCCESS, "Command successfully completed", "-",
"L00011", Notify::messages::LABEL_MOTOR_COMMAND_MANUAL_TERMINATION, "Command manually terminated", "-",
"L00012", Notify::messages::LABEL_MOTOR_ERROR_OBSTACLE_DETECTED, "Obstacle detected error", "-",
"L00013", Notify::messages::LABEL_MOTOR_ERROR_MOTOR_BUSY, "Motor busy error", "-",
"L00014", Notify::messages::LABEL_MOTOR_ERROR_INITIALIZATION, "Command initialization error", "-",
"L00015", Notify::messages::LABEL_MOTOR_ERROR_UNEXPECTED_STATUS, "Unexpected internal status error", "-",
"L00016", Notify::messages::LABEL_MOTOR_ERROR_LIMIT_SWITCH, "Limit switches activation error", "-",
"L00017", Notify::messages::LABEL_MOTOR_ERROR_BRAKE_DEVICE, "Brake device activation error", "-",
"L00019", Notify::messages::LABEL_MOTOR_ERROR_TIMOUT, "Timeout activation error", "-",
"L00020", Notify::messages::LABEL_MOTOR_ERROR_INTERNAL_FAULT, "Driver internal error", "-",
"L00021", Notify::messages::LABEL_MOTOR_ERROR_ACCESS_REGISTER, "Driver access register error", "-",
"L00022", Notify::messages::LABEL_MOTOR_ERROR_MISSING_HOME, "Missing zero setting error", "-",
"L00023", Notify::messages::LABEL_MOTOR_ERROR_COMMAND_DISABLED, "Motor activation disabled error", "-",
"L00024", Notify::messages::LABEL_MOTOR_ERROR_COMMAND_ABORTED, "Motor activation aborted", "-",

"L00026", Notify::messages::LABEL_COMPRESSION_ACTIVATED, "Compression Activated", "-",
"L00027", Notify::messages::LABEL_ARM_ACTIVATED, "Arm Motor Activated", "-",
"L00028", Notify::messages::LABEL_BODY_ACTIVATED, "Body Motor Activated", "-",
"L00028", Notify::messages::LABEL_VERTICAL_ACTIVATED, "Vertical Motor Activated", "-",

"L00028", Notify::messages::LABEL_SLIDE_ACTIVATED, "Slide Motor Activated", "-",
"L00028", Notify::messages::LABEL_TILT_ACTIVATED, "Tilt Motor Activated", "-",
"L00029", Notify::messages::LABEL_CALIBRATION_XRAY_TUBE_TITLE, "Xray Tube Calibration Panel", "-",



