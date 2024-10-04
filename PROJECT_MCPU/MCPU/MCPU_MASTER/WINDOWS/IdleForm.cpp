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
#include "PCB315.h"
#include "PCB326.h"
#include "VerticalMotor.h"
#include "ArmMotor.h"
#include "BodyMotor.h"
#include "SlideMotor.h"
#include "TiltMotor.h"

#include "AWS/awsProtocol.h"
#include "../gantry_global_status.h"


// Main Panel Definition
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


void IdleForm::formInitialization(void) {

	// Initialize the position of the form
	this->Left = Gantry::monitor_X0;
	this->Top = Gantry::monitor_Y0;

	mainPanel->SetBounds(0, 0, 600, 1024);
	mainPanel->BackgroundImage = BACKGROUND;

	this->xrayMode->BackgroundImage = XRAY_MODE;
	this->xrayMode->Hide();

	// Initialize the Installation name
	labelInstallation->Text = SystemConfig::Configuration->getParam(SystemConfig::PARAM_INSTALLATION_NAME)[SystemConfig::PARAM_INSTALLATION_NAME_TOP];


	// Error Button
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


	idleTimer = gcnew System::Timers::Timer(100);
	idleTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &IdleForm::onIdleTimeout);
	idleTimer->Stop();

	this->Hide();
	open_status = false;
}

void IdleForm::initIdleStatus(void) {
	System::DateTime date;
	date = System::DateTime::Now;

	Notify::clrInstant();

	labelDate->Text = date.Day + ":" + date.Month + ":" + date.Year;
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

	
	
	// Handle the Powerdown
	IDLESTATUS::Registers.powerdown = PCB301::getPowerdown();

	// Handle The Battery status
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

	IDLESTATUS::Registers.aws_connected = false;
	this->awsConnected->BackgroundImage = AWS_CONNECTED;
	this->awsConnected->Hide();

	IDLESTATUS::Registers.peripherals_connected = false;
	this->peripheralsConnected->BackgroundImage = PERIPHERALS_CONNECTED;
	this->peripheralsConnected->Hide();

	IDLESTATUS::Registers.closed_door = PCB301::isClosedDoor();
	this->doorClosed->BackgroundImage = DOOR_CLOSED;
	if (!IDLESTATUS::Registers.closed_door) this->doorClosed->Hide();
	else this->doorClosed->Show();


	IDLESTATUS::Registers.tube.anode = 0;
	IDLESTATUS::Registers.tube.stator = 0;
	IDLESTATUS::Registers.tube.bulb = 0;
	IDLESTATUS::Registers.tube.alarm = false;

	labelTubeData->Text = "0 %";
	labelTubeData->BackColor = Color::Transparent;

	this->tubeTempOk->BackgroundImage = TUBE_TEMP_OK;
	this->tubeTempOk->BackColor = Color::Transparent;
	this->tubeTempOk->Show();
	

	// Sets the current collimation to OPEN mode
	PCB303::setOpenCollimationMode();

	// Sets the current Filter selector to manual mode
	PCB315::setFilterManualMode(PCB315::filterMaterialCodes::FILTER_DEFAULT);

	// Activates the motors
	PCB301::setMotorPowerSupply(true);
	PCB301::setMotorSwitch(true);

	// Activate the Rotation Idle manual modes
	Gantry::setManualRotationMode(Gantry::manual_rotation_options::GANTRY_MANUAL_ROTATION_DISABLED);

	// Activates the compressor
	PCB301::SetCompressorEna(true);
	
	// XRAY mode setting
	if (Exposures::isSimulatorMode()) {
		this->xrayMode->Hide();
	}
	else {
		if (Exposures::isServiceToolConnected()) this->xrayMode->BackgroundImage = SERVICE_TOOL_MODE;
		else this->xrayMode->BackgroundImage = XRAY_MODE;
		this->xrayMode->Show();
	}
	
	// Activate the Automatic Potter Grid with the Out-Field position and unsync the generator signals
	PCB304::setAutoGridOutField();
	PCB304::syncGeneratorOff();
	
	// Start the startup session	
	idleTimer->Start();	

	


}

void IdleForm::open(void) {
	SendMessageA(window , WINMSG_OPEN, 0, 0); // OPEN EVENT MESSAGE
}

void IdleForm::close(void) {
	SendMessageA(window, WINMSG_CLOSE, 0, 0); // CLOSE EVENT MESSAGE	
}


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

void IdleForm::idleStatusManagement(void) {
	static int xray_mode_status = 0;

	System::DateTime date;
	date = System::DateTime::Now;
	
	labelTime->Text = date.Hour + ":" + date.Minute + ":" + date.Second;

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

	// Handles the Door Closed status
	if (IDLESTATUS::Registers.closed_door != PCB301::isClosedDoor())
	{
		IDLESTATUS::Registers.closed_door = PCB301::isClosedDoor();
		if (!IDLESTATUS::Registers.closed_door ) this->doorClosed->Hide();
		else this->doorClosed->Show();				
	}

	// Handles the AWS connection status
	if (awsProtocol::isConnected() != IDLESTATUS::Registers.aws_connected) {
		IDLESTATUS::Registers.aws_connected = awsProtocol::isConnected();

		if(IDLESTATUS::Registers.aws_connected) this->awsConnected->Show();
		else this->awsConnected->Hide();
	}

	// Handles the Device connection status
	bool peripherals_connected = false;
	if (
		(!PCB301::device->isCommunicationError()) &&
		(!PCB302::device->isCommunicationError()) &&
		(!PCB303::device->isCommunicationError()) &&
		(!PCB304::device->isCommunicationError()) &&
		(!PCB315::device->isCommunicationError())&&
		(!PCB326::device->isCommunicationError())
		)	peripherals_connected = true;

	if (IDLESTATUS::Registers.peripherals_connected != peripherals_connected) {
		IDLESTATUS::Registers.peripherals_connected = peripherals_connected;
		if(IDLESTATUS::Registers.peripherals_connected) this->peripheralsConnected->Show();
		else this->peripheralsConnected->Hide();
	}

	// Evaluates the maximum tube temperature
	int val = PCB315::getAnode();
	if (PCB315::getBulb() > val) val = PCB315::getBulb();
	if (PCB315::getStator() > val) val = PCB315::getStator();
	labelTubeData->Text = val.ToString() + " %";

	// Sets the background color based on the temperature error condition
	if (PCB315::isTubeAlarm() != IDLESTATUS::Registers.tube.alarm) {
		IDLESTATUS::Registers.tube.alarm = PCB315::isTubeAlarm();
		if (IDLESTATUS::Registers.tube.alarm) tubeTempOk->BackgroundImage = TUBE_TEMP_NOK;
		else tubeTempOk->BackgroundImage = TUBE_TEMP_OK;
	}

	// Error Button
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

	// XRAY mode setting
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

	// Popup panels at the end of the timer thread:
	// if a panel should be open this thread stops to the ShowDialog() 
	evaluatePopupPanels();

}

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

void IdleForm::errorButton_Click(System::Object^ sender, System::EventArgs^ e) {
	Notify::open_error(this);
}
System::Void IdleForm::serviceButton_Click(System::Object^ sender, System::EventArgs^ e) {
	Gantry::setService();
}

void IdleForm::onPowerOffOkCallback(void) {

	// Request the AWS to power off the system
	awsProtocol::EVENT_Poweroff();

	if (!Gantry::isOperatingNormal()) {
		LogClass::logInFile("APPLICATION DEMO POWER OFF COMMAND");
		Application::Exit();
		return;
	}

}
System::Void IdleForm::powerOff_Click(System::Object^ sender, System::EventArgs^ e) {
	System::String^ confInfoTitle = "[" + Notify::TranslateNumber(Notify::messages::INFO_POWER_OFF_REQUEST_ACTIVATION) + "] " + Notify::TranslateTitle(Notify::messages::INFO_POWER_OFF_REQUEST_ACTIVATION);
	System::String^ confInfoContent = Notify::TranslateContent(Notify::messages::INFO_POWER_OFF_REQUEST_ACTIVATION);
	ConfirmationWindow^ pConfirmation = gcnew ConfirmationWindow(this, ConfirmationWindow::InfoType::INF_WIN, confInfoTitle, confInfoContent);
	
	pConfirmation->button_ok_event += gcnew ConfirmationWindow::delegate_button_callback(this, &IdleForm::onPowerOffOkCallback);
	pConfirmation->open();
}