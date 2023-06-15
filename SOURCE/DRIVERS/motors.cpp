#include "application.h"
#include "motors.h"



motorsProcess::motorsProcess(void):masterInterface("MOTORS",SYSCONFIG->getParam<QString>(SYS_MOTORS_PROCESS_PARAM,SYS_PROCESS_NAME),
                                                   SYSCONFIG->getParam<QString>(SYS_MOTORS_PROCESS_PARAM,SYS_PROCESS_PARAM),
                                                   SYSCONFIG->getParam<QString>(SYS_MOTORS_PROCESS_PARAM,SYS_PROCESS_IP),
                                                   SYSCONFIG->getParam<int>(SYS_MOTORS_PROCESS_PARAM,SYS_PROCESS_PORT))
{

    trx_initialized = false;
    slide_initialized = false;
    body_initialized = false;
    carm_initialized = false;
    mvert_initialized = false;

    trx_init_error = false;
    slide_init_error = false;
    carm_init_error = false;
    body_init_error = false;
    mvert_init_error = false;
}

void motorsProcess::handleServerConnections(bool status){
    if(status){
        qDebug() << debugProcessName << " PROCESS CONNECTED";
    }else qDebug() << debugProcessName << " PROCESS DISCONNECTED";
}

void motorsProcess::handleReceivedAck(QList<QString>* ack_content){


    return;
}

//________________________________________ EVENTS MANAGEMENT ______________________________________________________ //
void motorsProcess::handleReceivedEvent(QList<QString>* event_content){

    if(event_content->at(EVENT_CMD) == EVENT_TRX_INIT_COMPLETED){
        if(event_content->size() != EVENT_TRX_INIT_COMPLETED_LEN) return;

        trx_initialized = true;

        if(event_content->at(EVENT_FIRST_PARAM_CODE).toInt() == 0) trx_init_error = true;
        else trx_init_error = false;
        return;
    }

    if(event_content->at(EVENT_CMD) == EVENT_SLIDE_INIT_COMPLETED){
        if(event_content->size() != EVENT_SLIDE_INIT_COMPLETED_LEN) return;
        slide_initialized = true;

        if(event_content->at(EVENT_FIRST_PARAM_CODE).toInt() == 0) slide_init_error= true;
        else slide_init_error = false;
        return;
    }

    if(event_content->at(EVENT_CMD) == EVENT_BODY_INIT_COMPLETED){
        if(event_content->size() != EVENT_BODY_INIT_COMPLETED_LEN) return;
        body_initialized = true;

        if(event_content->at(EVENT_FIRST_PARAM_CODE).toInt() == 0) body_init_error= true;
        else body_init_error = false;
        return;
    }

    if(event_content->at(EVENT_CMD) == EVENT_CARM_INIT_COMPLETED){
        if(event_content->size() != EVENT_CARM_INIT_COMPLETED_LEN) return;
        carm_initialized = true;

        if(event_content->at(EVENT_FIRST_PARAM_CODE).toInt() == 0) carm_init_error= true;
        else carm_init_error = false;
        return;
    }

    if(event_content->at(EVENT_CMD) == EVENT_MVERT_INIT_COMPLETED){
        if(event_content->size() != EVENT_MVERT_INIT_COMPLETED_LEN) return;
        mvert_initialized = true;

        if(event_content->at(EVENT_FIRST_PARAM_CODE).toInt() == 0) mvert_init_error= true;
        else mvert_init_error = false;
        return;
    }

    return;
}



