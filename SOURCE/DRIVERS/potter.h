#ifndef POTTER_H
#define POTTER_H

#include "QObject"
#include "masterInterface.h"

class potterProcess : public masterInterface
{
    Q_OBJECT

public:


    explicit potterProcess(void);
    ~potterProcess(){};

   void handleReceivedEvent(QList<QString>* event_content);
   void handleReceivedAck(QList<QString>* ack_content);
   void handleServerConnections(bool status);


private slots:


private:


};

#endif // POTTER_H
