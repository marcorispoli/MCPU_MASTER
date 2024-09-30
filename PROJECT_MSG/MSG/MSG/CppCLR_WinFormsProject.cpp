#include "application.h"
#include "MainForm.h"





using namespace System;
using namespace System::Windows::Forms;



[STAThread]
int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	msgApp::initialize();
	Application::Run(gcnew CppCLRWinFormsProject::MainForm());
	return 0;
}