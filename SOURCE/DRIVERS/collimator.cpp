#include "application.h"
#include "collimator.h"



collimatorProcess::collimatorProcess(void):masterInterface("COLLIMATOR",SYSCONFIG->getParam<QString>(SYS_COLLIMATOR_PROCESS_PARAM,SYS_PROCESS_NAME),SYSCONFIG->getParam<QString>(SYS_COLLIMATOR_PROCESS_PARAM,SYS_PROCESS_PARAM),SYSCONFIG->getParam<QString>(SYS_COLLIMATOR_PROCESS_PARAM,SYS_PROCESS_IP),SYSCONFIG->getParam<int>(SYS_COLLIMATOR_PROCESS_PARAM,SYS_PROCESS_PORT))
{


}


void collimatorProcess::handleReceivedEvent(QList<QString>* event_content){

    return;
}

void collimatorProcess::handleReceivedAck(QList<QString>* ack_content){

    return;
}

void collimatorProcess::handleServerConnections(bool status){
    if(status){
        qDebug() << debugProcessName  << " CONNECTED";
    }else qDebug() << debugProcessName  << " DISCONNECTED";
}

