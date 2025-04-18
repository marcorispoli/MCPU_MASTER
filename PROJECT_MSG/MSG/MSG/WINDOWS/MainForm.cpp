#include "application.h"
#include "mainForm.h"


using namespace CppCLRWinFormsProject;


#define DOC_NAME "MessageNotifyDescription"
#define NATIVE_LIST "translate_eng.h"

void MainForm::MainFormInitialize(void) {

	// Initialize the position of the form
	//this->Left = Gantry::monitor_X0;
	//this->Top = Gantry::monitor_Y0;
	selectedError->Text = "-";
	labelSelectedTag->Text = "-";
	selectedTitle->Text = "-";
	selectedContent->Text = "-";

	startupFase = 0;
	startupSubFase = 0;
	startupCompleted = false;
	startupError = false;

	descriptionBox->Enabled = false;
	descriptionBox->Clear();

	// Start the startup session
	startupTimer = gcnew System::Timers::Timer(100);
	startupTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &MainForm::onStartupTimeout);
	startupTimer->Start();


}




void MainForm::WndProc(System::Windows::Forms::Message% m) 
{
	switch (m.Msg) {

	case (WM_USER + 1): // onStartupTimeout		
		
		break;

	case (WM_USER + 2): 
		
		break;

	case (WM_USER + 3): 
		
		break;

	case (WM_USER + 4): 
		
		break;

	case (WM_USER + 5): 
		
		break;
	}


	Form::WndProc(m);
}

using namespace System::IO;

void MainForm::loadConfig(System::String^ dir) {
	
	config = nullptr;
	if (tagsArray == nullptr) return;

	// Initializes the configuration
	config = gcnew configurationClass(tagsArray->Length);

	for (int i = 0; i < tagsArray->Length; i++) {
		config->tags[i] = tagsArray[i];
		config->descriptions[i] = "-";
	}

	// Populates the Description from the file
	if (!File::Exists(dir + "\\" + "DocConfig.cnf")) {

		// Store a new file without comments
		storeConfig(dir);
		return ;
	}
	

	System::String^ linestr;
	System::String^ tag;
	System::String^ desc;
	StreamReader^ din;
	
	try {
		din = File::OpenText(dir + "\\" + "DocConfig.cnf");

		while ((linestr = din->ReadLine()) != nullptr)
		{
			if (linestr->Contains("|")) {

				// Trova il primo |
				int p = linestr->IndexOf("|");
				if (p < 0) continue;
				linestr = linestr->Substring(p + 1);
				
				// Trova il secondo |
				p = linestr->IndexOf("|");
				if (p < 0) continue;
				tag = linestr->Substring(0, p);
				linestr = linestr->Substring(p + 1);

				// Trova il terzo |
				p = linestr->IndexOf("|");
				if (p < 0) continue;
				desc = linestr->Substring(0, p)->Replace("<br>", "\n");
				
				p = System::Array::IndexOf(tagsArray, tag);
				if (p >= 0) {
					config->descriptions[p] = desc;
				}
				
			}
		}

		din->Close();
	}
	catch (...) {
		return;
	}

	
}

void MainForm::storeConfig(System::String^ dir) {
	if (config == nullptr) return;

	// Creates the document configuration
	StreamWriter^ sw = gcnew StreamWriter(dir + "\\" + "DocConfig.cnf");
	System::String^ desc;

	for (int i = 0; i < config->tags->Length; i++) {
		desc = config->descriptions[i]->Replace( "\n","<br>");
		sw->WriteLine("|" + config->tags[i] + "|" + desc + "|");
	}
	sw->Close();
}

void MainForm::loadTagList(System::String^ dir) {

	selectedError->Text = "-";
	labelSelectedTag->Text = "-";
	selectedTitle->Text = "-";
	selectedContent->Text = "-";

	tagsArray = nullptr;

	if (!File::Exists(dir + "\\..\\MESSAGES\\Notify.h")) {		
		return;
	}

	
	int itemCnt = 0;

	// Prevent to faith against a runtime attempt
	StreamReader^ din;
	try {
		din = File::OpenText(dir + "\\..\\MESSAGES\\Notify.h");
		listTag->Items->Clear();

		System::String^ linestr;
		System::String^ errstr;
		int count = 0;
		bool found = false;
		while ((linestr = din->ReadLine()) != nullptr)
		{
			linestr = linestr->Replace(" ", "");
			if (linestr->Contains("enumclassmessages{")) {
				found = true; 
				break;
			}
		}

		if (!found) {
			din->Close();
			return;
		}

		while ((linestr = din->ReadLine()) != nullptr)
		{
			linestr = linestr->Replace(" ", "");
			if (linestr->Contains("};")) break;
			if (linestr->Contains("NUM_MESSAGES")) break;
			if (linestr->Contains("NO_MESSAGE")) continue;
			if (linestr->Contains("WARNING_MESSAGES_SECTION")) continue;
			if (linestr->Contains("INFO_MESSAGES_SECTION")) continue;
			if (linestr->Contains("LABEL_MESSAGES_SECTION")) continue;			
			if (linestr->Contains("\/")) continue;

			if (linestr->Contains(",")) {
				linestr = linestr->Replace(",", "");
				linestr = linestr->Replace("\t", ""); 
				listTag->Items->Add(linestr);
			}
			
		}

		tagsArray = gcnew cli::array<System::String^>(listTag->Items->Count);
		for (int i = 0; i < tagsArray->Length; i++) {
			tagsArray[i] = listTag->Items[i]->ToString();
		}

		din->Close();
	}
	catch (...) {
		return;
	}

	

	
}
void MainForm::loadLanguageList(System::String^ dir) {
	selectedError->Text = "-";
	labelSelectedTag->Text = "-";
	selectedTitle->Text = "-";
	selectedContent->Text = "-";

	array<System::String^>^ files = Directory::GetFiles(dir);
	comboLanguages->Items->Clear();

	for (int i = 0; i < files->Length; i++) {
		if (files[i]->Contains("translate")) comboLanguages->Items->Add(Path::GetFileName(files[i]));
	}
	try {
		comboLanguages->SelectedIndex = comboLanguages->FindStringExact("translate_eng.h");
	}
	catch (...) {

	}
}

void MainForm::loadNativeMessageList(void) {

	nativeArray = nullptr;
	if (!File::Exists(dirBrowser->SelectedPath + "\\" + NATIVE_LIST)) return;
	int itemCnt = 0;

	// Creates the array
	nativeArray = gcnew cli::array<msgItem^>(tagsArray->Length);

	// Reads the file and populates the array
	StreamReader^ din;
	System::String^ linestr;
	msgItem^ pItem;
	int count = 0;

	try {
		din = File::OpenText(dirBrowser->SelectedPath + "\\" + NATIVE_LIST);
		while ((linestr = din->ReadLine()) != nullptr)
		{
			pItem = gcnew msgItem(linestr);
			if (pItem->tp != msgItem::item_type::INVALID){
				int p = System::Array::IndexOf(tagsArray, pItem->msg_tag);
				if (p >= 0) {
					nativeArray[count++] = pItem;
				}				
			}
		}
		din->Close();
	}
	catch (...) {
		return;
	}

}

void MainForm::loadMessageList(System::String^ filename) {

	selectedError->Text = "-";
	labelSelectedTag->Text = "-";
	selectedTitle->Text = "-";
	selectedContent->Text = "-";


	errorList->Items->Clear();
	itemArray = nullptr;
	if (!File::Exists(dirBrowser->SelectedPath + "\\" + filename)) return;
	int itemCnt = 0;

	// Creates the array
	itemArray = gcnew cli::array<msgItem^>(tagsArray->Length);

	// Reads the file and populates the array
	StreamReader^ din;
	System::String^ linestr;
	msgItem^ pItem;
	int count = 0;

	try {
		din = File::OpenText(dirBrowser->SelectedPath + "\\" + filename);
		while ((linestr = din->ReadLine()) != nullptr)
		{
			pItem = gcnew msgItem(linestr);
			if (pItem->tp != msgItem::item_type::INVALID) {
				int p = System::Array::IndexOf(tagsArray, pItem->msg_tag);
				if (p >= 0) {
					itemArray[count++] = pItem;
					errorList->Items->Add(pItem->msg_code);
				}
			}
		}
		din->Close();
	}
	catch (...) {
		return;
	}


}

void MainForm::GenerateTranslatorFile(bool only_changed) {

	// No translator file for the english language
	if(comboLanguages->Text->Equals("translate_eng.h")) return;
	System::String^ language = comboLanguages->Text->Replace(".h", "");
	language = language->Replace("translate", "translator");

	// Creates the cvs file
	StreamWriter^ sw = gcnew StreamWriter(dirBrowser->SelectedPath + "\\" + language + ".cvs");

	// Header
	sw->WriteLine("MESSAGE-ID;TITLE-ENG;CONTENT-ENG;TITLE-TRANSLATED;CONTENT-TRANSLATED");

	System::String^ stringa;
	for (int i = 0; i < itemArray->Length; i++) {
		if (only_changed) {
			stringa = itemArray[i]->msg_title;
			stringa = stringa->Replace(" ", "");
			if (!((stringa->Equals("-")) || (stringa->Equals("")))) continue;
		}
		sw->WriteLine(itemArray[i]->msg_code + ";" + nativeArray[i]->msg_title + ";" + nativeArray[i]->msg_content + ";" + itemArray[i]->msg_title + ";" + itemArray[i]->msg_content);
		
	}

	
	sw->Close();
}

void MainForm::ImportTranslatorFile(void) {

}

void MainForm::GenerateTemplate(System::String^ dir, bool new_template) {
	if (config == nullptr) return;

	// Creates the document configuration
	StreamWriter^ sw = gcnew StreamWriter(dir + "\\" + "template_xxx.h");
	System::String^ desc;
	System::String^ title;
	System::String^ content;
	System::String^ id;
	System::String^ tag;

	for (int i = 0; i < itemArray->Length; i++) {
		id = "\"" + itemArray[i]->msg_code + "\", ";
		tag = "Notify::messages::" + itemArray[i]->msg_tag + ", ";

		if (new_template) {
			title = "\"-\", ";
			content = "\"-\", ";
		}
		else {
			title = "\"" + itemArray[i]->msg_title + "\", ";
			content = "\"" + itemArray[i]->msg_content + "\", ";
		}
		sw->WriteLine(id + tag + title + content);
	}
	sw->Close();


}

void MainForm::GenerateDocumentation(System::String^ dir) {
	System::String^ language = comboLanguages->Text->Replace(".h", "");
	language = language->Replace("translate", "language");

	StreamWriter^ sw = gcnew StreamWriter(dir + "\\" + language + "_doc.h");
	System::String^ desc;
	System::String^ strdata;

	// Creates the Doxygen style Table
	sw->WriteLine(" /** \\addtogroup " + DOC_NAME);
	sw->WriteLine(" ");

	sw->WriteLine("\\section MSGDESC Messages Description Table ");
	sw->WriteLine(" ");

	sw->WriteLine("In this section the meaning of every message will be described in detail.");
	sw->WriteLine(" ");

	// Write the Message Content
	sw->WriteLine(" | MESSAGE CODE | TYPE | Description |");
	sw->WriteLine(" |:--|:--|:--|");
	for (int i = 0, l = 0; i < nativeArray->Length; i++, l++) {
		desc = "-";
		
		if (config != nullptr) {			
			int p = System::Array::IndexOf(config->tags, nativeArray[i]->msg_tag);
			if (p >= 0) desc = config->descriptions[p]->Replace("\n", "<br>");
			if (desc->Equals("")) desc = "-";
		}

		if(desc->Equals("-")) desc = nativeArray[i]->msg_title + "<br>" + nativeArray[i]->msg_content;


		System::String^ msgtype;

		if (nativeArray[i]->msg_code->Contains("E")) msgtype = "ERROR";
		else if (nativeArray[i]->msg_code->Contains("W")) msgtype = "WARNING";
		else if (nativeArray[i]->msg_code->Contains("I")) msgtype = "INFORMATION";
		else if (nativeArray[i]->msg_code->Contains("L")) msgtype = "LABEL";
		else continue;

		strdata = "|" + itemArray[i]->msg_code + "|" + msgtype + "|" + desc + "|";
		sw->WriteLine(strdata);
		if (l == 10) {
			l = 0;
			sw->WriteLine(" ");
			sw->WriteLine("<div style=\"page-break-after: always;\"></div>");
			sw->WriteLine(" ");
			sw->WriteLine(" | MESSAGE CODE | TYPE | Description |");
			sw->WriteLine(" |:--|:--|:--|");
		}
	}

	sw->WriteLine("\\section MSGTRANS Messages Translation Tables ");
	sw->WriteLine(" ");

	sw->WriteLine("In this section, a table for every translated language is showed.");
	sw->WriteLine(" ");


	// For every language, it adds the translation table
	for (int lng = 0; lng < comboLanguages->Items->Count; lng++) {
		System::String^ language = comboLanguages->Items[lng]->ToString();
		language = language->Replace(".h", "");
		language = language->Replace("translate_", "");
		sw->WriteLine(" ");
		sw->WriteLine(" ## Translation  " + language + " table:");
		sw->WriteLine(" ");
		sw->WriteLine(" | MESSAGE CODE | TITLE | CONTENT |");
		sw->WriteLine(" |:--|:--|:--|");

		loadMessageList(comboLanguages->Items[lng]->ToString());

		for (int i = 0, l = 0; i < itemArray->Length; i++, l++) {
			

			strdata = "|" + itemArray[i]->msg_code + "|" + itemArray[i]->msg_title + "|" + itemArray[i]->msg_content + "|" ;
			sw->WriteLine(strdata);
			if (l == 20) {
				l = 0;
				sw->WriteLine(" ");
				sw->WriteLine("<div style=\"page-break-after: always;\"></div>");
				sw->WriteLine(" ");
				sw->WriteLine(" | MESSAGE CODE | TITLE | CONTENT |");
				sw->WriteLine(" |:--|:--|:--|");
			}
		}
	}


	sw->WriteLine(" */");
	sw->Close();

	// Rerload the current language
	loadMessageList(comboLanguages->Text);

}
