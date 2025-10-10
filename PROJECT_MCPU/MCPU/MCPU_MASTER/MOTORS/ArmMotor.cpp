#include "ConfigurationFiles.h"
#include "ArmMotor.h"
#include "VerticalMotor.h"
#include "TiltMotor.h"
#include "pd4_od.h"
#include "Notify.h"
#include "../gantry_global_status.h"



#define ROT_PER_CDEGREE ((double) 112/ (double) 36000) //!< Defines the position user units in 0.01 degree
#define COMPRESSION_PLANE_MM ((double) 100) //!< Distance from the compression plane and the C-ARM rotation axe for the Isocentric calculation

#define MAX_ROTATION_ANGLE 18000
#define MIN_ROTATION_ANGLE -18000
#define HOMING_ON_METHOD 19
#define HOMING_OFF_METHOD 20

/// <summary>
/// 
/// </summary>
/// 
/// The constructor initializes the Base class with:
/// + the motor address;
/// + the motor gear ratio; 
/// + the rotation direction;
/// + the actual mechanical Arm position stored into the \ref MotorConfig after the last acivation, in order to initialize the motor internal encoder;
/// + the target acceptable range set to +/- 0.2°;
/// 
/// <param name=""></param>
ArmMotor::ArmMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::ARM_ID, L"MOTOR_ARM", MotorConfig::PARAM_ARM, Notify::messages::ERROR_ARM_MOTOR_HOMING, MIN_ROTATION_ANGLE, MAX_ROTATION_ANGLE, ROT_PER_CDEGREE, 1, true)
{
    // Sets +/- 0.2 ° as the acceptable target range
    setTargetRange(20, 20);
   
}


/// <summary>
/// 
/// 
/// </summary>
/// 
/// The following registers will be uploaded here:
/// 
/// |Register Address|Value|Description|
/// | :--| : --| : --|
/// |0x3210 : 01 | 50000 | Position Loop, Proportional Gain(closed Loop)|
/// |0x3210 : 02 | 10 | Position Loop, Integral Gain(closed Loop) |
/// 
#define ARM_OD_CODE 0x0001
unsigned short ArmMotor::initializeSpecificObjectDictionaryCallback(void) {

    // Motor Drive Parameter Set
    while (!blocking_writeOD(OD_3210_01, 50000)); // Position Loop, Proportional Gain (closed Loop)
    while (!blocking_writeOD(OD_3210_02, 10));	 // Position Loop, Integral Gain (closed Loop)

  
    return ARM_OD_CODE;
}

/// <summary>
/// The module overrides this callback in order to update the module status at the command completion.
/// </summary>
/// 
/// When a command completes, the module shall executes the following steps:
/// + in case the command shoul fail, the target position shall be invalidated;
/// + in case the command should be an Isocentric activation, 
/// the Vertical motor is activated to compensate the changed compression plane position;
/// 
/// \note
/// In case of Isocentric activation:
/// + The Vertical activation will take place only if the Arm rotation command succesfully completes;
/// + The command completion event \ref EVENT_Executed  will be sent to AWS only after the last activation in the Isocentric activation sequence;
/// 
/// <param name="id">the identifier of the AWS command;</param>
/// <param name="current_command">the current executed command code;</param>
/// <param name="current_position">the final Tilt position</param>
/// <param name="term_code">the command_completed code of the activation;</param>
void ArmMotor::completedCallback(int id, MotorCommands current_command, int current_position, MotorCompletedCodes term_code) {
    if (current_command != MotorCommands::MOTOR_AUTO_POSITIONING) return;

    // Invalidate the target if the command should fails
    if (term_code != MotorCompletedCodes::COMMAND_SUCCESS) valid_target = false;


    // If the C-ARM activation is not a Isocentric mode (or is terminated in error) then the command termines here
    // end the command_completed event is generated 
    if ((!iso_activation_mode) || (term_code != MotorCompletedCodes::COMMAND_SUCCESS)) {
        command_completed_event(id, (int) term_code);
        return;
    }

    // Activates the Vertical Motor for the isocentric correction
    // The C-ARM position is expressed in cents of degrees;
    // The Vertical Arm position is espressed in millimeters

    double init_h = -1 * COMPRESSION_PLANE_MM * cos((double)getPreviousPosition() * 3.14159 / (double)18000);
    double end_h = -1 * COMPRESSION_PLANE_MM * cos((double)getCurrentUposition() * 3.14159 / (double)18000);
    int delta_h = (int)init_h - (int)end_h;
    if (!VerticalMotor::activateIsocentricCorrection(id, delta_h)) {
        // The target is not invalidated because the rotation angle is still valid!
        device->command_completed_event(id, (int)MotorCompletedCodes::COMMAND_SUCCESS);
        //command_completed_event(getCommandId(), (int) VerticalMotor::device->getCommandCompletedCode());
    }
    return;
}

/// <summary>
/// This function invalidate a current selected target.
/// 
/// </summary>
/// 
/// The current selected target is flagged as not valid.
/// 
/// The method clears the current selected projection.
/// 
/// <param name=""></param>
void ArmMotor::abortTarget(void) {
    if (!valid_target) return;
    valid_target = false;
    projections->clrProjection();
    
}

/// <summary>
/// This function returns true in case the current arm position is already at the requested target.
/// </summary>
/// <param name="angle">this is the target angle in degree</param>
/// <returns>true: the angle is already in target</returns>
bool ArmMotor::isTarget(int angle) {
    
    // Convert the degree in the motor user units
    int tg = angle * 100;
    if ((device->getCurrentUposition() >= (tg - 10)) && (device->getCurrentUposition() <= (tg + 10)))  return true;
    return false;
}

/// <summary>
/// 
/// 
/// </summary>
/// 
/// The application shall calls this method in order to set a valid c-arm position for 
/// the next exposure.
/// 
/// A valid target position and its related acceptable position range is set with this command:
/// + a valid target condition shall be checked by the application in order to enable for an exposure;
/// + a valid range shall be set by the application in order to let a mamual arm rotation in a determined range valid for the exposure;
/// 
/// Th projection name is used to validate the tarfget respect the list of acceptable projections that the 
/// application should have set before to call this method.
/// 
/// Moreover, this function, if the requested rotation should successfully terminate, 
/// triggers the Vertical motor activation so that the final vertical c-arm position be equal to the position
/// at the beginning of the rotation. (Virtual isocentric rotation)
///  
/// If the identifier parameter @param id should be grater than zero,
/// when the command termines the module will notify the AWS with the command termination EVENT, (EVENT_Executed(id));
/// 
/// <param name="pos">this is the target position in 0.01 degrees</param>
/// <param name="low">lower angle of the acceptable range</param>
/// <param name="high">higher angl of the acceptable range</param>
/// <param name="proj">projection name</param>
/// <param name="id">AWS command identifier (if greater than 0)</param>
/// <returns></returns>
int ArmMotor::setTarget(int pos, int low, int high, System::String^ proj, int id) {
    
    // Checks the validity of the requested projection
    if (!projections->isValidProjection(proj)) {
        LogClass::logInFile("ArmMotor::setTarget() - wrong projection requested: " + proj);
        return -1;
    }

    // If the device is already in target answer ok immediate
    if (isTarget(pos)) {
        // Already in the rght position
        projections->setProjection(proj);

        // Assignes the target data
        allowed_low = low * 100;
        allowed_high = high * 100;
        valid_target = true;
        selected_target = pos;
        return 0; // Immediate
    }
    
    if ((!TiltMotor::isScoutPosition())) {
        LogClass::logInFile("ArmMotor::setTarget() - command: error, tilt not in scout ");
        return -2;

    }
    
    if (!device->isReady()) {
        LogClass::logInFile("ArmMotor::setTarget() - motor busy ");
        return -3;
    }

    // Activate an Isocentric C-ARM rotation
    device->iso_activation_mode = true;

    // Assignes the projection
    if (device->activateAutomaticPositioning(id, pos * 100, true)) {
        projections->setProjection(proj);

        // Assignes the target data
        allowed_low = low * 100;
        allowed_high = high * 100;
        valid_target = true;
        selected_target = pos;
        return 1; // Command is executing
    }    


    // Command activation failed
    valid_target = false;
    return -4;
}

/// <summary>
/// This is the service automatic activation that is not valid for an exposure.
/// </summary>
/// 
/// This method should be called by the Application only for service purpose,
/// since the target position is flagged as not valid.
/// 
/// \note
/// The Isocentric correction is not activated with this command.
/// 
/// <param name="pos">the target position in 0.01 ° units</param>
/// <returns>
/// + true: the command is successfully started;
/// + false: the command is rejected;
/// </returns>
bool ArmMotor::serviceAutoPosition(int pos) {
    
    if ((!TiltMotor::isScoutPosition())) {
        LogClass::logInFile("ArmMotor::serviceAutoPosition() - command: error, tilt not in scout ");
        return false;

    }
    device->iso_activation_mode = false;
    valid_target = false;
    return device->activateAutomaticPositioning(0, pos * 100, true);
}

/// <summary>
/// This is the method to be called by the Application to activate the c-ARM chained Idle command.
/// </summary>
/// 
/// The Idle position is a System procedure that involves
/// the Tilt motor, the Slide motor and finally the Arm motor in an activation daisy chain:
/// + The Tilt is the first activation in the sequence;
/// + The Slide activation starts after the Tilt completion;
/// + Finally the Arm starts the last activation of the chain.
/// 
/// Scope of this activation chain is to implement a System position called "Idle Position"
/// that corresponds to a Gantry setup out of the operating mode (System Idle Mode).
/// 
/// The Application can call one of the command and the activated motor will in turn 
/// activates the next in the chain.
/// 
/// \note 
/// The application not necessarily shall call the first in the chain (the Tilt):
/// in this case the activated motors will be only those that follows in the described order.
/// 
/// Being the last in the chain, this method will be the last activation.
/// 
/// <param name=""></param>
/// <returns></returns>
bool ArmMotor::setIdlePosition(void) {
    if ((!TiltMotor::isScoutPosition())) {
        LogClass::logInFile("ArmMotor::serviceAutoPosition() - command: error, tilt not in scout ");
        return false;

    }
    device->iso_activation_mode = false;
    valid_target = false;
    return device->activateAutomaticPositioning(0,0, true);
}



/// <summary>
/// 
/// 
/// </summary>
/// 
/// The automatic zero setting procedure activates the motor rotation 
/// until a photocell dedicate to the zero setting procedure intercepts 
/// the mechanical zero setting point.
/// 
/// This method should be called by the application for service,
/// in case a automatic zero setting should be executed.
/// 
/// This methods is not a blocking method for the caller thread:
/// + the encoder reset may takes time after this method returns.
/// 
/// \note:
/// + the Application shall call the method CanOpenMotor::isRunning() to checks if the 
/// manual command is terminated;
/// + the application shall call the CanOpenMotor::getCommandCompletedCode() to get the result 
/// of the command as soon as the CanOpenMotor::isRunning() should return false.
/// 
/// <param name=""></param>
/// <returns>
/// + true: the zero setting process is actually started;
/// + false: the zero setting cannot start for some reason (CanOpenMotor::getCommandCompletedCode() for gets the error code) 
/// </returns>
bool ArmMotor::startAutoHoming(void) {
    

    // Gets the Speed and Acceleration from the configuration file
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_HOME_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_ARM)[MotorConfig::PARAM_HOME_ACC]);
    return device->activateAutomaticHoming(HOMING_ON_METHOD, HOMING_OFF_METHOD, speed, acc);
}

/// <summary>
/// 
/// 
/// </summary>
/// 
/// The manual zero setting procedure is aprocedure that doesn't require
/// any motor activation. The motor sets its internal encoder to the value
/// manually passed by this module.
/// 
/// This method should be called by the application for service,
/// in case a position manual zero setting should be executed.
/// 
/// This methods is not a blocking method for the caller thread:
/// + the encoder reset may takes time after this method returns.
/// 
/// \note:
/// + the Application shall call the method CanOpenMotor::isRunning() to checks if the 
/// manual command is terminated;
/// + the application shall call the CanOpenMotor::getCommandCompletedCode() to get the result 
/// of the command as soon as the CanOpenMotor::isRunning() should return false.
/// 
/// <param name="target_position">This is the current effective position in 0.01 degrees units</param>
/// <returns>
/// + true: the zero setting process is actually started;
/// + false: the zero setting cannot start
/// </returns>
bool ArmMotor::startManualHoming(int target_position) {
    if (device->isPositionFromExternalSensor()) return device->activateExternalHoming(target_position);
    else return device->activateManualHoming(target_position);
}

/// <summary>
/// The module overrrides this callback in order to activate a specific error message
/// </summary>
/// 
/// 
/// <param name="errstat"></param>
/// <param name="data_changed"></param>
/// <param name="error_class"></param>
/// <param name="error_code"></param>
void ArmMotor::faultCallback(bool errstat, bool data_changed, unsigned int error_class, unsigned int error_code) {
    

    if (errstat == false) {
        Notify::deactivate(Notify::messages::INFO_ARM_DRIVER);
        return;
    }


    if (data_changed) {
        //System::String^ driver_error = "CL:" + error_class.ToString() + " CD:" + error_code.ToString();
        System::String^ driver_error = CanOpenMotor::getErrorCode1003(error_code);
        Notify::activate(Notify::messages::INFO_ARM_DRIVER, driver_error);
    }

   

}

