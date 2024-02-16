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

#define CALIB_ZERO_SETTING_BACKGROUND System::Drawing::Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\CALIBRATION\\ZERO_SETTING\\ZeroSettingBackground.PNG")

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

void ServiceForm::createZeroSettingPanel(void) {
	calibZerosettingPanel->SetBounds(0, 0, PANEL_WIDTH, PANEL_HIGH);
	calibZerosettingPanel->BackgroundImage = CALIB_ZERO_SETTING_BACKGROUND;
	calibZerosettingPanel->Location = System::Drawing::Point(PANEL_X, PANEL_Y);

	zeroSettingArm->BackColor = Color::Transparent;
	zeroSettingBody->BackColor = Color::Transparent;
	zeroSettingSlide->BackColor = Color::Transparent;
	zeroSettingTilt->BackColor = Color::Transparent;
	zeroSettingVertical->BackColor = Color::Transparent;
	zeroSettingAll->BackColor = Color::Transparent;

	//
	return;
}

void ServiceForm::initZeroSettingCalibrationPanel(void) {
	serviceMenuTitle->Text = Notify::TranslateLabel(Notify::messages::LABEL_ZERO_SETTING_PANEL_TITLE);
	current_zero_command = zero_commands::NO_COMMAND;
	zeroSettingLog->Clear();	
	return;
}
void  ServiceForm::cancZeroSettingPanel(void) {
	if (current_zero_command != zero_commands::NO_COMMAND) {
		switch (current_zero_command) {
		case zero_commands::ZERO_BODY:BodyMotor::device->abortActivation(); break;
		case zero_commands::ZERO_VERTICAL:VerticalMotor::device->abortActivation(); break;
		case zero_commands::ZERO_TILT:TiltMotor::device->abortActivation(); break;
		case zero_commands::ZERO_ARM:ArmMotor::device->abortActivation(); break;
		case zero_commands::ZERO_SLIDE:SlideMotor::device->abortActivation(); break;
		case zero_commands::ZERO_ALL:
			SlideMotor::device->abortActivation(); 
			ArmMotor::device->abortActivation();
			BodyMotor::device->abortActivation();
			VerticalMotor::device->abortActivation();
			TiltMotor::device->abortActivation();
			break;
		}
		return;
	}

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
	current_zero_command = zero_commands::ZERO_ALL;
	serviceTimer->Start();
}


using namespace System::Drawing;

void ServiceForm::zeroSettingPanelTimer(void) {
	bool finish = false;
	static int zero_all_fase = 0;

	// The zero all fase is initiated
	if (zero_all_fase) {
		switch (zero_all_fase) {
			
		case 1: // TILT
			if (!TiltMotor::startHoming()) {
				zeroSettingLog->Text += "TILT ERROR -> " + TiltMotor::device->getCommandCompletedCode().ToString() + "\n";
				zero_all_fase += 2;
				return;
			}

			zero_all_fase++;
			zeroSettingLog->Text += "TILT MOTOR RUN \n";
			break;

		case 2: 
			if (!TiltMotor::device->isReady()) return;
			zeroSettingLog->Text += "TILT MOTOR:" + TiltMotor::device->getCommandCompletedCode().ToString() + "\n";
			zero_all_fase++;
			return;

		case 3: // SLIDE
			if (!SlideMotor::startHoming()) {
				zeroSettingLog->Text += "SLIDE ERROR -> " + SlideMotor::device->getCommandCompletedCode().ToString() + "\n";
				zero_all_fase += 2;
				return;
			}

			zero_all_fase++;
			zeroSettingLog->Text += "SLIDE MOTOR RUN \n";
			break;

		case 4:
			if (!SlideMotor::device->isReady()) return;
			zeroSettingLog->Text += "SLIDE MOTOR:" + SlideMotor::device->getCommandCompletedCode().ToString() + "\n";
			zero_all_fase++;
			return;

		case 5: // ARM
			if (!ArmMotor::startHoming()) {
				zeroSettingLog->Text += "ARM ERROR -> " + ArmMotor::device->getCommandCompletedCode().ToString() + "\n";
				zero_all_fase += 2;
				return;
			}

			zero_all_fase++;
			zeroSettingLog->Text += "ARM MOTOR RUN \n";
			break;

		case 6:
			if (!ArmMotor::device->isReady()) return;
			zeroSettingLog->Text += "ARM MOTOR:" + ArmMotor::device->getCommandCompletedCode().ToString() + "\n";
			zero_all_fase++;
			return;

		case 7: // BODY
			if (!BodyMotor::startHoming()) {
				zeroSettingLog->Text += "BODY ERROR -> " + BodyMotor::device->getCommandCompletedCode().ToString() + "\n";
				zero_all_fase += 2;
				return;
			}

			zero_all_fase++;
			zeroSettingLog->Text += "BODY MOTOR RUN \n";
			break;

		case 8:
			if (!BodyMotor::device->isReady()) return;
			zeroSettingLog->Text += "BODY MOTOR:" + BodyMotor::device->getCommandCompletedCode().ToString() + "\n";
			zero_all_fase++;
			return;

		case 9: // VERTICAL
			if (!VerticalMotor::startHoming()) {
				zeroSettingLog->Text += "VERTICAL ERROR -> " + VerticalMotor::device->getCommandCompletedCode().ToString() + "\n";
				zero_all_fase += 2;
				return;
			}

			zero_all_fase++;
			zeroSettingLog->Text += "VERTICAL MOTOR RUN \n";
			break;

		case 10:
			if (!VerticalMotor::device->isReady()) return;
			zeroSettingLog->Text += "VERTICAL MOTOR:" + VerticalMotor::device->getCommandCompletedCode().ToString() + "\n";
			zero_all_fase++;
			return;
		case 11:
			zeroSettingLog->Text += "ZERO ALL COMMAND COMPLETED \n";
			zero_all_fase = 0;
			current_zero_command = zero_commands::NO_COMMAND;
			serviceTimer->Stop();
			break;

		default:
			zeroSettingLog->Text += "ZERO ALL COMMAND COMPLETED WITH INVALID STATUS\n";
			zero_all_fase = 0;
			current_zero_command = zero_commands::NO_COMMAND;
			serviceTimer->Stop();
		}

		return;
	}

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
				zeroSettingLog->Text = "BODY MOTOR:" + BodyMotor::device->getCommandCompletedCode().ToString() + "\n";
				current_zero_command = zero_commands::NO_COMMAND;			
				finish = true;
				break;
			}
			zeroSettingLog->Text += "BODY MOTOR RUN \n";			
			break;
		}

		if (!BodyMotor::device->isReady()) break;
		zeroSettingLog->Text += "BODY MOTOR:" + BodyMotor::device->getCommandCompletedCode().ToString() + "\n";

		current_zero_command = zero_commands::NO_COMMAND;
		finish = true;
		break;

	case zero_commands::ZERO_VERTICAL:

		// Command Initialization
		if (command_delay == 1) {
			command_delay = 0;
			if (!VerticalMotor::startHoming()) {
				zeroSettingLog->Text = "VERTICAL MOTOR:" + VerticalMotor::device->getCommandCompletedCode().ToString() + "\n";
				current_zero_command = zero_commands::NO_COMMAND;
				finish = true;
				break;
			}
			zeroSettingLog->Text += "VERTICAL MOTOR RUN \n";
			break;
		}

		if (!VerticalMotor::device->isReady()) break;
		zeroSettingLog->Text += "VERTICAL MOTOR:" + VerticalMotor::device->getCommandCompletedCode().ToString() + "\n";

		current_zero_command = zero_commands::NO_COMMAND;
		finish = true;
		break;
	
	case zero_commands::ZERO_TILT:
		// Command Initialization
		if (command_delay == 1) {
			command_delay = 0;

			if (!TiltMotor::startHoming()) {
				zeroSettingLog->Text = "TILT MOTOR:" + TiltMotor::device->getCommandCompletedCode().ToString() + "\n";
				current_zero_command = zero_commands::NO_COMMAND;
				finish = true;
				break;
			}
			zeroSettingLog->Text += "TILT MOTOR RUN \n";
			break;
		}

		if (!TiltMotor::device->isReady()) break;
		zeroSettingLog->Text += "TILT MOTOR:" + TiltMotor::device->getCommandCompletedCode().ToString() + "\n";

		current_zero_command = zero_commands::NO_COMMAND;
		break;

	case zero_commands::ZERO_SLIDE:
		
		// Command Initialization
		if (command_delay == 1) {
			command_delay = 0;

			if (!SlideMotor::startHoming()) {
				zeroSettingLog->Text = "SLIDE MOTOR:" + SlideMotor::device->getCommandCompletedCode().ToString() + "\n";
				current_zero_command = zero_commands::NO_COMMAND;
				finish = true;
				break;
			}
			zeroSettingLog->Text += "SLIDE MOTOR RUN \n";
			break;
		}

		if (!SlideMotor::device->isReady()) break;
		zeroSettingLog->Text += "SLIDE MOTOR:" + SlideMotor::device->getCommandCompletedCode().ToString() + "\n";

		current_zero_command = zero_commands::NO_COMMAND;
		break;

	case zero_commands::ZERO_ARM:

		// Command Initialization
		if (command_delay == 1) {
			command_delay = 0;

			if (!ArmMotor::startHoming()) {
				zeroSettingLog->Text = "ARM MOTOR:" + ArmMotor::device->getCommandCompletedCode().ToString() + "\n";
				current_zero_command = zero_commands::NO_COMMAND;
				finish = true;
				break;
			}
			zeroSettingLog->Text += "ARM MOTOR RUN \n";
			break;
		}

		if (!ArmMotor::device->isReady()) break;
		zeroSettingLog->Text += "ARM MOTOR:" + ArmMotor::device->getCommandCompletedCode().ToString() + "\n";

		current_zero_command = zero_commands::NO_COMMAND;
		break;

	case zero_commands::ZERO_ALL:

		// Command Initialization
		if (command_delay == 1) {
			command_delay = 0;

			zeroSettingLog->Text = "ZERO ALL COMMAND STARTED \n";
			zero_all_fase = 1;
			return;
		}
		
		break;


	}

	if(finish) serviceTimer->Stop();
}