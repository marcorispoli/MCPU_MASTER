#include "OperatingForm.h"
#include "Notify.h"
#include "ErrorForm.h"
#include "gantry_global_status.h"
#include "Projections.h"
#include "ConfirmationWindow.h"
#include "IconWindow.h"
#include "VerticalMotor.h"
#include "ArmMotor.h"
#include "BodyMotor.h"
#include "SlideMotor.h"
#include "TiltMotor.h"
#include "ExposureModule.h"
#include "../DEVICES/PCB325.h"
#include "../DEVICES/PCB303.h"
#include "../DEVICES/PCB302.h"
#include "../DEVICES/PCB301.h"
#include "../DEVICES/PCB304.h"
#include "../DEVICES/PCB326.h"
#include "awsProtocol.h"


#define PROJ_NOT_SELECTED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\NoProjectionSelected.PNG")
#define PROJ_SELECTED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ProjectionSelected.PNG")

// Main Panel Definition
#define XRAY_ON_IMAGE  Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\XOn.PNG")
#define XRAY_STDBY_IMAGE  Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\XStdby.PNG")
#define XRAY_READY_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\XReady.PNG")
#define DEMO_IMAGE   Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\Demo.PNG")
#define SYM_IMAGE   Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\Sym.PNG")

#define LAMP_OFF_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\LampOff.PNG")
#define LAMP_ON_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\LampOn.PNG")

#define SLIDE_0_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\Slide0.PNG")
#define SLIDE_10_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\Slide10.PNG")

#define INFO_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\InfoIcon.PNG")
#define WARNING_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\WarningOn.PNG")
#define ERROR_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\AlarmOn.PNG")
#define ERROR_BUTTON_OFF Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\AlarmOff.PNG")

#define PADDLE_DISABLED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\CompressionNotEnabled.PNG")
#define PADDLE_ENABLE_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ActivePaddleDetected.PNG")
#define PADDLE_NOT_DETECTED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\PaddleNotPresent.PNG")

#define THICKNESS_DISABLED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ThicknessDisabled.PNG")
#define THICKNESS_ENABLED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ThicknessEnabled.PNG")

#define FORCE_DISABLED_NOT_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ForceDisabledNotCompressed.PNG")
#define FORCE_DISABLED_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ForceDisabledCompressed.PNG")
#define FORCE_ENABLED_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ForceEnabledCompressed.PNG")
#define FORCE_ENABLED_NOT_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\ForceEnabledNotCompressed.PNG")

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
#define XRAY_ICON  Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\xrayIcon.PNG")

#define COMPRESSING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\COMPRESSING_ICON.PNG")
#define ARM_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\ARM_MOTOR_ICON.PNG")
#define BODY_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\BODY_MOTOR_ICON.PNG")
#define VERTICAL_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\VERTICAL_MOTOR_ICON.PNG")
#define SLIDE_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\SLIDE_MOTOR_ICON.PNG")
#define TILT_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\TILT_MOTOR_ICON.PNG")


namespace OPERSTATUS {



	typedef struct {

		unsigned char	anode;
		unsigned char	stator;
		unsigned char	bulb;
		bool			tube_alarm;
		bool			light_on;
	}collimatorStatus;

	
	

	typedef struct {
		int exposure_mode;
		bool exposure_data_valid;

	}exposureStatus;

	static struct {
		
		collimatorStatus collimator;
		
		bool closed_door;		
		unsigned char currentPanel;
		bool alarm;
		bool warning;
		bool info;

		unsigned char mag_factor;		
		

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
	System::String^ confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION);
	System::String^ confInfoContent = Notify::TranslateContent(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION);
	pShiftConf = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
	((ConfirmationWindow^)pShiftConf)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &OperatingForm::onShiftConfirmCanc);
	((ConfirmationWindow^)pShiftConf)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &OperatingForm::onShiftConfirmOk);
	
	//________________________________________________________________________________________
	pXray = gcnew IconWindow(this, XRAY_ICON);
	

	OPERSTATUS::Registers.alarm = false;
	OPERSTATUS::Registers.warning = false;

	// Sets the Projection selection button
	projSelection->BackgroundImage = PROJ_NOT_SELECTED_IMAGE;
	selectedIcon->Hide();
	
	
	// Sets the Xray status	
	xrayStat->BackColor = this->BackColor;	
	labelXrayStatus->BackColor = Color::Transparent;
	labelXrayStatus->Text = Notify::TranslateLabel(Notify::messages::LABEL_NOT_READY_FOR_EXPOSURE);
	

	
	
	
	// Activate the Demo Icon if the Xrays are not 
	demoIcon->BackColor = Color::Transparent;
	if (Gantry::isOperatingDemo()) {		
		demoIcon->BackgroundImage = DEMO_IMAGE;
		demoIcon->Show();
	}
	else if (Exposures::isSimulatorMode()) {
		demoIcon->BackgroundImage = SYM_IMAGE;
		demoIcon->Show();
	}
	else {
		demoIcon->Hide();
	}

	// Sets the current lamp status
	OPERSTATUS::Registers.collimator.light_on= false;
	lampButton->BackgroundImage = LAMP_OFF_IMAGE;

	// Error Button
	OPERSTATUS::Registers.alarm = false;
	alarmButton->BackgroundImage = ERROR_BUTTON_OFF;


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
	decompressionStatus->BackgroundImage = COMPRESSION_KEEP_IMAGE;

	// Slide status	
	slideButton->BackgroundImage = SLIDE_10_IMAGE;

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

	// Clears any Instant popup window
	Notify::clrInstant();

	// Activate the Operating Status  manual modes
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_STANDARD_STATUS_MANUAL_ROTATION);

	// Activates the compressor
	PCB301::SetCompressorEna(true);
	

	// initialize the evaluation function
	evaluateCompressorStatus(true);

	// init of the slide status view
	evaluateSlideStatus(true);

	// init of the projection status view
	evaluateProjectionStatus(true);

	// Start the startup session	
	operatingTimer->Start();		
	resume();

	
	
}

void OperatingForm::open(void) {
	//SendNotifyMessageA(window, WINMSG_OPEN, 0, 0); // OPEN EVENT MESSAGE
	SendMessageA(window, WINMSG_OPEN, 0, 0); // OPEN EVENT MESSAGE
}

void OperatingForm::close(void) {
	SendMessageA(window, WINMSG_CLOSE, 0, 0); // CLOSE EVENT MESSAGE
	
}



void OperatingForm::onArmTargetChangedCallback(int id, int target){

	if (ArmMotor::device->isValidTarget()) {		
		selectedIcon->BackgroundImage = ArmMotor::getProjectionsList()->getCurrentProjectionIcon();
		selectedIcon->Show();		
	}
	else {
		selectedIcon->Hide();
	}

}


void OperatingForm::evaluateProjectionStatus(bool init) {
	static ProjectionOptions::options projection = ProjectionOptions::options::RESERVED_FOR_INIT;
	if(init) projection = ProjectionOptions::options::RESERVED_FOR_INIT;

	if (projection != ArmMotor::getSelectedProjectionCode()) {
		projection = ArmMotor::getSelectedProjectionCode();

		if (projection == ProjectionOptions::options::UNDEF) {
			projSelection->BackgroundImage = PROJ_NOT_SELECTED_IMAGE;
			selectedIcon->Hide();
		}
		else {
			projSelection->BackgroundImage = PROJ_SELECTED_IMAGE;
			selectedIcon->BackgroundImage = ArmMotor::getProjectionsList()->getCurrentProjectionIcon();
			selectedIcon->Show();
		}
	}

	float arm = (float)ArmMotor::device->getCurrentPosition() / 100;
	int iarm = (int)arm;

	if ((arm - (float)iarm) >= 0.5) iarm++;
	angleText->Text = iarm.ToString();
}

void OperatingForm::evaluateXrayStatus(void) {
	static int ready_stat = 255;
	int stat;

	if (Exposures::isXrayRunning()) stat = 3;
	else if (Notify::isError()) stat = 0;// Not Ready for errors (Standby)
	else if(Notify::isWarning()) stat = 1; // Not Ready for warnings (Standby)
	else  stat = 2; // Ready for exposure
	
	if (ready_stat != stat) {
		ready_stat = stat;
		
		if (stat == 0) {
			xrayStat->BackgroundImage = XRAY_STDBY_IMAGE;
			labelXrayStatus->Text = Notify::TranslateLabel(Notify::messages::LABEL_NOT_READY_FOR_EXPOSURE);
			awsProtocol::EVENT_ReadyForExposure();			
		}else if(stat == 1) {
			xrayStat->BackgroundImage = XRAY_STDBY_IMAGE;
			labelXrayStatus->Text = Notify::TranslateLabel(Notify::messages::LABEL_NOT_READY_FOR_EXPOSURE);
			awsProtocol::EVENT_ReadyForExposure();
		}else if (stat == 2) {
			xrayStat->BackgroundImage = XRAY_READY_IMAGE;
			labelXrayStatus->Text = Notify::TranslateLabel(Notify::messages::LABEL_READY_FOR_EXPOSURE);
			awsProtocol::EVENT_ReadyForExposure();
		}
		else {
			xrayStat->BackgroundImage = XRAY_ON_IMAGE;
			labelXrayStatus->Text = Notify::TranslateLabel(Notify::messages::LABEL_X_RAY_ON);
		}
	}

	
	// Evaluate the Xray icon display activation during Exposure
	/*
	static bool xray_running = true;
	if (xray_running != Exposures::isXrayRunning()) {
		xray_running = Exposures::isXrayRunning();
		if (xray_running) {
			((IconWindow^)pXray)->open();
		}
		else {
			((IconWindow^)pXray)->close();
		}
	}*/
	

}
void OperatingForm::evaluateReadyWarnings(bool reset) {

	// Clears all the warnings
	if (reset) {
		Notify::deactivate(Notify::messages::WARNING_MISSING_COMPRESSION);
		Notify::deactivate(Notify::messages::WARNING_MISSING_PATIENT_PROTECTION);
		Notify::deactivate(Notify::messages::WARNING_ARM_POSITION_WARNING);
		Notify::deactivate(Notify::messages::WARNING_WRONG_PADDLE);
		Notify::deactivate(Notify::messages::WARNING_MISSING_EXPOSURE_MODE);
		Notify::deactivate(Notify::messages::WARNING_MISSING_EXPOSURE_DATA);
		Notify::deactivate(Notify::messages::WARNING_XRAY_BUTTON_DISABLED);
		return;
	}

	static bool cmp_force_error = false;

	// Compression Mode Warning
	if ((Exposures::getCompressorMode() != Exposures::compression_mode_option::CMP_DISABLE) && (PCB302::getForce() == 0))
		Notify::activate(Notify::messages::WARNING_MISSING_COMPRESSION);
	else Notify::deactivate(Notify::messages::WARNING_MISSING_COMPRESSION);


	// Patient Protection Mode Warning
	bool patient_protection = (PCB302::getPatientProtection() == PCB302::PatientProtection::POSITIONED);

	if ((Exposures::getProtectionMode() != Exposures::patient_protection_option::PROTECTION_DIS) && (!patient_protection))
		Notify::activate(Notify::messages::WARNING_MISSING_PATIENT_PROTECTION);
	else 
		Notify::deactivate(Notify::messages::WARNING_MISSING_PATIENT_PROTECTION);
	
	// C-Arm Mode
	if (
		(Exposures::getArmMode() != Exposures::arm_mode_option::ARM_DIS) &&
		(!ArmMotor::device->isValidPosition())
		) Notify::activate(Notify::messages::WARNING_ARM_POSITION_WARNING);
	else Notify::deactivate(Notify::messages::WARNING_ARM_POSITION_WARNING);

	// Paddle identification
	if ((Exposures::getCompressorMode() != Exposures::compression_mode_option::CMP_DISABLE) &&
		(PCB302::getDetectedPaddleCode() == PCB302::paddleCodes::PADDLE_NOT_DETECTED)) Notify::activate(Notify::messages::WARNING_WRONG_PADDLE);
	else Notify::deactivate(Notify::messages::WARNING_WRONG_PADDLE);

	
	// Exposure Mode selection	
	if (Exposures::getExposureMode() == Exposures::exposure_type_options::EXP_NOT_DEFINED) Notify::activate(Notify::messages::WARNING_MISSING_EXPOSURE_MODE);
	else Notify::deactivate(Notify::messages::WARNING_MISSING_EXPOSURE_MODE);
	

	// Valid Exposure Data present
	if (!Exposures::getExposurePulse(0)->isValid()) Notify::activate(Notify::messages::WARNING_MISSING_EXPOSURE_DATA);
	else Notify::deactivate(Notify::messages::WARNING_MISSING_EXPOSURE_DATA);

	
	// Xray Push Button Enable
	if (!Exposures::getXrayPushButtonEvent()) Notify::activate(Notify::messages::WARNING_XRAY_BUTTON_DISABLED);
	else Notify::deactivate(Notify::messages::WARNING_XRAY_BUTTON_DISABLED);


	
}

#define PADDLE_DISABLED 1
#define PADDLE_NOT_DETECT 2
#define PADDLE_DETECTED 3

#define THICKNESS_DISABLED 1
#define THICKNESS_ENABLED 2

#define FORCE_DISABLED_NOT_COMPRESSED 1
#define FORCE_DISABLED_COMPRESSED 2
#define FORCE_ENABLED_NOT_COMPRESSED 3
#define FORCE_ENABLED_COMPRESSED 4

void OperatingForm::evaluateCompressorStatus(bool init) {

	static bool colli_light = false;
	static int paddle_status = 255;
	static int thick_status = 255;
	static int force_status = 255;

	int cur_paddle;
	int cur_thick;
	int cur_force;
	PCB302::paddleCodes paddle;
	int force, thick;


	// used to send the EVENT_Compressor() to AWS
	static int event_force = 0;
	static int event_thickness = 0;
	static int event_compressor_timer = 0;
	
	force = PCB302::getForce();
	thick = PCB302::getThickness();
	paddle = PCB302::getDetectedPaddleCode();
	
	if (colli_light != PCB302::isDownwardActivationStatus()) {
		colli_light = PCB302::isDownwardActivationStatus();

		// Activates the mirror (not during initialization)
		if ((colli_light) && (!init)) PCB303::setCollimationLight(true);
	}


	// Evaluates if the force or the thickness is changed:
	// the evaluation is performed every 1 seconds about.	
	if (init) {
		event_compressor_timer = 10;
		event_thickness = thick;
		event_force = force;
	}
	else {
		if (event_compressor_timer) event_compressor_timer--;
		else
		{
			event_compressor_timer = 10;
			bool event_compressor = false;

			if (abs(force - event_force) > 5) {
				event_force = force;
				event_compressor = true;
			}
			if (abs(thick - event_thickness) > 2) {
				event_thickness = thick;
				event_compressor = true;
			}

			// Signals to AWS the change event
			if (event_compressor) awsProtocol::EVENT_Compressor();
		}
	}
	

	if (Exposures::getCompressorMode() == Exposures::compression_mode_option::CMP_DISABLE) {
		cur_paddle = PADDLE_DISABLED;
		cur_thick = THICKNESS_DISABLED;

		labelPaddle->Hide();

		if (force) {
			labelForce->Show();
			labelForce->Text = force.ToString() + " N";
			cur_force = FORCE_DISABLED_COMPRESSED;
		}
		else {
			labelForce->Hide();
			cur_force = FORCE_DISABLED_NOT_COMPRESSED;
		}

		if (thick) {
			labelThickness->Text = thick.ToString() + " mm";
			labelThickness->Show();
		}
		else {
			labelThickness->Hide();
		}
	}
	else {
		if (paddle == PCB302::paddleCodes::PADDLE_NOT_DETECTED) {
			cur_paddle = PADDLE_NOT_DETECT;
			cur_thick = THICKNESS_DISABLED;
			cur_force = FORCE_DISABLED_NOT_COMPRESSED;

			labelPaddle->Hide();
			labelThickness->Hide();
			labelForce->Hide();
		}
		else {
			cur_paddle = PADDLE_DETECTED;
			cur_thick = THICKNESS_ENABLED;
			if (force) cur_force = FORCE_ENABLED_COMPRESSED;
			else cur_force = FORCE_ENABLED_NOT_COMPRESSED;

			labelPaddle->Show();
			labelPaddle->Text = PCB302::getPaddleName(paddle);

			if (force) {
				labelForce->Show();
				labelForce->Text = force.ToString() + " N";
				cur_force = FORCE_ENABLED_COMPRESSED;
			}
			else {
				labelForce->Hide();
				cur_force = FORCE_ENABLED_NOT_COMPRESSED;
			}

			if (thick) {
				labelThickness->Text = thick.ToString() + " mm";
				labelThickness->Show();
			}
			else {
				labelThickness->Hide();
			}
		}
	}

	if (cur_paddle != paddle_status) {
		paddle_status = cur_paddle;
		if (paddle_status == PADDLE_DISABLED) paddleStatus->BackgroundImage = PADDLE_DISABLED_IMAGE;
		else if (paddle_status == PADDLE_NOT_DETECT) paddleStatus->BackgroundImage = PADDLE_NOT_DETECTED_IMAGE;
		else paddleStatus->BackgroundImage = PADDLE_ENABLE_IMAGE;
	}

	if (cur_force != force_status) {
		force_status = cur_force;
		if (force_status == FORCE_DISABLED_NOT_COMPRESSED) forceStatus->BackgroundImage = FORCE_DISABLED_NOT_COMPRESSED_IMAGE;
		else if (force_status == FORCE_DISABLED_COMPRESSED) forceStatus->BackgroundImage = FORCE_DISABLED_COMPRESSED_IMAGE;
		else if (force_status == FORCE_ENABLED_NOT_COMPRESSED) forceStatus->BackgroundImage = FORCE_ENABLED_NOT_COMPRESSED_IMAGE;
		else  forceStatus->BackgroundImage = FORCE_ENABLED_COMPRESSED_IMAGE;
	}

	if (cur_thick != thick_status) {
		thick_status = cur_thick;
		if (thick_status == THICKNESS_DISABLED) thicknessStatus->BackgroundImage = THICKNESS_DISABLED_IMAGE;
		else thicknessStatus->BackgroundImage = THICKNESS_ENABLED_IMAGE;
	}

	

}

void OperatingForm::evaluateCompressorReleaseStatus(void) {
	static  Exposures::compression_mode_option cmpmode = Exposures::compression_mode_option::CMP_DISABLE;
	static bool init = true;
	if (init) {
		init = false;
		decompressionStatus->BackgroundImage = COMPRESSION_KEEP_IMAGE;
		return;
	}
	
	if (cmpmode == Exposures::getCompressorMode()) return;
	cmpmode = Exposures::getCompressorMode();

	if (cmpmode == Exposures::compression_mode_option::CMP_RELEASE) decompressionStatus->BackgroundImage = COMPRESSION_RELEASE_IMAGE;
	else decompressionStatus->BackgroundImage = COMPRESSION_KEEP_IMAGE;
	
}
void OperatingForm::evaluateCollimatorStatus(void) {
	

	// Sets the current lamp status
	if (PCB303::getPowerLightStatus() != OPERSTATUS::Registers.collimator.light_on) {
		OPERSTATUS::Registers.collimator.light_on = PCB303::getPowerLightStatus();
		if(OPERSTATUS::Registers.collimator.light_on) lampButton->BackgroundImage = LAMP_ON_IMAGE;
		else lampButton->BackgroundImage = LAMP_OFF_IMAGE;
	}


	// Evaluates the maximum tube temperature
	int val = 0;// PCB315::getAnode();
	if (PCB303::getBulb() > val) val = PCB303::getBulb();
	if (PCB303::getStator() > val) val = PCB303::getStator();
	

	// Cumulated max energy of the X-RAY tube
	labelTubeData->Text = val.ToString() + " %";

	if (PCB303::isTubeAlarm() != OPERSTATUS::Registers.collimator.tube_alarm) {
		OPERSTATUS::Registers.collimator.tube_alarm = PCB303::isTubeAlarm();
		if (OPERSTATUS::Registers.collimator.tube_alarm) tubeStatus->BackgroundImage = TUBE_TEMP_NOK_IMAGE;
		else tubeStatus->BackgroundImage = TUBE_TEMP_OK_IMAGE;
	}
}

void OperatingForm::evaluateMagStatus(void) {

	float magfactor = (float)PCB302::getMagnifierFactor() / 10;
	labelMag->Text = magfactor.ToString() +"x";

}

void OperatingForm::evaluateDoorStatus(void) {

	if (PCB301::isClosedDoor() != OPERSTATUS::Registers.closed_door) {
		OPERSTATUS::Registers.closed_door = PCB301::isClosedDoor();

		if (OPERSTATUS::Registers.closed_door ) {
			doorStatus->BackgroundImage = DOOR_CLOSED_IMAGE;
		}
		else {
			doorStatus->BackgroundImage = DOOR_OPEN_IMAGE;
		}
	}
	
}

void OperatingForm::evaluateSlideStatus(bool init) {
	static int stat = 255;
	if (init) stat = 255;

	int cur_stat;
	if (SlideMotor::device->getCurrentPosition() < 500) cur_stat = 0;
	else cur_stat = 1;
	
	if (stat != cur_stat) {
		stat = cur_stat;

		if(stat == 0)  slideButton->BackgroundImage = SLIDE_0_IMAGE;
		else slideButton->BackgroundImage = SLIDE_10_IMAGE;
	}

	
}

void OperatingForm::evaluateGridStatus(void) {
	// Evaluates the current selected Exposure mode to define what is the current grid position
	bool inOut_Field = false; // Out Field

	switch (Exposures::getExposureMode()) {
	case Exposures::exposure_type_options::AEC_2D: inOut_Field = true; break; // In Field
	case Exposures::exposure_type_options::AEC_3D: inOut_Field = false; break; // Out Field
	case Exposures::exposure_type_options::AEC_AE: inOut_Field = true; break; // In Field
	case Exposures::exposure_type_options::AEC_COMBO: inOut_Field = true; break; // In Field
	case Exposures::exposure_type_options::MAN_2D: inOut_Field = true; break; // In Field
	case Exposures::exposure_type_options::MAN_3D: inOut_Field = false; break; // Out Field
	case Exposures::exposure_type_options::MAN_AE: inOut_Field = true; break; // In Field
	case Exposures::exposure_type_options::MAN_COMBO: inOut_Field = true; break; // In Field
	default:inOut_Field = true; break; // In Field
	}

	if(inOut_Field)	PCB304::setAutoGridInField();
	else PCB304::setAutoGridOutField();
}

void OperatingForm::operatingStatusManagement(void) {
	
	System::DateTime date;
	date = System::DateTime::Now;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	
	evaluateCompressorStatus(false);
	evaluateCompressorReleaseStatus();
	evaluateCollimatorStatus();
	evaluateMagStatus();
	evaluateDoorStatus();
	evaluateSlideStatus(false);
	evaluateProjectionStatus(false);
	evaluateDigitDisplays();
	evaluateGridStatus();

	// This shall be posed at the end of the management
	evaluateReadyWarnings(false);
	evaluateXrayStatus(); 
	
	evaluatePopupPanels();

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
		this->Show();
		initOperatingStatus();
		break;

	case (WINMSG_CLOSE): // on Open Event
		if (!open_status) break; 
		open_status = false;
		operatingTimer->Stop();
		((ProjectionForm^)pProj)->close();
		Notify::close_error();
		Notify::close_instant();
		((ConfirmationWindow^)pAbort)->close();
		((IconWindow^)pXray)->close();
		this->Hide();
		break;		
	}


	Form::WndProc(m);
}

void OperatingForm::errorButton_Click(System::Object^ sender, System::EventArgs^ e) {
	if(Notify::isError() || Notify::isWarning())	Notify::open_error(this);
}

void OperatingForm::viewSelection_Click(System::Object^ sender, System::EventArgs^ e) {
	if (!ArmMotor::device->isValidTarget())	((ProjectionForm^)pProj)->open();
	else {
		((ConfirmationWindow^)pAbort)->setContentBackground(selectedIcon->BackgroundImage);
		((ConfirmationWindow^)pAbort)->open();
	}
}


void OperatingForm::ShiftSelection_Click(System::Object^ sender, System::EventArgs^ e) {
	
	// If a motorization is activated returns 
	if (Gantry::isMotorsActive()) return;

	((ConfirmationWindow^)pShiftConf)->setContentBackground(slideButton->BackgroundImage);
	((ConfirmationWindow^)pShiftConf)->open();
}



void OperatingForm::lampButton_Click(System::Object^ sender, System::EventArgs^ e) {
	PCB303::setCollimationLight(true);
	
}

void OperatingForm::onConfirmOk(void) {
	((ProjectionForm^)pProj)->close();
	ArmMotor::projectionRequest(((ProjectionForm^)pProj)->selectedProjection);
	
}
void OperatingForm::onConfirmCanc(void) {
	((ProjectionForm^)pProj)->close();
}

void OperatingForm::onShiftConfirmOk(void) {

	if (SlideMotor::device->getCurrentPosition() < 500) SlideMotor::isoAutoPosition(1000);
	else SlideMotor::isoAutoPosition(0);
	
}
void OperatingForm::onShiftConfirmCanc(void) {
	
}

void OperatingForm::onAbortConfirmOk(void) {	
	ArmMotor::abortProjectionRequest();

}
void OperatingForm::onAbortConfirmCanc(void) {
}


void OperatingForm::evaluateDigitDisplays(void) {

	// If a compression force is detected the display shall be activated!
	short dspval;
	unsigned char digits, intensity, blink;
	
	if (PCB302::getForce()) {
		blink = false;
		intensity = 0;

		if (PCB302::isDownwardActivationStatus()) intensity = 0xf;
		else intensity = 0;

		// newtons
		digits = 0;		
		dspval = PCB302::getForce();

		if (dspval < 30) blink = true;
		else blink = false;
		
		PCB304::setDisplay(dspval, digits, blink, intensity);
		return;
	}
	
	if (ArmMotor::device->isRunning()) {
		blink = false;
		intensity = 0xf;

		// Degrees
		digits = 0;
		dspval = (ArmMotor::device->getCurrentPosition() / 100);
		PCB304::setDisplay(dspval, digits, blink, intensity);
		return;
	}

	if (BodyMotor::device->isRunning()) {
		blink = false;
		intensity = 0xf;

		// Degrees
		digits = 0;
		dspval = (BodyMotor::device->getCurrentPosition() / 10);
		PCB304::setDisplay(dspval, digits, blink, intensity);
		return;
	}

	if (SlideMotor::device->isRunning()) {
		blink = false;
		intensity = 0xf;

		// Degrees
		digits = 0;
		dspval = (SlideMotor::device->getCurrentPosition() / 100);
		PCB304::setDisplay(dspval, digits, blink, intensity);
		return;
	}

	PCB304::setDisplay(false);
}

void OperatingForm::evaluatePopupPanels(void) {
#define TMO 20
	static bool compression = false;
	static bool arm = false;
	static bool body = false;
	static bool slide = false;
	static bool vertical = false;
	static bool tilt = false;
	static int  timer = 0;

	// With a panel already open do not continue;
	if (/*Exposures::isXrayRunning() || */Notify::isInstantOpen() || Notify::isErrorOpen()) {
		if(Gantry::getValuePopupWindow()->open_status) Gantry::getValuePopupWindow()->close();
		compression = false;
		arm = false;
		body = false;
		slide = false;
		vertical = false;
		tilt = false;
		timer = 0;
		return;
	}

	// Error Button
	if (Notify::isError()) {
		OPERSTATUS::Registers.warning = false;
		OPERSTATUS::Registers.info = false;
		if (!OPERSTATUS::Registers.alarm) {
			alarmButton->BackgroundImage = ERROR_BUTTON_ON;
			OPERSTATUS::Registers.alarm = true;
		}
	}
	else if (Notify::isWarning()) {
		OPERSTATUS::Registers.alarm = false;
		OPERSTATUS::Registers.info = false;
		if (!OPERSTATUS::Registers.warning) {
			alarmButton->BackgroundImage = WARNING_BUTTON_ON;
			OPERSTATUS::Registers.warning = true;
		}
	}
	else if (Notify::isInfo()) {
		OPERSTATUS::Registers.alarm = false;
		OPERSTATUS::Registers.warning = false;
		if (!OPERSTATUS::Registers.info) {
			alarmButton->BackgroundImage = INFO_BUTTON_ON;
			OPERSTATUS::Registers.info = true;
		}
	}
	else {
		if ((OPERSTATUS::Registers.warning) || (OPERSTATUS::Registers.alarm)) {
			OPERSTATUS::Registers.warning = false;
			OPERSTATUS::Registers.alarm = false;
			OPERSTATUS::Registers.info = false;
			alarmButton->BackgroundImage = ERROR_BUTTON_OFF;
		}

	}

	
	if (!compression) {
		// Compressor Window Initialization
		if ((!PCB302::isIdleStatus()) && (PCB302::getForce() > 0)) {
			timer = TMO;
			compression = true;
			arm = false;
			body = false;
			vertical = false;
			slide = false;
			tilt = false;
			if (Gantry::getValuePopupWindow()->open_status) Gantry::getValuePopupWindow()->retitle(COMPRESSING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_COMPRESSION_ACTIVATED), "(N)");
			else Gantry::getValuePopupWindow()->open(this, COMPRESSING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_COMPRESSION_ACTIVATED), "(N)");
			Gantry::getValuePopupWindow()->content(PCB302::getForce().ToString());
			return;
		}
	}
	else{
		// When the compressing timer is working the window keeps the value updated
		Gantry::getValuePopupWindow()->content(PCB302::getForce().ToString());
		if ((!PCB302::isIdleStatus()) && (PCB302::getForce() > 0)) timer = TMO;
	}

	if (!arm) {
		// Compressor Window Initialization
		if (ArmMotor::device->isRunning()) {
			timer = TMO;
			compression = false;
			arm = true;
			body = false;
			vertical = false;
			slide = false;
			tilt = false;
			if (Gantry::getValuePopupWindow()->open_status) Gantry::getValuePopupWindow()->retitle(ARM_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_ARM_ACTIVATED), "(°)");
			else Gantry::getValuePopupWindow()->open(this, ARM_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_ARM_ACTIVATED), "(°)");
			
			// Set the value to the current compression
			int position = ArmMotor::device->getCurrentPosition() / 100;
			Gantry::getValuePopupWindow()->content(position.ToString());
			return;
		}
	}
	else {
		// Set the value to the current compression
		int position = ArmMotor::device->getCurrentPosition() / 100;
		Gantry::getValuePopupWindow()->content(position.ToString());
		if (ArmMotor::device->isRunning()) timer = TMO;
	}

	
	if (!body) {
		// Compressor Window Initialization
		if (BodyMotor::device->isRunning()) {
			timer = TMO;
			compression = false;
			arm = false;
			body = true;
			vertical = false;
			slide = false;
			tilt = false;
			if (Gantry::getValuePopupWindow()->open_status) Gantry::getValuePopupWindow()->retitle(BODY_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_BODY_ACTIVATED), "(°)");
			else Gantry::getValuePopupWindow()->open(this, BODY_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_BODY_ACTIVATED), "(°)");

			// Set the value to the current compression
			float position = (float)BodyMotor::device->getCurrentPosition() / 10;
			Gantry::getValuePopupWindow()->content(position.ToString());
			return;
		}
	}
	else {
		// Set the value to the current compression
		float position = (float)BodyMotor::device->getCurrentPosition() / 10;
		Gantry::getValuePopupWindow()->content(position.ToString());
		if (BodyMotor::device->isRunning()) timer = TMO;
	}

	if (!vertical) {
		// Compressor Window Initialization
		if (VerticalMotor::device->isRunning()) {
			timer = TMO;
			compression = false;
			arm = false;
			body = false;
			vertical = true;
			slide = false;
			tilt = false;
			if (Gantry::getValuePopupWindow()->open_status) Gantry::getValuePopupWindow()->retitle(VERTICAL_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_VERTICAL_ACTIVATED), "(mm)");
			else Gantry::getValuePopupWindow()->open(this, VERTICAL_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_VERTICAL_ACTIVATED), "(mm)");

			// Set the value to the current compression
			int position = (int)VerticalMotor::device->getCurrentPosition();
			Gantry::getValuePopupWindow()->content(position.ToString());
			return;
		}
	}
	else {
		// Set the value to the current compression
		int position = (int)VerticalMotor::device->getCurrentPosition();
		Gantry::getValuePopupWindow()->content(position.ToString());
		if (VerticalMotor::device->isRunning()) timer = TMO;
	}
	
	
	if (!slide) {
		// Compressor Window Initialization
		if (SlideMotor::device->isRunning()) {
			timer = TMO;
			compression = false;
			arm = false;
			body = false;
			vertical = false;
			slide = true;
			tilt = false;
			if (Gantry::getValuePopupWindow()->open_status) Gantry::getValuePopupWindow()->retitle(SLIDE_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_SLIDE_ACTIVATED), "(°)");
			else Gantry::getValuePopupWindow()->open(this, SLIDE_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_SLIDE_ACTIVATED), "(°)");

			// Set the value to the current compression
			int position = (int)SlideMotor::device->getCurrentPosition() / 100;
			Gantry::getValuePopupWindow()->content(position.ToString());
			return;
		}
	}
	else {
		// Set the value to the current compression
		int position = (int)SlideMotor::device->getCurrentPosition() / 100;
		Gantry::getValuePopupWindow()->content(position.ToString());
		if (SlideMotor::device->isRunning()) timer = TMO;
	}
	
	if (!tilt) {
		// Compressor Window Initialization
		if (TiltMotor::device->isRunning()) {
			timer = TMO;
			compression = false;
			arm = false;
			body = false;
			vertical = false;
			slide = false;
			tilt = true;
			if (Gantry::getValuePopupWindow()->open_status) Gantry::getValuePopupWindow()->retitle(TILT_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_TILT_ACTIVATED), "(.01°)");
			else Gantry::getValuePopupWindow()->open(this, TILT_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_TILT_ACTIVATED), "(°)");

			// Set the value to the current compression
			int position = (int)TiltMotor::device->getCurrentPosition() / 100;
			Gantry::getValuePopupWindow()->content(position.ToString());
			return;
		}
	}
	else {
		// Set the value to the current compression
		int position = (int)TiltMotor::device->getCurrentPosition() / 100;
		Gantry::getValuePopupWindow()->content(position.ToString());
		if (TiltMotor::device->isRunning()) timer = TMO;
	}
	

	// Keeps the popup alive for extra time
	if (timer) {
		timer--;
		if (!timer) {
			compression = false;
			arm = false;
			body = false;
			vertical = false;
			slide = false;
			tilt = false;
			Gantry::getValuePopupWindow()->close();			
		}
		return;
	}

	if (Notify::isInstant()) Notify::open_instant(this);

}