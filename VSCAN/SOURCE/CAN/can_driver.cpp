#include "application.h"
#include <QApplication>

/**
 * @brief canDriver class constructor
 */
canDriver::canDriver(){
    canTimer=0;
    for(int i=0; i<8; i++)    rxCanData.append((uchar) 0);
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
bool canDriver::driverOpen(_CanBR BR, bool loopback){
    VSCAN_STATUS status;
    char string[33];


    status = VSCAN_Ioctl(NULL, VSCAN_IOCTL_SET_DEBUG, VSCAN_DEBUG_LOW);

    // Open the device
    uchar modo = VSCAN_MODE_NORMAL;
    if(loopback){
        modo = VSCAN_MODE_SELF_RECEPTION;
        qDebug() << "CAN DRIVER: SELF RECEPTION MODE";
    }else {
        qDebug() << "CAN DRIVER: NORMAL MODE";
    }

    for(int i=0 ; i<5; i++){
        handle = VSCAN_Open(VSCAN_FIRST_FOUND, modo);
        if(handle > 0) break;
        qDebug() << "CAN OPEN ATTEMPT " << i;
    }

    if(handle <= 0){
        VSCAN_GetErrorString((VSCAN_STATUS) handle, string, 32);
        qDebug() << "Open Command: " << string;
        return false;
    }

    qDebug() << "Can Driver Opened";

    // Get the APi release code
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_API_VERSION, &version);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "Get Api Version Command: " << string;
        return false;
    }

    qDebug() << "VSCAN API: " << version.Major << "." << version.Minor << "." << version.SubMinor;

    // Get Hardware release code
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_HWPARAM, &hwparam);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "Get Hw Param Command: " << string;
        return false;
    }

    uchar hwmaj = hwparam.HwVersion >> 4;
    uchar hwmin = hwparam.HwVersion & 0x0F;
    uchar swmaj = hwparam.SwVersion >> 4;
    uchar swmin = hwparam.SwVersion & 0x0F;

    qDebug() << "VSCAN HARDWARE:  SN=" << hwparam.SerialNr << " HwREV=" << hwmaj <<"."<<hwmin << " SwREV=" << swmaj<<"."<<swmin << " TYPE=" << hwparam.HwType;

    // Set Baudrate
    void* br = VSCAN_SPEED_1M;
    QString brstring = " 1Mbs";

    switch(BR){
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
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "Set Baudrate Command: " << string;
        return false;
    }

    qDebug() << "SET BAUDRATE TO " << brstring;


    // Set the Filter to accept all frames
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_FILTER_MODE, VSCAN_FILTER_MODE_DUAL);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "VSCAN_IOCTL_SET_FILTER_MODE Command: " << string;
        return false;
    }
    // receive all frames on the CAN bus (default)

    VSCAN_CODE_MASK codeMask;
    codeMask.Code = VSCAN_IOCTL_ACC_CODE_ALL;
    codeMask.Mask = VSCAN_IOCTL_ACC_MASK_ALL;
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_ACC_CODE_MASK, &codeMask);

    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "Set Code And Mask Command: " << string;
        return false;
    }

    // clear all filters
    VSCAN_FILTER filter;
    filter.Size = 0;
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_FILTER, &filter);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "VSCAN_IOCTL_SET_FILTER Command: " << string;
        return false;
    }

    //VSCAN_Ioctl(NULL, VSCAN_IOCTL_SET_DEBUG_MODE, VSCAN_DEBUG_MODE_CONSOLE);
    //VSCAN_Ioctl(NULL, VSCAN_IOCTL_SET_DEBUG, VSCAN_DEBUG_HIGH);
    qDebug() << "VSCAN DRIVER READY";

    if(canTimer) killTimer(canTimer);
    canTimer = startTimer(1);
    return true;

}

/**
 * This function close the communication with
 * the device driver.
 *
 */
void canDriver::driverClose(void){

    if(handle <= 0) return;

    // Termines the timer callback
    if(canTimer) killTimer(canTimer);
    canTimer = 0;

    // Close the device driver
    VSCAN_STATUS status = VSCAN_Close(handle);
    if(status != VSCAN_ERR_OK){
        char string[33];
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << string;
        return ;
    }

    return;
}


/**
 * @brief This function sends a CAN frame on CAN Network.
 *
 * This function is declared as Slot and should be connected
 * to the can frame emitter signal (See the @ref interfaceModule).
 *
 *
 *
 * @param canId: is the CANID address
 * @param data: is the eight data byte frame content.
 */
void canDriver::sendOnCanSlot(ushort canId, QByteArray data){

    VSCAN_MSG msg;
    DWORD written;
    uchar len = data.size();
    if(len > 8) len =8;

    msg.Flags = VSCAN_FLAGS_STANDARD;
    msg.Id = canId;
    msg.Size = len;

    for(uchar i=0; i<msg.Size; i++) {
        if(i < len) msg.Data[i] = data[i];
        else msg.Data[i] = 0;
    }


    if(VSCAN_Write(handle, &msg, 1, &written) != VSCAN_ERR_OK) return;
    VSCAN_Flush(handle);
    return;
}


/**
 * @brief Timer callback scheduled every 1ms
 *
 * This function polls the driver reception queue\n
 * in order to get the received can frames.
 *
 * The received can frames then will be forwarded \n
 * to the can frame consumers in the application \n
 * emitting the canDriver::receivedCanFrame() signal.
 *
 * @param ev: QTimer::QTimerEvent parameter type;
 */
void canDriver::timerEvent(QTimerEvent* ev)
{

    /*
    static ushort test_msg = 0;
    QByteArray test_message;
    test_msg++;


    if(test_msg == 20){
        printErrors();
        test_message.append((uchar) 1);
        test_message.append((uchar) 0);
        test_message.append((uchar) 0);
        test_message.append((uchar) 0);
        test_message.append((uchar) 0);
        test_message.append((uchar) 0);
        test_message.append((uchar) 0);
        test_message.append((uchar) 1);

        test_msg = 0;
        sendTestMessage(0x211, test_message);
        return;
    }
    return;
    */

    if(ev->timerId() ==  canTimer)
    {

        rxmsg = 0;
        VSCAN_Read(handle, rxmsgs, VSCAN_NUM_MESSAGES, &rxmsg);
        if(rxmsg){

            for(uint i=0; i < (uint) rxmsg; i++){
                rxCanId = rxmsgs[i].Id;
                for(int j=0; j < 8; j++) rxCanData[j] = rxmsgs[i].Data[j];                
                emit receivedCanFrame(rxCanId, rxCanData);
            }
        }

    }

}

void canDriver::sendTestMessage(ushort canId, QByteArray data){

    VSCAN_MSG msg;
    DWORD written;
    uchar len = data.size();
    if(len > 8) len =8;

    msg.Flags = VSCAN_FLAGS_STANDARD;
    msg.Id = canId;
    msg.Size = len;

    for(uchar i=0; i<msg.Size; i++) {
        if(i < len) msg.Data[i] = data[i];
        else msg.Data[i] = 0;
    }

    if(VSCAN_Write(handle, &msg, 1, &written) != VSCAN_ERR_OK) return;
    VSCAN_Flush(handle);
    return;
}

void canDriver::printErrors(void){
    static DWORD flag_back = 0;
    DWORD flags;
    QString errstr = "";

    VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_FLAGS, &flags);
    if(flags == flag_back) return;

    flag_back = flags;

    if(flags&0x1) errstr += " RX-FIFO-FULL ";
    if(flags&0x2) errstr += " TX-FIFO-FULL ";
    if(flags&0x4) errstr += " ERR-WARNING ";
    if(flags&0x8) errstr += " DATA-OVERRUN ";
    if(flags&0x10) errstr += " UNUSED ";
    if(flags&0x20) errstr += " ERR-PASSIVE ";
    if(flags&0x40) errstr += " ARBIT-LOST ";
    if(flags&0x80) errstr += " BUS-ERROR ";
    qDebug() << errstr;
    return;

}
