#include "gantry_global_status.h"
#include "Notify.h"
#include "MainForm.h"
#include "SystemConfig.h"

using namespace System;
using namespace System::Windows::Forms;



[STAThread]
int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	
	Gantry::initialize();
	
	Notify::disable(Notify::messages::WARNING_GENERATOR_NOT_READY);
	Notify::disable(Notify::messages::WARNING_GENERATOR_INIT);
	Notify::disable(Notify::messages::ERROR_GENERATOR_ERROR_CONNECTION);
	Notify::disable(Notify::messages::ERROR_STATOR_SENSOR_LOW);
	Notify::disable(Notify::messages::ERROR_BULB_SENSOR_LOW);

	Notify::disable(Notify::messages::ERROR_VERTICAL_MOTOR_HOMING);
	Notify::disable(Notify::messages::ERROR_ARM_MOTOR_HOMING);
	Notify::disable(Notify::messages::ERROR_SLIDE_MOTOR_HOMING);
	Notify::disable(Notify::messages::ERROR_TILT_MOTOR_HOMING);
	Notify::disable(Notify::messages::ERROR_BODY_MOTOR_HOMING);
	Notify::disable(Notify::messages::ERROR_BODY_MOTOR_BRAKE_FAULT);
	

	Application::Run(gcnew CppCLRWinFormsProject::MainForm());
	return 0;
}