#include "application.h"
#include "can_driver.h"



canDriver::canDriver(void):masterInterface(SYSCONFIG->getParam<QString>(SYS_CAN_PROCESS_PARAM,SYS_PROCESS_NAME),SYSCONFIG->getParam<QString>(SYS_CAN_PROCESS_PARAM,SYS_PROCESS_PARAM),SYSCONFIG->getParam<QString>(SYS_CAN_PROCESS_PARAM,SYS_CAN_IP),SYSCONFIG->getParam<int>(SYS_CAN_PROCESS_PARAM,SYS_CAN_SERVICE))
{


}

void canDriver::handleReceivedEvent(QList<QString>* event_content){

    return;
}

void canDriver::handleReceivedAck(QList<QString>* ack_content){

    if(ack_content->at(ACK_CMD_CODE) == GET_REVISION){

        if(ack_content->size() != GET_REVISION_LEN) return;
        setRevision(ack_content->at(ACK_FIRST_PARAM_CODE).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+1).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+2).toUInt());
        qDebug() << "CAN DRIVER REVISION: " << maj_rev << "." << min_rev << "." << sub_rev;
    }
    return;
}

void canDriver::handleServerConnections(bool status){
    if(status){
        qDebug() << "CAN DRIVER CONNECTED";
    }else qDebug() << "CAN DRIVER DISCONNECTED";
}

