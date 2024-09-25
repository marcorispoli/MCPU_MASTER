#include "ServicePaddleCalibration.h"
#include "ConfigurationFiles.h"
#include "ServiceForm.h"
#include "Notify.h"
#include "KeyPaddle.h"
#include "gantry_global_status.h"


// Main Panel Definition
#define FORM_BACKGROUND Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ServiceBackground.PNG")
#define BODY_ICON_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\BODY_MOTOR_ICON.PNG")
#define ARM_ICON_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\ARM_MOTOR_ICON.PNG")
#define VERTICAL_ICON_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\VERTICAL_MOTOR_ICON.PNG")
#define TILT_ICON_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\TILT_MOTOR_ICON.PNG")
#define SLIDE_ICON_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\SLIDE_MOTOR_ICON.PNG")
#define CALIB_ZERO_SETTING_BACKGROUND System::Drawing::Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\CALIBRATION\\ZERO_SETTING\\ZeroSettingBackground.PNG")

typedef enum {
	NO_PANEL = 0,
	ZERO_BODY_PANEL,
	ZERO_ARM_PANEL,
	ZERO_VERTICAL_PANEL,
	ZERO_TILT_PANEL,
	ZERO_SLIDE_PANEL,
}zero_panels;

/// <summary>
/// Thuis is the enumeration of the possible zero setting commands
/// </summary>
typedef enum {
	NO_COMMAND = 0,
	ZERO_AUTO_START,
	ZERO_MANUAL_START,
	ZERO_EXECUTING,
	ZERO_COMPLETED,	
}zero_commands;

static zero_panels current_panel = zero_panels::NO_PANEL;
static zero_commands current_zero_command = zero_commands::NO_COMMAND;
static int command_delay = 30;



void ServicePaddleCalibrationTool::formInitialization(void) {

	// Initialize the position of the form
	this->Left = Gantry::monitor_X0;
	this->Top = Gantry::monitor_Y0;

	// Common Items
	BackgroundImage = FORM_BACKGROUND;
	serviceMenuTitle->Text = "";
	labelInstallation->Text = SystemConfig::Configuration->getParam(SystemConfig::PARAM_INSTALLATION_NAME)[SystemConfig::PARAM_INSTALLATION_NAME_TOP];
	serviceCanc->BackColor = Color::Transparent;
		
	serviceTimer = gcnew System::Timers::Timer(100);
	serviceTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &ServicePaddleCalibrationTool::OnPaddleCalibrationToolTimeout);
	serviceTimer->Stop();
	
	calibZerosettingPanel->SetBounds(0, 0, PANEL_WIDTH, PANEL_HIGH);
	calibZerosettingPanel->BackgroundImage = CALIB_ZERO_SETTING_BACKGROUND;
	calibZerosettingPanel->Location = System::Drawing::Point(PANEL_X, PANEL_Y);

	
	executeIcon->BackColor = Color::Transparent;

	max_target_angle = 0;
	min_target_angle = 0;

	this->Hide();
	open_status = false;
}

void ServicePaddleCalibrationTool::initPanel(void) {
	System::DateTime date;
	date = System::DateTime::Now;

	serviceMenuTitle->Text = Notify::TranslateLabel(Notify::messages::LABEL_ZERO_SETTING_PANEL_TITLE);
	current_zero_command = zero_commands::NO_COMMAND;
	
	
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED);
	
	// Start the startup session	
	serviceTimer->Start();	
	
}

void ServicePaddleCalibrationTool::dialog_open(Form^ p) {
	if (panel->open_status) return;
	try {
		panel->parent = p;
		panel->open_status = true;
		panel->initPanel();
		this->ShowDialog(parent);
		window = static_cast<HWND>(Handle.ToPointer());
	}
	catch (...) {
		panel->open_status = false;
		LogClass::logInFile("ServicePaddleCalibrationTool::open() Exception!");
	}

}

void ServicePaddleCalibrationTool::close(void) {
	if (!panel->open_status) return;
	panel->open_status = false;
	panel->serviceTimer->Stop();
	this->Hide();
}

void ServicePaddleCalibrationTool::onBodyPanelTimerCallback(void) {
	System::String^ device_str = "BODY MOTOR";
	

}

void ServicePaddleCalibrationTool::onTiltPanelTimerCallback(void) {
	System::String^ device_str = "TILT MOTOR";

	float val;
	

}


void ServicePaddleCalibrationTool::timerManagement(void) {

	// Update the date time fields
	System::DateTime date;
	date = System::DateTime::Now;

	labelDate->Text = date.Day + ":" + date.Month + ":" + date.Year;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;


	switch (current_panel) {
	
	case zero_panels::ZERO_TILT_PANEL:  onTiltPanelTimerCallback(); break;
	case zero_panels::ZERO_BODY_PANEL: onBodyPanelTimerCallback();  break;
	
	}

	return;


}

void ServicePaddleCalibrationTool::WndProc(System::Windows::Forms::Message% m)
{
	switch (m.Msg) {

	case (WM_USER + 1): // on statusTimer Timeout		
		timerManagement();
		break;
	}

	Form::WndProc(m);
}


System::Void ServicePaddleCalibrationTool::OnPaddleCalibrationToolTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
{	
	SendMessageA(window, WM_USER + 1, 0, 0);
}



// Canc button common management
void ServicePaddleCalibrationTool::cancButton_Click(System::Object^ sender, System::EventArgs^ e) {
	
	if ((current_panel == zero_panels::NO_PANEL) && (current_zero_command == zero_commands::NO_COMMAND)) {
		
		serviceTimer->Stop();
		Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED);
		

		close();
		return;
	}
	
	
	if (current_zero_command != zero_commands::NO_COMMAND) {

		
	}

	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED);
	current_panel = zero_panels::NO_PANEL;
	current_zero_command = zero_commands::NO_COMMAND;
	
	serviceTimer->Stop();
	return;
	
}

System::Void ServicePaddleCalibrationTool::zeroSettingBody_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;	
	
	
	

	executeIcon->BackgroundImage = BODY_ICON_IMAGE;
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_BODY_MANUAL_ROTATION);
		

	
	current_panel = zero_panels::ZERO_BODY_PANEL;
	serviceTimer->Start();
}


System::Void ServicePaddleCalibrationTool::zeroSettingTilt_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;
	
	
	current_panel = zero_panels::ZERO_TILT_PANEL;
	executeIcon->BackgroundImage = TILT_ICON_IMAGE;
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_TILT_MANUAL_ROTATION);
	
	
	serviceTimer->Start();
}


void ServicePaddleCalibrationTool::onTargetSelectionCallback(System::String^ value) {

	

}

System::Void ServicePaddleCalibrationTool::targetSelection_Click(System::Object^ sender, System::EventArgs^ e) {
	//KeyPaddleWindow^ pkeyPaddle = gcnew KeyPaddleWindow(this, "Target Selection (°)", targetSelection->Text);
	//pkeyPaddle->button_ok_event += gcnew KeyPaddleWindow::delegate_button_callback(this, &ServiceZeroSettingTool::onTargetSelectionCallback);
	//pkeyPaddle->open();
}

System::Void ServicePaddleCalibrationTool::runButton_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;
	if (current_panel == zero_panels::NO_PANEL) return;
	current_zero_command = zero_commands::ZERO_AUTO_START;
}

System::Void ServicePaddleCalibrationTool::manButton_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;
	if (current_panel == zero_panels::NO_PANEL) return;
	current_zero_command = zero_commands::ZERO_MANUAL_START;
}