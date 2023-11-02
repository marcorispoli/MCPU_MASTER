#include "PCB315.h"
#include "CalibrationConfig.h"

#include "../gantry_global_status.h"
#include <thread>

void PCB315::runningLoop(void) {

    while (!send(GET_STATUS_SYSTEM_REGISTER));

    unsigned char filters =  GET_SYSTEM_FILTER(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    unsigned char stator = GET_SYSTEM_STATOR(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    unsigned char bulb = GET_SYSTEM_BULB(getRxRegister()->b3, getRxRegister()->b4, getRxRegister()->b5, getRxRegister()->b6);
    GantryStatusRegisters::TubeDataRegister::setTubeTemp(stator, bulb);


    std::this_thread::sleep_for(std::chrono::microseconds(10000));

    return;
}


bool PCB315::configurationLoop(void) {

    
    // 0.1mm position of the filters and mirror in the filter slots
    unsigned short calibrated_filter_1 = System::Convert::ToInt16(CalibrationConfig::Configuration->getParam(CalibrationConfig::PARAM_FILTER_CALIBRATION)[CalibrationConfig::PARAM_FILTER_CALIBRATION_FILTER1]);
    unsigned short calibrated_filter_2 = System::Convert::ToInt16(CalibrationConfig::Configuration->getParam(CalibrationConfig::PARAM_FILTER_CALIBRATION)[CalibrationConfig::PARAM_FILTER_CALIBRATION_FILTER2]);
    unsigned short calibrated_filter_3 = System::Convert::ToInt16(CalibrationConfig::Configuration->getParam(CalibrationConfig::PARAM_FILTER_CALIBRATION)[CalibrationConfig::PARAM_FILTER_CALIBRATION_FILTER3]);
    unsigned short calibrated_filter_4 = System::Convert::ToInt16(CalibrationConfig::Configuration->getParam(CalibrationConfig::PARAM_FILTER_CALIBRATION)[CalibrationConfig::PARAM_FILTER_CALIBRATION_FILTER4]);
    unsigned short calibrated_mirror = System::Convert::ToInt16(CalibrationConfig::Configuration->getParam(CalibrationConfig::PARAM_FILTER_CALIBRATION)[CalibrationConfig::PARAM_FILTER_CALIBRATION_MIRROR]);
    
    // Power Light timeout in seconds
    unsigned short power_ligth_tmo = System::Convert::ToInt16(CalibrationConfig::Configuration->getParam(CalibrationConfig::PARAM_FILTER_CALIBRATION)[CalibrationConfig::PARAM_FILTER_CALIBRATION_LIGHTON]);

    // Upload Filter calibrated positions
    while (!send(WRITE_FILTER1_REGISTER(calibrated_filter_1)));
    while (!send(WRITE_FILTER2_REGISTER(calibrated_filter_2)));
    while (!send(WRITE_FILTER3_REGISTER(calibrated_filter_3)));
    while (!send(WRITE_FILTER4_REGISTER(calibrated_filter_4)));
    while (!send(WRITE_MIRROR_REGISTER(power_ligth_tmo)));

    
    return true;
}
