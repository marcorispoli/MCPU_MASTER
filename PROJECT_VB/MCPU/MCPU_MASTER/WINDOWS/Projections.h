#pragma once
#include <Windows.h>


using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class ProjectionForm :  public System::Windows::Forms::Form
{

public:
	delegate void delegate_button_callback(void);
	event delegate_button_callback^ button_canc_event;
	event delegate_button_callback^ button_ok_event;


public:HWND window;
public: Object^ pConf;
public: Form^ parent;


private: System::Windows::Forms::Panel^ SelectionPanel;
public:

public:
private: System::Windows::Forms::Panel^ proj8;
private: System::Windows::Forms::Panel^ proj7;
private: System::Windows::Forms::Panel^ proj6;
private: System::Windows::Forms::Panel^ proj5;
private: System::Windows::Forms::Panel^ proj4;
private: System::Windows::Forms::Panel^ proj3;
private: System::Windows::Forms::Panel^ proj2;
private: System::Windows::Forms::Panel^ proj1;


private: System::Windows::Forms::Label^ TitleText;


private: System::Windows::Forms::Button^ CancButton;
private: System::Windows::Forms::Button^ PrevButton;
private: System::Windows::Forms::Button^ NextButton;








private: System::Windows::Forms::Button^ OkButton;






public:

public:
	void open(void);
	void close(void);
	bool open_status;

	void onConfirmOk(void);
	void onConfirmCanc(void);
	void loadProjections(void);
	System::String^ selectedProjection;
	cli::array<System::String^>^ proj_name;
	

	   enum class InfoType {
		  INF_WIN = 0,
		  WRN_WIN,
		  ERR_WIN
	  };
	  InfoType wType;
	  int panelNumber;


	  void formInitialization(void);
	  

	  ProjectionForm(Form^ pp)
	  {
		  parent = pp;
		  InitializeComponent();		  
		  window = static_cast<HWND>(Handle.ToPointer());
		  formInitialization();


	  }

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~ProjectionForm()
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





















private:
	/// <summary>
	/// Required designer variable.
	/// </summary>
	System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
	
private: System::Void InitializeComponent() {
	this->SelectionPanel = (gcnew System::Windows::Forms::Panel());
	this->proj8 = (gcnew System::Windows::Forms::Panel());
	this->proj7 = (gcnew System::Windows::Forms::Panel());
	this->proj6 = (gcnew System::Windows::Forms::Panel());
	this->proj5 = (gcnew System::Windows::Forms::Panel());
	this->proj4 = (gcnew System::Windows::Forms::Panel());
	this->proj3 = (gcnew System::Windows::Forms::Panel());
	this->proj2 = (gcnew System::Windows::Forms::Panel());
	this->proj1 = (gcnew System::Windows::Forms::Panel());
	this->TitleText = (gcnew System::Windows::Forms::Label());
	this->CancButton = (gcnew System::Windows::Forms::Button());
	this->OkButton = (gcnew System::Windows::Forms::Button());
	this->PrevButton = (gcnew System::Windows::Forms::Button());
	this->NextButton = (gcnew System::Windows::Forms::Button());
	this->SelectionPanel->SuspendLayout();
	this->SuspendLayout();
	// 
	// SelectionPanel
	// 
	this->SelectionPanel->Controls->Add(this->proj8);
	this->SelectionPanel->Controls->Add(this->proj7);
	this->SelectionPanel->Controls->Add(this->proj6);
	this->SelectionPanel->Controls->Add(this->proj5);
	this->SelectionPanel->Controls->Add(this->proj4);
	this->SelectionPanel->Controls->Add(this->proj3);
	this->SelectionPanel->Controls->Add(this->proj2);
	this->SelectionPanel->Controls->Add(this->proj1);
	this->SelectionPanel->Location = System::Drawing::Point(0, 90);
	this->SelectionPanel->Name = L"SelectionPanel";
	this->SelectionPanel->Size = System::Drawing::Size(600, 844);
	this->SelectionPanel->TabIndex = 37;
	// 
	// proj8
	// 
	this->proj8->BackColor = System::Drawing::Color::IndianRed;
	this->proj8->Location = System::Drawing::Point(323, 634);
	this->proj8->Name = L"proj8";
	this->proj8->Size = System::Drawing::Size(230, 200);
	this->proj8->TabIndex = 36;
	this->proj8->Click += gcnew System::EventHandler(this, &ProjectionForm::proj8_Click);
	// 
	// proj7
	// 
	this->proj7->BackColor = System::Drawing::Color::IndianRed;
	this->proj7->Location = System::Drawing::Point(47, 634);
	this->proj7->Name = L"proj7";
	this->proj7->Size = System::Drawing::Size(230, 200);
	this->proj7->TabIndex = 40;
	this->proj7->Click += gcnew System::EventHandler(this, &ProjectionForm::proj7_Click);
	// 
	// proj6
	// 
	this->proj6->BackColor = System::Drawing::Color::IndianRed;
	this->proj6->Location = System::Drawing::Point(323, 426);
	this->proj6->Name = L"proj6";
	this->proj6->Size = System::Drawing::Size(230, 200);
	this->proj6->TabIndex = 39;
	this->proj6->Click += gcnew System::EventHandler(this, &ProjectionForm::proj6_Click);
	// 
	// proj5
	// 
	this->proj5->BackColor = System::Drawing::Color::IndianRed;
	this->proj5->Location = System::Drawing::Point(47, 426);
	this->proj5->Name = L"proj5";
	this->proj5->Size = System::Drawing::Size(230, 200);
	this->proj5->TabIndex = 38;
	this->proj5->Click += gcnew System::EventHandler(this, &ProjectionForm::proj5_Click);
	// 
	// proj4
	// 
	this->proj4->BackColor = System::Drawing::Color::IndianRed;
	this->proj4->Location = System::Drawing::Point(323, 218);
	this->proj4->Name = L"proj4";
	this->proj4->Size = System::Drawing::Size(230, 200);
	this->proj4->TabIndex = 37;
	this->proj4->Click += gcnew System::EventHandler(this, &ProjectionForm::proj4_Click);
	// 
	// proj3
	// 
	this->proj3->BackColor = System::Drawing::Color::IndianRed;
	this->proj3->Location = System::Drawing::Point(47, 218);
	this->proj3->Name = L"proj3";
	this->proj3->Size = System::Drawing::Size(230, 200);
	this->proj3->TabIndex = 35;
	this->proj3->Click += gcnew System::EventHandler(this, &ProjectionForm::proj3_Click);
	// 
	// proj2
	// 
	this->proj2->BackColor = System::Drawing::Color::IndianRed;
	this->proj2->Location = System::Drawing::Point(323, 10);
	this->proj2->Name = L"proj2";
	this->proj2->Size = System::Drawing::Size(230, 200);
	this->proj2->TabIndex = 34;
	this->proj2->Click += gcnew System::EventHandler(this, &ProjectionForm::proj2_Click);
	// 
	// proj1
	// 
	this->proj1->BackColor = System::Drawing::Color::IndianRed;
	this->proj1->Location = System::Drawing::Point(47, 10);
	this->proj1->Name = L"proj1";
	this->proj1->Size = System::Drawing::Size(230, 200);
	this->proj1->TabIndex = 33;
	this->proj1->Click += gcnew System::EventHandler(this, &ProjectionForm::proj1_Click);
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
	this->TitleText->Text = L"Projection Selection Window [1]";
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
	this->CancButton->Location = System::Drawing::Point(501, 940);
	this->CancButton->Name = L"CancButton";
	this->CancButton->Size = System::Drawing::Size(99, 85);
	this->CancButton->TabIndex = 17;
	this->CancButton->UseVisualStyleBackColor = false;
	this->CancButton->Click += gcnew System::EventHandler(this, &ProjectionForm::CancButton_Click);
	// 
	// OkButton
	// 
	this->OkButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
		static_cast<System::Int32>(static_cast<System::Byte>(0)));
	this->OkButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->OkButton->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->OkButton->FlatAppearance->BorderSize = 0;
	this->OkButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->OkButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->OkButton->Location = System::Drawing::Point(0, 940);
	this->OkButton->Name = L"OkButton";
	this->OkButton->Size = System::Drawing::Size(99, 85);
	this->OkButton->TabIndex = 39;
	this->OkButton->UseVisualStyleBackColor = false;
	this->OkButton->Click += gcnew System::EventHandler(this, &ProjectionForm::OkButton_Click);
	// 
	// PrevButton
	// 
	this->PrevButton->BackColor = System::Drawing::Color::Black;
	this->PrevButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->PrevButton->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->PrevButton->FlatAppearance->BorderSize = 0;
	this->PrevButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->PrevButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->PrevButton->Location = System::Drawing::Point(132, 940);
	this->PrevButton->Name = L"PrevButton";
	this->PrevButton->Size = System::Drawing::Size(98, 85);
	this->PrevButton->TabIndex = 15;
	this->PrevButton->UseVisualStyleBackColor = false;
	this->PrevButton->Click += gcnew System::EventHandler(this, &ProjectionForm::PrevButton_Click);
	// 
	// NextButton
	// 
	this->NextButton->BackColor = System::Drawing::Color::Black;
	this->NextButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
	this->NextButton->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->NextButton->FlatAppearance->BorderSize = 0;
	this->NextButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->NextButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->NextButton->Location = System::Drawing::Point(371, 940);
	this->NextButton->Name = L"NextButton";
	this->NextButton->Size = System::Drawing::Size(98, 85);
	this->NextButton->TabIndex = 38;
	this->NextButton->UseVisualStyleBackColor = false;
	this->NextButton->Click += gcnew System::EventHandler(this, &ProjectionForm::NextButton_Click);
	// 
	// ProjectionForm
	// 
	this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
		static_cast<System::Int32>(static_cast<System::Byte>(224)));
	this->ClientSize = System::Drawing::Size(600, 1024);
	this->ControlBox = false;
	this->Controls->Add(this->OkButton);
	this->Controls->Add(this->NextButton);
	this->Controls->Add(this->PrevButton);
	this->Controls->Add(this->CancButton);
	this->Controls->Add(this->TitleText);
	this->Controls->Add(this->SelectionPanel);
	this->Cursor = System::Windows::Forms::Cursors::Hand;
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Location = System::Drawing::Point(25, 200);
	this->Name = L"ProjectionForm";
	this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
	this->SelectionPanel->ResumeLayout(false);
	this->ResumeLayout(false);

}

#pragma endregion


private: System::Void CancButton_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void OkButton_Click(System::Object^ sender, System::EventArgs^ e);

private: System::Void NextButton_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void proj1_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void proj2_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void proj3_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void proj4_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void proj5_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void proj6_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void proj7_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void proj8_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void PrevButton_Click(System::Object^ sender, System::EventArgs^ e);
};

