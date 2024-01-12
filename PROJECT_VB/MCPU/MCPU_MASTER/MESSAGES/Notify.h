#pragma once


using namespace System::Collections::Generic;

ref class Notify
{
public:
	
	enum class  messages {
		NO_MESSAGE =0,
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
		ERROR_BATTERY_LOW_ERROR,
		ERROR_GENERATOR_ERROR_CONNECTION,
		ERROR_COLLIMATION_SELECTION_ERROR,
		ERROR_FILTER_SELECTION_ERROR,
		ERROR_STATOR_SENSOR_LOW,
		ERROR_STATOR_SENSOR_SHORT,
		ERROR_BULB_SENSOR_LOW,
		ERROR_BULB_SENSOR_SHORT,
		ERROR_BODY_MOTOR_BRAKE_FAULT,
		ERROR_TILT_MOTOR_BRAKE_FAULT,
		ERROR_TILT_MOTOR_HOMING,
		ERROR_VERTICAL_MOTOR_HOMING,
		ERROR_VERTICAL_LIMIT_SWITCH, 
		ERROR_ARM_MOTOR_HOMING,
		ERROR_SLIDE_MOTOR_HOMING,
		ERROR_BODY_MOTOR_HOMING,
		ERROR_BODY_LIMIT_SWITCH,

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
		WARNING_WRONG_PADDLE,
		WARNING_MISSING_EXPOSURE_MODE,
		WARNING_MISSING_EXPOSURE_DATA,
		WARNING_XRAY_BUTTON_DISABLED,
		WARNING_DOOR_STUDY_OPEN,
		WARNING_GENERATOR_INIT,
		WARNING_GENERATOR_NOT_READY,
		WARNING_GENERATOR_MESSAGE,
		WARNING_TUBE_TEMPERATURE,
		WARNING_COLLI_LIGHT_ACTIVATION,
		WARNING_STATOR_SENSOR_HIGH,
		WARNING_BULB_SENSOR_HIGH,
		WARNING_ANODE_TEMPERATURE_HIGH,
		WARNING_FILTER_OUT_OF_POSITION,
		WARNING_COLLIMATOR_OUT_OF_POSITION,

		// INFO MESSAGE SECTION
		INFO_MESSAGES_SECTION,
		INFO_BATTERY_DISABLED,
		INFO_PROJECTION_CONFIRMATION,
		INFO_PROJECTION_ABORT,
		INFO_CARM_SHIFT_CONFIRMATION,

		// LABEL MESSAGE SECTION
		LABEL_MESSAGES_SECTION,

		LABEL_ERROR_WINDOW_PANEL,
		LABEL_PROJECTION_SELECTION_PANEL,		
		LABEL_ZERO_SETTING_PANEL_TITLE,
		LABEL_SERVICE_PANEL_TITLE,
		LABEL_CALIBRATION_PANEL_TITLE,
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
			one_shot = false;
			extra = nullptr;
			disabled = false;
			active = false;
		}

		
		bool one_shot;
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
	static bool isOneShot(void) { return one_shot; }

	static void clrNewError(void) { last_message = messages::NO_MESSAGE; }
	static void clrOneShotErrors(void);
	static System::String^ getListOfErrors(void);
	static System::String^ formatMsg(messages msg);

	static inline messages getLastMessage(void) { return last_message; }
	static void activate(messages msg, bool one_shot);
	static void activate(messages msg, System::String^ extra, bool one_shot);
	static void deactivate(messages msg);
	static void disable(messages msg);

	Notify() {
		message_list = gcnew List<item^>();

		for (int i = 0; i < (int)messages::NUM_MESSAGES; i++) {
			message_list->Add(gcnew item());
		}
	}
	static Notify^ device = gcnew Notify();

private:
	static List<item^>^ message_list;
	static messages last_message = messages::NO_MESSAGE;
	
	static bool one_shot = false;
	static int error_counter = 0;
	static int warning_counter = 0;
	static int info_counter = 0;


};

