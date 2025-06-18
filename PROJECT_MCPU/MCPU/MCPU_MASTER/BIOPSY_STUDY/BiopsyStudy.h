#pragma once


/**
	\defgroup BiopsyOperatingMode Biopsy Operating Mode Management	

	This section describes the implementation of the Biopsy Operating Mode

	# To be implemented

*/


/**
\defgroup BiopsyOperatingModeDescription Biopsy Operating Mode Description
\ingroup BiopsyOperatingMode

   
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

	# GUI description 

	The Biopsy study module display a dedicate GUI following described:

	\image html ./BiopsyStudy/GUI.png

	The section (A) of the GUI is dedicated to the header, containing the info
	about the installation name and the current date and time;

	The section (B) is dedicated to the X-RAY status and the type of running mode:
	+ Standby: this is the not ready condition status;
	+ Ready: the study is ready to activate any available exposure;
	
	See the following table with the possible scenarios:

	|Icon|Description|
	|:--:|:--:|
	|\image html ./BiopsyStudy/XStdby.png|Standby status|
	|\image html ./BiopsyStudy/XReady.png|Ready condition|
	|\image html ./BiopsyStudy/XOn.png|Exposure sequence in progress|
	|\image html ./BiopsyStudy/Demo.png|Exposure in demo mode|


	The section (C) is dedicated to the Needle position correction 
	when a Pointing action has successfully be executed.

	The section (D) is dedicated to the Pointer status and Pointer activation 
	pictures.

	Section (E) is dedicated to the Gantry status monitoring and 
	activation buttons:
	+ Slide activation button;
	+ Message window activation button;
	+ Collimation light activation;
	+ Control Image activation button;


	# Motors management
	
	## C-ARM Rotation Description
	
	In the Biopsy Study the following rules apply to the C-ARM motor activation:
	- The motor can be activated only in manual mode (manual keyboard buttons);
	- The maximum rotation angles will be limited to +/- 90°;
	- The Isocentric correction feature is disabled (no Vertical correction will be executed);

	Current angle visualization:
	+ During the C-ARM rotation, a Dialog on the displays will show the actual ARM angle. 
	The panel will disappear in few seconds after the activation termination;
	+ During the C-ARM rotation, the potter's display will show the actual ARM angle;
	
	The current C-ARM angle is currently displayed on a dedicated graphical box of the GUI:
	\image html ./BiopsyStudy/motors.png


	## Tilt motor

	The Tilt motor can be activated only with automatic rotation:
	+ The AWS software will command a proper Tilt angle based on the current workflow operation.
	
	\note There isn't any special rule in order to enable the Tilt to move. It can always be moved
	in the Biopsy Study.

	Current angle visualization:
	+ During the Tilt rotation, a Dialog on the displays will show the actual Tilt angle.
	The panel will disappear in few seconds after the activation termination;
	+ During the Tilt rotation, the potter's display will show the actual Tilt angle;
	
	The current Tilt angle is currently displayed on a dedicated graphical box of the GUI:
	\image html ./BiopsyStudy/motors.png

	
	## Slide motor

	The module provides a dedicated graphical button in the GUI 
	to allow the operatore to start an automatic Slide activation in two possible 
	target angles:
	+ 0° (standard position);
	+ 90° (usually needed with the table).

	\note The Isocentric correction feature is disabled (no Vertical correction will be executed);

	The table below describes the button activations:

	|Icon|Description|
	|:--:|:--:|
	|\image html ./BiopsyStudy/slide_to_0.png|The current Slide angle is 90°. Pressing this button the Slide motor will move to 0°|
	|\image html ./BiopsyStudy/slide_to_90.png|The current Slide angle is 0°. Pressing this button the Slide motor will move to 90°|
	
	Current angle visualization:
	+ During the Slide rotation, a Dialog on the displays will show the actual Slide angle.
	The panel will disappear in few seconds after the activation termination;
	+ During the Slide rotation, the potter's display will show the actual Slide angle;

	## Vertical Motor

	In the Biopsy Study the following rules apply to the Vertical motor activation:
	- The motor can be activated only in manual mode (manual keyboard buttons or Pedals);
	- The Isocentric correction feature is disabled (no Vertical correction will be executed with other motor activations);

	Current angle visualization:
	+ During the Slide rotation, a Dialog on the displays will show the actual Slide angle.
	The panel will disappear in few seconds after the activation termination;
	+ During the Slide rotation, the potter's display will show the actual Slide angle;
	+ The current Slide angle is currently displayed on a dedicated graphical box of the GUI;


	## Body motor	
	
	No activation can be executed in Biopsy Study.

	#  Compressor Management

	The compressor usage and performances are described in detail in a dedicated section.
	In this description it is reported the modalities and data visualization details.

	## Compression Force

	The current compression force is displayed in a proper box of the GUI
	as following described:

	When the AWS should set the Exposure mode without compression,
	the Icon has the following appearence:
	
	|Icon|Description|
	|:--:|:--:|
	|\image html ./BiopsyStudy/ForceDisabledNotCompressed.png|No compression is detected|
	|\image html ./BiopsyStudy/ForceDisabledCompressed.png|A compression has been detected|

	When the AWS should set the Exposure mode with compression,
	the Icon has the following appearence:
	
	|Icon|Description|
	|:--:|:--:|
	|\image html ./BiopsyStudy/ForceEnabledNotCompressed.png|No compression is detected|
	|\image html ./BiopsyStudy/ForceEnabledCompressed.png|A compression has been detected|

	In both conditions, if the compression should be detected, its value in N is displayed into the Icon box.

	See the Exposure section for details about the available compression modes.

	## Breast Thickness measurement

	As described in the Compressor module section,
	the thickness measurement takes place when:
	+ a valid compressor paddle is detected;
	+ a valid compression forse is detected.

	A dedicated box in the GUI reports the current paddle detection status 
	and, if applicable, the current thickness measured.

	|Icon|Description|
	|:--:|:--:|
	|\image html ./BiopsyStudy/ThicknessDisabled.png|No paddle detected|
	|\image html ./BiopsyStudy/ThicknessEnabled.png|A valid paddle is detected|

	In the case a valid thickness should be available, the current value in mm
	is displayed in the Icon Box.

	 
	# Control Image Management
	
	\todo Biopsy Control Image : implementare la funzione di visualizzazione immagine di controllo

	# Tube Temperature monitoring

	The Biopsy Study monitors the status of the Tube 
	providing the current information about the maximum percentage of 
	Bulb or Stator internal sensor:
	+ only the max value of the two sensors is displayed here.

	The current percentage value is displayed in a proper GUI box:

	|Icon|Description|
	|:--:|:--:|
	|\image html ./BiopsyStudy/TubeNok.png|The tube temperature is out of range (alarm condition)|
	|\image html ./BiopsyStudy/TubeOk.png|The tube temperature is below the alarm threshold|
	

	\note in the case the percantage should exceed a threshold 
	of 95%, no more exposure can be initiated. An error message 
	is activated.


	See the Collimator module for more details.


	# Study Door Monitoring

	The current status of the Study door is monitored 
	to warn the operator in the case the door should be open.

	|Icon|Description|
	|:--:|:--:|
	|\image html ./BiopsyStudy/DoorClosed.png|The study's door is closed|
	|\image html ./BiopsyStudy/DoorOpen.png|The study's door is open|

	\note In case the Door should be open the exposure is disabled for safety reasons.

	# Collimation Management
	
	In Biopsy study the collimator works as described in the Collimator module 
	section. No special function are handled in this operating mode.

	## Collimation Light

	The collimation light works as descriobed in the Collimator management section:
	+ it is activated when the compressor is activated for a limited time.

	The operator however can manually activate the collimation light
	clicking the dedicated button on the GUI:

	\image html ./BiopsyStudy/LampOff.png

	When the light is On the icon changes into the following:

	\image html ./BiopsyStudy/LampOn.png

	# Active Messages

	In case some message should be active a graphic push button
	opens a dedicated window, showing a list of active messages:
	+ (see \ref MessageNotifyDescription)

	|Icon|Description|
	|:--:|:--:|
	|\image html ./BiopsyStudy/AlarmOff.png|No active messages are present|
	|\image html ./BiopsyStudy/AlarmOn.png|Almost one Error message is active|
	|\image html ./BiopsyStudy/WarningOn.png|No Errors and almost one Warning message is active|
	
	# Exposure Management

	## Available exposure sequences

	In the Biopsy Study module, the followings Exposure modes can be executed:
	+ Manual 2D exposure;
	+ AEC 2D exposure;
	+ Manual 3D exposure;
	+ AEC 3D exposure;

	It is up the AWS to select the proper exposure mode based on the given workflow.
	See the Exposure module management for details.

	## Available Exposure mode options

	The modul e allows to use the following options:
	+ Compression Modes: DISABLE and KEEP are available options. The UNLOCK mode is not allowed in Biopsy study;
	+ Collimation Mode: option to select the type of collimation;

	The following options are not applicable:
	+ Compressor UNLOCK mode: the compressor unlock mode is not allowed in Biopsy study;
	+ Arm Mode: the current ARM angle is not controlled;
	+ Patient protection mode: the patient protection is not present in Biopsy.

	## Ready Conditions

	The Module checks the following conditions in order to 
	enable the execution of an Exposure sequence:
	+ No active error messages;
	+ When enabled, a valid compression shall be detected;
	+ Valid exposure data shall be set by the AWS;
	+ The xray push button shall be enabled by the AWS;

	The not ready conditions will ativate Warning messages
	that the operator may checks in the case of unexpected Not Ready
	visualization status.

	# Pointer Control

	## General rules

	### X-SCROLL

	The X-Axis can be mechanically scrolled in three possible positions: Left, Center and Right;
	+ The Module monitors the current position identifying an invalid position selection.

	\note Every Home position requires a given X-SCROLL position

	### Y Up/Down

	The Module detects the current status of the Y vertical position:
	+ Tourned Up;
	+ Tourned Down;

	\note Every Home position requires a given Y position;

	\note The pointer activation sequence depends by the current rotation position of the Y-Axes:
	+ the module prevents a possible impact with the Z-Base that may be possible if the Y should be tourned Down.

	### Home position

	The Module implements three possible Home position:
	+ HOME CENTER: pointer in the center of the axis and the X-SCROLL set in the center and Y tourned Up;
	+ HOME LEFT: pointer in the most left position and the X-SCROLL set in the left position and Y tourned Down;
	+ HOME RIGHT: pointer in the most right position and the X-SCROLL set in the right position and Y tourned Down;

	See the Home Procedure Description section for details.

	### Pointing activation

	The module provides dedicated methods to move a Pointer to a target position detected with the X-RAY biopsy procedure.
	The following rules are implemented in order to prevent operator mistakes:
	+ The Pointing activation can start only when a valid Home position has been selected first;
	+ Only one pointing at a time can be permitted: after a pointing is completed a next pointing will require a new Home position selectiuon first;

	See the Pointing Procedure Description section for details.

	## External Keyboard Usage

	The optional external keyboard provides the operator to modify the current pointer position in all the directions (X,Y,Z)
	with small steps of about 1mm.

	The module enables the Keyboard usage only after a valid Pointing activation:
	+ the keyboard is disabled as soon as an home position is selected;
	+ the keyboard is disabled at the Biopsy Study activation;

	The current activation status is displayed by the proper icon on the GUI:
	|Icon|Description|
	|:--:|:--:|
	|\image html ./BiopsyStudy/key_disabled.png|The Keyboard is currently disabled|
	|\image html ./BiopsyStudy/key_enabled.png|The Keyboard is currently enabled|

	## Current Pointer Status

	When there is no activation pending (homing or pointing activities)
	the current pointer status is described by a graphical visualization
	and a coordinate box visualization.

	### Grapical visualization description

	|Icon|Description|
	|:--:|:--:|
	|\image html ./BiopsyStudy/BYM_IMG/STATUS/NO_HOME_STATUS.png|No valid Home position is selected (or detected)|
	|\image html ./BiopsyStudy/BYM_IMG/STATUS/HOME_CENTER_STATUS.png|The Pointer is in Home Center position|
	|\image html ./BiopsyStudy/BYM_IMG/STATUS/HOME_LEFT_STATUS.png|The Pointer is in Home Left position|
	|\image html ./BiopsyStudy/BYM_IMG/STATUS/HOME_RIGHT_STATUS.png|The Pointer is in Home Right position|
	|\image html ./BiopsyStudy/BYM_IMG/STATUS/POINTED_CENTER_STATUS.png|The Pointer is pointing to a target of an upright approach|
	|\image html ./BiopsyStudy/BYM_IMG/STATUS/POINTED_LEFT_STATUS.png|The Pointer is pointing to a target of a Left approach|
	|\image html ./BiopsyStudy/BYM_IMG/STATUS/POINTED_RIGHT_STATUS.png|The Pointer is pointing to a target of a Right approach|

	### Coordinate Box

	A GUI box will display the current X,Y,Z coordinates:

	\image html ./BiopsyStudy/Coordinates.png



*/

/**
	\defgroup BiopsyOperatingModeClass Biopsy Operating Mode Implementation
	\ingroup BiopsyOperatingMode

	This section describes the class implementation details
*/

#include <windows.h>

using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

/// <summary>
/// 
/// This is the Class constructor.
/// 
/// \ingroup BiopsyOperatingModeClass
/// 
/// </summary>
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




private: System::Windows::Forms::Panel^ activationInfoPanel;
private: System::Windows::Forms::Panel^ activationInfoIcon;
private: System::Windows::Forms::Label^ activationInfoText;

private: System::Windows::Forms::Panel^ keyStatus;
private: System::Windows::Forms::Panel^ statusPanel;








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
	void evaluatePointerPictures(bool init);

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
		this->keyStatus = (gcnew System::Windows::Forms::Panel());
		this->statusPanel = (gcnew System::Windows::Forms::Panel());
		this->activationInfoPanel = (gcnew System::Windows::Forms::Panel());
		this->activationInfoText = (gcnew System::Windows::Forms::Label());
		this->activationInfoIcon = (gcnew System::Windows::Forms::Panel());
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
		this->mainPanel->SuspendLayout();
		this->statusPanel->SuspendLayout();
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
		// keyStatus
		// 
		this->keyStatus->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->keyStatus->Location = System::Drawing::Point(12, 725);
		this->keyStatus->Name = L"keyStatus";
		this->keyStatus->Size = System::Drawing::Size(135, 135);
		this->keyStatus->TabIndex = 28;
		// 
		// statusPanel
		// 
		this->statusPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->statusPanel->Controls->Add(this->activationInfoPanel);
		this->statusPanel->Controls->Add(this->confirmationButton);
		this->statusPanel->Location = System::Drawing::Point(192, 192);
		this->statusPanel->Name = L"statusPanel";
		this->statusPanel->Size = System::Drawing::Size(396, 364);
		this->statusPanel->TabIndex = 26;
		// 
		// activationInfoPanel
		// 
		this->activationInfoPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->activationInfoPanel->Controls->Add(this->activationInfoText);
		this->activationInfoPanel->Controls->Add(this->activationInfoIcon);
		this->activationInfoPanel->Location = System::Drawing::Point(3, 304);
		this->activationInfoPanel->Name = L"activationInfoPanel";
		this->activationInfoPanel->Size = System::Drawing::Size(390, 57);
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
		// confirmationButton
		// 
		this->confirmationButton->BackColor = System::Drawing::Color::Red;
		this->confirmationButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->confirmationButton->Location = System::Drawing::Point(98, 52);
		this->confirmationButton->Name = L"confirmationButton";
		this->confirmationButton->Size = System::Drawing::Size(200, 200);
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
		this->statusPanel->ResumeLayout(false);
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

