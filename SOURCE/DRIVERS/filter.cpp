#include "application.h"
#include "filter.h"



filterProcess::filterProcess(void):masterInterface(SYSCONFIG->getParam<QString>(SYS_FILTER_PROCESS_PARAM,SYS_PROCESS_NAME),SYSCONFIG->getParam<QString>(SYS_FILTER_PROCESS_PARAM,SYS_PROCESS_PARAM),SYSCONFIG->getParam<QString>(SYS_FILTER_PROCESS_PARAM,SYS_PROCESS_IP),SYSCONFIG->getParam<int>(SYS_FILTER_PROCESS_PARAM,SYS_PROCESS_PORT))
{


}

void filterProcess::handleReceivedEvent(QList<QString>* event_content){

    return;
}

void filterProcess::handleReceivedAck(QList<QString>* ack_content){

    if(ack_content->at(ACK_CMD_CODE) == GET_REVISION){

        if(ack_content->size() != GET_REVISION_LEN) return;
        setRevision(ack_content->at(ACK_FIRST_PARAM_CODE).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+1).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+2).toUInt());
        qDebug() << "FILTER  PROCESS REVISION: " << maj_rev << "." << min_rev << "." << sub_rev;
    }
    return;
}

void filterProcess::handleServerConnections(bool status){
    if(status){
        qDebug() << "FILTER PROCESS CONNECTED";
    }else qDebug() << "FILTER PROCESS DISCONNECTED";
}

