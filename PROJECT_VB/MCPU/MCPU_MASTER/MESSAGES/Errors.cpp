#include "Errors.h"
#include "Translate.h"
#include "mutex"

static std::mutex gantry_errors_mutex;

void Notify::activate(System::String^ msg, bool one_shot) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);
	Translate::item^ msgit;

	msgit = Translate::getItem(msg);
	if (msgit == nullptr) return;

	if (msgit->isError()) {
		for (int i = 0; i < error_list->Count; i++) {
			// The error code is already active: tests if the error message is changed
			if (error_list[i]->msg == msg) return;
		}

		// The error is a new error
		last_error = gcnew item(msg, one_shot);
		error_list->Add(gcnew item(msg, one_shot));
		return;
	}

	if (msgit->isWarning()) {
		for (int i = 0; i < warning_list->Count; i++) {	
			if (warning_list[i]->msg == msg) return;
		}

		// The error is a new error
		warning_list->Add(gcnew item(msg, one_shot));		
		return;
	}

	if (msgit->isInfo()) {
		for (int i = 0; i < info_list->Count; i++) {
			if (info_list[i]->msg == msg) return;
		}

		// The error is a new error
		info_list->Add(gcnew item(msg, one_shot));
		return;
	}

}

void Notify::deactivate(System::String^ msg) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);
	System::String^ curmsg;
	Translate::item^ msgit;

	msgit = Translate::getItem(msg);
	if (msgit == nullptr) return;

	if (msgit->isError()) {

		// Clear the last error if matchs with the deactivated message
		if (last_error != nullptr) {
			if (last_error->msg == msg) last_error = nullptr;
 		}

		for (int i = 0; i < error_list->Count; i++) {
			if (error_list[i]->msg == msg) {
				error_list->RemoveAt(i);
			}
		}

		return;
	}

	if (msgit->isWarning()) {
		
		for (int i = 0; i < warning_list->Count; i++) {
			if (warning_list[i]->msg == msg) {
				warning_list->RemoveAt(i);
			}
		}
		return;
	}
	
	if (msgit->isInfo()) {

		for (int i = 0; i < info_list->Count; i++) {
			if (info_list[i]->msg == msg) {
				info_list->RemoveAt(i);
			}
		}
		return;
	}

}


Notify::item^ Notify::getCurrent(void) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);
	return last_error;

}

void Notify::clrOneShotErrors(void) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);

	// Clears the one_shot errors from the list
	for (int i = 0; i < error_list->Count; ) {
		if (error_list[i]->one_shot) {
			error_list->RemoveAt(i);
			continue;
		}
		i++;
	}
	for (int i = 0; i < warning_list->Count; ) {
		if (warning_list[i]->one_shot) {
			warning_list->RemoveAt(i);
			continue;
		}
		i++;
	}
	for (int i = 0; i < info_list->Count; ) {
		if (info_list[i]->one_shot) {
			info_list->RemoveAt(i);
			continue;
		}
		i++;
	}

}

System::String^ Notify::formatMsg(System::String^ key) {
	Translate::item^ msgit = Translate::getItem(key);
	if (msgit == nullptr) return "";

	//return Translate::title(msgit->contest) + " [" + msgit->id + "]: " + msgit->title + "\n" +  msgit->content;
	return "[" + msgit->id + "]: " + msgit->title + "\n" + msgit->content;
}

System::String^ Notify::getListOfErrors(void) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);

	System::String^ errlist = "";
	Translate::item^ msgit;


	// Set the current error in the Top position of the string
	if(last_error != nullptr){
		errlist = formatMsg(last_error->msg);
		errlist += "\n\n";		
	}

	// Sets the full list
	for (int i = 0; i < error_list->Count; i++) {
		if ((last_error) && (last_error->msg == error_list[i]->msg)) continue;
		errlist += formatMsg(error_list[i]->msg);
		errlist += "\n\n";
	}

	for (int i = 0; i < warning_list->Count; i++) {
		errlist += formatMsg(warning_list[i]->msg);
		errlist += "\n\n";
	}

	for (int i = 0; i < info_list->Count; i++) {
		errlist += formatMsg(info_list[i]->msg);
		errlist += "\n\n";
	}

	return errlist;
}

