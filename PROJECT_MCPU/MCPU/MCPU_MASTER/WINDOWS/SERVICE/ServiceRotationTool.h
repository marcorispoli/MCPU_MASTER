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

public ref class ServiceRotationTool : public System::Windows::Forms::Form
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
	Object^ pConfirmation; // Used in the case a confirmation should needed

	void initPanel(void);
	void timerManagement(void);
	static ServiceRotationTool^ panel = gcnew ServiceRotationTool(); // Pointer to the Form instance from the static member
	System::Timers::Timer^ serviceTimer;

	ServiceRotationTool(void)
	{
		panel = this;
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		formInitialization();
	}

	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~ServiceRotationTool()
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

private: System::Windows::Forms::Panel^ rotationToolPanel;
private: System::Windows::Forms::PictureBox^ rotationToolArmButton;
private: System::Windows::Forms::PictureBox^ rotationToolManualButton;
private: System::Windows::Forms::PictureBox^ rotationToolSlideButton;
private: System::Windows::Forms::PictureBox^ rotationToolBodyButton;
private: System::Windows::Forms::PictureBox^ rotationToolTiltButton;
private: System::Windows::Forms::Panel^ rotationToolView;
private: System::Windows::Forms::PictureBox^ rotationToolSelectionIcon;
private: System::Windows::Forms::PictureBox^ rotationToolSelectAngle9;
private: System::Windows::Forms::PictureBox^ rotationToolSelectAngle8;
private: System::Windows::Forms::PictureBox^ rotationToolSelectAngle7;
private: System::Windows::Forms::PictureBox^ rotationToolSelectAngle6;
private: System::Windows::Forms::PictureBox^ rotationToolSelectAngle5;
private: System::Windows::Forms::PictureBox^ rotationToolSelectAngle4;
private: System::Windows::Forms::PictureBox^ rotationToolSelectAngle3;
private: System::Windows::Forms::PictureBox^ rotationToolSelectAngle2;
private: System::Windows::Forms::PictureBox^ rotationToolSelectAngle1;
private: System::Windows::Forms::Label^ rotationToolTargetAngle;

// Tool Rotation Panel
public: void rotationTool_select_item(System::Windows::Forms::PictureBox^ item);
public: void rotationTooCreateInfoPanel(unsigned char reason);
public: void rotationToolSelectionArmOkCallback(void);
public: void rotationToolSelectionTiltOkCallback(void);
public: void rotationToolSelectionBodyOkCallback(void);
public: void rotationToolSelectionSlideOkCallback(void);
public: void rotationToolSelectionCancCallback(void);


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
		this->rotationToolPanel = (gcnew System::Windows::Forms::Panel());
		this->rotationToolView = (gcnew System::Windows::Forms::Panel());
		this->rotationToolManualButton = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolTargetAngle = (gcnew System::Windows::Forms::Label());
		this->rotationToolSelectAngle9 = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolSelectAngle8 = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolSelectAngle7 = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolSelectAngle6 = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolSelectAngle5 = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolSelectAngle4 = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolSelectAngle3 = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolSelectAngle2 = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolSelectAngle1 = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolSelectionIcon = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolSlideButton = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolBodyButton = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolTiltButton = (gcnew System::Windows::Forms::PictureBox());
		this->rotationToolArmButton = (gcnew System::Windows::Forms::PictureBox());
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceCanc))->BeginInit();
		this->rotationToolPanel->SuspendLayout();
		this->rotationToolView->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolManualButton))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle9))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle8))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle7))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle6))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle5))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle4))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle3))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle2))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle1))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectionIcon))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSlideButton))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolBodyButton))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolTiltButton))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolArmButton))->BeginInit();
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
		this->serviceCanc->Click += gcnew System::EventHandler(this, &ServiceRotationTool::cancButton_Click);
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
		// rotationToolPanel
		// 
		this->rotationToolPanel->Controls->Add(this->rotationToolView);
		this->rotationToolPanel->Controls->Add(this->rotationToolSlideButton);
		this->rotationToolPanel->Controls->Add(this->rotationToolBodyButton);
		this->rotationToolPanel->Controls->Add(this->rotationToolTiltButton);
		this->rotationToolPanel->Controls->Add(this->rotationToolArmButton);
		this->rotationToolPanel->Location = System::Drawing::Point(0, 50);
		this->rotationToolPanel->Name = L"rotationToolPanel";
		this->rotationToolPanel->Size = System::Drawing::Size(600, 880);
		this->rotationToolPanel->TabIndex = 21;
		// 
		// rotationToolView
		// 
		this->rotationToolView->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->rotationToolView->Controls->Add(this->rotationToolManualButton);
		this->rotationToolView->Controls->Add(this->rotationToolTargetAngle);
		this->rotationToolView->Controls->Add(this->rotationToolSelectAngle9);
		this->rotationToolView->Controls->Add(this->rotationToolSelectAngle8);
		this->rotationToolView->Controls->Add(this->rotationToolSelectAngle7);
		this->rotationToolView->Controls->Add(this->rotationToolSelectAngle6);
		this->rotationToolView->Controls->Add(this->rotationToolSelectAngle5);
		this->rotationToolView->Controls->Add(this->rotationToolSelectAngle4);
		this->rotationToolView->Controls->Add(this->rotationToolSelectAngle3);
		this->rotationToolView->Controls->Add(this->rotationToolSelectAngle2);
		this->rotationToolView->Controls->Add(this->rotationToolSelectAngle1);
		this->rotationToolView->Controls->Add(this->rotationToolSelectionIcon);
		this->rotationToolView->Location = System::Drawing::Point(75, 391);
		this->rotationToolView->Name = L"rotationToolView";
		this->rotationToolView->Size = System::Drawing::Size(513, 450);
		this->rotationToolView->TabIndex = 29;
		// 
		// rotationToolManualButton
		// 
		this->rotationToolManualButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolManualButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolManualButton->Location = System::Drawing::Point(430, 12);
		this->rotationToolManualButton->Name = L"rotationToolManualButton";
		this->rotationToolManualButton->Size = System::Drawing::Size(70, 70);
		this->rotationToolManualButton->TabIndex = 28;
		this->rotationToolManualButton->TabStop = false;
		this->rotationToolManualButton->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_manual_Click);
		// 
		// rotationToolTargetAngle
		// 
		this->rotationToolTargetAngle->BackColor = System::Drawing::Color::Black;
		this->rotationToolTargetAngle->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 72, System::Drawing::FontStyle::Bold,
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->rotationToolTargetAngle->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(236)),
			static_cast<System::Int32>(static_cast<System::Byte>(187)), static_cast<System::Int32>(static_cast<System::Byte>(187)));
		this->rotationToolTargetAngle->Location = System::Drawing::Point(237, 344);
		this->rotationToolTargetAngle->Name = L"rotationToolTargetAngle";
		this->rotationToolTargetAngle->Size = System::Drawing::Size(193, 80);
		this->rotationToolTargetAngle->TabIndex = 39;
		this->rotationToolTargetAngle->Text = L"180°";
		this->rotationToolTargetAngle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// rotationToolSelectAngle9
		// 
		this->rotationToolSelectAngle9->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolSelectAngle9->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolSelectAngle9->Location = System::Drawing::Point(335, 243);
		this->rotationToolSelectAngle9->Name = L"rotationToolSelectAngle9";
		this->rotationToolSelectAngle9->Size = System::Drawing::Size(95, 95);
		this->rotationToolSelectAngle9->TabIndex = 38;
		this->rotationToolSelectAngle9->TabStop = false;
		this->rotationToolSelectAngle9->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_selection_angle_9_Click);
		// 
		// rotationToolSelectAngle8
		// 
		this->rotationToolSelectAngle8->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolSelectAngle8->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolSelectAngle8->Location = System::Drawing::Point(130, 344);
		this->rotationToolSelectAngle8->Name = L"rotationToolSelectAngle8";
		this->rotationToolSelectAngle8->Size = System::Drawing::Size(95, 95);
		this->rotationToolSelectAngle8->TabIndex = 37;
		this->rotationToolSelectAngle8->TabStop = false;
		this->rotationToolSelectAngle8->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_selection_angle_8_Click);
		// 
		// rotationToolSelectAngle7
		// 
		this->rotationToolSelectAngle7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolSelectAngle7->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolSelectAngle7->Location = System::Drawing::Point(231, 243);
		this->rotationToolSelectAngle7->Name = L"rotationToolSelectAngle7";
		this->rotationToolSelectAngle7->Size = System::Drawing::Size(95, 95);
		this->rotationToolSelectAngle7->TabIndex = 36;
		this->rotationToolSelectAngle7->TabStop = false;
		this->rotationToolSelectAngle7->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_selection_angle_7_Click);
		// 
		// rotationToolSelectAngle6
		// 
		this->rotationToolSelectAngle6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolSelectAngle6->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolSelectAngle6->Location = System::Drawing::Point(130, 243);
		this->rotationToolSelectAngle6->Name = L"rotationToolSelectAngle6";
		this->rotationToolSelectAngle6->Size = System::Drawing::Size(95, 95);
		this->rotationToolSelectAngle6->TabIndex = 35;
		this->rotationToolSelectAngle6->TabStop = false;
		this->rotationToolSelectAngle6->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_selection_angle_6_Click);
		// 
		// rotationToolSelectAngle5
		// 
		this->rotationToolSelectAngle5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolSelectAngle5->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolSelectAngle5->Location = System::Drawing::Point(29, 243);
		this->rotationToolSelectAngle5->Name = L"rotationToolSelectAngle5";
		this->rotationToolSelectAngle5->Size = System::Drawing::Size(95, 95);
		this->rotationToolSelectAngle5->TabIndex = 34;
		this->rotationToolSelectAngle5->TabStop = false;
		this->rotationToolSelectAngle5->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_selection_angle_5_Click);
		// 
		// rotationToolSelectAngle4
		// 
		this->rotationToolSelectAngle4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolSelectAngle4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolSelectAngle4->Location = System::Drawing::Point(332, 142);
		this->rotationToolSelectAngle4->Name = L"rotationToolSelectAngle4";
		this->rotationToolSelectAngle4->Size = System::Drawing::Size(95, 95);
		this->rotationToolSelectAngle4->TabIndex = 33;
		this->rotationToolSelectAngle4->TabStop = false;
		this->rotationToolSelectAngle4->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_selection_angle_4_Click);
		// 
		// rotationToolSelectAngle3
		// 
		this->rotationToolSelectAngle3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolSelectAngle3->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolSelectAngle3->Location = System::Drawing::Point(231, 142);
		this->rotationToolSelectAngle3->Name = L"rotationToolSelectAngle3";
		this->rotationToolSelectAngle3->Size = System::Drawing::Size(95, 95);
		this->rotationToolSelectAngle3->TabIndex = 32;
		this->rotationToolSelectAngle3->TabStop = false;
		this->rotationToolSelectAngle3->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_selection_angle_3_Click);
		// 
		// rotationToolSelectAngle2
		// 
		this->rotationToolSelectAngle2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolSelectAngle2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolSelectAngle2->Location = System::Drawing::Point(130, 142);
		this->rotationToolSelectAngle2->Name = L"rotationToolSelectAngle2";
		this->rotationToolSelectAngle2->Size = System::Drawing::Size(95, 95);
		this->rotationToolSelectAngle2->TabIndex = 31;
		this->rotationToolSelectAngle2->TabStop = false;
		this->rotationToolSelectAngle2->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_selection_angle_2_Click);
		// 
		// rotationToolSelectAngle1
		// 
		this->rotationToolSelectAngle1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolSelectAngle1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolSelectAngle1->Location = System::Drawing::Point(29, 142);
		this->rotationToolSelectAngle1->Name = L"rotationToolSelectAngle1";
		this->rotationToolSelectAngle1->Size = System::Drawing::Size(95, 95);
		this->rotationToolSelectAngle1->TabIndex = 30;
		this->rotationToolSelectAngle1->TabStop = false;
		this->rotationToolSelectAngle1->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_selection_angle_1_Click);
		// 
		// rotationToolSelectionIcon
		// 
		this->rotationToolSelectionIcon->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolSelectionIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolSelectionIcon->Location = System::Drawing::Point(29, 27);
		this->rotationToolSelectionIcon->Name = L"rotationToolSelectionIcon";
		this->rotationToolSelectionIcon->Size = System::Drawing::Size(95, 95);
		this->rotationToolSelectionIcon->TabIndex = 29;
		this->rotationToolSelectionIcon->TabStop = false;
		// 
		// rotationToolSlideButton
		// 
		this->rotationToolSlideButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolSlideButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolSlideButton->Location = System::Drawing::Point(325, 214);
		this->rotationToolSlideButton->Name = L"rotationToolSlideButton";
		this->rotationToolSlideButton->Size = System::Drawing::Size(160, 160);
		this->rotationToolSlideButton->TabIndex = 27;
		this->rotationToolSlideButton->TabStop = false;
		this->rotationToolSlideButton->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_slide_Click);
		// 
		// rotationToolBodyButton
		// 
		this->rotationToolBodyButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolBodyButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolBodyButton->Location = System::Drawing::Point(103, 214);
		this->rotationToolBodyButton->Name = L"rotationToolBodyButton";
		this->rotationToolBodyButton->Size = System::Drawing::Size(160, 160);
		this->rotationToolBodyButton->TabIndex = 26;
		this->rotationToolBodyButton->TabStop = false;
		this->rotationToolBodyButton->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_body_Click);
		// 
		// rotationToolTiltButton
		// 
		this->rotationToolTiltButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolTiltButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolTiltButton->Location = System::Drawing::Point(325, 23);
		this->rotationToolTiltButton->Name = L"rotationToolTiltButton";
		this->rotationToolTiltButton->Size = System::Drawing::Size(160, 160);
		this->rotationToolTiltButton->TabIndex = 25;
		this->rotationToolTiltButton->TabStop = false;
		this->rotationToolTiltButton->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_tilt_Click);
		// 
		// rotationToolArmButton
		// 
		this->rotationToolArmButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationToolArmButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->rotationToolArmButton->Location = System::Drawing::Point(103, 23);
		this->rotationToolArmButton->Name = L"rotationToolArmButton";
		this->rotationToolArmButton->Size = System::Drawing::Size(160, 160);
		this->rotationToolArmButton->TabIndex = 20;
		this->rotationToolArmButton->TabStop = false;
		this->rotationToolArmButton->Click += gcnew System::EventHandler(this, &ServiceRotationTool::rotationTool_arm_Click);
		// 
		// ServiceRotationTool
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
		this->Controls->Add(this->rotationToolPanel);
		this->Controls->Add(this->serviceMenuTitle);
		this->Controls->Add(this->labelInstallation);
		this->Controls->Add(this->labelDate);
		this->Controls->Add(this->labelTime);
		this->Controls->Add(this->serviceCanc);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Name = L"ServiceRotationTool";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceCanc))->EndInit();
		this->rotationToolPanel->ResumeLayout(false);
		this->rotationToolView->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolManualButton))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle9))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle8))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle7))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle6))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle5))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle4))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle3))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle2))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectAngle1))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSelectionIcon))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolSlideButton))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolBodyButton))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolTiltButton))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationToolArmButton))->EndInit();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	// Common Panel Buttons
	private: System::Void onServiceTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e);
	private: System::Void cancButton_Click(System::Object^ sender, System::EventArgs^ e);

	// Rotation Tool Button callbacks
	private: System::Void rotationTool_body_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotationTool_tilt_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotationTool_arm_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotationTool_slide_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotationTool_manual_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotationTool_selection_angle_1_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotationTool_selection_angle_2_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotationTool_selection_angle_3_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotationTool_selection_angle_4_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotationTool_selection_angle_5_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotationTool_selection_angle_6_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotationTool_selection_angle_7_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotationTool_selection_angle_8_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotationTool_selection_angle_9_Click(System::Object^ sender, System::EventArgs^ e);

	
};

