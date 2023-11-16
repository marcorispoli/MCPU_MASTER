
#include "PCB303.h"
#include "PCB302.h"
#include "CalibrationConfig.h"
#include "MESSAGES/Errors.h"
#include <thread>

/// <summary>
/// This routine handles the format collimation.
/// 
/// The procedure set the collimation format based on the 
/// collimation mode set by the application.
/// 
/// In the case the collimation should be AUTOMATIC,
/// the format collimation is set according with the current detected paddle 
/// and the related collimation format assigned to it (see the PaddleCalibration file).
/// 
/// In the case the collimation should be CUSTOM,
/// the format collimation is set to a specific format set by the application.
/// 
/// In the case the collimation should be CALIBRATION,
/// the format collimation is set according with the blade positions set by the application.
/// 
/// In the case the collimation should be OPEN,
/// the format collimation is set to 0 position for all the blades.
/// 
/// The procedure makes a maximum of 5 attempts to set the collimation format as expected. 
/// When the attempts termines the collimator remains in fault condition until the fault
/// condition is reset by the application (see the resetFault() function)
///   
/// </summary>
/// <param name=""></param>
void PCB303::formatCollimationManagement(void) {
    unsigned short current_front, current_back, current_left, current_right, current_trap;
    Register^ rxreg;
    ColliStandardSelections format;

    static bool collimation_select_error = false;

    // No more attempts can be done after some collimation repetition.
    if (valid_collimation_format) format_collimation_attempt = 0;
    else  if (format_collimation_attempt > 5) {

        // Activate the selectuion error if necessary
        if (!collimation_select_error) {
            collimation_select_error = true;
            Notify::activate("COLLIMATION_SELECTION_ERROR", false);
        }
        return;
    }

    // Resets the Selection error
    if (collimation_select_error) {
        collimation_select_error = false;
        Notify::deactivate("COLLIMATION_SELECTION_ERROR");
    }


    // Verifies the correct calibration format
    switch (collimationMode) {

    // Auto Collimation mode setting
    case collimationModeEnum::AUTO_COLLIMATION:

        // If a collimation is executing skips 
        if (collimation_status == CollimationStatusCode::COLLI_STATUS_2D_EXECUTING) return;

        // Get the current auto collimation format 
        format = getAutomaticStandardFormatIndex(); 
        
        // No collimation activation in case of invalid collimation code
        if (format == ColliStandardSelections::COLLI_INVALID_FORMAT) {
            valid_collimation_format = false;
            return;
        }
        
        // Try to set the valid Standard Collimation
        if ((collimation_status != CollimationStatusCode::COLLI_STATUS_STANDARD_FORMAT) || (format != format_index))
        {
            valid_collimation_format = false;
            format_collimation_attempt++;
            command(PCB303_COMMAND_STANDARD_FORMAT((int)getAutomaticStandardFormatIndex()), 30);
            return;
        }
        else valid_collimation_format = true;

        break;

        // Custom Collimation mode setting
    case collimationModeEnum::CUSTOM_COLLIMATION:

        // If a collimation is executing skips 
        if (collimation_status == CollimationStatusCode::COLLI_STATUS_2D_EXECUTING) return;

        // No collimation activation in case of invalid collimation code
        if (customStandardSelection == ColliStandardSelections::COLLI_INVALID_FORMAT) {
            valid_collimation_format = false;
            return;
        }

        // Try to set the valid Custom Collimation
        if ((collimation_status != CollimationStatusCode::COLLI_STATUS_STANDARD_FORMAT) || (customStandardSelection != format_index))
        {
            valid_collimation_format = false;
            format_collimation_attempt++;
            command(PCB303_COMMAND_STANDARD_FORMAT((int)customStandardSelection), 30);
            return;
        }
        else valid_collimation_format = true;
        
        break;

    case collimationModeEnum::CALIBRATION_MODE:

        // If a collimation is executing skips 
        if (collimation_status == CollimationStatusCode::COLLI_STATUS_2D_EXECUTING) return;

        // Reads the current selected blades from the device 
        rxreg = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_CURRENT_FB_REGISTER);
        if (rxreg == nullptr) {
            valid_collimation_format = false;
            // Wait before to proceed
            std::this_thread::sleep_for(std::chrono::microseconds(10000));
            return;
        }
        current_front = PCB303_GET_SYSTEM_FB_FRONT(rxreg);
        current_back = PCB303_GET_SYSTEM_FB_BACK(rxreg);

        rxreg = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_CURRENT_LR_REGISTER);
        if (rxreg == nullptr) {
            valid_collimation_format = false;

            // Wait before to proceed
            std::this_thread::sleep_for(std::chrono::microseconds(10000));
            return;
        }

        current_left = PCB303_GET_SYSTEM_LR_LEFT(rxreg);
        current_right = PCB303_GET_SYSTEM_LR_RIGHT(rxreg);

        rxreg = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_CURRENT_T_REGISTER);
        if (rxreg == nullptr) {
            valid_collimation_format = false;

            // Wait before to proceed
            std::this_thread::sleep_for(std::chrono::microseconds(10000));
            return;
        }
        current_trap = PCB303_GET_SYSTEM_T_TRAP(rxreg);

        // Compares the current blades position with the expected
        if ((collimation_status == CollimationStatusCode::COLLI_STATUS_CALIB_FORMAT) &&
            (current_front == calibrationBlades.front) &&
            (current_back == calibrationBlades.back) &&
            (current_left == calibrationBlades.left) &&
            (current_right == calibrationBlades.right) &&
            (current_trap == calibrationBlades.trap)
            ) {

            // Ok, no more actions
            valid_collimation_format = true;
            return;
        }

        // The current blades are not in the expected position
        // Upload the expected position in the proper DATA registers then executes the CALIBRATION command
        valid_collimation_format = false;
        if (!writeDataRegister(PCB303_WRITE_DATA_CALIBRATION_FB(calibrationBlades.front, calibrationBlades.back))) return;
        if (!writeDataRegister(PCB303_WRITE_DATA_CALIBRATION_LR(calibrationBlades.left, calibrationBlades.right))) return;
        if (!writeDataRegister(PCB303_WRITE_DATA_CALIBRATION_T(calibrationBlades.trap))) return;


        format_collimation_attempt++;

        // Executes the Calibration command
        command(PCB303_COMMAND_CALIBRATION_FORMAT, 30);
        return;

        break;

    case collimationModeEnum::OPEN_MODE:

        // If a collimation is executing skips 
        if (collimation_status == CollimationStatusCode::COLLI_STATUS_2D_EXECUTING) return;

        // Try to set the valid Open Collimation
        if (collimation_status != CollimationStatusCode::COLLI_STATUS_OPEN_FORMAT) {
            valid_collimation_format = false;
            format_collimation_attempt++;
            command(PCB303_COMMAND_OPEN_FORMAT, 30);
            return;
        }
        else  valid_collimation_format = true;

        break;
    case collimationModeEnum::TOMO_MODE:
        break;

    }

    return;
}

bool PCB303::updateStatusRegister(void) {

    // Read the System Status register from the device
    Register^ system_status_register = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_STATUS_REGISTER);
    if (system_status_register == nullptr) return false;

    // Stores the System status content into internal registers
    collimation_status = (CollimationStatusCode)PCB303_GET_SYSTEM_COLLIMATION_STATUS(system_status_register);
    format_index = (ColliStandardSelections)PCB303_GET_SYSTEM_FORMAT_INDEX(system_status_register);
    system_flags = PCB303_GET_SYSTEM_FLAGS(system_status_register);

    // In case of a fault condition, read the Error register from the device
    if (system_flags & PCB303_SYSTEM_FLAG_ERRORS) {
        System::String^ err_string = "";

        // Reads the error register
        Register^ error_register = readErrorRegister();
        if (error_register != nullptr)
        {
            if (PCB303_ERROR_LEFT(error_register)) err_string += "Left ";
            if (PCB303_ERROR_RIGHT(error_register)) err_string += "Right ";
            if (PCB303_ERROR_FRONT(error_register)) err_string += "Front ";
            if (PCB303_ERROR_BACK(error_register)) err_string += "Back ";
            if (PCB303_ERROR_TRAP(error_register)) err_string += "Trap ";
        }

        if (!collimator_fault) {
            collimator_fault = true;
            Notify::activate("COLLIMATOR_OUT_OF_POSITION", err_string, false);
        }
    }
    else {
        format_collimation_attempt = 0;
        if (collimator_fault) {
            collimator_fault = false;
            Notify::deactivate("COLLIMATOR_OUT_OF_POSITION");
        }
    }

    return true;
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
    static bool comm_error = false;
    static int comm_attempt =0;

    // Updates the Status register
    if (updateStatusRegister()) {

        formatCollimationManagement(); // Format collimation management

        comm_attempt = 0;

        // resets the communication error
        if (comm_error) {
            Notify::deactivate("PCB303_COMMUNICATION_ERROR");
            comm_error = false;
        }
    }
    else {

        // Communication issues
        if (comm_attempt > 10) {
            if (!comm_error) {
                comm_error = true;
                Notify::activate("PCB303_COMMUNICATION_ERROR", false);
            }
        }
        else comm_attempt++;
    }



    std::this_thread::sleep_for(std::chrono::microseconds(10000));
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
/// This is the configuration loop routine executed at the beginning of the 
/// device connection, before to execute the runningLoop() routine.
/// 
/// 
/// </summary>
/// <param name=""></param>
/// <returns>true if the configuration success</returns>
bool PCB303::configurationLoop(void) {
    unsigned short front, trap, left, right, back;

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
