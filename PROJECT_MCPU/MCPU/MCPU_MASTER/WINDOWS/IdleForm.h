#pragma once
#include <Windows.h>

/// \addtogroup OPERMODEDOC
/// <div style="page-break-after: always;"></div>
/// 
/// # Idle Operating Mode
/// 
/// ## Overview 
/// 
/// This operating mode handles the scenario where the Gantry is left in stand-by.
/// 
/// \remarks
/// When the Gantry is in Idle mode it is supposed that it can be left unattended.
/// In this situation the unit may be approached from persons not trained.
/// To prevent any possible unexpected activation the Gantry devices are set in a conservative and protective mode
/// 
/// ## Device Setting 
/// 
/// The following list provides the status of the Gantry device in Idle:
/// + The compressor device is disabled;
/// + All the Motors are disabled;
/// + The Format collimation is set to OPEN;
/// + The Filter selector is set on the Pb to protect the Tube hole;
/// + The Mirror is set to Home position and the light switched off;
/// + The manual key buttons are disabled;
/// + The Grid device is set to Out Of Field; 
/// 
/// <div style="page-break-after: always;"></div>
/// 
/// ## Data Monitoring
/// 
/// During the Idle operating status, several system imputs are 
/// constantly monitored and the status is displayed on the Idle GUI.
/// 
/// The following image shows the Idle GUI:
/// 
/// \image html ./IdleMode/IdleGui.png
/// 
/// In the previous image there are the followings
/// picture that displays a particolar status data:
/// 
/// ### Application Running Mode
/// 
/// |Icon|Description|
/// |:--:|:--:|
/// |\image html ./IdleMode/DemoMode.png|"Application running in Demo mode"|
/// |\image html ./IdleMode/XrayMode.png|"Application running in Normal mode"|
/// |\image html ./IdleMode/ServiceToolMode.png|"Generator Service Tool"|
/// |\image html ./IdleMode/Biopsy.png|"Biopsy detected"|
/// 
/// \note
/// When the Generator service tool is active, the Gantry disables the control 
/// of the generator device that is under the control of the generator service tool application.
/// The Gantry enables the xray enable signal on the generator interface synchronization signals 
/// allowing the service tool to activate the xrays for calibratior or test.
/// As soon as the service tool terminate, the Gantry restart the normal running mode.
/// 
/// 
/// 
/// ### Battery Status
/// 
/// |Icon|Description|
/// |:--:|:--:|
/// |\image html ./IdleMode/Battery/BatteryConnected.png| Battery connected and fully charged<\br>Ac power present|
/// |\image html ./IdleMode/Battery/BatteryConnectedLow.png| Battery connected with low charge<\br>Ac power present|
/// |\image html ./IdleMode/Battery/BatteryDisonnected.png| Battery disabled<\br>Ac power present|
/// |\image html ./IdleMode/Battery/BatteryPowerDownFull.png| Battery connected and fully charged<\br>Powerdown event|
/// |\image html ./IdleMode/Battery/PowerDownBattLow.png| Battery connected with low charge charged<\br>Powerdown event|
/// 
/// \warning
/// When the batteries are disabled (the button on the Gantry extermnal interface is switched off)
/// the Gantry is no more protected against power down conditions.
///  
/// ### AWS connection status
/// 
/// |Icon|Description|
/// |:--:|:--:|
/// |\image html ./IdleMode/AwsOff.png| Aws disconnected|
/// |\image html ./IdleMode/AwsOn.png| Aws connected|
/// 
/// \remarks
/// The application provides two tcp/ip sockets for the AWS connection.
/// Both channels need to be connected in order to run successfully the communication protocol.
///
/// ### Gantry Device connection status
/// 
/// |Icon|Description|
/// |:--:|:--:|
/// |\image html ./IdleMode/PeripheralsDisconnected.png| Some of the devices are not connected|
/// |\image html ./IdleMode/PeripheralsConnected.png| Device connected |
/// 
/// \remarks
/// In case one or more device is missing (connection lost) 
/// an error message is activated for eny missing device.
///  
/// ### Door Room status
/// 
/// |Icon|Description|
/// |:--:|:--:|
/// |\image html ./IdleMode/DoorOpen.png| The room's door is open|
/// |\image html ./IdleMode/DoorClosed.png| The room's door is closed |
/// 
/// \remarks
/// In order to successfully enter the Patient Operating Mode it is mandatory 
/// to get the room door closed.
///  
/// ### Tube Temperature Monitoring status
/// 
/// The application monitors the current tube temperature sensors, reading the registers of the collimator device (PCB303).
/// The temperature of the sensors are rapresented in percentage respect the maximum possible 
/// before the internal tube protection operates.
/// 
/// The temperature displayed is the maximum value of the sensors.
/// 
/// |Icon|Description|
/// |:--:|:--:|
/// |\image html ./IdleMode/TempNok.png|The tube temperature is High|
/// |\image html ./IdleMode/TempOk.png| The Tube Temperature is normal|
/// 
/// ### Active Messages
/// 
/// The Idle status monitors and dislays the current active messages 
/// 
/// In case some message should be active a graphic push button
/// opens a dedicated window, showing a list of active messages:
/// + (see \ref MessageNotifyDescription)
/// 
/// |Icon|Description|
/// |:--:|:--:|
/// |\image html ./IdleMode/AlarmOff.png|No active messages are present|
/// |\image html ./IdleMode/AlarmOn.png|Almost one Error message is active|
/// |\image html ./IdleMode/WarningOn.png|No Errors and almost one Warning message is active|
/// |\image html ./IdleMode/InfoOn.png|No Errors, No Warnings and almost one Info message is active|
/// 
/// ### Service Operating Mode Entry Button
/// 
/// The Idle mode allows to enter the service operating mode 
/// and its related GUI panels pressing the following graphic button :
/// 
/// \image html ./IdleMode/Service.png
///
/// \todo
/// Implementare la dialog per mostrare il package corrente
///  
/// 




using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;




/// <summary>
/// \ingroup IDLEIMPL
/// 
/// This is the Idle Operating mode implementing module
/// 
/// </summary>
public ref class IdleForm :  public System::Windows::Forms::Form
{
#define WINMSG_TIMER WM_USER + 1
#define WINMSG_OPEN WINMSG_TIMER + 1
#define WINMSG_CLOSE WINMSG_OPEN + 1


public:
	System::Timers::Timer^ idleTimer;
	

public:
	void idleStatusManagement(void);
	void initIdleStatus(void);
	void evaluatePopupPanels(void);
	void open(void);
	void close(void);
	bool open_status;
	void onPowerOffOkCallback(void);
	   

private:HWND window;
private: System::Windows::Forms::Label^ labelDate;
private: System::Windows::Forms::Label^ labelInstallation;
private: System::Windows::Forms::Label^ labelTime;
private: System::Windows::Forms::Label^ labelTubeData;
private: System::Windows::Forms::Panel^ tubeTempOk;
private: System::Windows::Forms::Panel^ errorButton;
private: System::Windows::Forms::PictureBox^ serviceButton;
private: System::Windows::Forms::Panel^ mainPanel;
private: System::Windows::Forms::PictureBox^ xrayMode;
private: System::Windows::Forms::PictureBox^ batteryConnected;
private: System::Windows::Forms::PictureBox^ awsConnected;
private: System::Windows::Forms::PictureBox^ peripheralsConnected;
private: System::Windows::Forms::PictureBox^ doorClosed;
private: System::Windows::Forms::PictureBox^ powerOff;

private:void formInitialization(void);

	
public:
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
		this->serviceButton = (gcnew System::Windows::Forms::PictureBox());
		this->powerOff = (gcnew System::Windows::Forms::PictureBox());
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
		this->errorButton = (gcnew System::Windows::Forms::Panel());
		this->mainPanel = (gcnew System::Windows::Forms::Panel());
		
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->xrayMode))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->batteryConnected))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->awsConnected))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->peripheralsConnected))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->doorClosed))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceButton))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->powerOff))->BeginInit();

		this->tubeTempOk->SuspendLayout();
		this->mainPanel->SuspendLayout();
		this->SuspendLayout();

		// 
		// powerOff
		// 
		this->powerOff->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->powerOff->Location = System::Drawing::Point(187, 60);
		this->powerOff->Name = L"powerOff";
		this->powerOff->Size = System::Drawing::Size(230, 200);
		this->powerOff->TabIndex = 2;
		this->powerOff->TabStop = false;
		this->powerOff->Click += gcnew System::EventHandler(this, &IdleForm::powerOff_Click);
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
		this->labelInstallation->Size = System::Drawing::Size(305, 24);
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
		// errorButton
		// 
		this->errorButton->Location = System::Drawing::Point(30, 834);
		this->errorButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		this->errorButton->Name = L"errorButton";
		this->errorButton->Size = System::Drawing::Size(160, 160);
		this->errorButton->TabIndex = 13;
		this->errorButton->Click += gcnew System::EventHandler(this, &IdleForm::errorButton_Click);
		// 
		// mainPanel
		// 
		this->mainPanel->Controls->Add(this->powerOff);
		this->mainPanel->Controls->Add(this->serviceButton);
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
		this->mainPanel->Location = System::Drawing::Point(0, 0);
		this->mainPanel->Name = L"mainPanel";
		this->mainPanel->Size = System::Drawing::Size(600, 1024);
		this->mainPanel->TabIndex = 14;
		// 
		// serviceButton
		// 
		this->serviceButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
		this->serviceButton->Location = System::Drawing::Point(410, 834);
		this->serviceButton->Name = L"serviceButton";
		this->serviceButton->Size = System::Drawing::Size(160, 160);
		this->serviceButton->TabIndex = 14;
		this->serviceButton->TabStop = false;
		this->serviceButton->Click += gcnew System::EventHandler(this, &IdleForm::serviceButton_Click);
		// 
		// IdleForm
		// 
		this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
			static_cast<System::Int32>(static_cast<System::Byte>(60)));
		this->ClientSize = System::Drawing::Size(600, 1024);
		this->ControlBox = false;
		this->Controls->Add(this->mainPanel);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Name = L"IdleForm";
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;

		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->powerOff))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->xrayMode))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->batteryConnected))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->awsConnected))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->peripheralsConnected))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->doorClosed))->EndInit();
		this->tubeTempOk->ResumeLayout(false);
		this->tubeTempOk->PerformLayout();
		this->mainPanel->ResumeLayout(false);
		this->mainPanel->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->serviceButton))->EndInit();
		this->ResumeLayout(false);

	}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	private: System::Void onIdleTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		SendMessageA(window, WINMSG_TIMER, 0, 0);
	}

private: System::Void errorButton_Click(System::Object^ sender, System::EventArgs^ e);


private: System::Void serviceButton_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void powerOff_Click(System::Object^ sender, System::EventArgs^ e);
};

