#ifndef GENERATOR_H
#define GENERATOR_H

#include "QObject"
#include "masterInterface.h"

class generatorProcess : public masterInterface
{
    Q_OBJECT

public:


    explicit generatorProcess(void);
    ~generatorProcess(){};

   void handleReceivedEvent(QList<QString>* event_content);
   void handleReceivedAck(QList<QString>* ack_content);
   void handleServerConnections(bool status);



private slots:


private:


};

#endif // GENERATOR_H
