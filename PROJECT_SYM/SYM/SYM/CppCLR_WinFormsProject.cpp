#include "gantry_global_status.h"
#include "MainForm.h"
#include "MOTORS/Motors.h"




using namespace System;
using namespace System::Windows::Forms;



[STAThread]
int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Gantry::initialize();

	
	Application::Run(gcnew CppCLRWinFormsProject::MainForm());
	return 0;
}