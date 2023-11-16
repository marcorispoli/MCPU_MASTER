#pragma once
#include "CanDeviceProtocol.h"


/// <summary>
/// \defgroup PCB315_Module PCB315 Module: filter selection and tube temperature control
/// This module implements the functions of the Filter device
/// 
/// - Filter selection;
/// - Mirror and collimation light activation; 
/// - Tube temperature monitoring;
/// 
/// The module implements the Device Communication protocol 
/// to communicate with the PCB315 device through the CAN bus.
/// 
/// In order to implement the protocol, the module inherits the 
/// CanDeviceProtocol class as the base class.
/// 
/// The module overrides the configurationLoop() routine of the Base class 
/// in order to writes the initialization Parameters and Data Registers as needed.\n 
/// The routine is automatically called by the Base class after system startup or always after a device reset event.
/// 
/// This module overrides the runningLoop() rotine of the base class in order 
/// to implement the device specific functions.
/// 
/// As soon as the module class is Instantiated, the constructor will call the 
/// Base class constructor.\n
/// The Base class constructor, through the CAN Driver module, tries to connect the 
/// Board Device getting the Bootloader and Application revision codes.
/// 
/// When the Device connection is established and the Board Device firmware revision is acquired,\n
/// the Base class calls the configurationLoop() routine (overridden in this module),\n
/// in order to let the module to upload and store the Device Parameters and the deviice DATA registers.\n
/// In detail, the moudule in the configurationLoop() routine:
/// 
/// - Uploads the Filter assignment parameter;
/// - Uploads the Filters and Mirror calibrated positions;
/// - The Light Timeout
/// 
/// When the configuration fase termines, the module runs the runningLoop() routine (overridden from the Base class),\n
/// to handle the normal workflow.
/// 
///  
/// # Application Interface 
/// 
/// The module provides the following Application interface:
/// 
/// - setFilterAutoMode(): this function sets the Auto mode. The filter is assigned by the Exposure pulse setting
/// - setFilterManualMode(): this function sets the Manual mode. The filter is assigned manually
/// - setMirrorMode(): this function sets the Mirror mode. The Filter device activate the MIRROR for collimation test
/// - getPowerLightStatus(): this function returnrs the current status of the collimation light;
/// - getStator(): this function returnrs the % of the Stator cumulated energy;
/// - getBulb(): this function returnrs the % of the Bulb cumulated energy;
/// - getAnode(): this function returnrs the % of the Anode cumulated energy;
/// - isTubeAlarm(): this function returns the temperature alarm status;
/// - getFilterFromTag(): this function returns the Filter selection code from the String code;
/// - getTagFromFilter(): this function returns the fiter name (tag) of the Filter selection code;
/// 
/// # Filter and Mirror Selection management
/// 
/// The module allows the Application to select one of the following filter operating mode
/// 
/// - AUTO-FILTER mode;
/// - MANUAL-FILTER mode; 
/// - MIRROR mode;
/// 
///
/// ## AUTO-FILTER Mode
/// 
/// The Auto filter mode should be selected for the Exposure settings.
/// 
/// If the Application should select the AUTO Filter mode,\n
///	the Module will force the Filter device to select the FILTER assigned to the Auto mode.
/// 
/// The module will try at least five times to set the requested filter, repeating the command 
/// in case of selection should fail. \n
/// If all the attempts should fail a non resettable error condition will be activated and 
/// the Filter selection will be disabled.
/// 
/// The Application can select the Auto Mode in two different ways:
/// - setFilterAutoMode(filter): activates the Auto mode, selecting what filter shall be used in Auto mode;
/// - setFilterAutoMode(): activates the AUTO mode but making use of the already selected Auto filter;
/// 
/// The function with the filter passed as the parameter should be used by the Module that is responsible\n
/// to set the exposure filter;
/// 
/// The function without the parameter should be used by the module controlling the exposure workflow,\n
/// in order to be sure that the Auto filter (already selected) be the actual filter (nor the mirror or manual filter).
/// 
/// 
/// ## MANUAL-FILTER Mode
/// 
/// If the Application should select the MANAUL Filter mode,\n
///	the Module will force the Filter device to select the FILTER assigned to the Manual mode.
/// 
/// The module will try at least five times to set the requested filter, repeating the command 
/// in case of selection should fail. \n
/// If all the attempts should fail a non resettable error condition will be activated and 
/// the Filter selection will be disabled.
/// 
/// A sthe Auto mode, also the manual mode can be selected with two functions:
/// - setFilterManualMode(filter): activates the manual mode, selecting what filter shall be used in Manual mode;
/// - setFilterManaulMode(): activates the Manual mode but making use of the already selected Manaul filter;
/// 
/// The purpose of the Manual mode is to override any Auto filter in case of calibration/Test scenario.
/// 
/// 
/// ## MIRROR Mode
/// 
/// If the Application should select the MIRROR mode,\n
///	the Module will force the Filter device to select the MIRROR.
/// 
/// When the Mirror is selected the power light automatically will be activated
/// for a given timeout.
/// 
///		NOTE: the current Manual filter and Auto Filter are not overridden.
/// 
/// The Application can activate the Mirror mode with the following interface function:
/// - setMirrorMode();
/// 
/// The Mirror mode should be activated by the Application when the collimation light shall be used 
/// in order to help the patient positioning during compression\n
/// or when a collimation test shall be performed.
/// 
/// 
/// ## FILTER SELECTION CODES AND TAGS
/// 
/// The Application can select one of the following Filters:
/// - Ag;
/// - Rh;
/// - Al;
/// - Cu;
/// - Mo;
///
/// The selection can be made either by String name or by Enumeration code.
/// 
/// + Enumeration codes: see the filterMaterialCodes  enum class definition;
/// + The String names (used by the AWS interface) are:
///		- "Ag";
///		- "Rh";
///		- "Al";
///		- "Cu";
///		- "Mo";
///
/// 
/// - usually the AWS module uses the filter string name (see the AWS protocol specification);
/// - usually the internal modules uses the filterMaterialCodes enumeration class;
/// 
/// There are two convenient functions to convert the code into its respective name and vice versa:
/// - getFilterFromTag(String name): returns the filter code from the Filter string name;
/// - getTagFromFilter(filterMaterialCodes filter_code): returns the string name from the filter code.
/// 
/// ## FILTER ASSIGNMENT TO THE FILTER SLOT
/// 
/// The Filter Device can select only four possible slots.\n
/// The name of the Slots are FILTER1, FILTER2, FILTER3, FILTER4.\n
/// 
/// The phisical positions of the slots are described in the Hardware specification document of the PCB315 board.
/// 
/// The FilterConfig file assignes the slots to the Material Filters.
/// 
/// For example, it can be assigned:
/// - The Filter "Ag" to the FILTER1 slot;
/// - The Filter "Al" to the FILTER2 slot;
/// - The Filter "Rh" to the FILTER3 slot;
/// - The Filter "Cu" to the FILTER4 slot;
/// In that example, the filter "Mo" shall be NOT assigned to prevent errors even if it can be reassigned to any of the possible slots.
/// 
/// See the FilterConfig class section for details.
/// 
/// # Tube Temperature Monitoring
/// 
/// The Filter device reads the Tube internal sensors positioned near the Rotating Anode Stator (Stator sensor) and the 
/// Cathode Filament (Bulb sensor) .\n
/// Both Cathode Filament and Anode Stator are equipped with internal thermocouple protections\n
/// that open at about 105 C, to protect the integrity of the X-RAY Tube.
/// 
/// The firmware of the PCB315 board will monitor the sensors in order to prevent the hardware activation\n
/// of the internal protection (105C), protecting the X-Ray tube from early aging or longer \n
/// stop due to the protection reset required time. 
/// 
/// The Module periodically acquires the current temperature value of the sensors and provides \n
/// its value to the Application in percent respect the maximum allowable temperature:
/// - 0%: the temperature is < 20C;
/// - 100%: the temperature is >= 105C
/// 
/// The Module monitors the sensor values detecting possible fault conditions:
/// - Low Fault condition: the sensor value is < 0C (possible sensor disconnected condition)
/// - Short Fault condition: the sensor value is > 120C (possible sensor or cable shorted)
///		NOTE: in the case of faults, the percentage of the sensor is set to 0%!
/// 
/// In case the monitored temperature should exceed the 90C (but lower than 120C)\n
/// a High Temperature Warning is reported.
///		IMPORTANT: this Warning is not a Fault, because it should depends by an intense use of the X-RAY Tube.
/// 
/// # FANCOIL control
/// 
/// ... To be implemented
/// 
/// </summary>

/// <summary>
/// \defgroup PCB315_Protocol Protocol Implementation
/// \ingroup PCB315_Module
/// This section implements the internal module data and structures in order to 
/// implement the protocol detail specifications related to the PCB315 board. 
/// 
/// 
/// </summary>



/// \defgroup PCB315_Internal Internal implementation section
/// \ingroup PCB315_Module

/// \defgroup PCB315_Interface Application Interface
/// \ingroup PCB315_Module

/// <summary>
/// This class implements the functions of the PCB315 Module
/// \ingroup PCB315_Module
/// 
/// </summary>
ref class PCB315 : public CanDeviceProtocol
{
	/// \ingroup PCB315_Protocol	
	/// 
	///@{
	
	#define PCB315_ERROR_BULB_LOW(reg)	(reg->d2 & 0x1) //!< This is the Bulb sensor low level
	#define PCB315_ERROR_BULB_SHORT(reg)	(reg->d2 & 0x2) //!< This is the Bulb sensor connetions open 
	#define PCB315_ERROR_BULB_HIGH(reg)	(reg->d2 & 0x4) //!< This is the Bulb sensor high temperature
	#define PCB315_ERROR_STATOR_LOW(reg)	(reg->d2 & 0x8) //!< This is the Bulb sensor low level
	#define PCB315_ERROR_STATOR_SHORT(reg)	(reg->d2 & 0x10) //!< This is the Bulb sensor connetions open 
	#define PCB315_ERROR_STATOR_HIGH(reg)	(reg->d2 & 0x20) //!< This is the Bulb sensor high temperature
	#define PCB315_ERROR_FILTER_SELECTION(reg)	(reg->d2 & 0x40) //!< This is the Filter selection error



	/// <summary>
	/// This is the Device STATUS Register implementation 
	/// </summary>
	private:enum class StatusRegisters {
		SYSTEM_STATUS_REGISTER = 0, //!> This is the System Status register index
	};

	/// <summary>
	/// This enumeration class defines the codes associated to the 
	/// device internal Slot selection code.
	/// 
	/// </summary>
	private:enum class FilterSlotCodes {
		FILTER_OUT_OF_POSITION = 0, //!< The Filter is not in an expected position. (error condition)
		FILTER_INVALID = 0,	//!< The Filter selected is invalid (usually after the startup)
		FILTER1_SELECTION,	//!< The Device is currently selecting the FILTER1 
		FILTER2_SELECTION,	//!< The Device is currently selecting the FILTER2 
		FILTER3_SELECTION,	//!< The Device is currently selecting the FILTER3 
		FILTER4_SELECTION,	//!< The Device is currently selecting the FILTER4 
		MIRROR_SELECTION,	//!< The Device is currently selecting the MIRROR 
		FILTER_SELECTION_PENDING //!< The Device is selecting a Slot (command is executing)
	};

	private:

	// Status registers	
	#define PCB315_GET_SYSTEM_FILTER(reg) (unsigned char) (reg->d0) //!< This macro returns the FILTER byte of the SYSTEM register
	#define PCB315_GET_SYSTEM_STATOR(reg) (unsigned char) (reg->d1) //!< This macro returns the STATOR byte of the SYSTEM register
	#define PCB315_GET_SYSTEM_BULB(reg) (unsigned char) (reg->d2)	//!< This macro returns the BULB byte of the SYSTEM register
	#define PCB315_GET_SYSTEM_FLAGS(reg) (unsigned char) (reg->d3)	//!< This macro returns the FLAGS byte of the SYSTEM register

	#define PCB315_GET_LIGHT_FLAG(flags) ((flags) & 0x1)//!< This macro returns the Light On flag 
	#define PCB315_GET_ERROR_FLAG(flags) ((flags) & 0x80)//!< This macro returns the status of the Error present condition

	/// <summary>
	/// This is the device protocol Parameters registers implementation 
	/// </summary>
	private:enum class ParamRegisters {
		FILTER1_PARAM_REGISTER = 0, //!< um unit of FILTER1 calibration 
		FILTER2_PARAM_REGISTER,		//!< um unit of FILTER2 calibration 
		FILTER3_PARAM_REGISTER,		//!< um unit of FILTER3 calibration 
		FILTER4_PARAM_REGISTER,		//!< um unit of FILTER4 calibration 
		MIRROR_PARAM_REGISTER,		//!< um unit of MIRROR calibration 
		POWERLIGHT_PARAM_REGISTER,  //!< Light activation timeout in seconds (s)

	};

private:

	// Parameters
	#define PCB315_WRITE_FILTER1_REGISTER(val) (System::Byte) ParamRegisters::FILTER1_PARAM_REGISTER,(System::Byte) (val&0xFF), (System::Byte) ((val>>8)&0xFF),(System::Byte) 0, (System::Byte) 0
	#define PCB315_WRITE_FILTER2_REGISTER(val) (System::Byte) ParamRegisters::FILTER2_PARAM_REGISTER,(System::Byte) (val&0xFF), (System::Byte) ((val>>8)&0xFF),(System::Byte) 0, (System::Byte) 0
	#define PCB315_WRITE_FILTER3_REGISTER(val) (System::Byte) ParamRegisters::FILTER3_PARAM_REGISTER,(System::Byte) (val&0xFF), (System::Byte) ((val>>8)&0xFF),(System::Byte) 0, (System::Byte) 0
	#define PCB315_WRITE_FILTER4_REGISTER(val) (System::Byte) ParamRegisters::FILTER4_PARAM_REGISTER,(System::Byte) (val&0xFF), (System::Byte) ((val>>8)&0xFF),(System::Byte) 0, (System::Byte) 0
	#define PCB315_WRITE_MIRROR_REGISTER(val) (System::Byte) ParamRegisters::MIRROR_PARAM_REGISTER,(System::Byte) (val&0xFF), (System::Byte) 0,(System::Byte) 0, (System::Byte) 0
	#define PCB315_WRITE_POWERLIGHT_REGISTER(val) (System::Byte) ParamRegisters::POWERLIGHT_PARAM_REGISTER,(System::Byte) (val&0xFF), (System::Byte) 0,(System::Byte) 0, (System::Byte) 0

	/// <summary>	
	/// This enumeration class defines the Indexes of the Command Execution
	///
	/// </summary>
	private: enum class Commandregister {
		ABORT_COMMAND = 0, //!< Abort Command (mandatory as for device protocol)
		SET_POSITIONER_COMMAND,//!< Select Filter or Mirror devices 
		SET_POSITIONER_RAW_COMMAND,//!< To be done
		SET_LIGHT,//!< Activates/Deactivate the light
	};

	private: enum class PositionerCommands {
		POSITIONER_SELECT_FILTER1 = 1,	//!< The Positioner select the Filter1 slot
		POSITIONER_SELECT_FILTER2 ,		//!< The Positioner select the Filter2 slot
		POSITIONER_SELECT_FILTER3,		//!< The Positioner select the Filter3 slot
		POSITIONER_SELECT_FILTER4,		//!< The Positioner select the Filter4 slot
		POSITIONER_SELECT_MIRROR,		//!< The Positioner select the Filter4 slot
	};

	private: enum class LightCommands {
		LIGHT_OFF = 0,					//!< The power light shall be set OFF
		LIGHT_ON,						//!< The power light shall be set ON (with internal predefined timeout)
	};
	
private:
	#define PCB315_SET_POSITIONER_COMMAND(cmd) (System::Byte) Commandregister::SET_POSITIONER_COMMAND, (System::Byte) (cmd),(System::Byte) 0,(System::Byte) 0,(System::Byte) 0 //!< This is the SET_POSITIONER_COMMAND byte frame
	#define PCB315_SET_LIGH_COMMAND(cmd) (System::Byte) Commandregister::SET_LIGHT, (System::Byte) (cmd),(System::Byte) 0,(System::Byte) 0,(System::Byte) 0 //!< This is the SET_POSITIONER_COMMAND byte frame
	///@}


	/// <summary>
	///  PCB315 Class constructor.
	/// 
	/// The construction initializes several internal variables.
	/// The constructor call the base class setting the address of the controlled device.
	/// 
	/// As soon as the base class is initialized, the communication process starts
	/// and the device is initialized with configurationLoop() routine.
	/// \ingroup PCB315_Interface
	/// </summary>
	public:PCB315() : CanDeviceProtocol(0x13, L"FILTER_DEVICE")
	{

	}

	/// <summary>
	/// This is the enumeration class defining the possible selectable filters
	/// 
	/// The FILTER_DEFAULT is a special selection code that\n 
	/// causes a selection of a filter defined into the FilterCalibraiton file.
	/// 
	/// The Module set this value at the system startup to provide an initial 
	/// Filter selection.
	/// 
	/// The Application should use this selection whenever an initial state should be applied.
	/// 
	///  \ingroup PCB315_Interface
	/// </summary>
	public: enum class filterMaterialCodes {		
		FILTER_DEFAULT=0, //!< This is a default filter set in the FilterCalibration configuration file 
		FILTER_AG, //!< Ag Application selection option
		FILTER_AL, //!< Al Application selection option
		FILTER_MO, //!< Mo Application selection option
		FILTER_RH, //!< Rh Application selection option
		FILTER_CU, //!< Cu Application selection option		
		FILTER_LEN,//!< Reserved for internal usage
		FILTER_INVALID = FILTER_LEN,//!< Reserved for internal usage
	};
	
	/// <summary>
	/// This enumeration class defines the possibles filter working modes
	/// 
	/// \ingroup PCB315_Interface
	/// </summary>
	public: enum class filterWorkingMode {
		FILTER_AUTO_MODE = 0, //!< The filter is selected by the aws for the exposure mode
		FILTER_MANUAL_MODE,//!< The filter is selected manually
		FILTER_MIRROR_MODE //!< The Mirror shall be selected
	};
	
	/// \ingroup PCB315_Internal
	///@{ 
	
	private: static filterWorkingMode filter_working_mode = filterWorkingMode::FILTER_MANUAL_MODE; //!< This is the current Filter working mode
	private: static FilterSlotCodes auto_filter_selected = FilterSlotCodes::FILTER_INVALID; //!< This is the current filter automatically selected
	private: static FilterSlotCodes manual_filter_selected = FilterSlotCodes::FILTER_INVALID; //!< This is the current filter automatically selected	
	private: static FilterSlotCodes filterAssignment(System::String^ assignment); //!< This function traslates the assignment parameter value (in string format) to a FilterSlotCodes related code 

	private: static bool valid_filter_format; //!< This flag is set when the filter format is correct and coherent with the filterWorkingMode register
	private: static bool filter_fault; //!< This flag is set when the filter is in fault condition
	private: static int  filter_selection_attempt; //!< This register counts the attempt to exit from a fault condition
	private: static FilterSlotCodes  filter_status = FilterSlotCodes::FILTER_OUT_OF_POSITION; //!< last STATUS filter status read
	private: static unsigned char flags_status = 0; //!< This is the last read of the Flags status
	private: static bool request_light_activation = false; //!< A light activation request is pending
	
	protected: 	void runningLoop(void) override; //!< This is the Base class runningLoop override function
	protected: 	bool configurationLoop(void) override;//!< This is the Base class configurationLoop override function
	
	// Tube Monitoring
	private: static unsigned char stator_perc = 0;//!< This is the current stator temperature in percent of the maximum possible
	private: static unsigned char bulb_perc = 0;//!< This is the current bulb temperature in percent of the maximum possible
	private: static unsigned char anode_perc = 0;//!< This is the current anode temperature in percent of the maximum possible
	private: static bool tube_high_temp_alarm = false;//!< This is the current tube temperature alarm flag in case of fault detected

	// Macro function reading registers
	private: bool updateStatusRegister(void); //!< Function handling the status register acquisition
	private: void manageFilterSelection(void);//!< Function handling the Filter selection operations
	
	///@}

public: 

	/// \ingroup PCB315_Interface
	///@{
	
	/// <summary>
	/// This function returns the current Stator temperature in Percent
	/// 
	/// The Percent value is calculated:
	/// - 0%: the temperature is < 20°;
	/// - 100%: the temperature is >= 105°
	/// 
	///		Note: in case of fault, this value is 0
	/// 
	/// </summary>
	/// <param name=""></param>
	/// <returns>The percent of the Stator temperature</returns>
	inline static unsigned char getStator(void) { return stator_perc; }

	/// <summary>
	/// This function returns the current Bulb temperature in Percent
	/// 
	/// The Percent value is calculated:
	/// - 0%: the temperature is < 20°;
	/// - 100%: the temperature is >= 105°
	/// 
	///		Note: in case of fault, this value is 0
	/// 
	/// </summary>
	/// <param name=""></param>
	/// <returns>The percent of the Bulb temperature</returns>
	inline static unsigned char getBulb(void) { return bulb_perc; }

	/// <summary>
	/// This function returns the current Anode cumulated HU in percent
	/// 
	/// </summary>
	/// <param name=""></param>
	/// <returns>The percent of the Anode cumulated HU</returns>
	inline static unsigned char getAnode(void) { return anode_perc; }

	/// <summary>
	/// This function provides a quick info about the Temperature alarm active 
	/// irrespective of which alarm is actually present (stator, bulb or anode)
	/// 
	/// /// </summary>
	/// <param name=""></param>
	/// <returns>The alarm condition due to temperature</returns>	
	inline static bool isTubeAlarm(void) { return tube_high_temp_alarm; }

	
	static filterMaterialCodes getFilterFromTag(System::String^ filter_tag); //!< Returns the Filter selection code
	
	static System::String^ getTagFromFilter(filterMaterialCodes filter); //!< Returns the Filter name

	static void setFilterAutoMode(filterMaterialCodes code); //!< This function sets the Auto mode, assigning the filter to be selected
	static void setFilterAutoMode(void); //!< This function acctivates the auto mode: the selected filter has been already set.
	static void setFilterManualMode(filterMaterialCodes code); //!< This function activates the manaul mode assigning the filter to be selected
	static void setFilterManualMode(void); //!< This function activates the manaul mode: the filter shall be already set
	static void setMirrorMode(bool request_power_light); //!< This function activates the Mirror Mode and the light (if requested)

	/// <summary>
	/// This function returns the current status of the Power light 
	/// </summary>
	/// <param name=""></param>
	/// <returns>true if the power light is currently ON</returns>
	inline static bool getPowerLightStatus(void) { return PCB315_GET_LIGHT_FLAG(flags_status); }

	

	///@}

};

