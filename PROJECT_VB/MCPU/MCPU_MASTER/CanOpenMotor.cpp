#include "pch.h"
#include "pd4_od.h"
#include <thread>


using namespace CANOPEN;

// Those defines SHALL be defined here!!!
#define MOTOR_GEAR 452.8

#define VMM_CTRL_OD OD_1F51_02
#define VMM_DATA_OD OD_1F50_02
#define VMM_STATUS_OD OD_1F57_02



/*
void CanOpenMotor::createWorkingDictionary(void) {
    DECLARE_NOINIT_REGISTER(OD_6041_00); // CiA 402 status word
    DECLARE_NOINIT_REGISTER(OD_6040_00); // CiA Control Word & Status change flags
    DECLARE_NOINIT_REGISTER(OD_6060_00); // OPERATING MODE CONROL WORD
    DECLARE_NOINIT_REGISTER(OD_6064_00); // Actual position encoder value (user units)
    DECLARE_NOINIT_REGISTER(OD_1001_00); // ERROR REGISTER
    DECLARE_NOINIT_REGISTER(OD_1003_01); // SPECIFIC ERROR REGISTER
    DECLARE_NOINIT_REGISTER(OD_1010_01); // Save All Parameters To Non-volatile Memory
    DECLARE_NOINIT_REGISTER(OD_2700_01); // User data control word: SET to 1 to save User content. reset autom. to 0 when finished
    DECLARE_NOINIT_REGISTER(OD_2700_02); // USER #0 - RESERVED FOR THE CONFIG VECTOR CONFIGURATION CODE
    DECLARE_NOINIT_REGISTER(OD_2700_03); // USER #1 - RESERVED FOR THE NANOJ CONFIGURATION CODE
    DECLARE_NOINIT_REGISTER(OD_2700_04); // USER #2 - RESERVED FOR RESET SENSING
    
}
*/

int CanOpenMotor::convert_cGRADsec_TO_ROT_min(int x) {
    return (int)((double) x * gear_ratio * (double) SPEED_DENOMINATOR / (double) 36000.);
}

int CanOpenMotor::convert_cGRAD_TO_EncoderUnit(int x) {
    return (int)((double) x * gear_ratio * (double) 10. / (double) 180.);
}

int CanOpenMotor::convert_EncoderUnit_TO_cGRAD(int x) {
    return (int)((double) x * (double) 180.0 / (gear_ratio * (double) 10.));
}

CanOpenMotor::CanOpenMotor(unsigned char devid, LPCWSTR motorname, double gear) {
    
    internal_status = status_options::MOTOR_NOT_CONNECTED;
    configuration_command = false;
    pNanoj = nullptr;
    nanojSize = 0;

    // Gets the handler of the class instance to be used for the Post/Send message functions.
    hwnd = static_cast<HWND>(Handle.ToPointer());    
    device_id = devid;
    gear_ratio = gear;

    error_condition = false;
    error_class = 0;
    error_code = 0;

    rxSDOEvent = CreateEvent(
        NULL,               // default security attributes
        FALSE,              // Automatic reset after signaling
        FALSE,              // initial state is nonsignaled
        motorname           // object name
    );

    // The OD shall be initialized
    od_initialized = false;
    nanoj_initialized = false;

    // Creates the receiving register
    rxSdoRegister = gcnew Register();
    sdo_rx_pending = false;
    nanoj_rx_pending = false;
    

    // Add the callback handling the SDO reception
    CanDriver::canrx_canopen_sdo_event += gcnew CanDriver::delegate_can_rx_frame(this, &CanOpenMotor::thread_canopen_rx_sdo_callback);

    // Add the bootup handler
    CanDriver::canrx_canopen_bootup_event += gcnew CanDriver::delegate_can_rx_frame(this, &CanOpenMotor::thread_canopen_bootup_callback);


    

    // Assignes the initial Cia status to undefined
    CiA_current_status = _CiA402Status::CiA402_Undefined;

  
    // Creates the Startup thread
    main_thread = gcnew Thread(gcnew ThreadStart(this, &CanOpenMotor::mainWorker));
    main_thread->Name = "Motor Worker" + Convert::ToString(device_id);
    main_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
    main_thread->Start();

}


void CanOpenMotor::thread_canopen_rx_sdo_callback(unsigned short canid, unsigned char* data, unsigned char len) {
    // Be careful!! This call is runing into the program thread pull, out of the class thread.
    
    // No data is expected
    if ((!sdo_rx_pending) && (!nanoj_rx_pending)) return;

    // Only 8-byte frame is accepted
    if(len < 8) return; // Invalid message length

    // Checks the canId address
    if (canid != device_id + 0x580) return; // This frame is not a SDO or is not addressed to this device!
   
    if (sdo_rx_pending) {
        sdo_rx_pending = false;
        rxSdoRegister->validateSdo(data);    
    }
    else {
        nanoj_rx_pending = false;
        if (rxNanojAck == data[0]) rxNanojAckValid = true;
        else rxNanojAckValid = false;
    }
    SetEvent(rxSDOEvent);
    
    
}

void CanOpenMotor::thread_canopen_bootup_callback(unsigned short canid, unsigned char* data, unsigned char len) {
    // Be careful!! This call is runing into the program thread pull, out of the class thread.

    // Checks the canId address
    if (canid != device_id + 0x700) return; // This frame is not a SDO or is not addressed to this device!

    od_initialized = false;
}

bool CanOpenMotor::blocking_writeOD(unsigned short index, unsigned char sub, unsigned char dim, int val) {
    unsigned char buffer[8];
    DWORD dwWaitResult;

    nanoj_rx_pending = false;

    // Repeats five times before fail
    for (int i = 0; i < 5; i++) {

        // Create the register to be received
        rxSdoRegister = gcnew Register(index, sub, dim, val);
        rxSdoRegister->cmd = Register::WRCMD;
        rxSdoRegister->getWriteBuffer(buffer);

        // Activates the transmission
        CanDriver::multithread_send(0x600 + device_id, buffer, 8);

        sdo_rx_pending = true;

        // Waits to be signalled: waits for 50ms
        dwWaitResult = WaitForSingleObject(rxSDOEvent, 1000);
        sdo_rx_pending = false;


        // Checks if the Event has been signalled or it is a timeout event.
        if (dwWaitResult != WAIT_OBJECT_0)
            continue;
        if (!rxSdoRegister->valid)
            continue;

        return true;
    }

    return false;
}



void CanOpenMotor::write_resetNode(void) {
    unsigned char buffer[8];

    memset(buffer, 0, 8);
    buffer[0] = 0x81;
    buffer[1] = device_id;

    // Activates the transmission
    CanDriver::multithread_send(0, buffer, 2);
}

bool CanOpenMotor::blocking_readOD(unsigned short index, unsigned char sub, unsigned char dim) {
    unsigned char buffer[8];
    DWORD dwWaitResult;

    nanoj_rx_pending = false;

    // Repeats five times before fail
    for (int i = 0; i < 5; i++) {

        // Create the register to be received
        rxSdoRegister = gcnew Register(index, sub, dim);
        rxSdoRegister->cmd = Register::RDCMD;
        rxSdoRegister->getReadBuffer(buffer);

        // Activates the transmission
        CanDriver::multithread_send(0x600 + device_id, buffer, 8);
        sdo_rx_pending = true;

        // Waits to be signalled: waits for 50ms
        dwWaitResult = WaitForSingleObject(rxSDOEvent, 1000);
        sdo_rx_pending = false;

        // Checks if the Event has been signalled or it is a timeout event.
        if (dwWaitResult != WAIT_OBJECT_0) {
            Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: TMO");
            continue;
        }

        if (!rxSdoRegister->valid) {
            Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: INVALID");
            continue;
        }

        return true;

    }
    
    return false;
}

bool CanOpenMotor::activateConfiguration(void) {
    // Executes the configuration but only in two status
    if ((internal_status == status_options::MOTOR_READY) || (internal_status == status_options::MOTOR_FAULT))
    {
        configuration_command = true;
        return true;
    }

    return false;
}

void CanOpenMotor::mainWorker(void) {
    

    Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: MAIN THREAD STARTED");

    while (1) {

        // Wait for the CAN DRIVER connection
        while (!CanDriver::isConnected()) {
            // Pause waiting for the connection
            internal_status = status_options::MOTOR_NOT_CONNECTED;
            std::this_thread::sleep_for(std::chrono::microseconds(500000));
            continue;
        }

        // Read the status word
        if (!blocking_readOD(OD_6041_00)) {
            internal_status = status_options::MOTOR_NOT_CONNECTED;
            continue;
        }

        // Activate  the configuration: both object dictionary registers and nanoj program (if present) shall be uploaded
        if (configuration_command) {            
            internal_status = status_options::MOTOR_CONFIGURATION;

            // If the nanoj program is not present (pNanoj = nullptr)  the nanoj_initialized flag  shall be set to true !
            if (pNanoj) nanoj_initialized = uploadNanojProgram();
            else nanoj_initialized = true;

            od_initialized = initializeObjectDictionary();
            configuration_command = false;
        }
        


        // Identifies what is the current ciA status of the motor 
        switch (getCiAStatus(rxSdoRegister->data)) {
        
        case _CiA402Status::CiA402_Undefined: internal_status = status_options::MOTOR_NOT_CONNECTED;  break;
        case _CiA402Status::CiA402_NotReadyToSwitchOn: internal_status = status_options::MOTOR_BUSY;  break; // Do nothing ... The motor handles internally this status
        case _CiA402Status::CiA402_FaultReactionActive: internal_status = status_options::MOTOR_FAULT; break; // Do nothing ... The motor handles internally this status
        
        case _CiA402Status::CiA402_QuickStopActive:internal_status = status_options::MOTOR_BUSY; CiA402_QuickStopActiveCallback(); break;

        case _CiA402Status::CiA402_SwitchOnDisabled: internal_status = status_options::MOTOR_BUSY; CiA402_SwitchOnDisabledCallback();  break;

        case _CiA402Status::CiA402_ReadyToSwitchOn: internal_status = status_options::MOTOR_BUSY; CiA402_ReadyToSwitchOnCallback();  break;

        case _CiA402Status::CiA402_SwitchedOn:  internal_status = status_options::MOTOR_READY; CiA402_SwitchedOnCallback();  break;
                
        case _CiA402Status::CiA402_OperationEnabled: internal_status = status_options::MOTOR_BUSY; break;
       
        case _CiA402Status::CiA402_Fault: CiA402_FaultCallback(); internal_status = status_options::MOTOR_FAULT; break;

        }
    }
                
}


void CanOpenMotor::CiA402_SwitchOnDisabledCallback(void) {
    unsigned int ctrlw;

    if (CiA_current_status != _CiA402Status::CiA402_SwitchOnDisabled) {
        CiA_current_status = _CiA402Status::CiA402_SwitchOnDisabled;
        Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: SWITCH-ON DISABLED STATUS");
    }

    // Read the Control Word
    if (!blocking_readOD(OD_6040_00)) return;
    
    // Set the shutdown value to run the status to Ready to Swithc On
    ctrlw = (unsigned int) rxSdoRegister->data;
    ctrlw &= ~OD_6040_00_SHUTDOWN_MASK;
    ctrlw |= OD_6040_00_SHUTDOWN_VAL;

    // Sets the Ready To Switch On status
    blocking_writeOD(OD_6040_00, ctrlw);
}

void CanOpenMotor::CiA402_QuickStopActiveCallback(void) {
    unsigned int ctrlw;

    if (CiA_current_status != _CiA402Status::CiA402_QuickStopActive) {
        CiA_current_status = _CiA402Status::CiA402_QuickStopActive;
        Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: QUICK STOP ACTIVE STATUS");
    }

    // Read the Control Word
    if (!blocking_readOD(OD_6040_00)) return;

    // Set the shutdown value to run the status to Ready to Swithc On
    ctrlw = (unsigned int)rxSdoRegister->data;
    ctrlw &= ~OD_6040_00_SHUTDOWN_MASK;
    ctrlw |= OD_6040_00_SHUTDOWN_VAL;

    // Sets the Ready To Switch On status
    blocking_writeOD(OD_6040_00, ctrlw);
    return;
}


void CanOpenMotor::CiA402_ReadyToSwitchOnCallback(void) {
    unsigned int ctrlw;

    if (CiA_current_status != _CiA402Status::CiA402_ReadyToSwitchOn) {
        CiA_current_status = _CiA402Status::CiA402_ReadyToSwitchOn;
        Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: READY TO SWITCH-ON STATUS");
    }

    // Read the Control Word
    if (!blocking_readOD(OD_6040_00)) return;

    // Set the shutdown value to run the status to Switched-On
    ctrlw = (unsigned int)rxSdoRegister->data;
    ctrlw &= ~OD_6040_00_SWITCHON_MASK;
    ctrlw |= OD_6040_00_SWITCHON_VAL;

    // Sets the Switched-On status
    blocking_writeOD(OD_6040_00, ctrlw);
    return;
}



void CanOpenMotor::CiA402_SwitchedOnCallback(void) {

    if (CiA_current_status != _CiA402Status::CiA402_SwitchedOn) {
        CiA_current_status = _CiA402Status::CiA402_SwitchedOn;
        Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: SWITCHED-ON STATUS");
        
    }

    
    return;
}

void CanOpenMotor::CiA402_OperationEnabledCallback(void) {

    if (CiA_current_status != _CiA402Status::CiA402_OperationEnabled) {
        CiA_current_status = _CiA402Status::CiA402_OperationEnabled;
        Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: OPERATION ENABLED STATUS");
    }


    return;
}

String^ CanOpenMotor::getErrorClass1001(unsigned int val) {
    unsigned char  cval = (unsigned char) val;
    String^ errstr = "";

    if (cval & OD_1001_00_GENERAL_ERROR) errstr += "[GENERAL]";
    if (cval & OD_1001_00_I_ERROR) errstr += "[I]";
    if (cval & OD_1001_00_VOLTAGE_ERROR) errstr += "[VOLTAGE]";
    if (cval & OD_1001_00_TEMP_ERROR) errstr += "[TEMP]";
    if (cval & OD_1001_00_COM_ERROR) errstr += "[COM]";
    if (cval & OD_1001_00_PROFILE_ERROR) errstr += "[PROFILE]";
    if (cval & OD_1001_00_RESERVED_ERROR) errstr += "[RESERVED]";
    if (cval & OD_1001_00_MANUFACT_ERROR) errstr += "[MANUFACTURER]";
    return errstr;

}

String^ CanOpenMotor::getErrorClass1003(unsigned int val) {
    return getErrorClass1001(val >> 16);
}

String^ CanOpenMotor::getErrorCode1003(unsigned int val) {
    unsigned short sval = (unsigned short ) (val & 0xFFFF);

    switch (sval) {
    case 0x1000: return "General error";
    case 0x2300: return "Current at the controller output too large";
    case 0x3100: return "Overvoltage/undervoltage at controller input";
    case 0x4200: return "Temperature error within the controller";
    case 0x6010: return "Software reset (watchdog)";
    case 0x6100: return "Internal software error, generic";
    case 0x6320: return "Rated current must be set (203Bh:01h)";
    case 0x7121: return "Motor blocked";
    case 0x7305: return "Incremental encoder or Hall sensor faulty";
    case 0x7600: return "Nonvolatile memory full or corrupt; restart the controller for cleanup work";
    case 0x8000: return "Error during fieldbus monitoring";
    case 0x8130: return "CANopen only: Life Guard error or Heartbeat error";
    case 0x8200: return "CANopen only: Slave took too long to send PDO messages.";
    case 0x8210: return "CANopen only: PDO was not processed due to a length error";
    case 0x8220: return "CANopen only: PDO length exceeded";
    case 0x8611: return "Position monitoring error: Following error too large";
    case 0x8612: return "Position monitoring error: Limit switch and tolerance zone exceeded";
    case 0x9000: return "EtherCAT: Motor running while EtherCAT changes from OP -> SafeOp, PreOP, etc.";
    }

    return "No Code identified";
}


void CanOpenMotor::CiA402_FaultCallback(void) {
    bool data_changed = false;
    unsigned int ctrlw;

    if (CiA_current_status != _CiA402Status::CiA402_Fault) {
        CiA_current_status = _CiA402Status::CiA402_Fault;
        Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: FAULT STATUS");
    }
    
    // Read the error class register
    if (!blocking_readOD(OD_1001_00)) return;
    if (error_class != rxSdoRegister->data) data_changed = true;
    error_class = rxSdoRegister->data;

    // Read the error code register
    if (!blocking_readOD(OD_1003_01)) return;
    if (error_code != rxSdoRegister->data) data_changed = true;
    error_code = rxSdoRegister->data;

    if (error_class) {
        if(!error_condition) data_changed = true;
        error_condition = true;

        if(data_changed){
            Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: ERROR CLASS = " + getErrorClass1001(error_class));
            Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: ERROR CODE = " + getErrorCode1003(error_code));
        }
        
        return;
    }

    // Tries to reset the error condition
    if (!blocking_readOD(OD_6040_00)) return;
    ctrlw = rxSdoRegister->data;
    ctrlw |= 0x80;
    if (!blocking_writeOD(OD_6040_00, ctrlw)) return;
    ctrlw &= ~0x80;
    if (!blocking_writeOD(OD_6040_00, ctrlw)) return;

    return;
}

bool CanOpenMotor::initNanojDataRegister(void) {
    unsigned char buffer[8];
    DWORD dwWaitResult;

    nanoj_rx_pending = false;

    // Special register with dimension set to 0
    rxSdoRegister = gcnew Register(VMM_DATA_OD, 0);
    rxSdoRegister->data_dim = 0;
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

bool CanOpenMotor::uploadNanojProgram(void) {

    // Calculate the nanoj-program checksum
    unsigned short vmmchk = 0;
    for (int i = 0; i < nanojSize; i++) vmmchk += (unsigned short) pNanoj[i];

    // Read the stored checksum to check if the program needs to be updated
    if(!blocking_readOD(NANOJ_USER_PARAM)) return false; // Reads the user parameter containing the stored nanoj checksum
    if (rxSdoRegister->data == vmmchk) {
        Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: NANOJ PROGRAM ALREADY PROGRAMMED");
        return true;
    }

    Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: NANOJ PROGRAMMING PROCESS");
    
    // WMM FLASH DELETE
    if (!blocking_writeOD(VMM_CTRL_OD, VMM_DELETE)) return false;
    
    // Reset node to let the WMM be cleared by the Engine
    write_resetNode();

    // Wait for the reset completed
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    while (!blocking_readOD(VMM_STATUS_OD)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: NODE RESET COMPLETED");
    
    // Read the VMM status  register
    if (!blocking_readOD(VMM_STATUS_OD)) return false;
    if (rxSdoRegister->data != 0) {
        Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: VMM FLASH IN ERROR CONDITION");
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
            Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: ERROR TRANSFERRING BLOCK ");
            return false;
        }

        Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: STORE BLOCK [" + index.ToString() + "]");
       

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
            Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: VMM FLASH IN ERROR CONDITION");
            return false;
        }

        if (block_size < 1024) break;
        index++;

    }

    // Store the checksum
    Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: STORE CHECKSUM");
    if (!blocking_writeOD(NANOJ_USER_PARAM, vmmchk)) return false;

    if (!blocking_writeOD(SAVE_USER_PARAM, 1)) return false;

    for (i = 30; i > 0; i--) {
        if (!blocking_readOD(SAVE_USER_PARAM)) return false;
        if (rxSdoRegister->data == 0) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (i == 0) return false;


    Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: NANOJ PROGRAM SUCCESSFULLY COMPLETED");
    return true;

}


bool CanOpenMotor::initializeObjectDictionary(void) {
    Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: INITIALIZATION");

    if(!blocking_writeOD(OD_4013_01,1)) return false; // Hardware configuration : 1 = EXTERNAL VCC LOGIC ON

    // NMT Behavior in case of fault
    if (!blocking_writeOD(OD_1029_01, 0)) return false;
    if (!blocking_writeOD(OD_1029_02, 1)) return false;
    if (!blocking_writeOD(OD_2031_00, 5000)) return false; // Peak current
    if (!blocking_writeOD(OD_2032_00, 5000)) return false; // Maximum Speed
    if (!blocking_writeOD(OD_2033_00, 0)) return false;	// Plunger Block
    if (!blocking_writeOD(OD_2034_00, 51500)) return false;// Upper Voltage Warning Level
    if (!blocking_writeOD(OD_2035_00, 21000)) return false;// ****************************************Lower Voltage Warning Level
    if (!blocking_writeOD(OD_2036_00, 2000)) return false; 	// Open Loop Current Reduction Idle Time
    if (!blocking_writeOD(OD_2037_00, -50)) return false;	// Open Loop Current Reduction Value/factor

    // I2t Parameters
    if (!blocking_writeOD(OD_203B_01, 5000)) return false;	// Nominal Current
    if (!blocking_writeOD(OD_203B_02, 1000)) return false;	// Maximum Duration Of Peak Current
    if (!blocking_writeOD(OD_203B_03, 0)) return false;	// Threshold
    if (!blocking_writeOD(OD_203B_04, 0)) return false;	// CalcValue
    if (!blocking_writeOD(OD_203B_05, 5000)) return false; 	// LimitedCurrent
    if (!blocking_writeOD(OD_2056_00, 500)) return false;	// Limit Switch Tolerance Band

    // user units
    if (!blocking_writeOD(OD_2061_00, 1)) return false;	// Velocity Numerator
    if (!blocking_writeOD(OD_2062_00, SPEED_DENOMINATOR)) return false;  // Velocity Denominator        ***********************
    if (!blocking_writeOD(OD_2063_00, 1)) return false;	// Acceleration Numerator
    if (!blocking_writeOD(OD_2064_00, SPEED_DENOMINATOR)) return false; 	// Acceleration Denominator    ***********************
    if (!blocking_writeOD(OD_2065_00, 1)) return false;	// Jerk Numerator
    if (!blocking_writeOD(OD_2066_00, 60)) return false;	// Jerk Denominator
    if (!blocking_writeOD(OD_3202_00, 9)) return false; 	// Motor Drive Submode Select: 6:BLDC 3:CurRed 2:Brake 1:VoS 0:CLOOP/OLOOP

    // Motor Drive Sensor Display Closed Loop
    if (!blocking_writeOD(OD_320B_01, 0)) return false;  // Commutation
    if (!blocking_writeOD(OD_320B_02, 0)) return false;	// Torque
    if (!blocking_writeOD(OD_320B_03, 1)) return false;	// Velocity
    if (!blocking_writeOD(OD_320B_04, 1)) return false; 	// Position

    // Motor Drive Parameter Set
    if (!blocking_writeOD(OD_3210_01, 50000)) return false; // Position Loop, Proportional Gain (closed Loop)
    if (!blocking_writeOD(OD_3210_02, 10)) return false;	 // Position Loop, Integral Gain (closed Loop)

    // Analogue Inputs Control
    if (!blocking_writeOD(OD_3221_00, 0)) return false;     // 0 , Voltage, 1, Current

    // Digital Input Capture
    //while(!blocking_writeOD(OD_3241_01,0 ) return false;    // Control (0:off, 1:RE, 2:FE, 3:RE+FE)


    // Following Error Option Code
    if (!blocking_writeOD(OD_3700_00, (unsigned short)-1)) return false; // -1 No reaction;0 Immediate stop; 1 Braking with "slow down ramp"; 2 Braking with "quick stop ramp"


    // Quick Stop Option Code
    if (!blocking_writeOD(OD_605A_00, 2)) return false;   // 0 Immediate stop  // 1 Braking with "slow down ramp"  // 2 Braking with "quick stop ramp"

    // Shutdown Option Code
    if (!blocking_writeOD(OD_605B_00, 0)) return false;   // 0 Immediate stop  // 1 Braking with "slow down ramp"

    // Disable Option Code
    if (!blocking_writeOD(OD_605C_00, 0)) return false;   // 0 Immediate stop  // 1 Braking with "slow down ramp"

    // Halt Option Code
    if (!blocking_writeOD(OD_605D_00, 0)) return false;  // 0 Immediate stop  // 1 Braking with "slow down ramp"  // 2 Braking with "quick stop ramp"

    // Fault Option Code
    if (!blocking_writeOD(OD_605E_00, 0)) return false;  // 0 Immediate stop  // 1 Braking with "slow down ramp"   // 2 Braking with "quick stop ramp"

    // Following Error Window and time
    if (!blocking_writeOD(OD_6065_00, 256)) return false; 	// Window
    if (!blocking_writeOD(OD_6066_00, 100)) return false;	// Time (ms)

    // Position Window + time
    if (!blocking_writeOD(OD_6067_00, 10)) return false;	// Window
    if (!blocking_writeOD(OD_6068_00, 100)) return false;	// Time


    // Position Range Limit
    if (!blocking_writeOD(OD_607B_01, 0)) return false; 	// Min Position Range Limit
    if (!blocking_writeOD(OD_607B_02, 0)) return false;	// Max Position Range Limit

    // Software Position Limit
    if (!blocking_writeOD(OD_607D_01, convert_cGRAD_TO_EncoderUnit(-2700))) return false;	// Min Position Limit
    if (!blocking_writeOD(OD_607D_02, convert_cGRAD_TO_EncoderUnit(2700))) return false;	// Max Position Limit

    // Polarity
    if (!blocking_writeOD(OD_607E_00, 0)) return false;	// b7:1-> inverse rotaion


    // Position Encoder Resolution: EncInc/MotRev
    if (!blocking_writeOD(OD_608F_01, 2000)) return false;	// Encoder Increments
    if (!blocking_writeOD(OD_608F_02, 1)) return false; 	// Motor Revolutions
    if (!blocking_writeOD(OD_60F2_00, 0x0002)) return false;// Absolute positionning

    // Gear Ratio
    if (!blocking_writeOD(OD_6091_01, 1)) return false; 	// Motor Revolutions
    if (!blocking_writeOD(OD_6091_02, 1)) return false;	// Shaft Revolutions

    // Max Absolute Acceleration and Deceleration
    if (!blocking_writeOD(OD_60C5_00, 5000)) return false;// Max Acceleration
    if (!blocking_writeOD(OD_60C6_00, 5000)) return false;// Max Deceleration

    // Homing registers
    if (!blocking_writeOD(OD_6098_00, 21)) return false;  // Homing method 21
    if (!blocking_writeOD(OD_607C_00, 0)) return false;
    
    // Read the Object dictionary flag initialization
    if (!blocking_readOD(CONFIG_USER_PARAM)) return false; // Reads the user parameter containing the stored nanoj checksum
    if (rxSdoRegister->data == 0x1A1B) {
        Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: INITIALIZED");
        return true;
    }
    
    Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: STORING DATA IN FLASH");
    
    // Save the Flag in the User Param register and store the User param space
    if (!blocking_writeOD(CONFIG_USER_PARAM, 0x1A1B)) return false;
    if (!blocking_writeOD(OD_2700_01, 1)) return false;

    // Save the whole object dictionary
    if (!blocking_writeOD(OD_1010_01, OD_SAVE_CODE)) return false;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    int i;
    for (i = 30; i > 0; i--) {
        if (!blocking_readOD(OD_1010_01)) return false;
        if (rxSdoRegister->data == 1) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (i == 0) return false;
    Debug::WriteLine("Motor Device <" + Convert::ToString(device_id) + ">: OBJECT DICTIONARY STORED IN FLASH");
    return true;
}



CanOpenMotor::_CiA402Status CanOpenMotor::getCiAStatus(int regval) {
    unsigned char val = (unsigned char)(regval & 0xFF);
    if ((val & 0x4F) == 0) return  _CiA402Status::CiA402_NotReadyToSwitchOn;
    else if ((val & 0x4F) == 0x40) return  _CiA402Status::CiA402_SwitchOnDisabled;
    else if ((val & 0x6F) == 0x21) return  _CiA402Status::CiA402_ReadyToSwitchOn;
    else if ((val & 0x6F) == 0x23) return  _CiA402Status::CiA402_SwitchedOn;
    else if ((val & 0x6F) == 0x27) return  _CiA402Status::CiA402_OperationEnabled;
    else if ((val & 0x6F) == 0x7) return  _CiA402Status::CiA402_QuickStopActive;
    else if ((val & 0x4F) == 0xF) return  _CiA402Status::CiA402_FaultReactionActive;
    else if ((val & 0x4F) == 0x8) return  _CiA402Status::CiA402_Fault;
    return _CiA402Status::CiA402_NotReadyToSwitchOn;
}

String^ CanOpenMotor::getCiAStatusString(_CiA402Status status) {
    switch (status) {
    case _CiA402Status::CiA402_NotReadyToSwitchOn: return "CiA402_NotReadyToSwitchOn";
    case _CiA402Status::CiA402_SwitchOnDisabled: return "CiA402_SwitchOnDisabled";
    case _CiA402Status::CiA402_ReadyToSwitchOn: return "CiA402_ReadyToSwitchOn";
    case _CiA402Status::CiA402_SwitchedOn: return "CiA402_SwitchedOn";
    case _CiA402Status::CiA402_OperationEnabled: return "CiA402_OperationEnabled";
    case _CiA402Status::CiA402_QuickStopActive: return "CiA402_QuickStopActive";
    case _CiA402Status::CiA402_FaultReactionActive: return "CiA402_FaultReactionActive";
    case _CiA402Status::CiA402_Fault: return "CiA402_Fault";
    }

    return "";
}