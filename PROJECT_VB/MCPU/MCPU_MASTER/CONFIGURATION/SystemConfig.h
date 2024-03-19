#pragma once
#include "ConfigFile.h"

ref class SystemConfig
{
public:
    literal System::String^ FILENAME = "SystemInit.cnf";
    literal int     FILE_REVISION = 1;

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_RUNNING_MODE = "RUNNING_MODE";
    literal System::String^ PARAM_RUNNING_MODE_COMMENT = "[1=Run, 0=Sim]: tilt, Arm,Body,Vertical,Slide, 301, 302, 303,304,315,326";
    literal int     PARAM_RUNNING_MODE_TILT = 0;
    literal int     PARAM_RUNNING_MODE_ARM = 1;
    literal int     PARAM_RUNNING_MODE_BODY = 2;
    literal int     PARAM_RUNNING_MODE_VERTICAL = 3;
    literal int     PARAM_RUNNING_MODE_SLIDE = 4;
    literal int     PARAM_RUNNING_MODE_PCB301 = 5;
    literal int     PARAM_RUNNING_MODE_PCB302 = 6;
    literal int     PARAM_RUNNING_MODE_PCB303 = 7;
    literal int     PARAM_RUNNING_MODE_PCB304 = 8;
    literal int     PARAM_RUNNING_MODE_PCB315 = 9;
    literal int     PARAM_RUNNING_MODE_PCB326 = 10;



    literal System::String^ PARAM_RUNNING_MODE_TILT_DEFAULT = "1";
    literal System::String^ PARAM_RUNNING_MODE_ARM_DEFAULT = "1";
    literal System::String^ PARAM_RUNNING_MODE_BODY_DEFAULT = "1";
    literal System::String^ PARAM_RUNNING_MODE_VERTICAL_DEFAULT = "1";
    literal System::String^ PARAM_RUNNING_MODE_SLIDE_DEFAULT = "1";
    literal System::String^ PARAM_RUNNING_MODE_PCB301_DEFAULT = "1";
    literal System::String^ PARAM_RUNNING_MODE_PCB302_DEFAULT = "1";
    literal System::String^ PARAM_RUNNING_MODE_PCB303_DEFAULT = "1";
    literal System::String^ PARAM_RUNNING_MODE_PCB304_DEFAULT = "1";
    literal System::String^ PARAM_RUNNING_MODE_PCB315_DEFAULT = "1";
    literal System::String^ PARAM_RUNNING_MODE_PCB326_DEFAULT = "0";



    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_INSTALLATION_NAME = "INSTALL_NAME";
    literal System::String^ PARAM_INSTALLATION_NAME_COMMENT = "Name of the installation";
    literal int     PARAM_INSTALLATION_NAME_TOP = 0;
    literal System::String^ PARAM_INSTALLATION_NAME_TOP_DEFAULT = "FACTORY INSTALLATION";

    //___________________________________________________________________________________________________//
    literal System::String^    PARAM_PACKAGE = "PACKAGE";
    literal System::String^    PARAM_PACKAGE_COMMENT = "Package revision set";
    literal int        PARAM_PACKAGE_ID = 0;
    literal int        PARAM_PACKAGE_FW301 = 1;
    literal int        PARAM_PACKAGE_FW302 = 2;
    literal int        PARAM_PACKAGE_FW303 = 3;
    literal int        PARAM_PACKAGE_FW304 = 4;
    literal int        PARAM_PACKAGE_FW315 = 5;
    literal int        PARAM_PACKAGE_FW326 = 6;
    literal int        PARAM_PACKAGE_FWCAN = 7;
    literal int        PARAM_PACKAGE_FWGEN = 8;

    literal System::String^    PARAM_PACKAGE_ID_DEFAULT =    "1.0";
    literal System::String^    PARAM_PACKAGE_FW301_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FW302_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FW303_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FW304_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FW315_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FW326_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FWCAN_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FWGEN_DEFAULT = "0.1";

    //___________________________________________________________________________________________________//
    literal System::String^    PARAM_AWS_CONNECTIONS = "AWS_CONNECTION";
    literal System::String^    PARAM_AWS_CONNECTIONS_COMMENT = "Addresses of the connections with the AWS";
    literal int        PARAM_AWS_CONNECTIONS_IP = 0;
    literal int        PARAM_AWS_CONNECTIONS_PORT_COMMAND = 1;
    literal int        PARAM_AWS_CONNECTIONS_PORT_EVENTS = 2;
    literal System::String^    PARAM_AWS_CONNECTIONS_IP_DEFAULT = "127.0.0.1";
    literal System::String^    PARAM_AWS_CONNECTIONS_PORT_COMMAND_DEFAULT = "10000";
    literal System::String^    PARAM_AWS_CONNECTIONS_PORT_EVENTS_DEFAULT = "10001";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_DEMO_MODE = "DEMO_MODE";
    literal System::String^ PARAM_DEMO_MODE_COMMENT = "This is the Demo Mode activation ";
    literal int             PARAM_DEMO_MODE_STATUS = 0;
    literal System::String^ PARAM_DEMO_MODE_DEFAULT = "1";

    //___________________________________________________________________________________________________//
    literal System::String^ PARAM_LOG_DIRECTORY = "LOG_DIRECTORY";
    literal System::String^ PPARAM_LOG_DIRECTORY_COMMENT = "The directory reserved for logs ";
    literal int PARAM_DIR = 0;
    literal int PARAM_ENA = 1;
    literal System::String^ PARAM_LOG_DIRECTORY_DEFAULT = "\\GANTRY_LOGS";
    literal System::String^ PARAM_LOG_ENA_DEFAULT = "ON";


    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_RUNNING_MODE, PARAM_RUNNING_MODE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_RUNNING_MODE_TILT_DEFAULT,
            PARAM_RUNNING_MODE_ARM_DEFAULT,
            PARAM_RUNNING_MODE_BODY_DEFAULT,
            PARAM_RUNNING_MODE_VERTICAL_DEFAULT,
            PARAM_RUNNING_MODE_SLIDE_DEFAULT,
            PARAM_RUNNING_MODE_PCB301_DEFAULT,
            PARAM_RUNNING_MODE_PCB302_DEFAULT,
            PARAM_RUNNING_MODE_PCB303_DEFAULT,
            PARAM_RUNNING_MODE_PCB304_DEFAULT,
            PARAM_RUNNING_MODE_PCB315_DEFAULT,
            PARAM_RUNNING_MODE_PCB326_DEFAULT
            }),


            CONFIG_FILE_ITEM(PARAM_INSTALLATION_NAME, PARAM_INSTALLATION_NAME_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_INSTALLATION_NAME_TOP_DEFAULT,            
            }),

            CONFIG_FILE_ITEM(PARAM_PACKAGE, PARAM_PACKAGE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_PACKAGE_ID_DEFAULT,
            PARAM_PACKAGE_FW301_DEFAULT, 
            PARAM_PACKAGE_FW302_DEFAULT,
            PARAM_PACKAGE_FW303_DEFAULT,
            PARAM_PACKAGE_FW304_DEFAULT,
            PARAM_PACKAGE_FW315_DEFAULT,
            PARAM_PACKAGE_FW326_DEFAULT,
            PARAM_PACKAGE_FWCAN_DEFAULT,
            PARAM_PACKAGE_FWGEN_DEFAULT,
            }),
         
            CONFIG_FILE_ITEM(PARAM_AWS_CONNECTIONS, PARAM_AWS_CONNECTIONS_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_AWS_CONNECTIONS_IP_DEFAULT,
            PARAM_AWS_CONNECTIONS_PORT_COMMAND_DEFAULT,
            PARAM_AWS_CONNECTIONS_PORT_EVENTS_DEFAULT,           
            }),

            CONFIG_FILE_ITEM(PARAM_DEMO_MODE, PARAM_DEMO_MODE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_DEMO_MODE_DEFAULT,            
            }),

            CONFIG_FILE_ITEM(PARAM_LOG_DIRECTORY, PPARAM_LOG_DIRECTORY_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_LOG_DIRECTORY_DEFAULT,
            PARAM_LOG_ENA_DEFAULT,
            }),
            
        }
    );
};