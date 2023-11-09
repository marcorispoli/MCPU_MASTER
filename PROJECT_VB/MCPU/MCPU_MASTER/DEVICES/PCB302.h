#pragma once
#include "CanDeviceProtocol.h"

ref class PCB302 : public CanDeviceProtocol
{
public:

#define PCB302_GET_STATUS_SYSTEM_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false



	PCB302() : CanDeviceProtocol(0x11, L"COMPRESSOR_DEVICE")
	{

	}

	/// <summary>
	///  This enumeration class defines the Paddle identified by the Application
	/// 
	///  
	/// </summary>
	enum class  paddleEnum {
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
	};

protected: 	virtual void runningLoop(void) override;

};


