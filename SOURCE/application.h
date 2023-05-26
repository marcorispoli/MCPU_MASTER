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

}


#include <QApplication>
#include <QObject>
#include <QTimer>
#include "time.h"
#include "config.h"
#include "startup.h"

#define     SYSCONFIG          pSysConfig
#define     STARTUP            pStartup

// Global definitions
#ifdef MAIN_CPP
     sysConfig*                SYSCONFIG;
     startup*                  STARTUP;

#else
    extern sysConfig*                SYSCONFIG;
    extern startup*                  STARTUP;
#endif

#ifdef TIME_MEASURE
    #define T1START  this->t1 = clock()
    #define T1MEASURE(x) { qDebug() << x << ((double)(clock() - this->t1)/CLOCKS_PER_SEC);}
#else
    #define T1START
    #define T1MEASURE(x)
#endif

#endif // APPLICATION_H
