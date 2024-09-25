#pragma once
#include "ConfigFile.h"


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