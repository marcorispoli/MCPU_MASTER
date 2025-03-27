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
/// + HOME_CENTER_POSITION: defines the coordinates of the home center position;
/// + HOME_RIGHT_POSITION: defines the coordinates of the home right position;
/// + HOME_LEFT_POSITION: defines the coordinates of the home left position;
/// + PARK:defines the coordinates of the park position;
/// 
///


ref class BiopsyConfig
{
public:

    

    literal System::String^ FILENAME = "BiopsyConfig.cnf";
    literal int     FILE_REVISION = 3;


    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### HOME_CENTER_POSITION Home Center coordinates definition
    /// 
    /// This parameter sets Home center position coordinates.
    /// 
    /// < HOME_CENTER_POSITION,X,Y,Z>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |X|Word|1290|0:2580|Set the x-coordinate of the home center position|
    /// |Y|Word|0|0:700|Set the y-coordinate of the home center position|
    /// |Z|Word|0|0:1350|Set the z-coordinate of the home center position|
    /// 
    literal System::String^ PARAM_HOME_CENTER_POSITION = "HOME_CENTER_POSITION";
    literal System::String^ PARAM_HOME_CENTER_POSITION_COMMENT = "Defines the coordinates of the Center home position";
    literal int     PARAM_HOME_CENTER_POSITION_X = 0;
    literal int     PARAM_HOME_CENTER_POSITION_Y = 1;
    literal int     PARAM_HOME_CENTER_POSITION_Z = 2;
    literal System::String^ PARAM_HOME_CENTER_POSITION_X_DEFAULT = "1290";
    literal System::String^ PARAM_HOME_CENTER_POSITION_Y_DEFAULT = "0";
    literal System::String^ PARAM_HOME_CENTER_POSITION_Z_DEFAULT = "0";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### HOME_RIGHT_POSITION Home Right coordinates definition
    /// 
    /// This parameter sets Home right position coordinates.
    /// 
    /// < HOME_RIGHT_POSITION,X,Y,Z>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |X|Word|0|0:2580|Set the x-coordinate of the home right position|
    /// |Y|Word|0|0:700|Set the y-coordinate of the home right position|
    /// |Z|Word|0|0:1350|Set the z-coordinate of the home right position|
    /// 
    literal System::String^ PARAM_HOME_RIGHT_POSITION = "HOME_RIGHT_POSITION";
    literal System::String^ PARAM_HOME_RIGHT_POSITION_COMMENT = "Defines the coordinates of the Right home position";
    literal int     PARAM_HOME_RIGHT_POSITION_X = 0;
    literal int     PARAM_HOME_RIGHT_POSITION_Y = 1;
    literal int     PARAM_HOME_RIGHT_POSITION_Z = 2;
    literal System::String^ PARAM_HOME_RIGHT_POSITION_X_DEFAULT = "0";
    literal System::String^ PARAM_HOME_RIGHT_POSITION_Y_DEFAULT = "0";
    literal System::String^ PARAM_HOME_RIGHT_POSITION_Z_DEFAULT = "0";

    /// \addtogroup  ConfigurationFilesDescription 
    /// 
    /// ### HOME_LEFT_POSITION Home Left coordinates definition
    /// 
    /// This parameter sets Home left position coordinates.
    /// 
    /// < HOME_LEFT_POSITION,X,Y,Z>
    /// 
    /// |Param|Type|Default|Range|Descrption|
    /// |:--|:--|:--|:--|:--|
    /// |X|Word|2580|0:2580|Set the x-coordinate of the home left position|
    /// |Y|Word|0|0:700|Set the y-coordinate of the home left position|
    /// |Z|Word|0|0:1350|Set the z-coordinate of the home left position|
    /// 
    literal System::String^ PARAM_HOME_LEFT_POSITION = "HOME_LEFT_POSITION";
    literal System::String^ PARAM_HOME_LEFT_POSITION_COMMENT = "Defines the coordinates of the Left home position";
    literal int     PARAM_HOME_LEFT_POSITION_X = 0;
    literal int     PARAM_HOME_LEFT_POSITION_Y = 1;
    literal int     PARAM_HOME_LEFT_POSITION_Z = 2;
    literal System::String^ PARAM_HOME_LEFT_POSITION_X_DEFAULT = "2580";
    literal System::String^ PARAM_HOME_LEFT_POSITION_Y_DEFAULT = "0";
    literal System::String^ PARAM_HOME_LEFT_POSITION_Z_DEFAULT = "0";

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
    /// |X|Word|0|0:2580|Position X of the parking|
    /// |Y|Word|0|0:700|Position Y of the parking|
    /// |Z|Word|1280|0:1300|Position Z of the parking|
    /// 
    literal System::String^ PARAM_PARK = "PARK";
    literal System::String^ PARAM_PARK_COMMENT = "Defines the X,Y,Z coordinate of the Park position";
    literal int     PARAM_PARK_X = 0;
    literal int     PARAM_PARK_Y = 1;
    literal int     PARAM_PARK_Z = 2;
    literal System::String^ PARAM_PARK_X_DEFAULT = "0";
    literal System::String^ PARAM_PARK_Y_DEFAULT = "0";
    literal System::String^ PARAM_PARK_Z_DEFAULT = "1280";

    
    static ConfigFile^ Configuration = gcnew ConfigFile(FILENAME, FILE_REVISION,
        CONFIG_FILE_DESCRIPTOR
        {
            CONFIG_FILE_ITEM(PARAM_HOME_CENTER_POSITION, PARAM_HOME_CENTER_POSITION_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_HOME_CENTER_POSITION_X_DEFAULT,
            PARAM_HOME_CENTER_POSITION_Y_DEFAULT,
            PARAM_HOME_CENTER_POSITION_Z_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_HOME_RIGHT_POSITION, PARAM_HOME_RIGHT_POSITION_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_HOME_RIGHT_POSITION_X_DEFAULT,
            PARAM_HOME_RIGHT_POSITION_Y_DEFAULT,
            PARAM_HOME_RIGHT_POSITION_Z_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_HOME_LEFT_POSITION, PARAM_HOME_LEFT_POSITION_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_HOME_LEFT_POSITION_X_DEFAULT,
            PARAM_HOME_LEFT_POSITION_Y_DEFAULT,
            PARAM_HOME_LEFT_POSITION_Z_DEFAULT
            }),

            CONFIG_FILE_ITEM(PARAM_PARK, PARAM_PARK_COMMENT, CONFIG_FILE_DEFAULT{
            PARAM_PARK_X_DEFAULT,
            PARAM_PARK_Y_DEFAULT,
            PARAM_PARK_Z_DEFAULT
            }),
        }

    );
};