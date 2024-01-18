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

public ref class infoPopupForm :  public System::Windows::Forms::Form
{

public:
	
public:HWND window;
public: Form^ parent;
public:System::Timers::Timer^ infoPanelTimer;


private: System::Windows::Forms::Label^ infoId;
private: System::Windows::Forms::Label^ infoTitle;
private: System::Windows::Forms::RichTextBox^ infoContent;
private: System::Windows::Forms::PictureBox^ cancButton;

private: System::Windows::Forms::Panel^ infoIcon;


public:
	
	  void formInitialization(void);
	  void open(Form^ parent_form, int msg, System::String^ extra);
	  void close(void);
	  bool open_status;

	  infoPopupForm(void)
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
	~infoPopupForm()
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
	this->infoId = (gcnew System::Windows::Forms::Label());
	this->infoTitle = (gcnew System::Windows::Forms::Label());
	this->infoContent = (gcnew System::Windows::Forms::RichTextBox());
	this->infoIcon = (gcnew System::Windows::Forms::Panel());
	this->cancButton = (gcnew System::Windows::Forms::PictureBox());
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->cancButton))->BeginInit();
	this->SuspendLayout();
	// 
	// infoId
	// 
	this->infoId->AutoSize = true;
	this->infoId->BackColor = System::Drawing::Color::Transparent;
	this->infoId->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 24, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->infoId->Location = System::Drawing::Point(157, 32);
	this->infoId->Name = L"infoId";
	this->infoId->Size = System::Drawing::Size(119, 37);
	this->infoId->TabIndex = 18;
	this->infoId->Text = L"I00001";
	// 
	// infoTitle
	// 
	this->infoTitle->BackColor = System::Drawing::Color::Transparent;
	this->infoTitle->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->infoTitle->Location = System::Drawing::Point(157, 69);
	this->infoTitle->Name = L"infoTitle";
	this->infoTitle->Size = System::Drawing::Size(331, 29);
	this->infoTitle->TabIndex = 19;
	this->infoTitle->Text = L"Title of info message";
	this->infoTitle->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
	// 
	// infoContent
	// 
	this->infoContent->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(216)), static_cast<System::Int32>(static_cast<System::Byte>(207)),
		static_cast<System::Int32>(static_cast<System::Byte>(208)));
	this->infoContent->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
	this->infoContent->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->infoContent->Location = System::Drawing::Point(42, 152);
	this->infoContent->Name = L"infoContent";
	this->infoContent->ReadOnly = true;
	this->infoContent->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
	this->infoContent->Size = System::Drawing::Size(436, 100);
	this->infoContent->TabIndex = 20;
	this->infoContent->Text = L"";
	// 
	// infoIcon
	// 
	this->infoIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->infoIcon->Location = System::Drawing::Point(42, 32);
	this->infoIcon->Name = L"infoIcon";
	this->infoIcon->Size = System::Drawing::Size(109, 103);
	this->infoIcon->TabIndex = 21;
	// 
	// cancButton
	// 
	this->cancButton->Location = System::Drawing::Point(410, 260);
	this->cancButton->Name = L"cancButton";
	this->cancButton->Size = System::Drawing::Size(68, 68);
	this->cancButton->TabIndex = 22;
	this->cancButton->TabStop = false;
	this->cancButton->Click += gcnew System::EventHandler(this, &infoPopupForm::cancButton_Click);
	// 
	// infoPopupForm
	// 
	this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
		static_cast<System::Int32>(static_cast<System::Byte>(224)));
	this->ClientSize = System::Drawing::Size(513, 340);
	this->ControlBox = false;
	this->Controls->Add(this->cancButton);
	this->Controls->Add(this->infoIcon);
	this->Controls->Add(this->infoContent);
	this->Controls->Add(this->infoTitle);
	this->Controls->Add(this->infoId);
	this->Cursor = System::Windows::Forms::Cursors::Hand;
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Location = System::Drawing::Point(25, 200);
	this->Name = L"infoPopupForm";
	this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->cancButton))->EndInit();
	this->ResumeLayout(false);
	this->PerformLayout();

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

	private: System::Void onInfoTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{		
		SendNotifyMessageA(window, WM_USER + 1, 0, 0);
	}


private: System::Void cancButton_Click(System::Object^ sender, System::EventArgs^ e) {
	SendNotifyMessageA(window, WM_USER + 1, 0, 0);
}
};

	