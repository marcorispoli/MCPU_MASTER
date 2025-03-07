#pragma once

/**

	\addtogroup BiopsyModuleDescription


	<div style="page-break-after: always;"></div>
	# Homing procedure description

	The following flowchart describes the Homing procedure
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
	};

	// Process Status
	static bool running;
	static bool result;
	static bool completed;
	static int  step;
	static unsigned short X, Y, Z;
	static home_positions target;

	// Process activation procedures
	static int activateCenter(int awsid);
	static int activateLeft(int awsid);
	static int activateRight(int awsid);

	// Periodical callback handling the procedure state machine
	static void procedure_callback(void);

private:

	


	

	

};

