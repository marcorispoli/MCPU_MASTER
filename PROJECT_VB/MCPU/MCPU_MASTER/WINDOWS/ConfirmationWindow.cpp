#include "ConfirmationWindow.h"
#include "../gantry_global_status.h"
#include "Log.h"


#define ERR_ICON_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\error_160x145.PNG")
#define INFO_ICON_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\info_64x64.PNG")
#define WRN_ICON_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\info_64x64.PNG")
#define CANC_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\CANC_ICON.PNG")
#define OK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\OK_ICON.PNG")
#define FORM_BACKGROUND Image::FromFile(Gantry::applicationResourcePath + "Icons\\DIALOG_BACKGROUND.PNG")

void ConfirmationWindow::formInitialization(InfoType wtype, System::String^ info_title, System::String^ info_content) {

	// Initialize the position of the form
	// The form position has been set to Center with the parent

	// Set the Form trasparent assigning the background to a transparent key
	// The color is just different from the Black!!
	this->BackColor = Color::FromArgb(1,0,0);
	this->TransparencyKey = Color::FromArgb(1, 0, 0);
	mainPanel->BackgroundImage = FORM_BACKGROUND;
	
	ContentPanel->BackColor = Color::Transparent;
	//

	// Sets the Icon
	switch (wtype) {
	case ConfirmationWindow::InfoType::INF_WIN:
		IconPanel->BackgroundImage = INFO_ICON_IMAGE;
		break;
	case ConfirmationWindow::InfoType::WRN_WIN:
		IconPanel->BackgroundImage = WRN_ICON_IMAGE;
		break;
		
	case ConfirmationWindow::InfoType::ERR_WIN:
		IconPanel->BackgroundImage = ERR_ICON_IMAGE;
		break;
	}
	IconPanel->BackColor = Color::Transparent;

	
	// Sets the Info
	InfoTitleText->Text = info_title;
	InfoTitleText->BackColor = Color::Transparent;
	InfoContentText->Text = info_content;
	InfoContentText->BackColor = Color::Transparent;
	
	// Sets the Buttons
	CancButton->BackColor = Color::Transparent;
	CancButton->BackgroundImage = CANC_IMAGE;

	OkButton->BackColor = Color::Transparent;
	OkButton->BackgroundImage = OK_IMAGE;

	this->Hide();
	open_status = false;
}

void ConfirmationWindow::open(void) {
	if (open_status) return;

	try{
		open_status = true;
		this->ShowDialog(parent);
	}
	catch (...) {
		open_status = false;
		LogClass::logInFile("ConfirmationWindow::open() Exception!");
	}
	
}
void ConfirmationWindow::close(void) {
	if (!open_status) return;
	open_status = false;
	this->Hide();
}

void ConfirmationWindow::setContentBackground(Image^ img) {
	ContentPanel->BackgroundImage = img;
}