#include "CanDriver.h"
#include "CanDeviceProtocol.h"
#include "gantry_global_status.h"
#include <thread>
#include <mutex>
#include "Log.h"

using namespace System::Diagnostics;
using namespace System::Collections::Generic;

static std::mutex command_wait_mutex; //!< Mutex used for command with waiting access

CanDeviceProtocol::CanDeviceProtocol(unsigned char devid, LPCWSTR devname) {

    // Gets the handler of the class instance to be used for the Post/Send message functions.
    //hwnd = static_cast<HWND>(Handle.ToPointer());
    device_id = devid;
    device_reset = false;
    rx_register = gcnew CanDeviceProtocol::CanDeviceRegister();
    tx_register = gcnew CanDeviceProtocol::CanDeviceRegister();
    attempt = 0;
    communication_error = false;
    run = false;
    simulator_mode = false;
    command_executing = false;
    
    register_access_fault = 0;
    register_access_fault_counter = 0;

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


    // Creates the Startup thread
    main_thread = gcnew Thread(gcnew ThreadStart(this, &CanDeviceProtocol::mainWorker));
    main_thread->Name = "DEVICE" + System::Convert::ToString(device_id);
    main_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
    main_thread->Start();

}

void CanDeviceProtocol::thread_can_rx_callback(unsigned short canid, unsigned char* data, unsigned char len) {
    // Be careful!! This call is runing into the program thread pull, out of the class thread.
    bool bootloader = false;

    // No more transaction with a register access fault activated
    if (register_access_fault) return ;
    if (len < 8) return;
    if (!rx_pending) return;


    // Checks the canId address
    if (canid == device_id + 0x140) bootloader = false;
    else if (canid == device_id + 0x100) bootloader = true;
    else
    {
        return; // Not a target
    }

    bool decode_ok = rx_register->decode(data, bootloader);

    // Verifies if there is a Device Reset condition to be signaled
    if ((!bootloader) && (rx_register->b1 == (unsigned char) ProtocolFrameCode::FRAME_DEVICE_RESET)) {
        device_reset = true;
        SetEvent(rxEvent);
        return;
    }

    // Wrong destination address
    if (bootloader != tx_register->bootloader) return;
    
    // Wrong CRC
    if (!decode_ok) {
        LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: wrong crc");        
        return;
    }

    // Invalid sequence number
    if(bootloader && (rx_register->b0 != 0xFF) && (rx_register->b0 != tx_register->b0) ) return;
    if (!bootloader && (rx_register->b0 != tx_register->b0)) return;

    
    // Invalid register access
    if ( (bootloader && (rx_register->b0 == 0xFF)) || (!bootloader && (rx_register->b1 == 0)) ){
        if (register_access_fault_counter > 10) {
            register_access_fault = true;
            LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: FATAL!! invalid register access. Stop communication");
        } else {
            register_access_fault_counter++;
            LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: invalid register access");
        }

        SetEvent(rxEvent);
        return;
    }

    // Wrong Command match
    if ((!bootloader) && (rx_register->b1 != tx_register->b1)) {
        LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: wrong command match");
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

    // No more transaction with a register access fault activated
    if (register_access_fault) return false;

    if (bootl) {
        canid = device_id + 0x100;
        tx_register->set(d0, d1, d2, d3, d4, d5, d6, d7, bootl);
        tx_register->format(buffer);
    }
    else {
        canid = device_id + 0x140;
        rx_sequence++;
        if (!rx_sequence) rx_sequence = 1;
        tx_register->set(rx_sequence, d1, d2, d3, d4, d5, d6, d7, bootl);
        tx_register->format(buffer);
        buffer[7] = tx_register->crc;
    }
    
    // Activates the transmission
    rxOk = false;
    communication.sent_messages++;
    long start = clock();
    if (!CanDriver::multithread_send(canid, buffer, 8))   return false;
    rx_pending = true;

    // Waits to be signalled: waits for 100ms
    dwWaitResult = WaitForSingleObject(rxEvent, 2000);
    rx_pending = false;

    // Checks if the Event has been signalled or it is a timeout event.
    if ((dwWaitResult != WAIT_OBJECT_0) || (!rxOk)) {
        communication.unreceived_messages++;
        if (communication.unreceived_messages > 5) communication_error = true;
        return false;
    }
    communication.unreceived_messages = 0;
    communication_error = false;

    long stop = clock();
    communication.txrx_time = ((double)(stop - start)) / (double)CLOCKS_PER_SEC;
    communication.updateStatistics();


    return true;
    
}


void CanDeviceProtocol::mainWorker(void) {
    LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: thread started: wait run command");
    while (!run) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Demo mode activation
    if (simulator_mode) {
        internal_status = status_options::DEVICE_SIMULATOR;
        LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Module Run in Demo mode");
        while (simulator_mode) {
            demoLoop();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    // Add the callback handling the  reception
    CanDriver::canrx_device_event += gcnew CanDriver::delegate_can_rx_frame(this, &CanDeviceProtocol::thread_can_rx_callback);
    internal_status = status_options::WAITING_CAN_DRIVER_CONNECTION;

    LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Module Run mode");
    while (1) {
        
        if (!CanDriver::isConnected()) internal_status = status_options::WAITING_CAN_DRIVER_CONNECTION;

        switch (internal_status) {
        
        case status_options::WAITING_CAN_DRIVER_CONNECTION: // Waits for the CAN DRIVER connection
            if (CanDriver::isConnected()) {
                internal_status = status_options::WAITING_REVISION;
                LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Wait Revision");
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
                LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Configuration");
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            
            break;

        case status_options::DEVICE_CONFIGURATION: // Waits for the device configuration
            if (configurationLoop()) {
                LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Running");
                internal_status = status_options::DEVICE_RUNNING;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));            
            break;

        case status_options::DEVICE_RUNNING: // Device running
            // If the device should reset
            if (device_reset) {
                LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Reset Detected");
                device_reset = false;
                internal_status = status_options::WAITING_CAN_DRIVER_CONNECTION;
                resetLoop();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                break;
            }
            InternalRunningLoop();
            break;


        } // Switch internal_status
    }

}

/// <summary>
/// This function handles the Module workflow.
/// </summary>
/// 
/// The Module workflow is following described:
/// + run the application loop;
/// + handles the command execution;
/// + if the command takes time to be completed, a nested loop waits for the command completion;
///     + In the nested loop, every 100ms, he application loop is always called;
/// 
/// <param name=""></param>
void CanDeviceProtocol::InternalRunningLoop(void) {

    // Calls the application loop 
    runningLoop();
   
    if (!command_executing) return;

    // handles the command execution  
    if (isCommunicationError()) {
        LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Command Communication error");
        command_error = (System::Byte)CommandRegisterErrors::COMMAND_COMMUNICATION_ERROR;
        command_executing = false;
        return;
    }
       
    int timeout = command_tmo;
    
    // Reads the command register of the remote device
    if (!send(GET_COMMAND_REGISTER)) return;


    if (getRxRegister()->b3 == (System::Byte) CommandRegisterStatus::COMMAND_EXECUTING) {
        LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Command Execution Busy");
        command_error = (System::Byte) CommandRegisterErrors::COMMAND_ERROR_BUSY;
        command_executing = false;
        return;
    }

    // Sends the Requested Command
    if (!send(0, 10, command_code, command_d0, command_d1, command_d2, command_d3, 0, false)) {
        LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Command Not Executed");
        command_error = (System::Byte)CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED;
        command_executing = false;
        return;
    }
        
    // Gets the command returning code
    if (getRxRegister()->b3 == (System::Byte)CommandRegisterStatus::COMMAND_ERROR) {
        LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Command Execution Error");
        command_error = getRxRegister()->b6;
        command_executing = false;
        return;
    }

    // Command is immediate
    if (getRxRegister()->b3 == (System::Byte)CommandRegisterStatus::COMMAND_TERMINATED) {
        LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Command Immediate Executed");
        command_error = (System::Byte) CommandRegisterErrors::COMMAND_NO_ERROR;
        command_ris0 = getRxRegister()->b4;
        command_ris1 = getRxRegister()->b5;
        command_executing = false;
        return;
    }

    // If the command takes time to be completed a new loop is handled here
    while (true) {
            
        // During the command loop, the application loop shall continue to work!
        runningLoop();

        timeout--;
        if (!timeout) {
            LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Command Execution Timeout");
            command_error = (System::Byte)CommandRegisterErrors::COMMAND_DEVICE_TMO;
            command_executing = false;
            return;
        }

        while (!send(GET_COMMAND_REGISTER)) {
            if (isCommunicationError()) {
                LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Command Read Error");
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
            LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Command Successfully Terminated");
            command_error = (System::Byte)CommandRegisterErrors::COMMAND_NO_ERROR;
            command_ris0 = getRxRegister()->b4;
            command_ris1 = getRxRegister()->b5;
            command_executing = false;
            return;
        }

        // Error condition
        LogClass::logInFile("Device Board <" + System::Convert::ToString(device_id) + ">: Command Terminated in Error");
        command_error = getRxRegister()->b6;
        command_executing = false;
        return;
            

        // Repeats the read command register
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Command completed
    return;
}


void CanDeviceProtocol::runningLoop(void) {    
    std::this_thread::sleep_for(std::chrono::microseconds(500000));
    return;
}

void CanDeviceProtocol::resetLoop(void) {        
    std::this_thread::sleep_for(std::chrono::microseconds(500000));
    return ;
}
bool CanDeviceProtocol::configurationLoop(void) {    
    std::this_thread::sleep_for(std::chrono::microseconds(500000));
    return true;
}

bool CanDeviceProtocol::commandNoWaitCompletion(unsigned char code, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, int tmo) {
    // Protect the function with a mutex
    const std::lock_guard<std::mutex> lock(command_wait_mutex);

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

CanDeviceProtocol::CanDeviceCommandResult^ CanDeviceProtocol::commandWaitCompletion(unsigned char code, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, int tmo, Object^ src) {
    
    // Protect the function with a mutex
    const std::lock_guard<std::mutex> lock(command_wait_mutex);

    // Verifies that the calling module is different from the class implementing the communication with the device
    if(src == (Object^) this) return gcnew CanDeviceCommandResult(CommandRegisterErrors::COMMAND_INVALID_DEVICE);

    // Invalid Device Status 
    if (internal_status != status_options::DEVICE_RUNNING) return gcnew CanDeviceCommandResult(CommandRegisterErrors::COMMAND_ERROR_MOMENTARY_DISABLED);

    // Command Busy
    if (command_executing)  return gcnew CanDeviceCommandResult(CommandRegisterErrors::COMMAND_ERROR_BUSY);
    

    // Command preparation
    command_executing = true;
    command_tmo = tmo;
    command_code = code;
    command_d0 = d0;
    command_d1 = d1;
    command_d2 = d2;
    command_d3 = d3;
    command_error = 0;

    // Wait for command completion
    while (!isCommandCompleted()) std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Returns the result
    return gcnew CanDeviceCommandResult((CommandRegisterErrors) command_error, command_ris0, command_ris1);
    
}




CanDeviceProtocol::Register^ CanDeviceProtocol::readCommandRegister(void) {
    return nullptr;
}
CanDeviceProtocol::Register^ CanDeviceProtocol::readErrorRegister(void) {
    
    if (!send(
        (System::Byte)0, // Internally set to the sequence number
        (System::Byte) ProtocolFrameCode::FRAME_READ_ERRORS, // Frame code
        (System::Byte)0, // Index not used
        (System::Byte)0, // D0 not used
        (System::Byte)0, // D1 not used
        (System::Byte)0, // D2 not used
        (System::Byte)0, // D3 not used
        (System::Byte)0, // CRC internally set
        false)
        ) return nullptr;

    return gcnew Register(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);

}
CanDeviceProtocol::Register^ CanDeviceProtocol::readStatusRegister(unsigned char index) {

    if (!send(
        (System::Byte) 0, // Internally set to the sequence number
        (System::Byte) ProtocolFrameCode::FRAME_READ_STATUS, // Frame code
        (System::Byte) index, // Status register index
        (System::Byte)0, // d0
        (System::Byte)0, // D1
        (System::Byte)0, // D2 
        (System::Byte)0, // D3
        (System::Byte)0, // CRC internally set
        false)// Bootloader == true, Application = false
    ) return nullptr;

    return gcnew Register(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    
}

CanDeviceProtocol::Register^ CanDeviceProtocol::readDataRegister(unsigned char index) {
    return nullptr;
}
bool CanDeviceProtocol::writeParamRegister(unsigned char index, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3) {
    return send(
        (System::Byte)0, // Internally set to the sequence number
        (System::Byte)ProtocolFrameCode::FRAME_WRITE_PARAM, // Frame code
        (System::Byte)index, // PARAM register index
        (System::Byte)d0, // d0
        (System::Byte)d1, // D1
        (System::Byte)d2, // D2 
        (System::Byte)d3, // D3
        (System::Byte)0, // CRC internally set
        false); // Bootloader == true, Application = false

}
bool CanDeviceProtocol::writeDataRegister(unsigned char index, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3) {

    return send(
        (System::Byte)0, // Internally set to the sequence number
        (System::Byte)ProtocolFrameCode::FRAME_WRITE_DATA, // Frame code
        (System::Byte)index, // DATA register index
        (System::Byte)d0, // d0
        (System::Byte)d1, // D1
        (System::Byte)d2, // D2 
        (System::Byte)d3, // D3
        (System::Byte)0, // CRC internally set
        false); // Bootloader == true, Application = false

}
