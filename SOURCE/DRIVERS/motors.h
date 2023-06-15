#ifndef MOTORS_H
#define MOTORS_H

#include "QObject"
#include "masterInterface.h"

class motorsProcess : public masterInterface
{
    Q_OBJECT

public:


    explicit motorsProcess(void);
    ~motorsProcess(){};

   void handleReceivedEvent(QList<QString>* event_content);
   void handleReceivedAck(QList<QString>* ack_content);
   void handleServerConnections(bool status);

   const char* TRX_INITIALIZE = "TrxInitialize";
   const uchar TRX_INITIALIZE_LEN = ACK_FIRST_PARAM_CODE + 0;
   inline void SEND_TRX_INITIALIZE(void){txCommand(QString(TRX_INITIALIZE));};

   const char* SLIDE_INITIALIZE = "SlideInitialize";
   const uchar SLIDE_INITIALIZE_LEN = ACK_FIRST_PARAM_CODE + 0;
   inline void SEND_SLIDE_INITIALIZE(void){txCommand(QString(SLIDE_INITIALIZE));};

   const char* BODY_INITIALIZE = "BodyInitialize";
   const uchar BODY_INITIALIZE_LEN = ACK_FIRST_PARAM_CODE + 0;
   inline void SEND_BODY_INITIALIZE(void){txCommand(QString(BODY_INITIALIZE));};

   const char* CARM_INITIALIZE = "CarmInitialize";
   const uchar CARM_INITIALIZE_LEN = ACK_FIRST_PARAM_CODE + 0;
   inline void SEND_CARM_INITIALIZE(void){txCommand(QString(CARM_INITIALIZE));};

   const char* MVERT_INITIALIZE = "MvertInitialize";
   const uchar MVERT_INITIALIZE_LEN = ACK_FIRST_PARAM_CODE + 0;
   inline void SEND_MVERT_INITIALIZE(void){txCommand(QString(MVERT_INITIALIZE));};



   const char* EVENT_TRX_INIT_COMPLETED = "EVENT_TRX_InitCompleted";
   const uchar EVENT_TRX_INIT_COMPLETED_LEN = EVENT_FIRST_PARAM_CODE + 1;
   const char* EVENT_SLIDE_INIT_COMPLETED = "EVENT_SLIDE_InitCompleted";
   const uchar EVENT_SLIDE_INIT_COMPLETED_LEN = EVENT_FIRST_PARAM_CODE + 1;
   const char* EVENT_CARM_INIT_COMPLETED = "EVENT_CARM_InitCompleted";
   const uchar EVENT_CARM_INIT_COMPLETED_LEN = EVENT_FIRST_PARAM_CODE + 1;
   const char* EVENT_BODY_INIT_COMPLETED = "EVENT_BODY_InitCompleted";
   const uchar EVENT_BODY_INIT_COMPLETED_LEN = EVENT_FIRST_PARAM_CODE + 1;
   const char* EVENT_MVERT_INIT_COMPLETED = "EVENT_MVERT_InitCompleted";
   const uchar EVENT_MVERT_INIT_COMPLETED_LEN = EVENT_FIRST_PARAM_CODE + 1;

   inline bool isTrxInitialized(void){return trx_initialized;};
   inline bool isTrxInitError(void){return trx_init_error;};

   inline bool isSlideInitialized(void){return slide_initialized;};
   inline bool isSlideInitError(void){return slide_init_error;};

   inline bool isBodyInitialized(void){return body_initialized;};
   inline bool isBodyInitError(void){return body_init_error;};

   inline bool isCarmInitialized(void){return carm_initialized;};
   inline bool isCarmInitError(void){return carm_init_error;};

   inline bool isMvertInitialized(void){return mvert_initialized;};
   inline bool isMvertInitError(void){return mvert_init_error;};

private slots:


private:
   bool trx_initialized;
   bool slide_initialized;
   bool body_initialized;
   bool carm_initialized;
   bool mvert_initialized;

   bool trx_init_error;
   bool slide_init_error;
   bool carm_init_error;
   bool body_init_error;
   bool mvert_init_error;
};

#endif // MOTORS_H
