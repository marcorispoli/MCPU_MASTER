#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "configfile.h"

class canDriverConfiguration:  public configFile
{
    public:


    #define REVISION     1  // This is the revision code
    #define CONFIG_FILENAME     "/OEM/Gantry/candriver.ini" // This is the configuration file name and path

    // This section defines labels helping the param identification along the application
    #define VIRTUAL_COM         "VIRTUAL_COM"
    #define INTERFACE_ADDRESS   "INTERFACE_ADDRESS"
    #define CAN_SETUP           "CAN_SETUP"



    // your class constructor
    canDriverConfiguration():configFile( (const configFile::fileDescriptorT)
    {   CONFIG_FILENAME, REVISION,
        {{
            { INTERFACE_ADDRESS,        {{"127.0.0.1", "10001"}},  "ADDRESS OF THE TCP/IP INTERFACE"},
            { CAN_SETUP,                {{"1000", "STANDARD"}},     "Baudrate, STANDARD/LOOPBACK mode"},
        }}
    })
    {
        this->loadFile();
    };


}; // End class definition

#endif // CONFIGURATION_H
