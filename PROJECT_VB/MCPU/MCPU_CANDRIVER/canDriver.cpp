#include "canDriver.h"
#include <iostream>

using namespace std;

/**
 * @brief canDriver class constructor
 */
canDriver::canDriver() {

    deviceOpen = false;
    
    handle = 0;
}

/**
 * The function opens the connection with the device driver \n
 * controlling the USB-CAN Plus device.
 *
 * The Function tries to open the device more than once
 * before to exit in error condition.
 *
 * When the connection has been successfully established, \n
 * the function retrive the current API version and hardware \n
 * details that can be logged in case the logs should be activated.
 *
 * The function than activate the filter so that ALL messages
 * can be received (RECEIVE ALL).
 *
 *
 * @param BR: this is the baudarate of the CAN communication;
 * @param loopback: this activated the loopback mode if true.
 * @return true in case of activation success
 */
bool canDriver::driverOpen(_CanBR BR, bool loopback) {
    VSCAN_STATUS status;
    char string[33];


    status = VSCAN_Ioctl(0, VSCAN_IOCTL_SET_DEBUG, VSCAN_DEBUG_LOW);

    // Open the device
    unsigned char modo = VSCAN_MODE_NORMAL;
    if (loopback) {
        modo = VSCAN_MODE_SELF_RECEPTION;
        cout << "CAN DRIVER: SELF RECEPTION MODE";
    }
    else {
        cout << "CAN DRIVER: NORMAL MODE" ;
    }

    for (int i = 0; i < 5; i++) {
        handle = VSCAN_Open(VSCAN_FIRST_FOUND, modo);
        if (handle > 0) break;
        cout << "CAN OPEN ATTEMPT " << i;
    }

    if (handle <= 0) {
        VSCAN_GetErrorString((VSCAN_STATUS)handle, string, 32);
        cout << "Open Command: " << string;
        return false;
    }

    cout << "Can Driver Opened";

    // Get the APi release code
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_API_VERSION, &version);
    if (status != VSCAN_ERR_OK) {
        VSCAN_GetErrorString(status, string, 32);
        cout << "Get Api Version Command: " << string;
        return false;
    }

    cout << "VSCAN API: " << version.Major << "." << version.Minor << "." << version.SubMinor;

    // Get Hardware release code
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_HWPARAM, &hwparam);
    if (status != VSCAN_ERR_OK) {
        VSCAN_GetErrorString(status, string, 32);
        cout << "Get Hw Param Command: " << string;
        return false;
    }

    unsigned char hwmaj = hwparam.HwVersion >> 4;
    unsigned char hwmin = hwparam.HwVersion & 0x0F;
    unsigned char swmaj = hwparam.SwVersion >> 4;
    unsigned char swmin = hwparam.SwVersion & 0x0F;

    cout << "VSCAN HARDWARE:  SN=" << hwparam.SerialNr << " HwREV=" << hwmaj << "." << hwmin << " SwREV=" << swmaj << "." << swmin << " TYPE=" << hwparam.HwType;

    // Set Baudrate
    void* br = VSCAN_SPEED_1M;
    std::string brstring = " 1Mbs";

    switch (BR) {
    case _CAN_1000K: br = VSCAN_SPEED_1M; brstring = " 1Mbs"; break;
    case _CAN_800K: br = VSCAN_SPEED_800K; brstring = " 800Kbs"; break;
    case _CAN_500K: br = VSCAN_SPEED_500K; brstring = " 500Kbs"; break;
    case _CAN_250K: br = VSCAN_SPEED_250K; brstring = " 250Kbs"; break;
    case _CAN_125K: br = VSCAN_SPEED_125K; brstring = " 125Kbs"; break;
    case _CAN_100K: br = VSCAN_SPEED_100K; brstring = " 100Kbs"; break;
    case _CAN_50K: br = VSCAN_SPEED_50K; brstring = " 50Kbs"; break;
    case _CAN_20K: br = VSCAN_SPEED_20K; brstring = " 20Kbs"; break;
    }

    // Set Baudrate
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_SPEED, br);
    if (status != VSCAN_ERR_OK) {
        VSCAN_GetErrorString(status, string, 32);
        cout << "Set Baudrate Command: " << string;
        return false;
    }

    cout << "SET BAUDRATE TO " << brstring;


    // Set the Filter to accept all frames
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_FILTER_MODE, VSCAN_FILTER_MODE_DUAL);
    if (status != VSCAN_ERR_OK) {
        VSCAN_GetErrorString(status, string, 32);
        cout << "VSCAN_IOCTL_SET_FILTER_MODE Command: " << string;
        return false;
    }
    // receive all frames on the CAN bus (default)

    VSCAN_CODE_MASK codeMask;
    codeMask.Code = VSCAN_IOCTL_ACC_CODE_ALL;
    codeMask.Mask = VSCAN_IOCTL_ACC_MASK_ALL;
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_ACC_CODE_MASK, &codeMask);

    if (status != VSCAN_ERR_OK) {
        VSCAN_GetErrorString(status, string, 32);
        cout << "Set Code And Mask Command: " << string;
        return false;
    }

    // clear all filters
    VSCAN_FILTER filter;
    filter.Size = 0;
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_FILTER, &filter);
    if (status != VSCAN_ERR_OK) {
        VSCAN_GetErrorString(status, string, 32);
        cout << "VSCAN_IOCTL_SET_FILTER Command: " << string;
        return false;
    }

    //VSCAN_Ioctl(NULL, VSCAN_IOCTL_SET_DEBUG_MODE, VSCAN_DEBUG_MODE_CONSOLE);
    //VSCAN_Ioctl(NULL, VSCAN_IOCTL_SET_DEBUG, VSCAN_DEBUG_HIGH);
    cout << "VSCAN DRIVER READY";

    // Start the Can Tx/Rx every 1ms
    /*
    canTimer.stop();
    rxEvent = false;
    canTimer.start(1);

    deviceOpen = true;
    return true;
    */
    return true;
}


/**
 * This function close the communication with
 * the device driver.
 *
 */
void canDriver::driverClose(void) {

    if (handle <= 0) return;

    // Termines the timer callback
    //canTimer.stop();
    //rxEvent = false;

    // Close the device driver
    VSCAN_STATUS status = VSCAN_Close(handle);
    if (status != VSCAN_ERR_OK) {
        char string[33];
        VSCAN_GetErrorString(status, string, 32);
        //qDebug() << string;
        return;
    }

    return;
}