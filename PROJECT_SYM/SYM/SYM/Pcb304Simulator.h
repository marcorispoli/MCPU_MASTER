#pragma once

using namespace System;



ref class PCB304Simulator
{
public:	
	

	// Simulation Section
	enum class simul_tx_struct {
		STX = 0,
		LENGHT,
		DEVICE_ID,
		
		ENDFRAME,
		BUFLEN
	};

	enum class simul_rx_struct {
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

	enum class command_codes {
		ABORT_COMMAND = 0, 
		
	};

	enum class simul_command_struct {
		STX = 0,
		LENGHT,
		DEVICE_ID,
		CODE,
		D0,
		D1,
		D2,
		D3,
		ENDFRAME,
		BUFLEN
	};

	PCB304Simulator() {
		
		to_master = gcnew cli::array<Byte>((int)simul_tx_struct::BUFLEN);
		to_master_previous = gcnew cli::array<Byte>((int)simul_tx_struct::BUFLEN);
		from_master = gcnew cli::array<Byte>((int)simul_rx_struct::BUFLEN);
		board_initialization();
	}
	static PCB304Simulator^ device = gcnew PCB304Simulator;


	static void handle_reception(cli::array<Byte>^ receiveBuffer, int index, int len) {
		if (receiveBuffer[0] == 0x3) {

			// DATA REGISTER FROM MASTER
			if (len != (int)simul_rx_struct::BUFLEN) return;
			for (int i = 0; i < len; i++) from_master[i] = receiveBuffer[index + i];
		}
		else {
			// COMMAND FROM MASTER
			handle_commands(receiveBuffer);
		}


	};

	static void handle_connection(bool stat) {

		// Re-initialize the board
		board_initialization();
		if (!board_elaboration()) send();

	};
	
	
	static void board_initialization(void) {
		
		for (int i = 0; i < (int) simul_rx_struct::BUFLEN; i++) {
			from_master[i] = 0;
		}

		for (int i = 0; i < (int)simul_tx_struct::BUFLEN; i++) {
			to_master[i] = 0;
		}

		to_master[0] = 0x3;
		to_master[1] = (int)simul_tx_struct::BUFLEN;
		
		to_master[(int)simul_tx_struct::ENDFRAME] = 0x2;
	}

	/// <summary>
	/// This function manage the internal and external signals in order to set the Internal device.
	/// 
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static bool board_elaboration(void) {
		
		// Display format 
		display_on = (from_master[(int)simul_rx_struct::DISPLAY_D0] & 0x1) ? true : false;
		display_blink = (from_master[(int)simul_rx_struct::DISPLAY_D0] & 0x2) ? true : false;
		display_reverse = (from_master[(int)simul_rx_struct::DISPLAY_D0] & 0x4) ? true : false;
		display_dots = (from_master[(int)simul_rx_struct::DISPLAY_D0] >> 4) & 0x03;
		display_intensity = (from_master[(int)simul_rx_struct::DISPLAY_D1] ) & 0x0F;
		display_val = from_master[(int)simul_rx_struct::DISPLAY_D2] + 256 * from_master[(int)simul_rx_struct::DISPLAY_D3];

		
		bool changed = false;
		for (int i = 0; i < to_master->Length; i++) {
			if (to_master[i] != to_master_previous[i]) {
				changed = true;
				to_master_previous[i] = to_master[i];
			}
		}

		if (changed) send();
		return changed;
	}

	static void handle_commands(cli::array<Byte>^ receiveBuffer) {

		

	}

	// To Master	
	static cli::array<Byte>^ to_master;
	static cli::array<Byte>^ to_master_previous;
	static cli::array<Byte>^ from_master;

	// Internal devices

	// Display device
	static bool display_blink = false;
	static bool display_on = false;
	static bool display_reverse = false;
	static int  display_intensity = 0;
	static int  display_dots = 0;
	static int  display_val = 0;

	// External Inputs
	static bool generator_grid_start = false; //!< This signal should be set by the generator board (bus hardware)

	// External Outputs
	static bool generator_grid_disable = false; //!< This signal should be set to the generator (bus hardware)

private:

	static void send(void) {
		
	};
};

