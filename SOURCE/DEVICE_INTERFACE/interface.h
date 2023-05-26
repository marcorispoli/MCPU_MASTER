#ifndef INTERFACE_H
#define INTERFACE_H

#include "QObject"
#include "applicationInterface.h"

/*!
 * \defgroup  interfaceModule Application Command Interface Module.
 *
 * This Module implements the communication protocol with the Gantry.
 *
 * # CONNECTION DETAILS
 *
 * The Server IP address is: 127.0.0.1@10004.
 *
 * # EVENTS FROM GANTRY DESCRIPTION
 *
 * The Events from Gantry are considered *Commands* to be executed;
 *
 * Follows the list of the Implemented Commands:
 *
 *
 *  > The Application will acknowledge the command ONLY to the client that sent the command;
 *
 *
 * # EVENTS TO GANTRY DESCRIPTION
 *
 * The Events to Gantry are considered *Events* to be notified to the Gantry;
 *
 * Follows a list of the implemented Events:
 *
 *
 *  > The Application will forward the EVENTS to all the connected clients.
 *
 */


/**
 * @brief This class resides into the Main Thread and implements the
 * communication protocol with the Gantry.
 *
 * The class starts listening an incoming Client connection and creates\n
 * for any individual connection a given socket handler (SocketItem class). \n
 * Every Socket is assigned to a unique ID so that the Server can redirect the
 * answer frame with the sender client.
 *
 * The Reception and Transmission are implemented into the  QTcpServer library
 * that runs in a separate thread and exchanges data only through the SIgnal/Slot
 * mechanism.
 *
 *
 * \ingroup InterfaceModule
 */
class Interface : public applicationInterface
{
    Q_OBJECT

public:

    explicit Interface(void);
    ~Interface(){};

    uint handleReceivedCommand(QList<QString>* frame, QList<QString>* answer); //!< Reimplement the handler for the received Commmands


private slots:


private:

    // Protocol Interface Events

    // Protocol Interface commands
    // uint GetStatus(QList<QString>* command, QList<QString>* answer); //!< Gantry requests the current device status

};

#endif // SERVER_H
