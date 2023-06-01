#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include "configfile.h"


class pkgConfig : public configFile
{
    public:


    #define PKG_REVISION            1  // This is the revision code
    #define PKG_CONFIG_FILENAME     "C:\\OEM\\Gantry\\Config\\package_config.cnf" // This is the configuration file name and path

    // This section defines labels helping the param identification along the application
    #define PKG_ID                  "PACKAGE_ID"
    #define PKG_MCPU_MASTER         "MCPU_MASTER_REV"
    #define PKG_MCPU_CAN_DRIVER     "MCPU_CAN_DRIVER_REV"
    #define PKG_MCPU_POWER_SERVICE  "MCPU_POWER_SERVICE_REV"
    #define PKG_MCPU_COMPRESSOR     "MCPU_COMPRESSOR_REV"
    #define PKG_MCPU_COLLIMATOR     "MCPU_COLLIMATOR_REV"
    #define PKG_MCPU_FILTER         "MCPU_FILTER_REV"
    #define PKG_MCPU_POTTER         "MCPU_POTTER_REV"
    #define PKG_MCPU_GENERATOR      "MCPU_GENERATOR_REV"
    #define PKG_MCPU_MOTORS         "MCPU_MOTORS_REV"

    #define PKG_FW_POWER_SERVICE    "FW_POWER_SERVICE_REV"
    #define PKG_FW_COMPRESSOR       "FW_COMPRESSOR_REV"
    #define PKG_FW_COLLIMATOR       "FW_COLLIMATOR_REV"
    #define PKG_FW_FILTER           "FW_FILTER_REV"
    #define PKG_FW_POTTER           "FW_POTTER_REV"


    // your class constructor
    pkgConfig():configFile( (const configFile::fileDescriptorT)
        {
            PKG_CONFIG_FILENAME, PKG_REVISION,
            {{
                { PKG_ID,                   {{ "01" }},     "Package revision code"},
                { PKG_MCPU_MASTER,          {{ "0","1" }},  "MCPU Master Major and Minor revision codes"},
                { PKG_MCPU_CAN_DRIVER,      {{ "0","1" }},  "MCPU Can Driver Major and Minor revision codes"},
                { PKG_MCPU_POWER_SERVICE,   {{ "0","1" }},  "MCPU Power Service Major and Minor revision codes"},
                { PKG_MCPU_COMPRESSOR,      {{ "0","1" }},  "MCPU Compressor Major and Minor revision codes"},
                { PKG_MCPU_COLLIMATOR,      {{ "0","1" }},  "MCPU Collimator Major and Minor revision codes"},
                { PKG_MCPU_FILTER,          {{ "0","1" }},  "MCPU Filter Major and Minor revision codes"},
                { PKG_MCPU_POTTER,          {{ "0","1" }},  "MCPU Potter Major and Minor revision codes"},
                { PKG_MCPU_GENERATOR,       {{ "0","1" }},  "MCPU Generator Major and Minor revision codes"},
                { PKG_MCPU_MOTORS,          {{ "0","1" }},  "MCPU Motors Major and Minor revision codes"},

                { PKG_FW_POWER_SERVICE,     {{ "0","1" }},  "Firmware Powerservice Major and Minor revision codes"},
                { PKG_FW_COMPRESSOR,        {{ "0","1" }},  "Firmware Compressor Major and Minor revision codes"},
                { PKG_FW_COLLIMATOR,        {{ "0","1" }},  "Firmware Collimator Major and Minor revision codes"},
                { PKG_FW_FILTER,            {{ "0","1" }},  "Firmware Filter Major and Minor revision codes"},
                { PKG_FW_POTTER,            {{ "0","1" }},  "Firmware Potter Major and Minor revision codes"},

            }}
        })
    {
        // Your constructor code ...
        this->loadFile();
    };


}; // End class definition
#endif // CONFIG_H
