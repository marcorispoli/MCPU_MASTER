#include "application.h"
#include "collimator.h"



collimatorProcess::collimatorProcess(void):masterInterface(SYSCONFIG->getParam<QString>(SYS_COLLIMATOR_PROCESS_PARAM,SYS_PROCESS_NAME),SYSCONFIG->getParam<QString>(SYS_COLLIMATOR_PROCESS_PARAM,SYS_PROCESS_PARAM),SYSCONFIG->getParam<QString>(SYS_COLLIMATOR_PROCESS_PARAM,SYS_PROCESS_IP),SYSCONFIG->getParam<int>(SYS_COLLIMATOR_PROCESS_PARAM,SYS_PROCESS_PORT))
{


}

#define PROCESS_NAME QString("COLLIMATOR")

void collimatorProcess::handleReceivedEvent(QList<QString>* event_content){
    if(event_content->at(EVENT_CMD) == EVENT_INIT_COMPLETED){
        if(event_content->size() != EVENT_INIT_COMPLETED_LEN) return;

        boardInitialized = true;
        bootloader_error = event_content->at(EVENT_FIRST_PARAM_CODE).toUInt();
        bootloader_present = event_content->at(EVENT_FIRST_PARAM_CODE+1).toUInt();
        bootloader_running = event_content->at(EVENT_FIRST_PARAM_CODE+2).toUInt();
        boardAppMaj = event_content->at(EVENT_FIRST_PARAM_CODE+3).toUInt();
        boardAppMin = event_content->at(EVENT_FIRST_PARAM_CODE+4).toUInt();
        boardAppSub = event_content->at(EVENT_FIRST_PARAM_CODE+5).toUInt();
        bootloaderMaj = event_content->at(EVENT_FIRST_PARAM_CODE+6).toUInt();
        bootloaderMin = event_content->at(EVENT_FIRST_PARAM_CODE+7).toUInt();
        bootloaderSub = event_content->at(EVENT_FIRST_PARAM_CODE+8).toUInt();

        if(bootloader_error) qDebug() << "POWERSERVICE BOARD STATUS: BOOTLOADER ERROR " << bootloader_error;
        else{
            qDebug() << PROCESS_NAME  << " BOARD STATUS: BOOTPRESENT->" << bootloader_present \
                     << " BOOTRUN->" << bootloader_running \
                     << " BOOTREV->" << bootloaderMaj <<"."<<bootloaderMin<<"."<<bootloaderSub \
                     << " APPREV->" << boardAppMaj <<"."<<boardAppMin<<"."<<boardAppSub ;
        }

        return;
    }
    return;
}

void collimatorProcess::handleReceivedAck(QList<QString>* ack_content){

    if(ack_content->at(ACK_CMD_CODE) == GET_REVISION){

        if(ack_content->size() != GET_REVISION_LEN) return;
        setRevision(ack_content->at(ACK_FIRST_PARAM_CODE).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+1).toUInt(), ack_content->at(ACK_FIRST_PARAM_CODE+2).toUInt());
        qDebug() << PROCESS_NAME  <<  " REVISION: " << maj_rev << "." << min_rev << "." << sub_rev;
        return;
    }

    if(ack_content->at(ACK_CMD_CODE) == BOARD_INIT){

        if(ack_content->size() != BOARD_INIT_LEN) return;
        qDebug() << PROCESS_NAME  << " BOARD INIT ACK";
        return;
    }

    return;
}

void collimatorProcess::handleServerConnections(bool status){
    if(status){
        qDebug() << PROCESS_NAME  << " CONNECTED";
    }else qDebug() << PROCESS_NAME  << " DISCONNECTED";
}

