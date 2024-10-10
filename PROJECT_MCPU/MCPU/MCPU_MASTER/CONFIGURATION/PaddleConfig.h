#pragma once
#include "ConfigFile.h"


/// \addtogroup ConfigurationFilesDescription 
/// <div style="page-break-after: always;"></div>
/// 
/// \section PaddleConfig PaddleCalibration.cnf configuration file
/// 
/// ## Overview
/// 
/// The Application handles several parameters that configure and calibrate 
/// the compressor device. 
/// 
/// 
/// + **Name of the configuration file:** PaddleCalibration.cnf
/// + **Current revision code:** 1;
/// 
/// ## Parameter Description
/// 
/// This configuration file contains the following parameter rows: 
/// + Compressor Parameters: sets the compressor related parameters;
/// + PROSTHESIS Paddle descriptor;
/// + BIOP2D Paddle descriptor;
/// + BIOP3D Paddle descriptor;
/// + TOMO Paddle descriptor;
/// + 24x30-CONTACT Paddle descriptor;
/// + 18x24_C_CONTACT Paddle descriptor;
/// + 18x24_L_CONTACT Paddle descriptor;
/// + 10x24_CONTACT Paddle descriptor;
/// + 9x9_MAG Paddle descriptor;
/// + 9x21_MAG Paddle descriptor;
/// + D75_MAG Paddle descriptor;
/// 
/// 

ref class PaddleConfig
{
public:
    literal System::String^ FILENAME = "PaddleCalibration.cnf";
    literal int     FILE_REVISION = 3;

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### Compressor Calibration Parameters Row
    /// 
    /// This parameter sets the Compressor calibration parameters.
    /// 
    /// < COMPRESSOR_DEVICE, PosCalib,ForceCalib,HolderOffset,HolderK,HolderMaxPos,HolderMinPos,TargetForce,MaxForce >
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |PosCalib|Byte|0|0:1|Position calibration status: 1=Calibrated, 0 = Not Calibrated|
    /// |ForceCalib|Byte|0|0:1|Force calibration status: 1=Calibrated, 0 = Not Calibrated|
    /// |HolderOffset|Word|0|-|Holder Calibration Offset |
    /// |HolderK|Word|100|-|Percent of linear correction|
    /// |HolderMaxPos|Word|300|-|Maximum holder position in mm|
    /// |HolderMinPos|Word|0|-|Minimum holder position in mm|
    /// |TargetForce|Word|150|-|Current Automatic Target compression in N|
    /// |MaxForce|Word|200|70:200|Maximum Autmatic Compression Force in N|
    /// 
    literal System::String^ PARAM_COMPRESSOR = "COMPRESSOR_DEVICE";
    literal System::String^ PARAM_COMPRESSOR_COMMENT = "Compressor device general parameters";
    literal int     COMPRESSOR_CALIBRATION_POSITION_STATUS = 0; //!< Set to 1 if the calibration position has been executed
    literal int     COMPRESSOR_CALIBRATION_FORCE_STATUS = 1;    //!< Set to 1 if the calibration force has been executed
    literal int     COMPRESSOR_HOLDER_OP = 2;                   //!< Holder calibration: Op parameter
    literal int     COMPRESSOR_HOLDER_KP = 3;                   //!< Holder calibration: Kp parameter
    literal int     COMPRESSOR_HOLDER_MAX_POSITION = 4;         //!< Maximum holder position (mm)
    literal int     COMPRESSOR_HOLDER_MIN_POSITION = 5;         //!< Minimum Holder Position (mm)
    literal int     COMPRESSOR_TARGET_FORCE = 6;                //!< Automatic Target force (N)
    literal int     COMPRESSOR_MAX_FORCE = 7;                   //!< Absolute automatic compression force (N)

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

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### Prosthesis Paddle Descriptor Parameters Row
    /// 
    /// This parameter sets the Prosthesis parameters.
    /// 
    /// < PADDLE_PROSTHESIS, CollimationFormat,CompressionOffset,Weight,IdentificationId>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |CollimationFormat|String|"1"|Collimator Format Range|Sets the assigned collimator format|
    /// |CompressionOffset|Word|120|-|Offset of the paddle plane respect of the Holder position in mm|
    /// |Weight|Word|10|-|Paddle Weight in N |
    /// |IdentificationId|Word|1|-|Identifier Code|
    /// 
    ///     NOTE: The collimation format 0 is reserved for the OPEN collimation
    /// 
    literal System::String^ PARAM_PADDLE_PROSTHESIS = "PADDLE_PROSTHESIS";
    literal System::String^ PARAM_PADDLE_PROSTHESIS_COMMENT = "PADDLE_PROSTHESIS calibration data";
    literal System::String^ PADDLE_PROSTHESIS_COLLIMATION_DEFAULT = "1"; // Format Collimation index
    literal System::String^ PADDLE_PROSTHESIS_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_PROSTHESIS_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_PROSTHESIS_TAG_DEFAULT = "1"; //  Tag Code

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### BIOP_2D Paddle Descriptor Parameters Row
    /// 
    /// This parameter sets the BIOP_2D parameters.
    /// 
    /// < PADDLE_BIOP2D, CollimationFormat,CompressionOffset,Weight,IdentificationId>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |CollimationFormat|String|"2"|Collimator Format Range|Sets the assigned collimator format|
    /// |CompressionOffset|Word|120|-|Offset of the paddle plane respect of the Holder position in mm|
    /// |Weight|Word|10|-|Paddle Weight in N |
    /// |IdentificationId|Word|2|-|Identifier Code|
    /// 
    ///     NOTE: The collimation format 0 is reserved for the OPEN collimation
    /// 
    literal System::String^ PARAM_PADDLE_BIOP2D = "PADDLE_BIOP2D";
    literal System::String^ PARAM_PADDLE_BIOP2D_COMMENT = "PADDLE_BIOP2D calibration data";
    literal System::String^ PADDLE_BIOP2D_COLLIMATION_DEFAULT = "2"; // Format Collimation index
    literal System::String^ PADDLE_BIOP2D_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_BIOP2D_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_BIOP2D_TAG_DEFAULT = "2"; //  Tag Code

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### BIOP_3D Paddle Descriptor Parameters Row
    /// 
    /// This parameter sets the BIOP_3D parameters.
    /// 
    /// < PADDLE_BIOP3D, CollimationFormat,CompressionOffset,Weight,IdentificationId>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |CollimationFormat|String|"3"|Collimator Format Range|Sets the assigned collimator format|
    /// |CompressionOffset|Word|120|-|Offset of the paddle plane respect of the Holder position in mm|
    /// |Weight|Word|10|-|Paddle Weight in N |
    /// |IdentificationId|Word|3|-|Identifier Code|
    /// 
    ///     NOTE: The collimation format 0 is reserved for the OPEN collimation
    /// 
    literal System::String^ PARAM_PADDLE_BIOP3D = "PADDLE_BIOP3D";
    literal System::String^ PARAM_PADDLE_BIOP3D_COMMENT = "PADDLE_BIOP3D calibration data";
    literal System::String^ PADDLE_BIOP3D_COLLIMATION_DEFAULT = "3"; // Format Collimation index
    literal System::String^ PADDLE_BIOP3D_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_BIOP3D_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_BIOP3D_TAG_DEFAULT = "3"; //  Tag Code

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### TOMO Paddle Descriptor Parameters Row
    /// 
    /// This parameter sets the TOMO parameters.
    /// 
    /// < PADDLE_TOMO, CollimationFormat,CompressionOffset,Weight,IdentificationId>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |CollimationFormat|String|"4"|Collimator Format Range|Sets the assigned collimator format|
    /// |CompressionOffset|Word|120|-|Offset of the paddle plane respect of the Holder position in mm|
    /// |Weight|Word|10|-|Paddle Weight in N |
    /// |IdentificationId|Word|4|-|Identifier Code|
    /// 
    ///     NOTE: The collimation format 0 is reserved for the OPEN collimation
    /// 
    literal System::String^ PARAM_PADDLE_TOMO = "PADDLE_TOMO";
    literal System::String^ PARAM_PADDLE_TOMO_COMMENT = "PADDLE_TOMO calibration data";
    literal System::String^ PADDLE_TOMO_COLLIMATION_DEFAULT = "4"; // Format Collimation index
    literal System::String^ PADDLE_TOMO_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_TOMO_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_TOMO_TAG_DEFAULT = "4"; //  Tag Code

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### 24x30_CONTACT Paddle Descriptor Parameters Row
    /// 
    /// This parameter sets the 24x30_CONTACT parameters.
    /// 
    /// < PADDLE_24x30_CONTACT, CollimationFormat,CompressionOffset,Weight,IdentificationId>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |CollimationFormat|String|"5"|Collimator Format Range|Sets the assigned collimator format|
    /// |CompressionOffset|Word|120|-|Offset of the paddle plane respect of the Holder position in mm|
    /// |Weight|Word|10|-|Paddle Weight in N |
    /// |IdentificationId|Word|5|-|Identifier Code|
    /// 
    ///     NOTE: The collimation format 0 is reserved for the OPEN collimation
    /// 
    literal System::String^ PARAM_PADDLE_24x30_CONTACT = "PADDLE_24x30_CONTACT";
    literal System::String^ PARAM_PADDLE_24x30_CONTACT_COMMENT = "PADDLE_24x30_CONTACT calibration data";
    literal System::String^ PADDLE_24x30_CONTACT_COLLIMATION_DEFAULT = "5"; // Format Collimation index
    literal System::String^ PADDLE_24x30_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_24x30_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_24x30_TAG_DEFAULT = "5"; //  Tag Code

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### 18x24_C_CONTACT Paddle Descriptor Parameters Row
    /// 
    /// This parameter sets the 18x24_C_CONTACT parameters.
    /// 
    /// < PADDLE_18x24_C_CONTACT, CollimationFormat,CompressionOffset,Weight,IdentificationId>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |CollimationFormat|String|"6"|Collimator Format Range|Sets the assigned collimator format|
    /// |CompressionOffset|Word|120|-|Offset of the paddle plane respect of the Holder position in mm|
    /// |Weight|Word|10|-|Paddle Weight in N |
    /// |IdentificationId|Word|6|-|Identifier Code|
    /// 
    ///     NOTE: The collimation format 0 is reserved for the OPEN collimation
    /// 
    literal System::String^ PARAM_PADDLE_18x24_C_CONTACT = "PADDLE_18x24_C_CONTACT";
    literal System::String^ PARAM_PADDLE_18x24_C_CONTACT_COMMENT = "PADDLE_18x24_C_CONTACT calibration data";
    literal System::String^ PADDLE_18x24_C_CONTACT_COLLIMATION_DEFAULT = "6"; // Format Collimation index
    literal System::String^ PADDLE_18x24_C_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_18x24_C_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_18x24_C_TAG_DEFAULT = "6"; //  Tag Code

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### 18x24_L_CONTACT Paddle Descriptor Parameters Row
    /// 
    /// This parameter sets the 18x24_L_CONTACT parameters.
    /// 
    /// < PADDLE_18x24_L_CONTACT, CollimationFormat,CompressionOffset,Weight,IdentificationId>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |CollimationFormat|String|"7"|Collimator Format Range|Sets the assigned collimator format|
    /// |CompressionOffset|Word|120|-|Offset of the paddle plane respect of the Holder position in mm|
    /// |Weight|Word|10|-|Paddle Weight in N |
    /// |IdentificationId|Word|7|-|Identifier Code|
    /// 
    ///     NOTE: The collimation format 0 is reserved for the OPEN collimation
    /// 
    literal System::String^ PARAM_PADDLE_18x24_L_CONTACT = "PADDLE_18x24_L_CONTACT";
    literal System::String^ PARAM_PADDLE_18x24_L_CONTACT_COMMENT = "PADDLE_18x24_L_CONTACT calibration data";
    literal System::String^ PADDLE_18x24_L_CONTACT_COLLIMATION_DEFAULT = "7"; // Format Collimation index
    literal System::String^ PADDLE_18x24_L_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_18x24_L_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_18x24_L_TAG_DEFAULT = "7"; //  Tag Code

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### 18x24_R_CONTACT Paddle Descriptor Parameters Row
    /// 
    /// This parameter sets the 18x24_R_CONTACT parameters.
    /// 
    /// < PADDLE_18x24_R_CONTACT, CollimationFormat,CompressionOffset,Weight,IdentificationId>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |CollimationFormat|String|"8"|Collimator Format Range|Sets the assigned collimator format|
    /// |CompressionOffset|Word|120|-|Offset of the paddle plane respect of the Holder position in mm|
    /// |Weight|Word|10|-|Paddle Weight in N |
    /// |IdentificationId|Word|8|-|Identifier Code|
    /// 
    ///     NOTE: The collimation format 0 is reserved for the OPEN collimation
    /// 
    literal System::String^ PARAM_PADDLE_18x24_R_CONTACT = "PADDLE_18x24_R_CONTACT";
    literal System::String^ PARAM_PADDLE_18x24_R_CONTACT_COMMENT = "PADDLE_18x24_R_CONTACT calibration data";
    literal System::String^ PADDLE_18x24_R_CONTACT_COLLIMATION_DEFAULT = "8"; // Format Collimation index
    literal System::String^ PADDLE_18x24_R_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_18x24_R_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_18x24_R_TAG_DEFAULT = "8"; //  Tag Code

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### 10x24_CONTACT Paddle Descriptor Parameters Row
    /// 
    /// This parameter sets the 10x24_CONTACT parameters.
    /// 
    /// < PADDLE_10x24_CONTACT, CollimationFormat,CompressionOffset,Weight,IdentificationId>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |CollimationFormat|String|"9"|Collimator Format Range|Sets the assigned collimator format|
    /// |CompressionOffset|Word|120|-|Offset of the paddle plane respect of the Holder position in mm|
    /// |Weight|Word|10|-|Paddle Weight in N |
    /// |IdentificationId|Word|9|-|Identifier Code|
    /// 
    ///     NOTE: The collimation format 0 is reserved for the OPEN collimation
    /// 
    literal System::String^ PARAM_PADDLE_10x24_CONTACT = "PADDLE_10x24_CONTACT";
    literal System::String^ PARAM_PADDLE_10x24_CONTACT_COMMENT = "PADDLE_10x24_CONTACT calibration data";
    literal System::String^ PADDLE_10x24_CONTACT_COLLIMATION_DEFAULT = "9"; // Format Collimation index
    literal System::String^ PADDLE_10x24_CONTACT_POSITION_OFFSET_DEFAULT = "120"; // mm offset 
    literal System::String^ PADDLE_10x24_CONTACT_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_10x24_CONTACT_TAG_DEFAULT = "9"; //  Tag Code

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### 9x9_MAG Paddle Descriptor Parameters Row
    /// 
    /// This parameter sets the 9x9_MAG parameters.
    /// 
    /// < PADDLE_9x9_MAG, CollimationFormat,CompressionOffset,Weight,IdentificationId>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |CollimationFormat|String|"10"|Collimator Format Range|Sets the assigned collimator format|
    /// |CompressionOffset|Word|0|-|Offset of the paddle plane respect of the Holder position in mm|
    /// |Weight|Word|10|-|Paddle Weight in N |
    /// |IdentificationId|Word|10|-|Identifier Code|
    /// 
    ///     NOTE: The collimation format 0 is reserved for the OPEN collimation
    /// 
    literal System::String^ PARAM_PADDLE_9x9_MAG = "PADDLE_9x9_MAG";
    literal System::String^ PARAM_PADDLE_9x9_MAG_COMMENT = "PADDLE_9x9_MAG calibration data";
    literal System::String^ PADDLE_9x9_MAG_COLLIMATION_DEFAULT = "10"; // Format Collimation index
    literal System::String^ PADDLE_9x9_MAG_POSITION_OFFSET_DEFAULT = "0"; // mm offset 
    literal System::String^ PADDLE_9x9_MAG_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_9x9_MAG_TAG_DEFAULT = "10"; //  Tag Code

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### 9x21_MAG Paddle Descriptor Parameters Row
    /// 
    /// This parameter sets the 9x21_MAG parameters.
    /// 
    /// < PADDLE_9x21_MAG, CollimationFormat,CompressionOffset,Weight,IdentificationId>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |CollimationFormat|String|"11"|Collimator Format Range|Sets the assigned collimator format|
    /// |CompressionOffset|Word|0|-|Offset of the paddle plane respect of the Holder position in mm|
    /// |Weight|Word|10|-|Paddle Weight in N |
    /// |IdentificationId|Word|11|-|Identifier Code|
    /// 
    ///     NOTE: The collimation format 0 is reserved for the OPEN collimation
    /// 
    literal System::String^ PARAM_PADDLE_9x21_MAG = "PADDLE_9x21_MAG";
    literal System::String^ PARAM_PADDLE_9x21_MAG_COMMENT = "PADDLE_9x21_MAG calibration data";
    literal System::String^ PADDLE_9x21_MAG_COLLIMATION_DEFAULT = "11"; // Format Collimation index
    literal System::String^ PADDLE_9x21_MAG_POSITION_OFFSET_DEFAULT = "0"; // mm offset 
    literal System::String^ PADDLE_9x21_MAG_WEIGHT_DEFAULT = "10"; //  Weight in Newton
    literal System::String^ PADDLE_9x21_MAG_TAG_DEFAULT = "11"; //  Tag Code

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### D75_MAG Paddle Descriptor Parameters Row
    /// 
    /// This parameter sets the D75_MAG parameters.
    /// 
    /// < PADDLE_D75_MAG, CollimationFormat,CompressionOffset,Weight,IdentificationId>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |CollimationFormat|String|"12"|Collimator Format Range|Sets the assigned collimator format|
    /// |CompressionOffset|Word|0|-|Offset of the paddle plane respect of the Holder position in mm|
    /// |Weight|Word|10|-|Paddle Weight in N |
    /// |IdentificationId|Word|12|-|Identifier Code|
    /// 
    ///     NOTE: The collimation format 0 is reserved for the OPEN collimation
    /// 
    literal System::String^ PARAM_PADDLE_D75_MAG = "PADDLE_D75_MAG";
    literal System::String^ PARAM_PADDLE_D75_MAG_COMMENT = "PADDLE_D75_MAG calibration data";
    literal System::String^ PADDLE_D75_MAG_COLLIMATION_DEFAULT = "12"; // Format Collimation index
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
