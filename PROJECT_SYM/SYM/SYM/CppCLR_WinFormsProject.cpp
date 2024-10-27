#include "gantry_global_status.h"
#include "MainForm.h"
#include "MOTORS/Motors.h"




using namespace System;
using namespace System::Windows::Forms;



[STAThread]
int main()
{
	CreateMutexA(0, FALSE, "Local\\$myprogram$"); // try to create a named mutex
	if (GetLastError() == ERROR_ALREADY_EXISTS) // did the mutex already exist?
		return -1; // quit; mutex is released automatically

	// ... program code ...

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Gantry::initialize();

	
	Application::Run(gcnew CppCLRWinFormsProject::MainForm());
	return 0;
}