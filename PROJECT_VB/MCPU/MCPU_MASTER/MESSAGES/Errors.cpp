#include "pch.h"
#include "mutex"

static mutex gantry_errors_mutex;

void Errors::activate(String^ msg, bool one_shot) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);

	for (int i = 0; i < list->Count; i++) {
		// The error code is already active: tests if the error message is changed
		if (list[i]->errmsg == msg) return;
	}

	// The error is a new error
	list->Add(gcnew item(msg, one_shot));
	current = list->Count - 1;
	update = true;

}

void Errors::deactivate(String^ msg) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);
	String^ curmsg;

	if (current >= 0) curmsg = list[current]->errmsg;
	if (msg == curmsg) current = -1;

	for (int i = 0; i < list->Count; i++) {		
		if (list[i]->errmsg == msg) {	
			list->RemoveAt(i);			
		}
	}

	// recalc the current position
	if (current >= 0 ) {
		for (int i = 0; i < list->Count; i++) {
			if (list[i]->errmsg == curmsg) {
				current = i;
				return;
			}
		}
	}

	return;
}


Errors::item^ Errors::getCurrent(void) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);

	if (current < 0) return nullptr;
	if (current >= list->Count) return nullptr;
	return list[current];
}

void Errors::clrOneShotErrors(void) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);

	// Clears the one_shot errors from the list
	for (int i = 0; i < list->Count; ) {
		if (list[i]->one_shot) {
			list->RemoveAt(i);
			continue;
		}
		i++;
	}
}


String^ Errors::getListOfErrors(void) {
	const std::lock_guard<std::mutex> lock(gantry_errors_mutex);

	String^ errlist = "";
	Translate::item^ msgit;


	// Set the current error in the Top position of the string
	if (current >= 0) {
		msgit = Translate::getItem(list[current]->errmsg);
		errlist += Translate::title(msgit->contest) + " [" + msgit->id + "]: " + msgit->title + "\n";
		errlist += msgit->content + "\n\n";
	}

	// Sets the remaining list
	for (int i = 0; i < list->Count; i++) {
		if (i == current) continue;
		msgit = Translate::getItem(list[i]->errmsg);

		errlist += Translate::title(msgit->contest) + " [" + msgit->id + "]: " + msgit->title + "\n";
		errlist += msgit->content + "\n\n";
	}

	return errlist;
}

