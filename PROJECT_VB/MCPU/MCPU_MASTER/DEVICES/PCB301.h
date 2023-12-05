#pragma once
#include "CanDeviceProtocol.h"

/// <summary>
/// \defgroup PCB301_Module PCB301 Module: power service board module control
/// \ingroup CanDevice_Module 
/// 
/// </summary>
ref class PCB301 :  public CanDeviceProtocol
{
public:
	
	#define PCB301_GET_STATUS_SYSTEM_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
	#define PCB301_GET_SYSTEM_BATTENA(d0,d1,d2,d3) (bool) (d0 & 0x80)
	#define PCB301_GET_SYSTEM_BATT1LOW(d0,d1,d2,d3) (bool) (d0 & 0x20)
	#define PCB301_GET_SYSTEM_BATT2LOW(d0,d1,d2,d3) (bool) (d0 & 0x40)
	#define PCB301_GET_SYSTEM_POWERDOWN(d0,d1,d2,d3) (bool) (d0 & 0x04)
	
	#define PCB301_GET_VERTICAL_UP(d0,d1,d2,d3) (bool) (d1 & 0x1)
	#define PCB301_GET_VERTICAL_DOWN(d0,d1,d2,d3) (bool) (d1 & 0x2)
	#define PCB301_GET_SHIFT_UP(d0,d1,d2,d3) (bool) (d1 & 0x4)
	#define PCB301_GET_SHIFT_DOWN(d0,d1,d2,d3) (bool) (d1 & 0x8)
	#define PCB301_GET_ARM_CW(d0,d1,d2,d3) (bool) (d1 & 0x10)
	#define PCB301_GET_ARM_CCW(d0,d1,d2,d3) (bool) (d1 & 0x20)
	#define PCB301_GET_BODY_CW(d0,d1,d2,d3) (bool) (d1 & 0x40)
	#define PCB301_GET_BODY_CCW(d0,d1,d2,d3) (bool) (d1 & 0x80)

	#define PCB301_GET_XRAY_PUSH_BUTTON(d0,d1,d2,d3) (bool) (d2 & 0x08)
	#define PCB301_GET_SYSTEM_CLOSEDOOR(d0,d1,d2,d3) (bool) (d2 & 0x10)


	#define PCB301_GET_STATUS_BATTERY_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
	#define PCB301_GET_BATTERY_VBATT1(d3,d4,d5,d6) (unsigned char) (d3)
	#define PCB301_GET_BATTERY_VBATT2(d3,d4,d5,d6) (unsigned char) (d4)


	PCB301(): CanDeviceProtocol(0x10, L"POWER_SERVICE")
	{

	}
	static PCB301^ device = gcnew PCB301();

	protected: 	virtual void runningLoop(void) override;

	/// <summary>
	/// This function returns the current activation status of the X-RAY push button
	/// 
	/// </summary>
	/// <param name=""></param>
	/// <returns>True if the X-RAY push button should be activated</returns>
	public: static inline bool isXrayButton(void) { return xray_push_button_input; }

	/// <summary>
	/// This function sets the current status of the X-RAY enable signal.
	///  
	/// </summary>
	/// <param name="status"></param>
	/// <returns></returns>
	public: static inline void setXrayEnable(bool status) { xray_enable_status_output = status; }

	
	public: enum class vertical_activation_options {
		VERTICAL_NO_ACTIVATION = 0,
		VERTICAL_UP_ACTIVATION,
		VERTICAL_DOWN_ACTIVATION,
		VERTICAL_INVALID_CODE,
	};

	public: enum class body_activation_options {
		BODY_NO_ACTIVATION = 0,
		BODY_CW_ACTIVATION,
		BODY_CCW_ACTIVATION,
		BODY_INVALID_CODE,
	};

	public: enum class door_options {
		CLOSED_DOOR = 0,
		OPEN_DOOR
	};

	/// <summary>
	/// This function returns the current status of the Vertical activation hardware inputs
	/// 
	/// </summary>	
	/// <returns>the current status request</returns>
	public: static vertical_activation_options getVerticalActivationStatus(void) { return vertical_activation_status; }

	/// <summary>
	/// This function returns the current status of the Body activation hardware inputs
	/// 
	/// </summary>	
	/// <returns>the current status request</returns>
	public: static body_activation_options getBodyActivationStatus(void) { return body_activation_status; }

	
	public: static bool getPowerdown(void) { return power_down_status; }
	public: static door_options getDoorStatus(void) { return door_status; }
	public: static bool getBatteryEna(void) { return battery_enabled_status; }
	public: static bool getBatteryAlarm(void) { return (batt1_low_alarm || batt2_low_alarm); }
	public: static unsigned char getVoltageBatt1(void) { return voltage_batt1; }
	public: static unsigned char getVoltageBatt2(void) { return voltage_batt2; }
	
	public: static bool getXrayStat(void) { return xray_push_button; }
	public: static bool getXrayEventEna(void) { return xray_push_button_event_enable; }
	public: static void setXrayEventEna(bool stat) { xray_push_button_event_enable = stat; }




private:
	static bool xray_push_button_input   = false;	//!< This is the current X-RAY push button activation status
	static bool xray_enable_status_output = false; //!< This is the current X-RAY enable status setting
	static vertical_activation_options vertical_activation_status = vertical_activation_options::VERTICAL_NO_ACTIVATION;		//!< This is the current status of the vertical activation request
	static body_activation_options body_activation_status = body_activation_options::BODY_NO_ACTIVATION;		//!< This is the current status of the body activation request	
	static door_options door_status = door_options::OPEN_DOOR; //!< This is the current status of the Study door
	
	static bool power_down_status = false; // Powerdown Status

	// Battery management
	static bool battery_enabled_status = false; //!< Battery enabled system button status
	static bool batt1_low_alarm = false;		//!< Low voltage of battery 1
	static bool batt2_low_alarm = false;		//!< Low voltage of battery 2
	static unsigned char voltage_batt1 = 0;		//!< 10 * voltage level of battery 1
	static unsigned char voltage_batt2 = 0;		//!< 10 * voltage level of battery 2

	// X-RAY push button handling
	static bool xray_push_button = false; //!> This is the current X-RAY status 
	static bool xray_push_button_event_enable = false; //!> This is the event genration flag
};

