#include "ServiceZeroSettingTool.h"
#include "SystemConfig.h"
#include "ServiceForm.h"
#include "Notify.h"
#include "ArmMotor.h"
#include "VerticalMotor.h"
#include "TiltMotor.h"
#include "BodyMotor.h"
#include "SlideMotor.h"
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



void ServiceZeroSettingTool::formInitialization(void) {

	// Initialize the position of the form
	this->Left = Gantry::monitor_X0;
	this->Top = Gantry::monitor_Y0;

	// Common Items
	BackgroundImage = FORM_BACKGROUND;
	serviceMenuTitle->Text = "";
	labelInstallation->Text = SystemConfig::Configuration->getParam(SystemConfig::PARAM_INSTALLATION_NAME)[SystemConfig::PARAM_INSTALLATION_NAME_TOP];
	serviceCanc->BackColor = Color::Transparent;
		
	serviceTimer = gcnew System::Timers::Timer(100);
	serviceTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &ServiceZeroSettingTool::onServiceTimeout);
	serviceTimer->Stop();
	
	calibZerosettingPanel->SetBounds(0, 0, PANEL_WIDTH, PANEL_HIGH);
	calibZerosettingPanel->BackgroundImage = CALIB_ZERO_SETTING_BACKGROUND;
	calibZerosettingPanel->Location = System::Drawing::Point(PANEL_X, PANEL_Y);

	zeroSettingArm->BackColor = Color::Transparent;
	zeroSettingBody->BackColor = Color::Transparent;
	zeroSettingSlide->BackColor = Color::Transparent;
	zeroSettingTilt->BackColor = Color::Transparent;
	zeroSettingVertical->BackColor = Color::Transparent;	
	executeIcon->BackColor = Color::Transparent;

	max_target_angle = 0;
	min_target_angle = 0;

	this->Hide();
	open_status = false;
}

void ServiceZeroSettingTool::initPanel(void) {
	System::DateTime date;
	date = System::DateTime::Now;

	serviceMenuTitle->Text = Notify::TranslateLabel(Notify::messages::LABEL_ZERO_SETTING_PANEL_TITLE);
	current_zero_command = zero_commands::NO_COMMAND;
	
	zeroSettingLog->Clear();
	executePanel->Hide();

	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED);
	
	// Start the startup session	
	serviceTimer->Stop();	
	
}

void ServiceZeroSettingTool::dialog_open(Form^ p) {
	if (panel->open_status) return;
	try {
		panel->parent = p;
		panel->open_status = true;
		panel->initPanel();
		this->ShowDialog(parent);
	}
	catch (...) {
		panel->open_status = false;
		LogClass::logInFile("ServiceZeroSettingTool::open() Exception!");
	}

}

void ServiceZeroSettingTool::close(void) {
	if (!panel->open_status) return;
	panel->open_status = false;
	panel->serviceTimer->Stop();
	this->Hide();
}

void ServiceZeroSettingTool::onBodyPanelTimerCallback(void) {
	System::String^ device_str = "BODY MOTOR";
	float val;
	if (current_zero_command == zero_commands::NO_COMMAND) {
		if (BodyMotor::device->isPositionFromExternalSensor()) {
			sensorValue->Text = BodyMotor::device->getExternalSensor().ToString();
			val = BodyMotor::device->getExternalPosition() * 0.1;			
			externalPosition->Text = val.ToString();
			val = BodyMotor::device->getEncoderPosition() * 0.1;
			encoderPosition->Text = val.ToString();
		}
		else {
			sensorValue->Text = "-";
			externalPosition->Text = "-";
			val = BodyMotor::device->getEncoderPosition() * 0.1;
			encoderPosition->Text = val.ToString();
		}
		return;
	}

	switch (current_zero_command) {
	case zero_commands::ZERO_MANUAL_START:
		val = System::Convert::ToDouble(targetSelection->Text) * 10;

		if (!BodyMotor::startManualHoming((int) val)) {
			zeroSettingLog->Text += device_str + ": Unable to start the manual home procedure\n";
			current_zero_command = zero_commands::NO_COMMAND;
			return;
		}
		current_zero_command = zero_commands::ZERO_EXECUTING;
		break;

	case zero_commands::ZERO_AUTO_START:
		if (!BodyMotor::startAutoHoming()) {
			zeroSettingLog->Text += device_str + ": Unable to start the auto home procedure\n";
			current_zero_command = zero_commands::NO_COMMAND;
			return;
		}

		current_zero_command = zero_commands::ZERO_EXECUTING;
		zeroSettingLog->Text += device_str + ": Home procedure started\n";
		break;
		
	case zero_commands::ZERO_EXECUTING:
		if (!BodyMotor::device->isReady()) return;
		current_zero_command = zero_commands::ZERO_COMPLETED;
		break;

	case zero_commands::ZERO_COMPLETED:
		if (BodyMotor::device->getCommandCompletedCode() == BodyMotor::MotorCompletedCodes::COMMAND_SUCCESS) {
			zeroSettingLog->Text += "BODY MOTOR: COMPLETED\n";
		}else zeroSettingLog->Text += "BODY MOTOR:" + BodyMotor::device->getCommandCompletedCode().ToString() + "\n";
		current_zero_command = zero_commands::NO_COMMAND;
		return;
	}

}

void ServiceZeroSettingTool::onTiltPanelTimerCallback(void) {
	System::String^ device_str = "TILT MOTOR";

	float val;
	if (current_zero_command == zero_commands::NO_COMMAND) {		
		if (TiltMotor::device->isPositionFromExternalSensor()) {
			sensorValue->Text = TiltMotor::device->getExternalSensor().ToString();
			val = TiltMotor::device->getExternalPosition() * 0.01;
			externalPosition->Text = val.ToString();
			val = TiltMotor::device->getEncoderPosition() * 0.01;
			encoderPosition->Text = val.ToString();
		}
		else {
			sensorValue->Text = "-";
			externalPosition->Text = "-";
			val = TiltMotor::device->getEncoderPosition() * 0.01;
			encoderPosition->Text = val.ToString();
		}
		return;
	}

	switch (current_zero_command) {
	case zero_commands::ZERO_MANUAL_START:
		val = System::Convert::ToDouble(targetSelection->Text) * 100;
		if (!TiltMotor::startManualHoming((int) val)) {
			zeroSettingLog->Text += device_str + ": Unable to start the manual home procedure\n";
			current_zero_command = zero_commands::NO_COMMAND;
			return;
		}
		current_zero_command = zero_commands::ZERO_EXECUTING;
		break;

	case zero_commands::ZERO_AUTO_START:
		if (!TiltMotor::startAutoHoming()) {
			zeroSettingLog->Text += device_str + ": Unable to start the auto home procedure\n";
			current_zero_command = zero_commands::NO_COMMAND;
			return;
		}

		current_zero_command = zero_commands::ZERO_EXECUTING;
		zeroSettingLog->Text += device_str + ": Home procedure started\n";
		break;
	
	case zero_commands::ZERO_EXECUTING:
		if (!TiltMotor::device->isReady()) return;
		current_zero_command = zero_commands::ZERO_COMPLETED;
		break;

	case zero_commands::ZERO_COMPLETED:
		if (TiltMotor::device->getCommandCompletedCode() == CANOPEN::CanOpenMotor::MotorCompletedCodes::COMMAND_SUCCESS) {
			zeroSettingLog->Text += device_str + ": COMPLETED\n";
		}
		else zeroSettingLog->Text += device_str + ":" + TiltMotor::device->getCommandCompletedCode().ToString() + "\n";
		current_zero_command = zero_commands::NO_COMMAND;
		return;
	}

}

void ServiceZeroSettingTool::onVerticalPanelTimerCallback(void) {
	System::String^ device_str = "VERTICAL MOTOR";

	if (current_zero_command == zero_commands::NO_COMMAND) {
		if (VerticalMotor::device->isPositionFromExternalSensor()) {
			sensorValue->Text = VerticalMotor::device->getExternalSensor().ToString();
			externalPosition->Text = VerticalMotor::device->getExternalPosition().ToString();
			encoderPosition->Text = VerticalMotor::device->getEncoderPosition().ToString();
		}
		else {
			sensorValue->Text = "-";
			externalPosition->Text = "-";
			encoderPosition->Text = VerticalMotor::device->getEncoderPosition().ToString();
		}
		return;
	}

	switch (current_zero_command) {
	case zero_commands::ZERO_MANUAL_START:
		if (!VerticalMotor::startManualHoming(System::Convert::ToInt16(targetSelection->Text))) {
			zeroSettingLog->Text += device_str + ": Unable to start the manual home procedure\n";
			current_zero_command = zero_commands::NO_COMMAND;
			return;
		}
		current_zero_command = zero_commands::ZERO_EXECUTING;
		break;

	case zero_commands::ZERO_AUTO_START:
		if (!VerticalMotor::startAutoHoming()) {
			zeroSettingLog->Text += device_str + ": Unable to start the auto home procedure\n";
			current_zero_command = zero_commands::NO_COMMAND;
			return;
		}

		current_zero_command = zero_commands::ZERO_EXECUTING;
		zeroSettingLog->Text += device_str + ": Home procedure started\n";
		break;

	case zero_commands::ZERO_EXECUTING:
		if (!VerticalMotor::device->isReady()) return;
		current_zero_command = zero_commands::ZERO_COMPLETED;
		break;

	case zero_commands::ZERO_COMPLETED:
		if (VerticalMotor::device->getCommandCompletedCode() == CANOPEN::CanOpenMotor::MotorCompletedCodes::COMMAND_SUCCESS) {
			zeroSettingLog->Text += device_str + ": COMPLETED\n";
		}
		else zeroSettingLog->Text += device_str + ":" + VerticalMotor::device->getCommandCompletedCode().ToString() + "\n";
		current_zero_command = zero_commands::NO_COMMAND;
		return;
	}

}

void ServiceZeroSettingTool::onArmPanelTimerCallback(void) {
	System::String^ device_str = "ARM MOTOR";

	float val;
	if (current_zero_command == zero_commands::NO_COMMAND) {
		if (ArmMotor::device->isPositionFromExternalSensor()) {
			sensorValue->Text = ArmMotor::device->getExternalSensor().ToString();
			val = ArmMotor::device->getExternalPosition() * 0.01;
			externalPosition->Text = val.ToString();
			val = ArmMotor::device->getEncoderPosition() * 0.01;
			encoderPosition->Text = val.ToString();
		}
		else {
			sensorValue->Text = "-";
			externalPosition->Text = "-";
			val = ArmMotor::device->getEncoderPosition() * 0.01;
			encoderPosition->Text = val.ToString();
		}
		return;
	}

	switch (current_zero_command) {
	case zero_commands::ZERO_MANUAL_START:
		val = System::Convert::ToDouble(targetSelection->Text) * 100;
		if (!ArmMotor::startManualHoming((int) val)) {
			zeroSettingLog->Text += device_str + ": Unable to start the manual home procedure\n";
			current_zero_command = zero_commands::NO_COMMAND;
			return;
		}
		current_zero_command = zero_commands::ZERO_EXECUTING;
		break;

	case zero_commands::ZERO_AUTO_START:
		if (!ArmMotor::startAutoHoming()) {
			zeroSettingLog->Text += device_str + ": Unable to start the auto home procedure\n";
			current_zero_command = zero_commands::NO_COMMAND;
			return;
		}

		current_zero_command = zero_commands::ZERO_EXECUTING;
		zeroSettingLog->Text += device_str + ": Home procedure started\n";
		break;

	case zero_commands::ZERO_EXECUTING:
		if (!ArmMotor::device->isReady()) return;
		current_zero_command = zero_commands::ZERO_COMPLETED;
		break;

	case zero_commands::ZERO_COMPLETED:
		if (ArmMotor::device->getCommandCompletedCode() == CANOPEN::CanOpenMotor::MotorCompletedCodes::COMMAND_SUCCESS) {
			zeroSettingLog->Text += device_str + ": COMPLETED\n";
		}
		else zeroSettingLog->Text += device_str + ":" + ArmMotor::device->getCommandCompletedCode().ToString() + "\n";
		current_zero_command = zero_commands::NO_COMMAND;
		return;
	}

}

void ServiceZeroSettingTool::onSlidePanelTimerCallback(void) {
	System::String^ device_str = "SLIDE MOTOR";

	float val;
	if (current_zero_command == zero_commands::NO_COMMAND) {
		if (SlideMotor::device->isPositionFromExternalSensor()) {
			sensorValue->Text = SlideMotor::device->getExternalSensor().ToString();
			val = SlideMotor::device->getExternalPosition() * 0.01;
			externalPosition->Text = val.ToString();
			val = SlideMotor::device->getEncoderPosition() * 0.01;
			encoderPosition->Text = val.ToString();
		}
		else {
			sensorValue->Text = "-";
			externalPosition->Text = "-";
			val = SlideMotor::device->getEncoderPosition() * 0.01;
			encoderPosition->Text = val.ToString();
		}
		return;
	}

	switch (current_zero_command) {
	case zero_commands::ZERO_MANUAL_START:
		val = System::Convert::ToDouble(targetSelection->Text) * 100;
		if (!SlideMotor::startManualHoming((int) val)) {
			zeroSettingLog->Text += device_str + ": Unable to start the manual home procedure\n";
			current_zero_command = zero_commands::NO_COMMAND;
			return;
		}
		current_zero_command = zero_commands::ZERO_EXECUTING;
		break;

	case zero_commands::ZERO_AUTO_START:
		if (!SlideMotor::startAutoHoming()) {
			zeroSettingLog->Text += device_str + ": Unable to start the auto home procedure\n";
			current_zero_command = zero_commands::NO_COMMAND;
			return;
		}

		current_zero_command = zero_commands::ZERO_EXECUTING;
		zeroSettingLog->Text += device_str + ": Home procedure started\n";
		break;

	case zero_commands::ZERO_EXECUTING:
		if (!SlideMotor::device->isReady()) return;
		current_zero_command = zero_commands::ZERO_COMPLETED;
		break;

	case zero_commands::ZERO_COMPLETED:
		if (SlideMotor::device->getCommandCompletedCode() == CANOPEN::CanOpenMotor::MotorCompletedCodes::COMMAND_SUCCESS) {
			zeroSettingLog->Text += device_str + ": COMPLETED\n";
		}
		else zeroSettingLog->Text += device_str + ":" + SlideMotor::device->getCommandCompletedCode().ToString() + "\n";
		current_zero_command = zero_commands::NO_COMMAND;
		return;
	}

}


void ServiceZeroSettingTool::timerManagement(void) {

	// Update the date time fields
	System::DateTime date;
	date = System::DateTime::Now;

	labelDate->Text = date.Day + ":" + date.Month + ":" + date.Year;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;


	switch (current_panel) {
	case zero_panels::ZERO_ARM_PANEL:  onArmPanelTimerCallback(); break;
	case zero_panels::ZERO_TILT_PANEL:  onTiltPanelTimerCallback(); break;
	case zero_panels::ZERO_BODY_PANEL: onBodyPanelTimerCallback();  break;
	case zero_panels::ZERO_VERTICAL_PANEL:  onVerticalPanelTimerCallback(); break;
	case zero_panels::ZERO_SLIDE_PANEL:  onSlidePanelTimerCallback(); break;
	}

	return;


}

void ServiceZeroSettingTool::WndProc(System::Windows::Forms::Message% m)
{
	switch (m.Msg) {

	case (WM_USER + 1): // on statusTimer Timeout		
		timerManagement();
		break;
	}

	Form::WndProc(m);
}


System::Void ServiceZeroSettingTool::onServiceTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
{	
	SendMessageA(window, WM_USER + 1, 0, 0);
}



// Canc button common management
void ServiceZeroSettingTool::cancButton_Click(System::Object^ sender, System::EventArgs^ e) {
	BodyMotor::device->setServiceMode(false);
	TiltMotor::device->setServiceMode(false);
	SlideMotor::device->setServiceMode(false);
	ArmMotor::device->setServiceMode(false);
	VerticalMotor::device->setServiceMode(false);

	if ((current_panel == zero_panels::NO_PANEL) && (current_zero_command == zero_commands::NO_COMMAND)) {
		executePanel->Hide();
		serviceTimer->Stop();
		Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED);
		

		close();
		return;
	}
	
	
	if (current_zero_command != zero_commands::NO_COMMAND) {

		switch (current_panel) {
		case zero_panels::ZERO_BODY_PANEL:BodyMotor::device->abortActivation(); break;
		case zero_panels::ZERO_ARM_PANEL:ArmMotor::device->abortActivation(); break;
		case zero_panels::ZERO_TILT_PANEL:TiltMotor::device->abortActivation(); break;
		case zero_panels::ZERO_VERTICAL_PANEL:VerticalMotor::device->abortActivation(); break;
		case zero_panels::ZERO_SLIDE_PANEL:SlideMotor::device->abortActivation(); break;
		}
	}

	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED);
	current_panel = zero_panels::NO_PANEL;
	current_zero_command = zero_commands::NO_COMMAND;
	executePanel->Hide();
	serviceTimer->Stop();
	return;
	
}

System::Void ServiceZeroSettingTool::zeroSettingBody_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;	
	
	max_target_angle = BodyMotor::device->getMaxPosition() * 0.1;
	min_target_angle = BodyMotor::device->getMinPosition() * 0.1;
	executeIcon->BackgroundImage = BODY_ICON_IMAGE;
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_BODY_MANUAL_ROTATION);
	BodyMotor::device->setServiceMode(true);	

	float val = BodyMotor::device->getCurrentPosition() * 0.1;
	targetSelection->Text = val.ToString();

	if (BodyMotor::device->isPositionFromExternalSensor()) {
		manButton->Show();
		runButton->Hide();
	}
	else {
		manButton->Show();
		runButton->Show();		
	}

	
	zeroSettingLog->Clear();
	executePanel->Show();
	current_panel = zero_panels::ZERO_BODY_PANEL;
	serviceTimer->Start();
}

System::Void ServiceZeroSettingTool::zeroSettingVertical_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;

	current_panel = zero_panels::ZERO_VERTICAL_PANEL;
	executeIcon->BackgroundImage = VERTICAL_ICON_IMAGE;
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_VERTICAL_MANUAL_ROTATION);
	VerticalMotor::device->setServiceMode(true);

	float val = VerticalMotor::device->getCurrentPosition() * 1;
	targetSelection->Text = val.ToString();
	

	if (VerticalMotor::device->isPositionFromExternalSensor()) {
		
		manButton->Show();
		runButton->Hide();
	}
	else {
		manButton->Show();
		runButton->Show();
		
	}

	zeroSettingLog->Clear();
	executePanel->Show();
	serviceTimer->Start();
}
System::Void ServiceZeroSettingTool::zeroSettingArm_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;

	current_panel = zero_panels::ZERO_ARM_PANEL;
	executeIcon->BackgroundImage = ARM_ICON_IMAGE;
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_ARM_MANUAL_ROTATION);
	ArmMotor::device->setServiceMode(true);

	float val = ArmMotor::device->getCurrentPosition() * 0.01;
	targetSelection->Text = val.ToString();
	

	if (ArmMotor::device->isPositionFromExternalSensor()) {
		
		manButton->Show();
		runButton->Hide();
	}
	else {
		manButton->Show();
		runButton->Show();
		
	}

	zeroSettingLog->Clear();
	executePanel->Show();
	serviceTimer->Start();
}
System::Void ServiceZeroSettingTool::zeroSettingTilt_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;

	current_panel = zero_panels::ZERO_TILT_PANEL;
	executeIcon->BackgroundImage = TILT_ICON_IMAGE;
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_TILT_MANUAL_ROTATION);
	TiltMotor::device->setServiceMode(true);
	
	float val = TiltMotor::device->getCurrentPosition() * 0.01;
	targetSelection->Text = val.ToString();

	if (TiltMotor::device->isPositionFromExternalSensor()) {
		
		manButton->Show();
		runButton->Hide();
	}
	else {
		manButton->Show();
		runButton->Show();
		
	}

	zeroSettingLog->Clear();
	executePanel->Show();
	serviceTimer->Start();
}

System::Void ServiceZeroSettingTool::zeroSettingSlide_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;

	current_panel = zero_panels::ZERO_SLIDE_PANEL;
	executeIcon->BackgroundImage = SLIDE_ICON_IMAGE;
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_SLIDE_MANUAL_ROTATION);
	SlideMotor::device->setServiceMode(true);
	
	float val =  SlideMotor::device->getCurrentPosition() * 0.01;
	targetSelection->Text = val.ToString();

	if (SlideMotor::device->isPositionFromExternalSensor()) {
		
		manButton->Show();
		runButton->Hide();
	}
	else {
		manButton->Show();
		runButton->Show();
		
	}

	zeroSettingLog->Clear();
	executePanel->Show();
	serviceTimer->Start();
}


void ServiceZeroSettingTool::onTargetSelectionCallback(System::String^ value) {

	float angle = System::Convert::ToDouble(value);
	if (angle > max_target_angle) return;
	if (angle < min_target_angle) return;
	targetSelection->Text = value;

}

System::Void ServiceZeroSettingTool::targetSelection_Click(System::Object^ sender, System::EventArgs^ e) {
	KeyPaddleWindow^ pkeyPaddle = gcnew KeyPaddleWindow(this, "Target Selection (°)", targetSelection->Text);
	pkeyPaddle->button_ok_event += gcnew KeyPaddleWindow::delegate_button_callback(this, &ServiceZeroSettingTool::onTargetSelectionCallback);
	pkeyPaddle->open();
}

System::Void ServiceZeroSettingTool::runButton_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;
	if (current_panel == zero_panels::NO_PANEL) return;
	current_zero_command = zero_commands::ZERO_AUTO_START;
}

System::Void ServiceZeroSettingTool::manButton_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;
	if (current_panel == zero_panels::NO_PANEL) return;
	current_zero_command = zero_commands::ZERO_MANUAL_START;
}