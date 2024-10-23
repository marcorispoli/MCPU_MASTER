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

public ref class ServiceForm : public System::Windows::Forms::Form
{
public: static ServiceForm^ panel;
private:void formInitialization(void);
public:
	ServiceForm(void)
	{
		panel = this;
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		formInitialization();

	}
	

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~ServiceForm()
	{
		if (components)
		{
			delete components;
		}
	}


protected:


Object^ pConfirmation; // Used in the case a confirmation should needed

private: System::Windows::Forms::PictureBox^ exposureTool;
private: System::Windows::Forms::PictureBox^ biopsyTool;


public:System::Timers::Timer^ serviceTimer;

// Handles the Window status
public:	void open(void);
public:	void close(void);
public: bool open_status;
public:HWND window;
public:void initServiceStatus(void);
public:void serviceStatusManagement(void);

	   // Handles the system passwords
public: static bool passwd_operator = false;
public: static bool passwd_factory = false;
public: static bool isPasswordOperator() { return passwd_operator; }
public: static bool isPasswordFactory() { return passwd_factory; }

public: enum class panels {
	PANEL_NOT_DEFINED = 0,
	MAIN_SERVICE_PANEL,
	CALIB_PANEL,
	SETUP_PANEL,
	CALIB_ZEROSETTING_PANEL,
	TOOL_EXPOSURE_PANEL,
	TOOL_ROTATION_PANEL,
};

#define PANEL_WIDTH 600
#define PANEL_HIGH 880
#define PANEL_X 0
#define PANEL_Y 50

public: ServiceForm::panels current_panel;
public: void setActivePanel(ServiceForm::panels p);

public: void initServicePanel(void);
public: void cancServicePanel(void);


public: void initCalibrationPanel(void);
public: void cancCalibrationPanel(void);




private: System::Windows::Forms::Label^ serviceMenuTitle;
private: System::Windows::Forms::Label^ labelDate;
private: System::Windows::Forms::Label^ labelInstallation;
private: System::Windows::Forms::Label^ labelTime;
private: System::Windows::Forms::PictureBox^ serviceCanc;

private: System::Windows::Forms::Panel^ servicePanel;
private: System::Windows::Forms::PictureBox^ rotationTool;
private: System::Windows::Forms::PictureBox^ systemSetup;
private: System::Windows::Forms::PictureBox^ systemCalibration;

private: System::Windows::Forms::Panel^ calibPanel;
private: System::Windows::Forms::PictureBox^ zeroSetting;



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
		this->rotationTool = (gcnew System::Windows::Forms::PictureBox());
		this->systemSetup = (gcnew System::Windows::Forms::PictureBox());
		this->systemCalibration = (gcnew System::Windows::Forms::PictureBox());
		this->labelDate = (gcnew System::Windows::Forms::Label());
		this->labelInstallation = (gcnew System::Windows::Forms::Label());
		this->labelTime = (gcnew System::Windows::Forms::Label());
		this->servicePanel = (gcnew System::Windows::Forms::Panel());
		this->calibPanel = (gcnew System::Windows::Forms::Panel());
		this->exposureTool = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSetting = (gcnew System::Windows::Forms::PictureBox());
		this->serviceCanc = (gcnew System::Windows::Forms::PictureBox());
		this->serviceMenuTitle = (gcnew System::Windows::Forms::Label());
		this->biopsyTool = (gcnew System::Windows::Forms::PictureBox());
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationTool))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->systemSetup))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->systemCalibration))->BeginInit();
		this->servicePanel->SuspendLayout();
		this->calibPanel->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->exposureTool))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSetting))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceCanc))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->biopsyTool))->BeginInit();
		this->SuspendLayout();
		// 
		// rotationTool
		// 
		this->rotationTool->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationTool->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->rotationTool->Location = System::Drawing::Point(216, 116);
		this->rotationTool->Name = L"rotationTool";
		this->rotationTool->Size = System::Drawing::Size(160, 160);
		this->rotationTool->TabIndex = 1;
		this->rotationTool->TabStop = false;
		this->rotationTool->Click += gcnew System::EventHandler(this, &ServiceForm::systemRotation_Click);
		// 
		// systemSetup
		// 
		this->systemSetup->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->systemSetup->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->systemSetup->Location = System::Drawing::Point(216, 621);
		this->systemSetup->Name = L"systemSetup";
		this->systemSetup->Size = System::Drawing::Size(160, 160);
		this->systemSetup->TabIndex = 2;
		this->systemSetup->TabStop = false;
		// 
		// systemCalibration
		// 
		this->systemCalibration->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->systemCalibration->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->systemCalibration->Location = System::Drawing::Point(216, 368);
		this->systemCalibration->Name = L"systemCalibration";
		this->systemCalibration->Size = System::Drawing::Size(160, 160);
		this->systemCalibration->TabIndex = 3;
		this->systemCalibration->TabStop = false;
		this->systemCalibration->Click += gcnew System::EventHandler(this, &ServiceForm::systemCalibration_Click);
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
		// servicePanel
		// 
		this->servicePanel->Controls->Add(this->rotationTool);
		this->servicePanel->Controls->Add(this->systemSetup);
		this->servicePanel->Controls->Add(this->systemCalibration);
		this->servicePanel->Location = System::Drawing::Point(1000, 50);
		this->servicePanel->Name = L"servicePanel";
		this->servicePanel->Size = System::Drawing::Size(600, 880);
		this->servicePanel->TabIndex = 14;
		// 
		// calibPanel
		// 
		this->calibPanel->Controls->Add(this->biopsyTool);
		this->calibPanel->Controls->Add(this->exposureTool);
		this->calibPanel->Controls->Add(this->zeroSetting);
		this->calibPanel->Location = System::Drawing::Point(0, 50);
		this->calibPanel->Name = L"calibPanel";
		this->calibPanel->Size = System::Drawing::Size(600, 880);
		this->calibPanel->TabIndex = 15;
		// 
		// exposureTool
		// 
		this->exposureTool->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->exposureTool->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->exposureTool->Location = System::Drawing::Point(237, 40);
		this->exposureTool->Name = L"exposureTool";
		this->exposureTool->Size = System::Drawing::Size(165, 165);
		this->exposureTool->TabIndex = 2;
		this->exposureTool->TabStop = false;
		this->exposureTool->Click += gcnew System::EventHandler(this, &ServiceForm::exposureTool_Click);
		// 
		// zeroSetting
		// 
		this->zeroSetting->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSetting->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->zeroSetting->Location = System::Drawing::Point(40, 40);
		this->zeroSetting->Name = L"zeroSetting";
		this->zeroSetting->Size = System::Drawing::Size(165, 165);
		this->zeroSetting->TabIndex = 1;
		this->zeroSetting->TabStop = false;
		this->zeroSetting->Click += gcnew System::EventHandler(this, &ServiceForm::zeroSetting_Click);
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
		this->serviceCanc->Click += gcnew System::EventHandler(this, &ServiceForm::cancButton_Click);
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
		// biopsyTool
		// 
		this->biopsyTool->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->biopsyTool->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->biopsyTool->Location = System::Drawing::Point(40, 224);
		this->biopsyTool->Name = L"biopsyTool";
		this->biopsyTool->Size = System::Drawing::Size(165, 165);
		this->biopsyTool->TabIndex = 3;
		this->biopsyTool->TabStop = false;
		this->biopsyTool->Click += gcnew System::EventHandler(this, &ServiceForm::biopsyTool_Click);
		// 
		// ServiceForm
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
		this->Controls->Add(this->serviceMenuTitle);
		this->Controls->Add(this->labelInstallation);
		this->Controls->Add(this->labelDate);
		this->Controls->Add(this->labelTime);
		this->Controls->Add(this->serviceCanc);
		this->Controls->Add(this->calibPanel);
		this->Controls->Add(this->servicePanel);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Location = System::Drawing::Point(1000, 0);
		this->Name = L"ServiceForm";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationTool))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->systemSetup))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->systemCalibration))->EndInit();
		this->servicePanel->ResumeLayout(false);
		this->calibPanel->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->exposureTool))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSetting))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceCanc))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->biopsyTool))->EndInit();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	// Service Panel Buttons
	private: System::Void onServiceTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e);
	private: System::Void cancButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void systemCalibration_Click(System::Object^ sender, System::EventArgs^ e);
	

	// Calibration Panel
	private: System::Void zeroSetting_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void exposureTool_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void systemRotation_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void biopsyTool_Click(System::Object^ sender, System::EventArgs^ e);
	
};

