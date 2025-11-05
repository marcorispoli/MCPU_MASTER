
#include "gantry_global_status.h"
#include "awsProtocol.h"
#include "ArmMotor.h"
#include "VerticalMotor.h"
#include "TiltMotor.h"
#include "BodyMotor.h"
#include "SlideMotor.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB304.h"
#include "PCB325.h"
#include "PCB326.h"
#include "ExposureModule.h"
#include "Generator.h"
#include "Notify.h"
#include "Log.h"
#include "BiopsyStudy.h"
#include "BiopsyHomeProcedure.h"


using namespace System::Diagnostics;


/// \addtogroup AWSProtocolDescription
///  
/// <div style="page-break-after: always;"></div>
/// 
/// \section CMDDESC Command Description Section
///


/// \addtogroup AWSProtocolDescription
///
/// \subsection EXEC_OpenStudy 
/// 
/// This command shall be sent by AWS to request to Open the Study.
/// 
/// The Open Study is necessary to enter the Operating Status.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EXEC_OpenStudy patient_name>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |patient_name|String|Study's patient name| 
/// 
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_SYSTEM_ERRORS| "SYSTEM_ERRORS" | system error condition are presents|
/// |AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"| wrong number of parameters (it should be 1)|
/// |AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_IDLE_MODE"| the Gantry is not in IDLE status|
/// 

/// <summary>
/// This command shall be sent by AWS to request to Open the Study.
/// </summary>
/// <param name=""></param>
void  awsProtocol::EXEC_OpenStudy(void) {

    if (!Gantry::isIDLE()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_IDLE_MODE"; ackNok(); return; }

    // Not in error condition !!!
    if (Notify::isError()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_SYSTEM_ERRORS; pDecodedFrame->errstr = "SYSTEM_ERRORS"; ackNok(); return; }

    if (pDecodedFrame->Count() != 1) { pDecodedFrame->errcode = (int) return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    String^ patient_name = pDecodedFrame->parameters[0];

    // Open the study and assignes the patient name !!!
    if (!Gantry::setOpenStandardStudy(patient_name)) { pDecodedFrame->errcode = (int) return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_IDLE_MODE"; ackNok(); return; }

 
    // With the OPEN Study, the collimator is automatically set to AUTO mode.    
    PCB303::setFormatCollimationMode(PCB303::collimationModeEnum::AUTO,0);

    TiltMotor::setIdlePosition();

    ackOk();
}

/// \addtogroup AWSProtocolDescription
///
/// \subsection EXEC_BiopsyStudy 
/// 
/// The AWS send this command whenever a Biopsy study should be open.
/// 
/// NOTE: The biopsy device may not be present when the command is sent.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EXEC_OpenStudy patient_name>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |patient_name|String|Study's patient name| 
/// 
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_SYSTEM_ERRORS| "SYSTEM_ERRORS" | system error condition are presents|
/// |AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"| wrong number of parameters (it should be 1)|
/// |AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_IDLE_MODE"| the Gantry is not in IDLE status|
/// 

/// <summary>
/// This command shall be sent by AWS to request to Open a Biopsy Study.
/// 
/// </summary>
/// <param name=""></param>
void  awsProtocol::EXEC_BiopsyStudy(void) {
    
    if (!Gantry::isIDLE()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_IDLE_MODE"; ackNok(); return; }


    // Not in error condition !!!
    if (Notify::isError()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_SYSTEM_ERRORS; pDecodedFrame->errstr = "SYSTEM_ERRORS"; ackNok(); return; }

    if (pDecodedFrame->Count() != 1) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    String^ patient_name = pDecodedFrame->parameters[0];

    // Open the study and assignes the patient name !!!
    if (!Gantry::setOpenBiopsyStudy(patient_name)) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_IDLE_MODE"; ackNok(); return; }

    // With the OPEN Study, the collimator is automatically set to AUTO mode.
    PCB303::setFormatCollimationMode(PCB303::collimationModeEnum::AUTO, 0);


    ackOk();
}


/// \addtogroup AWSProtocolDescription
///
/// \subsection EXEC_TestStudy 
/// 
/// The AWS sends this command whenever a Test study should be open.
/// 
/// NOTE: The test study is a special exposure session where the Patient 
/// is not present. IN this session several controls are disabled, allowing 
/// te execution of calibration and test procedures.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EXEC_TestStudy test_name>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |test_name|String|A string descriptor of the current test session| 
/// 
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_SYSTEM_ERRORS| "SYSTEM_ERRORS" | system error condition are presents|
/// |AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"| wrong number of parameters (it should be 1)|
/// |AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_IDLE_MODE"| the Gantry is not in IDLE status|
/// 

/// <summary>
/// The AWS sends this command whenever a Test study should be open.
/// 
/// </summary>
/// <param name=""></param>
void  awsProtocol::EXEC_TestStudy(void) {

    // Only in Idle can be accepted this command
    if (!Gantry::isIDLE()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_IDLE_MODE"; ackNok(); return; }


    // Not in error condition !!!
    if (Notify::isError()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_SYSTEM_ERRORS; pDecodedFrame->errstr = "SYSTEM_ERRORS"; ackNok(); return; }

    if (pDecodedFrame->Count() != 1) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    String^ test_name = pDecodedFrame->parameters[0];

    // Open the study and assignes the patient name !!!
    if (!Gantry::setOpenTestStudy(test_name)) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_IDLE_MODE"; ackNok(); return; }

    ackOk();
}



/// \addtogroup AWSProtocolDescription
///
/// \subsection EXEC_BiopsyHoming 
/// 
/// The AWS sends this command whenever shall select a given home position.
///  
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EXEC_BiopsyHoming home_position>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |home_position|String|"C": Center<br>"L": Left<br>"R": Right| 
/// 
/// 
/// ### Command Returned Code 
/// 
/// + OK: if the biopsy is already in the given home position.
/// + Executing: the command is initiated;
/// + Nok (see table below): the command has been aborted due to errors.
///  
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"| wrong number of parameters (it should be 1)|
/// |AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_BIOPSY_MODE"| the Gantry is not in Biopsy Study|
/// |AWS_RET_INVALID_PARAMETER_VALUE|"NOT_A_VALID_HOME_PARAMETER"| The parameter value is not correct|
/// |AWS_RET_DEVICE_ERROR|"ERROR SIGNALED BY THE BIOPSY DEVICE. CODE=xxx"| The device command has returned an error code|
/// 
/// 

/// <summary>
/// This command activates the biopsy homing procedure
/// 
/// </summary>
/// <param name=""></param>
void  awsProtocol::EXEC_BiopsyHoming(void) {
    if (!Gantry::isBIOPSY()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_BIOPSY_MODE"; ackNok(); return; }

    if (pDecodedFrame->Count() != 1) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    String^ home_command = pDecodedFrame->parameters[0];

    int res;

    if (home_command == "C") {
        LogClass::logInFile("AWS: COMMAND HOME CENTER REQUEST");
        res = BiopsyHomeProcedure::activateCenter(pDecodedFrame->ID);
        
    }
    else if (home_command == "L") {
        LogClass::logInFile("AWS: COMMAND HOME LEFT REQUEST");
        res = BiopsyHomeProcedure::activateLeft(pDecodedFrame->ID);
    }
    else if (home_command == "R") {
        LogClass::logInFile("AWS: COMMAND HOME RIGHT REQUEST");
        res = BiopsyHomeProcedure::activateRight(pDecodedFrame->ID);
    }
    else {
        pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "NOT_A_VALID_HOME_PARAMETER"; ackNok(); return;
    }

    if (res < 0) {
        LogClass::logInFile("AWS: HOME COMMAND ERROR CODE = " + res.ToString());
        pDecodedFrame->errcode = (int) return_errors::AWS_RET_DEVICE_ERROR;
        pDecodedFrame->errstr = "ERROR SIGNALED BY THE BIOPSY DEVICE. CODE=" + res.ToString();
        ackNok();
    }
    else if (res == 0) ackOk();
    else ackExecuting();
    return;
}

/// \addtogroup AWSProtocolDescription
///
/// \subsection EXEC_BiopsyPointing 
/// 
/// The AWS sends this command whenever shall move the pointer to a target position.
///  
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EXEC_BiopsyPointing targetX targetY targetZ >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |targetX|Integer|target X position, in 0.1mm units | 
/// |targetY|Integer|target Y position, in 0.1mm units |
/// |targetZ|Integer|target Z position, in 0.1mm units | 
/// 
/// ### Command description
/// 
/// The AWS software shall use this command to move the needle to a target 
/// position. 
/// There following conditions shall be necessary to enable the command execution:
/// + A valid Home position has been selected (center/Left or Right);
/// + The current detected home position is present and matches with the selected ones;
/// + The target position can be reached without impacting the body;
/// + The Needle is not mounted on the needle holder (in this case the motor driver will be disabled!);
/// 
/// ### Command Returned Code 
/// 
/// + OK: if the biopsy is already in the given target position.
/// + Executing: the command is initiated;
/// + Nok (see table below): the command has been aborted due to errors.
///  
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"| wrong number of parameters (it should be 3)|
/// |AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_BIOPSY_MODE"| the Gantry is not in Biopsy Study|
/// |AWS_RET_WRONG_OPERATING_STATUS|"MISSING_A_VALID_HOME_POSITION"| There is not a valid home position selected, or the Biopsy is not in the home position|
/// 
/// 

/// <summary>
/// This command activates the biopsy homing procedure
/// 
/// </summary>
/// <param name=""></param>
void  awsProtocol::EXEC_BiopsyPointing(void) {
    if (!Gantry::isBIOPSY()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_BIOPSY_MODE"; ackNok(); return; }
    if (pDecodedFrame->Count() != 3) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    
    // Test if a Valid Home position is selected and detected:
    if(!BiopsyHomeProcedure::isValidHome()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "MISSING_A_VALID_HOME_POSITION"; ackNok(); return; }

    String^ home_command = pDecodedFrame->parameters[0];


    ackNa();
}

void  awsProtocol::EXEC_BiopsyParking(void) {
    ackNa();
}

void  awsProtocol::SET_BiopsyImage2D(void) {
    ackNa();
}



/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
///  
/// \subsection EXEC_CloseStudy
/// 
/// This command shall be sent by AWS to Close a current study and set the Gantry in IDLE operating status.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EXEC_CloseStudy >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-| 
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_WRONG_OPERATING_STATUS| "NOT_IN_OPEN_MODE" | The gantry is not in Open Status mode|
/// 

/// <summary>
/// This command shall be sent by AWS to Close a current study and set the Gantry in IDLE operating status.
/// </summary>
/// <param name=""></param>
void  awsProtocol::EXEC_CloseStudy(void) {
    if (!Gantry::setCloseStudy()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    // TiltMotor::setIdlePosition();
    ackOk();

}


/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection SET_ProjectionList
/// 
/// This command shall be sent by AWS to set the gantry selectable projections.
/// 
/// The AWS provides a set of projection that the operator can select\n
/// from the local displays.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % SET_ProjectionList proj1, proj2, .. , proj-n>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |proj1..proj-n|Projection name|List of the selectable projections| 
/// 
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |return_errors::AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_OPEN_MODE"| the Gantry is not in Open Study operating status|
/// |return_errors::AWS_RET_INVALID_PARAMETER_VALUE|"INVALID_PROJECTION_IN_THE_LIST"|a projection name in the list is not valid|
/// 

/// <summary>
/// This command shall be sent by AWS to set the gantry selectable projections.
/// </summary>
/// <param name=""></param>
void awsProtocol::SET_ProjectionList(void) {
    if (!Gantry::isSTANDARD()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    if (!ArmMotor::getProjectionsList()->setList(pDecodedFrame->parameters)) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_PROJECTION_IN_THE_LIST"; ackNok(); return; }

    ackOk();
    return;

}



/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection EXEC_ArmPosition
/// 
/// This command shall be sent by AWS to activate the C-ARM to a given projection or position.
/// 
/// There are two forms of this command:
/// + The standard form is dedicated to the Patient operating mode: <ID % EXEC_ArmPosition projection target_angle Min Max>
/// + The generic form is dedicated for test and calibration mode: <ID % EXEC_ArmPosition target_angle>
/// 
/// In Patient Operating mode, with the first command form, the following rules applies:
/// + The \ref SET_ProjectionList shall be received first, in order to have a valid list of acceptable projections;
/// + The Gantry automatically modifies the Vertical position of the C-ARM,\n
/// in order to keep unchanged the position of the copression plane (Virtual Isometric feature);
/// 
/// In the Test mode with the second form:
/// + The Arm will move to the target angle;
/// + The Vertical position of the Arm remains unchanged.
/// 
/// For both command forms, the command may teminate in three different ways:
/// + Immediate OK: the ARM is already in the requested target;
/// + Executing: the ARM is running to the requested target;
/// + NOK: an error prevent to activate the ARM.
/// 
/// ### Command Data Format
/// 
/// Frame format syntax 1: <ID % EXEC_ArmPosition projection target_angle Min Max>
/// Frame format syntax 2: <ID % EXEC_ArmPosition target_angle >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |projection|String|Name of the projection the AWS is selecting| 
/// |target_angle|Integer|Degree of the target angle| 
/// |Min|Integer|Degree of the minimum acceptable angle|
/// |Max|Integer|Degree of the maximum acceptable angle|
/// 
/// - Projection name (form 1 only): it shall be present in the list of the selectable projections (see the \ref SET_ProjectionList command);
/// - target_angle (forms 1 and 2): is the target angle in degree.
/// - The Min and the Max (form 1 only) value define the acceptable range in the case the operator should manually change the projection angle:
///     - if the actual ARM angle should be < Min or > Max the gantry will reject the Exposure activation;
/// 
/// NOTE: 
/// + the Min shall be < Ange;
/// + the Max shall be > Ange;
/// 
/// ### Success  Returned Code
/// 
/// <ID % OK 0 >: the ARM is already in target position
/// <ID % EXECUTING >: the ARM is running o the target position
/// 
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_OPEN_MODE"| the Gantry is not in Open Study operating status|
/// |AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"|the number of parameters is not correct (it should be 4)|
/// |AWS_RET_DEVICE_BUSY|"MOTORS_BUSY"|One of the motors is running|
/// |AWS_RET_DEVICE_BUSY|"ARM_NOT_READY"|The ARM is not ready to execute an activation|
/// |AWS_RET_DATA_NOT_ALLOWED | "WRONG_PROJECTION" | The projection name is not valid or it isn't in the list of selectable projections |
/// |AWS_RET_INVALID_PARAMETER_VALUE |  "WRONG_TARGET_DATA" | One of the angle parameter is not correct or out of range |
///
/// The Command always returns a <ID % EXECUTING%> frame in case of success, because 
/// the ARM requires some time to be positioned, even if the ARM should be already in the target position.
///  

/// <summary>
/// This command shall be sent by AWS to activate the C-ARM to a given projection.
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::EXEC_ArmPosition(void) {

    // General conditions
    if (Gantry::isMotorsActive()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_BUSY; pDecodedFrame->errstr = "MOTORS_BUSY"; ackNok(); return; }

    // Form 1 received
    if (pDecodedFrame->parameters->Count == 4) {

        // Only in Patient Operating Mode
        if (!Gantry::isSTANDARD()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_PATIENT_MODE"; ackNok(); return; }        

        int errcode = ArmMotor::setTarget(
            Convert::ToInt16(pDecodedFrame->parameters[1]),
            Convert::ToInt16(pDecodedFrame->parameters[2]),
            Convert::ToInt16(pDecodedFrame->parameters[3]),
            pDecodedFrame->parameters[0], // Projection code
            pDecodedFrame->ID);

        // Command Error condition
        if (errcode < 0) {
            if(errcode == -1) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DATA_NOT_ALLOWED; pDecodedFrame->errstr = "WRONG_PROJECTION"; ackNok(); return; }
            if(errcode == -2) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_ERROR; pDecodedFrame->errstr = "TILT_NOT_IN_SCOUT"; ackNok(); return; }
            if(errcode == -3) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_BUSY; pDecodedFrame->errstr = "ARM_MOTOR_BUSY"; ackNok(); return; }
            pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_ERROR; pDecodedFrame->errstr = "DEVICE_ERROR"; ackNok(); return;
        }

        // Immediate: target already in position
        if (errcode == 0) {
            ackOk();
            return;
        }

        // Command is in execution
        ackExecuting();
        return;

    }else if (pDecodedFrame->parameters->Count == 1) { // Form 2 received:  Only in Test Operating Mode    
        if (!Gantry::isTEST()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_TEST_MODE"; ackNok(); return; }

        int angle = Convert::ToInt16(pDecodedFrame->parameters[1]);

        // Already in position
        if (ArmMotor::isTarget(angle)) {
            ackOk();
            return;
        }

        // The Tilt is not in the correct position to move the Arm
        if ((!TiltMotor::isScoutPosition())) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_ERROR; pDecodedFrame->errstr = "TILT_NOT_IN_SCOUT"; ackNok(); return; }
        if ((!ArmMotor::device->isReady())) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_BUSY; pDecodedFrame->errstr = "ARM_MOTOR_BUSY"; ackNok(); return; }

        if (ArmMotor::serviceAutoPosition(angle)) {
            // Command is in execution
            ackExecuting();
            return;
        }
        
        // Command error
        pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_ERROR; pDecodedFrame->errstr = "DEVICE_ERROR"; ackNok(); return;
        
    }
    
    // Wrong number of parameters
    pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; 
    pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; 
    ackNok(); 
    return; 

}

/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection EXEC_AbortProjection
/// 
/// This command shall be sent by AWS to invalidate any selected projection.
/// </summary>
/// 
/// 
/// When the Application receive this COMMAND:
/// + The current projection is invalidated;
/// + If the current exposure mode should enable the projection check, then the exposure 
/// cannot be further initiated, until a new valid projection is selected;
/// + The ARM remains in the current position;
/// + The GUI removes the projection icon from the panel;
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EXEC_AbortProjection >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-| 
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_WRONG_OPERATING_STATUS| "NOT_IN_OPEN_MODE" | The gantry is not in Open Status mode|
/// 

/// <summary>
/// This command shall be sent by AWS to invalidate any selected projection.
/// </summary>
/// <param name=""></param>
void awsProtocol::EXEC_AbortProjection(void) {
    if (!Gantry::isSTANDARD()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    ArmMotor::abortTarget();
    ackOk();

}

/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection EXEC_TrxPosition
/// 
/// This command activates the Tube-Arm rotation.
/// 
/// The AWS may use this command in two scenarios:
/// + During a Tomo sequence;
/// + During a Biopsy study;
/// 
/// In a Tomo sequence the AWS can command an early Tube position 
/// to the starting sequence angle (Home position): in this way 
/// the sequence activation time is reduced. 
///     Note: the Gantry however auto positions the Tube in home position 
///     before to start the exposure, if the Tube should not already be in the right position.
/// 
/// In a Biopsy sequence, the AWS set the Tube in the proper stereotactic 
/// positions, following the biopsy worflow.
/// 
/// The AWS cannot set an arbitrary target angle with this command: instead, a predefined subset of 
/// targets are already predefined.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EXEC_TrxPosition trx_target>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |trx_target|String|the TRX target position. See table below| 
/// 
/// |TRX TARGET VALUE|DESCRIPTION|
/// |:--|:--|
/// |"SCOUT"| Scout Position |
/// |"BP_R"| Biopsy Right Position |
/// |"BP_L"| Biopsy left Position |
/// |"TOMO_H"| Tomo Home Position |
/// |"TOMO_E"| Tomo Final Position |
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_OPEN_MODE"| the Gantry is not in Open Study operating status|
/// |AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"|the number of parameters is not correct (it should be 1)|
/// |AWS_RET_DEVICE_BUSY|"MOTORS_BUSY"|One of the motors is running|
/// |AWS_RET_DEVICE_BUSY|"TRX_NOT_READY"|The TRX is not ready to execute an activation|
/// |AWS_RET_INVALID_PARAMETER_VALUE |  "INVALID_TARGET" | One of the angle parameter is not correct or out of range |
/// |AWS_RET_DEVICE_ERROR |  "DEVICE_ERROR" | The Tilt Device cannot activate the command for an internal reason |
/// 
/// Gantry always returns <ID % EXECUTING%> frame: the TRX start executing;

/// <summary>
/// This command activates the Tube-Arm rotation.
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::EXEC_TrxPosition(void) {
    if ((!Gantry::isSTANDARD()) && (!Gantry::isBIOPSY())) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    
    if (pDecodedFrame->parameters->Count != 1) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (Gantry::isMotorsActive()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_BUSY; pDecodedFrame->errstr = "MOTORS_BUSY"; ackNok(); return; }
    if (!TiltMotor::device->isReady()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_BUSY; pDecodedFrame->errstr = "TRX_NOT_READY"; ackNok(); return; }
    TiltMotor::target_options target = TiltMotor::getTargetCode(pDecodedFrame->parameters[0]);
    if(target == TiltMotor::target_options::UNDEF) { pDecodedFrame->errcode = (int) return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_TARGET"; ackNok(); return; }
    if (!TiltMotor::setTarget(target, pDecodedFrame->ID)) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_ERROR; pDecodedFrame->errstr = "DEVICE_ERROR"; ackNok(); return; }

    ackExecuting();
    
    return;
}


/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection SET_TomoConfig
/// 
/// This command selects the next Tomo sequence geometry.   
/// 
/// Every tomo sequence is characterized by a set of paramneters:
/// + Starting Position;
/// + Ending Position;
/// + Acceleration;
/// + Speed;
/// + Deceleration;
/// + Number of samples;
/// + Number of discarded inital pulses (skip pulses);
/// 
/// All those parameters are stored into a TomoConfig.cnf file 
/// (see the \ref TomoConfig for reference) 
/// where a unique identifier name is assigned to a given tomo sequence.
/// 
/// The AWS with this command can select one of the available sequences
/// using the predefined Identifier.
/// 
/// \note This command can have two possible sintax: the second sintax allow to add extra parameters
/// for test purpose. See below
/// 
/// 
/// ### Command Data Format
/// 
/// Frame format Sintax 1: <ID % SET_TomoConfig tomo_name >
/// Frame format Sintax 2: <ID % SET_TomoConfig tomo_name nsamples_modifier nskip_modifier>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |tomo_name|String|the predefined name assigned to the Tomo sequence to be selected| 
/// |nsamples_modifier|Integer|(optional)Modifier of the sequence number of samples |
/// |nskip_modifier|Integer|(optional)Modifier of the sequence number of skips|
/// 
/// \Note: the comnmand
/// |tomo_name|
/// |:--|
/// |TOMO1F_NARROW| 
/// |TOMO1F_INTERMEDIATE|  
/// |TOMO1F_WIDE|  
/// |TOMO2F_NARROW| 
/// |TOMO2F_INTERMEDIATE|  
/// |TOMO2F_WIDE|  
/// |TOMO3F_NARROW| 
/// |TOMO3F_INTERMEDIATE|  
/// |TOMO3F_WIDE|  
/// |TOMO4F_NARROW| 
/// |TOMO4F_INTERMEDIATE|  
/// |TOMO4F_WIDE|  
/// |TOMO5F_NARROW| 
/// |TOMO5F_INTERMEDIATE|  
/// |TOMO5F_WIDE|  
///
///     NOTE: The previous table reflects the current Tomo configurations. 
///     The TomoConfig.cnf file however can be updated in the future.
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_OPEN_MODE"| the Gantry is not in Open Study operating status|
/// |AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"|the number of parameters is not correct (it should be 1)|
/// |AWS_RET_INVALID_PARAMETER_VALUE |  "WRONG_CONFIGURATION_ID" | The Tomo ID is not present in the TomoConfig.cnf configuration file |
/// 


/// <summary>
/// This command selects the next Tomo sequence geometry. 
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::SET_TomoConfig(void) {

    if ( (pDecodedFrame->parameters->Count != 1) && 
        (pDecodedFrame->parameters->Count != 3)) 
    { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    
    if ((!Gantry::isSTANDARD()) && (!Gantry::isBIOPSY()) && (!Gantry::isTEST())) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    // Sintax with only one parameter
    if (pDecodedFrame->parameters->Count == 1) {
        if (!Exposures::getTomoExposure()->set(pDecodedFrame->parameters[0])) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "WRONG_CONFIGURATION_ID"; ackNok(); return; }
        ackOk();
        return;
    }

    // Sintax with modifier parameters
    int samples_modifier = Convert::ToInt16(pDecodedFrame->parameters[1]);
    int skips_modifier = Convert::ToInt16(pDecodedFrame->parameters[2]);
    if(!Exposures::getTomoExposure()->set(pDecodedFrame->parameters[0], samples_modifier, skips_modifier)) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "WRONG_CONFIGURATION_ID"; ackNok(); return; }
    ackOk();

    return;
}

/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection GET_TomoInfo
/// 
/// This command returns the parameters of a given Tomo sequence;
/// 
/// Every tomo sequence is characterized by a set of parameters:
/// + Starting Position;
/// + Ending Position;
/// + Acceleration;
/// + Speed;
/// + Deceleration;
/// + Number of samples;
/// + Number of discarded inital pulses (skip pulses);
/// 
/// All those parameters are stored into a TomoConfig.cnf file 
/// (see the \ref TomoConfig for reference) 
/// where a unique identifier name is assigned to a given tomo sequence.
/// 
/// The AWS with this command can get the parameters of a given sequence.
/// 
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % GET_TomoInfo tomo_name>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |tomo_name|String|the predefined name assigned to the Tomo sequence to be selected| 
/// 
/// |tomo_name|
/// |:--|
/// |TOMO1F_NARROW| 
/// |TOMO1F_INTERMEDIATE|  
/// |TOMO1F_WIDE|  
/// |TOMO2F_NARROW| 
/// |TOMO2F_INTERMEDIATE|  
/// |TOMO2F_WIDE|  
/// |TOMO3F_NARROW| 
/// |TOMO3F_INTERMEDIATE|  
/// |TOMO3F_WIDE|  
/// |TOMO4F_NARROW| 
/// |TOMO4F_INTERMEDIATE|  
/// |TOMO4F_WIDE|  
/// |TOMO5F_NARROW| 
/// |TOMO5F_INTERMEDIATE|  
/// |TOMO5F_WIDE|  
///
///     NOTE: The previous table reflects the current Tomo configurations. 
///     The TomoConfig.cnf file however can be updated in the future.
/// 
/// ### Command Success Returned Code 
/// 
/// <ID % OK  home end skip samples fps >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |home|Integer|Starting position in 0.01 degree|
/// |end|Integer|End position in 0.01 degree|
/// |skip|Byte|number of sync pulses: those pulses shall be discarded|
/// |samples|Byte|number of valid pulses|
/// |fps|Byte|frame per second|
/// 
///     NOTE: the detector will provides the skip + samples integration window,
///     but the first skip images shall be discarded by the AWS. 
///
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"|the number of parameters is not correct (it should be 1)|
/// |AWS_RET_INVALID_PARAMETER_VALUE |  "WRONG_CONFIGURATION_ID" | The Tomo ID is not present in the TomoConfig.cnf configuration file |
/// 


/// <summary>
/// This command selects the next Tomo sequence geometry. 
/// 
/// </summary>
/// <param name=""></param>
void awsProtocol::GET_TomoInfo(void) {
    if (pDecodedFrame->parameters->Count != 1) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    
    // Parameter 0: Tomo configiguration selection;
    System::String^ tomoid = pDecodedFrame->parameters[0];
    if (!Exposures::getTomoExposure()->exist(tomoid)) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "WRONG_CONFIGURATION_ID"; ackNok(); return; }

    // Prepares the parameter list
    List<String^>^ lista = gcnew List<String^>;
    lista->Add(Exposures::getTomoExposure()->getHome(tomoid));
    lista->Add(Exposures::getTomoExposure()->getEnd(tomoid));
    lista->Add(Exposures::getTomoExposure()->getSkip(tomoid));
    lista->Add(Exposures::getTomoExposure()->getSamples(tomoid));
    lista->Add(Exposures::getTomoExposure()->getFps(tomoid));
    
    ackOk(lista);
    return;
}





/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection SET_ExposureMode
/// 
/// This command selects exposure type and characteristics of the next exposure sequence.   
/// 
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % SET_ExposureMode exp_type detector_type compression_mode collimation_mode protection_mode arm_mode >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |exp_type|String|Defines the type of the exposure (see tble below)| 
/// |detector_type|String|Defines the target Detector (see table below)| 
/// |compression_mode|String|Sets the behavior of the compressor (see table below)| 
/// |collimation_mode|String|Sets the collimation format used (see table below)| 
/// |protection_mode|String|Sets the use of the patient protection (see table below)| 
/// |arm_mode|String|Sets the use of the accurate ARM positioning (see table below)| 
///
/// |exp_type|Description|
/// |:--|:--|
/// |MAN_2D|Exposure 2D in Manual Mode|
/// |AEC_2D|Exposure 2D with pre pulse|
/// |MAN_3D|Exposure 3D in Manual Mode|
/// |AEC_3D|Exposure 3D with pre pulse|
/// |MAN_3D_STATIC|Exposure 3D in Manual Mode with no Tube rotation|
/// |MAN_COMBO|Combo in manual mode|
/// |AEC_COMBO|Combo with pre-pulse|
/// |MAN_AE|Exposure CESM in Manual Mode|
/// |AEC_AE|Exposure CESM with pre pulse|
/// 
/// |detector_type (see the DetectorConfiguration.cnf description \ref DetectorConfig) |Description|
/// |:--|:--|
/// |GENERIC|A generic detector with tipical timing|
/// |LMAM2V2|Analogic LMAM2V2 tuned timings|
/// |FDIV2|Analogic LMAM2V2 tuned timings|
/// |DRTECH|DRTECH tuned timings|
/// |VAREX|VAREX tuned timings|
///
/// |compression_mode |Description|
/// |:--|:--|
/// |CMP_KEEP|The compressor shall be used and it will remain in compression after exposure|
/// |CMP_RELEASE|The compressor shall be used and it will relase the compression after exposure|
/// |CMP_DISABLE|The compressor may not be used (no error is set)|
/// 
/// |collimation_mode |Description|
/// |:--|:--|
/// |COLLI_AUTO|The collimation format is automatically selected by the Gantry, based on the current detected compression paddle|
/// |COLLI_CUSTOM|The Custom collimation format is selected|
/// |PADDLE_PROSTHESIS|Manually sets the collimation for that paddle format|
/// |PADDLE_BIOP2D|Manually sets the collimation for that paddle format|
/// |PADDLE_BIOP3D|Manually sets the collimation for that paddle format|
/// |PADDLE_TOMO|Manually sets the collimation for that paddle format|
/// |PADDLE_24x30_CONTACT|Manually sets the collimation for that paddle format|
/// |PADDLE_18x24_C_CONTACT|Manually sets the collimation for that paddle format|
/// |PADDLE_18x24_L_CONTACT|Manually sets the collimation for that paddle format|
/// |PADDLE_18x24_R_CONTACT|Manually sets the collimation for that paddle format|
/// |PADDLE_10x24_CONTACT|Manually sets the collimation for that paddle format|
/// |PADDLE_9x21_MAG|Manually sets the collimation for that paddle format|
/// |PADDLE_9x9_MAG|Manually sets the collimation for that paddle format|
/// |PADDLE_D75_MAG|Manually sets the collimation for that paddle format|
/// 
/// |protection_mode|Description|
/// |:--|:--|
/// |PROTECTION_ENA|The Patient protection shall be used for the next exposure|
/// |PROTECTION_DIS|The Patient protection may not be used for the next exposure|
/// 
/// |arm_mode|Description|
/// |:--|:--|
/// |ARM_ENA|The ARM position shall properly be set |
/// |ARM_DIS|The current ARM angle is not checked by the Gantry|
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_OPEN_MODE"| the Gantry is not in Open Study operating status|
/// |AWS_RET_WRONG_OPERATING_STATUS|"COMMAND_NOT_ALLOWED_IN_THE_CURRENT_STUDY"| The requested exposure is not allowed in the curent Study|
/// |AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"|the number of parameters is not correct (it should be 6)|
/// |AWS_RET_INVALID_PARAMETER_VALUE |  "INVALID_EXPOSURE_TYPE" | The exp_type parameter is wrong|
/// |AWS_RET_INVALID_PARAMETER_VALUE |  "INVALID_COMPRESSION_MODE" | The compression_mode parameter is wrong|
/// |AWS_RET_INVALID_PARAMETER_VALUE |  "INVALID_PADDLE" | The manual collimation paddle is wrong|
/// |AWS_RET_INVALID_PARAMETER_VALUE |  "INVALID_COLLIMATION_FORMAT" | An invalid collimation format is assigned to the selected paddle|
/// |AWS_RET_INVALID_PARAMETER_VALUE |  "INVALID_PATIENT_PROTECTION_MODE" | The protection_mode parameter is wrong|
/// |AWS_RET_INVALID_PARAMETER_VALUE |  "INVALID_ARM_MODE" | The arm_mode parameter is wrong|
/// 
/// 

/// <summary>
/// This command selects exposure type and characteristics of the next exposure sequence.   
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::SET_ExposureMode(void) {
    if (pDecodedFrame->parameters->Count != 6) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if ((!Gantry::isSTANDARD()) && (!Gantry::isBIOPSY()) && (!Gantry::isTEST())) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    String^ exposure_type = pDecodedFrame->parameters[0];
    String^ detector_type = pDecodedFrame->parameters[1];
    String^ compression_mode = pDecodedFrame->parameters[2];
    String^ collimation_mode = pDecodedFrame->parameters[3];
    String^ protection_mode = pDecodedFrame->parameters[4];
    String^ arm_mode = pDecodedFrame->parameters[5];

    // For the Test session, some of the parameter sahll be initialized and some command verified
    if (!Gantry::isTEST()) {
        if (exposure_type == "MAN_3D_STATIC") { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "COMMAND_NOT_ALLOWED_IN_THE_CURRENT_STUDY"; ackNok(); return; }
        
    }
    else {
        protection_mode = "PROTECTION_DIS";
        compression_mode = "CMP_DISABLE";
        arm_mode = "ARM_DIS";
    }

    // Sets the next exposure mode
    if (exposure_type == "MAN_2D") Exposures::setExposureMode(Exposures::exposure_type_options::MAN_2D);
    else if (exposure_type == "AEC_2D") Exposures::setExposureMode(Exposures::exposure_type_options::AEC_2D);
    else if (exposure_type == "MAN_3D") Exposures::setExposureMode(Exposures::exposure_type_options::MAN_3D);
    else if (exposure_type == "MAN_3D_STATIC") Exposures::setExposureMode(Exposures::exposure_type_options::MAN_3D_STATIC);
    else if (exposure_type == "AEC_3D") Exposures::setExposureMode(Exposures::exposure_type_options::AEC_3D);
    else if (exposure_type == "MAN_COMBO") Exposures::setExposureMode(Exposures::exposure_type_options::MAN_COMBO);
    else if (exposure_type == "AEC_COMBO") Exposures::setExposureMode(Exposures::exposure_type_options::AEC_COMBO);
    else if (exposure_type == "MAN_AE") Exposures::setExposureMode(Exposures::exposure_type_options::MAN_AE);
    else if (exposure_type == "AEC_AE") Exposures::setExposureMode(Exposures::exposure_type_options::AEC_AE);
    else { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_EXPOSURE_TYPE"; ackNok(); return; }

    // Sets the detector used for the exposure
    bool detector_found = false;
    for (int i = 0; i < (int)DetectorConfig::detector_model_option::DETECTOR_LIST_SIZE; i++) {
        if (((DetectorConfig::detector_model_option)i).ToString() == detector_type) {
            Exposures::setDetectorType((DetectorConfig::detector_model_option)i);
            detector_found = true;
            break;
        }
    }

    // If the detector should not be in the available list, a generic detector is selected
    if (!detector_found) {
        Exposures::setDetectorType(DetectorConfig::detector_model_option::GENERIC);
        LogClass::logInFile("SET_ExposureMode: Detector not in the available list. Set the generic detector!");
    }

    
    // Sets the Compression Mode used for the exposure
    if (compression_mode == "CMP_KEEP") Exposures::setCompressorMode(Exposures::compression_mode_option::CMP_KEEP);
    else if (compression_mode == "CMP_RELEASE") Exposures::setCompressorMode(Exposures::compression_mode_option::CMP_RELEASE);
    else if (compression_mode == "CMP_DISABLE") Exposures::setCompressorMode(Exposures::compression_mode_option::CMP_DISABLE);
    else { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_COMPRESSION_MODE"; ackNok(); return; }
    
    // Collimation Mode
    if (collimation_mode == "COLLI_AUTO") {
        PCB303::setFormatCollimationMode(PCB303::collimationModeEnum::AUTO, 0);
    }else if (collimation_mode == "COLLI_CUSTOM") {
        PCB303::setFormatCollimationMode(PCB303::collimationModeEnum::CUSTOM, 255);// The last available slot is reserved for custom 
    }
    else {
        // The AWS sets a format related to a given Paddle 
        
        // Gets the code of the paddle to be used as collimation format
        PCB302::paddleCodes paddle = PCB302::getPaddleCode(pDecodedFrame->parameters[3]);
        if (paddle == PCB302::paddleCodes::PADDLE_NOT_DETECTED){
            // The Paddle is not a valid paddle
            pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE;
            pDecodedFrame->errstr = "INVALID_PADDLE"; 
            ackNok(); 
            return;
        }

        // Gets the collimation format associated to the paddle code
        PCB303::setFormatCollimationMode(PCB303::collimationModeEnum::CUSTOM, PCB302::getPaddleCollimationFormat(paddle));

    }

    // Patient protection usage 
    if (protection_mode == "PROTECTION_ENA") Exposures::setProtectionMode(Exposures::patient_protection_option::PROTECTION_ENA);
    else if (protection_mode == "PROTECTION_DIS") Exposures::setProtectionMode(Exposures::patient_protection_option::PROTECTION_DIS);
    else { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_PATIENT_PROTECTION_MODE"; ackNok(); return; }

    // Arm Mode setting  
    if (arm_mode == "ARM_ENA") Exposures::setArmMode(Exposures::arm_mode_option::ARM_ENA);
    else if (arm_mode == "ARM_DIS") Exposures::setArmMode(Exposures::arm_mode_option::ARM_DIS);
    else { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_ARM_MODE"; ackNok(); return; }

    // Grid Selection
    bool inOut_Field = true;
    switch (Exposures::getExposureMode()) {
    case Exposures::exposure_type_options::AEC_2D: inOut_Field = true; break; // In Field
    case Exposures::exposure_type_options::AEC_3D: inOut_Field = false; break; // Out Field
    case Exposures::exposure_type_options::AEC_AE: inOut_Field = true; break; // In Field
    case Exposures::exposure_type_options::AEC_COMBO: inOut_Field = true; break; // In Field
    case Exposures::exposure_type_options::MAN_2D: inOut_Field = true; break; // In Field
    case Exposures::exposure_type_options::MAN_3D: inOut_Field = false; break; // Out Field
    case Exposures::exposure_type_options::MAN_AE: inOut_Field = true; break; // In Field
    case Exposures::exposure_type_options::MAN_COMBO: inOut_Field = true; break; // In Field
    default:inOut_Field = true; break; // In Field
    }

    if (inOut_Field){
        PCB304::setAutoGridInField();
        Exposures::setGrid(Exposures::grid_selection_index::GRID_IN);
    }
    else {
        PCB304::setAutoGridOutField();
        Exposures::setGrid(Exposures::grid_selection_index::GRID_OUT);

    } 

    // Set focus to Auto
    Exposures::setFocus(Exposures::focus_selection_index::FOCUS_AUTO);

    ackOk();
    return;
}

/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection SET_ExposureData
/// 
/// This command assignes the exposure parameters for the next Exposure pulse in the current sequence.
/// 
/// Depending by the Exposure type, the exposure is composed by a number of single pulses:
/// + The MAN_2D: is composed by 1 pulse;
/// + The AEC_2D: is composed by 2 pulses;
/// + The MAN_3D: is composed by 1 pulse (in this case is intended the whole train of pulses);
/// + The AEC_3D: is composed by 2 pulses (pre pulse and the Tomo pulses);
/// + The MAN_COMBO: is composed by 2 pulses (the manual 2D and the Manual Tomo);
/// + The AEC_COMBO: is composed by 3 pulses (the pre, the pulse 2D and the Tomo pulses);
/// + The MAN_AE: is composed by 2 pulses (the Low Energy pulse and the High energy pulse);
/// + The AEC_AE: is composed by 3 pulses (the pre pulse, the Low Energy pulse and the High energy pulse);
/// 
/// During the exposure sequence the AWS shall set the next pulse parameters. 
/// For example, in an AEC 2D sequence:
/// + The AWS sends this command with the index 0 at the beginning, to set the parameters of the pre pulse;
/// + Then, after the pre pulse completion, the AWS will send again this command, but with the index 1, in order to set 
/// the parameters of the Main pulse;
/// 
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % SET_ExposureData pulse_number kV mAs filter>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |pulse_number|Byte|the pulse sequence number, starting from 0|
/// |kV|float|Kv selection|
/// |mAs|float|mAs for the pulse (*)|
/// |filter|String|The filter that shall be selected for this pulse (see table below)|
/// 
///     (*) NOTE: in case of a tomo sequence, the mAs is the ***Total*** amount of mAs in the sequence.
/// 
/// 
/// |filter|
/// |:--|
/// |Ag| 
/// |Al| 
/// |Rh| 
/// |Mo| 
/// |Cu| 
/// || 
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_OPEN_MODE"| the Gantry is not in Open Study operating status|
/// |AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"|the number of parameters is not correct (it should be 4)|
/// |AWS_RET_INVALID_PARAMETER_VALUE |  "INVALID_PARAMETERS" | One of the pulse parameter is not valid. See the Gantry log for detail|
/// 


/// <summary>
/// This command assignes the exposure parameters for the next Exposure pulse in the current sequence.
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::SET_ExposureData(void) {
    if (pDecodedFrame->parameters->Count != 4) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if ((!Gantry::isSTANDARD()) && (!Gantry::isBIOPSY())) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }

    String^ pulse_param = pDecodedFrame->parameters[0];
    String^ kv_param = pDecodedFrame->parameters[1];
    String^ mAs_param = pDecodedFrame->parameters[2];
    String^ filter_param = pDecodedFrame->parameters[3];

    System::Globalization::CultureInfo^ myInfo = gcnew  System::Globalization::CultureInfo("en-US", false);
    unsigned char pulse_seq = Convert::ToByte(pulse_param);
    Double kV = Convert::ToDouble(kv_param, myInfo);
    Double mAs = Convert::ToDouble(mAs_param, myInfo);
    
    if (!Exposures::setExposurePulse(pulse_seq, gcnew Exposures::exposure_pulse(kV, mAs, PCB303::getFilterFromTag(filter_param)))) {
        pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_PARAMETERS"; ackNok(); return;
    }
   
    ackOk();
    return;
}

void   awsProtocol::SET_TestMode(void) {
    if (pDecodedFrame->parameters->Count != 3) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS(3)"; ackNok(); return; }
   
    // Behavior of the Focus
    if (pDecodedFrame->parameters[0] == "AUTO") {
        
        Exposures::setFocus(Exposures::focus_selection_index::FOCUS_AUTO);
        
    }else if(pDecodedFrame->parameters[0] == "LARGE") {
        Exposures::setFocus(Exposures::focus_selection_index::FOCUS_LARGE);

    }else if (pDecodedFrame->parameters[0] == "SMALL") {
        Exposures::setFocus(Exposures::focus_selection_index::FOCUS_SMALL);
    }
    else {
        pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_FOCUS_PARAMETERS"; ackNok(); return;
    }

    // Behavior of the Grid
    if (pDecodedFrame->parameters[1] == "AUTO") {
        // The Grid is set based on the exposure modes
    }
    else if (pDecodedFrame->parameters[1] == "GRID_IN") {
        Exposures::setGrid(Exposures::grid_selection_index::GRID_IN);
        PCB304::setAutoGridInField();
    }
    else if (pDecodedFrame->parameters[1] == "GRID_OUT") {
        Exposures::setGrid(Exposures::grid_selection_index::GRID_OUT);
        PCB304::setAutoGridOutField();
    }
    else {
        pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_GRID_PARAMETERS"; ackNok(); return;
    }

    // Behavior of the TOMO
    if (pDecodedFrame->parameters[2] == "AUTO") {
        Exposures::setTomoMode(Exposures::tomo_mode_selection_index::TOMO_AUTO);
    }else if (pDecodedFrame->parameters[2] == "CALIB") {
        Exposures::setTomoMode(Exposures::tomo_mode_selection_index::TOMO_CALIB);
    }
    else {
        pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_TOMO_PARAMETERS"; ackNok(); return;
    }

    ackOk();
    return;
}

/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection SET_EnableXrayPush
/// 
/// This command enables/disables the X-RAY push button to init an exposure sequence.
/// 
/// 
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % SET_EnableXrayPush enable_status>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |enable_status|String|"ON": Enabled;<br>"OFF": Disabled|
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_OPEN_MODE"| the Gantry is not in Open Study operating status|
/// |AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"|the number of parameters is not correct (it should be 1)|
/// 
/// 

/// <summary>
///  This command enables/disables the X-RAY push button to init an exposure sequence.
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::SET_EnableXrayPush(void) {
    if (pDecodedFrame->parameters->Count != 1) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if ((!Gantry::isSTANDARD()) && (!Gantry::isBIOPSY())) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_OPEN_MODE"; ackNok(); return; }
    
    if (pDecodedFrame->parameters[0] == "ON") Exposures::enableXrayPushButtonEvent(true);
    else  Exposures::enableXrayPushButtonEvent(false);

    ackOk();

    return;
}

/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection GET_ReadyForExposure
/// 
/// This COMMAND returns the Gantry current Ready For Exposure status.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % GET_ReadyForExposure >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-|
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_SYSTEM_ERRORS|"GANTRY_NOT_READY"| Gantry is not Ready because there are active system Errors|
/// |AWS_RET_SYSTEM_WARNINGS|"GANTRY_NOT_READY"| Gantry is not Ready because there are active system Warnings|
/// 
/// 
/// 

/// <summary>
/// This COMMAND returns the Gantry current Ready For Exposure status.
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_ReadyForExposure(void) {    
    if(Notify::isError()) pDecodedFrame->errcode = (int) return_errors::AWS_RET_SYSTEM_ERRORS;
    else if (Notify::isWarning()) pDecodedFrame->errcode = (int)return_errors::AWS_RET_SYSTEM_WARNINGS;
    else    pDecodedFrame->errcode = 0;

    if (pDecodedFrame->errcode) { pDecodedFrame->errstr = "GANTRY_NOT_READY"; ackNok(); return; }

    // Ready for exposure
    ackOk();
    return;
}

/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection EXEC_StartXraySequence
/// 
/// This command requests to starts an exposure.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % EXEC_StartXraySequence >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-|
/// 
/// ### Command Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_SYSTEM_ERRORS|"GANTRY_NOT_READY"| Gantry is not Ready because there are active system Errors|
/// |AWS_RET_SYSTEM_WARNINGS|"GANTRY_NOT_READY"| Gantry is not Ready because there are active system Warnings|
/// |AWS_RET_DEVICE_ERROR|"GENERATOR_ERROR"| The Generator device rejected the start exposure.|
/// 
/// 

/// <summary>
/// This command requests for the exposure start 
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::EXEC_StartXraySequence(void) {
    if (Notify::isError()) pDecodedFrame->errcode = (int)return_errors::AWS_RET_SYSTEM_ERRORS;
    else if (Notify::isWarning()) pDecodedFrame->errcode = (int)return_errors::AWS_RET_SYSTEM_WARNINGS;
    else pDecodedFrame->errcode = 0;

    if (pDecodedFrame->errcode) {
        pDecodedFrame->errstr = "GANTRY_NOT_READY";
        ackNok();
        return;
    }

    
    // Tries to start the sequence
    if (Exposures::startExposure()) ackOk();
    else {
        pDecodedFrame->errcode = (int)return_errors::AWS_RET_DEVICE_ERROR;
        pDecodedFrame->errstr = "GENERATOR_ERROR";
        ackNok();
    }

    return;
}

/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection GET_Compressor
/// 
/// This COMMAND returns the Compressor Data.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % GET_Compressor >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-|
/// 
/// ### Command Success Returned Code 
/// 
/// <ID % OK  Thickness Force>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |Thickness|Integer|Current breast thickness in mm|
/// |Force|Integer|Current compression force in N|
/// 
/// ### Command Error Returned Code 
/// 
/// No Error for this command.


/// <summary>
/// This command requests the Compressor data:
/// 
/// + Compressor Thickness;
/// + Compressore Force;
///  
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_Compressor(void) {
    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;
    lista->Add(PCB302::getThickness().ToString());
    lista->Add(PCB302::getForce().ToString());

    ackOk(lista);
    return;
}

/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection GET_Components
/// 
/// This COMMAND returns the detected component on the Gantry.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % GET_Components >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-|
/// 
/// ### Command Success Returned Code 
/// 
/// <ID % OK  component_type mag_factor  compressor_paddle protection_type collimation_tool >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |component_type|String|"BIOPY"/"MAGNIFIER"/"POTTER"|
/// |mag_factor|Float|Current magnifier factor value|
/// |compressor_paddle|String|Detected compressor paddle code|
/// |protection_type|String|"BIOPY"/"MAGNIFIER"/"POTTER"|
/// |collimation_tool|String|"LEAD_SCREEN"/"SPECIMEN"/"UNDETECTED_COLLIMATOR"|
/// 
/// 
/// ### Command Error Returned Code 
/// 
/// No Error for this command.


/// <summary>
/// This command requests the component identified by the system.
/// 
/// </summary>
/// 
/// + Potter-Type, Mag Factor, ComprPaddle, ProtectionType, CollimationTool
/// <param name=""></param>
void   awsProtocol::GET_Components(void) {
    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;

    // Potter Type parameter
    if (Biopsy::isBiopsy()) {
        lista->Add("BIOPSY");
    }else  if (PCB302::getMagnifierFactor() != 10) {
        lista->Add("MAGNIFIER");
    }else  {
        lista->Add("POTTER");
    }

    // Magnification factor
    lista->Add(PCB302::getMagnifierFactor().ToString());

    // Compressor paddle
    lista->Add(PCB302::getDetectedPaddleCode().ToString());

    // Protection Type
    if (PCB302::getPatientProtection() == PCB302::PatientProtection::POSITIONED) {
        lista->Add("PROTECTION_POSITIONED");
    }else if (PCB302::getPatientProtection() == PCB302::PatientProtection::SHIFTED) {
        lista->Add("PROTECTION_SHIFTED");
    }else lista->Add("UNDETECTED_PROTECTION");

    // Collimation Tool
    lista->Add("UNDETECTED_COLLIMATOR");


    ackOk(lista);
    return;
}


/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection GET_Trx
/// 
/// This COMMAND returns the current Tube Arm position.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % GET_Trx >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-|
/// 
/// ### Command Success Returned Code 
/// 
/// <ID % OK  target_name angle >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |target_name|String|The name of the current target position(See table below)|
/// |angle|Float|The current angle in .01 degree|
/// 
/// |target_name|Description|
/// |:--|:--|
/// |BP_R|Biopsy positive target (usually +15 degree)|
/// |BP_L|Biopsy negative target (usually -15 degree)|
/// |TOMO_H|Tomo Home position|
/// |TOMO_E|Tomo End position|
/// |UNDEF|Unclassified target position|
///
/// ### Command Error Returned Code 
/// 
/// No Error for this command.


/// <summary>
/// This command provides the current TRX position:
/// + The Symbolic position;
/// + The actual angle position;
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_Trx(void) {
    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;

    lista->Add(TiltMotor::getTargetPosition().ToString());
    lista->Add(TiltMotor::device->getCurrentPosition().ToString());


    ackOk(lista);
    return;
}

/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection GET_Arm
/// 
/// This COMMAND returns the current Tube Arm position.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % GET_Arm >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-|
/// 
/// ### Command Success Returned Code 
/// 
/// <ID % OK  projection_name angle >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |target_name|String|The name of the current projection|
/// |angle|Float|The current angle in .01 degree|
/// 
/// ### Command Error Returned Code 
/// 
/// No Error for this command.

/// <summary>
/// This command provides the current ARM position
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_Arm(void) {
    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;
    lista->Add(ArmMotor::getSelectedProjection());
    lista->Add(ArmMotor::device->getCurrentPosition().ToString());
    
    ackOk(lista);
    return;
}


/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection GET_TubeTemperature
/// 
/// This COMMAND returns the current Tube Arm position.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % GET_TubeTemperature >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-|
/// 
/// ### Command Success Returned Code 
/// 
/// <ID % OK anode bulb stator >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |anode|Byte|% of the cumulated HU [0:100]|
/// |bulb|Byte|% of the cumulated Bulb Temperature [0:100]|
/// |stator|Byte|% of the cumulated Stator Temperature [0:100]|
/// 
/// ### Command Error Returned Code 
/// 
/// No Error for this command.


/// <summary>
/// This command returns the Tube cumulated energy 
/// for the Anode and the internal Filament and Stator device.
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_TubeTemperature(void) {
    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;

    lista->Add("0"); // To Be Done ..
    lista->Add(PCB303::getBulbPerc().ToString());
    lista->Add(PCB303::getStatorPerc().ToString());

    ackOk(lista);
    return;
}

/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection SET_Language
/// 
/// This command sets the language of the Gantry messages and labels.
/// 
///     NOTE: This command requires to restart the Gantry Application to apply the language changes.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % SET_Language language>
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |language|String|This is the language identifier code (see table below)|
/// 
/// |language||Description|
/// |:--|:--|
/// |ITA|Italian Language|
/// |ENG|English Language|
/// |FRA|Franch Language|
/// |ESP|Spanish Language|
/// |POR|Portuguese Language|
/// |RUS|Russian Language|
/// 
/// 
/// 
/// ### Error Returned Code 
/// 
/// |ERROR CODE|ERROR STRING|DESCRIPTION|
/// |:--|:--|:--|
/// |AWS_RET_WRONG_OPERATING_STATUS|"NOT_IN_CLOSE_MODE"| The command can be executed only in Close Study status|
/// |AWS_RET_WRONG_PARAMETERS|"WRONG_NUMBER_OF_PARAMETERS"|Wrong number of parameters received (it should be 1)|
/// |AWS_RET_INVALID_PARAMETER_VALUE|"INVALID_LANGUAGE"| Wrong languge identifier|

/// <summary>
/// This command sets the GUI language.
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::SET_Language(void) {
    if (!Gantry::isIDLE()) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_OPERATING_STATUS; pDecodedFrame->errstr = "NOT_IN_CLOSE_MODE"; ackNok(); return; }
    if (pDecodedFrame->parameters->Count != 1) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_WRONG_PARAMETERS; pDecodedFrame->errstr = "WRONG_NUMBER_OF_PARAMETERS"; ackNok(); return; }
    if (!Notify::setLanguage(pDecodedFrame->parameters[0])) { pDecodedFrame->errcode = (int)return_errors::AWS_RET_INVALID_PARAMETER_VALUE; pDecodedFrame->errstr = "INVALID_LANGUAGE"; ackNok(); return; }

    ackOk();
    return;
}


/// \addtogroup AWSProtocolDescription
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// \subsection GET_ProtocolRevision
/// 
/// This COMMAND returns the current protocol revision code.
/// 
/// ### Command Data Format
/// 
/// Frame format: <ID % GET_ProtocolRevision >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |-|-|-|
/// 
/// ### Command Success Returned Code 
/// 
/// <ID % OK revision_code >
/// 
/// |PARAMETER|Data Type|Description|
/// |:--|:--|:--|
/// |revision_code|String|current revision code in the format maj.min.sub|
/// 
/// ### Command Error Returned Code 
/// 
/// No Error for this command.


/// <summary>
/// This COMMAND returns the current protocol revision code.
/// 
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::GET_ProtocolRevision(void) {
    // Create the list of the results
    List<String^>^ lista = gcnew List<String^>;

    lista->Add(PROTOCOL_REVISION_CODE); 
    ;
    return;
}


/// <summary>
/// This command requests for the exposure start 
/// 
/// </summary>
/// <param name=""></param>
void   awsProtocol::EXEC_TestCommand(void) {
    if (pDecodedFrame->parameters[0] == "TILT") {
        LogClass::logInFile("TEST ON TILT MOTOR");
        int pos = System::Convert::ToInt16(pDecodedFrame->parameters[1]);
        int speed = System::Convert::ToInt16(pDecodedFrame->parameters[2]);
        int acc = System::Convert::ToInt16(pDecodedFrame->parameters[3]);
        int dec = System::Convert::ToInt16(pDecodedFrame->parameters[4]);
        TiltMotor::device->activateAutomaticPositioning(0, pos, speed, acc, dec, true);

    }
    else if (pDecodedFrame->parameters[0] == "TOMO") {
        LogClass::logInFile("TEST ON TOMO TILT MOTOR");
        int pos = System::Convert::ToInt16(pDecodedFrame->parameters[1]);
        int speed = System::Convert::ToInt16(pDecodedFrame->parameters[2]);
        int acc = System::Convert::ToInt16(pDecodedFrame->parameters[3]);
        int dec = System::Convert::ToInt16(pDecodedFrame->parameters[4]);
        TiltMotor::activateTomoScan(pos, speed, acc, dec);
    }
    else if (pDecodedFrame->parameters[0] == "SLIDE") {
        LogClass::logInFile("TEST ON SLIDE MOTOR");
        int pos = System::Convert::ToInt16(pDecodedFrame->parameters[1]);
        int speed = System::Convert::ToInt16(pDecodedFrame->parameters[2]);
        int acc = System::Convert::ToInt16(pDecodedFrame->parameters[3]);
        int dec = System::Convert::ToInt16(pDecodedFrame->parameters[4]);
        SlideMotor::device->activateAutomaticPositioning(0, pos, speed, acc, dec, true);
    }
    else if (pDecodedFrame->parameters[0] == "ARM") {
        LogClass::logInFile("TEST ON ARM MOTOR");
        int pos = System::Convert::ToInt16(pDecodedFrame->parameters[1]);
        int speed = System::Convert::ToInt16(pDecodedFrame->parameters[2]);
        int acc = System::Convert::ToInt16(pDecodedFrame->parameters[3]);
        int dec = System::Convert::ToInt16(pDecodedFrame->parameters[4]);
        ArmMotor::device->activateAutomaticPositioning(0, pos, speed, acc, dec, true);
    }
    else if (pDecodedFrame->parameters[0] == "BODY") {
        LogClass::logInFile("TEST ON BODY MOTOR");
        int pos = System::Convert::ToInt16(pDecodedFrame->parameters[1]);
        int speed = System::Convert::ToInt16(pDecodedFrame->parameters[2]);
        int acc = System::Convert::ToInt16(pDecodedFrame->parameters[3]);
        int dec = System::Convert::ToInt16(pDecodedFrame->parameters[4]);
        BodyMotor::device->activateAutomaticPositioning(0, pos, speed, acc, dec, true);
    }
    else if (pDecodedFrame->parameters[0] == "VERTICAL") {
        LogClass::logInFile("TEST ON VERTICAL MOTOR");
        int pos = System::Convert::ToInt16(pDecodedFrame->parameters[1]);
        int speed = System::Convert::ToInt16(pDecodedFrame->parameters[2]);
        int acc = System::Convert::ToInt16(pDecodedFrame->parameters[3]);
        int dec = System::Convert::ToInt16(pDecodedFrame->parameters[4]);
        VerticalMotor::device->activateAutomaticPositioning(0, pos, speed, acc, dec, true);
    }
    else if (pDecodedFrame->parameters[0] == "BIOPSY_X") {
        LogClass::logInFile("TEST ON BIOPSY X");
        unsigned short pos = System::Convert::ToUInt16(pDecodedFrame->parameters[1]);
        if(!PCB325::moveX(pos)) LogClass::logInFile("FAILED");
    }
    else if (pDecodedFrame->parameters[0] == "BIOPSY_Y") {
        LogClass::logInFile("TEST ON BIOPSY Y");
        unsigned short pos = System::Convert::ToUInt16(pDecodedFrame->parameters[1]);
        if (!PCB325::moveY((unsigned short)pos)) LogClass::logInFile("FAILED");
    }
    else if (pDecodedFrame->parameters[0] == "BIOPSY_Z") {
        LogClass::logInFile("TEST ON BIOPSY Z");
        unsigned short pos = System::Convert::ToUInt16(pDecodedFrame->parameters[1]);
        if (!PCB325::moveZ((unsigned short)pos)) LogClass::logInFile("FAILED");
    }
    else if (pDecodedFrame->parameters[0] == "BIOPSY_HOME_C") {
        LogClass::logInFile("TEST ON BIOPSY HOME_C");
        int res = BiopsyHomeProcedure::activateCenter(pDecodedFrame->ID);
        if (res < 0) {
            pDecodedFrame->errcode = -res;
            ackNok();
        }
        else if (res == 0) ackOk();
        else ackExecuting();
    }
    else if (pDecodedFrame->parameters[0] == "BIOPSY_HOME_R") {
        LogClass::logInFile("TEST ON BIOPSY HOME_R");
        int res = BiopsyHomeProcedure::activateRight(pDecodedFrame->ID);
        if (res < 0) {
            pDecodedFrame->errcode = -res;
            ackNok();
        }
        else if (res == 0) ackOk();
        else ackExecuting();
    }
    else if (pDecodedFrame->parameters[0] == "BIOPSY_HOME_L") {
        LogClass::logInFile("TEST ON BIOPSY HOME_L");
        int res = BiopsyHomeProcedure::activateLeft(pDecodedFrame->ID);
        if (res < 0) {
            pDecodedFrame->errcode = -res;
            ackNok();
        }
        else if (res == 0) ackOk();
        else ackExecuting();
    }


    return;
}