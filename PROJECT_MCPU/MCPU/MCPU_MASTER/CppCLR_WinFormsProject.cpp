#include "gantry_global_status.h"
#include "Notify.h"
#include "MainForm.h"
#include "ConfigurationFiles.h"
#include "Log.h"

using namespace System;
using namespace System::Windows::Forms;

/// <summary>
/// \defgroup APPDOC Application Documentation
/// 
/// This section provides detailed description of the Application functions
/// and performances.
/// 
/// </summary>
/// <returns></returns>

/// <summary>
/// \defgroup APPIMPL Application Implementation
/// 
/// This section provides detailed description of the modules iplementation.
/// 
/// </summary>
/// <returns></returns>


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
	Application::Run(gcnew CppCLRWinFormsProject::MainForm());
	return 0;
}