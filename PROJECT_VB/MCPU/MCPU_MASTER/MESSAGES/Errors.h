#pragma once

using namespace System;
using namespace System::Collections::Generic;

ref class Errors
{
public:


	ref class item{
	public:
		item(String^ msg, bool os) {
			errmsg = msg;
			one_shot = os;
		}

		String^ errmsg;
		bool	one_shot;
	};

	static bool isError(void) { return (list->Count != 0); }
	static bool isNewError(void) { return (current >= 0); }
	static bool isUpdateError(void) { return (update); }
	
	static void clrUpdate(void) { update=false; }
	static void clrNewError(void) { current = -1; }
	static void clrOneShotErrors(void);
	static String^ getListOfErrors(void);

	static item^ getCurrent(void);
	static void activate(String^ msg, bool one_shot);
	static void deactivate(String^ msg);

private:
	static List<item^>^ list = gcnew List<item^>();
	static int  current = -1;
	static bool update = false;

};

