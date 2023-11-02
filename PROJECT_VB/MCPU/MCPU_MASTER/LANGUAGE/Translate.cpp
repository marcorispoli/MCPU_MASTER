#include "Translate.h"

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

void Translate::setLanguage(System::String^ language){

	const char** pLanguage;

	if (language == "ITA")  pLanguage = ITA_ARRAY;
	else if (language == "ENG")  pLanguage = ENG_ARRAY;
	else if (language == "FRA")  pLanguage = FRA_ARRAY;
	else if (language == "ESP")  pLanguage = ESP_ARRAY;
	else if (language == "POR")  pLanguage = POR_ARRAY;
	else if (language == "RUS")  pLanguage = RUS_ARRAY;
	else pLanguage = DEFAULT_ARRAY;

	dictionary = gcnew Dictionary<System::String^, item^>();

	int i = 0;
	while (1) {
		
		if (gcnew System::String(pLanguage[i]) == "LAST") break;
		try
		{
			item^ it = gcnew item(
				gcnew System::String(pLanguage[i+CONTEST]),
				gcnew System::String(pLanguage[i+ ID]),
				gcnew System::String(pLanguage[i+ TITLE]),
				gcnew System::String(pLanguage[i + CONTENT])
				);

			dictionary->Add(gcnew System::String(pLanguage[i+KEY]), it);
			
		}
		catch (System::Exception^ ArgumentException)
		{
			
		}
		i += ITEMLEN;
		
	}
	
};

System::String^ Translate::title(System::String^ key) {
	item^ it;
	if (dictionary == nullptr) return key;

	try
	{
		it = dictionary[key];
	}
	catch (System::Exception^ KeyNotFoundException)
	{
		return key;
	}

	return it->title;
}
System::String^ Translate::content(System::String^ key) {
	item^ it;
	if (dictionary == nullptr) return key;

	try
	{
		it = dictionary[key];
	}
	catch (System::Exception^ KeyNotFoundException)
	{
		return key;
	}

	return it->content;
}

System::String^ Translate::id(System::String^ key) {
	item^ it;
	if (dictionary == nullptr) return key;

	try
	{
		it = dictionary[key];
	}
	catch (System::Exception^ KeyNotFoundException)
	{
		return key;
	}

	return it->id;
}


Translate::item^ Translate::getItem(System::String^ key) {
	item^ it;
	if (dictionary == nullptr) return nullptr;

	try
	{
		it = dictionary[key];
	}
	catch (System::Exception^ KeyNotFoundException)
	{
		return nullptr;
	}

	return it;
}
