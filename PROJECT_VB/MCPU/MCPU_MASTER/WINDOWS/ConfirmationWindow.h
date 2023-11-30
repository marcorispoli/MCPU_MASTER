#pragma once
#include <Windows.h>


using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


public ref class ConfirmationWindow :  public System::Windows::Forms::Form
{

public:
	delegate void delegate_button_callback(void);
	event delegate_button_callback^ button_canc_event;
	event delegate_button_callback^ button_ok_event;

public:HWND window;
private: System::Windows::Forms::Label^ InfoTitleText;
private: System::Windows::Forms::Label^ InfoContentText;
private: System::Windows::Forms::Panel^ mainPanel;
public:

public:
	void open(void);
	void close(void);
	bool open_status;
	

	   enum class InfoType {
		  INF_WIN = 0,
		  WRN_WIN,
		  ERR_WIN
	  };
	  InfoType wType;
	  Form^ parent;

	  void formInitialization(InfoType wtype, System::String^ info_title, System::String^ info_content);
	  void setContentBackground(Image^ img);

	  ConfirmationWindow(Form^ pp, InfoType wtype, System::String^ info_title, System::String^ info_content)
	  {
		  parent = pp;
		  InitializeComponent();
		  wType = wtype;
		  window = static_cast<HWND>(Handle.ToPointer());
		  formInitialization(wtype, info_title, info_content);


	  }

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~ConfirmationWindow()
	{
		if (components)
		{
			delete components;
		}

	}



protected:


protected:

protected:

protected:

protected:





private: System::Windows::Forms::Panel^ CancButton;
private: System::Windows::Forms::Panel^ OkButton;

private: System::Windows::Forms::Panel^ IconPanel;
private: System::Windows::Forms::Panel^ ContentPanel;











private:
	/// <summary>
	/// Required designer variable.
	/// </summary>
	System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
	
private: System::Void InitializeComponent() {
	this->CancButton = (gcnew System::Windows::Forms::Panel());
	this->OkButton = (gcnew System::Windows::Forms::Panel());
	this->IconPanel = (gcnew System::Windows::Forms::Panel());
	this->ContentPanel = (gcnew System::Windows::Forms::Panel());
	this->InfoTitleText = (gcnew System::Windows::Forms::Label());
	this->InfoContentText = (gcnew System::Windows::Forms::Label());
	this->mainPanel = (gcnew System::Windows::Forms::Panel());
	this->mainPanel->SuspendLayout();
	this->SuspendLayout();
	// 
	// CancButton
	// 
	this->CancButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->CancButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->CancButton->Location = System::Drawing::Point(404, 238);
	this->CancButton->Name = L"CancButton";
	this->CancButton->Size = System::Drawing::Size(80, 80);
	this->CancButton->TabIndex = 31;
	this->CancButton->Click += gcnew System::EventHandler(this, &ConfirmationWindow::CancButton_Click);
	// 
	// OkButton
	// 
	this->OkButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
		static_cast<System::Int32>(static_cast<System::Byte>(128)));
	this->OkButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->OkButton->Location = System::Drawing::Point(290, 238);
	this->OkButton->Name = L"OkButton";
	this->OkButton->Size = System::Drawing::Size(80, 80);
	this->OkButton->TabIndex = 30;
	this->OkButton->Click += gcnew System::EventHandler(this, &ConfirmationWindow::OkButton_Click);
	// 
	// IconPanel
	// 
	this->IconPanel->BackColor = System::Drawing::Color::Black;
	this->IconPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->IconPanel->Location = System::Drawing::Point(23, 20);
	this->IconPanel->Name = L"IconPanel";
	this->IconPanel->Size = System::Drawing::Size(80, 80);
	this->IconPanel->TabIndex = 35;
	// 
	// ContentPanel
	// 
	this->ContentPanel->BackColor = System::Drawing::Color::DimGray;
	this->ContentPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->ContentPanel->Location = System::Drawing::Point(23, 118);
	this->ContentPanel->Name = L"ContentPanel";
	this->ContentPanel->Size = System::Drawing::Size(230, 200);
	this->ContentPanel->TabIndex = 34;
	// 
	// InfoTitleText
	// 
	this->InfoTitleText->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->InfoTitleText->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
		static_cast<System::Int32>(static_cast<System::Byte>(64)));
	this->InfoTitleText->Location = System::Drawing::Point(116, 20);
	this->InfoTitleText->Name = L"InfoTitleText";
	this->InfoTitleText->Size = System::Drawing::Size(394, 30);
	this->InfoTitleText->TabIndex = 36;
	this->InfoTitleText->Text = L"Projection Selection Window [1]";
	this->InfoTitleText->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
	// 
	// InfoContentText
	// 
	this->InfoContentText->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular,
		System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
	this->InfoContentText->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
		static_cast<System::Int32>(static_cast<System::Byte>(64)));
	this->InfoContentText->Location = System::Drawing::Point(116, 49);
	this->InfoContentText->Name = L"InfoContentText";
	this->InfoContentText->Size = System::Drawing::Size(385, 51);
	this->InfoContentText->TabIndex = 37;
	this->InfoContentText->Text = L"Projection Selection Window [1] 1234m";
	// 
	// mainPanel
	// 
	this->mainPanel->BackColor = System::Drawing::Color::Transparent;
	this->mainPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->mainPanel->Controls->Add(this->IconPanel);
	this->mainPanel->Controls->Add(this->OkButton);
	this->mainPanel->Controls->Add(this->CancButton);
	this->mainPanel->Controls->Add(this->InfoTitleText);
	this->mainPanel->Controls->Add(this->InfoContentText);
	this->mainPanel->Controls->Add(this->ContentPanel);
	this->mainPanel->Location = System::Drawing::Point(0, 0);
	this->mainPanel->Name = L"mainPanel";
	this->mainPanel->Size = System::Drawing::Size(513, 340);
	this->mainPanel->TabIndex = 38;
	// 
	// ConfirmationWindow
	// 
	this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
		static_cast<System::Int32>(static_cast<System::Byte>(224)));
	this->ClientSize = System::Drawing::Size(513, 340);
	this->ControlBox = false;
	this->Controls->Add(this->mainPanel);
	this->Cursor = System::Windows::Forms::Cursors::Hand;
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Location = System::Drawing::Point(25, 200);
	this->Name = L"ConfirmationWindow";
	this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
	this->mainPanel->ResumeLayout(false);
	this->ResumeLayout(false);

}

#pragma endregion


private: System::Void CancButton_Click(System::Object^ sender, System::EventArgs^ e) {
	button_canc_event();
	
}
private: System::Void OkButton_Click(System::Object^ sender, System::EventArgs^ e) {
	button_ok_event();
	
}


};

