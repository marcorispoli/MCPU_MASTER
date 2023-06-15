#include "application.h"
#include "power_service.h"
#include "QProcess"



powerService::powerService(void):masterInterface("POWERSERVICE", SYSCONFIG->getParam<QString>(SYS_POWERSERVICE_PROCESS_PARAM,SYS_PROCESS_NAME),SYSCONFIG->getParam<QString>(SYS_POWERSERVICE_PROCESS_PARAM,SYS_PROCESS_PARAM),SYSCONFIG->getParam<QString>(SYS_POWERSERVICE_PROCESS_PARAM,SYS_PROCESS_IP),SYSCONFIG->getParam<int>(SYS_POWERSERVICE_PROCESS_PARAM,SYS_PROCESS_PORT))
{


}

#define PROCESS_NAME QString("POWERSERVICE")

void powerService::handleReceivedEvent(QList<QString>* event_content){

    return;
}

void powerService::handleReceivedAck(QList<QString>* ack_content){


    return;
}

void powerService::handleServerConnections(bool status){
    if(status){
        qDebug() << PROCESS_NAME  << " CONNECTED";
    }else qDebug() << PROCESS_NAME  << " DISCONNECTED";
}

