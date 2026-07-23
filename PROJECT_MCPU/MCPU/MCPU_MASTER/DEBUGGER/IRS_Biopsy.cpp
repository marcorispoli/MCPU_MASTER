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

void DebuggerCLI::BiopsyCommandList(void) {
	// Shows the list of available commands if no valid command is detedcted
	System::String^ lista;
	lista =  " -> RESET - reset Board\n\r";
	lista += " -> GET_STATUS            - Shows the protocol registers\n\r";	

	lista += " \r\n --- Activation Commands ----------------------------------------- \n\r \r\n";
	lista += " -> MOVE_XYZ	x,y,z		 - Move the pointer to the XYZ position \n\r";
	lista += " -> MOVE_X	x		     - Move the pointer to X position \n\r";
	lista += " -> MOVE_Y	y		     - Move the pointer to Y position \n\r";
	lista += " -> MOVE_Z	z		     - Move the pointer to Z position \n\r";
	lista += " -> MOVE_HOME	home		 - Activate the homing \n\r";

	lista += " \r\n --- Test Commands ----------------------------------------- \n\r \r\n";
	lista += " -> TEST_LOOP  x0,y0,z0,y1,y1,z1,n		- Move the pointer from (x0,y0,z0) to (x1,y1,z1)  for N cycles \n\r";
	
	send(System::Text::Encoding::Unicode->GetBytes(lista));
	return;
}



void DebuggerCLI::handleBiopsyCommands(System::String^ cmd) {
	System::String^ stringa;

	if (cmd->Contains("RESET")) {
		if (cmd->Contains("?")) {
			stringa = "This command causes the PCB326 board reset.\n\r";
			stringa += "The board parameters will be automatically uploaded by Gantry after the board startup\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		send(System::Text::Encoding::Unicode->GetBytes(" -> Biopsy Board Reset Command!\n\r"));
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

	

	if (cmd->Contains("MOVE_XYZ")) {
		if (cmd->Contains("?")) {
			stringa = "FORMAT: MOVE_XYZ x,y,z\n\r";
			stringa += "This command moves the pointer to a (x,y,z) target position : \n\r";
			stringa += "- x = : X coordinate in dm, [0:2580]\n\r";
			stringa += "- y = : Y coordinate in dm, [0:700]\n\r";
			stringa += "- z = : Z coordinate in dm, [0:1400]\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		stringa = "TO BE DONE\n\r";
		send(System::Text::Encoding::Unicode->GetBytes(stringa));
		
		return;
	}

	if (cmd->Contains("MOVE_X")) {
		if (cmd->Contains("?")) {
			stringa = "FORMAT: MOVE_X x\n\r";
			stringa += "This command moves the pointer to a (x) target position without change the Y and Z position : \n\r";
			stringa += "-x = X coordinate in dm, [0:2580]\n\r";			
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		stringa = "TO BE DONE\n\r";
		send(System::Text::Encoding::Unicode->GetBytes(stringa));

		return;
	}

	if (cmd->Contains("MOVE_Y")) {
		if (cmd->Contains("?")) {
			stringa = "FORMAT: MOVE_Y y\n\r";
			stringa += "This command moves the pointer to a (y) target position without change the X and Z position : \n\r";
			stringa += "- y = Y  coordinate in dm, [0:700]\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		stringa = "TO BE DONE\n\r";
		send(System::Text::Encoding::Unicode->GetBytes(stringa));

		return;
	}

	if (cmd->Contains("MOVE_Z")) {
		if (cmd->Contains("?")) {
			stringa = "FORMAT: MOVE_Z z\n\r";
			stringa += "This command moves the pointer to a (z) target position without change the X and Y position : \n\r";
			stringa += "- z = Z  coordinate in dm, [0:1400]\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		stringa = "TO BE DONE\n\r";
		send(System::Text::Encoding::Unicode->GetBytes(stringa));

		return;
	}

	if (cmd->Contains("MOVE_HOME")) {
		if (cmd->Contains("?")) {
			stringa = "FORMAT: MOVE_HOME h\n\r";
			stringa += "This command activates the Homing procedure. \n\r";
			stringa += "- h = C/L/R  where:\n\r";
			stringa += "- C = Home Center\n\r";
			stringa += "- L = Home Left\n\r";
			stringa += "- R = Home Right\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		stringa = "TO BE DONE\n\r";
		send(System::Text::Encoding::Unicode->GetBytes(stringa));

		return;
	}
	
	if (cmd->Contains("TEST_LOOP")) {
		if (cmd->Contains("?")) {
			stringa = "FORMAT: TEST_LOOP x0,y0,z0,y1,y1,z1,N\n\r";
			stringa += "This command activates the pointer moving from the target (x0,y0,z0) to (x1,y1,z1) interactively. \n\r";
			stringa += "- x0,y0,z0 = starting position\n\r";
			stringa += "- x1,y1,z1 = ending position\n\r";
			stringa += "- N = Number of cycles\n\r";			
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		stringa = "TO BE DONE\n\r";
		send(System::Text::Encoding::Unicode->GetBytes(stringa));

		return;
	}
	

	BiopsyCommandList();
	return;
}

/*
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
*/