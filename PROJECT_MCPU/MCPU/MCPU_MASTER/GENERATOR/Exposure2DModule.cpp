#include "ExposureModule.h"
#include "awsProtocol.h"
#include "Generator.h"
#include "R2CP_Eth.h"
#include "CaDataDicGen.h"
#include "PCB304.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB303.h"
#include <thread>
#include "Log.h"

using namespace System::Diagnostics;
using namespace System::Collections::Generic;

Exposures::exposure_completed_errors Exposures::man_2d_exposure_procedure(bool demo) {
    System::String^ ExpName;    
    bool large_focus;
    bool detector_synch = true;
    bool grid_synch = true;
    System::String^ detector_param;
    int exposure_time;
    exposure_completed_errors  error;

    if (demo) ExpName = "Demo Exposure 2D Manual>";
    else ExpName = "Exposure 2D Manual>";

    try {
        detector_param = getDetectorType().ToString();
        exposure_time = System::Convert::ToInt16(DetectorConfig::Configuration->getParam(detector_param)[DetectorConfig::PARAM_MAX_2D_INTEGRATION_TIME]);
    }
    catch (...) {
        LogClass::logInFile(ExpName + "Invalid Detector Parameter ");
        return exposure_completed_errors::XRAY_INVALID_2D_PARAMETERS;
    }

    // Set the filter selected is the expected into the pulse(0). No wait for positioning here    
    PCB303::selectFilter(getExposurePulse(0)->filter);
    if (PCB303::isFilterInError()) return Exposures::exposure_completed_errors::XRAY_FILTER_ERROR;

    // Sets the Grid On Field (if not yet) : wait for the ready condition   
    if (grid_synch) PCB304::syncGeneratorOn();
    else PCB304::syncGeneratorOff();
   
    // Determines if the Focus is Small or large based on the presence of the Magnifier device
    if (PCB302::getMagnifierFactor() != 10) large_focus = false;
    else large_focus = true;
    
    
    // The format collimation should be already configured
    if (!PCB303::isValidCollimationFormat()) {
        return exposure_completed_errors::XRAY_COLLI_FORMAT_ERROR;
    }
    
    // Pulse Preparation
    System::String^ exposure_data_str = ExpName + " ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR TYPE: " + detector_param; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR MAX INTEGRATION TIME: " + exposure_time; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter:" + Exposures::getExposurePulse(0)->filter.ToString(); LogClass::logInFile(exposure_data_str);

    error = (exposure_completed_errors)generator2DPulsePreparation(ExpName, Exposures::getExposurePulse(0)->kV, Exposures::getExposurePulse(0)->mAs, large_focus, detector_synch, grid_synch, exposure_time);
    if (error != Exposures::exposure_completed_errors::XRAY_NO_ERRORS) return error;


    // Checks the filter in position
    if(!PCB303::waitFilterCompleted()) return Exposures::exposure_completed_errors::XRAY_FILTER_ERROR;

    if (!demo) {
        error = (exposure_completed_errors)generatorExecutePulseSequence(ExpName, 40000);
        setXrayEnable(false);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!!
        if (large_focus) setExposedData(1, (unsigned char)0, getExposurePulse(0)->filter, 1);
        else setExposedData(1, (unsigned char)0, getExposurePulse(0)->filter, 0);

    }
    else {
        // Demo pulse implementation
        exposure_pulse^ epulse = Exposures::getExposurePulse(0);

        // Activate the Buzzer in manual mode
        PCB301::setBuzzerManualMode(true);

        float result_mas = demo2DPulse(Exposures::getExposurePulse(0)->mAs, getSelectedAnodeCurrent());
        if (result_mas <= 0) {
            setExposedPulse(0, gcnew exposure_pulse(epulse->getKv(), -1 * result_mas, epulse->getFilter()));
            error = exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }
        else {
            setExposedPulse(0, gcnew exposure_pulse(epulse->getKv(), epulse->getmAs(), epulse->getFilter()));
            error = exposure_completed_errors::XRAY_NO_ERRORS;
        }
        PCB301::setBuzzerManualMode(false);

    }
    
    return error;
};

Exposures::exposure_completed_errors Exposures::aec_2d_exposure_procedure(bool demo) {
    System::String^ ExpName;
    bool large_focus;
    bool detector_synch = true;
    bool grid_synch = true;

    System::String^ detector_param;
    int exposure_time;
    exposure_completed_errors  error;

    if (demo) ExpName = "Demo Exposure 2D + AEC>";
    else ExpName = "Exposure 2D + AEC>";

    try {
        detector_param = Exposures::getDetectorType().ToString();
        exposure_time = System::Convert::ToInt16(DetectorConfig::Configuration->getParam(detector_param)[DetectorConfig::PARAM_MAX_2D_PRE_INTEGRATION_TIME]);
    }
    catch (...) {
        LogClass::logInFile(ExpName + "Invalid Detector Parameter ");
        return exposure_completed_errors::XRAY_INVALID_2D_PARAMETERS;
    }

    // Set the filter selected is the expected into the pulse(0). No wait for positioning here    
    PCB303::selectFilter(getExposurePulse(0)->filter);
    if (PCB303::isFilterInError()) return Exposures::exposure_completed_errors::XRAY_FILTER_ERROR;

    // Sets the Grid On Field (if not yet) : wait for the ready condition   
    if (grid_synch) PCB304::syncGeneratorOn();
    else PCB304::syncGeneratorOff();

    // Determines if the Focus is Small or large based on the presence of the Magnifier device
    if (PCB302::getMagnifierFactor() != 10) large_focus = false;
    else large_focus = true;


    // Verifies that a valid the format collimation is present
    if (!PCB303::isValidCollimationFormat()) {
        return Exposures::exposure_completed_errors::XRAY_COLLI_FORMAT_ERROR;
    }

    // Pre-Pulse Preparation
    System::String^ exposure_data_str = ExpName + "AEC PRE-PULSE DATA ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR TYPE: " + detector_param; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR MAX AEC INTEGRATION TIME: " + exposure_time; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter Pre:" + Exposures::getExposurePulse(0)->filter.ToString(); LogClass::logInFile(exposure_data_str);

    //exposure_time = 5000;
    error = (exposure_completed_errors)generator2DAecPrePulsePreparation(ExpName, Exposures::getExposurePulse(0)->kV, Exposures::getExposurePulse(0)->mAs, large_focus,  exposure_time);
    if (error != Exposures::exposure_completed_errors::XRAY_NO_ERRORS) return error;

    // Checks the filter in position
    if (!PCB303::waitFilterCompleted()) return Exposures::exposure_completed_errors::XRAY_FILTER_ERROR;

    
    if (!demo) {
        // Sequence for the AEC: only the Standby is admitted as returned code: the WaitFootRelease is not admitted here
        error = (exposure_completed_errors) generatorExecutePulseSequence(ExpName, 40000);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!!
        if (large_focus) setExposedData(1, (unsigned char)0, getExposurePulse(0)->filter, 1);
        else setExposedData(1, (unsigned char)0, getExposurePulse(0)->filter, 0);
        if (getGeneratorStatus() != R2CP::Stat_Standby) error = exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
    }
    else {
        // Demo pre-pulse implementation
        exposure_pulse^ epulse = Exposures::getExposurePulse(0);

        // Activate the Buzzer in manual mode
        PCB301::setBuzzerManualMode(true);

        float result_mas = demo2DPulse(Exposures::getExposurePulse(0)->mAs, getSelectedAnodeCurrent());
        if (result_mas <= 0) {
            setExposedPulse(0, gcnew exposure_pulse(epulse->getKv(), -1 * result_mas, epulse->getFilter()));
            error = exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }
        else {
            setExposedPulse(0, gcnew exposure_pulse(epulse->getKv(), epulse->getmAs(), epulse->getFilter()));
            error = exposure_completed_errors::XRAY_NO_ERRORS;
        }

        PCB301::setBuzzerManualMode(false);
    }
    
    
    // If the sequence should return an error condition termines here
    if (error != exposure_completed_errors::XRAY_NO_ERRORS) return error;
        

    // Sends the pulse-completed event to the AWS
    awsProtocol::EVENT_exposurePulseCompleted(0);

    // Waits for the Validated Pulse from the acquisition software
    int timeout = 100; // 10 seconds timeout
    while (timeout--) {
        if (getExposurePulse(1)->isValid()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (!timeout) {
        return exposure_completed_errors::XRAY_TIMEOUT_AEC;
    }
    getExposurePulse(1)->validated = false;

    // Set the filter selected is the expected into the pulse(1). No wait for positioning here    
    PCB303::selectFilter(getExposurePulse(1)->filter);

    // Checks the filter in position
    if (!PCB303::waitFilterCompleted()) return Exposures::exposure_completed_errors::XRAY_FILTER_ERROR;


    try {        
        exposure_time = System::Convert::ToInt16(DetectorConfig::Configuration->getParam(detector_param)[DetectorConfig::PARAM_MAX_2D_INTEGRATION_TIME]);
    }
    catch (...) {
        LogClass::logInFile(ExpName + "Invalid Detector Parameter ");
        return exposure_completed_errors::XRAY_INVALID_2D_PARAMETERS;
    }

    // Preparation for pulse
    exposure_data_str = ExpName + "AEC PULSE DATA ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter:" + Exposures::getExposurePulse(1)->filter.ToString(); LogClass::logInFile(exposure_data_str);
    error = (exposure_completed_errors)generator2DAecPulsePreparation(ExpName, Exposures::getExposurePulse(1)->kV, Exposures::getExposurePulse(1)->mAs, large_focus, exposure_time);
    if (error != Exposures::exposure_completed_errors::XRAY_NO_ERRORS) return error;


    if (!demo) {

        error = (exposure_completed_errors)generatorExecutePulseSequence(ExpName, 40000);
        setXrayEnable(false);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!! 
        if (large_focus) setExposedData(2, (unsigned char)0, getExposurePulse(1)->filter, 1);
        else setExposedData(2, (unsigned char)0, getExposurePulse(1)->filter, 0);
    }
    else {
        // Demo pulse implementation
        exposure_pulse^ epulse = Exposures::getExposurePulse(1);

        // Activate the Buzzer in manual mode
        PCB301::setBuzzerManualMode(true);

        float result_mas = demo2DPulse(Exposures::getExposurePulse(1)->mAs, getSelectedAnodeCurrent());
        if (result_mas <= 0) {
            setExposedPulse(1, gcnew exposure_pulse(epulse->getKv(), -1 * result_mas, epulse->getFilter()));
            error = exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }
        else {
            setExposedPulse(1, gcnew exposure_pulse(epulse->getKv(), epulse->getmAs(), epulse->getFilter()));
            error = exposure_completed_errors::XRAY_NO_ERRORS;
        }

        PCB301::setBuzzerManualMode(false);
    }
    
    return error;
    
};
