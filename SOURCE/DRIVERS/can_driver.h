#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include "QObject"
#include "masterInterface.h"

class canDriver : public masterInterface
{
    Q_OBJECT

public:


    explicit canDriver(void);
    ~canDriver(){};

   void handleReceivedEvent(QList<QString>* event_content);
   void handleReceivedAck(QList<QString>* ack_content);
   void handleServerConnections(bool status);

   inline bool isDeviceOpen(void){return deviceOpen;}
   inline bool isStatusReceived(void){return status_updated;}
   inline uint8_t getConverterApiMaj(void){return apiMaj;}
   inline uint8_t getConverterApiMin(void){return apiMin;}
   inline uint8_t getConverterApiSub(void){return apiSub;}
   inline uint32_t getConverterHWSn(void){return deviceSN;}
   inline uint8_t getConverterHWrev(void){return hwRev;}
   inline uint8_t getConverterHWsrev(void){return fwRev;}



   const char* GET_REVISION = "GetRevision";
   const uchar GET_REVISION_LEN = ACK_FIRST_PARAM_CODE + 3;
   inline void SEND_GET_REVISION(void){txCommand(QString(GET_REVISION));};

   const char* GET_STATUS = "GetStatus";
   const uchar GET_STATUS_LEN = ACK_FIRST_PARAM_CODE + 7;
   inline void SEND_GET_STATUS(void){status_updated = false; txCommand(QString(GET_STATUS));};

private slots:


private:
    bool deviceOpen;
    bool status_updated;
    uint8_t apiMaj;
    uint8_t apiMin;
    uint8_t apiSub;
    uint32_t deviceSN;
    uint8_t hwRev;
    uint8_t fwRev;

};

#endif // CAN_DRIVER_H
