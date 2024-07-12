
#include "PCB303.h"
#include "PCB302.h"
#include "CalibrationConfig.h"
#include "../gantry_global_status.h"
#include "Notify.h"
#include <thread>


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





void PCB302::evaluateEvents(void) {
	
	// Assignes the paddle detected code
	if (detected_paddle == paddleCodes::PADDLE_NOT_DETECTED) {
		compression_force = 0;
		breast_thickness = 0;		
		return;
	}

	// Assignes the breast_thickness based on the current paddle and the paddle offset
	if (protocol.status_register.compression_on) {
		breast_thickness = (int) protocol.status_register.paddle_position - thickness_correction;
		compression_force = protocol.status_register.paddle_force;
	}
	else {
		breast_thickness = 0;
		compression_force = 0;
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

	// 0.1mm position of the filters and mirror in the filter slots
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
	compression_executing = protocol.status_register.compression_on && (protocol.status_register.downward_direction || protocol.status_register.upward_direction);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
	// Paddle Register
	protocol.status_register.decodePaddleRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::PADDLE_REGISTER));
	
	// Raw Paddle Register
	protocol.status_register.decodeRawPaddleRegister(readStatusRegister((unsigned char)ProtocolStructure::StatusRegister::register_index::RAW_PADDLE_REGISTER));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// To be modified
	detected_paddle = paddleCodes::PADDLE_24x30_CONTACT;		
	magnifier_device_detected = false;
	patient_protection_detected = true; 
	patient_protection_shifted = false; 
	setPositionLimit(200);// To be modified
	
	
	writeDataRegister((unsigned char)ProtocolStructure::DataRegister::register_index::POSITION_LIMIT_REGISTER, protocol.data_register.encodePositionLimitRegister());
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	writeDataRegister((unsigned char)ProtocolStructure::DataRegister::register_index::OPTIONS_REGISTER, protocol.data_register.encodeOptionsRegister());

	evaluateEvents();

	return;
}

void PCB302::demoLoop(void) {
	
	// To be done
	magnifier_device_detected = false;
	patient_protection_detected = true;
	patient_protection_shifted = false;

	protocol.status_register.compression_on = false;
	protocol.status_register.downward_direction = false;
	compression_executing = false;

	protocol.status_register.paddle_position = 0;
	protocol.status_register.paddle_force = 0;


	detected_paddle = paddleCodes::PADDLE_24x30_CONTACT;
	evaluateEvents();
	std::this_thread::sleep_for(std::chrono::microseconds(1000));
	return;
}
