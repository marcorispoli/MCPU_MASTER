#include "BiopsyStudy.h"
#include "Notify.h"
#include "ErrorForm.h"
#include "gantry_global_status.h"
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
#define MOTORS_PANEL_IMAGE  Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\Motors.PNG")
#define COORDINATES_IMAGE  Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\Coordinates.PNG")


#define XRAY_ON_IMAGE  Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\XOn.PNG")
#define XRAY_STDBY_IMAGE  Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\XStdby.PNG")
#define XRAY_READY_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\XReady.PNG")
#define DEMO_IMAGE   Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\Demo.PNG")
#define SYM_IMAGE   Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\Sym.PNG")

#define LAMP_OFF_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\LampOff.PNG")
#define LAMP_ON_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\LampOn.PNG")


#define SLIDE_0_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\Slide0.PNG")
#define SLIDE_10_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\Slide10.PNG")

#define INFO_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\InfoIcon.PNG")
#define WARNING_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\WarningOn.PNG")
#define ERROR_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\AlarmOn.PNG")
#define ERROR_BUTTON_OFF Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\AlarmOff.PNG")

#define PADDLE_DISABLED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\CompressionNotEnabled.PNG")
#define PADDLE_ENABLE_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\ActivePaddleDetected.PNG")
#define PADDLE_NOT_DETECTED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\PaddleNotPresent.PNG")

#define THICKNESS_DISABLED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\ThicknessDisabled.PNG")
#define THICKNESS_ENABLED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\ThicknessEnabled.PNG")

#define FORCE_DISABLED_NOT_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\ForceDisabledNotCompressed.PNG")
#define FORCE_DISABLED_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\ForceDisabledCompressed.PNG")
#define FORCE_ENABLED_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\ForceEnabledCompressed.PNG")
#define FORCE_ENABLED_NOT_COMPRESSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\ForceEnabledNotCompressed.PNG")


#define TUBE_TEMP_OK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\TubeOk.PNG")
#define TUBE_TEMP_NOK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\TubeNok.PNG")

#define DOOR_CLOSED_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\DoorClosed.PNG")
#define DOOR_OPEN_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\DoorOpen.PNG")

#define RESIDUAL_EXPOSURE_OK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\ExposureLeftOk.PNG")
#define RESIDUAL_EXPOSURE_NOK_IMAGE Image::FromFile(Gantry::applicationResourcePath + "BiopsyStudy\\ExposureLeftNok.PNG")

#define XRAY_ICON  Image::FromFile(Gantry::applicationResourcePath + "OperatingForm\\xrayIcon.PNG")

#define COMPRESSING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\COMPRESSING_ICON.PNG")
#define ARM_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\ARM_MOTOR_ICON.PNG")
#define BODY_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\BODY_MOTOR_ICON.PNG")
#define VERTICAL_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\VERTICAL_MOTOR_ICON.PNG")
#define SLIDE_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\SLIDE_MOTOR_ICON.PNG")
#define TILT_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\TILT_MOTOR_ICON.PNG")


namespace BIOPSTATUS {

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

void BiopsyStudy::suspend(void) {
	
	this->mainPanel->SuspendLayout();
	this->xrayStat->SuspendLayout();	
	this->forceStatus->SuspendLayout();
	this->thicknessStatus->SuspendLayout();	
	this->tubeStatus->SuspendLayout();
	this->slideButton->SuspendLayout();	
	this->SuspendLayout();
}
void BiopsyStudy::resume(void) {
	
	this->mainPanel->ResumeLayout(false);
	this->mainPanel->PerformLayout();
	this->xrayStat->ResumeLayout(false);
	this->forceStatus->ResumeLayout(false);
	this->thicknessStatus->ResumeLayout(false);
	this->tubeStatus->ResumeLayout(false);
	this->slideButton->ResumeLayout(false);	
	this->ResumeLayout(false);
}

void BiopsyStudy::formInitialization(void) {

	// Initialize the position of the form
	this->Left = Gantry::monitor_X0;
	this->Top = Gantry::monitor_Y0;

	mainPanel->SetBounds(0, 0, 600, 1024);
	
	
	// Confirmation Panel Setup ____________________________________________________________
	System::String^ confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION);
	System::String^ confInfoContent = Notify::TranslateContent(Notify::messages::INFO_SLIDE_ACTIVATION_CONFIRMATION);
	pShiftConf = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
	((ConfirmationWindow^)pShiftConf)->button_canc_event += gcnew ConfirmationWindow::delegate_button_callback(this, &BiopsyStudy::onShiftConfirmCanc);
	((ConfirmationWindow^)pShiftConf)->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &BiopsyStudy::onShiftConfirmOk);
	

	//________________________________________________________________________________________
	pXray = gcnew IconWindow(this, XRAY_ICON);
	

	BIOPSTATUS::Registers.alarm = false;
	BIOPSTATUS::Registers.warning = false;

	
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

	// Motors Panel
	armPosition->BackgroundImage = MOTORS_PANEL_IMAGE;
	pointerPosition->BackgroundImage = COORDINATES_IMAGE;


	// Sets the current lamp status
	BIOPSTATUS::Registers.collimator.light_on= false;
	lampButton->BackgroundImage = LAMP_OFF_IMAGE;

	// Error Button
    BIOPSTATUS::Registers.alarm = false;
	alarmButton->BackgroundImage = ERROR_BUTTON_OFF;


	// Tube Status
	BIOPSTATUS::Registers.collimator.bulb = 0;
	BIOPSTATUS::Registers.collimator.stator = 0;
	BIOPSTATUS::Registers.collimator.anode = 0;
	BIOPSTATUS::Registers.collimator.tube_alarm = false;
	tubeStatus->BackgroundImage = TUBE_TEMP_OK_IMAGE;

	// Door Status 
	BIOPSTATUS::Registers.closed_door = false;
	doorStatus->BackgroundImage = DOOR_OPEN_IMAGE;


	// Slide status	
	slideButton->BackgroundImage = SLIDE_10_IMAGE;

	operatingTimer = gcnew System::Timers::Timer(100);
	operatingTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &BiopsyStudy::onOperatingTimeout);
	operatingTimer->Stop();

	
	this->Hide();
	open_status = false;


}

void BiopsyStudy::initOperatingStatus(void) {
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
	armLabel->Text = "ARM:" + arm_position.ToString() + "°";

	// Init Tilt Angle 
	float tilt_position = ((float)TiltMotor::device->getCurrentPosition() / 100);
	tiltLabel->Text = "TILT:" + tilt_position.ToString() + "°";

	// Init X Cursor	
	XLabel->Text = "X: 0";

	// Init X Cursor	
	YLabel->Text = "Y: 0";

	// Init X Cursor	
	ZLabel->Text = "Z: 0";


	// Collimation in Auto mode
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

	// The Biopsy device is set in Command mode and the keyboard is set in normal mode
	PCB325::setBiopsyCommandMode();
	PCB325::setKeyStepMode(false);
	sequence_executing = command_execution::COMMAND_IDLE;
	sequence_step = 0;
	seqid = 0;

	// Start the startup session	
	operatingTimer->Start();		
	resume();

	
}

void BiopsyStudy::open(void) {
	//SendNotifyMessageA(window, WINMSG_OPEN, 0, 0); // OPEN EVENT MESSAGE
	SendMessageA(window, WINMSG_OPEN, 0, 0); // OPEN EVENT MESSAGE
}

void BiopsyStudy::close(void) {
	SendMessageA(window, WINMSG_CLOSE, 0, 0); // CLOSE EVENT MESSAGE
	
}


void BiopsyStudy::evaluateXrayStatus(void) {
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


}

void BiopsyStudy::evaluateReadyWarnings(bool reset) {

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
	//bool patient_protection = (PCB302::getPatientProtection() == PCB302::PatientProtection::POSITIONED);
	bool patient_protection = true;

	/*
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
	*/

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

void BiopsyStudy::evaluateCompressorStatus(bool init) {

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
			
			labelThickness->Hide();
			labelForce->Hide();
		}
		else {
			cur_paddle = PADDLE_DETECTED;
			cur_thick = THICKNESS_ENABLED;
			if (force) cur_force = FORCE_ENABLED_COMPRESSED;
			else cur_force = FORCE_ENABLED_NOT_COMPRESSED;

			
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


void BiopsyStudy::evaluateCollimatorStatus(void) {
	

	// Sets the current lamp status
	if (PCB303::getPowerLightStatus() != BIOPSTATUS::Registers.collimator.light_on) {
		BIOPSTATUS::Registers.collimator.light_on = PCB303::getPowerLightStatus();
		if(BIOPSTATUS::Registers.collimator.light_on) lampButton->BackgroundImage = LAMP_ON_IMAGE;
		else lampButton->BackgroundImage = LAMP_OFF_IMAGE;
	}


	// Evaluates the maximum tube temperature	
	labelTubeData->Text = PCB303::getMaxTubePerc().ToString() + " %";

	if (PCB303::isTubeAlarm() != BIOPSTATUS::Registers.collimator.tube_alarm) {
	    BIOPSTATUS::Registers.collimator.tube_alarm = PCB303::isTubeAlarm();
		if (BIOPSTATUS::Registers.collimator.tube_alarm) tubeStatus->BackgroundImage = TUBE_TEMP_NOK_IMAGE;
		else tubeStatus->BackgroundImage = TUBE_TEMP_OK_IMAGE;
	}
}


void BiopsyStudy::evaluateDoorStatus(void) {

	if (PCB301::isClosedDoor() != BIOPSTATUS::Registers.closed_door) {
		BIOPSTATUS::Registers.closed_door = PCB301::isClosedDoor();

		if (BIOPSTATUS::Registers.closed_door ) {
			doorStatus->BackgroundImage = DOOR_CLOSED_IMAGE;
		}
		else {
			doorStatus->BackgroundImage = DOOR_OPEN_IMAGE;
		}
	}
	
}

void BiopsyStudy::evaluateSlideStatus(bool init) {
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

void BiopsyStudy::evaluateGridStatus(void) {
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

void BiopsyStudy::evaluateAwsComponentEvent(void) {
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

void BiopsyStudy::evaluateArmStatus(void) {
	float val = (float)ArmMotor::device->getCurrentPosition() / 100;
	int ival = (int)val;

	if ((val - (float)ival) >= 0.5) ival++;
	armLabel->Text = "ARM:" + ival.ToString();

	val = (float)TiltMotor::device->getCurrentPosition() / 100;
	ival = (int)val;

	if ((val - (float)ival) >= 0.5) ival++;
	tiltLabel->Text = "TILT:" + ival.ToString();


}

void BiopsyStudy::evaluatePointerStatus(void) {
	XLabel->Text = "X:" + PCB325::getX().ToString();
	YLabel->Text = "Y:" + PCB325::getY().ToString();
	ZLabel->Text = "Z:" + PCB325::getZ().ToString();
}



/// <summary>
/// 
/// </summary>
/// <param name="home"></param>
/// <param name="id"></param>
/// <returns></returns>
int BiopsyStudy::pointerHome(home_positions home, int id) {
	

	// Executing conditions
	if (!PCB325::isBiopsyConnected()) return -1;
	if (sequence_executing != command_execution::COMMAND_IDLE) return -2;

	// Gets the target for the given home position
	if (home == home_positions::HOME_CENTER) {
		xtarget = (unsigned short) System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_CENTER_POSITION)[BiopsyConfig::PARAM_HOME_CENTER_POSITION_X]);
		ytarget = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_CENTER_POSITION)[BiopsyConfig::PARAM_HOME_CENTER_POSITION_Y]);
		ztarget = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_CENTER_POSITION)[BiopsyConfig::PARAM_HOME_CENTER_POSITION_Z]);
		sequence_executing = command_execution::COMMAND_HOME_CENTER;
	}
	else if (home == home_positions::HOME_LEFT) {
		xtarget = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_LEFT_POSITION)[BiopsyConfig::PARAM_HOME_LEFT_POSITION_X]);
		ytarget = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_LEFT_POSITION)[BiopsyConfig::PARAM_HOME_LEFT_POSITION_Y]);
		ztarget = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_LEFT_POSITION)[BiopsyConfig::PARAM_HOME_LEFT_POSITION_Z]);
		sequence_executing = command_execution::COMMAND_HOME_LEFT;
	}
	else {
		xtarget = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_RIGHT_POSITION)[BiopsyConfig::PARAM_HOME_RIGHT_POSITION_X]);
		ytarget = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_RIGHT_POSITION)[BiopsyConfig::PARAM_HOME_RIGHT_POSITION_Y]);
		ztarget = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_RIGHT_POSITION)[BiopsyConfig::PARAM_HOME_RIGHT_POSITION_Z]);
		sequence_executing = command_execution::COMMAND_HOME_RIGHT;
	}

	// Verifies if the actual position is aready in target
	if (PCB325::isXtarget(xtarget) && PCB325::isYtarget(ytarget) && PCB325::isZtarget(ztarget)) {
		sequence_executing = command_execution::COMMAND_IDLE;
		return 0;
	}

	// Prepare for the command activation
	seqid = id;							
	sequence_step = 0;
	return 1;
	
}

int BiopsyStudy::pointerXYZ(unsigned short X, unsigned short Y, unsigned short Z, int id) {
	return 0;
}

void BiopsyStudy::home_workflow(void) {
	switch (sequence_step) {
	case 0:
		
		if (PCB325::isYUp()) {
			sequence_step = 100; // starts moving x-y-z to home
			break;
		}

		if (PCB325::getX() < 1290) {
			sequence_step = 10; // starts moving right
		}else sequence_step = 20; // starts moving left
		break;

	//____________________________________________________________
	case 10:  // starts moving right
		if (!PCB325::moveX(0)) {
			execution_error = 1;
			sequence_step = 200;
			break;
		}
		sequence_step++;
		break;
	case 11:		
		// wait for command completion
		if (PCB325::isPointerMoving()) break;
		
		// Terminated successfully
		if (PCB325::isPointerSuccessfullyMoved()) {
			sequence_step = 19;
			break;
		}

		// Error
		execution_error = 2;
		sequence_step = 200;
		break;

	case 19: 
		sequence_step = 30; // start requesting to y flip up
		break;

	//____________________________________________________________
	case 20:  // starts moving left
		if (!PCB325::moveX(PCB325::BIOPSY_MAX_X_POSITION)) {
			execution_error = 3;
			sequence_step = 200;
			break;
		}
		sequence_step++;
		break;
	case 21:
		// wait for command completion
		if (PCB325::isPointerMoving()) break;

		// Terminated successfully
		if (PCB325::isPointerSuccessfullyMoved()) {
			sequence_step = 29;
			break;
		}

		// Error
		execution_error = 4;
		sequence_step = 200;
		break;
	case 29:
		sequence_step = 30; // start requesting to y flip up
		break;
		
	//____________________________________________________________
	case 30:  // start requesting to y flip up
		sequence_step = 39;
		break;
	case 39:
		sequence_step = 100; // starts moving x-y-z to home
		break;

	//____________________________________________________________
	case 100: // starts moving x-y-z to home
		if (!PCB325::moveX(xtarget)) {
			execution_error = 5;
			sequence_step = 200;
			break;
		}
		sequence_step++;		
		break;
	case 101:
		// wait for command completion
		if (PCB325::isPointerMoving()) break;

		// Terminated successfully
		if (PCB325::isPointerSuccessfullyMoved()) {
			sequence_step++;
			break;
		}

		// Error
		execution_error = 6;
		sequence_step = 200;
		break;

	case 102:
		if (!PCB325::moveY(ytarget)) {
			execution_error = 7;
			sequence_step = 200;
			break;
		}
		sequence_step++;
		break;

	case 103:
		// wait for command completion
		if (PCB325::isPointerMoving()) break;

		// Terminated successfully
		if (PCB325::isPointerSuccessfullyMoved()) {
			sequence_step++;
			break;
		}

		// Error
		execution_error = 8;
		sequence_step = 200;
		break;

	case 104:
		if (!PCB325::moveZ(ztarget)) {
			execution_error = 9;
			sequence_step = 200;
			break;
		}
		sequence_step++;
		break;

	case 105:
		// wait for command completion
		if (PCB325::isPointerMoving()) break;

		// Terminated successfully
		if (PCB325::isPointerSuccessfullyMoved()) {
			sequence_step = 200;
			execution_error = 0;
			break;
		}

		// Error
		execution_error = 10;
		sequence_step = 200;
		break;

	case 200: // end sequence
		if (seqid) awsProtocol::EVENT_Executed(seqid, execution_error);
		sequence_executing = command_execution::COMMAND_IDLE;
		break;
	}
}

void BiopsyStudy::xyz_positioning_workflow(void) {

}

void BiopsyStudy::evaluatePointerActivations() {
	switch (sequence_executing) {
	case command_execution::COMMAND_HOME_CENTER: home_workflow(); break;
	case command_execution::COMMAND_HOME_LEFT: home_workflow(); break;
	case command_execution::COMMAND_HOME_RIGHT: home_workflow(); break;
	case command_execution::COMMAND_XYZ_POSITIONING: xyz_positioning_workflow(); break;
	}
}


void BiopsyStudy::operatingStatusManagement(void) {
	
	System::DateTime date;
	date = System::DateTime::Now;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	
	evaluateCompressorStatus(false);	
	evaluateCollimatorStatus();	
	evaluateDoorStatus();
	evaluateSlideStatus(false);	
	evaluateDigitDisplays();
	evaluateGridStatus();
	evaluateAwsComponentEvent();
	evaluateArmStatus();
	evaluatePointerStatus();
	evaluatePointerActivations();

	// This shall be posed at the end of the management
	evaluateReadyWarnings(false);
	evaluateXrayStatus(); 	
	evaluatePopupPanels();

	// Calls the periodical evenrt for tghe external class consumers
	periodical_event();

}

void BiopsyStudy::WndProc(System::Windows::Forms::Message% m)
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

	case (WINMSG_CLOSE): // on Close Event
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

void BiopsyStudy::errorButton_Click(System::Object^ sender, System::EventArgs^ e) {
	if(Notify::isError() || Notify::isWarning())	Notify::open_error(this);
}


void BiopsyStudy::ShiftSelection_Click(System::Object^ sender, System::EventArgs^ e) {
	
	// If a motorization is activated returns 
	if (Gantry::isMotorsActive()) return;

	((ConfirmationWindow^)pShiftConf)->setContentBackground(slideButton->BackgroundImage);
	((ConfirmationWindow^)pShiftConf)->open();
}



void BiopsyStudy::lampButton_Click(System::Object^ sender, System::EventArgs^ e) {
	PCB303::setCollimationLight(true);
	
}



void BiopsyStudy::onShiftConfirmOk(void) {

	if (SlideMotor::device->getCurrentPosition() < 500) SlideMotor::isoAutoPosition(9000);
	else SlideMotor::isoAutoPosition(0);
	
}
void BiopsyStudy::onShiftConfirmCanc(void) {
	
}



void BiopsyStudy::evaluateDigitDisplays(void) {

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

void BiopsyStudy::evaluatePopupPanels(void) {
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
		BIOPSTATUS::Registers.warning = false;
		BIOPSTATUS::Registers.info = false;
		if (!BIOPSTATUS::Registers.alarm) {
			alarmButton->BackgroundImage = ERROR_BUTTON_ON;
			BIOPSTATUS::Registers.alarm = true;
		}
	}
	else if (Notify::isWarning()) {
		BIOPSTATUS::Registers.alarm = false;
		BIOPSTATUS::Registers.info = false;
		if (!BIOPSTATUS::Registers.warning) {
			alarmButton->BackgroundImage = WARNING_BUTTON_ON;
			BIOPSTATUS::Registers.warning = true;
		}
	}
	else if (Notify::isInfo()) {
		BIOPSTATUS::Registers.alarm = false;
		BIOPSTATUS::Registers.warning = false;
		if (!BIOPSTATUS::Registers.info) {
			alarmButton->BackgroundImage = INFO_BUTTON_ON;
			BIOPSTATUS::Registers.info = true;
		}
	}
	else {
		if ((BIOPSTATUS::Registers.warning) || (BIOPSTATUS::Registers.alarm)) {
			BIOPSTATUS::Registers.warning = false;
			BIOPSTATUS::Registers.alarm = false;
			BIOPSTATUS::Registers.info = false;
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