#include "Projections.h"
#include "Translate.h"
#include "ConfirmationWindow.h"
#include "ArmMotor.h"
#include "gantry_global_status.h"

#define FORM_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\PROJ_BACKGROUND.PNG")
#define NO_PROJ_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "Projections\\no_proj.PNG")


#define ERR_ICON_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\error_160x145.PNG")
#define INFO_ICON_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\info_64x64.PNG")
#define WRN_ICON_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\info_64x64.PNG")
#define CANC_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\close_65x65.PNG")
#define OK_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\close_65x65.PNG")

void ProjectionForm::formInitialization(void) {

	// Initialize the position of the form
	// NOTE: Set the Position of the Form as Manual to be valid
	this->Left = GlobalObjects::monitor_X0 ;
	this->Top = GlobalObjects::monitor_Y0 ;

	this->BackColor = Color::FromArgb(216, 207, 208);
	this->BackgroundImage = FORM_IMAGE;

	panelNumber = 0;

	// Title
	TitleText->Text = Translate::label("PROJECTION_SELECTION_PANEL");
	TitleText->BackColor = Color::Transparent;

	// Selection Area
	SelectionPanel->BackColor = Color::Transparent;

	// Sets the Buttons
	CancButton->BackColor = Color::Transparent;
	OkButton->BackColor = Color::Transparent;
	NextButton->BackColor = Color::Transparent;
	PrevButton->BackColor = Color::Transparent;
	

	// Confirmation Panel Setup ____________________________________________________________
	System::String^ confInfoTitle = "[" + Translate::id("PROJECTION_CONFIRMATION_INFO") + "] " + Translate::title("PROJECTION_CONFIRMATION_INFO");
	System::String^ confInfoContent = Translate::content("PROJECTION_CONFIRMATION_INFO");
	pConf = gcnew ConfirmationWindow(parent, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
	((ConfirmationWindow^)pConf)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ProjectionForm::onConfirmCanc);
	((ConfirmationWindow^)pConf)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &ProjectionForm::onConfirmOk);
	//________________________________________________________________________________________

	proj1->BackColor = Color::Transparent;
	proj2->BackColor = Color::Transparent;
	proj3->BackColor = Color::Transparent;
	proj4->BackColor = Color::Transparent;
	proj5->BackColor = Color::Transparent;
	proj6->BackColor = Color::Transparent;
	proj7->BackColor = Color::Transparent;
	proj8->BackColor = Color::Transparent;

	// Array of projection tags associated to the panels
    proj_name = gcnew cli::array<System::String^>{"", "", "", "", "", "", "", ""};


	open_status = false;
	this->Hide();
}

void ProjectionForm::open(void) {
	if (open_status) return;
	open_status = true;
	loadProjections();
	this->ShowDialog(parent);
}
void ProjectionForm::close(void) {
	if (!open_status) return;
	open_status = false;

	((ConfirmationWindow^)pConf)->close();
	this->Hide();
}


void ProjectionForm::onConfirmOk(void) {
	((ConfirmationWindow^)pConf)->close();
	button_ok_event();
	this->close();
}
void ProjectionForm::onConfirmCanc(void) {
	((ConfirmationWindow^)pConf)->close();		
}


void ProjectionForm::CancButton_Click(System::Object^ sender, System::EventArgs^ e) {
	button_canc_event();
	this->close();
}
void ProjectionForm::OkButton_Click(System::Object^ sender, System::EventArgs^ e) {
	button_ok_event();
	this->close();
}

void ProjectionForm::NextButton_Click(System::Object^ sender, System::EventArgs^ e) {
	SetCursorPos(0, 0);// Necessary to remove the mouse from the panel area (remains highlited)

	panelNumber++;	
	loadProjections();
}
void ProjectionForm::PrevButton_Click(System::Object^ sender, System::EventArgs^ e) {
	SetCursorPos(0, 0); // Necessary to remove the mouse from the panel area (remains highlited)

	panelNumber--;
	if (panelNumber < 0) panelNumber = 0;
	loadProjections();
}

void ProjectionForm::proj1_Click(System::Object^ sender, System::EventArgs^ e) {
	if (!proj1->Enabled) return;
	selectedProjection = proj_name[0];
	((ConfirmationWindow^)pConf)->setContentBackground(proj1->BackgroundImage);
	((ConfirmationWindow^)pConf)->open();
}
void ProjectionForm::proj2_Click(System::Object^ sender, System::EventArgs^ e) {
	if (!proj2->Enabled) return;
	selectedProjection = proj_name[1];
	((ConfirmationWindow^)pConf)->setContentBackground(proj2->BackgroundImage);
	((ConfirmationWindow^)pConf)->open();
}
void ProjectionForm::proj3_Click(System::Object^ sender, System::EventArgs^ e) {
	if (!proj3->Enabled) return;
	selectedProjection = proj_name[2];
	((ConfirmationWindow^)pConf)->setContentBackground(proj3->BackgroundImage);
	((ConfirmationWindow^)pConf)->open();
}
void ProjectionForm::proj4_Click(System::Object^ sender, System::EventArgs^ e) {
	if (!proj4->Enabled) return;
	selectedProjection = proj_name[3];
	((ConfirmationWindow^)pConf)->setContentBackground(proj4->BackgroundImage);
	((ConfirmationWindow^)pConf)->open();
}
void ProjectionForm::proj5_Click(System::Object^ sender, System::EventArgs^ e) {
	if (!proj5->Enabled) return;
	selectedProjection = proj_name[4];
	((ConfirmationWindow^)pConf)->setContentBackground(proj5->BackgroundImage);
	((ConfirmationWindow^)pConf)->open();
}
void ProjectionForm::proj6_Click(System::Object^ sender, System::EventArgs^ e) {
	if (!proj6->Enabled) return;
	selectedProjection = proj_name[5];
	((ConfirmationWindow^)pConf)->setContentBackground(proj6->BackgroundImage);
	((ConfirmationWindow^)pConf)->open();
}
void ProjectionForm::proj7_Click(System::Object^ sender, System::EventArgs^ e) {
	if (!proj7->Enabled) return;
	selectedProjection = proj_name[6];
	((ConfirmationWindow^)pConf)->setContentBackground(proj7->BackgroundImage);
	((ConfirmationWindow^)pConf)->open();
}
void ProjectionForm::proj8_Click(System::Object^ sender, System::EventArgs^ e) {
	if (!proj8->Enabled) return;
	selectedProjection = proj_name[7];
	((ConfirmationWindow^)pConf)->setContentBackground(proj8->BackgroundImage);
	((ConfirmationWindow^)pConf)->open();
}

void ProjectionForm::loadProjections(void) {
	cli::array<Panel^>^  panels = gcnew array<Panel^> { proj1 , proj2, proj3, proj4, proj5 , proj6, proj7 , proj8};

	// Number of panels
	int pn = (ArmMotor::getProjectionsList()->Value->Count() - 1) / 8;
	if (panelNumber > pn) panelNumber = pn;


	for (int i = 0, j = panelNumber * 8; i < 8; i++, j++) {
		if (j < ArmMotor::getProjectionsList()->Value->Count()) {
			System::String^ tag = ArmMotor::getProjectionsList()->Value->getItem(j);
			panels[i]->BackgroundImage = ArmMotor::getProjectionsList()->ProjectionsIcons[tag];
			panels[i]->Enabled = true;
			proj_name[i] = tag;
		}
		else {
			panels[i]->BackgroundImage = ArmMotor::getProjectionsList()->ProjectionsIcons["UNDEF"];
			panels[i]->Enabled = false;
			proj_name[i] = "";
		}
	}

}