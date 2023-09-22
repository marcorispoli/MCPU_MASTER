#include "pch.h"

Errors::Errors() {
	list = gcnew List<item^> ();	
	current = -1;
}

void Errors::activate(String^ msg, bool one_shot) {

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

	for (int i = 0; i < list->Count; i++) {
		// The error code is already active: tests if the error message is changed
		if (list[i]->errmsg == msg) {
			if (current == i) current = -1;
			list->RemoveAt(i);
			update = true;
			return; 
		}
	}

}
