#include "ErrorForm.h"
#include "Notify.h"
#include "gantry_global_status.h"

#define FORM_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Windows\\ErrorFormBackground.PNG")
#define ERROR_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\error.PNG")
#define WARNING_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\warning.PNG")
#define INFO_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\info.PNG")

// Error Window timeout
#define ERROR_WINDOW_TMO	15000


void ErrorForm::formInitialization(void) {

	// Initialize the position of the form
	// NOTE: Set the Position of the Form as Manual to be valid
	this->Left = Gantry::monitor_X0 ;
	this->Top = Gantry::monitor_Y0 ;

	this->BackColor = Color::FromArgb(216, 207, 208);
	this->BackgroundImage = FORM_IMAGE;

	// Title
	
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

void ErrorForm::open(Form^ parent_form) {
	if(open_status) return;
	parent = parent_form;
	open_status = true;

	TitleText->Text = Notify::TranslateLabel(Notify::messages::LABEL_ERROR_WINDOW_PANEL);
	
	// Stops the exit timer
	errorPanelTimer->Stop();
	errorPanelTimer->Start();

	
	// Sete the ICON based on the current error or warnings
	if (Notify::isError()) notifyIcon->BackgroundImage = ERROR_IMAGE;
	else if (Notify::isWarning()) notifyIcon->BackgroundImage = WARNING_IMAGE;
	else notifyIcon->BackgroundImage = INFO_IMAGE;

	// Set the Content space with the list of active errors
	errorContent->Text = Notify::getListOfErrors();
	

	try {
		window = static_cast<HWND>(Handle.ToPointer());
		this->ShowDialog(parent);		
		window = static_cast<HWND>(Handle.ToPointer());
	}
	catch (...) {
		open_status = false;
		errorPanelTimer->Stop();
		LogClass::logInFile("ErrorForm::open Exception!");
	}

}


void ErrorForm::CancButton_Click(System::Object^ sender, System::EventArgs^ e) {
	button_canc_event();
	this->close();
}
