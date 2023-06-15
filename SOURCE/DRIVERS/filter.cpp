#include "application.h"
#include "filter.h"



filterProcess::filterProcess(void):masterInterface("FILTER",SYSCONFIG->getParam<QString>(SYS_FILTER_PROCESS_PARAM,SYS_PROCESS_NAME),SYSCONFIG->getParam<QString>(SYS_FILTER_PROCESS_PARAM,SYS_PROCESS_PARAM),SYSCONFIG->getParam<QString>(SYS_FILTER_PROCESS_PARAM,SYS_PROCESS_IP),SYSCONFIG->getParam<int>(SYS_FILTER_PROCESS_PARAM,SYS_PROCESS_PORT))
{


}

void filterProcess::handleReceivedEvent(QList<QString>* event_content){

    return;
}

void filterProcess::handleReceivedAck(QList<QString>* ack_content){

    return;
}

void filterProcess::handleServerConnections(bool status){
    if(status){
        qDebug() << debugProcessName  << " CONNECTED";
    }else qDebug() << debugProcessName  << " DISCONNECTED";
}

