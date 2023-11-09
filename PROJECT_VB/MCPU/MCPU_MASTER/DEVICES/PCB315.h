#pragma once
#include "CanDeviceProtocol.h"

ref class PCB315 : public CanDeviceProtocol
{
public:

	enum class StatusRegisters {
		SYSTEM_STATUS_REGISTER = 0,
	};

	enum class ParamRegisters {
		FILTER1_PARAM_REGISTER = 0,
		FILTER2_PARAM_REGISTER,
		FILTER3_PARAM_REGISTER,
		FILTER4_PARAM_REGISTER,
		MIRROR_PARAM_REGISTER,
		POWERLIGHT_PARAM_REGISTER,

	};

	enum class FilterStatus {
		FILTER_OUT_OF_POSITION = 0,
		FILTER1_SELECTED,
		FILTER2_SELECTED,
		FILTER3_SELECTED,
		FILTER4_SELECTED,
		MIRROR_SELECTED,
		FILTER_SELECTION_PENDING
	};

	// Status registers
	#define PCB315_GET_STATUS_SYSTEM_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) StatusRegisters::SYSTEM_STATUS_REGISTER, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
	#define PCB315_GET_SYSTEM_FILTER(d0,d1,d2,d3) (unsigned char) (d0)
	#define PCB315_GET_SYSTEM_STATOR(d0,d1,d2,d3) (unsigned char) (d1)
	#define PCB315_GET_SYSTEM_BULB(d0,d1,d2,d3) (unsigned char) (d2)
	#define PCB315_GET_SYSTEM_FLAGS_LIGHT_ON(d0,d1,d2,d3) ((unsigned char) (d3) & 0x1)

	// Parameters
	#define PCB315_WRITE_FILTER1_REGISTER(val) (System::Byte) 0, (System::Byte) 8,(System::Byte) ParamRegisters::FILTER1_PARAM_REGISTER,(System::Byte) (val&0xFF), (System::Byte) ((val>>8)&0xFF),(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
	#define PCB315_WRITE_FILTER2_REGISTER(val) (System::Byte) 0, (System::Byte) 8,(System::Byte) ParamRegisters::FILTER2_PARAM_REGISTER,(System::Byte) (val&0xFF), (System::Byte) ((val>>8)&0xFF),(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
	#define PCB315_WRITE_FILTER3_REGISTER(val) (System::Byte) 0, (System::Byte) 8,(System::Byte) ParamRegisters::FILTER3_PARAM_REGISTER,(System::Byte) (val&0xFF), (System::Byte) ((val>>8)&0xFF),(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
	#define PCB315_WRITE_FILTER4_REGISTER(val) (System::Byte) 0, (System::Byte) 8,(System::Byte) ParamRegisters::FILTER4_PARAM_REGISTER,(System::Byte) (val&0xFF), (System::Byte) ((val>>8)&0xFF),(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
	#define PCB315_WRITE_MIRROR_REGISTER(val) (System::Byte) 0, (System::Byte) 8,(System::Byte) ParamRegisters::MIRROR_PARAM_REGISTER,(System::Byte) (val&0xFF), (System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false
	#define PCB315_WRITE_POWERLIGHT_REGISTER(val) (System::Byte) 0, (System::Byte) 8,(System::Byte) ParamRegisters::POWERLIGHT_PARAM_REGISTER,(System::Byte) (val&0xFF), (System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false

	// COMMANDS: CODE, DATA0, DATA1, DATA2, DATA3
	#define PCB315_COMMAND_LIGHT_ON (System::Byte) 1, (System::Byte) 5,(System::Byte) 0,(System::Byte) 0,(System::Byte) 0

	PCB315() : CanDeviceProtocol(0x13, L"FILTER_DEVICE")
	{

	}

	protected: 	void runningLoop(void) override;
	protected: 	bool configurationLoop(void) override;

	public: inline static bool getPowerLightStatus(void) { return power_light_status; }
	public: inline static unsigned char getStator(void) { return stator_perc; }
	public: inline static unsigned char getBulb(void) { return bulb_perc; }
	public: inline static unsigned char getAnode(void) { return anode_perc; }
	public: inline static unsigned char getTubeMaxCumulated(void) { return cumulated_perc; }
	public: inline static bool isTubeAlarm(void) { return tube_alarm; }

	public: void setAnodeHu(unsigned char ahu);

private:
	static bool power_light_status = false;
	static FilterStatus  filter_status = FilterStatus::FILTER_OUT_OF_POSITION;

	// Tube Monitoring
	static unsigned char stator_perc = 0;
	static unsigned char bulb_perc = 0;
	static unsigned char anode_perc = 0;
	static unsigned char cumulated_perc = 0;
	static bool tube_alarm = false;

	// Macro function reading registers
	void updateStatusRegister(void);
};

