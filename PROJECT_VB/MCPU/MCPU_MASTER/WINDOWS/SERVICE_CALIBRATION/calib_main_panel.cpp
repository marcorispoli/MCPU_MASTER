#include "ServiceForm.h"

void ServiceForm::initCalibrationPanel(void) {
	return;
}
System::Void ServiceForm::zeroSetting_Click(System::Object^ sender, System::EventArgs^ e) {
	setActivePanel(panels::CALIB_ZEROSETTING_PANEL);
}

System::Void ServiceForm::calibPanelExit_Click(System::Object^ sender, System::EventArgs^ e) {
	setActivePanel(panels::MAIN_SERVICE_PANEL);
}

