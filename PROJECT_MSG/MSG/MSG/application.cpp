#include "application.h"
#include <mutex>



using namespace System;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;

msgApp::msgApp() {
 
        // Sets the resource path
        applicationResourcePath = System::IO::Directory::GetCurrentDirectory() + "\\RESOURCES\\";

        // Monitor Identification
        int monitors = Screen::AllScreens->Length;

        // Gets the Monitors position
        for (int i = 0; i < monitors; i++) {
            
            int H = Screen::AllScreens[i]->Bounds.Height;
            int W = Screen::AllScreens[i]->Bounds.Width;

            if (W == 600) {
                monitor_X0 = Screen::AllScreens[i]->Bounds.Left;
                monitor_Y0 = Screen::AllScreens[i]->Bounds.Top;                
            }
        }
       
    
}


void msgApp::initialize(void) {
   
   
}

System::String^ msgItem::decodeTypeString(System::String^ stringa) {

	int p = stringa->IndexOf(",");
	if (p < 0) return "";
	stringa = stringa->Substring(0, p);
	stringa = stringa->Replace("\"", "");
	stringa = stringa->Replace("\t", "");
	stringa = stringa->Replace(" ", "");

	return stringa;
}

System::String^ msgItem::decodeTagString(System::String^ stringa) {

	// Finds the first and second coloumn

	int p = stringa->IndexOf(",");
	if (p < 0) return "";
	stringa = stringa->Substring(p + 1);
	p = stringa->IndexOf(",");
	if (p < 0) return "";
	stringa = stringa->Substring(0, p);
	stringa = stringa->Replace("Notify::messages::", "");
	stringa = stringa->Replace("\t", "");
	stringa = stringa->Replace(" ", "");

	return stringa;
}

System::String^ msgItem::decodeTitleString(System::String^ stringa) {

	int p = stringa->IndexOf(",");
	if (p < 0) return "";
	stringa = stringa->Substring(p + 1);

	p = stringa->IndexOf(",");
	if (p < 0) return "";
	stringa = stringa->Substring(p + 1);

	p = stringa->IndexOf(",");
	if (p < 0) return "";
	stringa = stringa->Substring(0, p);

	stringa = stringa->Replace("\"", "");


	return stringa;
}
System::String^ msgItem::decodeContentString(System::String^ stringa) {

	int p = stringa->IndexOf(",");
	if (p < 0) return "";
	stringa = stringa->Substring(p + 1);

	p = stringa->IndexOf(",");
	if (p < 0) return "";
	stringa = stringa->Substring(p + 1);

	p = stringa->IndexOf(",");
	if (p < 0) return "";
	stringa = stringa->Substring(p + 1);

	p = stringa->IndexOf(",");
	if (p < 0) return "";
	stringa = stringa->Substring(0, p);

	stringa = stringa->Replace("\"", "");


	return stringa;
}

msgItem::msgItem(System::String^ stringa) {
	System::String^ msg = decodeTypeString(stringa);

	if (msg->Contains("E")) {
		tp = item_type::ERRMSG;

	}
	else if (msg->Contains("W")) {
		tp = item_type::WRNMSG;
	}
	else if (msg->Contains("L")) {
		tp = item_type::LABMSG;
	}
	else if (msg->Contains("I")) {
		tp = item_type::INFMSG;
	}
	else {
		tp = item_type::INVALID;
		return;
	}
	msg_code = msg;
	msg_tag = decodeTagString(stringa);
	msg_title = decodeTitleString(stringa);
	msg_content = decodeContentString(stringa);
	return;
}