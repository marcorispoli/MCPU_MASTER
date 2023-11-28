#include "gantry_global_status.h"
#include "mainForm.h"
#include "operatingForm.h"
#include "idleForm.h"
#include "awsProtocol.h"

#include "SystemConfig.h"

#include "CanOpenMotor.h"
#include "BodyMotor.h"
#include "TiltMotor.h"
#include "ArmMotor.h"
#include "VerticalMotor.h"

#include "CanDriver.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB304.h"
#include "PCB315.h"
#include "PCB326.h"
#include "Generator.h"


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
	labelFW304->Text = "FW304 REVISION:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FW304];
	labelFW315->Text = "FW315 REVISION:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FW315];
	labelFW326->Text = "FW326 REVISION:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FW326];
	labelCanDriver->Text = "CAN REVISION:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FWCAN];
	labelGeneratorRevision->Text = "SFHD REVISION:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FWGEN];


	// Initialize the Activities
	labelPcb301Activity->Text = "Waiting ..";
	labelPcb302Activity->Text = "Waiting ..";
	labelPcb303Activity->Text = "Waiting ..";
	labelPcb304Activity->Text = "Waiting ..";
	labelPcb315Activity->Text = "Waiting ..";
	labelPcb326Activity->Text = "Waiting ..";
	labelCanDriverActivity->Text = "Waiting ..";
	labelGeneratorActivity->Text = "Waiting ..";
	labelShActivity->Text = "Waiting ..";

	labelMotorArmActivity->Text = "Waiting ..";
	labelMotorTiltActivity->Text = "Waiting ..";
	labelMotorBodyActivity->Text = "Waiting ..";
	labelMotorShiftActivity->Text = "Waiting ..";
	labelMotorUpDownActivity->Text = "Waiting ..";

	startupFase = 0;
	startupSubFase = 0;
	startupCompleted = false;
	startupError = false;

	// Init of the Global operating status
	GantryStatusRegisters::OperatingStatusRegister::setStartup();

	// Start the startup session
	startupTimer = gcnew System::Timers::Timer(100);
	startupTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &MainForm::onStartupTimeout);
	startupTimer->Start();
	
	
	
}

bool MainForm::Startup_CanDriver(void) {

	switch (startupSubFase) {

	case 0: // Creates the Can Driver Object
		StartupLogMessages->Text += "> can driver initialization ..\n";
		labelCanDriverActivity->Text = "Connection ..";
		//GlobalObjects::pCan = gcnew CanDriver();
		startupSubFase++;
		break;

	case 1:
		if (CanDriver::isError()) {
			labelCanDriverActivity->Text = "STARTUP ERROR!";
			StartupErrorMessages->Text += "> can driver error: " + CanDriver::getErrorString();
			startupError = true;
			break;
		}

		if (CanDriver::isConnected()) {
			labelCanDriverActivity->Text = "CONNECTED AND RUNNING";
			StartupLogMessages->Text += "> can driver revision: ";
			StartupLogMessages->Text += " API:" + CanDriver::apirev_maj.ToString() + "." + CanDriver::apirev_min.ToString();
			StartupLogMessages->Text += " SW:" + CanDriver::swrev_maj.ToString() + "." + CanDriver::swrev_min.ToString();
			StartupLogMessages->Text += " HW:" + CanDriver::hwrev_maj.ToString() + "." + CanDriver::hwrev_min.ToString();
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
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (PCB301::device->getStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb301Activity->Text = "CONFIGURATION ..";
			StartupLogMessages->Text += "> pcb301 firmware revision: ";
			StartupLogMessages->Text += " BOOT:" + PCB301::device->getBootStatus() + ", REV:" + PCB301::device->getBootRevision();
			StartupLogMessages->Text += " APP:" + PCB301::device->getAppRevision();
			StartupLogMessages->Text += "\n";
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (PCB301::device->getStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
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
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (PCB302::device->getStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb302Activity->Text = "CONFIGURATION ..";
			StartupLogMessages->Text += "> pcb302 firmware revision: ";
			StartupLogMessages->Text += " BOOT:" + PCB302::device->getBootStatus() + ", REV:" + PCB302::device->getBootRevision();
			StartupLogMessages->Text += " APP:" + PCB302::device->getAppRevision();
			StartupLogMessages->Text += "\n";
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (PCB302::device->getStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
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
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (PCB303::device->getStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb303Activity->Text = "CONFIGURATION ..";
			StartupLogMessages->Text += "> pcb303 firmware revision: ";
			StartupLogMessages->Text += " BOOT:" + PCB303::device->getBootStatus() + ", REV:" + PCB303::device->getBootRevision();
			StartupLogMessages->Text += " APP:" + PCB303::device->getAppRevision();
			StartupLogMessages->Text += "\n";
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (PCB303::device->getStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
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
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (PCB304::device->getStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb304Activity->Text = "CONFIGURATION ..";
			StartupLogMessages->Text += "> pcb304 firmware revision: ";
			StartupLogMessages->Text += " BOOT:" + PCB304::device->getBootStatus() + ", REV:" + PCB304::device->getBootRevision();
			StartupLogMessages->Text += " APP:" + PCB304::device->getAppRevision();
			StartupLogMessages->Text += "\n";
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (PCB304::device->getStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
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
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (PCB315::device->getStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb315Activity->Text = "CONFIGURATION ..";
			StartupLogMessages->Text += "> pcb315 firmware revision: ";
			StartupLogMessages->Text += " BOOT:" + PCB315::device->getBootStatus() + ", REV:" + PCB315::device->getBootRevision();
			StartupLogMessages->Text += " APP:" + PCB315::device->getAppRevision();
			StartupLogMessages->Text += "\n";
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (PCB315::device->getStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb315Activity->Text = "CONNECTED AND RUNNING";
			labelPcb315Activity->ForeColor = Color::LightGreen;
			return true;
		}

		break;

	}
	return false;
}

bool MainForm::Startup_PCB326(void) {
	switch (startupSubFase) {

	case 0: // Creates the PCB315 process
		labelPcb326Activity->Text = "CONNECTION ..";
		StartupLogMessages->Text += "> pcb326 initialization ..\n";		
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (PCB326::device->getStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb326Activity->Text = "CONFIGURATION ..";
			StartupLogMessages->Text += "> pcb326 firmware revision: ";
			StartupLogMessages->Text += " BOOT:" + PCB326::device->getBootStatus() + ", REV:" + PCB326::device->getBootRevision();
			StartupLogMessages->Text += " APP:" + PCB326::device->getAppRevision();
			StartupLogMessages->Text += "\n";
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (PCB326::device->getStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb326Activity->Text = "CONNECTED AND RUNNING";
			labelPcb326Activity->ForeColor = Color::LightGreen;
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
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (!BodyMotor::device->activateConfiguration()) break;

		labelMotorBodyActivity->Text = "CONFIGURATION ..";
		StartupLogMessages->Text += "> motor body status:" + BodyMotor::device->getInternalStatusStr();
		StartupLogMessages->Text += "\n";
		startupSubFase++;
		break;

	case 2: // Wait the connection and configuration
		if (BodyMotor::device->isConfigurating()) break;
		if ((!BodyMotor::device->isODConfigured()) || (!BodyMotor::device->isNanojConfigured())) {
			startupError = true;
			labelMotorBodyActivity->Text = "CONFIGURATION ERROR";
			labelMotorBodyActivity->ForeColor = Color::Red;

			if (!BodyMotor::device->isODConfigured()) StartupErrorMessages->Text += "> Motor Body Object Dictionary initialization Failed\n";
			if (!BodyMotor::device->isNanojConfigured()) StartupErrorMessages->Text += "> Motor Body Nanoj initialization Failed\n";
			return true;
		}
		
		labelMotorBodyActivity->Text = "CONNECTED AND CONFIGURED";
		labelMotorBodyActivity->ForeColor = Color::LightGreen;
		return true;
		
		break;

	}
	return false;
}

bool MainForm::Startup_MotorTilt(void) {
	switch (startupSubFase) {

	case 0: // Creates the Body Motor controller process
		labelMotorTiltActivity->Text = "CONNECTION ..";
		StartupLogMessages->Text += "> Motor Tilt initialization ..\n";
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (!TiltMotor::device->activateConfiguration()) break;

		labelMotorTiltActivity->Text = "CONFIGURATION ..";
		StartupLogMessages->Text += "> Motor Tilt status:" + TiltMotor::device->getInternalStatusStr();
		StartupLogMessages->Text += "\n";
		startupSubFase++;
		break;

	case 2: // Wait the connection and configuration
		if (TiltMotor::device->isConfigurating()) break;
		if ( (!TiltMotor::device->isODConfigured()) || (!TiltMotor::device->isNanojConfigured())) {
			startupError = true;
			labelMotorTiltActivity->Text = "CONFIGURATION ERROR";
			labelMotorTiltActivity->ForeColor = Color::Red;

			if(!TiltMotor::device->isODConfigured()) StartupErrorMessages->Text += "> Motor Tilt Object Dictionary initialization Failed\n";
			if (!TiltMotor::device->isNanojConfigured()) StartupErrorMessages->Text += "> Motor Tilt Nanoj initialization Failed\n";
			return true;
		}

		labelMotorTiltActivity->Text = "CONNECTED AND CONFIGURED";
		labelMotorTiltActivity->ForeColor = Color::LightGreen;
		return true;

		break;

	}
	return false;
}


bool MainForm::Startup_MotorArm(void) {
	switch (startupSubFase) {

	case 0: // Creates the Body Motor controller process
		labelMotorArmActivity->Text = "CONNECTION ..";
		StartupLogMessages->Text += "> Motor Arm initialization ..\n";
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (!ArmMotor::device->activateConfiguration()) break;

		labelMotorArmActivity->Text = "CONFIGURATION ..";
		StartupLogMessages->Text += "> Motor Arm status:" + ArmMotor::device->getInternalStatusStr();
		StartupLogMessages->Text += "\n";
		startupSubFase++;
		break;

	case 2: // Wait the connection and configuration
		if (ArmMotor::device->isConfigurating()) break;
		if ((!ArmMotor::device->isODConfigured()) || (!ArmMotor::device->isNanojConfigured())) {
			startupError = true;
			labelMotorArmActivity->Text = "CONFIGURATION ERROR";
			labelMotorArmActivity->ForeColor = Color::Red;

			if (!ArmMotor::device->isODConfigured()) StartupErrorMessages->Text += "> Motor Arm Object Dictionary initialization Failed\n";
			if (!ArmMotor::device->isNanojConfigured()) StartupErrorMessages->Text += "> Motor Arm Nanoj initialization Failed\n";
			return true;
		}

		labelMotorArmActivity->Text = "CONNECTED AND CONFIGURED";
		labelMotorArmActivity->ForeColor = Color::LightGreen;
		return true;

		break;

	}
	return false;
}

bool MainForm::Startup_MotorShift(void) {
	switch (startupSubFase) {

	case 0: // Creates the Body Motor controller process
		labelMotorShiftActivity->Text = "CONNECTION ..";
		StartupLogMessages->Text += "> Motor Shift initialization ..\n";
		GlobalObjects::pMotShift = gcnew CanOpenMotor(0x5, L"MOTOR_SHIFT", 453.2);
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (!pMSHIFT->activateConfiguration()) break;

		labelMotorShiftActivity->Text = "CONFIGURATION ..";
		StartupLogMessages->Text += "> Motor Shift status:" + pMSHIFT->getInternalStatusStr();
		StartupLogMessages->Text += "\n";
		startupSubFase++;
		break;

	case 2: // Wait the connection and configuration
		if (pMSHIFT->isConfigurating()) break;
		if ((!pMSHIFT->isODConfigured()) || (!pMSHIFT->isNanojConfigured())) {
			startupError = true;
			labelMotorShiftActivity->Text = "CONFIGURATION ERROR";
			labelMotorShiftActivity->ForeColor = Color::Red;

			if (!pMSHIFT->isODConfigured()) StartupErrorMessages->Text += "> Motor Shift Object Dictionary initialization Failed\n";
			if (!pMSHIFT->isNanojConfigured()) StartupErrorMessages->Text += "> Motor Shift Nanoj initialization Failed\n";
			return true;
		}

		labelMotorShiftActivity->Text = "CONNECTED AND CONFIGURED";
		labelMotorShiftActivity->ForeColor = Color::LightGreen;
		return true;

		break;

	}
	return false;
}

bool MainForm::Startup_MotorVertical(void) {
	switch (startupSubFase) {

	case 0: // Creates the Body Motor controller process
		labelMotorUpDownActivity->Text = "CONNECTION ..";
		StartupLogMessages->Text += "> Motor Up/Down initialization ..\n";
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (!VerticalMotor::device->activateConfiguration()) break;

		labelMotorUpDownActivity->Text = "CONFIGURATION ..";
		StartupLogMessages->Text += "> Motor Up/Down status:" + VerticalMotor::device->getInternalStatusStr();
		StartupLogMessages->Text += "\n";
		startupSubFase++;
		break;

	case 2: // Wait the connection and configuration
		if (VerticalMotor::device->isConfigurating()) break;
		if ((!VerticalMotor::device->isODConfigured()) || (!VerticalMotor::device->isNanojConfigured())) {
			startupError = true;
			labelMotorUpDownActivity->Text = "CONFIGURATION ERROR";
			labelMotorUpDownActivity->ForeColor = Color::Red;

			if (!VerticalMotor::device->isODConfigured()) StartupErrorMessages->Text += "> Motor UpDown Object Dictionary initialization Failed\n";
			if (!VerticalMotor::device->isNanojConfigured()) StartupErrorMessages->Text += "> Motor UpDown Nanoj initialization Failed\n";
			return true;
		}

		labelMotorUpDownActivity->Text = "CONNECTED AND CONFIGURED";
		labelMotorUpDownActivity->ForeColor = Color::LightGreen;
		return true;

		break;

	}
	return false;
}

bool MainForm::Startup_Generator(void) {
	switch (startupSubFase) {

	case 0: // Smart Hub Connection
		labelShActivity->Text = "WAIT SMART HUB CONNECTION..";
		StartupLogMessages->Text += "> Generator: connection with SH ..\n";
		startupSubFase++;
		break;

	case 1: // Wait Generator connection
		if (!Generator::isSmartHubConnected()) break;
		labelShActivity->Text = "CONNECTED AND RUNNING";
		labelShActivity->ForeColor = Color::LightGreen;

		labelGeneratorActivity->Text = "WAIT GENERATOR CONNECTION..";
		StartupLogMessages->Text += "> Generator: connection with generator ..\n";
		startupSubFase++;
		break;

	case 2: // Wait Generator setup
		if (!Generator::isGeneratorConnected()) break;
		labelGeneratorActivity->Text = "WAIT GENERATOR SETUP..";
		StartupLogMessages->Text += "> Generator: setup protocol ..\n";
		startupSubFase++;
		break;

	case 3: // Wait Clear System Messages
		if (!Generator::isGeneratorSetupCompleted()) break;
		labelGeneratorActivity->Text = "WAIT GENERATOR IDLE..";
		StartupLogMessages->Text += "> Generator: configuration ..\n";
		startupSubFase++;
		break;

	case 4: // Wait Generator Idle
		if (!Generator::isGeneratorIdle()) break;
		labelGeneratorActivity->Text = "CONNECTED AND CONFIGURED";
		labelGeneratorActivity->ForeColor = Color::LightGreen;
		return true;
	}
	return false;
}

void MainForm::StartupProcedure(void) {
	
	if (startupError) {
		startupTimer->Stop();
		return;
	}
	if (startupCompleted) {
		GantryStatusRegisters::OperatingStatusRegister::setIdle();
		startupTimer->Stop();
		this->Hide();

		// Opens the Idle
		pIDLEFORM->open();
		return;
	}

	switch (startupFase) {

	case 0: if (Startup_CanDriver()) { startupFase++; startupSubFase = 0; } break; // Startup of the Can Driver process
	case 1: if (Startup_PCB301()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB301 process
	case 2: if (Startup_PCB302()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB302 process
	case 3: if (Startup_PCB315()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB315 process
	case 4: if (Startup_PCB303()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB303 process
	case 5: if (Startup_PCB304()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB304 process
	
	case 6: if (Startup_PCB326()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB315 process
	case 7: if (Startup_MotorTilt()) { startupFase++; startupSubFase = 0; } break; // Startup of the Motor body process
	case 8: if (Startup_MotorArm()) { startupFase++; startupSubFase = 0; } break; // Startup of the Motor body process
	case 9: if (Startup_MotorShift()) { startupFase++; startupSubFase = 0; } break; // Startup of the Motor body process
	case 10: if (Startup_MotorBody()) { startupFase++; startupSubFase = 0; } break; // Startup of the Motor body process
	case 11: if (Startup_MotorVertical()) { startupFase+=2; startupSubFase = 0; } break; // Startup of the Motor body process
	case 12: if (Startup_Generator()) { startupFase++; startupSubFase = 0; } break; // Startup of the Generator process
	case 13:

		// Creates thw AWS
		GlobalObjects::pAws = gcnew awsProtocol(
			SystemConfig::Configuration->getParam(SystemConfig::PARAM_AWS_CONNECTIONS)[SystemConfig::PARAM_AWS_CONNECTIONS_IP],
			Convert::ToInt32(SystemConfig::Configuration->getParam(SystemConfig::PARAM_AWS_CONNECTIONS)[SystemConfig::PARAM_AWS_CONNECTIONS_PORT_COMMAND]),
			Convert::ToInt32(SystemConfig::Configuration->getParam(SystemConfig::PARAM_AWS_CONNECTIONS)[SystemConfig::PARAM_AWS_CONNECTIONS_PORT_EVENTS])
		);
		startupFase++; startupSubFase = 0;
		break;

	case 14:
		startupCompleted = true;
		break;

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
