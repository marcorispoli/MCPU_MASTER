#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include "QObject"
#include "masterInterface.h"

class canDriver : public masterInterface
{
    Q_OBJECT

public:


    explicit canDriver(void);
    ~canDriver(){};

   void handleReceivedEvent(QList<QString>* event_content);
   void handleReceivedAck(QList<QString>* ack_content);
   void handleServerConnections(bool status);


   const char* GET_REVISION = "GetRevision";
   const uchar GET_REVISION_LEN = ACK_FIRST_PARAM_CODE + 3;

   inline void SEND_GET_REVISION(void){txCommand(QString(GET_REVISION));};

private slots:


private:


};

#endif // CAN_DRIVER_H
