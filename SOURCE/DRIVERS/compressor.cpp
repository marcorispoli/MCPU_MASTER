#include "application.h"
#include "compressor.h"



compressorProcess::compressorProcess(void):masterInterface("COMPRESSOR",SYSCONFIG->getParam<QString>(SYS_COMPRESSOR_PROCESS_PARAM,SYS_PROCESS_NAME),SYSCONFIG->getParam<QString>(SYS_COMPRESSOR_PROCESS_PARAM,SYS_PROCESS_PARAM),SYSCONFIG->getParam<QString>(SYS_COMPRESSOR_PROCESS_PARAM,SYS_PROCESS_IP),SYSCONFIG->getParam<int>(SYS_COMPRESSOR_PROCESS_PARAM,SYS_PROCESS_PORT))
{


}


void compressorProcess::handleReceivedEvent(QList<QString>* event_content){

    return;
}

void compressorProcess::handleReceivedAck(QList<QString>* ack_content){

    return;
}

void compressorProcess::handleServerConnections(bool status){
    if(status){
        qDebug() << debugProcessName  << " CONNECTED";
    }else qDebug() <<  debugProcessName << " DISCONNECTED";

}

