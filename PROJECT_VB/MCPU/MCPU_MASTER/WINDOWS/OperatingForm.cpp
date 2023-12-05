#include "OperatingForm.h"
#include "Notify.h"
#include "ErrorForm.h"
#include "gantry_global_status.h"
#include "Projections.h"
#include "ConfirmationWindow.h"
#include "ArmMotor.h"
#include "ExposureModule.h"
#include "../DEVICES/PCB315.h"
#include "../DEVICES/PCB303.h"
#include "../DEVICES/PCB302.h"
#include "../DEVICES/PCB301.h"
#include "../DEVICES/PCB304.h"
#include "../DEVICES/PCB326.h"

#define PROJ_SELCTION_BACKGROUND_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ProjSelectionButton.PNG")

// Main Panel Definition
#define XRAY_ON_IMAGE  Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\XOn.PNG")
#define XRAY_STDBY_IMAGE  Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\XStdby.PNG")
#define XRAY_READY_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\XReady.PNG")

#define LAMP_OFF_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\LampOff.PNG")
#define LAMP_ON_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\LampOn.PNG")

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
		unsigned char residual_exposures;
		bool projectionSelected;

	}Registers;
};

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
	System::String^ confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_PROJECTION_ABORT) + "] " + Notify::TranslateTitle(Notify::messages::INFO_PROJECTION_ABORT);
	System::String^ confInfoContent = Notify::TranslateContent(Notify::messages::INFO_PROJECTION_ABORT);
	pAbort = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
	((ConfirmationWindow^)pAbort)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &OperatingForm::onAbortConfirmCanc);
	((ConfirmationWindow^)pAbort)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &OperatingForm::onAbortConfirmOk);
	//________________________________________________________________________________________
	
	OPERSTATUS::Registers.alarm = false;
	OPERSTATUS::Registers.warning = false;

	// Sets the Projection selection button
	projSelection->BackgroundImage = PROJ_SELCTION_BACKGROUND_IMAGE;
	selectedIcon->Hide();
	OPERSTATUS::Registers.projectionSelected = false;
	ArmMotor::target_change_event += gcnew ArmMotor::delegate_target_change_callback(this, &OperatingForm::onArmTargetChangedCallback);		
	ArmMotor::target_abort_event += gcnew ArmMotor::delegate_target_abort_callback(this, &OperatingForm::onArmAbortTargetCallback);
	
	
	// Sets the Xray status
	OPERSTATUS::Registers.xray_status = OPERSTATUS::XRAY_STATUS_NOT_READY;
	labelXrayStatus->Text = Notify::TranslateLabel(Notify::messages::LABEL_NOT_READY_FOR_EXPOSURE);
	xrayStat->BackgroundImage = XRAY_STDBY_IMAGE;

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

	// Residual Exposures
	OPERSTATUS::Registers.residual_exposures = 255;
	residualExposures->BackgroundImage = RESIDUAL_EXPOSURE_OK_IMAGE;

	operatingTimer = gcnew System::Timers::Timer(100);
	operatingTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &OperatingForm::onOperatingTimeout);
	operatingTimer->Stop();

	
	this->Hide();
	open_status = false;


}

void OperatingForm::initOperatingStatus(void) {

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

	// Sets the current collimation to OPEN mode
	PCB303::setAutoCollimationMode();

	// Sets the current Filter selector to manual mode
	PCB315::setFilterAutoMode(PCB315::filterMaterialCodes::FILTER_DEFAULT);

	// Start the startup session	
	operatingTimer->Start();

	
}

void OperatingForm::open(void) {
	if (open_status) return;
	open_status = true;
	initOperatingStatus();
	
	this->Show();

}

void OperatingForm::close(void) {
	if (!open_status) return;
	open_status = false;
	operatingTimer->Stop();
	((ProjectionForm^)pProj)->close();
	((ErrorForm^)pError)->close();
	((ConfirmationWindow^)pAbort)->close();
	this->Hide();
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

void OperatingForm::onArmAbortTargetCallback(void) {
	OPERSTATUS::Registers.projectionSelected = false;
	selectedIcon->Hide();
}


void OperatingForm::onArmPositionChangeCallback(void) {
	angleText->Text = (((float)ArmMotor::device->getCurrentPosition())/100).ToString();
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
	if (!ExposureModule::getExposedPulse(0)->getValidated()) Notify::activate(Notify::messages::WARNING_MISSING_EXPOSURE_DATA, false);
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

	// Paddle Section
	if (ExposureModule::getCompressorMode() == ExposureModule::compression_mode_option::CMP_DISABLE) {
		// Compressor Disabled Option active

		if (OPERSTATUS::Registers.paddle.compressor_mode != OPERSTATUS::COMPRESSOR_DISABLED) {
			OPERSTATUS::Registers.paddle.compressor_mode = OPERSTATUS::COMPRESSOR_DISABLED;
			paddleStatus->BackgroundImage = COMPRESSOR_NOT_DETECTED_IMAGE;
		}
		
		// Thickness Disabled
		if (OPERSTATUS::Registers.paddle.thickness_mode != OPERSTATUS::THICKNESS_DISABLED) {
			OPERSTATUS::Registers.paddle.thickness_mode = OPERSTATUS::THICKNESS_DISABLED;
			thicknessStatus->BackgroundImage = THICKNESS_DISABLED_IMAGE;
		}
		
		// Force Disabled Compressed
		if (OPERSTATUS::Registers.paddle.force_mode != OPERSTATUS::FORCE_DISABLED_COMPRESSED) {
			OPERSTATUS::Registers.paddle.force_mode = OPERSTATUS::FORCE_DISABLED_COMPRESSED;
			forceStatus->BackgroundImage = FORCE_DISABLED_COMPRESSED_IMAGE;
		}
		
		// Compressor relase 		
		if (OPERSTATUS::Registers.compressor_release) {
			OPERSTATUS::Registers.compressor_release = false;
			decompressionStatus->BackgroundImage = COMPRESSION_KEEP_IMAGE;
		}
		
		labelPaddle->Hide();
		labelThickness->Show();
		labelForce->Show();

	}
	else {

		// Compressor relase 
		if (ExposureModule::getCompressorMode() != ExposureModule::compression_mode_option::CMP_KEEP) {
			if (OPERSTATUS::Registers.compressor_release) {
				OPERSTATUS::Registers.compressor_release = false;
				decompressionStatus->BackgroundImage = COMPRESSION_KEEP_IMAGE;
			}
		}
		else if (ExposureModule::getCompressorMode() != ExposureModule::compression_mode_option::CMP_RELEASE) {
			if (!OPERSTATUS::Registers.compressor_release) {
				OPERSTATUS::Registers.compressor_release = true;
				decompressionStatus->BackgroundImage = COMPRESSION_RELEASE_IMAGE;
			}
		}

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

void OperatingForm::operatingStatusManagement(void) {

	System::DateTime date;
	date = System::DateTime::Now;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	evaluateXrayStatus();
	evaluateErrorStatus();
	evaluateCompressorStatus();
	evaluateCollimatorStatus();
	evaluateMagStatus();
	evaluateDoorStatus();
	onArmPositionChangeCallback();

	
}

void OperatingForm::WndProc(System::Windows::Forms::Message% m)
{
	switch (m.Msg) {

	case (WM_USER + 1): // onIdleTimeout

		operatingStatusManagement();
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

void OperatingForm::onAbortConfirmOk(void) {
	((ConfirmationWindow^)pAbort)->close();
	ArmMotor::abortProjectionRequest();

}
void OperatingForm::onAbortConfirmCanc(void) {
	((ConfirmationWindow^)pAbort)->close();
}
