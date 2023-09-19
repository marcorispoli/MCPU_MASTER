#pragma once
#include "ConfigFile.h"

ref class SystemConfig
{
public:
    static const String^ FILENAME = "SystemInit.cnf";
    static const int     FILE_REVISION = 1;

    static const String^    PARAM_PACKAGE = "PACKAGE";
    static const String^    PARAM_PACKAGE_COMMENT = "Package revision set";
    static const int        PARAM_PACKAGE_ID = 0;
    static const int        PARAM_PACKAGE_FW301 = 1;
    static const int        PARAM_PACKAGE_FW302 = 2;
    static const int        PARAM_PACKAGE_FW303 = 3;
    static const int        PARAM_PACKAGE_FW304 = 4;
    static const int        PARAM_PACKAGE_FW315 = 5;
    static const int        PARAM_PACKAGE_FWCAN = 6;
    static const int        PARAM_PACKAGE_FWGEN = 7;
    static const String^    PARAM_PACKAGE_ID_DEFAULT =    "1.0";
    static const String^    PARAM_PACKAGE_FW301_DEFAULT = "0.1";
    static const String^    PARAM_PACKAGE_FW302_DEFAULT = "0.1";
    static const String^    PARAM_PACKAGE_FW303_DEFAULT = "0.1";
    static const String^    PARAM_PACKAGE_FW304_DEFAULT = "0.1";
    static const String^    PARAM_PACKAGE_FW315_DEFAULT = "0.1";
    static const String^    PARAM_PACKAGE_FWCAN_DEFAULT = "0.1";
    static const String^    PARAM_PACKAGE_FWGEN_DEFAULT = "0.1";

    static const String^    PARAM_AWS_CONNECTIONS = "CONNECTIONS";
    static const String^    PARAM_AWS_CONNECTIONS_COMMENT = "Addresses of the connections with the AWS";
    static const int        PARAM_AWS_CONNECTIONS_IP = 0;
    static const int        PARAM_AWS_CONNECTIONS_PORT_COMMAND = 1;
    static const int        PARAM_AWS_CONNECTIONS_PORT_EVENTS = 2;
    static const String^    PARAM_AWS_CONNECTIONS_IP_DEFAULT = "127.0.0.1";
    static const String^    PARAM_AWS_CONNECTIONS_PORT_COMMAND_DEFAULT = "10000";
    static const String^    PARAM_AWS_CONNECTIONS_PORT_EVENTS_DEFAULT = "10001";



    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_PACKAGE, PARAM_PACKAGE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_PACKAGE_ID_DEFAULT,
            PARAM_PACKAGE_FW301_DEFAULT, 
            PARAM_PACKAGE_FW302_DEFAULT,
            PARAM_PACKAGE_FW303_DEFAULT,
            PARAM_PACKAGE_FW304_DEFAULT,
            PARAM_PACKAGE_FW315_DEFAULT,
            PARAM_PACKAGE_FWCAN_DEFAULT,
            PARAM_PACKAGE_FWGEN_DEFAULT,
            }),
         
            CONFIG_FILE_ITEM(PARAM_AWS_CONNECTIONS, PARAM_AWS_CONNECTIONS_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_AWS_CONNECTIONS_IP_DEFAULT,
            PARAM_AWS_CONNECTIONS_PORT_COMMAND_DEFAULT,
            PARAM_AWS_CONNECTIONS_PORT_EVENTS_DEFAULT,           
            }),
        }

    );
};