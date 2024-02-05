#include "gantry_global_status.h"
#include "Notify.h"
#include "MainForm.h"
#include "SystemConfig.h"
#include "Log.h"

using namespace System;
using namespace System::Windows::Forms;



[STAThread]
int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Gantry::initialize();

	//Notify::disable(Notify::messages::ERROR_VERTICAL_MOTOR_HOMING);
	Notify::disable(Notify::messages::ERROR_ARM_MOTOR_HOMING);
	Notify::disable(Notify::messages::ERROR_SLIDE_MOTOR_HOMING);
	Notify::disable(Notify::messages::ERROR_TILT_MOTOR_HOMING);


	LogClass::logInFile("PROGRAM INITIALIZATION");

	Application::Run(gcnew CppCLRWinFormsProject::MainForm());
	return 0;
}