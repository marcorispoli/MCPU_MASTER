#pragma once
#include "ConfigFile.h"
#include "CollimatorConfig.h"

ref class PaddleConfig
{
public:
    literal System::String^ FILENAME = "PaddleCalibration.cnf";
    literal int     FILE_REVISION = 3;

    //___________________________________________________________________________________________________//
    //  Compressor caibration parameters
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COMPRESSOR = "COMPRESSOR_DEVICE";
    literal System::String^ PARAM_COMPRESSOR_COMMENT = "Compressor device general parameters";
    literal int     COMPRESSOR_CALIBRATION_POSITION_STATUS = 0; //!< Set to 1 if the calibration position has been executed
    literal int     COMPRESSOR_CALIBRATION_FORCE_STATUS = 1;    //!< Set to 1 if the calibration force has been executed
    literal int     COMPRESSOR_HOLDER_OP = 2;                   //!< Holder calibration: Op parameter
    literal int     COMPRESSOR_HOLDER_KP = 3;                   //!< Holder calibration: Kp parameter
    literal int     COMPRESSOR_HOLDER_MAX_POSITION = 4;         //!< Maximum holder position (mm)
    literal int     COMPRESSOR_HOLDER_MIN_POSITION = 5;         //!< Minimum Holder Position (mm)
    literal int     COMPRESSOR_TARGET_FORCE = 6;                //!< Automatic Target force (mm)
    literal int     COMPRESSOR_MAX_FORCE = 7;                   //!< Absolute automatic compression force

    literal System::String^ COMPRESSOR_CALIBRATION_POSITION_STATUS_DEFAULT = "0";  //!<  No calibrated as default
    literal System::String^ COMPRESSOR_CALIBRATION_FORCE_STATUS_DEFAULT = "0";  //!<  No calibrated as default
    literal System::String^ COMPRESSOR_HOLDER_OP_DEFAULT = "0";
    literal System::String^ COMPRESSOR_HOLDER_KP_DEFAULT = "100";
    literal System::String^ COMPRESSOR_HOLDER_MAX_POSITION_DEFAULT = "300";
    literal System::String^ COMPRESSOR_HOLDER_MIN_POSITION_DEFAULT = "0";
    literal System::String^ COMPRESSOR_TARGET_FORCE_DEFAULT = "150";
    literal System::String^ COMPRESSOR_MAX_FORCE_DEFAULT = "200";


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
    literal System::String^ PADDLE_9x9_MAG_POSITION_OFFSET_DEFAULT = "0"; // mm offset 
    literal System::String^ PADDLE_9x9_MAG_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_9x9_MAG_TAG_DEFAULT = "10"; //  Tag Code
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_9x21_MAG = "PADDLE_9x21_MAG";
    literal System::String^ PARAM_PADDLE_9x21_MAG_COMMENT = "PADDLE_9x21_MAG calibration data";
    literal System::String^ PADDLE_9x21_MAG_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD11; // Format Collimation index
    literal System::String^ PADDLE_9x21_MAG_POSITION_OFFSET_DEFAULT = "0"; // mm offset 
    literal System::String^ PADDLE_9x21_MAG_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_9x21_MAG_TAG_DEFAULT = "11"; //  Tag Code
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_D75_MAG = "PADDLE_D75_MAG";
    literal System::String^ PARAM_PADDLE_D75_MAG_COMMENT = "PADDLE_D75_MAG calibration data";
    literal System::String^ PADDLE_D75_MAG_COLLIMATION_DEFAULT = CollimatorConfig::PARAM_COLLI_STANDARD12; // Format Collimation index
    literal System::String^ PADDLE_D75_MAG_POSITION_OFFSET_DEFAULT = "0"; // mm offset 
    literal System::String^ PADDLE_D75_MAG_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_D75_MAG_TAG_DEFAULT = "12"; //  Tag Code


    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_COMPRESSOR, PARAM_COMPRESSOR_COMMENT, CONFIG_FILE_DEFAULT{
            COMPRESSOR_CALIBRATION_POSITION_STATUS_DEFAULT,
            COMPRESSOR_CALIBRATION_FORCE_STATUS_DEFAULT,
            COMPRESSOR_HOLDER_OP_DEFAULT,
            COMPRESSOR_HOLDER_KP_DEFAULT,
            COMPRESSOR_HOLDER_MAX_POSITION_DEFAULT,
            COMPRESSOR_HOLDER_MIN_POSITION_DEFAULT,
            COMPRESSOR_TARGET_FORCE_DEFAULT,
            COMPRESSOR_MAX_FORCE_DEFAULT
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
