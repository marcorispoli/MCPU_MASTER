#include "BiopsyHomeProcedure.h"
#include "gantry_global_status.h"
#include "BiopsyStudy.h"
#include "PCB325.h"
#include "ConfigurationFiles.h"


#define BIOPSY ((BiopsyStudy^) Gantry::pBiopsyStudy)

// Strings to be translated
#define CONFIRM_BUTTON "CONFIRM"
#define CONFIRM_INFO "PRESS BUTTON TO PROCEED"

#define PROCEDURE_HOME_INVALID_XSCROLL_IMG ""
#define INVALID_XSCROLL_INFO_STRING "MOVE THE X-AXIS TO A VALID NOCH POSITION"

#define PROCEDURE_HOME_VALID_XSCROLL_IMG ""

#define PROCEDURE_FLIP_UP_MOVE_LEFT_IMG ""
#define FLIP_UP_MOVE_LEFT_INFO "MOVING TO THE CORNER LEFT"

#define PROCEDURE_FLIP_UP_MOVE_RIGHT_IMG ""
#define FLIP_UP_MOVE_RIGHT_INFO "MOVING TO THE CORNER RIGHT"

#define PROCEDURE_FLIP_UP_EXECUTE_UP_IMG ""
#define PROCEDURE_FLIP_UP_EXECUTE_UP_INFO "FLIP UP THE Y AXIS"

#define PROCEDURE_FLIP_UP_EXECUTED_IMG ""
		

BiopsyHomeProcedure::BiopsyHomeProcedure() {
	

	running = false;
	completed = false;
	result = false;
	step = 0;
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
int BiopsyHomeProcedure::activateCenter(int awsid) {

	if (!PCB325::isBiopsyConnected()) return -1; // Biopsy not connected
	if (running) return -2; // Busy
	
	// Assignes the targets
	X = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_CENTER_POSITION)[BiopsyConfig::PARAM_HOME_CENTER_POSITION_X]);
	Y = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_CENTER_POSITION)[BiopsyConfig::PARAM_HOME_CENTER_POSITION_Y]);
	Z = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_CENTER_POSITION)[BiopsyConfig::PARAM_HOME_CENTER_POSITION_Z]);
	
	// Verifies if it is already in target
	if (PCB325::isXtarget(X) && PCB325::isYtarget(Y) && PCB325::isZtarget(Z) && PCB325::isXC()) {
		return 0;
	}

	// Activate the procedure callback
	step = 0;
	completed = false;
	running = true;
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
int BiopsyHomeProcedure::activateLeft(int awsid) {

	if (!PCB325::isBiopsyConnected()) return -1; // Biopsy not connected
	if (running) return -2; // Busy

	// Executing conditions
	X = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_LEFT_POSITION)[BiopsyConfig::PARAM_HOME_LEFT_POSITION_X]);
	Y = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_LEFT_POSITION)[BiopsyConfig::PARAM_HOME_LEFT_POSITION_Y]);
	Z = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_LEFT_POSITION)[BiopsyConfig::PARAM_HOME_LEFT_POSITION_Z]);
	
	// Verifies if it is already in target
	if (PCB325::isXtarget(X) && PCB325::isYtarget(Y) && PCB325::isZtarget(Z) && PCB325::isXL()) {
		return 0;
	}

	// Activate the procedure callback
	step = 0;
	completed = false;
	running = true;
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
int BiopsyHomeProcedure::activateRight(int awsid) {

	if (!PCB325::isBiopsyConnected()) return -1; // Biopsy not connected
	if (running) return -2; // Busy

	X = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_RIGHT_POSITION)[BiopsyConfig::PARAM_HOME_RIGHT_POSITION_X]);
	Y = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_RIGHT_POSITION)[BiopsyConfig::PARAM_HOME_RIGHT_POSITION_Y]);
	Z = (unsigned short)System::Convert::ToInt32(BiopsyConfig::Configuration->getParam(BiopsyConfig::PARAM_HOME_RIGHT_POSITION)[BiopsyConfig::PARAM_HOME_RIGHT_POSITION_Z]);
	
	// Verifies if it is already in target
	if (PCB325::isXtarget(X) && PCB325::isYtarget(Y) && PCB325::isZtarget(Z) && PCB325::isXR()) {
		return 0;
	}

	// Activate the procedure callback
	step = 0;
	completed = false;
	running = true;
	return 1;
};


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
	EXECUTE_XYZ_END,

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
		BIOPSY->showProcedureImage(PROCEDURE_HOME_INVALID_XSCROLL_IMG, "", INVALID_XSCROLL_INFO_STRING);
		step = SET_VALID_SCROLL_WAIT_POSITIONING;
		break;

	case SET_VALID_SCROLL_WAIT_POSITIONING:
		if (PCB325::isXU()) break;

		BIOPSY->showProcedureImage(PROCEDURE_HOME_VALID_XSCROLL_IMG, CONFIRM_BUTTON, CONFIRM_INFO);
		step = CONFIRM_VALID_SCROLL_POSITIONING;
		BIOPSY->getProcedureConfirmation(true); // reset the confirmation flag
		break;

	case CONFIRM_VALID_SCROLL_POSITIONING:
		if (!BIOPSY->getProcedureConfirmation(false)) break;
		step = EVAL_YUP_INIT;
		break;

		// --------------------------------------------------------------------------------------
	case EVAL_YUP_INIT: // Eval Y Up STate
		if (PCB325::isYUp()) step = EXECUTE_XYZ_INIT;
		
		// Evaluates if shall move left or right
		if (PCB325::isGroupYleftThanBody()) step = YUP_MOVE_LEFT;
		else step = YUP_MOVE_RIGHT;
		break;

	case YUP_MOVE_LEFT:
		BIOPSY->showProcedureImage(PROCEDURE_FLIP_UP_MOVE_LEFT_IMG, "", FLIP_UP_MOVE_LEFT_INFO);
		if (!PCB325::moveXHomeLeft()) {
			step = PROCEDURE_ERROR;
			break;
		}
		step = YUP_WAIT_MOTION_TERMINATION;
		break;

	case YUP_MOVE_RIGHT:
		BIOPSY->showProcedureImage(PROCEDURE_FLIP_UP_MOVE_RIGHT_IMG, "", FLIP_UP_MOVE_RIGHT_INFO);
		if (!PCB325::moveXHomeRight()) {
			step = PROCEDURE_ERROR;
			break;
		}
		step = YUP_WAIT_MOTION_TERMINATION;
		break;

	case YUP_WAIT_MOTION_TERMINATION:
		if (PCB325::isPointerMoving()) break;
		if (!PCB325::isPointerSuccessfullyMoved()) {
			step = PROCEDURE_ERROR;
			break;
		}
		break;

	case YUP_EXECUTE_FLIP_UP:
		BIOPSY->showProcedureImage(PROCEDURE_FLIP_UP_EXECUTE_UP_IMG, "", PROCEDURE_FLIP_UP_EXECUTE_UP_INFO);
		step = YUP_EXECUTE_FLIP_UP_WAIT_SENSOR;
		break;

	case YUP_EXECUTE_FLIP_UP_WAIT_SENSOR:
		if (!PCB325::isYUp()) break;
		BIOPSY->showProcedureImage(PROCEDURE_FLIP_UP_EXECUTED_IMG, CONFIRM_BUTTON, CONFIRM_INFO);
		BIOPSY->getProcedureConfirmation(true); // reset the confirmation flag
		break;
	
	case YUP_EXECUTE_FLIP_UP_WAIT_CONFIRMATION:
		if(!BIOPSY->getProcedureConfirmation(false)) break;
		step = EXECUTE_XYZ_INIT;
		break;

	

		// --------------------------------------------------------------------------------------
	case EXECUTE_XYZ_INIT:
		break;
	case EXECUTE_XYZ_END:
		break;
		
		// --------------------------------------------------------------------------------------
	
	case PROCEDURE_ERROR:
		break;
	} // Switch()
};

case 10:  // starts moving right
	
	sequence_step++;
	break;
case 11:
	// wait for command completion
	

	// Terminated successfully
	if (PCB325::isPointerSuccessfullyMoved()) {
		sequence_step = 19;
		break;
	}

	// Error
	execution_error = 2;
	sequence_step = 200;
	break;