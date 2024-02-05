#pragma once

/// <summary>
/// \defgroup OperatingGUI_Module Operating Status Window Management Module
/// \ingroup GUI_Module Gantry GUI modules
/// 
/// 
/// </summary>
/// 


#include <windows.h>

using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class OperatingForm :  public System::Windows::Forms::Form
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
	Object^ pProj;
	Object^ pAbort;
	Object^ pShiftConf;
	void suspend(void);
	void resume(void);
	void onAbortConfirmOk(void);
	void onAbortConfirmCanc(void);
	void onConfirmOk(void);
	void onConfirmCanc(void);
	void onShiftConfirmOk(void);
	void onShiftConfirmCanc(void);

	void initOperatingStatus(void);
	void operatingStatusManagement(void);
	void evaluateXrayStatus(void);
	void evaluateReadyWarnings(bool reset);
	void evaluateCompressorStatus(void);
	void evaluateCompressorReleaseStatus(void);
	void evaluateCollimatorStatus(void);
	void evaluateMagStatus(void);
	void evaluateDoorStatus(void);
	void evaluateSlideStatus(void);
	void evaluateProjectionStatus(void);
	void evaluatePopupPanels(void);
	void onArmTargetChangedCallback(int id, int target);
	

public:System::Timers::Timer^ operatingTimer;

public:
	OperatingForm(void)
	{
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		formInitialization();

	}

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~OperatingForm()
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

// Projection selection panel
private: System::Windows::Forms::Panel^ projSelection;
private: System::Windows::Forms::Panel^ selectedIcon;
private: System::Windows::Forms::Label^ angleText;

// Collimation lamp activation
private: System::Windows::Forms::Panel^ lampButton;

// Slide selection
private: System::Windows::Forms::Panel^ slideButton; 

// Alarm button
private: System::Windows::Forms::Panel^ alarmButton;

// Collimation status
private: System::Windows::Forms::Panel^ collimationStatus;
private: System::Windows::Forms::Label^ labelColli;

// Paddle Status
private: System::Windows::Forms::Panel^ paddleStatus; 
private: System::Windows::Forms::Label^ labelPaddle;

// Thickness measurment
private: System::Windows::Forms::Panel^ thicknessStatus;
private: System::Windows::Forms::Label^ labelThickness;

// Force measurment
private: System::Windows::Forms::Panel^ forceStatus; 
private: System::Windows::Forms::Label^ labelForce;

// Magnifier status
private: System::Windows::Forms::Panel^ magnifierStatus;
private: System::Windows::Forms::Label^ labelMag;

// Release compression status
private: System::Windows::Forms::Panel^ decompressionStatus;

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
		this->magnifierStatus = (gcnew System::Windows::Forms::Panel());
		this->labelMag = (gcnew System::Windows::Forms::Label());
		this->mainPanel = (gcnew System::Windows::Forms::Panel());
		this->xrayStat = (gcnew System::Windows::Forms::Panel());
		this->demoIcon = (gcnew System::Windows::Forms::PictureBox());
		this->labelXrayStatus = (gcnew System::Windows::Forms::Label());
		this->alarmButton = (gcnew System::Windows::Forms::Panel());
		this->lampButton = (gcnew System::Windows::Forms::Panel());
		this->projSelection = (gcnew System::Windows::Forms::Panel());
		this->angleText = (gcnew System::Windows::Forms::Label());
		this->selectedIcon = (gcnew System::Windows::Forms::Panel());
		this->forceStatus = (gcnew System::Windows::Forms::Panel());
		this->labelForce = (gcnew System::Windows::Forms::Label());
		this->thicknessStatus = (gcnew System::Windows::Forms::Panel());
		this->labelThickness = (gcnew System::Windows::Forms::Label());
		this->paddleStatus = (gcnew System::Windows::Forms::Panel());
		this->labelPaddle = (gcnew System::Windows::Forms::Label());
		this->doorStatus = (gcnew System::Windows::Forms::Panel());
		this->tubeStatus = (gcnew System::Windows::Forms::Panel());
		this->labelTubeData = (gcnew System::Windows::Forms::Label());
		this->slideButton = (gcnew System::Windows::Forms::Panel());
		this->collimationStatus = (gcnew System::Windows::Forms::Panel());
		this->labelColli = (gcnew System::Windows::Forms::Label());
		this->decompressionStatus = (gcnew System::Windows::Forms::Panel());
		this->magnifierStatus->SuspendLayout();
		this->mainPanel->SuspendLayout();
		this->xrayStat->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->demoIcon))->BeginInit();
		this->projSelection->SuspendLayout();
		this->forceStatus->SuspendLayout();
		this->thicknessStatus->SuspendLayout();
		this->paddleStatus->SuspendLayout();
		this->tubeStatus->SuspendLayout();
		this->collimationStatus->SuspendLayout();
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
		// magnifierStatus
		// 
		this->magnifierStatus->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->magnifierStatus->Controls->Add(this->labelMag);
		this->magnifierStatus->Location = System::Drawing::Point(12, 872);
		this->magnifierStatus->Name = L"magnifierStatus";
		this->magnifierStatus->Size = System::Drawing::Size(135, 135);
		this->magnifierStatus->TabIndex = 0;
		// 
		// labelMag
		// 
		this->labelMag->BackColor = System::Drawing::Color::Transparent;
		this->labelMag->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelMag->Location = System::Drawing::Point(0, 110);
		this->labelMag->Name = L"labelMag";
		this->labelMag->Size = System::Drawing::Size(135, 25);
		this->labelMag->TabIndex = 0;
		this->labelMag->Text = L"MAG 1x";
		this->labelMag->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// mainPanel
		// 
		this->mainPanel->Controls->Add(this->xrayStat);
		this->mainPanel->Controls->Add(this->alarmButton);
		this->mainPanel->Controls->Add(this->lampButton);
		this->mainPanel->Controls->Add(this->projSelection);
		this->mainPanel->Controls->Add(this->forceStatus);
		this->mainPanel->Controls->Add(this->thicknessStatus);
		this->mainPanel->Controls->Add(this->paddleStatus);
		this->mainPanel->Controls->Add(this->doorStatus);
		this->mainPanel->Controls->Add(this->tubeStatus);
		this->mainPanel->Controls->Add(this->slideButton);
		this->mainPanel->Controls->Add(this->collimationStatus);
		this->mainPanel->Controls->Add(this->decompressionStatus);
		this->mainPanel->Controls->Add(this->labelDate);
		this->mainPanel->Controls->Add(this->magnifierStatus);
		this->mainPanel->Controls->Add(this->labelPatientName);
		this->mainPanel->Controls->Add(this->labelTime);
		this->mainPanel->Location = System::Drawing::Point(0, 0);
		this->mainPanel->Name = L"mainPanel";
		this->mainPanel->Size = System::Drawing::Size(600, 1024);
		this->mainPanel->TabIndex = 14;
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
		this->alarmButton->Location = System::Drawing::Point(404, 524);
		this->alarmButton->Name = L"alarmButton";
		this->alarmButton->Size = System::Drawing::Size(184, 184);
		this->alarmButton->TabIndex = 0;
		this->alarmButton->Click += gcnew System::EventHandler(this, &OperatingForm::errorButton_Click);
		// 
		// lampButton
		// 
		this->lampButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->lampButton->Location = System::Drawing::Point(12, 524);
		this->lampButton->Name = L"lampButton";
		this->lampButton->Size = System::Drawing::Size(184, 184);
		this->lampButton->TabIndex = 0;
		this->lampButton->Click += gcnew System::EventHandler(this, &OperatingForm::lampButton_Click);
		// 
		// projSelection
		// 
		this->projSelection->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->projSelection->Controls->Add(this->angleText);
		this->projSelection->Controls->Add(this->selectedIcon);
		this->projSelection->Location = System::Drawing::Point(12, 192);
		this->projSelection->Name = L"projSelection";
		this->projSelection->Size = System::Drawing::Size(576, 315);
		this->projSelection->TabIndex = 20;
		this->projSelection->Click += gcnew System::EventHandler(this, &OperatingForm::viewSelection_Click);
		// 
		// angleText
		// 
		this->angleText->BackColor = System::Drawing::Color::Transparent;
		this->angleText->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 50.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->angleText->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
		this->angleText->Location = System::Drawing::Point(101, 176);
		this->angleText->Margin = System::Windows::Forms::Padding(0);
		this->angleText->Name = L"angleText";
		this->angleText->Size = System::Drawing::Size(198, 91);
		this->angleText->TabIndex = 2;
		this->angleText->Text = L"-180°";
		this->angleText->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// selectedIcon
		// 
		this->selectedIcon->BackColor = System::Drawing::Color::Transparent;
		this->selectedIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->selectedIcon->Location = System::Drawing::Point(285, 47);
		this->selectedIcon->Name = L"selectedIcon";
		this->selectedIcon->Size = System::Drawing::Size(270, 220);
		this->selectedIcon->TabIndex = 0;
		this->selectedIcon->Click += gcnew System::EventHandler(this, &OperatingForm::viewSelection_Click);
		// 
		// forceStatus
		// 
		this->forceStatus->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->forceStatus->Controls->Add(this->labelForce);
		this->forceStatus->Location = System::Drawing::Point(453, 724);
		this->forceStatus->Name = L"forceStatus";
		this->forceStatus->Size = System::Drawing::Size(135, 135);
		this->forceStatus->TabIndex = 0;
		// 
		// labelForce
		// 
		this->labelForce->BackColor = System::Drawing::Color::Transparent;
		this->labelForce->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelForce->Location = System::Drawing::Point(0, 110);
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
		this->thicknessStatus->Location = System::Drawing::Point(306, 724);
		this->thicknessStatus->Name = L"thicknessStatus";
		this->thicknessStatus->Size = System::Drawing::Size(135, 135);
		this->thicknessStatus->TabIndex = 0;
		// 
		// labelThickness
		// 
		this->labelThickness->BackColor = System::Drawing::Color::Transparent;
		this->labelThickness->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelThickness->Location = System::Drawing::Point(0, 110);
		this->labelThickness->Name = L"labelThickness";
		this->labelThickness->Size = System::Drawing::Size(135, 25);
		this->labelThickness->TabIndex = 0;
		this->labelThickness->Text = L"150 (mm)";
		this->labelThickness->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// paddleStatus
		// 
		this->paddleStatus->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->paddleStatus->Controls->Add(this->labelPaddle);
		this->paddleStatus->Location = System::Drawing::Point(159, 724);
		this->paddleStatus->Name = L"paddleStatus";
		this->paddleStatus->Size = System::Drawing::Size(135, 135);
		this->paddleStatus->TabIndex = 0;
		// 
		// labelPaddle
		// 
		this->labelPaddle->BackColor = System::Drawing::Color::Transparent;
		this->labelPaddle->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelPaddle->Location = System::Drawing::Point(0, 110);
		this->labelPaddle->Name = L"labelPaddle";
		this->labelPaddle->Size = System::Drawing::Size(135, 25);
		this->labelPaddle->TabIndex = 1;
		this->labelPaddle->Text = L"PAD 24x30";
		this->labelPaddle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
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
		this->labelTubeData->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelTubeData->Location = System::Drawing::Point(0, 110);
		this->labelTubeData->Name = L"labelTubeData";
		this->labelTubeData->Size = System::Drawing::Size(135, 25);
		this->labelTubeData->TabIndex = 0;
		this->labelTubeData->Text = L"100%";
		this->labelTubeData->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// slideButton
		// 
		this->slideButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->slideButton->Location = System::Drawing::Point(208, 524);
		this->slideButton->Name = L"slideButton";
		this->slideButton->Size = System::Drawing::Size(184, 184);
		this->slideButton->TabIndex = 0;
		this->slideButton->Click += gcnew System::EventHandler(this, &OperatingForm::ShiftSelection_Click);
		// 
		// collimationStatus
		// 
		this->collimationStatus->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->collimationStatus->Controls->Add(this->labelColli);
		this->collimationStatus->Location = System::Drawing::Point(12, 725);
		this->collimationStatus->Name = L"collimationStatus";
		this->collimationStatus->Size = System::Drawing::Size(135, 135);
		this->collimationStatus->TabIndex = 0;
		// 
		// labelColli
		// 
		this->labelColli->BackColor = System::Drawing::Color::Transparent;
		this->labelColli->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelColli->Location = System::Drawing::Point(0, 110);
		this->labelColli->Name = L"labelColli";
		this->labelColli->Size = System::Drawing::Size(135, 25);
		this->labelColli->TabIndex = 0;
		this->labelColli->Text = L"AUTO";
		this->labelColli->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// decompressionStatus
		// 
		this->decompressionStatus->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->decompressionStatus->Location = System::Drawing::Point(159, 872);
		this->decompressionStatus->Name = L"decompressionStatus";
		this->decompressionStatus->Size = System::Drawing::Size(135, 135);
		this->decompressionStatus->TabIndex = 0;
		// 
		// OperatingForm
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
		this->Controls->Add(this->mainPanel);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Name = L"OperatingForm";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		this->magnifierStatus->ResumeLayout(false);
		this->mainPanel->ResumeLayout(false);
		this->mainPanel->PerformLayout();
		this->xrayStat->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->demoIcon))->EndInit();
		this->projSelection->ResumeLayout(false);
		this->forceStatus->ResumeLayout(false);
		this->thicknessStatus->ResumeLayout(false);
		this->paddleStatus->ResumeLayout(false);
		this->tubeStatus->ResumeLayout(false);
		this->collimationStatus->ResumeLayout(false);
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
	private: System::Void viewSelection_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void ShiftSelection_Click(System::Object^ sender, System::EventArgs^ e);

};

