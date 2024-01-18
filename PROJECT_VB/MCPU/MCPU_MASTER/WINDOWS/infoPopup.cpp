#include "infoPopup.h"
#include "Notify.h"
#include "gantry_global_status.h"

#define FORM_BACKGROUND Image::FromFile(Gantry::applicationResourcePath + "Icons\\DIALOG_BACKGROUND.PNG")
#define ERROR_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\error.PNG")
#define WARNING_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\warning.PNG")
#define INFO_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\info.PNG")
#define CANC_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\CANC_ICON.PNG")

// Info Window timeout
#define INFO_WINDOW_TMO	15000


void infoPopupForm::formInitialization(void) {

	this->BackColor = Color::FromArgb(1, 0, 0);
	this->TransparencyKey = Color::FromArgb(1, 0, 0);
	
	this->BackColor = Color::FromArgb(216, 207, 208);
	this->BackgroundImage = FORM_BACKGROUND;


	infoIcon->BackColor = Color::Transparent;
	infoIcon->BackgroundImage = INFO_IMAGE;

	cancButton->BackColor = Color::Transparent;
	cancButton->BackgroundImage = CANC_IMAGE;

	// Auto/Close timer
	infoPanelTimer = gcnew System::Timers::Timer(INFO_WINDOW_TMO);
	infoPanelTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &infoPopupForm::onInfoTimeout);
	infoPanelTimer->Stop();

	open_status = false;
	this->Hide();
	
}

void infoPopupForm::close(void) {
	if (!open_status) return;
	open_status = false;

	infoPanelTimer->Stop();
	this->Hide();
}

void infoPopupForm::open(Form^ parent_form, int msg, System::String^ extra) {
	Notify::messages nmsg = (Notify::messages)msg;

	if (nmsg == Notify::messages::NO_MESSAGE) return;
	if(open_status) return;
	parent = parent_form;
	open_status = true;

	// Stops the exit timer
	infoPanelTimer->Stop();
	infoPanelTimer->Start();

	// Sets the new error in evidence 
	Notify::translate^ translated_info_message = Notify::Translate(nmsg);
	infoTitle->Text = translated_info_message->title;
	infoId->Text = translated_info_message->number;

	// Sete the ICON based on thelast error or warnings
	if(translated_info_message->isError()) infoIcon->BackgroundImage = ERROR_IMAGE;
	else if (translated_info_message->isWarning()) infoIcon->BackgroundImage = WARNING_IMAGE;
	else infoIcon->BackgroundImage = INFO_IMAGE;

	// Set the Content space with the list of active errors
	infoContent->Text = translated_info_message->content;
	if(extra != nullptr) infoContent->Text += "\n" + extra;
	this->ShowDialog(parent);
	window = static_cast<HWND>(Handle.ToPointer());
}
