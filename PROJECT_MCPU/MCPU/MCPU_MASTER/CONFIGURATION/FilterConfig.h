#pragma once
#include "ConfigFile.h"


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

    literal System::String^ PARAM_FILTER_CALIBRATION_FILTER1_DEFAULT = "50"; // 1um step
    literal System::String^ PARAM_FILTER_CALIBRATION_FILTER2_DEFAULT = "50"; // 1um step
    literal System::String^ PARAM_FILTER_CALIBRATION_FILTER3_DEFAULT = "50"; // 1um step
    literal System::String^ PARAM_FILTER_CALIBRATION_FILTER4_DEFAULT = "50"; // 1um step
    literal System::String^ PARAM_FILTER_CALIBRATION_MIRROR_DEFAULT = "50"; // 1um step
    literal System::String^ PARAM_FILTER_CALIBRATION_LIGHTON_DEFAULT = "20"; // seconds ON


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