#pragma once
#include "ConfigFile.h"

/// \addtogroup ConfigurationFilesDescription 
/// <div style="page-break-after: always;"></div>
/// 
/// \section BiopsyConfig BiopsyConfig.cnf configuration file
/// 
/// ## Overview
/// 
/// This configuration files describes the parameters of the biopsy device.
/// 
/// + **Name of the configuration file:** BiopsyConfig.cnf
/// + **Current revision code:** 1;
/// 
/// ## Parameter Description
/// 
/// This configuration file contains the following parameter rows: 
/// + POWER_LIGH Power light descriptor;
///


ref class BiopsyConfig
{
public:

    

    literal System::String^ FILENAME = "BiopsyConfig.cnf";
    literal int     FILE_REVISION = 2;


    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### POWER_LIGH Power light Descriptor Parameters Row
    /// 
    /// This parameter sets the power light characteristics.
    /// 
    /// < POWER_LIGH,Enable,Duty>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |Enable|Byte|1|0:1|Enables the activation of the power light|
    /// |Duty|Byte|15|0:100|Percent of pwm duty cycle on the power light|
    /// 
    /// 
    literal System::String^ PARAM_POWER_LIGHT = "POWER_LIGHT";
    literal System::String^ PARAM_POWER_LIGHT_COMMENT = "Defines the power light performances";
    literal int     PARAM_POWER_LIGHT_ENABLE = 0;
    literal int     PARAM_POWER_LIGHT_DUTY = 1;
    literal System::String^ PARAM_POWER_LIGHT_ENABLE_DEFAULT = "1";
    literal System::String^ PARAM_POWER_LIGHT_DUTY_DEFAULT = "15";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### PARK Coordinates of the Park position Row
    /// 
    /// This parameter sets the Park position o fhte Biopsy Device.
    /// 
    /// < PARK,X,Y,Z>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |X|Word|1290|0:2580|Position X of the parking|
    /// |Y|Word|0|0:700|Position Y of the parking|
    /// |Z|Word|1280|0:1300|Position Z of the parking|
    /// 
    literal System::String^ PARAM_PARK = "PARK";
    literal System::String^ PARAM_PARK_COMMENT = "Defines the X,Y,Z coordinate of the Park position";
    literal int     PARAM_PARK_X = 0;
    literal int     PARAM_PARK_Y = 1;
    literal int     PARAM_PARK_Z = 2;
    literal System::String^ PARAM_PARK_X_DEFAULT = "1290";
    literal System::String^ PARAM_PARK_Y_DEFAULT = "0";
    literal System::String^ PARAM_PARK_Z_DEFAULT = "1280";

    
    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_POWER_LIGHT, PARAM_POWER_LIGHT_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_POWER_LIGHT_ENABLE_DEFAULT,
            PARAM_POWER_LIGHT_DUTY_DEFAULT,
            
            }),

            CONFIG_FILE_ITEM(PARAM_PARK, PARAM_PARK_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_PARK_X_DEFAULT,
            PARAM_PARK_Y_DEFAULT,
            PARAM_PARK_Z_DEFAULT
            }),
        }

    );
};
