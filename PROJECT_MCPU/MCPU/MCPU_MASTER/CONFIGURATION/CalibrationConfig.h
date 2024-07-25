#pragma once
#include "ConfigFile.h"
#include "PCB326.h"

/// <summary>
/// \defgroup ModuleParam System Parameter Module
/// This section implements all the configuration files used in the Application.
/// 
/// </summary>
///
/// The System configuration files are based on a common 
/// file structure as following described:
/// 
/// 
/// configuration file content:\n
/// <REVISION,xx>\n
/// 
/// 
/// "comment to the param_name1"\n
/// <"PARAM_NAME1", "data-1", "data-2", "...", "data-j">\n
/// 
/// 
/// "comment to the param_name2"\n
/// <"PARAM_NAME2", "data-1", "data-2", "...", "data-k">\n
///
/// 
/// "comment to the param_name_nn"\n
/// <"PARAM_NAME_nn", "data-1", "data-2", "...", "data-x">\n
///
/// 
/// All the data content are in String format which value type \n
/// depends by the command itself and it is not encoded in the parameter. \n
/// It is up to the application to properly decode the string format value.\n
/// 
/// 
/// The configuration files are stored into the same directory of the \n
/// Applicaiton executable file and cannot be removed from there.\n
/// 
/// 
/// Every configuration file is auto-generated with default values at the first application \n
/// start or in the case the revision code should be changed.\n
/// 
/// 
/// In the case the Application should handle the old configuation file \n
/// it can override a callback that is called whenever a revision change code is detected.\n
/// 
/// 
/// See the ConfigFile class for more details.
/// 
/// 

/// <summary>
/// \defgroup ModuleMotorParam Motor Parameter Configuration
///
/// This parameter module implements the parameter related to the Gantry motors. 
/// \ingroup ModuleParam
///
///  </summary>
/// 
/// # Overview
/// 
/// + **Name of the configuration file:** MotorCalibration.cnf
/// + **Current revision code:** 1;
/// 
/// # Parameter Description
/// 
/// This configuration file contains the following parameters: 
/// + Body Motor Parameters: the parameters controlling the Body rotation;
/// + Vertical Motor Parameters: the parameters controlling the vertical rotation;
/// + Arm Motor Parameters: the parameters controlling the C-ARM rotation;
/// + Shift Motor Parameters: the parameters controlling the SHIFT rotation;
/// + Tilt Motor Parameters: the parameters controlling the TILT rotation;
/// 
/// # Body Motor Parameters
/// 
/// |POSITION|PARAM-CODE|TYPE|DESCRIPTION|
/// |:--|:--|:--|:--|
/// |0|PARAM_POSITION|32 bit Integer|Motor encoder position (motor units) assigned at the startup to avoid the Homing procedure|
/// |1|PARAM_SPEED|16 bit Integer|Speed in 0.1 degree / seconds of the Motor rotation|
/// |2|PARAM_ACC|16 bit Integer|Acceleration in 0.1 degree / seconds^2 of the Motor rotation|
/// |3|PARAM_DEC|16 bit Integer|Deceleration in 0.1 degree / seconds^2 of the Motor rotation|
/// |4|PARAM_HOME_SPEED|16 bit Integer|Speed in 0.1 degree / seconds of the Motor Homing |
/// |5|PARAM_HOME_ACC|16 bit Integer|Acceleration in 0.1 degree / seconds^2 of the Motor Homing|
/// 
/// # Arm Motor Parameters
/// 
/// |POSITION|PARAM-CODE|TYPE|DESCRIPTION|
/// |:--|:--|:--|:--|
/// |0|PARAM_POSITION|32 bit Integer|Motor encoder position (motor units) assigned at the startup to avoid the Homing procedure|
/// |1|PARAM_SPEED|16 bit Integer|Speed in 0.01 degree / seconds of the Motor rotation|
/// |2|PARAM_ACC|16 bit Integer|Acceleration in 0.01 degree / seconds^2 of the Motor rotation|
/// |3|PARAM_DEC|16 bit Integer|Deceleration in 0.01 degree / seconds^2 of the Motor rotation|
/// |4|PARAM_HOME_SPEED|16 bit Integer|Speed in 0.01 degree / seconds of the Motor Homing |
/// |5|PARAM_HOME_ACC|16 bit Integer|Acceleration in 0.01 degree / seconds^2 of the Motor Homing| 
///
/// # Vertical Motor Parameters
/// 
/// |POSITION|PARAM-CODE|TYPE|DESCRIPTION|
/// |:--|:--|:--|:--|
/// |0|PARAM_POSITION|32 bit Integer|Motor encoder position (motor units) assigned at the startup to avoid the Homing procedure|
/// |1|PARAM_SPEED|16 bit Integer|Speed in 1mm / seconds of the Motor rotation|
/// |2|PARAM_ACC|16 bit Integer|Acceleration in 1mm / seconds^2 of the Motor rotation|
/// |3|PARAM_DEC|16 bit Integer|Deceleration in 1mm / seconds^2 of the Motor rotation|
/// |4|PARAM_HOME_SPEED|16 bit Integer|Speed in 1mm / seconds of the Motor Homing |
/// |5|PARAM_HOME_ACC|16 bit Integer|Acceleration 1mm degree / seconds^2 of the Motor Homing| 
///  
/// # PARAM_POSITION Details
/// 
/// This field is an integer value set to the last valid Body position.\n
/// At the system startup, if this value should be set to "UNDEF" \n
/// the Body Motor will perform an automatic homing position at the first \n
/// Automatic activation command.\n
/// 
/// If this field is avalid value, after the startup, the motor \n
/// will take this position as the valid current position and the \n
/// homeing procedure will not be executed.\n
/// 
/// The Default value at the file creation is "UNDEF". \n
///  
///  

/// <summary>
/// \ingroup ModuleMotorParam
/// 
/// This is the class implementing the Body Motor parameter configuration file.
/// </summary>
ref class MotorConfig
{
public:
    literal System::String^ FILENAME = "MotorCalibration.cnf"; //!< This is the name of the configuration file
    literal int     FILE_REVISION = 1;//!< This is the current revision code

    literal System::String^ MOTOR_UNDEFINED_POSITION = "UNDEF";    //!< This is the TAG of the Undefined position 
    literal System::String^ MOTOR_INTERNAL_POSITION = "INTERNAL";    //!< This is the TAG of the Internal position mode 
    literal System::String^ MOTOR_EXTERNAL_UNDEFINED_POSITION = "UNDEF";    //!< This is the TAG of the External not initialized position

    literal int PARAM_EXTERNAL_POSITION = 0;                       //!< Potentiometer zero setting if present 
    literal int PARAM_CURRENT_POSITION = 1;                        //!< Parameter position for the last valid motor position 
    literal int PARAM_AUTO_SPEED = 2;                              //!< Parameter position for the Automatic mode motor speed
    literal int PARAM_AUTO_ACC = 3;                                //!< Parameter position for the Automatic mode  motor acceleration
    literal int PARAM_AUTO_DEC = 4;                                //!< Parameter position for the Automatic mode  motor deceleration 
    literal int PARAM_MANUAL_SPEED = 5;                            //!< Parameter position for the Manual mode  motor speed
    literal int PARAM_MANUAL_ACC = 6;                              //!< Parameter position for the Manual mode  motor acceleration
    literal int PARAM_MANUAL_DEC = 7;                              //!< Parameter position for the Manual mode   motor deceleration 
    literal int PARAM_HOME_SPEED = 8;                              //!< Parameter speed in home procedure
    literal int PARAM_HOME_ACC = 9;                                //!< Parameter acceleration in home procedure
    
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_OBSTACLE = "OBSTACLE_CALIBRATION";                               //!< This is the Obstacle calibration parameter
    literal System::String^ PARAM_OBSTACLE_COMMENT = "OBSTACLE_CALIBRATION : gain[l0:h3] sensitivity[h0:l7] recal-time[l0:h15] enable[..] csx_th[..]";
    literal System::String^ PARAM_OBSTACLE_GAIN_DEFAULT = "0"; //!< 0:3 This is the general sensors gain
    literal System::String^ PARAM_OBSTACLE_SENSITIVITY_DEFAULT = "3"; //!< 0:7 This is the sensors gain with 0 = max sensitivity
    literal System::String^ PARAM_OBSTACLE_RECAL_TIME_DEFAULT = "0"; //!< 0:15 This is detection time causing the recalibration
    literal System::String^ PARAM_OBSTACLE_SENS_ENA_DEFAULT = "3";   //!< 0:255 This is a bit field sensor enable
    literal System::String^ PARAM_OBSTACLE_TH_CS1_DEFAULT = "64";           //!< 1:127 threshold
    literal System::String^ PARAM_OBSTACLE_TH_CS2_DEFAULT = "64";           //!< 1:127 threshold
    literal System::String^ PARAM_OBSTACLE_TH_CS3_DEFAULT = "64";           //!< 1:127 threshold
    literal System::String^ PARAM_OBSTACLE_TH_CS4_DEFAULT = "64";           //!< 1:127 threshold
    literal System::String^ PARAM_OBSTACLE_TH_CS5_DEFAULT = "64";           //!< 1:127 threshold
    literal System::String^ PARAM_OBSTACLE_TH_CS6_DEFAULT = "64";           //!< 1:127 threshold
    literal System::String^ PARAM_OBSTACLE_TH_CS7_DEFAULT = "64";           //!< 1:127 threshold
    literal System::String^ PARAM_OBSTACLE_TH_CS8_DEFAULT = "64";           //!< 1:127 threshold

   
    literal int PARAM_OBSTACLE_GAIN = 0;
    literal int PARAM_OBSTACLE_SENSITIVITY = 1;
    literal int PARAM_OBSTACLE_RECAL_TIME = 2;
    literal int PARAM_OBSTACLE_SENS_ENA = 3;
    literal int PARAM_OBSTACLE_TH_CS1 = 4;
    literal int PARAM_OBSTACLE_TH_CS2 = 5;
    literal int PARAM_OBSTACLE_TH_CS3 = 6;
    literal int PARAM_OBSTACLE_TH_CS4 = 7;
    literal int PARAM_OBSTACLE_TH_CS5 = 8;
    literal int PARAM_OBSTACLE_TH_CS6 = 9;
    literal int PARAM_OBSTACLE_TH_CS7 = 10;
    literal int PARAM_OBSTACLE_TH_CS8 = 11;



    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_BODY = "BODY_CALIBRATION";                               //!< This is the Body Motor parameter section
    literal System::String^ PARAM_BODY_COMMENT = "BODY_CALIBRATION calibration data";
    literal System::String^ PARAM_BODY_EXTERNAL_ZERO_DEFAULT = MOTOR_EXTERNAL_UNDEFINED_POSITION;           //!< Initial pot zero setting
    literal System::String^ PARAM_BODY_CURRENT_POSITION_DEFAULT = MOTOR_UNDEFINED_POSITION;        //!< The last valid Body Motor position (for initial zero setting)
    literal System::String^ PARAM_BODY_AUTO_SPEED_DEFAULT = "50";                                  //!< Body Auto Speed in 0.1 degree/s
    literal System::String^ PARAM_BODY_AUTO_ACC_DEFAULT = "20";                                    //!< Body Auto Acc in 0.1 degree/ss
    literal System::String^ PARAM_BODY_AUTO_DEC_DEFAULT = "20";                                    //!< Body Auto  Dec in 0.1 degree/ss
    literal System::String^ PARAM_BODY_MANUAL_SPEED_DEFAULT = "50";                                //!< Body Manual  Speed in 0.1 degree/s
    literal System::String^ PARAM_BODY_MANUAL_ACC_DEFAULT = "20";                                  //!< Body Manual  Acc in 0.1 degree/ss
    literal System::String^ PARAM_BODY_MANUAL_DEC_DEFAULT = "100";                                  //!< Body Manual Dec in 0.1 degree/ss
    literal System::String^ PARAM_BODY_HOME_SPEED_DEFAULT = "50";                                  //!< Body Home Speed in 0.1 degree/ss
    literal System::String^ PARAM_BODY_HOME_ACC_DEFAULT = "10";                                    //!< Body Home Acc in 0.1 degree/ss

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_ARM = "ARM_CALIBRATION";                                         //!< This is the ARM Motor parameter section
    literal System::String^ PARAM_ARM_COMMENT = "ARM_CALIBRATION calibration data";
    literal System::String^ PARAM_ARM_EXTERNAL_ZERO_DEFAULT = MOTOR_INTERNAL_POSITION;            //!< Initial pot zero setting
    literal System::String^ PARAM_ARM_CURRENT_POSITION_DEFAULT = MOTOR_UNDEFINED_POSITION;         //!< The last valid encoder ARM Motor position (for initial zero setting)
    literal System::String^ PARAM_ARM_AUTO_SPEED_DEFAULT = "1000";                                 //!< Arm Auto Speed in 0.01 degree/s
    literal System::String^ PARAM_ARM_AUTO_ACC_DEFAULT = "500";                                    //!< Arm Auto Acc in 0.01 degree/ss
    literal System::String^ PARAM_ARM_AUTO_DEC_DEFAULT = "500";                                    //!< Arm Auto Dec in 0.01 degree/ss
    literal System::String^ PARAM_ARM_MANUAL_SPEED_DEFAULT = "500";                               //!< Arm Manual Speed in 0.01 degree/s
    literal System::String^ PARAM_ARM_MANUAL_ACC_DEFAULT = "300";                                  //!< Arm Manual Acc in 0.01 degree/ss
    literal System::String^ PARAM_ARM_MANUAL_DEC_DEFAULT = "500";                                  //!< Arm Manual Dec in 0.01 degree/ss
    literal System::String^ PARAM_ARM_HOME_SPEED_DEFAULT = "5000";                                 //!< Arm Home Speed in 0.01 degree/ss
    literal System::String^ PARAM_ARM_HOME_ACC_DEFAULT = "100";                                    //!< Arm Home Acc in 0.01 degree/ss

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_VERTICAL = "VERTICAL_CALIBRATION";                                   //!< This is the VERTICAL Motor parameter section
    literal System::String^ PARAM_VERTICAL_COMMENT = "VERTICAL_CALIBRATION calibration data";
    literal System::String^ PARAM_VERTICAL_EXTERNAL_ZERO_DEFAULT = MOTOR_INTERNAL_POSITION;                        //!< Initial pot zero setting
    literal System::String^ PARAM_VERTICAL_CURRENT_POSITION_DEFAULT = MOTOR_UNDEFINED_POSITION;        //!< The last valid encoder VERTICAL Motor position (for initial zero setting)
    literal System::String^ PARAM_VERTICAL_AUTO_SPEED_DEFAULT = "30";                                  //!< VERTICAL Auto Speed in 1 mm/s
    literal System::String^ PARAM_VERTICAL_AUTO_ACC_DEFAULT = "20";                                    //!< VERTICAL Auto Acc in 1mm /ss
    literal System::String^ PARAM_VERTICAL_AUTO_DEC_DEFAULT = "10";                                    //!< VERTICAL Auto Dec in 1mm /ss
    literal System::String^ PARAM_VERTICAL_MANUAL_SPEED_DEFAULT = "30";                                //!< VERTICAL Manual Speed in 1 mm/s
    literal System::String^ PARAM_VERTICAL_MANUAL_ACC_DEFAULT = "20";                                  //!< VERTICAL Manual Acc in 1mm /ss
    literal System::String^ PARAM_VERTICAL_MANUAL_DEC_DEFAULT = "60";                                  //!< VERTICAL Manual Dec in 1mm /ss
    literal System::String^ PARAM_VERTICAL_HOME_SPEED_DEFAULT = "20";                                  //!< VERTICAL Home Speed in 1mm degree/ss
    literal System::String^ PARAM_VERTICAL_HOME_ACC_DEFAULT = "10";                                    //!< VERTICAL Home Acc in 1mm degree/ss

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TILT = "TILT_CALIBRATION";                                       //!< This is the TILT Motor parameter section
    literal System::String^ PARAM_TILT_COMMENT = "TILT_CALIBRATION calibration data";
    literal System::String^ PARAM_TILT_EXTERNAL_ZERO_DEFAULT = MOTOR_INTERNAL_POSITION;                         //!< Initial pot zero setting
    literal System::String^ PARAM_TILT_CURRENT_POSITION_DEFAULT = MOTOR_UNDEFINED_POSITION;         //!< The last valid encoder TILT Motor position (for initial zero setting)
    literal System::String^ PARAM_TILT_AUTO_SPEED_DEFAULT = "400";                                  //!< TILT Auto Speed in 0.01 degree/s
    literal System::String^ PARAM_TILT_AUTO_ACC_DEFAULT = "400";                                    //!< TILT Auto Acc in 0.01 degree /ss
    literal System::String^ PARAM_TILT_AUTO_DEC_DEFAULT = "400";                                    //!< TILT Auto Dec in 0.01 degree /ss
    literal System::String^ PARAM_TILT_MANUAL_SPEED_DEFAULT = "100";                                //!< TILT Manual Speed in 0.01 degree mm/s
    literal System::String^ PARAM_TILT_MANUAL_ACC_DEFAULT = "100";                                  //!< TILT Manual Acc in 0.01 degree /ss
    literal System::String^ PARAM_TILT_MANUAL_DEC_DEFAULT = "200";                                  //!< TILT Manual Dec in 0.01 degree /ss
    literal System::String^ PARAM_TILT_HOME_SPEED_DEFAULT = "200";                                  //!< TILT Home Speed in 0.01 degree degree/ss
    literal System::String^ PARAM_TILT_HOME_ACC_DEFAULT = "100";                                    //!< TILT Home Acc in 0.01 degree degree/ss

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_SLIDE = "SLIDE_CALIBRATION";                                       //!< This is the SLIDE Motor parameter section
    literal System::String^ PARAM_SLIDE_COMMENT = "SLIDE_CALIBRATION calibration data";
    literal System::String^ PARAM_SLIDE_EXTERNAL_ZERO_DEFAULT = MOTOR_INTERNAL_POSITION;                         //!< Initial pot zero setting
    literal System::String^ PARAM_SLIDE_CURRENT_POSITION_DEFAULT = MOTOR_UNDEFINED_POSITION;         //!< The last valid encoder SLIDE Motor position (for initial zero setting)
    literal System::String^ PARAM_SLIDE_AUTO_SPEED_DEFAULT = "400";                                  //!< SLIDE Auto Speed in 0.01 degree/s
    literal System::String^ PARAM_SLIDE_AUTO_ACC_DEFAULT = "400";                                    //!< SLIDE Auto Acc in 0.01 degree/ss
    literal System::String^ PARAM_SLIDE_AUTO_DEC_DEFAULT = "400";                                    //!< SLIDE Auto Dec in 0.01 degree/ss
    literal System::String^ PARAM_SLIDE_MANUAL_SPEED_DEFAULT = "400";                                //!< SLIDE Manual Speed in 0.01 degree/s
    literal System::String^ PARAM_SLIDE_MANUAL_ACC_DEFAULT = "400";                                  //!< SLIDE Manual Acc in 0.01 degree/ss
    literal System::String^ PARAM_SLIDE_MANUAL_DEC_DEFAULT = "400";                                  //!< SLIDE Manual Dec in 0.01 degree/ss
    literal System::String^ PARAM_SLIDE_HOME_SPEED_DEFAULT = "100";                                  //!< SLIDE Home Speed in 0.01 degree/ss
    literal System::String^ PARAM_SLIDE_HOME_ACC_DEFAULT = "100";                                    //!< SLIDE Home Acc in 0.01 degree/ss

    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_OBSTACLE, PARAM_OBSTACLE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_OBSTACLE_GAIN_DEFAULT,
            PARAM_OBSTACLE_SENSITIVITY_DEFAULT,
            PARAM_OBSTACLE_RECAL_TIME_DEFAULT,
            PARAM_OBSTACLE_SENS_ENA_DEFAULT,  
            PARAM_OBSTACLE_TH_CS1_DEFAULT,
            PARAM_OBSTACLE_TH_CS2_DEFAULT,
            PARAM_OBSTACLE_TH_CS3_DEFAULT,
            PARAM_OBSTACLE_TH_CS4_DEFAULT,
            PARAM_OBSTACLE_TH_CS5_DEFAULT,
            PARAM_OBSTACLE_TH_CS6_DEFAULT,
            PARAM_OBSTACLE_TH_CS7_DEFAULT,
            PARAM_OBSTACLE_TH_CS8_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_BODY, PARAM_BODY_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_BODY_EXTERNAL_ZERO_DEFAULT,
            PARAM_BODY_CURRENT_POSITION_DEFAULT,
            PARAM_BODY_AUTO_SPEED_DEFAULT,
            PARAM_BODY_AUTO_ACC_DEFAULT,
            PARAM_BODY_AUTO_DEC_DEFAULT,
            PARAM_BODY_MANUAL_SPEED_DEFAULT,
            PARAM_BODY_MANUAL_ACC_DEFAULT,
            PARAM_BODY_MANUAL_DEC_DEFAULT,
            PARAM_BODY_HOME_SPEED_DEFAULT,
            PARAM_BODY_HOME_ACC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_ARM, PARAM_ARM_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_ARM_EXTERNAL_ZERO_DEFAULT,
            PARAM_ARM_CURRENT_POSITION_DEFAULT,
            PARAM_ARM_AUTO_SPEED_DEFAULT,
            PARAM_ARM_AUTO_ACC_DEFAULT,
            PARAM_ARM_AUTO_DEC_DEFAULT,
            PARAM_ARM_MANUAL_SPEED_DEFAULT,
            PARAM_ARM_MANUAL_ACC_DEFAULT,
            PARAM_ARM_MANUAL_DEC_DEFAULT,
            PARAM_ARM_HOME_SPEED_DEFAULT,
            PARAM_ARM_HOME_ACC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_VERTICAL, PARAM_VERTICAL_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_VERTICAL_EXTERNAL_ZERO_DEFAULT,
            PARAM_VERTICAL_CURRENT_POSITION_DEFAULT,
            PARAM_VERTICAL_AUTO_SPEED_DEFAULT,
            PARAM_VERTICAL_AUTO_ACC_DEFAULT,
            PARAM_VERTICAL_AUTO_DEC_DEFAULT,
            PARAM_VERTICAL_MANUAL_SPEED_DEFAULT,
            PARAM_VERTICAL_MANUAL_ACC_DEFAULT,
            PARAM_VERTICAL_MANUAL_DEC_DEFAULT,
            PARAM_VERTICAL_HOME_SPEED_DEFAULT,
            PARAM_VERTICAL_HOME_ACC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TILT, PARAM_TILT_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TILT_EXTERNAL_ZERO_DEFAULT,
            PARAM_TILT_CURRENT_POSITION_DEFAULT,
            PARAM_TILT_AUTO_SPEED_DEFAULT,
            PARAM_TILT_AUTO_ACC_DEFAULT,
            PARAM_TILT_AUTO_DEC_DEFAULT,
            PARAM_TILT_MANUAL_SPEED_DEFAULT,
            PARAM_TILT_MANUAL_ACC_DEFAULT,
            PARAM_TILT_MANUAL_DEC_DEFAULT,
            PARAM_TILT_HOME_SPEED_DEFAULT,
            PARAM_TILT_HOME_ACC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_SLIDE, PARAM_SLIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_SLIDE_EXTERNAL_ZERO_DEFAULT,
            PARAM_SLIDE_CURRENT_POSITION_DEFAULT,
            PARAM_SLIDE_AUTO_SPEED_DEFAULT,
            PARAM_SLIDE_AUTO_ACC_DEFAULT,
            PARAM_SLIDE_AUTO_DEC_DEFAULT,
            PARAM_SLIDE_MANUAL_SPEED_DEFAULT,
            PARAM_SLIDE_MANUAL_ACC_DEFAULT,
            PARAM_SLIDE_MANUAL_DEC_DEFAULT,
            PARAM_SLIDE_HOME_SPEED_DEFAULT,
            PARAM_SLIDE_HOME_ACC_DEFAULT
            }),

        }
    );
};



ref class FilterConfig
{
public:

    // This is the enumeration for the Filter to Slot assignement
    /*
    enum class FilterAssignment {
        NOT_ASSIGNED = 0,
        ASSIGNED_TO_FILTER1,
        ASSIGNED_TO_FILTER2,
        ASSIGNED_TO_FILTER3,
        ASSIGNED_TO_FILTER4,
    };
    */

    literal System::String^ FILTER_NOT_ASSIGNED = "0";
    literal System::String^ FILTER1_ASSIGNMENT = "1";
    literal System::String^ FILTER2_ASSIGNMENT = "2";
    literal System::String^ FILTER3_ASSIGNMENT = "3";
    literal System::String^ FILTER4_ASSIGNMENT = "4";
    
    literal System::String^ FILENAME = "FilterCalibration.cnf";
    literal int     FILE_REVISION = 1;

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_FILTER_CONFIG = "FILTER_CONFIG";
    literal System::String^ PARAM_FILTER_CONFIG_COMMENT = "Defines the Filter assignment";
    literal int     PARAM_FILTER_RH_POSITION = 0;
    literal int     PARAM_FILTER_AG_POSITION = 1;
    literal int     PARAM_FILTER_AL_POSITION = 2;
    literal int     PARAM_FILTER_CU_POSITION = 3;
    literal int     PARAM_FILTER_MO_POSITION = 4;
    literal int     PARAM_FILTER_DEFAULT_POSITION = 5;

    literal System::String^ PARAM_FILTER_RH_POSITION_DEFAULT = FILTER1_ASSIGNMENT; // FILTER1 position
    literal System::String^ PARAM_FILTER_AG_POSITION_DEFAULT = FILTER2_ASSIGNMENT; // FILTER2 position
    literal System::String^ PARAM_FILTER_AL_POSITION_DEFAULT = FILTER3_ASSIGNMENT; // FILTER3 position
    literal System::String^ PARAM_FILTER_CU_POSITION_DEFAULT = FILTER4_ASSIGNMENT; // FILTER4 position
    literal System::String^ PARAM_FILTER_MO_POSITION_DEFAULT = FILTER_NOT_ASSIGNED;
    literal System::String^ PARAM_FILTER_DEFAULT_POSITION_DEFAULT = FILTER4_ASSIGNMENT; // Assigned to FILTER4 AS DEFAULT
    
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_FILTER_CALIBRATION = "FILTER_CALIB";
    literal System::String^ PARAM_FILTER_CALIBRATION_COMMENT = "Filter calibration fields";
    literal int     PARAM_FILTER_CALIBRATION_FILTER1 = 0;
    literal int     PARAM_FILTER_CALIBRATION_FILTER2 = 1;
    literal int     PARAM_FILTER_CALIBRATION_FILTER3 = 2;
    literal int     PARAM_FILTER_CALIBRATION_FILTER4 = 3;
    literal int     PARAM_FILTER_CALIBRATION_MIRROR = 4;
    literal int     PARAM_FILTER_CALIBRATION_LIGHTON = 5;

    literal System::String^ PARAM_FILTER_CALIBRATION_FILTER1_DEFAULT   = "50"; // 1um step
    literal System::String^ PARAM_FILTER_CALIBRATION_FILTER2_DEFAULT   = "50"; // 1um step
    literal System::String^ PARAM_FILTER_CALIBRATION_FILTER3_DEFAULT   = "50"; // 1um step
    literal System::String^ PARAM_FILTER_CALIBRATION_FILTER4_DEFAULT   = "50"; // 1um step
    literal System::String^ PARAM_FILTER_CALIBRATION_MIRROR_DEFAULT    = "50"; // 1um step
    literal System::String^ PARAM_FILTER_CALIBRATION_LIGHTON_DEFAULT   = "20"; // seconds ON


    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_FILTER_CONFIG, PARAM_FILTER_CONFIG_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_FILTER_RH_POSITION_DEFAULT,
            PARAM_FILTER_AG_POSITION_DEFAULT,
            PARAM_FILTER_AL_POSITION_DEFAULT,
            PARAM_FILTER_CU_POSITION_DEFAULT,
            PARAM_FILTER_MO_POSITION_DEFAULT,
            PARAM_FILTER_DEFAULT_POSITION_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_FILTER_CALIBRATION, PARAM_FILTER_CALIBRATION_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_FILTER_CALIBRATION_FILTER1_DEFAULT,
            PARAM_FILTER_CALIBRATION_FILTER2_DEFAULT,
            PARAM_FILTER_CALIBRATION_FILTER3_DEFAULT,
            PARAM_FILTER_CALIBRATION_FILTER4_DEFAULT,
            PARAM_FILTER_CALIBRATION_MIRROR_DEFAULT,
            PARAM_FILTER_CALIBRATION_LIGHTON_DEFAULT
            }),
    
        }
    );
};

ref class CollimatorConfig
{
public:

    literal System::String^ FILENAME = "CollimatorCalibration.cnf";
    literal int     FILE_REVISION = 1;

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD_FT = "COLLI_STANDARD_FT";
    literal System::String^ PARAM_COLLI_STANDARD_FT_COMMENT = "Front and Trap blades for standard collimations";
    literal int     PARAM_COLLI_STANDARD_FT_FRONT = 0;
    literal int     PARAM_COLLI_STANDARD_FT_TRAP = 1;

    literal System::String^ PARAM_COLLI_STANDARD_FT_FRONT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD_FT_TRAP_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD1 = "COLLI_STANDARD1";
    literal System::String^ PARAM_COLLI_STANDARD1_COMMENT = "left Right and Back blades for standard collimation 1";
    literal int     PARAM_COLLI_STANDARD1_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD1_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD1_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD1_LEFT_DEFAULT = "5000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD1_RIGHT_DEFAULT = "5000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD1_BACK_DEFAULT = "5000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD2 = "COLLI_STANDARD2";
    literal System::String^ PARAM_COLLI_STANDARD2_COMMENT = "left Right and Back blades for standard collimation 2";
    literal int     PARAM_COLLI_STANDARD2_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD2_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD2_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD2_LEFT_DEFAULT = "10000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD2_RIGHT_DEFAULT = "10000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD2_BACK_DEFAULT = "10000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD3 = "COLLI_STANDARD3";
    literal System::String^ PARAM_COLLI_STANDARD3_COMMENT = "left Right and Back blades for standard collimation 3";
    literal int     PARAM_COLLI_STANDARD3_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD3_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD3_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD3_LEFT_DEFAULT = "15000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD3_RIGHT_DEFAULT = "15000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD3_BACK_DEFAULT = "15000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD4 = "COLLI_STANDARD4";
    literal System::String^ PARAM_COLLI_STANDARD4_COMMENT = "left Right and Back blades for standard collimation 4";
    literal int     PARAM_COLLI_STANDARD4_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD4_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD4_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD4_LEFT_DEFAULT = "20000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD4_RIGHT_DEFAULT = "20000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD4_BACK_DEFAULT = "20000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD5 = "COLLI_STANDARD5";
    literal System::String^ PARAM_COLLI_STANDARD5_COMMENT = "left Right and Back blades for standard collimation 5";
    literal int     PARAM_COLLI_STANDARD5_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD5_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD5_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD5_LEFT_DEFAULT = "20000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD5_RIGHT_DEFAULT = "20000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD5_BACK_DEFAULT = "20000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD6 = "COLLI_STANDARD6";
    literal System::String^ PARAM_COLLI_STANDARD6_COMMENT = "left Right and Back blades for standard collimation 6";
    literal int     PARAM_COLLI_STANDARD6_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD6_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD6_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD6_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD6_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD6_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD7 = "COLLI_STANDARD7";
    literal System::String^ PARAM_COLLI_STANDARD7_COMMENT = "left Right and Back blades for standard collimation 7";
    literal int     PARAM_COLLI_STANDARD7_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD7_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD7_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD7_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD7_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD7_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD8 = "COLLI_STANDARD8";
    literal System::String^ PARAM_COLLI_STANDARD8_COMMENT = "left Right and Back blades for standard collimation 8";
    literal int     PARAM_COLLI_STANDARD8_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD8_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD8_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD8_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD8_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD8_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD9 = "COLLI_STANDARD9";
    literal System::String^ PARAM_COLLI_STANDARD9_COMMENT = "left Right and Back blades for standard collimation 9";
    literal int     PARAM_COLLI_STANDARD9_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD9_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD9_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD9_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD9_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD9_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD10 = "COLLI_STANDARD10";
    literal System::String^ PARAM_COLLI_STANDARD10_COMMENT = "left Right and Back blades for standard collimation 10";
    literal int     PARAM_COLLI_STANDARD10_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD10_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD10_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD10_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD10_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD10_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD11 = "COLLI_STANDARD11";
    literal System::String^ PARAM_COLLI_STANDARD11_COMMENT = "left Right and Back blades for standard collimation 11";
    literal int     PARAM_COLLI_STANDARD11_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD11_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD11_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD11_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD11_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD11_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD12 = "COLLI_STANDARD12";
    literal System::String^ PARAM_COLLI_STANDARD12_COMMENT = "left Right and Back blades for standard collimation 12";
    literal int     PARAM_COLLI_STANDARD12_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD12_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD12_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD12_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD12_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD12_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD13 = "COLLI_STANDARD13";
    literal System::String^ PARAM_COLLI_STANDARD13_COMMENT = "left Right and Back blades for standard collimation 13";
    literal int     PARAM_COLLI_STANDARD13_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD13_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD13_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD13_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD13_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD13_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD14 = "COLLI_STANDARD14";
    literal System::String^ PARAM_COLLI_STANDARD14_COMMENT = "left Right and Back blades for standard collimation 14";
    literal int     PARAM_COLLI_STANDARD14_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD14_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD14_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD14_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD14_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD14_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD15 = "COLLI_STANDARD15";
    literal System::String^ PARAM_COLLI_STANDARD15_COMMENT = "left Right and Back blades for standard collimation 15";
    literal int     PARAM_COLLI_STANDARD15_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD15_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD15_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD15_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD15_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD15_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD16 = "COLLI_STANDARD16";
    literal System::String^ PARAM_COLLI_STANDARD16_COMMENT = "left Right and Back blades for standard collimation 16";
    literal int     PARAM_COLLI_STANDARD16_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD16_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD16_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD16_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD16_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD16_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD17 = "COLLI_STANDARD17";
    literal System::String^ PARAM_COLLI_STANDARD17_COMMENT = "left Right and Back blades for standard collimation 17";
    literal int     PARAM_COLLI_STANDARD17_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD17_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD17_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD17_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD17_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD17_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD18 = "COLLI_STANDARD18";
    literal System::String^ PARAM_COLLI_STANDARD18_COMMENT = "left Right and Back blades for standard collimation 18";
    literal int     PARAM_COLLI_STANDARD18_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD18_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD18_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD18_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD18_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD18_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD19 = "COLLI_STANDARD19";
    literal System::String^ PARAM_COLLI_STANDARD19_COMMENT = "left Right and Back blades for standard collimation 19";
    literal int     PARAM_COLLI_STANDARD19_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD19_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD19_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD19_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD19_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD19_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_STANDARD20 = "COLLI_STANDARD20";
    literal System::String^ PARAM_COLLI_STANDARD20_COMMENT = "left Right and Back blades for standard collimation 20";
    literal int     PARAM_COLLI_STANDARD20_LEFT = 0;
    literal int     PARAM_COLLI_STANDARD20_RIGHT = 1;
    literal int     PARAM_COLLI_STANDARD20_BACK = 2;
    literal System::String^ PARAM_COLLI_STANDARD20_LEFT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD20_RIGHT_DEFAULT = "1000"; // 1um step
    literal System::String^ PARAM_COLLI_STANDARD20_BACK_DEFAULT = "1000"; // 1um step

    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
           
            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD_FT, PARAM_COLLI_STANDARD_FT_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD_FT_FRONT_DEFAULT,
            PARAM_COLLI_STANDARD_FT_TRAP_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD1, PARAM_COLLI_STANDARD1_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD1_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD1_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD1_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD2, PARAM_COLLI_STANDARD2_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD2_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD2_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD2_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD3, PARAM_COLLI_STANDARD3_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD3_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD3_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD3_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD4, PARAM_COLLI_STANDARD4_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD4_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD4_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD4_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD5, PARAM_COLLI_STANDARD5_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD5_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD5_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD5_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD6, PARAM_COLLI_STANDARD6_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD6_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD6_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD6_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD7, PARAM_COLLI_STANDARD7_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD7_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD7_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD7_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD8, PARAM_COLLI_STANDARD8_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD8_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD8_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD8_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD9, PARAM_COLLI_STANDARD9_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD9_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD9_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD9_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD10, PARAM_COLLI_STANDARD10_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD10_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD10_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD10_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD11, PARAM_COLLI_STANDARD11_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD11_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD11_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD11_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD12, PARAM_COLLI_STANDARD12_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD12_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD12_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD12_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD13, PARAM_COLLI_STANDARD13_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD13_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD13_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD13_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD14, PARAM_COLLI_STANDARD14_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD14_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD14_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD14_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD15, PARAM_COLLI_STANDARD15_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD15_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD15_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD15_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD16, PARAM_COLLI_STANDARD16_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD16_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD16_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD16_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD17, PARAM_COLLI_STANDARD17_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD17_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD17_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD17_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD18, PARAM_COLLI_STANDARD18_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD18_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD18_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD18_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD19, PARAM_COLLI_STANDARD19_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD19_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD19_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD19_BACK_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_STANDARD20, PARAM_COLLI_STANDARD20_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_STANDARD20_LEFT_DEFAULT,
            PARAM_COLLI_STANDARD20_RIGHT_DEFAULT,
            PARAM_COLLI_STANDARD20_BACK_DEFAULT,
            }),

        }
    );
};

ref class PaddleConfig
{
public:
    literal System::String^ FILENAME = "PaddleCalibration.cnf";
    literal int     FILE_REVISION = 3;

    //___________________________________________________________________________________________________//
    //  Position caibration parameters
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_POSITION_CALIBRATION = "POSITION_CALIBRATION";
    literal System::String^ PARAM_POSITION_CALIBRATION_COMMENT = "Position calibration parameters";
    literal int     POSITION_CALIBRATION_STATUS = 0; //!< Set to 1 if the calibration has been executed
    literal int     POSITION_MAX_HEIGHT = 1;        //!< Defines the maximum position in Compressor units
    literal int     POSITION_HOLDER_OFFSET = 2;     //!< Holder calibration: converts the position to the distance from the holder and the carbon fiber
    literal System::String^ POSITION_CALIBRATION_STATUS_DEFAULT = "0";  //!<  No calibrated as default
    literal System::String^ POSITION_MAX_HEIGHT_DEFAULT = "3000";          //!<  Max heigh parameter in 0.1mm value
    literal System::String^ POSITION_HOLDER_OFFSET_DEFAULT = "0";           //!<  Offset to convert the holder position in distance from the carbon fiber

    //___________________________________________________________________________________________________//
    //  Paddle parameter data position definition
    //___________________________________________________________________________________________________//

    literal int     PADDLE_COLLIMATION = 0;     //!< This is the autoomatic collimation slot assigned to the paddle
    literal int     PADDLE_POSITION_OFFSET = 1; //!< This is the position offset for the thickness calculation
    literal int     PADDLE_WEIGHT = 2;          //!< This is the weight of the Paddle assembly when the ARM angle is 0
    literal int     PADDLE_TAG = 3;             //!< This is the TAG value assigned to the paddle

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_PROSTHESIS = "PADDLE_PROSTHESIS";
    literal System::String^ PARAM_PADDLE_PROSTHESIS_COMMENT = "PADDLE_PROSTHESIS calibration data";
    literal System::String^ PADDLE_PROSTHESIS_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD1; // Format Collimation index
    literal System::String^ PADDLE_PROSTHESIS_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_PROSTHESIS_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_PROSTHESIS_TAG_DEFAULT = "1"; //  Tag Code

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_BIOP2D = "PADDLE_BIOP2D";
    literal System::String^ PARAM_PADDLE_BIOP2D_COMMENT = "PADDLE_BIOP2D calibration data";
    literal System::String^ PADDLE_BIOP2D_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD2; // Format Collimation index
    literal System::String^ PADDLE_BIOP2D_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_BIOP2D_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_BIOP2D_TAG_DEFAULT = "2"; //  Tag Code

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_BIOP3D = "PADDLE_BIOP3D";
    literal System::String^ PARAM_PADDLE_BIOP3D_COMMENT = "PADDLE_BIOP3D calibration data";
    literal System::String^ PADDLE_BIOP3D_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD3; // Format Collimation index
    literal System::String^ PADDLE_BIOP3D_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_BIOP3D_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_BIOP3D_TAG_DEFAULT = "3"; //  Tag Code

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_TOMO = "PADDLE_TOMO";
    literal System::String^ PARAM_PADDLE_TOMO_COMMENT = "PADDLE_TOMO calibration data";
    literal System::String^ PADDLE_TOMO_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD4; // Format Collimation index
    literal System::String^ PADDLE_TOMO_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_TOMO_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_TOMO_TAG_DEFAULT = "4"; //  Tag Code

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_24x30_CONTACT = "PADDLE_24x30_CONTACT";
    literal System::String^ PARAM_PADDLE_24x30_CONTACT_COMMENT = "PADDLE_24x30_CONTACT calibration data";
    literal System::String^ PADDLE_24x30_CONTACT_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD5; // Format Collimation index
    literal System::String^ PADDLE_24x30_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_24x30_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_24x30_TAG_DEFAULT = "5"; //  Tag Code
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_18x24_C_CONTACT = "PADDLE_18x24_C_CONTACT";
    literal System::String^ PARAM_PADDLE_18x24_C_CONTACT_COMMENT = "PADDLE_18x24_C_CONTACT calibration data";
    literal System::String^ PADDLE_18x24_C_CONTACT_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD6; // Format Collimation index
    literal System::String^ PADDLE_18x24_C_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_18x24_C_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_18x24_C_TAG_DEFAULT = "6"; //  Tag Code
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_18x24_L_CONTACT = "PADDLE_18x24_L_CONTACT";
    literal System::String^ PARAM_PADDLE_18x24_L_CONTACT_COMMENT = "PADDLE_18x24_L_CONTACT calibration data";
    literal System::String^ PADDLE_18x24_L_CONTACT_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD7; // Format Collimation index
    literal System::String^ PADDLE_18x24_L_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_18x24_L_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_18x24_L_TAG_DEFAULT = "7"; //  Tag Code
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_18x24_R_CONTACT = "PADDLE_18x24_R_CONTACT";
    literal System::String^ PARAM_PADDLE_18x24_R_CONTACT_COMMENT = "PADDLE_18x24_R_CONTACT calibration data";
    literal System::String^ PADDLE_18x24_R_CONTACT_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD8; // Format Collimation index
    literal System::String^ PADDLE_18x24_R_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_18x24_R_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_18x24_R_TAG_DEFAULT = "8"; //  Tag Code
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_10x24_CONTACT = "PADDLE_10x24_CONTACT";
    literal System::String^ PARAM_PADDLE_10x24_CONTACT_COMMENT = "PADDLE_10x24_CONTACT calibration data";
    literal System::String^ PADDLE_10x24_CONTACT_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD9; // Format Collimation index
    literal System::String^ PADDLE_10x24_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_10x24_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_10x24_CONTACT_TAG_DEFAULT = "9"; //  Tag Code
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_9x9_MAG = "PADDLE_9x9_MAG";
    literal System::String^ PARAM_PADDLE_9x9_MAG_COMMENT = "PADDLE_9x9_MAG calibration data";
    literal System::String^ PADDLE_9x9_MAG_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD10; // Format Collimation index
    literal System::String^ PADDLE_9x9_MAG_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_9x9_MAG_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_9x9_MAG_TAG_DEFAULT = "10"; //  Tag Code
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_9x21_MAG = "PADDLE_9x21_MAG";
    literal System::String^ PARAM_PADDLE_9x21_MAG_COMMENT = "PADDLE_9x21_MAG calibration data";
    literal System::String^ PADDLE_9x21_MAG_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD11; // Format Collimation index
    literal System::String^ PADDLE_9x21_MAG_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_9x21_MAG_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_9x21_MAG_TAG_DEFAULT = "11"; //  Tag Code
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_D75_MAG = "PADDLE_D75_MAG";
    literal System::String^ PARAM_PADDLE_D75_MAG_COMMENT = "PADDLE_D75_MAG calibration data";
    literal System::String^ PADDLE_D75_MAG_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD12; // Format Collimation index
    literal System::String^ PADDLE_D75_MAG_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_D75_MAG_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_D75_MAG_TAG_DEFAULT = "12"; //  Tag Code

    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_POSITION_CALIBRATION, PARAM_POSITION_CALIBRATION_COMMENT, CONFIG_FILE_DEFAULT{
            POSITION_CALIBRATION_STATUS_DEFAULT,
            POSITION_MAX_HEIGHT_DEFAULT,
            POSITION_HOLDER_OFFSET_DEFAULT,            
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_PROSTHESIS, PARAM_PADDLE_PROSTHESIS_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_PROSTHESIS_COLLIMATION_DEFAULT,
            PADDLE_PROSTHESIS_POSITION_OFFSET_DEFAULT,
            PADDLE_PROSTHESIS_WEIGHT_DEFAULT,
            PADDLE_PROSTHESIS_TAG_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_BIOP2D, PARAM_PADDLE_BIOP2D_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_BIOP2D_COLLIMATION_DEFAULT,
            PADDLE_BIOP2D_POSITION_OFFSET_DEFAULT,
            PADDLE_BIOP2D_WEIGHT_DEFAULT,
            PADDLE_BIOP2D_TAG_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_BIOP3D, PARAM_PADDLE_BIOP3D_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_BIOP3D_COLLIMATION_DEFAULT,
            PADDLE_BIOP3D_POSITION_OFFSET_DEFAULT,
            PADDLE_BIOP3D_WEIGHT_DEFAULT,
            PADDLE_BIOP3D_TAG_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_TOMO, PARAM_PADDLE_TOMO_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_TOMO_COLLIMATION_DEFAULT,
            PADDLE_TOMO_POSITION_OFFSET_DEFAULT,
            PADDLE_TOMO_WEIGHT_DEFAULT,
            PADDLE_TOMO_TAG_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_24x30_CONTACT, PARAM_PADDLE_24x30_CONTACT_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_24x30_CONTACT_COLLIMATION_DEFAULT,
            PADDLE_24x30_CONTACT_POSITION_OFFSET_DEFAULT,
            PADDLE_24x30_CONTACT_WEIGHT_DEFAULT,
            PADDLE_24x30_TAG_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_18x24_C_CONTACT, PARAM_PADDLE_18x24_C_CONTACT_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_18x24_C_CONTACT_COLLIMATION_DEFAULT,
            PADDLE_18x24_C_CONTACT_POSITION_OFFSET_DEFAULT,
            PADDLE_18x24_C_CONTACT_WEIGHT_DEFAULT,
            PADDLE_18x24_C_TAG_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_18x24_L_CONTACT, PARAM_PADDLE_18x24_L_CONTACT_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_18x24_L_CONTACT_COLLIMATION_DEFAULT,
            PADDLE_18x24_L_CONTACT_POSITION_OFFSET_DEFAULT,
            PADDLE_18x24_L_CONTACT_WEIGHT_DEFAULT,
            PADDLE_18x24_L_TAG_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_18x24_R_CONTACT, PARAM_PADDLE_18x24_R_CONTACT_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_18x24_R_CONTACT_COLLIMATION_DEFAULT,
            PADDLE_18x24_R_CONTACT_POSITION_OFFSET_DEFAULT,
            PADDLE_18x24_R_CONTACT_WEIGHT_DEFAULT,
            PADDLE_18x24_R_TAG_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_10x24_CONTACT, PARAM_PADDLE_10x24_CONTACT_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_10x24_CONTACT_COLLIMATION_DEFAULT,
            PADDLE_10x24_CONTACT_POSITION_OFFSET_DEFAULT,
            PADDLE_10x24_CONTACT_WEIGHT_DEFAULT,
            PADDLE_10x24_CONTACT_TAG_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_9x9_MAG, PARAM_PADDLE_9x9_MAG_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_9x9_MAG_COLLIMATION_DEFAULT,
            PADDLE_9x9_MAG_POSITION_OFFSET_DEFAULT,
            PADDLE_9x9_MAG_WEIGHT_DEFAULT,
            PADDLE_9x9_MAG_TAG_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_9x21_MAG, PARAM_PADDLE_9x21_MAG_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_9x21_MAG_COLLIMATION_DEFAULT,
            PADDLE_9x21_MAG_POSITION_OFFSET_DEFAULT,
            PADDLE_9x21_MAG_WEIGHT_DEFAULT,
            PADDLE_9x21_MAG_TAG_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_D75_MAG, PARAM_PADDLE_D75_MAG_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_D75_MAG_COLLIMATION_DEFAULT,
            PADDLE_D75_MAG_POSITION_OFFSET_DEFAULT,
            PADDLE_D75_MAG_WEIGHT_DEFAULT,
            PADDLE_D75_MAG_TAG_DEFAULT
            }),
        }
    );
};

ref class TomoConfig
{
public:

    enum class tomo_id {
        TOMO1F_NARROW = 0,
        TOMO1F_INTERMEDIATE,
        TOMO1F_WIDE,
        TOMO2F_NARROW,
        TOMO2F_INTERMEDIATE,
        TOMO2F_WIDE,
        TOMO3F_NARROW,
        TOMO3F_INTERMEDIATE,
        TOMO3F_WIDE,
        TOMO4F_NARROW,
        TOMO4F_INTERMEDIATE,
        TOMO4F_WIDE,
        TOMO5F_NARROW,
        TOMO5F_INTERMEDIATE,
        TOMO5F_WIDE,
        TOMO_ID_NUM
    };

    literal System::String^ FILENAME = "TomoConfig.cnf";
    literal int     FILE_REVISION = 1;

    literal int     PARAM_TOMO_HOME = 0;
    literal int     PARAM_TOMO_END = 1;
    literal int     PARAM_TOMO_FPS = 2;
    literal int     PARAM_TOMO_SAMPLES = 3;
    literal int     PARAM_TOMO_SKIP = 4;
    literal int     PARAM_TOMO_SPEED = 5;
    literal int     PARAM_TOMO_ACC = 6;
    literal int     PARAM_TOMO_DEC = 7;


    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO1F_NARROW = "TOMO1F_NARROW";
    literal System::String^ PARAM_TOMO1F_NARROW_COMMENT = "Defines the Tomo 1F NARROW configuration";
    literal System::String^ PARAM_TOMO1F_NARROW_HOME_DEFAULT = "825";
    literal System::String^ PARAM_TOMO1F_NARROW_END_DEFAULT = "-825";
    literal System::String^ PARAM_TOMO1F_NARROW_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO1F_NARROW_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_NARROW_SPEED_DEFAULT = "150";
    literal System::String^ PARAM_TOMO1F_NARROW_ACC_DEFAULT = "150";
    literal System::String^ PARAM_TOMO1F_NARROW_DEC_DEFAULT = "150";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE = "TOMO1F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_COMMENT = "Defines the Tomo 1F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_HOME_DEFAULT = "1300";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_END_DEFAULT = "-1300";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_SPEED_DEFAULT = "200";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_ACC_DEFAULT = "200";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_DEC_DEFAULT = "200";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO1F_WIDE = "TOMO1F_WIDE";
    literal System::String^ PARAM_TOMO1F_WIDE_COMMENT = "Defines the Tomo 1F WIDE configuration";
    literal System::String^ PARAM_TOMO1F_WIDE_HOME_DEFAULT = "1900";
    literal System::String^ PARAM_TOMO1F_WIDE_END_DEFAULT = "-1900";
    literal System::String^ PARAM_TOMO1F_WIDE_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_WIDE_SAMPLES_DEFAULT = "19";
    literal System::String^ PARAM_TOMO1F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_WIDE_SPEED_DEFAULT = "200";
    literal System::String^ PARAM_TOMO1F_WIDE_ACC_DEFAULT = "200";
    literal System::String^ PARAM_TOMO1F_WIDE_DEC_DEFAULT = "200";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO2F_NARROW = "TOMO2F_NARROW";
    literal System::String^ PARAM_TOMO2F_NARROW_COMMENT = "Defines the Tomo 2F NARROW configuration";
    literal System::String^ PARAM_TOMO2F_NARROW_HOME_DEFAULT = "825";
    literal System::String^ PARAM_TOMO2F_NARROW_END_DEFAULT = "-825";
    literal System::String^ PARAM_TOMO2F_NARROW_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO2F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO2F_NARROW_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO2F_NARROW_SPEED_DEFAULT = "300";
    literal System::String^ PARAM_TOMO2F_NARROW_ACC_DEFAULT = "600";
    literal System::String^ PARAM_TOMO2F_NARROW_DEC_DEFAULT = "600";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE = "TOMO2F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_COMMENT = "Defines the Tomo 2F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_HOME_DEFAULT = "1300";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_END_DEFAULT = "-1300";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_SPEED_DEFAULT = "400";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_ACC_DEFAULT = "800";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_DEC_DEFAULT = "800";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO2F_WIDE = "TOMO2F_WIDE";
    literal System::String^ PARAM_TOMO2F_WIDE_COMMENT = "Defines the Tomo 2F WIDE configuration";
    literal System::String^ PARAM_TOMO2F_WIDE_HOME_DEFAULT = "1900";
    literal System::String^ PARAM_TOMO2F_WIDE_END_DEFAULT = "-1900";
    literal System::String^ PARAM_TOMO2F_WIDE_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO2F_WIDE_SAMPLES_DEFAULT = "19";
    literal System::String^ PARAM_TOMO2F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO2F_WIDE_SPEED_DEFAULT = "400";
    literal System::String^ PARAM_TOMO2F_WIDE_ACC_DEFAULT = "800";
    literal System::String^ PARAM_TOMO2F_WIDE_DEC_DEFAULT = "800";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO3F_NARROW = "TOMO3F_NARROW";
    literal System::String^ PARAM_TOMO3F_NARROW_COMMENT = "Defines the Tomo 3F NARROW configuration";
    literal System::String^ PARAM_TOMO3F_NARROW_HOME_DEFAULT = "825";
    literal System::String^ PARAM_TOMO3F_NARROW_END_DEFAULT = "-825";
    literal System::String^ PARAM_TOMO3F_NARROW_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO3F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO3F_NARROW_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO3F_NARROW_SPEED_DEFAULT = "300";
    literal System::String^ PARAM_TOMO3F_NARROW_ACC_DEFAULT = "600";
    literal System::String^ PARAM_TOMO3F_NARROW_DEC_DEFAULT = "600";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE = "TOMO3F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_COMMENT = "Defines the Tomo 3F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_HOME_DEFAULT = "1300";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_END_DEFAULT = "-1300";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_SPEED_DEFAULT = "400";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_ACC_DEFAULT = "800";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_DEC_DEFAULT = "800";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO3F_WIDE = "TOMO3F_WIDE";
    literal System::String^ PARAM_TOMO3F_WIDE_COMMENT = "Defines the Tomo 3F WIDE configuration";
    literal System::String^ PARAM_TOMO3F_WIDE_HOME_DEFAULT = "2608";
    literal System::String^ PARAM_TOMO3F_WIDE_END_DEFAULT = "-2608";
    literal System::String^ PARAM_TOMO3F_WIDE_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO3F_WIDE_SAMPLES_DEFAULT = "24";
    literal System::String^ PARAM_TOMO3F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO3F_WIDE_SPEED_DEFAULT = "435";
    literal System::String^ PARAM_TOMO3F_WIDE_ACC_DEFAULT = "869";
    literal System::String^ PARAM_TOMO3F_WIDE_DEC_DEFAULT = "869";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO4F_NARROW = "TOMO4F_NARROW";
    literal System::String^ PARAM_TOMO4F_NARROW_COMMENT = "Defines the Tomo 4F NARROW configuration";
    literal System::String^ PARAM_TOMO4F_NARROW_HOME_DEFAULT = "975";
    literal System::String^ PARAM_TOMO4F_NARROW_END_DEFAULT = "-975";
    literal System::String^ PARAM_TOMO4F_NARROW_FPS_DEFAULT = "4";
    literal System::String^ PARAM_TOMO4F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO4F_NARROW_SKIP_DEFAULT = "3";
    literal System::String^ PARAM_TOMO4F_NARROW_SPEED_DEFAULT = "600";
    literal System::String^ PARAM_TOMO4F_NARROW_ACC_DEFAULT = "800";
    literal System::String^ PARAM_TOMO4F_NARROW_DEC_DEFAULT = "800";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE = "TOMO4F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_COMMENT = "Defines the Tomo 4F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_HOME_DEFAULT = "1500";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_END_DEFAULT = "-1500";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_FPS_DEFAULT = "3";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_SKIP_DEFAULT = "3";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_SPEED_DEFAULT = "600";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_ACC_DEFAULT = "600";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_DEC_DEFAULT = "600";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO4F_WIDE = "TOMO4F_WIDE";
    literal System::String^ PARAM_TOMO4F_WIDE_COMMENT = "Defines the Tomo 4F WIDE configuration";
    literal System::String^ PARAM_TOMO4F_WIDE_HOME_DEFAULT = "2550";
    literal System::String^ PARAM_TOMO4F_WIDE_END_DEFAULT = "-2550";
    literal System::String^ PARAM_TOMO4F_WIDE_FPS_DEFAULT = "3";
    literal System::String^ PARAM_TOMO4F_WIDE_SAMPLES_DEFAULT = "24";
    literal System::String^ PARAM_TOMO4F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO4F_WIDE_SPEED_DEFAULT = "675";
    literal System::String^ PARAM_TOMO4F_WIDE_ACC_DEFAULT = "900";
    literal System::String^ PARAM_TOMO4F_WIDE_DEC_DEFAULT = "900";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO5F_NARROW = "TOMO5F_NARROW";
    literal System::String^ PARAM_TOMO5F_NARROW_COMMENT = "Defines the Tomo 5F NARROW configuration";
    literal System::String^ PARAM_TOMO5F_NARROW_HOME_DEFAULT = "825";
    literal System::String^ PARAM_TOMO5F_NARROW_END_DEFAULT = "-825";
    literal System::String^ PARAM_TOMO5F_NARROW_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO5F_NARROW_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_NARROW_SPEED_DEFAULT = "150";
    literal System::String^ PARAM_TOMO5F_NARROW_ACC_DEFAULT = "150";
    literal System::String^ PARAM_TOMO5F_NARROW_DEC_DEFAULT = "150";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE = "TOMO5F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_COMMENT = "Defines the Tomo 5F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_HOME_DEFAULT = "1300";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_END_DEFAULT = "-1300";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_SPEED_DEFAULT = "200";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_ACC_DEFAULT = "200";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_DEC_DEFAULT = "200";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO5F_WIDE = "TOMO5F_WIDE";
    literal System::String^ PARAM_TOMO5F_WIDE_COMMENT = "Defines the Tomo 5F WIDE configuration";
    literal System::String^ PARAM_TOMO5F_WIDE_HOME_DEFAULT = "2609";
    literal System::String^ PARAM_TOMO5F_WIDE_END_DEFAULT = "-2609";
    literal System::String^ PARAM_TOMO5F_WIDE_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_WIDE_SAMPLES_DEFAULT = "24";
    literal System::String^ PARAM_TOMO5F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_WIDE_SPEED_DEFAULT = "217";
    literal System::String^ PARAM_TOMO5F_WIDE_ACC_DEFAULT = "217";
    literal System::String^ PARAM_TOMO5F_WIDE_DEC_DEFAULT = "217";


    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_TOMO1F_NARROW, PARAM_TOMO1F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO1F_NARROW_HOME_DEFAULT,
            PARAM_TOMO1F_NARROW_END_DEFAULT,
            PARAM_TOMO1F_NARROW_FPS_DEFAULT,
            PARAM_TOMO1F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO1F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO1F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO1F_NARROW_ACC_DEFAULT,
            PARAM_TOMO1F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO1F_INTERMEDIATE, PARAM_TOMO1F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO1F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO1F_WIDE, PARAM_TOMO1F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO1F_WIDE_HOME_DEFAULT,
            PARAM_TOMO1F_WIDE_END_DEFAULT,
            PARAM_TOMO1F_WIDE_FPS_DEFAULT,
            PARAM_TOMO1F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO1F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO1F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO1F_WIDE_ACC_DEFAULT,
            PARAM_TOMO1F_WIDE_DEC_DEFAULT
            }),
           
            CONFIG_FILE_ITEM(PARAM_TOMO2F_NARROW, PARAM_TOMO2F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO2F_NARROW_HOME_DEFAULT,
            PARAM_TOMO2F_NARROW_END_DEFAULT,
            PARAM_TOMO2F_NARROW_FPS_DEFAULT,
            PARAM_TOMO2F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO2F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO2F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO2F_NARROW_ACC_DEFAULT,
            PARAM_TOMO2F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO2F_INTERMEDIATE, PARAM_TOMO2F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO2F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO2F_WIDE, PARAM_TOMO2F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO2F_WIDE_HOME_DEFAULT,
            PARAM_TOMO2F_WIDE_END_DEFAULT,
            PARAM_TOMO2F_WIDE_FPS_DEFAULT,
            PARAM_TOMO2F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO2F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO2F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO2F_WIDE_ACC_DEFAULT,
            PARAM_TOMO2F_WIDE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO3F_NARROW, PARAM_TOMO3F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO3F_NARROW_HOME_DEFAULT,
            PARAM_TOMO3F_NARROW_END_DEFAULT,
            PARAM_TOMO3F_NARROW_FPS_DEFAULT,
            PARAM_TOMO3F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO3F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO3F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO3F_NARROW_ACC_DEFAULT,
            PARAM_TOMO3F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO3F_INTERMEDIATE, PARAM_TOMO3F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO3F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO3F_WIDE, PARAM_TOMO3F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO3F_WIDE_HOME_DEFAULT,
            PARAM_TOMO3F_WIDE_END_DEFAULT,
            PARAM_TOMO3F_WIDE_FPS_DEFAULT,
            PARAM_TOMO3F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO3F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO3F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO3F_WIDE_ACC_DEFAULT,
            PARAM_TOMO3F_WIDE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO4F_NARROW, PARAM_TOMO4F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO4F_NARROW_HOME_DEFAULT,
            PARAM_TOMO4F_NARROW_END_DEFAULT,
            PARAM_TOMO4F_NARROW_FPS_DEFAULT,
            PARAM_TOMO4F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO4F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO4F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO4F_NARROW_ACC_DEFAULT,
            PARAM_TOMO4F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO4F_INTERMEDIATE, PARAM_TOMO4F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO4F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO4F_WIDE, PARAM_TOMO4F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO4F_WIDE_HOME_DEFAULT,
            PARAM_TOMO4F_WIDE_END_DEFAULT,
            PARAM_TOMO4F_WIDE_FPS_DEFAULT,
            PARAM_TOMO4F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO4F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO4F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO4F_WIDE_ACC_DEFAULT,
            PARAM_TOMO4F_WIDE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO5F_NARROW, PARAM_TOMO5F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO5F_NARROW_HOME_DEFAULT,
            PARAM_TOMO5F_NARROW_END_DEFAULT,
            PARAM_TOMO5F_NARROW_FPS_DEFAULT,
            PARAM_TOMO5F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO5F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO5F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO5F_NARROW_ACC_DEFAULT,
            PARAM_TOMO5F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO5F_INTERMEDIATE, PARAM_TOMO5F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO5F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO5F_WIDE, PARAM_TOMO5F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO5F_WIDE_HOME_DEFAULT,
            PARAM_TOMO5F_WIDE_END_DEFAULT,
            PARAM_TOMO5F_WIDE_FPS_DEFAULT,
            PARAM_TOMO5F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO5F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO5F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO5F_WIDE_ACC_DEFAULT,
            PARAM_TOMO5F_WIDE_DEC_DEFAULT
            }),


        }
    );
};

ref class DetectorConfig
{
public:

    /// <summary>
    /// This is the enumeration of the possible Detectors used for exposures
    /// 
    /// </summary>
    enum class detector_model_option {
        GENERIC = 0,    //!< Generic Detector Type
        LMAM2V2,        //!< Digital Analogic LMAM2 V2
        DRTECH,         //!< Digital DRTECH Detector
        DETECTOR_LIST_SIZE
    };
    literal int     MAX_TOMO_FPS = 6;

    literal System::String^ FILENAME = "DetecorConfiguration.cnf";
    literal int     FILE_REVISION = 1;

    literal int     PARAM_PRE_TO_PULSE_AEC_WAITING_TIME = 0;
    literal int     PARAM_MAX_2D_INTEGRATION_TIME = 1;
    literal int     PARAM_MAX_2D_PRE_INTEGRATION_TIME = 2;
    literal int     PARAM_MAX_3D_INTEGRATION_TIME_1FPS = 3;
    literal int     PARAM_MAX_3D_INTEGRATION_TIME_2FPS = 4;
    literal int     PARAM_MAX_3D_INTEGRATION_TIME_3FPS = 5;
    literal int     PARAM_MAX_3D_INTEGRATION_TIME_4FPS = 6;
    literal int     PARAM_MAX_3D_INTEGRATION_TIME_5FPS = 7;
    literal int     PARAM_MAX_3D_INTEGRATION_TIME_6FPS = 8;
    literal int     PARAM_MAX_3D_FPS = 9;


    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_GENERIC_CONFIG = "GENERIC";
    literal System::String^ PARAM_GENERIC_COMMENT = "This is the Generic Detector description";
    

    literal System::String^ PARAM_GENERIC_PRE_TO_PULSE_AEC_WAITING_TIME_DEFAULT = "5000"; 
    literal System::String^ PARAM_GENERIC_MAX_2D_INTEGRATION_TIME_DEFAULT = "5000";
    literal System::String^ PARAM_GENERIC_MAX_2D_PRE_INTEGRATION_TIME_DEFAULT = "1000";
    literal System::String^ PARAM_GENERIC_MAX_3D_INTEGRATION_TIME_1FPS_DEFAULT = "500";
    literal System::String^ PARAM_GENERIC_MAX_3D_INTEGRATION_TIME_2FPS_DEFAULT = "250";
    literal System::String^ PARAM_GENERIC_MAX_3D_INTEGRATION_TIME_3FPS_DEFAULT = "200";
    literal System::String^ PARAM_GENERIC_MAX_3D_INTEGRATION_TIME_4FPS_DEFAULT = "120";
    literal System::String^ PARAM_GENERIC_MAX_3D_INTEGRATION_TIME_5FPS_DEFAULT = "80";
    literal System::String^ PARAM_GENERIC_MAX_3D_INTEGRATION_TIME_6FPS_DEFAULT = "33";
    literal System::String^ PARAM_GENERIC_MAX_3D_FPS_DEFAULT = "4";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_LMAM2V2_CONFIG = "LMAM2V2";
    literal System::String^ PARAM_LMAM2V2_COMMENT = "This is the LMAM2V2 Detector description";


    literal System::String^ PARAM_LMAM2V2_PRE_TO_PULSE_AEC_WAITING_TIME_DEFAULT = "5000";
    literal System::String^ PARAM_LMAM2V2_MAX_2D_INTEGRATION_TIME_DEFAULT = "5000";
    literal System::String^ PARAM_LMAM2V2_MAX_2D_PRE_INTEGRATION_TIME_DEFAULT = "1000";
    literal System::String^ PARAM_LMAM2V2_MAX_3D_INTEGRATION_TIME_1FPS_DEFAULT = "500";
    literal System::String^ PARAM_LMAM2V2_MAX_3D_INTEGRATION_TIME_2FPS_DEFAULT = "250";
    literal System::String^ PARAM_LMAM2V2_MAX_3D_INTEGRATION_TIME_3FPS_DEFAULT = "200";
    literal System::String^ PARAM_LMAM2V2_MAX_3D_INTEGRATION_TIME_4FPS_DEFAULT = "120";
    literal System::String^ PARAM_LMAM2V2_MAX_3D_INTEGRATION_TIME_5FPS_DEFAULT = "80";
    literal System::String^ PARAM_LMAM2V2_MAX_3D_INTEGRATION_TIME_6FPS_DEFAULT = "33";
    literal System::String^ PARAM_LMAM2V2_MAX_3D_FPS_DEFAULT = "4";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_DRTECH_CONFIG = "DRTECH";
    literal System::String^ PARAM_DRTECH_COMMENT = "This is the DRTECH Detector description";


    literal System::String^ PARAM_DRTECH_PRE_TO_PULSE_AEC_WAITING_TIME_DEFAULT = "5000";
    literal System::String^ PARAM_DRTECH_MAX_2D_INTEGRATION_TIME_DEFAULT = "5000";
    literal System::String^ PARAM_DRTECH_MAX_2D_PRE_INTEGRATION_TIME_DEFAULT = "1000";
    literal System::String^ PARAM_DRTECH_MAX_3D_INTEGRATION_TIME_1FPS_DEFAULT = "500";
    literal System::String^ PARAM_DRTECH_MAX_3D_INTEGRATION_TIME_2FPS_DEFAULT = "250";
    literal System::String^ PARAM_DRTECH_MAX_3D_INTEGRATION_TIME_3FPS_DEFAULT = "200";
    literal System::String^ PARAM_DRTECH_MAX_3D_INTEGRATION_TIME_4FPS_DEFAULT = "120";
    literal System::String^ PARAM_DRTECH_MAX_3D_INTEGRATION_TIME_5FPS_DEFAULT = "80";
    literal System::String^ PARAM_DRTECH_MAX_3D_INTEGRATION_TIME_6FPS_DEFAULT = "33";
    literal System::String^ PARAM_DRTECH_MAX_3D_FPS_DEFAULT = "4";

    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_GENERIC_CONFIG, PARAM_GENERIC_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_GENERIC_PRE_TO_PULSE_AEC_WAITING_TIME_DEFAULT,
            PARAM_GENERIC_MAX_2D_INTEGRATION_TIME_DEFAULT,
            PARAM_GENERIC_MAX_2D_PRE_INTEGRATION_TIME_DEFAULT,
            PARAM_GENERIC_MAX_3D_INTEGRATION_TIME_1FPS_DEFAULT,
            PARAM_GENERIC_MAX_3D_INTEGRATION_TIME_2FPS_DEFAULT,
            PARAM_GENERIC_MAX_3D_INTEGRATION_TIME_3FPS_DEFAULT,
            PARAM_GENERIC_MAX_3D_INTEGRATION_TIME_4FPS_DEFAULT,
            PARAM_GENERIC_MAX_3D_INTEGRATION_TIME_5FPS_DEFAULT,
            PARAM_GENERIC_MAX_3D_INTEGRATION_TIME_6FPS_DEFAULT,
            PARAM_GENERIC_MAX_3D_FPS_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_LMAM2V2_CONFIG, PARAM_LMAM2V2_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_LMAM2V2_PRE_TO_PULSE_AEC_WAITING_TIME_DEFAULT,
            PARAM_LMAM2V2_MAX_2D_INTEGRATION_TIME_DEFAULT,
            PARAM_LMAM2V2_MAX_2D_PRE_INTEGRATION_TIME_DEFAULT,
            PARAM_LMAM2V2_MAX_3D_INTEGRATION_TIME_1FPS_DEFAULT,
            PARAM_LMAM2V2_MAX_3D_INTEGRATION_TIME_2FPS_DEFAULT,
            PARAM_LMAM2V2_MAX_3D_INTEGRATION_TIME_3FPS_DEFAULT,
            PARAM_LMAM2V2_MAX_3D_INTEGRATION_TIME_4FPS_DEFAULT,
            PARAM_LMAM2V2_MAX_3D_INTEGRATION_TIME_5FPS_DEFAULT,
            PARAM_LMAM2V2_MAX_3D_INTEGRATION_TIME_6FPS_DEFAULT,
            PARAM_LMAM2V2_MAX_3D_FPS_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_DRTECH_CONFIG, PARAM_DRTECH_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_DRTECH_PRE_TO_PULSE_AEC_WAITING_TIME_DEFAULT,
            PARAM_DRTECH_MAX_2D_INTEGRATION_TIME_DEFAULT,
            PARAM_DRTECH_MAX_2D_PRE_INTEGRATION_TIME_DEFAULT,
            PARAM_DRTECH_MAX_3D_INTEGRATION_TIME_1FPS_DEFAULT,
            PARAM_DRTECH_MAX_3D_INTEGRATION_TIME_2FPS_DEFAULT,
            PARAM_DRTECH_MAX_3D_INTEGRATION_TIME_3FPS_DEFAULT,
            PARAM_DRTECH_MAX_3D_INTEGRATION_TIME_4FPS_DEFAULT,
            PARAM_DRTECH_MAX_3D_INTEGRATION_TIME_5FPS_DEFAULT,
            PARAM_DRTECH_MAX_3D_INTEGRATION_TIME_6FPS_DEFAULT,
            PARAM_DRTECH_MAX_3D_FPS_DEFAULT
            }),

        }
    );
};