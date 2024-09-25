#include "../AWS/awsProtocol.h"
#include "ServiceForm.h"
#include "ErrorForm.h"
#include "ConfigurationFiles.h"
#include "Notify.h"
#include "../gantry_global_status.h"

#include "ServiceExposureTool.h"
#include "ServiceRotationTool.h"
#include "ServiceZeroSettingTool.h"


// Main Panel Definition
#define FORM_BACKGROUND Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ServiceBackground.PNG")
#define SERVICE_MENU_BACKGROUND Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ServiceMenuBackground.PNG")
#define CALIB_MENU_BACKGROUND System::Drawing::Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\CALIBRATION\\CalibrationBackground.PNG")


void ServiceForm::formInitialization(void) {

	// Initialize the position of the form
	this->Left = Gantry::monitor_X0;
	this->Top = Gantry::monitor_Y0;

	// Common Items
	BackgroundImage = FORM_BACKGROUND;
	serviceMenuTitle->Text = "";
	labelInstallation->Text = SystemConfig::Configuration->getParam(SystemConfig::PARAM_INSTALLATION_NAME)[SystemConfig::PARAM_INSTALLATION_NAME_TOP];
	serviceCanc->BackColor = Color::Transparent;
	
	servicePanel->SetBounds(0, 0, PANEL_WIDTH, PANEL_HIGH);
	servicePanel->BackgroundImage = SERVICE_MENU_BACKGROUND;
	servicePanel->Location = System::Drawing::Point(PANEL_X, PANEL_Y);
	systemCalibration->BackColor = Color::Transparent;
	systemSetup->BackColor = Color::Transparent;
	rotationTool->BackColor = Color::Transparent;

	// Calib panel Definition
	calibPanel->SetBounds(0, 0, PANEL_WIDTH, PANEL_HIGH);
	calibPanel->BackgroundImage = CALIB_MENU_BACKGROUND;
	calibPanel->Location = System::Drawing::Point(PANEL_X, PANEL_Y);

	// Set of the Menu Buttons
	zeroSetting->BackColor = Color::Transparent;
	exposureTool->BackColor = Color::Transparent;


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

	// Activate the Operating Status  manual modes
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED);

	
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

	// Hides all the panels
	servicePanel->Hide();
	calibPanel->Hide();
	
	

	// Stops the timer: each individual panel starts its timer version 
	serviceTimer->Stop();

	switch (p) {
	case panels::MAIN_SERVICE_PANEL:
		initServicePanel();
		servicePanel->Show();
		break;
	case panels::CALIB_PANEL:
		initCalibrationPanel();
		calibPanel->Show();
		break;

	
	}
}

void ServiceForm::serviceStatusManagement(void) {

	// Update the date time fields
	System::DateTime date;
	date = System::DateTime::Now;

	labelDate->Text = date.Day + ":" + date.Month + ":" + date.Year;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

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
	
	SendMessageA(window, WM_USER + 1, 0, 0);
}



void ServiceForm::initServicePanel(void) {
	serviceMenuTitle->Text = Notify::TranslateLabel(Notify::messages::LABEL_SERVICE_PANEL_TITLE);

	return;
}
void ServiceForm::cancServicePanel(void) {	
	Gantry::setIdle();
	return;
}

System::Void  ServiceForm::systemCalibration_Click(System::Object^ sender, System::EventArgs^ e) {
	setActivePanel(panels::CALIB_PANEL);
}




// Canc button common management
void ServiceForm::cancButton_Click(System::Object^ sender, System::EventArgs^ e) {
	switch (current_panel) {
		case panels::MAIN_SERVICE_PANEL: cancServicePanel(); break;
		case panels::CALIB_PANEL: cancCalibrationPanel(); break;
		
		

	}

}

void ServiceForm::cancCalibrationPanel(void) {
	setActivePanel(panels::MAIN_SERVICE_PANEL);
	return;
}

void ServiceForm::initCalibrationPanel(void) {
	serviceMenuTitle->Text = Notify::TranslateLabel(Notify::messages::LABEL_CALIBRATION_PANEL_TITLE);
	return;
}



System::Void ServiceForm::exposureTool_Click(System::Object^ sender, System::EventArgs^ e) {
	ServiceExposureTool::panel->dialog_open(this); // Opens as a Dialog the service Panel
}
System::Void  ServiceForm::systemRotation_Click(System::Object^ sender, System::EventArgs^ e) {
	ServiceRotationTool::panel->dialog_open(this); // Opens as a Dialog the service Panel
}
System::Void ServiceForm::zeroSetting_Click(System::Object^ sender, System::EventArgs^ e) {
	ServiceZeroSettingTool::panel->dialog_open(this); // Opens as a Dialog the service Panel
}
