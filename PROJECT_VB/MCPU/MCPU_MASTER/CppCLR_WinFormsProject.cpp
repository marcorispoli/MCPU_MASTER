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

  for (int i = 0; i < monitors; i++) {
	  H = Screen::AllScreens[i]->Bounds.Height;
	  W = Screen::AllScreens[i]->Bounds.Width;
	  
	  if (W == 600) {
		  GlobalObjects::monitor_X0 = Screen::AllScreens[i]->Bounds.Left;
		  GlobalObjects::monitor_Y0 = Screen::AllScreens[i]->Bounds.Top;
		  Debug::WriteLine("SELECTED MONITOR: X0 = " + GlobalObjects::monitor_X0.ToString() + ", Y0 = " + GlobalObjects::monitor_Y0.ToString());
	  }
  }

  
  CanDeviceProtocol^ p = gcnew CanDeviceProtocol(10, L"PIPPO");
  
  /*
  awsProtocol^ paws = gcnew awsProtocol("127.0.0.1", 10000, 10001);
  GlobalObjects::pAws = paws; // Assignes the class pointer to the global pointer for the application usage

  
  CanOpenMotor^ pMot7 = gcnew CanOpenMotor(0x7, L"MOT7", 453.2);
  CanOpenMotor^ pMot6 = gcnew CanOpenMotor(0x6, L"MOT6", 453.2);
  CanOpenMotor^ pMot5 = gcnew CanOpenMotor(0x5, L"MOT5", 453.2);
  CanOpenMotor^ pMot4 = gcnew CanOpenMotor(0x4, L"MOT4", 453.2);
  CanOpenMotor^ pMot3 = gcnew CanOpenMotor(0x3, L"MOT3", 453.2);
  
  GlobalObjects::pMotTilt = pMot3;
  GlobalObjects::pMotArm = pMot4;
  GlobalObjects::pMotBody = pMot5;
  GlobalObjects::pMotShift = pMot6;
  GlobalObjects::pMotUpDown = pMot7;
  */



  //globalObjects::pProtocol = paws; // Assignes the class pointer to the global pointer for the application usage
  GlobalObjects::pIdleForm = gcnew IdleForm();

  Application::Run(gcnew CppCLRWinFormsProject::MainForm());
  return 0;
}