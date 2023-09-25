#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class IdleForm :  public System::Windows::Forms::Form
{
	

public:
	public:System::Timers::Timer^ idleTimer;
public:System::Timers::Timer^ errorPanelTimer;
private: System::Windows::Forms::Label^ labelDate;
private: System::Windows::Forms::Label^ labelInstallation;
private: System::Windows::Forms::Label^ labelTime;
private: System::Windows::Forms::Label^ labelTubeData;
private: System::Windows::Forms::Panel^ tubeTempOk;
private: System::Windows::Forms::Panel^ errorPanel;
private: System::Windows::Forms::Label^ errorId;

private: System::Windows::Forms::Label^ errorTitle;
private: System::Windows::Forms::PictureBox^ errpanel_erricon;


private: System::Windows::Forms::RichTextBox^ errorContent;







private: System::Windows::Forms::Button^ buttonCanc;
private: System::Windows::Forms::Panel^ errorButton;
private: System::Windows::Forms::Panel^ mainPanel;
private: System::Windows::Forms::Panel^ panel1;
private: System::Windows::Forms::Panel^ panel2;
private: System::Windows::Forms::Label^ errorPanelTitle;



















public:

public:
public:HWND window;

	void formInitialization(void);
	void initIdleStatus(void);
	void idleStatusManagement(void);
	void openErrorWindow(bool status);

	IdleForm(void)
	{
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		formInitialization();

	}

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~IdleForm()
	{
		if (components)
		{
			delete components;
		}
	}
private: System::Windows::Forms::PictureBox^ xrayMode;
protected:

private: System::Windows::Forms::PictureBox^ batteryConnected;
private: System::Windows::Forms::PictureBox^ awsConnected;
private: System::Windows::Forms::PictureBox^ peripheralsConnected;
private: System::Windows::Forms::PictureBox^ doorClosed;




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
		this->xrayMode = (gcnew System::Windows::Forms::PictureBox());
		this->batteryConnected = (gcnew System::Windows::Forms::PictureBox());
		this->awsConnected = (gcnew System::Windows::Forms::PictureBox());
		this->peripheralsConnected = (gcnew System::Windows::Forms::PictureBox());
		this->doorClosed = (gcnew System::Windows::Forms::PictureBox());
		this->labelDate = (gcnew System::Windows::Forms::Label());
		this->labelInstallation = (gcnew System::Windows::Forms::Label());
		this->labelTime = (gcnew System::Windows::Forms::Label());
		this->labelTubeData = (gcnew System::Windows::Forms::Label());
		this->tubeTempOk = (gcnew System::Windows::Forms::Panel());
		this->errorPanel = (gcnew System::Windows::Forms::Panel());
		this->panel2 = (gcnew System::Windows::Forms::Panel());
		this->errorPanelTitle = (gcnew System::Windows::Forms::Label());
		this->panel1 = (gcnew System::Windows::Forms::Panel());
		this->buttonCanc = (gcnew System::Windows::Forms::Button());
		this->errorContent = (gcnew System::Windows::Forms::RichTextBox());
		this->errpanel_erricon = (gcnew System::Windows::Forms::PictureBox());
		this->errorTitle = (gcnew System::Windows::Forms::Label());
		this->errorId = (gcnew System::Windows::Forms::Label());
		this->errorButton = (gcnew System::Windows::Forms::Panel());
		this->mainPanel = (gcnew System::Windows::Forms::Panel());
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->xrayMode))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->batteryConnected))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->awsConnected))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->peripheralsConnected))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->doorClosed))->BeginInit();
		this->tubeTempOk->SuspendLayout();
		this->errorPanel->SuspendLayout();
		this->panel2->SuspendLayout();
		this->panel1->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->errpanel_erricon))->BeginInit();
		this->mainPanel->SuspendLayout();
		this->SuspendLayout();
		// 
		// xrayMode
		// 
		this->xrayMode->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->xrayMode->Location = System::Drawing::Point(30, 337);
		this->xrayMode->Name = L"xrayMode";
		this->xrayMode->Size = System::Drawing::Size(160, 160);
		this->xrayMode->TabIndex = 1;
		this->xrayMode->TabStop = false;
		// 
		// batteryConnected
		// 
		this->batteryConnected->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->batteryConnected->Location = System::Drawing::Point(220, 337);
		this->batteryConnected->Name = L"batteryConnected";
		this->batteryConnected->Size = System::Drawing::Size(160, 160);
		this->batteryConnected->TabIndex = 2;
		this->batteryConnected->TabStop = false;
		// 
		// awsConnected
		// 
		this->awsConnected->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->awsConnected->Location = System::Drawing::Point(410, 337);
		this->awsConnected->Name = L"awsConnected";
		this->awsConnected->Size = System::Drawing::Size(160, 160);
		this->awsConnected->TabIndex = 3;
		this->awsConnected->TabStop = false;
		// 
		// peripheralsConnected
		// 
		this->peripheralsConnected->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->peripheralsConnected->Location = System::Drawing::Point(30, 527);
		this->peripheralsConnected->Name = L"peripheralsConnected";
		this->peripheralsConnected->Size = System::Drawing::Size(160, 160);
		this->peripheralsConnected->TabIndex = 4;
		this->peripheralsConnected->TabStop = false;
		// 
		// doorClosed
		// 
		this->doorClosed->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->doorClosed->Location = System::Drawing::Point(220, 527);
		this->doorClosed->Name = L"doorClosed";
		this->doorClosed->Size = System::Drawing::Size(160, 160);
		this->doorClosed->TabIndex = 5;
		this->doorClosed->TabStop = false;
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
		// labelInstallation
		// 
		this->labelInstallation->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular,
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->labelInstallation->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)), static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->labelInstallation->Location = System::Drawing::Point(143, 13);
		this->labelInstallation->Name = L"labelInstallation";
		this->labelInstallation->Size = System::Drawing::Size(305, 20);
		this->labelInstallation->TabIndex = 8;
		this->labelInstallation->Text = L"IFO SAN GALLICANO";
		this->labelInstallation->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
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
		// labelTubeData
		// 
		this->labelTubeData->AutoSize = true;
		this->labelTubeData->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelTubeData->Location = System::Drawing::Point(87, 126);
		this->labelTubeData->Name = L"labelTubeData";
		this->labelTubeData->Size = System::Drawing::Size(70, 25);
		this->labelTubeData->TabIndex = 10;
		this->labelTubeData->Text = L"label1";
		// 
		// tubeTempOk
		// 
		this->tubeTempOk->Controls->Add(this->labelTubeData);
		this->tubeTempOk->Location = System::Drawing::Point(410, 527);
		this->tubeTempOk->Name = L"tubeTempOk";
		this->tubeTempOk->Size = System::Drawing::Size(160, 160);
		this->tubeTempOk->TabIndex = 11;
		// 
		// errorPanel
		// 
		this->errorPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)), static_cast<System::Int32>(static_cast<System::Byte>(207)),
			static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->errorPanel->Controls->Add(this->panel2);
		this->errorPanel->Controls->Add(this->panel1);
		this->errorPanel->Controls->Add(this->errorContent);
		this->errorPanel->Controls->Add(this->errpanel_erricon);
		this->errorPanel->Controls->Add(this->errorTitle);
		this->errorPanel->Controls->Add(this->errorId);
		this->errorPanel->Location = System::Drawing::Point(0, 0);
		this->errorPanel->Name = L"errorPanel";
		this->errorPanel->Size = System::Drawing::Size(600, 1024);
		this->errorPanel->TabIndex = 12;
		// 
		// panel2
		// 
		this->panel2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(174)),
			static_cast<System::Int32>(static_cast<System::Byte>(191)));
		this->panel2->Controls->Add(this->errorPanelTitle);
		this->panel2->Location = System::Drawing::Point(0, 0);
		this->panel2->Name = L"panel2";
		this->panel2->Size = System::Drawing::Size(600, 90);
		this->panel2->TabIndex = 17;
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
		this->buttonCanc->Click += gcnew System::EventHandler(this, &IdleForm::buttonCanc_Click);
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
		// errorButton
		// 
		this->errorButton->Location = System::Drawing::Point(30, 834);
		this->errorButton->Name = L"errorButton";
		this->errorButton->Size = System::Drawing::Size(160, 160);
		this->errorButton->TabIndex = 13;
		this->errorButton->Click += gcnew System::EventHandler(this, &IdleForm::errorButton_Click);
		// 
		// mainPanel
		// 
		this->mainPanel->Controls->Add(this->labelDate);
		this->mainPanel->Controls->Add(this->errorButton);
		this->mainPanel->Controls->Add(this->labelInstallation);
		this->mainPanel->Controls->Add(this->labelTime);
		this->mainPanel->Controls->Add(this->tubeTempOk);
		this->mainPanel->Controls->Add(this->xrayMode);
		this->mainPanel->Controls->Add(this->doorClosed);
		this->mainPanel->Controls->Add(this->batteryConnected);
		this->mainPanel->Controls->Add(this->peripheralsConnected);
		this->mainPanel->Controls->Add(this->awsConnected);
		this->mainPanel->Location = System::Drawing::Point(1000, 0);
		this->mainPanel->Name = L"mainPanel";
		this->mainPanel->Size = System::Drawing::Size(600, 1024);
		this->mainPanel->TabIndex = 14;
		// 
		// IdleForm
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
		this->Controls->Add(this->mainPanel);
		this->Controls->Add(this->errorPanel);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Name = L"IdleForm";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->xrayMode))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->batteryConnected))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->awsConnected))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->peripheralsConnected))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->doorClosed))->EndInit();
		this->tubeTempOk->ResumeLayout(false);
		this->tubeTempOk->PerformLayout();
		this->errorPanel->ResumeLayout(false);
		this->errorPanel->PerformLayout();
		this->panel2->ResumeLayout(false);
		this->panel2->PerformLayout();
		this->panel1->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->errpanel_erricon))->EndInit();
		this->mainPanel->ResumeLayout(false);
		this->mainPanel->PerformLayout();
		this->ResumeLayout(false);

	}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	private: System::Void onIdleTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
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

