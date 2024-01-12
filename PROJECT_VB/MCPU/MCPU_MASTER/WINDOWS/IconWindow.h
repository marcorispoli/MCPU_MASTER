#pragma once
#include <Windows.h>

/// <summary>
/// \defgroup ConfirmationGUI_Module Confirmation Window Management Module
/// \ingroup GUI_Module Gantry GUI modules
/// 
/// 
/// </summary>
/// 


using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


public ref class IconWindow :  public System::Windows::Forms::Form
{

public:
	

public:HWND window;
	void open(void);
	void close(void);
	bool open_status;
	
	Form^ parent;
		

	  void formInitialization(Image^ img);
	  void setContentBackground(Image^ img);

	  IconWindow(Form^ pp, Image^ img)
	  {
		  parent = pp;
		  InitializeComponent();
		  window = static_cast<HWND>(Handle.ToPointer());
		  formInitialization(img);
	  }
private: System::Windows::Forms::Panel^ mainPanel;
protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~IconWindow()
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
	this->mainPanel = (gcnew System::Windows::Forms::Panel());
	this->mainPanel->SuspendLayout();
	this->SuspendLayout();

	// 
	// mainPanel
	// 
	this->mainPanel->BackColor = System::Drawing::Color::Transparent;
	this->mainPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;	
	this->mainPanel->Location = System::Drawing::Point(0, 0);
	this->mainPanel->Name = L"mainPanel";
	this->mainPanel->Size = System::Drawing::Size(513, 340);
	this->mainPanel->TabIndex = 0;
	// 
	// IconWindow
	// 
	this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
		static_cast<System::Int32>(static_cast<System::Byte>(224)));
	this->ClientSize = System::Drawing::Size(513, 340);
	this->ControlBox = false;
	this->Controls->Add(this->mainPanel);
	this->Cursor = System::Windows::Forms::Cursors::Hand;
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Location = System::Drawing::Point(25, 200);
	this->Name = L"IconWindow";
	this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
	this->mainPanel->ResumeLayout(false);
	this->ResumeLayout(false);

}

#pragma endregion


};

