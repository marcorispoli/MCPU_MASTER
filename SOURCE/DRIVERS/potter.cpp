#include "application.h"
#include "potter.h"



potterProcess::potterProcess(void):masterInterface("POTTER",SYSCONFIG->getParam<QString>(SYS_POTTER_PROCESS_PARAM,SYS_PROCESS_NAME),
                                                   SYSCONFIG->getParam<QString>(SYS_POTTER_PROCESS_PARAM,SYS_PROCESS_PARAM),
                                                   SYSCONFIG->getParam<QString>(SYS_POTTER_PROCESS_PARAM,SYS_PROCESS_IP),
                                                   SYSCONFIG->getParam<int>(SYS_POTTER_PROCESS_PARAM,SYS_PROCESS_PORT))
{


}

void potterProcess::handleReceivedEvent(QList<QString>* event_content){
    return;
}

void potterProcess::handleReceivedAck(QList<QString>* ack_content){

    return;
}

void potterProcess::handleServerConnections(bool status){
    if(status){
        qDebug() << debugProcessName  << " CONNECTED";
    }else qDebug() << debugProcessName  << " DISCONNECTED";
}

