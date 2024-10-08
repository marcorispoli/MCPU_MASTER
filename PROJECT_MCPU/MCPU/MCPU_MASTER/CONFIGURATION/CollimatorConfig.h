#pragma once
#include "ConfigFile.h"

ref class CollimatorConfig
{
public:

    literal System::String^ FILENAME = "CollimatorCalibration.cnf";
    literal int     FILE_REVISION = 1;
    literal int     PARAM_FORMAT_FRONT = 0;
    literal int     PARAM_FORMAT_BACK = 1;
    literal int     PARAM_FORMAT_LEFT = 2;
    literal int     PARAM_FORMAT_RIGHT = 3;
    literal int     PARAM_FORMAT_TRAP = 4;

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_0 = "COLLI_STANDARD_FORMAT_0";
    literal System::String^ PARAM_COLLI_FORMAT_0_COMMENT = "Collimation format Slot 0";
    
    literal System::String^ PARAM_COLLI_FORMAT_FRONT_0_DEFAULT = "0"; 
    literal System::String^ PARAM_COLLI_FORMAT_BACK_0_DEFAULT = "0"; 
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_0_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_0_DEFAULT = "0"; 
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_0_DEFAULT = "0"; 

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_1 = "COLLI_STANDARD_FORMAT_1";
    literal System::String^ PARAM_COLLI_FORMAT_1_COMMENT = "Collimation format Slot 1";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_1_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_1_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_1_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_1_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_1_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_2 = "COLLI_STANDARD_FORMAT_2";
    literal System::String^ PARAM_COLLI_FORMAT_2_COMMENT = "Collimation format Slot 2";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_2_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_2_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_2_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_2_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_2_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_3 = "COLLI_STANDARD_FORMAT_3";
    literal System::String^ PARAM_COLLI_FORMAT_3_COMMENT = "Collimation format Slot 3";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_3_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_3_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_3_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_3_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_3_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_4 = "COLLI_STANDARD_FORMAT_4";
    literal System::String^ PARAM_COLLI_FORMAT_4_COMMENT = "Collimation format Slot 4";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_4_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_4_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_4_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_4_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_4_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_5 = "COLLI_STANDARD_FORMAT_5";
    literal System::String^ PARAM_COLLI_FORMAT_5_COMMENT = "Collimation format Slot 5";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_5_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_5_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_5_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_5_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_5_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_6 = "COLLI_STANDARD_FORMAT_6";
    literal System::String^ PARAM_COLLI_FORMAT_6_COMMENT = "Collimation format Slot 6";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_6_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_6_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_6_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_6_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_6_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_7 = "COLLI_STANDARD_FORMAT_7";
    literal System::String^ PARAM_COLLI_FORMAT_7_COMMENT = "Collimation format Slot 7";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_7_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_7_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_7_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_7_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_7_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_8 = "COLLI_STANDARD_FORMAT_8";
    literal System::String^ PARAM_COLLI_FORMAT_8_COMMENT = "Collimation format Slot 8";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_8_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_8_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_8_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_8_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_8_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_9 = "COLLI_STANDARD_FORMAT_9";
    literal System::String^ PARAM_COLLI_FORMAT_9_COMMENT = "Collimation format Slot 9";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_9_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_9_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_9_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_9_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_9_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_10 = "COLLI_STANDARD_FORMAT_10";
    literal System::String^ PARAM_COLLI_FORMAT_10_COMMENT = "Collimation format Slot 10";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_10_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_10_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_10_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_10_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_10_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_11 = "COLLI_STANDARD_FORMAT_11";
    literal System::String^ PARAM_COLLI_FORMAT_11_COMMENT = "Collimation format Slot 11";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_11_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_11_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_11_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_11_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_11_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_12 = "COLLI_STANDARD_FORMAT_12";
    literal System::String^ PARAM_COLLI_FORMAT_12_COMMENT = "Collimation format Slot 12";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_12_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_12_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_12_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_12_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_12_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_13 = "COLLI_STANDARD_FORMAT_13";
    literal System::String^ PARAM_COLLI_FORMAT_13_COMMENT = "Collimation format Slot 13";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_13_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_13_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_13_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_13_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_13_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_14 = "COLLI_STANDARD_FORMAT_14";
    literal System::String^ PARAM_COLLI_FORMAT_14_COMMENT = "Collimation format Slot 14";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_14_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_14_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_14_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_14_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_14_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_15 = "COLLI_STANDARD_FORMAT_15";
    literal System::String^ PARAM_COLLI_FORMAT_15_COMMENT = "Collimation format Slot 15";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_15_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_15_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_15_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_15_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_15_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_16 = "COLLI_STANDARD_FORMAT_16";
    literal System::String^ PARAM_COLLI_FORMAT_16_COMMENT = "Collimation format Slot 16";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_16_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_16_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_16_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_16_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_16_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_17 = "COLLI_STANDARD_FORMAT_17";
    literal System::String^ PARAM_COLLI_FORMAT_17_COMMENT = "Collimation format Slot 17";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_17_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_17_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_17_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_17_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_17_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_18 = "COLLI_STANDARD_FORMAT_18";
    literal System::String^ PARAM_COLLI_FORMAT_18_COMMENT = "Collimation format Slot 18";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_18_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_18_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_18_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_18_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_18_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COLLI_FORMAT_19 = "COLLI_STANDARD_FORMAT_19";
    literal System::String^ PARAM_COLLI_FORMAT_19_COMMENT = "Collimation format Slot 19";

    literal System::String^ PARAM_COLLI_FORMAT_FRONT_19_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_BACK_19_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_LEFT_19_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_RIGHT_19_DEFAULT = "0";
    literal System::String^ PARAM_COLLI_FORMAT_TRAP_19_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_MIRROR = "MIRROR_POSITION";
    literal System::String^ PARAM_MIRROR_COMMENT = "Mirror calibrated params";
    literal int     MIRROR_INFIELD = 0;
    literal System::String^ PARAM_MIRROR_INFIELD_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_FILTER_POSITION = "FILTER_POSITION";
    literal System::String^ PARAM_FILTER_POSITION_COMMENT = "Filter calibrated params";
    literal int     PARAM_FILTER_POSITION_0 = 0;
    literal int     PARAM_FILTER_POSITION_1 = 1;
    literal int     PARAM_FILTER_POSITION_2 = 2;
    literal int     PARAM_FILTER_POSITION_3 = 3;
    literal int     PARAM_FILTER_POSITION_4 = 4;
    literal System::String^ PARAM_FILTER_POSITION_0_DEFAULT = "0";
    literal System::String^ PARAM_FILTER_POSITION_1_DEFAULT = "0";
    literal System::String^ PARAM_FILTER_POSITION_2_DEFAULT = "0";
    literal System::String^ PARAM_FILTER_POSITION_3_DEFAULT = "0";
    literal System::String^ PARAM_FILTER_POSITION_4_DEFAULT = "0";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_FILTER_CONFIG = "FILTER_CONFIG";
    literal System::String^ PARAM_FILTER_CONFIG_COMMENT = "Defines the Filter material assignment to selector slot";
    literal int     PARAM_FILTER_CONFIG_RH_SLOT = 0;
    literal int     PARAM_FILTER_CONFIG_AG_SLOT = 1;
    literal int     PARAM_FILTER_CONFIG_AL_SLOT = 2;
    literal int     PARAM_FILTER_CONFIG_CU_SLOT = 3;
    literal int     PARAM_FILTER_CONFIG_MO_SLOT = 4;
    literal int     PARAM_FILTER_CONFIG_LD_SLOT = 5;

    literal System::String^ PARAM_FILTER_CONFIG_RH_SLOT_DEFAULT = "0"; 
    literal System::String^ PARAM_FILTER_CONFIG_AG_SLOT_DEFAULT = "1";
    literal System::String^ PARAM_FILTER_CONFIG_AL_SLOT_DEFAULT = "2";
    literal System::String^ PARAM_FILTER_CONFIG_CU_SLOT_DEFAULT = "3";
    literal System::String^ PARAM_FILTER_CONFIG_MO_SLOT_DEFAULT = "3";
    literal System::String^ PARAM_FILTER_CONFIG_LD_SLOT_DEFAULT = "4";

    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_0, PARAM_COLLI_FORMAT_0_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_0_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_0_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_0_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_0_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_0_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_1, PARAM_COLLI_FORMAT_1_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_1_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_1_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_1_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_1_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_1_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_2, PARAM_COLLI_FORMAT_2_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_2_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_2_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_2_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_2_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_2_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_3, PARAM_COLLI_FORMAT_3_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_3_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_3_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_3_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_3_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_3_DEFAULT
            }),
            
            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_4, PARAM_COLLI_FORMAT_4_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_4_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_4_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_4_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_4_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_4_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_5, PARAM_COLLI_FORMAT_5_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_5_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_5_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_5_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_5_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_5_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_6, PARAM_COLLI_FORMAT_6_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_6_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_6_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_6_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_6_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_6_DEFAULT

            }),
            
            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_7, PARAM_COLLI_FORMAT_7_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_7_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_7_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_7_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_7_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_7_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_8, PARAM_COLLI_FORMAT_8_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_8_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_8_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_8_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_8_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_8_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_9, PARAM_COLLI_FORMAT_9_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_9_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_9_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_9_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_9_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_9_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_10, PARAM_COLLI_FORMAT_10_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_10_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_10_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_10_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_10_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_10_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_11, PARAM_COLLI_FORMAT_11_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_11_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_11_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_11_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_11_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_11_DEFAULT
            }),
            
            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_12, PARAM_COLLI_FORMAT_12_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_12_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_12_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_12_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_12_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_12_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_13, PARAM_COLLI_FORMAT_13_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_13_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_13_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_13_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_13_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_13_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_14, PARAM_COLLI_FORMAT_14_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_14_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_14_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_14_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_14_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_14_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_15, PARAM_COLLI_FORMAT_15_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_15_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_15_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_15_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_15_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_15_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_16, PARAM_COLLI_FORMAT_16_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_16_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_16_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_16_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_16_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_16_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_17, PARAM_COLLI_FORMAT_17_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_17_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_17_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_17_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_17_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_17_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_18, PARAM_COLLI_FORMAT_18_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_18_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_18_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_18_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_18_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_18_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_COLLI_FORMAT_19, PARAM_COLLI_FORMAT_19_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COLLI_FORMAT_FRONT_19_DEFAULT,
            PARAM_COLLI_FORMAT_BACK_19_DEFAULT,
            PARAM_COLLI_FORMAT_LEFT_19_DEFAULT,
            PARAM_COLLI_FORMAT_RIGHT_19_DEFAULT,
            PARAM_COLLI_FORMAT_TRAP_19_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_MIRROR, PARAM_MIRROR_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_MIRROR_INFIELD_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_FILTER_POSITION, PARAM_FILTER_POSITION_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_FILTER_POSITION_0_DEFAULT,
            PARAM_FILTER_POSITION_1_DEFAULT,
            PARAM_FILTER_POSITION_2_DEFAULT,
            PARAM_FILTER_POSITION_3_DEFAULT,
            PARAM_FILTER_POSITION_4_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_FILTER_CONFIG, PARAM_FILTER_CONFIG_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_FILTER_CONFIG_RH_SLOT_DEFAULT,
            PARAM_FILTER_CONFIG_AG_SLOT_DEFAULT,
            PARAM_FILTER_CONFIG_AL_SLOT_DEFAULT,
            PARAM_FILTER_CONFIG_CU_SLOT_DEFAULT,
            PARAM_FILTER_CONFIG_MO_SLOT_DEFAULT,
            PARAM_FILTER_CONFIG_LD_SLOT_DEFAULT
            }),
        }
        );
};
