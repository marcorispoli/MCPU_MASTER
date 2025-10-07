#pragma once

/// <summary>
/// \defgroup TestWindowModule Test Study Window Management Module
/// \ingroup GUI_Module
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

public ref class TestStudy :  public System::Windows::Forms::Form
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
	Object^ pAec;

private: System::Windows::Forms::Panel^ aecButton;
private: System::Windows::Forms::Label^ labelAec;
public:
	Object^ pShiftConf;
	void suspend(void);
	void resume(void);
	
	void onShiftConfirmOk(void);
	void onShiftConfirmCanc(void);

	void initOperatingStatus(void);
	void operatingStatusManagement(void);
	void evaluateXrayStatus(void);
	void evaluateReadyWarnings(bool reset);
	void evaluateCompressorStatus(bool init);
	void evaluateCollimatorStatus(void);
	void evaluateSlideStatus(bool init);
	void evaluatePopupPanels(void);
	void evaluateGridStatus(void);
	void evaluateAwsComponentEvent(void);
	

public:System::Timers::Timer^ operatingTimer;

public:
	TestStudy(void)
	{
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		formInitialization();

	}

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~TestStudy()
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
private: System::Windows::Forms::Label^ labelTestName;
private: System::Windows::Forms::Label^ labelTime;

// Xray Status Bar
private: System::Windows::Forms::Panel^ xrayStat;
private: System::Windows::Forms::Label^ labelXrayStatus;
private: System::Windows::Forms::PictureBox^ demoIcon;

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


// Tube data 
private: System::Windows::Forms::Panel^ tubeStatus;
private: System::Windows::Forms::Label^ labelTubeData;


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
		this->labelTestName = (gcnew System::Windows::Forms::Label());
		this->labelTime = (gcnew System::Windows::Forms::Label());
		this->mainPanel = (gcnew System::Windows::Forms::Panel());
		this->xrayStat = (gcnew System::Windows::Forms::Panel());
		this->demoIcon = (gcnew System::Windows::Forms::PictureBox());
		this->labelXrayStatus = (gcnew System::Windows::Forms::Label());
		this->alarmButton = (gcnew System::Windows::Forms::Panel());
		this->forceStatus = (gcnew System::Windows::Forms::Panel());
		this->labelForce = (gcnew System::Windows::Forms::Label());
		this->thicknessStatus = (gcnew System::Windows::Forms::Panel());
		this->labelThickness = (gcnew System::Windows::Forms::Label());
		this->paddleStatus = (gcnew System::Windows::Forms::Panel());
		this->labelPaddle = (gcnew System::Windows::Forms::Label());
		this->tubeStatus = (gcnew System::Windows::Forms::Panel());
		this->labelTubeData = (gcnew System::Windows::Forms::Label());
		this->slideButton = (gcnew System::Windows::Forms::Panel());
		this->collimationStatus = (gcnew System::Windows::Forms::Panel());
		this->labelColli = (gcnew System::Windows::Forms::Label());
		this->aecButton = (gcnew System::Windows::Forms::Panel());
		this->labelAec = (gcnew System::Windows::Forms::Label());
		this->mainPanel->SuspendLayout();
		this->xrayStat->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->demoIcon))->BeginInit();
		this->forceStatus->SuspendLayout();
		this->thicknessStatus->SuspendLayout();
		this->paddleStatus->SuspendLayout();
		this->tubeStatus->SuspendLayout();
		this->collimationStatus->SuspendLayout();
		this->aecButton->SuspendLayout();
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
		this->labelTestName->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular,
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->labelTestName->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)), static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->labelTestName->Location = System::Drawing::Point(143, 13);
		this->labelTestName->Name = L"labelPatientName";
		this->labelTestName->Size = System::Drawing::Size(305, 24);
		this->labelTestName->TabIndex = 8;
		this->labelTestName->Text = L"pippo caio sempronio";
		this->labelTestName->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
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
		// mainPanel
		// 
		this->mainPanel->Controls->Add(this->aecButton);
		this->mainPanel->Controls->Add(this->xrayStat);
		this->mainPanel->Controls->Add(this->alarmButton);		
		this->mainPanel->Controls->Add(this->forceStatus);
		this->mainPanel->Controls->Add(this->thicknessStatus);
		this->mainPanel->Controls->Add(this->paddleStatus);
		this->mainPanel->Controls->Add(this->tubeStatus);
		this->mainPanel->Controls->Add(this->slideButton);
		this->mainPanel->Controls->Add(this->collimationStatus);
		this->mainPanel->Controls->Add(this->labelDate);
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
		this->alarmButton->Location = System::Drawing::Point(453, 524);
		this->alarmButton->Name = L"alarmButton";
		this->alarmButton->Size = System::Drawing::Size(135, 135);
		this->alarmButton->TabIndex = 0;
		this->alarmButton->Click += gcnew System::EventHandler(this, &TestStudy::errorButton_Click);						
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
		this->labelThickness->Location = System::Drawing::Point(0, 105);
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
		this->labelPaddle->Location = System::Drawing::Point(0, 105);
		this->labelPaddle->Name = L"labelPaddle";
		this->labelPaddle->Size = System::Drawing::Size(135, 25);
		this->labelPaddle->TabIndex = 1;
		this->labelPaddle->Text = L"PAD 24x30";
		this->labelPaddle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;		
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
		this->slideButton->Location = System::Drawing::Point(159, 524);
		this->slideButton->Name = L"slideButton";
		this->slideButton->Size = System::Drawing::Size(135, 135);
		this->slideButton->TabIndex = 0;
		this->slideButton->Click += gcnew System::EventHandler(this, &TestStudy::ShiftSelection_Click);
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
		this->labelColli->Location = System::Drawing::Point(0, 105);
		this->labelColli->Name = L"labelColli";
		this->labelColli->Size = System::Drawing::Size(135, 25);
		this->labelColli->TabIndex = 0;
		this->labelColli->Text = L"AUTO";
		this->labelColli->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;		
		// 
		// aecButton
		// 
		this->aecButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->aecButton->Controls->Add(this->labelAec);
		this->aecButton->Location = System::Drawing::Point(306, 524);
		this->aecButton->Name = L"aecButton";
		this->aecButton->Size = System::Drawing::Size(135, 135);
		this->aecButton->TabIndex = 24;
		this->aecButton->Click += gcnew System::EventHandler(this, &TestStudy::aecButton_Click);
		// 
		// labelAec
		// 
		this->labelAec->BackColor = System::Drawing::Color::Transparent;
		this->labelAec->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelAec->Location = System::Drawing::Point(0, 101);
		this->labelAec->Name = L"labelAec";
		this->labelAec->Size = System::Drawing::Size(135, 25);
		this->labelAec->TabIndex = 1;
		this->labelAec->Text = L"AUTO";
		this->labelAec->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
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
		this->mainPanel->ResumeLayout(false);
		this->mainPanel->PerformLayout();
		this->xrayStat->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->demoIcon))->EndInit();		
		this->forceStatus->ResumeLayout(false);
		this->thicknessStatus->ResumeLayout(false);
		this->paddleStatus->ResumeLayout(false);
		this->tubeStatus->ResumeLayout(false);
		this->collimationStatus->ResumeLayout(false);
		this->aecButton->ResumeLayout(false);
		this->ResumeLayout(false);

	}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	private: System::Void onOperatingTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		SendMessageA(window, WINMSG_TIMER, 0, 0);
	}
	
	private: System::Void aecButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void errorButton_Click(System::Object^ sender, System::EventArgs^ e);	
	private: System::Void ShiftSelection_Click(System::Object^ sender, System::EventArgs^ e);

};

