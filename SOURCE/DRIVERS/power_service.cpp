#include "application.h"
#include "power_service.h"


powerService::powerService(void):masterInterface(SYSCONFIG->getParam<QString>(SYS_LOCALHOST_PARAM,0),SYSCONFIG->getParam<int>(SYS_POWERSERVICE_PORT_PARAM,0))
{


}

void powerService::handleReceivedEvent(QList<QString>* event_content){

    return;
}

void powerService::handleReceivedAck(QList<QString>* ack_content){

    if(ack_content->at(ACK_CMD_CODE) == GET_REVISION){
        if(ack_content->size() != GET_REVISION_LEN) return;
        setRevision(ack_content->at(ACK_FIRST_PARAM_CODE).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+1).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+2).toUInt());
        qDebug() << "POWER SERVICE REVISION: " << maj_rev << "." << min_rev << "." << sub_rev;
    }
    return;
}

void powerService::handleServerConnections(bool status){
    if(status){
        qDebug() << "POWER SERVICE CONNECTED";       
    }
}
