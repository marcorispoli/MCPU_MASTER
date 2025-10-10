#pragma once
#include <Windows.h>

/// <summary>
/// \defgroup ServiceGUI_Module Collimator Test And Calibration
/// \ingroup GUI_Module Gantry GUI modules
/// This module implements the GUI to handle the collimator device
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

public ref class ServiceCollimatorTool : public System::Windows::Forms::Form
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
private: System::Windows::Forms::ComboBox^ FormatSelectionBox;
public:

public:

public:

	
	public:

	private: System::Windows::Forms::Label^ label6;
private: System::Windows::Forms::Label^ label2;





private: System::Windows::Forms::Label^ label1;
private: System::Windows::Forms::Label^ label4;


private: System::Windows::Forms::PictureBox^ ActivateFilterSlot;

private: System::Windows::Forms::Label^ label8;
private: System::Windows::Forms::ComboBox^ FilterSlotSelectionBox;


private: System::Windows::Forms::Label^ label14;
private: System::Windows::Forms::Label^ RightField;

private: System::Windows::Forms::Label^ label12;
private: System::Windows::Forms::Label^ LeftField;

private: System::Windows::Forms::Label^ label10;
private: System::Windows::Forms::Label^ FrontField;

private: System::Windows::Forms::Label^ label9;
private: System::Windows::Forms::Label^ BackField;


private: System::Windows::Forms::Label^ label16;
private: System::Windows::Forms::Label^ SlotSteps;

private: System::Windows::Forms::Label^ label18;
private: System::Windows::Forms::Label^ label19;
private: System::Windows::Forms::PictureBox^ MirrorInButton;

private: System::Windows::Forms::Label^ label21;
private: System::Windows::Forms::PictureBox^ LightOffButton;

private: System::Windows::Forms::Label^ label22;
private: System::Windows::Forms::PictureBox^ LightOnButton;

private: System::Windows::Forms::Label^ label20;
private: System::Windows::Forms::PictureBox^ MirrorOutButton;

private: System::Windows::Forms::Label^ label23;
private: System::Windows::Forms::Label^ TubeTemperatureField;
private: System::Windows::Forms::ComboBox^ AssignedPaddleBox;
private: System::Windows::Forms::ComboBox^ FilterNameSelectionBox;
private: System::Windows::Forms::Label^ label5;
private: System::Windows::Forms::Label^ TrapField;
private: System::Windows::Forms::Label^ label7;
private: System::Windows::Forms::ComboBox^ FormatOperatingModeBox;
private: System::Windows::Forms::Label^ label13;
private: System::Windows::Forms::Label^ CurrentFormatField;
private: System::Windows::Forms::Label^ label15;
private: System::Windows::Forms::Label^ CurrentPaddleField;
private: System::Windows::Forms::Panel^ ServiceFormatPanel;




public:
	HWND window;
	
	void initPanel(void);
	void timerManagement(void);
	static ServiceCollimatorTool^ panel = gcnew ServiceCollimatorTool(); // Class auto generation
	System::Timers::Timer^ serviceTimer;

	ServiceCollimatorTool(void)
	{
		panel = this;
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		formInitialization();
	}

	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~ServiceCollimatorTool()
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


private: bool ExposureToolXrayEna;
private: System::Windows::Forms::Panel^ mainForm;













private: System::Windows::Forms::RichTextBox^ collimatorDeviceLog;



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
        this->mainForm = (gcnew System::Windows::Forms::Panel());
        this->ServiceFormatPanel = (gcnew System::Windows::Forms::Panel());
        this->AssignedPaddleBox = (gcnew System::Windows::Forms::ComboBox());
        this->FormatSelectionBox = (gcnew System::Windows::Forms::ComboBox());
        this->label6 = (gcnew System::Windows::Forms::Label());
        this->label2 = (gcnew System::Windows::Forms::Label());
        this->label15 = (gcnew System::Windows::Forms::Label());
        this->CurrentPaddleField = (gcnew System::Windows::Forms::Label());
        this->label13 = (gcnew System::Windows::Forms::Label());
        this->CurrentFormatField = (gcnew System::Windows::Forms::Label());
        this->label7 = (gcnew System::Windows::Forms::Label());
        this->FormatOperatingModeBox = (gcnew System::Windows::Forms::ComboBox());
        this->label5 = (gcnew System::Windows::Forms::Label());
        this->TrapField = (gcnew System::Windows::Forms::Label());
        this->FilterNameSelectionBox = (gcnew System::Windows::Forms::ComboBox());
        this->label23 = (gcnew System::Windows::Forms::Label());
        this->TubeTemperatureField = (gcnew System::Windows::Forms::Label());
        this->label21 = (gcnew System::Windows::Forms::Label());
        this->LightOffButton = (gcnew System::Windows::Forms::PictureBox());
        this->label22 = (gcnew System::Windows::Forms::Label());
        this->LightOnButton = (gcnew System::Windows::Forms::PictureBox());
        this->label20 = (gcnew System::Windows::Forms::Label());
        this->MirrorOutButton = (gcnew System::Windows::Forms::PictureBox());
        this->label19 = (gcnew System::Windows::Forms::Label());
        this->MirrorInButton = (gcnew System::Windows::Forms::PictureBox());
        this->label18 = (gcnew System::Windows::Forms::Label());
        this->label16 = (gcnew System::Windows::Forms::Label());
        this->SlotSteps = (gcnew System::Windows::Forms::Label());
        this->label14 = (gcnew System::Windows::Forms::Label());
        this->RightField = (gcnew System::Windows::Forms::Label());
        this->label12 = (gcnew System::Windows::Forms::Label());
        this->LeftField = (gcnew System::Windows::Forms::Label());
        this->label10 = (gcnew System::Windows::Forms::Label());
        this->FrontField = (gcnew System::Windows::Forms::Label());
        this->label9 = (gcnew System::Windows::Forms::Label());
        this->BackField = (gcnew System::Windows::Forms::Label());
        this->label1 = (gcnew System::Windows::Forms::Label());
        this->label4 = (gcnew System::Windows::Forms::Label());
        this->ActivateFilterSlot = (gcnew System::Windows::Forms::PictureBox());
        this->label8 = (gcnew System::Windows::Forms::Label());
        this->FilterSlotSelectionBox = (gcnew System::Windows::Forms::ComboBox());
        this->collimatorDeviceLog = (gcnew System::Windows::Forms::RichTextBox());
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceCanc))->BeginInit();
        this->mainForm->SuspendLayout();
        this->ServiceFormatPanel->SuspendLayout();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->LightOffButton))->BeginInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->LightOnButton))->BeginInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->MirrorOutButton))->BeginInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->MirrorInButton))->BeginInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ActivateFilterSlot))->BeginInit();
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
        this->serviceCanc->Click += gcnew System::EventHandler(this, &ServiceCollimatorTool::cancButton_Click);
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
        // mainForm
        // 
        this->mainForm->Controls->Add(this->ServiceFormatPanel);
        this->mainForm->Controls->Add(this->label15);
        this->mainForm->Controls->Add(this->CurrentPaddleField);
        this->mainForm->Controls->Add(this->label13);
        this->mainForm->Controls->Add(this->CurrentFormatField);
        this->mainForm->Controls->Add(this->label7);
        this->mainForm->Controls->Add(this->FormatOperatingModeBox);
        this->mainForm->Controls->Add(this->label5);
        this->mainForm->Controls->Add(this->TrapField);
        this->mainForm->Controls->Add(this->FilterNameSelectionBox);
        this->mainForm->Controls->Add(this->label23);
        this->mainForm->Controls->Add(this->TubeTemperatureField);
        this->mainForm->Controls->Add(this->label21);
        this->mainForm->Controls->Add(this->LightOffButton);
        this->mainForm->Controls->Add(this->label22);
        this->mainForm->Controls->Add(this->LightOnButton);
        this->mainForm->Controls->Add(this->label20);
        this->mainForm->Controls->Add(this->MirrorOutButton);
        this->mainForm->Controls->Add(this->label19);
        this->mainForm->Controls->Add(this->MirrorInButton);
        this->mainForm->Controls->Add(this->label18);
        this->mainForm->Controls->Add(this->label16);
        this->mainForm->Controls->Add(this->SlotSteps);
        this->mainForm->Controls->Add(this->label14);
        this->mainForm->Controls->Add(this->RightField);
        this->mainForm->Controls->Add(this->label12);
        this->mainForm->Controls->Add(this->LeftField);
        this->mainForm->Controls->Add(this->label10);
        this->mainForm->Controls->Add(this->FrontField);
        this->mainForm->Controls->Add(this->label9);
        this->mainForm->Controls->Add(this->BackField);
        this->mainForm->Controls->Add(this->label1);
        this->mainForm->Controls->Add(this->label4);
        this->mainForm->Controls->Add(this->ActivateFilterSlot);
        this->mainForm->Controls->Add(this->label8);
        this->mainForm->Controls->Add(this->FilterSlotSelectionBox);
        this->mainForm->Controls->Add(this->collimatorDeviceLog);
        this->mainForm->Location = System::Drawing::Point(0, 50);
        this->mainForm->Name = L"mainForm";
        this->mainForm->Size = System::Drawing::Size(600, 880);
        this->mainForm->TabIndex = 22;
        // 
        // ServiceFormatPanel
        // 
        this->ServiceFormatPanel->Controls->Add(this->AssignedPaddleBox);
        this->ServiceFormatPanel->Controls->Add(this->FormatSelectionBox);
        this->ServiceFormatPanel->Controls->Add(this->label6);
        this->ServiceFormatPanel->Controls->Add(this->label2);
        this->ServiceFormatPanel->Location = System::Drawing::Point(0, 127);
        this->ServiceFormatPanel->Name = L"ServiceFormatPanel";
        this->ServiceFormatPanel->Size = System::Drawing::Size(600, 86);
        this->ServiceFormatPanel->TabIndex = 77;
        // 
        // AssignedPaddleBox
        // 
        this->AssignedPaddleBox->BackColor = System::Drawing::Color::White;
        this->AssignedPaddleBox->Cursor = System::Windows::Forms::Cursors::No;
        this->AssignedPaddleBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->AssignedPaddleBox->ForeColor = System::Drawing::Color::Black;
        this->AssignedPaddleBox->FormattingEnabled = true;
        this->AssignedPaddleBox->Items->AddRange(gcnew cli::array< System::Object^  >(21) {
            L"FORMAT - 0 ", L"FORMAT - 1", L"FORMAT - 2",
                L"FORMAT - 3", L"FORMAT - 4", L"FORMAT - 5", L"FORMAT - 6", L"FORMAT - 7", L"FORMAT - 8", L"FORMAT - 9", L"FORMAT - 10", L"FORMAT - 11",
                L"FORMAT - 12", L"FORMAT - 13", L"FORMAT - 14", L"FORMAT - 15", L"FORMAT - 16", L"FORMAT - 17", L"FORMAT - 18", L"FORMAT - 19",
                L"       "
        });
        this->AssignedPaddleBox->Location = System::Drawing::Point(13, 36);
        this->AssignedPaddleBox->Name = L"AssignedPaddleBox";
        this->AssignedPaddleBox->Size = System::Drawing::Size(328, 28);
        this->AssignedPaddleBox->TabIndex = 67;
        this->AssignedPaddleBox->Text = L"PADDLE";
        this->AssignedPaddleBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ServiceCollimatorTool::AssignedPaddleBox_SelectedIndexChanged);
        // 
        // FormatSelectionBox
        // 
        this->FormatSelectionBox->BackColor = System::Drawing::Color::White;
        this->FormatSelectionBox->Cursor = System::Windows::Forms::Cursors::No;
        this->FormatSelectionBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->FormatSelectionBox->ForeColor = System::Drawing::Color::Black;
        this->FormatSelectionBox->FormattingEnabled = true;
        this->FormatSelectionBox->Items->AddRange(gcnew cli::array< System::Object^  >(21) {
            L"FORMAT - 0 ", L"FORMAT - 1", L"FORMAT - 2",
                L"FORMAT - 3", L"FORMAT - 4", L"FORMAT - 5", L"FORMAT - 6", L"FORMAT - 7", L"FORMAT - 8", L"FORMAT - 9", L"FORMAT - 10", L"FORMAT - 11",
                L"FORMAT - 12", L"FORMAT - 13", L"FORMAT - 14", L"FORMAT - 15", L"FORMAT - 16", L"FORMAT - 17", L"FORMAT - 18", L"FORMAT - 19",
                L"       "
        });
        this->FormatSelectionBox->Location = System::Drawing::Point(375, 36);
        this->FormatSelectionBox->Name = L"FormatSelectionBox";
        this->FormatSelectionBox->Size = System::Drawing::Size(211, 28);
        this->FormatSelectionBox->TabIndex = 34;
        this->FormatSelectionBox->Text = L"FORMAT - 0 ";
        this->FormatSelectionBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ServiceCollimatorTool::FormatSelectionBox_SelectedIndexChanged);
        // 
        // label6
        // 
        this->label6->Cursor = System::Windows::Forms::Cursors::Default;
        this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label6->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label6->Location = System::Drawing::Point(371, 12);
        this->label6->Name = L"label6";
        this->label6->Size = System::Drawing::Size(217, 21);
        this->label6->TabIndex = 35;
        this->label6->Text = L"Select Collimation By Format";
        this->label6->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // label2
        // 
        this->label2->Cursor = System::Windows::Forms::Cursors::Default;
        this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label2->Location = System::Drawing::Point(9, 12);
        this->label2->Name = L"label2";
        this->label2->Size = System::Drawing::Size(227, 21);
        this->label2->TabIndex = 38;
        this->label2->Text = L"Select Collimation By Paddle";
        this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // label15
        // 
        this->label15->Cursor = System::Windows::Forms::Cursors::Default;
        this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label15->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label15->Location = System::Drawing::Point(176, 78);
        this->label15->Name = L"label15";
        this->label15->Size = System::Drawing::Size(152, 22);
        this->label15->TabIndex = 76;
        this->label15->Text = L"Current Paddle";
        this->label15->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // CurrentPaddleField
        // 
        this->CurrentPaddleField->BackColor = System::Drawing::Color::Silver;
        this->CurrentPaddleField->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
        this->CurrentPaddleField->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->CurrentPaddleField->ForeColor = System::Drawing::Color::Black;
        this->CurrentPaddleField->Location = System::Drawing::Point(179, 100);
        this->CurrentPaddleField->Name = L"CurrentPaddleField";
        this->CurrentPaddleField->Size = System::Drawing::Size(233, 24);
        this->CurrentPaddleField->TabIndex = 75;
        this->CurrentPaddleField->Text = L"PADDLE-NAME";
        this->CurrentPaddleField->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // label13
        // 
        this->label13->Cursor = System::Windows::Forms::Cursors::Default;
        this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label13->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label13->Location = System::Drawing::Point(11, 78);
        this->label13->Name = L"label13";
        this->label13->Size = System::Drawing::Size(152, 22);
        this->label13->TabIndex = 74;
        this->label13->Text = L"Current Format";
        this->label13->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // CurrentFormatField
        // 
        this->CurrentFormatField->BackColor = System::Drawing::Color::Silver;
        this->CurrentFormatField->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
        this->CurrentFormatField->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->CurrentFormatField->ForeColor = System::Drawing::Color::Black;
        this->CurrentFormatField->Location = System::Drawing::Point(14, 100);
        this->CurrentFormatField->Name = L"CurrentFormatField";
        this->CurrentFormatField->Size = System::Drawing::Size(154, 24);
        this->CurrentFormatField->TabIndex = 73;
        this->CurrentFormatField->Text = L"FORMAT-0";
        this->CurrentFormatField->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // label7
        // 
        this->label7->Cursor = System::Windows::Forms::Cursors::Default;
        this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label7->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label7->Location = System::Drawing::Point(417, 15);
        this->label7->Name = L"label7";
        this->label7->Size = System::Drawing::Size(172, 27);
        this->label7->TabIndex = 72;
        this->label7->Text = L"Select Operating Mode";
        this->label7->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // FormatOperatingModeBox
        // 
        this->FormatOperatingModeBox->BackColor = System::Drawing::Color::White;
        this->FormatOperatingModeBox->Cursor = System::Windows::Forms::Cursors::No;
        this->FormatOperatingModeBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular,
            System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
        this->FormatOperatingModeBox->ForeColor = System::Drawing::Color::Black;
        this->FormatOperatingModeBox->FormattingEnabled = true;
        this->FormatOperatingModeBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"DISABLE-MODE", L"AUTO-MODE", L"SERVICE-MODE" });
        this->FormatOperatingModeBox->Location = System::Drawing::Point(421, 42);
        this->FormatOperatingModeBox->Name = L"FormatOperatingModeBox";
        this->FormatOperatingModeBox->Size = System::Drawing::Size(165, 28);
        this->FormatOperatingModeBox->TabIndex = 71;
        this->FormatOperatingModeBox->Text = L"DISABLED";
        this->FormatOperatingModeBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ServiceCollimatorTool::FormatOperatingModeBox_SelectedIndexChanged);
        // 
        // label5
        // 
        this->label5->Cursor = System::Windows::Forms::Cursors::Default;
        this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label5->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label5->Location = System::Drawing::Point(334, 15);
        this->label5->Name = L"label5";
        this->label5->Size = System::Drawing::Size(69, 27);
        this->label5->TabIndex = 70;
        this->label5->Text = L"Trap";
        this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // TrapField
        // 
        this->TrapField->BackColor = System::Drawing::Color::Silver;
        this->TrapField->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
        this->TrapField->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->TrapField->ForeColor = System::Drawing::Color::Black;
        this->TrapField->Location = System::Drawing::Point(334, 42);
        this->TrapField->Name = L"TrapField";
        this->TrapField->Size = System::Drawing::Size(74, 30);
        this->TrapField->TabIndex = 69;
        this->TrapField->Text = L"1000";
        this->TrapField->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // FilterNameSelectionBox
        // 
        this->FilterNameSelectionBox->BackColor = System::Drawing::Color::White;
        this->FilterNameSelectionBox->Cursor = System::Windows::Forms::Cursors::No;
        this->FilterNameSelectionBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Regular,
            System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
        this->FilterNameSelectionBox->ForeColor = System::Drawing::Color::Black;
        this->FilterNameSelectionBox->FormattingEnabled = true;
        this->FilterNameSelectionBox->Items->AddRange(gcnew cli::array< System::Object^  >(5) {
            L"SLOT - 0", L"SLOT - 1", L"SLOT - 2",
                L"SLOT - 3", L"SLOT - 4 "
        });
        this->FilterNameSelectionBox->Location = System::Drawing::Point(9, 415);
        this->FilterNameSelectionBox->Name = L"FilterNameSelectionBox";
        this->FilterNameSelectionBox->Size = System::Drawing::Size(253, 50);
        this->FilterNameSelectionBox->TabIndex = 68;
        this->FilterNameSelectionBox->Text = L"SLOT - 0";
        // 
        // label23
        // 
        this->label23->Cursor = System::Windows::Forms::Cursors::Default;
        this->label23->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label23->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label23->Location = System::Drawing::Point(9, 605);
        this->label23->Name = L"label23";
        this->label23->Size = System::Drawing::Size(227, 26);
        this->label23->TabIndex = 66;
        this->label23->Text = L"Tube Temperature";
        this->label23->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // TubeTemperatureField
        // 
        this->TubeTemperatureField->BackColor = System::Drawing::Color::Silver;
        this->TubeTemperatureField->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
        this->TubeTemperatureField->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Italic,
            System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
        this->TubeTemperatureField->ForeColor = System::Drawing::Color::Black;
        this->TubeTemperatureField->Location = System::Drawing::Point(12, 631);
        this->TubeTemperatureField->Name = L"TubeTemperatureField";
        this->TubeTemperatureField->Size = System::Drawing::Size(237, 40);
        this->TubeTemperatureField->TabIndex = 65;
        this->TubeTemperatureField->Text = L"35";
        this->TubeTemperatureField->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // label21
        // 
        this->label21->Cursor = System::Windows::Forms::Cursors::Default;
        this->label21->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label21->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label21->Location = System::Drawing::Point(465, 482);
        this->label21->Name = L"label21";
        this->label21->Size = System::Drawing::Size(124, 36);
        this->label21->TabIndex = 64;
        this->label21->Text = L"Light-OFF";
        this->label21->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // LightOffButton
        // 
        this->LightOffButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
            static_cast<System::Int32>(static_cast<System::Byte>(0)));
        this->LightOffButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
        this->LightOffButton->Location = System::Drawing::Point(468, 521);
        this->LightOffButton->Name = L"LightOffButton";
        this->LightOffButton->Size = System::Drawing::Size(118, 68);
        this->LightOffButton->TabIndex = 63;
        this->LightOffButton->TabStop = false;
        // 
        // label22
        // 
        this->label22->Cursor = System::Windows::Forms::Cursors::Default;
        this->label22->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label22->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label22->Location = System::Drawing::Point(347, 482);
        this->label22->Name = L"label22";
        this->label22->Size = System::Drawing::Size(111, 36);
        this->label22->TabIndex = 62;
        this->label22->Text = L"Light-ON";
        this->label22->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // LightOnButton
        // 
        this->LightOnButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
            static_cast<System::Int32>(static_cast<System::Byte>(0)));
        this->LightOnButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
        this->LightOnButton->Location = System::Drawing::Point(350, 521);
        this->LightOnButton->Name = L"LightOnButton";
        this->LightOnButton->Size = System::Drawing::Size(112, 68);
        this->LightOnButton->TabIndex = 61;
        this->LightOnButton->TabStop = false;
        // 
        // label20
        // 
        this->label20->Cursor = System::Windows::Forms::Cursors::Default;
        this->label20->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label20->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label20->Location = System::Drawing::Point(128, 491);
        this->label20->Name = L"label20";
        this->label20->Size = System::Drawing::Size(134, 27);
        this->label20->TabIndex = 60;
        this->label20->Text = L"Mirror-Out";
        this->label20->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // MirrorOutButton
        // 
        this->MirrorOutButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
            static_cast<System::Int32>(static_cast<System::Byte>(0)));
        this->MirrorOutButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
        this->MirrorOutButton->Location = System::Drawing::Point(131, 521);
        this->MirrorOutButton->Name = L"MirrorOutButton";
        this->MirrorOutButton->Size = System::Drawing::Size(118, 68);
        this->MirrorOutButton->TabIndex = 59;
        this->MirrorOutButton->TabStop = false;
        // 
        // label19
        // 
        this->label19->Cursor = System::Windows::Forms::Cursors::Default;
        this->label19->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label19->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label19->Location = System::Drawing::Point(10, 491);
        this->label19->Name = L"label19";
        this->label19->Size = System::Drawing::Size(111, 27);
        this->label19->TabIndex = 58;
        this->label19->Text = L"Mirror-In";
        this->label19->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // MirrorInButton
        // 
        this->MirrorInButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
            static_cast<System::Int32>(static_cast<System::Byte>(0)));
        this->MirrorInButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
        this->MirrorInButton->Location = System::Drawing::Point(13, 521);
        this->MirrorInButton->Name = L"MirrorInButton";
        this->MirrorInButton->Size = System::Drawing::Size(112, 68);
        this->MirrorInButton->TabIndex = 57;
        this->MirrorInButton->TabStop = false;
        // 
        // label18
        // 
        this->label18->Cursor = System::Windows::Forms::Cursors::Default;
        this->label18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label18->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label18->Location = System::Drawing::Point(14, 683);
        this->label18->Name = L"label18";
        this->label18->Size = System::Drawing::Size(398, 27);
        this->label18->TabIndex = 56;
        this->label18->Text = L"Device Status";
        this->label18->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // label16
        // 
        this->label16->Cursor = System::Windows::Forms::Cursors::Default;
        this->label16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label16->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label16->Location = System::Drawing::Point(277, 385);
        this->label16->Name = L"label16";
        this->label16->Size = System::Drawing::Size(89, 27);
        this->label16->TabIndex = 55;
        this->label16->Text = L"Steps";
        this->label16->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // SlotSteps
        // 
        this->SlotSteps->BackColor = System::Drawing::Color::Silver;
        this->SlotSteps->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
        this->SlotSteps->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->SlotSteps->ForeColor = System::Drawing::Color::Black;
        this->SlotSteps->Location = System::Drawing::Point(275, 415);
        this->SlotSteps->Name = L"SlotSteps";
        this->SlotSteps->Size = System::Drawing::Size(131, 50);
        this->SlotSteps->TabIndex = 54;
        this->SlotSteps->Text = L"35";
        this->SlotSteps->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // label14
        // 
        this->label14->Cursor = System::Windows::Forms::Cursors::Default;
        this->label14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label14->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label14->Location = System::Drawing::Point(254, 15);
        this->label14->Name = L"label14";
        this->label14->Size = System::Drawing::Size(69, 27);
        this->label14->TabIndex = 53;
        this->label14->Text = L"Right";
        this->label14->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // RightField
        // 
        this->RightField->BackColor = System::Drawing::Color::Silver;
        this->RightField->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
        this->RightField->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->RightField->ForeColor = System::Drawing::Color::Black;
        this->RightField->Location = System::Drawing::Point(254, 42);
        this->RightField->Name = L"RightField";
        this->RightField->Size = System::Drawing::Size(74, 30);
        this->RightField->TabIndex = 52;
        this->RightField->Text = L"1000";
        this->RightField->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // label12
        // 
        this->label12->Cursor = System::Windows::Forms::Cursors::Default;
        this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label12->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label12->Location = System::Drawing::Point(170, 15);
        this->label12->Name = L"label12";
        this->label12->Size = System::Drawing::Size(69, 27);
        this->label12->TabIndex = 51;
        this->label12->Text = L"Left";
        this->label12->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // LeftField
        // 
        this->LeftField->BackColor = System::Drawing::Color::Silver;
        this->LeftField->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
        this->LeftField->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->LeftField->ForeColor = System::Drawing::Color::Black;
        this->LeftField->Location = System::Drawing::Point(174, 42);
        this->LeftField->Name = L"LeftField";
        this->LeftField->Size = System::Drawing::Size(74, 30);
        this->LeftField->TabIndex = 50;
        this->LeftField->Text = L"1000";
        this->LeftField->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // label10
        // 
        this->label10->Cursor = System::Windows::Forms::Cursors::Default;
        this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label10->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label10->Location = System::Drawing::Point(94, 15);
        this->label10->Name = L"label10";
        this->label10->Size = System::Drawing::Size(69, 27);
        this->label10->TabIndex = 49;
        this->label10->Text = L"Front";
        this->label10->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // FrontField
        // 
        this->FrontField->BackColor = System::Drawing::Color::Silver;
        this->FrontField->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
        this->FrontField->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->FrontField->ForeColor = System::Drawing::Color::Black;
        this->FrontField->Location = System::Drawing::Point(94, 42);
        this->FrontField->Name = L"FrontField";
        this->FrontField->Size = System::Drawing::Size(74, 30);
        this->FrontField->TabIndex = 48;
        this->FrontField->Text = L"1000";
        this->FrontField->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // label9
        // 
        this->label9->Cursor = System::Windows::Forms::Cursors::Default;
        this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label9->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label9->Location = System::Drawing::Point(13, 15);
        this->label9->Name = L"label9";
        this->label9->Size = System::Drawing::Size(69, 27);
        this->label9->TabIndex = 47;
        this->label9->Text = L"Back";
        this->label9->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // BackField
        // 
        this->BackField->BackColor = System::Drawing::Color::Silver;
        this->BackField->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
        this->BackField->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->BackField->ForeColor = System::Drawing::Color::Black;
        this->BackField->Location = System::Drawing::Point(14, 42);
        this->BackField->Name = L"BackField";
        this->BackField->Size = System::Drawing::Size(74, 30);
        this->BackField->TabIndex = 46;
        this->BackField->Text = L"1000";
        this->BackField->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // label1
        // 
        this->label1->Cursor = System::Windows::Forms::Cursors::Default;
        this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label1->Location = System::Drawing::Point(423, 287);
        this->label1->Name = L"label1";
        this->label1->Size = System::Drawing::Size(98, 27);
        this->label1->TabIndex = 45;
        this->label1->Text = L"Activate";
        this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // label4
        // 
        this->label4->Cursor = System::Windows::Forms::Cursors::Default;
        this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label4->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label4->Location = System::Drawing::Point(9, 385);
        this->label4->Name = L"label4";
        this->label4->Size = System::Drawing::Size(282, 27);
        this->label4->TabIndex = 44;
        this->label4->Text = L"Select Filter By Name";
        this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // ActivateFilterSlot
        // 
        this->ActivateFilterSlot->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
            static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
        this->ActivateFilterSlot->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
        this->ActivateFilterSlot->Location = System::Drawing::Point(428, 317);
        this->ActivateFilterSlot->Name = L"ActivateFilterSlot";
        this->ActivateFilterSlot->Size = System::Drawing::Size(160, 150);
        this->ActivateFilterSlot->TabIndex = 42;
        this->ActivateFilterSlot->TabStop = false;
        // 
        // label8
        // 
        this->label8->Cursor = System::Windows::Forms::Cursors::Default;
        this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->label8->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
            static_cast<System::Int32>(static_cast<System::Byte>(149)));
        this->label8->Location = System::Drawing::Point(8, 301);
        this->label8->Name = L"label8";
        this->label8->Size = System::Drawing::Size(398, 27);
        this->label8->TabIndex = 41;
        this->label8->Text = L"Select Filter By Slot";
        this->label8->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        // 
        // FilterSlotSelectionBox
        // 
        this->FilterSlotSelectionBox->BackColor = System::Drawing::Color::White;
        this->FilterSlotSelectionBox->Cursor = System::Windows::Forms::Cursors::No;
        this->FilterSlotSelectionBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Regular,
            System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
        this->FilterSlotSelectionBox->ForeColor = System::Drawing::Color::Black;
        this->FilterSlotSelectionBox->FormattingEnabled = true;
        this->FilterSlotSelectionBox->Items->AddRange(gcnew cli::array< System::Object^  >(5) {
            L"SLOT - 0", L"SLOT - 1", L"SLOT - 2",
                L"SLOT - 3", L"SLOT - 4 "
        });
        this->FilterSlotSelectionBox->Location = System::Drawing::Point(9, 317);
        this->FilterSlotSelectionBox->Name = L"FilterSlotSelectionBox";
        this->FilterSlotSelectionBox->Size = System::Drawing::Size(398, 50);
        this->FilterSlotSelectionBox->TabIndex = 40;
        this->FilterSlotSelectionBox->Text = L"SLOT - 0";
        // 
        // collimatorDeviceLog
        // 
        this->collimatorDeviceLog->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)),
            static_cast<System::Int32>(static_cast<System::Byte>(207)), static_cast<System::Int32>(static_cast<System::Byte>(208)));
        this->collimatorDeviceLog->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
            System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
        this->collimatorDeviceLog->Location = System::Drawing::Point(14, 713);
        this->collimatorDeviceLog->Name = L"collimatorDeviceLog";
        this->collimatorDeviceLog->ReadOnly = true;
        this->collimatorDeviceLog->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
        this->collimatorDeviceLog->ShortcutsEnabled = false;
        this->collimatorDeviceLog->Size = System::Drawing::Size(574, 164);
        this->collimatorDeviceLog->TabIndex = 33;
        this->collimatorDeviceLog->Text = L"Collimator Device Log";
        // 
        // ServiceCollimatorTool
        // 
        this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
            static_cast<System::Int32>(static_cast<System::Byte>(60)));
        this->ClientSize = System::Drawing::Size(600, 1024);
        this->ControlBox = false;
        this->Controls->Add(this->mainForm);
        this->Controls->Add(this->serviceMenuTitle);
        this->Controls->Add(this->labelInstallation);
        this->Controls->Add(this->labelDate);
        this->Controls->Add(this->labelTime);
        this->Controls->Add(this->serviceCanc);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
        this->Location = System::Drawing::Point(1000, 0);
        this->Name = L"ServiceCollimatorTool";
        this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceCanc))->EndInit();
        this->mainForm->ResumeLayout(false);
        this->ServiceFormatPanel->ResumeLayout(false);
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->LightOffButton))->EndInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->LightOnButton))->EndInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->MirrorOutButton))->EndInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->MirrorInButton))->EndInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ActivateFilterSlot))->EndInit();
        this->ResumeLayout(false);
        this->PerformLayout();

    }
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	// Common Panel Buttons
	private: System::Void onServiceTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e);
	private: System::Void cancButton_Click(System::Object^ sender, System::EventArgs^ e);

    private: System::Void FormatSelectionBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
    private: System::Void AssignedPaddleBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);

    private: System::Void FormatOperatingModeBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
};

