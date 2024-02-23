#pragma once
#include "ConfigFile.h"



ref class DemoPcb302
{
public:
    literal System::String^ FILENAME = "DemoPcb302.cnf";
    literal int     FILE_REVISION = 2;

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_PADDLE_STAT = "PADDLE";
    literal System::String^ PARAM_PADDLE_COMMENT = "Detected Paddle";
    literal System::String^ PARAM_PADDLE_DEFAULT = "PADDLE_24x30_CONTACT";
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_THICKNESS = "THICKNESS";
    literal System::String^ PARAM_THICKNESS_COMMENT = "Measured Thickness";
    literal System::String^ PARAM_THICKNESS_DEFAULT = "0";
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_FORCE = "FORCE";
    literal System::String^ PARAM_FORCE_COMMENT = "Measured Force";
    literal System::String^ PARAM_FORCE_DEFAULT = "0";
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_COMPRESSING = "COMPRESSING";
    literal System::String^ PARAM_COMPRESSING_COMMENT = "Is compressing";
    literal System::String^ PARAM_COMPRESSING_DEFAULT = "0";
    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_DOWNWARD = "DOWNWARD_ACTIVATION";
    literal System::String^ PARAM_DOWNWARD_COMMENT = "Pedal Compression activated";
    literal System::String^ PARAM_DOWNWARD_DEFAULT = "0";

    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_PADDLE_STAT, PARAM_PADDLE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_PADDLE_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_THICKNESS, PARAM_THICKNESS_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_THICKNESS_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_FORCE, PARAM_FORCE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_FORCE_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_COMPRESSING, PARAM_COMPRESSING_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_COMPRESSING_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_DOWNWARD, PARAM_DOWNWARD_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_DOWNWARD_DEFAULT,
            }),

        
        }
    );
};