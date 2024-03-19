#pragma once
#include "CanDeviceProtocol.h"

/// <summary>
/// \defgroup PCB304_Module PCB304 Module: Grid device module controller
/// \ingroup CanDevice_Module  
/// </summary>
ref class PCB304 : public CanDeviceProtocol
{
public:

	/// <summary>
	///		This is the Device STATUS Register implementation 
	/// </summary>
	enum class StatusRegisters {
		SYSTEM_STATUS_REGISTER = 0, //!> This is the System Status register index		
	};

	//#define PCB326_GET_SYSTEM_CS1_STATUS(reg)			(bool)	(reg->d0 & 0x01)
	
	/// <summary>
	///	 This is the Device DATA Register implementation 
	/// </summary>
	enum class DataRegisters {
		DISPLAY_DATA_REGISTER = 0, //!> This is the Display Data register index
	};

	#define PCB304_DISPLAY_ON(reg,stat)	reg->D0(stat, 0x1)
	#define PCB304_DISPLAY_BLINK(reg,stat)	reg->D0(stat, 0x2)
	#define PCB304_DISPLAY_REVERSE(reg,stat)	reg->D0(stat, 0x4)
	#define PCB304_DISPLAY_KEEPALIVE(reg,stat)	reg->D0(stat, 0x8)
	#define PCB304_DISPLAY_DOT_POSITION(reg,val)	reg->d0 |= ((unsigned char) val << 4)

	#define PCB304_DISPLAY_INTENSITY(reg,val)	reg->d1 = ((unsigned char) val & 0xF)
	#define PCB304_DISPLAY_CONTENT_LOW(reg, val)	reg->d2 = (unsigned char) (*((unsigned short*)&val)) 
	#define PCB304_DISPLAY_CONTENT_HIGH(reg, val)	reg->d3 = (unsigned char) ((*((unsigned short*)&val)) >> 8) 

	

	PCB304() : CanDeviceProtocol(0x14, L"POTTER_DEVICE")
	{

	}
	static PCB304^ device = gcnew PCB304();

	
	/// <summary>
	/// This function returns the presence of the magnifier device
	/// 
	/// </summary>
	/// <param name=""></param>
	/// <returns>True if the Magnifier device has been detected </returns>
	inline static bool isMagnifierDeviceDetected(void) { return magnifier_device_detected; }
	static System::String^  getMagnifierfactorString(void) { return magnifier_factor_string; }

	/// <summary>
	/// This function set the Grid in Field.
	/// 
	/// The Grid is positioned in field, ready to be used.\n
	/// The Grid is positioned in the center of the travel.
	/// 
	/// See the synchGridWithGenerator() function for details about 
	/// the grid activation.
	/// 
	/// </summary>
	/// <param name=""></param>
	inline static void setGridOnField(void) { grid_on_field = true; }
	inline static void setGridOffField(void) {grid_on_field = false;}

	/// <summary>
	/// This function activates/deactivates the grid synchronizatio with the generator.
	/// 
	/// The Grid device moves the grid in home position ready to start:
	/// as soon as the X-RAY ON signal is detected, the grid device 
	/// starts moving.
	/// 
	/// </summary>
	/// <param name="status">If status == true, the grid shall activate the generator synchronization</param>	
	inline static void synchGridWithGenerator(bool status) { synch_grid = status; };

	/// <summary>
	/// This function returns true if the Grid is correctly positioned in Field.
	/// 
	/// </summary>
	/// <param name=""></param>
	/// <returns>True if the Grid is in field</returns>
	/// 
	inline static bool isGridOnFieldReady(void) { return grid_on_field_ready; }
	inline static bool isGridOffFieldReady(void) { return grid_off_field_ready; }

	/// <summary>
	/// If the grid has been set to Generator Synch,
	/// the function returns true when the grid is ready to start.
	/// 
	/// The ready condition is set when the grid device is positioned \n
	/// at the beginning of the travel (home position) 
	/// 
	/// </summary>
	/// <param name=""></param>
	/// <returns>True if the grid device is in Home position</returns>
	inline static bool isGridSynchReady(void) { return grid_sync_ready; }

	inline static bool isError(void) { return error; }

	inline static bool isPatientProtection(void) { return patient_protection_detected; }
	inline static bool isPatientProtectionShifted(void) { return patient_protection_shifted; }
	
	static void setDisplay(bool on_off) {
		display_on = on_off;
		return;
	}

	static void setDisplay(short val, unsigned char decimals, bool blink, unsigned char intensity) {
		display_on = true;
		display_blink = blink;
		display_val = val;
		display_decimals = decimals;
		display_intensity = intensity;
	}

protected: 
	void runningLoop(void) override;
	void demoLoop(void) override;

private:
	static bool patient_protection_detected = false; //!< Is set if the patient protection should be detcted
	static bool patient_protection_shifted = false; //!< Is set if the patient protection should be shifted out of standard position
	static bool magnifier_device_detected = false; //!< Is set if the Magnifier device has been detected
	static System::String^  magnifier_factor_string = "1.0"; //!< This is the current magnification factor detected in string format
	
	static bool grid_on_field = false; //!< This is the status of the current grid position.
	static bool grid_on_field_ready = false; //!< This flag stands for grid correctly positioned in Field
	static bool grid_off_field_ready = false; //!< This flag stands for grid correctly positioned out of Field

	static bool synch_grid = false; //!< This flag set the current synch status 
	static bool grid_sync_ready = false; //!< This flag stands for grid ready to be activated by the generator
	static bool error = false; //!< The 2D grid is in error condition

	// Remote display control
	static bool display_on = false;
	static bool display_blink = false;
	static short display_val = 0;	
	static unsigned char display_decimals = 0;	
	static unsigned char display_intensity = 0;
	static Register^ display_data_register = gcnew Register();
};

