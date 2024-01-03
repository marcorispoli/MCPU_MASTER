#include "CanDriver.h"
#include "Notify.h"
#include "CanOpenMotor.h"
#include "pd4_od.h"
#include <thread>
#include <mutex>
#include <chrono>


using namespace CANOPEN;
static std::mutex init_mutex;



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

    if(reverse_direction)  return -1 * (int)((double)x * rot_per_unit * (double)2000);
    else return (int)((double) x * rot_per_unit * (double) 2000 );
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
    if (reverse_direction) return -1* (int)((double)x / (rot_per_unit * (double)2000));
    else return (int)( (double) x / (rot_per_unit * (double) 2000) );
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
/// 
/// The Base Class constructor:
/// + Initializes all the internal variables;
/// + initializes the CiA internal status;
/// + Starts the working thread mainWorker
/// 
/// As soon as the function returns, the working thread 
/// starts the communication with the motor device.
/// 
/// See the mainWorker() for details.
/// 
/// <param name="devid">This is the address of the Motor device</param>
/// <param name="motorname">This is a string of the Motor name</param>
/// <param name="rounds_for_units">This is the unit conversion rate</param>
/// <param name="reverse">true if the position shall be reversed (+/-)</param>
CanOpenMotor::CanOpenMotor(unsigned char devid, LPCWSTR motorname, double rounds_for_units, bool reverse) {
    
    internal_status = status_options::MOTOR_NOT_CONNECTED;
    configuration_command = false;
    od_initialized = false;
    nanoj_initialized = false;
    pNanoj = nullptr;
    nanojSize = 0;
    home_initialized = false;
    encoder_initial_value = 0;
    reverse_direction = reverse;

    // For Can Diagnose
    read_sdo_tmo = false;
    write_sdo_tmo = false;
    sent_messages = 0;
    unreceived_messages = 0;
    meanTime = 0;

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
        if (!rxSdoRegister->validateSdo(data)) return;
        sdo_rx_pending = false;
        
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

    // Create the register to be received
    rxSdoRegister = gcnew ODRegister(index, sub, dim, val);
    rxSdoRegister->cmd = ODRegister::SDOCommandCodes::WRCMD;
    rxSdoRegister->getWriteBuffer(buffer);

    // Activates the transmission
    long start = clock();
    sent_messages++;
    CanDriver::multithread_send(0x600 + device_id, buffer, 8);
    sdo_rx_pending = true;

    // Waits to be signalled: waits for 50ms
    dwWaitResult = WaitForSingleObject(rxSDOEvent, SEND_TMO);
    sdo_rx_pending = false;

    // Checks if the Event has been signalled or it is a timeout event.
    if ((dwWaitResult != WAIT_OBJECT_0) || (!rxSdoRegister->valid)) {
        unreceived_messages++;
        return false;
    }
    long stop = clock();
    txrx_time = ((double)(stop - start)) / (double)CLOCKS_PER_SEC;

    if (txrx_time < 0.005)  sent_5++;
    else if (txrx_time < 0.010)  sent_10++;
    else if (txrx_time < 0.015)  sent_15++;
    else if (txrx_time < 0.020)  sent_20++;
    else if (txrx_time < 0.025)  sent_25++;
    else if (txrx_time < 0.030)  sent_30++;
    else if (txrx_time >= 0.030)  sent_xx++;
    meanTime += txrx_time;

    if (sent_messages == 1000) {
        perc5 = (double)sent_5 * 100 / (double)sent_messages;
        perc10 = (double)sent_10 * 100 / (double)sent_messages;
        perc15 = (double)sent_15 * 100 / (double)sent_messages;
        perc20 = (double)sent_20 * 100 / (double)sent_messages;
        perc25 = (double)sent_25 * 100 / (double)sent_messages;
        perc30 = (double)sent_30 * 100 / (double)sent_messages;
        percXX = (double)sent_xx * 100 / (double)sent_messages;

        percMeanTime = meanTime * 1000 / (double)sent_messages;

        meanTime = 0;
        sent_5 = 0;
        sent_10 = 0;
        sent_15 = 0;
        sent_20 = 0;
        sent_25 = 0;
        sent_30 = 0;
        sent_xx = 0;
        sent_messages = 100;
        System::String^ stringa = "Motor Device <" + System::Convert::ToString(device_id);
        stringa += ">: T:" + ((int)percMeanTime).ToString();
        stringa += " [5]:" + ((int)perc5).ToString();
        stringa += " [10]:" + ((int)perc10).ToString();
        stringa += " [15]:" + ((int)perc15).ToString();
        stringa += " [20]:" + ((int)perc20).ToString();
        stringa += " [25]:" + ((int)perc25).ToString();
        stringa += " [30]:" + ((int)perc30).ToString();
        stringa += " [>30]:" + ((int)percXX).ToString();
        // Debug::WriteLine(stringa);
    }

    return true;
    
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
   
    // Create the register to be received
    rxSdoRegister = gcnew ODRegister(index, sub, dim);
    rxSdoRegister->cmd = ODRegister::SDOCommandCodes::RDCMD;
    rxSdoRegister->getReadBuffer(buffer);

    // Activates the transmission
    long start = clock();
    sent_messages++;
        
    // Sends the message
    CanDriver::multithread_send(0x600 + device_id, buffer, 8);        
    sdo_rx_pending = true;

    // Waits to be signalled: waits for 50ms
    dwWaitResult = WaitForSingleObject(rxSDOEvent, SEND_TMO);
    sdo_rx_pending = false;

    // Checks if the Event has been signalled or it is a timeout event.
    if ( (dwWaitResult != WAIT_OBJECT_0) || (!rxSdoRegister->valid) ) {
        unreceived_messages++;
        return false;
    }
    
    long stop = clock();    
    txrx_time = ((double)( stop - start)) / (double) CLOCKS_PER_SEC;

    if (txrx_time < 0.005)  sent_5++;
    else if (txrx_time < 0.010)  sent_10++;
    else if (txrx_time < 0.015)  sent_15++;
    else if (txrx_time < 0.020)  sent_20++;
    else if (txrx_time < 0.025)  sent_25++;
    else if (txrx_time < 0.030)  sent_30++;
    else if (txrx_time >= 0.030)  sent_xx++;
    meanTime += txrx_time;

    if (sent_messages == 1000) {
        perc5 = (double)sent_5 * 100 / (double)sent_messages;
        perc10 = (double)sent_10 * 100 / (double)sent_messages;
        perc15 = (double)sent_15 * 100 / (double)sent_messages;
        perc20 = (double)sent_20 * 100 / (double)sent_messages;
        perc25 = (double)sent_25 * 100 / (double)sent_messages;
        perc30 = (double)sent_30 * 100 / (double)sent_messages;
        percXX = (double)sent_xx * 100 / (double)sent_messages;

        percMeanTime = meanTime * 1000 / (double)sent_messages;
        
        meanTime = 0;
        sent_5 = 0;
        sent_10 = 0;
        sent_15 = 0;
        sent_20 = 0;
        sent_25 = 0;
        sent_30 = 0;
        sent_xx = 0;
        sent_messages = 0;

        
        System::String^ stringa = "Motor Device <" + System::Convert::ToString(device_id);
        stringa +=">: T:" + ((int)percMeanTime).ToString();
        stringa += " [5]:" + ((int)perc5).ToString();
        stringa += " [10]:" + ((int)perc10).ToString();
        stringa += " [15]:" + ((int)perc15).ToString();
        stringa += " [20]:" + ((int)perc20).ToString();
        stringa += " [25]:" + ((int)perc25).ToString();
        stringa += " [30]:" + ((int)perc30).ToString();
        stringa += " [>30]:" + ((int)percXX).ToString();
        // Debug::WriteLine(stringa);
        
    }

    return true;
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
/// This is the Main thread managing the communication with the motor.
/// 
/// The threads:
/// - handles the CiA status machine;
/// - handles the execution of the commands (only in SwitchedOn status)
/// - handles the motor exceptions.
/// - handles the motor configuration.
/// 
/// The thread leads the motor driver to enter the CiA-SwitchedOn status if possible.
/// 
/// When the driver reaches the  CiA-SwitchedOn status, the motor activation commands can be performed.
/// 
/// If the Motor should enter a CiA-Fault status, the thread handles the fault condition \n
/// and as soon as the fault condition expires it tries to report the current status to the CiA-SwitchedOn status.
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
            unreceived_messages = 0;
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
/// 
/// This function initializes the principals motor driver registers.
/// </summary>
/// 
/// This function is called during the initialization fase in order to 
/// set the internal register with default values, widely used for PD4 motors.
/// 
/// The function will call the initializeSpecificObjectDictionaryCallback() function:
/// + The Subclass module should override this function to set specific registers.
/// 
/// <param name=""></param>
/// <returns>true in case of success</returns>
bool CanOpenMotor::initializeObjectDictionary(void) {
    const std::lock_guard<std::mutex> lock(init_mutex);
    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: INITIALIZATION");

    
    while(!blocking_writeOD(OD_4013_01,1)) ; // Hardware configuration : 1 = EXTERNAL VCC LOGIC ON

    // NMT Behavior in case of fault
    while (!blocking_writeOD(OD_1029_01, 0)) ;
    while (!blocking_writeOD(OD_1029_02, 1)) ;
    while (!blocking_writeOD(OD_2031_00, 5000)) ;  // Peak current
    while (!blocking_writeOD(OD_2032_00, 5000)) ;  // Maximum Speed
    while (!blocking_writeOD(OD_2033_00, 0)) ;	    // Plunger Block
    while (!blocking_writeOD(OD_2034_00, 51500)) ; // Upper Voltage Warning Level
    while (!blocking_writeOD(OD_2035_00, 21000)) ; // ****************************************Lower Voltage Warning Level
    while (!blocking_writeOD(OD_2036_00, 2000)) ; 	// Open Loop Current Reduction Idle Time
    while (!blocking_writeOD(OD_2037_00, -50)) ;	// Open Loop Current Reduction Value/factor

    // I2t Parameters
    while (!blocking_writeOD(OD_203B_01, 5000)) ;	// Nominal Current
    while (!blocking_writeOD(OD_203B_02, 1000)) ;	// Maximum Duration Of Peak Current
    while (!blocking_writeOD(OD_203B_03, 0)) ;	    // Threshold
    while (!blocking_writeOD(OD_203B_04, 0)) ;	    // CalcValue
    while (!blocking_writeOD(OD_203B_05, 5000)) ; 	// LimitedCurrent
    while (!blocking_writeOD(OD_2056_00, 500)) ;	// Limit Switch Tolerance Band

    // user units
    while (!blocking_writeOD(OD_2061_00, 1)) ;	// Velocity Numerator
    while (!blocking_writeOD(OD_2062_00, SPEED_DENOMINATOR)) ;  // Velocity Denominator        ***********************
    while (!blocking_writeOD(OD_2063_00, 1)) ;	// Acceleration Numerator
    while (!blocking_writeOD(OD_2064_00, SPEED_DENOMINATOR)) ; 	// Acceleration Denominator    ***********************
    while (!blocking_writeOD(OD_2065_00, 1)) ;	// Jerk Numerator
    while (!blocking_writeOD(OD_2066_00, 60)) ;	// Jerk Denominator
    while (!blocking_writeOD(OD_3202_00, 9)) ; 	// Motor Drive Submode Select: 6:BLDC 3:CurRed 2:Brake 1:VoS 0: 1=CLOSED_LOOP/O = OPEN_LOOP

    // Motor Drive Sensor Display Closed Loop
    while (!blocking_writeOD(OD_320B_01, 0)) ;     // Commutation
    while (!blocking_writeOD(OD_320B_02, 0)) ;	    // Torque
    while (!blocking_writeOD(OD_320B_03, 1)) ;	    // Velocity
    while (!blocking_writeOD(OD_320B_04, 1)) ; 	// Position

    // Motor Drive Parameter Set
    while (!blocking_writeOD(OD_3210_01, 50000)) ; // Position Loop, Proportional Gain (closed Loop)
    while (!blocking_writeOD(OD_3210_02, 10)) ;	 // Position Loop, Integral Gain (closed Loop)

    // Analogue Inputs Control
    while (!blocking_writeOD(OD_3221_00, 0)) ;     // 0 , Voltage, 1, Current


    // Following Error Option Code
    while (!blocking_writeOD(OD_3700_00, (unsigned short)-1)) ; // -1 No reaction;0 Immediate stop; 1 Braking with "slow down ramp"; 2 Braking with "quick stop ramp"

    // Quick Stop Option Code
    while (!blocking_writeOD(OD_605A_00, 2)) ;   // 0 Immediate stop  // 1 Braking with "slow down ramp"  // 2 Braking with "quick stop ramp"

    // Shutdown Option Code
    while (!blocking_writeOD(OD_605B_00, 0)) ;   // 0 Immediate stop  // 1 Braking with "slow down ramp"

    // Disable Option Code
    while (!blocking_writeOD(OD_605C_00, 0)) ;   // 0 Immediate stop  // 1 Braking with "slow down ramp"

    // Halt Option Code
    while (!blocking_writeOD(OD_605D_00, 0)) ;  // 0 Immediate stop  // 1 Braking with "slow down ramp"  // 2 Braking with "quick stop ramp"

    // Fault Option Code
    while (!blocking_writeOD(OD_605E_00, 0)) ;  // 0 Immediate stop  // 1 Braking with "slow down ramp"   // 2 Braking with "quick stop ramp"

    // Following Error Window and time
    while (!blocking_writeOD(OD_6065_00, 256)) ; 	// Window
    while (!blocking_writeOD(OD_6066_00, 100)) ;	// Time (ms)

    // Position Window + time
    while (!blocking_writeOD(OD_6067_00, 10)) ;	// Window
    while (!blocking_writeOD(OD_6068_00, 100)) ;	// Time

    // Position Range Limit
    while (!blocking_writeOD(OD_607B_01, 0)) ; 	// Min Position Range Limit
    while (!blocking_writeOD(OD_607B_02, 0)) ;	// Max Position Range Limit

    // Software Position Limit
    while (!blocking_writeOD(OD_607D_01, convert_User_To_Encoder(-18000))) ;	// Min Position Limit
    while (!blocking_writeOD(OD_607D_02, convert_User_To_Encoder(18000))) ;	// Max Position Limit

    // Polarity
    while (!blocking_writeOD(OD_607E_00, 0)) ;	// b7:1-> inverse rotation

    // Position Encoder Resolution: EncInc/MotRev
    while (!blocking_writeOD(OD_608F_01, 2000)) ;	// Encoder Increments
    while (!blocking_writeOD(OD_608F_02, 1)) ; 	// Motor Revolutions
    while (!blocking_writeOD(OD_60F2_00, 0x0002)) ;// Absolute positionning

    // Gear Ratio
    while (!blocking_writeOD(OD_6091_01, 1)) ; 	// Motor Revolutions
    while (!blocking_writeOD(OD_6091_02, 1)) ;	    // Shaft Revolutions

    // Max Absolute Acceleration and Deceleration
    while (!blocking_writeOD(OD_60C5_00, 5000)) ;  // Max Acceleration
    while (!blocking_writeOD(OD_60C6_00, 5000)) ;  // Max Deceleration

    // Homing registers
    while (!blocking_writeOD(OD_6098_00, 21)) ;  // Homing method 21
    while (!blocking_writeOD(OD_607C_00, 0));
    
    // Specific register set for the Subclassed motor
    if(!initializeSpecificObjectDictionaryCallback()) return false;

    // Read the Object dictionary flag initialization
    while (!blocking_readOD(CONFIG_USER_PARAM)) ; // Reads the user parameter containing the stored nanoj checksum
    if (rxSdoRegister->data == 0x1A1B) {
        Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: INITIALIZED");
        return true;
    }
    
    Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: STORING DATA IN FLASH");
    
    // Save the Flag in the User Param register and store the User param space
    blocking_writeOD(CONFIG_USER_PARAM, 0x1A1B);
    blocking_writeOD(OD_2700_01, 1);

    // Save the whole object dictionary
    blocking_writeOD(OD_1010_01, OD_SAVE_CODE);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    int i;
    for (i = 30; i > 0; i--) {
        if((blocking_readOD(OD_1010_01)) && (rxSdoRegister->data == 1)) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (i) Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: OBJECT DICTIONARY STORED IN FLASH");
    else Debug::WriteLine("Motor Device <" + System::Convert::ToString(device_id) + ">: FAILED STORING OD IN FLASH");    
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
/// This function returns  the activation timeout extimation, based on the speed and target parameters.
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
int CanOpenMotor::getActivationTimeout(int speed, int acc, int dec, int target) {
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
    return (int)(T * 1000 );

}

/// <summary>
/// This function is internally used to set a command completion code.
/// 
/// </summary>
/// 
/// The function calls the command related callbacks:
/// + automaticPositioningCompletedCallback: in case of Automatic positioning command termination
/// + automaticHomingCompletedCallback: in case of a Homing command termination 
/// + manualPositioningCompletedCallback: in case of manual command termination 
/// 
/// <param name=term_code>the termination code</param>
void CanOpenMotor::setCommandCompletedCode(MotorCompletedCodes term_code) {
    command_completed_code = term_code;
    MotorCommands cmd = current_command;
    current_command = MotorCommands::MOTOR_IDLE;

    if (cmd == MotorCommands::MOTOR_AUTO_POSITIONING) automaticPositioningCompletedCallback(term_code);
    else if(cmd == MotorCommands::MOTOR_HOMING) automaticHomingCompletedCallback(term_code);
    else if (cmd == MotorCommands::MOTOR_MANUAL_POSITIONING) manualPositioningCompletedCallback(term_code);
    
    return;
}


/// <summary>
/// This procedure returns a language-translated string 
/// describing the last terminated command code.
/// 
/// </summary>
/// <param name=""></param>
/// <returns>String describing the last termninated command code</returns>
System::String^ CanOpenMotor::getCompletedCodeString(void) {
    switch (command_completed_code) {

    case MotorCompletedCodes::COMMAND_SUCCESS: 
        return Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_COMMAND_SUCCESS);
        break;
    case MotorCompletedCodes::COMMAND_MANUAL_TERMINATION:
        return Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_COMMAND_MANUAL_TERMINATION);
        break;
    case MotorCompletedCodes::ERROR_OBSTACLE_DETECTED:
        return Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_ERROR_OBSTACLE_DETECTED);
        break;
    case MotorCompletedCodes::ERROR_MOTOR_BUSY:
        return Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_ERROR_MOTOR_BUSY);
        break;
    case MotorCompletedCodes::ERROR_INITIALIZATION:
        return Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_ERROR_INITIALIZATION);
        break;
    case MotorCompletedCodes::ERROR_UNEXPECTED_STATUS:
        Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_ERROR_UNEXPECTED_STATUS);
        break;
    case MotorCompletedCodes::ERROR_LIMIT_SWITCH:
        return Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_ERROR_LIMIT_SWITCH);
        break;
    case MotorCompletedCodes::ERROR_BRAKE_DEVICE:
        return Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_ERROR_BRAKE_DEVICE);
        break;
       
    case MotorCompletedCodes::ERROR_TIMOUT:
        return Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_ERROR_TIMOUT);
        break;
    case MotorCompletedCodes::ERROR_INTERNAL_FAULT:
        return Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_ERROR_INTERNAL_FAULT);
        break;
    case MotorCompletedCodes::ERROR_ACCESS_REGISTER:
        return Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_ERROR_ACCESS_REGISTER);
        break;
    case MotorCompletedCodes::ERROR_MISSING_HOME:
        return Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_ERROR_MISSING_HOME);
        break;
    case MotorCompletedCodes::ERROR_COMMAND_DISABLED:
        return Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_ERROR_COMMAND_DISABLED);
        break;
    case MotorCompletedCodes::ERROR_COMMAND_ABORTED:
        return Notify::TranslateLabel(Notify::messages::LABEL_MOTOR_ERROR_COMMAND_ABORTED);
        break;

        
    }

    return "";
}

/// <summary>
/// This command requests for an immediate activation abort.
/// </summary>
/// 
/// + If the motor is not active, the command has not effect.
/// + If the motor is active, a quick stop procedure is activated.
/// 
/// <param name=""></param>
void CanOpenMotor::abortActivation(void) {
    abort_request = true;
}