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


   const char* GET_REVISION = "GetRevision";
   const uchar GET_REVISION_LEN = ACK_FIRST_PARAM_CODE + 3;

   inline void SEND_GET_REVISION(void){txCommand(QString(GET_REVISION));};

private slots:


private:


};

#endif // GENERATOR_H
