
#include "PCB303.h"
#include "PCB302.h"
#include "Simulator.h"
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
	
	std::this_thread::sleep_for(std::chrono::microseconds(100));

	// Detected Paddle
	detected_paddle = (paddleCodes) from_simulator[(int)simul_rx_struct::PADDLE_CODE];

	// Thickness 
	current_paddle_position = from_simulator[(int)simul_rx_struct::THICKNESS];
	
	// Force
	current_force = from_simulator[(int)simul_rx_struct::FORCE];
	
	// Compression On 
	compression_on = from_simulator[(int)simul_rx_struct::COMPRESSION_ON];
	
	// Compression Executing 
	compression_executing = from_simulator[(int)simul_rx_struct::COMPRESSION_EXECUTING]; 

	// Downward activation status
	downward_activation_status = from_simulator[(int)simul_rx_struct::DOWNWARD_ACTIVATION];

	// Patient Protection detected
	patient_protection_detected = from_simulator[(int)simul_rx_struct::PATIENT_PROTECTION_DETECTED];
	
	// Patient protection shifted
	patient_protection_shifted = from_simulator[(int)simul_rx_struct::PATIENT_PROTECTION_SHIFTED];

	evaluateEvents();

	return;
}

void PCB302::simulRx(cli::array<System::Byte>^ receiveBuffer, int index, int rc) {
	if (rc != (int)simul_rx_struct::BUFLEN) return;

	for (int i = 0; i < rc; i++) device->from_simulator[i] = receiveBuffer[index + i];
}

void PCB302::simulSend(void) {

	to_simulator[0] = 0x3;
	to_simulator[1] = (int)simul_tx_struct::BUFLEN;
	to_simulator[(int)simul_tx_struct::DEVICE_ID] = PCB302_DEVID;
	to_simulator[(int)simul_tx_struct::ENDFRAME] = 0x2;

	// Sends the buffer
	((Simulator^)Gantry::pSimulator)->send(to_simulator);
}

void PCB302::simulInit(void) {

	// Create the Simulator structure if shuld be  necessary
	from_simulator = gcnew cli::array<System::Byte>((int)PCB302::simul_rx_struct::BUFLEN);
	to_simulator = gcnew cli::array<System::Byte>((int)PCB302::simul_tx_struct::BUFLEN);
	to_simulator_previous = gcnew cli::array<System::Byte>((int)PCB302::simul_tx_struct::BUFLEN);

	from_simulator[(int)simul_rx_struct::PADDLE_CODE] = (System::Byte) paddleCodes::PADDLE_NOT_DETECTED;
	from_simulator[(int)simul_rx_struct::THICKNESS] = 0;
	from_simulator[(int)simul_rx_struct::FORCE] = 0;
	from_simulator[(int)simul_rx_struct::COMPRESSION_ON] = 0;
	from_simulator[(int)simul_rx_struct::COMPRESSION_EXECUTING] = 0;
	from_simulator[(int)simul_rx_struct::DOWNWARD_ACTIVATION] = 0;
	from_simulator[(int)simul_rx_struct::PATIENT_PROTECTION_DETECTED] = false;
	from_simulator[(int)simul_rx_struct::PATIENT_PROTECTION_SHIFTED] = false;

	// Connects the reception event
	((Simulator^)Gantry::pSimulator)->pcb302_rx_event += gcnew Simulator::rxData_slot(&PCB302::simulRx);

}

bool PCB302::simulCommandNoWaitCompletion(unsigned char code, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, int tmo) {
	
	cli::array<System::Byte>^ buffer = gcnew cli::array<System::Byte>(9);

	buffer[0] = 0x4; // Command code
	buffer[1] = 9; // len
	buffer[2] = PCB302_DEVID;
	buffer[3] = code;
	buffer[4] = d0;
	buffer[5] = d1;
	buffer[6] = d2;
	buffer[7] = d3;
	buffer[8] = 0x2;

	// Sends the buffer
	((Simulator^)Gantry::pSimulator)->send(buffer);

	return true;
}
