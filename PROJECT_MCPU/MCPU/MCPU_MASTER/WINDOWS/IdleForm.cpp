#include "../AWS/awsProtocol.h"
#include "ConfirmationWindow.h"
#include "idleForm.h"
#include "ValuePopup.h"
#include "operatingForm.h"
#include "ServiceForm.h"
#include "ConfigurationFiles.h"
#include "Notify.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB304.h"
#include "PCB325.h"
#include "PCB326.h"
#include "VerticalMotor.h"
#include "ArmMotor.h"
#include "BodyMotor.h"
#include "SlideMotor.h"
#include "TiltMotor.h"

#include "AWS/awsProtocol.h"
#include "../gantry_global_status.h"


// Main Panel Definition
#define BIOPSY Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\Biopsy.PNG")
#define BACKGROUND Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\IdleFormBackground.PNG")
#define XRAY_MODE Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\XrayMode.PNG")
#define SERVICE_TOOL_MODE Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\ServiceToolMode.PNG")
#define BATTERY_CONNECTED Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\Battery\\BatteryConnected.PNG")
#define BATTERY_CONNECTED_LOW Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\Battery\\BatteryConnectedLow.PNG")
#define BATTERY_POWERDOWN Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\Battery\\BatteryPowerdownFull.PNG")
#define AWS_CONNECTED Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\AwsConnected.PNG")
#define PERIPHERALS_CONNECTED Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\PeripheralsConnected.PNG")
#define DOOR_CLOSED Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\DoorClosed.PNG")
#define TUBE_TEMP_OK Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\tubeTempOk.PNG")
#define TUBE_TEMP_NOK Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\tubeTempNok.PNG")
#define ERRORPANEL_ERRORICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\error_160x145.PNG")
#define INFO_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\InfoIcon.PNG")
#define WARNING_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\WarningOn.PNG")
#define ERROR_BUTTON_ON Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\AlarmOn.PNG")
#define ERROR_BUTTON_OFF Image::FromFile(Gantry::applicationResourcePath + "IdleForm\\AlarmOff.PNG")

#define COMPRESSING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\COMPRESSING_ICON.PNG")
#define ARM_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\ARM_MOTOR_ICON.PNG")
#define BODY_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\BODY_MOTOR_ICON.PNG")
#define VERTICAL_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\VERTICAL_MOTOR_ICON.PNG")
#define SLIDE_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\SLIDE_MOTOR_ICON.PNG")
#define TILT_EXECUTING_ICON Image::FromFile(Gantry::applicationResourcePath + "Icons\\TILT_MOTOR_ICON.PNG")

namespace IDLESTATUS {
	typedef struct {

		// Battery Status local variables
		bool battena;
		bool battery_alarm;
		unsigned char vbatt1;
		unsigned char vbatt2;

	}batteryStatus;

	typedef struct {

		unsigned char anode;
		unsigned char stator;
		unsigned char bulb;
		bool alarm;
		

	}tubeStatus;

	static struct {
		batteryStatus battery;
		tubeStatus	tube;

		bool powerdown;
		bool closed_door;
		bool aws_connected;
		bool peripherals_connected;
		
		bool alarm;
		bool warning;
		bool info;
	}Registers;
};

/// <summary>
/// \ingroup IDLEIMPL
/// 
/// This is the Form initialization callback.
/// 
/// </summary>
/// 
/// This routine initializes the object of the form:
/// 
/// <param name=""></param>
void IdleForm::formInitialization(void) {

	/// The geometry of the form is initialized at the X,Y offeset set by the Gantry at the program beginning 
	this->Left = Gantry::monitor_X0;
	this->Top = Gantry::monitor_Y0;
	mainPanel->SetBounds(0, 0, 600, 1024);
	mainPanel->BackgroundImage = BACKGROUND;

	
	/// The name of the istallation is set in the top of the window:
	/// The installation name is stored into the SystemIni.cnf configuration file \ref SystemConfig
	labelInstallation->Text = SystemConfig::Configuration->getParam(SystemConfig::PARAM_INSTALLATION_NAME)[SystemConfig::PARAM_INSTALLATION_NAME_TOP];

	/// The X-RAY icon is hidden 	
	this->xrayMode->Hide();

	/// The active message button is updated
	IDLESTATUS::Registers.alarm = false;
	IDLESTATUS::Registers.warning = false;
	IDLESTATUS::Registers.info = false;
	errorButton->BackgroundImage = ERROR_BUTTON_OFF;
	errorButton->Show();

	// Service Button
	serviceButton->BackColor = Color::Transparent;

	// PowerOff Button
	powerOff->BackColor = Color::Transparent;
	powerOff->Show();


	/// The Idle perating Mode window timer is initialized with 100ms schedule.
	idleTimer = gcnew System::Timers::Timer(100);
	idleTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &IdleForm::onIdleTimeout);
	idleTimer->Stop();

	/// The window form is finally Hided.
	this->Hide();
	open_status = false;
}

/// <summary>
/// This function is called when the Idle Operating Status is activated
/// </summary>
/// 
/// The function initializes the content of the GUI at the current 
/// gantry status.
/// 
/// The function sets the Gantry device at the expected status:
/// 
/// + The compressor device is disabled;
/// + All the Motors are disabled;
/// + The Format collimation is set to OPEN;
/// + The Filter selector is set on the Pb to protect the Tube hole;
/// + The Mirror is set to Home position and the light switched off;
/// + The manual key buttons are disabled;
/// + The Grid device is set to Out Of Field; 
/// 
/// <param name=""></param>
void IdleForm::initIdleStatus(void) {

	/// The system date and time is initialized;
	/// \note
	/// the date and time is always showed on the bottom of the window
	System::DateTime date;
	date = System::DateTime::Now;

	Notify::clrInstant();
	labelDate->Text = date.Day + ":" + date.Month + ":" + date.Year;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	
	
	/// The Powerdown and battery status is acquired and the related 
	/// icon is displayed on the GUI
	IDLESTATUS::Registers.powerdown = PCB301::getPowerdown();

	/// The Battery status is updated
	IDLESTATUS::Registers.battery.battena = PCB301::getBatteryEna();
	IDLESTATUS::Registers.battery.battery_alarm = PCB301::getBatteryAlarm();
	IDLESTATUS::Registers.battery.vbatt1 = PCB301::getVoltageBatt1();
	IDLESTATUS::Registers.battery.vbatt2 = PCB301::getVoltageBatt2();

	// Set the proper status to the Battery Icon
	if (IDLESTATUS::Registers.powerdown) {
		this->batteryConnected->Show();
		this->batteryConnected->BackgroundImage = BATTERY_POWERDOWN;
	}
	else if (IDLESTATUS::Registers.battery.battena) {
		if (IDLESTATUS::Registers.battery.battery_alarm) this->batteryConnected->BackgroundImage = BATTERY_CONNECTED_LOW;
		else this->batteryConnected->BackgroundImage = BATTERY_CONNECTED;
		this->batteryConnected->Show();
	}
	else {
		this->batteryConnected->Hide();
	}

	/// The AWS connection status is acquired and the related icon is displayed
	IDLESTATUS::Registers.aws_connected = false;
	this->awsConnected->BackgroundImage = AWS_CONNECTED;
	this->awsConnected->Hide();

	/// The Devices connection status is acquired and the related icon is displayed
	IDLESTATUS::Registers.peripherals_connected = false;
	this->peripheralsConnected->BackgroundImage = PERIPHERALS_CONNECTED;
	this->peripheralsConnected->Hide();

	/// The room's Door status is acquired and the related icon is displayed
	IDLESTATUS::Registers.closed_door = PCB301::isClosedDoor();
	this->doorClosed->BackgroundImage = DOOR_CLOSED;
	if (!IDLESTATUS::Registers.closed_door) this->doorClosed->Hide();
	else this->doorClosed->Show();

	/// The Tube temperature is updated and the related icon is displayed
	IDLESTATUS::Registers.tube.anode = 0;
	IDLESTATUS::Registers.tube.stator = 0;
	IDLESTATUS::Registers.tube.bulb = 0;
	IDLESTATUS::Registers.tube.alarm = false;

	labelTubeData->Text = "0 %";
	labelTubeData->BackColor = Color::Transparent;

	this->tubeTempOk->BackgroundImage = TUBE_TEMP_OK;
	this->tubeTempOk->BackColor = Color::Transparent;
	this->tubeTempOk->Show();
	
	///\todo 
	/// Per proteggere il collimatore forse sarebbe meglio chiudere le lame
	/// 
	/// The collimation format is set to OPEN to protect the blades inside the collimator
	PCB303::setOpenCollimationMode();

	/// \todo
	/// Il filtro in Idle dovrebbe essereimpostato su Piombo per proteggere la bocca del tubo
	/// Deve essere inseritoil piombo come filtro selezionabile
	/// 
	/// The filter is set to "lead" in order to protect the tube output.
	PCB303::selectFilter(PCB303::filter_index::FILTER_RH);

	/// The Mirror is set to Home and the collimation light switched Off
	PCB303::setCollimationLight(false);

	/// \todo
	/// Verificare se in Idle Mode si possono spegnere i motori
	/// 
	/// The motor power supply and the power switch are switched Off for safety.
	PCB301::setMotorPowerSupply(true);
	PCB301::setMotorSwitch(true);

	/// The Manual Buttons are disabled (keyboard led switchd off)
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED);

	/// \todo
	/// Il compressore deve essere disabilitato in Idle
	/// Al momento è lasciato attivo per facilitare lo sviluppo e il debug
	/// 
	/// 
	/// The compression is disabled
	PCB301::SetCompressorEna(true);
	
	/// \todo 
	/// Aggiungere il simbolo di modalità simulazione per completare il set di stati visualizzabili
	/// 
	/// 
	/// The actual Running mode is updated with the following priority:
	/// + If the biopsy is present, the biopsy icon is displayed;
	/// + If the application is running in simulation mode, the simulation icon is displayed;
	/// + If the application is running in demo mode, the demo icon is displayed;
	/// + If the application is running in normal mode and the generator service tool is active, the service tool icon is displayed;
	/// + If the application is running in Normal mode the X-RAy simbol is displayed;
	if (PCB325::isBiopsyConnected()) {
		this->xrayMode->BackgroundImage = BIOPSY;
		this->xrayMode->Show();
	}
	else {
		if (Exposures::isSimulatorMode()) {
			this->xrayMode->Hide();
		}
		else {
			if (Exposures::isServiceToolConnected()) this->xrayMode->BackgroundImage = SERVICE_TOOL_MODE;
			else this->xrayMode->BackgroundImage = XRAY_MODE;
			this->xrayMode->Show();
		}

	}
	
	/// The grid device is activated with an In/Out test to initialize the grid position 
	/// and to left the grid out of field.
	PCB304::resetErrorCount();
	PCB304::setAutoGridInField();
	PCB304::syncGeneratorOff();
	
	/// The Idle Operatig Mode window timer is started with 100ms schedule
	idleTimer->Start();	


}

/// <summary>
/// This is the function to open the Idle Form
/// </summary>
/// 
/// The function can be called by different threads then the main thread
/// where the form resides. 
/// 
/// Beacuse the window's items can be handled only in the same thread of the window form,
/// this function put a window message on the window queue so that it will be handled into the main thread.
/// 
/// \warning
/// When the function returns, an unpredictable delay time can happen before to get the form operating.
/// 
/// <param name=""></param>
void IdleForm::open(void) {
	SendMessageA(window , WINMSG_OPEN, 0, 0); // OPEN EVENT MESSAGE
}

/// <summary>
/// This is the function to close the Idle Form
/// </summary>
/// 
/// The function can be called by different threads then the main thread
/// where the form resides. 
/// 
/// Beacuse the window's items can be handled only in the same thread of the window form,
/// this function put a window message on the window queue so that it will be handled into the main thread.
/// 
/// \warning
/// When the function returns, an unpredictable delay time can happen before to get the form disappeared.
/// 
/// <param name=""></param>
void IdleForm::close(void) {
	SendMessageA(window, WINMSG_CLOSE, 0, 0); // CLOSE EVENT MESSAGE	
}

/// <summary>
/// This function opens if necessary a diaog window to show and track variable events .
/// </summary>
/// 
/// There are a set of vents that should require to be visualized on the 
/// application screen:
/// 
/// + Instant messages: the active messages that needs to be displayed and consumed istantly;
/// + The position of a running motor: if a motor is executing an activation, the current position is dinamically updated on the screen;
/// 
/// \note
/// Even if the motor are disabled in Idle mode, however the motors may be running during the 
/// operating mode transition.
/// 
/// \remarks
/// + The first event that gains the dialog, keeps the control of the dialog
/// for 2 seconds more since the event completion.
/// + If the system message window is open, the dialog remain hided until
/// the message window disappear. 
/// 
/// 
/// <param name=""></param>
void IdleForm::evaluatePopupPanels(void) {
	#define TMO 20
	static bool compression = false;
	static bool arm = false;
	static bool body = false;
	static bool slide = false;
	static bool vertical = false;
	static bool tilt = false;
	static int  timer = 0;

	// With a panel already open do not continue;
	if (Notify::isInstantOpen() || Notify::isErrorOpen()) {
		if (Gantry::getValuePopupWindow()->open_status) {
			Gantry::getValuePopupWindow()->close();
			compression = false;
			arm = false;
			body = false;
			slide = false;
			vertical = false;
			tilt = false;
			timer = 0;
		}
		return;
	}


	if (ArmMotor::device->isRunning()) {
		timer = TMO;
		if (!arm) {
			compression = false;
			arm = true;
			body = false;
			vertical = false;
			slide = false;
			tilt = false;
			if (Gantry::getValuePopupWindow()->open_status) Gantry::getValuePopupWindow()->retitle(ARM_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_ARM_ACTIVATED), "(°)");
			else Gantry::getValuePopupWindow()->open(this, ARM_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_ARM_ACTIVATED), "(°)");

		}

		// Set the value to the current compression
		int position = ArmMotor::device->getCurrentPosition() / 100;
		Gantry::getValuePopupWindow()->content(position.ToString());
		return;

	}
	else arm = false;

	if (BodyMotor::device->isRunning()) {
		timer = TMO;
		if (!body) {
			compression = false;
			arm = false;
			body = true;
			vertical = false;
			slide = false;
			tilt = false;
			if (Gantry::getValuePopupWindow()->open_status) Gantry::getValuePopupWindow()->retitle(BODY_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_BODY_ACTIVATED), "(°)");
			else Gantry::getValuePopupWindow()->open(this, BODY_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_BODY_ACTIVATED), "(°)");
		}

		// Set the value to the current compression
		float position = (float)BodyMotor::device->getCurrentPosition() / 10;
		Gantry::getValuePopupWindow()->content(position.ToString());
		return;
	}
	else body = false;

	
	if (VerticalMotor::device->isRunning()) {
		timer = TMO;
		if (!vertical) {
			compression = false;
			arm = false;
			body = false;
			vertical = true;
			slide = false;
			tilt = false;
			if (Gantry::getValuePopupWindow()->open_status) Gantry::getValuePopupWindow()->retitle(VERTICAL_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_VERTICAL_ACTIVATED), "(mm)");
			else Gantry::getValuePopupWindow()->open(this, VERTICAL_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_VERTICAL_ACTIVATED), "(mm)");
		}

		// Set the value to the current compression
		int position = (int)VerticalMotor::device->getCurrentPosition();
		Gantry::getValuePopupWindow()->content(position.ToString());
		return;
	}
	else vertical = false;
	

	if (SlideMotor::device->isRunning()) {
		timer = TMO;
		if (!slide) {
			compression = false;
			arm = false;
			body = false;
			vertical = false;
			slide = true;
			tilt = false;
			if (Gantry::getValuePopupWindow()->open_status) Gantry::getValuePopupWindow()->retitle(SLIDE_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_SLIDE_ACTIVATED), "(°)");
			else Gantry::getValuePopupWindow()->open(this, SLIDE_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_SLIDE_ACTIVATED), "(°)");
		}

		// Set the value to the current compression
		int position = (int)SlideMotor::device->getCurrentPosition() / 100;
		Gantry::getValuePopupWindow()->content(position.ToString());
		return;
	}
	else slide = false;

	if (TiltMotor::device->isRunning()) {
		timer = TMO;
		if (!tilt) {
			compression = false;
			arm = false;
			body = false;
			vertical = false;
			slide = false;
			tilt = true;
			if (Gantry::getValuePopupWindow()->open_status) Gantry::getValuePopupWindow()->retitle(TILT_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_TILT_ACTIVATED), "(.01°)");
			else Gantry::getValuePopupWindow()->open(this, TILT_EXECUTING_ICON, Notify::TranslateLabel(Notify::messages::LABEL_TILT_ACTIVATED), "(°)");
		}

		// Set the value to the current compression
		int position = (int)TiltMotor::device->getCurrentPosition() / 100;
		Gantry::getValuePopupWindow()->content(position.ToString());
		return;
	}
	else tilt = false;

	// Keeps the popup alive for extra time
	if (timer) {
		timer--;
		if(!timer) Gantry::getValuePopupWindow()->close();
		return;
	}

	if (Notify::isInstant()) Notify::open_instant(this);

}

/// <summary>
/// This is the main scheduled routine of the Idle Status management.
/// </summary>
/// 
/// This function is periodically called by the Idle Status timer, scheduled
/// every 100ms. (See the formInitializtion() )
/// 
/// The function updates the current device data that are displayed 
/// on the GUI.
/// 
/// In particolar:
/// 
/// <param name=""></param>
void IdleForm::idleStatusManagement(void) {
	static int xray_mode_status = 0;

	/// The date and the time is updated on the bottom of the windows;
	System::DateTime date;
	date = System::DateTime::Now;
	
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	/// The current powerdown status and the battery status is updated;
	bool powerdown = PCB301::getPowerdown();
	bool powerstatus_chg = false;
	IDLESTATUS::batteryStatus battery;
	
	// Handle The Battery status
	battery.battena = PCB301::getBatteryEna();
	battery.battery_alarm = PCB301::getBatteryAlarm();
	battery.vbatt1 = PCB301::getVoltageBatt1();
	battery.vbatt2 = PCB301::getVoltageBatt2();
	
	if (IDLESTATUS::Registers.powerdown != powerdown) {
		IDLESTATUS::Registers.powerdown = powerdown;
		powerstatus_chg = true;
	}

	if ((IDLESTATUS::Registers.battery.battena != battery.battena) ||
		(IDLESTATUS::Registers.battery.battery_alarm != battery.battery_alarm) ) {
		IDLESTATUS::Registers.battery = battery;
		powerstatus_chg = true;
	}

	if (powerstatus_chg) {
		if (powerdown) {
			this->batteryConnected->Show();
			this->batteryConnected->BackgroundImage = BATTERY_POWERDOWN;
		}
		else if (battery.battena) {
			if (battery.battery_alarm) this->batteryConnected->BackgroundImage = BATTERY_CONNECTED_LOW;
			else this->batteryConnected->BackgroundImage = BATTERY_CONNECTED;
			this->batteryConnected->Show();
		}else 
		{
			this->batteryConnected->Hide();
		}
	}

	/// The room door status icon is updated;
	if (IDLESTATUS::Registers.closed_door != PCB301::isClosedDoor())
	{
		IDLESTATUS::Registers.closed_door = PCB301::isClosedDoor();
		if (!IDLESTATUS::Registers.closed_door ) this->doorClosed->Hide();
		else this->doorClosed->Show();				
	}

	/// The status of the AWS connection is displayed
	if (awsProtocol::isConnected() != IDLESTATUS::Registers.aws_connected) {
		IDLESTATUS::Registers.aws_connected = awsProtocol::isConnected();

		if(IDLESTATUS::Registers.aws_connected) this->awsConnected->Show();
		else this->awsConnected->Hide();
	}

	/// The status of the devices connection is displayed;
	bool peripherals_connected = false;
	if (
		(!PCB301::device->isCommunicationError()) &&
		(!PCB302::device->isCommunicationError()) &&
		(!PCB303::device->isCommunicationError()) &&
		(!PCB304::device->isCommunicationError()) &&		
		(!PCB326::device->isCommunicationError())
		)	peripherals_connected = true;

	if (IDLESTATUS::Registers.peripherals_connected != peripherals_connected) {
		IDLESTATUS::Registers.peripherals_connected = peripherals_connected;
		if(IDLESTATUS::Registers.peripherals_connected) this->peripheralsConnected->Show();
		else this->peripheralsConnected->Hide();
	}

	/// The status of the Tube temperature is displayed;
	labelTubeData->Text = PCB303::getMaxTubePerc().ToString() + " %";

	// Sets the background color based on the temperature error condition
	if (PCB303::isTubeAlarm() != IDLESTATUS::Registers.tube.alarm) {
		IDLESTATUS::Registers.tube.alarm = PCB303::isTubeAlarm();
		if (IDLESTATUS::Registers.tube.alarm) tubeTempOk->BackgroundImage = TUBE_TEMP_NOK;
		else tubeTempOk->BackgroundImage = TUBE_TEMP_OK;
	}

	/// The system message display button is updated with the proper icon (Error, Warning, Info)
	if (Notify::isError()) {
		IDLESTATUS::Registers.warning = false;
		IDLESTATUS::Registers.info = false;
		if (!IDLESTATUS::Registers.alarm) {
			errorButton->BackgroundImage = ERROR_BUTTON_ON;
			IDLESTATUS::Registers.alarm = true;
			errorButton->Show();
		}
	}
	else if (Notify::isWarning()) {
		IDLESTATUS::Registers.alarm = false;
		IDLESTATUS::Registers.info = false;
		if (!IDLESTATUS::Registers.warning) {
			errorButton->BackgroundImage = WARNING_BUTTON_ON;
			IDLESTATUS::Registers.warning = true;
			errorButton->Show();
		}
	}
	else if (Notify::isInfo()) {
		IDLESTATUS::Registers.alarm = false;
		IDLESTATUS::Registers.warning = false;
		if (!IDLESTATUS::Registers.info) {
			errorButton->BackgroundImage = INFO_BUTTON_ON;
			IDLESTATUS::Registers.info = true;
			errorButton->Show();
		}
	}
	else {
		if ((IDLESTATUS::Registers.warning) || (IDLESTATUS::Registers.alarm) || (IDLESTATUS::Registers.info)) {
			IDLESTATUS::Registers.warning = false;
			IDLESTATUS::Registers.alarm = false;
			IDLESTATUS::Registers.info = false;
			errorButton->BackgroundImage = ERROR_BUTTON_OFF;
			errorButton->Show();
		}

	}

	/// The current exposure mode icon is updated
	if (!Exposures::isSimulatorMode()) {
		if (Exposures::isServiceToolConnected()) {
			if (xray_mode_status != 2) {
				Notify::activate(Notify::messages::WARNING_GENERATOR_SERVICE_MODE);
				this->xrayMode->BackgroundImage = SERVICE_TOOL_MODE;
				xray_mode_status = 2;
			}
		}
		else {
			if (xray_mode_status != 1) {
				Notify::deactivate(Notify::messages::WARNING_GENERATOR_SERVICE_MODE);
				this->xrayMode->BackgroundImage = XRAY_MODE;
				xray_mode_status = 1;
			}
		}
	}

	/// The popup event dialog activation is evaluated.
	evaluatePopupPanels();

}

/// <summary>
/// This is the callback of the Window Message Handler.
/// </summary>
/// 
/// The window message handler is used to handle 
/// calling request from other threads then the main thread for the following 
/// pourposes:
/// + The Idle Timer timeout event: this event calls the idleStatusManagement() ;
/// + The Open form request: this is a message coming from a thread that would open the Idle Status;
/// + The Close form request: this is a message coming from a thread that would close the Idle Status;
/// <param name="m">This is the message code </param>
void IdleForm::WndProc(System::Windows::Forms::Message% m)
{
	switch (m.Msg) {

	case (WINMSG_TIMER): // onIdleTimeout
		
		idleStatusManagement();
		break;
	case (WINMSG_OPEN): // on Open Event
		if (open_status) return;
		open_status = true;
		initIdleStatus();
		this->Show();
		break;

	case (WINMSG_CLOSE): // on Close Event
		if (!open_status) return;
		open_status = false;
		idleTimer->Stop();
		Notify::close_error();
		Notify::close_instant();
		this->Hide();
		break;
	}

	Form::WndProc(m);
}

/// <summary>
/// This is the callback of the graphic button that request to open the Messaqe dialog window.
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void IdleForm::errorButton_Click(System::Object^ sender, System::EventArgs^ e) {
	Notify::open_error(this);
}

/// <summary>
/// This is the callback of the graphic button that request to open the Service Operating form
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
System::Void IdleForm::serviceButton_Click(System::Object^ sender, System::EventArgs^ e) {
	Gantry::setService();
}

/// <summary>
/// This is the callback of the dialog for the power off confirmation event
/// </summary>
/// 
/// If the operator confirm the power off request action,
/// the power off request event is routed to the AWS for the confirmation.
/// 
/// + if the application is runnin in Normal mode,
/// the power off request is routed to the AWS software.
/// + if the application is runnin in Demo or Simulation mode,
/// only the Gantry software quits, the Gantry devices and the AWS software remain running.
/// 
/// \remarks
/// The system power off switches off the the whole Gantry,
/// activating the PC shutdown and finally switching off the power supply.
/// In demo or simulation this feature is disabled and only the Gantry application is closed.
/// 
/// 
/// <param name=""></param>
void IdleForm::onPowerOffOkCallback(void) {
	
	LogClass::logInFile("APPLICATION IMQ POWER OFF COMMAND");
	Application::Exit();
	return;

	// Request the AWS to power off the system
	awsProtocol::EVENT_Poweroff();

	if (!Gantry::isOperatingNormal()) {
		LogClass::logInFile("APPLICATION DEMO POWER OFF COMMAND");
		Application::Exit();
		return;
	}

}

/// <summary>
/// This is the callback of the Power Off button on the GUI.
/// </summary>
/// 
/// When the button is pressed a confirmation dialog is showed on the screen.
/// 
/// <param name="sender"></param>
/// <param name="e"></param>
/// <returns></returns>
System::Void IdleForm::powerOff_Click(System::Object^ sender, System::EventArgs^ e) {
	System::String^ confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_POWER_OFF_REQUEST_ACTIVATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_POWER_OFF_REQUEST_ACTIVATION);
	System::String^ confInfoContent = Notify::TranslateContent(Notify::messages::INFO_POWER_OFF_REQUEST_ACTIVATION);
	ConfirmationWindow^ pConfirmation = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
	
	pConfirmation->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &IdleForm::onPowerOffOkCallback);
	pConfirmation->open();
}