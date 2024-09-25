#pragma once
#include "ConfigFile.h"

ref class TomoConfig
{
public:

    enum class tomo_id {
        TOMO1F_NARROW = 0,
        TOMO1F_INTERMEDIATE,
        TOMO1F_WIDE,
        TOMO2F_NARROW,
        TOMO2F_INTERMEDIATE,
        TOMO2F_WIDE,
        TOMO3F_NARROW,
        TOMO3F_INTERMEDIATE,
        TOMO3F_WIDE,
        TOMO4F_NARROW,
        TOMO4F_INTERMEDIATE,
        TOMO4F_WIDE,
        TOMO5F_NARROW,
        TOMO5F_INTERMEDIATE,
        TOMO5F_WIDE,
        TOMO_ID_NUM
    };

    literal System::String^ FILENAME = "TomoConfig.cnf";
    literal int     FILE_REVISION = 1;

    literal int     PARAM_TOMO_HOME = 0;
    literal int     PARAM_TOMO_END = 1;
    literal int     PARAM_TOMO_FPS = 2;
    literal int     PARAM_TOMO_SAMPLES = 3;
    literal int     PARAM_TOMO_SKIP = 4;
    literal int     PARAM_TOMO_SPEED = 5;
    literal int     PARAM_TOMO_ACC = 6;
    literal int     PARAM_TOMO_DEC = 7;


    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO1F_NARROW = "TOMO1F_NARROW";
    literal System::String^ PARAM_TOMO1F_NARROW_COMMENT = "Defines the Tomo 1F NARROW configuration";
    literal System::String^ PARAM_TOMO1F_NARROW_HOME_DEFAULT = "825";
    literal System::String^ PARAM_TOMO1F_NARROW_END_DEFAULT = "-825";
    literal System::String^ PARAM_TOMO1F_NARROW_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO1F_NARROW_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_NARROW_SPEED_DEFAULT = "150";
    literal System::String^ PARAM_TOMO1F_NARROW_ACC_DEFAULT = "150";
    literal System::String^ PARAM_TOMO1F_NARROW_DEC_DEFAULT = "150";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE = "TOMO1F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_COMMENT = "Defines the Tomo 1F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_HOME_DEFAULT = "1300";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_END_DEFAULT = "-1300";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_SPEED_DEFAULT = "200";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_ACC_DEFAULT = "200";
    literal System::String^ PARAM_TOMO1F_INTERMEDIATE_DEC_DEFAULT = "200";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO1F_WIDE = "TOMO1F_WIDE";
    literal System::String^ PARAM_TOMO1F_WIDE_COMMENT = "Defines the Tomo 1F WIDE configuration";
    literal System::String^ PARAM_TOMO1F_WIDE_HOME_DEFAULT = "1900";
    literal System::String^ PARAM_TOMO1F_WIDE_END_DEFAULT = "-1900";
    literal System::String^ PARAM_TOMO1F_WIDE_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_WIDE_SAMPLES_DEFAULT = "19";
    literal System::String^ PARAM_TOMO1F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO1F_WIDE_SPEED_DEFAULT = "200";
    literal System::String^ PARAM_TOMO1F_WIDE_ACC_DEFAULT = "200";
    literal System::String^ PARAM_TOMO1F_WIDE_DEC_DEFAULT = "200";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO2F_NARROW = "TOMO2F_NARROW";
    literal System::String^ PARAM_TOMO2F_NARROW_COMMENT = "Defines the Tomo 2F NARROW configuration";
    literal System::String^ PARAM_TOMO2F_NARROW_HOME_DEFAULT = "825";
    literal System::String^ PARAM_TOMO2F_NARROW_END_DEFAULT = "-825";
    literal System::String^ PARAM_TOMO2F_NARROW_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO2F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO2F_NARROW_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO2F_NARROW_SPEED_DEFAULT = "300";
    literal System::String^ PARAM_TOMO2F_NARROW_ACC_DEFAULT = "600";
    literal System::String^ PARAM_TOMO2F_NARROW_DEC_DEFAULT = "600";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE = "TOMO2F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_COMMENT = "Defines the Tomo 2F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_HOME_DEFAULT = "1300";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_END_DEFAULT = "-1300";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_SPEED_DEFAULT = "400";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_ACC_DEFAULT = "800";
    literal System::String^ PARAM_TOMO2F_INTERMEDIATE_DEC_DEFAULT = "800";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO2F_WIDE = "TOMO2F_WIDE";
    literal System::String^ PARAM_TOMO2F_WIDE_COMMENT = "Defines the Tomo 2F WIDE configuration";
    literal System::String^ PARAM_TOMO2F_WIDE_HOME_DEFAULT = "1900";
    literal System::String^ PARAM_TOMO2F_WIDE_END_DEFAULT = "-1900";
    literal System::String^ PARAM_TOMO2F_WIDE_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO2F_WIDE_SAMPLES_DEFAULT = "19";
    literal System::String^ PARAM_TOMO2F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO2F_WIDE_SPEED_DEFAULT = "400";
    literal System::String^ PARAM_TOMO2F_WIDE_ACC_DEFAULT = "800";
    literal System::String^ PARAM_TOMO2F_WIDE_DEC_DEFAULT = "800";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO3F_NARROW = "TOMO3F_NARROW";
    literal System::String^ PARAM_TOMO3F_NARROW_COMMENT = "Defines the Tomo 3F NARROW configuration";
    literal System::String^ PARAM_TOMO3F_NARROW_HOME_DEFAULT = "825";
    literal System::String^ PARAM_TOMO3F_NARROW_END_DEFAULT = "-825";
    literal System::String^ PARAM_TOMO3F_NARROW_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO3F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO3F_NARROW_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO3F_NARROW_SPEED_DEFAULT = "300";
    literal System::String^ PARAM_TOMO3F_NARROW_ACC_DEFAULT = "600";
    literal System::String^ PARAM_TOMO3F_NARROW_DEC_DEFAULT = "600";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE = "TOMO3F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_COMMENT = "Defines the Tomo 3F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_HOME_DEFAULT = "1300";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_END_DEFAULT = "-1300";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_SPEED_DEFAULT = "400";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_ACC_DEFAULT = "800";
    literal System::String^ PARAM_TOMO3F_INTERMEDIATE_DEC_DEFAULT = "800";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO3F_WIDE = "TOMO3F_WIDE";
    literal System::String^ PARAM_TOMO3F_WIDE_COMMENT = "Defines the Tomo 3F WIDE configuration";
    literal System::String^ PARAM_TOMO3F_WIDE_HOME_DEFAULT = "2608";
    literal System::String^ PARAM_TOMO3F_WIDE_END_DEFAULT = "-2608";
    literal System::String^ PARAM_TOMO3F_WIDE_FPS_DEFAULT = "2";
    literal System::String^ PARAM_TOMO3F_WIDE_SAMPLES_DEFAULT = "24";
    literal System::String^ PARAM_TOMO3F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO3F_WIDE_SPEED_DEFAULT = "435";
    literal System::String^ PARAM_TOMO3F_WIDE_ACC_DEFAULT = "869";
    literal System::String^ PARAM_TOMO3F_WIDE_DEC_DEFAULT = "869";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO4F_NARROW = "TOMO4F_NARROW";
    literal System::String^ PARAM_TOMO4F_NARROW_COMMENT = "Defines the Tomo 4F NARROW configuration";
    literal System::String^ PARAM_TOMO4F_NARROW_HOME_DEFAULT = "975";
    literal System::String^ PARAM_TOMO4F_NARROW_END_DEFAULT = "-975";
    literal System::String^ PARAM_TOMO4F_NARROW_FPS_DEFAULT = "4";
    literal System::String^ PARAM_TOMO4F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO4F_NARROW_SKIP_DEFAULT = "3";
    literal System::String^ PARAM_TOMO4F_NARROW_SPEED_DEFAULT = "600";
    literal System::String^ PARAM_TOMO4F_NARROW_ACC_DEFAULT = "800";
    literal System::String^ PARAM_TOMO4F_NARROW_DEC_DEFAULT = "800";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE = "TOMO4F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_COMMENT = "Defines the Tomo 4F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_HOME_DEFAULT = "1500";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_END_DEFAULT = "-1500";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_FPS_DEFAULT = "3";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_SKIP_DEFAULT = "3";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_SPEED_DEFAULT = "600";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_ACC_DEFAULT = "600";
    literal System::String^ PARAM_TOMO4F_INTERMEDIATE_DEC_DEFAULT = "600";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO4F_WIDE = "TOMO4F_WIDE";
    literal System::String^ PARAM_TOMO4F_WIDE_COMMENT = "Defines the Tomo 4F WIDE configuration";
    literal System::String^ PARAM_TOMO4F_WIDE_HOME_DEFAULT = "2550";
    literal System::String^ PARAM_TOMO4F_WIDE_END_DEFAULT = "-2550";
    literal System::String^ PARAM_TOMO4F_WIDE_FPS_DEFAULT = "3";
    literal System::String^ PARAM_TOMO4F_WIDE_SAMPLES_DEFAULT = "24";
    literal System::String^ PARAM_TOMO4F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO4F_WIDE_SPEED_DEFAULT = "675";
    literal System::String^ PARAM_TOMO4F_WIDE_ACC_DEFAULT = "900";
    literal System::String^ PARAM_TOMO4F_WIDE_DEC_DEFAULT = "900";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO5F_NARROW = "TOMO5F_NARROW";
    literal System::String^ PARAM_TOMO5F_NARROW_COMMENT = "Defines the Tomo 5F NARROW configuration";
    literal System::String^ PARAM_TOMO5F_NARROW_HOME_DEFAULT = "825";
    literal System::String^ PARAM_TOMO5F_NARROW_END_DEFAULT = "-825";
    literal System::String^ PARAM_TOMO5F_NARROW_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_NARROW_SAMPLES_DEFAULT = "11";
    literal System::String^ PARAM_TOMO5F_NARROW_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_NARROW_SPEED_DEFAULT = "150";
    literal System::String^ PARAM_TOMO5F_NARROW_ACC_DEFAULT = "150";
    literal System::String^ PARAM_TOMO5F_NARROW_DEC_DEFAULT = "150";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE = "TOMO5F_INTERMEDIATE";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_COMMENT = "Defines the Tomo 5F INTERMEDIATE configuration";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_HOME_DEFAULT = "1300";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_END_DEFAULT = "-1300";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_SAMPLES_DEFAULT = "13";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_SPEED_DEFAULT = "200";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_ACC_DEFAULT = "200";
    literal System::String^ PARAM_TOMO5F_INTERMEDIATE_DEC_DEFAULT = "200";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_TOMO5F_WIDE = "TOMO5F_WIDE";
    literal System::String^ PARAM_TOMO5F_WIDE_COMMENT = "Defines the Tomo 5F WIDE configuration";
    literal System::String^ PARAM_TOMO5F_WIDE_HOME_DEFAULT = "2609";
    literal System::String^ PARAM_TOMO5F_WIDE_END_DEFAULT = "-2609";
    literal System::String^ PARAM_TOMO5F_WIDE_FPS_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_WIDE_SAMPLES_DEFAULT = "24";
    literal System::String^ PARAM_TOMO5F_WIDE_SKIP_DEFAULT = "1";
    literal System::String^ PARAM_TOMO5F_WIDE_SPEED_DEFAULT = "217";
    literal System::String^ PARAM_TOMO5F_WIDE_ACC_DEFAULT = "217";
    literal System::String^ PARAM_TOMO5F_WIDE_DEC_DEFAULT = "217";


    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_TOMO1F_NARROW, PARAM_TOMO1F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO1F_NARROW_HOME_DEFAULT,
            PARAM_TOMO1F_NARROW_END_DEFAULT,
            PARAM_TOMO1F_NARROW_FPS_DEFAULT,
            PARAM_TOMO1F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO1F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO1F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO1F_NARROW_ACC_DEFAULT,
            PARAM_TOMO1F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO1F_INTERMEDIATE, PARAM_TOMO1F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO1F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO1F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO1F_WIDE, PARAM_TOMO1F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO1F_WIDE_HOME_DEFAULT,
            PARAM_TOMO1F_WIDE_END_DEFAULT,
            PARAM_TOMO1F_WIDE_FPS_DEFAULT,
            PARAM_TOMO1F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO1F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO1F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO1F_WIDE_ACC_DEFAULT,
            PARAM_TOMO1F_WIDE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO2F_NARROW, PARAM_TOMO2F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO2F_NARROW_HOME_DEFAULT,
            PARAM_TOMO2F_NARROW_END_DEFAULT,
            PARAM_TOMO2F_NARROW_FPS_DEFAULT,
            PARAM_TOMO2F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO2F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO2F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO2F_NARROW_ACC_DEFAULT,
            PARAM_TOMO2F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO2F_INTERMEDIATE, PARAM_TOMO2F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO2F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO2F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO2F_WIDE, PARAM_TOMO2F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO2F_WIDE_HOME_DEFAULT,
            PARAM_TOMO2F_WIDE_END_DEFAULT,
            PARAM_TOMO2F_WIDE_FPS_DEFAULT,
            PARAM_TOMO2F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO2F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO2F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO2F_WIDE_ACC_DEFAULT,
            PARAM_TOMO2F_WIDE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO3F_NARROW, PARAM_TOMO3F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO3F_NARROW_HOME_DEFAULT,
            PARAM_TOMO3F_NARROW_END_DEFAULT,
            PARAM_TOMO3F_NARROW_FPS_DEFAULT,
            PARAM_TOMO3F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO3F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO3F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO3F_NARROW_ACC_DEFAULT,
            PARAM_TOMO3F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO3F_INTERMEDIATE, PARAM_TOMO3F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO3F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO3F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO3F_WIDE, PARAM_TOMO3F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO3F_WIDE_HOME_DEFAULT,
            PARAM_TOMO3F_WIDE_END_DEFAULT,
            PARAM_TOMO3F_WIDE_FPS_DEFAULT,
            PARAM_TOMO3F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO3F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO3F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO3F_WIDE_ACC_DEFAULT,
            PARAM_TOMO3F_WIDE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO4F_NARROW, PARAM_TOMO4F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO4F_NARROW_HOME_DEFAULT,
            PARAM_TOMO4F_NARROW_END_DEFAULT,
            PARAM_TOMO4F_NARROW_FPS_DEFAULT,
            PARAM_TOMO4F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO4F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO4F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO4F_NARROW_ACC_DEFAULT,
            PARAM_TOMO4F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO4F_INTERMEDIATE, PARAM_TOMO4F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO4F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO4F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO4F_WIDE, PARAM_TOMO4F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO4F_WIDE_HOME_DEFAULT,
            PARAM_TOMO4F_WIDE_END_DEFAULT,
            PARAM_TOMO4F_WIDE_FPS_DEFAULT,
            PARAM_TOMO4F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO4F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO4F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO4F_WIDE_ACC_DEFAULT,
            PARAM_TOMO4F_WIDE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO5F_NARROW, PARAM_TOMO5F_NARROW_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO5F_NARROW_HOME_DEFAULT,
            PARAM_TOMO5F_NARROW_END_DEFAULT,
            PARAM_TOMO5F_NARROW_FPS_DEFAULT,
            PARAM_TOMO5F_NARROW_SAMPLES_DEFAULT,
            PARAM_TOMO5F_NARROW_SKIP_DEFAULT,
            PARAM_TOMO5F_NARROW_SPEED_DEFAULT,
            PARAM_TOMO5F_NARROW_ACC_DEFAULT,
            PARAM_TOMO5F_NARROW_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO5F_INTERMEDIATE, PARAM_TOMO5F_INTERMEDIATE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO5F_INTERMEDIATE_HOME_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_END_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_FPS_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_SAMPLES_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_SKIP_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_SPEED_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_ACC_DEFAULT,
            PARAM_TOMO5F_INTERMEDIATE_DEC_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_TOMO5F_WIDE, PARAM_TOMO5F_WIDE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_TOMO5F_WIDE_HOME_DEFAULT,
            PARAM_TOMO5F_WIDE_END_DEFAULT,
            PARAM_TOMO5F_WIDE_FPS_DEFAULT,
            PARAM_TOMO5F_WIDE_SAMPLES_DEFAULT,
            PARAM_TOMO5F_WIDE_SKIP_DEFAULT,
            PARAM_TOMO5F_WIDE_SPEED_DEFAULT,
            PARAM_TOMO5F_WIDE_ACC_DEFAULT,
            PARAM_TOMO5F_WIDE_DEC_DEFAULT
            }),


        }
    );
};
