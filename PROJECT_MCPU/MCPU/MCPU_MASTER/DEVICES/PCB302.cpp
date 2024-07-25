
#include "PCB303.h"
#include "PCB302.h"
#include "CalibrationConfig.h"
#include "../gantry_global_status.h"
#include "Notify.h"
#include <thread>


void PCB302::moduleInitialize(void) {

	// Checks if the position has been already calibrated	
	if (System::Convert::ToByte(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_POSITION_CALIBRATION)[PaddleConfig::POSITION_CALIBRATION_STATUS]) == 0) {
		Notify::activate(Notify::messages::WARNING_POSITION_NOT_CALIBRATED);
		calibrated = false;
	}
	else {
		calibrated = true;
		Notify::deactivate(Notify::messages::WARNING_POSITION_NOT_CALIBRATED);
	}



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

void PCB302::evaluateEvents(void) {

	


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

	// 0.1mm position of the filters and mirror in the filter slots
	/*
	unsigned short position_offset = System::Convert::ToUInt16(CompressorConfig::Configuration->getParam(CompressorConfig::PARAM_POSITION_CALIBRATION)[CompressorConfig::PARAM_POSITION_CALIBRATION_OFFSET]);
	unsigned short position_gain = System::Convert::ToUInt16(CompressorConfig::Configuration->getParam(CompressorConfig::PARAM_POSITION_CALIBRATION)[CompressorConfig::PARAM_POSITION_CALIBRATION_GAIN]);
	unsigned short force_offset = System::Convert::ToUInt16(CompressorConfig::Configuration->getParam(CompressorConfig::PARAM_FORCE_CALIBRATION)[CompressorConfig::PARAM_FORCE_CALIBRATION_OFFSET]);
	unsigned short force_gain = System::Convert::ToUInt16(CompressorConfig::Configuration->getParam(CompressorConfig::PARAM_FORCE_CALIBRATION)[CompressorConfig::PARAM_FORCE_CALIBRATION_GAIN]);
	unsigned short force_limit = System::Convert::ToUInt16(CompressorConfig::Configuration->getParam(CompressorConfig::PARAM_FORCE_CALIBRATION)[CompressorConfig::PARAM_FORCE_LIMIT]);
	unsigned short force_target = System::Convert::ToUInt16(CompressorConfig::Configuration->getParam(CompressorConfig::PARAM_FORCE_CALIBRATION)[CompressorConfig::PARAM_FORCE_TARGET]);
	thickness_correction = System::Convert::ToInt16(CompressorConfig::Configuration->getParam(CompressorConfig::PARAM_POSITION_CALIBRATION)[CompressorConfig::PARAM_THICKNESS_CORRECTION]);
	
	// Upload Position parameter
	writeParamRegister((int)ProtocolStructure::ParameterRegister::command_index::POSITION_PARAM_REGISTER,protocol.parameter_register.encodePositionParamRegister(position_offset, position_gain));

	// Upload Force parameter
	writeParamRegister((int)ProtocolStructure::ParameterRegister::command_index::FORCE_CALIBRATION_PARAM_REGISTER, protocol.parameter_register.encodeForceParamRegister(force_offset, force_gain));

	// Compression parameter
	writeParamRegister((int)ProtocolStructure::ParameterRegister::command_index::COMPRESSION_PARAM_REGISTER, protocol.parameter_register.encodeCompressionParamRegister(force_limit, force_target));

	//setPositionLimit(200);// To be modified	
	*/

	return true;
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

	// Raw Paddle Register
	//protocol.status_register.decodeRawPaddleRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::RAW_PADDLE_REGISTER));
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// Gets the detected paddle parameters: code, weight and offset
	getDetectedPaddleData();

	// If calibrated, it is the current holder distance
	if (calibrated) {
		int holder_offset = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_POSITION_CALIBRATION)[PaddleConfig::POSITION_HOLDER_OFFSET]);
		holder_position = protocol.status_register.paddle_position - holder_offset;
	}
	else holder_position = 0;

	// Assignes the paddle Code from the protocol code
	if ((detected_paddle == paddleCodes::PADDLE_NOT_DETECTED) || (!protocol.status_register.compression_on) || (!calibrated)) {
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

	
	
	//writeDataRegister((unsigned char)ProtocolStructure::DataRegister::register_index::POSITION_LIMIT_REGISTER, protocol.data_register.encodePositionLimitRegister());
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));

	//writeDataRegister((unsigned char)ProtocolStructure::DataRegister::register_index::OPTIONS_REGISTER, protocol.data_register.encodeOptionsRegister());
	
	return;
}

