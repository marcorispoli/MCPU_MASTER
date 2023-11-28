
#include "../gantry_global_status.h"
#include "awsProtocol.h"
#include "ArmMotor.h"
#include "PCB315.h"
#include "PCB302.h"
#include "PCB303.h"
#include "ExposureModule.h"
#include "Generator.h"

using namespace GantryStatusRegisters;
using namespace System::Diagnostics;

/// <summary>
/// This is the EVENT requesting the selection of a given projection.
/// 
/// The projection shall belong to the projections list set by the AWS through 
/// the command SET_ProjectionList.
/// 
/// </summary>
/// <param name="projname">This is the Tag of the requested projection</param>
void awsProtocol::EVENT_SelectProjection(String^ projname) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EVENT_SelectProjection  " + projname + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_AbortProjection(void) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EVENT_AbortProjection %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_GantryStatus(String^ status) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EVENT_GantryStatus  " + status + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_Compressor(unsigned short thick, unsigned short force) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EVENT_Compressor  " + thick.ToString() + "  " + force.ToString() + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_Components(String^ component, String^ paddle, String^ colli_component) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EVENT_Components  " + component + " " + paddle + " " + colli_component + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}
void awsProtocol::EVENT_ReadyForExposure(bool ready, unsigned short code) {
    event_counter++;
    String^ answer;
    if (ready) {
        answer = "<" + event_counter.ToString() + " %EVENT_ReadyForExposure OK 0 %>";
    }
    else {
        answer = "<" + event_counter.ToString() + " %EVENT_ReadyForExposure NOK " + code.ToString() + " %>";
    }

    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_XrayPushButton(String^ status) {
    event_counter++;
    String^ answer = "<" + event_counter.ToString() + " %EVENT_XrayPushButton  " + status + " %>";
    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
}

void awsProtocol::EVENT_XraySequenceCompleted(String^ result,
    double kv0, double mas0, String^ filter0,
    double kv1, double mas1, String^ filter1,
    double kv2, double mas2, String^ filter2,
    double kv3, double mas3, String^ filter3) {
    event_counter++;
    String^ answer;

    System::Globalization::CultureInfo^ myInfo = gcnew  System::Globalization::CultureInfo("en-US", false);

    answer = "<" + event_counter.ToString() + " %EVENT_XraySequenceCompleted  " + result;
    answer += " " + kv0.ToString(myInfo) + " " + mas0.ToString(myInfo) + " " + filter0;
    answer += " " + kv1.ToString(myInfo) + " " + mas1.ToString(myInfo) + " " + filter1;
    answer += " " + kv2.ToString(myInfo) + " " + mas2.ToString(myInfo) + " " + filter2;
    answer += " " + kv3.ToString(myInfo) + " " + mas3.ToString(myInfo) + " " + filter3;
    answer += " %>";

    event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));

}




void awsProtocol::activationCompletedCallback(int id, int error) {
    if (error) eventExecutedNok(id, error);
    else eventExecutedOk(id);
}

void awsProtocol::selectProjectionCallback(String^ proj) {
    EVENT_SelectProjection(proj);
};

void awsProtocol::abortProjectionCallback(void) {
    EVENT_AbortProjection();
};

void awsProtocol::gantryStatusChangeCallback(void) {
    EVENT_GantryStatus(OperatingStatusRegister::getStatus());
}

void awsProtocol::compressorDataChangeCallback(void) {
    EVENT_Compressor(CompressorRegister::getThickness(), CompressorRegister::getForce());
}

void awsProtocol::componentDataChangeCallback(void) {
    EVENT_Components(ComponentRegister::Value->getTag(), CompressorRegister::getPaddle()->Value->getTag(), CollimatorComponentRegister::Value->getTag());
}


void awsProtocol::xrayPushbuttonStatusChangeCallback(void) {
    if (XrayPushButtonRegister::isEnabled()) {
        EVENT_XrayPushButton(XrayPushButtonRegister::activationStatus->getTag());
    }
}

void awsProtocol::exposureSequenceCompletedCallback(Generator::exposure_completed_options code ) {

    String^ result;
    if (code == Generator::exposure_completed_options::XRAY_SUCCESS) result = "OK";
    else if (code == Generator::exposure_completed_options::XRAY_PARTIAL_DOSE) result = "PARTIAL";
    else  result = "NOK";

    EVENT_XraySequenceCompleted(result,
        ExposureModule::getExposedPulse(0)->getKv(), ExposureModule::getExposedPulse(0)->getmAs(), PCB315::getTagFromFilter(ExposureModule::getExposedPulse(0)->getFilter()),
        ExposureModule::getExposedPulse(1)->getKv(), ExposureModule::getExposedPulse(1)->getmAs(), PCB315::getTagFromFilter(ExposureModule::getExposedPulse(1)->getFilter()),
        ExposureModule::getExposedPulse(2)->getKv(), ExposureModule::getExposedPulse(2)->getmAs(), PCB315::getTagFromFilter(ExposureModule::getExposedPulse(2)->getFilter()),
        ExposureModule::getExposedPulse(3)->getKv(), ExposureModule::getExposedPulse(3)->getmAs(), PCB315::getTagFromFilter(ExposureModule::getExposedPulse(3)->getFilter())
    );

}
