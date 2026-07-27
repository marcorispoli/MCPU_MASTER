#include "Debugger.h"
#include "Log.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB325.h"
#include "PCB304.h"
#include "PCB326.h"
#include "TiltMotor.h"
#include "ArmMotor.h"
#include "VerticalMotor.h"
#include "BodyMotor.h"
#include "SlideMotor.h"
#include "ExposureModule.h"
#include "BiopsyStudy.h"
#include "BiopsyHomeProcedure.h"
#include <thread>

void DebuggerCLI::BiopsyCommandList(void) {
	// Shows the list of available commands if no valid command is detedcted
	System::String^ lista;
	lista =  " -> RESET - reset Board\n\r";
	lista += " -> GET_STATUS            - Shows the device internal status\n\r";	
	lista += " -> SET_WORKING_MODE      - Sets the device working mode\n\r";
	lista += " -> BIOPSY_STUDY			- Open/Close a Biopsy Study Patient Operatiing Mode\n\r";

	lista += " \r\n --- Activation Commands ----------------------------------------- \n\r \r\n";	
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

		stringa =  "- DRIVER WORKING MODE: " + PCB325::getWorkingMode().ToString() + " \n\r";
		stringa += "- DEVICE WORKING MODE: " + PCB325::getDeviceMode().ToString() + " \n\r";
		stringa += "- XSCROLL: CODE=" + PCB325::getXScrollCode().ToString() + ", ANL=" + PCB325::getXscrollAnalog() + "\n\r";
		stringa += "- NEEDLE: CODE=" + PCB325::getNeedleCode().ToString() + ", ANL=" + PCB325::getNeedleAnalog() + "\n\r";
		stringa += "- Y-UP STATUS: " + PCB325::getYUpStat().ToString() + "\n\r";
		stringa += "- X-POSITION: " + PCB325::getX().ToString() + "\n\r";
		stringa += "- Y-POSITION: " + PCB325::getY().ToString() + "\n\r";
		stringa += "- Z-POSITION: " + PCB325::getZ().ToString() + "\n\r";
		stringa += "- SH-POSITION: " + PCB325::getS().ToString() + "\n\r";
		stringa += "- POWER SWITCH: " + PCB325::getPowerSwitchStat().ToString() + "\n\r";
		stringa += "- GENERAL ENABLE FLAG: " + PCB325::getGeneralEnableStat().ToString() + "\n\r";
		stringa += "- NEEDLE DISABLE FLAG: " + PCB325::getNeedleDisableStat().ToString() + "\n\r";
		stringa += "- KEYB ENABLE FLAG: " + PCB325::getKeybEnableStat().ToString() + "\n\r";
		stringa += "- KEYB CODE=" + PCB325::getKeboardAnalog().ToString() +  "\n\r";
		stringa += "- KEYSTEP ENABLE FLAG: " + PCB325::getKeyStepStat().ToString() + "\n\r";
		stringa += "- MOTOR POWER SUPPLY: " + PCB325::getMotorPowerAnalog().ToString() + "\n\r";

		stringa +=  "\n\r";

		send(System::Text::Encoding::Unicode->GetBytes(stringa));

		return;
	}

	if (cmd->Contains("SET_WORKING_MODE")) {
		if (cmd->Contains("?")) {
			stringa = "FORMAT: SET_WORKING_MODE mode\n\r";
			stringa += "This command sets the current working mode of the device.\n\r";
			stringa += "- mode = DISABLE -> Sets the device in DISABLE MODE\n\r";
			stringa += "- mode = CALIB -> Sets the device in CALIBRATION MODE\n\r";
			stringa += "- mode = COMMAND -> Sets the device in COMMAND MODE\n\r";
			stringa += "- mode = SERVICE -> Sets the device in SERVICE MODE\n\r";
			stringa += "- mode = FREE -> Sets the device in Stand Alone\n\r";
			
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}


		if (cmd->Contains("DISABLE")) {
			PCB325::setBiopsyDisableMode();
			stringa = "Disable Mode Requested\n\r";
		}else if (cmd->Contains("CALIB")) {
			PCB325::setBiopsyCalibrationMode();
			stringa = "Calibration Mode Requested\n\r";
		}
		else if (cmd->Contains("COMMAND")) {
			PCB325::setBiopsyCommandMode();
			stringa = "Command Mode Requested\n\r";
		}
		else if (cmd->Contains("SERVICE")) {
			PCB325::setBiopsyServiceMode();
			stringa = "Service Mode Requested\n\r";
		}
		else if (cmd->Contains("SERVICE")) {
			PCB325::setBiopsyServiceMode();
			stringa = "Service Mode Requested\n\r";
		}
		else if (cmd->Contains("FREE")) {
			PCB325::setBiopsyFreeMode();
			stringa = "Free Mode Requested\n\r";
		}
		else {
			stringa = "Wrong Parameter \n\r";
		}


		send(System::Text::Encoding::Unicode->GetBytes(stringa));

		return;
	}

	if (cmd->Contains("BIOPSY_STUDY")) {
		if (cmd->Contains("?")) {
			stringa = "FORMAT: BIOPSY_STUDY  stat\n\r";
			stringa += "This command open or close the Biopsy Study Patient Operating Mode.\n\r";
			stringa += "- stat = OPEN -> Open the Study\n\r";
			stringa += "- stat = CLOSE -> Close the Study\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}



		if (cmd->Contains("OPEN")) {
			// Open the study and assignes the patient name !!!
			if (!Gantry::setOpenBiopsyStudy("IRS")) stringa = "ERROR! The Study cannot be open\n\r";
			else		stringa = "Command Executed\n\r";
			
		}else if (cmd->Contains("CLOSE")) {
			// Open the study and assignes the patient name !!!
			if (!Gantry::setCloseStudy()) stringa = "ERROR! The Study cannot be closed\n\r";
			else		stringa = "Command Executed\n\r";
			
		}else stringa = "ERROR! Wrong Parameter\n\r";

		send(System::Text::Encoding::Unicode->GetBytes(stringa));
		return;
	}


	if (cmd->Contains("MOVE_X")) {
		if (cmd->Contains("?")) {
			stringa = "FORMAT: MOVE_X x\n\r";
			stringa += "This command moves the pointer to x target position : \n\r";
			stringa += "- x = : X coordinate in dm, [0:2580]\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		if (PCB325::getWorkingMode() != PCB325::working_mode::BIOPSY_COMMAND_MODE) {
			stringa = "ERROR! The Device shall be set in COMMAND WORKING MODE first. \n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		// Gets the coordinates X
		System::String^ param;
		int X;

		param = getStringParam(0, cmd);
		if (param == "") {
			stringa = "ERROR! Invalid X value. \n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}
		X = System::Convert::ToInt16(param);
		if (X > PCB325::ConstantDefinition::BIOPSY_MAX_X_POSITION) {
			stringa = "ERROR! X Out Of Range. Max value is " + PCB325::ConstantDefinition::BIOPSY_MAX_X_POSITION + "\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		// Verify if a possible crash is possible before to move
		if (PCB325::testXPossibleCrash(X)) {
			stringa = "ERROR! This X target may impact against the Bopsy body\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		if (!PCB325::moveX(X)) {
			stringa = "ERROR! Command Busy\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		stringa = "Command Executed\n\r";
		send(System::Text::Encoding::Unicode->GetBytes(stringa));
		return;
	}

	if (cmd->Contains("MOVE_Y")) {
		if (cmd->Contains("?")) {
			stringa = "FORMAT: MOVE_Y y\n\r";
			stringa += "This command moves the pointer to x target position : \n\r";
			stringa += "- y = : Y coordinate in dm, [0:700]\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		if (PCB325::getWorkingMode() != PCB325::working_mode::BIOPSY_COMMAND_MODE) {
			stringa = "ERROR! The Device shall be set in COMMAND WORKING MODE first. \n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		// Gets the coordinates X
		System::String^ param;
		int Y;

		param = getStringParam(0, cmd);
		if (param == "") {
			stringa = "ERROR! Invalid Y value. \n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}
		Y = System::Convert::ToInt16(param);
		if (Y > PCB325::ConstantDefinition::BIOPSY_MAX_Y_POSITION) {
			stringa = "ERROR! Y Out Of Range. Max value is " + PCB325::ConstantDefinition::BIOPSY_MAX_Y_POSITION + "\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		
		if (!PCB325::moveY(Y)) {
			stringa = "ERROR! Command Busy\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		stringa = "Command Executed\n\r";
		send(System::Text::Encoding::Unicode->GetBytes(stringa));
		return;
	}

	
	if (cmd->Contains("MOVE_Z")) {
		if (cmd->Contains("?")) {
			stringa = "FORMAT: MOVE_Z z\n\r";
			stringa += "This command moves the pointer to z target position : \n\r";
			stringa += "- z = : Z coordinate in dm, [0:1350]\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		if (PCB325::getWorkingMode() != PCB325::working_mode::BIOPSY_COMMAND_MODE) {
			stringa = "ERROR! The Device shall be set in COMMAND WORKING MODE first. \n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		// Gets the coordinates X
		System::String^ param;
		int Z;

		param = getStringParam(0, cmd);
		if (param == "") {
			stringa = "ERROR! Invalid Z value. \n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}
		Z = System::Convert::ToInt16(param);
		if (Z > PCB325::ConstantDefinition::BIOPSY_MAX_Z_POSITION) {
			stringa = "ERROR! Z Out Of Range. Max value is " + PCB325::ConstantDefinition::BIOPSY_MAX_Z_POSITION + "\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}


		if (!PCB325::moveZ(Z)) {
			stringa = "ERROR! Command Busy\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		stringa = "Command Executed\n\r";
		send(System::Text::Encoding::Unicode->GetBytes(stringa));
		return;
	}


	if (cmd->Contains("MOVE_HOME")) {
		if (cmd->Contains("?")) {
			stringa = "FORMAT: MOVE_HOME h\n\r";
			stringa += "This command activates the Homing procedure. \n\r";
			stringa += "- h = CENTER ->Home Center\n\r";
			stringa += "- h = LEFT -> Home Left\n\r";
			stringa += "- h = RIGHT -> Home Right\n\r";
			stringa += "NOTE: the command can be executed only with a Biopsy Study Open \n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		if (!Gantry::isBIOPSY()) {
			stringa = "ERROR: the Study is not Open.\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		if (PCB325::getWorkingMode() != PCB325::working_mode::BIOPSY_COMMAND_MODE) {
			stringa = "ERROR! The Device shall be set in COMMAND WORKING MODE first. \n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		if (cmd->Contains("CENTER")) {
			int res = BiopsyHomeProcedure::activateCenter(0);
			if (res < 0) {
				stringa = "ERROR: COMMAND ERROR CODE = " + res.ToString();
				send(System::Text::Encoding::Unicode->GetBytes(stringa));
				return;
			}

		}else if (cmd->Contains("LEFT")) {
			int res = BiopsyHomeProcedure::activateLeft(0);
			if (res < 0) {
				stringa = "ERROR: COMMAND ERROR CODE = " + res.ToString();
				send(System::Text::Encoding::Unicode->GetBytes(stringa));
				return;
			}

		}
		else if (cmd->Contains("RIGHT")) {
			int res = BiopsyHomeProcedure::activateRight(0);
			if (res < 0) {
				stringa = "ERROR: COMMAND ERROR CODE = " + res.ToString();
				send(System::Text::Encoding::Unicode->GetBytes(stringa));
				return;
			}

		}
		else {
			stringa = "ERROR: WRONG COMMAND PARAMETER";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		
		stringa = "Command Executed\n\r";
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
