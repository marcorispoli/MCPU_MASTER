
#include "PCB303.h"
#include "PCB302.h"
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

        if (commandNoWaitCompletion(protocol.command.encodeSetMirrorCommand((System::Byte) selected_mirror), 30)) {
            mirror_attempt++;
        }
        return;
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
        selected_format = current_auto_format;
        break;
    case collimationModeEnum::CUSTOM_COLLIMATION:
        selected_format = current_custom_format;
        break;
    case collimationModeEnum::OPEN_MODE:
        selected_format = 255;
        break;

    default:
        selected_format = 255;
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
/// This function returns the Collimation format assigned to a Paddle detected in the System 
/// by the Compressor device.
/// 
/// If the Paddle should not be detected, the COLLI_STANDARD1 Standard collimation is selected.
/// 
/// </summary>
/// <param name=""></param>
/// <returns>returns the Format collimation assigned to the current paddle</returns>
PCB303::ColliStandardSelections PCB303::getAutomaticStandardFormatIndex(void) {

    // Get the collimation format of the detected paddle
    int format = PCB302::getDetectedPaddleCollimationFormat();

    // If the paddle is not detected the -1 code is returned
    if( format < 0) return ColliStandardSelections::COLLI_INVALID_FORMAT;

    // If the collimation format is not valid the invalid code is returned
    if( format > (int) ColliStandardSelections::COLLI_STANDARD_LEN) return ColliStandardSelections::COLLI_INVALID_FORMAT;

    // Returns the related collimation format
    return (ColliStandardSelections) format;
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
    unsigned short front, trap, left, right, back;
  if (isSimulatorMode()) return true;

    front = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD_FT)[CollimatorConfig::PARAM_COLLI_STANDARD_FT_FRONT]);
    trap = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD_FT)[CollimatorConfig::PARAM_COLLI_STANDARD_FT_TRAP]);
    while(!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_FT(front, trap))) ;


    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD1)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD1)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD1)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(1, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(1, back)));
    
    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD2)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD2)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD2)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(2, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(2, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD3)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD3)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD3)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(3, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(3, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD4)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD4)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD4)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(4, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(4, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD5)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD5)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD5)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(5, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(5, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD6)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD6)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD6)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(6, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(6, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD7)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD7)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD7)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(7, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(7, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD8)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD8)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD8)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(8, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(8, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD9)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD9)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD9)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(9, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(9, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD10)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD10)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD10)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(10, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(10, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD11)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD11)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD11)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(11, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(11, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD12)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD12)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD12)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(12, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(12, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD13)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD13)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD13)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(13, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(13, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD14)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD14)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD14)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(14, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(14, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD15)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD15)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD15)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(15, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(15, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD16)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD16)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD16)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(16, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(16, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD17)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD17)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD17)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(17, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(17, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD18)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD18)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD18)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(18, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(18, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD19)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD19)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD19)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(19, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(19, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD20)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD20)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD20)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_LR(20, left, right)));
    while (!writeParamRegister(PCB303_WRITE_PARAM_STANDARD_B(20, back)));

    return true;
}


void PCB303::setAutoCollimationMode(void) {
    valid_collimation_format = false;
    collimationMode = collimationModeEnum::AUTO_COLLIMATION;
}

void PCB303::setOpenCollimationMode(void) {
    valid_collimation_format = false;
    collimationMode = collimationModeEnum::OPEN_MODE;
}

/// <summary>
/// This function sets the Custom Collimation mode
/// 
/// </summary>
/// <param name="custom">This is a valid collimation code</param>
void PCB303::setCustomCollimationMode(ColliStandardSelections custom) {
    valid_collimation_format = false;
    if (custom == ColliStandardSelections::COLLI_NOT_STANDARD) custom = ColliStandardSelections::COLLI_STANDARD1;
    customStandardSelection = custom;
    collimationMode = collimationModeEnum::CUSTOM_COLLIMATION;

}

/// <summary>
/// This command sets the CALIBRATION collimation mode
/// </summary>
/// <param name="blades">This is the register containing the blades values</param>
void PCB303::setCalibrationCollimationMode(formatBlades^ blades){
    valid_collimation_format = false;
    calibrationBlades.copy(blades);
    collimationMode = collimationModeEnum::CALIBRATION_MODE;

}

void PCB303::setTomoCollimationMode(void) 
{};

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
