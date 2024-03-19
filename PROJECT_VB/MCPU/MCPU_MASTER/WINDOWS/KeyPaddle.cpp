#include "KeyPaddle.h"
#include "../gantry_global_status.h"
#include "Log.h"



#define FORM_BACKGROUND Image::FromFile(Gantry::applicationResourcePath + "Windows\\KeyboardBackground.PNG")

void KeyPaddleWindow::formInitialization(System::String^ info_title, System::String^ info_content) {

	// Initialize the position of the form
	// The form position has been set to Center with the parent

	// Set the Form trasparent assigning the background to a transparent key
	// The color is just different from the Black!!
	this->BackColor = Color::FromArgb(1,0,0);
	this->TransparencyKey = Color::FromArgb(1, 0, 0);
	BackgroundImage = FORM_BACKGROUND;
	
	
	k1->BackColor = Color::Transparent;
	k2->BackColor = Color::Transparent;
	k3->BackColor = Color::Transparent;
	k4->BackColor = Color::Transparent;
	k5->BackColor = Color::Transparent;
	k6->BackColor = Color::Transparent;
	k7->BackColor = Color::Transparent;
	k8->BackColor = Color::Transparent;
	k9->BackColor = Color::Transparent;
	k0->BackColor = Color::Transparent;
	kp->BackColor = Color::Transparent;
	kb->BackColor = Color::Transparent;
	kok->BackColor = Color::Transparent;
	

	
	// Sets the Info
	InfoTitleText->Text = info_title;
	InfoTitleText->BackColor = Color::Transparent;
	InfoContentText->Text = info_content;
	InfoContentText->BackColor = Color::Transparent;
	
	// Sets the Buttons
	CancButton->BackColor = Color::Transparent;
	
	this->Hide();
	open_status = false;
}

void KeyPaddleWindow::open(void) {
	if (open_status) return;

	try{
		open_status = true;
		result = gcnew System::String("");
		this->ShowDialog(parent);
	}
	catch (...) {
		open_status = false;
		LogClass::logInFile("KeyPaddleWindow::open() Exception!");
	}
	
}
void KeyPaddleWindow::close(void) {
	if (!open_status) return;
	open_status = false;
	this->Hide();
}
