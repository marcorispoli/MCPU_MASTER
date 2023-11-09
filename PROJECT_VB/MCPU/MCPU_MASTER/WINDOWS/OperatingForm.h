#pragma once

#include <windows.h>

using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class OperatingForm :  public System::Windows::Forms::Form
{
	

public:

	public:System::Timers::Timer^ operatingTimer;
	
private: System::Windows::Forms::Label^ labelDate;
private: System::Windows::Forms::Label^ labelPatientName;

private: System::Windows::Forms::Label^ labelTime;



private: System::Windows::Forms::Panel^ magnifierStatus;

private: System::Windows::Forms::Panel^ mainPanel;


private: System::Windows::Forms::Panel^ residualExposures;

private: System::Windows::Forms::Panel^ decompressionStatus;

private: System::Windows::Forms::Panel^ xrayStat;
private: System::Windows::Forms::Panel^ alarmButton;
private: System::Windows::Forms::Panel^ lampButton;
private: System::Windows::Forms::Panel^ projSelection;

private: System::Windows::Forms::Panel^ forceStatus;


private: System::Windows::Forms::Panel^ thicknessStatus;

private: System::Windows::Forms::Panel^ paddleStatus;
private: System::Windows::Forms::Panel^ doorStatus;


private: System::Windows::Forms::Panel^ tubeStatus;

private: System::Windows::Forms::Panel^ collimationStatus;
private: System::Windows::Forms::Label^ labelXrayStatus;
private: System::Windows::Forms::Label^ labelPaddle;
private: System::Windows::Forms::Label^ labelForce;

private: System::Windows::Forms::Label^ labelThickness;
private: System::Windows::Forms::Label^ labelMag;
private: System::Windows::Forms::Label^ labelTubeData;




private: System::Windows::Forms::Label^ labelColli;



public:

public:
	public:HWND window;
private: System::Windows::Forms::Label^ angleText;
private: System::Windows::Forms::Panel^ selectedIcon;
public:

public:
	public:	void open(void);
	public:	void close(void);
	private: 
		bool open_status;
		Object^ pProj;
		Object^ pAbort;
		Object^ pError;

		void formInitialization(void);

		void onAbortConfirmOk(void);
		void onAbortConfirmCanc(void);
		void onConfirmOk(void);
		void onConfirmCanc(void);

		void initOperatingStatus(void);

		void operatingStatusManagement(void);
		void evaluateXrayStatus(void);
		void evaluateErrorStatus(void);
		void evaluateCompressorStatus(void);
		void evaluateCollimatorStatus(void);
		
		void evaluateMagStatus(void);
		void evaluateDoorStatus(void);


		void onArmTargetChangedCallback(int id, int target);
		void onArmAbortTargetCallback(void);
		void onArmPositionChangeCallback(void);

		// Command flags
		bool collimator_light_activation;

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

protected:









protected:
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
		this->collimationStatus = (gcnew System::Windows::Forms::Panel());
		this->labelColli = (gcnew System::Windows::Forms::Label());
		this->residualExposures = (gcnew System::Windows::Forms::Panel());
		this->decompressionStatus = (gcnew System::Windows::Forms::Panel());
		this->magnifierStatus->SuspendLayout();
		this->mainPanel->SuspendLayout();
		this->xrayStat->SuspendLayout();
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
		this->magnifierStatus->Controls->Add(this->labelMag);
		this->magnifierStatus->Location = System::Drawing::Point(22, 834);
		this->magnifierStatus->Name = L"magnifierStatus";
		this->magnifierStatus->Size = System::Drawing::Size(170, 170);
		this->magnifierStatus->TabIndex = 13;
		// 
		// labelMag
		// 
		this->labelMag->BackColor = System::Drawing::Color::Transparent;
		this->labelMag->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelMag->Location = System::Drawing::Point(5, 135);
		this->labelMag->Name = L"labelMag";
		this->labelMag->Size = System::Drawing::Size(160, 30);
		this->labelMag->TabIndex = 2;
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
		this->mainPanel->Controls->Add(this->collimationStatus);
		this->mainPanel->Controls->Add(this->residualExposures);
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
		this->xrayStat->Controls->Add(this->labelXrayStatus);
		this->xrayStat->Location = System::Drawing::Point(22, 61);
		this->xrayStat->Name = L"xrayStat";
		this->xrayStat->Size = System::Drawing::Size(554, 120);
		this->xrayStat->TabIndex = 23;
		// 
		// labelXrayStatus
		// 
		this->labelXrayStatus->BackColor = System::Drawing::Color::Transparent;
		this->labelXrayStatus->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 26.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelXrayStatus->Location = System::Drawing::Point(19, 21);
		this->labelXrayStatus->Name = L"labelXrayStatus";
		this->labelXrayStatus->Size = System::Drawing::Size(514, 82);
		this->labelXrayStatus->TabIndex = 0;
		this->labelXrayStatus->Text = L"STAND-BY";
		this->labelXrayStatus->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// alarmButton
		// 
		this->alarmButton->Location = System::Drawing::Point(471, 329);
		this->alarmButton->Name = L"alarmButton";
		this->alarmButton->Size = System::Drawing::Size(105, 105);
		this->alarmButton->TabIndex = 22;
		this->alarmButton->Click += gcnew System::EventHandler(this, &OperatingForm::errorButton_Click);
		// 
		// lampButton
		// 
		this->lampButton->Location = System::Drawing::Point(471, 200);
		this->lampButton->Name = L"lampButton";
		this->lampButton->Size = System::Drawing::Size(105, 105);
		this->lampButton->TabIndex = 21;
		this->lampButton->Click += gcnew System::EventHandler(this, &OperatingForm::lampButton_Click);
		// 
		// projSelection
		// 
		this->projSelection->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->projSelection->Controls->Add(this->angleText);
		this->projSelection->Controls->Add(this->selectedIcon);
		this->projSelection->Location = System::Drawing::Point(22, 200);
		this->projSelection->Name = L"projSelection";
		this->projSelection->Size = System::Drawing::Size(430, 234);
		this->projSelection->TabIndex = 20;
		this->projSelection->Click += gcnew System::EventHandler(this, &OperatingForm::viewSelection_Click);
		// 
		// angleText
		// 
		this->angleText->BackColor = System::Drawing::Color::Transparent;
		this->angleText->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->angleText->Location = System::Drawing::Point(10, 80);
		this->angleText->Margin = System::Windows::Forms::Padding(0);
		this->angleText->Name = L"angleText";
		this->angleText->Size = System::Drawing::Size(167, 74);
		this->angleText->TabIndex = 2;
		this->angleText->Text = L"-180 °";
		this->angleText->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// selectedIcon
		// 
		this->selectedIcon->BackColor = System::Drawing::Color::Transparent;
		this->selectedIcon->Location = System::Drawing::Point(180, 17);
		this->selectedIcon->Name = L"selectedIcon";
		this->selectedIcon->Size = System::Drawing::Size(230, 200);
		this->selectedIcon->TabIndex = 0;
		this->selectedIcon->Click += gcnew System::EventHandler(this, &OperatingForm::viewSelection_Click);
		// 
		// forceStatus
		// 
		this->forceStatus->Controls->Add(this->labelForce);
		this->forceStatus->Location = System::Drawing::Point(406, 454);
		this->forceStatus->Name = L"forceStatus";
		this->forceStatus->Size = System::Drawing::Size(170, 170);
		this->forceStatus->TabIndex = 19;
		// 
		// labelForce
		// 
		this->labelForce->BackColor = System::Drawing::Color::Transparent;
		this->labelForce->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelForce->Location = System::Drawing::Point(5, 135);
		this->labelForce->Name = L"labelForce";
		this->labelForce->Size = System::Drawing::Size(160, 30);
		this->labelForce->TabIndex = 2;
		this->labelForce->Text = L"15.0 (Kg)";
		this->labelForce->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// thicknessStatus
		// 
		this->thicknessStatus->Controls->Add(this->labelThickness);
		this->thicknessStatus->Location = System::Drawing::Point(214, 454);
		this->thicknessStatus->Name = L"thicknessStatus";
		this->thicknessStatus->Size = System::Drawing::Size(170, 170);
		this->thicknessStatus->TabIndex = 18;
		// 
		// labelThickness
		// 
		this->labelThickness->BackColor = System::Drawing::Color::Transparent;
		this->labelThickness->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelThickness->Location = System::Drawing::Point(5, 135);
		this->labelThickness->Name = L"labelThickness";
		this->labelThickness->Size = System::Drawing::Size(160, 30);
		this->labelThickness->TabIndex = 2;
		this->labelThickness->Text = L"150 (mm)";
		this->labelThickness->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// paddleStatus
		// 
		this->paddleStatus->Controls->Add(this->labelPaddle);
		this->paddleStatus->Location = System::Drawing::Point(22, 454);
		this->paddleStatus->Name = L"paddleStatus";
		this->paddleStatus->Size = System::Drawing::Size(170, 170);
		this->paddleStatus->TabIndex = 17;
		// 
		// labelPaddle
		// 
		this->labelPaddle->BackColor = System::Drawing::Color::Transparent;
		this->labelPaddle->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelPaddle->Location = System::Drawing::Point(5, 135);
		this->labelPaddle->Name = L"labelPaddle";
		this->labelPaddle->Size = System::Drawing::Size(160, 30);
		this->labelPaddle->TabIndex = 1;
		this->labelPaddle->Text = L"PAD 24x30";
		this->labelPaddle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// doorStatus
		// 
		this->doorStatus->Location = System::Drawing::Point(406, 644);
		this->doorStatus->Name = L"doorStatus";
		this->doorStatus->Size = System::Drawing::Size(170, 170);
		this->doorStatus->TabIndex = 16;
		// 
		// tubeStatus
		// 
		this->tubeStatus->Controls->Add(this->labelTubeData);
		this->tubeStatus->Location = System::Drawing::Point(214, 644);
		this->tubeStatus->Name = L"tubeStatus";
		this->tubeStatus->Size = System::Drawing::Size(170, 170);
		this->tubeStatus->TabIndex = 15;
		// 
		// labelTubeData
		// 
		this->labelTubeData->BackColor = System::Drawing::Color::Transparent;
		this->labelTubeData->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelTubeData->Location = System::Drawing::Point(68, 136);
		this->labelTubeData->Name = L"labelTubeData";
		this->labelTubeData->Size = System::Drawing::Size(99, 30);
		this->labelTubeData->TabIndex = 2;
		this->labelTubeData->Text = L"100%";
		this->labelTubeData->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// collimationStatus
		// 
		this->collimationStatus->Controls->Add(this->labelColli);
		this->collimationStatus->Location = System::Drawing::Point(22, 644);
		this->collimationStatus->Name = L"collimationStatus";
		this->collimationStatus->Size = System::Drawing::Size(170, 170);
		this->collimationStatus->TabIndex = 14;
		// 
		// labelColli
		// 
		this->labelColli->BackColor = System::Drawing::Color::Transparent;
		this->labelColli->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelColli->Location = System::Drawing::Point(5, 135);
		this->labelColli->Name = L"labelColli";
		this->labelColli->Size = System::Drawing::Size(160, 30);
		this->labelColli->TabIndex = 2;
		this->labelColli->Text = L"AUTO";
		this->labelColli->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// residualExposures
		// 
		this->residualExposures->Location = System::Drawing::Point(406, 834);
		this->residualExposures->Name = L"residualExposures";
		this->residualExposures->Size = System::Drawing::Size(170, 170);
		this->residualExposures->TabIndex = 15;
		// 
		// decompressionStatus
		// 
		this->decompressionStatus->Location = System::Drawing::Point(214, 834);
		this->decompressionStatus->Name = L"decompressionStatus";
		this->decompressionStatus->Size = System::Drawing::Size(170, 170);
		this->decompressionStatus->TabIndex = 14;
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
		SendNotifyMessageA(window, WM_USER + 1, 0, 0);
	}


	private: System::Void lampButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void errorButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void viewSelection_Click(System::Object^ sender, System::EventArgs^ e);

};

