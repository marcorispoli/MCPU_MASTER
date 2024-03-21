#pragma once
#include <Windows.h>

/// <summary>
/// \defgroup ServiceGUI_Module Service Window Management Module
/// \ingroup GUI_Module Gantry GUI modules
/// This module implements the features of the Gantry Service Status
/// 
/// # Features
/// 
/// </summary>
/// 

using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class ServiceExposureTool : public System::Windows::Forms::Form
{
#define PANEL_WIDTH 600
#define PANEL_HIGH 880
#define PANEL_X 0
#define PANEL_Y 50

// Handles the Window status
public:	
	void dialog_open(Form^ p);
	void close(void);
	bool open_status;

	Form^ parent;
	HWND window;
	
	void initPanel(void);
	void timerManagement(void);
	static ServiceExposureTool^ panel = gcnew ServiceExposureTool(); // Pointer to the Form instance from the static member
	System::Timers::Timer^ serviceTimer;

	ServiceExposureTool(void)
	{
		panel = this;
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		formInitialization();
	}

	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~ServiceExposureTool()
	{
		if (components)
		{
			delete components;
		}
	}

// Common items for the service dialog
private:void formInitialization(void);
private: System::Windows::Forms::Label^ serviceMenuTitle;
private: System::Windows::Forms::Label^ labelDate;
private: System::Windows::Forms::Label^ labelInstallation;
private: System::Windows::Forms::Label^ labelTime;
private: System::Windows::Forms::PictureBox^ serviceCanc;

private: void onKvSelectionCallback(System::String^ value);
private: void onmAsSelectionCallback(System::String^ value);
private: bool ExposureToolXrayEna;

private: System::Windows::Forms::Panel^ exposureToolPanel;
private: System::Windows::Forms::Label^ kVSelection;
private: System::Windows::Forms::Label^ mAsSelection;
private: System::Windows::Forms::Label^ label5;
private: System::Windows::Forms::ComboBox^ filterSelection;
private: System::Windows::Forms::Label^ label4;
private: System::Windows::Forms::Label^ label3;
private: System::Windows::Forms::ComboBox^ gridSelection;
private: System::Windows::Forms::ComboBox^ focusSelection;
private: System::Windows::Forms::Label^ label2;
private: System::Windows::Forms::Label^ label1;
private: System::Windows::Forms::PictureBox^ enableXray;
private: System::Windows::Forms::RichTextBox^ exposureLog;


private:
	/// <summary>
	/// Required designer variable.
	/// </summary>
	System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	void InitializeComponent(void)
	{
		this->labelDate = (gcnew System::Windows::Forms::Label());
		this->labelInstallation = (gcnew System::Windows::Forms::Label());
		this->labelTime = (gcnew System::Windows::Forms::Label());		
		this->serviceCanc = (gcnew System::Windows::Forms::PictureBox());
		this->serviceMenuTitle = (gcnew System::Windows::Forms::Label());
		this->exposureToolPanel = (gcnew System::Windows::Forms::Panel());
		this->exposureLog = (gcnew System::Windows::Forms::RichTextBox());
		this->enableXray = (gcnew System::Windows::Forms::PictureBox());
		this->label5 = (gcnew System::Windows::Forms::Label());
		this->filterSelection = (gcnew System::Windows::Forms::ComboBox());
		this->label4 = (gcnew System::Windows::Forms::Label());
		this->label3 = (gcnew System::Windows::Forms::Label());
		this->gridSelection = (gcnew System::Windows::Forms::ComboBox());
		this->focusSelection = (gcnew System::Windows::Forms::ComboBox());
		this->label2 = (gcnew System::Windows::Forms::Label());
		this->label1 = (gcnew System::Windows::Forms::Label());
		this->mAsSelection = (gcnew System::Windows::Forms::Label());
		this->kVSelection = (gcnew System::Windows::Forms::Label());		
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceCanc))->BeginInit();
		this->exposureToolPanel->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->enableXray))->BeginInit();
		this->SuspendLayout();
		// 
		// labelDate
		// 
		this->labelDate->AutoSize = true;
		this->labelDate->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelDate->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->labelDate->Location = System::Drawing::Point(5, 984);
		this->labelDate->Name = L"labelDate";
		this->labelDate->Size = System::Drawing::Size(100, 24);
		this->labelDate->TabIndex = 7;
		this->labelDate->Text = L"27/10/2023";
		// 
		// labelInstallation
		// 
		this->labelInstallation->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular,
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->labelInstallation->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)), static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->labelInstallation->Location = System::Drawing::Point(5, 956);
		this->labelInstallation->Name = L"labelInstallation";
		this->labelInstallation->Size = System::Drawing::Size(476, 24);
		this->labelInstallation->TabIndex = 8;
		this->labelInstallation->Text = L"IFO SAN GALLICANO";
		this->labelInstallation->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// labelTime
		// 
		this->labelTime->AutoSize = true;
		this->labelTime->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelTime->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->labelTime->Location = System::Drawing::Point(111, 984);
		this->labelTime->Name = L"labelTime";
		this->labelTime->Size = System::Drawing::Size(88, 24);
		this->labelTime->TabIndex = 9;
		this->labelTime->Text = L"10:35 PM";		
		// 
		// serviceCanc
		// 
		this->serviceCanc->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->serviceCanc->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->serviceCanc->Location = System::Drawing::Point(498, 936);
		this->serviceCanc->Name = L"serviceCanc";
		this->serviceCanc->Size = System::Drawing::Size(99, 85);
		this->serviceCanc->TabIndex = 19;
		this->serviceCanc->TabStop = false;
		this->serviceCanc->Click += gcnew System::EventHandler(this, &ServiceExposureTool::cancButton_Click);
		// 
		// serviceMenuTitle
		// 
		this->serviceMenuTitle->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20.25F, System::Drawing::FontStyle::Regular,
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->serviceMenuTitle->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)), static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->serviceMenuTitle->Location = System::Drawing::Point(3, 9);
		this->serviceMenuTitle->Name = L"serviceMenuTitle";
		this->serviceMenuTitle->Size = System::Drawing::Size(594, 36);
		this->serviceMenuTitle->TabIndex = 20;
		this->serviceMenuTitle->Text = L"TITLE";
		this->serviceMenuTitle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// exposureToolPanel
		// 
		this->exposureToolPanel->Controls->Add(this->exposureLog);
		this->exposureToolPanel->Controls->Add(this->enableXray);
		this->exposureToolPanel->Controls->Add(this->label5);
		this->exposureToolPanel->Controls->Add(this->filterSelection);
		this->exposureToolPanel->Controls->Add(this->label4);
		this->exposureToolPanel->Controls->Add(this->label3);
		this->exposureToolPanel->Controls->Add(this->gridSelection);
		this->exposureToolPanel->Controls->Add(this->focusSelection);
		this->exposureToolPanel->Controls->Add(this->label2);
		this->exposureToolPanel->Controls->Add(this->label1);
		this->exposureToolPanel->Controls->Add(this->mAsSelection);
		this->exposureToolPanel->Controls->Add(this->kVSelection);
		this->exposureToolPanel->Location = System::Drawing::Point(0, 50);
		this->exposureToolPanel->Name = L"exposureToolPanel";
		this->exposureToolPanel->Size = System::Drawing::Size(600, 880);
		this->exposureToolPanel->TabIndex = 22;
		// 
		// exposureLog
		// 
		this->exposureLog->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)), static_cast<System::Int32>(static_cast<System::Byte>(207)),
			static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->exposureLog->BorderStyle = System::Windows::Forms::BorderStyle::None;
		this->exposureLog->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->exposureLog->Location = System::Drawing::Point(12, 621);
		this->exposureLog->Name = L"exposureLog";
		this->exposureLog->ReadOnly = true;
		this->exposureLog->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
		this->exposureLog->ShortcutsEnabled = false;
		this->exposureLog->Size = System::Drawing::Size(576, 220);
		this->exposureLog->TabIndex = 33;
		this->exposureLog->Text = L"prova ";
		// 
		// enableXray
		// 
		this->enableXray->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->enableXray->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->enableXray->Location = System::Drawing::Point(323, 438);
		this->enableXray->Name = L"enableXray";
		this->enableXray->Size = System::Drawing::Size(160, 160);
		this->enableXray->TabIndex = 32;
		this->enableXray->TabStop = false;
		this->enableXray->Click += gcnew System::EventHandler(this, &ServiceExposureTool::enableXray_Click);
		// 
		// label5
		// 
		this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->label5->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->label5->Location = System::Drawing::Point(77, 401);
		this->label5->Name = L"label5";
		this->label5->Size = System::Drawing::Size(178, 36);
		this->label5->TabIndex = 31;
		this->label5->Text = L"Filter";
		this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// filterSelection
		// 
		this->filterSelection->BackColor = System::Drawing::Color::White;
		this->filterSelection->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 48, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->filterSelection->ForeColor = System::Drawing::Color::Black;
		this->filterSelection->FormattingEnabled = true;
		this->filterSelection->Items->AddRange(gcnew cli::array< System::Object^  >(5) { L"Rh", L"Mo", L"Al", L"Cu", L"Ag" });
		this->filterSelection->Location = System::Drawing::Point(83, 438);
		this->filterSelection->Name = L"filterSelection";
		this->filterSelection->Size = System::Drawing::Size(179, 81);
		this->filterSelection->TabIndex = 30;
		this->filterSelection->Text = L"Ag";
		// 
		// label4
		// 
		this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->label4->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->label4->Location = System::Drawing::Point(79, 275);
		this->label4->Name = L"label4";
		this->label4->Size = System::Drawing::Size(178, 36);
		this->label4->TabIndex = 29;
		this->label4->Text = L"Grid";
		this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// label3
		// 
		this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->label3->Location = System::Drawing::Point(79, 151);
		this->label3->Name = L"label3";
		this->label3->Size = System::Drawing::Size(178, 36);
		this->label3->TabIndex = 28;
		this->label3->Text = L"Focus";
		this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// gridSelection
		// 
		this->gridSelection->BackColor = System::Drawing::Color::White;
		this->gridSelection->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 48, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->gridSelection->ForeColor = System::Drawing::Color::Black;
		this->gridSelection->FormattingEnabled = true;
		this->gridSelection->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"GRID", L"NO GRID" });
		this->gridSelection->Location = System::Drawing::Point(85, 314);
		this->gridSelection->Name = L"gridSelection";
		this->gridSelection->Size = System::Drawing::Size(398, 81);
		this->gridSelection->TabIndex = 27;
		this->gridSelection->Text = L"NO GRID";
		// 
		// focusSelection
		// 
		this->focusSelection->BackColor = System::Drawing::Color::White;
		this->focusSelection->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 48, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->focusSelection->ForeColor = System::Drawing::Color::Black;
		this->focusSelection->FormattingEnabled = true;
		this->focusSelection->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"SMALL", L"LARGE" });
		this->focusSelection->Location = System::Drawing::Point(85, 190);
		this->focusSelection->Name = L"focusSelection";
		this->focusSelection->Size = System::Drawing::Size(398, 81);
		this->focusSelection->TabIndex = 26;
		this->focusSelection->Text = L"SMALL";
		// 
		// label2
		// 
		this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->label2->Location = System::Drawing::Point(303, 31);
		this->label2->Name = L"label2";
		this->label2->Size = System::Drawing::Size(178, 36);
		this->label2->TabIndex = 25;
		this->label2->Text = L"mAs";
		this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// label1
		// 
		this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->label1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->label1->Location = System::Drawing::Point(79, 31);
		this->label1->Name = L"label1";
		this->label1->Size = System::Drawing::Size(178, 36);
		this->label1->TabIndex = 24;
		this->label1->Text = L"kV";
		this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// mAsSelection
		// 
		this->mAsSelection->BackColor = System::Drawing::Color::White;
		this->mAsSelection->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 48, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->mAsSelection->ForeColor = System::Drawing::Color::Black;
		this->mAsSelection->Location = System::Drawing::Point(304, 70);
		this->mAsSelection->Name = L"mAsSelection";
		this->mAsSelection->Size = System::Drawing::Size(177, 70);
		this->mAsSelection->TabIndex = 23;
		this->mAsSelection->Text = L"120";
		this->mAsSelection->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		this->mAsSelection->Click += gcnew System::EventHandler(this, &ServiceExposureTool::mAsSelection_Click);
		// 
		// kVSelection
		// 
		this->kVSelection->BackColor = System::Drawing::Color::White;
		this->kVSelection->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 48, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->kVSelection->ForeColor = System::Drawing::Color::Black;
		this->kVSelection->Location = System::Drawing::Point(85, 70);
		this->kVSelection->Name = L"kVSelection";
		this->kVSelection->Size = System::Drawing::Size(177, 70);
		this->kVSelection->TabIndex = 22;
		this->kVSelection->Text = L"35";
		this->kVSelection->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		this->kVSelection->Click += gcnew System::EventHandler(this, &ServiceExposureTool::kvSelection_Click);
		// 
		// ServiceExposureTool
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
		this->Controls->Add(this->exposureToolPanel);
		this->Controls->Add(this->serviceMenuTitle);
		this->Controls->Add(this->labelInstallation);
		this->Controls->Add(this->labelDate);
		this->Controls->Add(this->labelTime);
		this->Controls->Add(this->serviceCanc);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Location = System::Drawing::Point(1000, 0);
		this->Name = L"ServiceForm";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;		
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceCanc))->EndInit();
		this->exposureToolPanel->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->enableXray))->EndInit();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	// Common Panel Buttons
	private: System::Void onServiceTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e);
	private: System::Void cancButton_Click(System::Object^ sender, System::EventArgs^ e);

	// ExposureTool Button Callbacks
	private: System::Void kvSelection_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void mAsSelection_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void enableXray_Click(System::Object^ sender, System::EventArgs^ e);
};

