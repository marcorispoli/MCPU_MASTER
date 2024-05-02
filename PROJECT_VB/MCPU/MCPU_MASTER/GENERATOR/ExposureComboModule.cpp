#include "ExposureModule.h"
#include "Generator.h"
#include <thread>

using namespace System::Diagnostics;
using namespace System::Collections::Generic;


ExposureModule::exposure_completed_errors ExposureModule::man_combo_exposure_procedure(bool demo) {
    // Set thecommunication error code as general case
    return exposure_completed_errors::XRAY_NO_ERRORS;
};

ExposureModule::exposure_completed_errors ExposureModule::aec_combo_exposure_procedure(bool demo) {
    // Set thecommunication error code as general case
    return exposure_completed_errors::XRAY_NO_ERRORS;

};
