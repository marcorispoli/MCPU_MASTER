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

public ref class ServicePaddleCalibrationTool : public System::Windows::Forms::Form
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
private: System::Windows::Forms::Label^ highPosition;
private: System::Windows::Forms::Label^ calibratedHolderOffset;
public:





private: System::Windows::Forms::Button^ calibrateHolder;

private: System::Windows::Forms::Button^ acceptLowPosition;

private: System::Windows::Forms::Label^ insertLowPosition;


private: System::Windows::Forms::Label^ label3;
private: System::Windows::Forms::Label^ label5;
private: System::Windows::Forms::Label^ label4;
private: System::Windows::Forms::Label^ calibratedHolderLinearity;
private: System::Windows::Forms::Label^ label2;
private: System::Windows::Forms::Label^ currentHolderPosition;
private: System::Windows::Forms::Button^ storeCalibratedParam;



public:

public:



public:
	HWND window;
	
	void initPanel(void);
	void timerManagement(void);
	static ServicePaddleCalibrationTool^ panel = gcnew ServicePaddleCalibrationTool(); // Pointer to the Form instance from the static member
	System::Timers::Timer^ serviceTimer;

	float max_target_angle;
	float min_target_angle;

	ServicePaddleCalibrationTool(void)
	{
		panel = this;
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		formInitialization();
	}

	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~ServicePaddleCalibrationTool()
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


	private: System::Windows::Forms::Label^ serviceMenuTitle;
	private: System::Windows::Forms::Label^ labelDate;
	private: System::Windows::Forms::Label^ labelInstallation;
	private: System::Windows::Forms::Label^ labelTime;
	private: System::Windows::Forms::PictureBox^ serviceCanc;
	private: System::Windows::Forms::Panel^ calibZerosettingPanel;

private: System::Windows::Forms::PictureBox^ paddleLimits;

private: System::Windows::Forms::PictureBox^ paddleForceCalibration;
private: System::Windows::Forms::PictureBox^ paddleHolderCalibration;
private: System::Windows::Forms::Panel^ holderPanel;






			private: System::Windows::Forms::Label^ positionLabel;

private: System::Windows::Forms::Label^ insertHighPosition;
private: System::Windows::Forms::Label^ lowPosition;






private: System::Windows::Forms::Button^ acceptHighPosition;


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
		this->holderPanel = (gcnew System::Windows::Forms::Panel());
		this->positionLabel = (gcnew System::Windows::Forms::Label());
		this->highPosition = (gcnew System::Windows::Forms::Label());
		this->executeIcon = (gcnew System::Windows::Forms::PictureBox());
		this->acceptHighPosition = (gcnew System::Windows::Forms::Button());
		this->lowPosition = (gcnew System::Windows::Forms::Label());
		this->insertHighPosition = (gcnew System::Windows::Forms::Label());
		this->paddleLimits = (gcnew System::Windows::Forms::PictureBox());
		this->paddleForceCalibration = (gcnew System::Windows::Forms::PictureBox());
		this->paddleHolderCalibration = (gcnew System::Windows::Forms::PictureBox());
		this->serviceCanc = (gcnew System::Windows::Forms::PictureBox());
		this->serviceMenuTitle = (gcnew System::Windows::Forms::Label());
		this->insertLowPosition = (gcnew System::Windows::Forms::Label());
		this->acceptLowPosition = (gcnew System::Windows::Forms::Button());
		this->calibrateHolder = (gcnew System::Windows::Forms::Button());
		this->calibratedHolderOffset = (gcnew System::Windows::Forms::Label());
		this->label3 = (gcnew System::Windows::Forms::Label());
		this->calibratedHolderLinearity = (gcnew System::Windows::Forms::Label());
		this->label4 = (gcnew System::Windows::Forms::Label());
		this->label5 = (gcnew System::Windows::Forms::Label());
		this->currentHolderPosition = (gcnew System::Windows::Forms::Label());
		this->label2 = (gcnew System::Windows::Forms::Label());
		this->storeCalibratedParam = (gcnew System::Windows::Forms::Button());
		this->calibZerosettingPanel->SuspendLayout();
		this->holderPanel->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->executeIcon))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->paddleLimits))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->paddleForceCalibration))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->paddleHolderCalibration))->BeginInit();
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
		this->calibZerosettingPanel->Controls->Add(this->holderPanel);
		this->calibZerosettingPanel->Controls->Add(this->paddleLimits);
		this->calibZerosettingPanel->Controls->Add(this->paddleForceCalibration);
		this->calibZerosettingPanel->Controls->Add(this->paddleHolderCalibration);
		this->calibZerosettingPanel->Location = System::Drawing::Point(0, 50);
		this->calibZerosettingPanel->Name = L"calibZerosettingPanel";
		this->calibZerosettingPanel->Size = System::Drawing::Size(600, 880);
		this->calibZerosettingPanel->TabIndex = 16;
		// 
		// holderPanel
		// 
		this->holderPanel->Controls->Add(this->storeCalibratedParam);
		this->holderPanel->Controls->Add(this->label2);
		this->holderPanel->Controls->Add(this->currentHolderPosition);
		this->holderPanel->Controls->Add(this->label5);
		this->holderPanel->Controls->Add(this->label4);
		this->holderPanel->Controls->Add(this->calibratedHolderLinearity);
		this->holderPanel->Controls->Add(this->label3);
		this->holderPanel->Controls->Add(this->calibratedHolderOffset);
		this->holderPanel->Controls->Add(this->calibrateHolder);
		this->holderPanel->Controls->Add(this->acceptLowPosition);
		this->holderPanel->Controls->Add(this->insertLowPosition);
		this->holderPanel->Controls->Add(this->positionLabel);
		this->holderPanel->Controls->Add(this->highPosition);
		this->holderPanel->Controls->Add(this->executeIcon);
		this->holderPanel->Controls->Add(this->acceptHighPosition);
		this->holderPanel->Controls->Add(this->lowPosition);
		this->holderPanel->Controls->Add(this->insertHighPosition);
		this->holderPanel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)));
		this->holderPanel->Location = System::Drawing::Point(30, 223);
		this->holderPanel->Name = L"holderPanel";
		this->holderPanel->Size = System::Drawing::Size(545, 602);
		this->holderPanel->TabIndex = 25;
		// 
		// positionLabel
		// 
		this->positionLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->positionLabel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->positionLabel->Location = System::Drawing::Point(13, 177);
		this->positionLabel->Name = L"positionLabel";
		this->positionLabel->Size = System::Drawing::Size(186, 24);
		this->positionLabel->TabIndex = 35;
		this->positionLabel->Text = L"insert high position (mm)";
		this->positionLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// highPosition
		// 
		this->highPosition->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)), static_cast<System::Int32>(static_cast<System::Byte>(207)),
			static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->highPosition->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->highPosition->ForeColor = System::Drawing::Color::Gray;
		this->highPosition->Location = System::Drawing::Point(359, 201);
		this->highPosition->Name = L"highPosition";
		this->highPosition->Size = System::Drawing::Size(170, 55);
		this->highPosition->TabIndex = 37;
		this->highPosition->Text = L"---";
		this->highPosition->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// executeIcon
		// 
		this->executeIcon->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->executeIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->executeIcon->Location = System::Drawing::Point(17, 17);
		this->executeIcon->Name = L"executeIcon";
		this->executeIcon->Size = System::Drawing::Size(140, 140);
		this->executeIcon->TabIndex = 26;
		this->executeIcon->TabStop = false;
		// 
		// acceptHighPosition
		// 
		this->acceptHighPosition->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
		this->acceptHighPosition->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
		this->acceptHighPosition->FlatAppearance->BorderSize = 5;
		this->acceptHighPosition->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Bold,
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->acceptHighPosition->Location = System::Drawing::Point(193, 201);
		this->acceptHighPosition->Name = L"acceptHighPosition";
		this->acceptHighPosition->Size = System::Drawing::Size(160, 55);
		this->acceptHighPosition->TabIndex = 31;
		this->acceptHighPosition->Text = L"=>";
		this->acceptHighPosition->UseVisualStyleBackColor = false;
		// 
		// lowPosition
		// 
		this->lowPosition->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)), static_cast<System::Int32>(static_cast<System::Byte>(207)),
			static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->lowPosition->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->lowPosition->ForeColor = System::Drawing::Color::Gray;
		this->lowPosition->Location = System::Drawing::Point(359, 291);
		this->lowPosition->Name = L"lowPosition";
		this->lowPosition->Size = System::Drawing::Size(170, 55);
		this->lowPosition->TabIndex = 29;
		this->lowPosition->Text = L"---";
		this->lowPosition->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// insertHighPosition
		// 
		this->insertHighPosition->BackColor = System::Drawing::Color::White;
		this->insertHighPosition->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
		this->insertHighPosition->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->insertHighPosition->ForeColor = System::Drawing::Color::Black;
		this->insertHighPosition->Location = System::Drawing::Point(17, 201);
		this->insertHighPosition->Name = L"insertHighPosition";
		this->insertHighPosition->Size = System::Drawing::Size(170, 55);
		this->insertHighPosition->TabIndex = 24;
		this->insertHighPosition->Text = L"90";
		this->insertHighPosition->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// paddleLimits
		// 
		this->paddleLimits->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->paddleLimits->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->paddleLimits->Location = System::Drawing::Point(410, 40);
		this->paddleLimits->Name = L"paddleLimits";
		this->paddleLimits->Size = System::Drawing::Size(165, 165);
		this->paddleLimits->TabIndex = 22;
		this->paddleLimits->TabStop = false;
		// 
		// paddleForceCalibration
		// 
		this->paddleForceCalibration->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->paddleForceCalibration->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->paddleForceCalibration->Location = System::Drawing::Point(220, 40);
		this->paddleForceCalibration->Name = L"paddleForceCalibration";
		this->paddleForceCalibration->Size = System::Drawing::Size(165, 165);
		this->paddleForceCalibration->TabIndex = 21;
		this->paddleForceCalibration->TabStop = false;
		// 
		// paddleHolderCalibration
		// 
		this->paddleHolderCalibration->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->paddleHolderCalibration->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->paddleHolderCalibration->Location = System::Drawing::Point(30, 40);
		this->paddleHolderCalibration->Name = L"paddleHolderCalibration";
		this->paddleHolderCalibration->Size = System::Drawing::Size(165, 165);
		this->paddleHolderCalibration->TabIndex = 20;
		this->paddleHolderCalibration->TabStop = false;
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
		// insertLowPosition
		// 
		this->insertLowPosition->BackColor = System::Drawing::Color::White;
		this->insertLowPosition->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
		this->insertLowPosition->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->insertLowPosition->ForeColor = System::Drawing::Color::Black;
		this->insertLowPosition->Location = System::Drawing::Point(17, 291);
		this->insertLowPosition->Name = L"insertLowPosition";
		this->insertLowPosition->Size = System::Drawing::Size(170, 55);
		this->insertLowPosition->TabIndex = 39;
		this->insertLowPosition->Text = L"90";
		this->insertLowPosition->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// acceptLowPosition
		// 
		this->acceptLowPosition->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
		this->acceptLowPosition->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
		this->acceptLowPosition->FlatAppearance->BorderSize = 5;
		this->acceptLowPosition->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->acceptLowPosition->Location = System::Drawing::Point(193, 291);
		this->acceptLowPosition->Name = L"acceptLowPosition";
		this->acceptLowPosition->Size = System::Drawing::Size(160, 55);
		this->acceptLowPosition->TabIndex = 40;
		this->acceptLowPosition->Text = L"=>";
		this->acceptLowPosition->UseVisualStyleBackColor = false;
		// 
		// calibrateHolder
		// 
		this->calibrateHolder->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(255)));
		this->calibrateHolder->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
		this->calibrateHolder->FlatAppearance->BorderSize = 5;
		this->calibrateHolder->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->calibrateHolder->Location = System::Drawing::Point(17, 435);
		this->calibrateHolder->Name = L"calibrateHolder";
		this->calibrateHolder->Size = System::Drawing::Size(160, 55);
		this->calibrateHolder->TabIndex = 41;
		this->calibrateHolder->Text = L"CALC";
		this->calibrateHolder->UseVisualStyleBackColor = false;
		// 
		// calibratedHolderOffset
		// 
		this->calibratedHolderOffset->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)),
			static_cast<System::Int32>(static_cast<System::Byte>(207)), static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->calibratedHolderOffset->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Bold,
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->calibratedHolderOffset->ForeColor = System::Drawing::Color::Gray;
		this->calibratedHolderOffset->Location = System::Drawing::Point(183, 435);
		this->calibratedHolderOffset->Name = L"calibratedHolderOffset";
		this->calibratedHolderOffset->Size = System::Drawing::Size(170, 55);
		this->calibratedHolderOffset->TabIndex = 42;
		this->calibratedHolderOffset->Text = L"---";
		this->calibratedHolderOffset->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// label3
		// 
		this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->label3->Location = System::Drawing::Point(13, 266);
		this->label3->Name = L"label3";
		this->label3->Size = System::Drawing::Size(174, 25);
		this->label3->TabIndex = 43;
		this->label3->Text = L"insert low position (mm)";
		this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// calibratedHolderLinearity
		// 
		this->calibratedHolderLinearity->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)),
			static_cast<System::Int32>(static_cast<System::Byte>(207)), static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->calibratedHolderLinearity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Bold,
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->calibratedHolderLinearity->ForeColor = System::Drawing::Color::Gray;
		this->calibratedHolderLinearity->Location = System::Drawing::Point(359, 435);
		this->calibratedHolderLinearity->Name = L"calibratedHolderLinearity";
		this->calibratedHolderLinearity->Size = System::Drawing::Size(170, 55);
		this->calibratedHolderLinearity->TabIndex = 44;
		this->calibratedHolderLinearity->Text = L"---";
		this->calibratedHolderLinearity->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// label4
		// 
		this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->label4->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->label4->Location = System::Drawing::Point(183, 411);
		this->label4->Name = L"label4";
		this->label4->Size = System::Drawing::Size(170, 24);
		this->label4->TabIndex = 45;
		this->label4->Text = L"calibrated offset";
		this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// label5
		// 
		this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->label5->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->label5->Location = System::Drawing::Point(359, 411);
		this->label5->Name = L"label5";
		this->label5->Size = System::Drawing::Size(165, 24);
		this->label5->TabIndex = 46;
		this->label5->Text = L"calibrated linearity";
		this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// currentHolderPosition
		// 
		this->currentHolderPosition->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)),
			static_cast<System::Int32>(static_cast<System::Byte>(207)), static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->currentHolderPosition->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->currentHolderPosition->ForeColor = System::Drawing::Color::Gray;
		this->currentHolderPosition->Location = System::Drawing::Point(329, 45);
		this->currentHolderPosition->Name = L"currentHolderPosition";
		this->currentHolderPosition->Size = System::Drawing::Size(185, 55);
		this->currentHolderPosition->TabIndex = 47;
		this->currentHolderPosition->Text = L"---";
		this->currentHolderPosition->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// label2
		// 
		this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->label2->Location = System::Drawing::Point(325, 21);
		this->label2->Name = L"label2";
		this->label2->Size = System::Drawing::Size(189, 24);
		this->label2->TabIndex = 48;
		this->label2->Text = L"holder position (mm)";
		this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// storeCalibratedParam
		// 
		this->storeCalibratedParam->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
		this->storeCalibratedParam->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
		this->storeCalibratedParam->FlatAppearance->BorderSize = 5;
		this->storeCalibratedParam->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Bold,
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->storeCalibratedParam->Location = System::Drawing::Point(17, 506);
		this->storeCalibratedParam->Name = L"storeCalibratedParam";
		this->storeCalibratedParam->Size = System::Drawing::Size(512, 55);
		this->storeCalibratedParam->TabIndex = 49;
		this->storeCalibratedParam->Text = L"STORE";
		this->storeCalibratedParam->UseVisualStyleBackColor = false;
		// 
		// ServicePaddleCalibrationTool
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
		this->Name = L"ServicePaddleCalibrationTool";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		this->calibZerosettingPanel->ResumeLayout(false);
		this->holderPanel->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->executeIcon))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->paddleLimits))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->paddleForceCalibration))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->paddleHolderCalibration))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceCanc))->EndInit();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	// Common Panel Buttons
	private: System::Void OnPaddleCalibrationToolTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e);
	private: System::Void cancButton_Click(System::Object^ sender, System::EventArgs^ e);

	// Calibration - Zero Setting Panel
	private: System::Void zeroSettingBody_Click(System::Object^ sender, System::EventArgs^ e);
	
	private: System::Void zeroSettingTilt_Click(System::Object^ sender, System::EventArgs^ e);
	
	private: System::Void targetSelection_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void runButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void manButton_Click(System::Object^ sender, System::EventArgs^ e);


	
};

