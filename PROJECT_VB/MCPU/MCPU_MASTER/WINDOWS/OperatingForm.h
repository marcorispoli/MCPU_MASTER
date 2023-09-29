#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class OperatingForm :  public System::Windows::Forms::Form
{
	

public:
	public:System::Timers::Timer^ operatingTimer;
	public:System::Timers::Timer^ errorPanelTimer;
private: System::Windows::Forms::Label^ labelDate;
private: System::Windows::Forms::Label^ labelPatientName;

private: System::Windows::Forms::Label^ labelTime;


private: System::Windows::Forms::Panel^ errorPanel;
private: System::Windows::Forms::Label^ errorId;

private: System::Windows::Forms::Label^ errorTitle;
private: System::Windows::Forms::PictureBox^ errpanel_erricon;


private: System::Windows::Forms::RichTextBox^ errorContent;







private: System::Windows::Forms::Button^ buttonCanc;
private: System::Windows::Forms::Panel^ magnifierStatus;

private: System::Windows::Forms::Panel^ mainPanel;
private: System::Windows::Forms::Panel^ panel1;
private: System::Windows::Forms::Panel^ operTopFrame;



private: System::Windows::Forms::Label^ errorPanelTitle;
private: System::Windows::Forms::Panel^ residualExposures;

private: System::Windows::Forms::Panel^ decompressionStatus;

private: System::Windows::Forms::Panel^ xrayStat;
private: System::Windows::Forms::Panel^ alarmButton;
private: System::Windows::Forms::Panel^ lampButton;




private: System::Windows::Forms::Panel^ viewSelection;
private: System::Windows::Forms::Panel^ forceStatus;





private: System::Windows::Forms::Panel^ thicknessStatus;

private: System::Windows::Forms::Panel^ paddleStatus;
private: System::Windows::Forms::Panel^ doorStatus;


private: System::Windows::Forms::Panel^ tubeStatus;

private: System::Windows::Forms::Panel^ collimationStatus;
private: System::Windows::Forms::Label^ labelXrayStatus;





















public:

public:
public:HWND window;

	void formInitialization(void);
	void initOperatingStatus(void);
	void operatingStatusManagement(void);
	void openErrorWindow(bool status);

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
		this->errorPanel = (gcnew System::Windows::Forms::Panel());
		this->operTopFrame = (gcnew System::Windows::Forms::Panel());
		this->errorPanelTitle = (gcnew System::Windows::Forms::Label());
		this->panel1 = (gcnew System::Windows::Forms::Panel());
		this->buttonCanc = (gcnew System::Windows::Forms::Button());
		this->errorContent = (gcnew System::Windows::Forms::RichTextBox());
		this->errpanel_erricon = (gcnew System::Windows::Forms::PictureBox());
		this->errorTitle = (gcnew System::Windows::Forms::Label());
		this->errorId = (gcnew System::Windows::Forms::Label());
		this->magnifierStatus = (gcnew System::Windows::Forms::Panel());
		this->mainPanel = (gcnew System::Windows::Forms::Panel());
		this->decompressionStatus = (gcnew System::Windows::Forms::Panel());
		this->residualExposures = (gcnew System::Windows::Forms::Panel());
		this->collimationStatus = (gcnew System::Windows::Forms::Panel());
		this->tubeStatus = (gcnew System::Windows::Forms::Panel());
		this->doorStatus = (gcnew System::Windows::Forms::Panel());
		this->paddleStatus = (gcnew System::Windows::Forms::Panel());
		this->thicknessStatus = (gcnew System::Windows::Forms::Panel());
		this->forceStatus = (gcnew System::Windows::Forms::Panel());
		this->viewSelection = (gcnew System::Windows::Forms::Panel());
		this->lampButton = (gcnew System::Windows::Forms::Panel());
		this->alarmButton = (gcnew System::Windows::Forms::Panel());
		this->xrayStat = (gcnew System::Windows::Forms::Panel());
		this->labelXrayStatus = (gcnew System::Windows::Forms::Label());
		this->errorPanel->SuspendLayout();
		this->operTopFrame->SuspendLayout();
		this->panel1->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->errpanel_erricon))->BeginInit();
		this->mainPanel->SuspendLayout();
		this->xrayStat->SuspendLayout();
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
		// errorPanel
		// 
		this->errorPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)), static_cast<System::Int32>(static_cast<System::Byte>(207)),
			static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->errorPanel->Controls->Add(this->operTopFrame);
		this->errorPanel->Controls->Add(this->panel1);
		this->errorPanel->Controls->Add(this->errorContent);
		this->errorPanel->Controls->Add(this->errpanel_erricon);
		this->errorPanel->Controls->Add(this->errorTitle);
		this->errorPanel->Controls->Add(this->errorId);
		this->errorPanel->Location = System::Drawing::Point(1000, 0);
		this->errorPanel->Name = L"errorPanel";
		this->errorPanel->Size = System::Drawing::Size(600, 1024);
		this->errorPanel->TabIndex = 12;
		// 
		// operTopFrame
		// 
		this->operTopFrame->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(174)),
			static_cast<System::Int32>(static_cast<System::Byte>(191)));
		this->operTopFrame->Controls->Add(this->errorPanelTitle);
		this->operTopFrame->Location = System::Drawing::Point(0, 0);
		this->operTopFrame->Name = L"operTopFrame";
		this->operTopFrame->Size = System::Drawing::Size(600, 90);
		this->operTopFrame->TabIndex = 17;
		// 
		// errorPanelTitle
		// 
		this->errorPanelTitle->AutoSize = true;
		this->errorPanelTitle->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->errorPanelTitle->Location = System::Drawing::Point(36, 25);
		this->errorPanelTitle->Name = L"errorPanelTitle";
		this->errorPanelTitle->Size = System::Drawing::Size(275, 42);
		this->errorPanelTitle->TabIndex = 16;
		this->errorPanelTitle->Text = L"Error Windows";
		// 
		// panel1
		// 
		this->panel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(174)),
			static_cast<System::Int32>(static_cast<System::Byte>(191)));
		this->panel1->Controls->Add(this->buttonCanc);
		this->panel1->Location = System::Drawing::Point(0, 934);
		this->panel1->Name = L"panel1";
		this->panel1->Size = System::Drawing::Size(600, 90);
		this->panel1->TabIndex = 16;
		// 
		// buttonCanc
		// 
		this->buttonCanc->BackColor = System::Drawing::Color::Transparent;
		this->buttonCanc->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->buttonCanc->FlatAppearance->BorderColor = System::Drawing::Color::Black;
		this->buttonCanc->FlatAppearance->BorderSize = 0;
		this->buttonCanc->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
		this->buttonCanc->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->buttonCanc->Location = System::Drawing::Point(518, 10);
		this->buttonCanc->Name = L"buttonCanc";
		this->buttonCanc->Size = System::Drawing::Size(70, 70);
		this->buttonCanc->TabIndex = 15;
		this->buttonCanc->UseVisualStyleBackColor = false;
		this->buttonCanc->Click += gcnew System::EventHandler(this, &OperatingForm::buttonCanc_Click);
		// 
		// errorContent
		// 
		this->errorContent->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)), static_cast<System::Int32>(static_cast<System::Byte>(207)),
			static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->errorContent->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		this->errorContent->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->errorContent->Location = System::Drawing::Point(40, 337);
		this->errorContent->Name = L"errorContent";
		this->errorContent->Size = System::Drawing::Size(520, 550);
		this->errorContent->TabIndex = 14;
		this->errorContent->Text = L"";
		// 
		// errpanel_erricon
		// 
		this->errpanel_erricon->Location = System::Drawing::Point(40, 137);
		this->errpanel_erricon->Name = L"errpanel_erricon";
		this->errpanel_erricon->Size = System::Drawing::Size(160, 145);
		this->errpanel_erricon->TabIndex = 13;
		this->errpanel_erricon->TabStop = false;
		// 
		// errorTitle
		// 
		this->errorTitle->AutoSize = true;
		this->errorTitle->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->errorTitle->Location = System::Drawing::Point(249, 226);
		this->errorTitle->Name = L"errorTitle";
		this->errorTitle->Size = System::Drawing::Size(256, 29);
		this->errorTitle->TabIndex = 10;
		this->errorTitle->Text = L"Xray Push Button Error";
		this->errorTitle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// errorId
		// 
		this->errorId->AutoSize = true;
		this->errorId->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->errorId->Location = System::Drawing::Point(247, 169);
		this->errorId->Name = L"errorId";
		this->errorId->Size = System::Drawing::Size(154, 42);
		this->errorId->TabIndex = 12;
		this->errorId->Text = L"E00001";
		// 
		// magnifierStatus
		// 
		this->magnifierStatus->Location = System::Drawing::Point(22, 834);
		this->magnifierStatus->Name = L"magnifierStatus";
		this->magnifierStatus->Size = System::Drawing::Size(170, 170);
		this->magnifierStatus->TabIndex = 13;
		this->magnifierStatus->Click += gcnew System::EventHandler(this, &OperatingForm::errorButton_Click);
		// 
		// mainPanel
		// 
		this->mainPanel->Controls->Add(this->xrayStat);
		this->mainPanel->Controls->Add(this->alarmButton);
		this->mainPanel->Controls->Add(this->lampButton);
		this->mainPanel->Controls->Add(this->viewSelection);
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
		// decompressionStatus
		// 
		this->decompressionStatus->Location = System::Drawing::Point(214, 834);
		this->decompressionStatus->Name = L"decompressionStatus";
		this->decompressionStatus->Size = System::Drawing::Size(170, 170);
		this->decompressionStatus->TabIndex = 14;
		// 
		// residualExposures
		// 
		this->residualExposures->Location = System::Drawing::Point(406, 834);
		this->residualExposures->Name = L"residualExposures";
		this->residualExposures->Size = System::Drawing::Size(170, 170);
		this->residualExposures->TabIndex = 15;
		// 
		// collimationStatus
		// 
		this->collimationStatus->Location = System::Drawing::Point(22, 644);
		this->collimationStatus->Name = L"collimationStatus";
		this->collimationStatus->Size = System::Drawing::Size(170, 170);
		this->collimationStatus->TabIndex = 14;
		// 
		// tubeStatus
		// 
		this->tubeStatus->Location = System::Drawing::Point(214, 644);
		this->tubeStatus->Name = L"tubeStatus";
		this->tubeStatus->Size = System::Drawing::Size(170, 170);
		this->tubeStatus->TabIndex = 15;
		// 
		// doorStatus
		// 
		this->doorStatus->Location = System::Drawing::Point(406, 644);
		this->doorStatus->Name = L"doorStatus";
		this->doorStatus->Size = System::Drawing::Size(170, 170);
		this->doorStatus->TabIndex = 16;
		// 
		// paddleStatus
		// 
		this->paddleStatus->Location = System::Drawing::Point(22, 454);
		this->paddleStatus->Name = L"paddleStatus";
		this->paddleStatus->Size = System::Drawing::Size(170, 170);
		this->paddleStatus->TabIndex = 17;
		// 
		// thicknessStatus
		// 
		this->thicknessStatus->Location = System::Drawing::Point(214, 454);
		this->thicknessStatus->Name = L"thicknessStatus";
		this->thicknessStatus->Size = System::Drawing::Size(170, 170);
		this->thicknessStatus->TabIndex = 18;
		// 
		// forceStatus
		// 
		this->forceStatus->Location = System::Drawing::Point(406, 454);
		this->forceStatus->Name = L"forceStatus";
		this->forceStatus->Size = System::Drawing::Size(170, 170);
		this->forceStatus->TabIndex = 19;
		// 
		// viewSelection
		// 
		this->viewSelection->Location = System::Drawing::Point(22, 200);
		this->viewSelection->Name = L"viewSelection";
		this->viewSelection->Size = System::Drawing::Size(430, 234);
		this->viewSelection->TabIndex = 20;
		// 
		// lampButton
		// 
		this->lampButton->Location = System::Drawing::Point(471, 200);
		this->lampButton->Name = L"lampButton";
		this->lampButton->Size = System::Drawing::Size(105, 105);
		this->lampButton->TabIndex = 21;
		// 
		// alarmButton
		// 
		this->alarmButton->Location = System::Drawing::Point(471, 329);
		this->alarmButton->Name = L"alarmButton";
		this->alarmButton->Size = System::Drawing::Size(105, 105);
		this->alarmButton->TabIndex = 22;
		// 
		// xrayStat
		// 
		this->xrayStat->Controls->Add(this->labelXrayStatus);
		this->xrayStat->Location = System::Drawing::Point(22, 61);
		this->xrayStat->Name = L"xrayStat";
		this->xrayStat->Size = System::Drawing::Size(554, 120);
		this->xrayStat->TabIndex = 23;
		// 
		// labelXrayStatus
		// 
		this->labelXrayStatus->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 26.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelXrayStatus->Location = System::Drawing::Point(19, 21);
		this->labelXrayStatus->Name = L"labelXrayStatus";
		this->labelXrayStatus->Size = System::Drawing::Size(514, 82);
		this->labelXrayStatus->TabIndex = 0;
		this->labelXrayStatus->Text = L"STAND-BY";
		this->labelXrayStatus->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// OperatingForm
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
		this->Controls->Add(this->mainPanel);
		this->Controls->Add(this->errorPanel);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Name = L"OperatingForm";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		this->errorPanel->ResumeLayout(false);
		this->errorPanel->PerformLayout();
		this->operTopFrame->ResumeLayout(false);
		this->operTopFrame->PerformLayout();
		this->panel1->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->errpanel_erricon))->EndInit();
		this->mainPanel->ResumeLayout(false);
		this->mainPanel->PerformLayout();
		this->xrayStat->ResumeLayout(false);
		this->ResumeLayout(false);

	}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	private: System::Void onOperatingTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		SendNotifyMessageA(window, WM_USER + 1, 0, 0);
	}

	private: System::Void onErrorTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		SendNotifyMessageA(window, WM_USER + 2, 0, 0);
	}



private: System::Void errorButton_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void buttonCanc_Click(System::Object^ sender, System::EventArgs^ e);

};

