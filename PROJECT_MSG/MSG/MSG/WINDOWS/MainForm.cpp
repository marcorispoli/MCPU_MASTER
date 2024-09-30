#include "application.h"
#include "mainForm.h"


using namespace CppCLRWinFormsProject;


#define STARTUP_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\Cybele.PNG")

#define COLOR_OFF Color::Red
#define COLOR_ON Color::Yellow

void MainForm::MainFormInitialize(void) {

	// Initialize the position of the form
	//this->Left = Gantry::monitor_X0;
	//this->Top = Gantry::monitor_Y0;
	selectedError->Text = "-";
	selectedTag->Text = "-";
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

void MainForm::loadConfig(System::String^ dir, int num_tags) {
	
	
	config = gcnew configurationClass(num_tags);

	// Populates the Description from the file
	if (!File::Exists(dir + "\\" + "DocConfig.cnf")) {
		for (int i = 0; i < num_tags; i++) {
			config->tags[i] = listTag->Items[i]->ToString();
			config->descriptions[i] = "-";
		}

		// Store a new file without comments
		storeConfig(dir);
		return ;
	}
	

	System::String^ linestr;
	System::String^ tag;
	System::String^ desc;
	StreamReader^ din;
	int count = 0;
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
				

				config->tags[count] = tag;
				config->descriptions[count] = desc;
				count++;
				if (count > config->tags->Length) break;
				
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
	selectedTag->Text = "-";
	selectedTitle->Text = "-";
	selectedContent->Text = "-";

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
			if (linestr->Contains("};")) return;
			if (linestr->Contains("NUM_MESSAGES")) return;
			if (linestr->Contains("NO_MESSAGE")) continue;
			if (linestr->Contains("\/")) continue;

			if (linestr->Contains(",")) {
				linestr = linestr->Replace(",", "");
				linestr = linestr->Replace("\t", ""); 
				listTag->Items->Add(linestr);
			}
			
		}

		din->Close();
	}
	catch (...) {
		return;
	}

	
	
}
void MainForm::loadLanguageList(System::String^ dir) {
	selectedError->Text = "-";
	selectedTag->Text = "-";
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

void MainForm::loadMessageList(System::String^ dir, System::String^ filename) {

	errorList->Items->Clear();
	itemArray = nullptr;
	selectedError->Text = "-";
	selectedTag->Text = "-";
	selectedTitle->Text = "-";
	selectedContent->Text = "-";


	if (!File::Exists(dir + "\\" + filename)) return;
	int itemCnt = 0;

	// Prevent to faith against a runtime attempt
	StreamReader^ din;
	try {
		din = File::OpenText(dir + "\\" + filename);

		System::String^ linestr;
		System::String^ errstr;
		int count = 0;
		while ((linestr = din->ReadLine()) != nullptr)
		{
			errstr = msgItem::decodeTypeString(linestr);
			if (errstr != "") {
				
				itemCnt++;
			}
			count++;			
		}
		din->Close();
	}
	catch (...) {
		return ;
	}
	
	itemArray = nullptr;
	if (itemCnt == 0) {
		return;
	}
	itemArray = gcnew cli::array<msgItem^>(itemCnt);

	int errcnt = 0;
	try {
		din = File::OpenText(dir + "\\" + filename);

		System::String^ linestr;
		msgItem^ pItem;
		int count = 0;		
		while ((linestr = din->ReadLine()) != nullptr)
		{	
			pItem = gcnew msgItem(linestr);
			if (pItem->tp != msgItem::item_type::INVALID) {
				itemArray[count++] = pItem;
			}
		}
		din->Close();
	}
	catch (...) {
		return;
	}

	for (int i = 0; i < itemArray->Length; i++) {
		errorList->Items->Add(itemArray[i]->msg_code);
	}
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
		tag = "Notify::messages::" + itemArray[i]->msg_code + ", ";

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

void MainForm::GenerateMessageList(System::String^ dir) {
	System::String^ language = comboLanguages->Text->Replace(".h", "");
	language = language->Replace("translate", "language");

	StreamWriter^ sw = gcnew StreamWriter(dir + "\\" + language + "_doc.h");
	System::String^ desc;
	System::String^ strdata;

	// Creates the Doxygen style Table
	sw->WriteLine(" /** \\addtogroup MSGDOC ");
	sw->WriteLine(" This is the message translation of the language: " + language);
	sw->WriteLine(" | ID | MESSAGE | Description |");
	sw->WriteLine(" |:--|:--|:--|");

	for (int i = 0, l=0; i < itemArray->Length; i++,l++) {
		if (config != nullptr) {
			int p = System::Array::IndexOf(config->tags, itemArray[i]->msg_tag);
			if (p >= 0) desc = config->descriptions[p]->Replace("\n","<br>");
			else desc = "-";
			if (desc == "") desc = "-";
		}else desc = "-";

		strdata = "|" + itemArray[i]->msg_code + "|" + itemArray[i]->msg_title + "<br>" + itemArray[i]->msg_content + "|" + desc + "|";
		sw->WriteLine(strdata);
		if (l == 10) {
			l = 0;
			strdata = "";
			strdata = "";
			sw->WriteLine(strdata);
			strdata = "<div style=\"page-break-after: always;\"></div>";			
			sw->WriteLine(strdata);
			strdata = "";
			sw->WriteLine(strdata);
			strdata = " | ID | MESSAGE | Description |";
			sw->WriteLine(strdata);
			strdata = " |:--|:--|:--|";
			sw->WriteLine(strdata);
		}
	}
	strdata = " */";
	sw->WriteLine(strdata);
	sw->Close();
	

}
