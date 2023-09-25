#pragma once

using namespace System;
using namespace System::Collections::Generic;

ref class Errors
{
public:

	#define ERROR_TUBE "TUBE_TEMP_FAULT"
	Errors();

	ref class item{
	public:
		item(String^ msg, bool os) {
			errmsg = msg;
			one_shot = os;
		}

		String^ errmsg;
		bool	one_shot;
	};

	List<item^>^ list;
	int  current;
	bool update;

	bool isError(void) { return (list->Count != 0); }
	bool isNewError(void) { return (current >= 0); }
	bool isUpdateError(void) { return (update); }
	void clrUpdate(void) { update=false; }
	void clrNewError(void) { current = -1; }
	
	void clrOneShotErrors(void) {
		// Clears the one_shot errors from the list
		for (int i = 0; i < pERRORS->list->Count; ) {
			if (pERRORS->list[i]->one_shot) {
				pERRORS->list->RemoveAt(i);
				continue;
			}
			i++;
		}
	}

	String^ getListOfErrors(void) {
		String^ errlist = "";
		Translate::item^ msgit;
		

		// Set the current error in the Top position of the string
		if (current >= 0) {
			msgit = pTRANSLATE->getItem(pERRORS->list[current]->errmsg);
			errlist += msgit->id + ": " + msgit->title + "\n";
			errlist += msgit->content + "\n\n";
		}

		// Sets the remaining list
		for (int i = 0; i < pERRORS->list->Count; i++) {
			if (i == pERRORS->current) continue;
			msgit = pTRANSLATE->getItem(pERRORS->list[i]->errmsg);

			errlist += msgit->id + ": " + msgit->title + "\n";
			errlist += msgit->content + "\n\n";
		}

		return errlist;
	}

	item^ getCurrent(void) {
		if (current < 0) return nullptr;
		if (current >= list->Count) return nullptr;
		return list[current];
	}
	void activate(String^ msg, bool one_shot);
	void deactivate(String^ msg);
};

