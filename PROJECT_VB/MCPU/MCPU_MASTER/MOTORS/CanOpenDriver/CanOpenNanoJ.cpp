#include "CanDriver.h"
#include "Notify.h"
#include "CanOpenMotor.h"
#include "pd4_od.h"
#include <thread>



using namespace CANOPEN;


/// <summary>
/// This function initializes the Nanoj program download 
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CanOpenMotor::initNanojDataRegister(void) {
    unsigned char buffer[8];
    DWORD dwWaitResult;

    nanoj_rx_pending = false;

    // Special register with dimension set to 0
    rxSdoRegister = gcnew ODRegister(VMM_DATA_OD, 0);
    rxSdoRegister->data_dim = ODRegister::SDODataDimension::R0B;
    rxSdoRegister->getWriteBuffer(buffer);

    // Activates the transmission
    CanDriver::multithread_send(0x600 + device_id, buffer, 8);
    sdo_rx_pending = true;

    // Waits to be signalled: waits for 50ms
    dwWaitResult = WaitForSingleObject(rxSDOEvent, 1000);
    sdo_rx_pending = false;


    // Checks if the Event has been signalled or it is a timeout event.
    if (dwWaitResult != WAIT_OBJECT_0) return false;
    if (!rxSdoRegister->valid) return false;

    return true;
}


/// <summary>
/// This function Upload a block of 1024 byte into the Motor nanoj Ram Area
/// 
/// </summary>
/// <param name="vectorIndex">pointer to the nanoj program vector at the current position</param>
/// <param name="block_size">block size definition</param>
/// <returns></returns>
bool CanOpenMotor::nanojWrite1024Block(int vectorIndex, int block_size) {
    unsigned char buffer[8];
    DWORD dwWaitResult;

    // Initialize the  Data Ram
    if (!initNanojDataRegister()) return false;


    int index = 0;
    int n = 0;
    bool t = false;

    while (index < block_size) {

        // buffer[0] is the data segment control byte
        buffer[0] = 0;
        if (t) buffer[0] |= 0x10;
        rxNanojAck = buffer[0] | 0x20;

        if (index + 7 >= block_size) buffer[0] |= 0x1; // End of block
        n = 7 - (block_size - index);
        if (n < 0) n = 0;
        buffer[0] |= (n * 2); // Data to be ignored

        // Data segment preparation
        for (int k = 0; k < 7; k++) {
            if (index + k < block_size) buffer[1 + k] = pNanoj[vectorIndex + index + k];
        }

        // Data send to can bus
        sdo_rx_pending = false;
        rxNanojAckValid = false;

        // Activates the transmission
        CanDriver::multithread_send(0x600 + device_id, buffer, 8);
        nanoj_rx_pending = true;

        // Waits to be signalled: waits for 50ms
        dwWaitResult = WaitForSingleObject(rxSDOEvent, 1000);
        nanoj_rx_pending = false;

        // Checks if the Event has been signalled or it is a timeout event.
        if (dwWaitResult != WAIT_OBJECT_0) return false;
        if (!rxNanojAckValid) return false;

        index += 7;
        t = !t;
    }

    return true;

}

/// <summary>
/// This function handles the Upload nanoj program steps.
/// 
/// The function executes the Uploading only in the case 
/// the program is changed or it is not yet programmed.
/// - The module verifies if the current program checksum 
///   is different of the sotred into the motor device.
/// 
/// The Module:
/// - Delete the VMM flash memory in the target;
/// - Reset the module to apply the Delete command (mandatory);
/// - Upload blocks of 1024 bytes and stores it;
/// - Stores the new program checksum in the USER register area;
/// 
/// </summary>
/// <param name=""></param>
/// <returns>true in case of upload success</returns>
bool CanOpenMotor::uploadNanojProgram(void) {

    // Calculate the nanoj-program checksum
    unsigned short vmmchk = 0;
    for (int i = 0; i < nanojSize; i++) vmmchk += (unsigned short)pNanoj[i];

    // Read the stored checksum to check if the program needs to be updated
    if (!blocking_readOD(NANOJ_USER_PARAM)) return false; // Reads the user parameter containing the stored nanoj checksum
    if (rxSdoRegister->data == vmmchk) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: NANOJ PROGRAM ALREADY PROGRAMMED");
        return true;
    }

    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: NANOJ PROGRAMMING PROCESS");

    // WMM FLASH DELETE
    if (!blocking_writeOD(VMM_CTRL_OD, VMM_DELETE)) return false;

    // Reset node to let the WMM be cleared by the Engine
    write_resetNode();

    // Wait for the reset completed
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    while (!blocking_readOD(VMM_STATUS_OD)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: NODE RESET COMPLETED");

    // Read the VMM status  register
    if (!blocking_readOD(VMM_STATUS_OD)) return false;
    if (rxSdoRegister->data != 0) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: VMM FLASH IN ERROR CONDITION");
        return false;
    }

    // WMM FLASH INITIALIZATION
    if (!blocking_writeOD(VMM_CTRL_OD, VMM_INIT)) return false;


    int index = 0;
    int block_size = 0;
    int i;

    while (1) {
        block_size = nanojSize - (index * 1024);
        if (block_size == 0) break;
        if (block_size > 1024) block_size = 1024;

        // Write a 1024 block
        if (!nanojWrite1024Block(1024 * index, block_size)) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR TRANSFERRING BLOCK ");
            return false;
        }

        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: STORE BLOCK [" + index.ToString() + "]");


        // Store Data ram in Flash
        if (!blocking_writeOD(VMM_CTRL_OD, VMM_WRITE)) return false;

        // Read the control register waiting for store completion
        for (i = 30; i > 0; i--) {
            if (!blocking_readOD(VMM_CTRL_OD)) return false;
            if (rxSdoRegister->data == 0) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        if (i == 0) return false;

        // Read the VMM status  register
        if (!blocking_readOD(VMM_STATUS_OD)) return false;
        if (rxSdoRegister->data != 0) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: VMM FLASH IN ERROR CONDITION");
            return false;
        }

        if (block_size < 1024) break;
        index++;

    }

    // Store the checksum
    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: STORE CHECKSUM");
    if (!blocking_writeOD(NANOJ_USER_PARAM, vmmchk)) return false;

    if (!blocking_writeOD(SAVE_USER_PARAM, 1)) return false;

    for (i = 30; i > 0; i--) {
        if (!blocking_readOD(SAVE_USER_PARAM)) return false;
        if (rxSdoRegister->data == 0) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (i == 0) return false;


    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: NANOJ PROGRAM SUCCESSFULLY COMPLETED");
    return true;

}

/// <summary>
/// This function runs the Nano-J program on the Motor Device
/// </summary>
/// 
/// If the Nanoj-Program has been uploaded into the device 
/// this function tries to run it. 
/// 
/// If a program is already in execution the function tries to stop it and restart again.
/// 
/// In case of error in running the program the function fails.
/// 
/// <param name=""></param>
/// <returns>true if the program successfully starts</returns>
bool CanOpenMotor::startNanoj(void) {
    
    // Read the status to check if there is a running program
    if (blocking_readOD(OD_2301_00)) {
        if (rxSdoRegister->data & 0x1) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: NANOJ PROGRAM IS ALREADY RUNNING");

            blocking_writeOD(OD_2300_00, 0); // Stops the running program
            std::this_thread::sleep_for(std::chrono::milliseconds(250));

            blocking_readOD(OD_2301_00);
            if (rxSdoRegister->data & 0x1) {
                Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: FAILED TO STOP THE NANOJ PROGRAM!");
                return false; // Failed to stop the program
            }
        }
    }
        
    // Activate the program
    if (!blocking_writeOD(OD_2300_00, 1)) return false;
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    // Waits the activation
    for (int i = 0; i < 10; i++) {
        if (blocking_readOD(OD_2301_00)) {
            if (rxSdoRegister->data & 0x1) {
                Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: NANOJ PROGRAM STARTED");
                return true;
            } 

            if (rxSdoRegister->data & 0x4) {
                Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: NANOJ PROGRAM START FAILED!");
                blocking_readOD(OD_2302_00);
                Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: NANOJ PROGRAM ERROR CODE:" + rxSdoRegister->data.ToString());
                return false;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return false;
}

/// <summary>
/// This function stops a running Nano-J program.
/// 
/// </summary>
/// <param name=""></param>
/// <returns>true if the program is successfully stopped</returns>
bool CanOpenMotor::stopNanoj(void) {

    blocking_writeOD(OD_2300_00, 0); // Stops the running program
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    blocking_readOD(OD_2301_00);
    if (rxSdoRegister->data & 0x1) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: FAILED TO STOP THE NANOJ PROGRAM!");
        return false; // Failed to stop the program
    }

    return true;
}