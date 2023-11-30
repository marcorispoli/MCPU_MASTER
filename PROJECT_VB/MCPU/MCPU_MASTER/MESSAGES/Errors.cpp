#include "Errors.h"
#include "Translate.h"
#include "mutex"

static std::mutex gantry_errors_mutex;

void Notify::activate(System::String^ msg, bool os) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);
	Translate::item^ msgit;

	msgit = Translate::getItem(msg);
	if (msgit == nullptr) return;
	
	// verifies if the message should be disabled
	for (int i = 0; i < disable_list->Count; i++) {
		if (disable_list[i]->msg == msg) return;
	}
	if (msgit->isError()) {
		for (int i = 0; i < error_list->Count; i++) {
			// The error code is already active: tests if the error message is changed
			if (error_list[i]->msg == msg) return;
		}

		// Add the error to the list
		error_list->Add(gcnew item(msg, os));
		
	}else if (msgit->isWarning()) {
		for (int i = 0; i < warning_list->Count; i++) {	
			if (warning_list[i]->msg == msg) return;
		}

		// The warning is a new warning
		warning_list->Add(gcnew item(msg, os));

	}else if (msgit->isInfo()) {
		for (int i = 0; i < info_list->Count; i++) {
			if (info_list[i]->msg == msg) return;
		}

		// The error is a new error
		info_list->Add(gcnew item(msg, os));
	}

	// Handle the last message but only if there is not a one_shot message to be served
	if (!one_shot) {
		last_error = gcnew item(msg, os);
		one_shot = os;
	}
	
}

void Notify::activate(System::String^ msg, System::String^ extra, bool os) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);
	Translate::item^ msgit;

	msgit = Translate::getItem(msg);
	if (msgit == nullptr) return;

	// verifies if the message should be disabled
	for (int i = 0; i < disable_list->Count; i++) {
		if (disable_list[i]->msg == msg) return;
	}

	if (msgit->isError()) {
		for (int i = 0; i < error_list->Count; i++) {
			// The error code is already active: tests if the error message is changed
			if (error_list[i]->msg == msg) return;
		}

		// Add the error to the list
		error_list->Add(gcnew item(msg, extra, os));

	}
	else if (msgit->isWarning()) {
		for (int i = 0; i < warning_list->Count; i++) {
			if (warning_list[i]->msg == msg) return;
		}

		// The warning is a new warning
		warning_list->Add(gcnew item(msg, extra, os));

	}
	else if (msgit->isInfo()) {
		for (int i = 0; i < info_list->Count; i++) {
			if (info_list[i]->msg == msg) return;
		}

		// The error is a new error
		info_list->Add(gcnew item(msg, extra, os));
	}

	// Handle the last message but only if there is not a one_shot message to be served
	if (!one_shot) {
		last_error = gcnew item(msg, extra, os);
		one_shot = os;
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

void Notify::disable(System::String^ msg) {
	
	System::String^ curmsg;
	Translate::item^ msgit;

	msgit = Translate::getItem(msg);
	if (msgit == nullptr) return;

	// Deactivate the message if it should be present
	deactivate(msg);
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);

	for (int i = 0; i < disable_list->Count; i++) {
		// The error code is already active: tests if the error message is changed
		if (disable_list[i]->msg == msg) return;		
	}
	disable_list->Add(gcnew item(msg, false));
}


Notify::item^ Notify::getCurrent(void) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);
	return last_error;

}

void Notify::clrOneShotErrors(void) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);
	one_shot = false;

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
		if (last_error->extra) errlist += "\n" + last_error->extra;
		errlist += "\n\n";		
	}

	// Sets the full list
	for (int i = 0; i < error_list->Count; i++) {
		if ((last_error) && (last_error->msg == error_list[i]->msg)) continue;
		errlist += formatMsg(error_list[i]->msg);
		if (error_list[i]->extra) errlist += "\n" + error_list[i]->extra;
		errlist += "\n\n";
	}

	for (int i = 0; i < warning_list->Count; i++) {
		errlist += formatMsg(warning_list[i]->msg);
		if (warning_list[i]->extra) errlist += "\n" + warning_list[i]->extra;
		errlist += "\n\n";
	}

	for (int i = 0; i < info_list->Count; i++) {
		errlist += formatMsg(info_list[i]->msg);
		if (info_list[i]->extra) errlist += "\n" + info_list[i]->extra;
		errlist += "\n\n";
	}

	return errlist;
}

