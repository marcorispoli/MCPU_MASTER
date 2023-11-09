#include "CanDriver.h"
#include "CanDeviceProtocol.h"
#include <thread>

using namespace System::Diagnostics;
using namespace System::Collections::Generic;


CanDeviceProtocol::CanDeviceProtocol(unsigned char devid, LPCWSTR devname) {

    // Gets the handler of the class instance to be used for the Post/Send message functions.
    //hwnd = static_cast<HWND>(Handle.ToPointer());
    device_id = devid;
    rx_register = gcnew CanDeviceProtocol::CanDeviceRegister();
    tx_register = gcnew CanDeviceProtocol::CanDeviceRegister();
    tmo = true;
    attempt = 0;

    // Creates the Reception event
    rxEvent = CreateEvent(
        NULL,               // default security attributes
        FALSE,              // Automatic reset after signaling
        FALSE,              // initial state is nonsignaled
        devname             // object name
    );


    // Creates the receiving register
    rx_pending = false;
    rx_sequence = 1;

    // Add the callback handling the  reception
    CanDriver::canrx_device_event += gcnew CanDriver::delegate_can_rx_frame(this, &CanDeviceProtocol::thread_can_rx_callback);

    internal_status = status_options::WAITING_CAN_DRIVER_CONNECTION;
    command_executing = false;


    // Creates the Startup thread
    main_thread = gcnew Thread(gcnew ThreadStart(this, &CanDeviceProtocol::mainWorker));
    main_thread->Name = "DEVICE" + System::Convert::ToString(device_id);
    main_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
    main_thread->Start();

}

void CanDeviceProtocol::thread_can_rx_callback(unsigned short canid, unsigned char* data, unsigned char len) {
    // Be careful!! This call is runing into the program thread pull, out of the class thread.
    bool bootloader = false;

    
    if ( 
        (!rx_pending) || // No data is expected 
        (len < 8) // Only 8-byte frame is accepted
        )
    {        
        return;
    }
    

    // Checks the canId address
    if (canid == device_id + 0x140) bootloader = false;
    else if (canid == device_id + 0x100) bootloader = true;
    else
    {
        return; // Not a target
    }

    
    if (
        (bootloader != tx_register->bootloader) ||  // Wrong destination
        (!rx_register->decode(data, bootloader)) || // CRC error
        (rx_register->b0 != tx_register->b0) ||     // First byte equal for both protocols
        ((!bootloader) && (rx_register->b1 != tx_register->b1))// The command equals only for non bootloader frames
        )
    {
        SetEvent(rxEvent);
        return;
    }
    

    // Checks that the received Object registers matches with the expected    
    rx_pending = false;    
    rxOk = true;

    // Wake the waiting command
    SetEvent(rxEvent);

    return;
}

bool CanDeviceProtocol::send(unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, unsigned char d4, unsigned char d5, unsigned char d6, unsigned char d7, bool bootl) {

    unsigned char buffer[8];
    DWORD dwWaitResult;
    unsigned short canid;


    if (bootl) {
        canid = device_id + 0x100;
        tx_register->set(d0, d1, d2, d3, d4, d5, d6, d7, bootl);
        tx_register->format(buffer);
    }
    else {
        canid = device_id + 0x140;
        rx_sequence++;
        tx_register->set(rx_sequence, d1, d2, d3, d4, d5, d6, d7, bootl);
        tx_register->format(buffer);
        buffer[7] = tx_register->crc;
    }
    
    if(!tmo)  attempt++;
    if (attempt > 10) {
        if(!tmo) Debug::WriteLine("Device Board <" + System::Convert::ToString(device_id) + ">: TMO");
        tmo = true;
    }

    // Activates the transmission
    rxOk = false;
    if (!CanDriver::multithread_send(canid, buffer, 8))   return false;

    rx_pending = true;

    // Waits to be signalled: waits for 100ms
    dwWaitResult = WaitForSingleObject(rxEvent, 100);
    rx_pending = false;


    // Checks if the Event has been signalled or it is a timeout event.
    if (dwWaitResult != WAIT_OBJECT_0) return false;
    if (!rxOk) return false;

    attempt=0;
    tmo = false;
    return true;
    
}


void CanDeviceProtocol::mainWorker(void) {


    Debug::WriteLine("Device Board <" + System::Convert::ToString(device_id) + ">: MAIN THREAD STARTED");

    while (1) {

        
        if (!CanDriver::isConnected()) internal_status = status_options::WAITING_CAN_DRIVER_CONNECTION;

        switch (internal_status) {
        
        case status_options::WAITING_CAN_DRIVER_CONNECTION: // Waits for the CAN DRIVER connection
            if (CanDriver::isConnected()) {
                internal_status = status_options::WAITING_REVISION;
            }else{
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }            
            break;
        

        case status_options::WAITING_REVISION: // Waits for the device revision acquisition     
            if (send(GET_BOOTLOADER_INFO)) {
                bootloader_status = rx_register->b1;
                boot_maj = rx_register->b2;
                boot_min = rx_register->b3;
                boot_sub = rx_register->b4;
                app_maj = rx_register->b5;
                app_min = rx_register->b6;
                app_sub = rx_register->b7;
                internal_status = status_options::DEVICE_CONFIGURATION;
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            
            break;

        case status_options::DEVICE_CONFIGURATION: // Waits for the device configuration
            if(configurationLoop()) internal_status = status_options::DEVICE_RUNNING;
            else internal_status = status_options::DEVICE_ERROR;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));            
            break;

        case status_options::DEVICE_RUNNING: // Device running
            InternalRunningLoop();
            break;

        case status_options::DEVICE_ERROR: // Device running
            if(errorLoop()) internal_status = status_options::DEVICE_CONFIGURATION;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            break;

        } // Switch internal_status
    }

}


void CanDeviceProtocol::InternalRunningLoop(void) {

    // Command execution sequence
    if (command_executing) {
        int timeout = command_tmo;

        // Test if a command is currently running
        while (!send(GET_COMMAND_REGISTER)) {
            if (isTmo()) {
                Debug::WriteLine("Device Board <" + System::Convert::ToString(device_id) + ">: Command Read Error");
                command_error = (System::Byte)CommandRegisterErrors::COMMAND_DEVICE_TMO;
                command_executing = false;
                return;
            }            
        }


        if (getRxRegister()->b3 == (System::Byte) CommandRegisterStatus::COMMAND_EXECUTING) {
            Debug::WriteLine("Device Board <" + System::Convert::ToString(device_id) + ">: Command Execution Busy");
            command_error = (System::Byte) CommandRegisterErrors::COMMAND_ERROR_BUSY;
            command_executing = false;
            return;
        }

        // Sends the command code
        if (!send(0, 10, command_code, command_d0, command_d1, command_d2, command_d3, 0, false)) {
            Debug::WriteLine("Device Board <" + System::Convert::ToString(device_id) + ">: Command Not Executed");
            command_error = (System::Byte)CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED;
            command_executing = false;
            return;
        }
        
        // Gets the command returning code
        if (getRxRegister()->b3 == (System::Byte)CommandRegisterStatus::COMMAND_ERROR) {
            Debug::WriteLine("Device Board <" + System::Convert::ToString(device_id) + ">: Command Execution Error");
            command_error = getRxRegister()->b6;
            command_executing = false;
            return;
        }

        // Command is immediate
        if (getRxRegister()->b3 == (System::Byte)CommandRegisterStatus::COMMAND_TERMINATED) {
            Debug::WriteLine("Device Board <" + System::Convert::ToString(device_id) + ">: Command Immediate Executed");
            command_error = (System::Byte) CommandRegisterErrors::COMMAND_NO_ERROR;
            command_ris0 = getRxRegister()->b4;
            command_ris1 = getRxRegister()->b5;
            command_executing = false;
            return;
        }

        // Reads the Command Register to detect the command completion
        while (true) {
            timeout--;
            if (!timeout) {
                Debug::WriteLine("Device Board <" + System::Convert::ToString(device_id) + ">: Command Execution Timeout");
                command_error = (System::Byte)CommandRegisterErrors::COMMAND_DEVICE_TMO;
                command_executing = false;
                return;
            }

            while (!send(GET_COMMAND_REGISTER)) {
                if (isTmo()) {
                    Debug::WriteLine("Device Board <" + System::Convert::ToString(device_id) + ">: Command Read Error");
                    command_error = (System::Byte)CommandRegisterErrors::COMMAND_DEVICE_TMO;
                    command_executing = false;
                    return;
                }
            }

            
            // The command is still executing
            if (getRxRegister()->b3 == (System::Byte)CommandRegisterStatus::COMMAND_EXECUTING) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            if (getRxRegister()->b3 == (System::Byte)CommandRegisterStatus::COMMAND_TERMINATED) {
                Debug::WriteLine("Device Board <" + System::Convert::ToString(device_id) + ">: Command Successfully Terminated");
                command_error = (System::Byte)CommandRegisterErrors::COMMAND_NO_ERROR;
                command_ris0 = getRxRegister()->b4;
                command_ris1 = getRxRegister()->b5;
                command_executing = false;
                return;
            }

            // Error condition
            Debug::WriteLine("Device Board <" + System::Convert::ToString(device_id) + ">: Command Terminated in Error");
            command_error = getRxRegister()->b6;
            command_executing = false;
            return;
            

            // Repeats the read command register
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

    }

    // Application runninig loop
    runningLoop();
    return;
}


void CanDeviceProtocol::runningLoop(void) {
    std::this_thread::sleep_for(std::chrono::microseconds(500000));
    return;
}

bool CanDeviceProtocol::errorLoop(void) {    
    std::this_thread::sleep_for(std::chrono::microseconds(500000));
    return false;
}
bool CanDeviceProtocol::configurationLoop(void) {
    std::this_thread::sleep_for(std::chrono::microseconds(500000));
    return true;
}

bool CanDeviceProtocol::command(unsigned char code, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, int tmo) {
    if (command_executing) return false;
    if (internal_status != status_options::DEVICE_RUNNING) return false;

    command_executing = true;
    command_tmo = tmo;
    command_code = code;
    command_d0 = d0;
    command_d1 = d1;
    command_d2 = d2;
    command_d3 = d3;
    command_error = 0;

    return true;
}
bool CanDeviceProtocol::isCommandCompleted(void) {
    return !command_executing;
}
