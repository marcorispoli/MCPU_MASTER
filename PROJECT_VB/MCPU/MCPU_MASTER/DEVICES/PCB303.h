#pragma once
#include "CanDeviceProtocol.h"

ref class PCB303 : public CanDeviceProtocol
{
public:

	#define PCB303_GET_ERROR_REGISTER (System::Byte) 0, (System::Byte) ProtocolFrameCode::FRAME_READ_ERRORS,(System::Byte) PCB303::StatusRegisters::SYSTEM_STATUS_REGISTER, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false	
	#define PCB303_ERROR_BLADES(d0,d1,d2,d3) (unsigned char) (d2) // PERS-0 code 
	#define PCB303_ERROR_LEFT	0x1
	#define PCB303_ERROR_RIGHT	0x2
	#define PCB303_ERROR_FRONT	0x4
	#define PCB303_ERROR_BACK	0x8
	#define PCB303_ERROR_TRAP	0x10


	/// <summary>
	/// This enumeration class defines the Status registers of the PCB303 board
	/// </summary>
	enum class StatusRegisters {
		SYSTEM_STATUS_REGISTER = 0, //!< System status register index 
		SYSTEM_CURRENT_FB_REGISTER, //!< This is the current Front and Back position (if the position is valid)
		SYSTEM_CURRENT_LR_REGISTER, //!< This is the current Left and Right position (if the position is valid)
		SYSTEM_CURRENT_T_REGISTER,//!< This is the current Trap position (if the position is valid)
	};
	
	/// <summary>
	/// This enumeration class defines the data content (D3 to D6) of the System Status register.
	/// 
	/// </summary>
	enum class StatusDataBytes {
		SYSTEM_COLLIMATION_STATUS = 0, //!> Collimation status data byte see (CollimationStatusCode)
		SYSTEM_FORMAT_INDEX,		//!< Current selected standard collimation format index	(see ColliStandardSelections)
		SYSTEM_TOMO_PULSE,			//!< Current Tomo pulse processing in Tomo dynamic mode
		SYSTEM_SPARE
	};

	/// <summary>
	/// This is the Collimation Status content definition.
	/// 
	/// This Status byte defines the current format selection in the PCB303 board
	/// </summary>
	enum class CollimationStatusCode {
		COLLI_STATUS_OPEN_FORMAT = 0, //!< The open format is currently selected
		COLLI_STATUS_STANDARD_FORMAT,//!< The standard format is currently selected (see the STATUS FORMAT INDEX in that case)
		COLLI_STATUS_CALIB_FORMAT,  //!< The Calibration format is currently selected
		COLLI_STATUS_TOMO_MODE,		//!< The Tomo mode is currently active
		COLLI_STATUS_OUT_OF_POSITION,//!< Not a valid collimation format is selected
		COLLI_STATUS_2D_EXECUTING,	//!< A 2D collimation format selection is executing
	};

	enum class ColliStandardSelections {
		COLLI_NOT_STANDARD = 0,
		COLLI_STANDARD1,
		COLLI_STANDARD2,
		COLLI_STANDARD3,
		COLLI_STANDARD4,
		COLLI_STANDARD5,
		COLLI_STANDARD6,
		COLLI_STANDARD7,
		COLLI_STANDARD8,
		COLLI_STANDARD9,
		COLLI_STANDARD10,
		COLLI_STANDARD11,
		COLLI_STANDARD12,
		COLLI_STANDARD13,
		COLLI_STANDARD14,
		COLLI_STANDARD15,
		COLLI_STANDARD16,
		COLLI_STANDARD17,
		COLLI_STANDARD18,
		COLLI_STANDARD19,
		COLLI_STANDARD20,
	};
	

	// Status registers command
	#define PCB303_GET_STATUS_SYSTEM_REGISTER (System::Byte) 0, (System::Byte) ProtocolFrameCode::FRAME_READ_STATUS,(System::Byte) PCB303::StatusRegisters::SYSTEM_STATUS_REGISTER, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false	
	#define PCB303_GET_SYSTEM_COLLIMATION_STATUS(d0,d1,d2,d3) (unsigned char) (d0)
	#define PCB303_GET_SYSTEM_FORMAT_INDEX(d0,d1,d2,d3) (unsigned char) (d1)
	#define PCB303_GET_SYSTEM_TOMO_PULSE(d0,d1,d2,d3) (unsigned char) (d2)
	#define PCB303_GET_SYSTEM_FLAGS(d0,d1,d2,d3) (unsigned char) (d3)
	#define PCB303_SYSTEM_FLAG_ERRORS 0x1
	
	#define PCB303_GET_STATUS_FB_REGISTER (System::Byte) 0, (System::Byte) ProtocolFrameCode::FRAME_READ_STATUS,(System::Byte) PCB303::StatusRegisters::SYSTEM_CURRENT_FB_REGISTER, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false	
	#define PCB303_GET_SYSTEM_FB_FRONT(d0,d1,d2,d3) ((unsigned short) d0 + 256 * (unsigned short) d1) 
	#define PCB303_GET_SYSTEM_FB_BACK(d0,d1,d2,d3) ((unsigned short) d2 + 256 * (unsigned short) d3) 

	#define PCB303_GET_STATUS_LR_REGISTER (System::Byte) 0, (System::Byte) ProtocolFrameCode::FRAME_READ_STATUS,(System::Byte) PCB303::StatusRegisters::SYSTEM_CURRENT_LR_REGISTER, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false	
	#define PCB303_GET_SYSTEM_LR_LEFT(d0,d1,d2,d3) ((unsigned short) d0 + 256 * (unsigned short) d1) 
	#define PCB303_GET_SYSTEM_LR_RIGHT(d0,d1,d2,d3) ((unsigned short) d2 + 256 * (unsigned short) d3) 

	#define PCB303_GET_STATUS_T_REGISTER (System::Byte) 0, (System::Byte) ProtocolFrameCode::FRAME_READ_STATUS,(System::Byte) PCB303::StatusRegisters::SYSTEM_CURRENT_T_REGISTER, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false	
	#define PCB303_GET_SYSTEM_T_TRAP(d0,d1,d2,d3) ((unsigned short) d0 + 256 * (unsigned short) d1) 

	/// <summary>
	/// This is the enumeration class definigs the indexes of the protocol DATA registers
	/// </summary>
	enum class DataRegisters {
		DATA_CALIBRATION_FB = 0,
		DATA_CALIBRATION_LR,
		DATA_CALIBRATION_T,
	};
	
	#define PCB303_WRITE_DATA_CALIBRATION_FB(front,back) (System::Byte) 0, (System::Byte) ProtocolFrameCode::FRAME_WRITE_DATA,(System::Byte) DataRegisters::DATA_CALIBRATION_FB,(System::Byte) (front&0xFF), (System::Byte) ((front>>8)&0xFF),(System::Byte) (back&0xFF), (System::Byte) ((back>>8)&0xFF), (System::Byte) 0,false
	#define PCB303_WRITE_DATA_CALIBRATION_LR(left,right) (System::Byte) 0, (System::Byte) ProtocolFrameCode::FRAME_WRITE_DATA,(System::Byte) DataRegisters::DATA_CALIBRATION_LR,(System::Byte) (left&0xFF), (System::Byte) ((left>>8)&0xFF),(System::Byte) (right&0xFF), (System::Byte) ((right>>8)&0xFF), (System::Byte) 0,false
	#define PCB303_WRITE_DATA_CALIBRATION_T(trap) (System::Byte) 0, (System::Byte) ProtocolFrameCode::FRAME_WRITE_DATA,(System::Byte) DataRegisters::DATA_CALIBRATION_T,(System::Byte) (trap&0xFF), (System::Byte) ((trap>>8)&0xFF),(System::Byte) (trap&0xFF), (System::Byte) ((trap>>8)&0xFF), (System::Byte) 0,false

	// Parameters
	enum class ParamRegisters {
		PARAM_STANDARD_FORMAT_FT = 0,
		PARAM_STANDARD_FORMAT_LR1,
		PARAM_STANDARD_FORMAT_B1,
		PARAM_STANDARD_FORMAT_LR2,
		PARAM_STANDARD_FORMAT_B2,
		PARAM_STANDARD_FORMAT_LR3,
		PARAM_STANDARD_FORMAT_B3,
		PARAM_STANDARD_FORMAT_LR4,
		PARAM_STANDARD_FORMAT_B4,
		PARAM_STANDARD_FORMAT_LR5,
		PARAM_STANDARD_FORMAT_B5,
		PARAM_STANDARD_FORMAT_LR6,
		PARAM_STANDARD_FORMAT_B6,
		PARAM_STANDARD_FORMAT_LR7,
		PARAM_STANDARD_FORMAT_B7,
		PARAM_STANDARD_FORMAT_LR8,
		PARAM_STANDARD_FORMAT_B8,
		PARAM_STANDARD_FORMAT_LR9,
		PARAM_STANDARD_FORMAT_B9,
		PARAM_STANDARD_FORMAT_LR10,
		PARAM_STANDARD_FORMAT_B10,
		PARAM_STANDARD_FORMAT_LR11,
		PARAM_STANDARD_FORMAT_B11,
		PARAM_STANDARD_FORMAT_LR12,
		PARAM_STANDARD_FORMAT_B12,
		PARAM_STANDARD_FORMAT_LR13,
		PARAM_STANDARD_FORMAT_B13,
		PARAM_STANDARD_FORMAT_LR14,
		PARAM_STANDARD_FORMAT_B14,
		PARAM_STANDARD_FORMAT_LR15,
		PARAM_STANDARD_FORMAT_B15,
		PARAM_STANDARD_FORMAT_LR16,
		PARAM_STANDARD_FORMAT_B16,
		PARAM_STANDARD_FORMAT_LR17,
		PARAM_STANDARD_FORMAT_B17,
		PARAM_STANDARD_FORMAT_LR18,
		PARAM_STANDARD_FORMAT_B18,
		PARAM_STANDARD_FORMAT_LR19,
		PARAM_STANDARD_FORMAT_B19,
		PARAM_STANDARD_FORMAT_LR20,
		PARAM_STANDARD_FORMAT_B20,
	};

	#define PCB303_WRITE_PARAM_STANDARD_FT(front,trap) (System::Byte) 0, (System::Byte) ProtocolFrameCode::FRAME_WRITE_PARAM,(System::Byte) ParamRegisters::PARAM_STANDARD_FORMAT_FT,(System::Byte) (front&0xFF), (System::Byte) ((front>>8)&0xFF),(System::Byte) (trap&0xFF), (System::Byte) ((trap>>8)&0xFF), (System::Byte) 0,false
	#define PCB303_WRITE_PARAM_STANDARD_LR(index, left, right) (System::Byte) 0, (System::Byte) ProtocolFrameCode::FRAME_WRITE_PARAM,(System::Byte) (ParamRegisters::PARAM_STANDARD_FORMAT_LR1) + (index-1) * 2, (System::Byte) (left&0xFF), (System::Byte) ((left>>8)&0xFF), (System::Byte) (right&0xFF), (System::Byte) ((right>>8)&0xFF), (System::Byte) 0,false
	#define PCB303_WRITE_PARAM_STANDARD_B(index, back) (System::Byte) 0, (System::Byte) ProtocolFrameCode::FRAME_WRITE_PARAM,(System::Byte) (ParamRegisters::PARAM_STANDARD_FORMAT_B1) + (index-1) *2 ,(System::Byte) (back&0xFF), (System::Byte) ((back>>8)&0xFF),(System::Byte) (back&0xFF), (System::Byte) ((back>>8)&0xFF), (System::Byte) 0,false

	enum class Commandregister {
		ABORT_COMMAND = 0,
		SET_OPEN_FORMAT_COMMAND,
		SET_STANDARD_FORMAT_COMMAND,
		SET_CALIBRATION_FORMAT_COMMAND,
		SET_TOMO_MODE_COMMAND
	};
	
	// COMMANDS: CODE, DATA0, DATA1, DATA2, DATA3
	#define PCB303_COMMAND_ABORT (System::Byte) PCB303::Commandregister::ABORT_COMMAND, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0,(System::Byte) 0
	#define PCB303_COMMAND_OPEN_FORMAT (System::Byte) PCB303::Commandregister::SET_OPEN_FORMAT_COMMAND, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0,(System::Byte) 0
	#define PCB303_COMMAND_STANDARD_FORMAT(index) (System::Byte) PCB303::Commandregister::SET_STANDARD_FORMAT_COMMAND, (System::Byte) (index),(System::Byte) 0,(System::Byte) 0,(System::Byte) 0
	#define PCB303_COMMAND_CALIBRATION_FORMAT (System::Byte) PCB303::Commandregister::SET_CALIBRATION_FORMAT_COMMAND, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0,(System::Byte) 0
	#define PCB303_COMMAND_ACTIVATE_TOMO (System::Byte) PCB303::Commandregister::SET_TOMO_MODE_COMMAND, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0,(System::Byte) 0

	

	PCB303() : CanDeviceProtocol(0x12, L"COLLIMATOR_DEVICE")
	{
		collimationMode = collimationModeEnum::OPEN_MODE;
		valid_collimation_format = false;
		collimator_fault = false;
		format_collimation_attempt = 0;
	}

	protected: 	virtual void runningLoop(void) override;
	protected: 	bool configurationLoop(void) override;
	private:	void formatCollimationManagement(void);

	// Application interface section 
	
	/// <summary>
	///  This enumeration class describes the possible collimation mode 
	///  in the Application.
	/// 
	///  
	/// </summary>
	enum class  collimationModeEnum{
		AUTO_COLLIMATION = 0, //!< The collimator is in Auto mode: the paddle model defines the current collimation format
		CUSTOM_COLLIMATION,   //!< The collimator is in Custom mode: the collimation format is set by the operator
		CALIBRATION_MODE,	  //!< The collimator is in Calibration mode: the collimation format is set manually by the service software
		OPEN_MODE,			  //!< The collimator is in Open mode: the collimation format set OPEN
		TOMO_MODE			  //!< The collimator is in Tomo mode: the collimation format is dinamically set by the tomo pulse sequence
	};

	public:ref class formatBlades {
	public:
		formatBlades(unsigned short ft, unsigned short bk, unsigned short lt, unsigned short rt, unsigned short tp) {
			front = ft;
			back = bk;
			left = lt;
			right = rt;
			trap = tp;
		}
		formatBlades() {
			front = back = left = right = trap = 0;
		}

		void copy(formatBlades^ b) {
			front = b->front;
			back = b->back;
			left = b->left;
			right = b->right;
			trap = b->trap;
		}

		unsigned short front;
		unsigned short back;
		unsigned short left;
		unsigned short right;
		unsigned short trap;
	};

	private: collimationModeEnum collimationMode; //!< Sets the current collimation mode 
	private: bool valid_collimation_format; //!< This flag is set when the collimation format is correct and coherent with the collimationMode register
	private: bool collimator_fault; //!< This flag is set when the collimator is in fault condition
	private: int format_collimation_attempt; //!< This register counts the attempt to exit from a fault condition
	private: ColliStandardSelections customStandardSelection;//!< Custom 2D standard format requested (in case of CUSTOM mode)
	private: ColliStandardSelections getAutomaticStandardFormatIndex(void);//!< Retrives what is the current automatic format to be calibrated
	private: formatBlades calibrationBlades;

	private: CollimationStatusCode   collimation_status; //!< last status collimation status read
	private: ColliStandardSelections format_index;		//!< last status format index read
	private: unsigned char system_flags;					//!< Last system flags


public:
	void setAutoCollimationMode(void);
	void setOpenCollimationMode(void);
	void setCalibrationCollimationMode(formatBlades^ blades) ;
	void setTomoCollimationMode(void) {};
	void setCustomCollimationMode(ColliStandardSelections custom );
};


