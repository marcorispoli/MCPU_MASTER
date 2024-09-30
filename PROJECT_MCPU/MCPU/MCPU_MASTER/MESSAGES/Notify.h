#pragma once

#include "ErrorForm.h"
#include "infoPopup.h"

/**
\defgroup MessageNotify Application Message Notification

	This section describes the Application message notification system.

*
*/

/**
	\defgroup MessageNotifyModule Message Notification Implementation
	\ingroup MessageNotify
	\internal

	This section describes the implementation of the message notification system.
*/


/**
\defgroup MessageNotifyDescription Message Notification Description
\ingroup MessageNotify

   # Abstract

   This document describes in detail the message notification 
   system and the GUI labeling.


   # Index

   + \ref MSGGUI \n;
   + \ref MSGTAB \n;

   # Overview

   The Application makes use of series offormatted string 
   for labeling the GUI field and for generating runtime information  messages 
   to the operator.

   All those strings can be translated in one of the following languages:

   + Italian;
   + English;
   + Portuguese;
   + Spanish;
   + Franch;
   + Russian;

   The current language shall be set into the SystemInit.cnf file (see \ref SystemConfig),
   and activate during the system startup. 
   However, the AWS software can change it with the proper protocol command.

   There are four types of translated strings:
   + The Error Messages: they are system errors related to malfunctions of some of the Gantry sub part;
   + The Warning Messages: they are momentary status that prevent to execute the current exposure sequence;
   + The Info Messages: they are the current status information that can help the Operator for the current workflow;
   + The Labels: they are strings used to name the GUI fields when required;

   \section MSGGUI Message Gui Display

   \section MSGTAB Message Description Table



*/


using namespace System::Collections::Generic;

public ref class Notify
{
public:
	
	enum class  messages {
		NO_MESSAGE =0,
		ERROR_FATAL_COMPILATION,
		ERROR_AWS_INITIALIZATION,
		ERROR_PCB301_RESET,
		ERROR_PCB302_RESET,
		ERROR_PCB303_RESET,
		ERROR_PCB304_RESET,
		ERROR_PCB315_RESET,
		ERROR_PCB326_RESET,
		ERROR_PCB301_COMMUNICATION_ERROR,
		ERROR_PCB302_COMMUNICATION_ERROR,
		ERROR_PCB303_COMMUNICATION_ERROR,
		ERROR_PCB304_COMMUNICATION_ERROR,
		ERROR_PCB315_COMMUNICATION_ERROR,
		ERROR_PCB326_COMMUNICATION_ERROR,
		ERROR_XRAY_PUSH_FAULT,
		ERROR_MOTOR_PEDALS_UD_FAULT,
		ERROR_MOTOR_PEDALS_COMP_FAULT,
		ERROR_MOTOR_BUTTON_CARM_FAULT,
		ERROR_MOTOR_BUTTON_UD_FAULT,
		ERROR_CONFIGURATION_FAULT,
		ERROR_POWER_DOWN_ERROR,
		ERROR_EMERGENCY_BUTTON,
		ERROR_CABINET_SAFETY,
		ERROR_BATTERY_LOW_ERROR,
		ERROR_GENERATOR_ERROR_CONNECTION,
		ERROR_COLLIMATION_SELECTION_ERROR,
		ERROR_FILTER_SELECTION_ERROR,
		ERROR_STATOR_SENSOR_LOW,
		ERROR_STATOR_SENSOR_SHORT,
		ERROR_BULB_SENSOR_LOW,
		ERROR_BULB_SENSOR_SHORT,
		ERROR_BODY_MOTOR_BRAKE_FAULT,
		ERROR_BODY_POTENTIOMETER_READ_FAULT,
		ERROR_TILT_MOTOR_BRAKE_FAULT,
		ERROR_TILT_MOTOR_HOMING,
		ERROR_VERTICAL_MOTOR_HOMING,
		ERROR_VERTICAL_LIMIT_SWITCH, 
		ERROR_ARM_MOTOR_HOMING,
		ERROR_SLIDE_MOTOR_HOMING,
		ERROR_BODY_MOTOR_HOMING,
		ERROR_BODY_LIMIT_SWITCH,
		ERROR_GENERATOR_SETUP,
		ERROR_GENERATOR_SYS_MESSAGE,
		ERROR_POTTER_GRID_POSITION,

		

		ERROR_X_RAY_BUTTON_RELEASED,
		ERROR_X_RAY_LOW_ANODIC,
		ERROR_X_RAY_LOW_KV,
		ERROR_X_RAY_TIMEOUT,
		ERROR_X_RAY_STARTER,

		// WARNING MESSAGE SECTION
		WARNING_MESSAGES_SECTION,
		WARNING_MISSING_COMPRESSION,
		WARNING_MISSING_PATIENT_PROTECTION,
		WARNING_ARM_POSITION_WARNING,
		WARNING_POSITION_NOT_CALIBRATED,
		WARNING_FORCE_NOT_CALIBRATED,
		WARNING_MISSING_PADDLE,
		WARNING_WRONG_PADDLE,
		WARNING_MISSING_EXPOSURE_MODE,
		WARNING_MISSING_EXPOSURE_DATA,
		WARNING_XRAY_BUTTON_DISABLED,
		WARNING_DOOR_STUDY_OPEN,		
		WARNING_GENERATOR_NOT_READY,
		WARNING_TUBE_TEMPERATURE,
		WARNING_COLLI_LIGHT_ACTIVATION,
		WARNING_STATOR_SENSOR_HIGH,
		WARNING_BULB_SENSOR_HIGH,
		WARNING_ANODE_TEMPERATURE_HIGH,
		WARNING_FILTER_OUT_OF_POSITION,
		WARNING_COLLIMATOR_OUT_OF_POSITION,
		WARNING_GENERATOR_SERVICE_MODE,
		WARNING_GRID_OUT_OF_POSITION,
		WARNING_GRID_GENERAL_ENABLE,

		// INFO MESSAGE SECTION
		INFO_MESSAGES_SECTION,
		INFO_BATTERY_DISABLED,
		INFO_PROJECTION_CONFIRMATION,
		INFO_PROJECTION_ABORT,
		INFO_SLIDE_ACTIVATION_CONFIRMATION,
		INFO_BODY_ACTIVATION_CONFIRMATION,
		INFO_ARM_ACTIVATION_CONFIRMATION,
		INFO_TILT_ACTIVATION_CONFIRMATION,
		INFO_SLIDE_ACTIVATION_FAILED,
		INFO_BODY_ACTIVATION_FAILED,
		INFO_ARM_ACTIVATION_FAILED,
		INFO_TILT_ACTIVATION_FAILED,

		INFO_BODY_DRIVER,
		INFO_VERTICAL_DRIVER,
		INFO_TILT_DRIVER,
		INFO_ARM_DRIVER,
		INFO_SLIDE_DRIVER,
		INFO_MOTOR_48V_SAFETY_LINE_OFF,
		INFO_MOTOR_48V_POWER_SUPPLY_OFF,
		INFO_BURNING_JUMPER_PRESENT,
		INFO_POWER_LOCK,
		INFO_ACTIVATION_MOTOR_MANUAL_DISABLE,
		INFO_ACTIVATION_MOTOR_SAFETY_DISABLE,
		INFO_ACTIVATION_MOTOR_ERROR_DISABLE,

		INFO_POWER_OFF_REQUEST_ACTIVATION,
		INFO_GRID_INOUT_MANUAL_MODE,
		

		// LABEL MESSAGE SECTION
		LABEL_MESSAGES_SECTION,

		LABEL_ERROR_WINDOW_PANEL,
		LABEL_PROJECTION_SELECTION_PANEL,		
		LABEL_ZERO_SETTING_PANEL_TITLE,
		LABEL_ROTATION_TOOL_PANEL_TITLE,
		LABEL_SERVICE_PANEL_TITLE,
		LABEL_CALIBRATION_PANEL_TITLE,
		LABEL_CALIBRATION_XRAY_TUBE_TITLE,
		LABEL_NOT_READY_FOR_EXPOSURE,
		LABEL_READY_FOR_EXPOSURE,
		LABEL_X_RAY_ON,

		LABEL_MOTOR_COMMAND_SUCCESS,
		LABEL_MOTOR_COMMAND_MANUAL_TERMINATION,
		LABEL_MOTOR_ERROR_OBSTACLE_DETECTED,
		LABEL_MOTOR_ERROR_MOTOR_BUSY,
		LABEL_MOTOR_ERROR_INITIALIZATION,
		LABEL_MOTOR_ERROR_UNEXPECTED_STATUS,
		LABEL_MOTOR_ERROR_LIMIT_SWITCH,
		LABEL_MOTOR_ERROR_BRAKE_DEVICE,
		LABEL_MOTOR_ERROR_TIMOUT,
		LABEL_MOTOR_ERROR_INTERNAL_FAULT,
		LABEL_MOTOR_ERROR_ACCESS_REGISTER,
		LABEL_MOTOR_ERROR_MISSING_HOME,
		LABEL_MOTOR_ERROR_COMMAND_DISABLED,
		LABEL_MOTOR_ERROR_COMMAND_ABORTED,

		LABEL_COMPRESSION_ACTIVATED,
		LABEL_ARM_ACTIVATED,
		LABEL_BODY_ACTIVATED,
		LABEL_VERTICAL_ACTIVATED,
		LABEL_SLIDE_ACTIVATED,
		LABEL_TILT_ACTIVATED,


		// Last declaration
		NUM_MESSAGES,
		LABEL_ERROR,
		LABEL_WARNING,
		LABEL_INFO,
		LABEL_LABEL
	};

	ref class item{
	public:
		item() {
			extra = nullptr;
			disabled = false;
			active = false;
		}

		bool active;
		bool disabled;
		System::String^ extra;
	};

	ref class translate {
	public:
		
		translate (System::String^ num, messages m, System::String^ t, System::String^ cn) {
			number = num;
			msg = m;
			title = t;
			content = cn;
		}

		bool isError(void) { return (msg < messages::WARNING_MESSAGES_SECTION); }
		bool isWarning(void) { return ((msg > messages::WARNING_MESSAGES_SECTION) && (msg < messages::INFO_MESSAGES_SECTION)); }
		bool isInfo(void) { return ((msg > messages::INFO_MESSAGES_SECTION) && (msg < messages::LABEL_MESSAGES_SECTION)); }

		messages msg;
		System::String^ number;
		System::String^ title;
		System::String^ content;
	};

	static List<translate^>^ dictionary = nullptr;
	static bool setLanguage(System::String^ language);
	
	static System::String^ TranslateTitle(messages msg);
	static System::String^ TranslateContent(messages msg);
	static System::String^ TranslateNumber(messages msg);
	inline static System::String^ TranslateLabel(messages msg) { return TranslateTitle(msg); };
	static translate^ Translate(messages msg);


	static bool isError(void) { return (error_counter != 0); }
	static bool isWarning(void) { return (warning_counter != 0); }
	static bool isInfo(void) { return (info_counter != 0); }
	static bool isInstant(void) { return (instant_msg != messages::NO_MESSAGE); }

	
	static void clrInstant(void) { instant_msg = messages::NO_MESSAGE; }

	static System::String^ getListOfErrors(void);
	static System::String^ formatMsg(messages msg);
	

	static void disable(messages msg);
	static void activate(messages msg);
	static void activate(messages msg, System::String^ extra);
	static void deactivate(messages msg);

	static void instant(messages msg);
	static void instant(messages msg, System::String^ extra);
	static messages getInstant(void) { return instant_msg; }


	Notify() {
		message_list = gcnew List<item^>();

		for (int i = 0; i < (int)messages::NUM_MESSAGES; i++) {
			message_list->Add(gcnew item());
		}

		errorWindow = gcnew ErrorForm();
		instantWindow = gcnew infoPopupForm();

	}
	static Notify^ device = gcnew Notify();


	static void open_error(Form^ parent) {
		errorWindow->open(parent);
	}
	static void close_error(void) {
		errorWindow->close();
	}

	static void open_instant(Form^ parent) {
		instantWindow->open(parent, (int) instant_msg, instant_extra);
		instant_msg = messages::NO_MESSAGE;
	}
	static void close_instant(void) {
		instantWindow->close();
	}

	inline static bool isInstantOpen(void) { return instantWindow->open_status; }
	inline static bool isErrorOpen(void) { return errorWindow->open_status; }

private:
	static List<item^>^ message_list;	
	static messages instant_msg = messages::NO_MESSAGE;
	static System::String^ instant_extra;

	static int error_counter = 0;
	static int warning_counter = 0;
	static int info_counter = 0;

	static ErrorForm^ errorWindow;
	static infoPopupForm^ instantWindow;


};

