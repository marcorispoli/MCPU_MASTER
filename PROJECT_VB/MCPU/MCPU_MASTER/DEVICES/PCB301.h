#pragma once
#include "CanDeviceProtocol.h"
ref class PCB301 :  public CanDeviceProtocol
{
public:
	
	#define PCB301_GET_STATUS_SYSTEM_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
	#define PCB301_GET_SYSTEM_BATTENA(d0,d1,d2,d3) (bool) (d0 & 0x80)
	#define PCB301_GET_SYSTEM_BATT1LOW(d0,d1,d2,d3) (bool) (d0 & 0x20)
	#define PCB301_GET_SYSTEM_BATT2LOW(d0,d1,d2,d3) (bool) (d0 & 0x40)
	#define PCB301_GET_SYSTEM_POWERDOWN(d0,d1,d2,d3) (bool) (d0 & 0x04)
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

private:
	static bool xray_push_button_input   = false;	//!< This is the current X-RAY push button activation status
	static bool xray_enable_status_output = false; //!< This is the current X-RAY enable status setting
};

