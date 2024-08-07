#pragma once
#include <Windows.h>
#include "Log.h"

/// <summary>
/// \defgroup ErrorGUI_Module Error Window Management Module
/// \ingroup GUI_Module Gantry GUI modules
/// 
/// 
/// </summary>
/// 


using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class ErrorForm :  public System::Windows::Forms::Form
{

public:
	delegate void delegate_button_callback(void);
	event delegate_button_callback^ button_canc_event;
	


public:HWND window;
public: Form^ parent;
public:System::Timers::Timer^ errorPanelTimer;

private: System::Windows::Forms::Label^ TitleText;


private: System::Windows::Forms::RichTextBox^ errorContent;
private: System::Windows::Forms::Panel^ notifyIcon;



private: System::Windows::Forms::Button^ CancButton;



public:

public:
	
	  void formInitialization(void);
	  void open(Form^ pp);
	  void close(void);
	  bool open_status;

	  ErrorForm(void)
	  {

		  InitializeComponent();		  
		  window = static_cast<HWND>(Handle.ToPointer());
		  formInitialization();


	  }

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~ErrorForm()
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
	this->CancButton = (gcnew System::Windows::Forms::Button());
	this->errorContent = (gcnew System::Windows::Forms::RichTextBox());
	this->notifyIcon = (gcnew System::Windows::Forms::Panel());
	this->SuspendLayout();
	// 
	// TitleText
	// 
	this->TitleText->BackColor = System::Drawing::Color::Transparent;
	this->TitleText->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 24, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->TitleText->Location = System::Drawing::Point(15, 24);
	this->TitleText->Name = L"TitleText";
	this->TitleText->Size = System::Drawing::Size(570, 42);
	this->TitleText->TabIndex = 16;
	this->TitleText->Text = L"Error Panel";
	this->TitleText->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
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
	this->CancButton->Location = System::Drawing::Point(489, 936);
	this->CancButton->Name = L"CancButton";
	this->CancButton->Size = System::Drawing::Size(99, 85);
	this->CancButton->TabIndex = 17;
	this->CancButton->UseVisualStyleBackColor = false;
	this->CancButton->Click += gcnew System::EventHandler(this, &ErrorForm::CancButton_Click);
	// 
	// errorContent
	// 
	this->errorContent->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)), static_cast<System::Int32>(static_cast<System::Byte>(207)),
		static_cast<System::Int32>(static_cast<System::Byte>(208)));
	this->errorContent->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
	this->errorContent->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->errorContent->Location = System::Drawing::Point(40, 251);
	this->errorContent->Name = L"errorContent";
	this->errorContent->ReadOnly = true;
	this->errorContent->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
	this->errorContent->Size = System::Drawing::Size(520, 636);
	this->errorContent->TabIndex = 20;
	this->errorContent->Text = L"";
	// 
	// notifyIcon
	// 
	this->notifyIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->notifyIcon->Location = System::Drawing::Point(40, 122);
	this->notifyIcon->Name = L"notifyIcon";
	this->notifyIcon->Size = System::Drawing::Size(109, 103);
	this->notifyIcon->TabIndex = 21;
	// 
	// ErrorForm
	// 
	this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
		static_cast<System::Int32>(static_cast<System::Byte>(224)));
	this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->ClientSize = System::Drawing::Size(600, 1024);
	this->ControlBox = false;
	this->Controls->Add(this->notifyIcon);
	this->Controls->Add(this->errorContent);
	this->Controls->Add(this->CancButton);
	this->Controls->Add(this->TitleText);
	this->Cursor = System::Windows::Forms::Cursors::Hand;
	this->DoubleBuffered = true;
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Location = System::Drawing::Point(25, 200);
	this->Name = L"ErrorForm";
	this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
	this->ResumeLayout(false);

}

#pragma endregion

	protected:  void WndProc(System::Windows::Forms::Message% m) override
	{
		switch (m.Msg) {

		case (WM_USER + 1): // onErrorTimeout
			close();
			break;

		}

		Form::WndProc(m);
	}

	private: System::Void onErrorTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		
		if (!SendMessageA(window, WM_USER + 1, 0, 0)) {
			int val = GetLastError();
			LogClass::logInFile("CAN OPEN ATTEMPT: " + System::Convert::ToString(val));
		}
	}

	private: System::Void CancButton_Click(System::Object^ sender, System::EventArgs^ e);

};

	