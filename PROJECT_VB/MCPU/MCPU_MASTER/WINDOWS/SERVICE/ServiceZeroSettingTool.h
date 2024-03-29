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

public ref class ServiceZeroSettingTool : public System::Windows::Forms::Form
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
private: System::Windows::Forms::Button^ manButton;
private: System::Windows::Forms::Label^ label2;
private: System::Windows::Forms::Label^ externalPosition;
private: System::Windows::Forms::Label^ sensorLabel;
public:

public:



public:
	HWND window;
	
	void initPanel(void);
	void timerManagement(void);
	static ServiceZeroSettingTool^ panel = gcnew ServiceZeroSettingTool(); // Pointer to the Form instance from the static member
	System::Timers::Timer^ serviceTimer;

	float max_target_angle;
	float min_target_angle;

	ServiceZeroSettingTool(void)
	{
		panel = this;
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		formInitialization();
	}

	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~ServiceZeroSettingTool()
	{
		if (components)
		{
			delete components;
		}
	}

	// Common items for the service dialog
	private:void formInitialization(void);
	private: void onTargetSelectionCallback(System::String^ value);
	private: void onBodyPanelTimerCallback(void);
	private: void onTiltPanelTimerCallback(void);
	private: void onVerticalPanelTimerCallback(void);
	private: void onSlidePanelTimerCallback(void);
	private: void onArmPanelTimerCallback(void);

	private: System::Windows::Forms::Label^ serviceMenuTitle;
	private: System::Windows::Forms::Label^ labelDate;
	private: System::Windows::Forms::Label^ labelInstallation;
	private: System::Windows::Forms::Label^ labelTime;
	private: System::Windows::Forms::PictureBox^ serviceCanc;
	private: System::Windows::Forms::Panel^ calibZerosettingPanel;
		private: System::Windows::Forms::PictureBox^ zeroSettingSlide;
		private: System::Windows::Forms::PictureBox^ zeroSettingTilt;
		private: System::Windows::Forms::PictureBox^ zeroSettingArm;
		private: System::Windows::Forms::PictureBox^ zeroSettingVertical;
		private: System::Windows::Forms::PictureBox^ zeroSettingBody;

		private: System::Windows::Forms::Panel^ executePanel;
			private: System::Windows::Forms::Label^ positionLabel;
private: System::Windows::Forms::Label^ encoderPosition;

			private: System::Windows::Forms::Label^ targetLabel;
			private: System::Windows::Forms::Label^ targetSelection;

			private: System::Windows::Forms::Label^ sensorValue;
			private: System::Windows::Forms::Button^ runButton;
			private: System::Windows::Forms::RichTextBox^ zeroSettingLog;
			private: System::Windows::Forms::PictureBox^ executeIcon;


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
		this->calibZerosettingPanel = (gcnew System::Windows::Forms::Panel());
		this->executePanel = (gcnew System::Windows::Forms::Panel());
		this->manButton = (gcnew System::Windows::Forms::Button());
		this->positionLabel = (gcnew System::Windows::Forms::Label());
		this->targetLabel = (gcnew System::Windows::Forms::Label());
		this->zeroSettingLog = (gcnew System::Windows::Forms::RichTextBox());
		this->executeIcon = (gcnew System::Windows::Forms::PictureBox());
		this->runButton = (gcnew System::Windows::Forms::Button());
		this->sensorValue = (gcnew System::Windows::Forms::Label());
		this->encoderPosition = (gcnew System::Windows::Forms::Label());
		this->targetSelection = (gcnew System::Windows::Forms::Label());
		this->zeroSettingSlide = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingTilt = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingArm = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingVertical = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingBody = (gcnew System::Windows::Forms::PictureBox());
		this->serviceCanc = (gcnew System::Windows::Forms::PictureBox());
		this->serviceMenuTitle = (gcnew System::Windows::Forms::Label());
		this->externalPosition = (gcnew System::Windows::Forms::Label());
		this->sensorLabel = (gcnew System::Windows::Forms::Label());
		this->label2 = (gcnew System::Windows::Forms::Label());
		this->calibZerosettingPanel->SuspendLayout();
		this->executePanel->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->executeIcon))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingSlide))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingTilt))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingArm))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingVertical))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingBody))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceCanc))->BeginInit();
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
		// calibZerosettingPanel
		// 
		this->calibZerosettingPanel->Controls->Add(this->executePanel);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingSlide);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingTilt);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingArm);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingVertical);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingBody);
		this->calibZerosettingPanel->Location = System::Drawing::Point(0, 50);
		this->calibZerosettingPanel->Name = L"calibZerosettingPanel";
		this->calibZerosettingPanel->Size = System::Drawing::Size(600, 880);
		this->calibZerosettingPanel->TabIndex = 16;
		// 
		// executePanel
		// 
		this->executePanel->Controls->Add(this->label2);
		this->executePanel->Controls->Add(this->manButton);
		this->executePanel->Controls->Add(this->positionLabel);
		this->executePanel->Controls->Add(this->externalPosition);
		this->executePanel->Controls->Add(this->sensorLabel);
		this->executePanel->Controls->Add(this->targetLabel);
		this->executePanel->Controls->Add(this->zeroSettingLog);
		this->executePanel->Controls->Add(this->executeIcon);
		this->executePanel->Controls->Add(this->runButton);
		this->executePanel->Controls->Add(this->sensorValue);
		this->executePanel->Controls->Add(this->encoderPosition);
		this->executePanel->Controls->Add(this->targetSelection);
		this->executePanel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)));
		this->executePanel->Location = System::Drawing::Point(40, 419);
		this->executePanel->Name = L"executePanel";
		this->executePanel->Size = System::Drawing::Size(531, 406);
		this->executePanel->TabIndex = 25;
		// 
		// manButton
		// 
		this->manButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(255)));
		this->manButton->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
		this->manButton->FlatAppearance->BorderSize = 5;
		this->manButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->manButton->Location = System::Drawing::Point(170, 194);
		this->manButton->Name = L"manButton";
		this->manButton->Size = System::Drawing::Size(351, 55);
		this->manButton->TabIndex = 36;
		this->manButton->Text = L"MAN";
		this->manButton->UseVisualStyleBackColor = false;
		this->manButton->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::manButton_Click);
		// 
		// positionLabel
		// 
		this->positionLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->positionLabel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->positionLabel->Location = System::Drawing::Point(163, 14);
		this->positionLabel->Name = L"positionLabel";
		this->positionLabel->Size = System::Drawing::Size(136, 24);
		this->positionLabel->TabIndex = 35;
		this->positionLabel->Text = L"Encoder Position";
		this->positionLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// targetLabel
		// 
		this->targetLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->targetLabel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->targetLabel->Location = System::Drawing::Point(166, 102);
		this->targetLabel->Name = L"targetLabel";
		this->targetLabel->Size = System::Drawing::Size(136, 21);
		this->targetLabel->TabIndex = 33;
		this->targetLabel->Text = L"Manual Position";
		this->targetLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// zeroSettingLog
		// 
		this->zeroSettingLog->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)), static_cast<System::Int32>(static_cast<System::Byte>(207)),
			static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->zeroSettingLog->BorderStyle = System::Windows::Forms::BorderStyle::None;
		this->zeroSettingLog->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->zeroSettingLog->Location = System::Drawing::Point(14, 266);
		this->zeroSettingLog->Name = L"zeroSettingLog";
		this->zeroSettingLog->ReadOnly = true;
		this->zeroSettingLog->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
		this->zeroSettingLog->ShortcutsEnabled = false;
		this->zeroSettingLog->Size = System::Drawing::Size(507, 116);
		this->zeroSettingLog->TabIndex = 32;
		this->zeroSettingLog->Text = L"";
		// 
		// executeIcon
		// 
		this->executeIcon->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->executeIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->executeIcon->Location = System::Drawing::Point(11, 38);
		this->executeIcon->Name = L"executeIcon";
		this->executeIcon->Size = System::Drawing::Size(140, 140);
		this->executeIcon->TabIndex = 26;
		this->executeIcon->TabStop = false;
		// 
		// runButton
		// 
		this->runButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(255)));
		this->runButton->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
		this->runButton->FlatAppearance->BorderSize = 5;
		this->runButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->runButton->Location = System::Drawing::Point(10, 194);
		this->runButton->Name = L"runButton";
		this->runButton->Size = System::Drawing::Size(145, 55);
		this->runButton->TabIndex = 31;
		this->runButton->Text = L"AUTO";
		this->runButton->UseVisualStyleBackColor = false;
		this->runButton->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::runButton_Click);
		// 
		// sensorValue
		// 
		this->sensorValue->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)), static_cast<System::Int32>(static_cast<System::Byte>(207)),
			static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->sensorValue->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->sensorValue->ForeColor = System::Drawing::Color::Gray;
		this->sensorValue->Location = System::Drawing::Point(351, 123);
		this->sensorValue->Name = L"sensorValue";
		this->sensorValue->Size = System::Drawing::Size(170, 55);
		this->sensorValue->TabIndex = 29;
		this->sensorValue->Text = L"1024";
		this->sensorValue->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// encoderPosition
		// 
		this->encoderPosition->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)), static_cast<System::Int32>(static_cast<System::Byte>(207)),
			static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->encoderPosition->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->encoderPosition->ForeColor = System::Drawing::Color::Gray;
		this->encoderPosition->Location = System::Drawing::Point(170, 38);
		this->encoderPosition->Name = L"encoderPosition";
		this->encoderPosition->Size = System::Drawing::Size(170, 55);
		this->encoderPosition->TabIndex = 25;
		this->encoderPosition->Text = L"1200";
		this->encoderPosition->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// targetSelection
		// 
		this->targetSelection->BackColor = System::Drawing::Color::White;
		this->targetSelection->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
		this->targetSelection->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->targetSelection->ForeColor = System::Drawing::Color::Black;
		this->targetSelection->Location = System::Drawing::Point(170, 123);
		this->targetSelection->Name = L"targetSelection";
		this->targetSelection->Size = System::Drawing::Size(170, 55);
		this->targetSelection->TabIndex = 24;
		this->targetSelection->Text = L"90";
		this->targetSelection->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		this->targetSelection->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::targetSelection_Click);
		// 
		// zeroSettingSlide
		// 
		this->zeroSettingSlide->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingSlide->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->zeroSettingSlide->Location = System::Drawing::Point(220, 236);
		this->zeroSettingSlide->Name = L"zeroSettingSlide";
		this->zeroSettingSlide->Size = System::Drawing::Size(165, 165);
		this->zeroSettingSlide->TabIndex = 23;
		this->zeroSettingSlide->TabStop = false;
		this->zeroSettingSlide->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::zeroSettingSlide_Click);
		// 
		// zeroSettingTilt
		// 
		this->zeroSettingTilt->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingTilt->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->zeroSettingTilt->Location = System::Drawing::Point(410, 40);
		this->zeroSettingTilt->Name = L"zeroSettingTilt";
		this->zeroSettingTilt->Size = System::Drawing::Size(165, 165);
		this->zeroSettingTilt->TabIndex = 22;
		this->zeroSettingTilt->TabStop = false;
		this->zeroSettingTilt->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::zeroSettingTilt_Click);
		// 
		// zeroSettingArm
		// 
		this->zeroSettingArm->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingArm->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->zeroSettingArm->Location = System::Drawing::Point(220, 40);
		this->zeroSettingArm->Name = L"zeroSettingArm";
		this->zeroSettingArm->Size = System::Drawing::Size(165, 165);
		this->zeroSettingArm->TabIndex = 21;
		this->zeroSettingArm->TabStop = false;
		this->zeroSettingArm->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::zeroSettingArm_Click);
		// 
		// zeroSettingVertical
		// 
		this->zeroSettingVertical->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingVertical->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->zeroSettingVertical->Location = System::Drawing::Point(30, 40);
		this->zeroSettingVertical->Name = L"zeroSettingVertical";
		this->zeroSettingVertical->Size = System::Drawing::Size(165, 165);
		this->zeroSettingVertical->TabIndex = 20;
		this->zeroSettingVertical->TabStop = false;
		this->zeroSettingVertical->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::zeroSettingVertical_Click);
		// 
		// zeroSettingBody
		// 
		this->zeroSettingBody->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingBody->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->zeroSettingBody->Location = System::Drawing::Point(30, 236);
		this->zeroSettingBody->Name = L"zeroSettingBody";
		this->zeroSettingBody->Size = System::Drawing::Size(165, 165);
		this->zeroSettingBody->TabIndex = 19;
		this->zeroSettingBody->TabStop = false;
		this->zeroSettingBody->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::zeroSettingBody_Click);
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
		this->serviceCanc->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::cancButton_Click);
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
		// externalPosition
		// 
		this->externalPosition->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)),
			static_cast<System::Int32>(static_cast<System::Byte>(207)), static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->externalPosition->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->externalPosition->ForeColor = System::Drawing::Color::Gray;
		this->externalPosition->Location = System::Drawing::Point(351, 38);
		this->externalPosition->Name = L"externalPosition";
		this->externalPosition->Size = System::Drawing::Size(170, 55);
		this->externalPosition->TabIndex = 37;
		this->externalPosition->Text = L"1024";
		this->externalPosition->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// sensorLabel
		// 
		this->sensorLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->sensorLabel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->sensorLabel->Location = System::Drawing::Point(347, 102);
		this->sensorLabel->Name = L"sensorLabel";
		this->sensorLabel->Size = System::Drawing::Size(164, 21);
		this->sensorLabel->TabIndex = 34;
		this->sensorLabel->Text = L"Digital Input";
		this->sensorLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// label2
		// 
		this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->label2->Location = System::Drawing::Point(347, 17);
		this->label2->Name = L"label2";
		this->label2->Size = System::Drawing::Size(164, 21);
		this->label2->TabIndex = 38;
		this->label2->Text = L"Sensor Position";
		this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// ServiceZeroSettingTool
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
		this->Controls->Add(this->serviceMenuTitle);
		this->Controls->Add(this->calibZerosettingPanel);
		this->Controls->Add(this->labelInstallation);
		this->Controls->Add(this->labelDate);
		this->Controls->Add(this->labelTime);
		this->Controls->Add(this->serviceCanc);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Location = System::Drawing::Point(1000, 0);
		this->Name = L"ServiceZeroSettingTool";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		this->calibZerosettingPanel->ResumeLayout(false);
		this->executePanel->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->executeIcon))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingSlide))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingTilt))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingArm))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingVertical))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingBody))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceCanc))->EndInit();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	// Common Panel Buttons
	private: System::Void onServiceTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e);
	private: System::Void cancButton_Click(System::Object^ sender, System::EventArgs^ e);

	// Calibration - Zero Setting Panel
	private: System::Void zeroSettingBody_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingVertical_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingArm_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingTilt_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingSlide_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void targetSelection_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void runButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void manButton_Click(System::Object^ sender, System::EventArgs^ e);


	
};

