#include "application.h"
#include "power_service.h"


powerService::powerService(void):masterInterface(SYSCONFIG->getParam<QString>(SYS_LOCALHOST_PARAM,0),SYSCONFIG->getParam<int>(SYS_POWERSERVICE_PORT_PARAM,0))
{


}

void powerService::handleReceivedEvent(QList<QString>* event_content){

    return;
}
