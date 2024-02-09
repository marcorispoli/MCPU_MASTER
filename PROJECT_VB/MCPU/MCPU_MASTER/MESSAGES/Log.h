#pragma once



using namespace System::Collections::Generic;

public ref class LogClass
{
public:
	LogClass();
	
	static LogClass^ module = gcnew LogClass(); //!< Auto generation declaration
	static void logInFile(System::String^ logstr);
	static void logClose();

private:
	

	static System::String^ gantry_log;
	static Object^ stream;
	static System::String^ path;
	static bool enabled;

};

