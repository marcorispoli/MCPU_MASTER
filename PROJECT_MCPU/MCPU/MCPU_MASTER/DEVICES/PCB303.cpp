
#include "PCB303.h"
#include "PCB302.h"
#include "ExposureModule.h"

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
    if ((selected_filter != protocol.status_register.filter_target_index) || (protocol.status_register.filter_action_status != ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED)) {
        valid_filter_format = false;

        if (commandNoWaitCompletion(protocol.command.encodeSetFilterCommand(selected_filter), 30)) {
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

    // No more attempts can be done after some collimation repetition.
    if (valid_collimation_format) format_collimation_attempt = 0;
    else  if (format_collimation_attempt > 5) {
        valid_collimation_format = false;

        // Activate the selectuion error if necessary
        if (!collimation_select_error) {
            collimation_select_error = true;
            Notify::activate(Notify::messages::ERROR_COLLIMATION_SELECTION_ERROR);
        }
        return;
    }

    // Resets the Selection error
    if (collimation_select_error) {
        collimation_select_error = false;
        format_collimation_attempt = 0;
        Notify::deactivate(Notify::messages::ERROR_COLLIMATION_SELECTION_ERROR);
    }

    // In case the position should undefined a warning message is activated
    if (protocol.status_register.collimation_action_status == ProtocolStructure::StatusRegister::action_code::STAT_UNDEFINED) {
        Notify::activate(Notify::messages::WARNING_COLLIMATOR_OUT_OF_POSITION);
        valid_collimation_format = false;
    }

    // A collimation target is selected       
    switch (collimationMode) {

    case collimationModeEnum::AUTO_COLLIMATION:
        // The selected format depends by the paddle detected and its related collimation format
        current_auto_format = PCB302::getDetectedPaddleCollimationFormat();
        selected_format = current_auto_format;
        break;

    case collimationModeEnum::CUSTOM_COLLIMATION:
        selected_format = current_custom_format;
        break;
    case collimationModeEnum::OPEN_MODE:
        selected_format = 0;
        break;

    default:
        selected_format = 0; // Open as default 
    }

    // Tries to send the format selection command
    if ((selected_format != protocol.status_register.collimation_target_index) || (protocol.status_register.collimation_action_status != ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED)) {
        if (commandNoWaitCompletion(protocol.command.encodeSetFormatCommand(selected_format), 30)) {
            format_collimation_attempt++;
        }
        valid_collimation_format = false;
        return;
    }

    // Activate the Valid collimation flag
    if (!valid_collimation_format) {
        Notify::deactivate(Notify::messages::WARNING_COLLIMATOR_OUT_OF_POSITION);
        valid_collimation_format = true;
    }

    return;
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
    demoLoop();
    return;
    
    // Notify::activate(Notify::messages::WARNING_COLLIMATOR_OUT_OF_POSITION, err_string);
    
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
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    formatManagement();
    filterManagement();
    mirrorManagement();

    //	Tube Register
    protocol.status_register.decodeTubeRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::TUBE_REGISTER));
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
/// This is the configuration loop routine executed at the beginning of the 
/// device connection, before to execute the runningLoop() routine.
/// 
/// 
/// </summary>
/// <param name=""></param>
/// <returns>true if the configuration success</returns>
bool PCB303::configurationLoop(void) {
   
    
    if (isSimulatorMode()) return true;

    // Read the parameters from the configuration files
    System::String^ Param;
   

    for (int index= 0; index < protocol.parameter_register.format_collimation->Length; index++) {
        Param = "COLLI_STANDARD_FORMAT_" + index.ToString();
        protocol.parameter_register.format_collimation[index]->front = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_FRONT]);
        protocol.parameter_register.format_collimation[index]->back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_BACK]);
        protocol.parameter_register.format_collimation[index]->left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_LEFT]);
        protocol.parameter_register.format_collimation[index]->right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_RIGHT]);
        protocol.parameter_register.format_collimation[index]->trap = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(Param)[CollimatorConfig::PARAM_FORMAT_TRAP]);
    }

    // Writes the register to the device
    for (int index = 0; index < protocol.parameter_register.format_collimation->Length; index++) {
        writeParamRegister(index, protocol.parameter_register.encodeFBCollimationSlotRegister(index));
        writeParamRegister(index, protocol.parameter_register.encodeLRCollimationSlotRegister(index));
    }

    for (int index = 0; index < protocol.parameter_register.format_collimation->Length/2; index++) {
        writeParamRegister(index, protocol.parameter_register.encodeTrapCollimationSlotRegister(index));
    }


    return true;
}


void PCB303::setAutoCollimationMode(void) {
    if (Exposures::isXrayRunning()) return;

    if (collimationMode != collimationModeEnum::AUTO_COLLIMATION) {
        valid_collimation_format = false;
        collimationMode = collimationModeEnum::AUTO_COLLIMATION;
    }

}

void PCB303::setOpenCollimationMode(void) {
    if (Exposures::isXrayRunning()) return;
    
    if (collimationMode != collimationModeEnum::OPEN_MODE) {
        collimationMode = collimationModeEnum::OPEN_MODE;
        if ((protocol.status_register.collimation_target_index != 0) || (protocol.status_register.collimation_action_status != ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED)) {
            valid_mirror_format = false;
        }
    }
    
}

/// <summary>
/// This function sets the Custom Collimation mode
/// 
/// </summary>
/// <param name="custom">This is a valid collimation code</param>
void PCB303::setCustomCollimationMode(System::Byte format_index) {
    if (Exposures::isXrayRunning()) return;

    collimationMode = collimationModeEnum::CUSTOM_COLLIMATION;
   
    // Assignes the custom format to the current
    if (format_index >= NUM_COLLIMATION_SLOTS) current_custom_format = NUM_COLLIMATION_SLOTS - 1;
    else current_custom_format = format_index;

    // Removes the valid format if the custom format is different from the selected
    if ((current_custom_format != protocol.status_register.collimation_target_index) || (protocol.status_register.collimation_action_status != ProtocolStructure::StatusRegister::action_code::STAT_POSITIONED)) {
        valid_mirror_format = false;      
    }

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
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return;
}
