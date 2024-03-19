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


public ref class KeyPaddleWindow :  public System::Windows::Forms::Form
{

public:
	delegate void delegate_button_callback(System::String^ value);
	event delegate_button_callback^ button_ok_event;

public:HWND window;
private: System::Windows::Forms::Label^ InfoTitleText;
private: System::Windows::Forms::PictureBox^ k7;
private: System::Windows::Forms::PictureBox^ k8;
private: System::Windows::Forms::PictureBox^ k9;
private: System::Windows::Forms::PictureBox^ kb;




private: System::Windows::Forms::PictureBox^ k4;
private: System::Windows::Forms::PictureBox^ k5;
private: System::Windows::Forms::PictureBox^ k6;
private: System::Windows::Forms::PictureBox^ k1;
private: System::Windows::Forms::PictureBox^ k2;
private: System::Windows::Forms::PictureBox^ k3;
private: System::Windows::Forms::PictureBox^ k0;
private: System::Windows::Forms::PictureBox^ kp;
private: System::Windows::Forms::PictureBox^ kok;




private: System::Windows::Forms::Label^ InfoContentText;

public:

public:
	void open(void);
	void close(void);
	bool open_status;
	
	  Form^ parent;
	  System::String^ result;

	  void formInitialization(System::String^ info_title, System::String^ info_content);
	 

	  KeyPaddleWindow(Form^ pp, System::String^ info_title, System::String^ info_content)
	  {
		  parent = pp;
		  InitializeComponent();
		 
		  window = static_cast<HWND>(Handle.ToPointer());
		  formInitialization(info_title, info_content);


	  }

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~KeyPaddleWindow()
	{
		if (components)
		{
			delete components;
		}

	}




private: System::Windows::Forms::Panel^ CancButton;







private:
	/// <summary>
	/// Required designer variable.
	/// </summary>
	System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
	
private: System::Void InitializeComponent() {
	this->CancButton = (gcnew System::Windows::Forms::Panel());
	this->InfoTitleText = (gcnew System::Windows::Forms::Label());
	this->InfoContentText = (gcnew System::Windows::Forms::Label());
	this->k7 = (gcnew System::Windows::Forms::PictureBox());
	this->k8 = (gcnew System::Windows::Forms::PictureBox());
	this->k9 = (gcnew System::Windows::Forms::PictureBox());
	this->kb = (gcnew System::Windows::Forms::PictureBox());
	this->k4 = (gcnew System::Windows::Forms::PictureBox());
	this->k5 = (gcnew System::Windows::Forms::PictureBox());
	this->k6 = (gcnew System::Windows::Forms::PictureBox());
	this->k1 = (gcnew System::Windows::Forms::PictureBox());
	this->k2 = (gcnew System::Windows::Forms::PictureBox());
	this->k3 = (gcnew System::Windows::Forms::PictureBox());
	this->k0 = (gcnew System::Windows::Forms::PictureBox());
	this->kp = (gcnew System::Windows::Forms::PictureBox());
	this->kok = (gcnew System::Windows::Forms::PictureBox());
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k7))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k8))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k9))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->kb))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k4))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k5))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k6))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k1))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k2))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k3))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k0))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->kp))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->kok))->BeginInit();
	this->SuspendLayout();
	// 
	// CancButton
	// 
	this->CancButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->CancButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->CancButton->Location = System::Drawing::Point(512, 935);
	this->CancButton->Name = L"CancButton";
	this->CancButton->Size = System::Drawing::Size(70, 70);
	this->CancButton->TabIndex = 31;
	this->CancButton->Click += gcnew System::EventHandler(this, &KeyPaddleWindow::CancButton_Click);
	// 
	// InfoTitleText
	// 
	this->InfoTitleText->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 24, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->InfoTitleText->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
		static_cast<System::Int32>(static_cast<System::Byte>(64)));
	this->InfoTitleText->Location = System::Drawing::Point(12, 9);
	this->InfoTitleText->Name = L"InfoTitleText";
	this->InfoTitleText->Size = System::Drawing::Size(576, 59);
	this->InfoTitleText->TabIndex = 36;
	this->InfoTitleText->Text = L"mAs Selection";
	this->InfoTitleText->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	// 
	// InfoContentText
	// 
	this->InfoContentText->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 48, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->InfoContentText->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
		static_cast<System::Int32>(static_cast<System::Byte>(64)));
	this->InfoContentText->Location = System::Drawing::Point(68, 188);
	this->InfoContentText->Name = L"InfoContentText";
	this->InfoContentText->Size = System::Drawing::Size(463, 104);
	this->InfoContentText->TabIndex = 37;
	this->InfoContentText->Text = L"1234.67";
	this->InfoContentText->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	// 
	// k7
	// 
	this->k7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->k7->Location = System::Drawing::Point(70, 337);
	this->k7->Name = L"k7";
	this->k7->Size = System::Drawing::Size(100, 100);
	this->k7->TabIndex = 38;
	this->k7->TabStop = false;
	this->k7->Click += gcnew System::EventHandler(this, &KeyPaddleWindow::k7_Click);
	// 
	// k8
	// 
	this->k8->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->k8->Location = System::Drawing::Point(190, 337);
	this->k8->Name = L"k8";
	this->k8->Size = System::Drawing::Size(100, 100);
	this->k8->TabIndex = 39;
	this->k8->TabStop = false;
	this->k8->Click += gcnew System::EventHandler(this, &KeyPaddleWindow::k8_Click);
	// 
	// k9
	// 
	this->k9->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->k9->Location = System::Drawing::Point(310, 337);
	this->k9->Name = L"k9";
	this->k9->Size = System::Drawing::Size(100, 100);
	this->k9->TabIndex = 40;
	this->k9->TabStop = false;
	this->k9->Click += gcnew System::EventHandler(this, &KeyPaddleWindow::k9_Click);
	// 
	// kb
	// 
	this->kb->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->kb->Location = System::Drawing::Point(430, 337);
	this->kb->Name = L"kb";
	this->kb->Size = System::Drawing::Size(100, 100);
	this->kb->TabIndex = 41;
	this->kb->TabStop = false;
	// 
	// k4
	// 
	this->k4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->k4->Location = System::Drawing::Point(70, 457);
	this->k4->Name = L"k4";
	this->k4->Size = System::Drawing::Size(100, 100);
	this->k4->TabIndex = 42;
	this->k4->TabStop = false;
	this->k4->Click += gcnew System::EventHandler(this, &KeyPaddleWindow::k4_Click);
	// 
	// k5
	// 
	this->k5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->k5->Location = System::Drawing::Point(190, 457);
	this->k5->Name = L"k5";
	this->k5->Size = System::Drawing::Size(100, 100);
	this->k5->TabIndex = 43;
	this->k5->TabStop = false;
	this->k5->Click += gcnew System::EventHandler(this, &KeyPaddleWindow::k5_Click);
	// 
	// k6
	// 
	this->k6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->k6->Location = System::Drawing::Point(310, 457);
	this->k6->Name = L"k6";
	this->k6->Size = System::Drawing::Size(100, 100);
	this->k6->TabIndex = 44;
	this->k6->TabStop = false;
	this->k6->Click += gcnew System::EventHandler(this, &KeyPaddleWindow::k6_Click);
	// 
	// k1
	// 
	this->k1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->k1->Location = System::Drawing::Point(70, 577);
	this->k1->Name = L"k1";
	this->k1->Size = System::Drawing::Size(100, 100);
	this->k1->TabIndex = 45;
	this->k1->TabStop = false;
	this->k1->Click += gcnew System::EventHandler(this, &KeyPaddleWindow::k1_Click);
	// 
	// k2
	// 
	this->k2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->k2->Location = System::Drawing::Point(190, 577);
	this->k2->Name = L"k2";
	this->k2->Size = System::Drawing::Size(100, 100);
	this->k2->TabIndex = 46;
	this->k2->TabStop = false;
	this->k2->Click += gcnew System::EventHandler(this, &KeyPaddleWindow::k2_Click);
	// 
	// k3
	// 
	this->k3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->k3->Location = System::Drawing::Point(310, 577);
	this->k3->Name = L"k3";
	this->k3->Size = System::Drawing::Size(100, 100);
	this->k3->TabIndex = 47;
	this->k3->TabStop = false;
	this->k3->Click += gcnew System::EventHandler(this, &KeyPaddleWindow::k3_Click);
	// 
	// k0
	// 
	this->k0->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->k0->Location = System::Drawing::Point(70, 704);
	this->k0->Name = L"k0";
	this->k0->Size = System::Drawing::Size(220, 100);
	this->k0->TabIndex = 48;
	this->k0->TabStop = false;
	this->k0->Click += gcnew System::EventHandler(this, &KeyPaddleWindow::k0_Click);
	// 
	// kp
	// 
	this->kp->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->kp->Location = System::Drawing::Point(310, 704);
	this->kp->Name = L"kp";
	this->kp->Size = System::Drawing::Size(100, 100);
	this->kp->TabIndex = 49;
	this->kp->TabStop = false;
	this->kp->Click += gcnew System::EventHandler(this, &KeyPaddleWindow::kp_Click);
	// 
	// kok
	// 
	this->kok->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->kok->Location = System::Drawing::Point(430, 457);
	this->kok->Name = L"kok";
	this->kok->Size = System::Drawing::Size(100, 347);
	this->kok->TabIndex = 50;
	this->kok->TabStop = false;
	this->kok->Click += gcnew System::EventHandler(this, &KeyPaddleWindow::OkButton_Click);	
	// 
	// KeyPaddleWindow
	// 
	this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
		static_cast<System::Int32>(static_cast<System::Byte>(224)));
	this->ClientSize = System::Drawing::Size(600, 1024);
	this->ControlBox = false;
	this->Controls->Add(this->kok);
	this->Controls->Add(this->kp);
	this->Controls->Add(this->k0);
	this->Controls->Add(this->k3);
	this->Controls->Add(this->k2);
	this->Controls->Add(this->k1);
	this->Controls->Add(this->k6);
	this->Controls->Add(this->k5);
	this->Controls->Add(this->k4);
	this->Controls->Add(this->kb);
	this->Controls->Add(this->k9);
	this->Controls->Add(this->k8);
	this->Controls->Add(this->k7);
	this->Controls->Add(this->InfoContentText);
	this->Controls->Add(this->CancButton);
	this->Controls->Add(this->InfoTitleText);
	this->Cursor = System::Windows::Forms::Cursors::Hand;
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Location = System::Drawing::Point(25, 200);
	this->Name = L"KeyPaddleWindow";
	this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k7))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k8))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k9))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->kb))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k4))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k5))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k6))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k1))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k2))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k3))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->k0))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->kp))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->kok))->EndInit();
	this->ResumeLayout(false);

}

#pragma endregion


private: System::Void CancButton_Click(System::Object^ sender, System::EventArgs^ e) {
	close();
	
	
}
private: System::Void OkButton_Click(System::Object^ sender, System::EventArgs^ e) {
	close();
	button_ok_event(result);
	
}
	 

private: System::Void k1_Click(System::Object^ sender, System::EventArgs^ e) {
	result += "1";
	InfoContentText->Text = result;
}
private: System::Void k2_Click(System::Object^ sender, System::EventArgs^ e) {
	result += "2";
	InfoContentText->Text = result;
}
private: System::Void k3_Click(System::Object^ sender, System::EventArgs^ e) {
	result += "3";
	InfoContentText->Text = result;
}
private: System::Void k4_Click(System::Object^ sender, System::EventArgs^ e) {
	result += "4";
	InfoContentText->Text = result;
}
private: System::Void k5_Click(System::Object^ sender, System::EventArgs^ e) {
	result += "5";
	InfoContentText->Text = result;
}
private: System::Void k6_Click(System::Object^ sender, System::EventArgs^ e) {
	result += "6";
	InfoContentText->Text = result;
}
private: System::Void k7_Click(System::Object^ sender, System::EventArgs^ e) {
	result += "7";
	InfoContentText->Text = result;
}
private: System::Void k8_Click(System::Object^ sender, System::EventArgs^ e) {
	result += "8";
	InfoContentText->Text = result;
}
private: System::Void k9_Click(System::Object^ sender, System::EventArgs^ e) {
	result += "9";
	InfoContentText->Text = result;
}
private: System::Void k0_Click(System::Object^ sender, System::EventArgs^ e) {
	result += "0";
	InfoContentText->Text = result;
}
private: System::Void kp_Click(System::Object^ sender, System::EventArgs^ e) {
	result += ".";
	InfoContentText->Text = result;
}

};

