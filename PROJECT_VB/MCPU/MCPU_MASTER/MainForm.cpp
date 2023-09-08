#include "pch.h"
#include "MainForm.h"
#include "SystemConfig.h"
#include "CanOpenMotor.h"
#include "CanDeviceProtocol.h"

using namespace CppCLRWinFormsProject;
using namespace CANOPEN;

void MainForm::MainFormInitialize(void) {

	// Initialize the position of the form
	this->Left = GlobalObjects::monitor_X0;
	this->Top = GlobalObjects::monitor_Y0;

	// Clear the messages area
	StartupLogMessages->Clear();
	StartupErrorMessages->Clear();

	// Debug on the System Configuration File
	if (SystemConfig::Configuration->isWarning()) {
		Debug::WriteLine("SYSTEM CONFIGURATION FILE:" + SystemConfig::Configuration->getWarningString());
	}

	// Initialize the Package field 
	if (SystemConfig::Configuration->isWarning()) {
		StartupLogMessages->Text += "> " + SystemConfig::Configuration->getWarningString();
	}

	groupBoxPackage->Text = "PACKAGE ID: " + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_ID];
	labelFW301->Text = "FW301 REVISION:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FW301];
	labelFW302->Text = "FW302 REVISION:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FW302];
	labelFW303->Text = "FW303 REVISION:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FW303];
	labelFW304->Text = "FW302 REVISION:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FW304];
	labelFW315->Text = "FW302 REVISION:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FW315];
	labelCanDriver->Text = "FW302 REVISION:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FWCAN];
	labelGeneratorRevision->Text = "FW302 REVISION:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FWGEN];


	// Initialize the Activities
	labelPcb301Activity->Text = "Waiting ..";
	labelPcb302Activity->Text = "Waiting ..";
	labelPcb303Activity->Text = "Waiting ..";
	labelPcb304Activity->Text = "Waiting ..";
	labelPcb315Activity->Text = "Waiting ..";
	labelCanDriverActivity->Text = "Waiting ..";
	labelGeneratorActivity->Text = "Waiting ..";

	labelMotorArmActivity->Text = "Waiting ..";
	labelMotorTiltActivity->Text = "Waiting ..";
	labelMotorBodyActivity->Text = "Waiting ..";
	labelMotorShiftActivity->Text = "Waiting ..";
	labelMotorUpDownActivity->Text = "Waiting ..";

	startupFase = 0;
	startupSubFase = 0;
	startupCompleted = false;
	startupError = false;

	startupTimer = gcnew System::Timers::Timer(100);
	startupTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &MainForm::onStartupTimeout);
	startupTimer->Start();

	
}

bool MainForm::Startup_CanDriver(void) {

	switch (startupSubFase) {

	case 0: // Creates the Can Driver Object
		StartupLogMessages->Text += "> can driver initialization ..\n";
		labelCanDriverActivity->Text = "Connection ..";
		GlobalObjects::pCan = gcnew CanDriver();
		startupSubFase++;
		break;

	case 1:
		if (pCAN->isError()) {
			labelCanDriverActivity->Text = "STARTUP ERROR!";
			StartupErrorMessages->Text += "> can driver error: " + ((CanDriver^)GlobalObjects::pCan)->getErrorString();
			startupError = true;
			break;
		}

		if (pCAN->isConnected()) {
			labelCanDriverActivity->Text = "CONNECTED AND RUNNING";
			StartupLogMessages->Text += "> can driver revision: ";
			StartupLogMessages->Text += " API:" + ((CanDriver^)GlobalObjects::pCan)->apirev_maj.ToString() + "." + ((CanDriver^)GlobalObjects::pCan)->apirev_min.ToString();
			StartupLogMessages->Text += " SW:" + ((CanDriver^)GlobalObjects::pCan)->swrev_maj.ToString() + "." + ((CanDriver^)GlobalObjects::pCan)->swrev_min.ToString();
			StartupLogMessages->Text += " HW:" + ((CanDriver^)GlobalObjects::pCan)->hwrev_maj.ToString() + "." + ((CanDriver^)GlobalObjects::pCan)->hwrev_min.ToString();
			StartupLogMessages->Text += "\n";
			labelCanDriverActivity->ForeColor = Color::LightGreen;
			return true; // Completed
		}

		break;
	
	}

	return false;
}

bool MainForm::Startup_PCB301(void) {
	switch (startupSubFase) {

	case 0: // Creates the PCB301 process
		labelPcb301Activity->Text = "CONNECTION ..";
		StartupLogMessages->Text += "> pcb301 initialization ..\n";
		GlobalObjects::pFw301 = gcnew CanDeviceProtocol(0x10, L"POWER_SERVICE");
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (pFW301->getStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb301Activity->Text = "CONFIGURATION ..";
			StartupLogMessages->Text += "> pcb301 firmware revision: ";
			StartupLogMessages->Text += " BOOT:" + pFW301->getBootStatus() + ", REV:" + pFW301->getBootRevision();
			StartupLogMessages->Text += " APP:" + pFW301->getAppRevision();
			StartupLogMessages->Text += "\n";
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (pFW301->getStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb301Activity->Text = "CONNECTED AND RUNNING";
			labelPcb301Activity->ForeColor = Color::LightGreen;
			return true;
		}

		break;

	}
	return false;
}

bool MainForm::Startup_PCB302(void) {
	switch (startupSubFase) {

	case 0: // Creates the PCB302 process
		labelPcb302Activity->Text = "CONNECTION ..";
		StartupLogMessages->Text += "> pcb302 initialization ..\n";
		GlobalObjects::pFw302 = gcnew CanDeviceProtocol(0x11, L"COMPRESSOR_DEVICE");
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (pFW302->getStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb302Activity->Text = "CONFIGURATION ..";
			StartupLogMessages->Text += "> pcb302 firmware revision: ";
			StartupLogMessages->Text += " BOOT:" + pFW302->getBootStatus() + ", REV:" + pFW302->getBootRevision();
			StartupLogMessages->Text += " APP:" + pFW302->getAppRevision();
			StartupLogMessages->Text += "\n";
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (pFW302->getStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb302Activity->Text = "CONNECTED AND RUNNING";
			labelPcb302Activity->ForeColor = Color::LightGreen;
			return true;
		}

		break;

	}
	return false;
}

bool MainForm::Startup_PCB303(void) {
	switch (startupSubFase) {

	case 0: // Creates the PCB303 process
		labelPcb303Activity->Text = "CONNECTION ..";
		StartupLogMessages->Text += "> pcb303 initialization ..\n";
		GlobalObjects::pFw303 = gcnew CanDeviceProtocol(0x12, L"COMPRESSOR_DEVICE");
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (pFW303->getStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb303Activity->Text = "CONFIGURATION ..";
			StartupLogMessages->Text += "> pcb303 firmware revision: ";
			StartupLogMessages->Text += " BOOT:" + pFW303->getBootStatus() + ", REV:" + pFW303->getBootRevision();
			StartupLogMessages->Text += " APP:" + pFW303->getAppRevision();
			StartupLogMessages->Text += "\n";
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (pFW303->getStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb303Activity->Text = "CONNECTED AND RUNNING";
			labelPcb303Activity->ForeColor = Color::LightGreen;
			return true;
		}

		break;

	}
	return false;
}

bool MainForm::Startup_PCB304(void) {
	switch (startupSubFase) {

	case 0: // Creates the PCB304 process
		labelPcb304Activity->Text = "CONNECTION ..";
		StartupLogMessages->Text += "> pcb304 initialization ..\n";
		GlobalObjects::pFw304 = gcnew CanDeviceProtocol(0x14, L"COMPRESSOR_DEVICE");
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (pFW304->getStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb304Activity->Text = "CONFIGURATION ..";
			StartupLogMessages->Text += "> pcb304 firmware revision: ";
			StartupLogMessages->Text += " BOOT:" + pFW304->getBootStatus() + ", REV:" + pFW304->getBootRevision();
			StartupLogMessages->Text += " APP:" + pFW304->getAppRevision();
			StartupLogMessages->Text += "\n";
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (pFW304->getStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb304Activity->Text = "CONNECTED AND RUNNING";
			labelPcb304Activity->ForeColor = Color::LightGreen;
			return true;
		}

		break;

	}
	return false;
}

bool MainForm::Startup_PCB315(void) {
	switch (startupSubFase) {

	case 0: // Creates the PCB315 process
		labelPcb315Activity->Text = "CONNECTION ..";
		StartupLogMessages->Text += "> pcb315 initialization ..\n";
		GlobalObjects::pFw315 = gcnew CanDeviceProtocol(0x13, L"COMPRESSOR_DEVICE");
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (pFW315->getStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb315Activity->Text = "CONFIGURATION ..";
			StartupLogMessages->Text += "> pcb315 firmware revision: ";
			StartupLogMessages->Text += " BOOT:" + pFW315->getBootStatus() + ", REV:" + pFW315->getBootRevision();
			StartupLogMessages->Text += " APP:" + pFW315->getAppRevision();
			StartupLogMessages->Text += "\n";
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (pFW315->getStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb315Activity->Text = "CONNECTED AND RUNNING";
			labelPcb315Activity->ForeColor = Color::LightGreen;
			return true;
		}

		break;

	}
	return false;
}

bool MainForm::Startup_MotorBody(void) {
	switch (startupSubFase) {

	case 0: // Creates the Body Motor controller process
		labelMotorBodyActivity->Text = "CONNECTION ..";
		StartupLogMessages->Text += "> Motor Body initialization ..\n";
		GlobalObjects::pMotBody = gcnew CanOpenMotor(0x6, L"MOTOR_BODY", 453.2);
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (pFW315->getStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb315Activity->Text = "CONFIGURATION ..";
			StartupLogMessages->Text += "> pcb315 firmware revision: ";
			StartupLogMessages->Text += " BOOT:" + pFW315->getBootStatus() + ", REV:" + pFW315->getBootRevision();
			StartupLogMessages->Text += " APP:" + pFW315->getAppRevision();
			StartupLogMessages->Text += "\n";
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (pFW315->getStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb315Activity->Text = "CONNECTED AND RUNNING";
			labelPcb315Activity->ForeColor = Color::LightGreen;
			return true;
		}

		break;

	}
	return false;
}

void MainForm::StartupProcedure(void) {
	
	if (startupError) {
		startupTimer->Stop();
		return;
	}
	if (startupCompleted) {
		startupTimer->Stop();
		return;
	}

	switch (startupFase) {

	case 0: if (Startup_CanDriver()) { startupFase++; startupSubFase = 0; } break; // Startup of the Can Driver process
	case 1: if (Startup_PCB301()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB301 process
	case 2: if (Startup_PCB302()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB302 process
	case 3: if (Startup_PCB303()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB303 process
	case 4: if (Startup_PCB304()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB304 process
	case 5: if (Startup_PCB315()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB315 process


	}
}
void MainForm::WndProc(System::Windows::Forms::Message% m) 
{
	switch (m.Msg) {

	case (WM_USER + 1): // onStartupTimeout
		StartupProcedure();
		break;

	case (WM_USER + 2): 
		
		break;
	case (WM_USER + 3): 
		
		break;

	case (WM_USER + 4): 
		
		break;

	case (WM_USER + 5): 
		
		break;
	}


	Form::WndProc(m);
}
