#define MAIN_CPP

#include "applog.h"
#include "application.h"



/**
 * @brief Main
 * This is the Application Main function.
 *
 *
 * @param argc
 * @param argv
 *  - -file: the debug messages are redirect to the gantrygen.log file;
 *  - -win: the application run with the debug Window;
 *  - -console: the debug messages are forwarded to the console
 *  - No Options: no debug messages are handled
 * @return
 *
 * \ingroup   applicationModule
 */
int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    appLog(argc, argv, "C:/OEM/Gantry/Log/mcpu_master.log");

    // Open the application config file
    SYSCONFIG = new sysConfig();
    PKGCONFIG = new pkgConfig();
    STARTUP = new startup(600, 1024);

    POWERSERVICE = new powerService();
    COMPRESSOR = new compressorProcess();
    COLLIMATOR = new collimatorProcess();
    FILTER = new filterProcess();
    CANDRIVER = new canDriver();


    // Enter the STARTUP status
    STARTUP->openWindow();
    a.exec();


    // Cleanup the Process started
    POWERSERVICE->stopDriver();
    COMPRESSOR->stopDriver();
    COLLIMATOR->stopDriver();
    FILTER->stopDriver();
    CANDRIVER->stopDriver();

    return 1;
}

