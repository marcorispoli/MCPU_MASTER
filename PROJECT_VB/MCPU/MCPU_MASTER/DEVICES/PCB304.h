#pragma once
#include "CanDeviceProtocol.h"

/// <summary>
/// \defgroup PCB304_Module PCB304 Module: Grid device module controller
/// \ingroup CanDevice_Module  
/// </summary>
ref class PCB304 : public CanDeviceProtocol
{
public:

#define PCB304_GET_STATUS_SYSTEM_REGISTER (System::Byte) 0, (System::Byte) 4,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0,(System::Byte) 0, (System::Byte) 0,(System::Byte) 0, false



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
	inline static void setGridOnField(void) {};

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

protected: 	virtual void runningLoop(void) override;

private:
	static bool magnifier_device_detected = false; //!< Is set if the Magnifier device has been detected
	static bool grid_on_field = false; //!< This is the status of the current grid position.
	static bool grid_on_field_ready = false; //!< This flag stands for grid correctly positioned in Field
	static bool synch_grid = false; //!< This flag set the current synch status 
	static bool grid_sync_ready = false; //!< This flag stands for grid ready to be activated by the generator
	static bool error = false; //!< The 2D grid is in error condition

};

