#pragma once

#include <Windows.h>
#include "CanSimulator.h"


#define DEVICE_PROTOCOL_DISPATCH_MESSAGE (WM_USER + 1)
#define CANOPEN_NMT_DISPATCH_MESSAGE (WM_USER + 2)
#define CANOPEN_SYNC_DISPATCH_MESSAGE (WM_USER + 3)
#define CANOPEN_EMERGENCY_DISPATCH_MESSAGE (WM_USER + 4)
#define CANOPEN_PDO_DISPATCH_MESSAGE (WM_USER + 5)
#define CANOPEN_SDO_DISPATCH_MESSAGE (WM_USER + 6)
#define CANOPEN_BOOTUP_DISPATCH_MESSAGE (WM_USER + 7)

//using namespace std;

ref class CanDriver
{
public:
    CanDriver();
    static CanDriver^ driver = gcnew  CanDriver();

    static void startSimulatorMode(void);
    static void startNormalMode(void);

    delegate void delegate_can_rx_frame(unsigned short canid, unsigned char* data, unsigned char len);
    static event delegate_can_rx_frame^ canrx_canopen_sdo_event;
    static event delegate_can_rx_frame^ canrx_canopen_pdo_event;
    static event delegate_can_rx_frame^ canrx_canopen_nmt_event;
    static event delegate_can_rx_frame^ canrx_canopen_emrg_event;
    static event delegate_can_rx_frame^ canrx_canopen_bootup_event;
    static event delegate_can_rx_frame^ canrx_canopen_sync_event;
    static event delegate_can_rx_frame^ canrx_device_event;

    // Simulator callbacks
    static void canrx_simulator_canopen_sdo_event(void) ; 
    static void canrx_simulator_device_event(void) ;

    static bool multithread_send(unsigned short canId, unsigned char* data, unsigned char len, bool simul);
    static inline bool isConnected(bool simul) { 
        if(simul) return CanSimulator::connection_status;
        else return can_connected;
    }
 

    static inline bool isError(void) { return error; }
    static inline bool isWarning(void) { return warning; }
    static inline System::String^ getErrorString(void) { return ErrorString; }
    static inline System::String^ getWarningString(void) { return WarningString; }


public :
    static int apirev_maj = 0;
    static int apirev_min = 0;
    static int apirev_sub = 0;
    static int hwrev_maj = 0;
    static int hwrev_min = 0;
    static int swrev_maj = 0;
    static int swrev_min = 0;

private:
    System::Threading::Thread^ running_thread;
    void threadWork(void);
    static bool can_connected = false;
    static bool error = false;
    static System::String^ ErrorString = "";

    static bool warning = false;
    static System::String^ WarningString = "";   
    static bool simulator_mode;
};

