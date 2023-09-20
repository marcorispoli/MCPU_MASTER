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
private: System::Windows::Forms::Label^ labelDate;
private: System::Windows::Forms::Label^ labelInstallation;
private: System::Windows::Forms::Label^ labelTime;
private: System::Windows::Forms::Label^ infoId;

private: System::Windows::Forms::Panel^ infoTubeData;
private: System::Windows::Forms::Label^ statorLabel;
private: System::Windows::Forms::Label^ statorTemperature;
private: System::Windows::Forms::Label^ anodeHu;
private: System::Windows::Forms::Label^ labelAnode;
private: System::Windows::Forms::Label^ bulbTemperature;
private: System::Windows::Forms::Label^ labelBulb;







public:

public:
public:HWND window;

	void formInitialization(void);
	void initIdleStatus(void);
	void idleStatusManagement(void);

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
private: System::Windows::Forms::PictureBox^ tubeTempOk;


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
		this->tubeTempOk = (gcnew System::Windows::Forms::PictureBox());
		this->labelDate = (gcnew System::Windows::Forms::Label());
		this->labelInstallation = (gcnew System::Windows::Forms::Label());
		this->labelTime = (gcnew System::Windows::Forms::Label());
		this->infoId = (gcnew System::Windows::Forms::Label());
		this->infoTubeData = (gcnew System::Windows::Forms::Panel());
		this->anodeHu = (gcnew System::Windows::Forms::Label());
		this->labelAnode = (gcnew System::Windows::Forms::Label());
		this->bulbTemperature = (gcnew System::Windows::Forms::Label());
		this->labelBulb = (gcnew System::Windows::Forms::Label());
		this->statorTemperature = (gcnew System::Windows::Forms::Label());
		this->statorLabel = (gcnew System::Windows::Forms::Label());
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->xrayMode))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->batteryConnected))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->awsConnected))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->peripheralsConnected))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->doorClosed))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tubeTempOk))->BeginInit();
		this->infoTubeData->SuspendLayout();
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
		// tubeTempOk
		// 
		this->tubeTempOk->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->tubeTempOk->Location = System::Drawing::Point(410, 527);
		this->tubeTempOk->Name = L"tubeTempOk";
		this->tubeTempOk->Size = System::Drawing::Size(160, 160);
		this->tubeTempOk->TabIndex = 6;
		this->tubeTempOk->TabStop = false;
		this->tubeTempOk->Click += gcnew System::EventHandler(this, &IdleForm::tubeTempOk_Click);
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
		// infoId
		// 
		this->infoId->AutoSize = true;
		this->infoId->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->infoId->Location = System::Drawing::Point(119, 37);
		this->infoId->Name = L"infoId";
		this->infoId->Size = System::Drawing::Size(70, 24);
		this->infoId->TabIndex = 0;
		this->infoId->Text = L"Info-ID";
		// 
		// infoTubeData
		// 
		this->infoTubeData->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->infoTubeData->Controls->Add(this->anodeHu);
		this->infoTubeData->Controls->Add(this->labelAnode);
		this->infoTubeData->Controls->Add(this->bulbTemperature);
		this->infoTubeData->Controls->Add(this->labelBulb);
		this->infoTubeData->Controls->Add(this->statorTemperature);
		this->infoTubeData->Controls->Add(this->statorLabel);
		this->infoTubeData->Controls->Add(this->infoId);
		this->infoTubeData->Location = System::Drawing::Point(12, 317);
		this->infoTubeData->Name = L"infoTubeData";
		this->infoTubeData->Size = System::Drawing::Size(512, 204);
		this->infoTubeData->TabIndex = 12;
		this->infoTubeData->Click += gcnew System::EventHandler(this, &IdleForm::infoTubeData_Click);
		// 
		// anodeHu
		// 
		this->anodeHu->AutoSize = true;
		this->anodeHu->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->anodeHu->Location = System::Drawing::Point(374, 190);
		this->anodeHu->Name = L"anodeHu";
		this->anodeHu->Size = System::Drawing::Size(48, 24);
		this->anodeHu->TabIndex = 6;
		this->anodeHu->Text = L"20%";
		// 
		// labelAnode
		// 
		this->labelAnode->AutoSize = true;
		this->labelAnode->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelAnode->Location = System::Drawing::Point(57, 190);
		this->labelAnode->Name = L"labelAnode";
		this->labelAnode->Size = System::Drawing::Size(243, 24);
		this->labelAnode->TabIndex = 5;
		this->labelAnode->Text = L"ANODE TEMPERATURE";
		// 
		// bulbTemperature
		// 
		this->bulbTemperature->AutoSize = true;
		this->bulbTemperature->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->bulbTemperature->Location = System::Drawing::Point(374, 160);
		this->bulbTemperature->Name = L"bulbTemperature";
		this->bulbTemperature->Size = System::Drawing::Size(48, 24);
		this->bulbTemperature->TabIndex = 4;
		this->bulbTemperature->Text = L"20%";
		// 
		// labelBulb
		// 
		this->labelBulb->AutoSize = true;
		this->labelBulb->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelBulb->Location = System::Drawing::Point(57, 160);
		this->labelBulb->Name = L"labelBulb";
		this->labelBulb->Size = System::Drawing::Size(221, 24);
		this->labelBulb->TabIndex = 3;
		this->labelBulb->Text = L"BULB TEMPERATURE";
		// 
		// statorTemperature
		// 
		this->statorTemperature->AutoSize = true;
		this->statorTemperature->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->statorTemperature->Location = System::Drawing::Point(374, 130);
		this->statorTemperature->Name = L"statorTemperature";
		this->statorTemperature->Size = System::Drawing::Size(48, 24);
		this->statorTemperature->TabIndex = 2;
		this->statorTemperature->Text = L"20%";
		// 
		// statorLabel
		// 
		this->statorLabel->AutoSize = true;
		this->statorLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->statorLabel->Location = System::Drawing::Point(57, 130);
		this->statorLabel->Name = L"statorLabel";
		this->statorLabel->Size = System::Drawing::Size(253, 24);
		this->statorLabel->TabIndex = 1;
		this->statorLabel->Text = L"STATOR TEMPERATURE";
		// 
		// IdleForm
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
		this->Controls->Add(this->infoTubeData);
		this->Controls->Add(this->labelTime);
		this->Controls->Add(this->labelInstallation);
		this->Controls->Add(this->labelDate);
		this->Controls->Add(this->tubeTempOk);
		this->Controls->Add(this->doorClosed);
		this->Controls->Add(this->peripheralsConnected);
		this->Controls->Add(this->awsConnected);
		this->Controls->Add(this->batteryConnected);
		this->Controls->Add(this->xrayMode);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Name = L"IdleForm";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->xrayMode))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->batteryConnected))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->awsConnected))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->peripheralsConnected))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->doorClosed))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tubeTempOk))->EndInit();
		this->infoTubeData->ResumeLayout(false);
		this->infoTubeData->PerformLayout();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	private: System::Void onIdleTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		SendNotifyMessageA(window, WM_USER + 1, 0, 0);
	}



private: System::Void infoTubeData_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void tubeTempOk_Click(System::Object^ sender, System::EventArgs^ e);
};

