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




using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Threading;
using namespace System::Diagnostics;

DebuggerCLI::DebuggerCLI(System::String^ ip, int port)
{

	// Gets the Handler of the Form parent class, to be used for the message exchange
	connection_status = false;

	
	// Sets the Tcp/Ip data for the socket
	IPAddress^ localAddress = IPAddress::Parse(ip);
	SocketType sockType = SocketType::Stream;
	ProtocolType sockProtocol = ProtocolType::Tcp;
	serverSocket = nullptr;

	IPEndPoint^ localEndPoint = gcnew IPEndPoint(localAddress, port);
	IPEndPoint^ senderAddress = gcnew IPEndPoint(localAddress, 0);

	clientSocket = nullptr;
	rxBuffer = gcnew cli::array<System::Byte>(4096);
	

	// Create the server socket
	serverSocket = gcnew Socket(localAddress->AddressFamily, sockType, sockProtocol);
	serverSocket->Bind(localEndPoint);

	// Start the reception thread
	running_thread = gcnew Thread(gcnew ThreadStart(this, &DebuggerCLI::threadWork));
	running_thread->Name = "Loop Debugger thread";
	running_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
	running_thread->Start();

}

/// <summary>
/// This is the worker thread for the connection management.
/// 
/// There are a two nexted loops: 
/// - The first is for the listening and connection management;\n
/// - The second is for the data reception.
/// 
/// When a conmnection is closed, the first loop automatically enters in listening
/// in order to wait for a further connection.
/// Only one connection at a time is allowed!!
/// 
/// Whenever a connection status changes, the worjer thread 
/// update the Applicaition rising an event for the connection notify.
///  
/// When a buffer is received, a semaphore is used to lock the thread 
/// until the previous received buffer has been dispatched.
/// When the semaphore is ready, the received buffer is copied into the dispatch buffer 
/// in order to be safely sent to the destination.
/// 
/// The buffer is then dispatched in two steps:
/// - 1: a message with the data buffer from the worker thread is Posted to the main thread's message queue, where this class resides;
/// - 2: the message is self-received by the class that can safely rise an event handler;
/// - 3: the event handler with the received buffer can now be safely (thread safe) consumed by any class or Form residing into the Applicaiton Main Thread. 
/// 
/// </summary>
/// <param name=""></param>
void DebuggerCLI::threadWork(void) {

	while (true) {
		serverSocket->Listen(5);
		clientSocket = serverSocket->Accept();
		LogClass::logInFile("Debugger Client Connected!\n");
		connection_status = true;

		// Notifies the connection status
		connection_event(true);
		
		while (true) {
			try{
				rx_rc = clientSocket->Receive(rxBuffer);
				if (rx_rc == 0) break; // Shutdown
				rxHandler();
			 }
			catch (...) {
				break;
			}

		}
		connection_status = false;

		// The Client closed the connection
		LogClass::logInFile("Debugger Shutdown!\n");
		clientSocket->Shutdown(SocketShutdown::Send);
		clientSocket->Close();
		
		// Notifies the disconnection status
		connection_event(false);
	}

}

/// <summary>
/// This function sends a buffer on the bus.
/// 
/// </summary>
/// <param name="buffer">This is the data buffer to be sent</param>
void DebuggerCLI::send(cli::array<System::Byte>^ buffer) {
	if (!connection_status) return;
	if (clientSocket == nullptr) return;

	clientSocket->Send(buffer);

}



void DebuggerCLI::rxHandler(void){

	// Decode with Unicode the byte array into a string
	System::String^ sFrame = System::Text::UTF8Encoding::UTF8->GetString(rxBuffer, 0, rx_rc);

	if ((sFrame->Length < 3) && (!sFrame->Contains("?"))) return;


	// Menu detection
	if (sFrame->Contains("..")) current_menu = menu_index::ROOT;
	else if (sFrame->Contains("potter:")) current_menu = menu_index::POTTER;
	else if (sFrame->Contains("communication:")) current_menu = menu_index::COM;
	else if (sFrame->Contains("generator:")) current_menu = menu_index::GENERATOR;
	else if (sFrame->Contains("compressor:")) current_menu = menu_index::COMPRESSOR;
	else if (sFrame->Contains("collimator:")) current_menu = menu_index::COLLIMATOR;
	else if (sFrame->Contains("exposures:")) current_menu = menu_index::EXPOSURES;

	// Menu command dispatcher
	switch (current_menu) {
	case menu_index::ROOT: handleRootCommands(sFrame); send(System::Text::Encoding::Unicode->GetBytes("root:")); break;
	case menu_index::POTTER: handlePotterCommands(sFrame); send(System::Text::Encoding::Unicode->GetBytes("potter:")); break;
	case menu_index::COM: handleComCommands(sFrame); send(System::Text::Encoding::Unicode->GetBytes("communication:")); break;
	case menu_index::GENERATOR: handleGeneratorCommands(sFrame); send(System::Text::Encoding::Unicode->GetBytes("generator:")); break;
	case menu_index::COMPRESSOR: handleCompressorCommands(sFrame); send(System::Text::Encoding::Unicode->GetBytes("compressor:")); break;
	case menu_index::COLLIMATOR: handleCollimatorCommands(sFrame); send(System::Text::Encoding::Unicode->GetBytes("collimator:")); break;
	case menu_index::EXPOSURES: handleExposureCommands(sFrame); send(System::Text::Encoding::Unicode->GetBytes("exposures:")); break;
	default:
		handleRootCommands(sFrame); 		
	}
	return;

	
}

void DebuggerCLI::handleRootCommands(System::String^ cmd) {
	System::String^ lista;

	if (cmd->Contains("?")) {
		lista = "potter: potter related commands\n\r";
		lista += "communication: communication related commands\n\r";
		lista += "compressor: compressor related commands\n\r";
		lista += "generator: generator related commands\n\r";		
		lista += "collimator: collimator related commands\n\r";
		lista += "exposures: exposures related commands\n\r";

		send(System::Text::Encoding::Unicode->GetBytes(lista));
		return;
	}
}

void DebuggerCLI::handlePotterCommands(System::String^ cmd) {
	System::String^ stringa;

	if (cmd->Contains("RESET")) {
		if (cmd->Contains("?")) {
			stringa = "This command causes the PCB304 board reset.\n\r";
			stringa += "The board parameters will be automatically uploaded by Gantry after the board startup\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		send(System::Text::Encoding::Unicode->GetBytes(" -> Potter Board Reset Command!\n\r"));
		return;
	}

	if (cmd->Contains("GET_STATUS")) {
		if (cmd->Contains("?")) {
			stringa = "This command returns the data content of the relevant Device Status and Data registers.\n\r";
			stringa += "";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		stringa = "Status Register Content: \n\r";
		stringa += "inField:" + PCB304::isGridInFieldReady() + "\n\r";
		stringa += "outField:" + PCB304::isGridOffFieldReady() + "\n\r";
		stringa += "InOutTestExecuting:" + PCB304::isInOutTest() + "\n\r";
		stringa += "TransversalTestExecuting:" + PCB304::isTrasversalTest() + "\n\r";
		stringa += "GridInHome:" + PCB304::isHome() + "\n\r";
		stringa += "GridInCenter:" + PCB304::isCenter() + "\n\r";
		stringa += "DisableSignalStatus:" + PCB304::isGridDisableOn() + "\n\r";
		stringa += "StartSignalStatus:" + PCB304::isGridStartOn() + "\n\r";
		stringa += "Errors:" + PCB304::isError() + "\n\r";
		stringa += "TestCycleNum:" + PCB304::getTestCycle() + "\n\r";
		
		send(System::Text::Encoding::Unicode->GetBytes(stringa));

		return;
	}

	if (cmd->Contains("AUTO_GRID_IN")) {
		if (cmd->Contains("?")) {
			stringa = "This command changes the automatic grid status to In-Field.\n\r";
			stringa += "This command overrides the current Operating status setting and may conflict with it.\n\r";
			stringa += "Use this command in Idle Status to prevent conflicts.\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		send(System::Text::Encoding::Unicode->GetBytes(" ->Auto Grid In Command!\n\r"));
		PCB304::resetErrorCount();
		PCB304::setAutoGridInField();		
		return;
	}


	if (cmd->Contains("AUTO_GRID_OUT")) {
		if (cmd->Contains("?")) {
			stringa = "This command changes the automatic grid status to Out-Field.\n\r";
			stringa += "This command overrides the current Operating status setting and may conflict with it.\n\r";
			stringa += "Use this command in Idle Status to prevent conflicts.\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		send(System::Text::Encoding::Unicode->GetBytes(" ->Auto Grid Out Command!\n\r"));
		PCB304::resetErrorCount();
		PCB304::setAutoGridOutField();		
		return;
	}

	if (cmd->Contains("TRANSLATION_TEST")) {
		if (cmd->Contains("?")) {
			stringa = "This command activates the Grid Translation test.\n\r";
			stringa += "The Grid is set in InField first, then a set of Translations will be executed.\n\r";
			stringa += "When the test terminates, the grid will be set in the current Auto position.\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		if (PCB304::requestTestTranslation())
			send(System::Text::Encoding::Unicode->GetBytes(" -> Grid Translation Test Command Started!\n\r"));
		else
			send(System::Text::Encoding::Unicode->GetBytes(" -> Grid Translation Test Command: error busy!\n\r"));

		return;
	}

	if (cmd->Contains("INOUT_TEST")) {
		if (cmd->Contains("?")) {
			stringa = "This command activates the Grid In-Out test.\n\r";
			stringa += "A series of In and Out positioning will be executed.\n\r";
			stringa += "When the test terminates, the grid will be set in the current Auto position.\n\r";
			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		if (PCB304::requestTestInOut())
			send(System::Text::Encoding::Unicode->GetBytes(" -> Grid Test In/Out Command Started!\n\r"));
		else
			send(System::Text::Encoding::Unicode->GetBytes(" -> Grid Test In/Out Command: error busy!\n\r"));

		return;
	}

	if (cmd->Contains("SYNCH_ON")) {
		
		if (cmd->Contains("?")) {
			stringa = "This test allows to verify the synchronization signal integrity with the Generator.\n\r";
			stringa += "This command activates the grid synchronization mode with the Generator.\n\r";
			stringa += "In the Synch-Mode, activating the Grid-Start hardware signals, the grid will be activated.\n\r";
			stringa += "The GridDisable Hardware signal should be set properly by the Potter device.\n\r";

			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		// Activates the synch mode 
		PCB304::syncGeneratorOn();
		send(System::Text::Encoding::Unicode->GetBytes(" -> Grid Synchronization activated!\n\r"));
		return;
	}

	if (cmd->Contains("SYNCH_OFF")) {

		if (cmd->Contains("?")) {			
			stringa = "This command disables the grid synchronization mode.\n\r";
			stringa += "The command should be used to terminate the SYNCH_ON test.\n\r";

			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		// Activates the synch mode 
		PCB304::syncGeneratorOff(true);
		send(System::Text::Encoding::Unicode->GetBytes(" -> Grid Synchronization deactivated!\n\r"));
		return;
	}


	// Shows the list of available commands if no valid command is detedcted
	System::String^ lista;
	lista = " -> RESET - reset Board\n\r";
	lista += " -> GET_STATUS	- Shows the protocol registers\n\r";
	lista += " -> AUTO_GRID_IN	- Grin In Field\n\r";
	lista += " -> AUTO_GRID_OUT	- Grid Out Field\n\r";
	lista += " -> TRANSLATION_TEST	- Translation Test Grid 10 times\n\r";
	lista += " -> INOUT_TEST	- Test In-Out 10 times\n\r";
	lista += " -> SYNCH_ON		- Generator Sync On\n\r";
	lista += " -> SYNCH_OFF		- Generator Sync Off\n\r";

	send(System::Text::Encoding::Unicode->GetBytes(lista));
	return;
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

	// Shows the list of available commands if no valid command is detedcted
	System::String^ lista;
	lista =  " -> RESET - reset Board\n\r";
	lista += " -> GET_STATUS            - Shows the protocol registers\n\r";
	lista += " -> SELECT_FILTER_SLOT    - Filter selection by slot number\n\r";
	lista += " -> SELECT_FILTER_MATTER	- Filter selection by Matter\n\r";
	lista += " -> SET_FAN	            - Fan Activation Mode \n\r";

	send(System::Text::Encoding::Unicode->GetBytes(lista));
	return;
}

void DebuggerCLI::handleComCommands(System::String^ cmd) {
	System::String^ result_string = "";

	// Can Statistics
	if (cmd->Contains("CANSTAT")) {
		result_string += "PCB301: " + PCB301::device->getCanCommunicationMonitorString() + "\n\r";
		result_string += "PCB302: " + PCB302::device->getCanCommunicationMonitorString() + "\n\r";
		result_string += "PCB303: " + PCB303::device->getCanCommunicationMonitorString() + "\n\r";
		result_string += "PCB304: " + PCB304::device->getCanCommunicationMonitorString() + "\n\r";
		result_string += "PCB326: " + PCB326::device->getCanCommunicationMonitorString() + "\n\r";

		if (!TiltMotor::device->isSimulatorMode()) result_string += "TiltMotor: " + TiltMotor::device->getCanCommunicationMonitorString() + "\n\r";
		if (!ArmMotor::device->isSimulatorMode()) result_string += "ArmMotor: " + ArmMotor::device->getCanCommunicationMonitorString() + "\n\r";
		if (!VerticalMotor::device->isSimulatorMode()) result_string += "VerticalMotor: " + VerticalMotor::device->getCanCommunicationMonitorString() + "\n\r";
		if (!BodyMotor::device->isSimulatorMode()) result_string += "BodyMotor: " + BodyMotor::device->getCanCommunicationMonitorString() + "\n\r";
		if (!SlideMotor::device->isSimulatorMode()) result_string += "SlideMotor: " + SlideMotor::device->getCanCommunicationMonitorString() + "\n\r";
		send(System::Text::Encoding::Unicode->GetBytes(result_string));
		return;
	}
}

void DebuggerCLI::handleGeneratorCommands(System::String^ cmd) {
	
	if (cmd->Contains("ANODE_TABLE")) {
		send(System::Text::Encoding::Unicode->GetBytes("Generator Anodic Table Generation .. "));
		Exposures::pExposure->generateAnodicCurrentTable(true);
		send(System::Text::Encoding::Unicode->GetBytes(" -> Completed!"));
		return;
	}
	
}
void DebuggerCLI::handleCompressorCommands(System::String^ cmd) {
	System::String^ result_string = "";


	if (cmd->Contains("COMPRESSOR")) {
		result_string += "PCB302> RAW POS:" + PCB302::device->getRawPosition().ToString() + "\n\r";
		result_string += "PCB302> RAW FORCE:" + PCB302::device->getRawForce().ToString() + "\n\r";
		result_string += "PCB302> PADDLE POS:" + PCB302::device->getPaddlePosition().ToString() + "\n\r";
		result_string += "PCB302> PADDLE FORCE:" + PCB302::device->getPaddleForce().ToString() + "\n\r";
		send(System::Text::Encoding::Unicode->GetBytes(result_string));
		return;
	}
}

void DebuggerCLI::handleExposureCommands(System::String^ cmd) {
	System::String^ stringa = "";


	if (cmd->Contains("SET_FOCUS_MODE")) {
		if (cmd->Contains("?")) {
			stringa = "This command select the focus usage with the following modes:\n\r";
			stringa += "- AUTO: the focus is selected automatically (magnifier device detection) \n\r";
			stringa += "- LARGE: the focus is forced to be Large \n\r";
			stringa += "- SMALL: the focus is forced to be Small \n\r";

			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}


		if (cmd->Contains("AUTO")) {
			Exposures::setFocus(Exposures::focus_selection_index::FOCUS_AUTO);
			stringa = "Focus in AUTO mode \n\r";
		}
		else if (cmd->Contains("LARGE")) {
			Exposures::setFocus(Exposures::focus_selection_index::FOCUS_LARGE);
			stringa = "Focus in LARGE mode \n\r";

		}
		else {
			Exposures::setFocus(Exposures::focus_selection_index::FOCUS_SMALL);
			stringa = "Focus in SMALL mode \n\r";

		}

		send(System::Text::Encoding::Unicode->GetBytes(stringa));
		return;
	}

	if (cmd->Contains("SET_GRID_MODE")) {
		if (cmd->Contains("?")) {
			stringa = "This command select the Grid In/Out for the 2D exposures:\n\r";
			stringa += "- AUTO: the grid is selected by the current operating condition \n\r";
			stringa += "- GRID_IN: the grid is forced In Field \n\r";
			stringa += "- GRID_OUT: the grid is forced out field \n\r";

			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		if (cmd->Contains("AUTO")) {
			// Set Grid to Auto
			Exposures::setGrid(Exposures::grid_selection_index::GRID_AUTO);
		}
		else if (cmd->Contains("GRID_IN")) {
			Exposures::setGrid(Exposures::grid_selection_index::GRID_IN);			
			stringa = "GRID mode set in GRID_IN mode \n\r";

		}
		else {
			Exposures::setGrid(Exposures::grid_selection_index::GRID_OUT);			
			stringa = "GRID mode set in GRID_OUT mode \n\r";
		}

		send(System::Text::Encoding::Unicode->GetBytes(stringa));
		return;
	}

	if (cmd->Contains("SET_TOMO_MODE")) {
		if (cmd->Contains("?")) {
			stringa = "This command select the 3D exposure behavior:\n\r";
			stringa += "- AUTO: the 3D exposure is set normally by the current operating mode \n\r";
			stringa += "- CALIB: the 3D is set for the calibration (steady arm with 25 pulses) \n\r";

			send(System::Text::Encoding::Unicode->GetBytes(stringa));
			return;
		}

		if (cmd->Contains("AUTO")) {
			Exposures::setTomoMode(Exposures::tomo_mode_selection_index::TOMO_AUTO);
			stringa = "TOMO mode set in AUTO mode \n\r";
		}
		else if (cmd->Contains("CALIB")) {
			Exposures::setTomoMode(Exposures::tomo_mode_selection_index::TOMO_CALIB);
			stringa = "TOMO mode set in CALIB mode \n\r";

		}
		else {
			stringa = "Wrong parameter syntax!! \n\r";
		}

		send(System::Text::Encoding::Unicode->GetBytes(stringa));
		return;
	}





	// Shows the list of available commands if no valid command is detedcted
	System::String^ lista;
	lista = " -> SET_FOCUS_MODE - sets the current exposure focus mode\n\r";
	lista += " -> SET_GRID_MODE - sets the current exposure grid usage mode\n\r";
	lista += " -> SET_TOMO_MODE - sets the current exposure Tomo usage mode\n\r";
	lista += "\n\r";

	send(System::Text::Encoding::Unicode->GetBytes(lista));
	return;
}

