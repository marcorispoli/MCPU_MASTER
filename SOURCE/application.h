#ifndef APPLICATION_H
#define APPLICATION_H

/*!
 * \defgroup  applicationModule Application Module.
 */

/*!
 * \defgroup  libraryModules Library Modules Used in the Application.
 */


/*!
 * \mainpage
 *
 * # GENERAL OVERVIEW
 *
 * The Application can run without debug messages or alternatively
 * can be launched with -file, -win or -console option:
 * - -win: allows to run the application with a graphical window to
 * provide manual interaction with the Power Service device;
 * - -file: the Application redirects the debug messages to a file
 * in C:/OEM/Logs/GANTRY/PowerService.log
 * - -console: redirects the debug messages to the standard console
 *
 * # DEPENDENCIES AND CONFIGURATION FILES
 *
 *
 * # APPLICATION MODULES
 *
 * The Application implements the following modules:
 * - @ref applicationModule : defines constant along the application
 * - @ref libraryModules : this is the set of the libraries used in the current application;
 * - @ref interfaceModule : this is the communication socket to receive command from Gantry;
 * - @ref protocolModule : this is the module implementing the Can Device protocol;
 * - @ref windowModule : this is an optional Windows interface used for Service/Debug;
 *
 * # SOFTWARE LICENCING
 *
 * The Application has been developed with the Qt Libraries
 * with the LGPL 3.0 licences.
 *
 * The Application uses the following Libraries:
 * - Network (LGPL-3.0)
 * - Core (LGPL-3.0);
 *
 * The Application uses only those libraries without change them and they are
 * dinamically linked into the application.
 */


/**
* @brief Application constants definition
*
*  This Namespace defines all the MAIN constants that are used along the \n
*  Application in order to interface the Generator and the Gantry.
*
* \ingroup applicationModule
*/
namespace Application
{

    static const unsigned short MAJ_REV = 0;
    static const unsigned short MIN_REV = 0;
    static const unsigned short SUB_REV = 1;

    #define PINK_COLOR  QString("rgb(239, 190, 189);")
    #define RED_COLOR  QString("rgb(255, 0, 0);")
    #define GREEN_COLOR  QString("rgb(0, 255, 0);")
    #define BLACK_COLOR  QString("rgb(0, 0, 0);")

    #define NO_BACKGROUND QString("background-image: url(); background-color: rgba(0, 0, 0, 0);")
    #define PINK_BACKGROUND QString("background-image: url(); background-color: ") + PINK_COLOR

    #define FRAME_TEXT "font: 700 normal 20pt \"Arial\"; color:" + PINK_COLOR
    #define HIGHLIGHT_FRAME_TEXT "font: 700 normal 20pt \"Arial\"; color:" + BLACK_COLOR
    #define HIGHLIGHT_FRAME_BORDER  QString("border-width: 1px; border-style: solid; border-color:") + PINK_COLOR + PINK_BACKGROUND + HIGHLIGHT_FRAME_TEXT
    #define NORMAL_FRAME_BORDER  QString("border-width: 1px; border-style: solid; border-color:") + PINK_COLOR + NO_BACKGROUND + FRAME_TEXT
    #define ERROR_FRAME_BORDER  QString("border-width: 1px; border-style: solid; border-color:") + RED_COLOR + NO_BACKGROUND + FRAME_TEXT
    #define CORRECT_FRAME_BORDER  QString("border-width: 1px; border-style: solid; border-color:") + GREEN_COLOR + NO_BACKGROUND + FRAME_TEXT


}


#include <QApplication>
#include <QObject>
#include <QTimer>
#include "time.h"
#include "config.h"
#include "sysconfig.h"
#include "startup.h"
#include "power_service.h"

#define     SYSCONFIG          pSysConfig
#define     PKGCONFIG          pPkgConfig
#define     STARTUP            pStartup
#define     POWERSERVICE       pPowerService


// Global definitions
#ifdef MAIN_CPP
     sysConfig*                SYSCONFIG;
     pkgConfig*                PKGCONFIG;
     startup*                  STARTUP;
     powerService*             POWERSERVICE;

#else
    extern sysConfig*                SYSCONFIG;
    extern pkgConfig*                PKGCONFIG;
    extern startup*                  STARTUP;
    extern powerService*             POWERSERVICE;

#endif

#ifdef TIME_MEASURE
    #define T1START  this->t1 = clock()
    #define T1MEASURE(x) { qDebug() << x << ((double)(clock() - this->t1)/CLOCKS_PER_SEC);}
#else
    #define T1START
    #define T1MEASURE(x)
#endif

#endif // APPLICATION_H
