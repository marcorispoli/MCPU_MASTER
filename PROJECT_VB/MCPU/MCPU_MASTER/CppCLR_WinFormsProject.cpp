#include "pch.h"

using namespace System;

// int main(array<System::String ^> ^args)
// {
//    return 0;
// }

#include "MainForm.h"
#include "IdleForm.h"
#include "CanDriver.h"
#include "CanOpenMotor.h"
#include "SystemConfig.h"
#include "CanDeviceProtocol.h"


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

  Debug::WriteLine("DETECTED MONITORS:" + monitors.ToString());

  GlobalObjects::pTranslate = gcnew Translate("ENG");
  GlobalObjects::pErrors = gcnew Errors();


  for (int i = 0; i < monitors; i++) {
	  H = Screen::AllScreens[i]->Bounds.Height;
	  W = Screen::AllScreens[i]->Bounds.Width;
	  
	  if (W == 600) {
		  GlobalObjects::monitor_X0 = Screen::AllScreens[i]->Bounds.Left;
		  GlobalObjects::monitor_Y0 = Screen::AllScreens[i]->Bounds.Top;
		  Debug::WriteLine("SELECTED MONITOR: X0 = " + GlobalObjects::monitor_X0.ToString() + ", Y0 = " + GlobalObjects::monitor_Y0.ToString());
	  }
  }

  
  
  
  
  



  //globalObjects::pProtocol = paws; // Assignes the class pointer to the global pointer for the application usage
  GlobalObjects::pIdleForm = gcnew IdleForm();

  Application::Run(gcnew CppCLRWinFormsProject::MainForm());
  return 0;
}