#define MAIN_CPP

#include "application.h"
#include "applog.h"

#include <QFile>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    appLog(argc, argv, "C:/OEM/Logs/GANTRY/CanDriver.log", debugWindow::debugMessageHandler);

    // Create the Window Log if necessary
    if(appLog::isWindow){
        WINDOW = new debugWindow();
        WINDOW->show();
    }


    INTERFACE = new Server(Application::SERVER_IP, Application::SERVER_PORT);

    bool loopback = false ;
    CAN = new canDriver();
    if(appLog::options.contains("-loopback")) loopback = true;
    CAN->driverOpen(Application::CAN_BAUDRATE, loopback);

    QObject::connect(CAN,SIGNAL(receivedCanFrame(ushort , QByteArray )), INTERFACE, SLOT(receivedCanFrame(ushort , QByteArray)),Qt::QueuedConnection);
    QObject::connect(INTERFACE,SIGNAL(sendToCan(ushort , QByteArray )), CAN, SLOT(sendOnCanSlot(ushort , QByteArray)),Qt::QueuedConnection);
    INTERFACE->Start();

    return a.exec();
}
