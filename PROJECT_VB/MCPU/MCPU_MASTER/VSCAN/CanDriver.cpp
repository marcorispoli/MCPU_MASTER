#include "CanDriver.h"
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include "vs_can_api.h"
#include "Log.h"


using namespace std;
using namespace std::chrono;
using namespace System::Threading;
using namespace System::Diagnostics;

static VSCAN_HANDLE handle = 0;     //!< Handle of the driver
static mutex send_mutex;


CanDriver::CanDriver() {
    
    
    can_connected = false;
    error = false;
    ErrorString ="";

    warning = false;
    WarningString = "";

    // Connects the simulator events
    CanSimulator::canrx_device_event += gcnew CanSimulator::rxData_slot(&CanDriver::canrx_simulator_device_event);
    CanSimulator::canrx_canopen_sdo_event += gcnew CanSimulator::rxData_slot(&CanDriver::canrx_simulator_canopen_sdo_event);
    CanSimulator::canrx_canopen_bootup_event += gcnew CanSimulator::rxData_slot(&CanDriver::canrx_simulator_canopen_bootup_event);
   
}

void CanDriver::startSimulatorMode(void) {
    can_connected = true;

    LogClass::logInFile("CAN DRIVER STARTED IN SIMULATION MODE");
}

void CanDriver::startNormalMode(void) {

    // Start the reception thread
    driver->running_thread = gcnew Thread(gcnew ThreadStart(driver, &CanDriver::threadWork));
    driver->running_thread->Name = "Can Driver Thread";
    driver->running_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
    driver->running_thread->Start();

}

/*
void CanDriver::threadWork(void) {
    driver_thread();
}*/

bool CanDriver::multithread_send(unsigned short canId, unsigned char* data, unsigned char len, bool simul) {
    
    // Lock the mutex to prevemt multi thread race access to the send function.
    const std::lock_guard<std::mutex> lock(send_mutex);
    if (simul)   return CanSimulator::send(canId, data, len);
   
    if (!can_connected) return false;

    VSCAN_MSG msg;
    DWORD written;

    if (len > 8) len = 8;

    msg.Flags = VSCAN_FLAGS_STANDARD;
    msg.Id = canId;
    msg.Size = len;

   
    // Upload the data buffer into the sending buffer
    for (unsigned char i = 0; i < msg.Size; i++) {
        if (i < len) msg.Data[i] = data[i];
        else data[i] = 0;
    }

    // Prova ad inviare il dati
    if (VSCAN_Write(handle, &msg, 1, &written) != VSCAN_ERR_OK) {
        LogClass::logInFile("Can Driver Failed to Send ");
        return false;
    }
    VSCAN_Flush(handle);
    
    return true;
}

//void driver_thread(void) {
void CanDriver::threadWork(void) {

    #define _CAN_1000K 0
    #define _CAN_800K  1
    #define _CAN_500K  2
    #define _CAN_250K  3
    #define _CAN_125K  4
    #define _CAN_100K  5
    #define _CAN_50K   6
    #define _CAN_20K   7
    #define VSCAN_NUM_MESSAGES 100

    VSCAN_API_VERSION   version;    //!< System Driver Api Version
    VSCAN_HWPARAM       hwparam;    //!< System Driver Hardware Version
    
    VSCAN_MSG           rxmsgs[VSCAN_NUM_MESSAGES]; //!< Array of the received Can Frames
    DWORD               rxmsg; //!< Number of received frames from CAN
    VSCAN_STATUS status;
    unsigned short      rxCanId;
    unsigned char       rxCanData[8];

    char cstring[33];
    bool loopback = false;
    unsigned char BR = _CAN_1000K;

    status = VSCAN_Ioctl(NULL, VSCAN_IOCTL_SET_DEBUG, VSCAN_DEBUG_NONE);
    for (int i = 0; i < 5; i++) {
        LogClass::logInFile("CAN OPEN ATTEMPT: " + System::Convert::ToString(i));
        //handle = VSCAN_Open(VSCAN_FIRST_FOUND, VSCAN_MODE_NORMAL);
        handle = VSCAN_Open("COM20", VSCAN_MODE_NORMAL);
        if (handle > 0) break;
    }

    if (handle <= 0) {
        error = true;
        ErrorString = "Can Driver Error: unable to open the driver.\n";
        VSCAN_GetErrorString((VSCAN_STATUS)handle, cstring, 32);
        ErrorString += "Driver error: "+ gcnew System::String(cstring) + "\n";
        LogClass::logInFile("CAN DRIVER: NORMAL MODE: " + System::Convert::ToString(cstring));
        return ;
    }

    LogClass::logInFile("DRIVER OPEN");


    // Get the APi release code
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_API_VERSION, &version);
    if (status != VSCAN_ERR_OK) {
        error = true;
        ErrorString = "Can Driver Error: unable to get the API revision.\n";
        VSCAN_GetErrorString(status, cstring, 32);
        ErrorString += "Driver error: " + gcnew System::String(cstring) + "\n";
        LogClass::logInFile("Get Api Version Command: " + System::Convert::ToString(cstring));
        return ;
    }

    printf("VSCAN API:  - %d.%d.%d", version.Major, version.Minor, version.SubMinor);
    LogClass::logInFile("VSCAN API: " + System::Convert::ToString(version.Major) + "." + System::Convert::ToString(version.Minor) + "." + System::Convert::ToString(version.SubMinor));

    // Get Hardware release code
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_HWPARAM, &hwparam);
    if (status != VSCAN_ERR_OK) {
        error = true;
        ErrorString = "Can Driver Error: unable to get the HW revision.\n";
        VSCAN_GetErrorString(status, cstring, 32);
        ErrorString += "Driver error: " + gcnew System::String(cstring) + "\n";
        LogClass::logInFile("Get Hw Param Command: " + System::Convert::ToString(cstring));
        return ;
    }

    apirev_maj = version.Major;
    apirev_min = version.Minor;
    apirev_sub = version.SubMinor;
    hwrev_maj = hwparam.HwVersion >> 4;
    hwrev_min = hwparam.HwVersion & 0x0F;
    swrev_maj = hwparam.SwVersion >> 4;
    swrev_min = hwparam.SwVersion & 0x0F;

   
    LogClass::logInFile("VSCAN HARDWARE:  SN=" + System::Convert::ToString(hwparam.SerialNr) +
        " HwREV=" + System::Convert::ToString(hwrev_maj) + "." + System::Convert::ToString(hwrev_min) +
        " SwREV=" + System::Convert::ToString(swrev_maj) + "." + System::Convert::ToString(swrev_min) +
        " TYPE=" + System::Convert::ToString(hwparam.HwType));

    // Set Baudrate
    void* br = VSCAN_SPEED_1M;
    string brstring = " 1Mbs";

    switch (BR) {
    case _CAN_1000K: br = VSCAN_SPEED_1M; brstring = " 1Mbs"; break;
    case _CAN_800K: br = VSCAN_SPEED_800K; brstring = " 800Kbs"; break;
    case _CAN_500K: br = VSCAN_SPEED_500K; brstring = " 500Kbs"; break;
    case _CAN_250K: br = VSCAN_SPEED_250K; brstring = " 250Kbs"; break;
    case _CAN_125K: br = VSCAN_SPEED_125K; brstring = " 125Kbs"; break;
    case _CAN_100K: br = VSCAN_SPEED_100K; brstring = " 100Kbs"; break;
    case _CAN_50K: br = VSCAN_SPEED_50K; brstring = " 50Kbs"; break;
    case _CAN_20K: br = VSCAN_SPEED_20K; brstring = " 20Kbs"; break;
    }

    // Set Baudrate
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_SPEED, br);
    if (status != VSCAN_ERR_OK) {
        error = true;
        ErrorString = "Can Driver Error: unable to set the Baud Rate.\n";
        VSCAN_GetErrorString(status, cstring, 32);
        ErrorString += "Driver error: " + gcnew System::String(cstring) + "\n";
        LogClass::logInFile("Set Baudrate Command: " + System::Convert::ToString(cstring));
        return ;
    }


    // Set the Filter to accept all frames
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_FILTER_MODE, VSCAN_FILTER_MODE_DUAL);
    if (status != VSCAN_ERR_OK) {
        error = true;
        ErrorString = "Can Driver Error: unable to set the Fiter mode.\n";
        VSCAN_GetErrorString(status, cstring, 32);
        ErrorString += "Driver error: " + gcnew System::String(cstring) + "\n";
        LogClass::logInFile("VSCAN_IOCTL_SET_FILTER_MODE Command: " + System::Convert::ToString(cstring));
        return ;
    }
    // receive all frames on the CAN bus (default)

    VSCAN_CODE_MASK codeMask;
    codeMask.Code = VSCAN_IOCTL_ACC_CODE_ALL;
    codeMask.Mask = VSCAN_IOCTL_ACC_MASK_ALL;
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_ACC_CODE_MASK, &codeMask);

    if (status != VSCAN_ERR_OK) {
        error = true;
        ErrorString = "Can Driver Error: unable to set the Acceptance filter Mask.\n";
        VSCAN_GetErrorString(status, cstring, 32);
        ErrorString += "Driver error: " + gcnew System::String(cstring) + "\n";
        LogClass::logInFile("Set Code And Mask Command: " + System::Convert::ToString(cstring));
        return ;
    }

    // clear all filters
    VSCAN_FILTER filter;
    filter.Size = 0;
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_FILTER, &filter);
    if (status != VSCAN_ERR_OK) {
        error = true;
        ErrorString = "Can Driver Error: unable to set the Acceptance filter Code.\n";
        VSCAN_GetErrorString(status, cstring, 32);
        ErrorString += "Driver error: " + gcnew System::String(cstring) + "\n";
        LogClass::logInFile("VSCAN_IOCTL_SET_FILTER Command: " + System::Convert::ToString(cstring));
        return ;
    }
    
    VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_BLOCKING_READ, VSCAN_IOCTL_ON);
    
    LogClass::logInFile("VSCAN DRIVER READY");
    can_connected = true;

    while (1) {
      
        // Blocking Read command
        VSCAN_Read(handle, rxmsgs, VSCAN_NUM_MESSAGES, &rxmsg); 
        if (!rxmsg) continue;
        
       
        for (int i = 0; i < (int)rxmsg; i++) {
            rxCanId = rxmsgs[i].Id;                
            for (int j = 0; j < rxmsgs[i].Size; j++) rxCanData[j] = rxmsgs[i].Data[j];
           

            if ((rxCanId >= 0x100) && (rxCanId <= 0x17F)) {                
                canrx_device_event(rxCanId, rxCanData, rxmsgs[i].Size);                
                continue;
            }

            if ((rxCanId >= 0x580) && (rxCanId <= 0x5FF)) {
                                
                canrx_canopen_sdo_event(rxCanId, rxCanData, rxmsgs[i].Size);                
                continue;
            }
            if ((rxCanId >= 0) && (rxCanId <= 0x7F)) {
                canrx_canopen_nmt_event(rxCanId, rxCanData, rxmsgs[i].Size);                
                continue;
            }
            if ((rxCanId >= 0x700) && (rxCanId <= 0x707)) {               
                canrx_canopen_bootup_event(rxCanId, rxCanData, rxmsgs[i].Size);                
                continue;
            }
            if (rxCanId == 0x80) {                
                canrx_canopen_sync_event(rxCanId, rxCanData, rxmsgs[i].Size);                
                continue;
            }                
            if ((rxCanId >= 0x81) && (rxCanId <= 0xFF)) {
                canrx_canopen_emrg_event(rxCanId, rxCanData, rxmsgs[i].Size);                
                continue;
            }
            if ((rxCanId >= 0x180) && (rxCanId <= 0x57F)) {
                canrx_canopen_pdo_event(rxCanId, rxCanData, rxmsgs[i].Size);                
                continue;
            }


        }
        


    }

}

void CanDriver::canrx_simulator_device_event(void) {
    unsigned char rxbuffer[8];

    for (int i = 0; i < (int)CanSimulator::datalen; i++) rxbuffer[i] = CanSimulator::canDataBuffer[i];
    canrx_device_event((unsigned short) CanSimulator::canId, rxbuffer, (unsigned char) CanSimulator::datalen);
};

void CanDriver::canrx_simulator_canopen_sdo_event(void) {
    unsigned char rxbuffer[8];

    for (int i = 0; i < (int)CanSimulator::datalen; i++) rxbuffer[i] = CanSimulator::canDataBuffer[i];
    canrx_canopen_sdo_event((unsigned short)CanSimulator::canId, rxbuffer, (unsigned char)CanSimulator::datalen);
};

void CanDriver::canrx_simulator_canopen_bootup_event(void) {
    unsigned char rxbuffer[8];

    for (int i = 0; i < (int)CanSimulator::datalen; i++) rxbuffer[i] = CanSimulator::canDataBuffer[i];
    canrx_canopen_bootup_event((unsigned short)CanSimulator::canId, rxbuffer, (unsigned char)CanSimulator::datalen);
};



