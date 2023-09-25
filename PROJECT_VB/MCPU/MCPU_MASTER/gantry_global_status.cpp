#include "pch.h"
#include "gantry_global_status.h"
#include "mutex"

namespace GantryStatusRegisters {
    static mutex gantry_status_mutex;

    void dataLock(void) {
        gantry_status_mutex.lock();
    }
    void dataUnlock(void) {
        gantry_status_mutex.unlock();
    }

    void PowerStatusRegister::setBatteryData(bool ena, bool batt1low, bool batt2low, unsigned char vbatt1, unsigned char vbatt2) {
        const std::lock_guard<std::mutex> lock(gantry_status_mutex);
        battery_enabled = ena;
        batt1_low = batt1low;
        batt2_low = batt2low;
        voltage_batt1 = vbatt1;
        voltage_batt2 = vbatt2;
        return;
    }

    void PowerStatusRegister::getBatteryData(bool* ena, bool* batt1low, bool* batt2low, unsigned char* vbatt1, unsigned char* vbatt2) {
        const std::lock_guard<std::mutex> lock(gantry_status_mutex);
        if (ena) *ena = battery_enabled;
        if (batt1low) *batt1low = batt1_low;
        if (batt2low) *batt2low = batt2_low;
        if (vbatt1) *vbatt1 = voltage_batt1;
        if (vbatt2) *vbatt2 = voltage_batt2;
        return;
    }

    void PowerStatusRegister::setPowerdown(bool stat) {
        const std::lock_guard<std::mutex> lock(gantry_status_mutex);
        power_down = stat;
        return;
    }

    

    bool PowerStatusRegister::getPowerdown(void) {
        const std::lock_guard<std::mutex> lock(gantry_status_mutex);
        return  power_down;
    }

    void SafetyStatusRegister::setCloseDoor(bool stat) {
        const std::lock_guard<std::mutex> lock(gantry_status_mutex);
        closed_door = stat;
    }

    bool SafetyStatusRegister::getCloseDoor(void) {
        const std::lock_guard<std::mutex> lock(gantry_status_mutex);
        return  closed_door;
    }

    void TubeDataRegister::setTubeTemp(unsigned char stator, unsigned char bulb) {
        const std::lock_guard<std::mutex> lock(gantry_status_mutex);
        bulbHeat = bulb;    
        statorHeat = stator;
        return;
    }


};







