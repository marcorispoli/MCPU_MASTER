#include "ServiceForm.h"
#include "Notify.h"
#include "ArmMotor.h"
#include "VerticalMotor.h"
#include "TiltMotor.h"
#include "BodyMotor.h"
#include "SlideMotor.h"


#define BODY_OK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ZeroSetting\\BodyOk.PNG")
#define BODY_NOK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ZeroSetting\\BodyNok.PNG")
#define ARM_OK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ZeroSetting\\ArmOk.PNG")
#define ARM_NOK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ZeroSetting\\ArmNok.PNG")
#define TILT_OK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ZeroSetting\\TiltOk.PNG")
#define TILT_NOK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ZeroSetting\\TiltNok.PNG")
#define VERTICAL_OK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ZeroSetting\\VerticalOk.PNG")
#define VERTICAL_NOK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ZeroSetting\\VerticalNok.PNG")
#define SLIDE_OK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ZeroSetting\\SlideOk.PNG")
#define SLIDE_NOK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ZeroSetting\\SlideNok.PNG")

/// <summary>
/// Thuis is the enumeration of the possible zero setting commands
/// </summary>
typedef enum {
	NO_COMMAND = 0,
	ZERO_BODY,
	ZERO_ARM,
	ZERO_VERTICAL,
	ZERO_TILT,
	ZERO_SLIDE,
	ZERO_ALL
}zero_commands;

static zero_commands current_zero_command = zero_commands::NO_COMMAND;
static int command_delay = 30;

void ServiceForm::initZeroSettingCalibrationPanel(void) {

	// Sets the geometry
	zeroSettingArm->BackColor = Color::Transparent;
	zeroSettingBody->BackColor = Color::Transparent;
	zeroSettingSlide->BackColor = Color::Transparent;
	zeroSettingTilt->BackColor = Color::Transparent;
	zeroSettingVertical->BackColor = Color::Transparent;
	zeroSettingAll->BackColor = Color::Transparent;
	buttonCalibZeroSettingExit->BackColor = Color::Transparent;

	zeroSettingPanelTitle->Text = Notify::TranslateLabel(Notify::messages::LABEL_ZERO_SETTING_PANEL_TITLE);
	current_zero_command = zero_commands::NO_COMMAND;

	zeroSettingAssigneImages();

	zeroSettingLog->Clear();
	
	return;
}

System::Void ServiceForm::buttonCalibZeroSettingExit_Click(System::Object^ sender, System::EventArgs^ e) {

	setActivePanel(panels::CALIB_PANEL);
}


System::Void ServiceForm::zeroSettingBody_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;
	command_delay = 50;
	current_zero_command = zero_commands::ZERO_BODY;
	serviceTimer->Start();
}
System::Void ServiceForm::zeroSettingVertical_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;
	command_delay = 50;
	current_zero_command = zero_commands::ZERO_VERTICAL;
	serviceTimer->Start();
}
System::Void ServiceForm::zeroSettingArm_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;
	command_delay = 50;
	current_zero_command = zero_commands::ZERO_ARM;
	serviceTimer->Start();
}
System::Void ServiceForm::zeroSettingTilt_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;
	command_delay = 50;
	current_zero_command = zero_commands::ZERO_TILT;
	serviceTimer->Start();
}

System::Void ServiceForm::zeroSettingSlide_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;
	command_delay = 50;
	current_zero_command = zero_commands::ZERO_SLIDE;
	serviceTimer->Start();
}

System::Void ServiceForm::zeroSettingAll_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command != zero_commands::NO_COMMAND) return;
	command_delay = 50;

}

System::Void ServiceForm::zeroSettingAbortButton_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_zero_command == zero_commands::NO_COMMAND) return;

	switch (current_zero_command) {
	case zero_commands::ZERO_BODY:BodyMotor::device->abortActivation(); break;
	case zero_commands::ZERO_VERTICAL:VerticalMotor::device->abortActivation(); break;
	case zero_commands::ZERO_TILT:TiltMotor::device->abortActivation(); break;
	case zero_commands::ZERO_ARM:ArmMotor::device->abortActivation(); break;
	case zero_commands::ZERO_SLIDE:SlideMotor::device->abortActivation(); break;
	}
}



void ServiceForm::zeroSettingAssigneImages(void) {

	// Sets the current color of the images based on the actual zero homing condition
	if (ArmMotor::device->isZeroOk()) zeroSettingArm->BackgroundImage = ARM_OK_IMAGE;
	else zeroSettingArm->BackgroundImage = ARM_NOK_IMAGE;

	if (BodyMotor::device->isZeroOk()) zeroSettingBody->BackgroundImage = BODY_OK_IMAGE;
	else zeroSettingBody->BackgroundImage = BODY_NOK_IMAGE;

	if (TiltMotor::device->isZeroOk()) zeroSettingTilt->BackgroundImage = TILT_OK_IMAGE;
	else zeroSettingTilt->BackgroundImage = TILT_NOK_IMAGE;

	if (VerticalMotor::device->isZeroOk()) zeroSettingVertical->BackgroundImage = VERTICAL_OK_IMAGE;
	else zeroSettingVertical->BackgroundImage = VERTICAL_NOK_IMAGE;

	if (SlideMotor::device->isZeroOk()) zeroSettingSlide->BackgroundImage = SLIDE_OK_IMAGE;
	else zeroSettingSlide->BackgroundImage = SLIDE_NOK_IMAGE;

	
}

using namespace System::Drawing;

void ServiceForm::zeroSettingPanelTimer(void) {
	bool finish = false;
	

	// Delay Before to start the command
	if (command_delay > 1) {
		zeroSettingLog->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 72, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,static_cast<System::Byte>(0)));
		zeroSettingLog->SelectionAlignment = HorizontalAlignment::Center;
		zeroSettingLog->Text = ((command_delay / 10) + 1).ToString();
		command_delay--;

		if (command_delay == 1) {
			zeroSettingLog->Text = "";
			zeroSettingLog->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			zeroSettingLog->SelectionAlignment = HorizontalAlignment::Left;
			
		}
		return;
	}

	switch (current_zero_command) {
	case zero_commands::ZERO_BODY:
		
		// Command Initialization
		if (command_delay == 1) {
			command_delay = 0;

			if (!BodyMotor::startHoming()) {
				zeroSettingLog->Text = "BODY MOTOR:" + BodyMotor::device->getCompletedCodeString() + "\n";
				current_zero_command = zero_commands::NO_COMMAND;			
				finish = true;
				break;
			}
			zeroSettingLog->Text += "BODY MOTOR RUN \n";			
			break;
		}

		if (!BodyMotor::device->isReady()) break;
		zeroSettingLog->Text += "BODY MOTOR:" + BodyMotor::device->getCompletedCodeString() + "\n";

		current_zero_command = zero_commands::NO_COMMAND;
		zeroSettingAssigneImages();		
		finish = true;
		break;

	case zero_commands::ZERO_VERTICAL:

		// Command Initialization
		if (command_delay == 1) {
			command_delay = 0;
			if (!VerticalMotor::startHoming()) {
				zeroSettingLog->Text = "VERTICAL MOTOR:" + VerticalMotor::device->getCompletedCodeString() + "\n";
				current_zero_command = zero_commands::NO_COMMAND;
				finish = true;
				break;
			}
			zeroSettingLog->Text += "VERTICAL MOTOR RUN \n";
			break;
		}

		if (!VerticalMotor::device->isReady()) break;
		zeroSettingLog->Text += "VERTICAL MOTOR:" + VerticalMotor::device->getCompletedCodeString() + "\n";

		current_zero_command = zero_commands::NO_COMMAND;
		zeroSettingAssigneImages();
		finish = true;
		break;
	
	case zero_commands::ZERO_TILT:
		// Command Initialization
		if (command_delay == 1) {
			command_delay = 0;

			if (!TiltMotor::startHoming()) {
				zeroSettingLog->Text = "TILT MOTOR:" + TiltMotor::device->getCompletedCodeString() + "\n";
				current_zero_command = zero_commands::NO_COMMAND;
				finish = true;
				break;
			}
			zeroSettingLog->Text += "TILT MOTOR RUN \n";
			break;
		}

		if (!TiltMotor::device->isReady()) break;
		zeroSettingLog->Text += "TILT MOTOR:" + TiltMotor::device->getCompletedCodeString() + "\n";

		current_zero_command = zero_commands::NO_COMMAND;
		zeroSettingAssigneImages();
		break;

	case zero_commands::ZERO_SLIDE:
		
		// Command Initialization
		if (command_delay == 1) {
			command_delay = 0;

			if (!SlideMotor::startHoming()) {
				zeroSettingLog->Text = "SLIDE MOTOR:" + SlideMotor::device->getCompletedCodeString() + "\n";
				current_zero_command = zero_commands::NO_COMMAND;
				finish = true;
				break;
			}
			zeroSettingLog->Text += "SLIDE MOTOR RUN \n";
			break;
		}

		if (!SlideMotor::device->isReady()) break;
		zeroSettingLog->Text += "SLIDE MOTOR:" + SlideMotor::device->getCompletedCodeString() + "\n";

		current_zero_command = zero_commands::NO_COMMAND;
		zeroSettingAssigneImages();
		break;

	case zero_commands::ZERO_ARM:

		// Command Initialization
		if (command_delay == 1) {
			command_delay = 0;

			if (!ArmMotor::startHoming()) {
				zeroSettingLog->Text = "ARM MOTOR:" + ArmMotor::device->getCompletedCodeString() + "\n";
				current_zero_command = zero_commands::NO_COMMAND;
				finish = true;
				break;
			}
			zeroSettingLog->Text += "ARM MOTOR RUN \n";
			break;
		}

		if (!ArmMotor::device->isReady()) break;
		zeroSettingLog->Text += "ARM MOTOR:" + ArmMotor::device->getCompletedCodeString() + "\n";

		current_zero_command = zero_commands::NO_COMMAND;
		zeroSettingAssigneImages();
		break;

	}

	if(finish) serviceTimer->Stop();
}