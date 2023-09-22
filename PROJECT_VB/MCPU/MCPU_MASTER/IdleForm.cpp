#include "pch.h"
#include "IdleForm.h"

#define MAIN_PANEL 1
#define ERROR_PANEL 2


// Main Panel Definition
#define BACKGROUND Image::FromFile("GUI\\IdleForm\\IdleFormBackground.PNG")
#define XRAY_MODE Image::FromFile("GUI\\IdleForm\\XrayMode.PNG")
#define BATTERY_CONNECTED Image::FromFile("GUI\\IdleForm\\Battery\\BatteryConnected.PNG")
#define BATTERY_CONNECTED_LOW Image::FromFile("GUI\\IdleForm\\Battery\\BatteryConnectedLow.PNG")
#define BATTERY_POWERDOWN Image::FromFile("GUI\\IdleForm\\Battery\\BatteryPowerdownFull.PNG")
#define AWS_CONNECTED Image::FromFile("GUI\\IdleForm\\AwsConnected.PNG")
#define PERIPHERALS_CONNECTED Image::FromFile("GUI\\IdleForm\\PeripheralsConnected.PNG")
#define DOOR_CLOSED Image::FromFile("GUI\\IdleForm\\DoorClosed.PNG")
#define TUBE_TEMP_OK Image::FromFile("GUI\\IdleForm\\tubeTempOk.PNG")
#define TUBE_TEMP_NOK Image::FromFile("GUI\\IdleForm\\tubeTempNok.PNG")
#define ERRORPANEL_ERRORICON Image::FromFile("GUI\\Icons\\error_160x145.PNG")
#define ERROR_BUTTON Image::FromFile("GUI\\IdleForm\\AlarmOn.PNG")

// Error Panel Definition
#define ERROR_PANEL_CANC_BUTTON Image::FromFile("GUI\\Icons\\close_65x65.PNG")
#define ERROR_PANEL_NEXT_BUTTON Image::FromFile("GUI\\Icons\\arrow_80x80_r.PNG")


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
		unsigned char currentPanel;
	}Registers;
};


void IdleForm::formInitialization(void) {

	// Initialize the position of the form
	this->Left = GlobalObjects::monitor_X0;
	this->Top = GlobalObjects::monitor_Y0;

	IDLESTATUS::Registers.currentPanel = MAIN_PANEL;
	mainPanel->Show();
	errorPanel->Hide();

	mainPanel->SetBounds(0, 0, 600, 1024);
	mainPanel->BackgroundImage = BACKGROUND;

	this->xrayMode->BackgroundImage = XRAY_MODE;
	this->xrayMode->Hide();

	// Initialize the Installation name
	labelInstallation->Text = SystemConfig::Configuration->getParam(SystemConfig::PARAM_INSTALLATION_NAME)[SystemConfig::PARAM_INSTALLATION_NAME_TOP];

	DateTime date;
	date = DateTime::Now;

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
	
	
	IDLESTATUS::Registers.tube.anode = 0;
	IDLESTATUS::Registers.tube.stator = 0;
	IDLESTATUS::Registers.tube.bulb = 0;
	IDLESTATUS::Registers.tube.alarm = false;

	labelTubeData->Text = "0 %";
	labelTubeData->BackColor = Color::Transparent;

	this->tubeTempOk->BackgroundImage = TUBE_TEMP_OK;
	this->tubeTempOk->BackColor = Color::Transparent;
	this->tubeTempOk->Show();

	// Error Button
	errorButton->BackgroundImage = ERROR_BUTTON;
	errorButton->Hide();

	// Build the Error Panel
	errorPanelTitle->Text = pTRANSLATE->title("ERROR_WINDOW_PANEL");
	errorPanel->SetBounds(0, 0, 600, 1024);
	errpanel_erricon->BackgroundImage = ERRORPANEL_ERRORICON;
	errpanel_erricon->SetBounds(40, 137, 160, 145);
	errorId->Location =  Point(247, 169);
	errorTitle->Location = Point(249, 226);
	errorContent->SetBounds(40,337, 520, 550);
	buttonCanc->SetBounds(518, 10, 70, 70);
	buttonCanc->BackgroundImage = ERROR_PANEL_CANC_BUTTON;
}

void IdleForm::initIdleStatus(void) {

	// Start the startup session
	idleTimer = gcnew System::Timers::Timer(100);
	idleTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &IdleForm::onIdleTimeout);
	idleTimer->Start();

}

void IdleForm::idleStatusManagement(void) {

	DateTime date;
	date = DateTime::Now;
	
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
			pERRORS->activate("DOOR_STUDY_OPEN", false);
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

	if ((GantryStatusRegisters::TubeDataRegister::getBulb() != IDLESTATUS::Registers.tube.bulb) ||
		(GantryStatusRegisters::TubeDataRegister::getStator() != IDLESTATUS::Registers.tube.stator) ||
		(GantryStatusRegisters::TubeDataRegister::getAnode() != IDLESTATUS::Registers.tube.anode)
		) {
		IDLESTATUS::Registers.tube.bulb = GantryStatusRegisters::TubeDataRegister::getBulb();
		IDLESTATUS::Registers.tube.stator = GantryStatusRegisters::TubeDataRegister::getStator();
		IDLESTATUS::Registers.tube.anode = GantryStatusRegisters::TubeDataRegister::getAnode();

		
		int max = IDLESTATUS::Registers.tube.bulb;
		if (IDLESTATUS::Registers.tube.stator > max) max = IDLESTATUS::Registers.tube.stator;
		if (IDLESTATUS::Registers.tube.anode > max) max = IDLESTATUS::Registers.tube.anode;
		labelTubeData->Text = max.ToString() + " %";

		if ((max > 90) && (!IDLESTATUS::Registers.tube.alarm)) {
			IDLESTATUS::Registers.tube.alarm = true;
			tubeTempOk->BackgroundImage = TUBE_TEMP_NOK;
			pERRORS->activate(ERROR_TUBE, false);
		}else if ((max <= 90) && (IDLESTATUS::Registers.tube.alarm)) {
			IDLESTATUS::Registers.tube.alarm = false;
			tubeTempOk->BackgroundImage = TUBE_TEMP_OK;
			pERRORS->deactivate(ERROR_TUBE);
		}

	}

	if (pERRORS->isError()) {
		errorButton->Show();
	}else errorButton->Hide();

	if ((pERRORS->isNewError()) || (pERRORS->isUpdateError())) {
		openErrorWindow(true);
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

void IdleForm::errorButton_Click(System::Object^ sender, System::EventArgs^ e) {
	
	openErrorWindow(true);
}

void IdleForm::openErrorWindow(bool status) {
	static bool local_status = false;
	Translate::item^ msgit;

	// Closing condition
	if ((status == false) && (local_status)) {
		local_status = false;
		mainPanel->Show();
		errorPanel->Hide();
		return;
	}


	// Open Window section
	
	// Set the new error in evidence 
	Errors::item^ newitem = pERRORS->getCurrent();
	if ((!pERRORS->isUpdateError()) && (local_status)) return;  // The panel is already open and no update on the error queue
	pERRORS->clrUpdate();

	local_status = true;

	// Clear the Error content box
	errorContent->Text = "";

	if (newitem) {
		msgit = pTRANSLATE->getItem(newitem->errmsg);

		errorTitle->Text = msgit->title;
		errorId->Text = msgit->id;

		// Populate the first box of the error content box
		errorContent->Text += msgit->id + ": " + msgit->title + "\n";
		errorContent->Text += msgit->content + "\n";
		errorContent->Text += "----------------------------------------------------------------------\n";
	}
	else {
		errorTitle->Text = "";
		errorId->Text = "";
	}
	

	// Populate the remaining boxes of the whole set of errors
	for (int i = 0; i < pERRORS->list->Count; i++) {
		if (i == pERRORS->current) continue;
		msgit = pTRANSLATE->getItem(pERRORS->list[i]->errmsg);

		errorContent->Text += msgit->id + ": " + msgit->title + "\n";
		errorContent->Text += msgit->content + "\n";
		errorContent->Text += "----------------------------------------------------------------------\n";
	}

	pERRORS->clrNewError();
	pERRORS->clrOneShotErrors();

	
	mainPanel->Hide();
	errorPanel->Show();

}

void IdleForm::buttonCanc_Click(System::Object^ sender, System::EventArgs^ e) {
	openErrorWindow(false);
}

