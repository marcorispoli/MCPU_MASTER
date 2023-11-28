#include "ExposureModule.h"
#include "Generator.h"
#include <thread>

using namespace System::Diagnostics;
using namespace System::Collections::Generic;


bool Generator::man_combo_exposure_procedure(void) {
    // Set thecommunication error code as general case
    xray_exposure_error = exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    return false;
};

bool Generator::aec_combo_exposure_procedure(void) {
    // Set thecommunication error code as general case
    xray_exposure_error = exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    return false;

};
