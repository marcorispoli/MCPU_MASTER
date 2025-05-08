#include "BiopsyHomeProcedure.h"
#include "gantry_global_status.h"
#include "BiopsyStudy.h"
#include "PCB325.h"
#include "ConfigurationFiles.h"
#include "awsProtocol.h"


#define BIOPSY ((BiopsyStudy^) Gantry::pBiopsyStudy)

// Strings to be translated
#define CONFIRM_INFO Notify::TranslateContent(Notify::messages::INFO_BIOPSY_CONFIRM_ACTION)
#define INVALID_XSCROLL_INFO_STRING Notify::TranslateContent(Notify::messages::INFO_BIOPSY_XSCROLL_UNDEFINED_ACTION)
#define FLIP_UP_MOVE_LEFT_INFO Notify::TranslateContent(Notify::messages::INFO_BIOPSY_MOVE_LEFT_ACTION)
#define FLIP_UP_MOVE_RIGHT_INFO Notify::TranslateContent(Notify::messages::INFO_BIOPSY_MOVE_RIGHT_ACTION)
#define PROCEDURE_FLIP_UP_EXECUTE_UP_INFO Notify::TranslateContent(Notify::messages::INFO_BIOPSY_Y_TOURN_UP_ACTION)
#define PROCEDURE_X_MOVE_TO_TARGET_INFO  Notify::TranslateContent(Notify::messages::INFO_BIOPSY_X_MOVE)
#define PROCEDURE_Y_MOVE_TO_TARGET_INFO  Notify::TranslateContent(Notify::messages::INFO_BIOPSY_Y_MOVE)
#define PROCEDURE_Z_MOVE_TO_TARGET_INFO  Notify::TranslateContent(Notify::messages::INFO_BIOPSY_Z_MOVE)
#define PROCEDURE_XSCROLL_CENTER_REQUEST_INFO Notify::TranslateContent(Notify::messages::INFO_BIOPSY_XSCROLL_TO_CENTER_ACTION)
#define PROCEDURE_XSCROLL_LEFT_REQUEST_INFO Notify::TranslateContent(Notify::messages::INFO_BIOPSY_XSCROLL_TO_LEFT_ACTION)
#define PROCEDURE_XSCROLL_RIGHT_REQUEST_INFO Notify::TranslateContent(Notify::messages::INFO_BIOPSY_XSCROLL_TO_RIGHT_ACTION)
#define PROCEDURE_Y_FLIP_DOWN_REQUEST_INFO Notify::TranslateContent(Notify::messages::INFO_BIOPSY_Y_TOURN_DOWN_ACTION)

// Images to be displayed
#define INFO_ICON  Gantry::applicationResourcePath + "Icons\\Info.PNG"
#define ACTION_ICON  Gantry::applicationResourcePath + "Icons\\man_action.PNG"
#define PROCEDURE_HOME_INVALID_XSCROLL_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\XSCROLL\\INVALID_SCROLL.PNG"
#define PROCEDURE_FLIP_UP_MOVE_LEFT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\YFLIP\\TO_FLIP_UP_LEFT.PNG"
#define PROCEDURE_FLIP_UP_MOVE_RIGHT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\YFLIP\\TO_FLIP_UP_RIGHT.PNG"
#define PROCEDURE_FLIP_UP_EXECUTE_UP_LEFT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\YFLIP\\YFLIP_UP_FROM_LEFT.PNG"
#define PROCEDURE_FLIP_UP_EXECUTE_UP_RIGHT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\YFLIP\\YFLIP_UP_FROM_RIGHT.PNG"
#define PROCEDURE_X_MOVE_TO_HOME_LEFT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\HOME_MOVING\\XMOVE_TO_HOME_LEFT.PNG"
#define PROCEDURE_X_MOVE_TO_HOME_RIGHT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\HOME_MOVING\\XMOVE_TO_HOME_RIGHT.PNG"
#define PROCEDURE_X_MOVE_TO_HOME_CENTER_FROM_LEFT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\HOME_MOVING\\XMOVE_TO_HOME_CENTER_FROM_LEFT.PNG"
#define PROCEDURE_X_MOVE_TO_HOME_CENTER_FROM_RIGHT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\HOME_MOVING\\XMOVE_TO_HOME_CENTER_FROM_RIGHT.PNG"
#define PROCEDURE_XSCROLL_CENTER_FROM_LEFT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\XSCROLL\\XSCROLL_TO_CENTER_FROM_LEFT.PNG"
#define PROCEDURE_XSCROLL_CENTER_FROM_RIGHT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\XSCROLL\\XSCROLL_TO_CENTER_FROM_RIGHT.PNG"
#define PROCEDURE_XSCROLL_LEFT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\XSCROLL\\XSCROLL_TO_LEFT.PNG"
#define PROCEDURE_XSCROLL_RIGHT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\XSCROLL\\XSCROLL_TO_RIGHT.PNG"
#define PROCEDURE_Y_FLIP_DOWN_LEFT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\YFLIP\\YFLIP_DOWN_FROM_LEFT.PNG"
#define PROCEDURE_Y_FLIP_DOWN_RIGHT_IMG Gantry::applicationResourcePath +"BiopsyStudy\\BYM_IMG\\YFLIP\\YFLIP_DOWN_FROM_RIGHT.PNG"

BiopsyHomeProcedure::BiopsyHomeProcedure() {
	
	running = false;
	completed = false;
	result = false;
	step = 0;
	awsid = 0;
	result = 0;
	
	target = home_positions::UNDEFINED;
	((BiopsyStudy^)Gantry::pBiopsyStudy)->periodical_event += gcnew BiopsyStudy::delegate_periodical_callback(procedure_callback);
}


/// <summary>
///  This is the home activation procedure for the Center Home position.
/// </summary>
/// 
/// <param name="awsid"></param>
/// <returns>
/// + 1: the procedure is started;
/// + 0: the pointer is already in center position;
/// + -1: the biopsy device is not connected;
/// + -2: a home position is already running;
/// 
/// </returns>
int BiopsyHomeProcedure::activateCenter(int id) {

	if (!PCB325::isBiopsyConnected()) return -1; // Biopsy not connected
	if (running) return -2; // Busy
	
	// Assignes the targets
	X = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_CENTER_POSITION)[BiopsyConfig::PARAM_HOME_CENTER_POSITION_X]);
	Y = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_CENTER_POSITION)[BiopsyConfig::PARAM_HOME_CENTER_POSITION_Y]);
	Z = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_CENTER_POSITION)[BiopsyConfig::PARAM_HOME_CENTER_POSITION_Z]);
	
	// Verifies if it is already in target
	if (PCB325::isXtarget(X) && PCB325::isYtarget(Y) && PCB325::isZtarget(Z) && PCB325::isXC()) {
		target = home_positions::HOME_CENTER;
		return 0;
	}

	// Activate the procedure callback
	target = home_positions::HOME_CENTER;
	step = 0;
	completed = false;
	running = true;
	awsid = id;
	result = 0;
	return 1;
};

/// <summary>
///  This is the home activation procedure for the Left Home position.
/// </summary>
/// 
/// <param name="awsid"></param>
/// <returns>
/// + 1: the procedure is started;
/// + 0: the pointer is already in left position;
/// + -1: the biopsy device is not connected;
/// + -2: a home position is already running;
/// 
/// </returns>
int BiopsyHomeProcedure::activateLeft(int id) {

	if (!PCB325::isBiopsyConnected()) return -1; // Biopsy not connected
	if (running) return -2; // Busy

	// Executing conditions
	X = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_LEFT_POSITION)[BiopsyConfig::PARAM_HOME_LEFT_POSITION_X]);
	Y = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_LEFT_POSITION)[BiopsyConfig::PARAM_HOME_LEFT_POSITION_Y]);
	Z = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_LEFT_POSITION)[BiopsyConfig::PARAM_HOME_LEFT_POSITION_Z]);
	
	// Verifies if it is already in target
	if (PCB325::isXtarget(X) && PCB325::isYtarget(Y) && PCB325::isZtarget(Z) && PCB325::isXL()) {
		target = home_positions::HOME_LEFT;
		return 0;
	}

	// Activate the procedure callback
	target = home_positions::HOME_LEFT;
	step = 0;
	completed = false;
	running = true;
	awsid = id;
	result = 0;
	return 1;
};

/// <summary>
///  This is the home activation procedure for the Right Home position.
/// </summary>
/// 
/// <param name="awsid"></param>
/// <returns>
/// + 1: the procedure is started;
/// + 0: the pointer is already in right position;
/// + -1: the biopsy device is not connected;
/// + -2: a home position is already running;
/// 
/// </returns>
int BiopsyHomeProcedure::activateRight(int id) {

	if (!PCB325::isBiopsyConnected()) return -1; // Biopsy not connected
	if (running) return -2; // Busy

	X = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_RIGHT_POSITION)[BiopsyConfig::PARAM_HOME_RIGHT_POSITION_X]);
	Y = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_RIGHT_POSITION)[BiopsyConfig::PARAM_HOME_RIGHT_POSITION_Y]);
	Z = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_RIGHT_POSITION)[BiopsyConfig::PARAM_HOME_RIGHT_POSITION_Z]);
	
	// Verifies if it is already in target
	if (PCB325::isXtarget(X) && PCB325::isYtarget(Y) && PCB325::isZtarget(Z) && PCB325::isXR()) {
		target = home_positions::HOME_RIGHT;
		return 0;
	}

	// Activate the procedure callback
	target = home_positions::HOME_RIGHT;
	step = 0;
	completed = false;
	running = true;
	awsid = id;
	result = 0;
	return 1;
};

/// <summary>
/// This procedure checks if there is a valid home position.
/// </summary>
/// 
/// The valid home position requires that:
/// + a home position has been selected;
/// + the current position is in the requested home;
/// 
/// <param name=""></param>
/// <returns></returns>
bool BiopsyHomeProcedure::isValidHome(void) {
	unsigned short x, y, z;

	if (target == home_positions::UNDEFINED) return false;

	if (target == home_positions::HOME_CENTER) {
		// Assignes the targets
		x = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_CENTER_POSITION)[BiopsyConfig::PARAM_HOME_CENTER_POSITION_X]);
		y = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_CENTER_POSITION)[BiopsyConfig::PARAM_HOME_CENTER_POSITION_Y]);
		z = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_CENTER_POSITION)[BiopsyConfig::PARAM_HOME_CENTER_POSITION_Z]);

		// Verifies if it is already in target
		if (PCB325::isXtarget(x) && PCB325::isYtarget(y) && PCB325::isZtarget(z) && PCB325::isXC()) {
			return true;
		}
		target = home_positions::UNDEFINED;
		return false;
	}
	else if (target == home_positions::HOME_LEFT) {
		// Executing conditions
		x = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_LEFT_POSITION)[BiopsyConfig::PARAM_HOME_LEFT_POSITION_X]);
		y = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_LEFT_POSITION)[BiopsyConfig::PARAM_HOME_LEFT_POSITION_Y]);
		z = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_LEFT_POSITION)[BiopsyConfig::PARAM_HOME_LEFT_POSITION_Z]);

		// Verifies if it is already in target
		if (PCB325::isXtarget(x) && PCB325::isYtarget(y) && PCB325::isZtarget(z) && PCB325::isXL()) {
			return true;
		}
		target = home_positions::UNDEFINED;
		return false;
	}
	else {
		x = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_RIGHT_POSITION)[BiopsyConfig::PARAM_HOME_RIGHT_POSITION_X]);
		y = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_RIGHT_POSITION)[BiopsyConfig::PARAM_HOME_RIGHT_POSITION_Y]);
		z = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_RIGHT_POSITION)[BiopsyConfig::PARAM_HOME_RIGHT_POSITION_Z]);

		// Verifies if it is already in target
		if (PCB325::isXtarget(x) && PCB325::isYtarget(y) && PCB325::isZtarget(z) && PCB325::isXR()) {
			return true;
		}
		target = home_positions::UNDEFINED;
		return false;
	}

	return false;
}


enum {
	INIT_PROC = 0,
	SET_VALID_SCROLL_INIT,
	SET_VALID_SCROLL_WAIT_POSITIONING,
	CONFIRM_VALID_SCROLL_POSITIONING,

	EVAL_YUP_INIT,
	YUP_MOVE_LEFT,
	YUP_MOVE_RIGHT,
	YUP_WAIT_MOTION_TERMINATION,
	YUP_EXECUTE_FLIP_UP,
	YUP_EXECUTE_FLIP_UP_WAIT_SENSOR, 
	YUP_EXECUTE_FLIP_UP_WAIT_CONFIRMATION,
	
	EXECUTE_XYZ_INIT,
	XYZ_WAIT_X,
	XYZ_WAIT_Y,
	XYZ_WAIT_Z,

	EXECUTE_XSCROLL_INIT,
	EXECUTE_XSCROLL_WAIT_SENSOR,
	EXECUTE_XSCROLL_WAIT_CONFIRMATION,

	EXECUTE_Y_FLIP_DOWN_INIT,
	EXECUTE_Y_FLIP_DOWN_WAIT_SENSOR,
	EXECUTE_Y_FLIP_DOWN_WAIT_CONFIRMATION,

	PROCEDURE_COMPLETED,
	PROCEDURE_ERROR,

};
void BiopsyHomeProcedure::procedure_callback(void) {
	if (!running) return;

	switch (step) {
	case INIT_PROC: 
		// Scroll Undefined State evaluation
		if (PCB325::isXU()) step = SET_VALID_SCROLL_INIT;
		else step = EVAL_YUP_INIT;
		break;

// --------------------------------------------------------------------------------------
	case SET_VALID_SCROLL_INIT:
		BIOPSY->showProcedureImage(PROCEDURE_HOME_INVALID_XSCROLL_IMG, INVALID_XSCROLL_INFO_STRING,ACTION_ICON, false);
		step = SET_VALID_SCROLL_WAIT_POSITIONING;
		break;

	case SET_VALID_SCROLL_WAIT_POSITIONING:
		if (PCB325::isXU()) break;

		BIOPSY->showProcedureImage("", CONFIRM_INFO, INFO_ICON, true);
		BIOPSY->getProcedureConfirmation(true); // reset the confirmation flag
		step = CONFIRM_VALID_SCROLL_POSITIONING;
		break;

	case CONFIRM_VALID_SCROLL_POSITIONING:
		if (!BIOPSY->getProcedureConfirmation(false)) break;
		step = EVAL_YUP_INIT;
		break;

	// --------------------------------------------------------------------------------------
	case EVAL_YUP_INIT: // Eval Y Up STate
		if (PCB325::isYUp()) {
			step = EXECUTE_XYZ_INIT;
			break;
		}
		
		// Evaluates if shall move left or right
		if (PCB325::isGroupYleftThanBody()) step = YUP_MOVE_LEFT;
		else step = YUP_MOVE_RIGHT;
		break;

	case YUP_MOVE_LEFT:
		BIOPSY->showProcedureImage(PROCEDURE_FLIP_UP_MOVE_LEFT_IMG, FLIP_UP_MOVE_LEFT_INFO, ACTION_ICON, false);
		if (!PCB325::moveXHomeLeft()) {
			step = PROCEDURE_ERROR;
			result = 1;
			break;
		}
		step = YUP_WAIT_MOTION_TERMINATION;
		break;

	case YUP_MOVE_RIGHT:
		BIOPSY->showProcedureImage(PROCEDURE_FLIP_UP_MOVE_RIGHT_IMG, FLIP_UP_MOVE_RIGHT_INFO, ACTION_ICON, false);
		if (!PCB325::moveXHomeRight()) {
			step = PROCEDURE_ERROR;
			result = 2;
			break;
		}
		step = YUP_WAIT_MOTION_TERMINATION;
		break;

	case YUP_WAIT_MOTION_TERMINATION:
		if (PCB325::isPointerMoving()) break;
		if (!PCB325::isPointerSuccessfullyMoved()) {
			step = PROCEDURE_ERROR;
			result = 3;
			break;
		}
		step = YUP_EXECUTE_FLIP_UP;
		break;

	case YUP_EXECUTE_FLIP_UP:
		if(PCB325::isXL())	BIOPSY->showProcedureImage(PROCEDURE_FLIP_UP_EXECUTE_UP_LEFT_IMG, PROCEDURE_FLIP_UP_EXECUTE_UP_INFO, ACTION_ICON, false);
		else BIOPSY->showProcedureImage(PROCEDURE_FLIP_UP_EXECUTE_UP_RIGHT_IMG, PROCEDURE_FLIP_UP_EXECUTE_UP_INFO, ACTION_ICON, false);
		step = YUP_EXECUTE_FLIP_UP_WAIT_SENSOR;
		break;

	case YUP_EXECUTE_FLIP_UP_WAIT_SENSOR:
		if (!PCB325::isYUp()) break;
		BIOPSY->showProcedureImage("", CONFIRM_INFO, INFO_ICON, true);
		BIOPSY->getProcedureConfirmation(true); // reset the confirmation flag
		step = YUP_EXECUTE_FLIP_UP_WAIT_CONFIRMATION;
		break;
	
	case YUP_EXECUTE_FLIP_UP_WAIT_CONFIRMATION:
		if(!BIOPSY->getProcedureConfirmation(false)) break;
		step = EXECUTE_XYZ_INIT;
		break;

		// --------------------------------------------------------------------------------------
	case EXECUTE_XYZ_INIT:

		if(target == home_positions::HOME_LEFT)
			BIOPSY->showProcedureImage(PROCEDURE_X_MOVE_TO_HOME_LEFT_IMG, PROCEDURE_X_MOVE_TO_TARGET_INFO, INFO_ICON, false);
		else if (target == home_positions::HOME_RIGHT)
			BIOPSY->showProcedureImage(PROCEDURE_X_MOVE_TO_HOME_RIGHT_IMG, PROCEDURE_X_MOVE_TO_TARGET_INFO, INFO_ICON, false);
		else {
			if(PCB325::isXL()) BIOPSY->showProcedureImage(PROCEDURE_X_MOVE_TO_HOME_CENTER_FROM_LEFT_IMG, PROCEDURE_X_MOVE_TO_TARGET_INFO, INFO_ICON, false);
			else BIOPSY->showProcedureImage(PROCEDURE_X_MOVE_TO_HOME_CENTER_FROM_RIGHT_IMG, PROCEDURE_X_MOVE_TO_TARGET_INFO, INFO_ICON, false);
		}

		if (!PCB325::moveX(X)) {
			step = PROCEDURE_ERROR;
			result = 4;
			break;
		}
		step = XYZ_WAIT_X;
		break;

	case XYZ_WAIT_X:
		if (PCB325::isPointerMoving()) break;
		if (!PCB325::isPointerSuccessfullyMoved()) {
			step = PROCEDURE_ERROR;
			result = 5;
			break;
		}

		BIOPSY->showProcedureImage("", PROCEDURE_Y_MOVE_TO_TARGET_INFO, INFO_ICON, false);
		if (!PCB325::moveY(Y)) {
			step = PROCEDURE_ERROR;
			result = 6;
			break;
		}
		step = XYZ_WAIT_Y;
		break;


	case XYZ_WAIT_Y:
		if (PCB325::isPointerMoving()) break;
		if (!PCB325::isPointerSuccessfullyMoved()) {
			step = PROCEDURE_ERROR;
			result = 7;
			break;
		}

		BIOPSY->showProcedureImage("", PROCEDURE_Z_MOVE_TO_TARGET_INFO, INFO_ICON, false);
		if (!PCB325::moveZ(Z)) {
			step = PROCEDURE_ERROR;
			result = 8;
			break;
		}
		step = XYZ_WAIT_Z;
		break;

	case XYZ_WAIT_Z:
		if (PCB325::isPointerMoving()) break;
		if (!PCB325::isPointerSuccessfullyMoved()) {
			step = PROCEDURE_ERROR;
			result = 9;
			break;
		}
		step = EXECUTE_XSCROLL_INIT;
		break;
	
	// --------------------------------------------------------------------------------------
	case EXECUTE_XSCROLL_INIT:
		if ((target == home_positions::HOME_CENTER) && (PCB325::isXC())) {			
			step = EXECUTE_Y_FLIP_DOWN_INIT;
			break;
		}
		if ((target == home_positions::HOME_LEFT) && (PCB325::isXL())) {
			step = EXECUTE_Y_FLIP_DOWN_INIT;
			break;
		}
		if ((target == home_positions::HOME_RIGHT) && (PCB325::isXR())) {
			step = EXECUTE_Y_FLIP_DOWN_INIT;
			break;
		}

		// Request to scroll
		if (target == home_positions::HOME_CENTER) {
			if(PCB325::isXL()) BIOPSY->showProcedureImage(PROCEDURE_XSCROLL_CENTER_FROM_LEFT_IMG, PROCEDURE_XSCROLL_CENTER_REQUEST_INFO, ACTION_ICON, false);
			else			BIOPSY->showProcedureImage(PROCEDURE_XSCROLL_CENTER_FROM_RIGHT_IMG, PROCEDURE_XSCROLL_CENTER_REQUEST_INFO, ACTION_ICON, false);
		}else if (target == home_positions::HOME_LEFT) {
			BIOPSY->showProcedureImage(PROCEDURE_XSCROLL_LEFT_IMG, PROCEDURE_XSCROLL_LEFT_REQUEST_INFO,ACTION_ICON, false);
		}else BIOPSY->showProcedureImage(PROCEDURE_XSCROLL_RIGHT_IMG, PROCEDURE_XSCROLL_RIGHT_REQUEST_INFO,ACTION_ICON, false);
		step = EXECUTE_XSCROLL_WAIT_SENSOR;
		break;

	case EXECUTE_XSCROLL_WAIT_SENSOR:
		
		if (target == home_positions::HOME_CENTER) {
			if (!PCB325::isXC()) break;
			BIOPSY->showProcedureImage("", CONFIRM_INFO, INFO_ICON, true);
			step = EXECUTE_XSCROLL_WAIT_CONFIRMATION;
			BIOPSY->getProcedureConfirmation(true);
			break;
		} 

		if (target == home_positions::HOME_LEFT) {
			if (!PCB325::isXL()) break;
			BIOPSY->showProcedureImage("", CONFIRM_INFO, INFO_ICON, true);
			step = EXECUTE_XSCROLL_WAIT_CONFIRMATION;
			BIOPSY->getProcedureConfirmation(true);
			break;
		}

		if (target == home_positions::HOME_RIGHT) {
			if (!PCB325::isXR()) break;
			BIOPSY->showProcedureImage("", CONFIRM_INFO, INFO_ICON, true);
			step = EXECUTE_XSCROLL_WAIT_CONFIRMATION;
			BIOPSY->getProcedureConfirmation(true);
			break;
		}

		break;

	case EXECUTE_XSCROLL_WAIT_CONFIRMATION:
		if (!BIOPSY->getProcedureConfirmation(false)) break;
		step = EXECUTE_Y_FLIP_DOWN_INIT;
		break;

	// --------------------------------------------------------------------------------------		
	case EXECUTE_Y_FLIP_DOWN_INIT:
		if (target == home_positions::HOME_CENTER) {
			step = PROCEDURE_COMPLETED;
			break;
		}
		
		if(target == home_positions::HOME_LEFT)	BIOPSY->showProcedureImage(PROCEDURE_Y_FLIP_DOWN_LEFT_IMG, PROCEDURE_Y_FLIP_DOWN_REQUEST_INFO,ACTION_ICON, false);
		else	BIOPSY->showProcedureImage(PROCEDURE_Y_FLIP_DOWN_RIGHT_IMG, PROCEDURE_Y_FLIP_DOWN_REQUEST_INFO, ACTION_ICON, false);

		step = EXECUTE_Y_FLIP_DOWN_WAIT_SENSOR;
		break;
	
	case EXECUTE_Y_FLIP_DOWN_WAIT_SENSOR:
		if (PCB325::isYUp()) break;
		BIOPSY->showProcedureImage("", CONFIRM_INFO, INFO_ICON, true);
		step = EXECUTE_Y_FLIP_DOWN_WAIT_CONFIRMATION;
		BIOPSY->getProcedureConfirmation(true);		
		break;

	case EXECUTE_Y_FLIP_DOWN_WAIT_CONFIRMATION:
		if (!BIOPSY->getProcedureConfirmation(false)) break;
		step = PROCEDURE_COMPLETED;
		break;

	// --------------------------------------------------------------------------------------
	case PROCEDURE_COMPLETED:
		running = false;
		completed = true;
		result = 0;
		step = 0;
		BIOPSY->showProcedureImage("", "", "", false);
		if(awsid) awsProtocol::EVENT_Executed(awsid, 0);

		break;
	case PROCEDURE_ERROR:
		running = false;
		completed = true;
		result = false;
		step = 0;
		target = home_positions::UNDEFINED;
		if (awsid) awsProtocol::EVENT_Executed(awsid, result);
		break;
	} // Switch()
};
