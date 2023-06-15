#ifndef FILTER_H
#define FILTER_H

#include "QObject"
#include "masterInterface.h"

class filterProcess : public masterInterface
{
    Q_OBJECT

public:


    explicit filterProcess(void);
    ~filterProcess(){};

   void handleReceivedEvent(QList<QString>* event_content);
   void handleReceivedAck(QList<QString>* ack_content);
   void handleServerConnections(bool status);



private slots:


private:


};

#endif // FILTER_H
