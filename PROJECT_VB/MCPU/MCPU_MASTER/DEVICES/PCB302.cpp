
#include "PCB302.h"
#include "CalibrationConfig.h"

#include <thread>

void PCB302::runningLoop(void) {

    while (!send(PCB302_GET_STATUS_SYSTEM_REGISTER));

    std::this_thread::sleep_for(std::chrono::microseconds(10000));

    return;
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

	int format = -1;
	
	switch ((paddleCodes) paddle_code) {
	case paddleCodes::PADDLE_PROSTHESIS:
		format = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_PROSTHESIS)[PaddleConfig::PADDLE_COLLIMATION]);
		break;
	case paddleCodes::PADDLE_BIOP2D:
		format = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_BIOP2D)[PaddleConfig::PADDLE_COLLIMATION]);
		break;
	case paddleCodes::PADDLE_BIOP3D:
		format = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_BIOP3D)[PaddleConfig::PADDLE_COLLIMATION]);
		break;
	case paddleCodes::PADDLE_TOMO:
		format = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_TOMO)[PaddleConfig::PADDLE_COLLIMATION]);
		break;
	case paddleCodes::PADDLE_24x30_CONTACT:
		format = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_24x30_CONTACT)[PaddleConfig::PADDLE_COLLIMATION]);
		break;
	case paddleCodes::PADDLE_18x24_C_CONTACT:
		format = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_C_CONTACT)[PaddleConfig::PADDLE_COLLIMATION]);
		break;
	case paddleCodes::PADDLE_18x24_L_CONTACT:
		format = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_L_CONTACT)[PaddleConfig::PADDLE_COLLIMATION]);
		break;
	case paddleCodes::PADDLE_18x24_R_CONTACT:
		format = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_18x24_R_CONTACT)[PaddleConfig::PADDLE_COLLIMATION]);
		break;
	case paddleCodes::PADDLE_10x24_CONTACT:
		format = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_10x24_CONTACT)[PaddleConfig::PADDLE_COLLIMATION]);
		break;
	case paddleCodes::PADDLE_9x21_MAG:
		format = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_9x21_MAG)[PaddleConfig::PADDLE_COLLIMATION]);
		break;
	case paddleCodes::PADDLE_9x9_MAG:
		format = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_9x9_MAG)[PaddleConfig::PADDLE_COLLIMATION]);
		break;
	case paddleCodes::PADDLE_D75_MAG:
		format = System::Convert::ToInt16(PaddleConfig::Configuration->getParam(PaddleConfig::PARAM_PADDLE_D75_MAG)[PaddleConfig::PADDLE_COLLIMATION]);
		break;
	
	}

	return format;
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
int PCB302::getPaddleCode(System::String^ tag) {
	for (int i = 0; i < (int) paddleCodes::PADDLE_LEN; i++) {
		if (tag == paddle_name[i]) return i;
	}
	return -1;
}

/// <summary>
/// This function returns the name of the paddle
/// 
///		NOTE: If the paddle code shouldn't match with any paddle name available\n
///		the function returns a NULLPTR. The Application shall check the result before to use it. 
/// 
/// </summary>
/// <param name="paddle"> This is the paddle code</param>
/// <returns>The name of the paddle or nullptr if the name is not found</returns>
System::String^ PCB302::getPaddleName(paddleCodes paddle) {
	for (int i = 0; i < (int) paddleCodes::PADDLE_LEN; i++) {
		if ((int) paddle == i) return paddle_name[i];
	}

	return nullptr;
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