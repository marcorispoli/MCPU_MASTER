#pragma once


using namespace System::Collections::Generic;

ref class Notify
{
public:


	ref class item{
	public:
		item(System::String^ pmsg, bool os) {
			msg = pmsg;
			one_shot = os;
		}

		System::String^ msg;
		bool	one_shot;
	};

	static bool isError(void) { return (error_list->Count != 0); }
	static bool isWarning(void) { return (warning_list->Count != 0); }
	static bool isInfo(void) { return (info_list->Count != 0); }
	
	static void clrNewError(void) { last_error = nullptr; }
	static void clrOneShotErrors(void);
	static System::String^ getListOfErrors(void);
	static System::String^ formatMsg(System::String^ key);

	static item^ getCurrent(void);
	static void activate(System::String^ msg, bool one_shot);
	static void deactivate(System::String^ msg);

private:
	static List<item^>^ error_list = gcnew List<item^>();
	static item^ last_error = nullptr;
	
	static List<item^>^ warning_list = gcnew List<item^>();
	static List<item^>^ info_list = gcnew List<item^>();

};

