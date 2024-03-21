#pragma once
#include <Windows.h>

/// <summary>
/// \defgroup ServiceGUI_Module Service Window Management Module
/// \ingroup GUI_Module Gantry GUI modules
/// This module implements the features of the Gantry Service Status
/// 
/// # Features
/// 
/// </summary>
/// 

using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class ServiceZeroSettingTool : public System::Windows::Forms::Form
{
#define PANEL_WIDTH 600
#define PANEL_HIGH 880
#define PANEL_X 0
#define PANEL_Y 50

// Handles the Window status
public:	
	void dialog_open(Form^ p);
	void close(void);
	bool open_status;

	Form^ parent;
	HWND window;
	
	void initPanel(void);
	void timerManagement(void);
	static ServiceZeroSettingTool^ panel = gcnew ServiceZeroSettingTool(); // Pointer to the Form instance from the static member
	System::Timers::Timer^ serviceTimer;

	ServiceZeroSettingTool(void)
	{
		panel = this;
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		formInitialization();
	}

	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~ServiceZeroSettingTool()
	{
		if (components)
		{
			delete components;
		}
	}

// Common items for the service dialog
private:void formInitialization(void);
private: System::Windows::Forms::Label^ serviceMenuTitle;
private: System::Windows::Forms::Label^ labelDate;
private: System::Windows::Forms::Label^ labelInstallation;
private: System::Windows::Forms::Label^ labelTime;
private: System::Windows::Forms::PictureBox^ serviceCanc;



private: System::Windows::Forms::Panel^ calibZerosettingPanel;
private: System::Windows::Forms::PictureBox^ zeroSettingSlide;
private: System::Windows::Forms::PictureBox^ zeroSettingTilt;
private: System::Windows::Forms::PictureBox^ zeroSettingArm;
private: System::Windows::Forms::PictureBox^ zeroSettingVertical;
private: System::Windows::Forms::PictureBox^ zeroSettingBody;
private: System::Windows::Forms::PictureBox^ zeroSettingAll;
private: System::Windows::Forms::RichTextBox^ zeroSettingLog;


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
		this->labelInstallation = (gcnew System::Windows::Forms::Label());
		this->labelTime = (gcnew System::Windows::Forms::Label());
		this->calibZerosettingPanel = (gcnew System::Windows::Forms::Panel());
		this->zeroSettingLog = (gcnew System::Windows::Forms::RichTextBox());
		this->zeroSettingSlide = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingTilt = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingArm = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingVertical = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingBody = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingAll = (gcnew System::Windows::Forms::PictureBox());
		this->serviceCanc = (gcnew System::Windows::Forms::PictureBox());
		this->serviceMenuTitle = (gcnew System::Windows::Forms::Label());
		this->calibZerosettingPanel->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingSlide))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingTilt))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingArm))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingVertical))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingBody))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingAll))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceCanc))->BeginInit();
		this->SuspendLayout();
		// 
		// labelDate
		// 
		this->labelDate->AutoSize = true;
		this->labelDate->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelDate->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->labelDate->Location = System::Drawing::Point(5, 984);
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
		this->labelInstallation->Location = System::Drawing::Point(5, 956);
		this->labelInstallation->Name = L"labelInstallation";
		this->labelInstallation->Size = System::Drawing::Size(476, 24);
		this->labelInstallation->TabIndex = 8;
		this->labelInstallation->Text = L"IFO SAN GALLICANO";
		this->labelInstallation->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// labelTime
		// 
		this->labelTime->AutoSize = true;
		this->labelTime->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->labelTime->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)), static_cast<System::Int32>(static_cast<System::Byte>(149)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->labelTime->Location = System::Drawing::Point(111, 984);
		this->labelTime->Name = L"labelTime";
		this->labelTime->Size = System::Drawing::Size(88, 24);
		this->labelTime->TabIndex = 9;
		this->labelTime->Text = L"10:35 PM";
		// 
		// calibZerosettingPanel
		// 
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingLog);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingSlide);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingTilt);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingArm);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingVertical);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingBody);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingAll);
		this->calibZerosettingPanel->Location = System::Drawing::Point(1000, 50);
		this->calibZerosettingPanel->Name = L"calibZerosettingPanel";
		this->calibZerosettingPanel->Size = System::Drawing::Size(600, 880);
		this->calibZerosettingPanel->TabIndex = 16;
		// 
		// zeroSettingLog
		// 
		this->zeroSettingLog->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)), static_cast<System::Int32>(static_cast<System::Byte>(207)),
			static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->zeroSettingLog->BorderStyle = System::Windows::Forms::BorderStyle::None;
		this->zeroSettingLog->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->zeroSettingLog->Location = System::Drawing::Point(50, 430);
		this->zeroSettingLog->Name = L"zeroSettingLog";
		this->zeroSettingLog->ReadOnly = true;
		this->zeroSettingLog->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
		this->zeroSettingLog->ShortcutsEnabled = false;
		this->zeroSettingLog->Size = System::Drawing::Size(500, 400);
		this->zeroSettingLog->TabIndex = 24;
		this->zeroSettingLog->Text = L"1";
		// 
		// zeroSettingSlide
		// 
		this->zeroSettingSlide->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingSlide->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->zeroSettingSlide->Location = System::Drawing::Point(220, 236);
		this->zeroSettingSlide->Name = L"zeroSettingSlide";
		this->zeroSettingSlide->Size = System::Drawing::Size(165, 165);
		this->zeroSettingSlide->TabIndex = 23;
		this->zeroSettingSlide->TabStop = false;
		this->zeroSettingSlide->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::zeroSettingSlide_Click);
		// 
		// zeroSettingTilt
		// 
		this->zeroSettingTilt->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingTilt->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->zeroSettingTilt->Location = System::Drawing::Point(410, 40);
		this->zeroSettingTilt->Name = L"zeroSettingTilt";
		this->zeroSettingTilt->Size = System::Drawing::Size(165, 165);
		this->zeroSettingTilt->TabIndex = 22;
		this->zeroSettingTilt->TabStop = false;
		this->zeroSettingTilt->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::zeroSettingTilt_Click);
		// 
		// zeroSettingArm
		// 
		this->zeroSettingArm->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingArm->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->zeroSettingArm->Location = System::Drawing::Point(220, 40);
		this->zeroSettingArm->Name = L"zeroSettingArm";
		this->zeroSettingArm->Size = System::Drawing::Size(165, 165);
		this->zeroSettingArm->TabIndex = 21;
		this->zeroSettingArm->TabStop = false;
		this->zeroSettingArm->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::zeroSettingArm_Click);
		// 
		// zeroSettingVertical
		// 
		this->zeroSettingVertical->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingVertical->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->zeroSettingVertical->Location = System::Drawing::Point(30, 40);
		this->zeroSettingVertical->Name = L"zeroSettingVertical";
		this->zeroSettingVertical->Size = System::Drawing::Size(165, 165);
		this->zeroSettingVertical->TabIndex = 20;
		this->zeroSettingVertical->TabStop = false;
		this->zeroSettingVertical->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::zeroSettingVertical_Click);
		// 
		// zeroSettingBody
		// 
		this->zeroSettingBody->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingBody->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->zeroSettingBody->Location = System::Drawing::Point(30, 236);
		this->zeroSettingBody->Name = L"zeroSettingBody";
		this->zeroSettingBody->Size = System::Drawing::Size(165, 165);
		this->zeroSettingBody->TabIndex = 19;
		this->zeroSettingBody->TabStop = false;
		this->zeroSettingBody->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::zeroSettingBody_Click);
		// 
		// zeroSettingAll
		// 
		this->zeroSettingAll->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingAll->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->zeroSettingAll->Location = System::Drawing::Point(410, 236);
		this->zeroSettingAll->Name = L"zeroSettingAll";
		this->zeroSettingAll->Size = System::Drawing::Size(165, 165);
		this->zeroSettingAll->TabIndex = 1;
		this->zeroSettingAll->TabStop = false;
		this->zeroSettingAll->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::zeroSettingAll_Click);
		// 
		// serviceCanc
		// 
		this->serviceCanc->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->serviceCanc->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->serviceCanc->Location = System::Drawing::Point(498, 936);
		this->serviceCanc->Name = L"serviceCanc";
		this->serviceCanc->Size = System::Drawing::Size(99, 85);
		this->serviceCanc->TabIndex = 19;
		this->serviceCanc->TabStop = false;
		this->serviceCanc->Click += gcnew System::EventHandler(this, &ServiceZeroSettingTool::cancButton_Click);
		// 
		// serviceMenuTitle
		// 
		this->serviceMenuTitle->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20.25F, System::Drawing::FontStyle::Regular,
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->serviceMenuTitle->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(156)),
			static_cast<System::Int32>(static_cast<System::Byte>(149)), static_cast<System::Int32>(static_cast<System::Byte>(149)));
		this->serviceMenuTitle->Location = System::Drawing::Point(3, 9);
		this->serviceMenuTitle->Name = L"serviceMenuTitle";
		this->serviceMenuTitle->Size = System::Drawing::Size(594, 36);
		this->serviceMenuTitle->TabIndex = 20;
		this->serviceMenuTitle->Text = L"TITLE";
		this->serviceMenuTitle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		// 
		// ServiceForm
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
		this->Controls->Add(this->serviceMenuTitle);
		this->Controls->Add(this->calibZerosettingPanel);
		this->Controls->Add(this->labelInstallation);
		this->Controls->Add(this->labelDate);
		this->Controls->Add(this->labelTime);
		this->Controls->Add(this->serviceCanc);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Location = System::Drawing::Point(1000, 0);
		this->Name = L"ServiceForm";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		this->calibZerosettingPanel->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingSlide))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingTilt))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingArm))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingVertical))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingBody))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingAll))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceCanc))->EndInit();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	// Common Panel Buttons
	private: System::Void onServiceTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e);
	private: System::Void cancButton_Click(System::Object^ sender, System::EventArgs^ e);

	// Calibration - Zero Setting Panel
	private: System::Void zeroSettingBody_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingVertical_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingArm_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingTilt_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingSlide_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingAll_Click(System::Object^ sender, System::EventArgs^ e);

};

