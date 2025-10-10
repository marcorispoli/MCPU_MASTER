#include "TestStudy.h"
#include "aecSelectionDialog.h"
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


// Main Panel Definition
#define XRAY_ON_IMAGE  Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\XOn.PNG")
#define XRAY_STDBY_IMAGE  Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\XStdby.PNG")
#define XRAY_READY_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\XReady.PNG")
#define SYM_IMAGE   Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\Sym.PNG")


#define AEC_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\aecSelection.PNG")

#define SLIDE_0_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\Slide0.PNG")
#define SLIDE_10_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\Slide10.PNG")

#define INFO_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\InfoIcon.PNG")
#define WARNING_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\WarningOn.PNG")
#define ERROR_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\AlarmOn.PNG")
#define ERROR_BUTTON_OFF Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\AlarmOff.PNG")

#define PADDLE_DISABLED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\CompressionNotEnabled.PNG")
#define PADDLE_ENABLE_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\ActivePaddleDetected.PNG")
#define PADDLE_NOT_DETECTED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\PaddleNotPresent.PNG")

#define THICKNESS_DISABLED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\ThicknessDisabled.PNG")
#define THICKNESS_ENABLED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\ThicknessEnabled.PNG")

#define FORCE_DISABLED_NOT_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\ForceDisabledNotCompressed.PNG")
#define FORCE_DISABLED_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\ForceDisabledCompressed.PNG")
#define FORCE_ENABLED_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\ForceEnabledCompressed.PNG")
#define FORCE_ENABLED_NOT_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\ForceEnabledNotCompressed.PNG")

#define COLLIMATION_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\CollimationMode.PNG")

#define TUBE_TEMP_OK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\TubeOk.PNG")
#define TUBE_TEMP_NOK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\TubeNok.PNG")

#define RESIDUAL_EXPOSURE_OK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\ExposureLeftOk.PNG")
#define RESIDUAL_EXPOSURE_NOK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\ExposureLeftNok.PNG")

#define XRAY_ICON  Image::FromFile(Gantry::applicationResourcePath + "TestStudy\\xrayIcon.PNG")

#define COMPRESSING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\COMPRESSING_ICON.PNG")
#define ARM_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\ARM_MOTOR_ICON.PNG")
#define BODY_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\BODY_MOTOR_ICON.PNG")
#define VERTICAL_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\VERTICAL_MOTOR_ICON.PNG")
#define SLIDE_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\SLIDE_MOTOR_ICON.PNG")
#define TILT_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\TILT_MOTOR_ICON.PNG")


namespace TESTSTATUS {

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
		
		unsigned char currentPanel;
		bool alarm;
		bool warning;
		bool info;


	}Registers;
};

void TestStudy::suspend(void) {
	
	this->mainPanel->SuspendLayout();
	this->xrayStat->SuspendLayout();
	
	this->forceStatus->SuspendLayout();
	this->thicknessStatus->SuspendLayout();
	this->paddleStatus->SuspendLayout();
	this->tubeStatus->SuspendLayout();
	this->slideButton->SuspendLayout();
	this->collimationStatus->SuspendLayout();
	this->SuspendLayout();
}
void TestStudy::resume(void) {
	
	this->mainPanel->ResumeLayout(false);
	this->mainPanel->PerformLayout();
	this->xrayStat->ResumeLayout(false);
	
	this->forceStatus->ResumeLayout(false);
	this->thicknessStatus->ResumeLayout(false);
	this->paddleStatus->ResumeLayout(false);
	this->tubeStatus->ResumeLayout(false);
	this->slideButton->ResumeLayout(false);
	this->collimationStatus->ResumeLayout(false);
	this->ResumeLayout(false);
}

void TestStudy::formInitialization(void) {

	// Initialize the position of the form
	this->Left = Gantry::monitor_X0;
	this->Top = Gantry::monitor_Y0;

	mainPanel->SetBounds(0, 0, 600, 1024);
	
	
	
	// Confirmation Panel Setup ____________________________________________________________
	System::String^ confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION);
	System::String^ confInfoContent = Notify::TranslateContent(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION);
	pShiftConf = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
	((ConfirmationWindow^)pShiftConf)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &TestStudy::onShiftConfirmCanc);
	((ConfirmationWindow^)pShiftConf)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &TestStudy::onShiftConfirmOk);
	
	// AEC Panel creation	
	pAec = gcnew aecSelectionDialog();

	//________________________________________________________________________________________
	pXray = gcnew IconWindow(this, XRAY_ICON);
	

	TESTSTATUS::Registers.alarm = false;
	TESTSTATUS::Registers.warning = false;

	
	// Sets the Xray status	
	xrayStat->BackColor = this->BackColor;	
	labelXrayStatus->BackColor = Color::Transparent;
	labelXrayStatus->Text = Notify::TranslateLabel(Notify::messages::LABEL_NOT_READY_FOR_EXPOSURE);
	

	// Activate the Demo Icon if the Xrays are not 
	demoIcon->BackColor = Color::Transparent;
	if (Exposures::isSimulatorMode()) {
		demoIcon->BackgroundImage = SYM_IMAGE;
		demoIcon->Show();
	}
	else {
		demoIcon->Hide();
	}


	// Sets the current AEC Status
	aecButton->BackgroundImage = AEC_IMAGE;
	labelAec->Text = "AUTO";

	// Error Button
	TESTSTATUS::Registers.alarm = false;
	alarmButton->BackgroundImage = ERROR_BUTTON_OFF;


	// Collimator Status option
	collimationStatus->BackgroundImage = COLLIMATION_IMAGE;

	// Tube Status
	TESTSTATUS::Registers.collimator.bulb = 0;
	TESTSTATUS::Registers.collimator.stator = 0;
	TESTSTATUS::Registers.collimator.anode = 0;
	TESTSTATUS::Registers.collimator.tube_alarm = false;
	tubeStatus->BackgroundImage = TUBE_TEMP_OK_IMAGE;

	
	// Slide status	
	slideButton->BackgroundImage = SLIDE_10_IMAGE;

	operatingTimer = gcnew System::Timers::Timer(100);
	operatingTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &TestStudy::onOperatingTimeout);
	operatingTimer->Stop();

	
	this->Hide();
	open_status = false;


}

void TestStudy::initOperatingStatus(void) {
	suspend();

	// Initialize Date and time
	System::DateTime date;
	date = System::DateTime::Now;
	labelDate->Text = date.Day + ":" + date.Month + ":" + date.Year;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	// Initialize the Patient name
	labelTestName->Text = Gantry::getTestName();

	
	// Init AecSelction	
	((aecSelectionDialog^)pAec)->field_selected = 0;

	// Invalidate the projections and clears the list
	ArmMotor::abortTarget();
	ArmMotor::getProjectionsList()->clrList();

	// Sets the current collimation to OPEN mode
	PCB303::setFormatCollimationMode(PCB303::collimationModeEnum::OPEN, 0);

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

	

	// Start the startup session	
	operatingTimer->Start();		
	resume();

	
	
}

void TestStudy::open(void) {
	//SendNotifyMessageA(window, WINMSG_OPEN, 0, 0); // OPEN EVENT MESSAGE
	SendMessageA(window, WINMSG_OPEN, 0, 0); // OPEN EVENT MESSAGE
}

void TestStudy::close(void) {
	SendMessageA(window, WINMSG_CLOSE, 0, 0); // CLOSE EVENT MESSAGE
	
}





void TestStudy::evaluateXrayStatus(void) {
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
void TestStudy::evaluateReadyWarnings(bool reset) {

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

void TestStudy::evaluateCompressorStatus(bool init) {

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

void TestStudy::evaluateCollimatorStatus(void) {
	


	// Evaluates the maximum tube temperature	
	labelTubeData->Text = PCB303::getMaxTubePerc().ToString() + " %";

	if (PCB303::isTubeAlarm() != TESTSTATUS::Registers.collimator.tube_alarm) {
		TESTSTATUS::Registers.collimator.tube_alarm = PCB303::isTubeAlarm();
		if (TESTSTATUS::Registers.collimator.tube_alarm) tubeStatus->BackgroundImage = TUBE_TEMP_NOK_IMAGE;
		else tubeStatus->BackgroundImage = TUBE_TEMP_OK_IMAGE;
	}
}

void TestStudy::evaluateSlideStatus(bool init) {
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

void TestStudy::evaluateGridStatus(void) {
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

void TestStudy::evaluateAwsComponentEvent(void) {
	bool generate_event_component = false;
	static PCB302::paddleCodes paddle = PCB302::paddleCodes::PADDLE_NOT_DETECTED;
	static PCB302::PatientProtection patient_protection = PCB302::PatientProtection::UNDETECTED;
	static unsigned char mag = 255;

	// Checks for patient protection detection changed	
	if (PCB302::getPatientProtection() != patient_protection) {
		generate_event_component = true;
		patient_protection = PCB302::getPatientProtection();
	}

	// Checks for paddle changing
	if (PCB302::getDetectedPaddleCode() != paddle) {
		paddle = PCB302::getDetectedPaddleCode();
		generate_event_component = true;
	}
	
	// Checks for mag factor changes
	if (mag != PCB302::getMagnifierFactor()) {
		mag = PCB302::getMagnifierFactor();
		generate_event_component = true;
	}

	if (generate_event_component) awsProtocol::EVENT_Components();
}

void TestStudy::operatingStatusManagement(void) {
	
	System::DateTime date;
	date = System::DateTime::Now;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	
	evaluateCompressorStatus(false);
	
	evaluateCollimatorStatus();
	
	evaluateSlideStatus(false);
	
	
	evaluateGridStatus();
	evaluateAwsComponentEvent();

	// Evaluates the AEC field manual selection
	if (Exposures::isXrayRunning()) {
		// Resets as soon as the exposure is activated
		((aecSelectionDialog^)pAec)->field_selected = 0;
	}
	else {
		if (((aecSelectionDialog^)pAec)->field_selected == 0) {
			labelAec->Text = "AUTO";
		}
		else {
			labelAec->Text = "M" + ((aecSelectionDialog^)pAec)->field_selected.ToString();
		}

	}

	// This shall be posed at the end of the management
	evaluateReadyWarnings(false);
	evaluateXrayStatus(); 
	
	evaluatePopupPanels();

}

void TestStudy::WndProc(System::Windows::Forms::Message% m)
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
		
		Notify::close_error();
		Notify::close_instant();		
		((IconWindow^)pXray)->close();
		this->Hide();
		break;		
	}


	Form::WndProc(m);
}

void TestStudy::errorButton_Click(System::Object^ sender, System::EventArgs^ e) {
	if(Notify::isError() || Notify::isWarning())	Notify::open_error(this);
}



void TestStudy::ShiftSelection_Click(System::Object^ sender, System::EventArgs^ e) {
	
	// If a motorization is activated returns 
	if (Gantry::isMotorsActive()) return;

	((ConfirmationWindow^)pShiftConf)->setContentBackground(slideButton->BackgroundImage);
	((ConfirmationWindow^)pShiftConf)->open();
}




void TestStudy::aecButton_Click(System::Object^ sender, System::EventArgs^ e) {
	
	((aecSelectionDialog^)pAec)->open(this);
}


void TestStudy::onShiftConfirmOk(void) {

	if (SlideMotor::device->getCurrentPosition() < 500) SlideMotor::isoAutoPosition(1000);
	else SlideMotor::isoAutoPosition(0);
	
}
void TestStudy::onShiftConfirmCanc(void) {
	
}


void TestStudy::evaluatePopupPanels(void) {
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
		TESTSTATUS::Registers.warning = false;
		TESTSTATUS::Registers.info = false;
		if (!TESTSTATUS::Registers.alarm) {
			alarmButton->BackgroundImage = ERROR_BUTTON_ON;
			TESTSTATUS::Registers.alarm = true;
		}
	}
	else if (Notify::isWarning()) {
		TESTSTATUS::Registers.alarm = false;
		TESTSTATUS::Registers.info = false;
		if (!TESTSTATUS::Registers.warning) {
			alarmButton->BackgroundImage = WARNING_BUTTON_ON;
			TESTSTATUS::Registers.warning = true;
		}
	}
	else if (Notify::isInfo()) {
		TESTSTATUS::Registers.alarm = false;
		TESTSTATUS::Registers.warning = false;
		if (!TESTSTATUS::Registers.info) {
			alarmButton->BackgroundImage = INFO_BUTTON_ON;
			TESTSTATUS::Registers.info = true;
		}
	}
	else {
		if ((TESTSTATUS::Registers.warning) || (TESTSTATUS::Registers.alarm)) {
			TESTSTATUS::Registers.warning = false;
			TESTSTATUS::Registers.alarm = false;
			TESTSTATUS::Registers.info = false;
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