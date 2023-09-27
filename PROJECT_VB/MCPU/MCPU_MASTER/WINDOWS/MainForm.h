#pragma once

extern  double dtempo;


namespace CppCLRWinFormsProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	
	public:System::Timers::Timer^ startupTimer;
	public:HWND window;
	public:int startupFase;
	public:int startupSubFase;

	public:bool startupCompleted;
	public:bool startupError;

	private: System::Windows::Forms::RichTextBox^ StartupLogMessages;	
	private: System::Windows::Forms::Label^ label4;
	private: bool Startup_CanDriver(void);
	private: bool Startup_PCB301(void);
	private: bool Startup_PCB302(void);
	private: bool Startup_PCB303(void);
	private: bool Startup_PCB304(void);
	private: bool Startup_PCB315(void);
	private: bool Startup_MotorBody(void);
	private: bool Startup_MotorTilt(void);
	private: bool Startup_MotorArm(void);
	private: bool Startup_MotorShift(void);
	private: bool Startup_MotorVertical(void);
	private: bool Startup_Generator(void);
		   



	public:
		MainForm(void)
		{
			InitializeComponent();
			window = static_cast<HWND>(Handle.ToPointer());
			MainFormInitialize();

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: void MainFormInitialize(void);
	private: void StartupProcedure(void);


	private: System::Windows::Forms::GroupBox^ BoardInitialization;

	private: System::Windows::Forms::Label^ labelBoardInitialization;

	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::GroupBox^ groupBox1;


	private: System::Windows::Forms::GroupBox^ groupBoxPackage;
	private: System::Windows::Forms::Label^ labelFW301;


	private: System::Windows::Forms::Label^ labelGeneratorRevision;
	private: System::Windows::Forms::Label^ labelFW315;
	private: System::Windows::Forms::Label^ labelFW304;
	private: System::Windows::Forms::Label^ labelFW303;
	private: System::Windows::Forms::Label^ labelFW302;
	private: System::Windows::Forms::RichTextBox^ StartupErrorMessages;


	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ labelCanDriver;
	private: System::Windows::Forms::Label^ labelPcb301Activity;
	private: System::Windows::Forms::Label^ labelPcb303Activity;

	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ labelPcb302Activity;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ labelCanDriverActivity;

	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::Label^ labelPcb315Activity;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ labelPcb304Activity;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ labelGeneratorActivity;
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



	protected:

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
			this->BoardInitialization = (gcnew System::Windows::Forms::GroupBox());
			this->labelGeneratorActivity = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->labelCanDriverActivity = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->labelPcb315Activity = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->labelPcb304Activity = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->labelPcb303Activity = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->labelPcb302Activity = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->labelPcb301Activity = (gcnew System::Windows::Forms::Label());
			this->labelBoardInitialization = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->labelMotorUpDownActivity = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->labelMotorShiftActivity = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->labelMotorBodyActivity = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->labelMotorArmActivity = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->labelMotorTiltActivity = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->groupBoxPackage = (gcnew System::Windows::Forms::GroupBox());
			this->labelCanDriver = (gcnew System::Windows::Forms::Label());
			this->labelGeneratorRevision = (gcnew System::Windows::Forms::Label());
			this->labelFW315 = (gcnew System::Windows::Forms::Label());
			this->labelFW304 = (gcnew System::Windows::Forms::Label());
			this->labelFW303 = (gcnew System::Windows::Forms::Label());
			this->labelFW302 = (gcnew System::Windows::Forms::Label());
			this->labelFW301 = (gcnew System::Windows::Forms::Label());
			this->StartupErrorMessages = (gcnew System::Windows::Forms::RichTextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->StartupLogMessages = (gcnew System::Windows::Forms::RichTextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->BoardInitialization->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->groupBoxPackage->SuspendLayout();
			this->SuspendLayout();
			// 
			// BoardInitialization
			// 
			this->BoardInitialization->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->BoardInitialization->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->BoardInitialization->Controls->Add(this->labelGeneratorActivity);
			this->BoardInitialization->Controls->Add(this->label10);
			this->BoardInitialization->Controls->Add(this->labelCanDriverActivity);
			this->BoardInitialization->Controls->Add(this->label9);
			this->BoardInitialization->Controls->Add(this->labelPcb315Activity);
			this->BoardInitialization->Controls->Add(this->label8);
			this->BoardInitialization->Controls->Add(this->labelPcb304Activity);
			this->BoardInitialization->Controls->Add(this->label7);
			this->BoardInitialization->Controls->Add(this->labelPcb303Activity);
			this->BoardInitialization->Controls->Add(this->label6);
			this->BoardInitialization->Controls->Add(this->labelPcb302Activity);
			this->BoardInitialization->Controls->Add(this->label5);
			this->BoardInitialization->Controls->Add(this->labelPcb301Activity);
			this->BoardInitialization->Controls->Add(this->labelBoardInitialization);
			this->BoardInitialization->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Bold));
			this->BoardInitialization->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)),
				static_cast<System::Int32>(static_cast<System::Byte>(189)), static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->BoardInitialization->Location = System::Drawing::Point(26, 203);
			this->BoardInitialization->Name = L"BoardInitialization";
			this->BoardInitialization->Size = System::Drawing::Size(549, 217);
			this->BoardInitialization->TabIndex = 1;
			this->BoardInitialization->TabStop = false;
			this->BoardInitialization->Text = L"BOARD INITIALIZATION";
			// 
			// labelGeneratorActivity
			// 
			this->labelGeneratorActivity->AutoSize = true;
			this->labelGeneratorActivity->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelGeneratorActivity->ForeColor = System::Drawing::Color::White;
			this->labelGeneratorActivity->Location = System::Drawing::Point(216, 180);
			this->labelGeneratorActivity->Name = L"labelGeneratorActivity";
			this->labelGeneratorActivity->Size = System::Drawing::Size(89, 18);
			this->labelGeneratorActivity->TabIndex = 13;
			this->labelGeneratorActivity->Text = L"Connection ...";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(10, 180);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(143, 18);
			this->label10->TabIndex = 12;
			this->label10->Text = L"GENERATOR Board";
			// 
			// labelCanDriverActivity
			// 
			this->labelCanDriverActivity->AutoSize = true;
			this->labelCanDriverActivity->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelCanDriverActivity->ForeColor = System::Drawing::Color::White;
			this->labelCanDriverActivity->Location = System::Drawing::Point(216, 155);
			this->labelCanDriverActivity->Name = L"labelCanDriverActivity";
			this->labelCanDriverActivity->Size = System::Drawing::Size(89, 18);
			this->labelCanDriverActivity->TabIndex = 11;
			this->labelCanDriverActivity->Text = L"Connection ...";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(10, 155);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(141, 18);
			this->label9->TabIndex = 10;
			this->label9->Text = L"CAN DRIVER Board";
			// 
			// labelPcb315Activity
			// 
			this->labelPcb315Activity->AutoSize = true;
			this->labelPcb315Activity->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelPcb315Activity->ForeColor = System::Drawing::Color::White;
			this->labelPcb315Activity->Location = System::Drawing::Point(216, 130);
			this->labelPcb315Activity->Name = L"labelPcb315Activity";
			this->labelPcb315Activity->Size = System::Drawing::Size(89, 18);
			this->labelPcb315Activity->TabIndex = 9;
			this->labelPcb315Activity->Text = L"Connection ...";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(10, 130);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(156, 18);
			this->label8->TabIndex = 8;
			this->label8->Text = L"PCB 315: Filter Board";
			// 
			// labelPcb304Activity
			// 
			this->labelPcb304Activity->AutoSize = true;
			this->labelPcb304Activity->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelPcb304Activity->ForeColor = System::Drawing::Color::White;
			this->labelPcb304Activity->Location = System::Drawing::Point(216, 105);
			this->labelPcb304Activity->Name = L"labelPcb304Activity";
			this->labelPcb304Activity->Size = System::Drawing::Size(89, 18);
			this->labelPcb304Activity->TabIndex = 7;
			this->labelPcb304Activity->Text = L"Connection ...";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(10, 105);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(162, 18);
			this->label7->TabIndex = 6;
			this->label7->Text = L"PCB 304: Potter Board";
			// 
			// labelPcb303Activity
			// 
			this->labelPcb303Activity->AutoSize = true;
			this->labelPcb303Activity->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelPcb303Activity->ForeColor = System::Drawing::Color::White;
			this->labelPcb303Activity->Location = System::Drawing::Point(216, 80);
			this->labelPcb303Activity->Name = L"labelPcb303Activity";
			this->labelPcb303Activity->Size = System::Drawing::Size(89, 18);
			this->labelPcb303Activity->TabIndex = 5;
			this->labelPcb303Activity->Text = L"Connection ...";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(10, 80);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(189, 18);
			this->label6->TabIndex = 4;
			this->label6->Text = L"PCB 303: Collimator Board";
			// 
			// labelPcb302Activity
			// 
			this->labelPcb302Activity->AutoSize = true;
			this->labelPcb302Activity->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelPcb302Activity->ForeColor = System::Drawing::Color::White;
			this->labelPcb302Activity->Location = System::Drawing::Point(216, 55);
			this->labelPcb302Activity->Name = L"labelPcb302Activity";
			this->labelPcb302Activity->Size = System::Drawing::Size(89, 18);
			this->labelPcb302Activity->TabIndex = 3;
			this->labelPcb302Activity->Text = L"Connection ...";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(10, 55);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(201, 18);
			this->label5->TabIndex = 2;
			this->label5->Text = L"PCB 302: Compressor Board";
			// 
			// labelPcb301Activity
			// 
			this->labelPcb301Activity->AutoSize = true;
			this->labelPcb301Activity->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelPcb301Activity->ForeColor = System::Drawing::Color::White;
			this->labelPcb301Activity->Location = System::Drawing::Point(216, 30);
			this->labelPcb301Activity->Name = L"labelPcb301Activity";
			this->labelPcb301Activity->Size = System::Drawing::Size(89, 18);
			this->labelPcb301Activity->TabIndex = 1;
			this->labelPcb301Activity->Text = L"Connection ...";
			// 
			// labelBoardInitialization
			// 
			this->labelBoardInitialization->AutoSize = true;
			this->labelBoardInitialization->Location = System::Drawing::Point(10, 30);
			this->labelBoardInitialization->Name = L"labelBoardInitialization";
			this->labelBoardInitialization->Size = System::Drawing::Size(172, 18);
			this->labelBoardInitialization->TabIndex = 0;
			this->labelBoardInitialization->Text = L"PCB 301: Service Board";
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 20.25F, System::Drawing::FontStyle::Bold));
			this->label2->Location = System::Drawing::Point(178, 17);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(216, 36);
			this->label2->TabIndex = 2;
			this->label2->Text = L"System Startup";
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->groupBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->groupBox1->Controls->Add(this->labelMotorUpDownActivity);
			this->groupBox1->Controls->Add(this->label15);
			this->groupBox1->Controls->Add(this->labelMotorShiftActivity);
			this->groupBox1->Controls->Add(this->label14);
			this->groupBox1->Controls->Add(this->labelMotorBodyActivity);
			this->groupBox1->Controls->Add(this->label13);
			this->groupBox1->Controls->Add(this->labelMotorArmActivity);
			this->groupBox1->Controls->Add(this->label12);
			this->groupBox1->Controls->Add(this->labelMotorTiltActivity);
			this->groupBox1->Controls->Add(this->label11);
			this->groupBox1->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Bold));
			this->groupBox1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->groupBox1->Location = System::Drawing::Point(26, 426);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(549, 162);
			this->groupBox1->TabIndex = 3;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"MOTORS INITIALIZATION";
			// 
			// labelMotorUpDownActivity
			// 
			this->labelMotorUpDownActivity->AutoSize = true;
			this->labelMotorUpDownActivity->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelMotorUpDownActivity->ForeColor = System::Drawing::Color::White;
			this->labelMotorUpDownActivity->Location = System::Drawing::Point(216, 130);
			this->labelMotorUpDownActivity->Name = L"labelMotorUpDownActivity";
			this->labelMotorUpDownActivity->Size = System::Drawing::Size(89, 18);
			this->labelMotorUpDownActivity->TabIndex = 11;
			this->labelMotorUpDownActivity->Text = L"Connection ...";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(10, 130);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(137, 18);
			this->label15->TabIndex = 10;
			this->label15->Text = L"VERTICAL MOTOR";
			// 
			// labelMotorShiftActivity
			// 
			this->labelMotorShiftActivity->AutoSize = true;
			this->labelMotorShiftActivity->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelMotorShiftActivity->ForeColor = System::Drawing::Color::White;
			this->labelMotorShiftActivity->Location = System::Drawing::Point(216, 105);
			this->labelMotorShiftActivity->Name = L"labelMotorShiftActivity";
			this->labelMotorShiftActivity->Size = System::Drawing::Size(89, 18);
			this->labelMotorShiftActivity->TabIndex = 9;
			this->labelMotorShiftActivity->Text = L"Connection ...";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(10, 105);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(108, 18);
			this->label14->TabIndex = 8;
			this->label14->Text = L"SHIFT MOTOR";
			// 
			// labelMotorBodyActivity
			// 
			this->labelMotorBodyActivity->AutoSize = true;
			this->labelMotorBodyActivity->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelMotorBodyActivity->ForeColor = System::Drawing::Color::White;
			this->labelMotorBodyActivity->Location = System::Drawing::Point(216, 80);
			this->labelMotorBodyActivity->Name = L"labelMotorBodyActivity";
			this->labelMotorBodyActivity->Size = System::Drawing::Size(89, 18);
			this->labelMotorBodyActivity->TabIndex = 7;
			this->labelMotorBodyActivity->Text = L"Connection ...";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(10, 80);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(107, 18);
			this->label13->TabIndex = 6;
			this->label13->Text = L"BODY MOTOR";
			// 
			// labelMotorArmActivity
			// 
			this->labelMotorArmActivity->AutoSize = true;
			this->labelMotorArmActivity->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelMotorArmActivity->ForeColor = System::Drawing::Color::White;
			this->labelMotorArmActivity->Location = System::Drawing::Point(216, 55);
			this->labelMotorArmActivity->Name = L"labelMotorArmActivity";
			this->labelMotorArmActivity->Size = System::Drawing::Size(89, 18);
			this->labelMotorArmActivity->TabIndex = 5;
			this->labelMotorArmActivity->Text = L"Connection ...";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(10, 55);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(98, 18);
			this->label12->TabIndex = 4;
			this->label12->Text = L"ARM MOTOR";
			// 
			// labelMotorTiltActivity
			// 
			this->labelMotorTiltActivity->AutoSize = true;
			this->labelMotorTiltActivity->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Italic));
			this->labelMotorTiltActivity->ForeColor = System::Drawing::Color::White;
			this->labelMotorTiltActivity->Location = System::Drawing::Point(216, 30);
			this->labelMotorTiltActivity->Name = L"labelMotorTiltActivity";
			this->labelMotorTiltActivity->Size = System::Drawing::Size(89, 18);
			this->labelMotorTiltActivity->TabIndex = 3;
			this->labelMotorTiltActivity->Text = L"Connection ...";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(10, 30);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(96, 18);
			this->label11->TabIndex = 2;
			this->label11->Text = L"TILT MOTOR";
			// 
			// groupBoxPackage
			// 
			this->groupBoxPackage->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->groupBoxPackage->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->groupBoxPackage->Controls->Add(this->labelCanDriver);
			this->groupBoxPackage->Controls->Add(this->labelGeneratorRevision);
			this->groupBoxPackage->Controls->Add(this->labelFW315);
			this->groupBoxPackage->Controls->Add(this->labelFW304);
			this->groupBoxPackage->Controls->Add(this->labelFW303);
			this->groupBoxPackage->Controls->Add(this->labelFW302);
			this->groupBoxPackage->Controls->Add(this->labelFW301);
			this->groupBoxPackage->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Bold));
			this->groupBoxPackage->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->groupBoxPackage->Location = System::Drawing::Point(26, 65);
			this->groupBoxPackage->Name = L"groupBoxPackage";
			this->groupBoxPackage->Size = System::Drawing::Size(549, 132);
			this->groupBoxPackage->TabIndex = 4;
			this->groupBoxPackage->TabStop = false;
			this->groupBoxPackage->Text = L"PACKAGE REVISION: ID01";
			// 
			// labelCanDriver
			// 
			this->labelCanDriver->AutoSize = true;
			this->labelCanDriver->ForeColor = System::Drawing::Color::White;
			this->labelCanDriver->Location = System::Drawing::Point(265, 30);
			this->labelCanDriver->Name = L"labelCanDriver";
			this->labelCanDriver->Size = System::Drawing::Size(176, 18);
			this->labelCanDriver->TabIndex = 6;
			this->labelCanDriver->Text = L"CAN DRIVER REVISION:";
			// 
			// labelGeneratorRevision
			// 
			this->labelGeneratorRevision->AutoSize = true;
			this->labelGeneratorRevision->ForeColor = System::Drawing::Color::White;
			this->labelGeneratorRevision->Location = System::Drawing::Point(265, 48);
			this->labelGeneratorRevision->Name = L"labelGeneratorRevision";
			this->labelGeneratorRevision->Size = System::Drawing::Size(178, 18);
			this->labelGeneratorRevision->TabIndex = 5;
			this->labelGeneratorRevision->Text = L"GENERATOR REVISION:";
			// 
			// labelFW315
			// 
			this->labelFW315->AutoSize = true;
			this->labelFW315->ForeColor = System::Drawing::Color::White;
			this->labelFW315->Location = System::Drawing::Point(20, 102);
			this->labelFW315->Name = L"labelFW315";
			this->labelFW315->Size = System::Drawing::Size(138, 18);
			this->labelFW315->TabIndex = 4;
			this->labelFW315->Text = L"FW 315 REVISION:";
			// 
			// labelFW304
			// 
			this->labelFW304->AutoSize = true;
			this->labelFW304->ForeColor = System::Drawing::Color::White;
			this->labelFW304->Location = System::Drawing::Point(20, 84);
			this->labelFW304->Name = L"labelFW304";
			this->labelFW304->Size = System::Drawing::Size(138, 18);
			this->labelFW304->TabIndex = 3;
			this->labelFW304->Text = L"FW 304 REVISION:";
			// 
			// labelFW303
			// 
			this->labelFW303->AutoSize = true;
			this->labelFW303->ForeColor = System::Drawing::Color::White;
			this->labelFW303->Location = System::Drawing::Point(20, 66);
			this->labelFW303->Name = L"labelFW303";
			this->labelFW303->Size = System::Drawing::Size(138, 18);
			this->labelFW303->TabIndex = 2;
			this->labelFW303->Text = L"FW 303 REVISION:";
			// 
			// labelFW302
			// 
			this->labelFW302->AutoSize = true;
			this->labelFW302->ForeColor = System::Drawing::Color::White;
			this->labelFW302->Location = System::Drawing::Point(20, 48);
			this->labelFW302->Name = L"labelFW302";
			this->labelFW302->Size = System::Drawing::Size(138, 18);
			this->labelFW302->TabIndex = 1;
			this->labelFW302->Text = L"FW 302 REVISION:";
			// 
			// labelFW301
			// 
			this->labelFW301->AutoSize = true;
			this->labelFW301->ForeColor = System::Drawing::Color::White;
			this->labelFW301->Location = System::Drawing::Point(20, 30);
			this->labelFW301->Name = L"labelFW301";
			this->labelFW301->Size = System::Drawing::Size(138, 18);
			this->labelFW301->TabIndex = 0;
			this->labelFW301->Text = L"FW 301 REVISION:";
			// 
			// StartupErrorMessages
			// 
			this->StartupErrorMessages->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->StartupErrorMessages->BulletIndent = 1;
			this->StartupErrorMessages->Cursor = System::Windows::Forms::Cursors::Default;
			this->StartupErrorMessages->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F));
			this->StartupErrorMessages->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->StartupErrorMessages->Location = System::Drawing::Point(26, 833);
			this->StartupErrorMessages->Name = L"StartupErrorMessages";
			this->StartupErrorMessages->ReadOnly = true;
			this->StartupErrorMessages->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
			this->StartupErrorMessages->ShortcutsEnabled = false;
			this->StartupErrorMessages->ShowSelectionMargin = true;
			this->StartupErrorMessages->Size = System::Drawing::Size(549, 179);
			this->StartupErrorMessages->TabIndex = 5;
			this->StartupErrorMessages->Text = L"Pippo";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Bold));
			this->label3->Location = System::Drawing::Point(23, 812);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(147, 18);
			this->label3->TabIndex = 6;
			this->label3->Text = L"ERROR MESSAGES";
			// 
			// StartupLogMessages
			// 
			this->StartupLogMessages->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->StartupLogMessages->BulletIndent = 1;
			this->StartupLogMessages->Cursor = System::Windows::Forms::Cursors::Default;
			this->StartupLogMessages->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F));
			this->StartupLogMessages->ForeColor = System::Drawing::Color::White;
			this->StartupLogMessages->Location = System::Drawing::Point(26, 627);
			this->StartupLogMessages->Name = L"StartupLogMessages";
			this->StartupLogMessages->ReadOnly = true;
			this->StartupLogMessages->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
			this->StartupLogMessages->ShortcutsEnabled = false;
			this->StartupLogMessages->ShowSelectionMargin = true;
			this->StartupLogMessages->Size = System::Drawing::Size(549, 174);
			this->StartupLogMessages->TabIndex = 7;
			this->StartupLogMessages->Text = L"Pippo";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Arial Unicode MS", 9.75F, System::Drawing::FontStyle::Bold));
			this->label4->Location = System::Drawing::Point(23, 606);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(126, 18);
			this->label4->TabIndex = 8;
			this->label4->Text = L"LOG MESSAGES";
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
			this->Controls->Add(this->label4);
			this->Controls->Add(this->StartupLogMessages);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->StartupErrorMessages);
			this->Controls->Add(this->groupBoxPackage);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->BoardInitialization);
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
			this->BoardInitialization->ResumeLayout(false);
			this->BoardInitialization->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBoxPackage->ResumeLayout(false);
			this->groupBoxPackage->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	private: System::Void onStartupTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		SendNotifyMessageA(window, WM_USER + 1, 0, 0);
	}
	

};


}
