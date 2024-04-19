
#include "../gantry_global_status.h"
#include "awsProtocol.h"
#include "ArmMotor.h"
#include "PCB315.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB301.h"
#include "PCB304.h"
#include "PCB326.h"
#include "ExposureModule.h"
#include "Generator.h"
#include "mutex"


using namespace System::Diagnostics;

static std::mutex event_mutex;


/// <summary>
/// This is the EVENT requesting the selection of a given projection.
/// 
/// The projection shall belong to the projections list set by the AWS through 
/// the command SET_ProjectionList.
/// 
/// Syntax:
/// 
/// </summary>
/// <param name="projname">This is the Tag of the requested projection</param>
void awsProtocol::EVENT_SelectProjection(String^ projname) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;

    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_SelectProjection  " + projname + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_AbortProjection(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;
    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_AbortProjection %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_GantryStatus(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;
    String^ status = Gantry::getOperatingStatusName();
    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_GantryStatus  " + status + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_Compressor(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;
    unsigned short thick = PCB302::getThickness();
    unsigned short force = PCB302::getForce();

    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_Compressor  " + thick.ToString() + "  " + force.ToString() + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

/// <summary>
/// Vedere GET_Component
/// </summary>
/// <param name=""></param>
void awsProtocol::EVENT_Components(void) {
    if (!device->event_server) return;
    const std::lock_guard<std::mutex> lock(event_mutex);
    System::String^ potter_type;
    System::String^ mag_factor;
    System::String^ paddle;
    System::String^ protection;
    System::String^ colli_tool;

    // Potter Type parameter
    if (Biopsy::isBiopsy()) {
        potter_type = "BIOPSY";
    }
    else  if (PCB304::isMagnifierDeviceDetected()) {
        potter_type = "MAGNIFIER";
    }
    else {
        potter_type = "POTTER";
    }

    // Magnification factor
    mag_factor = PCB304::getMagnifierfactorString();

    // Compressor paddle
    paddle = PCB302::getDetectedPaddleCode().ToString();

    // Protection Type
    if (PCB302::isPatientProtection()) protection = "PROTECTION_3D";
    //else if (PCB315::getComponent() == PCB315::component_options::PROTECTION_2D) protection = "PROTECTION_2D";
    else protection = "UNDETECTED_PROTECTION";

    // Collimation Tool
    if (PCB315::getComponent() == PCB315::component_options::LEAD_SCREEN)  colli_tool = "LEAD_SCREEN";
    else if (PCB315::getComponent() == PCB315::component_options::SPECIMEN)  colli_tool = "SPECIMEN";
    else colli_tool = "UNDETECTED_COLLIMATOR";

    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_Components  " + potter_type + " " + mag_factor + " " + paddle + " " + protection + " " + colli_tool + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_ReadyForExposure(bool ready, unsigned short code) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;
    device->event_counter++;
    String^ answer;
    if (ready) {
        answer = "<" + device->event_counter.ToString() + " %EVENT_ReadyForExposure OK 0 %>";
    }
    else {
        answer = "<" + device->event_counter.ToString() + " %EVENT_ReadyForExposure NOK " + code.ToString() + " %>";
    }

    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_XrayPushButton(bool status) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;
    
    device->event_counter++;
    System::String^ strstat;
    if (status) strstat = "ON";
    else strstat = "OFF";

    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_XrayPushButton  " + strstat + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    
   
}

void awsProtocol::EVENT_exposurePulseCompleted(unsigned char npulse) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;

    device->event_counter++;
   
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_exposurePulseCompleted  " + npulse.ToString() + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_XraySequenceCompleted(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;
    String^ result;

    ExposureModule::exposure_completed_options code = ExposureModule::getExposureCompletedCode();
    if (code == ExposureModule::exposure_completed_options::XRAY_SUCCESS) result = "OK";
    else if (code == ExposureModule::exposure_completed_options::XRAY_PARTIAL_DOSE) result = "PARTIAL";
    else  result = "NOK";

    ExposureModule::exposure_completed_errors error = ExposureModule::getExposureCompletedError();

    device->event_counter++;
    String^ answer;

    System::Globalization::CultureInfo^ myInfo = gcnew  System::Globalization::CultureInfo("en-US", false);

    answer = "<" + device->event_counter.ToString() + " %EVENT_XraySequenceCompleted  " + result + " " + ((int)error).ToString();
    answer += " " + ExposureModule::getExposedPulse(0)->getKv().ToString(myInfo) + " " + ExposureModule::getExposedPulse(0)->getmAs().ToString(myInfo) + " " + PCB315::getTagFromFilter(ExposureModule::getExposedPulse(0)->getFilter());
    answer += " " + ExposureModule::getExposedPulse(1)->getKv().ToString(myInfo) + " " + ExposureModule::getExposedPulse(1)->getmAs().ToString(myInfo) + " " + PCB315::getTagFromFilter(ExposureModule::getExposedPulse(1)->getFilter());
    answer += " " + ExposureModule::getExposedPulse(2)->getKv().ToString(myInfo) + " " + ExposureModule::getExposedPulse(2)->getmAs().ToString(myInfo) + " " + PCB315::getTagFromFilter(ExposureModule::getExposedPulse(2)->getFilter());
    answer += " " + ExposureModule::getExposedPulse(3)->getKv().ToString(myInfo) + " " + ExposureModule::getExposedPulse(3)->getmAs().ToString(myInfo) + " " + PCB315::getTagFromFilter(ExposureModule::getExposedPulse(3)->getFilter());
    answer += " %>";

    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));

}


/// <summary>
/// This Event is sent to AWS whenever the gantry requestes for a power-off sequence
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::EVENT_Poweroff(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;

    device->event_counter++;

    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_Poweroff %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

/// <summary>
/// This is the EVENT the Gantry shall generate when a delayed command completes.
/// 
/// </summary>
/// 
/// The Id == 0 codes are rserved for internal activations that shall not 
/// be acknowledge to the AWS softwrae.
/// 
/// <param name="id">Id > 0 is the ID of the pending AWS command, Id==0 is an internal command termination Event</param>
/// <param name="error">In case of error condition, it is the error code</param>
void awsProtocol::EVENT_Executed(int id, int error) {
    if (id == 0) return; // Only with valid ID's

    const std::lock_guard<std::mutex> lock(event_mutex);
    if (error) device->eventExecutedNok(id, error);
    else device->eventExecutedOk(id);
}


