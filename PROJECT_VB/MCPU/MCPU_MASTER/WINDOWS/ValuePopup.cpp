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

	this->Hide();
}

void ValuePopupForm::open(Form^ parent_form, Image^ icon, System::String^ title, System::String^ unit) {	
	if(open_status) return;
	parent = parent_form;
	open_status = true;

	valueIcon->BackgroundImage = icon;
	valueTitle->Text = title;
	valueUnit->Text = unit;
	valueContent->Text = "";

	
	this->ShowDialog(parent);
	window = static_cast<HWND>(Handle.ToPointer());
}

void ValuePopupForm::content(System::String^ val) {
	valueContent->Text = val;
}