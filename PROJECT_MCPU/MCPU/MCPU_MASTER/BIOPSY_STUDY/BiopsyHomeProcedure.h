#pragma once

/**

	\addtogroup BiopsyModuleDescription

	# Home Procedure Description

	## Procedure Overview

	The Home position is the starting position of every pointing activation.
	
	There are three possible home positions:
	+ Home Center position: the initial position for the upright approach;
	+ Home Right position: the initial position for the Right approach;
	+ Home Left position: the initial position for the Left approach;

	The Home Center position refers to the upright biopsy procedure:
	+ The X-SCROLL is set in the Center position;
	+ The Y-AXIS is in upright position;
	+ The X,Y,Z coordinate for the Home Center position are set into the Biopsy configuration file;

	The Home Left position refers to the left approach biopsy procedure:
	+ The X-SCROLL is set in Left position;
	+ The Y-AXIS is in upside-down position;
	+ The X,Y,Z coordinate for the Home Left position are set into the Biopsy configuration file;

	The Home Right position refers to the right approach biopsy procedure:
	+ The X-SCROLL is set in Right position;
	+ The Y-AXIS is in upside-down position;
	+ The X,Y,Z coordinate for the Home Left position are set into the Biopsy configuration file;

	\warning  At the beginning of every study the current home position is invalidated
	and any pointing activation is consequently disabled. The AWS shall command almost ones
	a valid Home position, based on the current approach.

	\note In case of unselected home position or in case a wrong position setting is detected
	(wrong xscroll position, wrong coordinates, wrong Y-Axis Up or Down position),
	the home position is invalidated.

	\note When an invalid home position should be detected, a Warning message is activated.

	## Procedure Activation
	
	The procedure can be activated with the AWS protocol command EXEC_BiopsyHoming().
	See the protocol documentation for details.

	\note the command can terminates instantly if the home position is already detected.

	After the command is activated, the GUI will help the operator 
	to execute the procedure as described in the following section.

	\warning In case of an error sould prevent to successfully complete the command,
	an error message will be activated until a valid activation will perform successfully.

	## Procedure Description

	The Home procedure is a complex procedure that requires automatic activations
	as well as operator manual activities.

	The process will drive the Pointer to the expected target position 
	with toghether the correct Xscroll and Y-rotation positions.

	During the process, if the pointer should need to cross the Base position,
	it may be necessary to Tourn Up the Y-Axis before:
	+ in this case, the pointer will be driven to the left or right available x-corner to let the operator to
	tourn up the Y axis.

	When the pointer is finally driven to the target X,Y,Z position, the  process will ask 
	the operator to tourn Down the Y-axis (onlyfor the Right and Left home positions).

	\note Every time the operator action is requested, and after the Gantry senses the correct executed action,
	a graphic button will appear asking for the confirmation. This solution prevents to starts the pointer motors
	with the operator hands still on the mechanical parts.


	<div style="page-break-after: always;"></div>
	## Procedure Flowchart

	\dot
digraph {
	compound=true;

	// Home node definition
	init_home [label = "init home" shape = oval]
	end_home [label = "end home" shape = oval]
	EvalScrollUndef [label = "scroll undefined ?" shape = diamond]
	WaitScrollOkDescr [label="The X-Scroll shall be\n in a well defined position" shape = note]
	WaitScrollOk [label = "Display image x-scroll\nwait scroll detection (L or C or R)\nwait operator confirmation"	shape = parallelogram]
	WaitScrollOk -> WaitScrollOkDescr [arrowhead = none style = dashed]
	EvalYud [label = "Yud ?" shape = diamond]
	EvalYudDescr [label="If the Y should be down\nit shall be flipped Up\nbefore to move the pointer" shape = note]
	EvalYud -> EvalYudDescr [arrowhead = none style = dashed]
	EvalCenter [label = "Center?" shape = diamond]
	EvalXScroll [label = "X-Scroll?" shape = diamond]
	X [label = "activate X\ndisplay X moving\nwait X-termination" shape = box]
	Y [label = "activate Y\ndisplay Y moving\nwait Y-termination" shape = box]
	Z [label = "activate Z\ndisplay Z moving\nwait Z-termination" shape = box]

	// YUP node definition
	init_yup [label = "init Yup" shape = oval]
	end_yup [label = "end Yup" shape = oval]
	error_yup [label = "error Yup" shape = oval]
	eval_movelr_yup [label = "Move Left\nor Right?" shape = diamond]
	mov_left_yup [label = "activate X to max\ndisplay X moving left\nwait X-termination" shape = box]
	mov_right_yup [label = "activate X to 0\ndisplay X moving right\nwait X-termination" shape = box]
	flip_yup [label = "display image flip up\nwait flip up sensor\nwait operator confirmation" shape = box]

	// Y-Down node definition
	flip_ydown [label = "display image flip down\nwait flip down sensor\nwait operator confirmation" shape = box]

	// Scroll node definition
	init_xscroll [label = "init X-scroll" shape = oval]
	end_xscroll [label = "end X-scroll" shape = oval]


	subgraph  cluster_home {
		label = "Homing Procedure"

		init_home -> EvalScrollUndef
		EvalScrollUndef -> WaitScrollOk [label = "undefined" ]
		EvalScrollUndef -> EvalYud [label = "defined"]
		WaitScrollOk -> EvalYud [label = "scrolled"]

		// Y up or down?
		EvalYud -> proc_xyz [xlabel = "Y-Up"]
		EvalYud -> init_yup [ label = "Y-Down" lhead=cluster_y_flip_up]



		// XYZ - activation
		proc_xyz -> X
		X -> end_home [ label = "x-error"]
		X -> Y [ label = "x-ok"]
		Y -> end_home [ label = "y-error"]
		Y -> Z [ label = "y-ok"]
		Z -> end_home [ label = "z-error"]
		Z -> endproc_xyz

		endproc_xyz -> EvalXScroll

		// X shall be scrolled ?
		EvalXScroll -> init_xscroll [ label = "needed" lhead=cluster_xscroll]
		EvalXScroll -> EvalCenter [ label = "in position"]
		end_xscroll -> EvalCenter

		// The home is Center ?
		EvalCenter -> end_home [label = "is Center"]

		// The home is not center:execute the flip down
		EvalCenter -> flip_ydown [label = "is (L or R)" lhead=cluster_y_flip_down]
		flip_ydown -> end_home

	}

	// Procedure Y-Flip Up Description
	subgraph  cluster_y_flip_up {
		label = "Flip Y Up procedure";



		init_yup -> eval_movelr_yup

		// where to go: left or right?
		eval_movelr_yup -> mov_left_yup [label = "left"]
		eval_movelr_yup -> mov_right_yup [label = "right"]

		mov_left_yup ->error_yup [label = "error"]
		mov_right_yup ->error_yup [label = "error"]

		mov_left_yup -> flip_yup [label = "left ok"]
		mov_right_yup -> flip_yup [label = "right ok"]
		flip_yup -> end_yup ->proc_xyz
		error_yup -> end_home
	}


	// Procedure for the mechanical scroll
	subgraph  cluster_xscroll {
		label = "X-SCROLL Procedure";

		init_xscroll -> end_xscroll
	}



}

\enddot


*/

ref class BiopsyHomeProcedure
{
public:
	
	BiopsyHomeProcedure();
	
	// Class Autogeneration
	static BiopsyHomeProcedure^ proc = gcnew BiopsyHomeProcedure();

	enum class home_positions {
		HOME_CENTER = 0,
		HOME_LEFT,
		HOME_RIGHT,
		UNDEFINED
	};

	// Process Status
	static bool running;
	static int result;
	static bool completed;
	static int  step;
	static unsigned short X, Y, Z;
	static home_positions target;
	static int awsid; 

	// Process activation procedures
	static int activateCenter(int awsid);
	static int activateLeft(int awsid);
	static int activateRight(int awsid);
	static bool isValidHome(void);

	// Periodical callback handling the procedure state machine
	static void procedure_callback(void);

private:

	


	

	

};

