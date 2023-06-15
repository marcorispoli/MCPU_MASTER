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
    SYSCONFIG = new sysConfig(configFile::_CFG_READWRITE);
    if(!SYSCONFIG->isFormatCorrect()){
        qDebug() <<" WRONG SYSTEM CONFIGURATION FILE FORMAT! EXIT PROGRAM";
        exit(0);
    }
    if(SYSCONFIG->isFormatDefault()){
        qDebug() <<" SYSTEM CONFIGURATION FILE SET TO DEFAULT VALUE";
    }
    sysMessages::loadLanguage(SYSCONFIG->getParam<QString>(SYS_LANGUAGE_PARAM,0), SYSCONFIG->getParam<QString>(SYS_LANGUAGE_PARAM,1));


    // Open the application config file

    PKGCONFIG = new pkgConfig();
    //STARTUP = new startup(600, 1024);
    STARTUP = new startup(1440, 900);

    CANDRIVER = new canDriver();
    POWERSERVICE = new powerService();
    COMPRESSOR = new compressorProcess();
    COLLIMATOR = new collimatorProcess();
    FILTER = new filterProcess();
    POTTER = new potterProcess();
    MOTORS = new motorsProcess();
    GENERATOR = new generatorProcess();



    // Enter the STARTUP status
    STARTUP->openWindow();
    a.exec();


    // Cleanup the Process started
    POWERSERVICE->stopDriver();
    COMPRESSOR->stopDriver();
    COLLIMATOR->stopDriver();
    FILTER->stopDriver();
    POTTER->stopDriver();
    MOTORS->stopDriver();
    GENERATOR->stopDriver();
    CANDRIVER->stopDriver();

    return 1;
}

