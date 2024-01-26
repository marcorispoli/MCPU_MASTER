#include "ValuePopup.h"
#include "gantry_global_status.h"

#define FORM_BACKGROUND Image::FromFile(Gantry::applicationResourcePath + "Icons\\DIALOG_BACKGROUND.PNG")

// Info Window timeout
#define INFO_WINDOW_TMO	15000


void ValuePopupForm::formInitialization(void) {

	this->BackColor = Color::FromArgb(1, 0, 0);
	this->TransparencyKey = Color::FromArgb(1, 0, 0);
	this->BackgroundImage = FORM_BACKGROUND;


	valueIcon->BackColor = Color::Transparent;
	valueTitle->BackColor = Color::Transparent;
	valueUnit->BackColor = Color::Transparent;
	valueContent->BackColor = Color::Transparent;

	open_status = false;
	this->Hide();
	
}

void ValuePopupForm::close(void) {
	if (!open_status) return;
	open_status = false;
	
	// Be careful!!! In order to hide a Dialog the parent shall process the event queue.
	// If a open() should be called after a close() may rise an error.
	// So: don't use 
	//	close(); 
	//  open(); 

	this->Hide();
}

void ValuePopupForm::open(Form^ parent_form, Image^ icon, System::String^ title, System::String^ unit) {	
	if(open_status) return;
	open_status = true;
	parent = parent_form;
	window = static_cast<HWND>(Handle.ToPointer());

	valueIcon->BackgroundImage = icon;
	valueTitle->Text = title;
	valueUnit->Text = unit;
	valueContent->Text = "";	
	
	// The thread stops here until the Dialog is closed (Hide())
	this->ShowDialog(parent);
	
}

void ValuePopupForm::retitle(Image^ icon, System::String^ title, System::String^ unit) {
	valueIcon->BackgroundImage = icon;
	valueTitle->Text = title;
	valueUnit->Text = unit;
	valueContent->Text = "";
}



void ValuePopupForm::content(System::String^ val) {
	valueContent->Text = val;
}