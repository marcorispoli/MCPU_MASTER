/// <summary>
/// \defgroup OperatingMode_Section Operating Mode Modules
/// This section groups all the modules controlling the system operating modes.
///   
/// </summary>
/// The system operations are dived in a subset of operating modes:
/// + Startup Mode: the initial status after system power on;
/// + Idle Mode: the system idle mode management;
/// + Operative Mode: the system operating mode management;
/// + Service Mode: the system service mode management;
/// 
/// Every Operating mode is associated to a given graphical GUI 
/// and a given functional workflow.
/// 
/// See to the related Operating Mode sections for details.
/// 


#pragma once

extern  double dtempo;
#include <Windows.h>


/// <summary>
/// \defgroup Startup_Module Startup Module 
/// This module implements the Workflow and GUI to handle 
/// the system startup workflow.
/// 
/// </summary>
/// \ingroup OperatingMode_Section
/// 
/// At the program beginning several operation shall be initiated 
/// before to enter any of the Operating modes:
/// 
/// + System Initialization;
/// + Device Module initialization;
/// + Revision check;
/// 
/// # System Initialization
/// 
/// The module setup and operation depends by several parameters stroed into the SystemConfig file.
/// 
/// This file determines:
/// + The current system running mode: DEMO, SIMULATED, RUNNING modes;
/// + The current modules operation mode for any of the possible system running modes;
/// + The current set of expected device revision codes;
/// + The communication connection addresses for some of the module;
/// 
/// In the System Initialization all the Operating modules will be instantiated.
/// 
/// # Device Module Initialization
/// 
/// Based on the System running mode and related module's operating mode,
/// every device module is activated in real or simulated mode.
/// 
/// The following device modules will be activated in the given sequence:
/// 
///  + CanDriver Module: this is the first to be activated, in order to open the CAN communication driver;
///  + PCB301 Module: this module initializes the PCB301 communication or the board simulation;
///  + PCB302 Module: this module initializes the PCB302 communication or the board simulation;
///  + PCB303 Module: this module initializes the PCB303 communication or the board simulation;
///  + PCB325 Module: this module initializes the PCB325 communication or the board simulation;
///  + PCB304 Module: this module initializes the PCB304 communication or the board simulation;
///  + PCB326 Module: this module initializes the PCB326 communication or the board simulation;
///  + Generator Module: this module initializes the Generator communication or the device simulation;
///  + TiltMotor Module: this module initializes the Tilt Motor communication or the board simulation;
///  + ArmMotor Module: this module initializes the Arm Motor communication or the board simulation;
///  + SlideMotor Module: this module initializes the Slide Motor communication or the board simulation;
///  + BodyMotor Module: this module initializes the Body Motor communication or the board simulation;
///  + VerticalMotor Module: this module initializes the Vertical Motor communication or the board simulation; 
/// 
/// # Revision Check
/// 
/// Every connected module provides the current bootloader and firmware revision code.
/// This module checks the validity of the detected revision code with a list of 
/// valid revision codes from the SystemConfig file.
/// 
/// If a revision code should not match with the expected, a general error message is activated 
/// preventing the system to enter an operating mode.
/// 
/// # Startup Completion 
/// 
/// When the startup completes, even if an error should happen, \n
/// the system automatically selects the Idle status.
///  
/// 
/// 


namespace CppCLRWinFormsProject {

	
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	

	/// <summary>
	/// This is the Class implementing the Startup Operating mode.
	/// </summary>
	/// \ingroup Startup_Module
	/// 
	/// 
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			window = static_cast<HWND>(Handle.ToPointer());
			MainFormInitialize();

		}
		
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	

	System::Timers::Timer^ startupTimer; //!< This is the Timer to step the workflow
	HWND window;//!< This is the handle of the windw for the windows message exchange
	int startupFase; //!< This is the current fase of the startup
	int startupSubFase;//!< This is the current sub-fase of the startup
	bool startupCompleted;//!< The Startup completed flag
	bool startupError;//!< The Startup completed in error condition

	private: void MainFormInitialize(void);
	private: void StartupProcedure(void);

	private: bool Startup_CanDriver(void);
	private: bool Startup_PCB301(void);
	private: bool Startup_PCB302(void);
	private: bool Startup_PCB303(void);
	private: bool Startup_PCB304(void);
	private: bool Startup_PCB325(void);
	private: bool Startup_PCB326(void);
	private: bool Startup_MotorBody(void);
	private: bool Startup_MotorTilt(void);
	private: bool Startup_MotorArm(void);
	private: bool Startup_MotorShift(void);
	private: bool Startup_MotorVertical(void);
	private: bool Startup_Generator(void);
		   
	
	

	private: System::Windows::Forms::Label^ labelPcb326Activity;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ labelGeneratorActivity;
	private: System::Windows::Forms::Label^ label17;
	private: System::Windows::Forms::PictureBox^ startupIcon;
	private: System::Windows::Forms::GroupBox^ infoPanel;
	private: System::Windows::Forms::Label^ labelBoardInitialization;
	private: System::Windows::Forms::Label^ labelPcb301Activity;
	private: System::Windows::Forms::Label^ labelPcb303Activity;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ labelPcb302Activity;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ labelCanDriverActivity;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::Label^ labelPcb325Activity;

	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ labelPcb304Activity;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ labelShActivity;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::Label^ labelMotorArmActivity;
	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::Label^ labelMotorTiltActivity;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Label^ labelMotorUpDownActivity;
	private: System::Windows::Forms::Label^ label15;
	private: System::Windows::Forms::Label^ labelMotorShiftActivity;
	private: System::Windows::Forms::Label^ label14;
	private: System::Windows::Forms::Label^ labelMotorBodyActivity;
	private: System::Windows::Forms::Label^ label13;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->infoPanel = (gcnew System::Windows::Forms::GroupBox());
			this->labelMotorUpDownActivity = (gcnew System::Windows::Forms::Label());
			this->labelGeneratorActivity = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->labelMotorShiftActivity = (gcnew System::Windows::Forms::Label());
			this->labelPcb326Activity = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->labelMotorBodyActivity = (gcnew System::Windows::Forms::Label());
			this->labelShActivity = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->labelMotorArmActivity = (gcnew System::Windows::Forms::Label());
			this->labelCanDriverActivity = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->labelMotorTiltActivity = (gcnew System::Windows::Forms::Label());
			this->labelPcb325Activity = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->labelPcb304Activity = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->labelPcb303Activity = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->labelPcb302Activity = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->labelPcb301Activity = (gcnew System::Windows::Forms::Label());
			this->labelBoardInitialization = (gcnew System::Windows::Forms::Label());
			this->startupIcon = (gcnew System::Windows::Forms::PictureBox());
			this->infoPanel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->startupIcon))->BeginInit();
			this->SuspendLayout();
			// 
			// infoPanel
			// 
			this->infoPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->infoPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->infoPanel->Controls->Add(this->labelMotorUpDownActivity);
			this->infoPanel->Controls->Add(this->labelGeneratorActivity);
			this->infoPanel->Controls->Add(this->label15);
			this->infoPanel->Controls->Add(this->label17);
			this->infoPanel->Controls->Add(this->labelMotorShiftActivity);
			this->infoPanel->Controls->Add(this->labelPcb326Activity);
			this->infoPanel->Controls->Add(this->label14);
			this->infoPanel->Controls->Add(this->label1);
			this->infoPanel->Controls->Add(this->labelMotorBodyActivity);
			this->infoPanel->Controls->Add(this->labelShActivity);
			this->infoPanel->Controls->Add(this->label13);
			this->infoPanel->Controls->Add(this->label10);
			this->infoPanel->Controls->Add(this->labelMotorArmActivity);
			this->infoPanel->Controls->Add(this->labelCanDriverActivity);
			this->infoPanel->Controls->Add(this->label12);
			this->infoPanel->Controls->Add(this->label9);
			this->infoPanel->Controls->Add(this->labelMotorTiltActivity);
			this->infoPanel->Controls->Add(this->labelPcb325Activity);
			this->infoPanel->Controls->Add(this->label11);
			this->infoPanel->Controls->Add(this->label8);
			this->infoPanel->Controls->Add(this->labelPcb304Activity);
			this->infoPanel->Controls->Add(this->label7);
			this->infoPanel->Controls->Add(this->labelPcb303Activity);
			this->infoPanel->Controls->Add(this->label6);
			this->infoPanel->Controls->Add(this->labelPcb302Activity);
			this->infoPanel->Controls->Add(this->label5);
			this->infoPanel->Controls->Add(this->labelPcb301Activity);
			this->infoPanel->Controls->Add(this->labelBoardInitialization);
			this->infoPanel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->infoPanel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->infoPanel->Location = System::Drawing::Point(26, 248);
			this->infoPanel->Name = L"infoPanel";
			this->infoPanel->Size = System::Drawing::Size(549, 325);
			this->infoPanel->TabIndex = 1;
			this->infoPanel->TabStop = false;
			this->infoPanel->Text = L"BOARD INITIALIZATION";
			// 
			// labelMotorUpDownActivity
			// 
			this->labelMotorUpDownActivity->AutoSize = true;
			this->labelMotorUpDownActivity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelMotorUpDownActivity->ForeColor = System::Drawing::Color::White;
			this->labelMotorUpDownActivity->Location = System::Drawing::Point(216, 245);
			this->labelMotorUpDownActivity->Name = L"labelMotorUpDownActivity";
			this->labelMotorUpDownActivity->Size = System::Drawing::Size(86, 16);
			this->labelMotorUpDownActivity->TabIndex = 11;
			this->labelMotorUpDownActivity->Text = L"Connection ...";
			// 
			// labelGeneratorActivity
			// 
			this->labelGeneratorActivity->AutoSize = true;
			this->labelGeneratorActivity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelGeneratorActivity->ForeColor = System::Drawing::Color::White;
			this->labelGeneratorActivity->Location = System::Drawing::Point(216, 285);
			this->labelGeneratorActivity->Name = L"labelGeneratorActivity";
			this->labelGeneratorActivity->Size = System::Drawing::Size(86, 16);
			this->labelGeneratorActivity->TabIndex = 17;
			this->labelGeneratorActivity->Text = L"Connection ...";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(10, 245);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(139, 16);
			this->label15->TabIndex = 10;
			this->label15->Text = L"VERTICAL MOTOR";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(10, 285);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(94, 16);
			this->label17->TabIndex = 16;
			this->label17->Text = L"SFHD Board";
			// 
			// labelMotorShiftActivity
			// 
			this->labelMotorShiftActivity->AutoSize = true;
			this->labelMotorShiftActivity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelMotorShiftActivity->ForeColor = System::Drawing::Color::White;
			this->labelMotorShiftActivity->Location = System::Drawing::Point(216, 225);
			this->labelMotorShiftActivity->Name = L"labelMotorShiftActivity";
			this->labelMotorShiftActivity->Size = System::Drawing::Size(86, 16);
			this->labelMotorShiftActivity->TabIndex = 9;
			this->labelMotorShiftActivity->Text = L"Connection ...";
			// 
			// labelPcb326Activity
			// 
			this->labelPcb326Activity->AutoSize = true;
			this->labelPcb326Activity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelPcb326Activity->ForeColor = System::Drawing::Color::White;
			this->labelPcb326Activity->Location = System::Drawing::Point(216, 145);
			this->labelPcb326Activity->Name = L"labelPcb326Activity";
			this->labelPcb326Activity->Size = System::Drawing::Size(86, 16);
			this->labelPcb326Activity->TabIndex = 15;
			this->labelPcb326Activity->Text = L"Connection ...";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(10, 225);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(110, 16);
			this->label14->TabIndex = 8;
			this->label14->Text = L"SHIFT MOTOR";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(10, 145);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(181, 16);
			this->label1->TabIndex = 14;
			this->label1->Text = L"PCB 326: Obstacle Board";
			// 
			// labelMotorBodyActivity
			// 
			this->labelMotorBodyActivity->AutoSize = true;
			this->labelMotorBodyActivity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelMotorBodyActivity->ForeColor = System::Drawing::Color::White;
			this->labelMotorBodyActivity->Location = System::Drawing::Point(216, 205);
			this->labelMotorBodyActivity->Name = L"labelMotorBodyActivity";
			this->labelMotorBodyActivity->Size = System::Drawing::Size(86, 16);
			this->labelMotorBodyActivity->TabIndex = 7;
			this->labelMotorBodyActivity->Text = L"Connection ...";
			// 
			// labelShActivity
			// 
			this->labelShActivity->AutoSize = true;
			this->labelShActivity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelShActivity->ForeColor = System::Drawing::Color::White;
			this->labelShActivity->Location = System::Drawing::Point(216, 265);
			this->labelShActivity->Name = L"labelShActivity";
			this->labelShActivity->Size = System::Drawing::Size(86, 16);
			this->labelShActivity->TabIndex = 13;
			this->labelShActivity->Text = L"Connection ...";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(10, 205);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(108, 16);
			this->label13->TabIndex = 6;
			this->label13->Text = L"BODY MOTOR";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(10, 265);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(83, 16);
			this->label10->TabIndex = 12;
			this->label10->Text = L"Smart HUB";
			// 
			// labelMotorArmActivity
			// 
			this->labelMotorArmActivity->AutoSize = true;
			this->labelMotorArmActivity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelMotorArmActivity->ForeColor = System::Drawing::Color::White;
			this->labelMotorArmActivity->Location = System::Drawing::Point(216, 185);
			this->labelMotorArmActivity->Name = L"labelMotorArmActivity";
			this->labelMotorArmActivity->Size = System::Drawing::Size(86, 16);
			this->labelMotorArmActivity->TabIndex = 5;
			this->labelMotorArmActivity->Text = L"Connection ...";
			// 
			// labelCanDriverActivity
			// 
			this->labelCanDriverActivity->AutoSize = true;
			this->labelCanDriverActivity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelCanDriverActivity->ForeColor = System::Drawing::Color::White;
			this->labelCanDriverActivity->Location = System::Drawing::Point(216, 25);
			this->labelCanDriverActivity->Name = L"labelCanDriverActivity";
			this->labelCanDriverActivity->Size = System::Drawing::Size(86, 16);
			this->labelCanDriverActivity->TabIndex = 11;
			this->labelCanDriverActivity->Text = L"Connection ...";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(10, 185);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(99, 16);
			this->label12->TabIndex = 4;
			this->label12->Text = L"ARM MOTOR";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(10, 25);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(145, 16);
			this->label9->TabIndex = 10;
			this->label9->Text = L"CAN DRIVER Board";
			// 
			// labelMotorTiltActivity
			// 
			this->labelMotorTiltActivity->AutoSize = true;
			this->labelMotorTiltActivity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelMotorTiltActivity->ForeColor = System::Drawing::Color::White;
			this->labelMotorTiltActivity->Location = System::Drawing::Point(216, 165);
			this->labelMotorTiltActivity->Name = L"labelMotorTiltActivity";
			this->labelMotorTiltActivity->Size = System::Drawing::Size(86, 16);
			this->labelMotorTiltActivity->TabIndex = 3;
			this->labelMotorTiltActivity->Text = L"Connection ...";
			// 
			// labelPcb325Activity
			// 
			this->labelPcb325Activity->AutoSize = true;
			this->labelPcb325Activity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelPcb325Activity->ForeColor = System::Drawing::Color::White;
			this->labelPcb325Activity->Location = System::Drawing::Point(216, 125);
			this->labelPcb325Activity->Name = L"labelPcb325Activity";
			this->labelPcb325Activity->Size = System::Drawing::Size(86, 16);
			this->labelPcb325Activity->TabIndex = 9;
			this->labelPcb325Activity->Text = L"Connection ...";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(10, 165);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(98, 16);
			this->label11->TabIndex = 2;
			this->label11->Text = L"TILT MOTOR";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(10, 125);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(167, 16);
			this->label8->TabIndex = 8;
			this->label8->Text = L"PCB 325: Biopsy Board";
			// 
			// labelPcb304Activity
			// 
			this->labelPcb304Activity->AutoSize = true;
			this->labelPcb304Activity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelPcb304Activity->ForeColor = System::Drawing::Color::White;
			this->labelPcb304Activity->Location = System::Drawing::Point(216, 105);
			this->labelPcb304Activity->Name = L"labelPcb304Activity";
			this->labelPcb304Activity->Size = System::Drawing::Size(86, 16);
			this->labelPcb304Activity->TabIndex = 7;
			this->labelPcb304Activity->Text = L"Connection ...";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(10, 105);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(160, 16);
			this->label7->TabIndex = 6;
			this->label7->Text = L"PCB 304: Potter Board";
			// 
			// labelPcb303Activity
			// 
			this->labelPcb303Activity->AutoSize = true;
			this->labelPcb303Activity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelPcb303Activity->ForeColor = System::Drawing::Color::White;
			this->labelPcb303Activity->Location = System::Drawing::Point(216, 85);
			this->labelPcb303Activity->Name = L"labelPcb303Activity";
			this->labelPcb303Activity->Size = System::Drawing::Size(86, 16);
			this->labelPcb303Activity->TabIndex = 5;
			this->labelPcb303Activity->Text = L"Connection ...";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(10, 85);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(189, 16);
			this->label6->TabIndex = 4;
			this->label6->Text = L"PCB 303: Collimator Board";
			// 
			// labelPcb302Activity
			// 
			this->labelPcb302Activity->AutoSize = true;
			this->labelPcb302Activity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelPcb302Activity->ForeColor = System::Drawing::Color::White;
			this->labelPcb302Activity->Location = System::Drawing::Point(216, 65);
			this->labelPcb302Activity->Name = L"labelPcb302Activity";
			this->labelPcb302Activity->Size = System::Drawing::Size(86, 16);
			this->labelPcb302Activity->TabIndex = 3;
			this->labelPcb302Activity->Text = L"Connection ...";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(10, 65);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(203, 16);
			this->label5->TabIndex = 2;
			this->label5->Text = L"PCB 302: Compressor Board";
			// 
			// labelPcb301Activity
			// 
			this->labelPcb301Activity->AutoSize = true;
			this->labelPcb301Activity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelPcb301Activity->ForeColor = System::Drawing::Color::White;
			this->labelPcb301Activity->Location = System::Drawing::Point(216, 45);
			this->labelPcb301Activity->Name = L"labelPcb301Activity";
			this->labelPcb301Activity->Size = System::Drawing::Size(86, 16);
			this->labelPcb301Activity->TabIndex = 1;
			this->labelPcb301Activity->Text = L"Connection ...";
			// 
			// labelBoardInitialization
			// 
			this->labelBoardInitialization->AutoSize = true;
			this->labelBoardInitialization->Location = System::Drawing::Point(10, 45);
			this->labelBoardInitialization->Name = L"labelBoardInitialization";
			this->labelBoardInitialization->Size = System::Drawing::Size(172, 16);
			this->labelBoardInitialization->TabIndex = 0;
			this->labelBoardInitialization->Text = L"PCB 301: Service Board";
			// 
			// startupIcon
			// 
			this->startupIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->startupIcon->Location = System::Drawing::Point(50, 26);
			this->startupIcon->Name = L"startupIcon";
			this->startupIcon->Size = System::Drawing::Size(500, 185);
			this->startupIcon->TabIndex = 9;
			this->startupIcon->TabStop = false;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->ClientSize = System::Drawing::Size(600, 1024);
			this->ControlBox = false;
			this->Controls->Add(this->startupIcon);
			this->Controls->Add(this->infoPanel);
			this->DoubleBuffered = true;
			this->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"MainForm";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"MainForm";
			this->infoPanel->ResumeLayout(false);
			this->infoPanel->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->startupIcon))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	private: System::Void onStartupTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		SendMessageA(window, WM_USER + 1, 0, 0);
	}
	
};


}
