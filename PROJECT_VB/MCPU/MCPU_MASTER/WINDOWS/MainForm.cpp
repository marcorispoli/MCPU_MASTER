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
#include "SlideMotor.h"
#include "CanDriver.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB304.h"
#include "PCB315.h"
#include "PCB326.h"
#include "ExposureModule.h"
#include "Log.h"



using namespace CppCLRWinFormsProject;
using namespace CANOPEN;

#define STARTUP_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\Cybele.PNG")


void MainForm::MainFormInitialize(void) {

	// Initialize the position of the form
	this->Left = Gantry::monitor_X0;
	this->Top = Gantry::monitor_Y0;
	

	// Debug on the System Configuration File
	if (SystemConfig::Configuration->isWarning()) {
		LogClass::logInFile("SYSTEM CONFIGURATION FILE:" + SystemConfig::Configuration->getWarningString());
	}

	// Initialize the Package field 
	if (SystemConfig::Configuration->isWarning()) {
		LogClass::logInFile( "> " + SystemConfig::Configuration->getWarningString());
	}

	// Program Icon
	startupIcon->BackgroundImage = STARTUP_IMAGE;

	infoPanel->Text = "STARTUP PACKAGE ID: " + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_ID];
	LogClass::logInFile("Package Installed: ID" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_ID]);
	LogClass::logInFile("FW301 Installed:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FW301]);
	LogClass::logInFile("FW302 Installed:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FW302]);
	LogClass::logInFile("FW303 Installed:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FW303]);
	LogClass::logInFile("FW304 Installed:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FW304]);
	LogClass::logInFile("FW315 Installed:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FW315]);
	LogClass::logInFile("FW-CAN Installed:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FWCAN]);
	LogClass::logInFile("FW-GENERATOR Installed:" + SystemConfig::Configuration->getParam(SystemConfig::PARAM_PACKAGE)[SystemConfig::PARAM_PACKAGE_FWGEN]);

	
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
	Gantry::setStartup();

	// Set the motor in safety condition
	PCB301::setMotorPowerSupply(true);
	PCB301::setMotorSwitch(true);

	// Start the startup session
	startupTimer = gcnew System::Timers::Timer(100);
	startupTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &MainForm::onStartupTimeout);
	startupTimer->Start();
	
	
}



bool MainForm::Startup_CanDriver(void) {
	System::String^ string;

	switch (startupSubFase) {

	case 0: // Creates the Can Driver Object
		if (Gantry::isCanDriverDemo()) {
			CanDriver::startSimulatorMode();
			labelCanDriverActivity->Text = "DEMO RUNNING";
			labelCanDriverActivity->ForeColor = Color::LightGreen;
			return true;
		}
		else CanDriver::startNormalMode();
		labelCanDriverActivity->Text = "Connection ..";
		startupSubFase++;
		break;

	case 1:
		if (CanDriver::isError()) {
			labelCanDriverActivity->Text = "STARTUP ERROR!";
			LogClass::logInFile("can driver error : " + CanDriver::getErrorString());
			startupError = true;
			break;
		}

		if (CanDriver::isConnected(false)) {
			labelCanDriverActivity->Text = "RUN IN NORMAL MODE";
			string =  "can driver revision: ";
			string += " API:" + CanDriver::apirev_maj.ToString() + "." + CanDriver::apirev_min.ToString();
			string += " SW:" + CanDriver::swrev_maj.ToString() + "." + CanDriver::swrev_min.ToString();
			string += " HW:" + CanDriver::hwrev_maj.ToString() + "." + CanDriver::hwrev_min.ToString();			
			LogClass::logInFile(string);
			labelCanDriverActivity->ForeColor = Color::LightGreen;
			return true; // Completed
		}

		break;
	
	}

	return false;
}

bool MainForm::Startup_PCB301(void) {
	System::String^ string;

	switch (startupSubFase) {

	case 0: // Creates the PCB301 process
		labelPcb301Activity->Text = "CONNECTION ..";
		string =  "pcb301 initialization ..";
		LogClass::logInFile(string);
		if (Gantry::isPcb301Demo()) PCB301::device->simulMode();
		else PCB301::device->runMode();
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (PCB301::device->isSimulatorMode()) {
			labelPcb301Activity->Text = "RUN IN SIMULATION MODE";
			labelPcb301Activity->ForeColor = Color::LightGreen;
			return true;
		}

		if (PCB301::device->getModuleStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb301Activity->Text = "CONFIGURATION ..";
			string =  "pcb301 firmware revision: ";
			string += " BOOT:" + PCB301::device->getBootStatus().ToString() + ", REV:" + PCB301::device->getBootRevision();
			string += " APP:" + PCB301::device->getAppRevision();
			LogClass::logInFile(string);
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (PCB301::device->getModuleStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb301Activity->Text = "RUN IN NORMAL MODE";
			labelPcb301Activity->ForeColor = Color::LightGreen;
			return true;
		}

		break;

	}
	return false;
}


bool MainForm::Startup_PCB302(void) {
	System::String^ string;

	switch (startupSubFase) {

	case 0: // Creates the PCB302 process
		labelPcb302Activity->Text = "CONNECTION ..";
		string = "pcb302 initialization ..\n";		
		LogClass::logInFile(string);

		if (Gantry::isPcb302Demo()) PCB302::device->simulMode();
		else PCB302::device->runMode();
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (PCB302::device->isSimulatorMode()) {
			labelPcb302Activity->Text = "RUN IN SIMULATION MODE";
			labelPcb302Activity->ForeColor = Color::LightGreen;
			return true;
		}

		if (PCB302::device->getModuleStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb302Activity->Text = "CONFIGURATION ..";
			string = "pcb302 firmware revision: ";
			string += " BOOT:" + PCB302::device->getBootStatus().ToString() + ", REV:" + PCB302::device->getBootRevision();
			string += " APP:" + PCB302::device->getAppRevision();
			LogClass::logInFile(string);
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (PCB302::device->getModuleStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb302Activity->Text = "RUN IN NORMAL MODE";
			labelPcb302Activity->ForeColor = Color::LightGreen;
			return true;
		}

		break;

	}
	return false;
}

bool MainForm::Startup_PCB303(void) {
	System::String^ string;

	switch (startupSubFase) {

	case 0: // Creates the PCB303 process
		labelPcb303Activity->Text = "CONNECTION ..";
		string = "pcb303 initialization ..";
		LogClass::logInFile(string);
		if (Gantry::isPcb303Demo()) PCB303::device->simulMode();
		else PCB303::device->runMode();
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (PCB303::device->isSimulatorMode()) {
			labelPcb303Activity->Text = "RUN IN SIMULATION MODE";
			labelPcb303Activity->ForeColor = Color::LightGreen;
			return true;
		}

		if (PCB303::device->getModuleStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb303Activity->Text = "CONFIGURATION ..";
			string = "pcb303 firmware revision: ";
			string += " BOOT:" + PCB303::device->getBootStatus().ToString() + ", REV:" + PCB303::device->getBootRevision();
			string += " APP:" + PCB303::device->getAppRevision();
			LogClass::logInFile(string);
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (PCB303::device->getModuleStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb303Activity->Text = "RUN IN NORMAL MODE";
			labelPcb303Activity->ForeColor = Color::LightGreen;
			return true;
		}

		break;

	}
	return false;
}

bool MainForm::Startup_PCB304(void) {
	System::String^ string;

	switch (startupSubFase) {

	case 0: // Creates the PCB304 process
		labelPcb304Activity->Text = "CONNECTION ..";
		string = "pcb304 initialization ..";
		LogClass::logInFile(string);
		if (Gantry::isPcb304Demo()) PCB304::device->simulMode();
		else PCB304::device->runMode();
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (PCB304::device->isSimulatorMode()) {
			labelPcb304Activity->Text = "RUN IN SIMULATION MODE";
			labelPcb304Activity->ForeColor = Color::LightGreen;
			return true;
		}

		if (PCB304::device->getModuleStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb304Activity->Text = "CONFIGURATION ..";
			string = "pcb304 firmware revision: ";
			string += " BOOT:" + PCB304::device->getBootStatus().ToString() + ", REV:" + PCB304::device->getBootRevision();
			string += " APP:" + PCB304::device->getAppRevision();
			LogClass::logInFile(string);
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (PCB304::device->getModuleStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb304Activity->Text = "RUN IN NORMAL MODE";
			labelPcb304Activity->ForeColor = Color::LightGreen;
			return true;
		}

		break;

	}
	return false;
}

bool MainForm::Startup_PCB315(void) {
	System::String^ string;

	switch (startupSubFase) {

	case 0: // Creates the PCB315 process
		labelPcb315Activity->Text = "CONNECTION ..";
		string = "pcb315 initialization ..";		
		LogClass::logInFile(string);
		if (Gantry::isPcb315Demo()) PCB315::device->simulMode();
		else PCB315::device->runMode();
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (PCB315::device->isSimulatorMode()) {
			labelPcb315Activity->Text = "RUN IN SIMULATION MODE";
			labelPcb315Activity->ForeColor = Color::LightGreen;
			return true;
		}

		if (PCB315::device->getModuleStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb315Activity->Text = "CONFIGURATION ..";
			string = "pcb315 firmware revision: ";
			string += " BOOT:" + PCB315::device->getBootStatus().ToString() + ", REV:" + PCB315::device->getBootRevision();
			string += " APP:" + PCB315::device->getAppRevision();
			LogClass::logInFile(string);
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (PCB315::device->getModuleStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb315Activity->Text = "RUN IN NORMAL MODE";
			labelPcb315Activity->ForeColor = Color::LightGreen;
			return true;
		}

		break;

	}
	return false;
}

bool MainForm::Startup_PCB326(void) {
	System::String^ string;
	

	switch (startupSubFase) {

	case 0: // Creates the PCB315 process
		labelPcb326Activity->Text = "CONNECTION ..";
		string = "pcb326 initialization ..";	
		LogClass::logInFile(string);
		if (Gantry::isPcb326Demo()) PCB326::device->simulMode();
		else PCB326::device->runMode();
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (PCB326::device->isSimulatorMode()) {
			labelPcb326Activity->Text = "RUN IN SIMULATION MODE";
			labelPcb326Activity->ForeColor = Color::LightGreen;
			return true;
		}

		if (PCB326::device->getModuleStatus() > CanDeviceProtocol::status_options::WAITING_REVISION) {
			labelPcb326Activity->Text = "CONFIGURATION ..";
			string = "pcb326 firmware revision: ";
			string += " BOOT:" + PCB326::device->getBootStatus().ToString() + ", REV:" + PCB326::device->getBootRevision();
			string += " APP:" + PCB326::device->getAppRevision();
			LogClass::logInFile(string);
			startupSubFase++;
		}
		break;

	case 2: // Wait the connection and configuration		
		if (PCB326::device->getModuleStatus() == CanDeviceProtocol::status_options::DEVICE_RUNNING) {
			labelPcb326Activity->Text = "RUN IN NORMAL MODE";
			labelPcb326Activity->ForeColor = Color::LightGreen;
			return true;
		}

		break;

	}
	return false;
}

bool MainForm::Startup_MotorBody(void) {
	System::String^ string;

	switch (startupSubFase) {

	case 0: // Creates the Body Motor controller process
		labelMotorBodyActivity->Text = "CONNECTION ..";
		string = "Motor Body initialization ..";	
		LogClass::logInFile(string);

		if (Gantry::isMotorBodyDemo()) {
			BodyMotor::device->demoMode();
			labelMotorBodyActivity->Text = "RUN IN SIMULATION MODE";
			labelMotorBodyActivity->ForeColor = Color::LightGreen;
			return true;
		}else BodyMotor::device->runMode();
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration		
		if (!BodyMotor::device->activateConfiguration()) break;

		labelMotorBodyActivity->Text = "CONFIGURATION ..";
		string = "motor body status:" + BodyMotor::device->getInternalStatusStr();
		LogClass::logInFile(string);
		startupSubFase++;
		break;

	case 2: // Wait the connection and configuration
		if (BodyMotor::device->isConfigurating()) break;
		if ((!BodyMotor::device->isODConfigured()) || (!BodyMotor::device->isNanojConfigured())) {
			startupError = true;
			labelMotorBodyActivity->Text = "CONFIGURATION ERROR";
			labelMotorBodyActivity->ForeColor = Color::Red;
			string = "";
			if (!BodyMotor::device->isODConfigured()) string += "Motor Body Object Dictionary initialization Failed\n";
			if (!BodyMotor::device->isNanojConfigured()) string += "Motor Body Nanoj initialization Failed\n";
			LogClass::logInFile(string);
			return true;
		}
		
		labelMotorBodyActivity->Text = "RUN IN NORMAL MODE";
		labelMotorBodyActivity->ForeColor = Color::LightGreen;
		return true;
		
		break;

	}
	return false;
}

bool MainForm::Startup_MotorTilt(void) {
	System::String^ string;

	switch (startupSubFase) {

	case 0: // Creates the Body Motor controller process
		labelMotorTiltActivity->Text = "CONNECTION ..";
		string = "Motor Tilt initialization ..";
		LogClass::logInFile(string);

		if (Gantry::isMotorTiltDemo()) {
			TiltMotor::device->demoMode();
			labelMotorTiltActivity->Text = "RUN IN SIMULATION MODE";
			labelMotorTiltActivity->ForeColor = Color::LightGreen;
			return true;
		}
		else TiltMotor::device->runMode();
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (!TiltMotor::device->activateConfiguration()) break;

		labelMotorTiltActivity->Text = "CONFIGURATION ..";
		string = "Motor Tilt status:" + TiltMotor::device->getInternalStatusStr();
		LogClass::logInFile(string);
		startupSubFase++;
		break;

	case 2: // Wait the connection and configuration
		if (TiltMotor::device->isConfigurating()) break;
		if ( (!TiltMotor::device->isODConfigured()) || (!TiltMotor::device->isNanojConfigured())) {
			startupError = true;
			labelMotorTiltActivity->Text = "CONFIGURATION ERROR";
			labelMotorTiltActivity->ForeColor = Color::Red;
			string = "";
			if(!TiltMotor::device->isODConfigured()) string += "Motor Tilt Object Dictionary initialization Failed\n";
			if (!TiltMotor::device->isNanojConfigured()) string += "Motor Tilt Nanoj initialization Failed\n";
			LogClass::logInFile(string);
			return true;
		}

		labelMotorTiltActivity->Text = "RUN IN NORMAL MODE";
		labelMotorTiltActivity->ForeColor = Color::LightGreen;
		return true;

		break;

	}
	return false;
}


bool MainForm::Startup_MotorArm(void) {
	System::String^ string;

	switch (startupSubFase) {

	case 0: // Creates the Body Motor controller process
		labelMotorArmActivity->Text = "CONNECTION ..";
		string = "Motor Arm initialization ..";
		LogClass::logInFile(string);

		if (Gantry::isMotorArmDemo()) {
			ArmMotor::device->demoMode();
			labelMotorArmActivity->Text = "RUN IN SIMULATION MODE";
			labelMotorArmActivity->ForeColor = Color::LightGreen;
			return true;
		}
		else ArmMotor::device->runMode();
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (!ArmMotor::device->activateConfiguration()) break;

		labelMotorArmActivity->Text = "CONFIGURATION ..";
		string = "Motor Arm status:" + ArmMotor::device->getInternalStatusStr();
		LogClass::logInFile(string);
		startupSubFase++;
		break;

	case 2: // Wait the connection and configuration
		if (ArmMotor::device->isConfigurating()) break;
		if ((!ArmMotor::device->isODConfigured()) || (!ArmMotor::device->isNanojConfigured())) {
			startupError = true;
			labelMotorArmActivity->Text = "CONFIGURATION ERROR";
			labelMotorArmActivity->ForeColor = Color::Red;
			string = "";
			if (!ArmMotor::device->isODConfigured()) string += "Motor Arm Object Dictionary initialization Failed\n";
			if (!ArmMotor::device->isNanojConfigured()) string += " Motor Arm Nanoj initialization Failed\n";
			LogClass::logInFile(string);
			return true;
		}

		labelMotorArmActivity->Text = "RUN IN NORMAL MODE";
		labelMotorArmActivity->ForeColor = Color::LightGreen;
		return true;

		break;

	}
	return false;
}

bool MainForm::Startup_MotorShift(void) {
	System::String^ string;

	switch (startupSubFase) {

	case 0: // Creates the Body Motor controller process
		labelMotorShiftActivity->Text = "CONNECTION ..";
		string = "Motor Slide initialization ..";	
		LogClass::logInFile(string);
		if (Gantry::isMotorSlideDemo()) {
			SlideMotor::device->demoMode();
			labelMotorShiftActivity->Text = "RUN IN SIMULATION MODE";
			labelMotorShiftActivity->ForeColor = Color::LightGreen;
			return true;
		}
		else SlideMotor::device->runMode();
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (!SlideMotor::device->activateConfiguration()) break;
		

		labelMotorShiftActivity->Text = "CONFIGURATION ..";
		string = "Motor Slide status:" + SlideMotor::device->getInternalStatusStr();
		LogClass::logInFile(string);
		startupSubFase++;
		break;

	case 2: // Wait the connection and configuration
		if (SlideMotor::device->isConfigurating()) break;
		if ((!SlideMotor::device->isODConfigured()) || (!SlideMotor::device->isNanojConfigured())) {
			startupError = true;
			labelMotorShiftActivity->Text = "CONFIGURATION ERROR";
			labelMotorShiftActivity->ForeColor = Color::Red;
			string = "";
			if (!SlideMotor::device->isODConfigured()) string += "Motor Slide Object Dictionary initialization Failed\n";
			if (!SlideMotor::device->isNanojConfigured()) string += "Motor Slide Nanoj initialization Failed\n";
			LogClass::logInFile(string);

			return true;
		}

		labelMotorShiftActivity->Text = "RUN IN NORMAL MODE";
		labelMotorShiftActivity->ForeColor = Color::LightGreen;
		return true;

		break;

	}
	return false;
}


bool MainForm::Startup_MotorVertical(void) {
	System::String^ string;

	switch (startupSubFase) {

	case 0: // Creates the Body Motor controller process
		labelMotorUpDownActivity->Text = "CONNECTION ..";
		string = "Motor Vertical initialization ..\n";
		LogClass::logInFile(string);

		if (Gantry::isMotorVerticalDemo()) {
			VerticalMotor::device->demoMode();
			labelMotorUpDownActivity->Text = "RUN IN SIMULATION MODE";
			labelMotorUpDownActivity->ForeColor = Color::LightGreen;
			return true;
		}
		else VerticalMotor::device->runMode();
		startupSubFase++;
		break;

	case 1: // Wait the connection and configuration
		if (!VerticalMotor::device->activateConfiguration()) break;

		labelMotorUpDownActivity->Text = "CONFIGURATION ..";
		string = "Motor Vertical status:" + VerticalMotor::device->getInternalStatusStr();
		LogClass::logInFile(string);
		startupSubFase++;
		break;

	case 2: // Wait the connection and configuration
		if (VerticalMotor::device->isConfigurating()) break;
		if ((!VerticalMotor::device->isODConfigured()) || (!VerticalMotor::device->isNanojConfigured())) {
			startupError = true;
			labelMotorUpDownActivity->Text = "CONFIGURATION ERROR";
			labelMotorUpDownActivity->ForeColor = Color::Red;
			string = "";
			if (!VerticalMotor::device->isODConfigured()) string += "Motor Vertical Object Dictionary initialization Failed\n";
			if (!VerticalMotor::device->isNanojConfigured()) string += "Motor Vertical Nanoj initialization Failed\n";
			LogClass::logInFile(string);
			return true;
		}

		labelMotorUpDownActivity->Text = "RUN IN NORMAL MODE";
		labelMotorUpDownActivity->ForeColor = Color::LightGreen;
		return true;

		break;

	}
	return false;
}


bool MainForm::Startup_Generator(void) {
	System::String^ string;

	switch (startupSubFase) {

	case 0: 

		if (Gantry::isGeneratorDemo()) {
			Exposures::startSimulator();
			return true;
		}
		else Exposures::startGenerator();

		// Smart Hub Connection
		labelShActivity->Text = "WAIT SMART HUB CONNECTION..";
		string = "Generator: connection with SH ..\n";
		LogClass::logInFile(string);
		startupSubFase++;
		break;

	case 1: // Wait Generator connection
		if (!Exposures::isSmartHubConnected()) break;
		labelShActivity->Text = "CONNECTED AND RUNNING";
		labelShActivity->ForeColor = Color::LightGreen;

		labelGeneratorActivity->Text = "WAIT GENERATOR CONNECTION..";
		string = "Generator: connection with generator ..";
		LogClass::logInFile(string);
		startupSubFase++;
		break;

	case 2: // Wait Generator setup
		if (!Exposures::isGeneratorConnected()) break;
		labelGeneratorActivity->Text = "WAIT GENERATOR SETUP..";
		string = "Generator: setup protocol ..";
		LogClass::logInFile(string);
		startupSubFase++;
		break;

	case 3: // Wait Generator Idle		
		if (!Exposures::isGeneratorIdle()) break;
		startupSubFase++;
		break;

	case 4: // Wait Generator Idle
		labelGeneratorActivity->Text = "RUN IN NORMAL MODE";
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
		startupTimer->Stop();
		this->Hide();
		Gantry::setIdle();
		return;
	}

	switch (startupFase) {

	case 0: if (Startup_CanDriver()) { startupFase++; startupSubFase = 0; } break; // Startup of the Can Driver process
	case 1: if (Startup_PCB301()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB301 process
	case 2: if (Startup_PCB302()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB302 process
	case 3: if (Startup_PCB303()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB303 process
	case 4: if (Startup_PCB315()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB315 process
	case 5: if (Startup_PCB304()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB304 process
	case 6: if (Startup_PCB326()) { startupFase++; startupSubFase = 0; } break; // Startup of the PCB315 process
	case 7: if (Startup_Generator()) { startupFase++; startupSubFase = 0; } break; // Startup of the Generator process
	case 8: if (Startup_MotorTilt()) { startupFase++; startupSubFase = 0; } break; // Startup of the Motor body process
	case 9: if (Startup_MotorArm()) { startupFase++; startupSubFase = 0; } break; // Startup of the Motor body process
	case 10: if (Startup_MotorShift()) { startupFase++; startupSubFase = 0; } break; // Startup of the Motor body process
	case 11: if (Startup_MotorBody()) { startupFase++; startupSubFase = 0; } break; // Startup of the Motor body process
	case 12: if (Startup_MotorVertical()) { startupFase++; startupSubFase = 0; } break; // Startup of the Motor body process	
	case 13: startupFase++; startupSubFase = 0;
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
