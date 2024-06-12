#pragma once
#include "CanDeviceProtocol.h"

/// <summary>
/// \defgroup PCB302_Module PCB302 Module: Compressor device module controller
/// \ingroup CanDevice_Module 
/// </summary>
ref class PCB302 : public CanDeviceProtocol
{
public:

#define PCB302_GET_STATUS_SYSTEM_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false

	/// <summary>
	///		This is the Device STATUS Register implementation 
	/// </summary>
	enum class StatusRegisters {
		SYSTEM_STATUS_REGISTER = 0, //!> This is the System Status register index
		SYSTEM_ERROR_REGISTER,
		PADDLE_STATUS_REGISTER,
		RAW_PADDLE_STATUS_REGISTER		
	};

	#define PCB302_GET_SYSTEM_MANUAL_SERVO_UP(reg)			(bool)	(reg->d0 & 0x01)
	#define PCB302_GET_SYSTEM_MANUAL_SERVO_DWN(reg)			(bool)	(reg->d0 & 0x02)
	#define PCB302_GET_SYSTEM_PEDAL_UP(reg)					(bool)	(reg->d0 & 0x04)
	#define PCB302_GET_SYSTEM_PEDAL_DWN(reg)				(bool)	(reg->d0 & 0x08)

	#define PCB302_GET_SYSTEM_IDLE(reg)						(bool)	(reg->d1 & 0x01)
	#define PCB302_GET_SYSTEM_MANUAL_ACTIVATION(reg)		(bool)	(reg->d1 & 0x02)
	#define PCB302_GET_SYSTEM_PEDAL_ACTIVATION(reg)			(bool)	(reg->d1 & 0x04)
	#define PCB302_GET_SYSTEM_COMMAND_ACTIVATION(reg)		(bool)	(reg->d1 & 0x08)
	#define PCB302_GET_SYSTEM_UPWARD_DIRECTION(reg)			(bool)	(reg->d1 & 0x10)
	#define PCB302_GET_SYSTEM_DOWNWARD_DIRECTION(reg)		(bool)	(reg->d1 & 0x20)
	#define PCB302_GET_SYSTEM_UNLOCK_ACTIVATION(reg)		(bool)	(reg->d1 & 0x40)
	#define PCB302_GET_SYSTEM_FAULT(reg)					(bool)	(reg->d1 & 0x80)

	#define PCB302_GET_SYSTEM_CMP_ENA(reg)					(bool)	(reg->d2 & 0x01)
	#define PCB302_GET_SYSTEM_CMP_ON(reg)					(bool)	(reg->d2 & 0x02)
	#define PCB302_GET_SYSTEM_ZERO_COMPRESSION(reg)			(bool)	(reg->d2 & 0x04)
	#define PCB302_GET_SYSTEM_SMARTUP_TARGET(reg)			(bool)	(reg->d2 & 0x08)
	#define PCB302_GET_SYSTEM_FORCE_TARGET(reg)				(bool)	(reg->d2 & 0x10)
	#define PCB302_GET_SYSTEM_LIMIT_COMPRESSION(reg)		(bool)	(reg->d2 & 0x20)

	#define PCB302_GET_PADDLE_POSITION_LOW(reg)				(reg->d0)
	#define PCB302_GET_PADDLE_POSITION_HIGH(reg)			(reg->d1&0x0F)
	#define PCB302_GET_PADDLE_FORCE_LOW(reg)				((reg->d1&0xF0) >> 4)
	#define PCB302_GET_PADDLE_FORCE_HIGH(reg)				(reg->d2)

	#define PCB302_GET_PADDLE_CODE(reg)						(reg->d3)

	/// <summary>
	///	 This is the Device DATA Register implementation 
	/// </summary>
	enum class DataRegisters {
		POSITION_LIMIT_DATA_REGISTER = 0, 
		OPTIONS_DATA_REGISTER, 
	};
	#define PCB302_POSITION_LIMIT_DATA_LOW(reg, val)					reg->d0 = (unsigned char) val;
	#define PCB302_POSITION_LIMIT_DATA_HIGH(reg,stat)					reg->d1 = (unsigned char) (val>>8);

	#define PCB302_OPTIONS_DATA_POSITION_CALIBRATION(reg,stat)				reg->D0(stat, 0x1) 
	#define PCB302_OPTIONS_DATA_FORCE_CALIBRATION(reg,stat)					reg->D0(stat, 0x2) 
	#define PCB302_OPTIONS_DATA_MASTER_ENABLE(reg,stat)						reg->D0(stat, 0x4) 
	#define PCB302_OPTIONS_DATA_ZERO_COMPRESSION(reg,stat)					reg->D0(stat, 0x8) 


	/// <summary>
	/// This is the device protocol Parameters registers implementation 
	/// </summary>
	enum class ParamRegisters {
		POSITION_PARAM_REGISTER = 0, 
		FORCE_CALIBRATION_PARAM_REGISTER,
		COMPRESSION_PARAM_REGISTER,		
		UPWARD_PARAM_REGISTER,		
		DOWNWARD_PARAM_REGISTER,			
	};

	
	/// <summary>	
	/// This enumeration class defines the Indexes of the Command Execution
	///
	/// </summary>
	enum class Commandregister {
		ABORT_COMMAND = 0, //!< Abort Command (mandatory as for device protocol)
		SET_TRIMMERS_COMMAND,
		
		SET_COMPRESSION,
		SET_UNLOCK,
	};
	#define PCB302_SET_UNLOCK_COMMAND (System::Byte) Commandregister::SET_UNLOCK, (System::Byte) (0),(System::Byte) 0,(System::Byte) 0,(System::Byte) 0 //!< This is the SET_POSITIONER_COMMAND byte frame

	PCB302() : CanDeviceProtocol(0x11, L"COMPRESSOR_DEVICE")
	{
		detected_paddle = paddleCodes::PADDLE_NOT_DETECTED;
		

	}
	static PCB302^ device = gcnew PCB302();

	/// <summary>
	///  This enumeration class defines the Paddle identified by the Application
	/// 
	///  
	/// </summary>
	enum class  paddleCodes {
	PADDLE_PROSTHESIS=0, //!< Paddle PROSTHESIS format
	PADDLE_BIOP2D, //!< Paddle BIOPSY 2D format
	PADDLE_BIOP3D, //!< Paddle BIOPSY STEREO format
	PADDLE_TOMO, //!< Paddle TOMO 24x30 format
	PADDLE_24x30_CONTACT, //!< Paddle 24x30 format
	PADDLE_18x24_C_CONTACT, //!< Paddle 18x24 CENTER format
	PADDLE_18x24_L_CONTACT, //!< Paddle 18x24 LEFT format
	PADDLE_18x24_R_CONTACT, //!< Paddle 18x24 RIGHT format
	PADDLE_10x24_CONTACT, //!< Paddle 10x24 format
	PADDLE_9x21_MAG, //!< Paddle 9x21(MAG) format
	PADDLE_9x9_MAG, //!< Paddle TOMO 9x9(MAG) format
	PADDLE_D75_MAG, //!< Paddle D75(MAG) format
	PADDLE_LEN,
	PADDLE_NOT_DETECTED = PADDLE_LEN
	};
	static const cli::array<System::String^>^ paddle_names = gcnew cli::array<System::String^> { 
		"PROSTHESIS", 
		"BIOP2D", 
		"BIOP3D", 
		"TOMO",
		"24x30",
		"18x24C",
		"18x24L",
		"18x24R",
		"10x24 CNT",
		"9x21 MAG",
		"9x9 MAG",
		"D75 MAG",
	};
	static System::String^ getPaddleName(paddleCodes paddle) {
		if (paddle == paddleCodes::PADDLE_NOT_DETECTED) return "";
		for (int i = 0; i < (int)paddleCodes::PADDLE_LEN; i++) {
			if ((int)paddle == i) return paddle_names[i];
		}
		return "";
	}

	inline static bool isPatientProtection(void) { return patient_protection_detected; }
	inline static bool isPatientProtectionShifted(void) { return patient_protection_shifted; }

	static inline unsigned short getThickness(void) { return breast_thickness; }; //!< This function returnrs the current thickness in mm
	static inline unsigned short getForce(void) { return compression_force; }; //!< This function returnrs the current compression force in N
	static inline bool isCompressing(void) { return compression_executing; }
	static inline bool getCompressionActivationStatus(void) { return downward_activation_status; }

	static inline void setMasterEna(bool stat) { PCB302_OPTIONS_DATA_MASTER_ENABLE(options_data_register, stat); }
	static inline void setPositionLimit(unsigned short val) { PCB302_POSITION_LIMIT_DATA_LOW(position_limit_data_register, val); PCB302_POSITION_LIMIT_DATA_HIGH(position_limit_data_register, val);}	
	static void setCompressorUnlock(void) { device->commandNoWaitCompletion(PCB302_SET_UNLOCK_COMMAND, 30); } //!< This function unlocks the compression

public:
	static int getPaddleCollimationFormatIndex(unsigned char paddle_code); //!< This function returns the index of the collimation format associated at the paddle.
	static paddleCodes getPaddleCode(System::String^ tag); //!< This function returns the paddle code from the paddle name	
	static paddleCodes getDetectedPaddleCode(void); //!< This function returns the current detected paddle code
	static int getDetectedPaddleCollimationFormat(void); //!< This function returns he collimation format index associated to the detected paddle
	
	inline static bool isMagnifierDeviceDetected(void) { return magnifier_device_detected; }
	static System::String^ getMagnifierfactorString(void) { return magnifier_factor_string; }


protected: 	
	void runningLoop(void) override;
	void demoLoop(void) override;
	bool configurationLoop(void) override;

private: 
	static paddleCodes detected_paddle;				//!< This is the current detected paddle
	static int thickness_correction;
	static unsigned short current_paddle_position;  //!< Current paddle position 
	static unsigned short breast_thickness = 0;		//!< Compressed breast thickness in mm (0 if the compression_on should be false)

	static unsigned short compression_force = 0;	//!< Evaluated compression force ( 0 if the compression_on should be false)
	static unsigned short current_force = 0;		//!< Force as received from the device

	static bool downward_activation_status = false; //!< A downward activation has been detected
	static bool compression_executing = false;		//!< A compression is executing
	static bool compression_on = false;				//!< A compression is detected

	static Register^ options_data_register = gcnew Register();
	static Register^ position_limit_data_register = gcnew Register();

	static bool patient_protection_detected = false; //!< Is set if the patient protection should be detcted
	static bool patient_protection_shifted = false; //!< Is set if the patient protection should be shifted out of standard position
	static bool magnifier_device_detected = false; //!< Is set if the magnifier device has been detected
	static System::String^ magnifier_factor_string = "1.0"; //!< This is the current magnification factor detected in string format


	void handleSystemStatusRegister(void);
	void handlePaddleStatusRegister(void);
	void evaluateEvents(void);


};


