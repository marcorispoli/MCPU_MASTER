#include "application.h"
#include "compressor.h"



compressorProcess::compressorProcess(void):masterInterface(SYSCONFIG->getParam<QString>(SYS_COMPRESSOR_PROCESS_PARAM,SYS_PROCESS_NAME),SYSCONFIG->getParam<QString>(SYS_COMPRESSOR_PROCESS_PARAM,SYS_PROCESS_PARAM),SYSCONFIG->getParam<QString>(SYS_COMPRESSOR_PROCESS_PARAM,SYS_PROCESS_IP),SYSCONFIG->getParam<int>(SYS_COMPRESSOR_PROCESS_PARAM,SYS_PROCESS_PORT))
{


}

void compressorProcess::handleReceivedEvent(QList<QString>* event_content){

    return;
}

void compressorProcess::handleReceivedAck(QList<QString>* ack_content){

    if(ack_content->at(ACK_CMD_CODE) == GET_REVISION){

        if(ack_content->size() != GET_REVISION_LEN) return;
        setRevision(ack_content->at(ACK_FIRST_PARAM_CODE).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+1).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+2).toUInt());
        qDebug() << "COMPRESSOR  PROCESS REVISION: " << maj_rev << "." << min_rev << "." << sub_rev;
    }
    return;
}

void compressorProcess::handleServerConnections(bool status){
    if(status){
        qDebug() << "COMPRESSOR PROCESS CONNECTED";
    }else qDebug() << "COMPRESSOR PROCESS DISCONNECTED";
}

