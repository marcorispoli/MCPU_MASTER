#include "gantry_global_status.h"
#include "Notify.h"
#include "MainForm.h"
#include "ConfigurationFiles.h"
#include "Log.h"

using namespace System;
using namespace System::Windows::Forms;

/////////////////////
/*
/// \test
/// test
///
/// \warning
/// warning
///
/// \note
/// Nota
///
///
/// \remarks
/// remarks
///
/// \since
/// definitsce il tempo di rilascio o la disponibilita
///
/// \warning
/// WARNING
///
/// \todo
/// to be done


*/
/// <summary>
/// \defgroup APPDOC Application Documentation
/// 
/// This section provides detailed description of the Application functions
/// and performances.
/// 
/// </summary>


/// <summary>
/// \defgroup APPIMPL Application Implementation
/// 
/// This section provides detailed description of the modules iplementation.
/// 
/// </summary>


/// \defgroup OPERMODE Application Operating Modes
///	\ingroup APPDOC
/// 
/// # Overview
/// 
/// The workflow of the Application is organized in operating modes.
/// 
/// The operating mode acts as a status in a status machine application.
/// 
/// Each operating mode defines a specific way the application behaves. 
/// 
/// Every operating mode runs with a dedicated GUI on the Gantry display 
/// that can be composed of several views or windows.
/// 
/// 
/// The Application handles the following operating modes:
/// + Startup Operating mode;
/// + Idle Operating mode;
/// + Service Operating mode;
/// + Patient Operating mode;
/// 
/// The Application implements for every operating status a given
/// 
/// ## Startup Operating Mode
/// 
/// The Startup Operating Mode is the first running after application starts.
/// 
/// The role of this operating mode is:
/// + startup the application modules;
/// + handle the connection with the Gantry devices;
/// + handle the system configuration;
/// 
/// ## Idle Operating Mode
/// 
/// The Idle Operating mode handles the Gantry activities 
/// when the Gantry is stand alone and no interactions are expected with the 
/// Actors. 
/// 
/// ## Service Operating Mode
/// 
/// The service operating mode, as the name clearly says, handles 
/// the service activities:
/// + test tools;
/// + calibration tools;
/// + configuration tools;
/// 
/// ## Patient Operating Mode
/// 
/// The Patient Operating Mode is the most important mode of the application because handles 
/// the functions related to the exposure management:
/// + compression handling;
/// + projection selection;
/// + exposure management;
/// + safety;
/// 
/// 
/// 
/// 

[STAThread]
int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Gantry::initialize();

	//Notify::disable(Notify::messages::ERROR_VERTICAL_MOTOR_HOMING);
	//Notify::disable(Notify::messages::ERROR_ARM_MOTOR_HOMING);
	//Notify::disable(Notify::messages::ERROR_SLIDE_MOTOR_HOMING);
	//Notify::disable(Notify::messages::ERROR_TILT_MOTOR_HOMING);


	LogClass::logInFile("PROGRAM INITIALIZATION");
	Application::Run(gcnew StartupOperatingMode::MainForm());
	return 0;
}