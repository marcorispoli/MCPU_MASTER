#include "pch.h"
#include "IdleForm.h"

#define BACKGROUND Image::FromFile("GUI\\IdleForm\\IdleFormBackground.PNG")
#define XRAY_MODE Image::FromFile("GUI\\IdleForm\\XrayMode.PNG")
#define BATTERY_CONNECTED Image::FromFile("GUI\\IdleForm\\Battery\\BatteryConnected.PNG")
#define BATTERY_CONNECTED_LOW Image::FromFile("GUI\\IdleForm\\Battery\\BatteryConnectedLow.PNG")
#define BATTERY_POWERDOWN Image::FromFile("GUI\\IdleForm\\Battery\\BatteryPowerdownFull.PNG")
#define AWS_CONNECTED Image::FromFile("GUI\\IdleForm\\AwsConnected.PNG")
#define PERIPHERALS_CONNECTED Image::FromFile("GUI\\IdleForm\\PeripheralsConnected.PNG")
#define DOOR_CLOSED Image::FromFile("GUI\\IdleForm\\DoorClosed.PNG")
#define TUBE_TEMP_OK Image::FromFile("GUI\\IdleForm\\TubeTempOk.PNG")

namespace IDLESTATUS {
	typedef struct {

		// Battery Status local variables
		bool battena;
		bool batt1low;
		bool batt2low;
		unsigned char vbatt1;
		unsigned char vbatt2;

	}batteryStatus;

	static struct {
		batteryStatus battery;
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
	this->BackgroundImage = BACKGROUND;
	
	this->xrayMode->BackgroundImage = XRAY_MODE;
	this->xrayMode->Hide();

	// Handle the Powerdown
	IDLESTATUS::Registers.powerdown = GantryStatusRegisters::PowerStatusRegister::getPowerdown();

	// Handle The Battery status
	GantryStatusRegisters::PowerStatusRegister::getBatteryData(&IDLESTATUS::Registers.battery.battena, &IDLESTATUS::Registers.battery.batt1low, &IDLESTATUS::Registers.battery.batt2low, &IDLESTATUS::Registers.battery.vbatt1, &IDLESTATUS::Registers.battery.vbatt2);

	// Set the proper status to the Battery Icon
	if (IDLESTATUS::Registers.powerdown) {
		this->batteryConnected->Show();
		this->batteryConnected->BackgroundImage = BATTERY_POWERDOWN;
	}else if (IDLESTATUS::Registers.battery.battena) {
		if((IDLESTATUS::Registers.battery.batt1low) ||(IDLESTATUS::Registers.battery.batt2low)) this->batteryConnected->BackgroundImage = BATTERY_CONNECTED_LOW;
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
	if(!IDLESTATUS::Registers.closed_door) this->doorClosed->Hide();
	else this->doorClosed->Show();
	

	this->tubeTempOk->BackgroundImage = TUBE_TEMP_OK;
	this->tubeTempOk->Hide();


}

void IdleForm::initIdleStatus(void) {

	// Start the startup session
	idleTimer = gcnew System::Timers::Timer(100);
	idleTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &IdleForm::onIdleTimeout);
	idleTimer->Start();

}

void IdleForm::idleStatusManagement(void) {

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
		if (!IDLESTATUS::Registers.closed_door) this->doorClosed->Hide();
		else this->doorClosed->Show();
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
		((pFW301) && (!pFW301->isTmo())) &&
		((pFW302) && (!pFW302->isTmo())) &&
		((pFW303) && (!pFW303->isTmo())) &&
		((pFW304) && (!pFW304->isTmo())) &&
		((pFW315) && (!pFW315->isTmo()))
		)	peripherals_connected = true;

	if (IDLESTATUS::Registers.peripherals_connected != peripherals_connected) {
		IDLESTATUS::Registers.peripherals_connected = peripherals_connected;
		if(IDLESTATUS::Registers.peripherals_connected) this->peripheralsConnected->Show();
		else this->peripheralsConnected->Hide();
	}
	
}

void IdleForm::WndProc(System::Windows::Forms::Message% m)
{
	switch (m.Msg) {

	case (WM_USER + 1): // onIdleTimeout
		
		idleStatusManagement();
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
