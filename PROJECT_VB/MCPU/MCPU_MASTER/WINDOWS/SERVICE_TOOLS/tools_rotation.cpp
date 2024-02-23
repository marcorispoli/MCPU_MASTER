#include "ServiceForm.h"
#include "Notify.h"
#include "ArmMotor.h"
#include "VerticalMotor.h"
#include "TiltMotor.h"
#include "BodyMotor.h"
#include "SlideMotor.h"
#include "../WINDOWS/ConfirmationWindow.h"

/// <summary>
/// Thuis is the enumeration of the possible zero setting commands
/// </summary>
typedef enum {
	NO_COMMAND = 0,
	BODY,
	ARM,
	TILT,
	SLIDE,
}rotation_commands;
static rotation_commands current_rotation_command = rotation_commands::NO_COMMAND;


typedef enum {
	NO_SELECTION = 0,
	BODY_SELECTED,
	ARM_SELECTED,
	TILT_SELECTED,
	SLIDE_SELECTED,
}rotation_tool_selection;
static rotation_tool_selection current_rotation_selected = rotation_tool_selection::NO_SELECTION;

static int command_delay = 30;
static bool manual_mode = false;
static int selected_target;

#define INFO_REASON_BODY_ACTIVATION 0
#define INFO_REASON_TILT_ACTIVATION 1
#define INFO_REASON_ARM_ACTIVATION 2
#define INFO_REASON_SLIDE_ACTIVATION 3
#define INFO_REASON_BODY_ERROR 4
#define INFO_REASON_TILT_ERROR 5
#define INFO_REASON_ARM_ERROR 6
#define INFO_REASON_SLIDE_ERROR 7


#define ARM_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\Arm.PNG")
#define ARM_SELECTED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\ArmSelected.PNG")
#define BODY_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\Body.PNG")
#define BODY_SELECTED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\BodySelected.PNG")
#define TILT_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\Tilt.PNG")
#define TILT_SELECTED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\TiltSelected.PNG")
#define SLIDE_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\Slide.PNG")
#define SLIDE_SELECTED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\SlideSelected.PNG")
#define MANUAL_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\Manual.PNG")
#define MANUAL_SELECTED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\ManualSelected.PNG")
#define SELECTION_ICON Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\SelectionIcon.PNG")

#define ARM_VIEW_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\ArmView.PNG")
#define ARM_VIEW_MANUAL_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\ArmViewManual.PNG")
#define BODY_VIEW_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\BodyView.PNG")
#define BODY_VIEW_MANUAL_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\BodyViewManual.PNG")
#define TILT_VIEW_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\TiltView.PNG")
#define TILT_VIEW_MANUAL_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\TiltViewManual.PNG")
#define SLIDE_VIEW_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\SlideView.PNG")
#define SLIDE_VIEW_MANUAL_IMAGE Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\ROTATION\\SlideViewManual.PNG")

void ServiceForm::createRotationToolsPanel(void) {
	rotationToolPanel->SetBounds(0, 0, PANEL_WIDTH, PANEL_HIGH);
	rotationToolPanel->Location = System::Drawing::Point(PANEL_X, PANEL_Y);

	rotationToolPanel->BackColor = Color::Black;
	rotationToolArmButton->BackColor = Color::Transparent;
	rotationToolTiltButton->BackColor = Color::Transparent;
	rotationToolBodyButton->BackColor = Color::Transparent;
	rotationToolSlideButton->BackColor = Color::Transparent;
	rotationToolManualButton->BackColor = Color::Transparent;

	rotationToolSelectAngle1->BackColor = Color::Transparent;
	rotationToolSelectAngle2->BackColor = Color::Transparent;
	rotationToolSelectAngle3->BackColor = Color::Transparent;
	rotationToolSelectAngle4->BackColor = Color::Transparent;
	rotationToolSelectAngle5->BackColor = Color::Transparent;
	rotationToolSelectAngle6->BackColor = Color::Transparent;
	rotationToolSelectAngle7->BackColor = Color::Transparent;
	rotationToolSelectAngle8->BackColor = Color::Transparent;
	rotationToolSelectAngle9->BackColor = Color::Transparent;
	rotationToolTargetAngle->BackColor = Color::Transparent;

	rotationToolSelectionIcon->BackColor = Color::Transparent;
	rotationToolSelectionIcon->BackgroundImage = SELECTION_ICON;

	return;
}

void ServiceForm::initRotationToolPanel(void) {
	serviceMenuTitle->Text = Notify::TranslateLabel(Notify::messages::LABEL_ROTATION_TOOL_PANEL_TITLE);
	

	rotationToolView->Hide();

	current_rotation_command = rotation_commands::NO_COMMAND;
	current_rotation_selected = rotation_tool_selection::NO_SELECTION;

	rotationToolArmButton->BackgroundImage = ARM_IMAGE;
	rotationToolTiltButton->BackgroundImage = TILT_IMAGE;
	rotationToolBodyButton->BackgroundImage = BODY_IMAGE;
	rotationToolSlideButton->BackgroundImage = SLIDE_IMAGE;
	
	manual_mode = false;
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_STANDARD_STATUS_MANUAL_ROTATION);


	rotationToolManualButton->BackgroundImage = MANUAL_IMAGE;
	rotationToolSelectionIcon->Hide();
	rotationToolTargetAngle->Hide();

	rotationTool_select_item(nullptr);
	serviceTimer->Start();
	return;
}

void  ServiceForm::cancRotationToolPanel(void) {
	
	if (current_rotation_command != rotation_commands::NO_COMMAND) {
		switch (current_rotation_command) {
		case rotation_commands::BODY:BodyMotor::device->abortActivation(); break;
		case rotation_commands::TILT:TiltMotor::device->abortActivation(); break;
		case rotation_commands::ARM:ArmMotor::device->abortActivation(); break;
		case rotation_commands::SLIDE:SlideMotor::device->abortActivation(); break;
		}

		// Reset of the current selection
		current_rotation_command = rotation_commands::NO_COMMAND;
		rotationTool_select_item(nullptr);
		return;
	}

	// Disables the manual rotations
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED);

	serviceTimer->Stop();
	setActivePanel(panels::MAIN_SERVICE_PANEL);
}

System::Void ServiceForm::rotationTool_arm_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_rotation_command != rotation_commands::NO_COMMAND) return;
	rotationToolView->Hide();
	rotationTool_select_item(nullptr);

	this->rotationToolView->Location = System::Drawing::Point(75, 391);
	this->rotationToolView->Size = System::Drawing::Size(513, 450);
	this->rotationToolManualButton->Location = System::Drawing::Point(430, 12);
	rotationToolTargetAngle->SetBounds(0, 0, 250, 100);
	rotationToolTargetAngle->Location = System::Drawing::Point((450 - 250) / 2, (450 - 100) / 2);

	// Arm Button
	current_rotation_selected = rotation_tool_selection::ARM_SELECTED;
	rotationToolArmButton->BackgroundImage = ARM_SELECTED_IMAGE;
	rotationToolBodyButton->BackgroundImage = BODY_IMAGE;
	rotationToolTiltButton->BackgroundImage = TILT_IMAGE;
	rotationToolSlideButton->BackgroundImage = SLIDE_IMAGE;


	// Manual Mode disbaled
	manual_mode = false;
	rotationToolManualButton->BackgroundImage = MANUAL_IMAGE;
	

	// RotationToolView setup
	rotationToolView->BackgroundImage = ARM_VIEW_IMAGE;

	rotationToolTargetAngle->Show();


	rotationToolSelectAngle1->Show(); // 0°
	rotationToolSelectAngle1->Tag = "0";
	rotationToolSelectAngle1->Location = System::Drawing::Point(175, 8);

	rotationToolSelectAngle2->Show(); // 45°
	rotationToolSelectAngle2->Tag = "45";
	rotationToolSelectAngle2->Location = System::Drawing::Point(296, 55);

	rotationToolSelectAngle3->Show(); // 90°
	rotationToolSelectAngle3->Tag = "90";
	rotationToolSelectAngle3->Location = System::Drawing::Point(349, 176);

	rotationToolSelectAngle4->Show(); // 135°
	rotationToolSelectAngle4->Tag = "135";
	rotationToolSelectAngle4->Location = System::Drawing::Point(301, 297);

	rotationToolSelectAngle5->Show(); // 180°
	rotationToolSelectAngle5->Tag = "180";
	rotationToolSelectAngle5->Location = System::Drawing::Point(181, 350);

	rotationToolSelectAngle6->Show(); // -135°
	rotationToolSelectAngle6->Tag = "-135";
	rotationToolSelectAngle6->Location = System::Drawing::Point(60, 302);

	rotationToolSelectAngle7->Show(); // -90°
	rotationToolSelectAngle7->Tag = "-90";
	rotationToolSelectAngle7->Location = System::Drawing::Point(7, 182);

	rotationToolSelectAngle8->Show(); // -45°
	rotationToolSelectAngle8->Tag = "-45";
	rotationToolSelectAngle8->Location = System::Drawing::Point(55, 60);

	rotationToolSelectAngle9->Hide();
	rotationToolView->Show();

	rotationTooCreateInfoPanel(INFO_REASON_ARM_ACTIVATION);

}

System::Void ServiceForm::rotationTool_body_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_rotation_command != rotation_commands::NO_COMMAND) return;
	rotationToolView->Hide();
	rotationTool_select_item(nullptr);

	this->rotationToolView->Location = System::Drawing::Point(75, 391);
	this->rotationToolView->Size = System::Drawing::Size(513, 450);
	this->rotationToolManualButton->Location = System::Drawing::Point(430, 12);
	rotationToolTargetAngle->SetBounds(0, 0, 250, 100);
	rotationToolTargetAngle->Location = System::Drawing::Point((450 - 250) / 2, (450 - 100) / 2);

	// Body Button
	current_rotation_selected = rotation_tool_selection::BODY_SELECTED;
	rotationToolArmButton->BackgroundImage = ARM_IMAGE;
	rotationToolBodyButton->BackgroundImage = BODY_SELECTED_IMAGE;
	rotationToolTiltButton->BackgroundImage = TILT_IMAGE;
	rotationToolSlideButton->BackgroundImage = SLIDE_IMAGE;

	// Manual Mode disabled
	manual_mode = false;
	rotationToolManualButton->BackgroundImage = MANUAL_IMAGE;
	
	// RotationToolView setup
	if (manual_mode)	rotationToolView->BackgroundImage = BODY_VIEW_MANUAL_IMAGE;
	else rotationToolView->BackgroundImage = BODY_VIEW_IMAGE;

	rotationToolTargetAngle->Show();


	rotationToolSelectAngle5->Show(); // 0°
	rotationToolSelectAngle5->Tag = "0";
	rotationToolSelectAngle5->Location = System::Drawing::Point(181, 350);

	rotationToolSelectAngle4->Show(); // 45°
	rotationToolSelectAngle4->Tag = "45";
	rotationToolSelectAngle4->Location = System::Drawing::Point(301, 297);

	rotationToolSelectAngle3->Show(); // 90°
	rotationToolSelectAngle3->Tag = "90";
	rotationToolSelectAngle3->Location = System::Drawing::Point(349, 176);

	rotationToolSelectAngle6->Show(); // -45°
	rotationToolSelectAngle6->Tag = "-45";
	rotationToolSelectAngle6->Location = System::Drawing::Point(60, 302);

	rotationToolSelectAngle7->Show(); // -90°
	rotationToolSelectAngle7->Tag = "-90";
	rotationToolSelectAngle7->Location = System::Drawing::Point(7, 182);

	
	
	rotationToolSelectAngle1->Hide();
	rotationToolSelectAngle2->Hide();
	rotationToolSelectAngle8->Hide();
	rotationToolSelectAngle9->Hide();
	rotationToolView->Show();

	rotationTooCreateInfoPanel(INFO_REASON_BODY_ACTIVATION);
}


System::Void ServiceForm::rotationTool_tilt_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_rotation_command != rotation_commands::NO_COMMAND) return;
	rotationToolView->Hide();
	rotationTool_select_item(nullptr);

	this->rotationToolView->Location = System::Drawing::Point((600-543)/2, 391);
	this->rotationToolView->Size = System::Drawing::Size(543, 451);

	this->rotationToolManualButton->Location = System::Drawing::Point(430, 50);
	
	rotationToolTargetAngle->SetBounds(0, 0, 250, 100);
	rotationToolTargetAngle->Location = System::Drawing::Point((543 - 250) / 2, 80 + (450 - 100) / 2);
	
	current_rotation_selected = rotation_tool_selection::TILT_SELECTED;
	rotationToolArmButton->BackgroundImage = ARM_IMAGE;
	rotationToolBodyButton->BackgroundImage = BODY_IMAGE;
	rotationToolTiltButton->BackgroundImage = TILT_SELECTED_IMAGE;
	rotationToolSlideButton->BackgroundImage = SLIDE_IMAGE;

	manual_mode = false;
	
	// RotationToolView setup
	if (manual_mode)	rotationToolView->BackgroundImage = TILT_VIEW_MANUAL_IMAGE;
	else rotationToolView->BackgroundImage = TILT_VIEW_IMAGE;

	rotationToolTargetAngle->Show();

	
	rotationToolSelectAngle1->Show(); // 0°
	rotationToolSelectAngle1->Tag = "0";
	rotationToolSelectAngle1->Location = System::Drawing::Point(227, 132);

	rotationToolSelectAngle2->Show(); // 15°
	rotationToolSelectAngle2->Tag = "15";
	rotationToolSelectAngle2->Location = System::Drawing::Point(350, 158);

	rotationToolSelectAngle3->Show(); // 25°
	rotationToolSelectAngle3->Tag = "25";
	rotationToolSelectAngle3->Location = System::Drawing::Point(442, 222);

	rotationToolSelectAngle4->Show(); // -15°
	rotationToolSelectAngle4->Tag = "-15";
	rotationToolSelectAngle4->Location = System::Drawing::Point(108, 158);

	rotationToolSelectAngle5->Show(); // -25°
	rotationToolSelectAngle5->Tag = "-25";
	rotationToolSelectAngle5->Location = System::Drawing::Point(6, 221);

	rotationToolSelectAngle6->Hide(); 
	rotationToolSelectAngle7->Hide(); 
	rotationToolSelectAngle8->Hide(); 
	rotationToolSelectAngle9->Hide();

	rotationToolView->Show();

	rotationTooCreateInfoPanel(INFO_REASON_TILT_ACTIVATION);

}
System::Void ServiceForm::rotationTool_slide_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_rotation_command != rotation_commands::NO_COMMAND) return;
	rotationToolView->Hide();
	rotationTool_select_item(nullptr);
	
	current_rotation_selected = rotation_tool_selection::SLIDE_SELECTED;
	
	this->rotationToolView->Location = System::Drawing::Point((600 - 543) / 2, 391);
	this->rotationToolView->Size = System::Drawing::Size(543, 451);

	this->rotationToolManualButton->Location = System::Drawing::Point(430, 50);

	rotationToolTargetAngle->SetBounds(0, 0, 250, 100);
	rotationToolTargetAngle->Location = System::Drawing::Point(30 + (543 - 250) / 2, ((450 - 100) / 2) - 40 );
	
	rotationToolArmButton->BackgroundImage = ARM_IMAGE;
	rotationToolBodyButton->BackgroundImage = BODY_IMAGE;
	rotationToolTiltButton->BackgroundImage = TILT_IMAGE;
	rotationToolSlideButton->BackgroundImage = SLIDE_SELECTED_IMAGE;


	manual_mode = false;
	
	// RotationToolView setup
	if (manual_mode)	rotationToolView->BackgroundImage = SLIDE_VIEW_MANUAL_IMAGE;
	else rotationToolView->BackgroundImage = SLIDE_VIEW_IMAGE;

	rotationToolTargetAngle->Show();

	rotationToolSelectAngle1->Show(); // 0°
	rotationToolSelectAngle1->Tag = "0";
	rotationToolSelectAngle1->Location = System::Drawing::Point(27, 41);

	rotationToolSelectAngle2->Show(); // 10°
	rotationToolSelectAngle2->Tag = "10";
	rotationToolSelectAngle2->Location = System::Drawing::Point(53, 152);

	rotationToolSelectAngle3->Show(); // 90°
	rotationToolSelectAngle3->Tag = "89";
	rotationToolSelectAngle3->Location = System::Drawing::Point(350, 336);

	rotationToolSelectAngle4->Hide(); 
	rotationToolSelectAngle5->Hide(); 
	rotationToolSelectAngle6->Hide();
	rotationToolSelectAngle7->Hide();
	rotationToolSelectAngle8->Hide();
	rotationToolSelectAngle9->Hide();

	rotationToolView->Show();

	// Confirmation Panel Setup ____________________________________________________________
	rotationTooCreateInfoPanel(INFO_REASON_SLIDE_ACTIVATION);


}

void ServiceForm::rotationTooCreateInfoPanel(unsigned char reason) {
	System::String^ confInfoTitle;
	System::String^ confInfoContent;
	ConfirmationWindow^ errInfo;

	switch (reason) {
	case INFO_REASON_BODY_ACTIVATION:
		confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_BODY_ACTIVATION_CONFIRMATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_BODY_ACTIVATION_CONFIRMATION);
		confInfoContent = Notify::TranslateContent(Notify::messages::INFO_BODY_ACTIVATION_CONFIRMATION);
		pConfirmation = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
		((ConfirmationWindow^)pConfirmation)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionCancCallback);
		((ConfirmationWindow^)pConfirmation)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionBodyOkCallback);

		break;
	case INFO_REASON_BODY_ERROR:
		confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_BODY_ACTIVATION_FAILED) + "] " + Notify::TranslateTitle(Notify::messages::INFO_BODY_ACTIVATION_FAILED);
		confInfoContent = Notify::TranslateContent(Notify::messages::INFO_BODY_ACTIVATION_FAILED);
		errInfo = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::ERR_WIN, confInfoTitle, confInfoContent);
		errInfo->open();

		break;
	case INFO_REASON_ARM_ACTIVATION:
		confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_ARM_ACTIVATION_CONFIRMATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_ARM_ACTIVATION_CONFIRMATION);
	    confInfoContent = Notify::TranslateContent(Notify::messages::INFO_ARM_ACTIVATION_CONFIRMATION);
		pConfirmation = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
		((ConfirmationWindow^)pConfirmation)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionCancCallback);
		((ConfirmationWindow^)pConfirmation)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionArmOkCallback);

		break;
	case INFO_REASON_ARM_ERROR:
		confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_ARM_ACTIVATION_FAILED) + "] " + Notify::TranslateTitle(Notify::messages::INFO_ARM_ACTIVATION_FAILED);
		confInfoContent = Notify::TranslateContent(Notify::messages::INFO_ARM_ACTIVATION_FAILED);
		errInfo = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::ERR_WIN, confInfoTitle, confInfoContent);
		errInfo->open();

		break;
	case INFO_REASON_TILT_ACTIVATION:
		confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_TILT_ACTIVATION_CONFIRMATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_TILT_ACTIVATION_CONFIRMATION);
		confInfoContent = Notify::TranslateContent(Notify::messages::INFO_TILT_ACTIVATION_CONFIRMATION);
		pConfirmation = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
		((ConfirmationWindow^)pConfirmation)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionCancCallback);
		((ConfirmationWindow^)pConfirmation)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionTiltOkCallback);

		break;
	case INFO_REASON_TILT_ERROR:
		confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_TILT_ACTIVATION_FAILED) + "] " + Notify::TranslateTitle(Notify::messages::INFO_TILT_ACTIVATION_FAILED);
		confInfoContent = Notify::TranslateContent(Notify::messages::INFO_TILT_ACTIVATION_FAILED);
		errInfo = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::ERR_WIN, confInfoTitle, confInfoContent);
		errInfo->open();

		break;
	case INFO_REASON_SLIDE_ACTIVATION:
		confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION);
		confInfoContent = Notify::TranslateContent(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION);

		pConfirmation = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
		((ConfirmationWindow^)pConfirmation)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionCancCallback);
		((ConfirmationWindow^)pConfirmation)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionSlideOkCallback);

		break;
	case INFO_REASON_SLIDE_ERROR:
		confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_SLIDE_ACTIVATION_FAILED) + "] " + Notify::TranslateTitle(Notify::messages::INFO_SLIDE_ACTIVATION_FAILED);
		confInfoContent = Notify::TranslateContent(Notify::messages::INFO_SLIDE_ACTIVATION_FAILED);
		errInfo = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::ERR_WIN, confInfoTitle, confInfoContent);
		errInfo->open();
		break;

	}
	
}

System::Void ServiceForm::rotationTool_manual_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_rotation_command != rotation_commands::NO_COMMAND) return;

	manual_mode = !manual_mode;

	// Set the manual button icon
	if(manual_mode) rotationToolManualButton->BackgroundImage = MANUAL_SELECTED_IMAGE;
	else {
		rotationToolManualButton->BackgroundImage = MANUAL_IMAGE;
		Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_STANDARD_STATUS_MANUAL_ROTATION);
	}

	// Resets the selection
	rotationTool_select_item(nullptr);

	// Activates the proper View Background
	if (current_rotation_selected == rotation_tool_selection::TILT_SELECTED) {
		if (manual_mode) {
			Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_TILT_MANUAL_ROTATION);
			rotationToolView->BackgroundImage = TILT_VIEW_MANUAL_IMAGE;
		}
		else rotationToolView->BackgroundImage = TILT_VIEW_IMAGE;

	}
	else if (current_rotation_selected == rotation_tool_selection::ARM_SELECTED) {
		if (manual_mode) {
			Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_ARM_MANUAL_ROTATION);
			rotationToolView->BackgroundImage = ARM_VIEW_MANUAL_IMAGE;
		}
		else rotationToolView->BackgroundImage = ARM_VIEW_IMAGE;

	}
	else if (current_rotation_selected == rotation_tool_selection::BODY_SELECTED) {
		if (manual_mode) {
			Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_BODY_MANUAL_ROTATION);
			rotationToolView->BackgroundImage = BODY_VIEW_MANUAL_IMAGE;
		}
		else rotationToolView->BackgroundImage = BODY_VIEW_IMAGE;

	}
	else if (current_rotation_selected == rotation_tool_selection::SLIDE_SELECTED) {
		if (manual_mode) {
			Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_SLIDE_MANUAL_ROTATION);
			rotationToolView->BackgroundImage = SLIDE_VIEW_MANUAL_IMAGE;
		}
		else rotationToolView->BackgroundImage = SLIDE_VIEW_IMAGE;

	}
}


void ServiceForm::rotationToolSelectionCancCallback(void) {
	rotationTool_select_item(nullptr);
}

void ServiceForm::rotationToolSelectionArmOkCallback(void) {

	// Busy condition
	if (current_rotation_command) return;

	if (ArmMotor::serviceAutoPosition(selected_target)) {
		current_rotation_command = rotation_commands::ARM;
	}
	else {
		rotationTool_select_item(nullptr);
		rotationTooCreateInfoPanel(INFO_REASON_ARM_ERROR);
	}
}

void ServiceForm::rotationToolSelectionTiltOkCallback(void) {	
	
	// Busy condition
	if (current_rotation_command) return;

	if (TiltMotor::serviceAutoPosition(selected_target)) {
		current_rotation_command = rotation_commands::TILT;
	}
	else {
		rotationTool_select_item(nullptr);
		rotationTooCreateInfoPanel(INFO_REASON_TILT_ERROR);
	}
}

void ServiceForm::rotationToolSelectionBodyOkCallback(void) {	

	// Busy condition
	if (current_rotation_command) return;

	if (BodyMotor::serviceAutoPosition(selected_target)) {
		current_rotation_command = rotation_commands::BODY;
	}
	else {
		rotationTool_select_item(nullptr);
		rotationTooCreateInfoPanel(INFO_REASON_BODY_ERROR);
	}
}

void ServiceForm::rotationToolSelectionSlideOkCallback(void) {
	
	// Busy condition
	if (current_rotation_command) return;

	if (SlideMotor::serviceAutoPosition(selected_target)) {
		current_rotation_command = rotation_commands::SLIDE;
	}
	else {
		rotationTool_select_item(nullptr);
		rotationTooCreateInfoPanel(INFO_REASON_SLIDE_ERROR);
	}
}

void ServiceForm::rotationTool_select_item(System::Windows::Forms::PictureBox^ item) {
	
	if (current_rotation_command != rotation_commands::NO_COMMAND) return;

	// Initialize the internal static pointer
	if ((item == nullptr) ||(manual_mode)) {
		
		rotationToolSelectAngle1->BackgroundImage = nullptr;
		rotationToolSelectAngle2->BackgroundImage = nullptr;
		rotationToolSelectAngle3->BackgroundImage = nullptr;
		rotationToolSelectAngle4->BackgroundImage = nullptr;
		rotationToolSelectAngle5->BackgroundImage = nullptr;
		rotationToolSelectAngle6->BackgroundImage = nullptr;
		rotationToolSelectAngle7->BackgroundImage = nullptr;
		rotationToolSelectAngle8->BackgroundImage = nullptr;
		rotationToolSelectAngle9->BackgroundImage = nullptr;

		return;
	}
		
	// The item is already selected
	if (item->BackgroundImage != nullptr) return;
		

	// Selects the angle
	rotationToolSelectAngle1->BackgroundImage = nullptr;
	rotationToolSelectAngle2->BackgroundImage = nullptr;
	rotationToolSelectAngle3->BackgroundImage = nullptr;
	rotationToolSelectAngle4->BackgroundImage = nullptr;
	rotationToolSelectAngle5->BackgroundImage = nullptr;
	rotationToolSelectAngle6->BackgroundImage = nullptr;
	rotationToolSelectAngle7->BackgroundImage = nullptr;
	rotationToolSelectAngle8->BackgroundImage = nullptr;
	rotationToolSelectAngle9->BackgroundImage = nullptr;
	item->BackgroundImage = SELECTION_ICON;

	selected_target = System::Convert::ToInt16(item->Tag);
	((ConfirmationWindow^)pConfirmation)->open();
	
}

System::Void ServiceForm::rotationTool_selection_angle_1_Click(System::Object^ sender, System::EventArgs^ e) {
	rotationTool_select_item(rotationToolSelectAngle1);
	
}

System::Void ServiceForm::rotationTool_selection_angle_2_Click(System::Object^ sender, System::EventArgs^ e) {
	rotationTool_select_item(rotationToolSelectAngle2);
}

System::Void ServiceForm::rotationTool_selection_angle_3_Click(System::Object^ sender, System::EventArgs^ e) {
	rotationTool_select_item(rotationToolSelectAngle3);
}

System::Void ServiceForm::rotationTool_selection_angle_4_Click(System::Object^ sender, System::EventArgs^ e) {
	rotationTool_select_item(rotationToolSelectAngle4);
}

System::Void ServiceForm::rotationTool_selection_angle_5_Click(System::Object^ sender, System::EventArgs^ e) {
	rotationTool_select_item(rotationToolSelectAngle5);
}

System::Void ServiceForm::rotationTool_selection_angle_6_Click(System::Object^ sender, System::EventArgs^ e) {
	rotationTool_select_item(rotationToolSelectAngle6);
}

System::Void ServiceForm::rotationTool_selection_angle_7_Click(System::Object^ sender, System::EventArgs^ e) {
	rotationTool_select_item(rotationToolSelectAngle7);
}

System::Void ServiceForm::rotationTool_selection_angle_8_Click(System::Object^ sender, System::EventArgs^ e) {
	rotationTool_select_item(rotationToolSelectAngle8);
}

System::Void ServiceForm::rotationTool_selection_angle_9_Click(System::Object^ sender, System::EventArgs^ e) {
	rotationTool_select_item(rotationToolSelectAngle9);
}



using namespace System::Drawing;

void ServiceForm::rotationToolPanelTimer(void) {
	
	
	if (current_rotation_selected == rotation_tool_selection::ARM_SELECTED) {
		rotationToolTargetAngle->Text = (ArmMotor::device->getCurrentPosition() / 100).ToString();
	}
	else if (current_rotation_selected == rotation_tool_selection::TILT_SELECTED) {
		rotationToolTargetAngle->Text = (TiltMotor::device->getCurrentPosition() / 100).ToString();
	}else if (current_rotation_selected == rotation_tool_selection::BODY_SELECTED) {
		rotationToolTargetAngle->Text = (BodyMotor::device->getCurrentPosition() / 10).ToString();
	}else if (current_rotation_selected == rotation_tool_selection::SLIDE_SELECTED) {
		rotationToolTargetAngle->Text = (SlideMotor::device->getCurrentPosition() / 100).ToString();
	}
	
	// Command completion
	if (current_rotation_command) {
		if (
			(!ArmMotor::device->isBusy()) &&
			(!TiltMotor::device->isBusy()) &&
			(!BodyMotor::device->isBusy()) &&
			(!SlideMotor::device->isBusy())
			) current_rotation_command = rotation_commands::NO_COMMAND;
	}
	
}