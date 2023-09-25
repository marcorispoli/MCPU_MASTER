#include "pch.h"


typedef struct {
	char* key;
	char* translation;
}item;

static const char* ENG_ARRAY[] = {
	#include "translate_eng.h"
	"LAST"
};

static const char* ITA_ARRAY[] = {
	#include "translate_ita.h"
	"LAST"
};

static const char* FRA_ARRAY[] = {
	#include "translate_fra.h"
	"LAST"
};

static const char* ESP_ARRAY[] = {
	#include "translate_esp.h"
	"LAST"
};

static const char* POR_ARRAY[] = {
	#include "translate_por.h"
	"LAST"
};

static const char* RUS_ARRAY[] = {
	#include "translate_rus.h"
	"LAST"
};

static const char* DEFAULT_ARRAY[] = {
	"LAST"
};


#define CONTEST 0
#define ID 1
#define KEY 2
#define TITLE 3
#define CONTENT 4
#define ITEMLEN 5 

void Translate::setLanguage(String^ language){

	const char** pLanguage;

	if (language == "ITA")  pLanguage = ITA_ARRAY;
	else if (language == "ENG")  pLanguage = ENG_ARRAY;
	else if (language == "FRA")  pLanguage = FRA_ARRAY;
	else if (language == "ESP")  pLanguage = ESP_ARRAY;
	else if (language == "POR")  pLanguage = POR_ARRAY;
	else if (language == "RUS")  pLanguage = RUS_ARRAY;
	else pLanguage = DEFAULT_ARRAY;

	dictionary = gcnew Dictionary<String^, item^>();

	int i = 0;
	while (1) {
		
		if (gcnew String(pLanguage[i]) == "LAST") break;
		try
		{
			item^ it = gcnew item(
				gcnew String(pLanguage[i+CONTEST]),
				gcnew String(pLanguage[i+ ID]),
				gcnew String(pLanguage[i+ TITLE]),
				gcnew String(pLanguage[i + CONTENT])
				);

			dictionary->Add(gcnew String(pLanguage[i+KEY]), it);
			
		}
		catch (Exception^ ArgumentException)
		{
			
		}
		i += ITEMLEN;
		
	}
	
};

String^ Translate::title(String^ key) {
	item^ it;
	if (dictionary == nullptr) return key;

	try
	{
		it = dictionary[key];
	}
	catch (Exception^ KeyNotFoundException)
	{
		return key;
	}

	return it->title;
}
String^ Translate::content(String^ key) {
	item^ it;
	if (dictionary == nullptr) return key;

	try
	{
		it = dictionary[key];
	}
	catch (Exception^ KeyNotFoundException)
	{
		return key;
	}

	return it->content;
}

String^ Translate::id(String^ key) {
	item^ it;
	if (dictionary == nullptr) return key;

	try
	{
		it = dictionary[key];
	}
	catch (Exception^ KeyNotFoundException)
	{
		return key;
	}

	return it->id;
}


Translate::item^ Translate::getItem(String^ key) {
	item^ it;
	if (dictionary == nullptr) return nullptr;

	try
	{
		it = dictionary[key];
	}
	catch (Exception^ KeyNotFoundException)
	{
		return nullptr;
	}

	return it;
}
