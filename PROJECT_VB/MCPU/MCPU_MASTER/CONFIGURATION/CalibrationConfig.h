#pragma once
#include "ConfigFile.h"



ref class PaddleConfig
{
public:
    static const System::String^ FILENAME = "PaddleCalibration.cnf";
    static const int     FILE_REVISION = 1;

    //___________________________________________________________________________________________________//
    //  Paddle parameter data position definition
    //___________________________________________________________________________________________________//

    static const int     PADDLE_COLLIMATION = 0;
    static const int     PADDLE_POSITION_OFFSET = 1;
    static const int     PADDLE_WEIGHT = 2;
    
    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_PADDLE_PROSTHESIS = "PADDLE_PROSTHESIS";
    static const System::String^ PARAM_PADDLE_PROSTHESIS_COMMENT = "PADDLE_PROSTHESIS calibration data";
    static const System::String^ PADDLE_PROSTHESIS_COLLIMATION_DEFAULT = "1"; // Format Collimation index
    static const System::String^ PADDLE_PROSTHESIS_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    static const System::String^ PADDLE_PROSTHESIS_WEIGHT_DEFAULT = "10"; //  Weight in Newton

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_PADDLE_BIOP2D = "PADDLE_BIOP2D";
    static const System::String^ PARAM_PADDLE_BIOP2D_COMMENT = "PADDLE_BIOP2D calibration data";
    static const System::String^ PADDLE_BIOP2D_COLLIMATION_DEFAULT = "1"; // Format Collimation index
    static const System::String^ PADDLE_BIOP2D_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    static const System::String^ PADDLE_BIOP2D_WEIGHT_DEFAULT = "10"; //  Weight in Newton

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_PADDLE_BIOP3D = "PADDLE_BIOP3D";
    static const System::String^ PARAM_PADDLE_BIOP3D_COMMENT = "PADDLE_BIOP3D calibration data";
    static const System::String^ PADDLE_BIOP3D_COLLIMATION_DEFAULT = "1"; // Format Collimation index
    static const System::String^ PADDLE_BIOP3D_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    static const System::String^ PADDLE_BIOP3D_WEIGHT_DEFAULT = "10"; //  Weight in Newton

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_PADDLE_TOMO = "PADDLE_TOMO";
    static const System::String^ PARAM_PADDLE_TOMO_COMMENT = "PADDLE_TOMO calibration data";
    static const System::String^ PADDLE_TOMO_COLLIMATION_DEFAULT = "1"; // Format Collimation index
    static const System::String^ PADDLE_TOMO_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    static const System::String^ PADDLE_TOMO_WEIGHT_DEFAULT = "10"; //  Weight in Newton

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_PADDLE_24x30_CONTACT = "PADDLE_24x30_CONTACT";
    static const System::String^ PARAM_PADDLE_24x30_CONTACT_COMMENT = "PADDLE_24x30_CONTACT calibration data";
    static const System::String^ PADDLE_24x30_CONTACT_COLLIMATION_DEFAULT = "1"; // Format Collimation index
    static const System::String^ PADDLE_24x30_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    static const System::String^ PADDLE_24x30_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_PADDLE_18x24_C_CONTACT = "PADDLE_18x24_C_CONTACT";
    static const System::String^ PARAM_PADDLE_18x24_C_CONTACT_COMMENT = "PADDLE_18x24_C_CONTACT calibration data";
    static const System::String^ PADDLE_18x24_C_CONTACT_COLLIMATION_DEFAULT = "1"; // Format Collimation index
    static const System::String^ PADDLE_18x24_C_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    static const System::String^ PADDLE_18x24_C_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_PADDLE_18x24_L_CONTACT = "PADDLE_18x24_L_CONTACT";
    static const System::String^ PARAM_PADDLE_18x24_L_CONTACT_COMMENT = "PADDLE_18x24_L_CONTACT calibration data";
    static const System::String^ PADDLE_18x24_L_CONTACT_COLLIMATION_DEFAULT = "1"; // Format Collimation index
    static const System::String^ PADDLE_18x24_L_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    static const System::String^ PADDLE_18x24_L_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_PADDLE_18x24_R_CONTACT = "PADDLE_18x24_R_CONTACT";
    static const System::String^ PARAM_PADDLE_18x24_R_CONTACT_COMMENT = "PADDLE_18x24_R_CONTACT calibration data";
    static const System::String^ PADDLE_18x24_R_CONTACT_COLLIMATION_DEFAULT = "1"; // Format Collimation index
    static const System::String^ PADDLE_18x24_R_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    static const System::String^ PADDLE_18x24_R_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_PADDLE_10x24_CONTACT = "PADDLE_10x24_CONTACT";
    static const System::String^ PARAM_PADDLE_10x24_CONTACT_COMMENT = "PADDLE_10x24_CONTACT calibration data";
    static const System::String^ PADDLE_10x24_CONTACT_COLLIMATION_DEFAULT = "1"; // Format Collimation index
    static const System::String^ PADDLE_10x24_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    static const System::String^ PADDLE_10x24_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_PADDLE_9x9_MAG = "PADDLE_9x9_MAG";
    static const System::String^ PARAM_PADDLE_9x9_MAG_COMMENT = "PADDLE_9x9_MAG calibration data";
    static const System::String^ PADDLE_9x9_MAG_COLLIMATION_DEFAULT = "1"; // Format Collimation index
    static const System::String^ PADDLE_9x9_MAG_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    static const System::String^ PADDLE_9x9_MAG_WEIGHT_DEFAULT = "10"; //  Weight in Newton

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_PADDLE_D75_MAG = "PADDLE_D75_MAG";
    static const System::String^ PARAM_PADDLE_D75_MAG_COMMENT = "PADDLE_D75_MAG calibration data";
    static const System::String^ PADDLE_D75_MAG_COLLIMATION_DEFAULT = "1"; // Format Collimation index
    static const System::String^ PADDLE_D75_MAG_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    static const System::String^ PADDLE_D75_MAG_WEIGHT_DEFAULT = "10"; //  Weight in Newton

    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_PADDLE_PROSTHESIS, PARAM_PADDLE_PROSTHESIS_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_PROSTHESIS_COLLIMATION_DEFAULT,
            PADDLE_PROSTHESIS_POSITION_OFFSET_DEFAULT,
            PADDLE_PROSTHESIS_WEIGHT_DEFAULT,            
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_BIOP2D, PARAM_PADDLE_BIOP2D_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_BIOP2D_COLLIMATION_DEFAULT,
            PADDLE_BIOP2D_POSITION_OFFSET_DEFAULT,
            PADDLE_BIOP2D_WEIGHT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_BIOP3D, PARAM_PADDLE_BIOP3D_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_BIOP3D_COLLIMATION_DEFAULT,
            PADDLE_BIOP3D_POSITION_OFFSET_DEFAULT,
            PADDLE_BIOP3D_WEIGHT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_TOMO, PARAM_PADDLE_TOMO_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_TOMO_COLLIMATION_DEFAULT,
            PADDLE_TOMO_POSITION_OFFSET_DEFAULT,
            PADDLE_TOMO_WEIGHT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_24x30_CONTACT, PARAM_PADDLE_24x30_CONTACT_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_24x30_CONTACT_COLLIMATION_DEFAULT,
            PADDLE_24x30_CONTACT_POSITION_OFFSET_DEFAULT,
            PADDLE_24x30_CONTACT_WEIGHT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_18x24_C_CONTACT, PARAM_PADDLE_18x24_C_CONTACT_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_18x24_C_CONTACT_COLLIMATION_DEFAULT,
            PADDLE_18x24_C_CONTACT_POSITION_OFFSET_DEFAULT,
            PADDLE_18x24_C_CONTACT_WEIGHT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_18x24_L_CONTACT, PARAM_PADDLE_18x24_L_CONTACT_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_18x24_L_CONTACT_COLLIMATION_DEFAULT,
            PADDLE_18x24_L_CONTACT_POSITION_OFFSET_DEFAULT,
            PADDLE_18x24_L_CONTACT_WEIGHT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_18x24_R_CONTACT, PARAM_PADDLE_18x24_R_CONTACT_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_18x24_R_CONTACT_COLLIMATION_DEFAULT,
            PADDLE_18x24_R_CONTACT_POSITION_OFFSET_DEFAULT,
            PADDLE_18x24_R_CONTACT_WEIGHT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_10x24_CONTACT, PARAM_PADDLE_10x24_CONTACT_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_10x24_CONTACT_COLLIMATION_DEFAULT,
            PADDLE_10x24_CONTACT_POSITION_OFFSET_DEFAULT,
            PADDLE_10x24_CONTACT_WEIGHT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_9x9_MAG, PARAM_PADDLE_9x9_MAG_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_9x9_MAG_COLLIMATION_DEFAULT,
            PADDLE_9x9_MAG_POSITION_OFFSET_DEFAULT,
            PADDLE_9x9_MAG_WEIGHT_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_PADDLE_D75_MAG, PARAM_PADDLE_D75_MAG_COMMENT, CONFIG_FILE_DEFAULT{
            PADDLE_D75_MAG_COLLIMATION_DEFAULT,
            PADDLE_D75_MAG_POSITION_OFFSET_DEFAULT,
            PADDLE_D75_MAG_WEIGHT_DEFAULT,
            }),
        }
    );
};


ref class FilterConfig
{
public:
    static const System::String^ FILENAME = "FilterCalibration.cnf";
    static const int     FILE_REVISION = 1;


    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_FILTER_CALIBRATION = "FILTER_CALIB";
    static const System::String^ PARAM_FILTER_CALIBRATION_COMMENT = "Filter calibration fields";
    static const int     PARAM_FILTER_CALIBRATION_FILTER1 = 0;
    static const int     PARAM_FILTER_CALIBRATION_FILTER2 = 1;
    static const int     PARAM_FILTER_CALIBRATION_FILTER3 = 2;
    static const int     PARAM_FILTER_CALIBRATION_FILTER4 = 3;
    static const int     PARAM_FILTER_CALIBRATION_MIRROR = 4;
    static const int     PARAM_FILTER_CALIBRATION_LIGHTON = 5;

    static const System::String^ PARAM_FILTER_CALIBRATION_FILTER1_DEFAULT   = "50"; // 1um step
    static const System::String^ PARAM_FILTER_CALIBRATION_FILTER2_DEFAULT   = "50"; // 1um step
    static const System::String^ PARAM_FILTER_CALIBRATION_FILTER3_DEFAULT   = "50"; // 1um step
    static const System::String^ PARAM_FILTER_CALIBRATION_FILTER4_DEFAULT   = "50"; // 1um step
    static const System::String^ PARAM_FILTER_CALIBRATION_MIRROR_DEFAULT    = "50"; // 1um step
    static const System::String^ PARAM_FILTER_CALIBRATION_LIGHTON_DEFAULT   = "20"; // seconds ON


    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
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
    static const System::String^ FILENAME = "CollimatorCalibration.cnf";
    static const int     FILE_REVISION = 1;

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD_FT = "COLLI_STANDARD_FT";
    static const System::String^ PARAM_COLLI_STANDARD_FT_COMMENT = "Front and Trap blades for standard collimations";
    static const int     PARAM_COLLI_STANDARD_FT_FRONT = 0;
    static const int     PARAM_COLLI_STANDARD_FT_TRAP = 1;

    static const System::String^ PARAM_COLLI_STANDARD_FT_FRONT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD_FT_TRAP_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD1 = "COLLI_STANDARD1";
    static const System::String^ PARAM_COLLI_STANDARD1_COMMENT = "left Right and Back blades for standard collimation 1";
    static const int     PARAM_COLLI_STANDARD1_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD1_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD1_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD1_LEFT_DEFAULT = "5000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD1_RIGHT_DEFAULT = "5000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD1_BACK_DEFAULT = "5000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD2 = "COLLI_STANDARD2";
    static const System::String^ PARAM_COLLI_STANDARD2_COMMENT = "left Right and Back blades for standard collimation 2";
    static const int     PARAM_COLLI_STANDARD2_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD2_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD2_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD2_LEFT_DEFAULT = "10000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD2_RIGHT_DEFAULT = "10000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD2_BACK_DEFAULT = "10000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD3 = "COLLI_STANDARD3";
    static const System::String^ PARAM_COLLI_STANDARD3_COMMENT = "left Right and Back blades for standard collimation 3";
    static const int     PARAM_COLLI_STANDARD3_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD3_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD3_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD3_LEFT_DEFAULT = "15000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD3_RIGHT_DEFAULT = "15000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD3_BACK_DEFAULT = "15000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD4 = "COLLI_STANDARD4";
    static const System::String^ PARAM_COLLI_STANDARD4_COMMENT = "left Right and Back blades for standard collimation 4";
    static const int     PARAM_COLLI_STANDARD4_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD4_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD4_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD4_LEFT_DEFAULT = "20000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD4_RIGHT_DEFAULT = "20000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD4_BACK_DEFAULT = "20000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD5 = "COLLI_STANDARD5";
    static const System::String^ PARAM_COLLI_STANDARD5_COMMENT = "left Right and Back blades for standard collimation 5";
    static const int     PARAM_COLLI_STANDARD5_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD5_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD5_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD5_LEFT_DEFAULT = "20000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD5_RIGHT_DEFAULT = "20000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD5_BACK_DEFAULT = "20000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD6 = "COLLI_STANDARD6";
    static const System::String^ PARAM_COLLI_STANDARD6_COMMENT = "left Right and Back blades for standard collimation 6";
    static const int     PARAM_COLLI_STANDARD6_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD6_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD6_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD6_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD6_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD6_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD7 = "COLLI_STANDARD7";
    static const System::String^ PARAM_COLLI_STANDARD7_COMMENT = "left Right and Back blades for standard collimation 7";
    static const int     PARAM_COLLI_STANDARD7_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD7_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD7_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD7_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD7_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD7_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD8 = "COLLI_STANDARD8";
    static const System::String^ PARAM_COLLI_STANDARD8_COMMENT = "left Right and Back blades for standard collimation 8";
    static const int     PARAM_COLLI_STANDARD8_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD8_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD8_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD8_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD8_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD8_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD9 = "COLLI_STANDARD9";
    static const System::String^ PARAM_COLLI_STANDARD9_COMMENT = "left Right and Back blades for standard collimation 9";
    static const int     PARAM_COLLI_STANDARD9_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD9_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD9_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD9_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD9_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD9_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD10 = "COLLI_STANDARD10";
    static const System::String^ PARAM_COLLI_STANDARD10_COMMENT = "left Right and Back blades for standard collimation 10";
    static const int     PARAM_COLLI_STANDARD10_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD10_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD10_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD10_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD10_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD10_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD11 = "COLLI_STANDARD11";
    static const System::String^ PARAM_COLLI_STANDARD11_COMMENT = "left Right and Back blades for standard collimation 11";
    static const int     PARAM_COLLI_STANDARD11_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD11_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD11_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD11_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD11_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD11_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD12 = "COLLI_STANDARD12";
    static const System::String^ PARAM_COLLI_STANDARD12_COMMENT = "left Right and Back blades for standard collimation 12";
    static const int     PARAM_COLLI_STANDARD12_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD12_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD12_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD12_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD12_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD12_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD13 = "COLLI_STANDARD13";
    static const System::String^ PARAM_COLLI_STANDARD13_COMMENT = "left Right and Back blades for standard collimation 13";
    static const int     PARAM_COLLI_STANDARD13_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD13_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD13_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD13_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD13_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD13_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD14 = "COLLI_STANDARD14";
    static const System::String^ PARAM_COLLI_STANDARD14_COMMENT = "left Right and Back blades for standard collimation 14";
    static const int     PARAM_COLLI_STANDARD14_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD14_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD14_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD14_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD14_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD14_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD15 = "COLLI_STANDARD15";
    static const System::String^ PARAM_COLLI_STANDARD15_COMMENT = "left Right and Back blades for standard collimation 15";
    static const int     PARAM_COLLI_STANDARD15_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD15_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD15_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD15_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD15_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD15_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD16 = "COLLI_STANDARD16";
    static const System::String^ PARAM_COLLI_STANDARD16_COMMENT = "left Right and Back blades for standard collimation 16";
    static const int     PARAM_COLLI_STANDARD16_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD16_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD16_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD16_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD16_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD16_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD17 = "COLLI_STANDARD17";
    static const System::String^ PARAM_COLLI_STANDARD17_COMMENT = "left Right and Back blades for standard collimation 17";
    static const int     PARAM_COLLI_STANDARD17_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD17_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD17_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD17_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD17_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD17_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD18 = "COLLI_STANDARD18";
    static const System::String^ PARAM_COLLI_STANDARD18_COMMENT = "left Right and Back blades for standard collimation 18";
    static const int     PARAM_COLLI_STANDARD18_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD18_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD18_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD18_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD18_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD18_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD19 = "COLLI_STANDARD19";
    static const System::String^ PARAM_COLLI_STANDARD19_COMMENT = "left Right and Back blades for standard collimation 19";
    static const int     PARAM_COLLI_STANDARD19_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD19_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD19_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD19_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD19_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD19_BACK_DEFAULT = "1000"; // 1um step

    //___________________________________________________________________________________________________//
    static const System::String^ PARAM_COLLI_STANDARD20 = "COLLI_STANDARD20";
    static const System::String^ PARAM_COLLI_STANDARD20_COMMENT = "left Right and Back blades for standard collimation 20";
    static const int     PARAM_COLLI_STANDARD20_LEFT = 0;
    static const int     PARAM_COLLI_STANDARD20_RIGHT = 1;
    static const int     PARAM_COLLI_STANDARD20_BACK = 2;
    static const System::String^ PARAM_COLLI_STANDARD20_LEFT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD20_RIGHT_DEFAULT = "1000"; // 1um step
    static const System::String^ PARAM_COLLI_STANDARD20_BACK_DEFAULT = "1000"; // 1um step

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