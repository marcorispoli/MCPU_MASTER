
#include "PCB303.h"
#include "PCB302.h"
#include "CalibrationConfig.h"
#include "../gantry_global_status.h"
#include "ArmMotor.h"
#include "Notify.h"
#include <thread>


void PCB302::moduleInitialize(void) {

	// Checks if the position has been already calibrated	
	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_COMPRESSOR)[PaddleConfig::COMPRESSOR_CALIBRATION_POSITION_STATUS]) == 0) {
		Notify::activate(Notify::messages::WARNING_POSITION_NOT_CALIBRATED);
		position_calibrated = false;
	}
	else {
		position_calibrated = true;
		Notify::deactivate(Notify::messages::WARNING_POSITION_NOT_CALIBRATED);
	}

	// Checks if the force has been already calibrated	
	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_COMPRESSOR)[PaddleConfig::COMPRESSOR_CALIBRATION_FORCE_STATUS]) == 0) {
		Notify::activate(Notify::messages::WARNING_FORCE_NOT_CALIBRATED);
		force_calibrated = false;
	}
	else {
		force_calibrated = true;
		Notify::deactivate(Notify::messages::WARNING_FORCE_NOT_CALIBRATED);
	}
}


/// <summary>
/// 
/// </summary>
/// 
/// This function is called by the Base class before to call the runningLoop() 
/// allowing the module to properly configure the device.
/// 
/// 
/// <param name=""></param>
/// <returns></returns>
bool PCB302::configurationLoop(void) {

	// Gets the limit compression force: the admittet limit is from 70 to 200N
	protocol.parameter_register.limit_compression = System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_COMPRESSOR)[PaddleConfig::COMPRESSOR_MAX_FORCE]);
	if (protocol.parameter_register.limit_compression > 200) protocol.parameter_register.limit_compression = 200;
	if (protocol.parameter_register.limit_compression < 70) protocol.parameter_register.limit_compression = 70;

	// Gets the Target compression force: the admittet value is from 70 to LIMIT
	protocol.parameter_register.target_compression = System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_COMPRESSOR)[PaddleConfig::COMPRESSOR_TARGET_FORCE]);
	if (protocol.parameter_register.target_compression >= protocol.parameter_register.limit_compression) protocol.parameter_register.target_compression = protocol.parameter_register.limit_compression;
	if (protocol.parameter_register.target_compression < 70) protocol.parameter_register.target_compression = 70;

	// Uploads the COMPRESSION parameters
	writeParamRegister((int)ProtocolStructure::ParameterRegister::command_index::COMPRESSION, protocol.parameter_register.encodeCompressionParamRegister());
	
	// Limit position Upload
	protocol.parameter_register.limit_position = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_COMPRESSOR)[PaddleConfig::COMPRESSOR_MAX_POSITION]);
	writeParamRegister((int)ProtocolStructure::ParameterRegister::command_index::POSITION, protocol.parameter_register.encodePositionParamRegister());

	return true;
}

/// <summary>
/// This function returns the index of the collimation format associated at the paddle.
/// 
/// The index of the collimation format should be in the range of 1:x where x 
/// should depend by the collimator number of available collimation format parameters.
/// 
///		NOTE: This module cannot check the index value that depends by other modules (the collimaotor).
/// 
/// 
/// </summary>
/// <param name="paddle_code">the code of the paddle to be investigated</param>
/// <returns>the collimation format or .-1 if the paddle is notn a valid paddle</returns>
int PCB302::getPaddleCollimationFormatIndex(unsigned char paddle_code) {


	System::String^ format = "";
	switch ((paddleCodes) paddle_code) {
	case paddleCodes::PADDLE_PROSTHESIS:
		format = PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_PROSTHESIS)[PaddleConfig::PADDLE_COLLIMATION];
		break;
	case paddleCodes::PADDLE_BIOP2D:
		format = PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_BIOP2D)[PaddleConfig::PADDLE_COLLIMATION];
		break;
	case paddleCodes::PADDLE_BIOP3D:
		format = PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_BIOP3D)[PaddleConfig::PADDLE_COLLIMATION];
		break;
	case paddleCodes::PADDLE_TOMO:
		format = PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_TOMO)[PaddleConfig::PADDLE_COLLIMATION];
		break;
	case paddleCodes::PADDLE_24x30_CONTACT:
		format = PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_24x30_CONTACT)[PaddleConfig::PADDLE_COLLIMATION];
		break;
	case paddleCodes::PADDLE_18x24_C_CONTACT:
		format = PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_C_CONTACT)[PaddleConfig::PADDLE_COLLIMATION];
		break;
	case paddleCodes::PADDLE_18x24_L_CONTACT:
		format = PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_L_CONTACT)[PaddleConfig::PADDLE_COLLIMATION];
		break;
	case paddleCodes::PADDLE_18x24_R_CONTACT:
		format = PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_R_CONTACT)[PaddleConfig::PADDLE_COLLIMATION];
		break;
	case paddleCodes::PADDLE_10x24_CONTACT:
		format = PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_10x24_CONTACT)[PaddleConfig::PADDLE_COLLIMATION];
		break;
	case paddleCodes::PADDLE_9x21_MAG:
		format = PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_9x21_MAG)[PaddleConfig::PADDLE_COLLIMATION];
		break;
	case paddleCodes::PADDLE_9x9_MAG:
		format = PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_9x9_MAG)[PaddleConfig::PADDLE_COLLIMATION];
		break;
	case paddleCodes::PADDLE_D75_MAG:
		format = PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_D75_MAG)[PaddleConfig::PADDLE_COLLIMATION];
		break;	
	}

	return (int) PCB303::getColliFormatIndexFromParam(format);
}

/// <summary>
/// This function returns the paddle code from the paddle name
/// 
/// The Paddle name is a string name describing the Paddle.\n
/// The paddle name is used in the system for string protocols like the AWS protocol.
/// 
/// </summary>
/// <param name="tag">name of the paddle</param>
/// <returns>the paddle code or -1 if the no paddle is found</returns>
PCB302::paddleCodes PCB302::getPaddleCode(System::String^ tag) {
	for (int i = 0; i < (int) paddleCodes::PADDLE_LEN; i++) {
		if (tag == ((paddleCodes)i).ToString() ) return (paddleCodes) i;
	}

	return paddleCodes::PADDLE_NOT_DETECTED;
}



/// <summary>
/// This function returns the current detected paddle code
/// 
/// </summary>
/// <param name=""></param>
/// <returns>the current detected paddle code</returns>
PCB302::paddleCodes PCB302::getDetectedPaddleCode(void) {
	return detected_paddle;
}

/// <summary>
/// This function returns he collimation format index associated to the detected paddle
/// 
/// </summary>
/// <param name=""></param>
/// <returns>the format index or -1 if no format is available </returns>
int PCB302::getDetectedPaddleCollimationFormat(void) {
	return getPaddleCollimationFormatIndex((int) detected_paddle);
}; 



void PCB302::getDetectedPaddleData(void) {
	static unsigned char current_tag = 255;
	if (protocol.status_register.paddle_tag == current_tag) return;
	current_tag = protocol.status_register.paddle_tag;

	// Find the data of the detected paddle exploring the configuration file
	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_PROSTHESIS)[PaddleConfig::PADDLE_TAG]) == current_tag) {
		detected_paddle = paddleCodes::PADDLE_PROSTHESIS;
		detected_paddle_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_PROSTHESIS)[PaddleConfig::PADDLE_POSITION_OFFSET]);
		detected_paddle_weight = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_PROSTHESIS)[PaddleConfig::PADDLE_WEIGHT]);
		Notify::deactivate(Notify::messages::WARNING_MISSING_PADDLE);
		return;
	}

	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_BIOP2D)[PaddleConfig::PADDLE_TAG]) == current_tag) {
		detected_paddle = paddleCodes::PADDLE_BIOP2D;
		detected_paddle_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_BIOP2D)[PaddleConfig::PADDLE_POSITION_OFFSET]);
		detected_paddle_weight = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_BIOP2D)[PaddleConfig::PADDLE_WEIGHT]);
		Notify::deactivate(Notify::messages::WARNING_MISSING_PADDLE);
		return;
	}

	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_BIOP3D)[PaddleConfig::PADDLE_TAG]) == current_tag) {
		detected_paddle = paddleCodes::PADDLE_BIOP3D;
		detected_paddle_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_BIOP3D)[PaddleConfig::PADDLE_POSITION_OFFSET]);
		detected_paddle_weight = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_BIOP3D)[PaddleConfig::PADDLE_WEIGHT]);
		Notify::deactivate(Notify::messages::WARNING_MISSING_PADDLE);
		return;
	}

	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_TOMO)[PaddleConfig::PADDLE_TAG]) == current_tag) {
		detected_paddle = paddleCodes::PADDLE_TOMO;
		detected_paddle_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_TOMO)[PaddleConfig::PADDLE_POSITION_OFFSET]);
		detected_paddle_weight = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_TOMO)[PaddleConfig::PADDLE_WEIGHT]);
		Notify::deactivate(Notify::messages::WARNING_MISSING_PADDLE);
		return;
	}

	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_24x30_CONTACT)[PaddleConfig::PADDLE_TAG]) == current_tag) {
		detected_paddle = paddleCodes::PADDLE_24x30_CONTACT;
		detected_paddle_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_24x30_CONTACT)[PaddleConfig::PADDLE_POSITION_OFFSET]);
		detected_paddle_weight = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_24x30_CONTACT)[PaddleConfig::PADDLE_WEIGHT]);
		Notify::deactivate(Notify::messages::WARNING_MISSING_PADDLE);
		return;
	}

	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_C_CONTACT)[PaddleConfig::PADDLE_TAG]) == current_tag) {
		detected_paddle = paddleCodes::PADDLE_18x24_C_CONTACT;
		detected_paddle_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_C_CONTACT)[PaddleConfig::PADDLE_POSITION_OFFSET]);
		detected_paddle_weight = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_C_CONTACT)[PaddleConfig::PADDLE_WEIGHT]);
		Notify::deactivate(Notify::messages::WARNING_MISSING_PADDLE);
		return;
	}

	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_L_CONTACT)[PaddleConfig::PADDLE_TAG]) == current_tag) {
		detected_paddle = paddleCodes::PADDLE_18x24_L_CONTACT;
		detected_paddle_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_L_CONTACT)[PaddleConfig::PADDLE_POSITION_OFFSET]);
		detected_paddle_weight = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_L_CONTACT)[PaddleConfig::PADDLE_WEIGHT]);
		Notify::deactivate(Notify::messages::WARNING_MISSING_PADDLE);
		return;
	}

	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_R_CONTACT)[PaddleConfig::PADDLE_TAG]) == current_tag) {
		detected_paddle = paddleCodes::PADDLE_18x24_R_CONTACT;
		detected_paddle_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_R_CONTACT)[PaddleConfig::PADDLE_POSITION_OFFSET]);
		detected_paddle_weight = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_R_CONTACT)[PaddleConfig::PADDLE_WEIGHT]);
		Notify::deactivate(Notify::messages::WARNING_MISSING_PADDLE);
		return;
	}

	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_10x24_CONTACT)[PaddleConfig::PADDLE_TAG]) == current_tag) {
		detected_paddle = paddleCodes::PADDLE_10x24_CONTACT;
		detected_paddle_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_10x24_CONTACT)[PaddleConfig::PADDLE_POSITION_OFFSET]);
		detected_paddle_weight = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_10x24_CONTACT)[PaddleConfig::PADDLE_WEIGHT]);
		Notify::deactivate(Notify::messages::WARNING_MISSING_PADDLE);
		return;
	}

	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_9x9_MAG)[PaddleConfig::PADDLE_TAG]) == current_tag) {
		detected_paddle = paddleCodes::PADDLE_9x9_MAG;
		detected_paddle_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_9x9_MAG)[PaddleConfig::PADDLE_POSITION_OFFSET]);
		detected_paddle_weight = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_9x9_MAG)[PaddleConfig::PADDLE_WEIGHT]);
		Notify::deactivate(Notify::messages::WARNING_MISSING_PADDLE);
		return;
	}

	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_9x21_MAG)[PaddleConfig::PADDLE_TAG]) == current_tag) {
		detected_paddle = paddleCodes::PADDLE_9x21_MAG;
		detected_paddle_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_9x21_MAG)[PaddleConfig::PADDLE_POSITION_OFFSET]);
		detected_paddle_weight = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_9x21_MAG)[PaddleConfig::PADDLE_WEIGHT]);
		Notify::deactivate(Notify::messages::WARNING_MISSING_PADDLE);
		return;
	}

	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_D75_MAG)[PaddleConfig::PADDLE_TAG]) == current_tag) {
		detected_paddle = paddleCodes::PADDLE_D75_MAG;
		detected_paddle_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_D75_MAG)[PaddleConfig::PADDLE_POSITION_OFFSET]);
		detected_paddle_weight = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_D75_MAG)[PaddleConfig::PADDLE_WEIGHT]);
		Notify::deactivate(Notify::messages::WARNING_MISSING_PADDLE);
		return;
	}

	detected_paddle = paddleCodes::PADDLE_NOT_DETECTED;
	detected_paddle_offset = 0;
	detected_paddle_weight = 0;
	Notify::activate(Notify::messages::WARNING_MISSING_PADDLE);
	return;

}


void PCB302::runningLoop(void) {
	static int count = 0;
	static bool commerr = false;

	// Test the communication status
	if (commerr != isCommunicationError()) {
		commerr = isCommunicationError();
		if (isCommunicationError()) {
			Notify::activate(Notify::messages::ERROR_PCB302_COMMUNICATION_ERROR);
		}
		else {
			Notify::deactivate(Notify::messages::ERROR_PCB302_COMMUNICATION_ERROR);
		}
	}

	//	Status Register
	protocol.status_register.decodeSystemRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::SYSTEM_REGISTER));	
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	
	// Paddle Register
	protocol.status_register.decodePaddleRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::PADDLE_REGISTER));
	std::this_thread::sleep_for(std::chrono::milliseconds(20));


	// Gets the detected paddle parameters: code, weight and offset
	getDetectedPaddleData();

	// If calibrated, it is the current holder distance
	if (position_calibrated) {
		int holder_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_COMPRESSOR)[PaddleConfig::COMPRESSOR_HOLDER_OFFSET]);
		holder_position = protocol.status_register.paddle_position - holder_offset;
	}
	else holder_position = 0;

	// Assignes the paddle Code from the protocol code
	if ((detected_paddle == paddleCodes::PADDLE_NOT_DETECTED) || (!protocol.status_register.compression_on) || (!position_calibrated) || (!force_calibrated)) {
		compression_force = 0;
		breast_thickness = 0;
	
	}
	else {
		// The magnifier correction (if present)
		int magnifier_offset;
		if (getMagnifierFactor() == 10) magnifier_offset = 0;
		else if (getMagnifierFactor() == 15) magnifier_offset = 150;
		else if (getMagnifierFactor() == 18) magnifier_offset = 180;
		else  magnifier_offset = 200;

		// Calculates the actual thickness and force 
		breast_thickness = holder_position - magnifier_offset - detected_paddle_offset;
		compression_force = protocol.status_register.paddle_force;
	}

	
	// Sets the Paddle Weight data only if they are changed or if it is the first 
	// and if the Arm is properly initialized and calibrated
	static bool update_weight = true;

	// If the motor is not ready or not initialized doesn't update the current weight
	// In this case the update will be forced as soon as the motor returns to be ready and calibrated
	if ( (!ArmMotor::device->isReady()) || (!ArmMotor::device->isZeroOk()))  {
		update_weight = true;
	}
	else {
		// When the Arm motor is ready and calibrated (no running) 
		if (protocol.data_register.absolute_arm_angle != abs(ArmMotor::device->getCurrentPosition() / 100)) {
			protocol.data_register.absolute_arm_angle = abs(ArmMotor::device->getCurrentPosition() / 100);
			update_weight = true;
		}
		if (protocol.data_register.paddle_weight != detected_paddle_weight) {
			protocol.data_register.paddle_weight = detected_paddle_weight;
			update_weight = true;
		}
		if (update_weight) writeDataRegister((unsigned char)ProtocolStructure::DataRegister::register_index::PADDLE_WEIGHT, protocol.data_register.encodePaddleWeightRegister());
		update_weight = false;
	}
	
	return;
}

