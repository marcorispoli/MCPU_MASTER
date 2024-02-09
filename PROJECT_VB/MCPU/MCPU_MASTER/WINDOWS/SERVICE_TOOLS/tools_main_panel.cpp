#include "ServiceForm.h"
#include "Notify.h"
#include "../gantry_global_status.h"

#define TOOLS_MENU_BACKGROUND System::Drawing::Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\CALIBRATION\\CalibrationBackground.PNG")

void ServiceForm::createToolsPanel(void) {

	
	// Creates the sub panels
	createRotationToolsPanel();

	return;
}
void ServiceForm::cancToolsPanel(void) {
	setActivePanel(panels::MAIN_SERVICE_PANEL);
	return;
}

void ServiceForm::initToolsPanel(void) {
	//serviceMenuTitle->Text = Notify::TranslateLabel(Notify::messages::LABEL_CALIBRATION_PANEL_TITLE);
	return;
}
