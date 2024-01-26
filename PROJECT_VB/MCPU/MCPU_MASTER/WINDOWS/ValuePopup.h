#pragma once
#include <Windows.h>


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

public ref class ValuePopupForm :  public System::Windows::Forms::Form
{

public:
	
public:HWND window;
public: Form^ parent;

private: System::Windows::Forms::PictureBox^ valueIcon; 
private: System::Windows::Forms::Label^ valueTitle;
private: System::Windows::Forms::Label^ valueUnit;
private: System::Windows::Forms::Label^ valueContent;

public:
	
	  void formInitialization(void);
	  void open(Form^ parent_form, Image^ icon, System::String^ title, System::String^ unit);
	  void retitle(Image^ icon, System::String^ title, System::String^ unit);
	  void close(void);
	  void content(System::String^ val);
	  bool open_status;

	  ValuePopupForm(void)
	  {
		  //parent = pp;
		  InitializeComponent();		  
		  window = static_cast<HWND>(Handle.ToPointer());
		  formInitialization();
	  }

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~ValuePopupForm()
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
	this->valueTitle = (gcnew System::Windows::Forms::Label());
	this->valueUnit = (gcnew System::Windows::Forms::Label());
	this->valueContent = (gcnew System::Windows::Forms::Label());
	this->valueIcon = (gcnew System::Windows::Forms::PictureBox());
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->valueIcon))->BeginInit();
	this->SuspendLayout();
	// 
	// valueTitle
	// 
	this->valueTitle->BackColor = System::Drawing::Color::Transparent;
	this->valueTitle->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 21.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->valueTitle->Location = System::Drawing::Point(147, 22);
	this->valueTitle->Name = L"valueTitle";
	this->valueTitle->Size = System::Drawing::Size(354, 37);
	this->valueTitle->TabIndex = 18;
	this->valueTitle->Text = L"Title";
	this->valueTitle->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
	// 
	// valueUnit
	// 
	this->valueUnit->BackColor = System::Drawing::Color::Transparent;
	this->valueUnit->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 24, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->valueUnit->Location = System::Drawing::Point(147, 75);
	this->valueUnit->Name = L"valueUnit";
	this->valueUnit->Size = System::Drawing::Size(106, 41);
	this->valueUnit->TabIndex = 19;
	this->valueUnit->Text = L"(unit)";
	this->valueUnit->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
	// 
	// valueContent
	// 
	this->valueContent->BackColor = System::Drawing::Color::Transparent;
	this->valueContent->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 99.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->valueContent->Location = System::Drawing::Point(12, 150);
	this->valueContent->Name = L"valueContent";
	this->valueContent->Size = System::Drawing::Size(489, 181);
	this->valueContent->TabIndex = 19;
	this->valueContent->Text = L"-170.5";
	this->valueContent->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	// 
	// valueIcon
	// 
	this->valueIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->valueIcon->Location = System::Drawing::Point(12, 12);
	this->valueIcon->Name = L"valueIcon";
	this->valueIcon->Size = System::Drawing::Size(120, 120);
	this->valueIcon->TabIndex = 22;
	this->valueIcon->TabStop = false;
	// 
	// ValuePopupForm
	// 
	this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
		static_cast<System::Int32>(static_cast<System::Byte>(224)));
	this->ClientSize = System::Drawing::Size(513, 340);
	this->ControlBox = false;
	this->Controls->Add(this->valueTitle);
	this->Controls->Add(this->valueIcon);
	this->Controls->Add(this->valueUnit);
	this->Controls->Add(this->valueContent);
	this->Cursor = System::Windows::Forms::Cursors::Hand;
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Location = System::Drawing::Point(25, 200);
	this->Name = L"ValuePopupForm";
	this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->valueIcon))->EndInit();
	this->ResumeLayout(false);

}

#pragma endregion

	protected:  void WndProc(System::Windows::Forms::Message% m) override
	{
		switch (m.Msg) {

		case (WM_USER + 1): 
			break;

		}

		Form::WndProc(m);
	}


};

	