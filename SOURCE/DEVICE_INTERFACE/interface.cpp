#include "application.h"
#include <QStringConverter>

/**
 * @brief Interface::Interface
 *
 * @param
 * - ipaddress: IP where server will be bounded;
 * - port: bounding port
 *
 */
Interface::Interface(void):applicationInterface(Application::IP_INTERFACE_ADDRESS, Application::INTERFACE_PORT)
{

    // QTimer::singleShot(1,this, SLOT(handleInterfaceStatus()));
}

uint Interface::handleReceivedCommand(QList<QString>* frame, QList<QString>* answer){

    // if(frame->at(2) == "GetStatus")  return GetStatus(frame, answer);
    return false;
}
