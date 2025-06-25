#include "ExposureModule.h"
#include "Generator.h"
#include "R2CP_Eth.h"
#include "CaDataDicGen.h"
#include "PCB304.h"
#include "PCB301.h"
#include <thread>
#include "Log.h"


using namespace System::Collections::Generic;

Exposures::exposure_completed_errors Exposures::test_exposure_procedure(bool demo) {
    System::String^ ExpName ;
    unsigned char focus = Exposures::getExposurePulse(0)->getFocus();
    bool large_focus = Exposures::getExposurePulse(0)->isLargeFocus();
    bool detector_synch = Exposures::getExposurePulse(0)->useDetector();
    bool grid_synch = Exposures::getExposurePulse(0)->useGrid();
    PCB303::filter_index filter;
    unsigned char current_status;
    int timeout;
    exposure_completed_errors  error;
  
    if (demo) ExpName = "Demo Exposure Test >";
    else ExpName = "Exposure Test >";
    
    // Sets the Grid On Field (if not yet) : wait for the ready condition   
    if (grid_synch) PCB304::syncGeneratorOn();
    else PCB304::syncGeneratorOff();

    // Filter selection 
    filter = getExposurePulse(0)->filter;
    
    // Set the filter selected is the expected into the pulse(0). No wait for positioning here    
    PCB303::selectExposureFilter(filter);
    if (PCB303::isFilterInError()) return Exposures::exposure_completed_errors::XRAY_FILTER_ERROR;

    // Pulse Preparation
    System::String^ exposure_data_str = ExpName + " PULSE DATA ---------------- "; LogClass::logInFile(exposure_data_str);   
    exposure_data_str = "Filter:" + Exposures::getExposurePulse(0)->filter.ToString(); LogClass::logInFile(exposure_data_str);
    error = (exposure_completed_errors) generator2DPulsePreparation(ExpName, Exposures::getExposurePulse(0)->kV, Exposures::getExposurePulse(0)->mAs, large_focus, detector_synch, grid_synch, 5000);
    if (error != Exposures::exposure_completed_errors::XRAY_NO_ERRORS) return error;

    
    if (!demo) {
        error = (exposure_completed_errors)generatorExecutePulseSequence(ExpName, 40000);

        // The index is the number associated to the Databank in the procedure definition. It is not the Databank index value itself!! 
        setExposedData(1, (unsigned char)0, filter, focus);
    }
    else {
        // Demo pulse implementation
        exposure_pulse^ epulse = Exposures::getExposurePulse(0);
        float mA = getSelectedAnodeCurrent();

        // Activate the Buzzer in manual mode
        PCB301::setBuzzerManualMode(true);

        float result_mas = demo2DPulse(Exposures::getExposurePulse(0)->mAs, mA);
        if (result_mas <= 0) {
            result_mas = -1 * result_mas;
            error = exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }
        else {
            result_mas = epulse->getmAs();
            error = exposure_completed_errors::XRAY_NO_ERRORS;
        }

        setExposedPulse(0, gcnew exposure_pulse(epulse->getKv(), result_mas, epulse->getFilter(), mA, (result_mas * 1000) / mA, 1, 1));
        PCB301::setBuzzerManualMode(false);
    }
    
    return error;

};
