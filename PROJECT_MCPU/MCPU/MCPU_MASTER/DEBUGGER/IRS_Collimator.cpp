#include "Debugger.h"
#include "Log.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB304.h"
#include "PCB326.h"
#include "TiltMotor.h"
#include "ArmMotor.h"
#include "VerticalMotor.h"
#include "BodyMotor.h"
#include "SlideMotor.h"
#include "ExposureModule.h"
#include <thread>

void DebuggerCLI::CollimatorCommandList(void) {
	// Shows the list of available commands if no valid command is detedcted
	System::String^ lista;
	lista = " -> RESET - reset Board\n\r";
	lista += " -> GET_STATUS            - Shows the protocol registers\n\r";
	lista += " -> SELECT_FILTER_SLOT    - Filter selection by slot number\n\r";
	lista += " -> SELECT_FILTER_MATTER	- Filter selection by Matter\n\r";
	lista += " -> SET_FAN	            - Fan Activation Mode \n\r";

	lista += " \r\n --- 2D Format Collimation Commands ----------------------------------------- \n\r \r\n";
	lista += " -> FORMAT_MODE	     - Set the current format collimation handling mode \n\r";
	lista += " -> FORMAT_SET_SLOT	 - Set the collimation data of a given slot \n\r";
	lista += " -> FORMAT_STORE_SLOT	 - Stores the slot's format data in the configuration file \n\r";
	lista += " -> FORMAT_SELECT_SLOT - Select the collimation slot \n\r";


	send(System::Text::Encoding::Unicode->GetBytes(lista));
	return;
}

void DebuggerCLI::handle2DFormatCommands(System::String^ cmd) {
	System::String^ stringa;

	if (cmd->Contains("FORMAT_MODE")) {
		if (cmd->Contains("?")) {
			stringa = "This command sets the current format collimation handling mode:\n\r";
			stringa += "FORMAT_MODE AUTO -> activates the automatic collimation mode (compressor paddle related) \n\r";
			stringa += "FORMAT_MODE DISABLED -> disables the internal format collimaton handling, allowing test commands to be executed\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		if (cmd->Contains("AUTO")) {
			PCB303::setFormatCollimationMode(PCB303::collimationModeEnum::AUTO, 0);
			send(System::Text::Encoding::Unicode->GetBytes("Format collimation set in AUTO mode \r\n"));
		}
		else {
			PCB303::setFormatCollimationMode(PCB303::collimationModeEnum::DISABLED, 0);
			send(System::Text::Encoding::Unicode->GetBytes("Format collimation set in DISABLED mode \r\n"));
		}
	}

	if (cmd->Contains("FORMAT_STORE_SLOT")) {
		if (cmd->Contains("?")) {
			stringa = "This command stores the current format of the target slot into the collimator configuration file.\n\r";
			stringa += "Sintax: FORMAT_STORE_SLOT slot  \n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		System::String^ param;
		int slot;

		// Slot number
		param = getStringParam(0, cmd);
		if (param == "") {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Slot Parameter \r\n"));
			return;
		}
		slot = System::Convert::ToInt16(param);
		if ((slot < 0) || (slot > 20)) {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Slot Parameter \r\n"));
			return;
		}


		// Set the current data in the parameter's slot and update the device
		if (PCB303::storeCollimationFormat(slot)) {
			send(System::Text::Encoding::Unicode->GetBytes("Format Collimation Data Successfully Stored  \r\n"));
			return;
		}
		else {
			send(System::Text::Encoding::Unicode->GetBytes("Comand Failed  \r\n"));
			return;
		}

		return;
	}

	if (cmd->Contains("FORMAT_SET_SLOT")) {
		if (cmd->Contains("?")) {
			stringa = "This command sets the data of a given collimation slot:\n\r";
			stringa += "Sintax: FORMAT_SET_SLOT slot front back left right trap \n\r";
			stringa += "- slot: slot number (0:20)\n\r";
			stringa += "- front: front blade value (0:5000)\n\r";
			stringa += "- back: back blade value (0:5000)\n\r";
			stringa += "- left: left blade value (0:5000)\n\r";
			stringa += "- right: right blade value (0:5000)\n\r";
			stringa += "- trap: trap blade value (0:5000)\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		System::String^ param;
		int value, slot, front, back, left, right, trap;

		// Slot number
		param = getStringParam(0, cmd);
		if (param == "") {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Slot Parameter \r\n"));
			return;
		}
		value = System::Convert::ToInt16(param);
		if ((value < 0) || (value > 20)) {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Slot Parameter \r\n"));
			return;
		}
		slot = value;

		// Front number
		param = getStringParam(1, cmd);
		if (param == "") {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Front Parameter \r\n"));
			return;
		}
		value = System::Convert::ToInt16(param);
		if ((value < 0) || (value > 5000)) {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Front Parameter \r\n"));
			return;
		}
		front = value;

		// Back number
		param = getStringParam(2, cmd);
		if (param == "") {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Back Parameter \r\n"));
			return;
		}
		value = System::Convert::ToInt16(param);
		if ((value < 0) || (value > 5000)) {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Back Parameter \r\n"));
			return;
		}
		back = value;

		// Left number
		param = getStringParam(3, cmd);
		if (param == "") {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Left Parameter \r\n"));
			return;
		}
		value = System::Convert::ToInt16(param);
		if ((value < 0) || (value > 5000)) {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Left Parameter \r\n"));
			return;
		}
		left = value;

		// Right number
		param = getStringParam(4, cmd);
		if (param == "") {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Right Parameter \r\n"));
			return;
		}
		value = System::Convert::ToInt16(param);
		if ((value < 0) || (value > 5000)) {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Right Parameter \r\n"));
			return;
		}
		right = value;

		// Trap number
		param = getStringParam(5, cmd);
		if (param == "") {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Trap Parameter \r\n"));
			return;
		}
		value = System::Convert::ToInt16(param);
		if ((value < 0) || (value > 5000)) {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Trap Parameter \r\n"));
			return;
		}
		trap = value;

		// Set the current data in the parameter's slot and update the device
		if (PCB303::setFormatConfiguration(slot, front, back, left, right, trap, false)) {
			send(System::Text::Encoding::Unicode->GetBytes("Collimator device updated with the new format in the requested slot  \r\n"));
			return;
		}
		else {
			send(System::Text::Encoding::Unicode->GetBytes("Error setting the ata register. \r\n"));
			return;
		}

		return;
	}

	if (cmd->Contains("FORMAT_SELECT_SLOT")) {
		if (cmd->Contains("?")) {
			stringa = "This command selects a given collimation slot:\n\r";
			stringa += "Sintax: FORMAT_SELECT_SLOT slot \n\r";
			stringa += "- slot: slot number (0:20)\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		System::String^ param;
		int slot;

		// Slot number
		param = getStringParam(0, cmd);
		if (param == "") {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Slot Parameter \r\n"));
			return;
		}
		slot = System::Convert::ToInt16(param);
		if ((slot < 0) || (slot > 20)) {
			send(System::Text::Encoding::Unicode->GetBytes("Invalid Slot Parameter \r\n"));
			return;
		}
	

		// Set the current data in the parameter's slot and update the device
		if (PCB303::selectManualCollimationFormat(slot)) {
			send(System::Text::Encoding::Unicode->GetBytes("Comand Executed  \r\n"));
			return;
		}
		else {
			send(System::Text::Encoding::Unicode->GetBytes("Comand Failed  \r\n"));
			return;
		}

		return;
	}
}

void DebuggerCLI::handleCollimatorCommands(System::String^ cmd) {
	System::String^ stringa;

	if (cmd->Contains("RESET")) {
		if (cmd->Contains("?")) {
			stringa = "This command causes the PCB303 board reset.\n\r";
			stringa += "The board parameters will be automatically uploaded by Gantry after the board startup\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		send(System::Text::Encoding::Unicode->GetBytes(" -> Collimator Board Reset Command!\n\r"));
		return;
	}

	if (cmd->Contains("GET_STATUS")) {
		if (cmd->Contains("?")) {
			stringa = "This command returns the data content of the relevant Device Status and Data registers.\n\r";			
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		stringa = "Status Register Content: \n\r";
		stringa += "Format Collimation Action: " + PCB303::getCollimationActionStatus().ToString() + "\n\r";
		stringa += "Format Collimation Index: " + PCB303::getCollimationTargetIndex().ToString() + "\n\r";
		stringa += "Mirror Action: " + PCB303::getMirrorActionStatus().ToString() + "\n\r";
		stringa += "Mirror Target: " + PCB303::getMirrorTargetIndex().ToString() + "\n\r";
		stringa += "Light Target: " + PCB303::getLightStatus().ToString() + "\n\r";
		stringa += "Bulb Temperature: " + PCB303::getBulbPercTemp().ToString() + "\n\r";
		stringa += "Stator Temperature: " + PCB303::getStatorPercTemp().ToString() + "\n\r";
		stringa += "FAN Status: " + PCB303::getFanStat().ToString() + "\n\r";
		stringa += "FAN Forced: " + PCB303::getFanForced().ToString() + "\n\r";

		stringa +=  "\n\r";

		send(System::Text::Encoding::Unicode->GetBytes(stringa));

		return;
	}

	if (cmd->Contains("SELECT_FILTER_SLOT")) {
		if (cmd->Contains("?")) {
			stringa = "This command selects a given Filter by slot number, from 0 to 4.\n\r";			
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		send(System::Text::Encoding::Unicode->GetBytes(" ->Filter Slot Selection Command!\n\r"));
		
		int slot = 0;
		if (cmd->Contains(" 0")) slot = 0;
		else if (cmd->Contains(" 1")) slot = 1;
		else if (cmd->Contains(" 2")) slot = 2;
		else if (cmd->Contains(" 3")) slot = 3;
		else if (cmd->Contains(" 4")) slot = 4;
		else {
			send(System::Text::Encoding::Unicode->GetBytes(" ->Invalid Slot Number!\n\r"));
			return;
		}
		
		PCB303::setFilterMode(PCB303::filterModeEnum::ACTIVE_MODE);
		if (!PCB303::selectFilterSlot(slot)) {
			send(System::Text::Encoding::Unicode->GetBytes(" ->Invalid Slot Number!\n\r"));
			return;
		}
		
		return;
	}

	if (cmd->Contains("SELECT_FILTER_MATTER")) {
		if (cmd->Contains("?")) {
			stringa = "This command selects a given Filter by matter.\n\r";
			stringa += "Available filters are:\n\r";
			stringa += "Cu Ag Rh Al Ag Ld \n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		send(System::Text::Encoding::Unicode->GetBytes(" ->Filter Matter Selection Command!\n\r"));		
		return;
	}


	if (cmd->Contains("SET_FAN")) {
		if (cmd->Contains("?")) {
			stringa  = "This command set the FAN activation mode:\n\r";
			stringa += "- AUTO: the FAN is activated withthe Tube temperature:\n\r";
			stringa += "- FORCED: the FAN is forced activated:\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		
		if (cmd->Contains(" AUTO")) {
			// Auto mode
			if (PCB303::setFanActivatonMode(false)) {
				send(System::Text::Encoding::Unicode->GetBytes(" ->FAN SET IN AUTO MODE!\n\r"));
			}
			else {
				send(System::Text::Encoding::Unicode->GetBytes(" ->FAN ACTIVATION MODE COMMAND REFUSED!\n\r"));
			}
		}
		else {
			// Forced Mode
			if (PCB303::setFanActivatonMode(true)) {
				send(System::Text::Encoding::Unicode->GetBytes(" ->FAN SET IN FORCED MODE!\n\r"));
			}
			else {
				send(System::Text::Encoding::Unicode->GetBytes(" ->FAN ACTIVATION MODE COMMAND REFUSED!\n\r"));
			}
		}


		return;
	}

	if (cmd->Contains("FORMAT")) {
		handle2DFormatCommands(cmd);
		return;
	}

	
	CollimatorCommandList();
	return;
}
