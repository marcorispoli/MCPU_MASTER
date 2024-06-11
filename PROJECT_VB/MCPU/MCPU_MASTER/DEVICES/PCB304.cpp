
#include "PCB304.h"
#include "Notify.h"
#include "Simulator.h"

#include <thread>

void PCB304::runningLoop(void) {
    static bool commerr = false;
    static bool keepalive = false;
 
    // Test the communication status
    if (commerr != isCommunicationError()) {
        commerr = isCommunicationError();
        if (isCommunicationError()) {
            Notify::activate(Notify::messages::ERROR_PCB304_COMMUNICATION_ERROR);
        }
        else {
            Notify::deactivate(Notify::messages::ERROR_PCB304_COMMUNICATION_ERROR);
        }
    }

    // Read the STATUS register
    Register^ system_status_register = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_STATUS_REGISTER);
    if (system_status_register) {
        grid_sync_ready = PCB304_GET_GRID_SYNC_STATUS(system_status_register);
    }

    // To be completed
    if (grid_sync_ready) {
        grid_on_field_ready = true;
        grid_off_field_ready = false;
    }
    else {
        grid_on_field_ready = false;
        grid_off_field_ready = true;
    }
    

    // Always toggles the keepalive to keep the display ON
    if(keepalive)   PCB304_DISPLAY_KEEPALIVE(display_data_register, true); 
    else  PCB304_DISPLAY_KEEPALIVE(display_data_register, false); 
    keepalive = !keepalive;

    // Sets the display outputs
    PCB304_DISPLAY_ON(display_data_register, display_on);
    PCB304_DISPLAY_BLINK(display_data_register, display_blink);
    PCB304_DISPLAY_DOT_POSITION(display_data_register, display_decimals);
    PCB304_DISPLAY_INTENSITY(display_data_register, display_intensity);
    
    short val = display_val;
    PCB304_DISPLAY_CONTENT_LOW(display_data_register, val);
    PCB304_DISPLAY_CONTENT_HIGH(display_data_register, val);
 
    // Udates the DISPLAY DATA REGISTER
    writeDataRegister((unsigned char)DataRegisters::DISPLAY_DATA_REGISTER, display_data_register);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Udates the DISPLAY GRID REGISTER
    writeDataRegister((unsigned char)DataRegisters::GRID_DATA_REGISTER, grid_data_register);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return;
}


void PCB304::demoLoop(void) {
        
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (grid_on_field) grid_on_field_ready = true;
    else grid_off_field_ready = true;

    grid_sync_ready = true;
    error = false;

    // Sets the display outputs to the simulator
    PCB304_DISPLAY_ON(display_data_register, display_on);
    PCB304_DISPLAY_BLINK(display_data_register, display_blink);
    PCB304_DISPLAY_DOT_POSITION(display_data_register, display_decimals);
    PCB304_DISPLAY_INTENSITY(display_data_register, display_intensity);

    short val = display_val;
    PCB304_DISPLAY_CONTENT_LOW(display_data_register, val);
    PCB304_DISPLAY_CONTENT_HIGH(display_data_register, val);

    to_simulator[(int)simul_tx_struct::DISPLAY_D0] = display_data_register->d0;
    to_simulator[(int)simul_tx_struct::DISPLAY_D1] = display_data_register->d1;
    to_simulator[(int)simul_tx_struct::DISPLAY_D2] = display_data_register->d2;
    to_simulator[(int)simul_tx_struct::DISPLAY_D3] = display_data_register->d3;


    // Sets the grid outputs to the simulator ... to be done

    // Evaluates i fthe data has been changed to update the Simulator     
    bool changed = false;
    for (int i = 0; i < to_simulator->Length; i++) {
        if (to_simulator[i] != to_simulator_previous[i]) {
            changed = true;
            to_simulator_previous[i] = to_simulator[i];
        }
    }

    if (changed) simulSend();
    return;
}


bool PCB304::setGridOnField(bool wait_completion) { 
    grid_on_field = true; 

    // Returns immediatelly if the function shall not wait for the command completion
    if (!wait_completion) return true;

    int timeout = 50;
    while (timeout--) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        if (PCB304::isError()) {
            LogClass::logInFile("PCB304: error during On field grid positioning");
            return false;
        }

        if (!PCB304::isGridOnFieldReady()) continue;
        
        // Grin On field
        return true;
    }

    LogClass::logInFile("PCB304: timeout waiting for the grid On Field");
    return false;


}

bool PCB304::setGridOffField(bool wait_completion) {
    grid_on_field = false; 

    // Returns immediatelly if the function shall not wait for the command completion
    if (!wait_completion) return true;

    int timeout = 50;
    while (timeout--) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        if (PCB304::isError()) {
            LogClass::logInFile("PCB304: error during OFF field grid positioning");
            return false;
        }

        if (!PCB304::isGridOffFieldReady()) continue;

        // Grin Off field
        return true;
    }

    LogClass::logInFile("PCB304: timeout waiting for the grid OFF Field");
    return false;

}



void PCB304::simulRx(cli::array<System::Byte>^ receiveBuffer, int index, int rc) {
    if (rc != (int)simul_rx_struct::BUFLEN) return;

    for (int i = 0; i < rc; i++) device->from_simulator[i] = receiveBuffer[index + i];
}

void PCB304::simulSend(void) {

    to_simulator[0] = 0x3;
    to_simulator[1] = (int)simul_tx_struct::BUFLEN;
    to_simulator[(int)simul_tx_struct::DEVICE_ID] = PCB304_DEVID;
    to_simulator[(int)simul_tx_struct::ENDFRAME] = 0x2;

    // Sends the buffer
    Simulator::device->send(to_simulator);
}

void PCB304::simulInit(void) {

    // Create the Simulator structure if shuld be  necessary
    from_simulator = gcnew cli::array<System::Byte>((int)PCB304::simul_rx_struct::BUFLEN);
    to_simulator = gcnew cli::array<System::Byte>((int)PCB304::simul_tx_struct::BUFLEN);
    to_simulator_previous = gcnew cli::array<System::Byte>((int)PCB304::simul_tx_struct::BUFLEN);

    // Initialize the from_simulator vector..
    //from_simulator[(int)simul_rx_struct::PADDLE_CODE] = (System::Byte)paddleCodes::PADDLE_NOT_DETECTED;
    
    // Connects the simulator reception event
    Simulator::device->pcb304_rx_event += gcnew Simulator::rxData_slot(&PCB304::simulRx);

}
