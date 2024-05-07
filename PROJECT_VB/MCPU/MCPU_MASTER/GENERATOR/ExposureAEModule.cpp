#include "ExposureModule.h"
#include "Generator.h"
#include <thread>

using namespace System::Diagnostics;
using namespace System::Collections::Generic;


Exposures::exposure_completed_errors Exposures::man_ae_exposure_procedure(bool demo) {
    // Set thecommunication error code as general case
    return exposure_completed_errors::XRAY_NO_ERRORS;

};

Exposures::exposure_completed_errors Exposures::aec_ae_exposure_procedure(bool demo) {
    // Set thecommunication error code as general case
    return exposure_completed_errors::XRAY_NO_ERRORS;

};
