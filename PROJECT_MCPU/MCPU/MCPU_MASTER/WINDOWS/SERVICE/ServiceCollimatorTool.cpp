#include "ServiceCollimatorTool.h"
#include "ExposureModule.h"
#include "Notify.h"
#include "gantry_global_status.h"
#include "PCB301.h"
#include "PCB303.h"
#include "PCB302.h"
#include "KeyPaddle.h"
#include "Generator.h"
#include "ConfigurationFiles.h"



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

void ServiceCollimatorTool::formInitialization(void) {

	// Initialize the position of the form
	this->Left = Gantry::monitor_X0;
	this->Top = Gantry::monitor_Y0;

	// Common Items
	BackgroundImage = FORM_BACKGROUND;
	serviceMenuTitle->Text = "";
	labelInstallation->Text = SystemConfig::Configuration->getParam(SystemConfig::PARAM_INSTALLATION_NAME)[SystemConfig::PARAM_INSTALLATION_NAME_TOP];
	serviceCanc->BackColor = Color::Transparent;
	
	mainForm->SetBounds(0, 0, PANEL_WIDTH, PANEL_HIGH);
	mainForm->BackColor = Color::Transparent;
	mainForm->Location = System::Drawing::Point(PANEL_X, PANEL_Y);
	mainForm->Show();
	
	
	serviceTimer = gcnew System::Timers::Timer(100);
	serviceTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &ServiceCollimatorTool::onServiceTimeout);
	serviceTimer->Stop();
	this->Hide();
	open_status = false;

	
}

void ServiceCollimatorTool::initPanel(void) {
	System::DateTime date;
	date = System::DateTime::Now;

	// Panel Name
	serviceMenuTitle->Text = Notify::TranslateLabel(Notify::messages::LABEL_CALIBRATION_XRAY_TUBE_TITLE);
	
	// Sets the disable mode at the beginning
	PCB303::setFormatCollimationMode(PCB303::collimationModeEnum::DISABLED, 0);	

	// Format Operating Mode Box Creation
	FormatOperatingModeBox->Items->Clear();
	for (int i = 0; i < (int) PCB303::collimationModeEnum::ITEMS; i++) {		
		FormatOperatingModeBox->Items->Add(((PCB303::collimationModeEnum)i).ToString());
	}

	FormatOperatingModeBox->SelectedIndex = (int)PCB303::getFormatCollimationMode();

	
	// Collimation Box creation
	FormatSelectionBox->Items->Clear();
	//AssignedPaddleBox->Items->Clear();
	
	System::String^ item;

	for (int i = 0; i < CollimatorConfig::TOTAL_BLADE_FORMAT_NUMBERS; i++) {
		

		if (i == 0) {
			item = "F0->OPEN";			
		}else if (i == CollimatorConfig::TOTAL_BLADE_FORMAT_NUMBERS-1) {
			item = "F" + i.ToString()+"->CUSTOM";
		}
		else {
			item = "F" + i.ToString();
		}		
		FormatSelectionBox->Items->Add(item);
	//	PCB302::paddleCodes code = PCB302::getPaddleFromColliIndex(i);
	//	if (code == PCB302::paddleCodes::PADDLE_NOT_DETECTED) AssignedPaddleBox->Items->Add("-");
	//	else AssignedPaddleBox->Items->Add(code.ToString());
	}
	FormatSelectionBox->SelectedIndex = 0;


	// Paddle Box Creation
	AssignedPaddleBox->Items->Clear();
	for (int i = 0; i <= (int) PCB302::paddleCodes::PADDLE_LEN; i++) {
		
		if (i == (int)PCB302::paddleCodes::PADDLE_NOT_DETECTED) item = "-";
		else item = ((PCB302::paddleCodes) i).ToString();		
		AssignedPaddleBox->Items->Add(item);
	}

	// Sets the initial selection of both fields:	
	AssignedPaddleBox->SelectedIndex = (int) PCB302::paddleCodes::PADDLE_NOT_DETECTED;
	//AssignedPaddleBox->Text = "-";

	// Log Initialization
	collimatorDeviceLog->Clear();
	collimatorDeviceLog->Show();
	
	// Start the startup session	
	serviceTimer->Start();	
	
}

void ServiceCollimatorTool::dialog_open(Form^ p) {
	if (panel->open_status) return;
	try {
		panel->parent = p;
		panel->open_status = true;
		panel->initPanel();
		this->ShowDialog(parent);
		window = static_cast<HWND>(Handle.ToPointer());
	}
	catch (...) {
		panel->open_status = false;
		LogClass::logInFile("ServiceCollimatorTool::open() Exception!");
	}

}

void ServiceCollimatorTool::close(void) {
	if (!panel->open_status) return;
	panel->open_status = false;
	panel->serviceTimer->Stop();
	this->Hide();
}

void ServiceCollimatorTool::FormatSelectionBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	 
	// Find the first Paddle assigned to the index
	for (int i = 0; i < (int)PCB302::paddleCodes::PADDLE_LEN; i++) {
		int index = PCB302::getPaddleCollimationFormat((PCB302::paddleCodes)i);
		if (index == FormatSelectionBox->SelectedIndex) {
			AssignedPaddleBox->SelectedIndex = i;
			PCB303::collimationModeEnum collimode = (PCB303::collimationModeEnum)FormatOperatingModeBox->SelectedIndex;
			PCB303::setFormatCollimationMode(collimode, FormatSelectionBox->SelectedIndex);
			return;
		}

	}
	AssignedPaddleBox->SelectedIndex = (int) PCB302::paddleCodes::PADDLE_NOT_DETECTED;	 
	PCB303::collimationModeEnum collimode = (PCB303::collimationModeEnum) FormatOperatingModeBox->SelectedIndex;
	PCB303::setFormatCollimationMode(collimode, FormatSelectionBox->SelectedIndex);
	
 }

void ServiceCollimatorTool::AssignedPaddleBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	
	int index = PCB302::getPaddleCollimationFormat((PCB302::paddleCodes)AssignedPaddleBox->SelectedIndex);
	if (index < 0) index = 0; // OPEN IF NOT ASSIGNED
	FormatSelectionBox->SelectedIndex = index;

	PCB303::collimationModeEnum collimode = (PCB303::collimationModeEnum)FormatOperatingModeBox->SelectedIndex;
	PCB303::setFormatCollimationMode(collimode, FormatSelectionBox->SelectedIndex);
}

void ServiceCollimatorTool::FormatOperatingModeBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	PCB303::collimationModeEnum collimode = (PCB303::collimationModeEnum) FormatOperatingModeBox->SelectedIndex;

	// Select the new operating mode, without chenging the current index 
	int idx = PCB303::getFormatIndex();
	if (idx >= CollimatorConfig::TOTAL_BLADE_FORMAT_NUMBERS) idx = -1;
	PCB303::setFormatCollimationMode(collimode, idx);
	
}

void ServiceCollimatorTool::timerManagement(void) {

	// Update the date time fields
	System::DateTime date;
	date = System::DateTime::Now;

	labelDate->Text = date.Day + ":" + date.Month + ":" + date.Year;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	// Update the current collimation data
	int idx = PCB303::getFormatIndex();
	if (idx >= CollimatorConfig::TOTAL_BLADE_FORMAT_NUMBERS) idx = -1;

	if (idx >=0 ) {
		LeftField->Text = PCB303::getConfig()->left_blade_formats[idx].ToString();
		RightField->Text = PCB303::getConfig()->right_blade_formats[idx].ToString();
		BackField->Text = PCB303::getConfig()->back_blade_formats[idx].ToString();
		FrontField->Text = PCB303::getConfig()->front_blade_formats[idx].ToString();
		TrapField->Text = PCB303::getConfig()->trap_blade_formats[idx].ToString();

		CurrentFormatField->Text = "FORMAT_" + idx.ToString();
	}
	else {
		// No valid format is actually selected
		LeftField->Text = "-";
		RightField->Text = "-";
		BackField->Text = "-";
		FrontField->Text = "-";
		TrapField->Text = "-";
		CurrentFormatField->Text = "-";
	}

	// Updates the current detected Paddle
	PCB302::paddleCodes paddle = PCB302::getDetectedPaddleCode();
	if (paddle >= PCB302::paddleCodes::PADDLE_NOT_DETECTED) {
		CurrentPaddleField->Text = "-";
	}
	else {
		CurrentPaddleField->Text = paddle.ToString();
	}

	// Evaluates the operating mode

	PCB303::collimationModeEnum collimode = PCB303::getFormatCollimationMode();
	if (collimode != PCB303::collimationModeEnum::CUSTOM) {
		ServiceFormatPanel->Hide();
	}else ServiceFormatPanel->Show();

	
}

void ServiceCollimatorTool::WndProc(System::Windows::Forms::Message% m)
{
	switch (m.Msg) {

	case (WM_USER + 1): // on statusTimer Timeout		
		timerManagement();
		break;
	}

	Form::WndProc(m);
}


System::Void ServiceCollimatorTool::onServiceTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
{	
	SendMessageA(window, WM_USER + 1, 0, 0);
}



// Canc button common management
void ServiceCollimatorTool::cancButton_Click(System::Object^ sender, System::EventArgs^ e) {
	close();

}

