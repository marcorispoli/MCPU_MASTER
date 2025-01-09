#include "aecSelectionDialog.h"
#include "Notify.h"
#include "gantry_global_status.h"



#define FORM_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\AecDialog.PNG")
#define AEC_FIELD_ON_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\AecFieldOn.PNG")
#define AEC_FIELD_OFF_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\AecFieldOff.PNG")

// Aec Window timeout
#define AEC_WINDOW_TMO	15000


void aecSelectionDialog::formInitialization(void) {

	// Initialize the position of the form
	// NOTE: Set the Position of the Form as Manual to be valid
	this->Left = Gantry::monitor_X0 ;
	this->Top = Gantry::monitor_Y0 ;

	this->BackColor = Color::FromArgb(216, 207, 208);
	this->BackgroundImage = FORM_IMAGE;

	
	TitleText->BackColor = Color::Transparent;

	CancButton->BackColor = Color::Transparent;
	OkButton->BackColor = Color::Transparent;

	field1->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field2->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field3->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field4->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field5->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field6->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field7->BackgroundImage = AEC_FIELD_OFF_IMAGE;


	// Auto/Close timer
	aecSelectionDialogTimer = gcnew System::Timers::Timer(AEC_WINDOW_TMO);
	aecSelectionDialogTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &aecSelectionDialog::onAecTimeout);
	aecSelectionDialogTimer->Stop();

	open_status = false;
	this->Hide();
	
}

void aecSelectionDialog::close(void) {
	if (!open_status) return;
	open_status = false;

	aecSelectionDialogTimer->Stop();
	this->Hide();
}

void aecSelectionDialog::open(Form^ parent_form) {
	if(open_status) return;
	parent = parent_form;
	open_status = true;

	//TitleText->Text = Notify::TranslateLabel(Notify::messages::LABEL_ERROR_WINDOW_PANEL);
	TitleText->Text = "AEC SELECTION";

	// Stops the exit timer
	aecSelectionDialogTimer->Stop();
	aecSelectionDialogTimer->Start();

	field_selected = 0;
	field1->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field2->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field3->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field4->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field5->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field6->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field7->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	
	try {
		window = static_cast<HWND>(Handle.ToPointer());
		this->ShowDialog(parent);		
		window = static_cast<HWND>(Handle.ToPointer());
	}
	catch (...) {
		open_status = false;
		aecSelectionDialogTimer->Stop();
		LogClass::logInFile("aecSelectionDialog::open Exception!");
	}

}


void aecSelectionDialog::CancButton_Click(System::Object^ sender, System::EventArgs^ e) {
	field_selected = 0;
	this->close();
}

void aecSelectionDialog::OkButton_Click(System::Object^ sender, System::EventArgs^ e) {
	
	this->close();
}

void aecSelectionDialog::Field1_Click(System::Object^ sender, System::EventArgs^ e) {
	field1->BackgroundImage = AEC_FIELD_ON_IMAGE;
	field2->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field3->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field4->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field5->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field6->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field7->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field_selected = 1;
}
void aecSelectionDialog::Field2_Click(System::Object^ sender, System::EventArgs^ e) {
	field1->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field2->BackgroundImage = AEC_FIELD_ON_IMAGE;
	field3->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field4->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field5->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field6->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field7->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field_selected = 2;
}
void aecSelectionDialog::Field3_Click(System::Object^ sender, System::EventArgs^ e) {
	field1->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field2->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field3->BackgroundImage = AEC_FIELD_ON_IMAGE;
	field4->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field5->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field6->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field7->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field_selected = 3;
}
void aecSelectionDialog::Field4_Click(System::Object^ sender, System::EventArgs^ e) {
	field1->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field2->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field3->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field4->BackgroundImage = AEC_FIELD_ON_IMAGE;
	field5->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field6->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field7->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field_selected = 4;
}
void aecSelectionDialog::Field5_Click(System::Object^ sender, System::EventArgs^ e) {
	field1->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field2->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field3->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field4->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field5->BackgroundImage = AEC_FIELD_ON_IMAGE;
	field6->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field7->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field_selected = 5;
}
void aecSelectionDialog::Field6_Click(System::Object^ sender, System::EventArgs^ e) {
	field1->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field2->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field3->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field4->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field5->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field6->BackgroundImage = AEC_FIELD_ON_IMAGE;
	field7->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field_selected = 6;
}
void aecSelectionDialog::Field7_Click(System::Object^ sender, System::EventArgs^ e) {
	field1->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field2->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field3->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field4->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field5->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field6->BackgroundImage = AEC_FIELD_OFF_IMAGE;
	field7->BackgroundImage = AEC_FIELD_ON_IMAGE;
	field_selected = 7;
}