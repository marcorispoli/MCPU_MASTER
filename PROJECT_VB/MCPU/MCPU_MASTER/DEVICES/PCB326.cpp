#include "PCB326.h"
#include "Notify.h"
#include "Log.h"
#include <thread>

void PCB326::handleSystemStatusRegister(void) {
    static bool cs1 = false;
    static bool rst = false;
    static bool cfg = false;
    static bool run = false;


    Register^ system_status_register = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_STATUS_REGISTER);
    if (system_status_register == nullptr) return;

    if (PCB326_GET_SYSTEM_RUN_STATUS(system_status_register)) {
        if (cs1 != PCB326_GET_SYSTEM_CS1_STATUS(system_status_register)) {
            cs1 = PCB326_GET_SYSTEM_CS1_STATUS(system_status_register);

            if (cs1) LogClass::logInFile("PCB326: CS1 ACTIVATED!");
            else LogClass::logInFile("PCB326: CS1 CLEARED!");
        }
    }
    else cs1 = false;


    if (rst != PCB326_GET_SYSTEM_RESET_STATUS(system_status_register)) {
        rst = PCB326_GET_SYSTEM_RESET_STATUS(system_status_register);

        if (rst) LogClass::logInFile("PCB326: RESET ACTIVATED!");
        else LogClass::logInFile("PCB326: RESET CLEARED!");
    }

    if (cfg != PCB326_GET_SYSTEM_CONFIG_STATUS(system_status_register)) {
        cfg = PCB326_GET_SYSTEM_CONFIG_STATUS(system_status_register);

        if (cfg) LogClass::logInFile("PCB326: CONFIG ACTIVATED!");
        else LogClass::logInFile("PCB326: CONFIG CLEARED!");
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

    // Resets the device
    PCB326_OUTPUTS_RESET(outputs_data_register, true);
    writeDataRegister((unsigned char)DataRegisters::OUTPUTS_DATA_REGISTER, outputs_data_register);
    
    LogClass::logInFile("PCB326: RESETTING!");

    Register^ system_status_register; 
    while (true) {
        system_status_register = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_STATUS_REGISTER);
        if (PCB326_GET_SYSTEM_RESET_STATUS(system_status_register)) break;
    }    

    // Configure ..
    LogClass::logInFile("PCB326: CONFIGURING!");

    // Clear the reset
    PCB326_OUTPUTS_RESET(outputs_data_register, false);
    writeDataRegister((unsigned char)DataRegisters::OUTPUTS_DATA_REGISTER, outputs_data_register);

    while (true) {
        system_status_register = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_STATUS_REGISTER);
        if (PCB326_GET_SYSTEM_RUN_STATUS(system_status_register)) break;
    }

    LogClass::logInFile("PCB326: RUN!");
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
    
    handleSystemStatusRegister();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    return;
}

