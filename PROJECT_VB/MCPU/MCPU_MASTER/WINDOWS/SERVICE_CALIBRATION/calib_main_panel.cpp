#include "ServiceForm.h"
#include "Notify.h"
#include "../gantry_global_status.h"

#define CALIB_MENU_BACKGROUND System::Drawing::Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\CALIBRATION\\CalibrationBackground.PNG")

void ServiceForm::createCalibrationPanel(void) {

	// Calib panel Definition
	calibPanel->SetBounds(0, 0, PANEL_WIDTH, PANEL_HIGH);
	calibPanel->BackgroundImage = CALIB_MENU_BACKGROUND;
	calibPanel->Location = System::Drawing::Point(PANEL_X, PANEL_Y);

	// Set of the Menu Buttons
	zeroSetting->BackColor = Color::Transparent;
	exposureTool->BackColor = Color::Transparent;

	// Creates the sub panels
	createZeroSettingPanel();
	createRotationToolsPanel();
	createExposureToolPanel();

	return;
}
void ServiceForm::cancCalibrationPanel(void) {
	setActivePanel(panels::MAIN_SERVICE_PANEL);
	return;
}

void ServiceForm::initCalibrationPanel(void) {
	serviceMenuTitle->Text = Notify::TranslateLabel(Notify::messages::LABEL_CALIBRATION_PANEL_TITLE);
	return;
}
System::Void ServiceForm::zeroSetting_Click(System::Object^ sender, System::EventArgs^ e) {
	setActivePanel(panels::CALIB_ZEROSETTING_PANEL);
}
System::Void ServiceForm::exposureTool_Click(System::Object^ sender, System::EventArgs^ e) {
	setActivePanel(panels::TOOL_EXPOSURE_PANEL);
}
