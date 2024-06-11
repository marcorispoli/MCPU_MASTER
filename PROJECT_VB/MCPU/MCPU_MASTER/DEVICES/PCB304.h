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
	#define PCB304_GET_GRID_SYNC_STATUS(reg)	(bool)	(reg->d0 & 0x01)

	//#define PCB326_GET_SYSTEM_CS1_STATUS(reg)			(bool)	(reg->d0 & 0x01)
	
	/// <summary>
	///	 This is the Device DATA Register implementation 
	/// </summary>
	enum class DataRegisters {
		DISPLAY_DATA_REGISTER = 0,	//!> This is the Display Data register index
		GRID_DATA_REGISTER			//!< This is the control of the Grid status
	};

	#define PCB304_DISPLAY_ON(reg,stat)	reg->D0(stat, 0x1)
	#define PCB304_DISPLAY_BLINK(reg,stat)	reg->D0(stat, 0x2)
	#define PCB304_DISPLAY_REVERSE(reg,stat)	reg->D0(stat, 0x4)
	#define PCB304_DISPLAY_KEEPALIVE(reg,stat)	reg->D0(stat, 0x8)
	#define PCB304_DISPLAY_DOT_POSITION(reg,val)	reg->d0 |= ((unsigned char) val << 4)

	#define PCB304_DISPLAY_INTENSITY(reg,val)	reg->d1 = ((unsigned char) val & 0xF)
	#define PCB304_DISPLAY_CONTENT_LOW(reg, val)	reg->d2 = (unsigned char) (*((unsigned short*)&val)) 
	#define PCB304_DISPLAY_CONTENT_HIGH(reg, val)	reg->d3 = (unsigned char) ((*((unsigned short*)&val)) >> 8) 

	#define PCB304_GRID_SYNC(reg,stat)	reg->D0(stat, 0x1) // Sets the Synch Grid Bit

	PCB304() : CanDeviceProtocol(0x14, L"POTTER_DEVICE")
	{
		simulInit();
	}
	static PCB304^ device = gcnew PCB304();

	
	
	
	static bool setGridOnField(bool wait_completion);
	static bool setGridOffField(bool wait_completion);

	/// <summary>
	/// This function activates/deactivates the grid synchronizatio with the generator.
	/// 
	/// The Grid device moves the grid in home position ready to start:
	/// as soon as the START_GRID signal is detected, the grid device 
	/// starts moving.
	/// 
	/// </summary>
	/// <param name="status">If status == true, the grid shall activate the generator synchronization</param>	
	inline static void synchGridWithGenerator(bool status) { PCB304_GRID_SYNC(grid_data_register, status); }

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
	static Register^ grid_data_register = gcnew Register();


	// Simulation Section
	enum class simul_rx_struct {
		STX = 0,
		LENGHT,
		DEVICE_ID,

		ENDFRAME,
		BUFLEN
	};

	enum class simul_tx_struct {
		STX = 0,
		LENGHT,
		DEVICE_ID,
		DISPLAY_D0,
		DISPLAY_D1,
		DISPLAY_D2,
		DISPLAY_D3,
		GRID_D0,
		GRID_D1,
		GRID_D2,
		GRID_D3,
		ENDFRAME,
		BUFLEN
	};

	void simulInit(void);
	static void simulRx(cli::array<System::Byte>^ receiveBuffer, int index, int rc);
	void simulSend(void);
	cli::array<System::Byte>^ from_simulator;
	cli::array<System::Byte>^ to_simulator;
	cli::array<System::Byte>^ to_simulator_previous;

};

