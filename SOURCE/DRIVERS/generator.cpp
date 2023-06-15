#include "application.h"
#include "generator.h"



generatorProcess::generatorProcess(void):masterInterface("GENERATOR",SYSCONFIG->getParam<QString>(SYS_GENERATOR_PROCESS_PARAM,SYS_PROCESS_NAME),
                                                   SYSCONFIG->getParam<QString>(SYS_GENERATOR_PROCESS_PARAM,SYS_PROCESS_PARAM),
                                                   SYSCONFIG->getParam<QString>(SYS_GENERATOR_PROCESS_PARAM,SYS_PROCESS_IP),
                                                   SYSCONFIG->getParam<int>(SYS_GENERATOR_PROCESS_PARAM,SYS_PROCESS_PORT))
{


}

void generatorProcess::handleReceivedEvent(QList<QString>* event_content){

    return;
}

void generatorProcess::handleReceivedAck(QList<QString>* ack_content){

    return;
}

void generatorProcess::handleServerConnections(bool status){
    if(status){
        qDebug() << debugProcessName << " PROCESS CONNECTED";
    }else qDebug() << debugProcessName << " PROCESS DISCONNECTED";
}

