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

    #define CTITLE          239, 190, 189,255   // pink
    #define CTEXT           255, 255, 255,255   // white
    #define CREVTEXT        0, 0, 0,255         // black
    #define CBORDER         239, 190, 189,255   // pink
    #define CERROR          255, 0, 0,255       // red
    #define COK             0, 255, 0,255       // green
    #define TRANSPARENT     0, 0, 0,0           // Transparent


    #define FONT_N(bold,dim) QString("font: %1 normal %2pt \"Arial\";").arg(bold).arg(dim)
    #define FONT_I(bold,dim) QString("font: %1 italic %2pt \"Arial\";").arg(bold).arg(dim)

    #define _COLOR(r,g,b,a) QString("color:rgba(%1, %2, %3, %4);").arg(r).arg(g).arg(b).arg(a)
    #define _BORDER_COLOR(r,g,b,a) QString("border-color:rgba(%1, %2, %3, %4);").arg(r).arg(g).arg(b).arg(a)
    #define _BOTTOM_BORDER(x,r,g,b,a) QString("border-bottom-width: %1px; border-style:solid; border-color:rgba(%2, %3, %4, %5);").arg(x).arg(r).arg(g).arg(b).arg(a)

    #define _BACKGROUND_COLOR(r,g,b,a) QString("background-color:rgba(%1, %2, %3, %4);").arg(r).arg(g).arg(b).arg(a)
    #define _BORDER(x,r,g,b,a) QString("border-width: %1px; border-style: solid;border-color:rgba(%2, %3, %4, %5);").arg(x).arg(r).arg(g).arg(b).arg(a)

    #define COLOR(x) _COLOR(x)
    #define BORDER_COLOR(x) _BORDER_COLOR(x)
    #define BACKGROUND_COLOR(x) _BACKGROUND_COLOR(x)
    #define NO_BACKGROUND_IMAGE QString("background-image: url();border-image: url();")
    #define NO_BORDER QString("border-width: 0px;")
    #define BORDER(x,col) _BORDER(x,col)
    #define BOTTOM_BORDER(x,col) _BOTTOM_BORDER(x,col)


}


#include <QApplication>
#include <QObject>
#include <QTimer>
#include "time.h"
#include "config.h"
#include "sysconfig.h"
#include "startup.h"
#include "power_service.h"
#include "can_driver.h"
#include "compressor.h"
#include "collimator.h"
#include "filter.h"

#define     SYSCONFIG          pSysConfig
#define     PKGCONFIG          pPkgConfig
#define     STARTUP            pStartup
#define     POWERSERVICE       pPowerService
#define     COMPRESSOR         pCompressorProcess
#define     COLLIMATOR         pCollimatorProcess
#define     FILTER             pFilterProcess
#define     CANDRIVER          pCanDriver


// Global definitions
#ifdef MAIN_CPP
     sysConfig*                SYSCONFIG;
     pkgConfig*                PKGCONFIG;
     startup*                  STARTUP;
     powerService*             POWERSERVICE;
     canDriver*                CANDRIVER;
     compressorProcess*        COMPRESSOR;
     collimatorProcess*        COLLIMATOR;
     filterProcess*            FILTER;

#else
    extern sysConfig*                SYSCONFIG;
    extern pkgConfig*                PKGCONFIG;
    extern startup*                  STARTUP;
    extern powerService*             POWERSERVICE;
    extern compressorProcess*        COMPRESSOR;
    extern canDriver*                CANDRIVER;
    extern collimatorProcess*        COLLIMATOR;
    extern filterProcess*            FILTER;

#endif

#ifdef TIME_MEASURE
    #define T1START  this->t1 = clock()
    #define T1MEASURE(x) { qDebug() << x << ((double)(clock() - this->t1)/CLOCKS_PER_SEC);}
#else
    #define T1START
    #define T1MEASURE(x)
#endif

#endif // APPLICATION_H
