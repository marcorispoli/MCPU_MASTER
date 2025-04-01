#ifndef APPLICATION_H
#define APPLICATION_H

/*!
 * \defgroup  applicationModule Application Module.
 */



/*!
 * \mainpage
 *
 * # GENERAL OVERVIEW
 *
 * The CanDriver application allows other Client applications to access the \n
 * CAN network in order to send and receive data to remote devices \n
 * using the CAN bus as the communication BUS.
 *
 * Actually the Application performs like a bridge from the ethernet bus and the \n
 * CAN bus.
 *
 * Each Client application that should connect a CAN device to a given canID address,\n
 * shall connect this Application via Local Host at the 127.0.0.1@10001.
 *
 * The Client application shall implement the communicatin protocol \n
 * in order to send and receive data to/from the remote Can Device throughout this Application.
 *
 *
 * The Application can be run with the following options:
 *
 * - -win: allows to run the application with a graphical window to \n
 *      provide manual interaction with the Can Driver. The Debug strings will be
 *      redirect into the Window panel.
 * - -log: the Application redirects the debug messages to a file:
 *      C:/OEM/Gantry/candriver.log
 * - -canLoopback: the can driver operates in loopback mode.

 * # DEPENDENCIES AND CONFIGURATION FILES
 *
 *  The application requires the vsCan Driver installed into the Operating System.
 *  See the [VSCAN Manual](https://www.vscom.de/download/multiio/others/info/VSCAN_Manual.pdf) for details.
 *
 *  The applications makes use of the vs_can_api.dll that shall be copied into the
 *  execution directory.
 *
 * # APPLICATION MODULES
 *
 * The Application implements the following modules:
 * - @ref applicationModule : defines constant along the application.
 * - @ref candriverModule : implements the communication with the can driver.
 * - @ref interfaceModule : implements the communication with Clients over Local Host.
 * - @ref windowModule : this is an optional Windows interface used for Service/Debug;
 *
 * # SOFTWARE LICENCING
 *
 * The Application has been developed with the Qt Libraries
 * with the LGPL licences.
 *
 * The Application uses the following Libraries:
 * - Network (LGPL)
 * - Core (LGPL);
 * - vs_can_api (LGPL)
 *
 * The Application uses only those libraries without change them and they are
 * dinamically linked into the application.
 *
 */


#include <QApplication>
#include <QObject>
#include <QTimer>



#include "can_driver.h"
#include "server.h"
#include "window.h"


#define WINDOW          pWindow
#define DEBUG           pWindow
#define INTERFACE       pServer
#define CAN             pCanDriver

// Global definitions
#ifdef MAIN_CPP
    Server*   pServer;
    canDriver*   pCanDriver;
    debugWindow* pWindow;

#else
    extern  Server*   INTERFACE;
    extern  canDriver*   CAN;
    extern  debugWindow* WINDOW ;

#endif



    /**
    * @brief Application constants definition
    *
    *  This Namespace defines all the MAIN constants that are used along the \n
    *  Application.
    *
    * \ingroup applicationModule
    */
    namespace Application
    {
       static const char*  SERVER_IP = "127.0.01"; //!< Server Interface Ip Address
       static const unsigned short  SERVER_PORT = 10001; //!< Server Interface Port
       static const canDriver::_CanBR CAN_BAUDRATE = canDriver::_CAN_1000K;

    }


#endif // APPLICATION_H
