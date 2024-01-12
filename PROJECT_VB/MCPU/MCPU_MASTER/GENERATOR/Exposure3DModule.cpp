#include "ExposureModule.h"
#include "Generator.h"
#include <thread>

using namespace System::Diagnostics;
using namespace System::Collections::Generic;



ExposureModule::exposure_completed_errors Generator::man_3d_exposure_procedure(void) {
    // Set thecommunication error code as general case
    return ExposureModule::exposure_completed_errors::XRAY_INVALID_PROCEDURE;

};

ExposureModule::exposure_completed_errors Generator::aec_3d_exposure_procedure(void) {
    // Set thecommunication error code as general case
    return ExposureModule::exposure_completed_errors::XRAY_INVALID_PROCEDURE;

};
