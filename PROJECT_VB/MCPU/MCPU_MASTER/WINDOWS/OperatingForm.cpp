#include "OperatingForm.h"
#include "Notify.h"
#include "ErrorForm.h"
#include "gantry_global_status.h"
#include "Projections.h"
#include "ConfirmationWindow.h"
#include "ArmMotor.h"
#include "SlideMotor.h"
#include "ExposureModule.h"
#include "../DEVICES/PCB315.h"
#include "../DEVICES/PCB303.h"
#include "../DEVICES/PCB302.h"
#include "../DEVICES/PCB301.h"
#include "../DEVICES/PCB304.h"
#include "../DEVICES/PCB326.h"

#define PROJ_NOT_SELECTED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\NoProjectionSelected.PNG")
#define PROJ_SELECTED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ProjectionSelected.PNG")

// Main Panel Definition
#define XRAY_ON_IMAGE  Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\XOn.PNG")
#define XRAY_STDBY_IMAGE  Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\XStdby.PNG")
#define XRAY_READY_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\XReady.PNG")
#define DEMO_IMAGE   Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\Demo.PNG")

#define LAMP_OFF_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\LampOff.PNG")
#define LAMP_ON_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\LampOn.PNG")

#define SLIDE_0_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\Slide0.PNG")
#define SLIDE_10_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\Slide10.PNG")

#define WARNING_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\WarningOn.PNG")
#define ERROR_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\AlarmOn.PNG")
#define ERROR_BUTTON_OFF Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\AlarmOff.PNG")

#define COMPRESSOR_DISABLED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\CompressionNotEnabled.PNG")
#define COMPRESSOR_ENABLE_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ActivePaddleDetected.PNG")
#define COMPRESSOR_NOT_DETECTED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\PaddleNotPresent.PNG")

#define THICKNESS_DISABLED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ThicknessDisabled.PNG")
#define THICKNESS_ENABLED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ThicknessEnabled.PNG")

#define FORCE_DISABLED_NOT_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ForceDisabledNotCompressed.PNG")
#define FORCE_DISABLED_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ForceDisabledCompressed.PNG")
#define FORCE_ENABLED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ForceEnabledCompressed.PNG")

#define COLLIMATION_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\CollimationMode.PNG")

#define TUBE_TEMP_OK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\TubeOk.PNG")
#define TUBE_TEMP_NOK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\TubeNok.PNG")

#define DOOR_CLOSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\DoorClosed.PNG")
#define DOOR_OPEN_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\DoorOpen.PNG")

#define MAGFACTOR_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\MagFactor.PNG")

#define COMPRESSION_RELEASE_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\CompressionReleaseOn.PNG")
#define COMPRESSION_KEEP_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\CompressionReleaseOff.PNG")

#define RESIDUAL_EXPOSURE_OK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ExposureLeftOk.PNG")
#define RESIDUAL_EXPOSURE_NOK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ExposureLeftNok.PNG")

#define PROJ_INFO_ICON  Image::FromFile(Gantry::applicationResourcePath + "Icons\\info_64x64.PNG")



namespace OPERSTATUS {

	typedef enum {		
		XRAY_STATUS_NOT_READY = 1,
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

	typedef enum {
		SLIDE_0 = 0,
		SLIDE_10 = 10
	}slide_t;


	typedef struct {

		unsigned char	anode;
		unsigned char	stator;
		unsigned char	bulb;
		bool			tube_alarm;
		bool			light_on;
	}collimatorStatus;

	typedef struct {
		compressor_mode_t	compressor_mode;
		thickness_mode_t	thickness_mode;
		force_mode_t		force_mode;
	}paddleStatus;

	

	typedef struct {
		int exposure_mode;
		bool exposure_data_valid;

	}exposureStatus;

	static struct {
		paddleStatus paddle;
		collimatorStatus collimator;
		xray_status_t xray_status;

		PCB301::door_options door_status;		
		unsigned char currentPanel;
		bool alarm;
		bool warning;

		unsigned char mag_factor;
		bool compressor_release;
		bool projectionSelected;
		slide_t slide_status;

	}Registers;
};

void OperatingForm::suspend(void) {
	this->magnifierStatus->SuspendLayout();
	this->mainPanel->SuspendLayout();
	this->xrayStat->SuspendLayout();
	this->projSelection->SuspendLayout();
	this->forceStatus->SuspendLayout();
	this->thicknessStatus->SuspendLayout();
	this->paddleStatus->SuspendLayout();
	this->tubeStatus->SuspendLayout();
	this->slideButton->SuspendLayout();
	this->collimationStatus->SuspendLayout();
	this->SuspendLayout();
}
void OperatingForm::resume(void) {
	this->magnifierStatus->ResumeLayout(false);
	this->mainPanel->ResumeLayout(false);
	this->mainPanel->PerformLayout();
	this->xrayStat->ResumeLayout(false);
	this->projSelection->ResumeLayout(false);
	this->forceStatus->ResumeLayout(false);
	this->thicknessStatus->ResumeLayout(false);
	this->paddleStatus->ResumeLayout(false);
	this->tubeStatus->ResumeLayout(false);
	this->slideButton->ResumeLayout(false);
	this->collimationStatus->ResumeLayout(false);
	this->ResumeLayout(false);
}

void OperatingForm::formInitialization(void) {

	// Initialize the position of the form
	this->Left = Gantry::monitor_X0;
	this->Top = Gantry::monitor_Y0;

	mainPanel->SetBounds(0, 0, 600, 1024);
	
	// Error Panel Setup ____________________________________________________________
	pError = gcnew ErrorForm(this);
	//________________________________________________________________________________________

	// Projections Panel Setup ____________________________________________________________
	pProj = gcnew ProjectionForm(this);
	((ProjectionForm^)pProj)->button_canc_event += gcnew ProjectionForm::delegate_button_callback(this, &OperatingForm::onConfirmCanc);
	((ProjectionForm^)pProj)->button_ok_event += gcnew ProjectionForm::delegate_button_callback(this, &OperatingForm::onConfirmOk);
	//________________________________________________________________________________________

	// Abort Projection Panel Setup ____________________________________________________________
	System::String^ abortTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_PROJECTION_ABORT) + "] " + Notify::TranslateTitle(Notify::messages::INFO_PROJECTION_ABORT);
	System::String^ abortContent = Notify::TranslateContent(Notify::messages::INFO_PROJECTION_ABORT);
	pAbort = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, abortTitle, abortContent);
	((ConfirmationWindow^)pAbort)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &OperatingForm::onAbortConfirmCanc);
	((ConfirmationWindow^)pAbort)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &OperatingForm::onAbortConfirmOk);
	//________________________________________________________________________________________
	
	// Confirmation Panel Setup ____________________________________________________________
	System::String^ confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_CARM_SHIFT_CONFIRMATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_CARM_SHIFT_CONFIRMATION);
	System::String^ confInfoContent = Notify::TranslateContent(Notify::messages::INFO_CARM_SHIFT_CONFIRMATION);
	pShiftConf = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
	((ConfirmationWindow^)pShiftConf)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &OperatingForm::onShiftConfirmCanc);
	((ConfirmationWindow^)pShiftConf)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &OperatingForm::onShiftConfirmOk);
	
	//________________________________________________________________________________________


	OPERSTATUS::Registers.alarm = false;
	OPERSTATUS::Registers.warning = false;

	// Sets the Projection selection button
	projSelection->BackgroundImage = PROJ_NOT_SELECTED_IMAGE;
	selectedIcon->Hide();
	OPERSTATUS::Registers.projectionSelected = false;
	
	
	// Sets the Xray status
	OPERSTATUS::Registers.xray_status = OPERSTATUS::XRAY_STATUS_NOT_READY;	
	xrayStat->BackColor = this->BackColor;
	xrayStat->BackgroundImage = XRAY_STDBY_IMAGE;
	
	labelXrayStatus->BackColor = Color::Transparent;
	labelXrayStatus->Text = Notify::TranslateLabel(Notify::messages::LABEL_NOT_READY_FOR_EXPOSURE);
	

	demoIcon->BackColor = Color::Transparent;
	demoIcon->BackgroundImage = DEMO_IMAGE;
	

	// Sets the current lamp status
	OPERSTATUS::Registers.collimator.light_on= false;
	lampButton->BackgroundImage = LAMP_OFF_IMAGE;

	// Error Button
	OPERSTATUS::Registers.alarm = false;
	alarmButton->BackgroundImage = ERROR_BUTTON_OFF;


	// Compressor Disabled
	OPERSTATUS::Registers.paddle.compressor_mode = OPERSTATUS::COMPRESSOR_DISABLED;
	paddleStatus->BackgroundImage = COMPRESSOR_NOT_DETECTED_IMAGE;

	// Thickness Disabled
	OPERSTATUS::Registers.paddle.thickness_mode = OPERSTATUS::THICKNESS_DISABLED;
	thicknessStatus->BackgroundImage = THICKNESS_DISABLED_IMAGE;

	OPERSTATUS::Registers.paddle.force_mode = OPERSTATUS::FORCE_DISABLED_COMPRESSED;
	forceStatus->BackgroundImage = FORCE_DISABLED_COMPRESSED_IMAGE;


	// Collimator Status option
	collimationStatus->BackgroundImage = COLLIMATION_IMAGE;

	// Tube Status
	OPERSTATUS::Registers.collimator.bulb = 0;
	OPERSTATUS::Registers.collimator.stator = 0;
	OPERSTATUS::Registers.collimator.anode = 0;
	OPERSTATUS::Registers.collimator.tube_alarm = false;
	tubeStatus->BackgroundImage = TUBE_TEMP_OK_IMAGE;

	// Door Status 
	OPERSTATUS::Registers.door_status = PCB301::door_options::OPEN_DOOR;
	doorStatus->BackgroundImage = DOOR_OPEN_IMAGE;

	// Magnification factor
	OPERSTATUS::Registers.mag_factor = 10;
	magnifierStatus->BackgroundImage = MAGFACTOR_IMAGE;

	// Decompression status
	OPERSTATUS::Registers.compressor_release = false;
	decompressionStatus->BackgroundImage = COMPRESSION_KEEP_IMAGE;

	// Slide status
	OPERSTATUS::Registers.slide_status = OPERSTATUS::SLIDE_0;
	slideButton->BackgroundImage = SLIDE_0_IMAGE;

	operatingTimer = gcnew System::Timers::Timer(100);
	operatingTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &OperatingForm::onOperatingTimeout);
	operatingTimer->Stop();

	
	this->Hide();
	open_status = false;


}

void OperatingForm::initOperatingStatus(void) {
	suspend();

	// Initialize Date and time
	System::DateTime date;
	date = System::DateTime::Now;
	labelDate->Text = date.Day + ":" + date.Month + ":" + date.Year;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	// Initialize the Patient name
	labelPatientName->Text = Gantry::getPatientName();

	// Init Arm Angle 
	float arm_position = ((float)ArmMotor::device->getCurrentPosition() / 100);
	angleText->Text = "[" + arm_position.ToString() + "°]";

	// Invalidate the projections and clears the list
	ArmMotor::abortTarget();
	ArmMotor::getProjectionsList()->clrList();

	// Sets the current collimation to OPEN mode
	PCB303::setAutoCollimationMode();

	// Sets the current Filter selector to manual mode
	PCB315::setFilterAutoMode(PCB315::filterMaterialCodes::FILTER_DEFAULT);

	// Start the startup session	
	operatingTimer->Start();
	resume();
	
}

void OperatingForm::open(void) {
	SendNotifyMessageA(window, WINMSG_OPEN, 0, 0); // OPEN EVENT MESSAGE
}

void OperatingForm::close(void) {
	SendNotifyMessageA(window, WINMSG_CLOSE, 0, 0); // CLOSE EVENT MESSAGE
	
}



void OperatingForm::onArmTargetChangedCallback(int id, int target){

	if (ArmMotor::device->isValidTarget()) {		
		selectedIcon->BackgroundImage = ArmMotor::getProjectionsList()->getCurrentProjectionIcon();
		selectedIcon->Show();
		OPERSTATUS::Registers.projectionSelected = true;
	}
	else {
		OPERSTATUS::Registers.projectionSelected = false;
		selectedIcon->Hide();
	}

}


void OperatingForm::evaluateProjectionStatus(void) {

	if (ArmMotor::device->isValidTarget()) {
		if (!OPERSTATUS::Registers.projectionSelected) {
			projSelection->BackgroundImage = PROJ_SELECTED_IMAGE;
			selectedIcon->BackgroundImage = ArmMotor::getProjectionsList()->getCurrentProjectionIcon();
			selectedIcon->Show();
			OPERSTATUS::Registers.projectionSelected = true;
		}		
	}
	else {
		if (OPERSTATUS::Registers.projectionSelected) {
			projSelection->BackgroundImage = PROJ_NOT_SELECTED_IMAGE;
			OPERSTATUS::Registers.projectionSelected = false;
			selectedIcon->Hide();
		}	
	}

	angleText->Text = (((float)ArmMotor::device->getCurrentPosition()) / 100).ToString() + "°";
}

void OperatingForm::evaluateXrayStatus(void) {

	if (Notify::isError() || Notify::isWarning()) {
		if (OPERSTATUS::Registers.xray_status != OPERSTATUS::XRAY_STATUS_NOT_READY){
			OPERSTATUS::Registers.xray_status = OPERSTATUS::XRAY_STATUS_NOT_READY;
			labelXrayStatus->Text = Notify::TranslateLabel(Notify::messages::LABEL_NOT_READY_FOR_EXPOSURE);
			xrayStat->BackgroundImage = XRAY_STDBY_IMAGE;
		}
	}
	else {
		if (OPERSTATUS::Registers.xray_status != OPERSTATUS::XRAY_STATUS_READY){
			OPERSTATUS::Registers.xray_status = OPERSTATUS::XRAY_STATUS_READY;
			labelXrayStatus->Text = Notify::TranslateLabel(Notify::messages::LABEL_READY_FOR_EXPOSURE);
			xrayStat->BackgroundImage = XRAY_READY_IMAGE;
		}
	}

}
void OperatingForm::evaluateErrorStatus(void) {
	static bool cmp_force_error = false;

	// Compression Mode Warning
	if ((ExposureModule::getCompressorMode() != ExposureModule::compression_mode_option::CMP_DISABLE) && (PCB302::getForce() == 0))
		Notify::activate(Notify::messages::WARNING_MISSING_COMPRESSION, false);
	else Notify::deactivate(Notify::messages::WARNING_MISSING_COMPRESSION);


	// Patient Protection Mode Warning
	bool patient_protection = false;
	if ((PCB304::isPatientProtection()) && (!PCB304::isPatientProtectionShifted())) patient_protection = true;
	else if(PCB315::getComponent() == PCB315::component_options::PROTECTION_2D) patient_protection = true;

	if ((ExposureModule::getProtectionMode() != ExposureModule::patient_protection_option::PROTECTION_DIS) && (patient_protection))
		Notify::activate(Notify::messages::WARNING_MISSING_PATIENT_PROTECTION, false);
	else 
		Notify::deactivate(Notify::messages::WARNING_MISSING_PATIENT_PROTECTION);
	
	// C-Arm Mode
	if (
		(ExposureModule::getArmMode() != ExposureModule::arm_mode_option::ARM_DIS) &&
		(!ArmMotor::device->isValidPosition())
		) Notify::activate(Notify::messages::WARNING_ARM_POSITION_WARNING, false);
	else Notify::deactivate(Notify::messages::WARNING_ARM_POSITION_WARNING);

	// Paddle identification
	if ((ExposureModule::getCompressorMode() != ExposureModule::compression_mode_option::CMP_DISABLE) &&
		(PCB302::getDetectedPaddleCode() == PCB302::paddleCodes::PADDLE_NOT_DETECTED)) Notify::activate(Notify::messages::WARNING_WRONG_PADDLE, false);
	else Notify::deactivate(Notify::messages::WARNING_WRONG_PADDLE);

	// Exposure Mode selection	
	if (ExposureModule::getExposureMode() == ExposureModule::exposure_type_options::EXP_NOT_DEFINED) Notify::activate(Notify::messages::WARNING_MISSING_EXPOSURE_MODE, false);
	else Notify::deactivate(Notify::messages::WARNING_MISSING_EXPOSURE_MODE);

	// Valid Exposure Data present
	if (!ExposureModule::getExposurePulse(0)->isValid()) Notify::activate(Notify::messages::WARNING_MISSING_EXPOSURE_DATA, false);
	else Notify::deactivate(Notify::messages::WARNING_MISSING_EXPOSURE_DATA);

	// Xray Push Button Enable
	if (!PCB301::getXrayEventEna()) Notify::activate(Notify::messages::WARNING_XRAY_BUTTON_DISABLED, false);
	else Notify::deactivate(Notify::messages::WARNING_XRAY_BUTTON_DISABLED);

	// Error Button
	if (Notify::isError()) {
		OPERSTATUS::Registers.warning = false;
		if (!OPERSTATUS::Registers.alarm) {
			alarmButton->BackgroundImage = ERROR_BUTTON_ON;
			OPERSTATUS::Registers.alarm = true;
		}
	}else if (Notify::isWarning()) {
		OPERSTATUS::Registers.alarm = false;
		if (!OPERSTATUS::Registers.warning) {
			alarmButton->BackgroundImage = WARNING_BUTTON_ON;
			OPERSTATUS::Registers.warning = true;
		}
	}else {
		if ((OPERSTATUS::Registers.warning) || (OPERSTATUS::Registers.alarm)) {
			OPERSTATUS::Registers.warning = false;
			OPERSTATUS::Registers.alarm = false;
			alarmButton->BackgroundImage = ERROR_BUTTON_OFF;
		}

	}

	// One Shot events
	if(Notify::isOneShot()) ((ErrorForm^)pError)->open();

	
}
void OperatingForm::evaluateCompressorStatus(void) {

	
	if (PCB302::getDetectedPaddleCode() == PCB302::paddleCodes::PADDLE_NOT_DETECTED)
	{
		// Compressor Not Detected
		if (OPERSTATUS::Registers.paddle.compressor_mode != OPERSTATUS::COMPRESSOR_NOT_DETECTED) {
			OPERSTATUS::Registers.paddle.compressor_mode = OPERSTATUS::COMPRESSOR_NOT_DETECTED;
			paddleStatus->BackgroundImage = COMPRESSOR_NOT_DETECTED_IMAGE;
		}		

		// Thickness Disabled
		if (OPERSTATUS::Registers.paddle.thickness_mode != OPERSTATUS::THICKNESS_DISABLED) {
			OPERSTATUS::Registers.paddle.thickness_mode = OPERSTATUS::THICKNESS_DISABLED;
			thicknessStatus->BackgroundImage = THICKNESS_DISABLED_IMAGE;
		}
			
		// Force Disabled Not Compressed
		if (OPERSTATUS::Registers.paddle.force_mode != OPERSTATUS::FORCE_DISABLED_NOT_COMPRESSED) {
			OPERSTATUS::Registers.paddle.force_mode = OPERSTATUS::FORCE_DISABLED_NOT_COMPRESSED;
			forceStatus->BackgroundImage = FORCE_DISABLED_NOT_COMPRESSED_IMAGE;
		}
			
		// The Lables shall not be showed because no data can be calculated
		labelPaddle->Hide();
		labelThickness->Hide();
		labelForce->Hide();
	}
	else {

		// Paddle Status
		if (OPERSTATUS::Registers.paddle.compressor_mode != OPERSTATUS::COMPRESSOR_DETECTED) {
			OPERSTATUS::Registers.paddle.compressor_mode = OPERSTATUS::COMPRESSOR_DETECTED;
			paddleStatus->BackgroundImage = COMPRESSOR_ENABLE_IMAGE;
		}
			
		// Thickness Enabled
		if (OPERSTATUS::Registers.paddle.thickness_mode != OPERSTATUS::THICKNESS_ENABLED) {
			OPERSTATUS::Registers.paddle.thickness_mode = OPERSTATUS::THICKNESS_ENABLED;
			thicknessStatus->BackgroundImage = THICKNESS_ENABLED_IMAGE;
		}
			
		// Force Active
		if (OPERSTATUS::Registers.paddle.force_mode != OPERSTATUS::FORCE_ENABLED) {
			OPERSTATUS::Registers.paddle.force_mode = OPERSTATUS::FORCE_ENABLED;
			forceStatus->BackgroundImage = FORCE_ENABLED_IMAGE;
		}

		// The Lables shall not be showed because no data can be calculated
		labelPaddle->Show();
		labelThickness->Show();
		labelForce->Show();
			
	}
	


}

void OperatingForm::evaluateCompressorReleaseStatus(void) {

	
	if (ExposureModule::getCompressorMode() == ExposureModule::compression_mode_option::CMP_DISABLE) {
		
		if (OPERSTATUS::Registers.compressor_release) {
			OPERSTATUS::Registers.compressor_release = false;
			decompressionStatus->BackgroundImage = COMPRESSION_KEEP_IMAGE;
		}

		
	}
	else {

		if (!OPERSTATUS::Registers.compressor_release) {
			OPERSTATUS::Registers.compressor_release = true;
			decompressionStatus->BackgroundImage = COMPRESSION_RELEASE_IMAGE;
		}

	}


}
void OperatingForm::evaluateCollimatorStatus(void) {
	

	// Sets the current lamp status
	if (PCB315::getPowerLightStatus() != OPERSTATUS::Registers.collimator.light_on) {
		OPERSTATUS::Registers.collimator.light_on = PCB315::getPowerLightStatus();
		if(OPERSTATUS::Registers.collimator.light_on) lampButton->BackgroundImage = LAMP_ON_IMAGE;
		else lampButton->BackgroundImage = LAMP_OFF_IMAGE;
	}


	// Evaluates the maximum tube temperature
	int val = PCB315::getAnode();
	if (PCB315::getBulb() > val) val = PCB315::getBulb();
	if (PCB315::getStator() > val) val = PCB315::getStator();
	

	// Cumulated max energy of the X-RAY tube
	labelTubeData->Text = val.ToString() + " %";

	if (PCB315::isTubeAlarm() != OPERSTATUS::Registers.collimator.tube_alarm) {
		OPERSTATUS::Registers.collimator.tube_alarm = PCB315::isTubeAlarm();
		if (OPERSTATUS::Registers.collimator.tube_alarm) tubeStatus->BackgroundImage = TUBE_TEMP_NOK_IMAGE;
		else tubeStatus->BackgroundImage = TUBE_TEMP_OK_IMAGE;
	}
}

void OperatingForm::evaluateMagStatus(void) {

	labelMag->Text = PCB304::getMagnifierfactorString();

}

void OperatingForm::evaluateDoorStatus(void) {

	if (PCB301::getDoorStatus() != OPERSTATUS::Registers.door_status) {
		OPERSTATUS::Registers.door_status = PCB301::getDoorStatus();

		if (OPERSTATUS::Registers.door_status == PCB301::door_options::CLOSED_DOOR) {
			doorStatus->BackgroundImage = DOOR_CLOSED_IMAGE;
		}
		else {
			doorStatus->BackgroundImage = DOOR_OPEN_IMAGE;
		}
	}
	
}

void OperatingForm::evaluateSlideStatus(void) {

	
}

void OperatingForm::operatingStatusManagement(void) {
	suspend();
	System::DateTime date;
	date = System::DateTime::Now;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	evaluateXrayStatus();
	evaluateErrorStatus();
	evaluateCompressorStatus();
	evaluateCompressorReleaseStatus();
	evaluateCollimatorStatus();
	evaluateMagStatus();
	evaluateDoorStatus();
	evaluateSlideStatus();
	evaluateProjectionStatus();
	resume();
	
}

void OperatingForm::WndProc(System::Windows::Forms::Message% m)
{
	switch (m.Msg) {

	case (WINMSG_TIMER): // on Timer Event
		operatingStatusManagement();
		break;

	case (WINMSG_OPEN): // on Open Event
		if (open_status) break;
		open_status = true;
		initOperatingStatus();
		this->Show();
		break;

	case (WINMSG_CLOSE): // on Open Event
		if (!open_status) break; 
		open_status = false;
		operatingTimer->Stop();
		((ProjectionForm^)pProj)->close();
		((ErrorForm^)pError)->close();
		((ConfirmationWindow^)pAbort)->close();
		this->Hide();
		break;		
	}


	Form::WndProc(m);
}

void OperatingForm::errorButton_Click(System::Object^ sender, System::EventArgs^ e) {
	if(Notify::isError() || Notify::isWarning())	((ErrorForm^)pError)->open();
}

void OperatingForm::viewSelection_Click(System::Object^ sender, System::EventArgs^ e) {
	if (!OPERSTATUS::Registers.projectionSelected)	((ProjectionForm^)pProj)->open();
	else {
		((ConfirmationWindow^)pAbort)->setContentBackground(selectedIcon->BackgroundImage);
		((ConfirmationWindow^)pAbort)->open();
	}
}
void OperatingForm::ShiftSelection_Click(System::Object^ sender, System::EventArgs^ e) {
	((ConfirmationWindow^)pShiftConf)->setContentBackground(slideButton->BackgroundImage);
	((ConfirmationWindow^)pShiftConf)->open();
}



void OperatingForm::lampButton_Click(System::Object^ sender, System::EventArgs^ e) {
	PCB315::setMirrorMode(true);
	
}

void OperatingForm::onConfirmOk(void) {
	((ProjectionForm^)pProj)->close();
	ArmMotor::projectionRequest(((ProjectionForm^)pProj)->selectedProjection);
	
}
void OperatingForm::onConfirmCanc(void) {
	((ProjectionForm^)pProj)->close();
}

void OperatingForm::onShiftConfirmOk(void) {
	((ConfirmationWindow^)pShiftConf)->close();

	if (OPERSTATUS::Registers.slide_status == OPERSTATUS::SLIDE_0) {
		OPERSTATUS::Registers.slide_status = OPERSTATUS::SLIDE_10;
		slideButton->BackgroundImage = SLIDE_10_IMAGE;
		//SlideMotor::activateAutomaticPositioning()
	}
	else {
		OPERSTATUS::Registers.slide_status = OPERSTATUS::SLIDE_0;
		slideButton->BackgroundImage = SLIDE_0_IMAGE;
		//SlideMotor::activateAutomaticPositioning()
	}
	

}
void OperatingForm::onShiftConfirmCanc(void) {
	((ConfirmationWindow^)pShiftConf)->close();
}

void OperatingForm::onAbortConfirmOk(void) {
	((ConfirmationWindow^)pAbort)->close();
	ArmMotor::abortProjectionRequest();

}
void OperatingForm::onAbortConfirmCanc(void) {
	((ConfirmationWindow^)pAbort)->close();
}
