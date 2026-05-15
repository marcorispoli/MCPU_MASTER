
#include "PCB303.h"
#include "PCB302.h"
#include "ExposureModule.h"
#include "CanSimulator.h"
#include "ConfigurationFiles.h"
#include "MESSAGES/Notify.h"
#include <thread>

void PCB303::mirrorManagement(void) {
    static bool mirror_select_error = false;

    // No operation if a protocol command is processing
    if (!device->isCommandCompleted()) return;

    // No operation if the format collimation is processing, even if the format is not the current selected!
    if (protocol.status_register.mirror_action_status == ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING) {
        valid_mirror_format = false;
        return;
    }

    // No more attempts can be done after some collimation repetition.
    if (valid_mirror_format) mirror_attempt = 0;
    else  if (mirror_attempt > 5) {
        valid_mirror_format = false;

        // Activate the selectuion error if necessary
        if (!mirror_select_error) {
            mirror_select_error = true;
            Notify::activate(Notify::messages::ERROR_MIRROR_SELECTION_ERROR);
        }
        return;
    }

    // Resets the Selection error
    if (mirror_select_error) {
        mirror_select_error = false;
        mirror_attempt = 0;
        Notify::deactivate(Notify::messages::ERROR_MIRROR_SELECTION_ERROR);
    }

    // In case the position should undefined a warning message is activated
    if (protocol.status_register.mirror_action_status == ProtocolStructure::StatusRegister::action_code::STAT_UNDEFINED) {
        Notify::activate(Notify::messages::WARNING_MIRROR_OUT_OF_POSITION);
        valid_mirror_format = false;
    }


    // Tries to send the format selection command
    if ((selected_mirror != protocol.status_register.mirror_target_index) || (protocol.status_register.mirror_action_status != ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED)) {
        valid_mirror_format = false;
        retrigger_light_on_command = false;

        if (commandNoWaitCompletion(protocol.command.encodeSetMirrorCommand((System::Byte) selected_mirror), 30)) {
            mirror_attempt++;
        }
        return;
    }
    
    // If the Mirror is already in field and the App whould only retrigger the power light ON
    if (retrigger_light_on_command) {
        retrigger_light_on_command = false;
        commandNoWaitCompletion(protocol.command.encodeSetLightCommand((System::Byte)ProtocolStructure::StatusRegister::light_target_code::LIGHT_ON), 30);
    }

    // Activate the Valid collimation flag
    if (!valid_mirror_format) {
        Notify::deactivate(Notify::messages::WARNING_MIRROR_OUT_OF_POSITION);
        valid_mirror_format = true;
    }

    return;
}


void PCB303::filterManagement(void) {
    static bool filter_select_error = false;

    // No operation if a protocol command is processing
    if (!device->isCommandCompleted()) return;

    // No operation if the format collimation is processing, even if the format is not the current selected!
    if (protocol.status_register.filter_action_status == ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING) {
        valid_filter_format = false;
        return;
    }

    // In the case of Disabled mode, all the flags are reset and no command to the device is handled
    if (filterMode == filterModeEnum::DISABLED_FILTER_MODE) {
        filter_attempt = 0;
        valid_filter_format = false;
        Notify::deactivate(Notify::messages::ERROR_FILTER_SELECTION_ERROR);
        Notify::deactivate(Notify::messages::WARNING_FILTER_OUT_OF_POSITION);
        return;
    }
    
    // No more attempts can be done after some collimation repetition.
    
    if (valid_filter_format) filter_attempt = 0;
    else  if (filter_attempt > 5) {
        valid_filter_format = false;

        // Activate the selectuion error if necessary
        if (!filter_select_error) {
            filter_select_error = true;
            Notify::activate(Notify::messages::ERROR_FILTER_SELECTION_ERROR);     
        }
        return;
    }

    // Resets the Selection error
    if (filter_select_error) {
        filter_select_error = false;
        filter_attempt = 0;
        Notify::deactivate(Notify::messages::ERROR_FILTER_SELECTION_ERROR);
    }

    // In case the position should undefined a warning message is activated
    if (protocol.status_register.filter_action_status == ProtocolStructure::StatusRegister::action_code::STAT_UNDEFINED) {
        Notify::activate(Notify::messages::WARNING_FILTER_OUT_OF_POSITION);
        valid_filter_format = false;
    }
    

    // Tries to send the format selection command
    if ((selected_slot != protocol.status_register.filter_target_index) || (protocol.status_register.filter_action_status != ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED)) {
        valid_filter_format = false;

        if (commandNoWaitCompletion(protocol.command.encodeSetFilterCommand(selected_slot), 30)) {
            filter_attempt++;         
        }        
        return;
    }

    // Activate the Valid collimation flag
    if (!valid_filter_format) {
        Notify::deactivate(Notify::messages::WARNING_FILTER_OUT_OF_POSITION);
        valid_filter_format = true;
    }

    return;
}


void PCB303::formatManagement(void) {    
    static bool collimation_select_error = false;

    // No operation if a protocol command is processing
    if (!device->isCommandCompleted()) return;

    // No operation if the format collimation is processing, even if the format is not the current selected!
    if (protocol.status_register.collimation_action_status == ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING) {
        valid_collimation_format = false;
        return;
    }

    // Only with a valid selection, the current selection status is evaluated
    if (selected_format >= 0){
        // In case the position should undefined a warning message is activated
        if (protocol.status_register.collimation_action_status == ProtocolStructure::StatusRegister::action_code::STAT_UNDEFINED) {
            Notify::activate(Notify::messages::WARNING_COLLIMATOR_OUT_OF_POSITION);
            valid_collimation_format = false;
        }        
    }


    // A collimation target is selected  
    int curselection = selected_format;
    switch (collimationMode) {

    case collimationModeEnum::AUTO:
        
        // The selected format depends by the paddle detected and its related collimation format
        current_auto_format = PCB302::getDetectedPaddleCollimationFormat();       
        selected_format = current_auto_format;
        break;

    case collimationModeEnum::CUSTOM:
       
        selected_format = current_custom_format;
        break;

    case collimationModeEnum::OPEN:
        selected_format = 0;
        break;

    case collimationModeEnum::DISABLED:

        selected_format = -1;
        break;

    case collimationModeEnum::TOMO:

        selected_format = -1;
        break;

    default:
        selected_format = -1; 
    }

    // ALWAYS: Resets of the errors if the selection is changed
    if (selected_format != curselection) {
        format_collimation_attempt = 0;
        valid_collimation_format = false;
        collimation_select_error = false;
        Notify::deactivate(Notify::messages::ERROR_COLLIMATION_SELECTION_ERROR);
        Notify::deactivate(Notify::messages::WARNING_COLLIMATOR_OUT_OF_POSITION);
    }

    // There is not a valid format selection: no action shall be initiated
    if (selected_format < 0) {
        format_collimation_attempt = 0;
        valid_collimation_format = false;
        collimation_select_error = false;      
        return;            
    }
    

    // A maximum attemp has been expired: no more collimation attempt can be initiated
    if(format_collimation_attempt > 5) {

        // Activate the selectuion error if necessary
        if (!collimation_select_error) {
            collimation_select_error = true;
            Notify::activate(Notify::messages::ERROR_COLLIMATION_SELECTION_ERROR);
        }
        return;
    }

    // A new collimation attempt can be initiated
    if ((force_collimation_command) || (selected_format != protocol.status_register.collimation_target_index) || (protocol.status_register.collimation_action_status != ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED)) {
        
        if (force_collimation_command) {
            if (commandNoWaitCompletion(protocol.command.encodeSetManualFormatCommand(selected_format), 30)) {
                format_collimation_attempt++;
                force_collimation_command = false;
            }
        }
        else {
            if (commandNoWaitCompletion(protocol.command.encodeSetFormatCommand(selected_format), 30)) {
                format_collimation_attempt++;
            }
        }
        
        valid_collimation_format = false;
        return;
    }

    // Activate the Valid collimation flag
    if (!valid_collimation_format) {
        Notify::deactivate(Notify::messages::WARNING_COLLIMATOR_OUT_OF_POSITION);
        valid_collimation_format = true;
        format_collimation_attempt = 0;
    }

    return;
}

void PCB303::tubeManagement(void) {

    // Assignes the current Tube Stator and Bulb temperatures
    int val = protocol.status_register.bulb_temp;
    if (val > 100) val = 100;
    if (val < 0) val = 0;    
    bulb_temperature_perc = val;

    val = protocol.status_register.stator_temp;
    if (val > 100) val = 100;
    if (val < 0) val = 0;
    stator_temperature_perc = val;

    // Calculates the maximum percent
    if (stator_temperature_perc > bulb_temperature_perc) max_temperature_perc = stator_temperature_perc;
    else  max_temperature_perc = bulb_temperature_perc;
}


/// <summary>
/// This is the main loop running after the device configuration.
/// 
/// The procedure reads the relevant registers from the device 
/// and manages the following collimator main workflows:
/// + handles the format collimation;
/// + handle the Tomo Dynamic collimation;
/// 
/// </summary>
/// <param name=""></param>
void PCB303::runningLoop(void) {
    static bool commerr = false;
   
    // Test the communication status
    if (commerr != isCommunicationError()) {
        commerr = isCommunicationError();
        if (isCommunicationError()) {
            Notify::activate(Notify::messages::ERROR_PCB303_COMMUNICATION_ERROR);
        }
        else {
            Notify::deactivate(Notify::messages::ERROR_PCB303_COMMUNICATION_ERROR);
        }
    }


    //	System Register
    protocol.status_register.decodeSystemRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::SYSTEM_REGISTER));
    formatManagement();
    filterManagement();
    mirrorManagement();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    //	Tube Register
    protocol.status_register.decodeTubeRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::TUBE_REGISTER));
    tubeManagement();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));


    return;


   
}



/// <summary>
/// 
/// </summary>
/// 
/// This function activate a One Shot alarm notifying the 
/// operator that the device has been reset. 
/// 
/// 
/// <param name=""></param>
void PCB303::resetLoop(void) {

    // Ths error is a one shot error: it is reset as soon as the operator open the error window
    Notify::activate(Notify::messages::ERROR_PCB303_RESET);
}

/// <summary>
/// This function returns the assigned slot of the requested collimation format
/// </summary>
/// <param name="format_name">A name of the collimation format</param>
/// <returns>
/// + -1: invalid format name
/// + >=0: collimation slot
/// 
/// </returns>
int  PCB303::getCollimationSlot(System::String^ format_name) {
    
    if (format_name == "COLLI_CUSTOM") return protocol.parameter_register.format_collimation->Length - 1; // last collimation slot reserved

    // Gets the paddle code from the paddle name (tag)     
    PCB302::paddleCodes paddle = PCB302::getPaddleCode(format_name);
    if (paddle == PCB302::paddleCodes::PADDLE_NOT_DETECTED) return -1;

    // Gets the collimation slot
   return PCB302::getPaddleCollimationFormat(paddle);


}

PCB303::formatData^ PCB303::getFormatConfiguration(int slot) {
    if (slot < 0) return nullptr;
    if (slot >= protocol.parameter_register.format_collimation->Length) return nullptr;

    formatData^ blades = gcnew formatData();
    blades->slot = slot;
    blades->front = protocol.parameter_register.format_collimation[slot]->front;
    blades->back = protocol.parameter_register.format_collimation[slot]->back;
    blades->left = protocol.parameter_register.format_collimation[slot]->left;
    blades->right = protocol.parameter_register.format_collimation[slot]->right;

    return blades;
}

bool PCB303::setFormatConfiguration(int slot, int front, int back, int left, int right, int trap, bool store) {
    if (slot < 0) return false;
    if (slot >= protocol.parameter_register.format_collimation->Length) return false;

    // Upload parameters related to the modified slot
    protocol.parameter_register.format_collimation[slot]->front = front;
    protocol.parameter_register.format_collimation[slot]->back = back;
    protocol.parameter_register.format_collimation[slot]->left = left;
    protocol.parameter_register.format_collimation[slot]->right = right;
    protocol.parameter_register.format_collimation[slot]->trap = trap;

    
    device->writeParamRegister((System::Byte)ProtocolStructure::ParameterRegister::register_index::FB_FORMAT_SLOT_IDX + slot, protocol.parameter_register.encodeFBCollimationSlotRegister(slot));
    device->writeParamRegister((System::Byte)ProtocolStructure::ParameterRegister::register_index::LR_FORMAT_SLOT_IDX + slot, protocol.parameter_register.encodeLRCollimationSlotRegister(slot));    
    device->writeParamRegister((System::Byte)ProtocolStructure::ParameterRegister::register_index::TR_FORMAT_SLOT_IDX + (slot/2), protocol.parameter_register.encodeTrapCollimationSlotRegister(slot/2));

   // Store if requested in the Configuratin file
    if (store) {
        System::String^ Param = "COLLI_STANDARD_FORMAT_" + slot.ToString();
        CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FORMAT_FRONT, front.ToString());
        CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FORMAT_BACK, back.ToString());
        CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FORMAT_LEFT, left.ToString());
        CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FORMAT_RIGHT, right.ToString());
        CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FORMAT_TRAP, trap.ToString());
        CollimatorConfig::Configuration->storeFile();
    }

    return true;
}

bool PCB303::storeCollimationFormat(int slot) {
    if (slot < 0) return false;
    if (slot >= protocol.parameter_register.format_collimation->Length) return false;

    System::String^ Param = "COLLI_STANDARD_FORMAT_" + slot.ToString();
    CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FORMAT_FRONT, protocol.parameter_register.format_collimation[slot]->front.ToString());
    CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FORMAT_BACK, protocol.parameter_register.format_collimation[slot]->back.ToString());
    CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FORMAT_LEFT, protocol.parameter_register.format_collimation[slot]->left.ToString());
    CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FORMAT_RIGHT, protocol.parameter_register.format_collimation[slot]->right.ToString());
    CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FORMAT_TRAP, protocol.parameter_register.format_collimation[slot]->trap.ToString());
    CollimatorConfig::Configuration->storeFile();

    return true;
}

void PCB303::loadFormatCollimationConfiguration(bool read_file, bool upload_device) {
    
    System::String^ Param;

    if (read_file) {
        // Upload the configuration file content into the Proocol registers
        for (int index = 0; index < protocol.parameter_register.format_collimation->Length; index++) {
            Param = "COLLI_STANDARD_FORMAT_" + index.ToString();
            protocol.parameter_register.format_collimation[index]->front = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_FRONT]);
            protocol.parameter_register.format_collimation[index]->back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_BACK]);
            protocol.parameter_register.format_collimation[index]->left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_LEFT]);
            protocol.parameter_register.format_collimation[index]->right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_RIGHT]);
            protocol.parameter_register.format_collimation[index]->trap = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_TRAP]);
        }

    }

    if (upload_device) {
        // Writes the first 20 register of the Front-Back datas
        for (int index = 0; index < NUM_COLLIMATION_SLOTS; index++) {
            device->writeParamRegister((System::Byte)ProtocolStructure::ParameterRegister::register_index::FB_FORMAT_SLOT_IDX + index, protocol.parameter_register.encodeFBCollimationSlotRegister(index));

        }

        // Writes the next 20 registers of the Left-Right datas
        for (int index = 0; index < NUM_COLLIMATION_SLOTS; index++) {
            device->writeParamRegister((System::Byte)ProtocolStructure::ParameterRegister::register_index::LR_FORMAT_SLOT_IDX + index, protocol.parameter_register.encodeLRCollimationSlotRegister(index));
        }

        // Writes the next 10 registers of the Trap datas
        for (int index = 0; index < NUM_COLLIMATION_SLOTS / 2; index++) {
            device->writeParamRegister((System::Byte)ProtocolStructure::ParameterRegister::register_index::TR_FORMAT_SLOT_IDX + index, protocol.parameter_register.encodeTrapCollimationSlotRegister(index));
        }
    }
    
}

void PCB303::loadCollimatorLightConfiguration(bool read_file, bool upload_device) {

    System::String^ Param;

    if (read_file) {
        protocol.parameter_register.mirror_slot = System::Convert::ToUInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_MIRROR)[CollimatorConfig::MIRROR_INFIELD]);
    }

    if (upload_device) {
        // Writes the next register of the Mirror data
        device->writeParamRegister((System::Byte)ProtocolStructure::ParameterRegister::register_index::MIRROR_SLOT_IDX, protocol.parameter_register.encodeMirrorRegister());
    }

}



bool PCB303::setLightConfiguration(int mirror_position, bool store) {
    if (mirror_position > 10000) return false;
    if (mirror_position < 0) return false;

    protocol.parameter_register.mirror_slot = mirror_position;
    device->writeParamRegister((System::Byte)ProtocolStructure::ParameterRegister::register_index::MIRROR_SLOT_IDX, protocol.parameter_register.encodeMirrorRegister());

    // Store if requested in the Configuratin file
    if (store) {
        System::String^ Param = "PARAM_MIRROR";
        CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::MIRROR_INFIELD , mirror_position.ToString());
        CollimatorConfig::Configuration->storeFile();
    }

    return true;
}

void PCB303::storeLightConfiguration(void) {
        System::String^ Param = "PARAM_MIRROR";
        CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::MIRROR_INFIELD, protocol.parameter_register.mirror_slot.ToString());
        CollimatorConfig::Configuration->storeFile();
}

bool PCB303::setFilterPositionConfiguration(int slot0, int slot1, int slot2, int slot3, int slot4, bool store) {
    if (slot0 < 0) return false;
    if (slot1 < 0) return false;
    if (slot2 < 0) return false;
    if (slot3 < 0) return false;
    if (slot4 < 0) return false;

    // Upload the filter positions from the configuration file
    protocol.parameter_register.filter_slots[0] = slot0;
    protocol.parameter_register.filter_slots[1] = slot1;
    protocol.parameter_register.filter_slots[2] = slot2;
    protocol.parameter_register.filter_slots[3] = slot3;
    protocol.parameter_register.filter_slots[4] = slot4;

    // Store if requested in the Configuratin file
    if (store) {
        System::String^ Param = "PARAM_FILTER_POSITION";
        CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FILTER_POSITION_0,slot0.ToString());
        CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FILTER_POSITION_1, slot1.ToString());
        CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FILTER_POSITION_2, slot2.ToString());
        CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FILTER_POSITION_3, slot3.ToString());
        CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FILTER_POSITION_4, slot4.ToString());
        CollimatorConfig::Configuration->storeFile();
    }

    return true;
}

void PCB303::storeFilterPositionConfiguration(void) {
    System::String^ Param = "PARAM_FILTER_POSITION";
    CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FILTER_POSITION_0, protocol.parameter_register.filter_slots[0].ToString());
    CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FILTER_POSITION_1, protocol.parameter_register.filter_slots[1].ToString());
    CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FILTER_POSITION_2, protocol.parameter_register.filter_slots[2].ToString());
    CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FILTER_POSITION_3, protocol.parameter_register.filter_slots[3].ToString());
    CollimatorConfig::Configuration->setParam(Param, CollimatorConfig::PARAM_FILTER_POSITION_4, protocol.parameter_register.filter_slots[4].ToString());
    CollimatorConfig::Configuration->storeFile();
}

void PCB303::loadFilterPositionConfiguration(bool read_file, bool upload_device) {

    System::String^ Param;

    if (read_file) {
        // Upload the filter positions from the configuration file
        protocol.parameter_register.filter_slots[0] = System::Convert::ToUInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_POSITION)[CollimatorConfig::PARAM_FILTER_POSITION_0]);
        protocol.parameter_register.filter_slots[1] = System::Convert::ToUInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_POSITION)[CollimatorConfig::PARAM_FILTER_POSITION_1]);
        protocol.parameter_register.filter_slots[2] = System::Convert::ToUInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_POSITION)[CollimatorConfig::PARAM_FILTER_POSITION_2]);
        protocol.parameter_register.filter_slots[3] = System::Convert::ToUInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_POSITION)[CollimatorConfig::PARAM_FILTER_POSITION_3]);
        protocol.parameter_register.filter_slots[4] = System::Convert::ToUInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_POSITION)[CollimatorConfig::PARAM_FILTER_POSITION_4]);
    }

    if (upload_device) {
        // Writes the next 3 registers of the Filter datas
        for (int index = 0; index < 3; index++) {
            device->writeParamRegister((System::Byte)ProtocolStructure::ParameterRegister::register_index::FILTER_SLOT_IDX + index, protocol.parameter_register.encodeFilterSlotRegister(index));
        }
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
bool PCB303::configurationLoop(void) {
    
    
    LogClass::logInFile("PCB303: Executes Configuration");

    // Upload and Update the data from the configuration file and updates the devices
    loadFormatCollimationConfiguration(true, true);

    // Upload the mirror position from the configuration file
    loadCollimatorLightConfiguration(true, true);

    // Upload the filter position from the configuration file
    loadFilterPositionConfiguration(true, true);

  
    // Sets the Filter in Disable Mode
    PCB303::setFilterMode(filterModeEnum::DISABLED_FILTER_MODE);

    if (device->isSimulatorMode()) CanSimulator::sendFilterConfiguration();

    return true;
}


/// <summary>
/// This function forces the module to reselect the current collimaiton mode.
/// </summary>
/// 
/// The module usually selects a collimation format ones when requested.
/// This function triggers the device to select the collimation ones more.
/// 
/// The function is useful when the configuration file should be changed
/// and the current format position should be refresh with the new 
/// position paramters.
/// 
/// <param name=""></param>
void PCB303::updateCurrentCollimationMode(void) {
    valid_collimation_format = false;
    force_collimation_command = true;
}


void PCB303::setFormatCollimationMode(collimationModeEnum mode, unsigned char format_index) {
    if (Exposures::isXrayRunning()) return;

    if (collimationMode != mode) {
        valid_collimation_format = false;
        collimationMode = mode;
    }

    // Special management for CUSTOM
    if (mode == collimationModeEnum::CUSTOM) {

        // Assignes the custom format to the current
        if (format_index >= NUM_COLLIMATION_SLOTS) current_custom_format = NUM_COLLIMATION_SLOTS - 1;
        else current_custom_format = format_index;

        // Removes the valid format if the custom format is different from the selected
        if (current_custom_format != protocol.status_register.collimation_target_index) {
            valid_collimation_format = false;
        }
    }
}


/// <summary>
/// This function forces the module to reselect the current filter.
/// </summary>
/// 
/// <param name=""></param>
void PCB303::refreshFilter(void) {
    valid_filter_format = false;
}

void PCB303::setFilterMode(filterModeEnum mode) { 
    if (filterMode != mode) {
        valid_filter_format = false;
        filterMode = mode;
    }

    return;
}

void PCB303::setCollimationLight(bool stat) {
    if (Exposures::isXrayRunning()) return;

    if (stat) {
        selected_mirror = ProtocolStructure::StatusRegister::mirror_target_code::IN_FIELD;
        selected_light = ProtocolStructure::StatusRegister::light_target_code::LIGHT_ON;
        retrigger_light_on_command = true;
                
    }
    else {
        selected_mirror = ProtocolStructure::StatusRegister::mirror_target_code::OUT_FIELD;
        selected_light = ProtocolStructure::StatusRegister::light_target_code::LIGHT_OFF;
        retrigger_light_on_command = false;        
    }

    // Removes the valid format if the current status is not the expected status
    if ((selected_mirror != protocol.status_register.mirror_target_index) || (protocol.status_register.mirror_action_status != ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED)) {
        valid_mirror_format = false;     
        retrigger_light_on_command = false;
    }
    
}

int PCB303::getFilterSlot(filter_index filter) {
    if(filter == filter_index::FILTER_AG) return (int) System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_AG_SLOT]);
    if(filter == filter_index::FILTER_AL) return (int) System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_AL_SLOT]);
    if(filter == filter_index::FILTER_RH) return (int) System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_RH_SLOT]);
    if(filter == filter_index::FILTER_CU) return (int) System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_CU_SLOT]);
    if(filter == filter_index::FILTER_MO) return (int) System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_MO_SLOT]);
    if(filter == filter_index::FILTER_LD) return (int) System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_LD_SLOT]);
    return 0;
}

PCB303::filter_index PCB303::getFilterIndex(int slot) {

    if (slot == (int)System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_AG_SLOT]))
        return filter_index::FILTER_AG;

    if (slot == (int)System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_AL_SLOT]))
        return filter_index::FILTER_AL;

    if (slot == (int)System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_RH_SLOT]))
        return filter_index::FILTER_RH;

    if (slot == (int)System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_CU_SLOT]))
        return filter_index::FILTER_CU;

    if (slot == (int)System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_MO_SLOT]))
        return filter_index::FILTER_MO;

    if (slot == (int)System::Convert::ToByte(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_FILTER_CONFIG)[CollimatorConfig::PARAM_FILTER_CONFIG_LD_SLOT]))
        return filter_index::FILTER_LD;

    return filter_index::FILTER_MO;
}

bool PCB303::selectFilter(filter_index filter) {
    
    // Gets the assigned slot from the filter code
    int req_filter_slot = getFilterSlot(filter);    
    if (req_filter_slot > 4) return false;
    if (req_filter_slot < 0) return false;

    selected_slot = req_filter_slot;
    if (selected_filter == filter) return true;
    selected_filter = filter;
    filter_attempt = 0; // Reset error counter
    valid_filter_format = false;
    return true;
}

bool PCB303::selectFilterSlot(int req_filter_slot) {

    // Gets the assigned slot from the filter code
    //int req_filter_slot = getFilterSlot(filter);
    if (req_filter_slot > 4) return false;
    if (req_filter_slot < 0) return false;

    selected_slot = req_filter_slot;

    
    selected_filter = getFilterIndex(req_filter_slot);
    valid_filter_format = false;
    filter_attempt = 0; // Reset error counter
    return true;
}

bool PCB303::selectManualCollimationFormat(int slot) {
    if (device->commandNoWaitCompletion(protocol.command.encodeSetManualFormatCommand(selected_format), 30)) {
        return true;
    }
    else return false;
}


bool PCB303::waitFilterCompleted(void) {
    
    for (int i = 0; i < 200; i++) {
        if (valid_filter_format) return true;
        if (filter_error) return false;
        if (protocol.status_register.filter_action_status == ProtocolStructure::StatusRegister::action_code::STAT_UNDEFINED) return false;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return false;
}

/// <summary>
/// This functions resets the Application fault condition for the format collimation.
/// 
/// The Fault is reset ONLY in the application not in the device.
/// After the fault should be reset, the application can try once more 
/// to set the format collimation.
/// 
/// </summary>
/// <param name=""></param>
void PCB303::resetFaults(void) {
    format_collimation_attempt = 0;
}

void PCB303::demoLoop(void) {
    valid_collimation_format = true;
    valid_filter_format = true;
    valid_mirror_format = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return;
}
