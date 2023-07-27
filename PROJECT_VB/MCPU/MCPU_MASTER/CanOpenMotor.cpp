#include "pch.h"
#include "CanOpenMotor.h"
#include "CanDriver.h"
#include <thread>


CanOpenMotor::CanOpenMotor(void) {
    hwnd = static_cast<HWND>(Handle.ToPointer());
    
    device_id = 0x7;

    // Start the reception thread
    startup_thread = gcnew Thread(gcnew ThreadStart(this, &CanOpenMotor::startupThread));
    startup_thread->Name = "Motor Startup";
    startup_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
    startup_thread->Start();

    CanDriver::canrx_canopen_sdo_event += gcnew CanDriver::delegate_can_rx_frame(this, &CanOpenMotor::canopen_rx_sdo_callback);
}

void CanOpenMotor::canopen_rx_sdo_callback(unsigned short canid, unsigned char* data, unsigned char len) {
    Debug::WriteLine("Motor received: " + Convert::ToString(canid));
}


void CanOpenMotor::startupThread(void) {
    unsigned char buffer[10];
    buffer[0] = 0x40;
    buffer[1] = 0x40;
    buffer[2] = 0x60;
    buffer[3] = 0x0;

    while (1) {
        std::this_thread::sleep_for(std::chrono::microseconds(1000000));
        CanDriver::multithread_send(0x607, buffer, 8);
    }
}
