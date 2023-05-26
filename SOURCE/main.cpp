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
    //appLog(argc, argv, "C:/OEM/Logs/GANTRY/PowerService.log", 0/*debugWindow::debugMessageHandler*/);

    // Open the application config file
    SYSCONFIG = new sysConfig();
    STARTUP = new startup(0);

    // Initialize the WINDOW to nullptr so that in case of Operating mode
    // the application skips the WINDOW callbacks
    //WINDOW = nullptr;

    // Create the Window Log if necessary
    //if(appLog::isWindow){
    //    WINDOW = new debugWindow();
    //    WINDOW->show();
    //}


    STARTUP->show();
    return a.exec();
}

