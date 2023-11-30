
#include "../gantry_global_status.h"
#include "awsProtocol.h"
#include "ArmMotor.h"
#include "PCB315.h"
#include "PCB302.h"
#include "PCB303.h"
#include "ExposureModule.h"
#include "Generator.h"
#include "mutex"

using namespace GantryStatusRegisters;
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
    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_SelectProjection  " + projname + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_AbortProjection(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_AbortProjection %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_GantryStatus(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    String^ status = OperatingStatusRegister::getStatus();
    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_GantryStatus  " + status + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_Compressor(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    unsigned short thick = CompressorRegister::getThickness();
    unsigned short force = CompressorRegister::getForce();

    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_Compressor  " + thick.ToString() + "  " + force.ToString() + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_Components(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    String^ component = ComponentRegister::Value->getTag();
    String^ paddle = CompressorRegister::getPaddle()->Value->getTag();
    String^ colli_component = CollimatorComponentRegister::Value->getTag();
    
    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_Components  " + component + " " + paddle + " " + colli_component + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_ReadyForExposure(bool ready, unsigned short code) {
    const std::lock_guard<std::mutex> lock(event_mutex);
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

void awsProtocol::EVENT_XrayPushButton(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (XrayPushButtonRegister::isEnabled()) {
        System::String^  status = XrayPushButtonRegister::activationStatus->getTag();
        device->event_counter++;
        String^ answer = "<" + device->event_counter.ToString() + " %EVENT_XrayPushButton  " + status + " %>";
        device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    }

   
}

void awsProtocol::EVENT_XraySequenceCompleted(Generator::exposure_completed_options code) {

    const std::lock_guard<std::mutex> lock(event_mutex);
    String^ result;
    if (code == Generator::exposure_completed_options::XRAY_SUCCESS) result = "OK";
    else if (code == Generator::exposure_completed_options::XRAY_PARTIAL_DOSE) result = "PARTIAL";
    else  result = "NOK";

   
    device->event_counter++;
    String^ answer;

    System::Globalization::CultureInfo^ myInfo = gcnew  System::Globalization::CultureInfo("en-US", false);

    answer = "<" + device->event_counter.ToString() + " %EVENT_XraySequenceCompleted  " + result;
    answer += " " + ExposureModule::getExposedPulse(0)->getKv().ToString(myInfo) + " " + ExposureModule::getExposedPulse(0)->getmAs().ToString(myInfo) + " " + PCB315::getTagFromFilter(ExposureModule::getExposedPulse(0)->getFilter());
    answer += " " + ExposureModule::getExposedPulse(1)->getKv().ToString(myInfo) + " " + ExposureModule::getExposedPulse(1)->getmAs().ToString(myInfo) + " " + PCB315::getTagFromFilter(ExposureModule::getExposedPulse(1)->getFilter());
    answer += " " + ExposureModule::getExposedPulse(2)->getKv().ToString(myInfo) + " " + ExposureModule::getExposedPulse(2)->getmAs().ToString(myInfo) + " " + PCB315::getTagFromFilter(ExposureModule::getExposedPulse(2)->getFilter());
    answer += " " + ExposureModule::getExposedPulse(3)->getKv().ToString(myInfo) + " " + ExposureModule::getExposedPulse(3)->getmAs().ToString(myInfo) + " " + PCB315::getTagFromFilter(ExposureModule::getExposedPulse(3)->getFilter());
    answer += " %>";

    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));

}


void awsProtocol::EVENT_ActivationCompleted(int id, int error) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (error) device->eventExecutedNok(id, error);
    else device->eventExecutedOk(id);
}


