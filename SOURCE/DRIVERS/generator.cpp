#include "application.h"
#include "generator.h"



generatorProcess::generatorProcess(void):masterInterface(SYSCONFIG->getParam<QString>(SYS_GENERATOR_PROCESS_PARAM,SYS_PROCESS_NAME),
                                                   SYSCONFIG->getParam<QString>(SYS_GENERATOR_PROCESS_PARAM,SYS_PROCESS_PARAM),
                                                   SYSCONFIG->getParam<QString>(SYS_GENERATOR_PROCESS_PARAM,SYS_PROCESS_IP),
                                                   SYSCONFIG->getParam<int>(SYS_GENERATOR_PROCESS_PARAM,SYS_PROCESS_PORT))
{


}

void generatorProcess::handleReceivedEvent(QList<QString>* event_content){

    return;
}

void generatorProcess::handleReceivedAck(QList<QString>* ack_content){

    if(ack_content->at(ACK_CMD_CODE) == GET_REVISION){

        if(ack_content->size() != GET_REVISION_LEN) return;
        setRevision(ack_content->at(ACK_FIRST_PARAM_CODE).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+1).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+2).toUInt());
        qDebug() << "GENERATOR  PROCESS REVISION: " << maj_rev << "." << min_rev << "." << sub_rev;
    }
    return;
}

void generatorProcess::handleServerConnections(bool status){
    if(status){
        qDebug() << "GENERATOR PROCESS CONNECTED";
    }else qDebug() << "GENERATOR PROCESS DISCONNECTED";
}

