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
		return;
	}

	serviceTimer->Stop();
	setActivePanel(panels::MAIN_SERVICE_PANEL);
}



System::Void ServiceForm::rotationTool_body_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_rotation_command != rotation_commands::NO_COMMAND) return;

	// Body Button
	current_rotation_selected = rotation_tool_selection::BODY_SELECTED;

	// Manual Mode disbaled
	manual_mode = false;
	rotationToolManualButton->BackgroundImage = MANUAL_IMAGE;
	
	// RotationToolView setup
	if (manual_mode)	rotationToolView->BackgroundImage = BODY_VIEW_MANUAL_IMAGE;
	else rotationToolView->BackgroundImage = BODY_VIEW_IMAGE;

	rotationToolTargetAngle->Show();

	rotationToolTargetAngle->SetBounds(0, 0, 250, 100);
	rotationToolTargetAngle->Location = System::Drawing::Point((450 - 250) / 2, (450 - 100) / 2);
	

	rotationToolSelectAngle1->Show(); // 0°
	rotationToolSelectAngle1->Tag = "0";
	rotationToolSelectAngle1->Location = System::Drawing::Point(174, 8);

	rotationToolSelectAngle2->Show(); // 45°
	rotationToolSelectAngle2->Tag = "45";
	rotationToolSelectAngle2->Location = System::Drawing::Point(295, 56);

	rotationToolSelectAngle3->Show(); // 90°
	rotationToolSelectAngle3->Tag = "90";
	rotationToolSelectAngle3->Location = System::Drawing::Point(347, 175);

	rotationToolSelectAngle4->Show(); // -45°
	rotationToolSelectAngle4->Tag = "-45";
	rotationToolSelectAngle4->Location = System::Drawing::Point(55, 60);


	rotationToolSelectAngle5->Show(); // -90°
	rotationToolSelectAngle5->Tag = "-90";
	rotationToolSelectAngle5->Location = System::Drawing::Point(7, 181);

	rotationToolSelectAngle6->Hide();
	rotationToolView->Show();
	rotationToolSelectAngle7->Hide();
	rotationToolView->Show();
	rotationToolSelectAngle8->Hide();
	rotationToolView->Show();
	rotationToolSelectAngle9->Hide();
	rotationToolView->Show();

	// Confirmation Panel Setup ____________________________________________________________
	System::String^ confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_BODY_ACTIVATION_CONFIRMATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_BODY_ACTIVATION_CONFIRMATION);
	System::String^ confInfoContent = Notify::TranslateContent(Notify::messages::INFO_BODY_ACTIVATION_CONFIRMATION);
	pConfirmation = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
	((ConfirmationWindow^)pConfirmation)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionCancCallback);
	((ConfirmationWindow^)pConfirmation)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionBodyOkCallback);


}
System::Void ServiceForm::rotationTool_arm_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_rotation_command != rotation_commands::NO_COMMAND) return;

	// Arm Button
	current_rotation_selected = rotation_tool_selection::ARM_SELECTED;
	rotationToolArmButton->BackgroundImage = ARM_SELECTED_IMAGE;

	// Manual Mode disbaled
	manual_mode = false;
	rotationToolManualButton->BackgroundImage = MANUAL_IMAGE;

	// RotationToolView setup
	if(manual_mode)	rotationToolView->BackgroundImage = ARM_VIEW_MANUAL_IMAGE;
	else rotationToolView->BackgroundImage = ARM_VIEW_IMAGE;

	rotationToolTargetAngle->Show();

	rotationToolTargetAngle->SetBounds(0, 0, 250, 100);
	rotationToolTargetAngle->Location = System::Drawing::Point((450-250)/2, (450-100)/2);
	

	rotationToolSelectAngle1->Show(); // 0°
	rotationToolSelectAngle1->Tag = "0";
	rotationToolSelectAngle1->Location = System::Drawing::Point(174, 8);

	rotationToolSelectAngle2->Show(); // 45°
	rotationToolSelectAngle2->Tag = "45";
	rotationToolSelectAngle2->Location = System::Drawing::Point(295, 56);

	rotationToolSelectAngle3->Show(); // 90°
	rotationToolSelectAngle3->Tag = "90";
	rotationToolSelectAngle3->Location = System::Drawing::Point(347, 175);

	rotationToolSelectAngle4->Show(); // 135°
	rotationToolSelectAngle4->Tag = "135";
	rotationToolSelectAngle4->Location = System::Drawing::Point(300, 296);

	rotationToolSelectAngle5->Show(); // 180°
	rotationToolSelectAngle5->Tag = "180";
	rotationToolSelectAngle5->Location = System::Drawing::Point(181, 349);

	rotationToolSelectAngle6->Show(); // -135°
	rotationToolSelectAngle6->Tag = "-135";
	rotationToolSelectAngle6->Location = System::Drawing::Point(59, 301);

	rotationToolSelectAngle7->Show(); // -90°
	rotationToolSelectAngle7->Tag = "-90";
	rotationToolSelectAngle7->Location = System::Drawing::Point(7, 181);

	rotationToolSelectAngle8->Show(); // -45°
	rotationToolSelectAngle8->Tag = "-45";
	rotationToolSelectAngle8->Location = System::Drawing::Point(55, 60);

	rotationToolSelectAngle9->Hide();
	rotationToolView->Show();

	// Confirmation Panel Setup ____________________________________________________________
	System::String^ confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_ARM_ACTIVATION_CONFIRMATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_ARM_ACTIVATION_CONFIRMATION);
	System::String^ confInfoContent = Notify::TranslateContent(Notify::messages::INFO_ARM_ACTIVATION_CONFIRMATION);
	pConfirmation = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
	((ConfirmationWindow^)pConfirmation)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionCancCallback);
	((ConfirmationWindow^)pConfirmation)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionArmOkCallback);

	
}

System::Void ServiceForm::rotationTool_tilt_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_rotation_command != rotation_commands::NO_COMMAND) return;
	current_rotation_selected = rotation_tool_selection::TILT_SELECTED;
	manual_mode = false;
	
	// RotationToolView setup
	if (manual_mode)	rotationToolView->BackgroundImage = TILT_VIEW_MANUAL_IMAGE;
	else rotationToolView->BackgroundImage = TILT_VIEW_IMAGE;

	rotationToolTargetAngle->Show();

	rotationToolTargetAngle->SetBounds(0, 0, 250, 100);
	rotationToolTargetAngle->Location = System::Drawing::Point((450 - 250) / 2, (450 - 100) / 2);
	

	rotationToolSelectAngle1->Show(); // 0°
	rotationToolSelectAngle1->Tag = "0";
	rotationToolSelectAngle1->Location = System::Drawing::Point(174, 8);

	rotationToolSelectAngle2->Show(); // 15°
	rotationToolSelectAngle2->Tag = "15";
	rotationToolSelectAngle2->Location = System::Drawing::Point(295, 56);

	rotationToolSelectAngle3->Show(); // 25°
	rotationToolSelectAngle3->Tag = "25";
	rotationToolSelectAngle3->Location = System::Drawing::Point(347, 175);

	rotationToolSelectAngle4->Show(); // -15°
	rotationToolSelectAngle4->Tag = "-15";
	rotationToolSelectAngle4->Location = System::Drawing::Point(300, 296);

	rotationToolSelectAngle5->Show(); // -25°
	rotationToolSelectAngle5->Tag = "-25";
	rotationToolSelectAngle5->Location = System::Drawing::Point(181, 349);

	rotationToolSelectAngle6->Hide(); 
	rotationToolSelectAngle7->Hide(); 
	rotationToolSelectAngle8->Hide(); 
	rotationToolSelectAngle9->Hide();

	rotationToolView->Show();

	// Confirmation Panel Setup ____________________________________________________________
	System::String^ confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_TILT_ACTIVATION_CONFIRMATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_TILT_ACTIVATION_CONFIRMATION);
	System::String^ confInfoContent = Notify::TranslateContent(Notify::messages::INFO_TILT_ACTIVATION_CONFIRMATION);
	pConfirmation = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
	((ConfirmationWindow^)pConfirmation)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionCancCallback);
	((ConfirmationWindow^)pConfirmation)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionTiltOkCallback);


}
System::Void ServiceForm::rotationTool_slide_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_rotation_command != rotation_commands::NO_COMMAND) return;
	current_rotation_selected = rotation_tool_selection::SLIDE_SELECTED;
	manual_mode = false;
	
	// RotationToolView setup
	if (manual_mode)	rotationToolView->BackgroundImage = SLIDE_VIEW_MANUAL_IMAGE;
	else rotationToolView->BackgroundImage = SLIDE_VIEW_IMAGE;

	rotationToolTargetAngle->Show();

	rotationToolTargetAngle->SetBounds(0, 0, 250, 100);
	rotationToolTargetAngle->Location = System::Drawing::Point((450 - 250) / 2, (450 - 100) / 2);


	rotationToolSelectAngle1->Show(); // 0°
	rotationToolSelectAngle1->Tag = "0";
	rotationToolSelectAngle1->Location = System::Drawing::Point(174, 8);

	rotationToolSelectAngle2->Show(); // 10°
	rotationToolSelectAngle2->Tag = "10";
	rotationToolSelectAngle2->Location = System::Drawing::Point(295, 56);

	rotationToolSelectAngle3->Show(); // 90°
	rotationToolSelectAngle3->Tag = "90";
	rotationToolSelectAngle3->Location = System::Drawing::Point(347, 175);

	rotationToolSelectAngle4->Hide(); // -15°
	rotationToolSelectAngle5->Hide(); // -25°
	rotationToolSelectAngle6->Hide();
	rotationToolSelectAngle7->Hide();
	rotationToolSelectAngle8->Hide();
	rotationToolSelectAngle9->Hide();

	rotationToolView->Show();

	// Confirmation Panel Setup ____________________________________________________________
	System::String^ confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION);
	System::String^ confInfoContent = Notify::TranslateContent(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION);
	pConfirmation = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
	((ConfirmationWindow^)pConfirmation)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionCancCallback);
	((ConfirmationWindow^)pConfirmation)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ServiceForm::rotationToolSelectionSlideOkCallback);


}


System::Void ServiceForm::rotationTool_manual_Click(System::Object^ sender, System::EventArgs^ e) {
	if (current_rotation_command != rotation_commands::NO_COMMAND) return;

	manual_mode = !manual_mode;

	// Set the manual button icon
	if(manual_mode) rotationToolManualButton->BackgroundImage = MANUAL_SELECTED_IMAGE;
	else rotationToolManualButton->BackgroundImage = MANUAL_IMAGE;

	// Resets the selection
	rotationTool_select_item(nullptr);

	// Activates the proper View Background
	if (current_rotation_selected == rotation_tool_selection::TILT_SELECTED) {
		if (manual_mode)	rotationToolView->BackgroundImage = TILT_VIEW_MANUAL_IMAGE;
		else rotationToolView->BackgroundImage = TILT_VIEW_IMAGE;

	}
	else if (current_rotation_selected == rotation_tool_selection::ARM_SELECTED) {
		if (manual_mode)	rotationToolView->BackgroundImage = ARM_VIEW_MANUAL_IMAGE;
		else rotationToolView->BackgroundImage = ARM_VIEW_IMAGE;

	}
	else if (current_rotation_selected == rotation_tool_selection::BODY_SELECTED) {
		if (manual_mode)	rotationToolView->BackgroundImage = BODY_VIEW_MANUAL_IMAGE;
		else rotationToolView->BackgroundImage = BODY_VIEW_IMAGE;

	}
	else if (current_rotation_selected == rotation_tool_selection::SLIDE_SELECTED) {
		if (manual_mode)	rotationToolView->BackgroundImage = SLIDE_VIEW_MANUAL_IMAGE;
		else rotationToolView->BackgroundImage = SLIDE_VIEW_IMAGE;

	}
}


void ServiceForm::rotationToolSelectionCancCallback(void) {
	((ConfirmationWindow^)pConfirmation)->close();
	rotationTool_select_item(nullptr);
}

void ServiceForm::rotationToolSelectionArmOkCallback(void) {
	((ConfirmationWindow^)pConfirmation)->close();

	// Busy condition
	if (current_rotation_command) return;

	if (ArmMotor::serviceAutoPosition(selected_target)) {
		current_rotation_command = rotation_commands::ARM;
	}
}

void ServiceForm::rotationToolSelectionTiltOkCallback(void) {
	((ConfirmationWindow^)pConfirmation)->close();
	
	// Busy condition
	if (current_rotation_command) return;

	if (TiltMotor::serviceAutoPosition(selected_target)) {
		current_rotation_command = rotation_commands::TILT;
	}
}

void ServiceForm::rotationToolSelectionBodyOkCallback(void) {
	((ConfirmationWindow^)pConfirmation)->close();

	// Busy condition
	if (current_rotation_command) return;

	if (BodyMotor::serviceAutoPosition(selected_target)) {
		current_rotation_command = rotation_commands::BODY;
	}
}

void ServiceForm::rotationToolSelectionSlideOkCallback(void) {
	((ConfirmationWindow^)pConfirmation)->close();

	// Busy condition
	if (current_rotation_command) return;

	if (SlideMotor::serviceAutoPosition(selected_target)) {
		current_rotation_command = rotation_commands::SLIDE;
	}
}

void ServiceForm::rotationTool_select_item(System::Windows::Forms::PictureBox^ item) {
	if (current_rotation_command != rotation_commands::NO_COMMAND) return;

	// Initialize the internal static pointer
	if ((item == nullptr) ||(manual_mode)) {
		
		rotationToolSelectAngle1->Text = ""; // Not selected
		rotationToolSelectAngle2->Text = ""; // Not selected
		rotationToolSelectAngle3->Text = ""; // Not selected
		rotationToolSelectAngle4->Text = ""; // Not selected
		rotationToolSelectAngle5->Text = ""; // Not selected
		rotationToolSelectAngle6->Text = ""; // Not selected
		rotationToolSelectAngle7->Text = ""; // Not selected
		rotationToolSelectAngle8->Text = ""; // Not selected
		rotationToolSelectAngle9->Text = ""; // Not selected

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
	if (item->Text != "") return;
	item->Text = "S";	

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
		rotationToolTargetAngle->Text = ((float)ArmMotor::device->getCurrentPosition() / 100).ToString();
	}
	else if (current_rotation_selected == rotation_tool_selection::TILT_SELECTED) {
		rotationToolTargetAngle->Text = ((float)TiltMotor::device->getCurrentPosition() / 100).ToString();
	}else if (current_rotation_selected == rotation_tool_selection::BODY_SELECTED) {
		rotationToolTargetAngle->Text = ((float)BodyMotor::device->getCurrentPosition() / 10).ToString();
	}else if (current_rotation_selected == rotation_tool_selection::SLIDE_SELECTED) {
		rotationToolTargetAngle->Text = ((float)SlideMotor::device->getCurrentPosition() / 10).ToString();
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