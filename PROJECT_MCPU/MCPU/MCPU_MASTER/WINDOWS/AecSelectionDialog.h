#pragma once
#include <Windows.h>
#include "Log.h"




using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class aecSelectionDialog :  public System::Windows::Forms::Form
{

public:
	delegate void delegate_button_callback(void);
	event delegate_button_callback^ button_canc_event;
	


public:HWND window;
public: Form^ parent;
public:System::Timers::Timer^ aecSelectionDialogTimer;

public:

private: System::Windows::Forms::PictureBox^ CancButton;
private: System::Windows::Forms::PictureBox^ OkButton;
private: System::Windows::Forms::PictureBox^ field1;
private: System::Windows::Forms::PictureBox^ field2;
private: System::Windows::Forms::PictureBox^ field3;
private: System::Windows::Forms::PictureBox^ field4;
private: System::Windows::Forms::PictureBox^ field5;
private: System::Windows::Forms::PictureBox^ field6;
private: System::Windows::Forms::PictureBox^ field7;


private: System::Windows::Forms::Label^ TitleText;












public:

public:
	
	  void formInitialization(void);
	  void open(Form^ pp);
	  void close(void);
	  bool open_status;
	  int  field_selected;

	  aecSelectionDialog(void)
	  {

		  InitializeComponent();		  
		  window = static_cast<HWND>(Handle.ToPointer());
		  formInitialization();


	  }

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~aecSelectionDialog()
	{
		if (components)
		{
			delete components;
		}

	}




private:
	/// <summary>
	/// Required designer variable.
	/// </summary>
	System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
	
private: System::Void InitializeComponent() {
	this->TitleText = (gcnew System::Windows::Forms::Label());
	this->CancButton = (gcnew System::Windows::Forms::PictureBox());
	this->OkButton = (gcnew System::Windows::Forms::PictureBox());
	this->field1 = (gcnew System::Windows::Forms::PictureBox());
	this->field2 = (gcnew System::Windows::Forms::PictureBox());
	this->field3 = (gcnew System::Windows::Forms::PictureBox());
	this->field4 = (gcnew System::Windows::Forms::PictureBox());
	this->field5 = (gcnew System::Windows::Forms::PictureBox());
	this->field6 = (gcnew System::Windows::Forms::PictureBox());
	this->field7 = (gcnew System::Windows::Forms::PictureBox());
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CancButton))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->OkButton))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field1))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field2))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field3))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field4))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field5))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field6))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field7))->BeginInit();
	this->SuspendLayout();
	// 
	// TitleText
	// 
	this->TitleText->BackColor = System::Drawing::Color::Transparent;
	this->TitleText->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 24, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->TitleText->Location = System::Drawing::Point(12, 9);
	this->TitleText->Name = L"TitleText";
	this->TitleText->Size = System::Drawing::Size(570, 42);
	this->TitleText->TabIndex = 16;
	this->TitleText->Text = L"Aec Selection";
	this->TitleText->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	// 
	// CancButton
	// 
	this->CancButton->Location = System::Drawing::Point(471, 927);
	this->CancButton->Name = L"CancButton";
	this->CancButton->Size = System::Drawing::Size(117, 85);
	this->CancButton->TabIndex = 25;
	this->CancButton->TabStop = false;
	this->CancButton->Click += gcnew System::EventHandler(this, &aecSelectionDialog::CancButton_Click);
	// 
	// OkButton
	// 
	this->OkButton->Location = System::Drawing::Point(12, 927);
	this->OkButton->Name = L"OkButton";
	this->OkButton->Size = System::Drawing::Size(117, 85);
	this->OkButton->TabIndex = 26;
	this->OkButton->TabStop = false;
	this->OkButton->Click += gcnew System::EventHandler(this, &aecSelectionDialog::OkButton_Click);
	// 
	// field1
	// 
	this->field1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->field1->Location = System::Drawing::Point(263, 658);
	this->field1->Name = L"field1";
	this->field1->Size = System::Drawing::Size(74, 37);
	this->field1->TabIndex = 27;
	this->field1->TabStop = false;
	this->field1->Click += gcnew System::EventHandler(this, &aecSelectionDialog::Field1_Click);
	// 
	// field2
	// 
	this->field2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->field2->Location = System::Drawing::Point(263, 617);
	this->field2->Name = L"field2";
	this->field2->Size = System::Drawing::Size(74, 37);
	this->field2->TabIndex = 28;
	this->field2->TabStop = false;
	this->field2->Click += gcnew System::EventHandler(this, &aecSelectionDialog::Field2_Click);
	// 
	// field3
	// 
	this->field3->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->field3->Location = System::Drawing::Point(263, 576);
	this->field3->Name = L"field3";
	this->field3->Size = System::Drawing::Size(74, 37);
	this->field3->TabIndex = 29;
	this->field3->TabStop = false;
	this->field3->Click += gcnew System::EventHandler(this, &aecSelectionDialog::Field3_Click);
	// 
	// field4
	// 
	this->field4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->field4->Location = System::Drawing::Point(263, 535);
	this->field4->Name = L"field4";
	this->field4->Size = System::Drawing::Size(74, 37);
	this->field4->TabIndex = 30;
	this->field4->TabStop = false;
	this->field4->Click += gcnew System::EventHandler(this, &aecSelectionDialog::Field4_Click);
	// 
	// field5
	// 
	this->field5->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->field5->Location = System::Drawing::Point(263, 494);
	this->field5->Name = L"field5";
	this->field5->Size = System::Drawing::Size(74, 37);
	this->field5->TabIndex = 31;
	this->field5->TabStop = false;
	this->field5->Click += gcnew System::EventHandler(this, &aecSelectionDialog::Field5_Click);
	// 
	// field6
	// 
	this->field6->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->field6->Location = System::Drawing::Point(263, 453);
	this->field6->Name = L"field6";
	this->field6->Size = System::Drawing::Size(74, 37);
	this->field6->TabIndex = 32;
	this->field6->TabStop = false;
	this->field6->Click += gcnew System::EventHandler(this, &aecSelectionDialog::Field6_Click);
	// 
	// field7
	// 
	this->field7->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->field7->Location = System::Drawing::Point(263, 412);
	this->field7->Name = L"field7";
	this->field7->Size = System::Drawing::Size(74, 37);
	this->field7->TabIndex = 33;
	this->field7->TabStop = false;
	this->field7->Click += gcnew System::EventHandler(this, &aecSelectionDialog::Field7_Click);
	// 
	// aecSelectionDialog
	// 
	this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
		static_cast<System::Int32>(static_cast<System::Byte>(224)));
	this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->ClientSize = System::Drawing::Size(600, 1024);
	this->ControlBox = false;
	this->Controls->Add(this->field7);
	this->Controls->Add(this->field6);
	this->Controls->Add(this->field5);
	this->Controls->Add(this->field4);
	this->Controls->Add(this->field3);
	this->Controls->Add(this->field2);
	this->Controls->Add(this->field1);
	this->Controls->Add(this->OkButton);
	this->Controls->Add(this->CancButton);
	this->Controls->Add(this->TitleText);
	this->Cursor = System::Windows::Forms::Cursors::Hand;
	this->DoubleBuffered = true;
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Location = System::Drawing::Point(25, 200);
	this->Name = L"aecSelectionDialog";
	this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CancButton))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->OkButton))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field1))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field2))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field3))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field4))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field5))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field6))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->field7))->EndInit();
	this->ResumeLayout(false);

}

#pragma endregion

	protected:  void WndProc(System::Windows::Forms::Message% m) override
	{
		switch (m.Msg) {

		case (WM_USER + 1): // onErrorTimeout
			field_selected = 0;
			close();
			break;

		}

		Form::WndProc(m);
	}

	private: System::Void onAecTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		
		if (!SendMessageA(window, WM_USER + 1, 0, 0)) {
			int val = GetLastError();
			LogClass::logInFile("CAN OPEN ATTEMPT: " + System::Convert::ToString(val));
		}
	}

	private: System::Void CancButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void OkButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Field1_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Field2_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Field3_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Field4_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Field5_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Field6_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Field7_Click(System::Object^ sender, System::EventArgs^ e);

};

	