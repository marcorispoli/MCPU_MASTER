#pragma once
#include "ConfigFile.h"

/// \addtogroup ConfigurationFilesDescription 
/// <div style="page-break-after: always;"></div>
/// 
/// \section SystemConfig SystemInit.cnf configuration file
/// 
/// ## Overview
/// 
/// This configuration file initializes the Application and its communication interaces.
/// 
/// + **Name of the configuration file:** SystemInit.cnf
/// + **Current revision code:** 1;
/// 
/// ## Parameter Description
/// 
/// This configuration file contains the following parameter rows: 
/// + Application Running Mode: sets the application run mode as Normal, Demo, Simulated modes;
/// + Simulated Mode Definition: sets the modulee behavior when in simulated mode;
/// + Installation name: set the name of the target installaion;
/// + Package revision: sets the required revision of the Gantry devices;
/// + AWS connection parameters: sets the IP address and port number for the AWS connection;
/// + Application Log parameters: defines the parameters for the Application log file generation;
/// + Application Debug parameters: defines the parameters for the Application Debug tool;
/// 
/// 


ref class SystemConfig
{
public:
    literal System::String^ FILENAME = "SystemInit.cnf";
    literal int     FILE_REVISION = 1;

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### Application Running Mode Parameters Row
    /// 
    /// This parameter sets the Application Running Mode.
    /// 
    /// < RUNNING_MODE, Mode >
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |Mode|String|"NORMAL"|"NORMAL"/"DEMO"/"SYM"|Application Running Mode|
    /// 
    /// + "NORMAL": the application runs normally with all the real peripherals connected and with all the Operating rules;
    /// + "DEMO": the application runs in demo mode with a subset of the peripheral connected and with a reduced operating rules;
    /// + "SYM": the application runs in simulation mode. 
    /// 
    /// When the Application runs in Simulation mode, it tries to connect with the McpuSym.exe application.
    /// The simulated peripheral (see the Simulated Mode Definition parameter) are then simulated by McpuSym.exe \n
    /// and all the communication to those peripherals are redirect to the Simulator.
    /// 
    ///     NOTE: When in Simulation mode, the Operating rules applies. 
    ///   
    /// 
    literal System::String^ PARAM_RUNNING_MODE = "RUNNING_MODE";
    literal System::String^ PARAM_RUNNING_MODE_COMMENT = "This is the Gantry Running Mode Definition: NORMAL, DEMO, SYM";
    literal int             PARAM_RUNNING_MODE_STATUS = 0;
    literal System::String^ PARAM_RUNNING_MODE_STATUS_DEFAULT = "NORMAL";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### Simulation Mode Parameters Row
    /// 
    /// This parameter sets the Simulator behavior.
    /// 
    /// < SYM_MODE, EthIp,EthPort,SymCan,SymGenerator,SymTilt,SymArm,SymBody,SymVertical,SymSlide,SymPcb301,SymPcb302,SymPcb303,SymPcb304,SymPcb325,SymPcb326 >
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |EthIp|Ip4 Address|"127.0.0.1"|Ip Address Range|IP Address of the Simulator process|
    /// |EthPort|16 Bit Integer|20000|Port Range|IP Port of the Simulator process|
    /// |SymCan|Byte|0|1/0|Can Module: 1= normal mode, 0 = simulated mode|
    /// |SymGenerator|Byte|0|1/0|Generator Module: 1= normal mode, 0 = simulated mode|
    /// |SymTilt|Byte|0|1/0|Tilt Module: 1= normal mode, 0 = simulated mode|  
    /// |SymArm|Byte|0|1/0|Arm Module: 1= normal mode, 0 = simulated mode|  
    /// |SymBody|Byte|0|1/0|Body Module: 1= normal mode, 0 = simulated mode|
    /// |SymVertical|Byte|0|1/0|Vertical Module: 1= normal mode, 0 = simulated mode| 
    /// |SymSlide|Byte|0|1/0|Slide Module: 1= normal mode, 0 = simulated mode| 
    /// |SymPcb301|Byte|0|1/0|PCB301 Module: 1= normal mode, 0 = simulated mode| 
    /// |SymPcb302|Byte|0|1/0|PCB302 Module: 1= normal mode, 0 = simulated mode|
    /// |SymPcb303|Byte|0|1/0|PCB303 Module: 1= normal mode, 0 = simulated mode|
    /// |SymPcb304|Byte|0|1/0|PCB304 Module: 1= normal mode, 0 = simulated mode|
    /// |SymPcb325|Byte|0|1/0|PCB325 Module: 1= normal mode, 0 = simulated mode|
    /// |SymPcb326|Byte|0|1/0|PCB326 Module: 1= normal mode, 0 = simulated mode|
    /// 
    literal System::String^ PARAM_SYM_MODE = "SYM_MODE";
    literal System::String^ PARAM_SYM_MODE_COMMENT = "SYM Mode Setting: [1=Run, 0=Sim]: can-driver, generator, tilt, Arm,Body,Vertical,Slide, 301, 302, 303,304,325,326";
    literal int     SYM_MODE_IP = 0;
    literal int     SYM_MODE_PORT = 1;
    literal int     SYM_MODE_CAN = 2;
    literal int     SYM_MODE_GENERATOR = 3;
    literal int     SYM_MODE_TILT = 4;
    literal int     SYM_MODE_ARM = 5;
    literal int     SYM_MODE_BODY = 6;
    literal int     SYM_MODE_VERTICAL = 7;
    literal int     SYM_MODE_SLIDE = 8;
    literal int     SYM_MODE_PCB301 = 9;
    literal int     SYM_MODE_PCB302 = 10;
    literal int     SYM_MODE_PCB303 = 11;
    literal int     SYM_MODE_PCB304 = 12;
    literal int     SYM_MODE_PCB325 = 13;
    literal int     SYM_MODE_PCB326 = 14;
    literal System::String^ PARAM_SYM_MODE_IP_DEFAULT = "127.0.0.1";
    literal System::String^ PARAM_SYM_MODE_PORT_DEFAULT = "20000";
    literal System::String^ PARAM_SYM_MODE_CAN_DEFAULT = "0";
    literal System::String^ PARAM_SYM_MODE_GENERATOR_DEFAULT = "0";
    literal System::String^ PARAM_SYM_MODE_TILT_DEFAULT = "0";
    literal System::String^ PARAM_SYM_MODE_ARM_DEFAULT = "0";
    literal System::String^ PARAM_SYM_MODE_BODY_DEFAULT = "0";
    literal System::String^ PARAM_SYM_MODE_VERTICAL_DEFAULT = "0";
    literal System::String^ PARAM_SYM_MODE_SLIDE_DEFAULT = "0";
    literal System::String^ PARAM_SYM_MODE_PCB301_DEFAULT = "0";
    literal System::String^ PARAM_SYM_MODE_PCB302_DEFAULT = "0";
    literal System::String^ PARAM_SYM_MODE_PCB303_DEFAULT = "0";
    literal System::String^ PARAM_SYM_MODE_PCB304_DEFAULT = "0";
    literal System::String^ PARAM_SYM_MODE_PCB325_DEFAULT = "0";
    literal System::String^ PARAM_SYM_MODE_PCB326_DEFAULT = "0";



    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### Installation Name Parameters Row
    /// 
    /// This parameter sets the Installation Name string.
    /// 
    /// < INSTALL_NAME, Name >
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |Name|String|"FACTORY INSTALLATION"|-|Name of the Installation|
    /// 
    /// The Installation name is displayed in the Top of the Gantry displays.  
    /// 
    literal System::String^ PARAM_INSTALLATION_NAME = "INSTALL_NAME";
    literal System::String^ PARAM_INSTALLATION_NAME_COMMENT = "Name of the installation";
    literal int     PARAM_INSTALLATION_NAME_TOP = 0;
    literal System::String^ PARAM_INSTALLATION_NAME_TOP_DEFAULT = "FACTORY INSTALLATION";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### Package Revision Check List Parameters Row
    /// 
    /// This parameter sets the required revisions of all the modules connected with the Gantry.
    /// 
    /// < PACKAGE, Id, RevPcb301, RevPcb302, RevPcb303, RevPcb304, RevPcb315, RevPcb326, RevCan, RevGen >
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |Id|String|"1.02|-|Package Identifier Code|
    /// |RevPcb301|String|"0.1"|-|PCB301 Firmware revision: Maj.Min|
    /// |RevPcb302|String|"0.1"|-|PCB302 Firmware revision: Maj.Min|
    /// |RevPcb303|String|"0.1"|-|PCB303 Firmware revision: Maj.Min|
    /// |RevPcb304|String|"0.1"|-|PCB304 Firmware revision: Maj.Min|
    /// |RevPcb325|String|"0.1"|-|PCB325 Firmware revision: Maj.Min|
    /// |RevPcb326|String|"0.1"|-|PCB326 Firmware revision: Maj.Min|
    /// |RevCan|String|"0.1"|-|Can Device Firmware revision: Maj.Min|
    /// |RevGen|String|"0.1"|-|Generator Device Firmware revision: Maj.Min|
    ///   
    /// 
    literal System::String^    PARAM_PACKAGE = "PACKAGE";
    literal System::String^    PARAM_PACKAGE_COMMENT = "Package revision set";
    literal int        PARAM_PACKAGE_ID = 0;
    literal int        PARAM_PACKAGE_FW301 = 1;
    literal int        PARAM_PACKAGE_FW302 = 2;
    literal int        PARAM_PACKAGE_FW303 = 3;
    literal int        PARAM_PACKAGE_FW304 = 4;
    literal int        PARAM_PACKAGE_FW325 = 5;
    literal int        PARAM_PACKAGE_FW326 = 6;
    literal int        PARAM_PACKAGE_FWCAN = 7;
    literal int        PARAM_PACKAGE_FWGEN = 8;

    literal System::String^    PARAM_PACKAGE_ID_DEFAULT =    "1.0";
    literal System::String^    PARAM_PACKAGE_FW301_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FW302_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FW303_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FW304_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FW325_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FW326_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FWCAN_DEFAULT = "0.1";
    literal System::String^    PARAM_PACKAGE_FWGEN_DEFAULT = "0.1";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### AWS Interface Parameters Row
    /// 
    /// This parameter sets the AWS Interface connection parameters.
    /// 
    /// < AWS_CONNECTION, EthIp, EthCommandPort, EthEventPort >
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |EthIp|Ip4 Address|"127.0.0.1"|Ip Address Range|IP Address of the AWS server socket|
    /// |EthCommandPort|16 Bit Integer|10000|Port Range|IP Port of the Command Server Socket|
    /// |EthEventPort|16 Bit Integer|10001|Port Range|IP Port of the Event Server Socket|   
    /// 
    literal System::String^    PARAM_AWS_CONNECTIONS = "AWS_CONNECTION";
    literal System::String^    PARAM_AWS_CONNECTIONS_COMMENT = "Addresses of the connections with the AWS";
    literal int        PARAM_AWS_CONNECTIONS_IP = 0;
    literal int        PARAM_AWS_CONNECTIONS_PORT_COMMAND = 1;
    literal int        PARAM_AWS_CONNECTIONS_PORT_EVENTS = 2;
    literal System::String^    PARAM_AWS_CONNECTIONS_IP_DEFAULT = "127.0.0.1";
    literal System::String^    PARAM_AWS_CONNECTIONS_PORT_COMMAND_DEFAULT = "10000";
    literal System::String^    PARAM_AWS_CONNECTIONS_PORT_EVENTS_DEFAULT = "10001";

    
    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### Application Logger Parameters Row
    /// 
    /// This parameter sets the Application Logger parameters.
    /// 
    /// < LOG_DIRECTORY, Directory, EnableLog>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |Directory|String|"\\GANTRY_LOGS"|-|Directory where the Log files are generated (if enabled)|
    /// |EnableLog|String|"ON"|"ON"/"OFF"|Enables (ON) or Disables (OFF) The log file generation|
    /// 
    literal System::String^ PARAM_LOG_DIRECTORY = "LOG_DIRECTORY";
    literal System::String^ PARAM_LOG_DIRECTORY_COMMENT = "The directory reserved for logs ";
    literal int PARAM_DIR = 0;
    literal int PARAM_ENA = 1;
    literal System::String^ PARAM_LOG_DIRECTORY_DEFAULT = "\\GANTRY_LOGS";
    literal System::String^ PARAM_LOG_ENA_DEFAULT = "ON";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### Application Debugger Parameters Row
    /// 
    /// This parameter sets the Debugger parameters.
    /// 
    /// < DEBUG, DbgEna, EthIp, EthPort>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |DbgEna|String|"OFF"|"ON"/"OFF"|Enables (ON) or Disables (OFF) The Debugger tool |
    /// |EthIp|Ip4 Address|"127.0.0.1"|Ip Address Range|IP Address of the Debugger server socket|
    /// |EthPort|16 Bit Integer|20001|Port Range|IP Port of the Debugger server socket|
    /// 
    literal System::String^ PARAM_DEBUG = "DEBUG";
    literal System::String^ PARAM_DEBUG_COMMENT = "This is the Debug fetaures";
    literal int PARAM_DEBUG_ENA = 0;
    literal int PARAM_DEBUG_IP = 1;
    literal int PARAM_DEBUG_PORT = 2;
    literal System::String^ PARAM_DEBUG_ENA_DEFAULT = "OFF";
    literal System::String^ PARAM_DEBUG_ENA_IP_DEFAULT = "127.0.0.1";
    literal System::String^ PARAM_DEBUG_ENA_PORT_DEFAULT = "20001";

    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_RUNNING_MODE, PARAM_RUNNING_MODE_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_RUNNING_MODE_STATUS_DEFAULT,           
            }),

            CONFIG_FILE_ITEM(PARAM_SYM_MODE, PARAM_SYM_MODE_COMMENT, CONFIG_FILE_DEFAULT{            
            PARAM_SYM_MODE_IP_DEFAULT,
            PARAM_SYM_MODE_PORT_DEFAULT,
            PARAM_SYM_MODE_CAN_DEFAULT,
            PARAM_SYM_MODE_GENERATOR_DEFAULT,
            PARAM_SYM_MODE_TILT_DEFAULT,
            PARAM_SYM_MODE_ARM_DEFAULT,
            PARAM_SYM_MODE_BODY_DEFAULT,
            PARAM_SYM_MODE_VERTICAL_DEFAULT,
            PARAM_SYM_MODE_SLIDE_DEFAULT,
            PARAM_SYM_MODE_PCB301_DEFAULT,
            PARAM_SYM_MODE_PCB302_DEFAULT,
            PARAM_SYM_MODE_PCB303_DEFAULT,
            PARAM_SYM_MODE_PCB304_DEFAULT,
            PARAM_SYM_MODE_PCB325_DEFAULT,
            PARAM_SYM_MODE_PCB326_DEFAULT
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
            PARAM_PACKAGE_FW325_DEFAULT,
            PARAM_PACKAGE_FW326_DEFAULT,
            PARAM_PACKAGE_FWCAN_DEFAULT,
            PARAM_PACKAGE_FWGEN_DEFAULT,
            }),
         
            CONFIG_FILE_ITEM(PARAM_AWS_CONNECTIONS, PARAM_AWS_CONNECTIONS_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_AWS_CONNECTIONS_IP_DEFAULT,
            PARAM_AWS_CONNECTIONS_PORT_COMMAND_DEFAULT,
            PARAM_AWS_CONNECTIONS_PORT_EVENTS_DEFAULT,           
            }),

            CONFIG_FILE_ITEM(PARAM_LOG_DIRECTORY, PARAM_LOG_DIRECTORY_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_LOG_DIRECTORY_DEFAULT,
            PARAM_LOG_ENA_DEFAULT,
            }),

            CONFIG_FILE_ITEM(PARAM_DEBUG, PARAM_DEBUG_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_DEBUG_ENA_DEFAULT,
            PARAM_DEBUG_ENA_IP_DEFAULT,
            PARAM_DEBUG_ENA_PORT_DEFAULT
            }),
            
        }
    );
};
