#include "PCB315.h"
#include "CalibrationConfig.h"

#include "../gantry_global_status.h"
#include <thread>

void PCB315::manageFilterSelection(void) {
    static bool filter_select_error = false;

    // No more attempts can be done after some collimation repetition.
    if (valid_filter_format) filter_selection_attempt = 0;
    else  if (filter_selection_attempt > 5) {

        // Activate the selectuion error if necessary
        if (!filter_select_error) {
            filter_select_error = true;
            Notify::activate(Notify::messages::ERROR_FILTER_SELECTION_ERROR, false);
        }
        return;
    }

    // Resets the Selection error
    if (filter_select_error) {
        filter_select_error = false;
        Notify::deactivate(Notify::messages::ERROR_FILTER_SELECTION_ERROR);
    }

    // Verifies the correct calibration format
    switch (filter_working_mode) {

        // Auto Filter selection mode (AWS) 
    case filterWorkingMode::FILTER_AUTO_MODE:

        // If a command is executing skips 
        if (filter_status == FilterSlotCodes::FILTER_SELECTION_PENDING) return;

        // No Filter activation in case of invalid filter code
        if (auto_filter_selected == FilterSlotCodes::FILTER_INVALID) {
            valid_filter_format = false;
            return;
        }

        // Try to set the valid filter selection
        if (filter_status != auto_filter_selected)
        {
            valid_filter_format = false;
            filter_selection_attempt++;

            // Calls the proper command based on the filter requested
            switch (auto_filter_selected) {
            case FilterSlotCodes::FILTER1_SELECTION:
                command(PCB315_SET_POSITIONER_COMMAND(PositionerCommands::POSITIONER_SELECT_FILTER1), 30);
                break;
            case FilterSlotCodes::FILTER2_SELECTION:
                command(PCB315_SET_POSITIONER_COMMAND(PositionerCommands::POSITIONER_SELECT_FILTER2), 30);
                break;
            case FilterSlotCodes::FILTER3_SELECTION:
                command(PCB315_SET_POSITIONER_COMMAND(PositionerCommands::POSITIONER_SELECT_FILTER3), 30);
                break;
            case FilterSlotCodes::FILTER4_SELECTION:
                command(PCB315_SET_POSITIONER_COMMAND(PositionerCommands::POSITIONER_SELECT_FILTER4), 30);
                break;
            }
            return;
        }
        else valid_filter_format = true;

        return;

        // Manual Filter selection mode (test/calibration)  
    case filterWorkingMode::FILTER_MANUAL_MODE:

        // If a command is executing skips 
        if (filter_status == FilterSlotCodes::FILTER_SELECTION_PENDING) return;

        // No Filter activation in case of invalid filter code
        if (manual_filter_selected == FilterSlotCodes::FILTER_INVALID) {
            valid_filter_format = false;
            return;
        }

        // Try to set the valid filter selection
        if (filter_status != manual_filter_selected)
        {
            valid_filter_format = false;
            filter_selection_attempt++;

            // Calls the proper command based on the filter requested
            switch (manual_filter_selected) {
            case FilterSlotCodes::FILTER1_SELECTION:
                command(PCB315_SET_POSITIONER_COMMAND(PositionerCommands::POSITIONER_SELECT_FILTER1), 30);
                break;
            case FilterSlotCodes::FILTER2_SELECTION:
                command(PCB315_SET_POSITIONER_COMMAND(PositionerCommands::POSITIONER_SELECT_FILTER2), 30);
                break;
            case FilterSlotCodes::FILTER3_SELECTION:
                command(PCB315_SET_POSITIONER_COMMAND(PositionerCommands::POSITIONER_SELECT_FILTER3), 30);
                break;
            case FilterSlotCodes::FILTER4_SELECTION:
                command(PCB315_SET_POSITIONER_COMMAND(PositionerCommands::POSITIONER_SELECT_FILTER4), 30);
                break;
            }
            return;
        }
        else valid_filter_format = true;

        return;

    case filterWorkingMode::FILTER_MIRROR_MODE:

        // If a command is executing skips 
        if (filter_status == FilterSlotCodes::FILTER_SELECTION_PENDING) return;

        if (filter_status != FilterSlotCodes::MIRROR_SELECTION)
        {
            valid_filter_format = false;
            request_light_activation = false; // The light is already activated when the mirror is selected
            filter_selection_attempt++;
            command(PCB315_SET_POSITIONER_COMMAND(PositionerCommands::POSITIONER_SELECT_MIRROR), 30);
        }
        else {
            valid_filter_format = true;

            // Executes a pending light activation request
            if (request_light_activation) {
                request_light_activation = false;
                command(PCB315_SET_LIGH_COMMAND(LightCommands::LIGHT_ON), 10);
            }
        }
        return;


    }
    return;
}


bool PCB315::updateStatusRegister(void) {
    static FilterSlotCodes current_filter_status = FilterSlotCodes::FILTER_SELECTION_PENDING;
    static bool error_status = false;


    Register^ system_status_register = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_STATUS_REGISTER);
    if (system_status_register == nullptr) return false;
        
    filter_status = (FilterSlotCodes) PCB315_GET_SYSTEM_FILTER(system_status_register);
    stator_perc = PCB315_GET_SYSTEM_STATOR(system_status_register);
    bulb_perc = PCB315_GET_SYSTEM_BULB(system_status_register);
    flags_status = PCB315_GET_SYSTEM_FLAGS(system_status_register);

    // Monitor the Out Of Position status
    if (current_filter_status != filter_status) {
        current_filter_status = filter_status;

        // Signals a warning if the filter is not in a valid position
        if(filter_status == FilterSlotCodes::FILTER_OUT_OF_POSITION) Notify::activate(Notify::messages::WARNING_FILTER_OUT_OF_POSITION, false);
        else Notify::deactivate(Notify::messages::WARNING_FILTER_OUT_OF_POSITION);
    }

    // If an error condition is signaled gets the error register 
    if (PCB315_GET_ERROR_FLAG(flags_status)) {

        error_status = true;

        // Reads the error register
        Register^ error_register = readErrorRegister();
        if (error_register != nullptr)
        {
            if (PCB315_ERROR_BULB_LOW(error_register))    Notify::activate(Notify::messages::ERROR_BULB_SENSOR_LOW, false);
            else Notify::deactivate(Notify::messages::ERROR_BULB_SENSOR_LOW);
            if (PCB315_ERROR_BULB_SHORT(error_register))    Notify::activate(Notify::messages::ERROR_BULB_SENSOR_SHORT, false);
            else Notify::deactivate(Notify::messages::ERROR_BULB_SENSOR_SHORT);
           

            if (PCB315_ERROR_STATOR_LOW(error_register))    Notify::activate(Notify::messages::ERROR_STATOR_SENSOR_LOW, false);
            else Notify::deactivate(Notify::messages::ERROR_STATOR_SENSOR_LOW);
            if (PCB315_ERROR_STATOR_SHORT(error_register))    Notify::activate(Notify::messages::ERROR_STATOR_SENSOR_SHORT, false);
            else Notify::deactivate(Notify::messages::ERROR_STATOR_SENSOR_SHORT);

            // Evaluates the Warning related to the High tube temperature
            if (PCB315_ERROR_BULB_HIGH(error_register) || PCB315_ERROR_STATOR_HIGH(error_register)) {
                tube_high_temp_alarm = true;

                if (PCB315_ERROR_BULB_HIGH(error_register)) Notify::activate(Notify::messages::WARNING_BULB_SENSOR_HIGH, false);
                else Notify::deactivate(Notify::messages::WARNING_BULB_SENSOR_HIGH);

                if (PCB315_ERROR_STATOR_HIGH(error_register)) Notify::activate(Notify::messages::WARNING_STATOR_SENSOR_HIGH, false);
                else Notify::deactivate(Notify::messages::WARNING_STATOR_SENSOR_HIGH);

            }
            else {
                Notify::deactivate(Notify::messages::WARNING_BULB_SENSOR_HIGH);
                Notify::deactivate(Notify::messages::WARNING_STATOR_SENSOR_HIGH);
                tube_high_temp_alarm = false;
            }

        }
    }
    else {
        if (error_status) {

            // Resets all the possible errors
            error_status = false;
            Notify::deactivate(Notify::messages::ERROR_BULB_SENSOR_LOW);
            Notify::deactivate(Notify::messages::ERROR_BULB_SENSOR_SHORT);
            Notify::deactivate(Notify::messages::WARNING_BULB_SENSOR_HIGH);
            Notify::deactivate(Notify::messages::ERROR_STATOR_SENSOR_LOW);
            Notify::deactivate(Notify::messages::ERROR_STATOR_SENSOR_SHORT);
            Notify::deactivate(Notify::messages::WARNING_STATOR_SENSOR_HIGH);
        }

       
    }


    return true;
}

/// <summary>
/// </summary>
/// 
/// This function is call by the Base class in order to handle the \n
/// communication workflow, after the module startup and configuration.
/// 
/// This module implements the device related functions here.
/// 
/// The module read the Status register in order to get the relevant registers 
/// from the device: updateStatusRegister():
/// 
///     + In case the function should fail in reading the Status register (several times), the\n
///       module activates a Communication Error condition until the Status register finally is successfully read.\n
///       During the fault condition the Filter selection and Tube temperature monitor activities are suspended. 
/// 
/// If the Status register should successfully read, the module:
/// 
/// + checks the presence of errors reading the Error register from the device;
/// + manage the Filter/Mirror selection operations: manageFilterSelection();
///      
/// 
/// 
/// <param name=""></param>
void PCB315::runningLoop(void) {
    static bool comm_error = false;
    static int comm_attempt = 0;

    // Updates the Status register
    if (updateStatusRegister()) {

        manageFilterSelection(); // manages the filter selection on the device

        comm_attempt = 0;

        // resets the communication error
        if (comm_error) {
            Notify::deactivate(Notify::messages::ERROR_PCB315_COMMUNICATION_ERROR);
            comm_error = false;
        }
    }
    else {
 
        // Communication issues section
        // In case the Status register read should fails several times, an error condition is generated
        if (comm_attempt > 10) {
            if (!comm_error) {
                comm_error = true;
                Notify::activate(Notify::messages::ERROR_PCB315_COMMUNICATION_ERROR, false);
            }
        }else comm_attempt++;
    }
    
    std::this_thread::sleep_for(std::chrono::microseconds(10000));

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
void PCB315::resetLoop(void) {

    // Ths error is a one shot error: it is reset as soon as the operator open the error window
    Notify::activate(Notify::messages::ERROR_PCB315_RESET, true);
}

/// <summary>
/// 
/// </summary>
/// 
/// This function is called by the Base class before to call the runningLoop() 
/// allowing the module to properly configure the device.
/// 
/// In the configurationLoop routine this module:
/// - Upload the Calibrated slot positions;
/// - Upload the Timeout for the Light;
/// 
/// The routine never exit in the case of communication issues
/// 
/// <param name=""></param>
/// <returns></returns>
bool PCB315::configurationLoop(void) {
    
    // 0.1mm position of the filters and mirror in the filter slots
    unsigned short calibrated_filter_1 = System::Convert::ToInt16(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CALIBRATION)[FilterConfig::PARAM_FILTER_CALIBRATION_FILTER1]);
    unsigned short calibrated_filter_2 = System::Convert::ToInt16(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CALIBRATION)[FilterConfig::PARAM_FILTER_CALIBRATION_FILTER2]);
    unsigned short calibrated_filter_3 = System::Convert::ToInt16(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CALIBRATION)[FilterConfig::PARAM_FILTER_CALIBRATION_FILTER3]);
    unsigned short calibrated_filter_4 = System::Convert::ToInt16(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CALIBRATION)[FilterConfig::PARAM_FILTER_CALIBRATION_FILTER4]);
    unsigned short calibrated_mirror = System::Convert::ToInt16(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CALIBRATION)[FilterConfig::PARAM_FILTER_CALIBRATION_MIRROR]);
    
    // Power Light timeout in seconds
    unsigned short power_ligth_tmo = System::Convert::ToInt16(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CALIBRATION)[FilterConfig::PARAM_FILTER_CALIBRATION_LIGHTON]);

    // Upload Filter calibrated positions
    while (!writeParamRegister(PCB315_WRITE_FILTER1_REGISTER(calibrated_filter_1))) ;
    while (!writeParamRegister(PCB315_WRITE_FILTER2_REGISTER(calibrated_filter_2)));
    while (!writeParamRegister(PCB315_WRITE_FILTER3_REGISTER(calibrated_filter_3)));
    while (!writeParamRegister(PCB315_WRITE_FILTER4_REGISTER(calibrated_filter_4)));
    while (!writeParamRegister(PCB315_WRITE_MIRROR_REGISTER(calibrated_mirror)));
    while (!writeParamRegister(PCB315_WRITE_POWERLIGHT_REGISTER(power_ligth_tmo)));
    
    
    return true;
}

/// <summary>
/// 
/// </summary>
/// 
/// This function is internally used to link the Filter assignment parameter 
/// in the FilterCalibratioon file to the Internal code used to compare the 
/// actual status of the filter selection in the device board.
/// 
/// The Assignment Parameter is in String format: "1", "2", ... "n" \n
/// and the converted integer value (1, 2, .. N) is compared with the \n 
/// FilterConfig::FilterAssignment enumeraiton class used in the assignment \n
/// declaration.
/// 
/// 
/// 
/// <param name="assignment"></param>
/// <returns>The FilterSlotCodes selection, or FilterSlotCodes::FILTER_INVALID in case of missing valid assignment</returns>
PCB315::FilterSlotCodes PCB315::filterAssignment(System::String^ assignment) {
    
    // Get the integer value
    int val = System::Convert::ToInt16(assignment);

    // Translate the integer value with the meaning in the FilterConfig file to the Filter selection in the FilterSlotCodes codes
    // This way the module is independent by the declaration into the FilterConfig file! 
    if (val == (int) System::Convert::ToInt16(FilterConfig::FILTER1_ASSIGNMENT)) return FilterSlotCodes::FILTER1_SELECTION;
    if (val == (int) System::Convert::ToInt16(FilterConfig::FILTER2_ASSIGNMENT)) return FilterSlotCodes::FILTER2_SELECTION;
    if (val == (int) System::Convert::ToInt16(FilterConfig::FILTER3_ASSIGNMENT)) return FilterSlotCodes::FILTER3_SELECTION;
    if (val == (int) System::Convert::ToInt16(FilterConfig::FILTER4_ASSIGNMENT)) return FilterSlotCodes::FILTER4_SELECTION;

    // In case there is not a valid assignment, the INVALID code is returned
    return PCB315::FilterSlotCodes::FILTER_INVALID;
}

/// <summary>
/// 
/// </summary>
/// 
/// This function activate the Automatic mode assigning 
/// what is the Filter slot to be selected in the Filter device.
/// 
/// The Application filter selection talks about Filter material,\n
/// wheather the internal filter management talks about filter slot.
/// 
/// setFilterAutoMode( filter-material ) --> Auto Filter Slot = slot assigned to the filter-material
/// 
/// <param name="code">The Filter (material) requested</param>
void PCB315::setFilterAutoMode(filterMaterialCodes code) {
    valid_filter_format = false;

    switch (code) {
    case filterMaterialCodes::FILTER_AG:
        auto_filter_selected = filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_AG_POSITION]);
        break;
    case filterMaterialCodes::FILTER_RH:
        auto_filter_selected = filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_RH_POSITION]);
        break;
    case filterMaterialCodes::FILTER_AL:
        auto_filter_selected = filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_AL_POSITION]);
        break;
    case filterMaterialCodes::FILTER_CU:
        auto_filter_selected = filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_CU_POSITION]);
        break;
    case filterMaterialCodes::FILTER_MO:
        auto_filter_selected = filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_MO_POSITION]);
        break;
    case filterMaterialCodes::FILTER_DEFAULT:
        auto_filter_selected = filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_DEFAULT_POSITION]);
        break;

    default: auto_filter_selected = FilterSlotCodes::FILTER_INVALID;
    }

    filter_working_mode = filterWorkingMode::FILTER_AUTO_MODE;
}

/// <summary>
/// 
/// </summary>
/// This function activates the Filter Auto mode;\n
/// The filter selection however shall be already selected.
///  
/// 
/// <param name=""></param>
void PCB315::setFilterAutoMode(void) {
    valid_filter_format = false;
    filter_working_mode = filterWorkingMode::FILTER_AUTO_MODE;    
}


/// <summary>
/// 
/// </summary>
/// 
/// This function activate the Manual mode assigning 
/// what is the Filter slot to be selected in the Filter device.
/// 
/// The Application filter selection talks about Filter material,\n
/// wheather the internal filter management talks about filter slot.
/// 
/// setFilterManaulMode( filter-material ) --> Manaul Filter Slot = slot assigned to the filter-material
/// 
/// <param name="code">The Filter (material) requested</param>
void PCB315::setFilterManualMode(filterMaterialCodes code) {
    valid_filter_format = false;

    switch (code) {
    case filterMaterialCodes::FILTER_AG:
        manual_filter_selected = filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_AG_POSITION]);
        break;
    case filterMaterialCodes::FILTER_RH:
        manual_filter_selected = filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_RH_POSITION]);
        break;
    case filterMaterialCodes::FILTER_AL:
        manual_filter_selected = filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_AL_POSITION]);
        break;
    case filterMaterialCodes::FILTER_CU:
        manual_filter_selected = filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_CU_POSITION]);
        break;
    case filterMaterialCodes::FILTER_MO:
        manual_filter_selected = filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_MO_POSITION]);
        break;
    case filterMaterialCodes::FILTER_DEFAULT:
        manual_filter_selected = filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_DEFAULT_POSITION]);
        break;

    default: manual_filter_selected = FilterSlotCodes::FILTER_INVALID;
    }

    filter_working_mode = filterWorkingMode::FILTER_MANUAL_MODE;

}

/// <summary>
/// 
/// </summary>
/// This function activates the Filter Manual mode;\n
/// The filter selection however shall be already selected.
///  
/// 
/// <param name=""></param>
void PCB315::setFilterManualMode(void) {
    valid_filter_format = false;
    filter_working_mode = filterWorkingMode::FILTER_MANUAL_MODE;
}

/// <summary>
/// 
/// </summary>
/// 
/// This function activates the Mirror mode:
/// 
/// in the mirror mode, the mirror is selected in the Filter Device 
/// allowing the usage of the colliamtion light.
/// 
/// When the Mirror is positioned, the light is switched on automatically.
/// 
/// Because the light remains active for a limited time, 
/// the application can reactivate the light calling ones more this function.
/// 
/// 
/// <param name="light">request the activation of the light</param>
void PCB315::setMirrorMode(bool light) {
    valid_filter_format = false;
    filter_working_mode = filterWorkingMode::FILTER_MIRROR_MODE;
    request_light_activation = light;
};

/// <summary>
/// 
/// </summary>
/// This is a convenient function to convert the Material Filter name to 
/// the Enumeration code.
///
/// The current implemented filter names are:
/// - "Ag"
/// - "Al"
/// - "Rh"
/// - "Cu"
/// - "Mo"
///  
/// <param name="filter_tag"></param>
/// <returns></returns>
PCB315::filterMaterialCodes PCB315::getFilterFromTag(System::String^ filter_tag) {

    if (filter_tag == "Ag") {
        if (filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_AG_POSITION]) != FilterSlotCodes::FILTER_INVALID)
            return filterMaterialCodes::FILTER_AG;
    }else if (filter_tag == "Al") {
        if (filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_AL_POSITION]) != FilterSlotCodes::FILTER_INVALID)
            return filterMaterialCodes::FILTER_AL;
    }else if (filter_tag == "Cu") {
        if (filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_CU_POSITION]) != FilterSlotCodes::FILTER_INVALID)
            return filterMaterialCodes::FILTER_CU;
    }else if (filter_tag == "Rh") {
        if (filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_RH_POSITION]) != FilterSlotCodes::FILTER_INVALID)
            return filterMaterialCodes::FILTER_RH;
    }else if (filter_tag == "Mo") {
        if (filterAssignment(FilterConfig::Configuration->getParam(FilterConfig::PARAM_FILTER_CONFIG)[FilterConfig::PARAM_FILTER_MO_POSITION]) != FilterSlotCodes::FILTER_INVALID)
            return filterMaterialCodes::FILTER_MO;
    }

    return filterMaterialCodes::FILTER_INVALID;
}

/// <summary>
/// 
/// </summary>
/// This is a convenient function to convert the Filter Enumeration code
/// to the Filter Material name.
/// 
/// The current implemented filter names are:
/// - "Ag"
/// - "Al"
/// - "Rh"
/// - "Cu"
/// - "Mo"
/// - "UNDEF" for invalid codes
/// 
/// <param name="filter_tag"></param>
/// <returns></returns>
System::String^ PCB315::getTagFromFilter(filterMaterialCodes filter) {
    if (filter == filterMaterialCodes::FILTER_AG) return "Ag";
    if (filter == filterMaterialCodes::FILTER_AL) return "Al";
    if (filter == filterMaterialCodes::FILTER_CU) return "Cu";
    if (filter == filterMaterialCodes::FILTER_RH) return "Rh";
    if (filter == filterMaterialCodes::FILTER_MO) return "Mo";

    return "UNDEF";
}