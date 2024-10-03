#pragma once
#include "ConfigFile.h"

/// \addtogroup ConfigurationFilesDescription 
/// <div style="page-break-after: always;"></div>
/// 
/// \section DetectorConfig DetectorConfiguration.cnf configuration file
/// 
/// ## Overview
/// 
/// The Application handles different Detector types.
/// 
/// Every Detector has its own behavior in term of timing and 
/// limitations. 
/// 
/// This configuration file sets all the relevant parameters for every Detector handled by the Application.
/// 
/// + **Name of the configuration file:** DetecorConfiguration.cnf
/// + **Current revision code:** 1;
/// 
/// ## Parameter Description
/// 
/// This configuration file contains the following parameter rows: 
/// + GENERIC Detector Descriptor;
/// + LMAM2V2 Detector descriptor;
/// + FDIV2 Detector descriptor;
/// + DRTECH Detector descriptor;
/// + VAREX Detector descriptor;
/// 
/// 


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
        FDIV2,          //!< Digital Analogic FDI V2
        DRTECH,         //!< Digital DRTECH Detector
        VAREX,          //!< Digital VAREX Detector
        DETECTOR_LIST_SIZE
    };
    literal int     MAX_TOMO_FPS = 6;

    literal System::String^ FILENAME = "DetectorConfiguration.cnf";
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


    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### Generic Detector Descriptor Parameters Row
    /// 
    /// This parameter sets the Generic Detector Descriptor parameters.
    /// 
    /// < GENERIC, PreToPulseTmo,PlsTmo,PreTmo,Tomo1Tmo,Tomo2Tmo,Tomo3Tmo,Tomo4Tmo,Tomo5Tmo,Tomo6Tmo,TomoMaxFps >
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |PreToPulseTmo|Integer|5000|-|Time in ms from the Pre pulse to the Pulse|
    /// |PlsTmo|Integer|5000|-|Integration Time in ms of a 2D pulse|
    /// |PreTmo|Integer|1000|-|Integration Time in ms of a 2D pre-pulse|
    /// |Tomo1Tmo|Integer|500|-|Integration Time in ms of a Tomo 1 FPS|
    /// |Tomo2Tmo|Integer|250|-|Integration Time in ms of a Tomo 2 FPS|
    /// |Tomo3Tmo|Integer|200|-|Integration Time in ms of a Tomo 3 FPS|
    /// |Tomo4Tmo|Integer|120|-|Integration Time in ms of a Tomo 4 FPS|
    /// |Tomo5Tmo|Integer|80|-|Integration Time in ms of a Tomo 5 FPS|
    /// |Tomo6Tmo|Integer|33|-|Integration Time in ms of a Tomo 6 FPS|
    /// |TomoMaxFps|Integer|4|1:6|Max FPS of the detector|
    /// 
    /// 
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

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### LMAM2V2 Detector Descriptor Parameters Row
    /// 
    /// This parameter sets the LMAM2V2 Detector Descriptor parameters.
    /// 
    /// < LMAM2V2, PreToPulseTmo,PlsTmo,PreTmo,Tomo1Tmo,Tomo2Tmo,Tomo3Tmo,Tomo4Tmo,Tomo5Tmo,Tomo6Tmo,TomoMaxFps >
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |PreToPulseTmo|Integer|5000|-|Time in ms from the Pre pulse to the Pulse|
    /// |PlsTmo|Integer|5000|-|Integration Time in ms of a 2D pulse|
    /// |PreTmo|Integer|1000|-|Integration Time in ms of a 2D pre-pulse|
    /// |Tomo1Tmo|Integer|500|-|Integration Time in ms of a Tomo 1 FPS|
    /// |Tomo2Tmo|Integer|250|-|Integration Time in ms of a Tomo 2 FPS|
    /// |Tomo3Tmo|Integer|200|-|Integration Time in ms of a Tomo 3 FPS|
    /// |Tomo4Tmo|Integer|120|-|Integration Time in ms of a Tomo 4 FPS|
    /// |Tomo5Tmo|Integer|80|-|Integration Time in ms of a Tomo 5 FPS|
    /// |Tomo6Tmo|Integer|33|-|Integration Time in ms of a Tomo 6 FPS|
    /// |TomoMaxFps|Integer|4|1:6|Max FPS of the detector|
    /// 
    /// 
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

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### FDIV2 Detector Descriptor Parameters Row
    /// 
    /// This parameter sets the FDIV2 Detector Descriptor parameters.
    /// 
    /// < FDIV2, PreToPulseTmo,PlsTmo,PreTmo,Tomo1Tmo,Tomo2Tmo,Tomo3Tmo,Tomo4Tmo,Tomo5Tmo,Tomo6Tmo,TomoMaxFps >
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |PreToPulseTmo|Integer|5000|-|Time in ms from the Pre pulse to the Pulse|
    /// |PlsTmo|Integer|5000|-|Integration Time in ms of a 2D pulse|
    /// |PreTmo|Integer|1000|-|Integration Time in ms of a 2D pre-pulse|
    /// |Tomo1Tmo|Integer|500|-|Integration Time in ms of a Tomo 1 FPS|
    /// |Tomo2Tmo|Integer|250|-|Integration Time in ms of a Tomo 2 FPS|
    /// |Tomo3Tmo|Integer|200|-|Integration Time in ms of a Tomo 3 FPS|
    /// |Tomo4Tmo|Integer|120|-|Integration Time in ms of a Tomo 4 FPS|
    /// |Tomo5Tmo|Integer|80|-|Integration Time in ms of a Tomo 5 FPS|
    /// |Tomo6Tmo|Integer|33|-|Integration Time in ms of a Tomo 6 FPS|
    /// |TomoMaxFps|Integer|4|1:6|Max FPS of the detector|
    /// 
    /// 
    literal System::String^ PARAM_FDIV2_CONFIG = "FDIV2";
    literal System::String^ PARAM_FDIV2_COMMENT = "This is the FDIV2 Detector description";


    literal System::String^ PARAM_FDIV2_PRE_TO_PULSE_AEC_WAITING_TIME_DEFAULT = "5000";
    literal System::String^ PARAM_FDIV2_MAX_2D_INTEGRATION_TIME_DEFAULT = "5000";
    literal System::String^ PARAM_FDIV2_MAX_2D_PRE_INTEGRATION_TIME_DEFAULT = "1000";
    literal System::String^ PARAM_FDIV2_MAX_3D_INTEGRATION_TIME_1FPS_DEFAULT = "500";
    literal System::String^ PARAM_FDIV2_MAX_3D_INTEGRATION_TIME_2FPS_DEFAULT = "250";
    literal System::String^ PARAM_FDIV2_MAX_3D_INTEGRATION_TIME_3FPS_DEFAULT = "200";
    literal System::String^ PARAM_FDIV2_MAX_3D_INTEGRATION_TIME_4FPS_DEFAULT = "120";
    literal System::String^ PARAM_FDIV2_MAX_3D_INTEGRATION_TIME_5FPS_DEFAULT = "80";
    literal System::String^ PARAM_FDIV2_MAX_3D_INTEGRATION_TIME_6FPS_DEFAULT = "33";
    literal System::String^ PARAM_FDIV2_MAX_3D_FPS_DEFAULT = "4";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### VAREX Detector Descriptor Parameters Row
    /// 
    /// This parameter sets the VAREX Detector Descriptor parameters.
    /// 
    /// < VAREX, PreToPulseTmo,PlsTmo,PreTmo,Tomo1Tmo,Tomo2Tmo,Tomo3Tmo,Tomo4Tmo,Tomo5Tmo,Tomo6Tmo,TomoMaxFps >
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |PreToPulseTmo|Integer|5000|-|Time in ms from the Pre pulse to the Pulse|
    /// |PlsTmo|Integer|5000|-|Integration Time in ms of a 2D pulse|
    /// |PreTmo|Integer|1000|-|Integration Time in ms of a 2D pre-pulse|
    /// |Tomo1Tmo|Integer|500|-|Integration Time in ms of a Tomo 1 FPS|
    /// |Tomo2Tmo|Integer|250|-|Integration Time in ms of a Tomo 2 FPS|
    /// |Tomo3Tmo|Integer|200|-|Integration Time in ms of a Tomo 3 FPS|
    /// |Tomo4Tmo|Integer|120|-|Integration Time in ms of a Tomo 4 FPS|
    /// |Tomo5Tmo|Integer|80|-|Integration Time in ms of a Tomo 5 FPS|
    /// |Tomo6Tmo|Integer|33|-|Integration Time in ms of a Tomo 6 FPS|
    /// |TomoMaxFps|Integer|4|1:6|Max FPS of the detector|
    /// 
    /// 
    literal System::String^ PARAM_VAREX_CONFIG = "VAREX";
    literal System::String^ PARAM_VAREX_COMMENT = "This is the VAREX Detector description";


    literal System::String^ PARAM_VAREX_PRE_TO_PULSE_AEC_WAITING_TIME_DEFAULT = "5000";
    literal System::String^ PARAM_VAREX_MAX_2D_INTEGRATION_TIME_DEFAULT = "5000";
    literal System::String^ PARAM_VAREX_MAX_2D_PRE_INTEGRATION_TIME_DEFAULT = "1000";
    literal System::String^ PARAM_VAREX_MAX_3D_INTEGRATION_TIME_1FPS_DEFAULT = "500";
    literal System::String^ PARAM_VAREX_MAX_3D_INTEGRATION_TIME_2FPS_DEFAULT = "250";
    literal System::String^ PARAM_VAREX_MAX_3D_INTEGRATION_TIME_3FPS_DEFAULT = "200";
    literal System::String^ PARAM_VAREX_MAX_3D_INTEGRATION_TIME_4FPS_DEFAULT = "120";
    literal System::String^ PARAM_VAREX_MAX_3D_INTEGRATION_TIME_5FPS_DEFAULT = "80";
    literal System::String^ PARAM_VAREX_MAX_3D_INTEGRATION_TIME_6FPS_DEFAULT = "33";
    literal System::String^ PARAM_VAREX_MAX_3D_FPS_DEFAULT = "4";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### DRTECH Detector Descriptor Parameters Row
    /// 
    /// This parameter sets the DRTECH Detector Descriptor parameters.
    /// 
    /// < DRTECH, PreToPulseTmo,PlsTmo,PreTmo,Tomo1Tmo,Tomo2Tmo,Tomo3Tmo,Tomo4Tmo,Tomo5Tmo,Tomo6Tmo,TomoMaxFps >
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |PreToPulseTmo|Integer|5000|-|Time in ms from the Pre pulse to the Pulse|
    /// |PlsTmo|Integer|5000|-|Integration Time in ms of a 2D pulse|
    /// |PreTmo|Integer|1000|-|Integration Time in ms of a 2D pre-pulse|
    /// |Tomo1Tmo|Integer|500|-|Integration Time in ms of a Tomo 1 FPS|
    /// |Tomo2Tmo|Integer|250|-|Integration Time in ms of a Tomo 2 FPS|
    /// |Tomo3Tmo|Integer|200|-|Integration Time in ms of a Tomo 3 FPS|
    /// |Tomo4Tmo|Integer|120|-|Integration Time in ms of a Tomo 4 FPS|
    /// |Tomo5Tmo|Integer|80|-|Integration Time in ms of a Tomo 5 FPS|
    /// |Tomo6Tmo|Integer|33|-|Integration Time in ms of a Tomo 6 FPS|
    /// |TomoMaxFps|Integer|4|1:6|Max FPS of the detector|
    /// 
    /// 
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

            CONFIG_FILE_ITEM(PARAM_FDIV2_CONFIG, PARAM_FDIV2_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_FDIV2_PRE_TO_PULSE_AEC_WAITING_TIME_DEFAULT,
            PARAM_FDIV2_MAX_2D_INTEGRATION_TIME_DEFAULT,
            PARAM_FDIV2_MAX_2D_PRE_INTEGRATION_TIME_DEFAULT,
            PARAM_FDIV2_MAX_3D_INTEGRATION_TIME_1FPS_DEFAULT,
            PARAM_FDIV2_MAX_3D_INTEGRATION_TIME_2FPS_DEFAULT,
            PARAM_FDIV2_MAX_3D_INTEGRATION_TIME_3FPS_DEFAULT,
            PARAM_FDIV2_MAX_3D_INTEGRATION_TIME_4FPS_DEFAULT,
            PARAM_FDIV2_MAX_3D_INTEGRATION_TIME_5FPS_DEFAULT,
            PARAM_FDIV2_MAX_3D_INTEGRATION_TIME_6FPS_DEFAULT,
            PARAM_FDIV2_MAX_3D_FPS_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_VAREX_CONFIG, PARAM_VAREX_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_VAREX_PRE_TO_PULSE_AEC_WAITING_TIME_DEFAULT,
            PARAM_VAREX_MAX_2D_INTEGRATION_TIME_DEFAULT,
            PARAM_VAREX_MAX_2D_PRE_INTEGRATION_TIME_DEFAULT,
            PARAM_VAREX_MAX_3D_INTEGRATION_TIME_1FPS_DEFAULT,
            PARAM_VAREX_MAX_3D_INTEGRATION_TIME_2FPS_DEFAULT,
            PARAM_VAREX_MAX_3D_INTEGRATION_TIME_3FPS_DEFAULT,
            PARAM_VAREX_MAX_3D_INTEGRATION_TIME_4FPS_DEFAULT,
            PARAM_VAREX_MAX_3D_INTEGRATION_TIME_5FPS_DEFAULT,
            PARAM_VAREX_MAX_3D_INTEGRATION_TIME_6FPS_DEFAULT,
            PARAM_VAREX_MAX_3D_FPS_DEFAULT
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