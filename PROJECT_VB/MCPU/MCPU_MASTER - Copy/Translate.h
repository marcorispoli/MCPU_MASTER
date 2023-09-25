#pragma once
#include "pch.h"

using namespace System;
using namespace System::Collections::Generic;

public ref class Translate 
{
public:
	Translate(String^ language);


	const String^ LABEL_LNG		= "LABEL";
	const String^ INFO_LNG		= "INFO";
	const String^ WARNING_LNG	= "WARNING";
	const String^ ERROR_LNG		= "ERROR";
	
	ref class item {
	public:

		item(String^ c, String^ i, String^ t, String^ cn) {
			contest = c;
			id = i;
			title = t;
			content = cn;
		}

		String^ contest;
		String^ id;
		String^ title;
		String^ content;
	};
	
	static Dictionary<String^, item^>^ dictionary = nullptr;	
	static String^ Translate::title(String^ key);
	static String^ Translate::content(String^ key);
	static String^ Translate::id(String^ key);
	static item^ Translate::getItem(String^ key);
};

