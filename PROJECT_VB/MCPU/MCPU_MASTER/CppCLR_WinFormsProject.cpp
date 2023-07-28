#include "pch.h"

using namespace System;

// int main(array<System::String ^> ^args)
// {
//    return 0;
// }

#include "Form1.h"
#include "CanDriver.h"
#include "CanOpenMotor.h"

using namespace System::Windows::Forms;
using namespace CANOPEN;

[STAThread]
int main()
{
  Application::EnableVisualStyles();
  Application::SetCompatibleTextRenderingDefault(false);
  awsProtocol^ paws = gcnew awsProtocol("127.0.0.1", 10000, 10001);
  CanDriver^   pcan = gcnew CanDriver();
  CanOpenMotor^ pMot = gcnew CanOpenMotor(0x7, 453.2);


  //globalObjects::pProtocol = paws; // Assignes the class pointer to the global pointer for the application usage

  Application::Run(gcnew CppCLRWinFormsProject::Form1());
  return 0;
}