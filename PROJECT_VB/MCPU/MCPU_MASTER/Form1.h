#pragma once

namespace CppCLRWinFormsProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace GantryStatusRegisters;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{

	public:
		HWND window;
		void InitSimulatorRegisters(void);
		void InitFormItems(void);
		void closeStudyRegisters(void);
		void openStudyRegisters(void);

		Form1(void)
		{
			InitializeComponent();
			window = static_cast<HWND>(Handle.ToPointer());

			//
			//TODO: Add the constructor code here
			//
			
			InitSimulatorRegisters();
			InitFormItems();
			closeStudyRegisters();

			
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ textForce;
	protected:

	private: System::Windows::Forms::Label^ labelCmpForce;
	private: System::Windows::Forms::TextBox^ textThickness;

	private: System::Windows::Forms::Label^ lableCmpThick;


	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::ComboBox^ comboPaddle;
	private: System::Windows::Forms::ComboBox^ comboComponents;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::ComboBox^ comboColliComponent;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ labelArmAngle;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::Label^ labelArmProjection;

	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ labelArmHigh;

	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ labelArmLow;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ labelArmTarget;




	protected:


	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ labelTrxTarget;
	private: System::Windows::Forms::Label^ label17;
	private: System::Windows::Forms::Label^ labelTrxAngle;
		   System::Timers::Timer^ armTimer;

	private: System::Windows::Forms::Label^ label10;
private: System::Windows::Forms::ComboBox^ comboProjections;
private: System::Windows::Forms::Button^ buttonAbortProjection;
private: System::Windows::Forms::GroupBox^ groupBox3;
private: System::Windows::Forms::GroupBox^ groupBox4;
private: System::Windows::Forms::GroupBox^ groupBox5;
private: System::Windows::Forms::GroupBox^ groupBox6;
private: System::Windows::Forms::Label^ labelPatientName;
private: System::Windows::Forms::Label^ labelGantryStatus;
private: System::Windows::Forms::Label^ label11;
private: System::Windows::Forms::Label^ label12;
private: System::Windows::Forms::GroupBox^ groupBox7;
private: System::Windows::Forms::Label^ label14;

private: System::Windows::Forms::Label^ label13;

private: System::Windows::Forms::Label^ label15;
private: System::Windows::Forms::NumericUpDown^ numericAnode;
private: System::Windows::Forms::NumericUpDown^ numericStator;
private: System::Windows::Forms::NumericUpDown^ numericBulb;
private: System::Windows::Forms::CheckBox^ checkBoxArmValid;
private: System::Windows::Forms::GroupBox^ groupBox8;
private: System::Windows::Forms::CheckBox^ checkBoxPushButton;
private: System::Windows::Forms::Button^ buttonPushButton;
private: System::Windows::Forms::Label^ label19;
private: System::Windows::Forms::Label^ labelTomoConfId;
private: System::Windows::Forms::Label^ labelCmpMode;
private: System::Windows::Forms::Label^ labelCompressionMode;



private: System::Windows::Forms::CheckBox^ checkBoxUseProtection;
private: System::Windows::Forms::CheckBox^ checkBoxUseArm;
private: System::Windows::Forms::Label^ label18;
private: System::Windows::Forms::Label^ labelExposureType;
private: System::Windows::Forms::Label^ label20;
private: System::Windows::Forms::Label^ labelTomoSeq;
private: System::Windows::Forms::Label^ label21;
private: System::Windows::Forms::NumericUpDown^ numericManualArm;
private: System::Windows::Forms::GroupBox^ groupBox9;

private: System::Windows::Forms::Label^ label22;
private: System::Windows::Forms::NumericUpDown^ numericManualTrx;
private: System::Windows::Forms::GroupBox^ groupBoxReady;

private: System::Windows::Forms::CheckBox^ checkBox_0x2;
private: System::Windows::Forms::CheckBox^ checkBox_0x1;



private: System::Windows::Forms::CheckBox^ checkBox_0x80;

private: System::Windows::Forms::CheckBox^ checkBox_0x40;
private: System::Windows::Forms::CheckBox^ checkBox_0x20;
private: System::Windows::Forms::CheckBox^ checkBox_0x10;
private: System::Windows::Forms::CheckBox^ checkBox_0x8;
private: System::Windows::Forms::CheckBox^ checkBox_0x4;




		   System::Timers::Timer^ trxTimer;
private: System::Windows::Forms::GroupBox^ groupBoxPulse0;

private: System::Windows::Forms::Label^ label16;
private: System::Windows::Forms::Label^ label_kV0;

private: System::Windows::Forms::Label^ label24;
private: System::Windows::Forms::Label^ label_mAs0;

private: System::Windows::Forms::Label^ label27;
private: System::Windows::Forms::Label^ label_filter0;
private: System::Windows::Forms::CheckBox^ checkBox_valid0;
private: System::Windows::Forms::GroupBox^ groupBox10;
private: System::Windows::Forms::CheckBox^ checkBox_valid1;

private: System::Windows::Forms::Label^ label23;
private: System::Windows::Forms::Label^ label_filter1;

private: System::Windows::Forms::Label^ label26;
private: System::Windows::Forms::Label^ label_kV1;
private: System::Windows::Forms::Label^ label29;
private: System::Windows::Forms::Label^ label_mAs1;

private: System::Windows::Forms::GroupBox^ groupBox11;
private: System::Windows::Forms::CheckBox^ checkBox_valid2;


private: System::Windows::Forms::Label^ label31;
private: System::Windows::Forms::Label^ label_fiter2;

private: System::Windows::Forms::Label^ label33;
private: System::Windows::Forms::Label^ label_kV2;

private: System::Windows::Forms::Label^ label35;
private: System::Windows::Forms::Label^ label_mAs2;

private: System::Windows::Forms::GroupBox^ groupBox12;
private: System::Windows::Forms::CheckBox^ checkBox_valid3;

private: System::Windows::Forms::Label^ label37;
private: System::Windows::Forms::Label^ label_filter3;

private: System::Windows::Forms::Label^ label39;
private: System::Windows::Forms::Label^ label_kV3;

private: System::Windows::Forms::Label^ label41;
private: System::Windows::Forms::Label^ label_mAs3;


	   System::Timers::Timer^ readyTimer;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textForce = (gcnew System::Windows::Forms::TextBox());
			this->labelCmpForce = (gcnew System::Windows::Forms::Label());
			this->textThickness = (gcnew System::Windows::Forms::TextBox());
			this->lableCmpThick = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->comboPaddle = (gcnew System::Windows::Forms::ComboBox());
			this->comboComponents = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboColliComponent = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->labelArmAngle = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBoxArmValid = (gcnew System::Windows::Forms::CheckBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->labelArmTarget = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->labelArmProjection = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->labelArmHigh = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->labelArmLow = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->labelTrxTarget = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->labelTrxAngle = (gcnew System::Windows::Forms::Label());
			this->comboProjections = (gcnew System::Windows::Forms::ComboBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->buttonAbortProjection = (gcnew System::Windows::Forms::Button());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->labelPatientName = (gcnew System::Windows::Forms::Label());
			this->labelGantryStatus = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->groupBox7 = (gcnew System::Windows::Forms::GroupBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->numericStator = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericBulb = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericAnode = (gcnew System::Windows::Forms::NumericUpDown());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->groupBox8 = (gcnew System::Windows::Forms::GroupBox());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->labelTomoSeq = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->labelTomoConfId = (gcnew System::Windows::Forms::Label());
			this->labelCmpMode = (gcnew System::Windows::Forms::Label());
			this->labelCompressionMode = (gcnew System::Windows::Forms::Label());
			this->checkBoxUseProtection = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxUseArm = (gcnew System::Windows::Forms::CheckBox());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->labelExposureType = (gcnew System::Windows::Forms::Label());
			this->checkBoxPushButton = (gcnew System::Windows::Forms::CheckBox());
			this->buttonPushButton = (gcnew System::Windows::Forms::Button());
			this->numericManualArm = (gcnew System::Windows::Forms::NumericUpDown());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->groupBox9 = (gcnew System::Windows::Forms::GroupBox());
			this->numericManualTrx = (gcnew System::Windows::Forms::NumericUpDown());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->groupBoxReady = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox_0x80 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_0x40 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_0x20 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_0x10 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_0x8 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_0x4 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_0x2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_0x1 = (gcnew System::Windows::Forms::CheckBox());
			this->groupBoxPulse0 = (gcnew System::Windows::Forms::GroupBox());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label_kV0 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->label_mAs0 = (gcnew System::Windows::Forms::Label());
			this->label_filter0 = (gcnew System::Windows::Forms::Label());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->checkBox_valid0 = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox10 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox_valid1 = (gcnew System::Windows::Forms::CheckBox());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->label_filter1 = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->label_kV1 = (gcnew System::Windows::Forms::Label());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->label_mAs1 = (gcnew System::Windows::Forms::Label());
			this->groupBox11 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox_valid2 = (gcnew System::Windows::Forms::CheckBox());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->label_fiter2 = (gcnew System::Windows::Forms::Label());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->label_kV2 = (gcnew System::Windows::Forms::Label());
			this->label35 = (gcnew System::Windows::Forms::Label());
			this->label_mAs2 = (gcnew System::Windows::Forms::Label());
			this->groupBox12 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox_valid3 = (gcnew System::Windows::Forms::CheckBox());
			this->label37 = (gcnew System::Windows::Forms::Label());
			this->label_filter3 = (gcnew System::Windows::Forms::Label());
			this->label39 = (gcnew System::Windows::Forms::Label());
			this->label_kV3 = (gcnew System::Windows::Forms::Label());
			this->label41 = (gcnew System::Windows::Forms::Label());
			this->label_mAs3 = (gcnew System::Windows::Forms::Label());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->groupBox6->SuspendLayout();
			this->groupBox7->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericStator))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericBulb))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericAnode))->BeginInit();
			this->groupBox8->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericManualArm))->BeginInit();
			this->groupBox9->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericManualTrx))->BeginInit();
			this->groupBoxReady->SuspendLayout();
			this->groupBoxPulse0->SuspendLayout();
			this->groupBox10->SuspendLayout();
			this->groupBox11->SuspendLayout();
			this->groupBox12->SuspendLayout();
			this->SuspendLayout();
			// 
			// textForce
			// 
			this->textForce->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->textForce->Location = System::Drawing::Point(14, 47);
			this->textForce->Name = L"textForce";
			this->textForce->Size = System::Drawing::Size(81, 22);
			this->textForce->TabIndex = 0;
			this->textForce->Text = L"0";
			this->textForce->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textForce_KeyPress);
			// 
			// labelCmpForce
			// 
			this->labelCmpForce->AutoSize = true;
			this->labelCmpForce->Location = System::Drawing::Point(13, 31);
			this->labelCmpForce->Name = L"labelCmpForce";
			this->labelCmpForce->Size = System::Drawing::Size(51, 13);
			this->labelCmpForce->TabIndex = 1;
			this->labelCmpForce->Text = L"Force (N)";
			// 
			// textThickness
			// 
			this->textThickness->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->textThickness->Location = System::Drawing::Point(109, 47);
			this->textThickness->Name = L"textThickness";
			this->textThickness->Size = System::Drawing::Size(78, 22);
			this->textThickness->TabIndex = 2;
			this->textThickness->Text = L"0";
			this->textThickness->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textThickness_KeyPress);
			// 
			// lableCmpThick
			// 
			this->lableCmpThick->AutoSize = true;
			this->lableCmpThick->Location = System::Drawing::Point(106, 31);
			this->lableCmpThick->Name = L"lableCmpThick";
			this->lableCmpThick->Size = System::Drawing::Size(81, 13);
			this->lableCmpThick->TabIndex = 3;
			this->lableCmpThick->Text = L"Thickness (mm)";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(212, 31);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(98, 13);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Compressor Paddle";
			// 
			// comboPaddle
			// 
			this->comboPaddle->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->comboPaddle->FormattingEnabled = true;
			this->comboPaddle->Items->AddRange(gcnew cli::array< System::Object^  >(13) {
				L"PAD-24x30", L"PAD-18x24_C", L"PAD-18x24_L",
					L"PAD-18x24_R", L"PAD-9x21", L"PAD-10x24", L"PAD-D75", L"PAD-BP2D", L"PAD-BP3D", L"PAD-TOMO", L"PAD-9x9", L"PAD-UNDETECTED",
					L"PAD-UNLOCKED"
			});
			this->comboPaddle->Location = System::Drawing::Point(203, 47);
			this->comboPaddle->Name = L"comboPaddle";
			this->comboPaddle->Size = System::Drawing::Size(188, 24);
			this->comboPaddle->TabIndex = 6;
			this->comboPaddle->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::listPaddle_SelectedIndexChanged);
			// 
			// comboComponents
			// 
			this->comboComponents->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->comboComponents->FormattingEnabled = true;
			this->comboComponents->Items->AddRange(gcnew cli::array< System::Object^  >(7) {
				L"PROTECTION_3D", L"MAG15", L"MAG18", L"MAG20",
					L"BIOPSY", L"UNDEFINED", L" "
			});
			this->comboComponents->Location = System::Drawing::Point(13, 49);
			this->comboComponents->Name = L"comboComponents";
			this->comboComponents->Size = System::Drawing::Size(188, 24);
			this->comboComponents->TabIndex = 7;
			this->comboComponents->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboComponents_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(21, 33);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(93, 13);
			this->label2->TabIndex = 8;
			this->label2->Text = L"Body Components";
			// 
			// comboColliComponent
			// 
			this->comboColliComponent->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->comboColliComponent->FormattingEnabled = true;
			this->comboColliComponent->Items->AddRange(gcnew cli::array< System::Object^  >(5) {
				L"PROTECTION_2D", L"PROTECTION_SHIFTED",
					L"LEAD_SCREEN", L"SPECIMEN", L"UNDEFINED"
			});
			this->comboColliComponent->Location = System::Drawing::Point(215, 50);
			this->comboColliComponent->Name = L"comboColliComponent";
			this->comboColliComponent->Size = System::Drawing::Size(176, 24);
			this->comboColliComponent->TabIndex = 9;
			this->comboColliComponent->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboColliComponent_SelectedIndexChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(240, 33);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(88, 13);
			this->label3->TabIndex = 10;
			this->label3->Text = L"Colli Components";
			// 
			// labelArmAngle
			// 
			this->labelArmAngle->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelArmAngle->Location = System::Drawing::Point(16, 135);
			this->labelArmAngle->Name = L"labelArmAngle";
			this->labelArmAngle->Size = System::Drawing::Size(69, 23);
			this->labelArmAngle->TabIndex = 11;
			this->labelArmAngle->Text = L"0 ";
			this->labelArmAngle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->checkBoxArmValid);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->labelArmTarget);
			this->groupBox1->Controls->Add(this->label9);
			this->groupBox1->Controls->Add(this->labelArmProjection);
			this->groupBox1->Controls->Add(this->label7);
			this->groupBox1->Controls->Add(this->labelArmHigh);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->labelArmLow);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->labelArmAngle);
			this->groupBox1->Location = System::Drawing::Point(452, 118);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(193, 179);
			this->groupBox1->TabIndex = 12;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"ARM DATA";
			// 
			// checkBoxArmValid
			// 
			this->checkBoxArmValid->AutoSize = true;
			this->checkBoxArmValid->Location = System::Drawing::Point(108, 139);
			this->checkBoxArmValid->Name = L"checkBoxArmValid";
			this->checkBoxArmValid->Size = System::Drawing::Size(49, 17);
			this->checkBoxArmValid->TabIndex = 21;
			this->checkBoxArmValid->Text = L"Valid";
			this->checkBoxArmValid->UseVisualStyleBackColor = true;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(19, 28);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(51, 13);
			this->label6->TabIndex = 20;
			this->label6->Text = L"Target (°)";
			// 
			// labelArmTarget
			// 
			this->labelArmTarget->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelArmTarget->Location = System::Drawing::Point(16, 41);
			this->labelArmTarget->Name = L"labelArmTarget";
			this->labelArmTarget->Size = System::Drawing::Size(69, 23);
			this->labelArmTarget->TabIndex = 19;
			this->labelArmTarget->Text = L"0 ";
			this->labelArmTarget->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(100, 28);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(53, 13);
			this->label9->TabIndex = 18;
			this->label9->Text = L"projection";
			// 
			// labelArmProjection
			// 
			this->labelArmProjection->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelArmProjection->Location = System::Drawing::Point(97, 41);
			this->labelArmProjection->Name = L"labelArmProjection";
			this->labelArmProjection->Size = System::Drawing::Size(79, 23);
			this->labelArmProjection->TabIndex = 17;
			this->labelArmProjection->Text = L"UNDEFINED";
			this->labelArmProjection->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(104, 75);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(42, 13);
			this->label7->TabIndex = 16;
			this->label7->Text = L"High (°)";
			// 
			// labelArmHigh
			// 
			this->labelArmHigh->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelArmHigh->Location = System::Drawing::Point(103, 88);
			this->labelArmHigh->Name = L"labelArmHigh";
			this->labelArmHigh->Size = System::Drawing::Size(69, 23);
			this->labelArmHigh->TabIndex = 15;
			this->labelArmHigh->Text = L"0 ";
			this->labelArmHigh->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(19, 75);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(40, 13);
			this->label4->TabIndex = 14;
			this->label4->Text = L"Low (°)";
			// 
			// labelArmLow
			// 
			this->labelArmLow->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelArmLow->Location = System::Drawing::Point(16, 88);
			this->labelArmLow->Name = L"labelArmLow";
			this->labelArmLow->Size = System::Drawing::Size(69, 23);
			this->labelArmLow->TabIndex = 13;
			this->labelArmLow->Text = L"0 ";
			this->labelArmLow->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(19, 122);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(47, 13);
			this->label5->TabIndex = 12;
			this->label5->Text = L"Angle (°)";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->label8);
			this->groupBox2->Controls->Add(this->labelTrxTarget);
			this->groupBox2->Controls->Add(this->label17);
			this->groupBox2->Controls->Add(this->labelTrxAngle);
			this->groupBox2->Location = System::Drawing::Point(452, 314);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(193, 82);
			this->groupBox2->TabIndex = 21;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"TRX DATA";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(19, 28);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(38, 13);
			this->label8->TabIndex = 20;
			this->label8->Text = L"Target";
			// 
			// labelTrxTarget
			// 
			this->labelTrxTarget->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelTrxTarget->Location = System::Drawing::Point(16, 41);
			this->labelTrxTarget->Name = L"labelTrxTarget";
			this->labelTrxTarget->Size = System::Drawing::Size(69, 23);
			this->labelTrxTarget->TabIndex = 19;
			this->labelTrxTarget->Text = L"SCOUT";
			this->labelTrxTarget->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(110, 28);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(47, 13);
			this->label17->TabIndex = 12;
			this->label17->Text = L"Angle (°)";
			// 
			// labelTrxAngle
			// 
			this->labelTrxAngle->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelTrxAngle->Location = System::Drawing::Point(107, 41);
			this->labelTrxAngle->Name = L"labelTrxAngle";
			this->labelTrxAngle->Size = System::Drawing::Size(69, 23);
			this->labelTrxAngle->TabIndex = 11;
			this->labelTrxAngle->Text = L"0 ";
			this->labelTrxAngle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// comboProjections
			// 
			this->comboProjections->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->comboProjections->FormattingEnabled = true;
			this->comboProjections->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"UNDEFINED" });
			this->comboProjections->Location = System::Drawing::Point(10, 47);
			this->comboProjections->Name = L"comboProjections";
			this->comboProjections->Size = System::Drawing::Size(191, 24);
			this->comboProjections->TabIndex = 22;
			this->comboProjections->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboProjections_SelectedIndexChanged);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(13, 31);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(112, 13);
			this->label10->TabIndex = 23;
			this->label10->Text = L"User Select Projection";
			// 
			// buttonAbortProjection
			// 
			this->buttonAbortProjection->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->buttonAbortProjection->Location = System::Drawing::Point(218, 39);
			this->buttonAbortProjection->Name = L"buttonAbortProjection";
			this->buttonAbortProjection->Size = System::Drawing::Size(173, 39);
			this->buttonAbortProjection->TabIndex = 24;
			this->buttonAbortProjection->Text = L"ABORT PROJECTION";
			this->buttonAbortProjection->UseVisualStyleBackColor = true;
			this->buttonAbortProjection->Click += gcnew System::EventHandler(this, &Form1::buttonAbortProjection_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->textForce);
			this->groupBox3->Controls->Add(this->labelCmpForce);
			this->groupBox3->Controls->Add(this->lableCmpThick);
			this->groupBox3->Controls->Add(this->textThickness);
			this->groupBox3->Controls->Add(this->label1);
			this->groupBox3->Controls->Add(this->comboPaddle);
			this->groupBox3->Location = System::Drawing::Point(49, 109);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(397, 91);
			this->groupBox3->TabIndex = 22;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"COMPRESSOR DATA";
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->label2);
			this->groupBox4->Controls->Add(this->comboComponents);
			this->groupBox4->Controls->Add(this->label3);
			this->groupBox4->Controls->Add(this->comboColliComponent);
			this->groupBox4->Location = System::Drawing::Point(49, 12);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(397, 91);
			this->groupBox4->TabIndex = 23;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"SYSTEM COMPONENTS";
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->buttonAbortProjection);
			this->groupBox5->Controls->Add(this->label10);
			this->groupBox5->Controls->Add(this->comboProjections);
			this->groupBox5->Location = System::Drawing::Point(49, 206);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(397, 91);
			this->groupBox5->TabIndex = 23;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"PROJECTION SELECTION";
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->labelPatientName);
			this->groupBox6->Controls->Add(this->labelGantryStatus);
			this->groupBox6->Controls->Add(this->label11);
			this->groupBox6->Controls->Add(this->label12);
			this->groupBox6->Location = System::Drawing::Point(452, 12);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(397, 91);
			this->groupBox6->TabIndex = 24;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"SYSTEM STATUS";
			// 
			// labelPatientName
			// 
			this->labelPatientName->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelPatientName->Location = System::Drawing::Point(113, 55);
			this->labelPatientName->Name = L"labelPatientName";
			this->labelPatientName->Size = System::Drawing::Size(278, 23);
			this->labelPatientName->TabIndex = 21;
			this->labelPatientName->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// labelGantryStatus
			// 
			this->labelGantryStatus->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelGantryStatus->Location = System::Drawing::Point(113, 23);
			this->labelGantryStatus->Name = L"labelGantryStatus";
			this->labelGantryStatus->Size = System::Drawing::Size(133, 23);
			this->labelGantryStatus->TabIndex = 20;
			this->labelGantryStatus->Text = L"IDLE";
			this->labelGantryStatus->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(16, 23);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(86, 13);
			this->label11->TabIndex = 8;
			this->label11->Text = L"Operating Status";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(16, 55);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(71, 13);
			this->label12->TabIndex = 10;
			this->label12->Text = L"Patient Name";
			// 
			// groupBox7
			// 
			this->groupBox7->Controls->Add(this->label14);
			this->groupBox7->Controls->Add(this->numericStator);
			this->groupBox7->Controls->Add(this->numericBulb);
			this->groupBox7->Controls->Add(this->numericAnode);
			this->groupBox7->Controls->Add(this->label15);
			this->groupBox7->Controls->Add(this->label13);
			this->groupBox7->Location = System::Drawing::Point(49, 400);
			this->groupBox7->Name = L"groupBox7";
			this->groupBox7->Size = System::Drawing::Size(397, 91);
			this->groupBox7->TabIndex = 22;
			this->groupBox7->TabStop = false;
			this->groupBox7->Text = L"TUBE DATA";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(215, 25);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(46, 13);
			this->label14->TabIndex = 22;
			this->label14->Text = L"Stator %";
			// 
			// numericStator
			// 
			this->numericStator->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->numericStator->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->numericStator->Location = System::Drawing::Point(207, 41);
			this->numericStator->Name = L"numericStator";
			this->numericStator->Size = System::Drawing::Size(75, 22);
			this->numericStator->TabIndex = 27;
			// 
			// numericBulb
			// 
			this->numericBulb->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->numericBulb->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->numericBulb->Location = System::Drawing::Point(112, 41);
			this->numericBulb->Name = L"numericBulb";
			this->numericBulb->Size = System::Drawing::Size(75, 22);
			this->numericBulb->TabIndex = 26;
			// 
			// numericAnode
			// 
			this->numericAnode->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->numericAnode->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->numericAnode->Location = System::Drawing::Point(20, 41);
			this->numericAnode->Name = L"numericAnode";
			this->numericAnode->Size = System::Drawing::Size(75, 22);
			this->numericAnode->TabIndex = 25;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(115, 25);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(39, 13);
			this->label15->TabIndex = 12;
			this->label15->Text = L"Bulb %";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(17, 25);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(49, 13);
			this->label13->TabIndex = 20;
			this->label13->Text = L"Anode %";
			// 
			// groupBox8
			// 
			this->groupBox8->Controls->Add(this->label20);
			this->groupBox8->Controls->Add(this->labelTomoSeq);
			this->groupBox8->Controls->Add(this->label19);
			this->groupBox8->Controls->Add(this->labelTomoConfId);
			this->groupBox8->Controls->Add(this->labelCmpMode);
			this->groupBox8->Controls->Add(this->labelCompressionMode);
			this->groupBox8->Controls->Add(this->checkBoxUseProtection);
			this->groupBox8->Controls->Add(this->checkBoxUseArm);
			this->groupBox8->Controls->Add(this->label18);
			this->groupBox8->Controls->Add(this->labelExposureType);
			this->groupBox8->Controls->Add(this->checkBoxPushButton);
			this->groupBox8->Location = System::Drawing::Point(651, 118);
			this->groupBox8->Name = L"groupBox8";
			this->groupBox8->Size = System::Drawing::Size(193, 278);
			this->groupBox8->TabIndex = 21;
			this->groupBox8->TabStop = false;
			this->groupBox8->Text = L"EXPOSURE";
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(19, 160);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(86, 13);
			this->label20->TabIndex = 30;
			this->label20->Text = L"Tomo Sequence";
			// 
			// labelTomoSeq
			// 
			this->labelTomoSeq->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelTomoSeq->Location = System::Drawing::Point(7, 173);
			this->labelTomoSeq->Name = L"labelTomoSeq";
			this->labelTomoSeq->Size = System::Drawing::Size(180, 23);
			this->labelTomoSeq->TabIndex = 29;
			this->labelTomoSeq->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(16, 116);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(86, 13);
			this->label19->TabIndex = 28;
			this->label19->Text = L"Tomo Config File";
			// 
			// labelTomoConfId
			// 
			this->labelTomoConfId->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelTomoConfId->Location = System::Drawing::Point(6, 129);
			this->labelTomoConfId->Name = L"labelTomoConfId";
			this->labelTomoConfId->Size = System::Drawing::Size(180, 23);
			this->labelTomoConfId->TabIndex = 27;
			this->labelTomoConfId->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// labelCmpMode
			// 
			this->labelCmpMode->AutoSize = true;
			this->labelCmpMode->Location = System::Drawing::Point(17, 71);
			this->labelCmpMode->Name = L"labelCmpMode";
			this->labelCmpMode->Size = System::Drawing::Size(97, 13);
			this->labelCmpMode->TabIndex = 26;
			this->labelCmpMode->Text = L"Compression Mode";
			// 
			// labelCompressionMode
			// 
			this->labelCompressionMode->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelCompressionMode->Location = System::Drawing::Point(6, 84);
			this->labelCompressionMode->Name = L"labelCompressionMode";
			this->labelCompressionMode->Size = System::Drawing::Size(180, 23);
			this->labelCompressionMode->TabIndex = 25;
			this->labelCompressionMode->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// checkBoxUseProtection
			// 
			this->checkBoxUseProtection->AutoSize = true;
			this->checkBoxUseProtection->Location = System::Drawing::Point(9, 208);
			this->checkBoxUseProtection->Name = L"checkBoxUseProtection";
			this->checkBoxUseProtection->Size = System::Drawing::Size(96, 17);
			this->checkBoxUseProtection->TabIndex = 24;
			this->checkBoxUseProtection->Text = L"Use Protection";
			this->checkBoxUseProtection->UseVisualStyleBackColor = true;
			// 
			// checkBoxUseArm
			// 
			this->checkBoxUseArm->AutoSize = true;
			this->checkBoxUseArm->Location = System::Drawing::Point(9, 231);
			this->checkBoxUseArm->Name = L"checkBoxUseArm";
			this->checkBoxUseArm->Size = System::Drawing::Size(66, 17);
			this->checkBoxUseArm->TabIndex = 23;
			this->checkBoxUseArm->Text = L"Use Arm";
			this->checkBoxUseArm->UseVisualStyleBackColor = true;
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(18, 24);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(74, 13);
			this->label18->TabIndex = 22;
			this->label18->Text = L"Exposure type";
			// 
			// labelExposureType
			// 
			this->labelExposureType->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelExposureType->Location = System::Drawing::Point(7, 37);
			this->labelExposureType->Name = L"labelExposureType";
			this->labelExposureType->Size = System::Drawing::Size(180, 23);
			this->labelExposureType->TabIndex = 14;
			this->labelExposureType->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// checkBoxPushButton
			// 
			this->checkBoxPushButton->AutoSize = true;
			this->checkBoxPushButton->Location = System::Drawing::Point(9, 254);
			this->checkBoxPushButton->Name = L"checkBoxPushButton";
			this->checkBoxPushButton->Size = System::Drawing::Size(120, 17);
			this->checkBoxPushButton->TabIndex = 0;
			this->checkBoxPushButton->Text = L"Push Button Enable";
			this->checkBoxPushButton->UseVisualStyleBackColor = true;
			// 
			// buttonPushButton
			// 
			this->buttonPushButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonPushButton->Location = System::Drawing::Point(122, 680);
			this->buttonPushButton->Name = L"buttonPushButton";
			this->buttonPushButton->Size = System::Drawing::Size(188, 39);
			this->buttonPushButton->TabIndex = 25;
			this->buttonPushButton->Text = L"XRAY PUSH BUTTON";
			this->buttonPushButton->UseVisualStyleBackColor = true;
			this->buttonPushButton->Click += gcnew System::EventHandler(this, &Form1::buttonPushButton_Click);
			// 
			// numericManualArm
			// 
			this->numericManualArm->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->numericManualArm->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->numericManualArm->Location = System::Drawing::Point(32, 46);
			this->numericManualArm->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 180, 0, 0, 0 });
			this->numericManualArm->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 180, 0, 0, System::Int32::MinValue });
			this->numericManualArm->Name = L"numericManualArm";
			this->numericManualArm->Size = System::Drawing::Size(116, 22);
			this->numericManualArm->TabIndex = 26;
			this->numericManualArm->ValueChanged += gcnew System::EventHandler(this, &Form1::numericManualArm_ValueChanged);
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(35, 30);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(113, 13);
			this->label21->TabIndex = 28;
			this->label21->Text = L"Manual Arm Activation";
			// 
			// groupBox9
			// 
			this->groupBox9->Controls->Add(this->numericManualTrx);
			this->groupBox9->Controls->Add(this->label22);
			this->groupBox9->Controls->Add(this->numericManualArm);
			this->groupBox9->Controls->Add(this->label21);
			this->groupBox9->Location = System::Drawing::Point(49, 303);
			this->groupBox9->Name = L"groupBox9";
			this->groupBox9->Size = System::Drawing::Size(397, 91);
			this->groupBox9->TabIndex = 29;
			this->groupBox9->TabStop = false;
			this->groupBox9->Text = L"MANUAL ACTIVATION";
			// 
			// numericManualTrx
			// 
			this->numericManualTrx->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->numericManualTrx->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100, 0, 0, 0 });
			this->numericManualTrx->Location = System::Drawing::Point(240, 46);
			this->numericManualTrx->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2700, 0, 0, 0 });
			this->numericManualTrx->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2700, 0, 0, System::Int32::MinValue });
			this->numericManualTrx->Name = L"numericManualTrx";
			this->numericManualTrx->Size = System::Drawing::Size(116, 22);
			this->numericManualTrx->TabIndex = 31;
			this->numericManualTrx->ValueChanged += gcnew System::EventHandler(this, &Form1::numericManualTrx_ValueChanged);
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(246, 30);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(110, 13);
			this->label22->TabIndex = 30;
			this->label22->Text = L"Manual Trx Activation";
			// 
			// groupBoxReady
			// 
			this->groupBoxReady->Controls->Add(this->checkBox_0x80);
			this->groupBoxReady->Controls->Add(this->checkBox_0x40);
			this->groupBoxReady->Controls->Add(this->checkBox_0x20);
			this->groupBoxReady->Controls->Add(this->checkBox_0x10);
			this->groupBoxReady->Controls->Add(this->checkBox_0x8);
			this->groupBoxReady->Controls->Add(this->checkBox_0x4);
			this->groupBoxReady->Controls->Add(this->checkBox_0x2);
			this->groupBoxReady->Controls->Add(this->checkBox_0x1);
			this->groupBoxReady->Location = System::Drawing::Point(49, 501);
			this->groupBoxReady->Name = L"groupBoxReady";
			this->groupBoxReady->Size = System::Drawing::Size(397, 133);
			this->groupBoxReady->TabIndex = 28;
			this->groupBoxReady->TabStop = false;
			this->groupBoxReady->Text = L"READY FOR EXPOSURE";
			// 
			// checkBox_0x80
			// 
			this->checkBox_0x80->AutoSize = true;
			this->checkBox_0x80->Location = System::Drawing::Point(217, 95);
			this->checkBox_0x80->Name = L"checkBox_0x80";
			this->checkBox_0x80->Size = System::Drawing::Size(160, 17);
			this->checkBox_0x80->TabIndex = 32;
			this->checkBox_0x80->Text = L"PUSH BUTTON DISABLED";
			this->checkBox_0x80->UseVisualStyleBackColor = true;
			// 
			// checkBox_0x40
			// 
			this->checkBox_0x40->AutoSize = true;
			this->checkBox_0x40->Location = System::Drawing::Point(217, 72);
			this->checkBox_0x40->Name = L"checkBox_0x40";
			this->checkBox_0x40->Size = System::Drawing::Size(165, 17);
			this->checkBox_0x40->TabIndex = 31;
			this->checkBox_0x40->Text = L"MISSING EXPOSURE DATA";
			this->checkBox_0x40->UseVisualStyleBackColor = true;
			// 
			// checkBox_0x20
			// 
			this->checkBox_0x20->AutoSize = true;
			this->checkBox_0x20->Location = System::Drawing::Point(217, 45);
			this->checkBox_0x20->Name = L"checkBox_0x20";
			this->checkBox_0x20->Size = System::Drawing::Size(168, 17);
			this->checkBox_0x20->TabIndex = 30;
			this->checkBox_0x20->Text = L"MISSING EXPOSURE MODE";
			this->checkBox_0x20->UseVisualStyleBackColor = true;
			// 
			// checkBox_0x10
			// 
			this->checkBox_0x10->AutoSize = true;
			this->checkBox_0x10->Location = System::Drawing::Point(217, 19);
			this->checkBox_0x10->Name = L"checkBox_0x10";
			this->checkBox_0x10->Size = System::Drawing::Size(115, 17);
			this->checkBox_0x10->TabIndex = 29;
			this->checkBox_0x10->Text = L"WRONG PADDLE";
			this->checkBox_0x10->UseVisualStyleBackColor = true;
			// 
			// checkBox_0x8
			// 
			this->checkBox_0x8->AutoSize = true;
			this->checkBox_0x8->Location = System::Drawing::Point(6, 95);
			this->checkBox_0x8->Name = L"checkBox_0x8";
			this->checkBox_0x8->Size = System::Drawing::Size(150, 17);
			this->checkBox_0x8->TabIndex = 28;
			this->checkBox_0x8->Text = L"WRONG ARM POSITION";
			this->checkBox_0x8->UseVisualStyleBackColor = true;
			// 
			// checkBox_0x4
			// 
			this->checkBox_0x4->AutoSize = true;
			this->checkBox_0x4->Location = System::Drawing::Point(6, 72);
			this->checkBox_0x4->Name = L"checkBox_0x4";
			this->checkBox_0x4->Size = System::Drawing::Size(193, 17);
			this->checkBox_0x4->TabIndex = 27;
			this->checkBox_0x4->Text = L"MISSING PATIENT PROTECTION";
			this->checkBox_0x4->UseVisualStyleBackColor = true;
			// 
			// checkBox_0x2
			// 
			this->checkBox_0x2->AutoSize = true;
			this->checkBox_0x2->Location = System::Drawing::Point(6, 46);
			this->checkBox_0x2->Name = L"checkBox_0x2";
			this->checkBox_0x2->Size = System::Drawing::Size(153, 17);
			this->checkBox_0x2->TabIndex = 26;
			this->checkBox_0x2->Text = L"MISSING COMPRESSION";
			this->checkBox_0x2->UseVisualStyleBackColor = true;
			// 
			// checkBox_0x1
			// 
			this->checkBox_0x1->AutoSize = true;
			this->checkBox_0x1->Location = System::Drawing::Point(6, 23);
			this->checkBox_0x1->Name = L"checkBox_0x1";
			this->checkBox_0x1->Size = System::Drawing::Size(128, 17);
			this->checkBox_0x1->TabIndex = 25;
			this->checkBox_0x1->Text = L"ERROR CONDITION";
			this->checkBox_0x1->UseVisualStyleBackColor = true;
			// 
			// groupBoxPulse0
			// 
			this->groupBoxPulse0->Controls->Add(this->checkBox_valid0);
			this->groupBoxPulse0->Controls->Add(this->label27);
			this->groupBoxPulse0->Controls->Add(this->label_filter0);
			this->groupBoxPulse0->Controls->Add(this->label16);
			this->groupBoxPulse0->Controls->Add(this->label_kV0);
			this->groupBoxPulse0->Controls->Add(this->label24);
			this->groupBoxPulse0->Controls->Add(this->label_mAs0);
			this->groupBoxPulse0->Location = System::Drawing::Point(549, 402);
			this->groupBoxPulse0->Name = L"groupBoxPulse0";
			this->groupBoxPulse0->Size = System::Drawing::Size(293, 82);
			this->groupBoxPulse0->TabIndex = 22;
			this->groupBoxPulse0->TabStop = false;
			this->groupBoxPulse0->Text = L"PULSE-0";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(19, 28);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(20, 13);
			this->label16->TabIndex = 20;
			this->label16->Text = L"kV";
			// 
			// label_kV0
			// 
			this->label_kV0->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_kV0->Location = System::Drawing::Point(6, 41);
			this->label_kV0->Name = L"label_kV0";
			this->label_kV0->Size = System::Drawing::Size(69, 23);
			this->label_kV0->TabIndex = 19;
			this->label_kV0->Text = L"-";
			this->label_kV0->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(91, 28);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(27, 13);
			this->label24->TabIndex = 12;
			this->label24->Text = L"mAs";
			// 
			// label_mAs0
			// 
			this->label_mAs0->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_mAs0->Location = System::Drawing::Point(81, 41);
			this->label_mAs0->Name = L"label_mAs0";
			this->label_mAs0->Size = System::Drawing::Size(69, 23);
			this->label_mAs0->TabIndex = 11;
			this->label_mAs0->Text = L"-";
			this->label_mAs0->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_filter0
			// 
			this->label_filter0->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_filter0->Location = System::Drawing::Point(156, 41);
			this->label_filter0->Name = L"label_filter0";
			this->label_filter0->Size = System::Drawing::Size(69, 23);
			this->label_filter0->TabIndex = 21;
			this->label_filter0->Text = L"0 ";
			this->label_filter0->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(166, 28);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(26, 13);
			this->label27->TabIndex = 22;
			this->label27->Text = L"filter";
			// 
			// checkBox_valid0
			// 
			this->checkBox_valid0->AutoSize = true;
			this->checkBox_valid0->Location = System::Drawing::Point(231, 45);
			this->checkBox_valid0->Name = L"checkBox_valid0";
			this->checkBox_valid0->Size = System::Drawing::Size(49, 17);
			this->checkBox_valid0->TabIndex = 23;
			this->checkBox_valid0->Text = L"Valid";
			this->checkBox_valid0->UseVisualStyleBackColor = true;
			// 
			// groupBox10
			// 
			this->groupBox10->Controls->Add(this->checkBox_valid1);
			this->groupBox10->Controls->Add(this->label23);
			this->groupBox10->Controls->Add(this->label_filter1);
			this->groupBox10->Controls->Add(this->label26);
			this->groupBox10->Controls->Add(this->label_kV1);
			this->groupBox10->Controls->Add(this->label29);
			this->groupBox10->Controls->Add(this->label_mAs1);
			this->groupBox10->Location = System::Drawing::Point(549, 490);
			this->groupBox10->Name = L"groupBox10";
			this->groupBox10->Size = System::Drawing::Size(293, 82);
			this->groupBox10->TabIndex = 30;
			this->groupBox10->TabStop = false;
			this->groupBox10->Text = L"PULSE-1";
			// 
			// checkBox_valid1
			// 
			this->checkBox_valid1->AutoSize = true;
			this->checkBox_valid1->Location = System::Drawing::Point(231, 45);
			this->checkBox_valid1->Name = L"checkBox_valid1";
			this->checkBox_valid1->Size = System::Drawing::Size(49, 17);
			this->checkBox_valid1->TabIndex = 23;
			this->checkBox_valid1->Text = L"Valid";
			this->checkBox_valid1->UseVisualStyleBackColor = true;
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(166, 28);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(26, 13);
			this->label23->TabIndex = 22;
			this->label23->Text = L"filter";
			// 
			// label_filter1
			// 
			this->label_filter1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_filter1->Location = System::Drawing::Point(156, 41);
			this->label_filter1->Name = L"label_filter1";
			this->label_filter1->Size = System::Drawing::Size(69, 23);
			this->label_filter1->TabIndex = 21;
			this->label_filter1->Text = L"0 ";
			this->label_filter1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(19, 28);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(20, 13);
			this->label26->TabIndex = 20;
			this->label26->Text = L"kV";
			// 
			// label_kV1
			// 
			this->label_kV1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_kV1->Location = System::Drawing::Point(6, 41);
			this->label_kV1->Name = L"label_kV1";
			this->label_kV1->Size = System::Drawing::Size(69, 23);
			this->label_kV1->TabIndex = 19;
			this->label_kV1->Text = L"-";
			this->label_kV1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(91, 28);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(27, 13);
			this->label29->TabIndex = 12;
			this->label29->Text = L"mAs";
			// 
			// label_mAs1
			// 
			this->label_mAs1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_mAs1->Location = System::Drawing::Point(81, 41);
			this->label_mAs1->Name = L"label_mAs1";
			this->label_mAs1->Size = System::Drawing::Size(69, 23);
			this->label_mAs1->TabIndex = 11;
			this->label_mAs1->Text = L"-";
			this->label_mAs1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// groupBox11
			// 
			this->groupBox11->Controls->Add(this->checkBox_valid2);
			this->groupBox11->Controls->Add(this->label31);
			this->groupBox11->Controls->Add(this->label_fiter2);
			this->groupBox11->Controls->Add(this->label33);
			this->groupBox11->Controls->Add(this->label_kV2);
			this->groupBox11->Controls->Add(this->label35);
			this->groupBox11->Controls->Add(this->label_mAs2);
			this->groupBox11->Location = System::Drawing::Point(549, 578);
			this->groupBox11->Name = L"groupBox11";
			this->groupBox11->Size = System::Drawing::Size(293, 82);
			this->groupBox11->TabIndex = 31;
			this->groupBox11->TabStop = false;
			this->groupBox11->Text = L"PULSE-2";
			// 
			// checkBox_valid2
			// 
			this->checkBox_valid2->AutoSize = true;
			this->checkBox_valid2->Location = System::Drawing::Point(231, 45);
			this->checkBox_valid2->Name = L"checkBox_valid2";
			this->checkBox_valid2->Size = System::Drawing::Size(49, 17);
			this->checkBox_valid2->TabIndex = 23;
			this->checkBox_valid2->Text = L"Valid";
			this->checkBox_valid2->UseVisualStyleBackColor = true;
			// 
			// label31
			// 
			this->label31->AutoSize = true;
			this->label31->Location = System::Drawing::Point(166, 28);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(26, 13);
			this->label31->TabIndex = 22;
			this->label31->Text = L"filter";
			// 
			// label_fiter2
			// 
			this->label_fiter2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_fiter2->Location = System::Drawing::Point(156, 41);
			this->label_fiter2->Name = L"label_fiter2";
			this->label_fiter2->Size = System::Drawing::Size(69, 23);
			this->label_fiter2->TabIndex = 21;
			this->label_fiter2->Text = L"0 ";
			this->label_fiter2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label33
			// 
			this->label33->AutoSize = true;
			this->label33->Location = System::Drawing::Point(19, 28);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(20, 13);
			this->label33->TabIndex = 20;
			this->label33->Text = L"kV";
			// 
			// label_kV2
			// 
			this->label_kV2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_kV2->Location = System::Drawing::Point(6, 41);
			this->label_kV2->Name = L"label_kV2";
			this->label_kV2->Size = System::Drawing::Size(69, 23);
			this->label_kV2->TabIndex = 19;
			this->label_kV2->Text = L"-";
			this->label_kV2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label35
			// 
			this->label35->AutoSize = true;
			this->label35->Location = System::Drawing::Point(91, 28);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(27, 13);
			this->label35->TabIndex = 12;
			this->label35->Text = L"mAs";
			// 
			// label_mAs2
			// 
			this->label_mAs2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_mAs2->Location = System::Drawing::Point(81, 41);
			this->label_mAs2->Name = L"label_mAs2";
			this->label_mAs2->Size = System::Drawing::Size(69, 23);
			this->label_mAs2->TabIndex = 11;
			this->label_mAs2->Text = L"-";
			this->label_mAs2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// groupBox12
			// 
			this->groupBox12->Controls->Add(this->checkBox_valid3);
			this->groupBox12->Controls->Add(this->label37);
			this->groupBox12->Controls->Add(this->label_filter3);
			this->groupBox12->Controls->Add(this->label39);
			this->groupBox12->Controls->Add(this->label_kV3);
			this->groupBox12->Controls->Add(this->label41);
			this->groupBox12->Controls->Add(this->label_mAs3);
			this->groupBox12->Location = System::Drawing::Point(549, 666);
			this->groupBox12->Name = L"groupBox12";
			this->groupBox12->Size = System::Drawing::Size(293, 82);
			this->groupBox12->TabIndex = 32;
			this->groupBox12->TabStop = false;
			this->groupBox12->Text = L"PULSE-3";
			// 
			// checkBox_valid3
			// 
			this->checkBox_valid3->AutoSize = true;
			this->checkBox_valid3->Location = System::Drawing::Point(231, 45);
			this->checkBox_valid3->Name = L"checkBox_valid3";
			this->checkBox_valid3->Size = System::Drawing::Size(49, 17);
			this->checkBox_valid3->TabIndex = 23;
			this->checkBox_valid3->Text = L"Valid";
			this->checkBox_valid3->UseVisualStyleBackColor = true;
			// 
			// label37
			// 
			this->label37->AutoSize = true;
			this->label37->Location = System::Drawing::Point(166, 28);
			this->label37->Name = L"label37";
			this->label37->Size = System::Drawing::Size(26, 13);
			this->label37->TabIndex = 22;
			this->label37->Text = L"filter";
			// 
			// label_filter3
			// 
			this->label_filter3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_filter3->Location = System::Drawing::Point(156, 41);
			this->label_filter3->Name = L"label_filter3";
			this->label_filter3->Size = System::Drawing::Size(69, 23);
			this->label_filter3->TabIndex = 21;
			this->label_filter3->Text = L"0 ";
			this->label_filter3->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label39
			// 
			this->label39->AutoSize = true;
			this->label39->Location = System::Drawing::Point(19, 28);
			this->label39->Name = L"label39";
			this->label39->Size = System::Drawing::Size(20, 13);
			this->label39->TabIndex = 20;
			this->label39->Text = L"kV";
			// 
			// label_kV3
			// 
			this->label_kV3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_kV3->Location = System::Drawing::Point(6, 41);
			this->label_kV3->Name = L"label_kV3";
			this->label_kV3->Size = System::Drawing::Size(69, 23);
			this->label_kV3->TabIndex = 19;
			this->label_kV3->Text = L"-";
			this->label_kV3->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label41
			// 
			this->label41->AutoSize = true;
			this->label41->Location = System::Drawing::Point(91, 28);
			this->label41->Name = L"label41";
			this->label41->Size = System::Drawing::Size(27, 13);
			this->label41->TabIndex = 12;
			this->label41->Text = L"mAs";
			// 
			// label_mAs3
			// 
			this->label_mAs3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_mAs3->Location = System::Drawing::Point(81, 41);
			this->label_mAs3->Name = L"label_mAs3";
			this->label_mAs3->Size = System::Drawing::Size(69, 23);
			this->label_mAs3->TabIndex = 11;
			this->label_mAs3->Text = L"-";
			this->label_mAs3->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(875, 767);
			this->Controls->Add(this->groupBox12);
			this->Controls->Add(this->groupBox11);
			this->Controls->Add(this->groupBox10);
			this->Controls->Add(this->groupBoxPulse0);
			this->Controls->Add(this->groupBoxReady);
			this->Controls->Add(this->groupBox9);
			this->Controls->Add(this->buttonPushButton);
			this->Controls->Add(this->groupBox8);
			this->Controls->Add(this->groupBox7);
			this->Controls->Add(this->groupBox6);
			this->Controls->Add(this->groupBox5);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			this->groupBox7->ResumeLayout(false);
			this->groupBox7->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericStator))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericBulb))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericAnode))->EndInit();
			this->groupBox8->ResumeLayout(false);
			this->groupBox8->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericManualArm))->EndInit();
			this->groupBox9->ResumeLayout(false);
			this->groupBox9->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericManualTrx))->EndInit();
			this->groupBoxReady->ResumeLayout(false);
			this->groupBoxReady->PerformLayout();
			this->groupBoxPulse0->ResumeLayout(false);
			this->groupBoxPulse0->PerformLayout();
			this->groupBox10->ResumeLayout(false);
			this->groupBox10->PerformLayout();
			this->groupBox11->ResumeLayout(false);
			this->groupBox11->PerformLayout();
			this->groupBox12->ResumeLayout(false);
			this->groupBox12->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion


	
	private: System::Void textForce_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
		int force;
		int thick;
		if (e->KeyChar != '\r') return;
		if (comboPaddle->SelectedIndex > 10) {
			textForce->Text = "0";
			textThickness->Text = "0";
			return;
		}

		if (textForce->Text == "") textForce->Text = "0";
		
		force = Convert::ToInt16(textForce->Text);
		thick = Convert::ToInt16(textThickness->Text);
		if (force == 0) textThickness->Text = "0";
		if ((force) && (!thick)) textThickness->Text = "30";
		
		force = Convert::ToInt16(textForce->Text);
		thick = Convert::ToInt16(textThickness->Text);

		GantryStatusRegisters::CompressorRegister::setForce(force);
		GantryStatusRegisters::CompressorRegister::setThickness(thick);

	}

	private: System::Void textThickness_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
		int force;
		int thick;
		if (e->KeyChar != '\r') return;
		if (comboPaddle->SelectedIndex > 10) {
			textForce->Text = "0";
			textThickness->Text = "0";
			return;
		}
		if (textThickness->Text == "") textThickness->Text = "0";
		
		force = Convert::ToInt16(textForce->Text);
		thick = Convert::ToInt16(textThickness->Text);
		
		if (force == 0) textThickness->Text = "0";
		if (thick == 0) textForce->Text = "0";
	
		force = Convert::ToInt16(textForce->Text);
		thick = Convert::ToInt16(textThickness->Text);

		GantryStatusRegisters::CompressorRegister::setForce(force);
		GantryStatusRegisters::CompressorRegister::setThickness(thick);

	}
	
	private: System::Void listPaddle_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		CompressorRegister::getPaddle()->Value->setCode((PaddleOption::options) comboPaddle->SelectedIndex);

		int force = Convert::ToInt16(textForce->Text);
		int thick = Convert::ToInt16(textThickness->Text);

		if ((comboPaddle->SelectedIndex > 10)&&((force)||(thick))) {
			textThickness->Text = "0";
			textForce->Text = "0";

			int force = Convert::ToInt16(textForce->Text);
			int thick = Convert::ToInt16(textThickness->Text);

			GantryStatusRegisters::CompressorRegister::setForce(force);
			GantryStatusRegisters::CompressorRegister::setThickness(thick);

		}
		
	}

	private: System::Void openStudyChange(void) {
		
		if (OperatingStatusRegister::isOPEN()) {
			labelPatientName->Text = OperatingStatusRegister::getPatientName();
			labelGantryStatus->Text = "OPEN STUDY";
			openStudyRegisters();
		}
		else if (OperatingStatusRegister::isCLOSE()) {
			labelPatientName->Text = "";
			labelGantryStatus->Text = "CLOSE STUDY";
			closeStudyRegisters();
		}
		else if (OperatingStatusRegister::isSERVICE()) {
			labelPatientName->Text = "";
			labelGantryStatus->Text = "SERVICE";
			closeStudyRegisters();

		}
	}

	private: System::Void comboComponents_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		ComponentRegister::Value->setCode((GantryStatusRegisters::ComponentRegister::options)comboComponents->SelectedIndex);
	}

	private: System::Void comboColliComponent_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		CollimatorComponentRegister::Value->setCode((GantryStatusRegisters::CollimatorComponentRegister::options) comboColliComponent->SelectedIndex);
	}

	private: System::Void comboProjections_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		static bool event_active = true;
		if (!event_active) {
			event_active = true;
			return;
		}

		String^ projection = comboProjections->SelectedItem->ToString();
		ArmStatusRegister::projectionRequest(projection);
		event_active = false;
		comboProjections->SelectedIndex = comboProjections->Items->Count - 1;

	}
	
	private: System::Void   ProjectionUpdateList(void) {
		comboProjections->Items->Clear();
		
		int count = ArmStatusRegister::getProjections()->Value->Count();
		if (count == 0) return;

		for (int i = 0; i < count; i++) {
			comboProjections->Items->Add(ArmStatusRegister::getProjections()->Value->getItem(i));
		}
		comboProjections->Items->Add("SELECT PROJECTION");
		comboProjections->SelectedIndex = count;
	}

	private: System::Void   ProjectionChange(void) {
		labelArmProjection->Text = ArmStatusRegister::getProjections()->Value->getTag();
	}

	private: System::Void buttonAbortProjection_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!ArmStatusRegister::isValid()) return;
		ArmStatusRegister::abortProjectionRequest();
	}

	private: System::Void ArmValidateChg(void) {
		checkBoxArmValid->Checked = ArmStatusRegister::isValid();
		if (checkBoxArmValid->Checked) return;
		labelArmTarget->Text = " - ";
		labelArmLow->Text = "-";
		labelArmHigh->Text = "-";

	}


	private: System::Void ArmUpdateData(void) {
		
		
		labelArmTarget->Text = ArmStatusRegister::getTarget().ToString();
		labelArmLow->Text = ArmStatusRegister::getLow().ToString();
		labelArmHigh->Text = ArmStatusRegister::getHigh().ToString();		
		armTimer->Start();
		
	}

	private: System::Void TrxUpdateData(void) {

		labelTrxTarget->Text = TrxStatusRegister::getTargetTag();		
		trxTimer->Start();

	}

	private: System::Void numericManualArm_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		int val = System::Decimal::ToInt16(numericManualArm->Value);
		ArmStatusRegister::updateCurrentPosition(val);
	}

	private: System::Void numericManualTrx_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		int val = System::Decimal::ToInt16(numericManualTrx->Value);
		TrxStatusRegister::updateCurrentPosition(val);
	}
		   
	private: System::Void updateArmPosition(void) {
		labelArmAngle->Text = ArmStatusRegister::getAngle().ToString();
	}

	private: System::Void updateTrxPosition(void) {
		labelTrxAngle->Text = TrxStatusRegister::getCurrentAngle().ToString();
	}

	private: System::Void updateExposureType(void) {
		labelExposureType->Text = ExposureModeRegister::exposureType->Value->getTag();
	}

	private: System::Void updateProtectionMode(void) {
		checkBoxUseProtection->Checked = ExposureModeRegister::protectionMode->useProtection();
		
	}

	private: System::Void updateArmMode(void) {
		checkBoxUseArm->Checked = (ExposureModeRegister::armMode->Value->getCode() == ArmModeOption::options::ARM_ENA);

	}

	private: System::Void updateXrayPushEna(void) {
		checkBoxPushButton->Checked = (XrayPushButtonRegister::isEnabled());

	}


	private: System::Void updateCompressionMode(void) {
		labelCompressionMode->Text = ExposureModeRegister::compressorMode->Value->getTag();
	}

	private: System::Void updateTomoConfig(void) {
		labelTomoConfId->Text = TomoConfigRegister::getConfig();
		labelTomoSeq->Text = TomoConfigRegister::getSequence();
	}

	private: System::Void onArmTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		armTimer->Stop();
		SendNotifyMessageA(window, WM_USER + 1, 0, 0);
	}

	private: System::Void onTrxTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		trxTimer->Stop();
		SendNotifyMessageA(window, WM_USER + 2, 0, 0);
	}

	private: System::Void onReadyTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		trxTimer->Stop();
		SendNotifyMessageA(window, WM_USER + 3, 0, 0);
	}

	private: System::Void verifyReady(void)
	{
		unsigned short notready = ReadyForExposureRegister::evaluateReadyForExposure();

		checkBox_0x1->Checked = (notready & 0x1);
		checkBox_0x2->Checked = (notready & 0x2);
		checkBox_0x4->Checked = (notready & 0x4);
		checkBox_0x8->Checked = (notready & 0x8);
		checkBox_0x10->Checked = (notready & 0x10);
		checkBox_0x20->Checked = (notready & 0x20);
		checkBox_0x40->Checked = (notready & 0x40);
		checkBox_0x80->Checked = (notready & 0x80);

		if (notready) groupBoxReady->Text = "NOT READY FOR EXPOSURE";
		else groupBoxReady->Text = "READY FOR EXPOSURE";

		
	}

		   

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override
	{
		switch (m.Msg) {
		
		case (WM_USER + 1): // Arm Update
			ArmStatusRegister::updateCurrentPosition(Convert::ToInt16(labelArmTarget->Text));
			numericManualArm->Value = ArmStatusRegister::getAngle();
			ArmStatusRegister::activationCompleted(Convert::ToInt16(labelArmAngle->Text), (int) 0);
			break;

		case (WM_USER + 2): // Trx Update
			TrxStatusRegister::updateCurrentPosition(TrxStatusRegister::getTargetAngle());
			numericManualTrx->Value = TrxStatusRegister::getTargetAngle();
			TrxStatusRegister::activationCompleted(Convert::ToInt16(labelArmAngle->Text), (int)0);
			break;
		case (WM_USER + 3): // Ready Timer
			verifyReady();
			break;
		}

		Form::WndProc(m);
	}



	
private: System::Void buttonPushButton_Click(System::Object^ sender, System::EventArgs^ e) {
}



};


}
