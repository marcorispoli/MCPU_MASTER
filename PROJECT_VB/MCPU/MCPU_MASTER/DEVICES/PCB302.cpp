
#include "PCB303.h"
#include "PCB302.h"
#include "./DEMO/pcb302_simulator.h"
#include "CalibrationConfig.h"
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


void PCB302::handleSystemStatusRegister(void) {
	Register^ reg = readStatusRegister((unsigned char)StatusRegisters::SYSTEM_STATUS_REGISTER);
	if (reg == nullptr) return;

	compression_on = PCB302_GET_SYSTEM_CMP_ON(reg);
	downward_activation_status = (PCB302_GET_SYSTEM_UPWARD_DIRECTION(reg) || PCB302_GET_SYSTEM_DOWNWARD_DIRECTION(reg));
	compression_executing = compression_on && downward_activation_status;


}

void PCB302::handlePaddleStatusRegister(void) {

	Register^ paddle_status_register = readStatusRegister((unsigned char)StatusRegisters::PADDLE_STATUS_REGISTER);
	if (paddle_status_register == nullptr) return;

	current_paddle_position = PCB302_GET_PADDLE_POSITION_LOW(paddle_status_register) + 256 * PCB302_GET_PADDLE_POSITION_HIGH(paddle_status_register);
	current_force = PCB302_GET_PADDLE_FORCE_LOW(paddle_status_register) + 16 * PCB302_GET_PADDLE_FORCE_HIGH(paddle_status_register);

	// To be modified
	detected_paddle = paddleCodes::PADDLE_24x30_CONTACT;
}

void PCB302::evaluateEvents(void) {
	
	// Assignes the paddle detected code
	if (detected_paddle == paddleCodes::PADDLE_NOT_DETECTED) {
		compression_force = 0;
		breast_thickness = 0;		
		return;
	}

	// Assignes the breast_thickness based on the current paddle and the paddle offset
	if (compression_on) {
		breast_thickness = (int)current_paddle_position - thickness_correction;
		compression_force = current_force;
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
	
	// Upload calibration registers
	unsigned char d0 = (unsigned char) position_offset;
	unsigned char d1 = (unsigned char) (position_offset >> 8) ;
	unsigned char d2 = (unsigned char) position_gain;
	unsigned char d3 = (unsigned char) (position_gain >> 8);
	while (!writeParamRegister((int) ParamRegisters::POSITION_PARAM_REGISTER,d0,d1,d2,d3));

	d0 = (unsigned char)force_offset;
	d1 = (unsigned char)(force_offset >> 8);
	d2 = (unsigned char)force_gain;
	d3 = (unsigned char)(force_gain >> 8);
	while (!writeParamRegister((int)ParamRegisters::FORCE_CALIBRATION_PARAM_REGISTER, d0, d1, d2, d3));

	d0 = (unsigned char)force_limit;	
	d1 = (unsigned char)force_target;
	d2 = 0;
	d3 = 0;
	while (!writeParamRegister((int)ParamRegisters::COMPRESSION_PARAM_REGISTER, d0, d1, d2, d3));


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

	handleSystemStatusRegister();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
	handlePaddleStatusRegister();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// Refresh the Data register
	setPositionLimit(200);
	writeDataRegister((unsigned char)DataRegisters::POSITION_LIMIT_DATA_REGISTER, position_limit_data_register);
	writeDataRegister((unsigned char)DataRegisters::OPTIONS_DATA_REGISTER, options_data_register);

	evaluateEvents();

	return;
}

void PCB302::demoLoop(void) {
	if (!DemoPcb302::Configuration->loadFile()) {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		return;
	}
	std::this_thread::sleep_for(std::chrono::microseconds(100));

	detected_paddle = getPaddleCode(DemoPcb302::Configuration->getParam(DemoPcb302::PARAM_PADDLE_STAT)[0]);
	

	current_paddle_position = System::Convert::ToUInt16(DemoPcb302::Configuration->getParam(DemoPcb302::PARAM_THICKNESS)[0]);
	current_force = System::Convert::ToUInt16(DemoPcb302::Configuration->getParam(DemoPcb302::PARAM_FORCE)[0]);
	
	if (current_force) compression_on = true;
	else compression_on = false;
	
	if (DemoPcb302::Configuration->getParam(DemoPcb302::PARAM_COMPRESSING)[0] == "1") compression_executing = true;
	else compression_executing = false;

	if (DemoPcb302::Configuration->getParam(DemoPcb302::PARAM_DOWNWARD)[0] == "1") downward_activation_status = true;
	else downward_activation_status = false;

	patient_protection_detected = true;
	patient_protection_shifted = false;

	evaluateEvents();

	return;
}