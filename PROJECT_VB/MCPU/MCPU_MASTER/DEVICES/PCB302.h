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
public: enum class  paddleCodes {
	PADDLE_PROSTHESIS, //!< Paddle PROSTHESIS format
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

private: static const cli::array<System::String^>^ paddle_name = gcnew cli::array<System::String^>  { 
		"PADDLE_PROSTHESIS",
		"PADDLE_BIOP2D", 
		"PADDLE_BIOP3D", 
		"PADDLE_TOMO", 
		"PADDLE_24x30_CONTACT",
		"PADDLE_18x24_C_CONTACT", 
		"PADDLE_18x24_L_CONTACT", 
		"PADDLE_18x24_R_CONTACT", 
		"PADDLE_10x24_CONTACT", 
		"PADDLE_9x21_MAG", 
		"PADDLE_9x9_MAG", 
		"PADDLE_D75_MAG"
	}; //!< This is the option-tags static array

	
protected: 	virtual void runningLoop(void) override;

private: 
	static paddleCodes detected_paddle; //!< This is the current detected paddle
	static unsigned short breast_thickness = 0;  //!< Compressed breast thickness in mm
	static unsigned short compression_force = 0; //!< Compression force in N

public:
	static int getPaddleCollimationFormatIndex(unsigned char paddle_code); //!< This function returns the index of the collimation format associated at the paddle.
	static int getPaddleCode(System::String^ tag); //!< This function returns the paddle code from the paddle name
	static System::String^ getPaddleName(paddleCodes paddle); //!< This function returns the name of the paddle
	static paddleCodes getDetectedPaddleCode(void); //!< This function returns the current detected paddle code
	static int getDetectedPaddleCollimationFormat(void); //!< This function returns he collimation format index associated to the detected paddle

//_________________________________________________________________________// 
// To be implemented
	
	static void setCompressorUnlock(void) {}; //!< This function unlocks the compression
	static unsigned short getThickness(void) { return breast_thickness; }; //!< This function returnrs the current thickness in mm
	static unsigned short getForce(void) { return compression_force; }; //!< This function returnrs the current compression force in N

};


