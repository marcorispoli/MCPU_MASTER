#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include "configfile.h"

class sysConfig : public configFile
{
    public:


    #define REVISION     1  // This is the revision code
    #define CONFIG_FILENAME     "C:\\OEM\\Gantry\\Config\\SystemConfig.cnf" // This is the configuration file name and path

    // This section defines labels helping the param identification along the application
    #define LOCALHOST_PARAM         "LOCALHOST"
    #define AWSPORT_PARAM           "AWS_PORT"
    #define GENERATOR_PORT_PARAM    "GENERATOR_PORT"
    #define POWERSERVICE_PORT_PARAM   "POWER_SERVICE_PORT"
    #define FILTER_PORT_PARAM       "FILTER_PORT"
    #define COLLIMATOR_PORT_PARAM   "COLLIMATOR_PORT"
    #define COMPRESSOR_PORT_PARAM   "COMPRESSOR_PORT"
    #define POTTER_PORT_PARAM       "POTTER_PORT"
    #define MOTORS_PORT_PARAM       "MOTORS_PORT"
    #define BIOPSY_PORT_PARAM       "BIOPSY_PORT"

    // your class constructor
    sysConfig():configFile( (const configFile::fileDescriptorT)
        {
            CONFIG_FILENAME, REVISION,
            {{
                { LOCALHOST_PARAM,          {{ "127.0.0.1" }},   "Address of the localhost used for application"},
                { AWSPORT_PARAM,            {{ "10000" }},       "Port for the AWS connection"},
                { GENERATOR_PORT_PARAM,     {{ "10001" }},       "Port for generator driver"},
                { POWERSERVICE_PORT_PARAM,  {{ "10002" }},       "Port for Power service driver"},
                { FILTER_PORT_PARAM,        {{ "10003" }},       "Port for Filter driver"},
                { COLLIMATOR_PORT_PARAM,    {{ "10004" }},       "Port for Collimator driver"},
                { COMPRESSOR_PORT_PARAM,    {{ "10005" }},       "Port for Compressor driver"},
                { POTTER_PORT_PARAM,        {{ "10006" }},       "Port for Potter driver"},
                { MOTORS_PORT_PARAM,        {{ "10009" }},       "Port for Motors driver"},
                { BIOPSY_PORT_PARAM,        {{ "10010" }},       "Port for Biopsy driver"},

            }}
        })
    {
        // Your constructor code ...
        this->loadFile();
    };


}; // End class definition

#endif // CONFIG_H
