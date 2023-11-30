#include "CanOpenMotor.h"
#include "Translate.h"
#include "Generator.h"
#include "ExposureModule.h"
#include "IdleForm.h"
#include "MainForm.h"
#include "OperatingForm.h"
#include "ArmMotor.h"
#include "./gantry_global_status.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace CANOPEN;


[STAThread]
int main()
{
  Application::EnableVisualStyles();
  Application::SetCompatibleTextRenderingDefault(false);

  // Monitor Identification
  int monitors = Screen::AllScreens->Length;  
  int H,W;

  // Set the current language for messages and GUI
  Translate::setLanguage("ENG");
  
  Notify::disable("GENERATOR_NOT_READY");
  Notify::disable("GENERATOR_INIT_WARNING");
  Notify::disable("GENERATOR_ERROR_CONNECTION");
  Notify::disable("STATOR_SENSOR_LOW");
  Notify::disable("BULB_SENSOR_LOW");


  Debug::WriteLine("DETECTED MONITORS:" + monitors.ToString());

  

  // Get the current directory for the resources
  GlobalObjects::applicationResourcePath = System::IO::Directory::GetCurrentDirectory() + "\\RESOURCES\\";

  
  // Gets the Monitors p
  // osition
  for (int i = 0; i < monitors; i++) {
	  H = Screen::AllScreens[i]->Bounds.Height;
	  W = Screen::AllScreens[i]->Bounds.Width;
	  
	  if (W == 600) {
		  GlobalObjects::monitor_X0 = Screen::AllScreens[i]->Bounds.Left;
		  GlobalObjects::monitor_Y0 = Screen::AllScreens[i]->Bounds.Top;
		  Debug::WriteLine("SELECTED MONITOR: X0 = " + GlobalObjects::monitor_X0.ToString() + ", Y0 = " + GlobalObjects::monitor_Y0.ToString());
	  }
  }

  //GlobalObjects::pGenerator = gcnew Generator();



  //globalObjects::pProtocol = paws; // Assignes the class pointer to the global pointer for the application usage
  GlobalObjects::pIdleForm = gcnew IdleForm();
  GlobalObjects::pOperatingForm = gcnew OperatingForm();

  Application::Run(gcnew CppCLRWinFormsProject::MainForm());
  return 0;
}