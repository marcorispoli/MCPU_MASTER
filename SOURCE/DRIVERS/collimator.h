#ifndef COLLIMATOR_H
#define COLLIMATOR_H

#include "QObject"
#include "masterInterface.h"

class collimatorProcess : public masterInterface
{
    Q_OBJECT

public:


    explicit collimatorProcess(void);
    ~collimatorProcess(){};

   void handleReceivedEvent(QList<QString>* event_content);
   void handleReceivedAck(QList<QString>* ack_content);
   void handleServerConnections(bool status);


private slots:


private:




};

#endif // COLLIMATOR_H
