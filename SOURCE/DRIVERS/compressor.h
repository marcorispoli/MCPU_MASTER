#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "QObject"
#include "masterInterface.h"

class compressorProcess : public masterInterface
{
    Q_OBJECT

public:


    explicit compressorProcess(void);
    ~compressorProcess(){};

   void handleReceivedEvent(QList<QString>* event_content);
   void handleReceivedAck(QList<QString>* ack_content);
   void handleServerConnections(bool status);


private slots:


private:


};

#endif // POWER_SERVICE_H
