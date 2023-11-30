#pragma once


using namespace System::Collections::Generic;

public ref class Translate 
{
public:
	


	
	
	ref class item {
	public:
		#define MSG_LBL 1
		#define MSG_WRN 2
		#define MSG_ERR 3
		#define MSG_INFO 4

		item(System::String^ c, System::String^ i, System::String^ t, System::String^ cn) {
			contest = c;
			id = i;
			title = t;
			content = cn;

			if (c == "LABEL") type = MSG_LBL;
			else if (c == "WARNING") type = MSG_WRN;
			else if (c == "ERROR") type = MSG_ERR;
			else type = MSG_INFO;
		}

		bool isLabel(void) { return (type == MSG_LBL); }
		bool isError(void) { return (type == MSG_ERR); }
		bool isWarning(void) { return (type == MSG_WRN); }
		bool isInfo(void) { return (type == MSG_INFO); }

		System::String^ contest;
		System::String^ id;
		System::String^ title;
		System::String^ content;

		unsigned char type;
	};
	
	
	static void setLanguage(System::String^ language);
	static System::String^ Translate::title(System::String^ key);
	static System::String^ Translate::content(System::String^ key);
	static System::String^ Translate::id(System::String^ key);
	static item^ Translate::getItem(System::String^ key);
	inline static System::String^ Translate::label(System::String^ key) { return title(key); };

private:

	static Dictionary<System::String^, item^>^ dictionary = nullptr;
};

