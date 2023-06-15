#ifndef POWER_SERVICE_H
#define POWER_SERVICE_H

#include "QObject"
#include "masterInterface.h"

class powerService : public masterInterface
{
    Q_OBJECT

public:


    explicit powerService(void);
    ~powerService(){};

   void handleReceivedEvent(QList<QString>* event_content);
   void handleReceivedAck(QList<QString>* ack_content);
   void handleServerConnections(bool status);





private slots:


private:



};

#endif // POWER_SERVICE_H
