#pragma once
#include "CanDeviceProtocol.h"

/// <summary>
/// \defgroup PCB301_Module PCB301 Module: power service board module control
/// \ingroup CanDevice_Module 
/// 
/// </summary>
ref class PCB301 :  public CanDeviceProtocol
{

	
	
	
	/// <summary>
	///		This is the Device STATUS Register implementation 
	/// </summary>
	private:enum class StatusRegisters {
		SYSTEM_STATUS_REGISTER = 0, //!> This is the System Status register index
		BATTERY_STATUS_REGISTER		//!> This is the Battery level Status register index
	};
	
	#define PCB301_GET_SYSTEM_BATTENA(reg) (bool) (reg->d0 & 0x80)
	#define PCB301_GET_SYSTEM_BATT1LOW(reg) (bool) (reg->d0 & 0x20)
	#define PCB301_GET_SYSTEM_BATT2LOW(reg) (bool) (reg->d0 & 0x40)
	#define PCB301_GET_SYSTEM_POWERDOWN(reg) (bool) (reg->d0 & 0x04)

	#define PCB301_GET_BUTTON_VERTICAL_UP(reg) (bool) (reg->d1 & 0x1)
	#define PCB301_GET_BUTTON_VERTICAL_DOWN(reg) (bool) (reg->d1 & 0x2)
	#define PCB301_GET_BUTTON_SLIDE_UP(reg) (bool) (reg->d1 & 0x4)
	#define PCB301_GET_BUTTON_SLIDE_DOWN(reg) (bool) (reg->d1 & 0x8)
	#define PCB301_GET_BUTTON_ARM_CW(reg) (bool) (reg->d1 & 0x10)
	#define PCB301_GET_BUTTON_ARM_CCW(reg) (bool) (reg->d1 & 0x20)
	#define PCB301_GET_BUTTON_BODY_CW(reg) (bool) (reg->d1 & 0x40)
	#define PCB301_GET_BUTTON_BODY_CCW(reg) (bool) (reg->d1 & 0x80)

	#define PCB301_GET_PEDAL_VERTICAL_UP(reg) (bool) (reg->d3 & 0x1)
	#define PCB301_GET_PEDAL_VERTICAL_DOWN(reg) (bool) (reg->d3 & 0x2)

	#define PCB301_GET_XRAY_PUSH_BUTTON(reg) (bool) (reg->d2 & 0x08)
	#define PCB301_GET_SYSTEM_CLOSEDOOR(reg) (bool) (reg->d2 & 0x10)


	#define PCB301_GET_BATTERY_VBATT1(reg) (unsigned char) (reg->d0)
	#define PCB301_GET_BATTERY_VBATT2(reg) (unsigned char) (reg->d1)

	/// <summary>
	///	 This is the Device DATA Register implementation 
	/// </summary>
	private:enum class DataRegisters {
		OUTPUTS_DATA_REGISTER = 0, //!> This is the Outputs Data register index
	};
	#define PCB301_OUTPUTS_DATA_BUZZER_STAT(reg,stat)	reg->D1(stat, 0x2) 
	#define PCB301_OUTPUTS_DATA_MANUAL_BUZZER(reg,stat)	reg->D1(stat, 0x4) 

public:

	PCB301(): CanDeviceProtocol(0x10, L"POWER_SERVICE")
	{

	}
	static PCB301^ device = gcnew PCB301();

	



	/// <summary>
	/// This function sets the current status of the X-RAY enable signal.
	///  
	/// </summary>
	/// <param name="status"></param>
	/// <returns></returns>
	public: static inline void setXrayEnable(bool status) { xray_enable_status_output = status; }


	public: enum class door_options {
		CLOSED_DOOR = 0,
		OPEN_DOOR
	};

	
	public: static bool getPowerdown(void) { return power_down_status; }
	public: static door_options getDoorStatus(void) { return door_status; }
	public: static bool getBatteryEna(void) { return battery_enabled_status; }
	public: static bool getBatteryAlarm(void) { return (batt1_low_alarm || batt2_low_alarm); }
	public: static unsigned char getVoltageBatt1(void) { return voltage_batt1; }
	public: static unsigned char getVoltageBatt2(void) { return voltage_batt2; }
	
	public: static bool getXrayPushButtonStat(void) { return xray_push_button; }
	public: static bool getXrayEventEna(void) { return xray_push_button_event_enable; }
	public: static void setXrayEventEna(bool stat) { xray_push_button_event_enable = stat; }

public: 

	inline static bool get_pedal_up_stat(void) { return pedal_up_stat; }
	inline static bool get_pedal_down_stat(void) { return pedal_down_stat; }
	inline static bool get_cmp_up_stat(void) { return cmp_up_stat; }
	inline static bool get_cmp_down_stat(void) { return cmp_down_stat; }
	inline static bool get_button_arm_cw_stat(void) { return button_arm_cw_stat; }
	inline static bool get_button_arm_ccw_stat(void) { return button_arm_ccw_stat; }
	inline static bool get_button_up_stat(void) { return button_up_stat; }
	inline static bool get_button_down_stat(void) { return button_down_stat; }
	inline static bool get_button_body_cw(void) { return button_body_cw; }
	inline static bool get_button_body_ccw(void) { return button_body_ccw; }
	inline static bool get_button_slide_up_stat(void) { return button_slide_up_stat; }
	inline static bool get_button_slide_down_stat(void) { return button_slide_down_stat; }

	inline static void set_manual_buzzer(bool stat) { PCB301_OUTPUTS_DATA_MANUAL_BUZZER(outputs_data_register,stat); }
	inline static void set_activation_buzzer(bool stat) { PCB301_OUTPUTS_DATA_BUZZER_STAT(outputs_data_register, stat); }

	

private:
	

	static Register^ outputs_data_register = gcnew Register(); 

	static bool xray_enable_status_output = false; //!< This is the current X-RAY enable status setting	
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

	static bool pedal_up_stat = false;
	static bool pedal_down_stat = false;
	static bool cmp_up_stat = false;
	static bool cmp_down_stat = false;
	static bool button_arm_cw_stat = false;
	static bool button_arm_ccw_stat = false;
	static bool button_up_stat = false;
	static bool button_down_stat = false;
	static bool button_body_cw = false;
	static bool button_body_ccw = false;
	static bool button_slide_up_stat = false;
	static bool button_slide_down_stat = false;

protected: 	
	    void runningLoop(void) override;

private:
		void handleSystemStatusRegister(void);
		void handleBatteryStatusRegister(void);
};

