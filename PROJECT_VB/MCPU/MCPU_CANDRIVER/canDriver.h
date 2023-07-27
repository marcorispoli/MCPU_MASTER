#pragma once

typedef void VOID;
typedef char CHAR;
typedef unsigned long DWORD;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;

#include <windows.h>
#include "vs_can_api.h"


class canDriver
{
public:

    canDriver();
    ~canDriver() { driverClose(); };

    static const unsigned char VSCAN_NUM_MESSAGES = 10; //!< Max number of quesued can frames

    /// This enumeration defines the Can Bauderate
    typedef enum {
        _CAN_1000K = 0,
        _CAN_800K,
        _CAN_500K,
        _CAN_250K,
        _CAN_125K,
        _CAN_100K,
        _CAN_50K,
        _CAN_20K
    }_CanBR;


    void driverClose(void); //!< Close the communication wioth the System Driver
    bool driverOpen(_CanBR BR, bool loopback); //!< Open the communication with the System Driver

    
private:
    bool deviceOpen;
    VSCAN_API_VERSION   version;    //!< System Driver Api Version
    VSCAN_HWPARAM       hwparam;    //!< System Driver Hardware Version
    VSCAN_HANDLE        handle;     //!< Handle of the driver
    VSCAN_MSG           rxmsgs[VSCAN_NUM_MESSAGES]; //!< Array of the received Can Frames
    DWORD               rxmsg; //!< Number of received frames from CAN
};

