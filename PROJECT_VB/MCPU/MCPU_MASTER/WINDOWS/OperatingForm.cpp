#include "OperatingForm.h"
#include "Translate.h"
#include "ErrorForm.h"
#include "gantry_global_status.h"
#include "Projections.h"
#include "ConfirmationWindow.h"
#include "ArmMotor.h"
#include "../DEVICES/PCB315.h"

#define PROJ_SELCTION_BACKGROUND_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\ProjSelectionButton.PNG")

// Main Panel Definition
#define XRAY_ON_IMAGE  Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\XOn.PNG")
#define XRAY_STDBY_IMAGE  Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\XStdby.PNG")
#define XRAY_READY_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\XReady.PNG")

#define LAMP_OFF_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\LampOff.PNG")
#define LAMP_ON_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\LampOn.PNG")

#define WARNING_BUTTON_ON Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\WarningOn.PNG")
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

#define DOOR_CLOSED_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\DoorClosed.PNG")
#define DOOR_OPEN_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\DoorOpen.PNG")

#define MAGFACTOR_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\MagFactor.PNG")

#define COMPRESSION_RELEASE_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\CompressionReleaseOn.PNG")
#define COMPRESSION_KEEP_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\CompressionReleaseOff.PNG")

#define RESIDUAL_EXPOSURE_OK_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\ExposureLeftOk.PNG")
#define RESIDUAL_EXPOSURE_NOK_IMAGE Image::FromFile(GlobalObjects::applicationResourcePath + "OperatingForm\\ExposureLeftNok.PNG")

#define PROJ_INFO_ICON  Image::FromFile(GlobalObjects::applicationResourcePath + "Icons\\info_64x64.PNG")


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

		bool closed_door;		
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
	this->Left = GlobalObjects::monitor_X0;
	this->Top = GlobalObjects::monitor_Y0;

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
	System::String^ confInfoTitle = "[" + Translate::id("PROJECTION_ABORT_INFO") + "] " + Translate::title("PROJECTION_ABORT_INFO");
	System::String^ confInfoContent = Translate::content("PROJECTION_ABORT_INFO");
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
	labelXrayStatus->Text = Translate::label("NOT-READY-FOR-EXPOSURE");
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
	OPERSTATUS::Registers.closed_door = false;
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

	collimator_light_activation = false;
}

void OperatingForm::initOperatingStatus(void) {

	// Initialize Date and time
	System::DateTime date;
	date = System::DateTime::Now;
	labelDate->Text = date.Day + ":" + date.Month + ":" + date.Year;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	// Initialize the Patient name
	labelPatientName->Text = GantryStatusRegisters::OperatingStatusRegister::getPatientName();

	// Init Arm Angle 
	float arm_position = ((float)pMARM->getCurrentPosition() / 100);
	angleText->Text = "[" + arm_position.ToString() + "°]";

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

	if (pMARM->isValidTarget()) {
		// Sets the current selected projection
		System::String^ tag = ArmMotor::getSelectedProjection();
		if (tag == "") return;

		selectedIcon->BackgroundImage = ArmMotor::getProjectionsList()->ProjectionsIcons[tag];
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
	angleText->Text = (((float) pMARM->getCurrentPosition())/100).ToString();
}

void OperatingForm::evaluateXrayStatus(void) {

	if (Notify::isError() || Notify::isWarning()) {
		if (OPERSTATUS::Registers.xray_status != OPERSTATUS::XRAY_STATUS_NOT_READY){
			OPERSTATUS::Registers.xray_status = OPERSTATUS::XRAY_STATUS_NOT_READY;
			labelXrayStatus->Text = Translate::label("NOT-READY-FOR-EXPOSURE");
			xrayStat->BackgroundImage = XRAY_STDBY_IMAGE;
		}
	}
	else {
		if (OPERSTATUS::Registers.xray_status != OPERSTATUS::XRAY_STATUS_READY){
			OPERSTATUS::Registers.xray_status = OPERSTATUS::XRAY_STATUS_READY;
			labelXrayStatus->Text = Translate::label("READY-FOR-EXPOSURE");
			xrayStat->BackgroundImage = XRAY_READY_IMAGE;
		}
	}

}
void OperatingForm::evaluateErrorStatus(void) {
	static bool cmp_force_error = false;

	// Compression Warning
	if ((GantryStatusRegisters::ExposureModeRegister::compressorMode->Value->getCode() != GantryStatusRegisters::CompressionModeOption::options::CMP_DISABLE) && (GantryStatusRegisters::CompressorRegister::getForce() == 0))
		Notify::activate("MISSING_COMPRESSION_WARNING", false);
	else Notify::deactivate("MISSING_COMPRESSION_WARNING");


	// Patient Protection
	if (
		(GantryStatusRegisters::ExposureModeRegister::protectionMode->Value->getCode() != GantryStatusRegisters::PatientProtectionMode::options::PROTECTION_DIS) &&
		(GantryStatusRegisters::ComponentRegister::Value->getCode() != GantryStatusRegisters::ComponentRegister::options::COMPONENT_PROTECTION_3D)&&
		(GantryStatusRegisters::CollimatorComponentRegister::Value->getCode() != GantryStatusRegisters::CollimatorComponentRegister::options::COLLI_COMPONENT_PROTECTION_2D)
		) Notify::activate("MISSING_PATIENT_PROTECTION_WARNING", false);
	else Notify::deactivate("MISSING_PATIENT_PROTECTION_WARNING");
	
	// C-Arm Mode
	if (
		(GantryStatusRegisters::ExposureModeRegister::armMode->Value->getCode() != GantryStatusRegisters::ArmModeOption::options::ARM_DIS) &&
		(!pMARM->isValidPosition())
		) Notify::activate("WRONG_ARM_POSITION_WARNING", false);
	else Notify::deactivate("WRONG_ARM_POSITION_WARNING");

	// Paddle identification
	if (
		(GantryStatusRegisters::ExposureModeRegister::compressorMode->Value->getCode() != GantryStatusRegisters::CompressionModeOption::options::CMP_DISABLE) &&
			(
				(GantryStatusRegisters::CompressorRegister::getPaddle()->Value->getCode() == GantryStatusRegisters::PaddleOption::options::PAD_UNDETECTED) ||
				(GantryStatusRegisters::CompressorRegister::getPaddle()->Value->getCode() == GantryStatusRegisters::PaddleOption::options::PAD_UNLOCKED) ||
				(GantryStatusRegisters::CompressorRegister::getPaddle()->Value->getCode() == GantryStatusRegisters::PaddleOption::options::UNDEF)
			)
		) Notify::activate("WRONG_PADDLE_WARNING", false);
	else Notify::deactivate("WRONG_PADDLE_WARNING");

	// Exposure Mode selection
	if (GantryStatusRegisters::ExposureModeRegister::exposureType->Value->getCode() == GantryStatusRegisters::ExposureTypeOption::options::UNDEF)
		Notify::activate("MISSING_EXPOSURE_MODE_WARNING", false);
	else Notify::deactivate("MISSING_EXPOSURE_MODE_WARNING");

	// Exposure Data selection
	if (!GantryStatusRegisters::ExposureDataRegister::getPulse(0)->isValid()) Notify::activate("MISSING_EXPOSURE_DATA_WARNING", false);
	else Notify::deactivate("MISSING_EXPOSURE_DATA_WARNING");

	// Xray Push Button Enable
	if (GantryStatusRegisters::XrayPushButtonRegister::isDisabled()) Notify::activate("XRAY_BUTTON_DISABLED_WARNING", false);
	else Notify::deactivate("XRAY_BUTTON_DISABLED_WARNING");

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
	if (GantryStatusRegisters::ExposureModeRegister::compressorMode->Value->getCode() == GantryStatusRegisters::CompressionModeOption::options::CMP_DISABLE) {
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
		if (GantryStatusRegisters::ExposureModeRegister::compressorMode->Value->getCode() == GantryStatusRegisters::CompressionModeOption::options::CMP_KEEP) {
			if (OPERSTATUS::Registers.compressor_release) {
				OPERSTATUS::Registers.compressor_release = false;
				decompressionStatus->BackgroundImage = COMPRESSION_KEEP_IMAGE;
			}
		}
		else if (GantryStatusRegisters::ExposureModeRegister::compressorMode->Value->getCode() == GantryStatusRegisters::CompressionModeOption::options::CMP_RELEASE) {
			if (!OPERSTATUS::Registers.compressor_release) {
				OPERSTATUS::Registers.compressor_release = true;
				decompressionStatus->BackgroundImage = COMPRESSION_RELEASE_IMAGE;
			}
		}

		if (!GantryStatusRegisters::CompressorRegister::isPaddle())
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


	// Cumulated max energy of the X-RAY tube
	labelTubeData->Text = PCB315::getTubeMaxCumulated().ToString() + " %";

	if (PCB315::isTubeAlarm() != OPERSTATUS::Registers.collimator.tube_alarm) {
		OPERSTATUS::Registers.collimator.tube_alarm = PCB315::isTubeAlarm();
		if (OPERSTATUS::Registers.collimator.tube_alarm) tubeStatus->BackgroundImage = TUBE_TEMP_NOK_IMAGE;
		else tubeStatus->BackgroundImage = TUBE_TEMP_OK_IMAGE;
	}
}

void OperatingForm::evaluateMagStatus(void) {
	return;
	// Magnification factor
	OPERSTATUS::Registers.mag_factor = GantryStatusRegisters::ComponentRegister::getMagFactor();
	magnifierStatus->BackgroundImage = MAGFACTOR_IMAGE;

}
void OperatingForm::evaluateDoorStatus(void) {
	if (GantryStatusRegisters::SafetyStatusRegister::getCloseDoor() != OPERSTATUS::Registers.closed_door) {
		OPERSTATUS::Registers.closed_door = GantryStatusRegisters::SafetyStatusRegister::getCloseDoor();

		if (OPERSTATUS::Registers.closed_door) {
			doorStatus->BackgroundImage = DOOR_CLOSED_IMAGE;
			Notify::deactivate("DOOR_STUDY_OPEN_WARNING");
		}
		else {
			doorStatus->BackgroundImage = DOOR_OPEN_IMAGE;
			Notify::activate("DOOR_STUDY_OPEN_WARNING", false);
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

	// Command Mirror selection
	if (collimator_light_activation) {
		collimator_light_activation = ! pFW315->isCommandCompleted();
	}
	
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
	if (collimator_light_activation) return;
	
	collimator_light_activation = pFW315->command(PCB315_COMMAND_LIGHT_ON, 30);
	if(!collimator_light_activation) Notify::activate("COLLI_LIGHT_ACTIVATION_WARNING", true);

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
