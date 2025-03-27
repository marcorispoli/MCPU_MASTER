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
/// \todo Format collimation for specimen

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
    /// |Position Calibration Status|Byte|0|0:1|Position calibration status: 1=Calibrated, 0 = Not Calibrated|
    /// |Force Calibration Status|Byte|0|0:1|Force calibration status: 1=Calibrated, 0 = Not Calibrated|
    /// |Holder Offset|Word|0|-|Holder Calibration Offset |
    /// |Holder K-Correction|Byte|100|50:150|Percent of linear correction|
    /// |Compressor Max Position|Word|190|-|Maximum compressor position in mm|
    /// |Compressor Min Position|Word|0|-|Minimum compressor position in mm|
    /// |Target Force|Byte|150|70:200|Automatic Target compression in N|
    /// |Max Automatic Compression Force|Byte|200|70:200|Maximum Automatic Compression Force in N|
    /// 
    /// #### Holder Calibration Status
    /// This parameter is automatically set by the application when the position calibration process
    /// is successfully executed.
    /// 
    /// \warning
    /// If this parameter should not be set (position not calibrated) an error message is generated.
    /// 
    /// #### Force Calibration Status
    /// This parameter is automatically set by the application when the compression force calibration process
    /// is successfully executed.
    /// 
    /// \warning
    /// If this parameter should not be set (position not calibrated) an error message is generated.
    /// 
    /// #### Holder Offset
    /// This is the distance from the 0 compressor position and the Compression Plane (Carbon Fiber).
    /// The Hoder offset is used, toghether with the paddle offset parameter, 
    /// in order to calculate the actual breast thickness.
    /// 
    /// #### Holder K-Correction
    /// 
    /// This parameter adjust the linearity of the Holder Position if necessary.
    /// 
    /// The linearity of the holder position is set into the compressor device 
    /// and usually should not require extra adjustments.
    /// However, because the holder position is related to the breast thickness measurement,
    /// the breast thickness may vary differently due to the compressor paddle elasticity.
    ///  
    /// The value is espressed in percent value where 100 means no correction:
    /// + HolderPositionCorrected = CompressorPosition * (K/100) + HolderOffset;
    /// 
    /// #### Compressor Max Position
    /// The compressor Max position is the Upper limit of the Compressor position.
    /// 
    /// #### Compressor Min Position
    /// The compressor Min position is the Lower limit of the Compressor position.
    /// 
    /// #### Target Force
    /// This is the Automatic target compression force.
    /// 
    /// When the operator presses the compression paddle (or the compression knobs on the compressor holder) 
    /// the compressor stops at this target compression force.
    /// 
    /// \warning 
    /// This value cannot be set more than 200N that is the maximum target force with the use of the motor.
    /// 
    /// \warning 
    /// This value cannot be set less than 70N as it is the minimum force largely considered as the correct breast compression.
    /// 
    /// \note
    /// In case a dense breast should detected first, the compression may stop earlier than this target. 
    ///  
    /// 
    /// #### Max Automatic Compression Force
    /// This is the maximum target that the operator can set during a Study.
    /// 
    /// \warning 
    /// This value cannot be set more than 200N that is the maximum target force with the use of the motor.
    /// 
    /// \warning 
    /// This value cannot be set less than 70N as it is the minimum force largely considered as the correct breast compression.
    /// 
    /// 
    
    literal System::String^ PARAM_COMPRESSOR = "COMPRESSOR_DEVICE";
    literal System::String^ PARAM_COMPRESSOR_COMMENT = "Position-Calibrated[1:0], Force-Calibrated[1:0], Holder-Offset, Holder-Kp, Max-Position,Min-Position,Target-Force,Max-Force";
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


    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### Paddle Calibration Descriptors
    /// 
    /// Every compressor Paddle used in the system, is defined by 
    /// well defined geometric aspect, that strongly impact specifically on the
    /// patiente Dose calculation and control.
    /// 
    /// In particolar:
    /// - The Paddle offset affects the breast measurement;
    /// - The Paddle whole dimension affects the x-ray collimation path;
    /// - The Paddle weight affects the actual compression force measurement;
    /// 
    /// The Application provides a set of parameters for any possible Paddle
    /// to fully face to the previous argument. 
    /// 
    /// In particolar:
    /// 
    /// < PADDLE_XXXX, Collimation_Format,Paddle_Offset,Paddle_Weight,Identification_Id>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |Collimation Format|String|"1"|Collimator Format Range|Sets the assigned collimator format slot number|
    /// |Paddle Offset|Word|120|-|Offset from the Holder position and the Compression plane (mm)|
    /// |Paddle Weight|Word|10|-|Paddle Weight in N |
    /// |Identification Id|Word|1|-|Identifier Code for the RFID paddle tag|
    /// 
    /// 
    /// #### Collimation Format
    /// 
    /// The Collimator device provides up to 20 collimation formats.
    /// Every format can be assigned to a paddle setting its number in this parameters.
    /// 
    /// \note
    /// The collimation format slot 0 is reserved for the OPEN collimation and should not be assigned to a paddle!
    /// 
    /// #### Paddle Offset
    /// 
    /// The Paddle offset (in mm) is the distance from the Holder Position (the base of the compressor moving mechanical part)
    /// and the compression plane of the paddle. 
    /// 
    /// The Application uses this value to calculate the actual breast thickness.
    /// 
    /// #### Paddle Weight
    /// 
    /// The paddle Weight (in N) is passed to the compressor device 
    /// in order to recalculate the current compression force.
    /// 
    /// #### Identification Id
    /// 
    /// Every paddle is tagged with an RFID tag that is detected by the compressor device.
    /// The RFID code is assigned uniquelly to a paddle model.
    /// This code can be configured into this configuration file. 
    ///     
    /// 
    /// 
    /// 

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
    /// < PADDLE_PROSTHESIS, Collimation_Format,Paddle_Offset,Paddle_Weight,Identification_Id>
    /// 
    /// |Param|Default|
    /// |:--|:--|
    /// |Collimation Format|"1"|
    /// |Compression Offset|120|
    /// |Paddle Weight|10|
    /// |Identification Id|1|
    /// 
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
    /// < PADDLE_BIOP2D, Collimation_Format,Paddle_Offset,Paddle_Weight,Identification_Id>
    /// 
    /// |Param|Default|
    /// |:--|:--|
    /// |Collimation Format|"2"|
    /// |Compression Offset|120|
    /// |Paddle Weight|10|
    /// |Identification Id|2|
    /// 
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
    /// < PADDLE_BIOP3D, Collimation_Format,Paddle_Offset,Paddle_Weight,Identification_Id>
    /// 
    /// |Param|Default|
    /// |:--|:--|
    /// |Collimation Format|"3"|
    /// |Compression Offset|120|
    /// |Paddle Weight|10|
    /// |Identification Id|3|
    /// 
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
    /// < PADDLE_TOMO, Collimation_Format,Paddle_Offset,Paddle_Weight,Identification_Id>
    /// 
    /// |Param|Default|
    /// |:--|:--|
    /// |Collimation Format|"4"|
    /// |Compression Offset|120|
    /// |Paddle Weight|10|
    /// |Identification Id|4|
    /// 
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
    /// < PADDLE_24x30_CONTACT, Collimation_Format,Paddle_Offset,Paddle_Weight,Identification_Id>
    /// 
    /// |Param|Default|
    /// |:--|:--|
    /// |Collimation Format|"5"|
    /// |Compression Offset|120|
    /// |Paddle Weight|10|
    /// |Identification Id|5|
    /// 
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
    /// < PADDLE_18x24_C_CONTACT, Collimation_Format,Paddle_Offset,Paddle_Weight,Identification_Id>
    /// 
    /// |Param|Default|
    /// |:--|:--|
    /// |Collimation Format|"6"|
    /// |Compression Offset|120|
    /// |Paddle Weight|10|
    /// |Identification Id|6|
    /// 
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
    /// < PADDLE_18x24_L_CONTACT, Collimation_Format,Paddle_Offset,Paddle_Weight,Identification_Id>
    /// 
    /// |Param|Default|
    /// |:--|:--|
    /// |Collimation Format|"7"|
    /// |Compression Offset|120|
    /// |Paddle Weight|10|
    /// |Identification Id|7|
    /// 
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
    /// < PADDLE_18x24_R_CONTACT, Collimation_Format,Paddle_Offset,Paddle_Weight,Identification_Id>
    /// 
    /// |Param|Default|
    /// |:--|:--|
    /// |Collimation Format|"8"|
    /// |Compression Offset|120|
    /// |Paddle Weight|10|
    /// |Identification Id|8|
    /// 
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
    /// < PADDLE_10x24_CONTACT, Collimation_Format,Paddle_Offset,Paddle_Weight,Identification_Id>
    /// 
    /// |Param|Default|
    /// |:--|:--|
    /// |Collimation Format|"9"|
    /// |Compression Offset|120|
    /// |Paddle Weight|10|
    /// |Identification Id|9|
    /// 
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
    /// < PADDLE_9x9_MAG, Collimation_Format,Paddle_Offset,Paddle_Weight,Identification_Id>
    /// 
    /// |Param|Default|
    /// |:--|:--|
    /// |Collimation Format|"10"|
    /// |Compression Offset|120|
    /// |Paddle Weight|10|
    /// |Identification Id|10|
    /// 
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
    /// < PADDLE_9x21_MAG, Collimation_Format,Paddle_Offset,Paddle_Weight,Identification_Id>
    /// 
    /// |Param|Default|
    /// |:--|:--|
    /// |Collimation Format|"11"|
    /// |Compression Offset|120|
    /// |Paddle Weight|10|
    /// |Identification Id|11|
    /// 
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
    /// < PADDLE_D75_MAG, Collimation_Format,Paddle_Offset,Paddle_Weight,Identification_Id>
    /// 
    /// |Param|Default|
    /// |:--|:--|
    /// |Collimation Format|"12"|
    /// |Compression Offset|120|
    /// |Paddle Weight|10|
    /// |Identification Id|12|
    /// 
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
