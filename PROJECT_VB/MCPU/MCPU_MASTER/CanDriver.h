#pragma once


#define DEVICE_PROTOCOL_DISPATCH_MESSAGE (WM_USER + 1)
#define CANOPEN_NMT_DISPATCH_MESSAGE (WM_USER + 2)
#define CANOPEN_SYNC_DISPATCH_MESSAGE (WM_USER + 3)
#define CANOPEN_EMERGENCY_DISPATCH_MESSAGE (WM_USER + 4)
#define CANOPEN_PDO_DISPATCH_MESSAGE (WM_USER + 5)
#define CANOPEN_SDO_DISPATCH_MESSAGE (WM_USER + 6)
#define CANOPEN_BOOTUP_DISPATCH_MESSAGE (WM_USER + 7)

using namespace std;

ref class CanDriver: public System::Windows::Forms::Form
{
public:
    CanDriver();
    delegate void delegate_can_rx_frame(unsigned short canid, unsigned char* data, unsigned char len);
    static event delegate_can_rx_frame^ canrx_canopen_sdo_event;
    static event delegate_can_rx_frame^ canrx_canopen_pdo_event;
    static event delegate_can_rx_frame^ canrx_canopen_nmt_event;
    static event delegate_can_rx_frame^ canrx_canopen_emrg_event;
    static event delegate_can_rx_frame^ canrx_canopen_bootup_event;
    static event delegate_can_rx_frame^ canrx_canopen_sync_event;
    static event delegate_can_rx_frame^ canrx_device_event;

    static bool multithread_send(unsigned short canId, unsigned char* data, unsigned char len);

    protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;//!< This is the function handling the Window's messages

private:
    Thread^ running_thread;
    void threadWork(void);
    HWND can_hwnd;
    bool can_connected;

    // For debug purpose
    System::Timers::Timer^ TestTimer;
    void testTimerTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e);
};

