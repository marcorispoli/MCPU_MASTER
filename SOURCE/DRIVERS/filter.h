#ifndef FILTER_H
#define FILTER_H

#include "QObject"
#include "masterInterface.h"

class filterProcess : public masterInterface
{
    Q_OBJECT

public:


    explicit filterProcess(void);
    ~filterProcess(){};

   void handleReceivedEvent(QList<QString>* event_content);
   void handleReceivedAck(QList<QString>* ack_content);
   void handleServerConnections(bool status);

   const char* GET_REVISION = "GetRevision";
   const uchar GET_REVISION_LEN = ACK_FIRST_PARAM_CODE + 3;
   inline void SEND_GET_REVISION(void){txCommand(QString(GET_REVISION));};

   const char* BOARD_INIT = "BoardInitialize";
   const uchar BOARD_INIT_LEN = ACK_FIRST_PARAM_CODE + 0;
   inline void SEND_BOARD_INIT(void){txCommand(QString(BOARD_INIT));};

   const char* EVENT_INIT_COMPLETED = "EVENT_InitCompleted";
   const uchar EVENT_INIT_COMPLETED_LEN = EVENT_FIRST_PARAM_CODE + 9;



   // Board initialization API
   inline bool isBoardInitialized(void) { return boardInitialized;}
   inline bool isBootloaderPresent(void){ return bootloader_present;}
   inline bool isBootloaderRunning(void){ return bootloader_running;}
   inline uint getBootloaderError(void) { return bootloader_error;}
   inline uint getBoardAppMaj(void)     { return boardAppMaj;}
   inline uint getBoardAppMin(void)     { return boardAppMin;}
   inline uint getBoardAppSub(void)     { return boardAppSub;}
   inline uint getBootloaderMaj(void)   { return bootloaderMaj;}
   inline uint getBootloaderMin(void)   { return bootloaderMin;}
   inline uint getBootloaderSub(void)   { return bootloaderSub;}

private slots:


private:

   // Board Application revision code from the initialization
   bool boardInitialized;
   bool bootloader_present;
   bool bootloader_running;
   uint bootloader_error;
   uint boardAppMaj;
   uint boardAppMin;
   uint boardAppSub;
   uint bootloaderMaj;
   uint bootloaderMin;
   uint bootloaderSub;


};

#endif // FILTER_H
