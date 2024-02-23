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

	#define PCB301_GET_SYSTEM_ERROR_STATUS(reg)			(bool)	(reg->d0 & 0x01)
	#define PCB301_GET_SYSTEM_EMERGENCY_STATUS(reg)		(bool)	(reg->d0 & 0x02)
	#define PCB301_GET_SYSTEM_POWERDOWN(reg)			(bool)	(reg->d0 & 0x04)
	#define PCB301_GET_SYSTEM_CABINET_SAFETY(reg)		(bool)	(reg->d0 & 0x08)
	#define PCB301_GET_SYSTEM_POWER_OFF_REQ(reg)		(bool)	(reg->d0 & 0x10)
	#define PCB301_GET_SYSTEM_BATT1LOW(reg)				(bool)	(reg->d0 & 0x20)
	#define PCB301_GET_SYSTEM_BATT2LOW(reg)				(bool)	(reg->d0 & 0x40)
	#define PCB301_GET_SYSTEM_BATTENA(reg)				(bool)	(reg->d0 & 0x80)
	
	#define PCB301_GET_BUTTON_VERTICAL_UP(reg)			(bool)	(reg->d1 & 0x1)
	#define PCB301_GET_BUTTON_VERTICAL_DOWN(reg)		(bool)	(reg->d1 & 0x2)
	#define PCB301_GET_BUTTON_SLIDE_UP(reg)				(bool)	(reg->d1 & 0x4)
	#define PCB301_GET_BUTTON_SLIDE_DOWN(reg)			(bool)	(reg->d1 & 0x8)
	#define PCB301_GET_BUTTON_ARM_CW(reg)				(bool)	(reg->d1 & 0x10)
	#define PCB301_GET_BUTTON_ARM_CCW(reg)				(bool)	(reg->d1 & 0x20)
	#define PCB301_GET_BUTTON_BODY_CW(reg)				(bool)	(reg->d1 & 0x40)
	#define PCB301_GET_BUTTON_BODY_CCW(reg)				(bool)	(reg->d1 & 0x80)
	
	#define PCB301_GET_SYSTEM_MOTOR_OK(reg)				(bool) (reg->d2 & 0x01)
	#define PCB301_GET_SYSTEM_MOTOR_SWITCH(reg)			(bool) (reg->d2 & 0x02)
	#define PCB301_GET_SYSTEM_COMPRESSION(reg)			(bool) (reg->d2 & 0x04)
	#define PCB301_GET_XRAY_PUSH_BUTTON(reg)			(bool) (reg->d2 & 0x08)
	#define PCB301_GET_SYSTEM_CLOSEDOOR(reg)			(bool) (reg->d2 & 0x10)
	#define PCB301_GET_SYSTEM_BURNING_JMP(reg)			(bool) (reg->d2 & 0x20)
	#define PCB301_GET_SYSTEM_POWER_LOCK(reg)			(bool) (reg->d2 & 0x40)

	#define PCB301_GET_PEDAL_VERTICAL_UP(reg)			 (bool) (reg->d3 & 0x1)
	#define PCB301_GET_PEDAL_VERTICAL_DOWN(reg)			(bool) (reg->d3 & 0x2)

	


	#define PCB301_GET_BATTERY_VBATT1(reg) (unsigned char) (reg->d0)
	#define PCB301_GET_BATTERY_VBATT2(reg) (unsigned char) (reg->d1)

	/// <summary>
	///	 This is the Device DATA Register implementation 
	/// </summary>
	private:enum class DataRegisters {
		OUTPUTS_DATA_REGISTER = 0, //!> This is the Outputs Data register index
	};
	#define PCB301_OUTPUTS_DATA_POWER_LOCK(reg,stat)				reg->D0(stat, 0x1) 
	#define PCB301_OUTPUTS_DATA_MOTOR_POWER_SUPPLY_ENABLE(reg,stat)	reg->D0(stat, 0x2)
	#define PCB301_OUTPUTS_DATA_MOTOR_SWITCH_ENABLE(reg,stat)		reg->D0(stat, 0x4)
	#define PCB301_OUTPUTS_DATA_COMPRESSOR_ENABLE(reg,stat)			reg->D0(stat, 0x8)
	#define PCB301_OUTPUTS_DATA_COMPRESSOR_CALIBRATION(reg,stat)	reg->D0(stat, 0x10)
	#define PCB301_OUTPUTS_DATA_XRAY_ENA(reg,stat)					reg->D0(stat, 0x20)
	

	#define PCB301_OUTPUTS_DATA_BURNING_STAT(reg,stat)	reg->D1(stat, 0x1) 
	#define PCB301_OUTPUTS_DATA_BUZZER_STAT(reg,stat)	reg->D1(stat, 0x2) 
	#define PCB301_OUTPUTS_DATA_MANUAL_BUZZER(reg,stat)	reg->D1(stat, 0x4) 
	#define PCB301_OUTPUTS_DATA_XRAY_LED(reg,stat)		reg->D1(stat, 0x8) 
	#define PCB301_OUTPUTS_DATA_XRAY_LAMP1(reg,stat)	reg->D1(stat, 0x10) 
	#define PCB301_OUTPUTS_DATA_XRAY_LAMP2(reg,stat)	reg->D1(stat, 0x20) 
	#define PCB301_OUTPUTS_DATA_MAN_ROT_LED(reg,stat)	reg->D1(stat, 0x40) 
	
	#define PCB301_OUTPUTS_DATA_POWER_OFF(reg,stat)		reg->D3(stat, 0x40) 
		#define PCB301_GET_OUTPUTS_DATA_POWER_OFF(reg)	(reg->d3 & 0x40) 
	#define PCB301_OUTPUTS_DATA_KEEP_ALIVE(reg,stat)	reg->D3(stat, 0x80)

public:

	PCB301(): CanDeviceProtocol(0x10, L"POWER_SERVICE")
	{

	}
	static PCB301^ device = gcnew PCB301();


	public: enum class door_options {
		CLOSED_DOOR = 0,
		OPEN_DOOR
	};

public:	
	static inline bool getPowerdown(void) { return power_down_status; }
	static inline bool getEmergency(void) { return emergency_status; }
	static inline bool getCabinetSafety(void) { return cabinet_safety_status; }
	static inline bool getMotorSwitch(void) { return motor_safety_switch; }
	static inline bool getMotorPowerSupply(void) { return motor_48V_ok; }
	static inline bool getCompressionStatus(void) { return compression_on_status; }
	static inline bool getBurningJumper(void) { return burning_jumper_present; }
	static inline bool getSoftPowerOffRequest(void) { return soft_power_off_request; }
	static inline bool getPowerLockStatus(void) { return power_lock_status; }

	static door_options getDoorStatus(void) { return door_status; }
	static bool getBatteryEna(void) { return battery_enabled_status; }
	static bool getBatteryAlarm(void) { return (batt1_low_alarm || batt2_low_alarm); }
	static unsigned char getVoltageBatt1(void) { return voltage_batt1; }
	static unsigned char getVoltageBatt2(void) { return voltage_batt2; }
	
	static bool getXrayPushButtonStat(void) { return xray_push_button; }

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


	// Outputs	
	inline static void set_power_lock(bool stat) { PCB301_OUTPUTS_DATA_POWER_LOCK(outputs_data_register, stat); }
	inline static void set_motor_power_supply(bool stat) { PCB301_OUTPUTS_DATA_MOTOR_POWER_SUPPLY_ENABLE(outputs_data_register, stat); }
	inline static void set_motor_switch(bool stat) { PCB301_OUTPUTS_DATA_MOTOR_SWITCH_ENABLE(outputs_data_register, stat); }
	inline static void set_compressor_ena(bool stat) { PCB301_OUTPUTS_DATA_COMPRESSOR_ENABLE(outputs_data_register, stat); }
	inline static void set_compressor_calib(bool stat) { PCB301_OUTPUTS_DATA_COMPRESSOR_CALIBRATION(outputs_data_register, stat); }
	inline static void set_xray_ena(bool stat) { PCB301_OUTPUTS_DATA_XRAY_ENA(outputs_data_register, stat); }

	inline static void set_manual_buzzer(bool stat) { PCB301_OUTPUTS_DATA_MANUAL_BUZZER(outputs_data_register,stat); }
	inline static void set_activation_buzzer(bool stat) { PCB301_OUTPUTS_DATA_BUZZER_STAT(outputs_data_register, stat); }
	inline static void set_manual_rot_ena_led(bool stat) { PCB301_OUTPUTS_DATA_MAN_ROT_LED(outputs_data_register, stat); }
	inline static void set_power_off(bool stat) { PCB301_OUTPUTS_DATA_POWER_OFF(outputs_data_register, stat); }

	

private:
	

	static Register^ outputs_data_register = gcnew Register(); 
	static door_options door_status = door_options::OPEN_DOOR; //!< This is the current status of the Study door
	
	static bool power_down_status = false;			//!< Current Powerdown Status 
	static bool emergency_status = false;			//!< Current Emergency Status
	static bool cabinet_safety_status = false;		//!< The Cabinet safety input status 
	static bool motor_safety_switch = false;		//!< Safety switch of the 48V status
	static bool motor_48V_ok = false;				//!< Feedback from the motor power supply
	static bool compression_on_status = false;		//!< Actual compression signal
	static bool burning_jumper_present = false;		//!< Burning jumper present in the system
	static bool soft_power_off_request = false;		//!< A power Off sequence is requested
	static bool power_lock_status = false;			//!< The power supply lock condition (for programming)


	// Battery management
	static bool battery_enabled_status = false; //!< Battery enabled system button status
	static bool batt1_low_alarm = false;		//!< Low voltage of battery 1
	static bool batt2_low_alarm = false;		//!< Low voltage of battery 2
	static unsigned char voltage_batt1 = 0;		//!< 10 * voltage level of battery 1
	static unsigned char voltage_batt2 = 0;		//!< 10 * voltage level of battery 2

	// X-RAY push button handling
	static bool xray_push_button = false; //!> This is the current X-RAY status 
	
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
		void demoLoop(void) override;

private:
		void handleSystemStatusRegister(void);
		void handleBatteryStatusRegister(void);
		void toggleKeepalive(void);

		void evaluateEvents(void);
};

