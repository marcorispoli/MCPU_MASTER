#pragma once
#include "ConfigFile.h"

/// <summary>
/// \defgroup ModuleMotorParam Motor Parameter Configuration
/// \ingroup ConfigurationFilesDescription
/// 
/// This parameter module implements the parameter related to the Gantry motors. 
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
/// \ingroup ConfigurationFilesModule
/// \internal
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