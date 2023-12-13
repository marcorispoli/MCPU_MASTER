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
	enum class panels {
		PANEL_NOT_DEFINED = 0,
		MAIN_SERVICE_PANEL,
		CALIB_PANEL,
		CALIB_ZEROSETTING_PANEL,
		TOOL_ROTATION_PANEL,
	};
private: panels current_panel;
private: void setActivePanel(ServiceForm::panels p);

private: void initMainPanel(void);
private: void initCalibrationPanel(void);
private: void initZeroSettingCalibrationPanel(void);
	   
	   
public:
public:System::Timers::Timer^ serviceTimer;
private: System::Windows::Forms::Label^ labelDate;
private: System::Windows::Forms::Label^ labelInstallation;
private: System::Windows::Forms::Label^ labelTime;
private: System::Windows::Forms::Button^ CancButton;
private: System::Windows::Forms::Panel^ calibPanel;
private: System::Windows::Forms::Panel^ calibZerosettingPanel;
private: System::Windows::Forms::PictureBox^ zeroSettingSlide;
private: System::Windows::Forms::RichTextBox^ zeroSettingLog;

private: System::Windows::Forms::PictureBox^ zeroSettingTilt;
private: System::Windows::Forms::PictureBox^ zeroSettingArm;
private: System::Windows::Forms::PictureBox^ zeroSettingVertical;
private: System::Windows::Forms::PictureBox^ zeroSettingBody;
private: System::Windows::Forms::Button^ buttonCalibZeroSettingExit;
private: System::Windows::Forms::PictureBox^ zeroSettingAll;

private: System::Windows::Forms::Button^ calibPanelExit;

private: System::Windows::Forms::PictureBox^ zeroSetting;
private: System::Windows::Forms::PictureBox^ pictureBox3;
private: System::Windows::Forms::Panel^ mainPanel;

public:

// Handles the Window status
public:	void open(void);
public:	void close(void);
private: bool open_status;
private:HWND window;
private:void formInitialization(void);
private:void initServiceStatus(void);
private:void serviceStatusManagement(void);

// Handles the system passwords
private: static bool passwd_operator = false;
private: static bool passwd_factory = false;
private: static bool isPasswordOperator() {return passwd_operator;}
private: static bool isPasswordFactory() { return passwd_factory; }

	
public:
	ServiceForm(void)
	{
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
private: System::Windows::Forms::PictureBox^ rotationTool;
private: System::Windows::Forms::PictureBox^ systemSetup;
private: System::Windows::Forms::PictureBox^ systemCalibration;
protected:

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
		this->rotationTool = (gcnew System::Windows::Forms::PictureBox());
		this->systemSetup = (gcnew System::Windows::Forms::PictureBox());
		this->systemCalibration = (gcnew System::Windows::Forms::PictureBox());
		this->labelDate = (gcnew System::Windows::Forms::Label());
		this->labelInstallation = (gcnew System::Windows::Forms::Label());
		this->labelTime = (gcnew System::Windows::Forms::Label());
		this->mainPanel = (gcnew System::Windows::Forms::Panel());
		this->CancButton = (gcnew System::Windows::Forms::Button());
		this->calibPanel = (gcnew System::Windows::Forms::Panel());
		this->calibPanelExit = (gcnew System::Windows::Forms::Button());
		this->zeroSetting = (gcnew System::Windows::Forms::PictureBox());
		this->calibZerosettingPanel = (gcnew System::Windows::Forms::Panel());
		this->zeroSettingSlide = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingTilt = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingArm = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingVertical = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingBody = (gcnew System::Windows::Forms::PictureBox());
		this->buttonCalibZeroSettingExit = (gcnew System::Windows::Forms::Button());
		this->zeroSettingAll = (gcnew System::Windows::Forms::PictureBox());
		this->zeroSettingLog = (gcnew System::Windows::Forms::RichTextBox());
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationTool))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->systemSetup))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->systemCalibration))->BeginInit();
		this->mainPanel->SuspendLayout();
		this->calibPanel->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSetting))->BeginInit();
		this->calibZerosettingPanel->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingSlide))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingTilt))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingArm))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingVertical))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingBody))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingAll))->BeginInit();
		this->SuspendLayout();
		// 
		// rotationTool
		// 
		this->rotationTool->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->rotationTool->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->rotationTool->Location = System::Drawing::Point(220, 127);
		this->rotationTool->Name = L"rotationTool";
		this->rotationTool->Size = System::Drawing::Size(160, 160);
		this->rotationTool->TabIndex = 1;
		this->rotationTool->TabStop = false;
		// 
		// systemSetup
		// 
		this->systemSetup->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->systemSetup->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->systemSetup->Location = System::Drawing::Point(220, 337);
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
		this->systemCalibration->Location = System::Drawing::Point(220, 555);
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
		this->labelDate->Location = System::Drawing::Point(3, 980);
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
		this->labelInstallation->Location = System::Drawing::Point(212, 980);
		this->labelInstallation->Name = L"labelInstallation";
		this->labelInstallation->Size = System::Drawing::Size(280, 24);
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
		this->labelTime->Location = System::Drawing::Point(109, 980);
		this->labelTime->Name = L"labelTime";
		this->labelTime->Size = System::Drawing::Size(88, 24);
		this->labelTime->TabIndex = 9;
		this->labelTime->Text = L"10:35 PM";
		// 
		// mainPanel
		// 
		this->mainPanel->Controls->Add(this->CancButton);
		this->mainPanel->Controls->Add(this->labelDate);
		this->mainPanel->Controls->Add(this->labelInstallation);
		this->mainPanel->Controls->Add(this->labelTime);
		this->mainPanel->Controls->Add(this->rotationTool);
		this->mainPanel->Controls->Add(this->systemSetup);
		this->mainPanel->Controls->Add(this->systemCalibration);
		this->mainPanel->Location = System::Drawing::Point(1000, 0);
		this->mainPanel->Name = L"mainPanel";
		this->mainPanel->Size = System::Drawing::Size(600, 1024);
		this->mainPanel->TabIndex = 14;
		// 
		// CancButton
		// 
		this->CancButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->CancButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->CancButton->FlatAppearance->BorderColor = System::Drawing::Color::Black;
		this->CancButton->FlatAppearance->BorderSize = 0;
		this->CancButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
		this->CancButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->CancButton->Location = System::Drawing::Point(498, 936);
		this->CancButton->Name = L"CancButton";
		this->CancButton->Size = System::Drawing::Size(99, 85);
		this->CancButton->TabIndex = 18;
		this->CancButton->UseVisualStyleBackColor = false;
		this->CancButton->Click += gcnew System::EventHandler(this, &ServiceForm::cancButton_Click);
		// 
		// calibPanel
		// 
		this->calibPanel->Controls->Add(this->calibPanelExit);
		this->calibPanel->Controls->Add(this->zeroSetting);
		this->calibPanel->Location = System::Drawing::Point(1000, 0);
		this->calibPanel->Name = L"calibPanel";
		this->calibPanel->Size = System::Drawing::Size(600, 1024);
		this->calibPanel->TabIndex = 15;
		// 
		// calibPanelExit
		// 
		this->calibPanelExit->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->calibPanelExit->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->calibPanelExit->FlatAppearance->BorderColor = System::Drawing::Color::Black;
		this->calibPanelExit->FlatAppearance->BorderSize = 0;
		this->calibPanelExit->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
		this->calibPanelExit->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->calibPanelExit->Location = System::Drawing::Point(498, 936);
		this->calibPanelExit->Name = L"calibPanelExit";
		this->calibPanelExit->Size = System::Drawing::Size(99, 85);
		this->calibPanelExit->TabIndex = 18;
		this->calibPanelExit->UseVisualStyleBackColor = false;
		this->calibPanelExit->Click += gcnew System::EventHandler(this, &ServiceForm::calibPanelExit_Click);
		// 
		// zeroSetting
		// 
		this->zeroSetting->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSetting->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->zeroSetting->Location = System::Drawing::Point(232, 320);
		this->zeroSetting->Name = L"zeroSetting";
		this->zeroSetting->Size = System::Drawing::Size(160, 160);
		this->zeroSetting->TabIndex = 1;
		this->zeroSetting->TabStop = false;
		this->zeroSetting->Click += gcnew System::EventHandler(this, &ServiceForm::zeroSetting_Click);
		// 
		// calibZerosettingPanel
		// 
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingLog);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingSlide);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingTilt);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingArm);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingVertical);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingBody);
		this->calibZerosettingPanel->Controls->Add(this->buttonCalibZeroSettingExit);
		this->calibZerosettingPanel->Controls->Add(this->zeroSettingAll);
		this->calibZerosettingPanel->Location = System::Drawing::Point(0, 0);
		this->calibZerosettingPanel->Name = L"calibZerosettingPanel";
		this->calibZerosettingPanel->Size = System::Drawing::Size(600, 1024);
		this->calibZerosettingPanel->TabIndex = 16;
		// 
		// zeroSettingSlide
		// 
		this->zeroSettingSlide->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingSlide->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->zeroSettingSlide->Location = System::Drawing::Point(219, 348);
		this->zeroSettingSlide->Name = L"zeroSettingSlide";
		this->zeroSettingSlide->Size = System::Drawing::Size(160, 160);
		this->zeroSettingSlide->TabIndex = 23;
		this->zeroSettingSlide->TabStop = false;
		this->zeroSettingSlide->Click += gcnew System::EventHandler(this, &ServiceForm::zeroSettingSlide_Click);
		// 
		// zeroSettingTilt
		// 
		this->zeroSettingTilt->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingTilt->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->zeroSettingTilt->Location = System::Drawing::Point(44, 348);
		this->zeroSettingTilt->Name = L"zeroSettingTilt";
		this->zeroSettingTilt->Size = System::Drawing::Size(160, 160);
		this->zeroSettingTilt->TabIndex = 22;
		this->zeroSettingTilt->TabStop = false;
		this->zeroSettingTilt->Click += gcnew System::EventHandler(this, &ServiceForm::zeroSettingTilt_Click);
		// 
		// zeroSettingArm
		// 
		this->zeroSettingArm->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingArm->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->zeroSettingArm->Location = System::Drawing::Point(394, 167);
		this->zeroSettingArm->Name = L"zeroSettingArm";
		this->zeroSettingArm->Size = System::Drawing::Size(160, 160);
		this->zeroSettingArm->TabIndex = 21;
		this->zeroSettingArm->TabStop = false;
		this->zeroSettingArm->Click += gcnew System::EventHandler(this, &ServiceForm::zeroSettingArm_Click);
		// 
		// zeroSettingVertical
		// 
		this->zeroSettingVertical->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingVertical->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->zeroSettingVertical->Location = System::Drawing::Point(219, 167);
		this->zeroSettingVertical->Name = L"zeroSettingVertical";
		this->zeroSettingVertical->Size = System::Drawing::Size(160, 160);
		this->zeroSettingVertical->TabIndex = 20;
		this->zeroSettingVertical->TabStop = false;
		this->zeroSettingVertical->Click += gcnew System::EventHandler(this, &ServiceForm::zeroSettingVertical_Click);
		// 
		// zeroSettingBody
		// 
		this->zeroSettingBody->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingBody->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->zeroSettingBody->Location = System::Drawing::Point(44, 167);
		this->zeroSettingBody->Name = L"zeroSettingBody";
		this->zeroSettingBody->Size = System::Drawing::Size(160, 160);
		this->zeroSettingBody->TabIndex = 19;
		this->zeroSettingBody->TabStop = false;
		this->zeroSettingBody->Click += gcnew System::EventHandler(this, &ServiceForm::zeroSettingBody_Click);
		// 
		// buttonCalibZeroSettingExit
		// 
		this->buttonCalibZeroSettingExit->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
			static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->buttonCalibZeroSettingExit->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->buttonCalibZeroSettingExit->FlatAppearance->BorderColor = System::Drawing::Color::Black;
		this->buttonCalibZeroSettingExit->FlatAppearance->BorderSize = 0;
		this->buttonCalibZeroSettingExit->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
		this->buttonCalibZeroSettingExit->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular,
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->buttonCalibZeroSettingExit->Location = System::Drawing::Point(498, 936);
		this->buttonCalibZeroSettingExit->Name = L"buttonCalibZeroSettingExit";
		this->buttonCalibZeroSettingExit->Size = System::Drawing::Size(99, 85);
		this->buttonCalibZeroSettingExit->TabIndex = 18;
		this->buttonCalibZeroSettingExit->UseVisualStyleBackColor = false;
		this->buttonCalibZeroSettingExit->Click += gcnew System::EventHandler(this, &ServiceForm::buttonCalibZeroSettingExit_Click);
		// 
		// zeroSettingAll
		// 
		this->zeroSettingAll->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
			static_cast<System::Int32>(static_cast<System::Byte>(0)));
		this->zeroSettingAll->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->zeroSettingAll->Location = System::Drawing::Point(394, 348);
		this->zeroSettingAll->Name = L"zeroSettingAll";
		this->zeroSettingAll->Size = System::Drawing::Size(160, 160);
		this->zeroSettingAll->TabIndex = 1;
		this->zeroSettingAll->TabStop = false;
		this->zeroSettingAll->Click += gcnew System::EventHandler(this, &ServiceForm::zeroSettingAll_Click);
		// 
		// zeroSettingLog
		// 
		this->zeroSettingLog->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)), static_cast<System::Int32>(static_cast<System::Byte>(207)),
			static_cast<System::Int32>(static_cast<System::Byte>(208)));
		this->zeroSettingLog->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		this->zeroSettingLog->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->zeroSettingLog->Location = System::Drawing::Point(44, 584);
		this->zeroSettingLog->Name = L"zeroSettingLog";
		this->zeroSettingLog->ReadOnly = true;
		this->zeroSettingLog->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
		this->zeroSettingLog->Size = System::Drawing::Size(510, 291);
		this->zeroSettingLog->TabIndex = 24;
		this->zeroSettingLog->Text = L"";
		// 
		// ServiceForm
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
		this->Controls->Add(this->calibZerosettingPanel);
		this->Controls->Add(this->calibPanel);
		this->Controls->Add(this->mainPanel);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Name = L"ServiceForm";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotationTool))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->systemSetup))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->systemCalibration))->EndInit();
		this->mainPanel->ResumeLayout(false);
		this->mainPanel->PerformLayout();
		this->calibPanel->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSetting))->EndInit();
		this->calibZerosettingPanel->ResumeLayout(false);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingSlide))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingTilt))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingArm))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingVertical))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingBody))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->zeroSettingAll))->EndInit();
		this->ResumeLayout(false);

	}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	private: System::Void onServiceTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e);

	private: System::Void cancButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void systemCalibration_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void calibPanelExit_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSetting_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void buttonCalibZeroSettingExit_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingBody_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingVertical_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingArm_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingTilt_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingSlide_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void zeroSettingAll_Click(System::Object^ sender, System::EventArgs^ e);
};

