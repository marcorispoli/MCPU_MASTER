#include "application.h"
#include "collimator.h"



collimatorProcess::collimatorProcess(void):masterInterface(SYSCONFIG->getParam<QString>(SYS_COLLIMATOR_PROCESS_PARAM,SYS_PROCESS_NAME),SYSCONFIG->getParam<QString>(SYS_COLLIMATOR_PROCESS_PARAM,SYS_PROCESS_PARAM),SYSCONFIG->getParam<QString>(SYS_COLLIMATOR_PROCESS_PARAM,SYS_PROCESS_IP),SYSCONFIG->getParam<int>(SYS_COLLIMATOR_PROCESS_PARAM,SYS_PROCESS_PORT))
{


}

void collimatorProcess::handleReceivedEvent(QList<QString>* event_content){

    return;
}

void collimatorProcess::handleReceivedAck(QList<QString>* ack_content){

    if(ack_content->at(ACK_CMD_CODE) == GET_REVISION){

        if(ack_content->size() != GET_REVISION_LEN) return;
        setRevision(ack_content->at(ACK_FIRST_PARAM_CODE).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+1).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+2).toUInt());
        qDebug() << "COLLIMATOR  PROCESS REVISION: " << maj_rev << "." << min_rev << "." << sub_rev;
    }
    return;
}

void collimatorProcess::handleServerConnections(bool status){
    if(status){
        qDebug() << "COLLIMATOR PROCESS CONNECTED";
    }else qDebug() << "COLLIMATOR PROCESS DISCONNECTED";
}

