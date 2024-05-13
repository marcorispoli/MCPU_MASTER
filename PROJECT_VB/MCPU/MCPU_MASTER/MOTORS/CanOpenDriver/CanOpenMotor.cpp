#include "CanDriver.h"
#include "gantry_global_status.h"
#include "Notify.h"
#include "CanOpenMotor.h"
#include "CalibrationConfig.h"
#include "pd4_od.h"
#include <thread>
#include <mutex>
#include <chrono>
#include "Log.h"


using namespace CANOPEN;
static std::mutex init_mutex;



/// <summary>
///  This is the Class constructor.
/// 
/// </summary>
/// <param name="devid">unique device id of the motor</param>
/// <param name="motorname">motor name assigned to the working thread</param>
/// <param name="parameter">string name of the parameter in the MotorCalibration.cnf file</param>
/// <param name="home_err">Error code to be activated in case of Position invalidated</param>
/// <param name="rounds_for_units">number of motor round for user unit</param>
/// <param name="reverse">set the current motor direction</param>
CanOpenMotor::CanOpenMotor(unsigned char devid, 
    LPCWSTR motorname, 
    System::String^ parameter, 
    Notify::messages home_err, 
    int min,
    int max,
    double rounds_for_units, 
    double external_k, 
    bool reverse) {

    config_param = parameter;
    external_position_mode = false;
    external_k_coeff = external_k;
    min_position = min;
    max_position = max;
    service_mode = false;

    reset_node = true;
    internal_status = status_options::MOTOR_NOT_CONNECTED;
    configuration_command = false;
    od_initialized = false;
    nanoj_initialized = false;
    pNanoj = nullptr;
    nanojSize = 0;
    home_initialized = false;
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
    fault_activation = false;

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



    // Set the current command to IDLE
    current_command = MotorCommands::MOTOR_IDLE;
    request_command = MotorCommands::MOTOR_IDLE;

    // As default the target range is 0
    target_range_h = 0;
    target_range_l = 0;

    // Error to be notified in case of encoder errors
    error_homing = home_err;


    // Defines the position method and related initialization
    try {
        // Identify what is the position detector method
        if (MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_EXTERNAL_POSITION] == MotorConfig::MOTOR_INTERNAL_POSITION) {
            // Internal Encoder is used to keep the position
            external_position_mode = false;
        }
        else {
            // External sensor is used to keep the position
            external_position_mode = true;
        }
    }
    catch (...) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + "> FATAL ERROR: CanOpenMotor() - Invalid Parameter");
        Application::Exit();
    }

    // Creates the Startup thread
    main_thread = gcnew Thread(gcnew ThreadStart(this, &CanOpenMotor::mainWorker));
    main_thread->Name = "Motor Worker" + System::Convert::ToString(device_id);
    main_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
    main_thread->Start();

}

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


bool CanOpenMotor::update_external_position(void) {
    if (simulator_mode) return true;

    // Potentiometer position acquisition
    if (!blocking_readOD(OD_3220_01)) return false;
    external_raw_position = (unsigned short) getRxReg()->data;

    int p1 = external_raw_position;
    int p2 = external_zero_setting;
    external_uposition = (int) (((float)(p1 - p2)) * external_k_coeff);
    return true;
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
    
    if (configuration_command) return;
    configuration_command = true;

    LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + "> Reset Management");
    
    // Invalidates the current register initialization
    od_initialized = false;
    home_initialized = false;

    // calls the specific reset callback
    resetCallback();
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
    //const std::lock_guard<std::mutex> lock(can_mutex);
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
        // LogClass::logInFile(stringa);
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
   // const std::lock_guard<std::mutex> lock(can_mutex);

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
        // LogClass::logInFile(stringa);
        
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
    //const std::lock_guard<std::mutex> lock(can_mutex);
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
/// This function reads the control word.
/// 
/// </summary>
/// <param name="ctrlw">This is the pointer to the variable where the control word will be copied in</param>
/// <returns>true if the register is successfully read</returns>
bool CanOpenMotor::readControlWord(unsigned int* ctrlw) {    
    if (!blocking_readOD(OD_6040_00)) return false;
    *ctrlw = (unsigned int) rxSdoRegister->data;
    return true;
}

/// <summary>
/// This function starts the motor rotation.
/// </summary>
/// 
/// The function activates the BIT-4 of the control word: 
/// this bit starts the motor rotation for the Positioning Mode.
/// 
/// <param name=""></param>
/// <returns></returns>
bool CanOpenMotor::startRotation(void) {
    return writeControlWord(POSITION_SETTING_START_MASK, POSITION_SETTING_START_VAL);
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
    LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: wait run command");

    while (!run) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Demo mode activation
    if (simulator_mode) {
        internal_status = status_options::MOTOR_READY;
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: Module Run in Demo mode");

        // Gets the initial position of the encoder. If the position is a valid position the homing is not necessary
        if (MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_CURRENT_POSITION] != MotorConfig::MOTOR_UNDEFINED_POSITION) {
            external_uposition = encoder_uposition = System::Convert::ToInt32(MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_CURRENT_POSITION]);
            home_initialized = true;
            Notify::deactivate(error_homing);
        }
        else {
            home_initialized = false;
            Notify::activate(error_homing);
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ERROR IN INITIALIZATION");
        }


        while (simulator_mode) {
            demoLoop();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: RUNNING MODE STARTED");
    
    // Add the callback handling the SDO reception
    CanDriver::canrx_canopen_sdo_event += gcnew CanDriver::delegate_can_rx_frame(this, &CanOpenMotor::thread_canopen_rx_sdo_callback);

    // Add the bootup handler
    CanDriver::canrx_canopen_bootup_event += gcnew CanDriver::delegate_can_rx_frame(this, &CanOpenMotor::thread_canopen_bootup_callback);

    // Assignes the initial Cia status to undefined
    CiA_current_status = _CiA402Status::CiA402_Undefined;


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
        
        
        if (reset_node) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: EXECUTES RESET_NODE");
            internal_status = status_options::MOTOR_NOT_CONNECTED;
            reset_node = false;
            write_resetNode();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));  
            configuration_command = true;
        }

        // Read the status word
        if (!blocking_readOD(OD_6041_00)) {
            internal_status = status_options::MOTOR_NOT_CONNECTED;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            unreceived_messages = 0;
            continue;
        }

        // Activate  the configuration: both object dictionary registers and nanoj program (if present) shall be uploaded
        if ((configuration_command) && ((internal_status == status_options::MOTOR_READY) || (internal_status == status_options::MOTOR_FAULT))) {
            internal_status = status_options::MOTOR_CONFIGURATION;            
            service_mode = false;

            // If the nanoj program is not present (pNanoj = nullptr)  the nanoj_initialized flag  shall be set to true !
            if (pNanoj) nanoj_initialized = uploadNanojProgram();
            else nanoj_initialized = true;

            // Initializes the object dictionary
            od_initialized = initializeObjectDictionary();
            
            // Initializes the Encoder
            bool init_valid = false;
            home_initialized = false;
            int init_eposition;
            if (external_position_mode) {
                // Read the external sensor
                if (MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_EXTERNAL_POSITION] != MotorConfig::MOTOR_EXTERNAL_UNDEFINED_POSITION) {
                    update_external_position();
                    init_eposition = convert_User_To_Encoder(external_uposition);
                    init_valid = true;
                }
            }
            else {
                // Gets the initial position of the encoder. If the position is a valid position the homing is not necessary
                if (MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_CURRENT_POSITION] != MotorConfig::MOTOR_UNDEFINED_POSITION) {
                    init_eposition = convert_User_To_Encoder(System::Convert::ToInt32(MotorConfig::Configuration->getParam(config_param)[MotorConfig::PARAM_CURRENT_POSITION]));
                    init_valid = true;
                }
            }

            if((init_valid) && (setEncoderCommand(init_eposition)) ) home_initialized = true;           
            configuration_command = false;

            if (!home_initialized) {
                Notify::activate(error_homing);
                LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ENCODER NOT INITIALIZED");                
            }
            else {
                Notify::deactivate(error_homing);
            }

            previous_uposition = getCurrentUposition();

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
    LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: INITIALIZATION");

    
    while(!blocking_writeOD(OD_4013_01,1)) ; // Hardware configuration : 1 = EXTERNAL VCC LOGIC ON
    while (!blocking_writeOD(OD_2300_00, 0)); // Stops early the NanoJ program
    

    // NMT Behavior in case of fault
    while (!blocking_writeOD(OD_1029_01, 0)) ;
    while (!blocking_writeOD(OD_1029_02, 1)) ;
    while (!blocking_writeOD(OD_2031_00, 5000)) ;  // Peak current
    while (!blocking_writeOD(OD_2032_00, 5000)) ;  // Maximum Speed
    while (!blocking_writeOD(OD_2033_00, 0)) ;	    // Plunger Block
    while (!blocking_writeOD(OD_2034_00, 51500)) ; // Upper Voltage Warning Level
    while (!blocking_writeOD(OD_2035_00, 35000)) ; // ****************************************Lower Voltage Warning Level
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
    while (!blocking_writeOD(OD_605A_00, 1)) ;   // 0 Immediate stop  // 1 Braking with "slow down ramp"  // 2 Braking with "quick stop ramp"

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
    while (!blocking_writeOD(OD_607B_01, convert_User_To_Encoder(min_position - 10))); 	// Min Position Range Limit
    while (!blocking_writeOD(OD_607B_02, convert_User_To_Encoder(max_position + 10)));	// Max Position Range Limit

    // Software Position Limit
    while (!blocking_writeOD(OD_607D_01, convert_User_To_Encoder(min_position))); 	// Min Position Limit
    while (!blocking_writeOD(OD_607D_02, convert_User_To_Encoder(max_position)));	    // Max Position Limit

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
    unsigned short od_code = initializeSpecificObjectDictionaryCallback();
    if(od_code == 0) return false;

    // Read the Object dictionary flag initialization
    while (!blocking_readOD(CONFIG_USER_PARAM)) ; // Reads the user parameter containing the stored nanoj checksum
    if (rxSdoRegister->data == od_code) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: INITIALIZED TO " + od_code.ToString());
        return true;
    }
    
    LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: STORING DATA IN FLASH WITH OD-CODE=" + od_code.ToString());
    
    // Save the Flag in the User Param register and store the User param space
    blocking_writeOD(CONFIG_USER_PARAM, od_code);
    blocking_writeOD(OD_2700_01, 1);

    // Save the whole object dictionary
    blocking_writeOD(OD_1010_01, OD_SAVE_CODE);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    int i;
    for (i = 30; i > 0; i--) {
        if((blocking_readOD(OD_1010_01)) && (rxSdoRegister->data == 1)) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (i) LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: OBJECT DICTIONARY STORED IN FLASH");
    else LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: FAILED STORING OD IN FLASH");    
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
    encoder_eposition = rxSdoRegister->data;
    encoder_uposition = convert_Encoder_To_User(encoder_eposition);
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
    double s = abs((double)encoder_uposition - (double)target);

     
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


    if (
        (cmd == MotorCommands::MOTOR_AUTO_POSITIONING) ||
        (cmd == MotorCommands::MOTOR_MANUAL_POSITIONING) ||
        (cmd == MotorCommands::MOTOR_MANUAL_SERVICE))
    {
        if (isEncoderInitialized()) {
            MotorConfig::Configuration->setParam(config_param, MotorConfig::PARAM_CURRENT_POSITION, getCurrentUposition().ToString());
            MotorConfig::Configuration->storeFile();
        }
    }
    completedCallback(command_id, cmd, getCurrentUposition(), term_code);
    
    if (command_completed_code != MotorCompletedCodes::COMMAND_SUCCESS) {
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + "> ERROR: "+ command_completed_code.ToString());
    }

    return;
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

void CanOpenMotor::demoLoop(void) {
    MotorCompletedCodes returned_code, termination_code;
    int _100ms_time;
    int unit_step;
    bool error_safety = false;
    int uposition;

    returned_code = idleCallback();
    
    // Test if the manual activation can be done
    if (returned_code == MotorCompletedCodes::COMMAND_PROCEED) {

        // If the safety condition prevent the command execution it is immediatelly aborted
        Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device_id);
        if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
            returned_code = MotorCompletedCodes::ERROR_SAFETY; // Priority over the limit switch
        }

        // Evaluates the manual termination condition
        if (returned_code == MotorCompletedCodes::COMMAND_PROCEED) {
            // handle the manual hardware inputs
            motor_rotation_activations manual_req = Gantry::getManualActivationRequestState(device_id);
            if (manual_req == motor_rotation_activations::MOTOR_INCREASE) {
                if (!activateManualPositioning(max_position)) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
            }
            else if (manual_req == motor_rotation_activations::MOTOR_DECREASE) {
                if (!activateManualPositioning(min_position)) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
            }
        }

    }



    // Initiate a requested command
    switch (request_command) {
    case MotorCommands::MOTOR_AUTO_POSITIONING:
        uposition = getCurrentUposition();

        // Idle status prevents to proceed
        if (returned_code != MotorCompletedCodes::COMMAND_PROCEED) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: DEMO AUTO POSITIONING FAILED DUE TO IDLE");
            encoder_eposition = convert_User_To_Encoder(uposition);
            setCommandCompletedCode(returned_code);
            request_command = MotorCommands::MOTOR_IDLE;
            return;
            
        }
         
        motionParameterCallback(MotorCommands::MOTOR_AUTO_POSITIONING, uposition, command_target);

        // Preparation
        returned_code = preparationCallback(MotorCommands::MOTOR_AUTO_POSITIONING, uposition, command_target);

        if (returned_code != MotorCompletedCodes::COMMAND_PROCEED) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: DEMO AUTO POSITIONING PREPARATION FAILED!");
            encoder_eposition = convert_User_To_Encoder(uposition);
            setCommandCompletedCode(returned_code);
            request_command = MotorCommands::MOTOR_IDLE;
            return;
        }

        // Starts
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: START DEMO AUTO POSITIONING");
        current_command = request_command;
        abort_request = false;
        previous_uposition = uposition;

        
        _100ms_time = ((command_target - uposition) * 10) / command_speed;
        if (_100ms_time) {
            unit_step = (command_target - uposition) / abs(_100ms_time);

            termination_code = MotorCompletedCodes::COMMAND_SUCCESS;
            for (int i = 0; i < abs(_100ms_time); i++) {
                uposition += unit_step;                
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                
                // Test the abort request flag
                if (abort_request) {
                    abort_request = false;
                    LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: ABORT REQUEST!");
                    termination_code = MotorCompletedCodes::ERROR_COMMAND_ABORTED;
                    break;
                }

                termination_code = runningCallback(MotorCommands::MOTOR_AUTO_POSITIONING, uposition, command_target);
                if (termination_code != MotorCompletedCodes::COMMAND_PROCEED) break;

                // If the safety condition prevent the command execution it is immediatelly aborted
                Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device_id);
                if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
                    LogClass::logInFile("Motor <" + device_id.ToString() + ">: safety condition error > " + safety.ToString());
                    termination_code = MotorCompletedCodes::ERROR_SAFETY;
                    break;
                }

                // Gets the obstacle condition
                if (Gantry::getObstacleRotationStatus(device_id, motor_direction)) {
                    LogClass::logInFile("Motor <" + device_id.ToString() + ">: obstacle condition error");
                    termination_code = MotorCompletedCodes::ERROR_OBSTACLE_DETECTED;
                    break;
                }

                external_uposition = encoder_uposition = uposition;
                encoder_eposition = convert_User_To_Encoder(uposition);
            }
        }

        external_uposition = encoder_uposition = uposition;
        if (termination_code == MotorCompletedCodes::COMMAND_SUCCESS) {
            external_uposition = encoder_uposition = uposition = command_target;
        }        
        encoder_eposition = convert_User_To_Encoder(uposition);
        
        setCommandCompletedCode(termination_code);
        request_command = MotorCommands::MOTOR_IDLE;
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: DEMO AUTO POSITIONING COMPLETED TO TARGET:" + uposition.ToString());
        break;

    case MotorCommands::MOTOR_MANUAL_POSITIONING:
        
        uposition = getCurrentUposition();

        // Idle status prevents to proceed
        if (returned_code != MotorCompletedCodes::COMMAND_PROCEED) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: DEMO MANUAL POSITIONING FAILED DUE TO IDLE");
            encoder_eposition = convert_User_To_Encoder(uposition);
            setCommandCompletedCode(returned_code);
            request_command = MotorCommands::MOTOR_IDLE;
            return;
        }

        motionParameterCallback(MotorCommands::MOTOR_MANUAL_POSITIONING, uposition, command_target);

        // Preparation
        returned_code = preparationCallback(MotorCommands::MOTOR_MANUAL_POSITIONING, uposition, command_target);
        if (returned_code != MotorCompletedCodes::COMMAND_PROCEED) {
            LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: DEMO MANUAL POSITIONING PREPARATION FAILED!");
            encoder_eposition = convert_User_To_Encoder(uposition);
            setCommandCompletedCode(returned_code);
            request_command = MotorCommands::MOTOR_IDLE;
            return;
        }


        // Starts
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: START DEMO MANUAL POSITIONING");
        current_command = request_command;
        abort_request = false;
        previous_uposition = uposition;


        _100ms_time = ((command_target - uposition) * 10) / command_speed;
        if (_100ms_time) {
            unit_step = (command_target - uposition) / abs(_100ms_time);

            termination_code = MotorCompletedCodes::COMMAND_SUCCESS;
            
            for (int i = 0; i < abs(_100ms_time); i++) {
                uposition += unit_step;                
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
                termination_code = runningCallback(MotorCommands::MOTOR_MANUAL_POSITIONING, uposition, command_target);
                if (termination_code != MotorCompletedCodes::COMMAND_PROCEED) break;

                // If the safety condition prevent the command execution it is immediatelly aborted
                Gantry::safety_rotation_conditions safety = Gantry::getSafetyRotationStatus(device_id);
                if (safety != Gantry::safety_rotation_conditions::GANTRY_SAFETY_OK) {
                    LogClass::logInFile("Motor <" + device_id.ToString() + ">: safety condition error > " + safety.ToString());
                    termination_code = MotorCompletedCodes::ERROR_SAFETY;
                    break;
                }
                
                // handle the manual hardware inputs
                motor_rotation_activations manual_req = Gantry::getManualActivationRequestState(device_id);
                if (motor_direction != manual_req) {
                    termination_code = MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
                    break;
                }
            }
        }

        external_uposition = encoder_uposition = uposition;
        if (termination_code == MotorCompletedCodes::COMMAND_SUCCESS) {
            external_uposition = encoder_uposition = uposition = command_target;
        }
        encoder_eposition = convert_User_To_Encoder(uposition);

        setCommandCompletedCode(termination_code);
        request_command = MotorCommands::MOTOR_IDLE;
        LogClass::logInFile("Motor Device <" + System::Convert::ToString(device_id) + ">: DEMO MANUAL POSITIONING COMPLETED TO TARGET:" + uposition.ToString());
        break;

    case MotorCommands::MOTOR_EXTERNAL_HOMING:
    case MotorCommands::MOTOR_MANUAL_HOMING:
    case MotorCommands::MOTOR_AUTO_HOMING:
        if (external_position_mode) {
            external_uposition = encoder_uposition = command_target;
            external_raw_position = 0;
            external_zero_setting = 0;
            encoder_eposition = convert_User_To_Encoder(command_target);            
        }
        else {
            external_uposition = encoder_uposition = encoder_eposition = 0;
        }
        current_command = request_command;
        abort_request = false;
        home_initialized = true;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        setCommandCompletedCode(MotorCompletedCodes::COMMAND_SUCCESS);
        request_command = MotorCommands::MOTOR_IDLE;
        break;
    
    }

    return;
};