#pragma once

/// <summary>
/// \defgroup BiopsyGUI_Module Biopsy Operating Status Window Management Module
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

public ref class BiopsyForm :  public System::Windows::Forms::Form
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
private: System::Windows::Forms::Panel^ biopsyPanel;



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

	void evaluateDoorStatus(void);
	void evaluateCollimatorStatus(void);
	void evaluateSlideStatus(bool init);
	void evaluatePopupPanels(void);
	void evaluateDigitDisplays(void);
	void evaluateGridStatus(void);
	void evaluateAwsComponentEvent(void);
	

public:System::Timers::Timer^ operatingTimer;

public:
	BiopsyForm(void)
	{
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		formInitialization();

	}

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~BiopsyForm()
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
private: System::Windows::Forms::Panel^ biopsyImageButton;
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
		this->biopsyImageButton = (gcnew System::Windows::Forms::Panel());
		this->mainPanel = (gcnew System::Windows::Forms::Panel());
		this->biopsyPanel = (gcnew System::Windows::Forms::Panel());
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
		// biopsyImageButton
		// 
		this->biopsyImageButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->biopsyImageButton->Location = System::Drawing::Point(12, 872);
		this->biopsyImageButton->Name = L"biopsyImageButton";
		this->biopsyImageButton->Size = System::Drawing::Size(135, 135);
		this->biopsyImageButton->TabIndex = 0;
		// 
		// mainPanel
		// 
		this->mainPanel->Controls->Add(this->biopsyPanel);
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
		this->mainPanel->Controls->Add(this->biopsyImageButton);
		this->mainPanel->Controls->Add(this->labelPatientName);
		this->mainPanel->Controls->Add(this->labelTime);
		this->mainPanel->Location = System::Drawing::Point(0, 0);
		this->mainPanel->Name = L"mainPanel";
		this->mainPanel->Size = System::Drawing::Size(600, 1024);
		this->mainPanel->TabIndex = 14;
		// 
		// biopsyPanel
		// 
		this->biopsyPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->biopsyPanel->Location = System::Drawing::Point(192, 192);
		this->biopsyPanel->Name = L"biopsyPanel";
		this->biopsyPanel->Size = System::Drawing::Size(396, 364);
		this->biopsyPanel->TabIndex = 26;
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
		this->armPosition->Size = System::Drawing::Size(282, 135);
		this->armPosition->TabIndex = 24;
		// 
		// tiltLabel
		// 
		this->tiltLabel->BackColor = System::Drawing::Color::Transparent;
		this->tiltLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->tiltLabel->Location = System::Drawing::Point(147, 74);
		this->tiltLabel->Name = L"tiltLabel";
		this->tiltLabel->Size = System::Drawing::Size(135, 25);
		this->tiltLabel->TabIndex = 2;
		this->tiltLabel->Text = L"TILT: 0°";
		this->tiltLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// armLabel
		// 
		this->armLabel->BackColor = System::Drawing::Color::Transparent;
		this->armLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->armLabel->Location = System::Drawing::Point(147, 36);
		this->armLabel->Name = L"armLabel";
		this->armLabel->Size = System::Drawing::Size(135, 25);
		this->armLabel->TabIndex = 1;
		this->armLabel->Text = L"ARM: 0°";
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
		this->alarmButton->Click += gcnew System::EventHandler(this, &BiopsyForm::errorButton_Click);
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
		this->labelForce->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
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
		this->labelThickness->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
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
		this->labelTubeData->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
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
		this->slideButton->Location = System::Drawing::Point(12, 725);
		this->slideButton->Name = L"slideButton";
		this->slideButton->Size = System::Drawing::Size(135, 135);
		this->slideButton->TabIndex = 0;
		this->slideButton->Click += gcnew System::EventHandler(this, &BiopsyForm::ShiftSelection_Click);
		// 
		// lampButton
		// 
		this->lampButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->lampButton->Location = System::Drawing::Point(159, 872);
		this->lampButton->Name = L"lampButton";
		this->lampButton->Size = System::Drawing::Size(135, 135);
		this->lampButton->TabIndex = 0;
		this->lampButton->Click += gcnew System::EventHandler(this, &BiopsyForm::lampButton_Click);
		// 
		// BiopsyForm
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
		this->Controls->Add(this->mainPanel);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Name = L"BiopsyForm";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		this->mainPanel->ResumeLayout(false);
		this->mainPanel->PerformLayout();
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


};

