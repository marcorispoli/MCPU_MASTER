#include "IconWindow.h"
#include "Log.h"

void IconWindow::formInitialization(Image^ img) {

	// Initialize the position of the form
	// The form position has been set to Center with the parent

	// Set the Form trasparent assigning the background to a transparent key
	// The color is just different from the Black!!
	this->BackColor = Color::FromArgb(1,0,0);
	this->TransparencyKey = Color::FromArgb(1, 0, 0);	
	mainPanel->BackgroundImage = img;

	this->ClientSize = System::Drawing::Size(img->Size.Width, img->Size.Height);
	this->mainPanel->Size = System::Drawing::Size(img->Size.Width, img->Size.Height);
	
	this->Hide();
	open_status = false;
}

void IconWindow::open(void) {
	if (open_status) return;
	
	try {
		open_status = true;
		this->ShowDialog(parent);		
	}
	catch (...) {
		open_status = false;		
		LogClass::logInFile("IconWindow::open Exception!");
	}

}
void IconWindow::close(void) {
	if (!open_status) return;
	open_status = false;
	this->Hide();
}

void IconWindow::setContentBackground(Image^ img) {
	this->BackgroundImage = img;
}