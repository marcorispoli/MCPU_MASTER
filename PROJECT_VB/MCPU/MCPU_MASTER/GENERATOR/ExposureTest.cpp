#include "ExposureModule.h"
#include "Generator.h"
#include "R2CP_Eth.h"
#include "CaDataDicGen.h"
#include "PCB304.h"
#include "PCB301.h"
#include <thread>
#include "Log.h"

using namespace System::Diagnostics;
using namespace System::Collections::Generic;


ExposureModule::exposure_completed_errors Generator::test_exposure_procedure(void) {
    System::String^ ExpName = "Exposure Test>";
    unsigned char focus = ExposureModule::getExposurePulse(0)->getFocus();
    bool large_focus = ExposureModule::getExposurePulse(0)->isLargeFocus();
    bool detector_synch = ExposureModule::getExposurePulse(0)->useDetector();
    bool grid_synch = ExposureModule::getExposurePulse(0)->useGrid();
    PCB315::filterMaterialCodes filter;
    unsigned char current_status;
    int timeout;
  
   

    // Sets the Grid On/Off Field and wait for the ready condition
    if (grid_synch) {        
        PCB304::synchGridWithGenerator(true);
        if (!PCB304::setGridOnField(true)) {
            return ExposureModule::exposure_completed_errors::XRAY_GRID_ERROR;
        }       
    }
    else {        
        PCB304::synchGridWithGenerator(false);
        if (!PCB304::setGridOffField(true)) {
            return ExposureModule::exposure_completed_errors::XRAY_GRID_ERROR;
        }        
    }


    // Filter selection 
    filter = ExposureModule::getExposurePulse(0)->filter;
    LogClass::logInFile(ExpName + "filter:" + filter.ToString());

    if (!PCB315::setFilterAutoMode(filter, true)) {
        return ExposureModule::exposure_completed_errors::XRAY_FILTER_ERROR;
    }

    // Load Data Bank For pulse    
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pulse, large_focus, ExposureModule::getExposurePulse(0)->kV, ExposureModule::getExposurePulse(0)->mAs, 5000);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Generator_Set_2D_Databank error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

       
    if (detector_synch) grid_synch = true; // Procedure activation 
    R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DProcedurePulse(detector_synch, grid_synch);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Patient_Activate2DProcedurePulse error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Clear the active system messages
    if (!clearSystemMessages()) {
        LogClass::logInFile(ExpName + "SystemMessages_Clear_Message error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }


    // Disables the safety disable RX message    
    R2CP::CaDataDicGen::GetInstance()->SystemMessages_SetDisableRx(false);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "SystemMessages_SetDisableRx error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Gets the current generator status
    R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile(ExpName + "Generator_Get_StatusV6 error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }
    current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    // Status not in standby
    if (current_status != R2CP::Stat_Standby) {
        LogClass::logInFile(ExpName + "generator not in standby error");
        return ExposureModule::exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
    }

   
    // Activate the X-RAY Enable Interface signal on the PCB301 board
    PCB301::set_xray_ena(true);

    ExposureModule::exposure_completed_errors  error = commonExposurePulseSequence(ExpName, false);
    
    // Updates the Exposed pulses     
    setExposedData(R2CP::DB_Pulse, (unsigned char) 0, filter, focus);
    return error;

};
