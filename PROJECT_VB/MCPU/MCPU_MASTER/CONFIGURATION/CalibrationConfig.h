#pragma once
#include "ConfigFile.h"

ref class CalibrationConfig
{
public:
    static const System::String^ FILENAME = "SystemCalibration.cnf";
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

    static const System::String^ PARAM_FILTER_CALIBRATION_FILTER1_DEFAULT   = "50"; // 0.1mm step
    static const System::String^ PARAM_FILTER_CALIBRATION_FILTER2_DEFAULT   = "50"; // 0.1mm step
    static const System::String^ PARAM_FILTER_CALIBRATION_FILTER3_DEFAULT   = "50"; // 0.1mm step
    static const System::String^ PARAM_FILTER_CALIBRATION_FILTER4_DEFAULT   = "50"; // 0.1mm step
    static const System::String^ PARAM_FILTER_CALIBRATION_MIRROR_DEFAULT    = "50"; // 0.1mm step
    static const System::String^ PARAM_FILTER_CALIBRATION_LIGHTON_DEFAULT   = "10"; // seconds ON



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