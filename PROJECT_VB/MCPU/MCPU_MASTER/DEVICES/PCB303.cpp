
#include "PCB303.h"
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

    // No more attempts can be done after some collimation repetition.
    if (valid_collimation_format) format_collimation_attempt = 0;
    else  if (format_collimation_attempt > 5) return;

    // Verifies the correct calibration format
    switch (collimationMode) {

        // Auto Collimation mode setting
    case collimationModeEnum::AUTO_COLLIMATION:
        if (collimation_status == CollimationStatusCode::COLLI_STATUS_2D_EXECUTING) return;

        // Try to set the valid Standard Collimation
        if ((collimation_status != CollimationStatusCode::COLLI_STATUS_STANDARD_FORMAT) || (getAutomaticStandardFormatIndex() != format_index))
        {
            valid_collimation_format = false;
            format_collimation_attempt++;
            command(PCB303_COMMAND_STANDARD_FORMAT((int)getAutomaticStandardFormatIndex()), 30);
            return;
        }
        else valid_collimation_format = true;

        break;

    case collimationModeEnum::CUSTOM_COLLIMATION:
        if (collimation_status == CollimationStatusCode::COLLI_STATUS_2D_EXECUTING) return;

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
        if (collimation_status == CollimationStatusCode::COLLI_STATUS_2D_EXECUTING) return;

        if (!send(PCB303_GET_STATUS_FB_REGISTER)) {
            valid_collimation_format = false;
            // Wait before to proceed
            std::this_thread::sleep_for(std::chrono::microseconds(10000));
            return;
        }
        current_front = PCB303_GET_SYSTEM_FB_FRONT(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
        current_back = PCB303_GET_SYSTEM_FB_BACK(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);

        if (!send(PCB303_GET_STATUS_LR_REGISTER)) {
            valid_collimation_format = false;

            // Wait before to proceed
            std::this_thread::sleep_for(std::chrono::microseconds(10000));
            return;
        }

        current_left = PCB303_GET_SYSTEM_LR_LEFT(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
        current_right = PCB303_GET_SYSTEM_LR_RIGHT(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);

        if (!send(PCB303_GET_STATUS_T_REGISTER)) {
            valid_collimation_format = false;

            // Wait before to proceed
            std::this_thread::sleep_for(std::chrono::microseconds(10000));
            return;
        }
        current_trap = PCB303_GET_SYSTEM_T_TRAP(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);

        if ((collimation_status == CollimationStatusCode::COLLI_STATUS_CALIB_FORMAT) &&
            (current_front == calibrationBlades.front) &&
            (current_back == calibrationBlades.back) &&
            (current_left == calibrationBlades.left) &&
            (current_right == calibrationBlades.right) &&
            (current_trap == calibrationBlades.trap)
            ) {
            valid_collimation_format = true;
            return;
        }

        valid_collimation_format = false;
        if (!send(PCB303_WRITE_DATA_CALIBRATION_FB(calibrationBlades.front, calibrationBlades.back))) return;
        if (!send(PCB303_WRITE_DATA_CALIBRATION_LR(calibrationBlades.left, calibrationBlades.right))) return;
        if (!send(PCB303_WRITE_DATA_CALIBRATION_T(calibrationBlades.trap))) return;

        format_collimation_attempt++;
        command(PCB303_COMMAND_CALIBRATION_FORMAT, 30);
        return;

        break;
    case collimationModeEnum::OPEN_MODE:
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
    unsigned short current_front, current_back, current_left, current_right, current_trap;

    // Read the System Status register from the device
    if (!send(PCB303_GET_STATUS_SYSTEM_REGISTER)) {
        // Wait before to proceed
        std::this_thread::sleep_for(std::chrono::microseconds(10000));
        return;
    }

    // Stores the System status content into internal registers
    collimation_status = (CollimationStatusCode)PCB303_GET_SYSTEM_COLLIMATION_STATUS(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    format_index = (ColliStandardSelections)PCB303_GET_SYSTEM_FORMAT_INDEX(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    system_flags = PCB303_GET_SYSTEM_FLAGS(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);

    // In case of a fault condition, read the Error register from the device
    if (system_flags & PCB303_SYSTEM_FLAG_ERRORS) {
        System::String^ err_string = "";

        // Reads the error register
        if (send(PCB303_GET_ERROR_REGISTER)) {
            unsigned char err_blades = PCB303_ERROR_BLADES(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
            if (err_blades & PCB303_ERROR_LEFT) err_string += "Left ";
            if (err_blades & PCB303_ERROR_RIGHT) err_string += "Right ";
            if (err_blades & PCB303_ERROR_FRONT) err_string += "Front ";
            if (err_blades & PCB303_ERROR_BACK) err_string += "Back ";
            if (err_blades & PCB303_ERROR_TRAP) err_string += "Trap ";
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

    // Format collimation management
    formatCollimationManagement();
    

   
}

/// <summary>
/// This function returns the Collimation format assigned to a Paddle detected in the System 
/// by the Compressor device.
/// 
/// If the Paddle should not be detected, the COLLI_STANDARD1 Standard collimation is selected.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
PCB303::ColliStandardSelections PCB303::getAutomaticStandardFormatIndex(void) {
    return ColliStandardSelections::COLLI_STANDARD1;
}

/// <summary>
/// This is the configuration loop routine executed at the beginning of the 
/// device connection, before to execute the runningLoop() routine.
/// 
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool PCB303::configurationLoop(void) {
    unsigned short front, trap, left, right, back;

    front = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD_FT)[CollimatorConfig::PARAM_COLLI_STANDARD_FT_FRONT]);
    trap = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD_FT)[CollimatorConfig::PARAM_COLLI_STANDARD_FT_TRAP]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_FT(front, trap)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD1)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD1)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD1)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(1,left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(1, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD2)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD2)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD2)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(2, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(2, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD3)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD3)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD3)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(3, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(3, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD4)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD4)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD4)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(4, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(4, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD5)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD5)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD5)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(5, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(5, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD6)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD6)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD6)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(6, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(6, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD7)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD7)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD7)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(7, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(7, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD8)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD8)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD8)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(8, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(8, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD9)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD9)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD9)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(9, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(9, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD10)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD10)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD10)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(10, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(10, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD11)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD11)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD11)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(11, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(11, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD12)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD12)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD12)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(12, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(12, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD13)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD13)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD13)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(13, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(13, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD14)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD14)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD14)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(14, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(14, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD15)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD15)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD15)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(15, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(15, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD16)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD16)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD16)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(16, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(16, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD17)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD17)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD17)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(17, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(17, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD18)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD18)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD18)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(18, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(18, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD19)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD19)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD19)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(19, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(19, back)));

    left = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD20)[CollimatorConfig::PARAM_COLLI_STANDARD1_LEFT]);
    right = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD20)[CollimatorConfig::PARAM_COLLI_STANDARD1_RIGHT]);
    back = System::Convert::ToInt16(CollimatorConfig::Configuration->getParam(CollimatorConfig::PARAM_COLLI_STANDARD20)[CollimatorConfig::PARAM_COLLI_STANDARD1_BACK]);
    while (!send(PCB303_WRITE_PARAM_STANDARD_LR(20, left, right)));
    while (!send(PCB303_WRITE_PARAM_STANDARD_B(20, back)));



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

void PCB303::setCustomCollimationMode(ColliStandardSelections custom) {
    valid_collimation_format = false;
    if (custom == ColliStandardSelections::COLLI_NOT_STANDARD) custom = ColliStandardSelections::COLLI_STANDARD1;
    customStandardSelection = custom;
    collimationMode = collimationModeEnum::CUSTOM_COLLIMATION;

}

void PCB303::setCalibrationCollimationMode(formatBlades^ blades){
    valid_collimation_format = false;
    calibrationBlades.copy(blades);
    collimationMode = collimationModeEnum::CALIBRATION_MODE;

}
