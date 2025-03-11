#pragma once

/**
	\defgroup BiopsyModuleImplementation Biopsy Module Source Code description
	\ingroup APPIMPL


	This section describes the implementation of the Biopsy module
*/


/**
\defgroup BiopsyModuleDescription Biopsy Module Description
\ingroup APPDOC

   
    # Abstract

    This document describes in detail the Biopsy Study implementation.


    # Index

    
	# Module Functions
	
	The module implements the features related to the Biopsy study workflow management:

	+ A dedicated GUI is developed for the Biopsy Study:
		+ graphic buttons for  local activations;
		+ graphic icons for Gantry status visualization;
		+ dedicated graphic icons and button to drive a correct homing procedure;

	+ Pointer management functions:
		+ home position management;
		+ pointing management;
		+ external keybord management;

	+ Slide Arm management:
		+ Slide activation to 0°;
		+ Slide activation to 90°;

	+ 2D Control image visualization;

	+ Tilt motor control for biopsy:
		+ Tilt activation to the stereo biopsy angles;
		+ Tilt activation to arbitrary angles helping the biopsy accessory mounting;

	+ Arm rotation management:
		+ manual rotation of th C-ARM for the projection selection;

	+ Exposure sequences:
		+ manual and AEC 2D exposures;
		+ Tomo in manual and AEC;
	
	# Module activation

	The module shall be activate only when the Gantry is in IDLE operating status.

	The Biopsy device shall be already connected: in case the device should not be connected
	the activation command will abort.

	The AWS shall make use of the \ref EXEC_BiopsyStudy command to open the biopsy study and activate 
	the module. 
	
	See the AWS protocol description for command details.


	# Pointer activation description

	## General rules
	
	### X-SCROLL
	
	The X-Axe can be mechanically scrolled in three possible positions: Left, Center and Right;
	+ The Module monitors the current position identifying an invalid position selection.
	
	### Y Up/Down

	The Module detects the current status of the Y:
	+ Tourned Up;
	+ Tourned Down;

	The pointer activation sequence depends by the current rotation position of the Y-Axes. 

	### Home position

	The Module implements three possible Home position:
	+ HOME CENTER: pointer in the center of the axis and the X-SCROLL set in the center and Y tourned Up;
	+ HOME LEFT: pointer in the most left position and the X-SCROLL set in the left position and Y tourned Down;
	+ HOME RIGHT: pointer in the most right position and the X-SCROLL set in the right position and Y tourned Down;

	\note The coordinates associated to the home positions are stored into the BiopsyConfig configuration file.

	Before to start any pointing activation, the pointer shall be set into one of the 
	valid home positions.
	
	\note When a valid home position has been selected, only one pointing activation will be permitted.

	\note When the Biopsy Study is open the current position is not considered as a valid home position
	until a proper home selection command is executed.

	## Home activation procedure

	The AWS shall request to set the pointer in one of the available Home positions
	calling one of the following protocol commands:
	+ EXEC_BiopsyHome("C"): activates the Center Home position;
	+ EXEC_BiopsyHome("L"): activates the Left Home position;
	+ EXEC_BiopsyHome("R"): activates the Right Home position;

	During the command execution, the GUI will provide graphical information
	about the current operating status.
	
	The GUI will inform the Operator about possible required activities like:
	+ Tourning Up or Down the Y axes if required;
	+ Scrolling the X-Axes if required;
	
	\note Although the module is able to detect the valid mechanical positions,
	every time a mechanical operation has been completed (and detected) 
	a confirmation is requested to the operator, preventing an unexpected activation
	with the operator hands still on the biopsy device mechanical parts.

	see \ref homingProcedure for activation details.

	## Pointing activation procedure

	The Pointing activation can take place only if a home position has been selected first.

	The AWS command EXEC_BiopsyPointing(x,y,z) starts the positioner moving to the 
	X,Y,Z target position.

	Because the home position procedure set the X-Scroll and Y-UpDown to the correct position,
	during the Pointing activation it is not requested any operator action.

	The GUI will show the current activation step with 
	proper graphical images. See GUI chapter for details.

	\note Only when the Pointing activation successfully completes 
	the exposures may be enabled.


	## Parking activation procedure


	# Motors management
	
	## C-ARM rotation
	
	When the Biopsy study is activated, the manual C-ARM rotation keyboard is enabled (green led switched On).

	In Biopsy Study, the C-ARM rotation can be executed only with manual commands.
	
	\warning the automatic activations with the protocol command has consequently been disabled.

	The maximum rotation angle in Biopsy will be limited to +/- 90°.

	The current C-ARM angle is visible in a dedicated graphical box of the GUI.

	During the C-ARM rotation, a Dialog on the local display shows the actual ARM angle.\n
	During the C-ARM rotation, the displays on the Potter plane show the actual ARM angle.\n
	When the rotation termines, after few seconds, both the Dialog and the displays will stop displayng the actual ARM angle.

	\note The Isocentric vertical correction will not take place in manual C-ARM rotation.

	## Tilt motor

	The Tilt motor can be activated only with automatic rotation 
	started through AWS protocol commands. 
	
	See the EXEC_TrxPosition protocol command for details.

	\note There isn't any special rule in order to enable the Tilt to move. It can always be moved 
	in the Biopsy Study.

	## Slide motor

	The module provides a dedicated graphical button in the GUI 
	to allow the operatore to start an automatic Arm positioning in two possible 
	Slide angles:
	+ 0° (standard position);
	+ 90° (usually needed with the table).

	The button works toggling the current status from 0° and 90°.
	
	\warning for safety reason this feature is disabled with a detected compression, as usual.

	\note If the operator should press the rotation button, a Dialog will appear requesting the action confirmation
	before to start the Slide rotation.

	## Vertical motor

	## Body motor
	
*/


#include <windows.h>

using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class BiopsyStudy :  public System::Windows::Forms::Form
{
#define WINMSG_TIMER WM_USER + 1
#define WINMSG_OPEN WINMSG_TIMER + 1
#define WINMSG_CLOSE WINMSG_OPEN + 1


public:
	HWND window;
	void open(void);
	void close(void);
	bool open_status;
	
	Object^ pXray;		
	Object^ pShiftConf;

private: System::Windows::Forms::Panel^ pointerPosition;
private: System::Windows::Forms::Label^ XLabel;
public:

private: System::Windows::Forms::Panel^ armPosition;
private: System::Windows::Forms::Label^ tiltLabel;
private: System::Windows::Forms::Label^ armLabel;
private: System::Windows::Forms::Label^ ZLabel;
private: System::Windows::Forms::Label^ YLabel;
private: System::Windows::Forms::Panel^ confirmationButton;



private: System::Windows::Forms::Panel^ activationPanelImg;
private: System::Windows::Forms::Panel^ activationInfoPanel;
private: System::Windows::Forms::Panel^ activationInfoIcon;
private: System::Windows::Forms::Label^ activationInfoText;
private: System::Windows::Forms::Panel^ statusPanel;
private: System::Windows::Forms::Panel^ keyStatus;

private: System::Windows::Forms::Panel^ activationPanel;






public:
	
	void suspend(void);
	void resume(void);	
	
	void onShiftConfirmOk(void);
	void onShiftConfirmCanc(void);

	void initOperatingStatus(void);
	void operatingStatusManagement(void);
	void evaluateXrayStatus(void);
	void evaluateReadyWarnings(bool reset);
	void evaluateCompressorStatus(bool init);
	void evaluateArmStatus(void);
	void evaluatePointerStatus(void);
	void evaluatePointerActivations(void);

	void evaluateDoorStatus(void);
	void evaluateCollimatorStatus(void);
	void evaluateSlideStatus(bool init);
	void evaluatePopupPanels(void);
	void evaluateDigitDisplays(void);
	void evaluateGridStatus(void);
	void evaluateAwsComponentEvent(void);
	
	void showProcedureImage(System::String^ action_img, System::String^ info_string, System::String^ info_icon, bool button);
	bool getProcedureConfirmation(bool reset);

	delegate void delegate_periodical_callback(void);//!< Delegate for the command completed event		
	event delegate_periodical_callback^ periodical_event; //!< Event generated at the command completion
	bool confirmation_button_status;


public:System::Timers::Timer^ operatingTimer;

public:
	BiopsyStudy(void)
	{
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		formInitialization();

	}

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~BiopsyStudy()
	{
		if (components)
		{
			delete components;
		}
	}
private: void formInitialization(void);

private: System::Windows::Forms::Panel^ mainPanel;

// Head of window
private: System::Windows::Forms::Label^ labelDate;
private: System::Windows::Forms::Label^ labelPatientName;
private: System::Windows::Forms::Label^ labelTime;


// Xray Status Bar
private: System::Windows::Forms::Panel^ xrayStat;
private: System::Windows::Forms::Label^ labelXrayStatus;
private: System::Windows::Forms::PictureBox^ demoIcon;
private: System::Windows::Forms::Panel^ cursorPanel;

// Projection selection panel




// Collimation lamp activation


// Slide selection


// Alarm button
private: System::Windows::Forms::Panel^ alarmButton;
private: System::Windows::Forms::Panel^ slideButton;


// Collimation status



// Paddle Status



// Thickness measurment
private: System::Windows::Forms::Panel^ thicknessStatus;
private: System::Windows::Forms::Label^ labelThickness;

// Force measurment
private: System::Windows::Forms::Panel^ forceStatus; 
private: System::Windows::Forms::Label^ labelForce;
private: System::Windows::Forms::Panel^ image2DButton;

private: System::Windows::Forms::Panel^ lampButton;

// Magnifier status



// Release compression status


// Tube data 
private: System::Windows::Forms::Panel^ tubeStatus;
private: System::Windows::Forms::Label^ labelTubeData;

// Door study status
private: System::Windows::Forms::Panel^ doorStatus;

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
		this->labelPatientName = (gcnew System::Windows::Forms::Label());
		this->labelTime = (gcnew System::Windows::Forms::Label());
		this->image2DButton = (gcnew System::Windows::Forms::Panel());
		this->mainPanel = (gcnew System::Windows::Forms::Panel());
		this->statusPanel = (gcnew System::Windows::Forms::Panel());
		this->activationPanel = (gcnew System::Windows::Forms::Panel());
		this->activationInfoPanel = (gcnew System::Windows::Forms::Panel());
		this->activationInfoText = (gcnew System::Windows::Forms::Label());
		this->activationInfoIcon = (gcnew System::Windows::Forms::Panel());
		this->activationPanelImg = (gcnew System::Windows::Forms::Panel());
		this->confirmationButton = (gcnew System::Windows::Forms::Panel());
		this->pointerPosition = (gcnew System::Windows::Forms::Panel());
		this->ZLabel = (gcnew System::Windows::Forms::Label());
		this->YLabel = (gcnew System::Windows::Forms::Label());
		this->XLabel = (gcnew System::Windows::Forms::Label());
		this->armPosition = (gcnew System::Windows::Forms::Panel());
		this->tiltLabel = (gcnew System::Windows::Forms::Label());
		this->armLabel = (gcnew System::Windows::Forms::Label());
		this->xrayStat = (gcnew System::Windows::Forms::Panel());
		this->demoIcon = (gcnew System::Windows::Forms::PictureBox());
		this->labelXrayStatus = (gcnew System::Windows::Forms::Label());
		this->alarmButton = (gcnew System::Windows::Forms::Panel());
		this->cursorPanel = (gcnew System::Windows::Forms::Panel());
		this->forceStatus = (gcnew System::Windows::Forms::Panel());
		this->labelForce = (gcnew System::Windows::Forms::Label());
		this->thicknessStatus = (gcnew System::Windows::Forms::Panel());
		this->labelThickness = (gcnew System::Windows::Forms::Label());
		this->doorStatus = (gcnew System::Windows::Forms::Panel());
		this->tubeStatus = (gcnew System::Windows::Forms::Panel());
		this->labelTubeData = (gcnew System::Windows::Forms::Label());
		this->slideButton = (gcnew System::Windows::Forms::Panel());
		this->lampButton = (gcnew System::Windows::Forms::Panel());
		this->keyStatus = (gcnew System::Windows::Forms::Panel());
		this->mainPanel->SuspendLayout();
		this->activationPanel->SuspendLayout();
		this->activationInfoPanel->SuspendLayout();
		this->pointerPosition->SuspendLayout();
		this->armPosition->SuspendLayout();
		this->xrayStat->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->demoIcon))->BeginInit();
		this->forceStatus->SuspendLayout();
		this->thicknessStatus->SuspendLayout();
		this->tubeStatus->SuspendLayout();
		this->SuspendLayout();
		// 
		// labelDate
		// 
		this->labelDate->AutoSize = true;
		this->labelDate->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelDate->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->labelDate->Location = System::Drawing::Point(26, 13);
		this->labelDate->Name = L"labelDate";
		this->labelDate->Size = System::Drawing::Size(100, 24);
		this->labelDate->TabIndex = 7;
		this->labelDate->Text = L"27/10/2023";
		// 
		// labelPatientName
		// 
		this->labelPatientName->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular,
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->labelPatientName->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)), static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->labelPatientName->Location = System::Drawing::Point(143, 13);
		this->labelPatientName->Name = L"labelPatientName";
		this->labelPatientName->Size = System::Drawing::Size(305, 24);
		this->labelPatientName->TabIndex = 8;
		this->labelPatientName->Text = L"pippo caio sempronio";
		this->labelPatientName->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// labelTime
		// 
		this->labelTime->AutoSize = true;
		this->labelTime->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelTime->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->labelTime->Location = System::Drawing::Point(481, 13);
		this->labelTime->Name = L"labelTime";
		this->labelTime->Size = System::Drawing::Size(88, 24);
		this->labelTime->TabIndex = 9;
		this->labelTime->Text = L"10:35 PM";
		// 
		// image2DButton
		// 
		this->image2DButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->image2DButton->Location = System::Drawing::Point(12, 872);
		this->image2DButton->Name = L"image2DButton";
		this->image2DButton->Size = System::Drawing::Size(135, 135);
		this->image2DButton->TabIndex = 0;
		// 
		// mainPanel
		// 
		this->mainPanel->Controls->Add(this->keyStatus);
		this->mainPanel->Controls->Add(this->statusPanel);
		this->mainPanel->Controls->Add(this->activationPanel);
		this->mainPanel->Controls->Add(this->pointerPosition);
		this->mainPanel->Controls->Add(this->armPosition);
		this->mainPanel->Controls->Add(this->xrayStat);
		this->mainPanel->Controls->Add(this->alarmButton);
		this->mainPanel->Controls->Add(this->cursorPanel);
		this->mainPanel->Controls->Add(this->forceStatus);
		this->mainPanel->Controls->Add(this->thicknessStatus);
		this->mainPanel->Controls->Add(this->doorStatus);
		this->mainPanel->Controls->Add(this->tubeStatus);
		this->mainPanel->Controls->Add(this->slideButton);
		this->mainPanel->Controls->Add(this->lampButton);
		this->mainPanel->Controls->Add(this->labelDate);
		this->mainPanel->Controls->Add(this->image2DButton);
		this->mainPanel->Controls->Add(this->labelPatientName);
		this->mainPanel->Controls->Add(this->labelTime);
		this->mainPanel->Location = System::Drawing::Point(0, 0);
		this->mainPanel->Name = L"mainPanel";
		this->mainPanel->Size = System::Drawing::Size(600, 1024);
		this->mainPanel->TabIndex = 14;
		// 
		// statusPanel
		// 
		this->statusPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->statusPanel->Location = System::Drawing::Point(192, 181);
		this->statusPanel->Name = L"statusPanel";
		this->statusPanel->Size = System::Drawing::Size(37, 10);
		this->statusPanel->TabIndex = 27;
		// 
		// activationPanel
		// 
		this->activationPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->activationPanel->Controls->Add(this->activationInfoPanel);
		this->activationPanel->Controls->Add(this->activationPanelImg);
		this->activationPanel->Controls->Add(this->confirmationButton);
		this->activationPanel->Location = System::Drawing::Point(192, 192);
		this->activationPanel->Name = L"activationPanel";
		this->activationPanel->Size = System::Drawing::Size(396, 364);
		this->activationPanel->TabIndex = 26;
		// 
		// activationInfoPanel
		// 
		this->activationInfoPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->activationInfoPanel->Controls->Add(this->activationInfoText);
		this->activationInfoPanel->Controls->Add(this->activationInfoIcon);
		this->activationInfoPanel->Location = System::Drawing::Point(3, 292);
		this->activationInfoPanel->Name = L"activationInfoPanel";
		this->activationInfoPanel->Size = System::Drawing::Size(390, 69);
		this->activationInfoPanel->TabIndex = 23;
		// 
		// activationInfoText
		// 
		this->activationInfoText->BackColor = System::Drawing::Color::Transparent;
		this->activationInfoText->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold,
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->activationInfoText->ForeColor = System::Drawing::Color::White;
		this->activationInfoText->Location = System::Drawing::Point(59, 3);
		this->activationInfoText->Name = L"activationInfoText";
		this->activationInfoText->Size = System::Drawing::Size(328, 50);
		this->activationInfoText->TabIndex = 3;
		this->activationInfoText->Text = L"X: 0.0 (mm)";
		this->activationInfoText->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// activationInfoIcon
		// 
		this->activationInfoIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->activationInfoIcon->Location = System::Drawing::Point(3, 3);
		this->activationInfoIcon->Name = L"activationInfoIcon";
		this->activationInfoIcon->Size = System::Drawing::Size(50, 50);
		this->activationInfoIcon->TabIndex = 1;
		// 
		// activationPanelImg
		// 
		this->activationPanelImg->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->activationPanelImg->Location = System::Drawing::Point(119, 3);
		this->activationPanelImg->Name = L"activationPanelImg";
		this->activationPanelImg->Size = System::Drawing::Size(274, 283);
		this->activationPanelImg->TabIndex = 21;
		// 
		// confirmationButton
		// 
		this->confirmationButton->BackColor = System::Drawing::Color::Red;
		this->confirmationButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->confirmationButton->Location = System::Drawing::Point(12, 98);
		this->confirmationButton->Name = L"confirmationButton";
		this->confirmationButton->Size = System::Drawing::Size(90, 90);
		this->confirmationButton->TabIndex = 1;
		this->confirmationButton->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &BiopsyStudy::confirmationButton_MouseClick);
		// 
		// pointerPosition
		// 
		this->pointerPosition->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->pointerPosition->Controls->Add(this->ZLabel);
		this->pointerPosition->Controls->Add(this->YLabel);
		this->pointerPosition->Controls->Add(this->XLabel);
		this->pointerPosition->Location = System::Drawing::Point(306, 575);
		this->pointerPosition->Name = L"pointerPosition";
		this->pointerPosition->Size = System::Drawing::Size(282, 135);
		this->pointerPosition->TabIndex = 25;
		// 
		// ZLabel
		// 
		this->ZLabel->BackColor = System::Drawing::Color::Transparent;
		this->ZLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->ZLabel->Location = System::Drawing::Point(147, 96);
		this->ZLabel->Name = L"ZLabel";
		this->ZLabel->Size = System::Drawing::Size(135, 25);
		this->ZLabel->TabIndex = 4;
		this->ZLabel->Text = L"Z: 0.0 (mm)";
		this->ZLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// YLabel
		// 
		this->YLabel->BackColor = System::Drawing::Color::Transparent;
		this->YLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->YLabel->Location = System::Drawing::Point(147, 62);
		this->YLabel->Name = L"YLabel";
		this->YLabel->Size = System::Drawing::Size(135, 25);
		this->YLabel->TabIndex = 3;
		this->YLabel->Text = L"Y: 0.0 (mm)";
		this->YLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// XLabel
		// 
		this->XLabel->BackColor = System::Drawing::Color::Transparent;
		this->XLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->XLabel->Location = System::Drawing::Point(147, 26);
		this->XLabel->Name = L"XLabel";
		this->XLabel->Size = System::Drawing::Size(135, 25);
		this->XLabel->TabIndex = 2;
		this->XLabel->Text = L"X: 0.0 (mm)";
		this->XLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// armPosition
		// 
		this->armPosition->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->armPosition->Controls->Add(this->tiltLabel);
		this->armPosition->Controls->Add(this->armLabel);
		this->armPosition->Location = System::Drawing::Point(12, 575);
		this->armPosition->Name = L"armPosition";
		this->armPosition->Size = System::Drawing::Size(135, 135);
		this->armPosition->TabIndex = 24;
		// 
		// tiltLabel
		// 
		this->tiltLabel->BackColor = System::Drawing::Color::Transparent;
		this->tiltLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->tiltLabel->Location = System::Drawing::Point(12, 110);
		this->tiltLabel->Name = L"tiltLabel";
		this->tiltLabel->Size = System::Drawing::Size(118, 25);
		this->tiltLabel->TabIndex = 2;
		this->tiltLabel->Text = L"TILT: 0°";
		this->tiltLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// armLabel
		// 
		this->armLabel->BackColor = System::Drawing::Color::Transparent;
		this->armLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->armLabel->Location = System::Drawing::Point(12, 85);
		this->armLabel->Name = L"armLabel";
		this->armLabel->Size = System::Drawing::Size(118, 25);
		this->armLabel->TabIndex = 1;
		this->armLabel->Text = L"ARM: 180°";
		this->armLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// xrayStat
		// 
		this->xrayStat->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->xrayStat->Controls->Add(this->demoIcon);
		this->xrayStat->Controls->Add(this->labelXrayStatus);
		this->xrayStat->Location = System::Drawing::Point(12, 52);
		this->xrayStat->Name = L"xrayStat";
		this->xrayStat->Size = System::Drawing::Size(576, 123);
		this->xrayStat->TabIndex = 23;
		// 
		// demoIcon
		// 
		this->demoIcon->BackColor = System::Drawing::Color::Red;
		this->demoIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->demoIcon->Location = System::Drawing::Point(220, 80);
		this->demoIcon->Name = L"demoIcon";
		this->demoIcon->Size = System::Drawing::Size(125, 40);
		this->demoIcon->TabIndex = 0;
		this->demoIcon->TabStop = false;
		// 
		// labelXrayStatus
		// 
		this->labelXrayStatus->BackColor = System::Drawing::Color::Transparent;
		this->labelXrayStatus->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
		this->labelXrayStatus->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 26.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelXrayStatus->Location = System::Drawing::Point(18, 10);
		this->labelXrayStatus->Name = L"labelXrayStatus";
		this->labelXrayStatus->Size = System::Drawing::Size(539, 99);
		this->labelXrayStatus->TabIndex = 1;
		this->labelXrayStatus->Text = L"STAND-BY";
		this->labelXrayStatus->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// alarmButton
		// 
		this->alarmButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->alarmButton->Location = System::Drawing::Point(453, 725);
		this->alarmButton->Name = L"alarmButton";
		this->alarmButton->Size = System::Drawing::Size(135, 135);
		this->alarmButton->TabIndex = 0;
		this->alarmButton->Click += gcnew System::EventHandler(this, &BiopsyStudy::errorButton_Click);
		// 
		// cursorPanel
		// 
		this->cursorPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->cursorPanel->Location = System::Drawing::Point(12, 192);
		this->cursorPanel->Name = L"cursorPanel";
		this->cursorPanel->Size = System::Drawing::Size(174, 364);
		this->cursorPanel->TabIndex = 20;
		// 
		// forceStatus
		// 
		this->forceStatus->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->forceStatus->Controls->Add(this->labelForce);
		this->forceStatus->Location = System::Drawing::Point(306, 725);
		this->forceStatus->Name = L"forceStatus";
		this->forceStatus->Size = System::Drawing::Size(135, 135);
		this->forceStatus->TabIndex = 0;
		// 
		// labelForce
		// 
		this->labelForce->BackColor = System::Drawing::Color::Transparent;
		this->labelForce->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelForce->Location = System::Drawing::Point(0, 105);
		this->labelForce->Name = L"labelForce";
		this->labelForce->Size = System::Drawing::Size(135, 25);
		this->labelForce->TabIndex = 2;
		this->labelForce->Text = L"15.0 (Kg)";
		this->labelForce->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// thicknessStatus
		// 
		this->thicknessStatus->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->thicknessStatus->Controls->Add(this->labelThickness);
		this->thicknessStatus->Location = System::Drawing::Point(159, 725);
		this->thicknessStatus->Name = L"thicknessStatus";
		this->thicknessStatus->Size = System::Drawing::Size(135, 135);
		this->thicknessStatus->TabIndex = 0;
		// 
		// labelThickness
		// 
		this->labelThickness->BackColor = System::Drawing::Color::Transparent;
		this->labelThickness->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelThickness->Location = System::Drawing::Point(0, 105);
		this->labelThickness->Name = L"labelThickness";
		this->labelThickness->Size = System::Drawing::Size(135, 25);
		this->labelThickness->TabIndex = 0;
		this->labelThickness->Text = L"150 (mm)";
		this->labelThickness->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// doorStatus
		// 
		this->doorStatus->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->doorStatus->Location = System::Drawing::Point(453, 872);
		this->doorStatus->Name = L"doorStatus";
		this->doorStatus->Size = System::Drawing::Size(135, 135);
		this->doorStatus->TabIndex = 0;
		// 
		// tubeStatus
		// 
		this->tubeStatus->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->tubeStatus->Controls->Add(this->labelTubeData);
		this->tubeStatus->Location = System::Drawing::Point(306, 872);
		this->tubeStatus->Name = L"tubeStatus";
		this->tubeStatus->Size = System::Drawing::Size(135, 135);
		this->tubeStatus->TabIndex = 0;
		// 
		// labelTubeData
		// 
		this->labelTubeData->BackColor = System::Drawing::Color::Transparent;
		this->labelTubeData->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelTubeData->Location = System::Drawing::Point(0, 105);
		this->labelTubeData->Name = L"labelTubeData";
		this->labelTubeData->Size = System::Drawing::Size(135, 25);
		this->labelTubeData->TabIndex = 0;
		this->labelTubeData->Text = L"100%";
		this->labelTubeData->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// slideButton
		// 
		this->slideButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->slideButton->Location = System::Drawing::Point(159, 575);
		this->slideButton->Name = L"slideButton";
		this->slideButton->Size = System::Drawing::Size(135, 135);
		this->slideButton->TabIndex = 0;
		this->slideButton->Click += gcnew System::EventHandler(this, &BiopsyStudy::ShiftSelection_Click);
		// 
		// lampButton
		// 
		this->lampButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->lampButton->Location = System::Drawing::Point(159, 872);
		this->lampButton->Name = L"lampButton";
		this->lampButton->Size = System::Drawing::Size(135, 135);
		this->lampButton->TabIndex = 0;
		this->lampButton->Click += gcnew System::EventHandler(this, &BiopsyStudy::lampButton_Click);
		// 
		// keyStatus
		// 
		this->keyStatus->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->keyStatus->Location = System::Drawing::Point(12, 725);
		this->keyStatus->Name = L"keyStatus";
		this->keyStatus->Size = System::Drawing::Size(135, 135);
		this->keyStatus->TabIndex = 28;
		// 
		// BiopsyStudy
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
		this->Controls->Add(this->mainPanel);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Name = L"BiopsyStudy";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		this->mainPanel->ResumeLayout(false);
		this->mainPanel->PerformLayout();
		this->activationPanel->ResumeLayout(false);
		this->activationInfoPanel->ResumeLayout(false);
		this->pointerPosition->ResumeLayout(false);
		this->armPosition->ResumeLayout(false);
		this->xrayStat->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->demoIcon))->EndInit();
		this->forceStatus->ResumeLayout(false);
		this->thicknessStatus->ResumeLayout(false);
		this->tubeStatus->ResumeLayout(false);
		this->ResumeLayout(false);

	}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	private: System::Void onOperatingTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		SendMessageA(window, WINMSG_TIMER, 0, 0);

	}


	private: System::Void lampButton_Click(System::Object^ sender, System::EventArgs^ e);
	
	private: System::Void errorButton_Click(System::Object^ sender, System::EventArgs^ e);
	
	private: System::Void ShiftSelection_Click(System::Object^ sender, System::EventArgs^ e);



	private: System::Void confirmationButton_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		confirmation_button_status = true;
	}
};

