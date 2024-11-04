
#include "PCB304.h"
#include "Notify.h"
#include <thread>

void PCB304::runningLoop(void) {
    static bool commerr = false;
    static int  error_count = 0;
    static bool grid_error = false;
    static bool keepalive = false;
    static bool grid_out_of_position = false;
    static bool inout_auto_mode = false;
    static bool general_enable = false;
 
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

    // Read the Grid Status register
    protocol.status_register.decodeGridRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::GRID));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    // System Error Message activation
    if (grid_error != protocol.status_register.error) {
        grid_error = protocol.status_register.error;

        if(grid_error) Notify::activate(Notify::messages::ERROR_POTTER_GRID_POSITION);
        else Notify::deactivate(Notify::messages::ERROR_POTTER_GRID_POSITION);
    }

    // Reads the special error register from the device in case an error should be signaled
    if (protocol.status_register.error) {
        readErrorRegister();

        error_count++;
        if (error_count < max_num_error_resets) {

            // Reset the error code 
            device->commandNoWaitCompletion(protocol.command.encodeResetCommand(), 30);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    else {
        error_count = 0;
    }


    // Info on the current InOut Positioning mode
    if (inout_auto_mode != protocol.data_register.InOutAutoEnable) {
        inout_auto_mode = protocol.data_register.InOutAutoEnable;

        if(!inout_auto_mode) Notify::activate(Notify::messages::INFO_GRID_INOUT_MANUAL_MODE);
        else  Notify::deactivate(Notify::messages::INFO_GRID_INOUT_MANUAL_MODE);
    }
 
    // Evaluates the General Enable condition
    if (general_enable != protocol.data_register.GeneralEnable) {
        general_enable = protocol.data_register.GeneralEnable;

        if(general_enable) Notify::deactivate(Notify::messages::WARNING_GRID_GENERAL_ENABLE);
        else {
            Notify::activate(Notify::messages::WARNING_GRID_GENERAL_ENABLE);

            // Deactivate the Out Of Position Warning
            Notify::deactivate(Notify::messages::WARNING_GRID_OUT_OF_POSITION);
            grid_out_of_position = false;

        }
    }

    // Whe the General enable bit is active, the current position is evaluated
    if (protocol.data_register.GeneralEnable) {

        // Evaluates the current grid position
        bool out_of_position = false;

        // Almost one status should be present
        if ((!protocol.status_register.inField) && (!protocol.status_register.outField)) out_of_position = true;

        // Auto mode requires a constant status
        if ((protocol.data_register.InOutAutoEnable) && (protocol.data_register.InOutStatus != protocol.status_register.inField)) out_of_position = true;

        // Out Of Position Warning activation
        if (out_of_position != grid_out_of_position) {
            grid_out_of_position = out_of_position;

            if (out_of_position) Notify::activate(Notify::messages::WARNING_GRID_OUT_OF_POSITION);
            else Notify::deactivate(Notify::messages::WARNING_GRID_OUT_OF_POSITION);
        }

    }

   

    

    // Always toggles the keepalive to keep the display ON
    protocol.data_register.display_keep_alive = !protocol.data_register.display_keep_alive;

    // In idle in order to initialize the position, the grid is moved in/Out
    if (test_grid_in_out) {
        test_grid_in_out = false;
        setAutoGridInField();
        writeDataRegister((unsigned char)ProtocolStructure::DataRegister::register_index::GRID_REGISTER, protocol.data_register.encodeGridRegister());
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        setAutoGridOutField();
        writeDataRegister((unsigned char)ProtocolStructure::DataRegister::register_index::GRID_REGISTER, protocol.data_register.encodeGridRegister());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // Sets the display outputs
    writeDataRegister((unsigned char)ProtocolStructure::DataRegister::register_index::DISPLAY_REGISTER, protocol.data_register.encodeDisplayRegister());
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    writeDataRegister((unsigned char)ProtocolStructure::DataRegister::register_index::GRID_REGISTER, protocol.data_register.encodeGridRegister());
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    return;
}


void PCB304::demoLoop(void) {
    
    // If the simulator is not connected, the errors are disabled and the
    // status of the grid is set according with the requested status.
    Notify::deactivate(Notify::messages::INFO_GRID_INOUT_MANUAL_MODE);
    Notify::deactivate(Notify::messages::ERROR_POTTER_GRID_POSITION);
    Notify::deactivate(Notify::messages::WARNING_GRID_OUT_OF_POSITION);
    protocol.status_register.error = false;
    protocol.status_register.inout_executing = false;
    protocol.status_register.transversal_executing = false;

    if (protocol.data_register.InOutAutoEnable) {
        protocol.status_register.inField = protocol.data_register.InOutStatus;
        protocol.status_register.outField = !protocol.data_register.InOutStatus;
    }

    if (protocol.status_register.inField) {
        protocol.status_register.home = true;
        protocol.status_register.center = false;
    }
    else {
        protocol.status_register.home = false;
        protocol.status_register.center = true;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return;
}
