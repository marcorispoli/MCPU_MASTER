

#pragma once

extern  double dtempo;
#include <Windows.h>

//#include "Pcb301Simulator.h"
//#include "Pcb302Simulator.h"

namespace CppCLRWinFormsProject {

	
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	

	

	/// <summary>
	/// This is the Class implementing the Startup Operating mode.
	/// </summary>
	/// \ingroup Startup_Module
	/// 
	/// 
	public ref class MainForm : public System::Windows::Forms::Form 
	{
	public:
	MainForm(void)
	{
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		MainFormInitialize();

	}
		
	~MainForm()
	{
		if (components)
		{
			delete components;
		}
	}

	
	

	



	private: System::Windows::Forms::TextBox^ selectedTitle;
	private: System::Windows::Forms::RichTextBox^ selectedContent;
	private: System::Windows::Forms::RichTextBox^ descriptionBox;


	private: System::Windows::Forms::ListBox^ listTag;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::GroupBox^ groupBox2;


	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::ComboBox^ comboLanguages;


	private: System::Windows::Forms::TextBox^ selectedError;

	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::TextBox^ selectedTag;
	private: System::Windows::Forms::Button^ storeButton;
	private: System::Windows::Forms::Button^ descriptionApplyButton;
	private: System::Windows::Forms::Button^ exitButton;
	private: System::Windows::Forms::Button^ generateButton;
	private: System::Windows::Forms::Button^ buttonTemplate;
	private: System::Windows::Forms::Button^ duplicateTemplateButton;











	public:
		int rx_len;


	void loadMessageList(System::String^ dir, System::String^ file);
	void loadTagList(System::String^ dir);
	void loadLanguageList(System::String^ dir);
	void GenerateMessageList(System::String^ dir);
	void GenerateTemplate(System::String^ dir, bool new_template);

	void loadConfig(System::String^ dir);
	void storeConfig(System::String^ dir);

	System::String^ msgPath;
	System::String^ currentLanguage;
	cli::array<msgItem^>^ itemArray;
	cli::array<System::String^>^ tagsArray;
	configurationClass^ config;

	
	System::Timers::Timer^ startupTimer; //!< This is the Timer to step the workflow
	HWND window;//!< This is the handle of the windw for the windows message exchange
	int startupFase; //!< This is the current fase of the startup
	int startupSubFase;//!< This is the current sub-fase of the startup

	public:

		   bool startupCompleted;//!< The Startup completed flag
	private: System::Windows::Forms::FolderBrowserDialog^ dirBrowser;
	public:

	private: System::Windows::Forms::Button^ open_message_folder;


	private: System::Windows::Forms::ListBox^ errorList;

	public:

	private:

		

	public:




	public:
		bool startupError;//!< The Startup completed in error condition

	private: void MainFormInitialize(void);
	



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->dirBrowser = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->open_message_folder = (gcnew System::Windows::Forms::Button());
			this->errorList = (gcnew System::Windows::Forms::ListBox());
			this->selectedTitle = (gcnew System::Windows::Forms::TextBox());
			this->selectedContent = (gcnew System::Windows::Forms::RichTextBox());
			this->descriptionBox = (gcnew System::Windows::Forms::RichTextBox());
			this->listTag = (gcnew System::Windows::Forms::ListBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->duplicateTemplateButton = (gcnew System::Windows::Forms::Button());
			this->buttonTemplate = (gcnew System::Windows::Forms::Button());
			this->generateButton = (gcnew System::Windows::Forms::Button());
			this->exitButton = (gcnew System::Windows::Forms::Button());
			this->descriptionApplyButton = (gcnew System::Windows::Forms::Button());
			this->storeButton = (gcnew System::Windows::Forms::Button());
			this->selectedError = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->selectedTag = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->comboLanguages = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// open_message_folder
			// 
			this->open_message_folder->ForeColor = System::Drawing::Color::Black;
			this->open_message_folder->Location = System::Drawing::Point(334, 448);
			this->open_message_folder->Name = L"open_message_folder";
			this->open_message_folder->Size = System::Drawing::Size(138, 52);
			this->open_message_folder->TabIndex = 0;
			this->open_message_folder->Text = L"OPEN";
			this->open_message_folder->UseVisualStyleBackColor = true;
			this->open_message_folder->Click += gcnew System::EventHandler(this, &MainForm::open_message_folder_Click);
			// 
			// errorList
			// 
			this->errorList->DisplayMember = L"ffff";
			this->errorList->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->errorList->FormattingEnabled = true;
			this->errorList->ItemHeight = 16;
			this->errorList->Location = System::Drawing::Point(216, 40);
			this->errorList->Name = L"errorList";
			this->errorList->Size = System::Drawing::Size(108, 516);
			this->errorList->TabIndex = 2;
			this->errorList->ValueMember = L"ffff";
			this->errorList->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::errorList_MouseClick);
			// 
			// selectedTitle
			// 
			this->selectedTitle->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->selectedTitle->Location = System::Drawing::Point(330, 328);
			this->selectedTitle->Name = L"selectedTitle";
			this->selectedTitle->Size = System::Drawing::Size(476, 26);
			this->selectedTitle->TabIndex = 6;
			this->selectedTitle->Text = L"-";
			// 
			// selectedContent
			// 
			this->selectedContent->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->selectedContent->Location = System::Drawing::Point(330, 360);
			this->selectedContent->Name = L"selectedContent";
			this->selectedContent->Size = System::Drawing::Size(476, 67);
			this->selectedContent->TabIndex = 7;
			this->selectedContent->Text = L"-";
			// 
			// descriptionBox
			// 
			this->descriptionBox->Location = System::Drawing::Point(330, 166);
			this->descriptionBox->Name = L"descriptionBox";
			this->descriptionBox->Size = System::Drawing::Size(476, 66);
			this->descriptionBox->TabIndex = 8;
			this->descriptionBox->Text = L"-";
			// 
			// listTag
			// 
			this->listTag->DisplayMember = L"ffff";
			this->listTag->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->listTag->FormattingEnabled = true;
			this->listTag->ItemHeight = 16;
			this->listTag->Location = System::Drawing::Point(6, 40);
			this->listTag->Name = L"listTag";
			this->listTag->Size = System::Drawing::Size(204, 516);
			this->listTag->TabIndex = 9;
			this->listTag->ValueMember = L"ffff";
			this->listTag->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::listTag_MouseClick);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(6, 17);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(115, 20);
			this->label1->TabIndex = 10;
			this->label1->Text = L"Internal Tags";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(212, 17);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(51, 20);
			this->label2->TabIndex = 11;
			this->label2->Text = L"Code";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->duplicateTemplateButton);
			this->groupBox2->Controls->Add(this->buttonTemplate);
			this->groupBox2->Controls->Add(this->generateButton);
			this->groupBox2->Controls->Add(this->exitButton);
			this->groupBox2->Controls->Add(this->descriptionApplyButton);
			this->groupBox2->Controls->Add(this->storeButton);
			this->groupBox2->Controls->Add(this->selectedError);
			this->groupBox2->Controls->Add(this->label7);
			this->groupBox2->Controls->Add(this->selectedTag);
			this->groupBox2->Controls->Add(this->label6);
			this->groupBox2->Controls->Add(this->comboLanguages);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Controls->Add(this->label1);
			this->groupBox2->Controls->Add(this->descriptionBox);
			this->groupBox2->Controls->Add(this->open_message_folder);
			this->groupBox2->Controls->Add(this->label2);
			this->groupBox2->Controls->Add(this->selectedContent);
			this->groupBox2->Controls->Add(this->listTag);
			this->groupBox2->Controls->Add(this->selectedTitle);
			this->groupBox2->Controls->Add(this->errorList);
			this->groupBox2->Location = System::Drawing::Point(30, 21);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(829, 564);
			this->groupBox2->TabIndex = 12;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"groupBox2";
			// 
			// duplicateTemplateButton
			// 
			this->duplicateTemplateButton->ForeColor = System::Drawing::Color::Black;
			this->duplicateTemplateButton->Location = System::Drawing::Point(668, 504);
			this->duplicateTemplateButton->Name = L"duplicateTemplateButton";
			this->duplicateTemplateButton->Size = System::Drawing::Size(138, 52);
			this->duplicateTemplateButton->TabIndex = 26;
			this->duplicateTemplateButton->Text = L"COPY TEMPLATE";
			this->duplicateTemplateButton->UseVisualStyleBackColor = true;
			this->duplicateTemplateButton->Click += gcnew System::EventHandler(this, &MainForm::duplicateTemplateButton_Click);
			// 
			// buttonTemplate
			// 
			this->buttonTemplate->ForeColor = System::Drawing::Color::Black;
			this->buttonTemplate->Location = System::Drawing::Point(668, 448);
			this->buttonTemplate->Name = L"buttonTemplate";
			this->buttonTemplate->Size = System::Drawing::Size(138, 52);
			this->buttonTemplate->TabIndex = 25;
			this->buttonTemplate->Text = L"NEW TEMPLATE";
			this->buttonTemplate->UseVisualStyleBackColor = true;
			this->buttonTemplate->Click += gcnew System::EventHandler(this, &MainForm::buttonTemplate_Click);
			// 
			// generateButton
			// 
			this->generateButton->ForeColor = System::Drawing::Color::Black;
			this->generateButton->Location = System::Drawing::Point(524, 504);
			this->generateButton->Name = L"generateButton";
			this->generateButton->Size = System::Drawing::Size(138, 52);
			this->generateButton->TabIndex = 24;
			this->generateButton->Text = L"GENERATE-DOC";
			this->generateButton->UseVisualStyleBackColor = true;
			this->generateButton->Click += gcnew System::EventHandler(this, &MainForm::generateButton_Click);
			// 
			// exitButton
			// 
			this->exitButton->ForeColor = System::Drawing::Color::Black;
			this->exitButton->Location = System::Drawing::Point(334, 506);
			this->exitButton->Name = L"exitButton";
			this->exitButton->Size = System::Drawing::Size(138, 52);
			this->exitButton->TabIndex = 23;
			this->exitButton->Text = L"EXIT";
			this->exitButton->UseVisualStyleBackColor = true;
			this->exitButton->Click += gcnew System::EventHandler(this, &MainForm::exitButton_Click);
			// 
			// descriptionApplyButton
			// 
			this->descriptionApplyButton->ForeColor = System::Drawing::Color::Black;
			this->descriptionApplyButton->Location = System::Drawing::Point(668, 238);
			this->descriptionApplyButton->Name = L"descriptionApplyButton";
			this->descriptionApplyButton->Size = System::Drawing::Size(138, 31);
			this->descriptionApplyButton->TabIndex = 22;
			this->descriptionApplyButton->Text = L"APPLAY";
			this->descriptionApplyButton->UseVisualStyleBackColor = true;
			this->descriptionApplyButton->Click += gcnew System::EventHandler(this, &MainForm::descriptionApplyButton_Click);
			// 
			// storeButton
			// 
			this->storeButton->ForeColor = System::Drawing::Color::Black;
			this->storeButton->Location = System::Drawing::Point(524, 448);
			this->storeButton->Name = L"storeButton";
			this->storeButton->Size = System::Drawing::Size(138, 52);
			this->storeButton->TabIndex = 21;
			this->storeButton->Text = L"STORE-CONFIG";
			this->storeButton->UseVisualStyleBackColor = true;
			this->storeButton->Click += gcnew System::EventHandler(this, &MainForm::storeButton_Click);
			// 
			// selectedError
			// 
			this->selectedError->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->selectedError->Location = System::Drawing::Point(330, 296);
			this->selectedError->Name = L"selectedError";
			this->selectedError->Size = System::Drawing::Size(121, 26);
			this->selectedError->TabIndex = 19;
			this->selectedError->Text = L"-";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(326, 89);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(48, 20);
			this->label7->TabIndex = 18;
			this->label7->Text = L"Tags";
			// 
			// selectedTag
			// 
			this->selectedTag->Font = (gcnew System::Drawing::Font(L"Arial", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->selectedTag->Location = System::Drawing::Point(330, 112);
			this->selectedTag->Name = L"selectedTag";
			this->selectedTag->Size = System::Drawing::Size(476, 26);
			this->selectedTag->TabIndex = 17;
			this->selectedTag->Text = L"-";
			this->selectedTag->TextChanged += gcnew System::EventHandler(this, &MainForm::selectedTag_TextChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(330, 17);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(108, 20);
			this->label6->TabIndex = 16;
			this->label6->Text = L"LANGUAGE";
			// 
			// comboLanguages
			// 
			this->comboLanguages->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->comboLanguages->FormattingEnabled = true;
			this->comboLanguages->Location = System::Drawing::Point(330, 41);
			this->comboLanguages->Name = L"comboLanguages";
			this->comboLanguages->Size = System::Drawing::Size(262, 24);
			this->comboLanguages->TabIndex = 15;
			this->comboLanguages->TextChanged += gcnew System::EventHandler(this, &MainForm::comboLanguages_TextChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(326, 143);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(85, 20);
			this->label3->TabIndex = 12;
			this->label3->Text = L"Comment";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->ClientSize = System::Drawing::Size(885, 597);
			this->ControlBox = false;
			this->Controls->Add(this->groupBox2);
			this->DoubleBuffered = true;
			this->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"MainForm";
			this->ShowIcon = false;
			this->Text = L"Cybele Input Simulator";
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	private: System::Void onStartupTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		SendMessageA(window, WM_USER + 1, 0, 0);
	}
	
	private: System::Void open_message_folder_Click(System::Object^ sender, System::EventArgs^ e) {
		dirBrowser->ShowDialog();		
		msgPath = dirBrowser->SelectedPath;
		loadLanguageList(dirBrowser->SelectedPath);
		loadTagList(dirBrowser->SelectedPath);
		loadConfig(dirBrowser->SelectedPath);
		loadMessageList(dirBrowser->SelectedPath, comboLanguages->Text);
		
	}
	private: System::Void errorList_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		if ((itemArray == nullptr) || (itemArray->Length == 0)) return;

		for (int i = 0; i < itemArray->Length; i++) {
			if (itemArray[i]->msg_code->Equals(errorList->GetItemText(errorList->SelectedItem))) {
				listTag->SelectedIndex = listTag->FindStringExact(itemArray[i]->msg_tag);
				selectedError->Text = itemArray[errorList->SelectedIndex]->msg_code;
				selectedTag->Text = itemArray[errorList->SelectedIndex]->msg_tag;
				selectedTitle->Text = itemArray[errorList->SelectedIndex]->msg_title;
				selectedContent->Text = itemArray[errorList->SelectedIndex]->msg_content;
				return;
			}
		}
		
	}
	private: System::Void listTag_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		if ((itemArray == nullptr) || (itemArray->Length == 0)) {
			selectedError->Text = "-";
			selectedTag->Text = listTag->SelectedItem->ToString();
			selectedTitle->Text = "-";
			selectedContent->Text = "-";
			return;
		}

		

		for (int i = 0; i < itemArray->Length; i++) {
			if (itemArray[i]->msg_tag->Equals(listTag->GetItemText(listTag->SelectedItem))) {
				errorList->SelectedIndex = errorList->FindStringExact(itemArray[i]->msg_code);

				selectedError->Text = itemArray[errorList->SelectedIndex]->msg_code;
				selectedTag->Text = itemArray[errorList->SelectedIndex]->msg_tag;
				selectedTitle->Text = itemArray[errorList->SelectedIndex]->msg_title;
				selectedContent->Text = itemArray[errorList->SelectedIndex]->msg_content;
				return;
			}
		}

	}
	private: System::Void comboLanguages_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		currentLanguage = comboLanguages->Text;
		loadMessageList(dirBrowser->SelectedPath, comboLanguages->Text);
	}

	/// <summary>
	/// When a tag is selected, the related comment is displayed
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	/// <returns></returns>
	private: System::Void selectedTag_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		if (config == nullptr) return;

		int p = System::Array::IndexOf(config->tags, selectedTag->Text);
		if(p < 0) return;

		descriptionBox->Enabled = true;
		descriptionBox->Clear();
		descriptionBox->Text = config->descriptions[p];
	}
	
	
	
	private: System::Void storeButton_Click(System::Object^ sender, System::EventArgs^ e) {
		if (config == nullptr) return;
		storeConfig(dirBrowser->SelectedPath);
	}

	private: System::Void descriptionApplyButton_Click(System::Object^ sender, System::EventArgs^ e) {
		if (config == nullptr) return;

		int p = System::Array::IndexOf(config->tags, selectedTag->Text);
		if (p < 0) return;
		config->descriptions[p] = descriptionBox->Text;
	}

	private: System::Void exitButton_Click(System::Object^ sender, System::EventArgs^ e) {
		Application::Exit();
	}
	private: System::Void generateButton_Click(System::Object^ sender, System::EventArgs^ e) {
		GenerateMessageList(dirBrowser->SelectedPath);
	}

	private: System::Void buttonTemplate_Click(System::Object^ sender, System::EventArgs^ e) {
		GenerateTemplate(dirBrowser->SelectedPath, true);
	}
	private: System::Void duplicateTemplateButton_Click(System::Object^ sender, System::EventArgs^ e) {
		GenerateTemplate(dirBrowser->SelectedPath, false);
	}
	
};

}