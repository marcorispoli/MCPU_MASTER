#include "pch.h"

using namespace System;

// int main(array<System::String ^> ^args)
// {
//    return 0;
// }

#include "Form1.h"

using namespace System::Windows::Forms;

[STAThread]
int main()
{
  Application::EnableVisualStyles();
  Application::SetCompatibleTextRenderingDefault(false);
  GlobalObjects::pAws = gcnew awsProtocol("127.0.0.1", 10000, 10001);

  Application::Run(gcnew CppCLRWinFormsProject::Form1());
  return 0;
}