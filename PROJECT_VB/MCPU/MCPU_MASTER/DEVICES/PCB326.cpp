#include "PCB326.h"
#include "Notify.h"
#include "Log.h"
#include <thread>
#include "CalibrationConfig.h"

void PCB326::handleSystemStatusRegister(void) {
    
    static bool rst = false;
    static bool cfg = false;
    static bool cal = false;
    static bool run = false;


    Register^ system_status_register = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_STATUS_REGISTER);
    if (system_status_register == nullptr) return;

    if (PCB326_GET_SYSTEM_RUN_STATUS(system_status_register)) {
        if (cs_status != PCB326_GET_SYSTEM_CS_STATUS(system_status_register)) {
            cs_status = PCB326_GET_SYSTEM_CS_STATUS(system_status_register);

            System::String^ stringa = "PCB326:";
            if(cs_status == 0) stringa = "PCB326:  NO SENSORS ACTIVATED";
            else{
                stringa = "PCB326: ACTIVE SENSORS >";
                if (cs_status & 0x1) stringa += " CS1=ON";
                if (cs_status & 0x2) stringa += " CS2=ON";
                if (cs_status & 0x4) stringa += " CS3=ON";
                if (cs_status & 0x8) stringa += " CS4=ON";
                if (cs_status & 0x10) stringa += " CS5=ON";
                if (cs_status & 0x20) stringa += " CS6=ON";
                if (cs_status & 0x40) stringa += " CS7=ON";
                if (cs_status & 0x80) stringa += " CS8=ON";
                LogClass::logInFile(stringa);
            }            
        }
    }
    else {
        cs_status = 0;
    }
   
    if (rst != PCB326_GET_SYSTEM_RESET_STATUS(system_status_register)) {
        rst = PCB326_GET_SYSTEM_RESET_STATUS(system_status_register);

        if (rst) LogClass::logInFile("PCB326: RESET ACTIVATED!");
        else LogClass::logInFile("PCB326: RESET COMPLETED!");
    }

    if (cfg != PCB326_GET_SYSTEM_CONFIG_STATUS(system_status_register)) {
        cfg = PCB326_GET_SYSTEM_CONFIG_STATUS(system_status_register);

        if (cfg) LogClass::logInFile("PCB326: CONFIG ACTIVATED!");
        else LogClass::logInFile("PCB326: CONFIG COMPLETED!");
    }

    if (cal != PCB326_GET_SYSTEM_CALIB_STATUS(system_status_register)) {
        cal = PCB326_GET_SYSTEM_CALIB_STATUS(system_status_register);

        if (cal) LogClass::logInFile("PCB326: CALIB ACTIVATED!");
        else LogClass::logInFile("PCB326: CALIB COMPLETED!");
    }

    

}


/// <summary>
/// This is the configuration loop routine executed at the beginning of the 
/// device connection, before to execute the runningLoop() routine.
/// 
/// 
/// </summary>
/// <param name=""></param>
/// <returns>true if the configuration success</returns>
bool PCB326::configurationLoop(void) {

    selected_gen_sens = (GeneralSensOption) System::Convert::ToByte(MotorConfig::Configuration->getParam(MotorConfig::PARAM_OBSTACLE)[MotorConfig::PARAM_OBSTACLE_GAIN]);
    selected_sensor_sens = (SensorSensOption) System::Convert::ToByte(MotorConfig::Configuration->getParam(MotorConfig::PARAM_OBSTACLE)[MotorConfig::PARAM_OBSTACLE_SENSITIVITY]);
    selected_recalib_time = (RecalibrationOption) System::Convert::ToByte(MotorConfig::Configuration->getParam(MotorConfig::PARAM_OBSTACLE)[MotorConfig::PARAM_OBSTACLE_RECAL_TIME]);
    selected_sensor_ena = System::Convert::ToByte(MotorConfig::Configuration->getParam(MotorConfig::PARAM_OBSTACLE)[MotorConfig::PARAM_OBSTACLE_SENS_ENA]);

    // Upload the Sensitivity parameter
    writeParamRegister((unsigned char)ParamRegisters::SENSITIVITY_PARAMETER_REGISTER, setSensitivitRegister(selected_gen_sens, selected_sensor_sens, selected_recalib_time, selected_sensor_ena));

    // Executes the reset and re-configuraiton of the CP1188 as soon as the device is in running 
    current_command = CommandRegister::RESET_COMMAND;

    LogClass::logInFile("PCB326: CONFIGURED");
    return true;
}


void PCB326::runningLoop(void) {
    static bool commerr = false;

    // Test the communication status
    if (commerr != isCommunicationError()) {
        commerr = isCommunicationError();
        if (isCommunicationError()) {
            Notify::activate(Notify::messages::ERROR_PCB326_COMMUNICATION_ERROR);
        }
        else {
            Notify::deactivate(Notify::messages::ERROR_PCB326_COMMUNICATION_ERROR);
        }
    }
    
    // verifies if the command can be executed
    if (current_command != CommandRegister::NO_COMMAND) {

        switch (current_command) {
        case CommandRegister::RESET_COMMAND:            
            if (resetCommand()) {
                current_command = CommandRegister::NO_COMMAND;
                LogClass::logInFile("PCB326: RESET COMMAND EXECUTED");
            }
            break;

        case CommandRegister::CONFIG_COMMAND:
            writeParamRegister((unsigned char)ParamRegisters::SENSITIVITY_PARAMETER_REGISTER, setSensitivitRegister(selected_gen_sens, selected_sensor_sens, selected_recalib_time, selected_sensor_ena));

            if (configCommand()) {
                current_command = CommandRegister::NO_COMMAND;
                LogClass::logInFile("PCB326: CONFIG COMMAND EXECUTED");
            }
            break;

        case CommandRegister::CALIBRATE_COMMAND:
            if (calibrateCommand()) {
                current_command = CommandRegister::NO_COMMAND;
                LogClass::logInFile("PCB326: CALIBRATE COMMAND EXECUTED");
            }
            break;

        }
    }

    // Reads the status register
    handleSystemStatusRegister();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    return;
}

