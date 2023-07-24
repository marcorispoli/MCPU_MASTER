#pragma once

namespace CppCLRWinFormsProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{

	public:
		HWND window;
		Form1(void)
		{
			InitializeComponent();
			window = static_cast<HWND>(Handle.ToPointer());

			//
			//TODO: Add the constructor code here
			//
			comboPaddle->SelectedIndex = 11;
			comboComponents->SelectedIndex = 5;
			comboColliComponent->SelectedIndex = 4;
			textForce->Text = "0";
			textThickness->Text = "0";
			labelArmAngle->Text = "0 (°)";
			labelArmTarget->Text = "0 (°)";
			labelArmLow->Text = "0 (°)";
			labelArmHigh->Text = "0 (°)";
			labelArmProjection->Text= "UNDEFINED";
			GantryStatusRegisters::ArmStatusRegister::target_change_event += gcnew GantryStatusRegisters::ArmStatusRegister::delegate_target_change(this, &Form1::ArmUpdateData);

			aTimer = gcnew System::Timers::Timer(3000);
			aTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &Form1::onArmTimeout);
			aTimer->Stop();

			GantryStatusRegisters::ArmStatusRegister::activation_completed_event += gcnew GantryStatusRegisters::ArmStatusRegister::delegate_activation_completed((awsProtocol^) GlobalObjects::pAws, &awsProtocol::armActivationCompletedCallback);
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
		System::Timers::Timer^ aTimer;


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
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->labelArmProjection = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->labelArmHigh = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->labelArmLow = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->labelArmTarget = (gcnew System::Windows::Forms::Label());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// textForce
			// 
			this->textForce->Location = System::Drawing::Point(171, 42);
			this->textForce->Name = L"textForce";
			this->textForce->Size = System::Drawing::Size(100, 20);
			this->textForce->TabIndex = 0;
			this->textForce->Text = L"0";
			this->textForce->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textForce_KeyPress);
			// 
			// labelCmpForce
			// 
			this->labelCmpForce->AutoSize = true;
			this->labelCmpForce->Location = System::Drawing::Point(46, 42);
			this->labelCmpForce->Name = L"labelCmpForce";
			this->labelCmpForce->Size = System::Drawing::Size(97, 13);
			this->labelCmpForce->TabIndex = 1;
			this->labelCmpForce->Text = L"Compression Force";
			// 
			// textThickness
			// 
			this->textThickness->Location = System::Drawing::Point(171, 68);
			this->textThickness->Name = L"textThickness";
			this->textThickness->Size = System::Drawing::Size(100, 20);
			this->textThickness->TabIndex = 2;
			this->textThickness->Text = L"0";
			this->textThickness->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textThickness_KeyPress);
			// 
			// lableCmpThick
			// 
			this->lableCmpThick->AutoSize = true;
			this->lableCmpThick->Location = System::Drawing::Point(46, 68);
			this->lableCmpThick->Name = L"lableCmpThick";
			this->lableCmpThick->Size = System::Drawing::Size(119, 13);
			this->lableCmpThick->TabIndex = 3;
			this->lableCmpThick->Text = L"Compression Thickness";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(46, 94);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(98, 13);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Compressor Paddle";
			// 
			// comboPaddle
			// 
			this->comboPaddle->FormattingEnabled = true;
			this->comboPaddle->Items->AddRange(gcnew cli::array< System::Object^  >(13) {
				L"PAD-24x30", L"PAD-18x24_C", L"PAD-18x24_L",
					L"PAD-18x24_R", L"PAD-9x21", L"PAD-10x24", L"PAD-D75", L"PAD-BP2D", L"PAD-BP3D", L"PAD-TOMO", L"PAD-9x9", L"PAD-UNDETECTED",
					L"PAD-UNLOCKED"
			});
			this->comboPaddle->Location = System::Drawing::Point(171, 94);
			this->comboPaddle->Name = L"comboPaddle";
			this->comboPaddle->Size = System::Drawing::Size(121, 21);
			this->comboPaddle->TabIndex = 6;
			this->comboPaddle->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::listPaddle_SelectedIndexChanged);
			// 
			// comboComponents
			// 
			this->comboComponents->FormattingEnabled = true;
			this->comboComponents->Items->AddRange(gcnew cli::array< System::Object^  >(7) {
				L"PROTECTION_3D", L"MAG15", L"MAG18", L"MAG20",
					L"BIOPSY", L"UNDEFINED", L" "
			});
			this->comboComponents->Location = System::Drawing::Point(171, 121);
			this->comboComponents->Name = L"comboComponents";
			this->comboComponents->Size = System::Drawing::Size(121, 21);
			this->comboComponents->TabIndex = 7;
			this->comboComponents->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboComponents_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(46, 121);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(66, 13);
			this->label2->TabIndex = 8;
			this->label2->Text = L"Components";
			// 
			// comboColliComponent
			// 
			this->comboColliComponent->FormattingEnabled = true;
			this->comboColliComponent->Items->AddRange(gcnew cli::array< System::Object^  >(5) {
				L"PROTECTION_2D", L"PROTECTION_SHIFTED",
					L"LEAD_SCREEN", L"SPECIMEN", L"UNDEFINED"
			});
			this->comboColliComponent->Location = System::Drawing::Point(171, 148);
			this->comboColliComponent->Name = L"comboColliComponent";
			this->comboColliComponent->Size = System::Drawing::Size(121, 21);
			this->comboColliComponent->TabIndex = 9;
			this->comboColliComponent->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboColliComponent_SelectedIndexChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(46, 148);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(88, 13);
			this->label3->TabIndex = 10;
			this->label3->Text = L"Colli-Components";
			// 
			// labelArmAngle
			// 
			this->labelArmAngle->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelArmAngle->Location = System::Drawing::Point(16, 79);
			this->labelArmAngle->Name = L"labelArmAngle";
			this->labelArmAngle->Size = System::Drawing::Size(69, 23);
			this->labelArmAngle->TabIndex = 11;
			this->labelArmAngle->Text = L"0 (°)";
			this->labelArmAngle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// groupBox1
			// 
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
			this->groupBox1->Location = System::Drawing::Point(58, 229);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(392, 121);
			this->groupBox1->TabIndex = 12;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"ARM DATA";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(245, 28);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(54, 13);
			this->label9->TabIndex = 18;
			this->label9->Text = L"Projection";
			// 
			// labelArmProjection
			// 
			this->labelArmProjection->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelArmProjection->Location = System::Drawing::Point(242, 41);
			this->labelArmProjection->Name = L"labelArmProjection";
			this->labelArmProjection->Size = System::Drawing::Size(115, 23);
			this->labelArmProjection->TabIndex = 17;
			this->labelArmProjection->Text = L"UNDEFINED";
			this->labelArmProjection->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(170, 28);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(29, 13);
			this->label7->TabIndex = 16;
			this->label7->Text = L"High";
			// 
			// labelArmHigh
			// 
			this->labelArmHigh->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelArmHigh->Location = System::Drawing::Point(167, 41);
			this->labelArmHigh->Name = L"labelArmHigh";
			this->labelArmHigh->Size = System::Drawing::Size(69, 23);
			this->labelArmHigh->TabIndex = 15;
			this->labelArmHigh->Text = L"0 (°)";
			this->labelArmHigh->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(95, 28);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(27, 13);
			this->label4->TabIndex = 14;
			this->label4->Text = L"Low";
			// 
			// labelArmLow
			// 
			this->labelArmLow->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelArmLow->Location = System::Drawing::Point(92, 41);
			this->labelArmLow->Name = L"labelArmLow";
			this->labelArmLow->Size = System::Drawing::Size(69, 23);
			this->labelArmLow->TabIndex = 13;
			this->labelArmLow->Text = L"0 (°)";
			this->labelArmLow->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(19, 66);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(34, 13);
			this->label5->TabIndex = 12;
			this->label5->Text = L"Angle";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(19, 28);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(38, 13);
			this->label6->TabIndex = 20;
			this->label6->Text = L"Target";
			// 
			// labelArmTarget
			// 
			this->labelArmTarget->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelArmTarget->Location = System::Drawing::Point(16, 41);
			this->labelArmTarget->Name = L"labelArmTarget";
			this->labelArmTarget->Size = System::Drawing::Size(69, 23);
			this->labelArmTarget->TabIndex = 19;
			this->labelArmTarget->Text = L"0 (°)";
			this->labelArmTarget->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(787, 620);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboColliComponent);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboComponents);
			this->Controls->Add(this->comboPaddle);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->lableCmpThick);
			this->Controls->Add(this->textThickness);
			this->Controls->Add(this->labelCmpForce);
			this->Controls->Add(this->textForce);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

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
		GantryStatusRegisters::PaddleRegister::setCode((GantryStatusRegisters::PaddleRegister::options)comboPaddle->SelectedIndex);

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

	private: System::Void comboComponents_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		GantryStatusRegisters::ComponentRegister::setCode((GantryStatusRegisters::ComponentRegister::options) comboPaddle->SelectedIndex);
	}

	private: System::Void comboColliComponent_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		GantryStatusRegisters::CollimatorComponentRegister::setCode((GantryStatusRegisters::CollimatorComponentRegister::options) comboPaddle->SelectedIndex);
	}
	
	private: System::Void ArmUpdateData(void) {
		
		//labelArmAngle->Text = "0 (°)";
		
		labelArmTarget->Text = GantryStatusRegisters::ArmStatusRegister::getTarget().ToString();
		labelArmLow->Text = GantryStatusRegisters::ArmStatusRegister::getLow().ToString();
		labelArmHigh->Text = GantryStatusRegisters::ArmStatusRegister::getHigh().ToString();
		labelArmProjection->Text = GantryStatusRegisters::ProjectionRegister::getTag();
		aTimer->Start();
		
	}

	private: System::Void onArmTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		aTimer->Stop();
		SendNotifyMessageA(window, WM_USER + 1, 0, 0);
	}

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override
	{
		switch (m.Msg) {
		
		case (WM_USER + 1): // Arm Update
			labelArmAngle->Text = labelArmTarget->Text;
			GantryStatusRegisters::ArmStatusRegister::activationCompleted(Convert::ToInt16(labelArmAngle->Text), (int) 0);
			break;
		}
		Form::WndProc(m);
	}


};


}
