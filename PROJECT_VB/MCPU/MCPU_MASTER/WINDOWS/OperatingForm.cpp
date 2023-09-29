#include "pch.h"


// Window Panels 
#define MAIN_PANEL			1
#define ERROR_PANEL			2


// Main Panel Definition
#define LAMP_OFF_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\LampOff.PNG")
#define ERROR_BUTTON_ON Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\AlarmOn.PNG")
#define ERROR_BUTTON_OFF Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\AlarmOff.PNG")

#define COMPRESSOR_DISABLED_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\CompressionNotEnabled.PNG")
#define COMPRESSOR_ENABLE_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\ActivePaddleDetected.PNG")
#define COMPRESSOR_NOT_DETECTED_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\PaddleNotPresent.PNG")

#define THICKNESS_DISABLED_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\ThicknessDisabled.PNG")
#define THICKNESS_ENABLED_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\ThicknessEnabled.PNG")

#define FORCE_DISABLED_NOT_COMPRESSED_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\ForceDisabledNotCompressed.PNG")
#define FORCE_DISABLED_COMPRESSED_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\ForceDisabledCompressed.PNG")
#define FORCE_ENABLED_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\ForceEnabledCompressed.PNG")

#define COLLIMATION_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\CollimationMode.PNG")

#define TUBE_TEMP_OK_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\TubeOk.PNG")
#define TUBE_TEMP_NOK_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\TubeNok.PNG")

#define ERRORPANEL_ERRORICON Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\error_160x145.PNG")

// Error Panel Definition
#define ERROR_PANEL_CANC_BUTTON Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\close_65x65.PNG")
#define ERROR_PANEL_NEXT_BUTTON Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\arrow_80x80_r.PNG")

// Error Window timeout
#define ERROR_WINDOW_TMO	15000



namespace OPERSTATUS {
	typedef enum {
		XRAY_STATUS_ON = 1,
		XRAY_STATUS_NOT_READY,
		XRAY_STATUS_READY
	}xray_status_t;

	typedef enum {
		COMPRESSOR_DISABLED = 1,
		COMPRESSOR_NOT_DETECTED,
		COMPRESSOR_DETECTED
	}compressor_mode_t;

	typedef enum {
		THICKNESS_DISABLED = 1,
		THICKNESS_ENABLED,
	}thickness_mode_t;

	typedef enum {
		FORCE_DISABLED_NOT_COMPRESSED = 1,
		FORCE_DISABLED_COMPRESSED,
		FORCE_ENABLED,
	}force_mode_t;


	typedef struct {

		unsigned char anode;
		unsigned char stator;
		unsigned char bulb;
		bool alarm;

	}tubeStatus;

	typedef struct {
		compressor_mode_t	compressor_mode;
		thickness_mode_t	thickness_mode;
		force_mode_t		force_mode;
	}paddleStatus;

	static struct {
		paddleStatus paddle;
		tubeStatus	tube;
		bool closed_door;
		xray_status_t xray_status;
		bool colli_lamp;
		unsigned char currentPanel;
		bool alarm;
		
	}Registers;
};


void OperatingForm::formInitialization(void) {

	// Initialize the position of the form
	this->Left = GlobalObjects::monitor_X0;
	this->Top = GlobalObjects::monitor_Y0;

	OPERSTATUS::Registers.currentPanel = MAIN_PANEL;
	mainPanel->Show();
	errorPanel->Hide();

	mainPanel->SetBounds(0, 0, 600, 1024);
	//mainPanel->BackgroundImage = BACKGROUND;

	// Sets the Xray status
	OPERSTATUS::Registers.xray_status = OPERSTATUS::XRAY_STATUS_NOT_READY;
	labelXrayStatus->BackColor = Color::FromArgb(140, 140, 140);
	labelXrayStatus->Text = Translate::label("NOT-READY-FOR-EXPOSURE");

	// Sets the current lamp status
	OPERSTATUS::Registers.colli_lamp = false;
	lampButton->BackgroundImage = LAMP_OFF_IMAGE;

	// Error Button
	OPERSTATUS::Registers.alarm = Errors::isError();
	if (Errors::isError()) alarmButton->BackgroundImage = ERROR_BUTTON_ON;
	else alarmButton->BackgroundImage = ERROR_BUTTON_OFF;

	// Paddle Section
	if (GantryStatusRegisters::ExposureModeRegister::compressorMode->Value->getCode() == GantryStatusRegisters::CompressionModeOption::options::CMP_DISABLE) {
		
		// Compressor Disabled
		OPERSTATUS::Registers.paddle.compressor_mode = OPERSTATUS::COMPRESSOR_DISABLED;
		paddleStatus->BackgroundImage = COMPRESSOR_NOT_DETECTED_IMAGE;

		// Thickness Disabled
		OPERSTATUS::Registers.paddle.thickness_mode = OPERSTATUS::THICKNESS_DISABLED;
		thicknessStatus->BackgroundImage = THICKNESS_DISABLED_IMAGE;

		OPERSTATUS::Registers.paddle.force_mode = OPERSTATUS::FORCE_DISABLED_COMPRESSED;
		forceStatus->BackgroundImage = FORCE_DISABLED_COMPRESSED_IMAGE;

	}
	else {
		if (!GantryStatusRegisters::CompressorRegister::isPaddle())
		{
			// Compressor Not Detected
			OPERSTATUS::Registers.paddle.compressor_mode = OPERSTATUS::COMPRESSOR_NOT_DETECTED;
			paddleStatus->BackgroundImage = COMPRESSOR_NOT_DETECTED_IMAGE;

			// Thickness Disabled
			OPERSTATUS::Registers.paddle.thickness_mode = OPERSTATUS::THICKNESS_DISABLED;
			thicknessStatus->BackgroundImage = THICKNESS_DISABLED_IMAGE;

			OPERSTATUS::Registers.paddle.force_mode = OPERSTATUS::FORCE_DISABLED_NOT_COMPRESSED;
			forceStatus->BackgroundImage = FORCE_DISABLED_NOT_COMPRESSED_IMAGE;
		}
		else {
			OPERSTATUS::Registers.paddle.compressor_mode = OPERSTATUS::COMPRESSOR_DETECTED;
			paddleStatus->BackgroundImage = COMPRESSOR_ENABLE_IMAGE;

			// Thickness Enabled
			OPERSTATUS::Registers.paddle.thickness_mode = OPERSTATUS::THICKNESS_ENABLED;
			thicknessStatus->BackgroundImage = THICKNESS_ENABLED_IMAGE;

			OPERSTATUS::Registers.paddle.force_mode = OPERSTATUS::FORCE_ENABLED;
			forceStatus->BackgroundImage = FORCE_ENABLED_IMAGE;
		}
	}

	// Collimator Status option
	collimationStatus->BackgroundImage = COLLIMATION_IMAGE;

	// Tube Status
	OPERSTATUS::Registers.tube.bulb = GantryStatusRegisters::TubeDataRegister::getBulb();
	OPERSTATUS::Registers.tube.stator = GantryStatusRegisters::TubeDataRegister::getStator();
	OPERSTATUS::Registers.tube.anode = GantryStatusRegisters::TubeDataRegister::getAnode();

	int max = OPERSTATUS::Registers.tube.bulb;
	if (OPERSTATUS::Registers.tube.stator > max) max = OPERSTATUS::Registers.tube.stator;
	if (OPERSTATUS::Registers.tube.anode > max) max = OPERSTATUS::Registers.tube.anode;
	//labelTubeData->Text = max.ToString() + " %";

	if (max > 90)  {
		OPERSTATUS::Registers.tube.alarm = true;
		tubeStatus->BackgroundImage = TUBE_TEMP_NOK_IMAGE;
	}
	else {
		OPERSTATUS::Registers.tube.alarm = false;
		tubeStatus->BackgroundImage = TUBE_TEMP_OK_IMAGE;
	}








	// Build the Error Panel
	errorPanelTitle->Text = Translate::title("ERROR_WINDOW_PANEL");
	errorPanel->SetBounds(0, 0, 600, 1024);
	errpanel_erricon->BackgroundImage = ERRORPANEL_ERRORICON;
	errpanel_erricon->SetBounds(40, 137, 160, 145);
	errorId->Location =  Point(247, 169);
	errorTitle->Location = Point(249, 226);
	errorContent->SetBounds(40,337, 520, 550);
	buttonCanc->SetBounds(518, 10, 70, 70);
	buttonCanc->BackgroundImage = ERROR_PANEL_CANC_BUTTON;

	operatingTimer = gcnew System::Timers::Timer(100);
	operatingTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &OperatingForm::onOperatingTimeout);
	operatingTimer->Stop();

	errorPanelTimer = gcnew System::Timers::Timer(2000);
	errorPanelTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &OperatingForm::onErrorTimeout);
	errorPanelTimer->Stop();

}

void OperatingForm::initOperatingStatus(void) {

	// Initialize Date and time
	DateTime date;
	date = DateTime::Now;
	labelDate->Text = date.Day + ":" + date.Month + ":" + date.Year;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	// Initialize the Patient name
	labelPatientName->Text = GantryStatusRegisters::OperatingStatusRegister::getPatientName();

	
	this->Show();

	// Start the startup session	
	//operatingTimer->Start();	

}

void OperatingForm::operatingStatusManagement(void) {

	DateTime date;
	date = DateTime::Now;
	
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;


	if (Errors::isError()) {
		alarmButton->Show();
	}else alarmButton->Hide();

	if ((Errors::isNewError()) || (Errors::isUpdateError())) {
		openErrorWindow(true);
	}

}

void OperatingForm::WndProc(System::Windows::Forms::Message% m)
{
	switch (m.Msg) {

	case (WM_USER + 1): // onIdleTimeout
		
		operatingStatusManagement();
		break;

	case (WM_USER + 2): // onErrorTimeout
		openErrorWindow(false);
		break;
	case (WM_USER + 3):

		break;

	case (WM_USER + 4):

		break;

	case (WM_USER + 5):

		break;
	}


	Form::WndProc(m);
}

void OperatingForm::errorButton_Click(System::Object^ sender, System::EventArgs^ e) {
	
	openErrorWindow(true);
}

void OperatingForm::openErrorWindow(bool status) {
	static bool local_status = false;
	Translate::item^ msgit;
	
	// Stops the exit timer
	errorPanelTimer->Stop();

	// Closing condition
	if ((status == false) && (local_status)) {
		local_status = false;
		mainPanel->Show();
		errorPanel->Hide();		
		return;
	}

	
	// Set the new error in evidence 
	Errors::item^ newitem = Errors::getCurrent();
	if ((!Errors::isUpdateError()) && (local_status)) return;  // The panel is already open and no update on the error queue
	Errors::clrUpdate();

	local_status = true;
	errorPanelTimer->Start();

	if (newitem) {
		msgit = Translate::getItem(newitem->errmsg);

		errorTitle->Text = msgit->title;
		errorId->Text = msgit->id;

	}
	else {
		errorTitle->Text = "";
		errorId->Text = "";
	}

	// Clear the Error content box
	errorContent->Text = "";

	// Set the Content space with the list of active errors
	errorContent->Text = Errors::getListOfErrors();

	// Clears the index of the last occurred error
	Errors::clrNewError();

	// Refresh the Error queue removing the one_shot flagged errors
	Errors::clrOneShotErrors();

	
	mainPanel->Hide();
	errorPanel->Show();

}

void OperatingForm::buttonCanc_Click(System::Object^ sender, System::EventArgs^ e) {
	openErrorWindow(false);
}

