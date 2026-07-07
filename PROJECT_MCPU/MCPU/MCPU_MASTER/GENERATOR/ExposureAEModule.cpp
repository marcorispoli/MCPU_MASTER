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



Exposures::exposure_completed_errors Exposures::man_ae_exposure_procedure(bool demo) {
    // Set thecommunication error code as general case
    // return exposure_completed_errors::XRAY_NO_ERRORS;


    System::String^ ExpName;
    bool large_focus;
    bool detector_synch = true;
    bool grid_synch = true;
    System::String^ detector_param;
    int exposure_time;
    exposure_completed_errors  error;
    unsigned char current_exposure_pulse_number;

    // PULSE NUMBER 0 IN THE SEQUENCE __________________________________________________________________________________________________
    current_exposure_pulse_number = EXP_PULSE_0;
    // Resets of the Internal Generator Module exposed pulses for all data-banks    
    if (!demo) R2CP::CaDataDicGen::GetInstance()->resetExecutedPulse();


    if (demo) ExpName = "Demo Exposure CESM(AE) Manual>";
    else ExpName = "Exposure CESM (AE)  Manual>";

    try {
        detector_param = getDetectorType().ToString();
        exposure_time = System::Convert::ToInt16(DetectorConfig::Configuration->getParam(detector_param)[DetectorConfig::PARAM_MAX_2D_INTEGRATION_TIME]);
    }
    catch (...) {
        LogClass::logInFile(ExpName + "Invalid Detector Parameter ");
        return exposure_completed_errors::XRAY_INVALID_2D_PARAMETERS;
    }

    // Set the filter selected is the expected into the pulse(0). No wait for positioning here    
    PCB303::selectFilter(getExposurePulse(current_exposure_pulse_number)->filter);

    // Set the grid to the requested status 
    if (getGrid() == grid_selection_index::GRID_AUTO) {

        PCB304::setAutoGridInField(); // The normal grid status in 2D exposure is Grid In Field

    }
    else if (getGrid() == grid_selection_index::GRID_IN) {
        PCB304::setAutoGridInField();
    }
    else {
        PCB304::setAutoGridOutField();
    }

    if (PCB304::isGridInFieldStatus()) {
        grid_synch = true;
        PCB304::syncGeneratorOn();
        LogClass::logInFile(ExpName + "GRID IN ");
    }
    else {
        grid_synch = false;
        PCB304::syncGeneratorOff(false);
        LogClass::logInFile(ExpName + "GRID OUT ");
    }

    // Sets the current focus
    if (focus_selection == focus_selection_index::FOCUS_AUTO) {
        large_focus = true;        
    }
    else if (focus_selection == focus_selection_index::FOCUS_LARGE) large_focus = true;
    else large_focus = false;

    if (large_focus) LogClass::logInFile(ExpName + "FOCUS LARGE ");
    else LogClass::logInFile(ExpName + "FOCUS SMALL ");


    // The format collimation should be already configured
    if (!PCB303::isValidCollimationFormat()) {
        return exposure_completed_errors::XRAY_COLLI_FORMAT_ERROR;
    }

    // Pulse Preparation
    System::String^ exposure_data_str = ExpName + " ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR TYPE: " + detector_param; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR MAX INTEGRATION TIME: " + exposure_time; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter:" + Exposures::getExposurePulse(current_exposure_pulse_number)->filter.ToString(); LogClass::logInFile(exposure_data_str);

    error = (exposure_completed_errors)generator2DPulsePreparation(ExpName, Exposures::getExposurePulse(current_exposure_pulse_number)->kV, Exposures::getExposurePulse(current_exposure_pulse_number)->mAs, large_focus, detector_synch, grid_synch, exposure_time);
    if (error != Exposures::exposure_completed_errors::XRAY_NO_ERRORS) return error;


    // Checks the filter in position
    if (!PCB303::waitFilterCompleted()) return Exposures::exposure_completed_errors::XRAY_FILTER_ERROR;

    // Checks the grid in positon
    if (!PCB304::waitGridCompleted()) return Exposures::exposure_completed_errors::XRAY_GRID_ERROR;

    if (!demo) {
        error = (exposure_completed_errors)generatorExecutePulseSequence(ExpName, 40000);
        setXrayEnable(false);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!!
        if (large_focus) setExposedData(DB_INDEX_2D_MANUAL_PULSE, current_exposure_pulse_number, getExposurePulse(current_exposure_pulse_number)->filter, LARGE_FOCUS_IDX);
        else setExposedData(DB_INDEX_2D_MANUAL_PULSE, current_exposure_pulse_number, getExposurePulse(current_exposure_pulse_number)->filter, SMALL_FOCUS_IDX);

    }
    else {
        // Demo pulse implementation
        exposure_pulse^ epulse = Exposures::getExposurePulse(current_exposure_pulse_number);

        // Activate the Buzzer in manual mode
        PCB301::setBuzzerManualMode(true);

        float result_mas = demo2DPulse(epulse->mAs, getSelectedAnodeCurrent());
        if (result_mas <= 0) {
            setExposedPulse(current_exposure_pulse_number, gcnew exposure_pulse(epulse->getKv(), -1 * result_mas, epulse->getFilter()));
            error = exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }
        else {
            setExposedPulse(current_exposure_pulse_number, gcnew exposure_pulse(epulse->getKv(), epulse->getmAs(), epulse->getFilter()));
            error = exposure_completed_errors::XRAY_NO_ERRORS;
        }
        PCB301::setBuzzerManualMode(false);

    }

    // ______________ HIGH ENERGY EXPOSURE PULSE : PULSE #1 _______________________________________________________________     
    if (error != exposure_completed_errors::XRAY_NO_ERRORS) return error;
    

    // Sends the pulse-completed event to the AWS
    awsProtocol::EVENT_exposurePulseCompleted(current_exposure_pulse_number);
    
    current_exposure_pulse_number = EXP_PULSE_1;
    
    // Resets of the Internal Generator Module exposed pulses for all data-banks    
    if (!demo) R2CP::CaDataDicGen::GetInstance()->resetExecutedPulse();


    // The filter is quickly set here 
    PCB303::selectFilter(PCB303::filter_index::FILTER_CU);

    // Waits for the Validated Pulse from the acquisition software
    int timeout = 100; // 10 seconds timeout
    while (timeout--) {
        if (getExposurePulse(current_exposure_pulse_number)->isValid()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (!timeout) {
        return exposure_completed_errors::XRAY_TIMEOUT_AEC;
    }
    getExposurePulse(current_exposure_pulse_number)->validated = false;

    // In the case the filter dshould be different by the standard Cu then the 
    // filter is changed again: this takes more time then the normal shenario
    if (Exposures::getExposurePulse(current_exposure_pulse_number)->filter != PCB303::filter_index::FILTER_CU) {
        // Wait for the previous command termination then change the filter in the requested (non Cu)
        PCB303::waitFilterCompleted();
        PCB303::selectFilter(Exposures::getExposurePulse(current_exposure_pulse_number)->filter);
    }


    error = (exposure_completed_errors)generator2DPulsePreparation(ExpName, Exposures::getExposurePulse(current_exposure_pulse_number)->kV, Exposures::getExposurePulse(current_exposure_pulse_number)->mAs, large_focus, detector_synch, grid_synch, exposure_time);
    if (error != Exposures::exposure_completed_errors::XRAY_NO_ERRORS) return error;

    // Checks the filter in position
    if (!PCB303::waitFilterCompleted()) return Exposures::exposure_completed_errors::XRAY_FILTER_ERROR;


    if (!demo) {
        error = (exposure_completed_errors)generatorExecutePulseSequence(ExpName, 40000);
        setXrayEnable(false);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!!
        if (large_focus) setExposedData(DB_INDEX_2D_MANUAL_PULSE, current_exposure_pulse_number, Exposures::getExposurePulse(current_exposure_pulse_number)->filter, LARGE_FOCUS_IDX);
        else setExposedData(DB_INDEX_2D_MANUAL_PULSE, current_exposure_pulse_number, Exposures::getExposurePulse(current_exposure_pulse_number)->filter, SMALL_FOCUS_IDX);

    }
    else {
        // Demo pulse implementation
        exposure_pulse^ epulse = Exposures::getExposurePulse(current_exposure_pulse_number);

        // Activate the Buzzer in manual mode
        PCB301::setBuzzerManualMode(true);

        float result_mas = demo2DPulse(epulse->mAs, getSelectedAnodeCurrent());
        if (result_mas <= 0) {
            setExposedPulse(current_exposure_pulse_number, gcnew exposure_pulse(epulse->getKv(), -1 * result_mas, epulse->getFilter()));
            error = exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }
        else {
            setExposedPulse(current_exposure_pulse_number, gcnew exposure_pulse(epulse->getKv(), epulse->getmAs(), epulse->getFilter()));
            error = exposure_completed_errors::XRAY_NO_ERRORS;
        }
        PCB301::setBuzzerManualMode(false);

    }


    return error;
};

Exposures::exposure_completed_errors Exposures::aec_ae_exposure_procedure(bool demo) {
    System::String^ ExpName;
    bool large_focus;
    bool detector_synch = true;
    bool grid_synch = true;

    System::String^ detector_param;
    int exposure_time;
    exposure_completed_errors  error;
    
    unsigned char current_exposure_pulse_number;

    // PULSE NUMBER 0 IN THE SEQUENCE __________________________________________________________________________________________________
    current_exposure_pulse_number = EXP_PULSE_0;
    // Resets of the Internal Generator Module exposed pulses for all data-banks    
    if (!demo) R2CP::CaDataDicGen::GetInstance()->resetExecutedPulse();


    if (demo) ExpName = "Demo Exposure AE + AEC>";
    else ExpName = "Exposure AE + AEC>";

    try {
        detector_param = Exposures::getDetectorType().ToString();
        exposure_time = System::Convert::ToInt16(DetectorConfig::Configuration->getParam(detector_param)[DetectorConfig::PARAM_MAX_2D_PRE_INTEGRATION_TIME]);
    }
    catch (...) {
        LogClass::logInFile(ExpName + "Invalid Detector Parameter ");
        return exposure_completed_errors::XRAY_INVALID_2D_PARAMETERS;
    }

    // Set the filter selected is the expected into the pulse(0). No wait for positioning here    
    PCB303::selectFilter(getExposurePulse(current_exposure_pulse_number)->filter);


    // Set the grid to the requested status 
    if (getGrid() == grid_selection_index::GRID_AUTO) {

        PCB304::setAutoGridInField(); // The normal grid status in 2D exposure is Grid In Field

    }
    else if (getGrid() == grid_selection_index::GRID_IN) {
        PCB304::setAutoGridInField();
    }
    else {
        PCB304::setAutoGridOutField();
    }

    if (PCB304::isGridInFieldStatus()) {
        grid_synch = true;
        PCB304::syncGeneratorOn();
        LogClass::logInFile(ExpName + "GRID IN ");
    }
    else {
        grid_synch = false;
        PCB304::syncGeneratorOff(false);
        LogClass::logInFile(ExpName + "GRID OUT ");
    }

    // Sets the current focus
    large_focus = true;
    
    if (large_focus) LogClass::logInFile(ExpName + "FOCUS LARGE ");
    else LogClass::logInFile(ExpName + "FOCUS SMALL ");



    // Verifies that a valid the format collimation is present
    if (!PCB303::isValidCollimationFormat()) {
        return Exposures::exposure_completed_errors::XRAY_COLLI_FORMAT_ERROR;
    }

    // Pre-Pulse Preparation
    System::String^ exposure_data_str = ExpName + "AEC PRE-PULSE DATA ---------------- "; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR TYPE: " + detector_param; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "DETECTOR MAX AEC INTEGRATION TIME: " + exposure_time; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Filter Pre:" + Exposures::getExposurePulse(current_exposure_pulse_number)->filter.ToString(); LogClass::logInFile(exposure_data_str);

    //exposure_time = 5000;
    error = (exposure_completed_errors)generator2DAecPrePulsePreparation(ExpName, grid_synch, Exposures::getExposurePulse(current_exposure_pulse_number)->kV, Exposures::getExposurePulse(current_exposure_pulse_number)->mAs, large_focus, exposure_time);
    if (error != Exposures::exposure_completed_errors::XRAY_NO_ERRORS) return error;

    // Checks the filter in position
    if (!PCB303::waitFilterCompleted()) return Exposures::exposure_completed_errors::XRAY_FILTER_ERROR;

    // Checks the grid in positon
    if (!PCB304::waitGridCompleted()) return Exposures::exposure_completed_errors::XRAY_GRID_ERROR;

    if (!demo) {
        // Sequence for the AEC: only the Standby is admitted as returned code: the WaitFootRelease is not admitted here
        error = (exposure_completed_errors)generatorExecutePulseSequence(ExpName, 40000);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!!
        if (large_focus) setExposedData(DB_INDEX_2D_AEC_PRE, current_exposure_pulse_number, getExposurePulse(current_exposure_pulse_number)->filter, LARGE_FOCUS_IDX);
        else setExposedData(DB_INDEX_2D_AEC_PRE, current_exposure_pulse_number, getExposurePulse(current_exposure_pulse_number)->filter, SMALL_FOCUS_IDX);

        if (getGeneratorStatus() != R2CP::Stat_Standby) error = exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
    }
    else {
        // Demo pre-pulse implementation
        exposure_pulse^ epulse = Exposures::getExposurePulse(current_exposure_pulse_number);

        // Activate the Buzzer in manual mode
        PCB301::setBuzzerManualMode(true);

        float result_mas = demo2DPulse(epulse->mAs, getSelectedAnodeCurrent());
        if (result_mas <= 0) {
            setExposedPulse(current_exposure_pulse_number, gcnew exposure_pulse(epulse->getKv(), -1 * result_mas, epulse->getFilter()));
            error = exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }
        else {
            setExposedPulse(current_exposure_pulse_number, gcnew exposure_pulse(epulse->getKv(), epulse->getmAs(), epulse->getFilter()));
            error = exposure_completed_errors::XRAY_NO_ERRORS;
        }

        PCB301::setBuzzerManualMode(false);
    }

    
    // If the sequence should return an error condition termines here
    if (error != exposure_completed_errors::XRAY_NO_ERRORS) return error;


    // Sends the pulse-completed event to the AWS
    awsProtocol::EVENT_exposurePulseCompleted(current_exposure_pulse_number);
    
    // EXPOSURE PULSE #1: The Pulse section of the AEC sequence _________________________________________________________________________
    current_exposure_pulse_number = EXP_PULSE_1;
    // Resets of the Internal Generator Module exposed pulses for all data-banks    
    if (!demo) R2CP::CaDataDicGen::GetInstance()->resetExecutedPulse();


    // Waits for the Validated Pulse from the acquisition software
    int timeout = 100; // 10 seconds timeout
    while (timeout--) {
        if (getExposurePulse(current_exposure_pulse_number)->isValid()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (!timeout) {
        return exposure_completed_errors::XRAY_TIMEOUT_AEC;
    }
    getExposurePulse(current_exposure_pulse_number)->validated = false;

    // Set the filter selected is the expected into the pulse(1). No wait for positioning here    
    PCB303::selectFilter(getExposurePulse(current_exposure_pulse_number)->filter);

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
    exposure_data_str = "Filter:" + Exposures::getExposurePulse(current_exposure_pulse_number)->filter.ToString(); LogClass::logInFile(exposure_data_str);
    error = (exposure_completed_errors)generator2DAecPulsePreparation(ExpName, grid_synch, Exposures::getExposurePulse(current_exposure_pulse_number)->kV, Exposures::getExposurePulse(current_exposure_pulse_number)->mAs, large_focus, exposure_time);
    if (error != Exposures::exposure_completed_errors::XRAY_NO_ERRORS) return error;


    if (!demo) {

        error = (exposure_completed_errors)generatorExecutePulseSequence(ExpName, 40000);
        setXrayEnable(false);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!! 
        if (large_focus) setExposedData(DB_INDEX_2D_AEC_PULSE, current_exposure_pulse_number, getExposurePulse(current_exposure_pulse_number)->filter, LARGE_FOCUS_IDX);
        else setExposedData(DB_INDEX_2D_AEC_PULSE, current_exposure_pulse_number, getExposurePulse(current_exposure_pulse_number)->filter, SMALL_FOCUS_IDX);
    }
    else {
        // Demo pulse implementation
        exposure_pulse^ epulse = Exposures::getExposurePulse(current_exposure_pulse_number);

        // Activate the Buzzer in manual mode
        PCB301::setBuzzerManualMode(true);

        float result_mas = demo2DPulse(epulse->mAs, getSelectedAnodeCurrent());
        if (result_mas <= 0) {
            setExposedPulse(current_exposure_pulse_number, gcnew exposure_pulse(epulse->getKv(), -1 * result_mas, epulse->getFilter()));
            error = exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }
        else {
            setExposedPulse(current_exposure_pulse_number, gcnew exposure_pulse(epulse->getKv(), epulse->getmAs(), epulse->getFilter()));
            error = exposure_completed_errors::XRAY_NO_ERRORS;
        }

        PCB301::setBuzzerManualMode(false);
    }

    if (error != exposure_completed_errors::XRAY_NO_ERRORS) return error;

    // Sends the pulse-completed event to the AWS
    awsProtocol::EVENT_exposurePulseCompleted(current_exposure_pulse_number);

    // ______________ HIGH ENERGY EXPOSURE PULSE: Pulse #2  _______________________________________________________________
    current_exposure_pulse_number = EXP_PULSE_2;
    // Resets of the Internal Generator Module exposed pulses for all data-banks    
    if (!demo) R2CP::CaDataDicGen::GetInstance()->resetExecutedPulse();


    // The filter is quickly set here as CU (usually is CU)
    PCB303::selectFilter(PCB303::filter_index::FILTER_CU);

    // Waits for the Validated Pulse from the acquisition software
    timeout = 100; // 10 seconds timeout
    while (timeout--) {
        if (getExposurePulse(current_exposure_pulse_number)->isValid()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (!timeout) {
        return exposure_completed_errors::XRAY_TIMEOUT_AEC;
    }
    getExposurePulse(current_exposure_pulse_number)->validated = false;

    // In the case the filter dshould be different by the standard Cu then the 
    // filter is changed again: this takes more time then the normal shenario
    if (Exposures::getExposurePulse(current_exposure_pulse_number)->filter != PCB303::filter_index::FILTER_CU) {
        // Wait for the previous command termination then change the filter in the requested (non Cu)
        PCB303::waitFilterCompleted();
        PCB303::selectFilter(Exposures::getExposurePulse(current_exposure_pulse_number)->filter);
    }

    error = (exposure_completed_errors)generator2DPulsePreparation(ExpName, Exposures::getExposurePulse(current_exposure_pulse_number)->kV, Exposures::getExposurePulse(current_exposure_pulse_number)->mAs, large_focus, detector_synch, grid_synch, exposure_time);
    if (error != Exposures::exposure_completed_errors::XRAY_NO_ERRORS) return error;

    // Checks the filter in position
    if (!PCB303::waitFilterCompleted()) return Exposures::exposure_completed_errors::XRAY_FILTER_ERROR;

    if (!demo) {
        error = (exposure_completed_errors)generatorExecutePulseSequence(ExpName, 40000);
        setXrayEnable(false);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!!
        if (large_focus) setExposedData(DB_INDEX_2D_MANUAL_PULSE, current_exposure_pulse_number, Exposures::getExposurePulse(current_exposure_pulse_number)->filter, LARGE_FOCUS_IDX);
        else setExposedData(DB_INDEX_2D_MANUAL_PULSE, current_exposure_pulse_number, Exposures::getExposurePulse(current_exposure_pulse_number)->filter, SMALL_FOCUS_IDX);

    }
    else {
        // Demo pulse implementation
        exposure_pulse^ epulse = Exposures::getExposurePulse(current_exposure_pulse_number);

        // Activate the Buzzer in manual mode
        PCB301::setBuzzerManualMode(true);

        float result_mas = demo2DPulse(epulse->mAs, getSelectedAnodeCurrent());
        if (result_mas <= 0) {
            setExposedPulse(current_exposure_pulse_number, gcnew exposure_pulse(epulse->getKv(), -1 * result_mas, epulse->getFilter()));
            error = exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }
        else {
            setExposedPulse(current_exposure_pulse_number, gcnew exposure_pulse(epulse->getKv(), epulse->getmAs(), epulse->getFilter()));
            error = exposure_completed_errors::XRAY_NO_ERRORS;
        }
        PCB301::setBuzzerManualMode(false);

    }


    return error;

};
