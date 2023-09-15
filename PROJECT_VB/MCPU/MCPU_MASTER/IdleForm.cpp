#include "pch.h"
#include "IdleForm.h"

#define BACKGROUND Image::FromFile("GUI\\IdleForm\\IdleFormBackground.PNG")
#define XRAY_MODE Image::FromFile("GUI\\IdleForm\\XrayMode.PNG")
#define BATTERY_CONNECTED Image::FromFile("GUI\\IdleForm\\Battery\\BatteryConnected.PNG")
#define BATTERY_POWERDOWN Image::FromFile("GUI\\IdleForm\\Battery\\BatteryPowerdownFull.PNG")
#define AWS_CONNECTED Image::FromFile("GUI\\IdleForm\\AwsConnected.PNG")
#define PERIPHERALS_CONNECTED Image::FromFile("GUI\\IdleForm\\PeripheralsConnected.PNG")
#define DOOR_CLOSED Image::FromFile("GUI\\IdleForm\\DoorClosed.PNG")
#define TUBE_TEMP_OK Image::FromFile("GUI\\IdleForm\\TubeTempOk.PNG")

void IdleForm::formInitialization(void) {

	// Initialize the position of the form
	this->Left = GlobalObjects::monitor_X0;
	this->Top = GlobalObjects::monitor_Y0;
	this->BackgroundImage = BACKGROUND;
	
	this->xrayMode->BackgroundImage = XRAY_MODE;
	this->xrayMode->Hide();

	this->batteryConnected->BackgroundImage = BATTERY_CONNECTED;
	this->batteryConnected->Hide();

	this->awsConnected->BackgroundImage = AWS_CONNECTED;
	this->awsConnected->Hide();

	this->peripheralsConnected->BackgroundImage = PERIPHERALS_CONNECTED;
	this->awsConnected->Hide();

	this->doorClosed->BackgroundImage = DOOR_CLOSED;
	this->awsConnected->Hide();

	this->tubeTempOk->BackgroundImage = TUBE_TEMP_OK;
	//this->awsConnected->Hide();


}