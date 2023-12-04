#include "CanDriver.h"
#include "CanOpenMotor.h"
#include "pd4_od.h"
#include <thread>


using namespace CANOPEN;

#define VMM_CTRL_OD OD_1F51_02
#define VMM_DATA_OD OD_1F50_02
#define VMM_STATUS_OD OD_1F57_02
#define DRIVER_POLLING_MS 100

/// <summary>
/// This function allows to convert the User units to Encoder units.
/// 
/// Whenever the application needs to write a Motor register with a position,
/// the motor register needs the Encoder value. 
/// This function allows to use application units instead of Encoder unit.
/// 
/// The application sets the application-unit in the Constructor function,
/// assigning the value of the (double) rot_per_unit parameter with the formula:
/// rot_per_unit = Rounds/units.
/// 
/// For example, if the Application should make use of 1 unit corresponding to be 0.01°,
/// and 1 unit should correspond to 10 rounds, the rot_per_unit = 10/1.
/// 
/// 
/// </summary>
/// <param name="x"></param>
/// <returns>The position in Encoder units (motor units)</returns>
int CanOpenMotor::convert_User_To_Encoder(int x) {
    return (int)((double) x * rot_per_unit * (double) 2000 );
}

/// <summary>
/// This function convert the Encoder units to Application (user)  units.
/// 
/// This function is the reverse of the convert_User_To_Encoder() function 
/// and it shall be used whenever the actual position should be read 
/// from the motor device.
/// 
/// </summary>
/// <param name="x"></param>
/// <returns>The position in application units (user units)</returns>
int CanOpenMotor::convert_Encoder_To_User(int x) {
    return (int)( (double) x / (rot_per_unit * (double) 2000) );
}

/// <summary>
/// This function converts a speed expressed in Application units/second 
/// to Motor units/second.
/// 
/// Always the speed or accelerations register shall be written, the units 
/// shall be expressed in motor units. This function is a convenient 
/// convertion function allowing to use more comprensible Application units.
///  
/// </summary>
/// <param name="x"></param>
/// <returns>The Speed or Acceleration ratio in motor units</returns>
int CanOpenMotor::convert_UserSec_To_Speed(int x) {
    return (int)((double)x * rot_per_unit * (double)SPEED_DENOMINATOR);
}

/// <summary>
/// This is the Class constructor.
/// 
/// </summary>
/// <param name="devid">This is the address of the Motor device</param>
/// <param name="motorname">This is a string of the Motor name</param>
/// <param name="rounds_for_units">This is the unit conversion rate</param>
CanOpenMotor::CanOpenMotor(unsigned char devid, LPCWSTR motorname, double rounds_for_units) {
    
    internal_status = status_options::MOTOR_NOT_CONNECTED;
    configuration_command = false;
    od_initialized = false;
    nanoj_initialized = false;
    pNanoj = nullptr;
    nanojSize = 0;
    home_initialized = false;
    encoder_initial_value = 0;
    
    
    // Gets the handler of the class instance to be used for the Post/Send message functions.
    //hwnd = static_cast<HWND>(Handle.ToPointer());    
    device_id = devid;
    rot_per_unit = rounds_for_units;

    error_condition = false;
    error_class = 0;
    error_code = 0;

    rxSDOEvent = CreateEvent(
        NULL,               // default security attributes
        FALSE,              // Automatic reset after signaling
        FALSE,              // initial state is nonsignaled
        motorname           // object name
    );

    
    // Creates the receiving register
    rxSdoRegister = gcnew ODRegister();
    sdo_rx_pending = false;
    nanoj_rx_pending = false;
    

    // Add the callback handling the SDO reception
    CanDriver::canrx_canopen_sdo_event += gcnew CanDriver::delegate_can_rx_frame(this, &CanOpenMotor::thread_canopen_rx_sdo_callback);

    // Add the bootup handler
    CanDriver::canrx_canopen_bootup_event += gcnew CanDriver::delegate_can_rx_frame(this, &CanOpenMotor::thread_canopen_bootup_callback);

    // Assignes the initial Cia status to undefined
    CiA_current_status = _CiA402Status::CiA402_Undefined;

    // Set the current command to IDLE
    current_command = MotorCommands::MOTOR_IDLE;
    request_command = MotorCommands::MOTOR_IDLE;

    // As default the target range is 0
    target_range_h = 0;
    target_range_l = 0;

    // Creates the Startup thread
    main_thread = gcnew Thread(gcnew ThreadStart(this, &CanOpenMotor::mainWorker));
    main_thread->Name = "Motor Worker" + System::Convert::ToString(device_id);
    main_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
    main_thread->Start();

}

/// <summary>
/// This is the reception callback assigned to the SDO register reception in the CanDriver module
/// 
/// The function verifies if the received address matches with the motor address 
/// in order to process the frame.
/// 
/// NOTE: Every motor device subclassing this module receives all the incomimng frames 
/// even if they are not addressed to the given motor.
/// 
/// </summary>
/// <param name="canid">This is the Can ID received</param>
/// <param name="data">This is the received buffer</param>
/// <param name="len">This is the lenght of the received buffer</param>
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

/// <summary>
/// This is the reception callback assigned to the CanOPEN Boot frames.
/// 
/// NOTE: Every motor device subclassing this module receives all the incomimng frames 
/// even if they are not addressed to the given motor.
/// 
/// </summary>
/// <param name="canid"></param>
/// <param name="data"></param>
/// <param name="len"></param>
void CanOpenMotor::thread_canopen_bootup_callback(unsigned short canid, unsigned char* data, unsigned char len) {
    // Be careful!! This call is runing into the program thread pull, out of the class thread.

    // Checks the canId address
    if (canid != device_id + 0x700) return; // This frame is not a SDO or is not addressed to this device!

    od_initialized = false;
    home_initialized = false;
}

/// <summary>
/// This function writes a SDO register in the target motor device;
/// 
/// The function sends to the CAN Bus the command to write the target register,
/// then waits the answer. In case of no answer, tries 5 times before to fail.
/// 
/// </summary>
/// <param name="index">Index of the register</param>
/// <param name="sub">Sub index of the register</param>
/// <param name="dim">Dimension code of the data register</param>
/// <param name="val">Value to be written into register</param>
/// <returns>true in case of success</returns>
bool CanOpenMotor::blocking_writeOD(unsigned short index, unsigned char sub, ODRegister::SDODataDimension dim, int val) {
    unsigned char buffer[8];
    DWORD dwWaitResult;

    nanoj_rx_pending = false;

    // Repeats five times before fail
    for (int i = 0; i < 5; i++) {

        // Create the register to be received
        rxSdoRegister = gcnew ODRegister(index, sub, dim, val);
        rxSdoRegister->cmd = ODRegister::SDOCommandCodes::WRCMD;
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

/// <summary>
/// This function Reads a SDO register in the target motor device;
/// 
/// The function sends to the CAN Bus the command to read the target register,
/// then waits the answer. In case of no answer, tries 5 times before to fail.
/// 
/// </summary>
/// <param name="index">Index of the register</param>
/// <param name="sub">Sub index of the register</param>
/// <param name="dim">Dimension code of the data register</param>
/// <returns>true in case of success</returns>
bool CanOpenMotor::blocking_readOD(unsigned short index, unsigned char sub, ODRegister::SDODataDimension dim) {
    unsigned char buffer[8];
    DWORD dwWaitResult;

    nanoj_rx_pending = false;

    // Repeats five times before fail
    for (int i = 0; i < 5; i++) {

        // Create the register to be received
        rxSdoRegister = gcnew ODRegister(index, sub, dim);
        rxSdoRegister->cmd = ODRegister::SDOCommandCodes::RDCMD;
        rxSdoRegister->getReadBuffer(buffer);

        // Activates the transmission
        CanDriver::multithread_send(0x600 + device_id, buffer, 8);
        sdo_rx_pending = true;

        // Waits to be signalled: waits for 50ms
        dwWaitResult = WaitForSingleObject(rxSDOEvent, 1000);
        sdo_rx_pending = false;

        // Checks if the Event has been signalled or it is a timeout event.
        if (dwWaitResult != WAIT_OBJECT_0) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: TMO");
            continue;
        }

        if (!rxSdoRegister->valid) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: INVALID");
            continue;
        }

        return true;

    }

    return false;
}

/// <summary>
/// This function sends a Reset Node command to the device.
/// 
/// NOTE: after the reset, the Motor device not responds to the command
/// and takes seconds before to be ready for a communication!
/// 
/// </summary>
/// <param name=""></param>
void CanOpenMotor::write_resetNode(void) {
    unsigned char buffer[8];

    memset(buffer, 0, 8);
    buffer[0] = 0x81;
    buffer[1] = device_id;

    // Activates the transmission
    CanDriver::multithread_send(0, buffer, 2);
}


/// <summary>
/// This function writes a part of the control word.
/// 
/// The function uses a masj to filter what are the bits to be written with the value;
/// 
/// </summary>
/// <param name="mask">Select the bit of the control word to be modified</param>
/// <param name="val">Set the value in the selected bits of the control word</param>
/// <returns>true if success</returns>
bool CanOpenMotor::writeControlWord(unsigned int mask, unsigned int val) {
    unsigned int ctrlw;
    if (!blocking_readOD(OD_6040_00)) return false;
    ctrlw = (unsigned int)rxSdoRegister->data;
    ctrlw &= ~mask;
    ctrlw |= val;
    if (!blocking_writeOD(OD_6040_00, ctrlw)) return false;
    return true;
}

/// <summary>
/// This function activate motor configuration fase.
/// 
/// During the motor configuration fase, the device Object registers 
/// are written with the wanted values (see initializeObjectDictionary() and initializeSpecificObjectDictionaryCallback());
/// 
/// If the implementation needs the Nanoj program, the applicaiton program is uploaded into the device.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CanOpenMotor::activateConfiguration(void) {
    // Executes the configuration but only in two status
    if ((internal_status == status_options::MOTOR_READY) || (internal_status == status_options::MOTOR_FAULT))
    {
        configuration_command = true;
        return true;
    }

    return false;
}


/// <summary>
/// This function is internally used to decode the content of the Status register 
/// and returning the current CiA status code.
/// 
/// </summary>
/// <param name="regval">The content of the Status register</param>
/// <returns>The detected CiA status code</returns>
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

/// <summary>
/// This convenient function return a String name of the curent CiA status
/// </summary>
/// <param name="status">the CiA status code</param>
/// <returns>The String name of the CiA status code</returns>
System::String^ CanOpenMotor::getCiAStatusString(_CiA402Status status) {
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


/// <summary>
/// This is the Main thread managing the communication with the motor.
/// 
/// The threads:
/// - handles the CiA status machine;
/// - handles the execution of the commands (only in SwitchedOn status)
/// - handles the motor exceptions.
/// - handles the motor configuration.
/// 
/// The thread force the system to selected the SwitchedOn status if possible.
/// When in SwitchedOn status, the activation command can be performed.
/// 
/// If the Motor should enter a Fault mode, the thread handles the fault condition 
/// and as soon as the fault condition expires tries to report the current status to the SwitchedOn status.
/// 
/// The thread starts the device configuration in the following scenario:
/// - Soon as possible after the application startup;
/// - In case the device should reset (Boot-Up frame received);
/// 
/// 
/// </summary>
/// <param name=""></param>
void CanOpenMotor::mainWorker(void) {
    

    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: MAIN THREAD STARTED");

    while (1) {
        
        // If a command execution is present the command shall be aborted!
        if (current_command != MotorCommands::MOTOR_IDLE) setCommandCompletedCode(MotorCompletedCodes::ERROR_UNEXPECTED_STATUS);

        // Wait for the CAN DRIVER connection
        while (!CanDriver::isConnected()) {
            // Pause waiting for the connection
            internal_status = status_options::MOTOR_NOT_CONNECTED;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
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

            // Assignes the initial value to the encoder
            for (int i = 0; i < 5; i++) { if (initResetEncoderCommand(encoder_initial_value)) break; }
            
            previous_uposition = current_uposition;
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
                
        case _CiA402Status::CiA402_OperationEnabled: internal_status = status_options::MOTOR_BUSY;  CiA402_OperationEnabledCallback(); break;
       
        case _CiA402Status::CiA402_Fault: internal_status = status_options::MOTOR_FAULT;  CiA402_FaultCallback();  break;

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(DRIVER_POLLING_MS));

    }
                
}

/// <summary>
/// This function handles the SwitchOnDisabled CiA status.
/// 
/// The module tries to enter the ReadyToSwitchOn from this status
/// </summary>
/// <param name=""></param>
void CanOpenMotor::CiA402_SwitchOnDisabledCallback(void) {
    if (CiA_current_status != _CiA402Status::CiA402_SwitchOnDisabled) {
        CiA_current_status = _CiA402Status::CiA402_SwitchOnDisabled;
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: SWITCH-ON DISABLED STATUS");
    }

    // Try to enter the CiA402_ReadyToSwitchOn
    if (!writeControlWord(OD_6040_00_SHUTDOWN_MASK,OD_6040_00_SHUTDOWN_VAL)) return;
}

/// <summary>
/// This function is called when the CiA402_QuickStopActive status is detected.
/// 
/// The module tries to enter the ReadyToSwitchOn from this status
/// </summary>
/// <param name=""></param>
void CanOpenMotor::CiA402_QuickStopActiveCallback(void) {

    if (CiA_current_status != _CiA402Status::CiA402_QuickStopActive) {
        CiA_current_status = _CiA402Status::CiA402_QuickStopActive;
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: QUICK STOP ACTIVE STATUS");
    }

    // Try to enter the CiA402_ReadyToSwitchOn
    if (!writeControlWord(OD_6040_00_SHUTDOWN_MASK, OD_6040_00_SHUTDOWN_VAL)) return;

    return;
}

/// <summary>
/// This function is called when the CiA402_ReadyToSwitchOn status is detected. 
/// 
/// The module tries to enter the CiA402_SwitchedOn from this status
/// </summary>
/// <param name=""></param>
void CanOpenMotor::CiA402_ReadyToSwitchOnCallback(void) {

    if (CiA_current_status != _CiA402Status::CiA402_ReadyToSwitchOn) {
        CiA_current_status = _CiA402Status::CiA402_ReadyToSwitchOn;
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: READY TO SWITCH-ON STATUS");
    }

    // Try to enter the CiA402_SwitchedOn
    if (!writeControlWord(OD_6040_00_SWITCHON_MASK, OD_6040_00_SWITCHON_VAL)) return;

    return;
}


/// <summary>
/// This function is called when the CiA402_SwitchedOn status is detected.  
/// 
/// This is the Operating status machine. The activation commands can be performed in this status.
/// </summary>
/// <param name=""></param>
void CanOpenMotor::CiA402_SwitchedOnCallback(void) {

    if (CiA_current_status != _CiA402Status::CiA402_SwitchedOn) {
        CiA_current_status = _CiA402Status::CiA402_SwitchedOn;
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: SWITCHED-ON STATUS");        

        
    }

    // The subclass can add extra commands in IDLE and enable the command execution
    if (!idleCallback()) {
        if (request_command != MotorCommands::MOTOR_IDLE) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR DUE TO SUBCLASS IDLE CONDITIONS");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_INVALID_COMMAND_INITIAL_CONDITION);
        }
        return;
    }

    // Initiate a requested command
    switch (request_command) {
    case MotorCommands::MOTOR_AUTO_POSITIONING:
        current_command = request_command;
        manageAutomaticPositioning();        
        request_command = MotorCommands::MOTOR_IDLE;
        break;

    case MotorCommands::MOTOR_MANUAL_POSITIONING:
        current_command = request_command;
        manageManualPositioning();
        request_command = MotorCommands::MOTOR_IDLE;
        break;

    case MotorCommands::MOTOR_HOMING:
        current_command = request_command;
        manageAutomaticHoming();
        request_command = MotorCommands::MOTOR_IDLE;
        break;

    case MotorCommands::MOTOR_IDLE:
        
        break;

    }
        
    return;
}

/// <summary>
/// This function is called when the CiA402_OperationEnabled status is detected.
/// 
/// The Application cannot remains in this status out of a command execution 
/// because the motor wires are powered and a torque is applied.
/// 
/// The module tries to enter the CiA402_SwitchedOn from this status
/// 
/// </summary>
/// <param name=""></param>
void CanOpenMotor::CiA402_OperationEnabledCallback(void) {
    
    if (CiA_current_status != _CiA402Status::CiA402_OperationEnabled) {
        CiA_current_status = _CiA402Status::CiA402_OperationEnabled;
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: OPERATION ENABLED STATUS");
    }

    // The Motor cannot remain in this state without a command execution
    writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);
    return;
}


/// <summary>
/// This is a convenient function to decode a Error string from the error 
/// of the register 1001.
/// 
/// </summary>
/// <param name="val">Register 1001 error code</param>
/// <returns></returns>
System::String^ CanOpenMotor::getErrorClass1001(unsigned int val) {
    unsigned char  cval = (unsigned char) val;
    System::String^ errstr = "";

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

/// <summary>
/// This is a convenient function to decode a Error Class string from the register 1003.
/// 
/// </summary>
/// <param name="val"></param>
/// <returns></returns>
System::String^ CanOpenMotor::getErrorClass1003(unsigned int val) {
    return getErrorClass1001(val >> 16);
}

/// <summary>
/// This is a convenient function to decode a Error Code string from the register 1003.
/// 
/// </summary>
/// <param name="val"></param>
/// <returns></returns>
System::String^ CanOpenMotor::getErrorCode1003(unsigned int val) {
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

/// <summary>
/// This function handles the CiA_Fault status
/// 
/// The motor device enters a fault status whenever an intrnal fault condition is detected.
/// 
/// This module read the error code from the device and, 
/// as soon as the error condition expires, try to exit from the CiA_Fault status 
/// to the CiA_SwitchedOn status (operating status).
///
/// When a new error is generated or when the error condition expires, 
/// the module generate the event: fault_event(error_code)
/// 
///  
/// </summary>
/// <param name=""></param>
void CanOpenMotor::CiA402_FaultCallback(void) {
    bool data_changed = false;
    unsigned int ctrlw;

    if (CiA_current_status != _CiA402Status::CiA402_Fault) {
        CiA_current_status = _CiA402Status::CiA402_Fault;
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: FAULT STATUS");
    }

    // If a command execution is present the command shall be aborted!
    if (current_command != MotorCommands::MOTOR_IDLE) setCommandCompletedCode(MotorCompletedCodes::ERROR_INTERNAL_FAULT);
    
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
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR CLASS = " + getErrorClass1001(error_class));
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR CODE = " + getErrorCode1003(error_code));
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
    for (int i = 0; i < nanojSize; i++) vmmchk += (unsigned short) pNanoj[i];

    // Read the stored checksum to check if the program needs to be updated
    if(!blocking_readOD(NANOJ_USER_PARAM)) return false; // Reads the user parameter containing the stored nanoj checksum
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
/// This is the function initializing the Motor register with common default values.
/// 
/// This function is called during the initialization fase in order to 
/// set the internal register with default values, widely used for PD4 motors.
/// 
/// In case a subclassed module should personalize the default values, the module should 
/// override the initializeSpecificObjectDictionaryCallback() in order to set those registers 
/// to be changed.
/// 
/// </summary>
/// <param name=""></param>
/// <returns>true in case of success</returns>
bool CanOpenMotor::initializeObjectDictionary(void) {
    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: INITIALIZATION");

    if(!blocking_writeOD(OD_4013_01,1)) return false; // Hardware configuration : 1 = EXTERNAL VCC LOGIC ON

    // NMT Behavior in case of fault
    if (!blocking_writeOD(OD_1029_01, 0)) return false;
    if (!blocking_writeOD(OD_1029_02, 1)) return false;
    if (!blocking_writeOD(OD_2031_00, 5000)) return false;  // Peak current
    if (!blocking_writeOD(OD_2032_00, 5000)) return false;  // Maximum Speed
    if (!blocking_writeOD(OD_2033_00, 0)) return false;	    // Plunger Block
    if (!blocking_writeOD(OD_2034_00, 51500)) return false; // Upper Voltage Warning Level
    if (!blocking_writeOD(OD_2035_00, 21000)) return false; // ****************************************Lower Voltage Warning Level
    if (!blocking_writeOD(OD_2036_00, 2000)) return false; 	// Open Loop Current Reduction Idle Time
    if (!blocking_writeOD(OD_2037_00, -50)) return false;	// Open Loop Current Reduction Value/factor

    // I2t Parameters
    if (!blocking_writeOD(OD_203B_01, 5000)) return false;	// Nominal Current
    if (!blocking_writeOD(OD_203B_02, 1000)) return false;	// Maximum Duration Of Peak Current
    if (!blocking_writeOD(OD_203B_03, 0)) return false;	    // Threshold
    if (!blocking_writeOD(OD_203B_04, 0)) return false;	    // CalcValue
    if (!blocking_writeOD(OD_203B_05, 5000)) return false; 	// LimitedCurrent
    if (!blocking_writeOD(OD_2056_00, 500)) return false;	// Limit Switch Tolerance Band

    // user units
    if (!blocking_writeOD(OD_2061_00, 1)) return false;	// Velocity Numerator
    if (!blocking_writeOD(OD_2062_00, SPEED_DENOMINATOR)) return false;  // Velocity Denominator        ***********************
    if (!blocking_writeOD(OD_2063_00, 1)) return false;	// Acceleration Numerator
    if (!blocking_writeOD(OD_2064_00, SPEED_DENOMINATOR)) return false; 	// Acceleration Denominator    ***********************
    if (!blocking_writeOD(OD_2065_00, 1)) return false;	// Jerk Numerator
    if (!blocking_writeOD(OD_2066_00, 60)) return false;	// Jerk Denominator
    if (!blocking_writeOD(OD_3202_00, 9)) return false; 	// Motor Drive Submode Select: 6:BLDC 3:CurRed 2:Brake 1:VoS 0: 1=CLOSED_LOOP/O = OPEN_LOOP

    // Motor Drive Sensor Display Closed Loop
    if (!blocking_writeOD(OD_320B_01, 0)) return false;     // Commutation
    if (!blocking_writeOD(OD_320B_02, 0)) return false;	    // Torque
    if (!blocking_writeOD(OD_320B_03, 1)) return false;	    // Velocity
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
    if (!blocking_writeOD(OD_607D_01, convert_User_To_Encoder(-18000))) return false;	// Min Position Limit
    if (!blocking_writeOD(OD_607D_02, convert_User_To_Encoder(18000))) return false;	// Max Position Limit

    // Polarity
    if (!blocking_writeOD(OD_607E_00, 0)) return false;	// b7:1-> inverse rotaion

    // Position Encoder Resolution: EncInc/MotRev
    if (!blocking_writeOD(OD_608F_01, 2000)) return false;	// Encoder Increments
    if (!blocking_writeOD(OD_608F_02, 1)) return false; 	// Motor Revolutions
    if (!blocking_writeOD(OD_60F2_00, 0x0002)) return false;// Absolute positionning

    // Gear Ratio
    if (!blocking_writeOD(OD_6091_01, 1)) return false; 	// Motor Revolutions
    if (!blocking_writeOD(OD_6091_02, 1)) return false;	    // Shaft Revolutions

    // Max Absolute Acceleration and Deceleration
    if (!blocking_writeOD(OD_60C5_00, 5000)) return false;  // Max Acceleration
    if (!blocking_writeOD(OD_60C6_00, 5000)) return false;  // Max Deceleration

    // Homing registers
    if (!blocking_writeOD(OD_6098_00, 21)) return false;  // Homing method 21
    if (!blocking_writeOD(OD_607C_00, 0)) return false;
    
    // Specific register set for the Subclassed motor
    if (!initializeSpecificObjectDictionaryCallback()) return false;

    // Read the Object dictionary flag initialization
    if (!blocking_readOD(CONFIG_USER_PARAM)) return false; // Reads the user parameter containing the stored nanoj checksum
    if (rxSdoRegister->data == 0x1A1B) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: INITIALIZED");
        return true;
    }
    
    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: STORING DATA IN FLASH");
    
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
    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: OBJECT DICTIONARY STORED IN FLASH");
    return true;
}

bool CanOpenMotor::activateAutomaticHoming(int method_on, int method_off, int speed, int acc) {

    // Command already in execution
    if (!isReady()) {
        command_completed_code = MotorCompletedCodes::ERROR_MOTOR_BUSY;
        return false;
    }

    command_homing_on_method = method_on;
    command_homing_off_method = method_off;
    command_acc = acc;
    command_speed = speed;
    request_command = MotorCommands::MOTOR_HOMING;

    return true;
}


/// <summary>
/// This is the function to activate an Automatic positioning.
/// 
/// This function can be used by the application in order to request 
/// the execution of a positioning.
/// 
/// The function return immediatelly if the motor shouldn't in the right status 
/// to execute a command (busy or not homing).
/// 
/// When a command should be accepted the Application shall wait for the 
/// command_completed_event() callback to detect the command completion.
/// 
/// In case the command should be an Isocentric command (iso == true) 
/// the internal iso_position register shall not be modified.
/// The Isocentric mode is used for those motor activation that needs to be syncronized
/// to keep a stable isocentric position. 
/// 
/// NOTE: the command will start as soon as the module main thread fetch the command request.
/// 
/// </summary>
/// <param name="id">This is the ID code assigned by the applicaiton</param>
/// <param name="target">This is the target position in Application units</param>
/// <param name="speed">This is the speed in the Application units</param>
/// <param name="acc">This is the Acceleration rate in Application units</param>
/// <param name="dec">This is the Deceleration rate in Application units</param>
/// <returns>true if the command can be executed</returns>
bool CanOpenMotor::activateAutomaticPositioning(int id, int target, int speed, int acc, int dec) {
    
    // Only withthe homing executed or initialized can be activated
    if (!home_initialized) {
        command_completed_code = MotorCompletedCodes::ERROR_MISSING_HOME;
        return false;
    }

    // Command already in execution
    if (!isReady()) {
        command_completed_code = MotorCompletedCodes::ERROR_MOTOR_BUSY;
        return false;
    }

    command_id= id;
    command_target = target;
    command_acc = acc;
    command_dec = dec;
    command_speed = speed;
    request_command = MotorCommands::MOTOR_AUTO_POSITIONING;
    return true;
}


/// <summary>
/// This function updates the current position.
/// 
/// The current position is stored in both units:
/// - current_eposition: is the position espressed in Encoder units;
/// - current_uposition: is the position espressed in User (application)  units;
/// 
/// </summary>
/// <param name=""></param>
void CanOpenMotor::updateCurrentPosition(void) {
    if (!blocking_readOD(OD_6064_00)) return ;
    current_eposition = rxSdoRegister->data;
    current_uposition = convert_Encoder_To_User(current_eposition);
}

/// <summary>
/// This function sets aan activation timeout based on the activation parameters.
/// 
/// The Activation algorithm control the speed with a trapezioidal mode:
/// - A constant acceleration until a target speed is reached;
/// - A constant speed until a deceleration point is reached;
/// - A constant deceleration until a target is reached;
/// 
/// The timeout is calculated based on this operating mode
/// 
/// 
/// </summary>
/// <param name="speed">The activation speed in User units</param>
/// <param name="acc">The activation Acc in user units</param>
/// <param name="dec">The activation Dec in user units</param>
/// <param name="target">The target position in user units</param>
void CanOpenMotor::setActivationTimeout(int speed, int acc, int dec, int target) {
    double T;

    // The Acceleration rate is calculated with a weighted means of the Acc and Dec ratio;
    double a = (dec + acc) / 2;

    // The total travel space is calculated;
    double s = abs((double)current_uposition - (double)target);

     
    if (s < 2 * speed * speed / a) {
        // If the total travel is less than the sum of the accelerations and decelerations
        T = 2* sqrt(s / a);
    }
    else {
        // The travel time is the sum of acceleration and deceleration time and the remaining time at the constant speed
        T = (2 * speed / a) + ((s - (2 * speed * speed / a)) / speed);
    }

    // The final timeout is aumented of 50% of the calculated travel time in milliseconds
    command_ms_tmo = (int)(T * 1000 * 1.5);

}

/// <summary>
/// This function is internally used to set a command completion code.
/// 
/// </summary>
/// <param name=LABEL_ERROR></param>

void CanOpenMotor::setCommandCompletedCode(MotorCompletedCodes error) {
    command_completed_code = error;
    MotorCommands cmd = current_command;
    current_command = MotorCommands::MOTOR_IDLE;

    if (cmd == MotorCommands::MOTOR_AUTO_POSITIONING) automaticPositioningCompletedCallback(error);
    else if(cmd == MotorCommands::MOTOR_HOMING) automaticHomingCompletedCallback(error);
    else if (cmd == MotorCommands::MOTOR_MANUAL_POSITIONING) manualPositioningCompletedCallback(error);

    return;
}

#define ZERO_INPUT_MASK(x) (x & 0x00040000) //!< Not in the Special region [00II][0000]
void CanOpenMotor::manageAutomaticHoming(void) {
    bool error_condition;
    home_initialized = false;

   

    // Sets the Speed activation
    error_condition = false;

    // Read the inputs before to proceed
    bool current_homing_input = false;
    if (blocking_readOD(OD_60FD_00)) {
        if (ZERO_INPUT_MASK(rxSdoRegister->data)) current_homing_input = true;
    }

    if (!blocking_writeOD(OD_6099_01, convert_UserSec_To_Speed(command_speed))) error_condition = true; // Homing Speed to switch
    if (!blocking_writeOD(OD_6099_02, convert_UserSec_To_Speed(command_speed))) error_condition = true; // Homing Speed to reference
    if (!blocking_writeOD(OD_609A_00, convert_UserSec_To_Speed(command_acc))) error_condition = true; // Sets the Rotation Acc    

    // Read the status of the input zero to determines witch algorithm shall be used
    blocking_readOD(OD_60FD_00);
    unsigned char homing;
    if (ZERO_INPUT_MASK(rxSdoRegister->data)) homing = command_homing_on_method;
    else homing = command_homing_off_method;
    if (!blocking_writeOD(OD_6098_00, homing)) error_condition = true; // Write the Homing method
    
    if (!blocking_writeOD(OD_6060_00, OD_6060_00_PROFILE_HOMING)) error_condition = true; // Write the operating mode to HOMING   
    if (!blocking_writeOD(OD_607C_00, 0)) error_condition = true; // Write the Position offset register 


    if (error_condition) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN AUTO HOMING PREPARATION");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }


    // Tries to activate the Operation Enabled 
    error_condition = true;
    for (int i = 0; i < 5; i++) {

        if (!writeControlWord(OD_6040_00_ENABLEOP_MASK, OD_6040_00_ENABLEOP_VAL)) continue;

        // Read the status word
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (!blocking_readOD(OD_6041_00)) continue;
        if (getCiAStatus(rxSdoRegister->data) == _CiA402Status::CiA402_OperationEnabled) {
            error_condition = false;
            break;
        }

    }

    if (error_condition) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN AUTO HOMING OPERATION ENABLE SETTING");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }


    // Allows the application to prepare for the motor activation
    MotorCompletedCodes preparation_error = automaticHomingPreparationCallback();
    if(preparation_error != MotorCompletedCodes::COMMAND_PROCEED){
        setCommandCompletedCode(preparation_error);
        return;
    }

    // Update the previous position
    previous_uposition = current_uposition;
    
    // Set the start bit (BIT4) in the control word register
    if (!writeControlWord(0x10, 0x10)) {        
        setCommandCompletedCode(MotorCompletedCodes::ERROR_ACTIVATION_REGISTER);
        return;
    }

    // Measuring the actuation time
    typedef std::chrono::high_resolution_clock clock;
    clock::time_point start = clock::now();

    command_ms_tmo = 60000; // Sets the timout to 60 seconds for all devices

    while (true) {

        // Reads the status to be sure that it is still Operation Enabled 
        if (!blocking_readOD(OD_6041_00)) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING ERROR READING STATUS REGISTER");            
            setCommandCompletedCode(MotorCompletedCodes::ERROR_ACTIVATION_REGISTER);
            break;
        }

        unsigned short statw = rxSdoRegister->data;

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING ERROR POSSIBLE FAULT");            
            setCommandCompletedCode(MotorCompletedCodes::ERROR_INTERNAL_FAULT);
            break;
        }

        // Test the Timeout condition
        if (command_ms_tmo <= 0)  {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING TIMEOUT ERROR");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_TIMOUT);
            break;
        }

        // The Application can early terminate the activation
        MotorCompletedCodes termination_condition = automaticHomingRunningCallback();
        if (termination_condition >= MotorCompletedCodes::MOTOR_ERRORS) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: Application terminated early for error detected");
            setCommandCompletedCode(termination_condition);
            break;
        }

        unsigned short zero_stat = statw & 0x3400;

        bool completed = false;
        bool error = false;

        switch (zero_stat) {

        case 0x1000: // CONFIRMED
        case 0: // PERFORMED
            break;

        case 0x2400: // ERROR IDLE
        case 0x2000: // ERROR
        case 0x0400: // INTERRUPTED
            error = true;
            break;

        
        case 0x1400: // COMPLETED
            completed = true;
            break;


        }

        if (completed) {

            // Calculates the effective duration time
            typedef std::chrono::milliseconds milliseconds;
            milliseconds ms = std::chrono::duration_cast<milliseconds>(clock::now() - start);
            home_initialized = true;
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING SUCCESSFULLY COMPLETED");
            
            setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);

            // resets the OMS bit of the control word
            writeControlWord(0x0270, 0);

            // set the cia Switched On status
            writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);

            break;
        }

        if (error) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC HOMING ERROR  FAULT");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_INTERNAL_FAULT);
            break;

        }

        command_ms_tmo -= 50;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    } // End of main controlling loop

    // Read the current position 
    updateCurrentPosition();

    // resets the OMS bit of the control word
    writeControlWord(0x0270, 0);


    // set the cia Switched On status
    writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);

    // Clears motor command
    current_command = MotorCommands::MOTOR_IDLE;

    return;

}

/// <summary>
/// This function executes the Automatic positioning of the motor at the expected target position.
/// 
/// The function executes the following steps for preparation:
/// - sets the expected timeout in ms;
/// - verifies if the current position should be already in target;
/// - upload the running parameters: speed, acceleration deceleration;
/// - sets the Positioning profile mode in the motor;
/// - activates the Start bit in the control register to start the rotation;
/// 
/// During the rotation:
/// - verifies if the current status should be changed. In this case termines with error;
/// - verifies if the activation timeout should expires. In this case termines with error;
/// - verifies if the target is detected in three possible ways:
///     - The motor signals automatically the target (usually it should);
///     - The current position matches with the target position (less probable);
///     - in case of timeout if the position should be in the acceptable range (recovery strategy);
/// 
/// When the command completes (successfully or in error) 
/// the CiA status mode is set to SwitchedOn status (or in a fault status if it should be actually active)
/// 
/// With the command completion, the following event is generated:
/// - command_completed_event(command_id, termination_code);
/// 
/// </summary>
/// <param name=""></param>
void CanOpenMotor::manageAutomaticPositioning(void) {
    bool error_condition;

    // Get the actual encoder position 
    updateCurrentPosition();

    // Set the timeout for the activation based on the actual speed and target
    setActivationTimeout(command_speed, command_acc, command_dec, command_target);

    // Test if the actual position is already in target position
    if (isTarget()) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING COMPLETED (ALREADY IN POSITION)");
        setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);
        return;
    }

    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + 
        "> INIT AUTO POSITIONING: From " + System::Convert::ToString(current_uposition) + 
        " To " + System::Convert::ToString(command_target) +
        " In " + System::Convert::ToString((int) ((double) command_ms_tmo / 1.5)) + " (ms)"  );


    // Sets the Speed activation
    error_condition = false;
    if (!blocking_writeOD(OD_6081_00, convert_UserSec_To_Speed(command_speed))) error_condition = true; // Sets the Rotation speed 
    if (!blocking_writeOD(OD_6083_00, convert_UserSec_To_Speed(command_acc))) error_condition = true; // Sets the Rotation Acc
    if (!blocking_writeOD(OD_6084_00, convert_UserSec_To_Speed(command_dec))) error_condition = true; // Sets the Rotation Dec
    if (!blocking_writeOD(OD_607A_00, convert_User_To_Encoder(command_target))) error_condition = true; // Set the target position
    if (!blocking_writeOD(OD_6060_00, 1)) error_condition = true;// Set the Activation mode to Profile Positioning Mode
    if (!writeControlWord(POSITION_SETTING_CTRL_INIT_MASK, POSITION_SETTING_CTRL_INIT_VAL)) error_condition = true;// Sets the Position control bits in the control word
    if (error_condition) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN AUTO POSITIONING PREPARATION");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }


    // Tries to activate the Operation Enabled 
    error_condition = true;
    for(int i=0; i<5; i++) {
        
        if (!writeControlWord(OD_6040_00_ENABLEOP_MASK, OD_6040_00_ENABLEOP_VAL)) continue;

        // Read the status word
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (!blocking_readOD(OD_6041_00)) continue;
        if (getCiAStatus(rxSdoRegister->data) == _CiA402Status::CiA402_OperationEnabled) {            
            error_condition = false;
            break;
        }

    }

    if (error_condition) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN AUTO POSITIONING OPERATION ENABLE SETTING");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }
   
    // Allows the application to prepare for the motor activation
    MotorCompletedCodes preparation_error = automaticPositioningPreparationCallback();
    if (preparation_error != MotorCompletedCodes::COMMAND_PROCEED) {
        setCommandCompletedCode(preparation_error);
        return;
    }

    // Update the previous position
    previous_uposition = current_uposition;


    // Set the start bit (BIT4) in the control word register
    if (!writeControlWord(POSITION_SETTING_START_MASK, POSITION_SETTING_START_VAL)){        
        setCommandCompletedCode(MotorCompletedCodes::ERROR_ACTIVATION_REGISTER);        
        return;
    }

    // Measuring the actuation time
    typedef std::chrono::high_resolution_clock clock;
    clock::time_point start = clock::now();
    
    
    while (true) {

        // Reads the status to be sure that it is still Operation Enabled 
        if (!blocking_readOD(OD_6041_00)) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING ERROR READING STATUS REGISTER");            
            setCommandCompletedCode(MotorCompletedCodes::ERROR_ACTIVATION_REGISTER);
            break;
        }

        unsigned short statw = rxSdoRegister->data;

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING ERROR POSSIBLE FAULT");            
            setCommandCompletedCode(MotorCompletedCodes::ERROR_INTERNAL_FAULT);            
            break;
        }

        // Read the current position 
        updateCurrentPosition();

        // Test the Timeout condition
        if ((command_ms_tmo <= 0) && (!isTarget())) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING TIMEOUT ERROR");            
            setCommandCompletedCode(MotorCompletedCodes::ERROR_TIMOUT);
            break;
        }

        // The Application can early terminate the activation
        MotorCompletedCodes termination_condition = automaticPositioningRunningCallback();
        if (termination_condition >= MotorCompletedCodes::MOTOR_ERRORS) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: Application terminated early for error detected");
            setCommandCompletedCode(termination_condition);
            break;
        }

        if (((statw & 0x1400) == 0x1400) || (current_uposition == command_target) || ((command_ms_tmo <= 0) && isTarget()) || (termination_condition == MotorCompletedCodes::COMMAND_MANUAL_TERMINATION)) {
            
            // Calculates the effective duration time
            typedef std::chrono::milliseconds milliseconds;
            milliseconds ms = std::chrono::duration_cast<milliseconds>(clock::now() - start);

            if (((command_ms_tmo <= 0) && isTarget())) {
                Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSITIONING COMPLETED (MANUALLY) - Time =  " + System::Convert::ToString(ms.count()));
            }
            else {
                Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: AUTOMATIC POSIITONING COMPLETED actually In =  " + System::Convert::ToString(ms.count()) + " (ms)");
            }
            
            setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);

            // resets the OMS bit of the control word
            writeControlWord(0x0270, 0);

            // set the cia Switched On status
            writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);
                        
            break;
        }

        command_ms_tmo -= 50;        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    } // End of main controlling loop
    
    // Read the current position 
    updateCurrentPosition();

    // resets the OMS bit of the control word
    writeControlWord(0x0270, 0);

  
    // set the cia Switched On status
    writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);
    
    // Clears motor command
    current_command = MotorCommands::MOTOR_IDLE;
    
    return;

}

bool CanOpenMotor::initResetEncoderCommand(int initial_eposition) {

    updateCurrentPosition();

    // Already reset in this position
    if (current_eposition == initial_eposition) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ENCODER INITIALIZATION SUCCESS, Position = " + current_uposition.ToString() );
        return true; 
    }

    // Write the Homing method to 35: current position method
    if (!blocking_writeOD(OD_6098_00, 35)) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR WRITING IDX 6098");
        return false;
    }

    // Write the Position offset register
    if (!blocking_writeOD(OD_607C_00, initial_eposition)) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR WRITING IDX 607C");
        return false;
    }

    
    // Write the operating mode to HOMING
    if (!blocking_writeOD(OD_6060_00, OD_6060_00_PROFILE_HOMING)) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR WRITING IDX 6060 TO HOMING");       
        return false;
    }

    // Set the start bit (BIT4) in the control word register
    if (!writeControlWord(0x10, 0x10)) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR WRITING CW BIT 4");
        return false;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Clear the start bit (BIT4) in the control word register
    if (!writeControlWord(0x10, 0)) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR CLEARING CW BIT 4");
        return false;
    }

    // Write the operating mode to PROFILE POSITIONING
    if (!blocking_writeOD(OD_6060_00, OD_6060_00_PROFILE_POSITIONING)) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR WRITING IDX 6060 TO POSITIONING");        
        return false;
    }

    updateCurrentPosition();
    if (current_eposition != initial_eposition) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ENCODER NOT IN THE INITIAL ASSIGNED VALUE");        
        return false;
    }

    
    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ENCODER INITIALIZATION SUCCESS, Position = " + current_uposition.ToString());
    return true;


}

bool CanOpenMotor::activateManualPositioning(int target, int speed, int acc, int dec) {

    // Only with the homing executed or initialized can be activated
    if (!home_initialized) {
        command_completed_code = MotorCompletedCodes::ERROR_MISSING_HOME;
        return false;
    }

    // Command already in execution
    if (!isReady()) {
        command_completed_code = MotorCompletedCodes::ERROR_MOTOR_BUSY;
        return false;
    }


    command_id = 0;
    command_target = target;
    command_acc = acc;
    command_dec = dec;
    command_speed = speed;
    command_stop = false;
    request_command = MotorCommands::MOTOR_MANUAL_POSITIONING;
    return true;
}



void CanOpenMotor::manageManualPositioning(void) {
    bool error_condition;

    // Get the actual encoder position 
    updateCurrentPosition();

    // Test if the actual position is already in target position
    if (isTarget()) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING COMPLETED (ALREADY IN POSITION)");
        setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);
        return;
    }

    
    // Sets the Speed activation
    error_condition = false;
    if (!blocking_writeOD(OD_6081_00, convert_UserSec_To_Speed(command_speed))) error_condition = true; // Sets the Rotation speed 
    if (!blocking_writeOD(OD_6083_00, convert_UserSec_To_Speed(command_acc))) error_condition = true; // Sets the Rotation Acc
    if (!blocking_writeOD(OD_6084_00, convert_UserSec_To_Speed(command_dec))) error_condition = true; // Sets the Rotation Dec
    if (!blocking_writeOD(OD_607A_00, convert_User_To_Encoder(command_target))) error_condition = true; // Set the target position
    if (!blocking_writeOD(OD_6060_00, 1)) error_condition = true;// Set the Activation mode to Profile Positioning Mode
    if (!writeControlWord(POSITION_SETTING_CTRL_INIT_MASK, POSITION_SETTING_CTRL_INIT_VAL)) error_condition = true;// Sets the Position control bits in the control word
    if (error_condition) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN MANUAL POSITIONING PREPARATION");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }


    // Tries to activate the Operation Enabled 
    error_condition = true;
    for (int i = 0; i < 5; i++) {

        if (!writeControlWord(OD_6040_00_ENABLEOP_MASK, OD_6040_00_ENABLEOP_VAL)) continue;

        // Read the status word
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (!blocking_readOD(OD_6041_00)) continue;
        if (getCiAStatus(rxSdoRegister->data) == _CiA402Status::CiA402_OperationEnabled) {
            error_condition = false;
            break;
        }

    }

    if (error_condition) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN MANUAL POSITIONING OPERATION ENABLE SETTING");
        setCommandCompletedCode(MotorCompletedCodes::ERROR_INITIALIZATION);
        return;
    }

    // Allows the application to prepare for the motor activation
    MotorCompletedCodes preparation_error = manualPositioningPreparationCallback();
    if (preparation_error != MotorCompletedCodes::COMMAND_PROCEED) {
        setCommandCompletedCode(preparation_error);
        return;
    }

    // Update the previous position
    previous_uposition = current_uposition;


    // Set the start bit (BIT4) in the control word register
    if (!writeControlWord(POSITION_SETTING_START_MASK, POSITION_SETTING_START_VAL)) {
        setCommandCompletedCode(MotorCompletedCodes::ERROR_ACTIVATION_REGISTER);
        return;
    }

  
    while (true) {

        // Reads the status to be sure that it is still Operation Enabled 
        if (!blocking_readOD(OD_6041_00)) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING ERROR READING STATUS REGISTER");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_ACTIVATION_REGISTER);
            break;
        }

        unsigned short statw = rxSdoRegister->data;

        // In case the status should be changed, the internal fault is activated
        if (getCiAStatus(statw) != _CiA402Status::CiA402_OperationEnabled) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING ERROR POSSIBLE FAULT");
            setCommandCompletedCode(MotorCompletedCodes::ERROR_INTERNAL_FAULT);
            break;
        }

        // Read the current position 
        updateCurrentPosition();

        // The Application can early terminate the activation
        MotorCompletedCodes termination_condition = manualPositioningRunningCallback();
        if (termination_condition >= MotorCompletedCodes::MOTOR_ERRORS) {
            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: Application terminated early for error detected");
            setCommandCompletedCode(termination_condition);
            break;
        }

        if (((statw & 0x1400) == 0x1400) || (current_uposition == command_target) || (termination_condition == MotorCompletedCodes::COMMAND_MANUAL_TERMINATION)) {

            Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: MANUAL POSITIONING COMPLETED ");

            setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);

            // resets the OMS bit of the control word
            writeControlWord(0x0270, 0);

            // set the cia Switched On status
            writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);

            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    } // End of main controlling loop

    // Read the current position 
    updateCurrentPosition();

    // resets the OMS bit of the control word
    writeControlWord(0x0270, 0);


    // set the cia Switched On status
    writeControlWord(OD_6040_00_DISABLEOP_MASK, OD_6040_00_DISABLEOP_VAL);

    // Clears motor command
    current_command = MotorCommands::MOTOR_IDLE;

    return;

}