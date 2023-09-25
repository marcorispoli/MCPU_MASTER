#include "pch.h"


// Window Panels 
#define MAIN_PANEL			1
#define ERROR_PANEL			2


// Main Panel Definition
#define BACKGROUND Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\OperatingFormBackground.PNG")
#define ERRORPANEL_ERRORICON Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\error_160x145.PNG")
#define ERROR_BUTTON Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\AlarmOn.PNG")

// Error Panel Definition
#define ERROR_PANEL_CANC_BUTTON Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\close_65x65.PNG")
#define ERROR_PANEL_NEXT_BUTTON Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\arrow_80x80_r.PNG")

// Error Window timeout
#define ERROR_WINDOW_TMO	15000

namespace OPERSTATUS {
	
	typedef struct {

		unsigned char anode;
		unsigned char stator;
		unsigned char bulb;
		bool alarm;

	}tubeStatus;

	static struct {
		
		tubeStatus	tube;
		bool powerdown;
		bool closed_door;
		unsigned char currentPanel;
	}Registers;
};


void OperatingForm::formInitialization(void) {

	// Initialize the position of the form
	this->Left = GlobalObjects::monitor_X0;
	this->Top = GlobalObjects::monitor_Y0;

	OPERSTATUS::Registers.currentPanel = MAIN_PANEL;
	mainPanel->Show();
	errorPanel->Hide();

	mainPanel->SetBounds(0, 0, 600, 1024);
	mainPanel->BackgroundImage = BACKGROUND;

	
	// Error Button
	errorButton->BackgroundImage = ERROR_BUTTON;
	errorButton->Hide();

	// Build the Error Panel
	errorPanelTitle->Text = Translate::title("ERROR_WINDOW_PANEL");
	errorPanel->SetBounds(0, 0, 600, 1024);
	errpanel_erricon->BackgroundImage = ERRORPANEL_ERRORICON;
	errpanel_erricon->SetBounds(40, 137, 160, 145);
	errorId->Location =  Point(247, 169);
	errorTitle->Location = Point(249, 226);
	errorContent->SetBounds(40,337, 520, 550);
	buttonCanc->SetBounds(518, 10, 70, 70);
	buttonCanc->BackgroundImage = ERROR_PANEL_CANC_BUTTON;

	operatingTimer = gcnew System::Timers::Timer(100);
	operatingTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &OperatingForm::onOperatingTimeout);
	operatingTimer->Stop();

	errorPanelTimer = gcnew System::Timers::Timer(2000);
	errorPanelTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &OperatingForm::onErrorTimeout);
	errorPanelTimer->Stop();

}

void OperatingForm::initOperatingStatus(void) {

	// Initialize Date and time
	DateTime date;
	date = DateTime::Now;
	labelDate->Text = date.Day + ":" + date.Month + ":" + date.Year;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	// Initialize the Patient name
	labelPatientName->Text = GantryStatusRegisters::OperatingStatusRegister::getPatientName();

	// Set the Close Door Status
	OPERSTATUS::Registers.closed_door = GantryStatusRegisters::SafetyStatusRegister::getCloseDoor();

	// Set the Tube Data
	OPERSTATUS::Registers.tube.anode = 0;
	OPERSTATUS::Registers.tube.stator = 0;
	OPERSTATUS::Registers.tube.bulb = 0;
	OPERSTATUS::Registers.tube.alarm = false;

	this->Show();

	// Start the startup session	
	operatingTimer->Start();	

}

void OperatingForm::operatingStatusManagement(void) {

	DateTime date;
	date = DateTime::Now;
	
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;


	if (Errors::isError()) {
		errorButton->Show();
	}else errorButton->Hide();

	if ((Errors::isNewError()) || (Errors::isUpdateError())) {
		openErrorWindow(true);
	}

}

void OperatingForm::WndProc(System::Windows::Forms::Message% m)
{
	switch (m.Msg) {

	case (WM_USER + 1): // onIdleTimeout
		
		operatingStatusManagement();
		break;

	case (WM_USER + 2): // onErrorTimeout
		openErrorWindow(false);
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

void OperatingForm::errorButton_Click(System::Object^ sender, System::EventArgs^ e) {
	
	openErrorWindow(true);
}

void OperatingForm::openErrorWindow(bool status) {
	static bool local_status = false;
	Translate::item^ msgit;
	
	// Stops the exit timer
	errorPanelTimer->Stop();

	// Closing condition
	if ((status == false) && (local_status)) {
		local_status = false;
		mainPanel->Show();
		errorPanel->Hide();		
		return;
	}

	
	// Set the new error in evidence 
	Errors::item^ newitem = Errors::getCurrent();
	if ((!Errors::isUpdateError()) && (local_status)) return;  // The panel is already open and no update on the error queue
	Errors::clrUpdate();

	local_status = true;
	errorPanelTimer->Start();

	if (newitem) {
		msgit = Translate::getItem(newitem->errmsg);

		errorTitle->Text = msgit->title;
		errorId->Text = msgit->id;

	}
	else {
		errorTitle->Text = "";
		errorId->Text = "";
	}

	// Clear the Error content box
	errorContent->Text = "";

	// Set the Content space with the list of active errors
	errorContent->Text = Errors::getListOfErrors();

	// Clears the index of the last occurred error
	Errors::clrNewError();

	// Refresh the Error queue removing the one_shot flagged errors
	Errors::clrOneShotErrors();

	
	mainPanel->Hide();
	errorPanel->Show();

}

void OperatingForm::buttonCanc_Click(System::Object^ sender, System::EventArgs^ e) {
	openErrorWindow(false);
}

