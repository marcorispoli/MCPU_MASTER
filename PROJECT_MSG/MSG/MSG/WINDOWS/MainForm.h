

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
	private: System::Windows::Forms::Label^ labelSelectedTag;





	private: System::Windows::Forms::ComboBox^ comboLanguages;


	private: System::Windows::Forms::TextBox^ selectedError;



	private: System::Windows::Forms::Button^ storeButton;
	private: System::Windows::Forms::Button^ descriptionApplyButton;
	private: System::Windows::Forms::Button^ exitButton;
	private: System::Windows::Forms::Button^ generateButton;
	private: System::Windows::Forms::Button^ buttonTemplate;


	private: System::Windows::Forms::GroupBox^ groupBox4;
	private: System::Windows::Forms::GroupBox^ groupBox3;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::GroupBox^ groupBox5;

	private: System::Windows::Forms::Button^ exportButton;











	public:
		int rx_len;


	void loadMessageList(System::String^ file);
	void loadNativeMessageList(void);
	void loadTagList(System::String^ dir);
	void loadLanguageList(System::String^ dir);
	void GenerateDocumentation(System::String^ dir);
	void GenerateTemplate(System::String^ dir, bool new_template);
	void GenerateTranslatorFile(bool only_changed);
	void ImportTranslatorFile(void);

	void loadConfig(System::String^ dir);
	void storeConfig(System::String^ dir);

	System::String^ msgPath;
	System::String^ currentLanguage;
	cli::array<msgItem^>^ nativeArray;
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
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->comboLanguages = (gcnew System::Windows::Forms::ComboBox());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->labelSelectedTag = (gcnew System::Windows::Forms::Label());
			this->descriptionApplyButton = (gcnew System::Windows::Forms::Button());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->selectedError = (gcnew System::Windows::Forms::TextBox());
			this->exportButton = (gcnew System::Windows::Forms::Button());
			this->exitButton = (gcnew System::Windows::Forms::Button());
			this->buttonTemplate = (gcnew System::Windows::Forms::Button());
			this->storeButton = (gcnew System::Windows::Forms::Button());
			this->generateButton = (gcnew System::Windows::Forms::Button());
			this->groupBox2->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->SuspendLayout();
			// 
			// open_message_folder
			// 
			this->open_message_folder->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->open_message_folder->ForeColor = System::Drawing::Color::Black;
			this->open_message_folder->Location = System::Drawing::Point(58, 34);
			this->open_message_folder->Name = L"open_message_folder";
			this->open_message_folder->Size = System::Drawing::Size(125, 52);
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
			this->errorList->Location = System::Drawing::Point(228, 40);
			this->errorList->Name = L"errorList";
			this->errorList->Size = System::Drawing::Size(108, 596);
			this->errorList->TabIndex = 2;
			this->errorList->ValueMember = L"ffff";
			this->errorList->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::errorList_MouseClick);
			// 
			// selectedTitle
			// 
			this->selectedTitle->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->selectedTitle->Location = System::Drawing::Point(6, 54);
			this->selectedTitle->Name = L"selectedTitle";
			this->selectedTitle->Size = System::Drawing::Size(509, 26);
			this->selectedTitle->TabIndex = 6;
			this->selectedTitle->Text = L"-";
			// 
			// selectedContent
			// 
			this->selectedContent->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->selectedContent->Location = System::Drawing::Point(6, 86);
			this->selectedContent->Name = L"selectedContent";
			this->selectedContent->Size = System::Drawing::Size(509, 85);
			this->selectedContent->TabIndex = 7;
			this->selectedContent->Text = L"-";
			// 
			// descriptionBox
			// 
			this->descriptionBox->Location = System::Drawing::Point(10, 41);
			this->descriptionBox->Name = L"descriptionBox";
			this->descriptionBox->Size = System::Drawing::Size(505, 82);
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
			this->listTag->Location = System::Drawing::Point(18, 40);
			this->listTag->Name = L"listTag";
			this->listTag->Size = System::Drawing::Size(204, 596);
			this->listTag->TabIndex = 9;
			this->listTag->ValueMember = L"ffff";
			this->listTag->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::listTag_MouseClick);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(18, 17);
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
			this->label2->Location = System::Drawing::Point(224, 17);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(51, 20);
			this->label2->TabIndex = 11;
			this->label2->Text = L"Code";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->exitButton);
			this->groupBox2->Controls->Add(this->groupBox5);
			this->groupBox2->Controls->Add(this->groupBox4);
			this->groupBox2->Controls->Add(this->groupBox3);
			this->groupBox2->Controls->Add(this->generateButton);
			this->groupBox2->Controls->Add(this->label1);
			this->groupBox2->Controls->Add(this->label2);
			this->groupBox2->Controls->Add(this->listTag);
			this->groupBox2->Controls->Add(this->errorList);
			this->groupBox2->Location = System::Drawing::Point(12, 21);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(917, 658);
			this->groupBox2->TabIndex = 12;
			this->groupBox2->TabStop = false;
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->open_message_folder);
			this->groupBox5->Controls->Add(this->exportButton);
			this->groupBox5->Controls->Add(this->comboLanguages);
			this->groupBox5->Controls->Add(this->buttonTemplate);
			this->groupBox5->Location = System::Drawing::Point(346, 40);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(526, 114);
			this->groupBox5->TabIndex = 30;
			this->groupBox5->TabStop = false;
			// 
			// comboLanguages
			// 
			this->comboLanguages->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->comboLanguages->FormattingEnabled = true;
			this->comboLanguages->Location = System::Drawing::Point(253, 19);
			this->comboLanguages->Name = L"comboLanguages";
			this->comboLanguages->Size = System::Drawing::Size(262, 24);
			this->comboLanguages->TabIndex = 15;
			this->comboLanguages->TextChanged += gcnew System::EventHandler(this, &MainForm::comboLanguages_TextChanged);
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->descriptionBox);
			this->groupBox4->Controls->Add(this->labelSelectedTag);
			this->groupBox4->Controls->Add(this->descriptionApplyButton);
			this->groupBox4->Controls->Add(this->storeButton);
			this->groupBox4->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->groupBox4->Location = System::Drawing::Point(346, 160);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(526, 192);
			this->groupBox4->TabIndex = 29;
			this->groupBox4->TabStop = false;
			// 
			// labelSelectedTag
			// 
			this->labelSelectedTag->AutoSize = true;
			this->labelSelectedTag->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->labelSelectedTag->Location = System::Drawing::Point(6, 18);
			this->labelSelectedTag->Name = L"labelSelectedTag";
			this->labelSelectedTag->Size = System::Drawing::Size(115, 20);
			this->labelSelectedTag->TabIndex = 12;
			this->labelSelectedTag->Text = L"Selected Tag";
			// 
			// descriptionApplyButton
			// 
			this->descriptionApplyButton->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->descriptionApplyButton->ForeColor = System::Drawing::Color::Black;
			this->descriptionApplyButton->Location = System::Drawing::Point(11, 134);
			this->descriptionApplyButton->Name = L"descriptionApplyButton";
			this->descriptionApplyButton->Size = System::Drawing::Size(138, 45);
			this->descriptionApplyButton->TabIndex = 22;
			this->descriptionApplyButton->Text = L"APPLY";
			this->descriptionApplyButton->UseVisualStyleBackColor = true;
			this->descriptionApplyButton->Click += gcnew System::EventHandler(this, &MainForm::descriptionApplyButton_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->label3);
			this->groupBox3->Controls->Add(this->selectedContent);
			this->groupBox3->Controls->Add(this->selectedTitle);
			this->groupBox3->Controls->Add(this->selectedError);
			this->groupBox3->Location = System::Drawing::Point(346, 358);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(526, 177);
			this->groupBox3->TabIndex = 28;
			this->groupBox3->TabStop = false;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(6, 24);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(226, 20);
			this->label3->TabIndex = 20;
			this->label3->Text = L"Selected Message Content";
			// 
			// selectedError
			// 
			this->selectedError->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->selectedError->Location = System::Drawing::Point(361, 22);
			this->selectedError->Name = L"selectedError";
			this->selectedError->Size = System::Drawing::Size(154, 26);
			this->selectedError->TabIndex = 19;
			this->selectedError->Text = L"-";
			// 
			// exportButton
			// 
			this->exportButton->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->exportButton->ForeColor = System::Drawing::Color::Black;
			this->exportButton->Location = System::Drawing::Point(390, 50);
			this->exportButton->Name = L"exportButton";
			this->exportButton->Size = System::Drawing::Size(125, 52);
			this->exportButton->TabIndex = 27;
			this->exportButton->Text = L"EXPORT";
			this->exportButton->UseVisualStyleBackColor = true;
			this->exportButton->Click += gcnew System::EventHandler(this, &MainForm::exportButton_Click);
			// 
			// exitButton
			// 
			this->exitButton->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->exitButton->ForeColor = System::Drawing::Color::Black;
			this->exitButton->Location = System::Drawing::Point(494, 563);
			this->exitButton->Name = L"exitButton";
			this->exitButton->Size = System::Drawing::Size(125, 52);
			this->exitButton->TabIndex = 23;
			this->exitButton->Text = L"EXIT";
			this->exitButton->UseVisualStyleBackColor = true;
			this->exitButton->Click += gcnew System::EventHandler(this, &MainForm::exitButton_Click);
			// 
			// buttonTemplate
			// 
			this->buttonTemplate->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonTemplate->ForeColor = System::Drawing::Color::Black;
			this->buttonTemplate->Location = System::Drawing::Point(253, 50);
			this->buttonTemplate->Name = L"buttonTemplate";
			this->buttonTemplate->Size = System::Drawing::Size(125, 52);
			this->buttonTemplate->TabIndex = 25;
			this->buttonTemplate->Text = L"NEW TEMPLATE";
			this->buttonTemplate->UseVisualStyleBackColor = true;
			this->buttonTemplate->Click += gcnew System::EventHandler(this, &MainForm::buttonTemplate_Click);
			// 
			// storeButton
			// 
			this->storeButton->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->storeButton->ForeColor = System::Drawing::Color::Black;
			this->storeButton->Location = System::Drawing::Point(390, 134);
			this->storeButton->Name = L"storeButton";
			this->storeButton->Size = System::Drawing::Size(125, 45);
			this->storeButton->TabIndex = 21;
			this->storeButton->Text = L"STORE-CONFIG";
			this->storeButton->UseVisualStyleBackColor = true;
			this->storeButton->Click += gcnew System::EventHandler(this, &MainForm::storeButton_Click);
			// 
			// generateButton
			// 
			this->generateButton->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->generateButton->ForeColor = System::Drawing::Color::Black;
			this->generateButton->Location = System::Drawing::Point(625, 563);
			this->generateButton->Name = L"generateButton";
			this->generateButton->Size = System::Drawing::Size(125, 52);
			this->generateButton->TabIndex = 24;
			this->generateButton->Text = L"GENERATE-DOC";
			this->generateButton->UseVisualStyleBackColor = true;
			this->generateButton->Click += gcnew System::EventHandler(this, &MainForm::generateButton_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->ClientSize = System::Drawing::Size(941, 714);
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
			this->groupBox5->ResumeLayout(false);
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
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

		// Load the Native and current list
		loadNativeMessageList();
		loadMessageList(comboLanguages->Text);
	}
	private: System::Void errorList_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		if ((itemArray == nullptr) || (itemArray->Length == 0)) return;

		for (int i = 0; i < itemArray->Length; i++) {
			if (itemArray[i]->msg_code->Equals(errorList->GetItemText(errorList->SelectedItem))) {
				listTag->SelectedIndex = listTag->FindStringExact(itemArray[i]->msg_tag);
				selectedError->Text = itemArray[errorList->SelectedIndex]->msg_code;
				labelSelectedTag->Text = itemArray[errorList->SelectedIndex]->msg_tag;
				selectedTitle->Text = itemArray[errorList->SelectedIndex]->msg_title;
				selectedContent->Text = itemArray[errorList->SelectedIndex]->msg_content;

				int p = System::Array::IndexOf(config->tags, labelSelectedTag->Text);
				if (p >= 0) {
					descriptionBox->Enabled = true;
					descriptionBox->Clear();
					descriptionBox->Text = config->descriptions[p];

				}
				return;
			}
		}
		
	}
	private: System::Void listTag_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		if ((itemArray == nullptr) || (itemArray->Length == 0)) {
			selectedError->Text = "-";
			labelSelectedTag->Text = listTag->SelectedItem->ToString();
			selectedTitle->Text = "-";
			selectedContent->Text = "-";

			descriptionBox->Enabled = false;
			descriptionBox->Clear();
			return;
		}

		

		for (int i = 0; i < itemArray->Length; i++) {
			if (itemArray[i]->msg_tag->Equals(listTag->GetItemText(listTag->SelectedItem))) {
				errorList->SelectedIndex = errorList->FindStringExact(itemArray[i]->msg_code);

				selectedError->Text = itemArray[errorList->SelectedIndex]->msg_code;
				labelSelectedTag->Text = itemArray[errorList->SelectedIndex]->msg_tag;
				selectedTitle->Text = itemArray[errorList->SelectedIndex]->msg_title;
				selectedContent->Text = itemArray[errorList->SelectedIndex]->msg_content;

				int p = System::Array::IndexOf(config->tags, labelSelectedTag->Text);
				if (p >= 0) {
					descriptionBox->Enabled = true;
					descriptionBox->Clear();
					descriptionBox->Text = config->descriptions[p];

				}
				return;
			}
		}

	}
	private: System::Void comboLanguages_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		currentLanguage = comboLanguages->Text;
		if (tagsArray == nullptr) return;
		loadMessageList(comboLanguages->Text);
	}

	/// <summary>
	/// When a tag is selected, the related comment is displayed
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	/// <returns></returns>
	private: System::Void selectedTag_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		if (config == nullptr) return;

		int p = System::Array::IndexOf(config->tags, labelSelectedTag->Text);
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

		int p = System::Array::IndexOf(config->tags, labelSelectedTag->Text);
		if (p < 0) return;
		config->descriptions[p] = descriptionBox->Text;
	}

	private: System::Void exitButton_Click(System::Object^ sender, System::EventArgs^ e) {
		Application::Exit();
	}
	private: System::Void generateButton_Click(System::Object^ sender, System::EventArgs^ e) {
		GenerateDocumentation(dirBrowser->SelectedPath);
	}

	private: System::Void buttonTemplate_Click(System::Object^ sender, System::EventArgs^ e) {
		GenerateTemplate(dirBrowser->SelectedPath, true);
	}
	private: System::Void duplicateTemplateButton_Click(System::Object^ sender, System::EventArgs^ e) {
		GenerateTemplate(dirBrowser->SelectedPath, false);
	}
	
	private: System::Void exportButton_Click(System::Object^ sender, System::EventArgs^ e) {
		GenerateTranslatorFile(false);
	}
	
};

}