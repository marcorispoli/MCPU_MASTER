#include "PCB315.h"
#include "CalibrationConfig.h"

#include "../gantry_global_status.h"
#include <thread>

void PCB315::setAnodeHu(unsigned char ahu) {
    anode_perc = ahu;

    // Evaluate the alarm conditions on the current tube moinitoring
    int max = anode_perc;
    if (bulb_perc > max) max = bulb_perc;
    if (stator_perc > max) max = stator_perc;
    cumulated_perc = max;

    // Sets a possible Tube Alarm condition if necessary
    if ((max > 90) && (!tube_alarm)) {
        tube_alarm = true;
        Notify::activate("TUBE_TEMP_WARNING", false);
    }
    else if ((max <= 90) && (tube_alarm)) {
        tube_alarm = false;
        Notify::deactivate("TUBE_TEMP_WARNING");
    }

    return;
}

void PCB315::updateStatusRegister(void) {
    static FilterStatus current_filter_status = FilterStatus::FILTER_SELECTION_PENDING;

    while (!send(PCB315_GET_STATUS_SYSTEM_REGISTER));

    filter_status = (FilterStatus) PCB315_GET_SYSTEM_FILTER(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    
    // Monitor the Out Of Position status
    if (current_filter_status != filter_status) {
        current_filter_status = filter_status;
        if(filter_status == FilterStatus::FILTER_OUT_OF_POSITION) Notify::activate("FILTER_OUT_OF_POSITION", false);
        else Notify::deactivate("FILTER_OUT_OF_POSITION");
    }
    
    stator_perc = PCB315_GET_SYSTEM_STATOR(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    bulb_perc = PCB315_GET_SYSTEM_BULB(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    power_light_status = PCB315_GET_SYSTEM_FLAGS_LIGHT_ON(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);

    // Evaluate the alarm conditions on the current tube moinitoring
    int max = anode_perc;
    if (bulb_perc > max) max = bulb_perc;
    if (stator_perc > max) max = stator_perc;
    cumulated_perc = max;

    // Sets a possible Tube Alarm condition if necessary
    if ((max > 90) && (!tube_alarm)) {
        tube_alarm = true;
        Notify::activate("TUBE_TEMP_WARNING", false);
    }
    else if ((max <= 90) && (tube_alarm)) {
        tube_alarm = false;
        Notify::deactivate("TUBE_TEMP_WARNING");
    }

}

void PCB315::runningLoop(void) {

    updateStatusRegister(); // Updates the Status register
   
    
    std::this_thread::sleep_for(std::chrono::microseconds(10000));

    return;
}


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
    while (!send(PCB315_WRITE_FILTER1_REGISTER(calibrated_filter_1)));
    while (!send(PCB315_WRITE_FILTER2_REGISTER(calibrated_filter_2)));
    while (!send(PCB315_WRITE_FILTER3_REGISTER(calibrated_filter_3)));
    while (!send(PCB315_WRITE_FILTER4_REGISTER(calibrated_filter_4)));
    while (!send(PCB315_WRITE_MIRROR_REGISTER(calibrated_mirror)));
    while (!send(PCB315_WRITE_POWERLIGHT_REGISTER(power_ligth_tmo)));
    
    return true;
}


