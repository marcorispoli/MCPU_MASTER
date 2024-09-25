#pragma once
#include "ConfigFile.h"

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
