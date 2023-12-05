#include "Notify.h"
#include "mutex"

static std::mutex gantry_errors_mutex;

typedef struct {	
	const char* error;
	Notify::messages msg;
	const char* title;
	const char* content;
} language_item;

static const language_item ENG_ARRAY[] = {
	#include "translate_eng.h"
	{"LAST",Notify::messages::NO_MESSAGE,"",""}
};

static const language_item ITA_ARRAY[] = {
	#include "translate_ita.h"
	{"LAST",Notify::messages::NO_MESSAGE,"",""}
};

static const language_item FRA_ARRAY[] = {
	#include "translate_fra.h"
	{"LAST",Notify::messages::NO_MESSAGE,"",""}
};

static const language_item ESP_ARRAY[] = {
	#include "translate_esp.h"
	{"LAST",Notify::messages::NO_MESSAGE,"",""}
};

static const language_item POR_ARRAY[] = {
	#include "translate_por.h"
	{"LAST",Notify::messages::NO_MESSAGE,"",""}
};

static const language_item RUS_ARRAY[] = {
	#include "translate_rus.h"
	{"LAST",Notify::messages::NO_MESSAGE,"",""}
};

static const language_item DEFAULT_ARRAY[] = {
	{"LAST",Notify::messages::NO_MESSAGE,"",""}
};

bool Notify::setLanguage(System::String^ language) {

	const language_item* pLanguage;
	bool language_ok = true;
	if (language == "ITA")  pLanguage = ITA_ARRAY;
	else if (language == "ENG")  pLanguage = ENG_ARRAY;
	else if (language == "FRA")  pLanguage = FRA_ARRAY;
	else if (language == "ESP")  pLanguage = ESP_ARRAY;
	else if (language == "POR")  pLanguage = POR_ARRAY;
	else if (language == "RUS")  pLanguage = RUS_ARRAY;
	else {
		pLanguage = DEFAULT_ARRAY;
		language_ok = false;
	}

	dictionary = gcnew List<translate^>();
	
	int i = 0;
	while (1) {
		if (pLanguage[i].msg == messages::NO_MESSAGE) break;
		System::String^ err = gcnew System::String(pLanguage[i].error);
		System::String^ title = gcnew System::String(pLanguage[i].title);
		System::String^ content = gcnew System::String(pLanguage[i].content);

		dictionary->Add(gcnew translate(err, pLanguage[i].msg, title, content));
		i ++;
	}

	return language_ok;

};

System::String^ Notify::TranslateTitle(messages msg) {
	for (int i = 0; i < dictionary->Count; i++)
	{
		if (dictionary[i]->msg == msg) return dictionary[i]->title;
	}
	return "";	
}

System::String^ Notify::TranslateContent(messages msg) {
	for (int i = 0; i < dictionary->Count; i++)
	{
		if (dictionary[i]->msg == msg) return dictionary[i]->content;
	}
	return "";
}

System::String^ Notify::TranslateNumber(messages msg) {
	for (int i = 0; i < dictionary->Count; i++)
	{
		if (dictionary[i]->msg == msg) return dictionary[i]->number;
	}
	return "";
}


Notify::translate^ Notify::Translate(messages msg) {
	for (int i = 0; i < dictionary->Count; i++)
	{
		if (dictionary[i]->msg == msg) return dictionary[i];
	}
	return nullptr;
}


void Notify::activate(messages msg, bool os) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);
	if (msg >= messages::LABEL_MESSAGES_SECTION) return;
	if (msg == messages::NO_MESSAGE) return;
	if (message_list[(int) msg]->disabled) return;
	if (message_list[(int)msg]->active) return;

	message_list[(int)msg]->active = true;
	message_list[(int)msg]->one_shot = os;
	message_list[(int)msg]->extra = nullptr;
	if (!os) last_message = msg;
	else one_shot = true;

	if (msg < messages::WARNING_MESSAGES_SECTION) error_counter++;
	else if (msg < messages::INFO_MESSAGES_SECTION) warning_counter++;
	else  info_counter++;

}

void Notify::activate(messages msg, System::String^ extra, bool os) {

	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);
	if (msg >= messages::LABEL_MESSAGES_SECTION) return;
	if (msg == messages::NO_MESSAGE) return;
	if (message_list[(int)msg]->disabled) return;
	if (message_list[(int)msg]->active) return;

	message_list[(int)msg]->active = true;
	message_list[(int)msg]->one_shot = os;
	message_list[(int)msg]->extra = extra;
	if (!os) last_message = msg;
	else one_shot = true;

	if (msg < messages::WARNING_MESSAGES_SECTION) error_counter++;
	else if (msg < messages::INFO_MESSAGES_SECTION) warning_counter++;
	else  info_counter++;
	

}


void Notify::deactivate(messages msg) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);
	if (msg >= messages::LABEL_MESSAGES_SECTION) return;
	if (msg == messages::NO_MESSAGE) return;
	if (!message_list[(int)msg]->active) return;
	message_list[(int)msg]->active = false;
	message_list[(int)msg]->extra = nullptr;

	if (msg < messages::WARNING_MESSAGES_SECTION) error_counter--;
	else if (msg < messages::INFO_MESSAGES_SECTION) warning_counter--;
	else  info_counter--;

	if (last_message == msg) last_message = messages::NO_MESSAGE;


}

void Notify::disable(messages msg) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);
	if (msg >= messages::LABEL_MESSAGES_SECTION) return;
	if (msg == messages::NO_MESSAGE) return;
	message_list[(int)msg]->disabled = true;
	if (!message_list[(int)msg]->active) return;

	message_list[(int)msg]->active = false;
	message_list[(int)msg]->extra = nullptr;

	if (msg < messages::WARNING_MESSAGES_SECTION) error_counter--;
	else if (msg < messages::INFO_MESSAGES_SECTION) warning_counter--;
	else  info_counter--;

	if (last_message == msg) last_message = messages::NO_MESSAGE;

}



void Notify::clrOneShotErrors(void) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);
	one_shot = false;

	// Clears the one_shot errors from the list
	for (int i = 0; i < (int)messages::NUM_MESSAGES; i++) {
		if ((message_list[(int)last_message]->active) && (message_list[(int)last_message]->one_shot)) {
			message_list[(int)last_message]->active = false;
			message_list[(int)last_message]->extra = nullptr;

			if (i < (int)messages::WARNING_MESSAGES_SECTION) error_counter--;
			else if (i < (int)messages::INFO_MESSAGES_SECTION) warning_counter--;
			else  info_counter--;
		}
	}

}

System::String^ Notify::formatMsg(messages msg) {
	translate^ msgit = Translate(msg);
	if (msgit == nullptr) return "";
	return "[" + msgit->number + "]: " + msgit->title + "\n" + msgit->content;
}

System::String^ Notify::getListOfErrors(void) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);

	System::String^ errlist = "";
	


	// Set the current error in the Top position of the string
	if(last_message != messages::NO_MESSAGE){
		errlist = formatMsg(last_message);
		if (message_list[(int)last_message]->extra) errlist += "\n" + message_list[(int)last_message]->extra;
		errlist += "\n\n";		
	}

	// Sets the full list
	for (int i = (int)messages::NO_MESSAGE + 1; i < (int) messages::NUM_MESSAGES; i++) {
		if ((int)last_message == i) continue;
		if (!message_list[i]->active) continue;
		if (i == (int)messages::WARNING_MESSAGES_SECTION) continue;
		if (i == (int)messages::INFO_MESSAGES_SECTION) continue;

		errlist += formatMsg((messages) i);
		if (message_list[i]->extra) errlist += "\n" + message_list[i]->extra;
		errlist += "\n\n";
	}


	return errlist;
}

