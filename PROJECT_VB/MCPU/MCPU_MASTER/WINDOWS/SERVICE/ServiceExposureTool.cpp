#include "ServiceExposureTool.h"
#include "ExposureModule.h"
#include "Notify.h"
#include "gantry_global_status.h"
#include "PCB301.h"
#include "KeyPaddle.h"
#include "Generator.h"
#include "SystemConfig.h"



// Main Panel Definition
#define FORM_BACKGROUND Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\ServiceBackground.PNG")
#define EXPTOOL_FOCUS_LARGE System::Drawing::Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\EXPOSURE\\FocusLarge.PNG")
#define EXPTOOL_FOCUS_SMALL System::Drawing::Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\EXPOSURE\\FocusSmall.PNG")
#define EXPTOOL_GRID_OFF System::Drawing::Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\EXPOSURE\\GridOff.PNG")
#define EXPTOOL_GRID_ON System::Drawing::Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\EXPOSURE\\GridOn.PNG")
#define EXPTOOL_XRAY_ON System::Drawing::Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\EXPOSURE\\XrayOn.PNG")
#define EXPTOOL_XRAY_OFF System::Drawing::Image::FromFile(Gantry::applicationResourcePath + "ServiceForm\\TOOLS\\EXPOSURE\\XrayOff.PNG")

typedef enum {
	EXPTOOL_NO_EXPOSURE = 0,
	EXPTOOL_WAIT_BUTTON,
	EXPTOOL_EXECUTING,
	EXPTOOL_COMPLETED,
	EXPTOOL_TERMINATED
}_exptool_steps_t;

static _exptool_steps_t exposureStep = EXPTOOL_NO_EXPOSURE;

void ServiceExposureTool::formInitialization(void) {

	// Initialize the position of the form
	this->Left = Gantry::monitor_X0;
	this->Top = Gantry::monitor_Y0;

	// Common Items
	BackgroundImage = FORM_BACKGROUND;
	serviceMenuTitle->Text = "";
	labelInstallation->Text = SystemConfig::Configuration->getParam(SystemConfig::PARAM_INSTALLATION_NAME)[SystemConfig::PARAM_INSTALLATION_NAME_TOP];
	serviceCanc->BackColor = Color::Transparent;
	
	exposureToolPanel->SetBounds(0, 0, PANEL_WIDTH, PANEL_HIGH);
	exposureToolPanel->BackColor = Color::Transparent;
	exposureToolPanel->Location = System::Drawing::Point(PANEL_X, PANEL_Y);
	exposureToolPanel->Show();
	enableXray->BackColor = Color::Transparent;
	
	serviceTimer = gcnew System::Timers::Timer(100);
	serviceTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &ServiceExposureTool::onServiceTimeout);
	serviceTimer->Stop();
	this->Hide();
	open_status = false;
}

void ServiceExposureTool::initPanel(void) {
	System::DateTime date;
	date = System::DateTime::Now;

	serviceMenuTitle->Text = Notify::TranslateLabel(Notify::messages::LABEL_CALIBRATION_XRAY_TUBE_TITLE);

	kVSelection->Text = "-";
	mAsSelection->Text = "-";
	focusSelection->Text = "LARGE";
	gridSelection->Text = "NO GRID";
	filterSelection->Text = "Rh";
	enableXray->BackgroundImage = EXPTOOL_XRAY_OFF;
	ExposureToolXrayEna = false;
	exposureStep = EXPTOOL_NO_EXPOSURE;
	exposureLog->Clear();
	exposureLog->Show();
	PCB301::set_xray_ena(false);

	// Start the startup session	
	serviceTimer->Start();	
	
}

void ServiceExposureTool::dialog_open(Form^ p) {
	if (panel->open_status) return;
	try {
		panel->parent = p;
		panel->open_status = true;
		panel->initPanel();
		this->ShowDialog(parent);
	}
	catch (...) {
		panel->open_status = false;
		LogClass::logInFile("ServiceExposureTool::open() Exception!");
	}

}

void ServiceExposureTool::close(void) {
	if (!panel->open_status) return;
	panel->open_status = false;
	panel->serviceTimer->Stop();
	this->Hide();
}


void ServiceExposureTool::timerManagement(void) {

	// Update the date time fields
	System::DateTime date;
	date = System::DateTime::Now;

	labelDate->Text = date.Day + ":" + date.Month + ":" + date.Year;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;


	ExposureModule::exposure_completed_errors xerror;
	ExposureModule::exposure_pulse^ pulse;
	float val;

	if (exposureStep == EXPTOOL_NO_EXPOSURE) return;

	switch (exposureStep) {
	case EXPTOOL_WAIT_BUTTON:
		if (PCB301::getXrayPushButtonStat()) {

			if (Generator::startExposure()) {
				exposureLog->Text += "BUTTON PRESSED: exposure started\n";
				exposureStep = EXPTOOL_EXECUTING;
			}
			else {
				exposureLog->Text += "startExposure() Failed!\nUnable to start exposure\n";
				exposureStep = EXPTOOL_TERMINATED;
			}

		}
		break;

	case EXPTOOL_EXECUTING:
		if (ExposureModule::isXrayRunning()) break;
		if (PCB301::getXrayPushButtonStat()) break;
		exposureStep = EXPTOOL_COMPLETED;
		break;

	case EXPTOOL_COMPLETED:

		xerror = ExposureModule::getExposureCompletedError();
		exposureLog->Text += "Exposure Completed:" + xerror.ToString() + "\n";
		pulse = ExposureModule::getExposedPulse(0);
		if (pulse) {
			if (pulse->isLargeFocus()) exposureLog->Text += "focus: large\n";
			else  exposureLog->Text += "focus: small\n";
			if (pulse->useGrid()) exposureLog->Text += "grid: ON\n";
			else  exposureLog->Text += "grid: OFF\n";
			exposureLog->Text += "samples:" + pulse->getSamples().ToString() + "\n";
			val = ((float)((int)(pulse->getKv() * 100))) / 100;
			exposureLog->Text += "kV:" + val.ToString();

			val = ((float)((int)(pulse->getmAs() * 100))) / 100;
			exposureLog->Text += ", mAs:" + val.ToString();

			val = ((float)((int)(pulse->getmA() * 100))) / 100;
			exposureLog->Text += ", mA:" + val.ToString();

			val = ((float)((int)(pulse->getms() * 100))) / 100;
			exposureLog->Text += ", ms:" + val.ToString();
		}

		exposureStep = EXPTOOL_TERMINATED;
		break;

	case EXPTOOL_TERMINATED:
		
		exposureStep = EXPTOOL_NO_EXPOSURE;
		ExposureToolXrayEna = false;
		enableXray->BackgroundImage = EXPTOOL_XRAY_OFF;
		break;

	}

}

void ServiceExposureTool::WndProc(System::Windows::Forms::Message% m)
{
	switch (m.Msg) {

	case (WM_USER + 1): // on statusTimer Timeout		
		timerManagement();
		break;
	}

	Form::WndProc(m);
}


System::Void ServiceExposureTool::onServiceTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
{	
	SendMessageA(window, WM_USER + 1, 0, 0);
}



// Canc button common management
void ServiceExposureTool::cancButton_Click(System::Object^ sender, System::EventArgs^ e) {
	close();

}

void ServiceExposureTool::onKvSelectionCallback(System::String^ value) {
	float kV;

	try {
		kV = System::Convert::ToDouble(value);
	}
	catch (...) {
		kV = 0;
	}

	if (kV < 20) kVSelection->Text = "20";
	else if (kV > 49) kVSelection->Text = "49";
	else kVSelection->Text = value;


}
void ServiceExposureTool::onmAsSelectionCallback(System::String^ value) {
	float mAs;

	try {
		mAs = System::Convert::ToDouble(value);
	}
	catch (...) {
		mAs = 0;
	}

	if (mAs < 1) mAsSelection->Text = "1";
	else if (mAs > 640) mAsSelection->Text = "640";
	else mAsSelection->Text = value;


}

System::Void  ServiceExposureTool::kvSelection_Click(System::Object^ sender, System::EventArgs^ e) {

	KeyPaddleWindow^ pkeyPaddle = gcnew KeyPaddleWindow(this, "kV Selection", "25");
	pkeyPaddle->button_ok_event += gcnew KeyPaddleWindow::delegate_button_callback(this, &ServiceExposureTool::onKvSelectionCallback);
	pkeyPaddle->open();

}
System::Void  ServiceExposureTool::mAsSelection_Click(System::Object^ sender, System::EventArgs^ e) {

	KeyPaddleWindow^ pkeyPaddle = gcnew KeyPaddleWindow(this, "kV Selection", "25");
	pkeyPaddle->button_ok_event += gcnew KeyPaddleWindow::delegate_button_callback(this, &ServiceExposureTool::onmAsSelectionCallback);
	pkeyPaddle->open();

}
System::Void ServiceExposureTool::enableXray_Click(System::Object^ sender, System::EventArgs^ e) {
	ExposureToolXrayEna = !ExposureToolXrayEna;

	if (ExposureToolXrayEna) {

		int mAs;
		float kV;
		bool use_grid;
		unsigned char focus;
		PCB315::filterMaterialCodes filter;

		try {
			mAs = System::Convert::ToDouble(mAsSelection->Text);
			kV = System::Convert::ToDouble(kVSelection->Text);
			if (gridSelection->Text == "GRID") use_grid = true;
			else use_grid = false;
			if (focusSelection->Text == "LARGE") focus = ExposureModule::FOCUS_LARGE;
			else focus = ExposureModule::FOCUS_SMALL;

			if (filterSelection->Text == "Rh") filter = PCB315::filterMaterialCodes::FILTER_RH;
			else if (filterSelection->Text == "Ag") filter = PCB315::filterMaterialCodes::FILTER_AG;
			else if (filterSelection->Text == "Al") filter = PCB315::filterMaterialCodes::FILTER_AL;
			else if (filterSelection->Text == "Cu") filter = PCB315::filterMaterialCodes::FILTER_CU;
			else if (filterSelection->Text == "Mo") filter = PCB315::filterMaterialCodes::FILTER_MO;
			else filter = PCB315::filterMaterialCodes::FILTER_RH;

		}
		catch (...) {
			ExposureToolXrayEna = false;
			return;
		}

		if (!ExposureModule::setExposurePulse(0, gcnew ExposureModule::exposure_pulse(kV, mAs, filter, focus, use_grid))) {
			ExposureToolXrayEna = false;
			return;
		}

		// Set the current exposure mode properly
		ExposureModule::setExposureMode(ExposureModule::exposure_type_options::TEST_2D);

		enableXray->BackgroundImage = EXPTOOL_XRAY_ON;
		exposureLog->Clear();
		exposureLog->Text += "WAIT X-RAY PUSH BUTTON ..\n";
		exposureStep = EXPTOOL_WAIT_BUTTON;
		serviceTimer->Start();
	}
	else {
		exposureStep = EXPTOOL_NO_EXPOSURE;
		serviceTimer->Stop();
		enableXray->BackgroundImage = EXPTOOL_XRAY_OFF;
		exposureLog->Clear();
	}
}


