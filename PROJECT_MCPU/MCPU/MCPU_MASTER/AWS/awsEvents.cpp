
#include "../gantry_global_status.h"
#include "awsProtocol.h"
#include "ArmMotor.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB301.h"
#include "PCB304.h"
#include "PCB326.h"
#include "ExposureModule.h"
#include "Generator.h"
#include "mutex"

/// \addtogroup AWSProtocolDescription
///  
/// <div style="page-break-after: always;"></div>
/// 
/// \section EVENTDESC Event Description Section
///

using namespace System::Diagnostics;

static std::mutex event_mutex;


/// \addtogroup AWSProtocolDescription
///
/// \subsection EVENT_SelectProjection 
/// 
/// This is the EVENT requesting the selection of a given projection.
/// 
/// The projection shall belong to the projections list set by the AWS through
/// the command \ref SET_ProjectionList.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EVENT_SelectProjection projection_name %>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |projection_name|String|It is one of the available projections. See the \ref ProtoProjection section| 
/// 



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
    LogClass::logInFile(answer);
}

/// \addtogroup AWSProtocolDescription
///
/// \subsection EVENT_AbortProjection 
/// 
/// This is the EVENT requesting to abort the current selected projection.
/// 
/// The Gantry sends this command whrn the operatore press the _Abort Button_ on the GUI display.
/// 
/// The Gantry doesn't proceed with the abort by its own.
/// 
/// The AWS will send the \ref EXEC_AbortProjection command in the case should 
/// accept the abort request.
/// 
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EVENT_AbortProjection  %>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-| 
/// 

/// <summary>
/// This is the EVENT requesting to abort the current selected projection.
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::EVENT_AbortProjection(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;
    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_AbortProjection %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
}

/// \addtogroup AWSProtocolDescription
///
/// \subsection EVENT_GantryStatus 
/// 
/// Gantry sends this EVENT always its internal status changes.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EVENT_GantryStatus  current_status %>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |current_status|String|the Gantry current status (see table below)|
/// 
/// |current_status|Description
/// |:--|:--|
/// |GANTRY_STARTUP|Startup fase|
/// |GANTRY_IDLE|Idle Status: not in Open Study|
/// |GANTRY_OPEN_STUDY|This is the Operating status |
/// |GANTRY_SERVICE|This is the Service status |
/// |UNDEF|This is a non defined Gantry Status|
 
/// <summary>
/// Gantry sends this EVENT always its internal status changes.
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::EVENT_GantryStatus(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;
    String^ status = Gantry::getOperatingStatusName();
    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_GantryStatus  " + status + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
}

/// \addtogroup AWSProtocolDescription
///
/// \subsection EVENT_Compressor 
/// 
/// Gantry sends this EVENT to notify the compressor data changes.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EVENT_Compressor  breast_thick compression_force %>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |breast_thick|Int|current measured breast thickness in mm|
/// |compression_force|Int|current measured compression force N|
/// 

/// <summary>
/// Gantry sends this EVENT to notify the compressor data changes.
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::EVENT_Compressor(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;
    unsigned short thick = PCB302::getThickness();
    unsigned short force = PCB302::getForce();

    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_Compressor  " + thick.ToString() + "  " + force.ToString() + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
}


/// \addtogroup AWSProtocolDescription
///
/// \subsection EVENT_Components 
/// 
/// Gantry sends this EVENT to notify the change in the detected components .
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EVENT_Components  component_type magnifier_factor paddle_name protection_name collimation_tool %>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |component_type|String|BIOPSY/MAGNIFIER/POTTER|
/// |magnifier_factor|Int|Current Magnifier factor|
/// |paddle_name|String|Nome of the detected compression paddle|
/// |protection_name|String|PROTECTION_POSITIONED/PROTECTION_SHIFTED/UNDETECTED_PROTECTION|
/// |collimation_tool|String|LEAD_SCREEN/SPECIMEN/UNDETECTED_COLLIMATOR|
/// 

/// <summary>
/// Gantry sends this EVENT to notify the change in the detected components .
/// 
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
    else  if (PCB304::getMagnifierFactor() != 10) {
        potter_type = "MAGNIFIER";
    }
    else {
        potter_type = "POTTER";
    }

    // Magnification factor
    mag_factor = PCB304::getMagnifierFactor().ToString();

    // Compressor paddle
    paddle = PCB302::getDetectedPaddleCode().ToString();

   
    // Protection Type
    if (PCB302::getPatientProtection() == PCB302::PatientProtection::POSITIONED) {
        protection = "PROTECTION_POSITIONED";
    }
    else if (PCB302::getPatientProtection() == PCB302::PatientProtection::SHIFTED) {
        protection = "PROTECTION_SHIFTED";
    }
    else protection = "UNDETECTED_PROTECTION";

    // Collimation Tool
    colli_tool = "UNDETECTED_COLLIMATOR";

    device->event_counter++;
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_Components  " + potter_type + " " + mag_factor + " " + paddle + " " + protection + " " + colli_tool + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
}


/// \addtogroup AWSProtocolDescription
///
/// \subsection EVENT_ReadyForExposure 
/// 
/// Gantry sends this EVENT to notify the change in the Ready For Exposure current status .
/// 
/// ### Command Data Format
/// 
/// In case of Ready condition: <ID % EVENT_ReadyForExposure  OK 0 %>
/// 
/// In case of Not Ready condition: <ID % EVENT_ReadyForExposure  NOK code %>
/// 
/// |CODE|Description|
/// |:--|:--|
/// |AWS_RET_SYSTEM_ERRORS|There are active errors in the system|
/// |AWS_RET_SYSTEM_WARNINGS|There are active warnings in the system|
/// 

/// <summary>
///  Gantry sends this EVENT to notify the change in the Ready For Exposure current status .
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::EVENT_ReadyForExposure(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;
    device->event_counter++;
    
    int code;
    if (Notify::isError()) code = (int)return_errors::AWS_RET_SYSTEM_ERRORS;
    else if (Notify::isWarning()) code = (int)return_errors::AWS_RET_SYSTEM_WARNINGS;
    else    code = 0;

    
    String^ answer;
    if (!code) {
        answer = "<" + device->event_counter.ToString() + " %EVENT_ReadyForExposure OK 0 %>";
    }
    else {
        answer = "<" + device->event_counter.ToString() + " %EVENT_ReadyForExposure NOK " + code.ToString() + " %>";
    }

    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
}

/// \addtogroup AWSProtocolDescription
///
/// \subsection EVENT_XrayPushButton 
/// 
/// Gantry sends this EVENT when the status of the X-RAY push button changes .
/// 
/// ### Command Data Format
/// 
/// <ID % EVENT_XrayPushButton  x_status %>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |x_status|String|ON/OFF|
/// 

/// <summary>
/// Gantry sends this EVENT when the status of the X-RAY push button changes .
/// 
/// </summary>
/// <param name="status"></param>
void awsProtocol::EVENT_XrayPushButton(bool status) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;
    
    device->event_counter++;
    System::String^ strstat;
    if (status) strstat = "ON";
    else strstat = "OFF";

    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_XrayPushButton  " + strstat + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
    
}

/// \addtogroup AWSProtocolDescription
///
/// \subsection EVENT_exposurePulseCompleted 
/// 
/// Gantry sends this EVENT whenever a pulse of a sequence is executed.
/// 
///     NOTE: this EVENT is not sent if the pulse is the last pulse of a sequence.
///     In this case the \ref EVENT_XraySequenceCompleted is sent instead.
/// 
/// 
/// ### Command Data Format
/// 
/// <ID % EVENT_exposurePulseCompleted  pulse_number %>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |pulse_number|Byte|Pulse number starting from 0|
/// 

/// <summary>
/// Gantry sends this EVENT whenever a pulse of a sequence is executed.
/// 
///     NOTE: this EVENT is not sent if the pulse is the last pulse of a sequence.
///     In this case the \ref EVENT_XraySequenceCompleted is sent instead. 
/// </summary>
/// <param name="npulse"></param>
void awsProtocol::EVENT_exposurePulseCompleted(unsigned char npulse) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;

    device->event_counter++;
   
    String^ answer = "<" + device->event_counter.ToString() + " %EVENT_exposurePulseCompleted  " + npulse.ToString() + " %>";
    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);
}

/// \addtogroup AWSProtocolDescription
///
/// \subsection EVENT_XraySequenceCompleted 
/// 
/// Gantry sends this EVENT when a X-Ray sequence ends, whether with or without success.
/// 
///    
/// 
/// ### Command Data Format
/// 
/// <ID % EVENT_XraySequenceCompleted result kv0 mAs0 filter0 kv1 mAs1 filter1 kv2 mAs2 filter2 kv3 mAs3 filter3 %>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |result|String|OK: exposure success;<br>NOK: exposure aborted without dose;<br>PARTIAL: exposure aborted with partial dose released|
/// |kv0|float|kV measured in the pulse #0|
/// |mAs0|float|mAs measured in the pulse #0|
/// |filter0|String|filter code used for the pulse #0|
/// |kv1|float|kV measured in the pulse #1|
/// |mAs1|float|mAs measured in the pulse #1|
/// |filter1|String|filter code used for the pulse #1|
/// |kv2|float|kV measured in the pulse #2|
/// |mAs2|float|mAs measured in the pulse #2|
/// |filter2|String|filter code used for the pulse #2|
/// |kv3|float|kV measured in the pulse #3|
/// |mAs3|float|mAs measured in the pulse #3|
/// |filter3|String|filter code used for the pulse #3|
/// 
///     NOTE: the pulse data related to pulses not in the procedure shall be not considered!



/// <summary>
/// Gantry sends this EVENT when a X-Ray sequence ends, whether with or without success.
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::EVENT_XraySequenceCompleted(void) {
    const std::lock_guard<std::mutex> lock(event_mutex);
    if (!device->event_server) return;
    String^ result;

    Exposures::exposure_completed_options code = Exposures::getExposureCompletedCode();
    if (code == Exposures::exposure_completed_options::XRAY_SUCCESS) result = "OK";
    else if (code == Exposures::exposure_completed_options::XRAY_PARTIAL_DOSE) result = "PARTIAL";
    else  result = "NOK";

    Exposures::exposure_completed_errors error = Exposures::getExposureCompletedError();

    device->event_counter++;
    String^ answer;

    System::Globalization::CultureInfo^ myInfo = gcnew  System::Globalization::CultureInfo("en-US", false);

    answer = "<" + device->event_counter.ToString() + " %EVENT_XraySequenceCompleted  " + result + " " + ((int)error).ToString();
    answer += " " + Exposures::getExposedPulse(0)->getKv().ToString(myInfo) + " " + Exposures::getExposedPulse(0)->getmAs().ToString(myInfo) + " " + PCB303::getTagFromFilter(Exposures::getExposedPulse(0)->getFilter());
    answer += " " + Exposures::getExposedPulse(1)->getKv().ToString(myInfo) + " " + Exposures::getExposedPulse(1)->getmAs().ToString(myInfo) + " " + PCB303::getTagFromFilter(Exposures::getExposedPulse(1)->getFilter());
    answer += " " + Exposures::getExposedPulse(2)->getKv().ToString(myInfo) + " " + Exposures::getExposedPulse(2)->getmAs().ToString(myInfo) + " " + PCB303::getTagFromFilter(Exposures::getExposedPulse(2)->getFilter());
    answer += " " + Exposures::getExposedPulse(3)->getKv().ToString(myInfo) + " " + Exposures::getExposedPulse(3)->getmAs().ToString(myInfo) + " " + PCB303::getTagFromFilter(Exposures::getExposedPulse(3)->getFilter());
    answer += " %>";

    device->event_server->send(System::Text::Encoding::Unicode->GetBytes(answer));
    LogClass::logInFile(answer);

}


/// \addtogroup AWSProtocolDescription
///
/// \subsection EVENT_Poweroff 
/// 
/// This Event is sent to AWS whenever the gantry requestes for a power-off sequence.
/// 
/// 
/// The AWS may accept the request: in this case the AWS shall send the command \ref EXEC_PowerOff
/// to the Gantry.
/// 
/// 
/// ### Command Data Format
/// 
/// <ID % EVENT_Poweroff  %>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-|
/// 
///    

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
    LogClass::logInFile(answer);
}


/// \addtogroup AWSProtocolDescription
///
/// \subsection EVENT_Executed 
/// 
///  This is the EVENT the Gantry shall generate when a delayed command completes.
/// 
/// 
/// ### Command Data Format
/// 
/// <ID % EVENT_Executed   %>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-|
/// 
///    

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


