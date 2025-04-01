#ifndef VSCAN_CAN_DRIVER_H
#define VSCAN_CAN_DRIVER_H

/*!
 * \defgroup  candriverModule CAN Driver Module.
 *
 * This Module implements the communication protocol with the Gantry.
 *
 * # DEVICE DRIVER OVERVIEW
 *
 * This Module is based on the vs_can_api.dll library interface \n
 * that allows to handle the  USB-CAN Plus device from VSCAN.
 *
 * The module needs that in the Window system it is installed the \n
 * device driver and all the initialization procedures has been completed.
 * See the [VSCAN Manual](https://www.vscom.de/download/multiio/others/info/VSCAN_Manual.pdf) for details.
 *
 * The Device Gets data coming from the Ethernet Server interface then forwards them to the CAN network and viceversa.
 *
 *  NOTE: the can data format is always STANDARD 11bit address.
 *  The byte in the frame are always 8. The bytes not used are set to 0.
 *
 * The Driver can be opened in normal mode or loopback mode:
 * - When in loopback mode, the data driver receives also the data sent.
 *
 *
 * # INTERFACE FUNCTIONS
 *
 * The Driver implements the following functions:
 * - canDriver::driverOpen(): opens the connection with the system device driver;
 * - canDriver::driverClose(): close the connection with the system device driver;
 * - canDriver::sendOnCanSlot(): slot function that sends the data on the CAN bus
 * - canDriver::receivedCanFrame(): signal function emitted when a valid frame is received from the CAN bus;
 *
 *
 *
 */
#include <QTimer>
#include <QTimerEvent>

typedef void VOID;
typedef char CHAR;
typedef unsigned long DWORD;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
#include "vs_can_api.h"

/**
 * @brief This is the class implementing the Can Driver Interface
 *
 * \ingroup candriverModule
 */
class canDriver: public QObject
{
   Q_OBJECT
   public:

   canDriver();
   ~canDriver(){driverClose();};

    static const unsigned char VSCAN_NUM_MESSAGES = 10; //!< Max number of quesued can frames


   /// This enumeration defines the Can Bauderate
   typedef enum{
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


signals:
    void receivedCanFrame(ushort id, QByteArray data); //!< Signal emitted when a CAN frame is received

public slots:
    void sendOnCanSlot(ushort canId, QByteArray data); //!< Slot function activated when a Frame shall be sent on the CAN bus

private:
    VSCAN_API_VERSION   version;    //!< System Driver Api Version
    VSCAN_HWPARAM       hwparam;    //!< System Driver Hardware Version
    VSCAN_HANDLE        handle;     //!< Handle of the driver
    VSCAN_MSG           rxmsgs[VSCAN_NUM_MESSAGES]; //!< Array of the received Can Frames
    DWORD               rxmsg; //!< Number of received frames from CAN

    ushort rxCanId; //!< canId of the pending can message
    QByteArray rxCanData; //!< Data of the can frame
    void timerEvent(QTimerEvent* ev);   //!< Timer scheduled to read the queue of the received messages
    int canTimer;

    void sendTestMessage(ushort canId, QByteArray data);
    void printErrors(void);
};

#endif // VSCAN_CAN_DRIVER_H
