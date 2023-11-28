#include "../AWS/awsProtocol.h"
#include "idleForm.h"
#include "ErrorForm.h"
#include "operatingForm.h"
#include "SystemConfig.h"
#include "Translate.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB304.h"
#include "PCB315.h"
#include "PCB326.h"
#include "../gantry_global_status.h"


// Main Panel Definition
#define BACKGROUND Image::FromFile(GlobalObjects::applicationResourcePath + "IdleForm\\IdleFormBackground.PNG")
#define XRAY_MODE Image::FromFile(GlobalObjects::applicationResourcePath + "IdleForm\\XrayMode.PNG")
#define BATTERY_CONNECTED Image::FromFile(GlobalObjects::applicationResourcePath + "IdleForm\\Battery\\BatteryConnected.PNG")
#define BATTERY_CONNECTED_LOW Image::FromFile(GlobalObjects::applicationResourcePath + "IdleForm\\Battery\\BatteryConnectedLow.PNG")
#define BATTERY_POWERDOWN Image::FromFile(GlobalObjects::applicationResourcePath + "IdleForm\\Battery\\BatteryPowerdownFull.PNG")
#define AWS_CONNECTED Image::FromFile(GlobalObjects::applicationResourcePath + "IdleForm\\AwsConnected.PNG")
#define PERIPHERALS_CONNECTED Image::FromFile(GlobalObjects::applicationResourcePath + "IdleForm\\PeripheralsConnected.PNG")
#define DOOR_CLOSED Image::FromFile(GlobalObjects::applicationResourcePath + "IdleForm\\DoorClosed.PNG")
#define TUBE_TEMP_OK Image::FromFile(GlobalObjects::applicationResourcePath + "IdleForm\\tubeTempOk.PNG")
#define TUBE_TEMP_NOK Image::FromFile(GlobalObjects::applicationResourcePath + "IdleForm\\tubeTempNok.PNG")
#define ERRORPANEL_ERRORICON Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\error_160x145.PNG")
#define ERROR_BUTTON Image::FromFile(GlobalObjects::applicationResourcePath + "IdleForm\\AlarmOn.PNG")

namespace IDLESTATUS {
	typedef struct {

		// Battery Status local variables
		bool battena;
		bool batt1low;
		bool batt2low;
		unsigned char vbatt1;
		unsigned char vbatt2;

	}batteryStatus;

	typedef struct {

		unsigned char anode;
		unsigned char stator;
		unsigned char bulb;
		bool alarm;

	}tubeStatus;

	static struct {
		batteryStatus battery;
		tubeStatus	tube;

		bool powerdown;
		bool closed_door;
		bool aws_connected;
		bool peripherals_connected;
		
	}Registers;
};


void IdleForm::formInitialization(void) {

	// Initialize the position of the form
	this->Left = GlobalObjects::monitor_X0;
	this->Top = GlobalObjects::monitor_Y0;

	mainPanel->SetBounds(0, 0, 600, 1024);
	mainPanel->BackgroundImage = BACKGROUND;

	// Error Panel Setup ____________________________________________________________
	pError = gcnew ErrorForm(this);
	//________________________________________________________________________________________

	this->xrayMode->BackgroundImage = XRAY_MODE;
	this->xrayMode->Hide();

	// Initialize the Installation name
	labelInstallation->Text = SystemConfig::Configuration->getParam(SystemConfig::PARAM_INSTALLATION_NAME)[SystemConfig::PARAM_INSTALLATION_NAME_TOP];


	// Error Button
	errorButton->BackgroundImage = ERROR_BUTTON;
	errorButton->Hide();


	idleTimer = gcnew System::Timers::Timer(100);
	idleTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &IdleForm::onIdleTimeout);
	idleTimer->Stop();

	this->Hide();
	open_status = false;
}

void IdleForm::initIdleStatus(void) {
	System::DateTime date;
	date = System::DateTime::Now;

	labelDate->Text = date.Day + ":" + date.Month + ":" + date.Year;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	// Handle the Powerdown
	IDLESTATUS::Registers.powerdown = GantryStatusRegisters::PowerStatusRegister::getPowerdown();

	// Handle The Battery status
	GantryStatusRegisters::PowerStatusRegister::getBatteryData(&IDLESTATUS::Registers.battery.battena, &IDLESTATUS::Registers.battery.batt1low, &IDLESTATUS::Registers.battery.batt2low, &IDLESTATUS::Registers.battery.vbatt1, &IDLESTATUS::Registers.battery.vbatt2);

	// Set the proper status to the Battery Icon
	if (IDLESTATUS::Registers.powerdown) {
		this->batteryConnected->Show();
		this->batteryConnected->BackgroundImage = BATTERY_POWERDOWN;
	}
	else if (IDLESTATUS::Registers.battery.battena) {
		if ((IDLESTATUS::Registers.battery.batt1low) || (IDLESTATUS::Registers.battery.batt2low)) this->batteryConnected->BackgroundImage = BATTERY_CONNECTED_LOW;
		else this->batteryConnected->BackgroundImage = BATTERY_CONNECTED;
		this->batteryConnected->Show();
	}
	else {
		this->batteryConnected->Hide();
	}

	IDLESTATUS::Registers.aws_connected = false;
	this->awsConnected->BackgroundImage = AWS_CONNECTED;
	this->awsConnected->Hide();

	IDLESTATUS::Registers.peripherals_connected = false;
	this->peripheralsConnected->BackgroundImage = PERIPHERALS_CONNECTED;
	this->peripheralsConnected->Hide();

	IDLESTATUS::Registers.closed_door = GantryStatusRegisters::SafetyStatusRegister::getCloseDoor();
	this->doorClosed->BackgroundImage = DOOR_CLOSED;
	if (!IDLESTATUS::Registers.closed_door) this->doorClosed->Hide();
	else this->doorClosed->Show();


	IDLESTATUS::Registers.tube.anode = 0;
	IDLESTATUS::Registers.tube.stator = 0;
	IDLESTATUS::Registers.tube.bulb = 0;
	IDLESTATUS::Registers.tube.alarm = false;

	labelTubeData->Text = "0 %";
	labelTubeData->BackColor = Color::Transparent;

	this->tubeTempOk->BackgroundImage = TUBE_TEMP_OK;
	this->tubeTempOk->BackColor = Color::Transparent;
	this->tubeTempOk->Show();
	

	// Sets the current collimation to OPEN mode
	PCB303::setOpenCollimationMode();

	// Sets the current Filter selector to manual mode
	PCB315::setFilterManualMode(PCB315::filterMaterialCodes::FILTER_DEFAULT);


	// Start the startup session	
	idleTimer->Start();	

	


}

void IdleForm::open(void) {
	if (open_status) return;
	open_status = true;
	initIdleStatus();

	
	this->Show();
}

void IdleForm::close(void) {
	if (!open_status) return;
	open_status = false;
	idleTimer->Stop();

	((ErrorForm^)pError)->close();
	this->Hide();
}

void IdleForm::idleStatusManagement(void) {

	System::DateTime date;
	date = System::DateTime::Now;
	
	// Verifies if there is a StudyOpen condition
	if (GantryStatusRegisters::OperatingStatusRegister::isOPEN()) {
		this->close();
		pOPERFORM->open();
		return;
	}

	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	IDLESTATUS::batteryStatus battery;
	bool powerdown;
	bool powerstatus_chg = false;

	// Handle The Battery status
	GantryStatusRegisters::PowerStatusRegister::getBatteryData(&battery.battena, &battery.batt1low, &battery.batt2low, &battery.vbatt1, &battery.vbatt2);
	powerdown = GantryStatusRegisters::PowerStatusRegister::getPowerdown();

	if (IDLESTATUS::Registers.powerdown != powerdown) {
		IDLESTATUS::Registers.powerdown = powerdown;
		powerstatus_chg = true;
	}

	if ((IDLESTATUS::Registers.battery.battena != battery.battena) ||
		(IDLESTATUS::Registers.battery.batt1low != battery.batt1low) ||
		(IDLESTATUS::Registers.battery.batt2low != battery.batt2low)) {

		IDLESTATUS::Registers.battery = battery;
		powerstatus_chg = true;
	}

	if (powerstatus_chg) {
		if (powerdown) {
			this->batteryConnected->Show();
			this->batteryConnected->BackgroundImage = BATTERY_POWERDOWN;
		}
		else if (battery.battena) {
			if ((battery.batt1low) || (battery.batt2low)) this->batteryConnected->BackgroundImage = BATTERY_CONNECTED_LOW;
			else this->batteryConnected->BackgroundImage = BATTERY_CONNECTED;
			this->batteryConnected->Show();
		}else 
		{
			this->batteryConnected->Hide();
		}
	}

	// Handles the Door Closed status
	if (IDLESTATUS::Registers.closed_door != GantryStatusRegisters::SafetyStatusRegister::getCloseDoor())
	{
		IDLESTATUS::Registers.closed_door = GantryStatusRegisters::SafetyStatusRegister::getCloseDoor();
		if (!IDLESTATUS::Registers.closed_door) {
			this->doorClosed->Hide();			
		}
		else {
			this->doorClosed->Show();			
		}
	}

	// Handles the AWS connection status
	if ((pAWS == nullptr) || (pAWS->isConnected() != IDLESTATUS::Registers.aws_connected)) {
		if (pAWS == nullptr) IDLESTATUS::Registers.aws_connected = false;
		else IDLESTATUS::Registers.aws_connected = pAWS->isConnected();

		if(IDLESTATUS::Registers.aws_connected) this->awsConnected->Show();
		else this->awsConnected->Hide();
	}

	// Handles the Device connection status
	bool peripherals_connected = false;
	if (
		(!PCB301::device->isTmo()) && 
		(!PCB302::device->isTmo()) &&
		(!PCB303::device->isTmo()) &&
		(!PCB304::device->isTmo()) &&
		(!PCB315::device->isTmo())&&
		(!PCB326::device->isTmo())
		)	peripherals_connected = true;

	if (IDLESTATUS::Registers.peripherals_connected != peripherals_connected) {
		IDLESTATUS::Registers.peripherals_connected = peripherals_connected;
		if(IDLESTATUS::Registers.peripherals_connected) this->peripheralsConnected->Show();
		else this->peripheralsConnected->Hide();
	}

	// Evaluates the maximum tube temperature
	int val = PCB315::getAnode();
	if (PCB315::getBulb() > val) val = PCB315::getBulb();
	if (PCB315::getStator() > val) val = PCB315::getStator();
	labelTubeData->Text = val.ToString() + " %";

	// Sets the background color based on the temperature error condition
	if (PCB315::isTubeAlarm() != IDLESTATUS::Registers.tube.alarm) {
		IDLESTATUS::Registers.tube.alarm = PCB315::isTubeAlarm();
		if (IDLESTATUS::Registers.tube.alarm) tubeTempOk->BackgroundImage = TUBE_TEMP_NOK;
		else tubeTempOk->BackgroundImage = TUBE_TEMP_OK;
	}

	if ((Notify::isError()) || (Notify::isWarning())) {
		errorButton->Show();
	}
	else errorButton->Hide();

}

void IdleForm::WndProc(System::Windows::Forms::Message% m)
{
	switch (m.Msg) {

	case (WM_USER + 1): // onIdleTimeout
		
		idleStatusManagement();
		break;
	}

	Form::WndProc(m);
}

void IdleForm::errorButton_Click(System::Object^ sender, System::EventArgs^ e) {
	((ErrorForm^)pError)->open();
}
