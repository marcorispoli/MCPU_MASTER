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

	void formInitialization(void);

	IdleForm(void)
	{
		InitializeComponent();
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
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->xrayMode))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->batteryConnected))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->awsConnected))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->peripheralsConnected))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->doorClosed))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tubeTempOk))->BeginInit();
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
		// 
		// IdleForm
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
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
		this->ResumeLayout(false);

	}
#pragma endregion


};

