#include "../AWS/awsProtocol.h"
#include "ServiceForm.h"
#include "ErrorForm.h"
#include "operatingForm.h"
#include "SystemConfig.h"
#include "Notify.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB304.h"
#include "PCB315.h"
#include "PCB326.h"
#include "VerticalMotor.h"
#include "BodyMotor.h"
#include "../gantry_global_status.h"


// Main Panel Definition
#define BACKGROUND Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ServiceFormBackground.PNG")


void ServiceForm::formInitialization(void) {

	// Initialize the position of the form
	this->Left = Gantry::monitor_X0;
	this->Top = Gantry::monitor_Y0;

	mainPanel->SetBounds(0, 0, 600, 1024);
	mainPanel->BackgroundImage = BACKGROUND;
	calibPanel->SetBounds(0, 0, 600, 1024);
	calibPanel->BackgroundImage = BACKGROUND;
	calibZerosettingPanel->SetBounds(0, 0, 600, 1024);
	calibZerosettingPanel->BackgroundImage = BACKGROUND;

	
	// Sets the Buttons
	CancButton->BackColor = Color::Transparent;


	serviceTimer = gcnew System::Timers::Timer(100);
	serviceTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &ServiceForm::onServiceTimeout);
	serviceTimer->Stop();

	this->Hide();
	open_status = false;
}

void ServiceForm::initServiceStatus(void) {
	System::DateTime date;
	date = System::DateTime::Now;

	
	current_panel = panels::PANEL_NOT_DEFINED;
	setActivePanel(panels::MAIN_SERVICE_PANEL);

	// Start the startup session	
	serviceTimer->Start();	
	
}

void ServiceForm::open(void) {
	if (open_status) return;
	open_status = true;	
	initServiceStatus();

	this->Show();
}

void ServiceForm::close(void) {
	if (!open_status) return;
	open_status = false;
	serviceTimer->Stop();

	
	this->Hide();
}

void ServiceForm::setActivePanel(ServiceForm::panels p) {
	if (p == current_panel) return;
	current_panel = p;

	// ides all the panels
	mainPanel->Hide();
	calibPanel->Hide();
	calibZerosettingPanel->Hide();

	// Stops the timer: each individual panel starts its timer version 
	serviceTimer->Stop();

	if (p == panels::MAIN_SERVICE_PANEL) {
		initMainPanel();
		mainPanel->Show();
	}
	else if (p == panels::CALIB_PANEL) {
		initCalibrationPanel();
		calibPanel->Show();
	}
	else if (p == panels::CALIB_ZEROSETTING_PANEL) {
		initZeroSettingCalibrationPanel();
		calibZerosettingPanel->Show();
	}
}

void ServiceForm::serviceStatusManagement(void) {
	switch (current_panel) {
	case panels::CALIB_ZEROSETTING_PANEL: zeroSettingPanelTimer(); break;
	}
	
}

void ServiceForm::WndProc(System::Windows::Forms::Message% m)
{
	switch (m.Msg) {

	case (WM_USER + 1): // on statusTimer Timeout
		
		serviceStatusManagement();
		break;
	}

	Form::WndProc(m);
}


System::Void ServiceForm::onServiceTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
{
	
	SendNotifyMessageA(window, WM_USER + 1, 0, 0);
}

// ____________________________ MAIN PANEL ACTIVATION __________________________________ //
void ServiceForm::initMainPanel(void) {
	return;
}
void ServiceForm::cancButton_Click(System::Object^ sender, System::EventArgs^ e) {
	Gantry::setIdle();
}


System::Void  ServiceForm::systemCalibration_Click(System::Object^ sender, System::EventArgs^ e) {
	setActivePanel(panels::CALIB_PANEL);
}

