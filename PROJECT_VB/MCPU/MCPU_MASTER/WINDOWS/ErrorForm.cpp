#include "ErrorForm.h"
#include "Translate.h"
#include "gantry_global_status.h"

#define FORM_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "Windows\\ErrorFormBackground.PNG")
#define ERROR_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\error.PNG")
#define WARNING_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\warning.PNG")
#define INFO_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\info.PNG")

// Error Window timeout
#define ERROR_WINDOW_TMO	15000


void ErrorForm::formInitialization(void) {

	// Initialize the position of the form
	// NOTE: Set the Position of the Form as Manual to be valid
	this->Left = GlobalObjects::monitor_X0 ;
	this->Top = GlobalObjects::monitor_Y0 ;

	this->BackColor = Color::FromArgb(216, 207, 208);
	this->BackgroundImage = FORM_IMAGE;

	// Title
	TitleText->Text = Translate::label("ERROR_WINDOW_PANEL");
	TitleText->BackColor = Color::Transparent;

	notifyIcon->BackColor = Color::Transparent;

	// Sets the Buttons
	CancButton->BackColor = Color::Transparent;

	// Auto/Close timer
	errorPanelTimer = gcnew System::Timers::Timer(ERROR_WINDOW_TMO);
	errorPanelTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &ErrorForm::onErrorTimeout);
	errorPanelTimer->Stop();

	open_status = false;
	this->Hide();
	
}

void ErrorForm::close(void) {
	if (!open_status) return;
	open_status = false;

	errorPanelTimer->Stop();
	this->Hide();
}

void ErrorForm::open(void) {
	if(open_status) return;
	open_status = true;

	// Stops the exit timer
	errorPanelTimer->Stop();
	errorPanelTimer->Start();

	// Sets the new error in evidence 
	Translate::item^ msgit;
	Notify::item^ newitem = Notify::getCurrent();
	
	if (newitem) {
		msgit = Translate::getItem(newitem->msg);
		errorTitle->Text = msgit->title;
		errorId->Text = msgit->id;

		// Sete the ICON based on thelast error or warnings
		if(msgit->isError()) notifyIcon->BackgroundImage = ERROR_IMAGE;
		else if (msgit->isWarning()) notifyIcon->BackgroundImage = WARNING_IMAGE;
		else notifyIcon->BackgroundImage = INFO_IMAGE;
	}
	else {
		errorTitle->Text = "";
		errorId->Text = "";

		// Sete the ICON based on the current error or warnings
		if (Notify::isError()) notifyIcon->BackgroundImage = ERROR_IMAGE;
		else if (Notify::isWarning()) notifyIcon->BackgroundImage = WARNING_IMAGE;
		else notifyIcon->BackgroundImage = INFO_IMAGE;

	}

	// Set the Content space with the list of active errors
	errorContent->Text = Notify::getListOfErrors();

	// Clears the index of the last occurred error
	Notify::clrNewError();

	// Refresh the Error queue removing the one_shot flagged errors
	Notify::clrOneShotErrors();
	this->ShowDialog(parent);
	window = static_cast<HWND>(Handle.ToPointer());
}


void ErrorForm::CancButton_Click(System::Object^ sender, System::EventArgs^ e) {
	button_canc_event();
	this->close();
}
