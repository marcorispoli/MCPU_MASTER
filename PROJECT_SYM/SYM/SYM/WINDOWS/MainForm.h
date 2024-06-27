

#pragma once

extern  double dtempo;
#include <Windows.h>
#include "PCB301/Pcb301.h"
#include "PCB302/Pcb302.h"

//#include "Pcb301Simulator.h"
//#include "Pcb302Simulator.h"

namespace CppCLRWinFormsProject {

	
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	

	/// <summary>
	/// This is the Class implementing the Startup Operating mode.
	/// </summary>
	/// \ingroup Startup_Module
	/// 
	/// 
	public ref class MainForm : public System::Windows::Forms::Form 
	{
	public:
	MainForm(void)
	{
		InitializeComponent();
		window = static_cast<HWND>(Handle.ToPointer());
		MainFormInitialize();

	}
		
	~MainForm()
	{
		if (components)
		{
			delete components;
		}
	}

	
	void clientConnectionStatus(bool status);
	void configurationCallback(void);

	cli::array<Byte>^ rxBuffer;
	int rx_len;

	
	System::Timers::Timer^ startupTimer; //!< This is the Timer to step the workflow
	HWND window;//!< This is the handle of the windw for the windows message exchange
	int startupFase; //!< This is the current fase of the startup
	int startupSubFase;//!< This is the current sub-fase of the startup

	public:
	private: System::Windows::Forms::GroupBox^ groupBox3;
	private: System::Windows::Forms::Label^ label16;
	private: System::Windows::Forms::TextBox^ current_thickness;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::TextBox^ current_force;
	private: System::Windows::Forms::Label^ label19;
	private: System::Windows::Forms::PictureBox^ rotation_led;
	private: System::Windows::Forms::GroupBox^ groupBox4;
	private: System::Windows::Forms::Button^ power_button;
	private: System::Windows::Forms::Label^ label18;
	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::CheckBox^ main_ac;
	private: System::Windows::Forms::CheckBox^ batt_ena;
	private: System::Windows::Forms::TextBox^ batt_v1;
	private: System::Windows::Forms::PictureBox^ vdc48_sw;
	private: System::Windows::Forms::TextBox^ batt_v2;
	private: System::Windows::Forms::Label^ label17;
	private: System::Windows::Forms::Label^ label15;
	private: System::Windows::Forms::PictureBox^ vdc48_stat;
	private: System::Windows::Forms::GroupBox^ groupBox6;










	private: System::Windows::Forms::PictureBox^ pictureBox2;
	private: System::Windows::Forms::Label^ label21;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Label^ label23;
	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::Windows::Forms::Button^ exit_simulator;

	private: System::Windows::Forms::Label^ potter_display_label;

	private: System::Windows::Forms::TextBox^ potter_display;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::GroupBox^ pcb315_panel;




	private: System::Windows::Forms::Label^ filter_status;
	private: System::Windows::Forms::RadioButton^ bulb_low;

	private: System::Windows::Forms::RadioButton^ bulb_short;
	private: System::Windows::Forms::RadioButton^ bulb_high;
	private: System::Windows::Forms::GroupBox^ groupBox11;
	private: System::Windows::Forms::GroupBox^ groupBox7;
	private: System::Windows::Forms::RadioButton^ stator_short;

	private: System::Windows::Forms::RadioButton^ stator_low;
	private: System::Windows::Forms::RadioButton^ stator_high;
	private: System::Windows::Forms::TextBox^ current_filter;
	private: System::Windows::Forms::GroupBox^ motor_vertical_box;


	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::TextBox^ motor_vertical_encoder;


	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::TextBox^ motor_vertical_status;
	private: System::Windows::Forms::TextBox^ motor_vertical_max_val;

	private: System::Windows::Forms::TextBox^ motor_vertical_min_val;
private: System::Windows::Forms::GroupBox^ motor_arm_box;

private: System::Windows::Forms::TextBox^ motor_arm_max_val;
private: System::Windows::Forms::TextBox^ motor_arm_min_val;
private: System::Windows::Forms::Label^ label20;
private: System::Windows::Forms::TextBox^ motor_arm_status;
private: System::Windows::Forms::Label^ label22;
private: System::Windows::Forms::TextBox^ motor_arm_encoder;
private: System::Windows::Forms::GroupBox^ motor_tilt_box;

private: System::Windows::Forms::TextBox^ motor_tilt_max_val;
private: System::Windows::Forms::TextBox^ motor_tilt_min_val;
private: System::Windows::Forms::Label^ label24;
private: System::Windows::Forms::TextBox^ motor_tilt_status;
private: System::Windows::Forms::Label^ label25;
private: System::Windows::Forms::TextBox^ motor_tilt_encoder;
private: System::Windows::Forms::GroupBox^ motor_slide_box;
private: System::Windows::Forms::TextBox^ motor_slide_max_val;
private: System::Windows::Forms::TextBox^ motor_slide_min_val;
private: System::Windows::Forms::Label^ label26;
private: System::Windows::Forms::TextBox^ motor_slide_status;
private: System::Windows::Forms::Label^ label27;
private: System::Windows::Forms::TextBox^ motor_slide_encoder;
private: System::Windows::Forms::GroupBox^ motor_body_box;
private: System::Windows::Forms::TextBox^ motor_body_max_val;


private: System::Windows::Forms::TextBox^ motor_body_min_val;

private: System::Windows::Forms::Label^ label28;
private: System::Windows::Forms::TextBox^ motor_body_status;

private: System::Windows::Forms::Label^ label29;
private: System::Windows::Forms::TextBox^ motor_body_encoder;


private: System::Windows::Forms::Label^ label36;
private: System::Windows::Forms::Label^ label31;
private: System::Windows::Forms::Label^ label37;
private: System::Windows::Forms::Label^ label32;
private: System::Windows::Forms::Label^ label38;
private: System::Windows::Forms::Label^ label33;
private: System::Windows::Forms::Label^ label39;
private: System::Windows::Forms::Label^ label34;
private: System::Windows::Forms::GroupBox^ pcb326_panel;
private: System::Windows::Forms::GroupBox^ pcb303_panel;
private: System::Windows::Forms::RadioButton^ motor_vertical_user;

private: System::Windows::Forms::RadioButton^ motor_vertical_rot;

private: System::Windows::Forms::RadioButton^ motor_vertical_tic;
private: System::Windows::Forms::Label^ label35;
private: System::Windows::Forms::Label^ label30;
private: System::Windows::Forms::RadioButton^ motor_arm_user;

private: System::Windows::Forms::RadioButton^ motor_arm_rot;

private: System::Windows::Forms::RadioButton^ motor_arm_tic;
private: System::Windows::Forms::RadioButton^ motor_tilt_user;

private: System::Windows::Forms::RadioButton^ motor_tilt_rot;

private: System::Windows::Forms::RadioButton^ motor_tilt_tic;
private: System::Windows::Forms::RadioButton^ motor_slide_user;

private: System::Windows::Forms::RadioButton^ motor_slide_rot;

private: System::Windows::Forms::RadioButton^ motor_slide_tic;
private: System::Windows::Forms::RadioButton^ motor_body_user;

private: System::Windows::Forms::RadioButton^ motor_body_rot;

private: System::Windows::Forms::RadioButton^ motor_body_tic;
private: System::Windows::Forms::TextBox^ motor_vertical_uconv;
private: System::Windows::Forms::Label^ label40;
private: System::Windows::Forms::Label^ label41;
private: System::Windows::Forms::TextBox^ motor_arm_uconv;
private: System::Windows::Forms::Label^ label42;
private: System::Windows::Forms::TextBox^ motor_tilt_uconv;

private: System::Windows::Forms::Label^ label43;
private: System::Windows::Forms::TextBox^ motor_slide_uconv;

private: System::Windows::Forms::Label^ label44;
private: System::Windows::Forms::TextBox^ motor_body_uconv;





























		   bool startupCompleted;//!< The Startup completed flag

	private:

		
		

		void pcb301Simulator(void);
		void pcb302Simulator(void);
		void pcb303Simulator(void);
		void pcb304Simulator(void);
		void pcb315Simulator(void);
		void pcb326Simulator(void);


	private: System::Windows::Forms::Button^ pedal_up;
	private: System::Windows::Forms::Button^ pedal_cmp_up;
	private: System::Windows::Forms::Button^ pedal_cmp_dwn;
	public:



	private: System::Windows::Forms::Button^ pedal_down;

	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::Button^ keypad_ccw;
	private: System::Windows::Forms::Button^ keypad_cw;


	private: System::Windows::Forms::Button^ keypad_down;

	private: System::Windows::Forms::Button^ keypad_up;

	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::CheckBox^ burning_input;

	private: System::Windows::Forms::Button^ xray_button;
	private: System::Windows::Forms::PictureBox^ x_led;
	private: System::Windows::Forms::PictureBox^ x_lmp2;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::PictureBox^ x_lmp1;
	private: System::Windows::Forms::Label^ label1;

	private: System::Windows::Forms::CheckBox^ emergency_input;

	private: System::Windows::Forms::CheckBox^ cabinet_input;

	private: System::Windows::Forms::CheckBox^ door_input;





	private: System::Windows::Forms::Button^ pedal_cmp_unlock;
private: System::Windows::Forms::GroupBox^ pcb302_panel;

	private: System::Windows::Forms::PictureBox^ calib_ena;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::PictureBox^ cmp_ena;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::TextBox^ compression_thickness;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::TextBox^ compression_force;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::ComboBox^ paddle_list;
	private: System::Windows::Forms::Button^ paddle_up;
	private: System::Windows::Forms::Button^ paddle_down;



	private: System::Windows::Forms::Label^ label13;
	private: System::Windows::Forms::ComboBox^ patient_protection;
private: System::Windows::Forms::GroupBox^ pcb304_panel;











	private: System::Windows::Forms::PictureBox^ grid_in;

	private: System::Windows::Forms::Label^ label14;
	private: System::Windows::Forms::PictureBox^ grid_on;

	private: System::Windows::Forms::Label^ label11;
private: System::Windows::Forms::GroupBox^ pcb301_panel;





















	public:
		bool startupError;//!< The Startup completed in error condition

	private: void MainFormInitialize(void);
	private: void StartupProcedure(void);

	


	private: System::Windows::Forms::GroupBox^ infoPanel;

	private: System::Windows::Forms::Label^ labelPcb301Activity;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->infoPanel = (gcnew System::Windows::Forms::GroupBox());
			this->pedal_cmp_unlock = (gcnew System::Windows::Forms::Button());
			this->pedal_cmp_up = (gcnew System::Windows::Forms::Button());
			this->pedal_cmp_dwn = (gcnew System::Windows::Forms::Button());
			this->pedal_down = (gcnew System::Windows::Forms::Button());
			this->pedal_up = (gcnew System::Windows::Forms::Button());
			this->labelPcb301Activity = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->keypad_ccw = (gcnew System::Windows::Forms::Button());
			this->rotation_led = (gcnew System::Windows::Forms::PictureBox());
			this->keypad_cw = (gcnew System::Windows::Forms::Button());
			this->keypad_down = (gcnew System::Windows::Forms::Button());
			this->keypad_up = (gcnew System::Windows::Forms::Button());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->x_lmp2 = (gcnew System::Windows::Forms::PictureBox());
			this->burning_input = (gcnew System::Windows::Forms::CheckBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->x_lmp1 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->x_led = (gcnew System::Windows::Forms::PictureBox());
			this->xray_button = (gcnew System::Windows::Forms::Button());
			this->emergency_input = (gcnew System::Windows::Forms::CheckBox());
			this->cabinet_input = (gcnew System::Windows::Forms::CheckBox());
			this->door_input = (gcnew System::Windows::Forms::CheckBox());
			this->pcb302_panel = (gcnew System::Windows::Forms::GroupBox());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->current_thickness = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->current_force = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->paddle_up = (gcnew System::Windows::Forms::Button());
			this->patient_protection = (gcnew System::Windows::Forms::ComboBox());
			this->calib_ena = (gcnew System::Windows::Forms::PictureBox());
			this->paddle_down = (gcnew System::Windows::Forms::Button());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->cmp_ena = (gcnew System::Windows::Forms::PictureBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->compression_thickness = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->compression_force = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->paddle_list = (gcnew System::Windows::Forms::ComboBox());
			this->pcb304_panel = (gcnew System::Windows::Forms::GroupBox());
			this->potter_display_label = (gcnew System::Windows::Forms::Label());
			this->potter_display = (gcnew System::Windows::Forms::TextBox());
			this->grid_on = (gcnew System::Windows::Forms::PictureBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->grid_in = (gcnew System::Windows::Forms::PictureBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->pcb301_panel = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->power_button = (gcnew System::Windows::Forms::Button());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->main_ac = (gcnew System::Windows::Forms::CheckBox());
			this->batt_ena = (gcnew System::Windows::Forms::CheckBox());
			this->batt_v1 = (gcnew System::Windows::Forms::TextBox());
			this->vdc48_sw = (gcnew System::Windows::Forms::PictureBox());
			this->batt_v2 = (gcnew System::Windows::Forms::TextBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->vdc48_stat = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->exit_simulator = (gcnew System::Windows::Forms::Button());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->pcb315_panel = (gcnew System::Windows::Forms::GroupBox());
			this->current_filter = (gcnew System::Windows::Forms::TextBox());
			this->groupBox7 = (gcnew System::Windows::Forms::GroupBox());
			this->stator_short = (gcnew System::Windows::Forms::RadioButton());
			this->stator_low = (gcnew System::Windows::Forms::RadioButton());
			this->stator_high = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox11 = (gcnew System::Windows::Forms::GroupBox());
			this->bulb_short = (gcnew System::Windows::Forms::RadioButton());
			this->bulb_low = (gcnew System::Windows::Forms::RadioButton());
			this->bulb_high = (gcnew System::Windows::Forms::RadioButton());
			this->filter_status = (gcnew System::Windows::Forms::Label());
			this->motor_vertical_box = (gcnew System::Windows::Forms::GroupBox());
			this->motor_vertical_max_val = (gcnew System::Windows::Forms::TextBox());
			this->motor_vertical_min_val = (gcnew System::Windows::Forms::TextBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->motor_vertical_status = (gcnew System::Windows::Forms::TextBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->motor_vertical_encoder = (gcnew System::Windows::Forms::TextBox());
			this->motor_arm_box = (gcnew System::Windows::Forms::GroupBox());
			this->label36 = (gcnew System::Windows::Forms::Label());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->motor_arm_max_val = (gcnew System::Windows::Forms::TextBox());
			this->motor_arm_min_val = (gcnew System::Windows::Forms::TextBox());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->motor_arm_status = (gcnew System::Windows::Forms::TextBox());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->motor_arm_encoder = (gcnew System::Windows::Forms::TextBox());
			this->motor_tilt_box = (gcnew System::Windows::Forms::GroupBox());
			this->label37 = (gcnew System::Windows::Forms::Label());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->motor_tilt_max_val = (gcnew System::Windows::Forms::TextBox());
			this->motor_tilt_min_val = (gcnew System::Windows::Forms::TextBox());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->motor_tilt_status = (gcnew System::Windows::Forms::TextBox());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->motor_tilt_encoder = (gcnew System::Windows::Forms::TextBox());
			this->motor_slide_box = (gcnew System::Windows::Forms::GroupBox());
			this->label38 = (gcnew System::Windows::Forms::Label());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->motor_slide_max_val = (gcnew System::Windows::Forms::TextBox());
			this->motor_slide_min_val = (gcnew System::Windows::Forms::TextBox());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->motor_slide_status = (gcnew System::Windows::Forms::TextBox());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->motor_slide_encoder = (gcnew System::Windows::Forms::TextBox());
			this->motor_body_box = (gcnew System::Windows::Forms::GroupBox());
			this->label39 = (gcnew System::Windows::Forms::Label());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->motor_body_max_val = (gcnew System::Windows::Forms::TextBox());
			this->motor_body_min_val = (gcnew System::Windows::Forms::TextBox());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->motor_body_status = (gcnew System::Windows::Forms::TextBox());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->motor_body_encoder = (gcnew System::Windows::Forms::TextBox());
			this->pcb326_panel = (gcnew System::Windows::Forms::GroupBox());
			this->pcb303_panel = (gcnew System::Windows::Forms::GroupBox());
			this->motor_vertical_tic = (gcnew System::Windows::Forms::RadioButton());
			this->motor_vertical_rot = (gcnew System::Windows::Forms::RadioButton());
			this->motor_vertical_user = (gcnew System::Windows::Forms::RadioButton());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->label35 = (gcnew System::Windows::Forms::Label());
			this->motor_arm_user = (gcnew System::Windows::Forms::RadioButton());
			this->motor_arm_rot = (gcnew System::Windows::Forms::RadioButton());
			this->motor_arm_tic = (gcnew System::Windows::Forms::RadioButton());
			this->motor_tilt_user = (gcnew System::Windows::Forms::RadioButton());
			this->motor_tilt_rot = (gcnew System::Windows::Forms::RadioButton());
			this->motor_tilt_tic = (gcnew System::Windows::Forms::RadioButton());
			this->motor_slide_user = (gcnew System::Windows::Forms::RadioButton());
			this->motor_slide_rot = (gcnew System::Windows::Forms::RadioButton());
			this->motor_slide_tic = (gcnew System::Windows::Forms::RadioButton());
			this->motor_body_user = (gcnew System::Windows::Forms::RadioButton());
			this->motor_body_rot = (gcnew System::Windows::Forms::RadioButton());
			this->motor_body_tic = (gcnew System::Windows::Forms::RadioButton());
			this->motor_vertical_uconv = (gcnew System::Windows::Forms::TextBox());
			this->label40 = (gcnew System::Windows::Forms::Label());
			this->label41 = (gcnew System::Windows::Forms::Label());
			this->motor_arm_uconv = (gcnew System::Windows::Forms::TextBox());
			this->label42 = (gcnew System::Windows::Forms::Label());
			this->motor_tilt_uconv = (gcnew System::Windows::Forms::TextBox());
			this->label43 = (gcnew System::Windows::Forms::Label());
			this->motor_slide_uconv = (gcnew System::Windows::Forms::TextBox());
			this->label44 = (gcnew System::Windows::Forms::Label());
			this->motor_body_uconv = (gcnew System::Windows::Forms::TextBox());
			this->infoPanel->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotation_led))->BeginInit();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->x_lmp2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->x_lmp1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->x_led))->BeginInit();
			this->pcb302_panel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->calib_ena))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->cmp_ena))->BeginInit();
			this->pcb304_panel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grid_on))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grid_in))->BeginInit();
			this->pcb301_panel->SuspendLayout();
			this->groupBox4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->vdc48_sw))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->vdc48_stat))->BeginInit();
			this->groupBox3->SuspendLayout();
			this->groupBox6->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			this->pcb315_panel->SuspendLayout();
			this->groupBox7->SuspendLayout();
			this->groupBox11->SuspendLayout();
			this->motor_vertical_box->SuspendLayout();
			this->motor_arm_box->SuspendLayout();
			this->motor_tilt_box->SuspendLayout();
			this->motor_slide_box->SuspendLayout();
			this->motor_body_box->SuspendLayout();
			this->SuspendLayout();
			// 
			// infoPanel
			// 
			this->infoPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->infoPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->infoPanel->Controls->Add(this->pedal_cmp_unlock);
			this->infoPanel->Controls->Add(this->pedal_cmp_up);
			this->infoPanel->Controls->Add(this->pedal_cmp_dwn);
			this->infoPanel->Controls->Add(this->pedal_down);
			this->infoPanel->Controls->Add(this->pedal_up);
			this->infoPanel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->infoPanel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->infoPanel->Location = System::Drawing::Point(273, 149);
			this->infoPanel->Name = L"infoPanel";
			this->infoPanel->Size = System::Drawing::Size(218, 123);
			this->infoPanel->TabIndex = 1;
			this->infoPanel->TabStop = false;
			this->infoPanel->Text = L"PEDALBOARD";
			// 
			// pedal_cmp_unlock
			// 
			this->pedal_cmp_unlock->ForeColor = System::Drawing::Color::Black;
			this->pedal_cmp_unlock->Location = System::Drawing::Point(113, 88);
			this->pedal_cmp_unlock->Name = L"pedal_cmp_unlock";
			this->pedal_cmp_unlock->Size = System::Drawing::Size(99, 23);
			this->pedal_cmp_unlock->TabIndex = 6;
			this->pedal_cmp_unlock->Text = L"CMP-UNLK";
			this->pedal_cmp_unlock->UseVisualStyleBackColor = true;
			this->pedal_cmp_unlock->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pedalboard_cmp_unlk_MouseDown);
			this->pedal_cmp_unlock->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pedalboard_cmp_unlk_MouseUp);
			// 
			// pedal_cmp_up
			// 
			this->pedal_cmp_up->ForeColor = System::Drawing::Color::Black;
			this->pedal_cmp_up->Location = System::Drawing::Point(113, 30);
			this->pedal_cmp_up->Name = L"pedal_cmp_up";
			this->pedal_cmp_up->Size = System::Drawing::Size(99, 23);
			this->pedal_cmp_up->TabIndex = 5;
			this->pedal_cmp_up->Text = L"CMP-UP";
			this->pedal_cmp_up->UseVisualStyleBackColor = true;
			this->pedal_cmp_up->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pedalboard_cmp_up_MouseDown);
			this->pedal_cmp_up->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pedalboard_cmp_up_MouseUp);
			// 
			// pedal_cmp_dwn
			// 
			this->pedal_cmp_dwn->ForeColor = System::Drawing::Color::Black;
			this->pedal_cmp_dwn->Location = System::Drawing::Point(113, 59);
			this->pedal_cmp_dwn->Name = L"pedal_cmp_dwn";
			this->pedal_cmp_dwn->Size = System::Drawing::Size(99, 23);
			this->pedal_cmp_dwn->TabIndex = 4;
			this->pedal_cmp_dwn->Text = L"CMP-DOWN";
			this->pedal_cmp_dwn->UseVisualStyleBackColor = true;
			this->pedal_cmp_dwn->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pedalboard_cmp_down_MouseDown);
			this->pedal_cmp_dwn->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pedalboard_cmp_down_MouseUp);
			// 
			// pedal_down
			// 
			this->pedal_down->ForeColor = System::Drawing::Color::Black;
			this->pedal_down->Location = System::Drawing::Point(6, 59);
			this->pedal_down->Name = L"pedal_down";
			this->pedal_down->Size = System::Drawing::Size(101, 23);
			this->pedal_down->TabIndex = 3;
			this->pedal_down->Text = L"DWN";
			this->pedal_down->UseVisualStyleBackColor = true;
			this->pedal_down->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pedalboard_down_MouseDown);
			this->pedal_down->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pedalboard_down_MouseUp);
			// 
			// pedal_up
			// 
			this->pedal_up->ForeColor = System::Drawing::Color::Black;
			this->pedal_up->Location = System::Drawing::Point(6, 31);
			this->pedal_up->Name = L"pedal_up";
			this->pedal_up->Size = System::Drawing::Size(101, 23);
			this->pedal_up->TabIndex = 2;
			this->pedal_up->Text = L"UP";
			this->pedal_up->UseVisualStyleBackColor = true;
			this->pedal_up->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pedalboard_up_MouseDown);
			this->pedal_up->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pedalboard_up_MouseUp);
			// 
			// labelPcb301Activity
			// 
			this->labelPcb301Activity->AutoSize = true;
			this->labelPcb301Activity->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->labelPcb301Activity->ForeColor = System::Drawing::Color::White;
			this->labelPcb301Activity->Location = System::Drawing::Point(131, 53);
			this->labelPcb301Activity->Name = L"labelPcb301Activity";
			this->labelPcb301Activity->Size = System::Drawing::Size(28, 13);
			this->labelPcb301Activity->TabIndex = 1;
			this->labelPcb301Activity->Text = L"LED";
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->groupBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->groupBox1->Controls->Add(this->label19);
			this->groupBox1->Controls->Add(this->keypad_ccw);
			this->groupBox1->Controls->Add(this->rotation_led);
			this->groupBox1->Controls->Add(this->keypad_cw);
			this->groupBox1->Controls->Add(this->keypad_down);
			this->groupBox1->Controls->Add(this->keypad_up);
			this->groupBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->groupBox1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->groupBox1->Location = System::Drawing::Point(15, 149);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(218, 123);
			this->groupBox1->TabIndex = 6;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"KEYPAD";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label19->ForeColor = System::Drawing::Color::White;
			this->label19->Location = System::Drawing::Point(58, 25);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(103, 13);
			this->label19->TabIndex = 8;
			this->label19->Text = L"KEYPAD-ENABLED";
			// 
			// keypad_ccw
			// 
			this->keypad_ccw->ForeColor = System::Drawing::Color::Black;
			this->keypad_ccw->Location = System::Drawing::Point(112, 59);
			this->keypad_ccw->Name = L"keypad_ccw";
			this->keypad_ccw->Size = System::Drawing::Size(99, 23);
			this->keypad_ccw->TabIndex = 5;
			this->keypad_ccw->Text = L"CCW";
			this->keypad_ccw->UseVisualStyleBackColor = true;
			this->keypad_ccw->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::keypad_ccw_MouseDown);
			this->keypad_ccw->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::keypad_ccw_MouseUp);
			// 
			// rotation_led
			// 
			this->rotation_led->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->rotation_led->Location = System::Drawing::Point(167, 21);
			this->rotation_led->Name = L"rotation_led";
			this->rotation_led->Size = System::Drawing::Size(44, 23);
			this->rotation_led->TabIndex = 7;
			this->rotation_led->TabStop = false;
			// 
			// keypad_cw
			// 
			this->keypad_cw->ForeColor = System::Drawing::Color::Black;
			this->keypad_cw->Location = System::Drawing::Point(112, 88);
			this->keypad_cw->Name = L"keypad_cw";
			this->keypad_cw->Size = System::Drawing::Size(99, 23);
			this->keypad_cw->TabIndex = 4;
			this->keypad_cw->Text = L"CW";
			this->keypad_cw->UseVisualStyleBackColor = true;
			this->keypad_cw->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::keypad_cw_MouseDown);
			this->keypad_cw->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::keypad_cw_MouseUp);
			// 
			// keypad_down
			// 
			this->keypad_down->ForeColor = System::Drawing::Color::Black;
			this->keypad_down->Location = System::Drawing::Point(5, 88);
			this->keypad_down->Name = L"keypad_down";
			this->keypad_down->Size = System::Drawing::Size(101, 23);
			this->keypad_down->TabIndex = 3;
			this->keypad_down->Text = L"DWN";
			this->keypad_down->UseVisualStyleBackColor = true;
			this->keypad_down->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::keypad_down_MouseDown);
			this->keypad_down->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::keypad_down_MouseUp);
			// 
			// keypad_up
			// 
			this->keypad_up->ForeColor = System::Drawing::Color::Black;
			this->keypad_up->Location = System::Drawing::Point(5, 60);
			this->keypad_up->Name = L"keypad_up";
			this->keypad_up->Size = System::Drawing::Size(101, 23);
			this->keypad_up->TabIndex = 2;
			this->keypad_up->Text = L"UP";
			this->keypad_up->UseVisualStyleBackColor = true;
			this->keypad_up->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::keypad_up_MouseDown);
			this->keypad_up->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::keypad_up_MouseUp);
			// 
			// groupBox2
			// 
			this->groupBox2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->groupBox2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->groupBox2->Controls->Add(this->x_lmp2);
			this->groupBox2->Controls->Add(this->burning_input);
			this->groupBox2->Controls->Add(this->label2);
			this->groupBox2->Controls->Add(this->x_lmp1);
			this->groupBox2->Controls->Add(this->label1);
			this->groupBox2->Controls->Add(this->x_led);
			this->groupBox2->Controls->Add(this->xray_button);
			this->groupBox2->Controls->Add(this->labelPcb301Activity);
			this->groupBox2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->groupBox2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->groupBox2->Location = System::Drawing::Point(453, 21);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(296, 122);
			this->groupBox2->TabIndex = 7;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"X-RAY";
			// 
			// x_lmp2
			// 
			this->x_lmp2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->x_lmp2->Location = System::Drawing::Point(234, 75);
			this->x_lmp2->Name = L"x_lmp2";
			this->x_lmp2->Size = System::Drawing::Size(44, 23);
			this->x_lmp2->TabIndex = 8;
			this->x_lmp2->TabStop = false;
			// 
			// burning_input
			// 
			this->burning_input->AutoSize = true;
			this->burning_input->Location = System::Drawing::Point(134, 29);
			this->burning_input->Name = L"burning_input";
			this->burning_input->Size = System::Drawing::Size(161, 20);
			this->burning_input->TabIndex = 3;
			this->burning_input->Text = L"BURNING JUMPER";
			this->burning_input->UseVisualStyleBackColor = true;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(231, 53);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(35, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"LMP2";
			// 
			// x_lmp1
			// 
			this->x_lmp1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->x_lmp1->Location = System::Drawing::Point(184, 75);
			this->x_lmp1->Name = L"x_lmp1";
			this->x_lmp1->Size = System::Drawing::Size(44, 23);
			this->x_lmp1->TabIndex = 6;
			this->x_lmp1->TabStop = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(181, 53);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 13);
			this->label1->TabIndex = 5;
			this->label1->Text = L"LMP1";
			// 
			// x_led
			// 
			this->x_led->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->x_led->Location = System::Drawing::Point(134, 75);
			this->x_led->Name = L"x_led";
			this->x_led->Size = System::Drawing::Size(44, 23);
			this->x_led->TabIndex = 4;
			this->x_led->TabStop = false;
			// 
			// xray_button
			// 
			this->xray_button->ForeColor = System::Drawing::Color::Black;
			this->xray_button->Location = System::Drawing::Point(22, 31);
			this->xray_button->Name = L"xray_button";
			this->xray_button->Size = System::Drawing::Size(85, 71);
			this->xray_button->TabIndex = 2;
			this->xray_button->Text = L"BUTTON";
			this->xray_button->UseVisualStyleBackColor = true;
			this->xray_button->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::xray_MouseDown);
			this->xray_button->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::xray_MouseUp);
			// 
			// emergency_input
			// 
			this->emergency_input->AutoSize = true;
			this->emergency_input->Location = System::Drawing::Point(24, 92);
			this->emergency_input->Name = L"emergency_input";
			this->emergency_input->Size = System::Drawing::Size(121, 20);
			this->emergency_input->TabIndex = 5;
			this->emergency_input->Text = L"EMERGENCY";
			this->emergency_input->UseVisualStyleBackColor = true;
			// 
			// cabinet_input
			// 
			this->cabinet_input->AutoSize = true;
			this->cabinet_input->Location = System::Drawing::Point(24, 63);
			this->cabinet_input->Name = L"cabinet_input";
			this->cabinet_input->Size = System::Drawing::Size(156, 20);
			this->cabinet_input->TabIndex = 4;
			this->cabinet_input->Text = L"CABINET-CLOSED";
			this->cabinet_input->UseVisualStyleBackColor = true;
			// 
			// door_input
			// 
			this->door_input->AutoSize = true;
			this->door_input->Location = System::Drawing::Point(24, 32);
			this->door_input->Name = L"door_input";
			this->door_input->Size = System::Drawing::Size(135, 20);
			this->door_input->TabIndex = 3;
			this->door_input->Text = L"DOOR-CLOSED";
			this->door_input->UseVisualStyleBackColor = true;
			// 
			// pcb302_panel
			// 
			this->pcb302_panel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->pcb302_panel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->pcb302_panel->Controls->Add(this->label16);
			this->pcb302_panel->Controls->Add(this->current_thickness);
			this->pcb302_panel->Controls->Add(this->label5);
			this->pcb302_panel->Controls->Add(this->current_force);
			this->pcb302_panel->Controls->Add(this->label13);
			this->pcb302_panel->Controls->Add(this->paddle_up);
			this->pcb302_panel->Controls->Add(this->patient_protection);
			this->pcb302_panel->Controls->Add(this->calib_ena);
			this->pcb302_panel->Controls->Add(this->paddle_down);
			this->pcb302_panel->Controls->Add(this->label8);
			this->pcb302_panel->Controls->Add(this->cmp_ena);
			this->pcb302_panel->Controls->Add(this->label7);
			this->pcb302_panel->Controls->Add(this->label6);
			this->pcb302_panel->Controls->Add(this->compression_thickness);
			this->pcb302_panel->Controls->Add(this->label4);
			this->pcb302_panel->Controls->Add(this->compression_force);
			this->pcb302_panel->Controls->Add(this->label3);
			this->pcb302_panel->Controls->Add(this->paddle_list);
			this->pcb302_panel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->pcb302_panel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->pcb302_panel->Location = System::Drawing::Point(12, 305);
			this->pcb302_panel->Name = L"pcb302_panel";
			this->pcb302_panel->Size = System::Drawing::Size(778, 122);
			this->pcb302_panel->TabIndex = 10;
			this->pcb302_panel->TabStop = false;
			this->pcb302_panel->Text = L"PCB302 - COMPRESSOR";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label16->ForeColor = System::Drawing::Color::White;
			this->label16->Location = System::Drawing::Point(644, 70);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(102, 13);
			this->label16->TabIndex = 18;
			this->label16->Text = L"CURR THICKNESS";
			// 
			// current_thickness
			// 
			this->current_thickness->Enabled = false;
			this->current_thickness->Location = System::Drawing::Point(647, 85);
			this->current_thickness->Name = L"current_thickness";
			this->current_thickness->Size = System::Drawing::Size(100, 22);
			this->current_thickness->TabIndex = 17;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(644, 22);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(77, 13);
			this->label5->TabIndex = 16;
			this->label5->Text = L"CURR FORCE";
			// 
			// current_force
			// 
			this->current_force->Enabled = false;
			this->current_force->Location = System::Drawing::Point(647, 38);
			this->current_force->Name = L"current_force";
			this->current_force->Size = System::Drawing::Size(100, 22);
			this->current_force->TabIndex = 15;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label13->ForeColor = System::Drawing::Color::White;
			this->label13->Location = System::Drawing::Point(394, 22);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(126, 13);
			this->label13->TabIndex = 2;
			this->label13->Text = L"PATIENT PROTECTION";
			// 
			// paddle_up
			// 
			this->paddle_up->ForeColor = System::Drawing::Color::Black;
			this->paddle_up->Location = System::Drawing::Point(21, 27);
			this->paddle_up->Name = L"paddle_up";
			this->paddle_up->Size = System::Drawing::Size(129, 38);
			this->paddle_up->TabIndex = 14;
			this->paddle_up->Text = L"PADDLE-UP";
			this->paddle_up->UseVisualStyleBackColor = true;
			this->paddle_up->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::paddle_up_MouseDown);
			this->paddle_up->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::paddle_up_MouseUp);
			// 
			// patient_protection
			// 
			this->patient_protection->FormattingEnabled = true;
			this->patient_protection->Items->AddRange(gcnew cli::array< System::Object^  >(6) {
				L"UNSELECTED", L"PROTECTION", L"PROT-SHIFTED",
					L"MAGNIFIER_1.5x", L"MAGNIFIER_1.8x", L"MAGNIFIER_2x"
			});
			this->patient_protection->Location = System::Drawing::Point(397, 38);
			this->patient_protection->Name = L"patient_protection";
			this->patient_protection->Size = System::Drawing::Size(215, 24);
			this->patient_protection->TabIndex = 0;
			// 
			// calib_ena
			// 
			this->calib_ena->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->calib_ena->Location = System::Drawing::Point(317, 85);
			this->calib_ena->Name = L"calib_ena";
			this->calib_ena->Size = System::Drawing::Size(44, 23);
			this->calib_ena->TabIndex = 12;
			this->calib_ena->TabStop = false;
			// 
			// paddle_down
			// 
			this->paddle_down->ForeColor = System::Drawing::Color::Black;
			this->paddle_down->Location = System::Drawing::Point(21, 70);
			this->paddle_down->Name = L"paddle_down";
			this->paddle_down->Size = System::Drawing::Size(129, 38);
			this->paddle_down->TabIndex = 13;
			this->paddle_down->Text = L"PADDLE-DOWN";
			this->paddle_down->UseVisualStyleBackColor = true;
			this->paddle_down->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::paddle_down_MouseDown);
			this->paddle_down->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::paddle_down_MouseUp);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label8->ForeColor = System::Drawing::Color::White;
			this->label8->Location = System::Drawing::Point(314, 69);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(37, 13);
			this->label8->TabIndex = 11;
			this->label8->Text = L"CALIB";
			// 
			// cmp_ena
			// 
			this->cmp_ena->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->cmp_ena->Location = System::Drawing::Point(317, 39);
			this->cmp_ena->Name = L"cmp_ena";
			this->cmp_ena->Size = System::Drawing::Size(44, 23);
			this->cmp_ena->TabIndex = 10;
			this->cmp_ena->TabStop = false;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label7->ForeColor = System::Drawing::Color::White;
			this->label7->Location = System::Drawing::Point(314, 23);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(57, 13);
			this->label7->TabIndex = 9;
			this->label7->Text = L"ENABLED";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label6->ForeColor = System::Drawing::Color::White;
			this->label6->Location = System::Drawing::Point(183, 68);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(115, 13);
			this->label6->TabIndex = 6;
			this->label6->Text = L"TARGET THICKNESS";
			// 
			// compression_thickness
			// 
			this->compression_thickness->Location = System::Drawing::Point(186, 84);
			this->compression_thickness->Name = L"compression_thickness";
			this->compression_thickness->Size = System::Drawing::Size(100, 22);
			this->compression_thickness->TabIndex = 5;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(183, 23);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(90, 13);
			this->label4->TabIndex = 4;
			this->label4->Text = L"TARGET FORCE";
			// 
			// compression_force
			// 
			this->compression_force->Location = System::Drawing::Point(186, 39);
			this->compression_force->Name = L"compression_force";
			this->compression_force->Size = System::Drawing::Size(100, 22);
			this->compression_force->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(396, 67);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(50, 13);
			this->label3->TabIndex = 2;
			this->label3->Text = L"PADDLE";
			// 
			// paddle_list
			// 
			this->paddle_list->FormattingEnabled = true;
			this->paddle_list->Items->AddRange(gcnew cli::array< System::Object^  >(13) {
				L"PROSTHESIS", L"BIOP2D", L"BIOP3D", L"TOMO",
					L"24x30", L"18x24", L"18x24_L", L"18x24_R", L"10x24", L"MAG-9x21", L"MAG-9x9", L"MAG-D75", L"NOT_DETECTED"
			});
			this->paddle_list->Location = System::Drawing::Point(399, 83);
			this->paddle_list->Name = L"paddle_list";
			this->paddle_list->Size = System::Drawing::Size(213, 24);
			this->paddle_list->TabIndex = 0;
			// 
			// pcb304_panel
			// 
			this->pcb304_panel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->pcb304_panel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->pcb304_panel->Controls->Add(this->potter_display_label);
			this->pcb304_panel->Controls->Add(this->potter_display);
			this->pcb304_panel->Controls->Add(this->grid_on);
			this->pcb304_panel->Controls->Add(this->label11);
			this->pcb304_panel->Controls->Add(this->grid_in);
			this->pcb304_panel->Controls->Add(this->label14);
			this->pcb304_panel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->pcb304_panel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->pcb304_panel->Location = System::Drawing::Point(388, 433);
			this->pcb304_panel->Name = L"pcb304_panel";
			this->pcb304_panel->Size = System::Drawing::Size(192, 122);
			this->pcb304_panel->TabIndex = 13;
			this->pcb304_panel->TabStop = false;
			this->pcb304_panel->Text = L"PCB304 - POTTER";
			// 
			// potter_display_label
			// 
			this->potter_display_label->AutoSize = true;
			this->potter_display_label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->potter_display_label->ForeColor = System::Drawing::Color::White;
			this->potter_display_label->Location = System::Drawing::Point(74, 29);
			this->potter_display_label->Name = L"potter_display_label";
			this->potter_display_label->Size = System::Drawing::Size(52, 13);
			this->potter_display_label->TabIndex = 20;
			this->potter_display_label->Text = L"DISPLAY";
			// 
			// potter_display
			// 
			this->potter_display->Enabled = false;
			this->potter_display->Location = System::Drawing::Point(77, 43);
			this->potter_display->Name = L"potter_display";
			this->potter_display->Size = System::Drawing::Size(100, 22);
			this->potter_display->TabIndex = 19;
			// 
			// grid_on
			// 
			this->grid_on->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->grid_on->Location = System::Drawing::Point(20, 83);
			this->grid_on->Name = L"grid_on";
			this->grid_on->Size = System::Drawing::Size(44, 23);
			this->grid_on->TabIndex = 14;
			this->grid_on->TabStop = false;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label11->ForeColor = System::Drawing::Color::White;
			this->label11->Location = System::Drawing::Point(17, 67);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(53, 13);
			this->label11->TabIndex = 13;
			this->label11->Text = L"GRID-ON";
			// 
			// grid_in
			// 
			this->grid_in->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->grid_in->Location = System::Drawing::Point(20, 43);
			this->grid_in->Name = L"grid_in";
			this->grid_in->Size = System::Drawing::Size(44, 23);
			this->grid_in->TabIndex = 10;
			this->grid_in->TabStop = false;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label14->ForeColor = System::Drawing::Color::White;
			this->label14->Location = System::Drawing::Point(17, 27);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(51, 13);
			this->label14->TabIndex = 9;
			this->label14->Text = L"IN-FIELD";
			// 
			// pcb301_panel
			// 
			this->pcb301_panel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->pcb301_panel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->pcb301_panel->Controls->Add(this->groupBox4);
			this->pcb301_panel->Controls->Add(this->groupBox3);
			this->pcb301_panel->Controls->Add(this->infoPanel);
			this->pcb301_panel->Controls->Add(this->groupBox2);
			this->pcb301_panel->Controls->Add(this->groupBox1);
			this->pcb301_panel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->pcb301_panel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->pcb301_panel->Location = System::Drawing::Point(12, 12);
			this->pcb301_panel->Name = L"pcb301_panel";
			this->pcb301_panel->Size = System::Drawing::Size(778, 287);
			this->pcb301_panel->TabIndex = 15;
			this->pcb301_panel->TabStop = false;
			this->pcb301_panel->Text = L"PCB301 - SERVICE BOARD";
			// 
			// groupBox4
			// 
			this->groupBox4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->groupBox4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->groupBox4->Controls->Add(this->power_button);
			this->groupBox4->Controls->Add(this->label18);
			this->groupBox4->Controls->Add(this->label12);
			this->groupBox4->Controls->Add(this->main_ac);
			this->groupBox4->Controls->Add(this->batt_ena);
			this->groupBox4->Controls->Add(this->batt_v1);
			this->groupBox4->Controls->Add(this->vdc48_sw);
			this->groupBox4->Controls->Add(this->batt_v2);
			this->groupBox4->Controls->Add(this->label17);
			this->groupBox4->Controls->Add(this->label15);
			this->groupBox4->Controls->Add(this->vdc48_stat);
			this->groupBox4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->groupBox4->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->groupBox4->Location = System::Drawing::Point(15, 21);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(432, 122);
			this->groupBox4->TabIndex = 15;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"POWER";
			// 
			// power_button
			// 
			this->power_button->ForeColor = System::Drawing::Color::Black;
			this->power_button->Location = System::Drawing::Point(15, 41);
			this->power_button->Name = L"power_button";
			this->power_button->Size = System::Drawing::Size(85, 55);
			this->power_button->TabIndex = 2;
			this->power_button->Text = L"ON/OFF";
			this->power_button->UseVisualStyleBackColor = true;
			this->power_button->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::poweron_MouseDown);
			this->power_button->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::poweron_MouseUp);
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label18->ForeColor = System::Drawing::Color::White;
			this->label18->Location = System::Drawing::Point(341, 69);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(50, 13);
			this->label18->TabIndex = 22;
			this->label18->Text = L"+48SWC";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label12->ForeColor = System::Drawing::Color::White;
			this->label12->Location = System::Drawing::Point(225, 69);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(27, 13);
			this->label12->TabIndex = 23;
			this->label12->Text = L"VB2";
			// 
			// main_ac
			// 
			this->main_ac->AutoSize = true;
			this->main_ac->Location = System::Drawing::Point(114, 41);
			this->main_ac->Name = L"main_ac";
			this->main_ac->Size = System::Drawing::Size(87, 20);
			this->main_ac->TabIndex = 6;
			this->main_ac->Text = L"MAIN AC";
			this->main_ac->UseVisualStyleBackColor = true;
			// 
			// batt_ena
			// 
			this->batt_ena->AutoSize = true;
			this->batt_ena->Location = System::Drawing::Point(113, 71);
			this->batt_ena->Name = L"batt_ena";
			this->batt_ena->Size = System::Drawing::Size(102, 20);
			this->batt_ena->TabIndex = 5;
			this->batt_ena->Text = L"BATT-ENA";
			this->batt_ena->UseVisualStyleBackColor = true;
			// 
			// batt_v1
			// 
			this->batt_v1->Location = System::Drawing::Point(258, 41);
			this->batt_v1->Name = L"batt_v1";
			this->batt_v1->Size = System::Drawing::Size(72, 22);
			this->batt_v1->TabIndex = 4;
			// 
			// vdc48_sw
			// 
			this->vdc48_sw->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->vdc48_sw->Location = System::Drawing::Point(394, 69);
			this->vdc48_sw->Name = L"vdc48_sw";
			this->vdc48_sw->Size = System::Drawing::Size(28, 23);
			this->vdc48_sw->TabIndex = 21;
			this->vdc48_sw->TabStop = false;
			// 
			// batt_v2
			// 
			this->batt_v2->Location = System::Drawing::Point(258, 69);
			this->batt_v2->Name = L"batt_v2";
			this->batt_v2->Size = System::Drawing::Size(72, 22);
			this->batt_v2->TabIndex = 15;
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label17->ForeColor = System::Drawing::Color::White;
			this->label17->Location = System::Drawing::Point(341, 44);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(47, 13);
			this->label17->TabIndex = 20;
			this->label17->Text = L"+48VDC";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label15->ForeColor = System::Drawing::Color::White;
			this->label15->Location = System::Drawing::Point(225, 41);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(27, 13);
			this->label15->TabIndex = 16;
			this->label15->Text = L"VB1";
			// 
			// vdc48_stat
			// 
			this->vdc48_stat->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->vdc48_stat->Location = System::Drawing::Point(394, 40);
			this->vdc48_stat->Name = L"vdc48_stat";
			this->vdc48_stat->Size = System::Drawing::Size(28, 23);
			this->vdc48_stat->TabIndex = 19;
			this->vdc48_stat->TabStop = false;
			// 
			// groupBox3
			// 
			this->groupBox3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->groupBox3->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->groupBox3->Controls->Add(this->emergency_input);
			this->groupBox3->Controls->Add(this->door_input);
			this->groupBox3->Controls->Add(this->cabinet_input);
			this->groupBox3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->groupBox3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->groupBox3->Location = System::Drawing::Point(530, 149);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(218, 123);
			this->groupBox3->TabIndex = 7;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"SAFETY";
			// 
			// groupBox6
			// 
			this->groupBox6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->groupBox6->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->groupBox6->Controls->Add(this->button2);
			this->groupBox6->Controls->Add(this->exit_simulator);
			this->groupBox6->Controls->Add(this->label21);
			this->groupBox6->Controls->Add(this->textBox1);
			this->groupBox6->Controls->Add(this->label23);
			this->groupBox6->Controls->Add(this->textBox2);
			this->groupBox6->Controls->Add(this->pictureBox2);
			this->groupBox6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->groupBox6->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->groupBox6->Location = System::Drawing::Point(10, 694);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(1085, 108);
			this->groupBox6->TabIndex = 16;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"CONNECTION STATUS";
			// 
			// button2
			// 
			this->button2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button2->ForeColor = System::Drawing::Color::Black;
			this->button2->Location = System::Drawing::Point(359, 21);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(132, 71);
			this->button2->TabIndex = 27;
			this->button2->Text = L"STORE VIEW";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// exit_simulator
			// 
			this->exit_simulator->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->exit_simulator->ForeColor = System::Drawing::Color::Black;
			this->exit_simulator->Location = System::Drawing::Point(617, 21);
			this->exit_simulator->Name = L"exit_simulator";
			this->exit_simulator->Size = System::Drawing::Size(132, 71);
			this->exit_simulator->TabIndex = 17;
			this->exit_simulator->Text = L"EXIT SIMULATOR";
			this->exit_simulator->UseVisualStyleBackColor = true;
			this->exit_simulator->Click += gcnew System::EventHandler(this, &MainForm::exit_simulator_Click);
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label21->ForeColor = System::Drawing::Color::White;
			this->label21->Location = System::Drawing::Point(6, 66);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(87, 13);
			this->label21->TabIndex = 26;
			this->label21->Text = L"PORT NUMBER";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(101, 63);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(133, 22);
			this->textBox1->TabIndex = 25;
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label23->ForeColor = System::Drawing::Color::White;
			this->label23->Location = System::Drawing::Point(30, 40);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(61, 13);
			this->label23->TabIndex = 24;
			this->label23->Text = L"IP STRING";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(101, 35);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(133, 22);
			this->textBox2->TabIndex = 23;
			// 
			// pictureBox2
			// 
			this->pictureBox2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->pictureBox2->Location = System::Drawing::Point(260, 36);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(54, 49);
			this->pictureBox2->TabIndex = 19;
			this->pictureBox2->TabStop = false;
			// 
			// pcb315_panel
			// 
			this->pcb315_panel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->pcb315_panel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->pcb315_panel->Controls->Add(this->current_filter);
			this->pcb315_panel->Controls->Add(this->groupBox7);
			this->pcb315_panel->Controls->Add(this->groupBox11);
			this->pcb315_panel->Controls->Add(this->filter_status);
			this->pcb315_panel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->pcb315_panel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->pcb315_panel->Location = System::Drawing::Point(12, 433);
			this->pcb315_panel->Name = L"pcb315_panel";
			this->pcb315_panel->Size = System::Drawing::Size(361, 122);
			this->pcb315_panel->TabIndex = 17;
			this->pcb315_panel->TabStop = false;
			this->pcb315_panel->Text = L"PCB315 - FILTER";
			this->pcb315_panel->UseCompatibleTextRendering = true;
			// 
			// current_filter
			// 
			this->current_filter->Enabled = false;
			this->current_filter->Location = System::Drawing::Point(255, 58);
			this->current_filter->Name = L"current_filter";
			this->current_filter->Size = System::Drawing::Size(100, 22);
			this->current_filter->TabIndex = 20;
			// 
			// groupBox7
			// 
			this->groupBox7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->groupBox7->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->groupBox7->Controls->Add(this->stator_short);
			this->groupBox7->Controls->Add(this->stator_low);
			this->groupBox7->Controls->Add(this->stator_high);
			this->groupBox7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->groupBox7->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->groupBox7->Location = System::Drawing::Point(129, 25);
			this->groupBox7->Name = L"groupBox7";
			this->groupBox7->Size = System::Drawing::Size(115, 81);
			this->groupBox7->TabIndex = 19;
			this->groupBox7->TabStop = false;
			this->groupBox7->Text = L"Stator-Alarms";
			// 
			// stator_short
			// 
			this->stator_short->AutoSize = true;
			this->stator_short->Location = System::Drawing::Point(6, 56);
			this->stator_short->Name = L"stator_short";
			this->stator_short->Size = System::Drawing::Size(61, 20);
			this->stator_short->TabIndex = 16;
			this->stator_short->TabStop = true;
			this->stator_short->Text = L"Short";
			this->stator_short->UseVisualStyleBackColor = true;
			// 
			// stator_low
			// 
			this->stator_low->AutoSize = true;
			this->stator_low->Location = System::Drawing::Point(6, 18);
			this->stator_low->Name = L"stator_low";
			this->stator_low->Size = System::Drawing::Size(52, 20);
			this->stator_low->TabIndex = 14;
			this->stator_low->TabStop = true;
			this->stator_low->Text = L"Low";
			this->stator_low->UseVisualStyleBackColor = true;
			// 
			// stator_high
			// 
			this->stator_high->AutoSize = true;
			this->stator_high->Location = System::Drawing::Point(6, 37);
			this->stator_high->Name = L"stator_high";
			this->stator_high->Size = System::Drawing::Size(57, 20);
			this->stator_high->TabIndex = 15;
			this->stator_high->TabStop = true;
			this->stator_high->Text = L"High";
			this->stator_high->UseVisualStyleBackColor = true;
			// 
			// groupBox11
			// 
			this->groupBox11->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->groupBox11->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->groupBox11->Controls->Add(this->bulb_short);
			this->groupBox11->Controls->Add(this->bulb_low);
			this->groupBox11->Controls->Add(this->bulb_high);
			this->groupBox11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->groupBox11->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->groupBox11->Location = System::Drawing::Point(9, 25);
			this->groupBox11->Name = L"groupBox11";
			this->groupBox11->Size = System::Drawing::Size(114, 81);
			this->groupBox11->TabIndex = 18;
			this->groupBox11->TabStop = false;
			this->groupBox11->Text = L"Bulb-Alarms";
			// 
			// bulb_short
			// 
			this->bulb_short->AutoSize = true;
			this->bulb_short->Location = System::Drawing::Point(6, 56);
			this->bulb_short->Name = L"bulb_short";
			this->bulb_short->Size = System::Drawing::Size(61, 20);
			this->bulb_short->TabIndex = 16;
			this->bulb_short->TabStop = true;
			this->bulb_short->Text = L"Short";
			this->bulb_short->UseVisualStyleBackColor = true;
			// 
			// bulb_low
			// 
			this->bulb_low->AutoSize = true;
			this->bulb_low->Location = System::Drawing::Point(6, 18);
			this->bulb_low->Name = L"bulb_low";
			this->bulb_low->Size = System::Drawing::Size(52, 20);
			this->bulb_low->TabIndex = 14;
			this->bulb_low->TabStop = true;
			this->bulb_low->Text = L"Low";
			this->bulb_low->UseVisualStyleBackColor = true;
			// 
			// bulb_high
			// 
			this->bulb_high->AutoSize = true;
			this->bulb_high->Location = System::Drawing::Point(6, 37);
			this->bulb_high->Name = L"bulb_high";
			this->bulb_high->Size = System::Drawing::Size(57, 20);
			this->bulb_high->TabIndex = 15;
			this->bulb_high->TabStop = true;
			this->bulb_high->Text = L"High";
			this->bulb_high->UseVisualStyleBackColor = true;
			this->bulb_high->CheckedChanged += gcnew System::EventHandler(this, &MainForm::bulb_high_CheckedChanged);
			// 
			// filter_status
			// 
			this->filter_status->AutoSize = true;
			this->filter_status->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->filter_status->ForeColor = System::Drawing::Color::White;
			this->filter_status->Location = System::Drawing::Point(257, 39);
			this->filter_status->Name = L"filter_status";
			this->filter_status->Size = System::Drawing::Size(44, 13);
			this->filter_status->TabIndex = 6;
			this->filter_status->Text = L"FILTER";
			// 
			// motor_vertical_box
			// 
			this->motor_vertical_box->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->motor_vertical_box->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->motor_vertical_box->Controls->Add(this->label40);
			this->motor_vertical_box->Controls->Add(this->motor_vertical_uconv);
			this->motor_vertical_box->Controls->Add(this->label35);
			this->motor_vertical_box->Controls->Add(this->label30);
			this->motor_vertical_box->Controls->Add(this->motor_vertical_user);
			this->motor_vertical_box->Controls->Add(this->motor_vertical_rot);
			this->motor_vertical_box->Controls->Add(this->motor_vertical_tic);
			this->motor_vertical_box->Controls->Add(this->motor_vertical_max_val);
			this->motor_vertical_box->Controls->Add(this->motor_vertical_min_val);
			this->motor_vertical_box->Controls->Add(this->label10);
			this->motor_vertical_box->Controls->Add(this->motor_vertical_status);
			this->motor_vertical_box->Controls->Add(this->label9);
			this->motor_vertical_box->Controls->Add(this->motor_vertical_encoder);
			this->motor_vertical_box->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->motor_vertical_box->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)),
				static_cast<System::Int32>(static_cast<System::Byte>(189)), static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->motor_vertical_box->Location = System::Drawing::Point(807, 12);
			this->motor_vertical_box->Name = L"motor_vertical_box";
			this->motor_vertical_box->Size = System::Drawing::Size(290, 122);
			this->motor_vertical_box->TabIndex = 18;
			this->motor_vertical_box->TabStop = false;
			this->motor_vertical_box->Text = L"VERTICAL-MOTOR";
			// 
			// motor_vertical_max_val
			// 
			this->motor_vertical_max_val->Enabled = false;
			this->motor_vertical_max_val->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_vertical_max_val->Location = System::Drawing::Point(53, 94);
			this->motor_vertical_max_val->Name = L"motor_vertical_max_val";
			this->motor_vertical_max_val->Size = System::Drawing::Size(61, 20);
			this->motor_vertical_max_val->TabIndex = 26;
			// 
			// motor_vertical_min_val
			// 
			this->motor_vertical_min_val->Enabled = false;
			this->motor_vertical_min_val->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_vertical_min_val->Location = System::Drawing::Point(53, 72);
			this->motor_vertical_min_val->Name = L"motor_vertical_min_val";
			this->motor_vertical_min_val->Size = System::Drawing::Size(61, 20);
			this->motor_vertical_min_val->TabIndex = 25;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label10->ForeColor = System::Drawing::Color::White;
			this->label10->Location = System::Drawing::Point(12, 26);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(35, 13);
			this->label10->TabIndex = 24;
			this->label10->Text = L"STAT";
			// 
			// motor_vertical_status
			// 
			this->motor_vertical_status->Enabled = false;
			this->motor_vertical_status->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_vertical_status->Location = System::Drawing::Point(53, 23);
			this->motor_vertical_status->Name = L"motor_vertical_status";
			this->motor_vertical_status->Size = System::Drawing::Size(216, 20);
			this->motor_vertical_status->TabIndex = 23;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label9->ForeColor = System::Drawing::Color::White;
			this->label9->Location = System::Drawing::Point(12, 51);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(29, 13);
			this->label9->TabIndex = 22;
			this->label9->Text = L"POS";
			// 
			// motor_vertical_encoder
			// 
			this->motor_vertical_encoder->Enabled = false;
			this->motor_vertical_encoder->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_vertical_encoder->Location = System::Drawing::Point(53, 50);
			this->motor_vertical_encoder->Name = L"motor_vertical_encoder";
			this->motor_vertical_encoder->Size = System::Drawing::Size(60, 20);
			this->motor_vertical_encoder->TabIndex = 21;
			// 
			// motor_arm_box
			// 
			this->motor_arm_box->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->motor_arm_box->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->motor_arm_box->Controls->Add(this->label41);
			this->motor_arm_box->Controls->Add(this->motor_arm_uconv);
			this->motor_arm_box->Controls->Add(this->motor_arm_user);
			this->motor_arm_box->Controls->Add(this->motor_arm_rot);
			this->motor_arm_box->Controls->Add(this->motor_arm_tic);
			this->motor_arm_box->Controls->Add(this->label36);
			this->motor_arm_box->Controls->Add(this->label31);
			this->motor_arm_box->Controls->Add(this->motor_arm_max_val);
			this->motor_arm_box->Controls->Add(this->motor_arm_min_val);
			this->motor_arm_box->Controls->Add(this->label20);
			this->motor_arm_box->Controls->Add(this->motor_arm_status);
			this->motor_arm_box->Controls->Add(this->label22);
			this->motor_arm_box->Controls->Add(this->motor_arm_encoder);
			this->motor_arm_box->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->motor_arm_box->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->motor_arm_box->Location = System::Drawing::Point(807, 152);
			this->motor_arm_box->Name = L"motor_arm_box";
			this->motor_arm_box->Size = System::Drawing::Size(290, 122);
			this->motor_arm_box->TabIndex = 19;
			this->motor_arm_box->TabStop = false;
			this->motor_arm_box->Text = L"ARM-MOTOR";
			// 
			// label36
			// 
			this->label36->AutoSize = true;
			this->label36->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label36->ForeColor = System::Drawing::Color::White;
			this->label36->Location = System::Drawing::Point(12, 93);
			this->label36->Name = L"label36";
			this->label36->Size = System::Drawing::Size(30, 13);
			this->label36->TabIndex = 29;
			this->label36->Text = L"MAX";
			// 
			// label31
			// 
			this->label31->AutoSize = true;
			this->label31->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label31->ForeColor = System::Drawing::Color::White;
			this->label31->Location = System::Drawing::Point(12, 72);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(27, 13);
			this->label31->TabIndex = 28;
			this->label31->Text = L"MIN";
			// 
			// motor_arm_max_val
			// 
			this->motor_arm_max_val->Enabled = false;
			this->motor_arm_max_val->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_arm_max_val->Location = System::Drawing::Point(53, 94);
			this->motor_arm_max_val->Name = L"motor_arm_max_val";
			this->motor_arm_max_val->Size = System::Drawing::Size(61, 20);
			this->motor_arm_max_val->TabIndex = 26;
			// 
			// motor_arm_min_val
			// 
			this->motor_arm_min_val->Enabled = false;
			this->motor_arm_min_val->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_arm_min_val->Location = System::Drawing::Point(53, 72);
			this->motor_arm_min_val->Name = L"motor_arm_min_val";
			this->motor_arm_min_val->Size = System::Drawing::Size(61, 20);
			this->motor_arm_min_val->TabIndex = 25;
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label20->ForeColor = System::Drawing::Color::White;
			this->label20->Location = System::Drawing::Point(12, 51);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(29, 13);
			this->label20->TabIndex = 24;
			this->label20->Text = L"POS";
			// 
			// motor_arm_status
			// 
			this->motor_arm_status->Enabled = false;
			this->motor_arm_status->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_arm_status->Location = System::Drawing::Point(53, 23);
			this->motor_arm_status->Name = L"motor_arm_status";
			this->motor_arm_status->Size = System::Drawing::Size(216, 20);
			this->motor_arm_status->TabIndex = 23;
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label22->ForeColor = System::Drawing::Color::White;
			this->label22->Location = System::Drawing::Point(12, 26);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(35, 13);
			this->label22->TabIndex = 22;
			this->label22->Text = L"STAT";
			// 
			// motor_arm_encoder
			// 
			this->motor_arm_encoder->Enabled = false;
			this->motor_arm_encoder->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_arm_encoder->Location = System::Drawing::Point(53, 50);
			this->motor_arm_encoder->Name = L"motor_arm_encoder";
			this->motor_arm_encoder->Size = System::Drawing::Size(60, 20);
			this->motor_arm_encoder->TabIndex = 21;
			// 
			// motor_tilt_box
			// 
			this->motor_tilt_box->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->motor_tilt_box->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->motor_tilt_box->Controls->Add(this->label42);
			this->motor_tilt_box->Controls->Add(this->motor_tilt_uconv);
			this->motor_tilt_box->Controls->Add(this->motor_tilt_user);
			this->motor_tilt_box->Controls->Add(this->motor_tilt_rot);
			this->motor_tilt_box->Controls->Add(this->motor_tilt_tic);
			this->motor_tilt_box->Controls->Add(this->label37);
			this->motor_tilt_box->Controls->Add(this->label32);
			this->motor_tilt_box->Controls->Add(this->motor_tilt_max_val);
			this->motor_tilt_box->Controls->Add(this->motor_tilt_min_val);
			this->motor_tilt_box->Controls->Add(this->label24);
			this->motor_tilt_box->Controls->Add(this->motor_tilt_status);
			this->motor_tilt_box->Controls->Add(this->label25);
			this->motor_tilt_box->Controls->Add(this->motor_tilt_encoder);
			this->motor_tilt_box->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->motor_tilt_box->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->motor_tilt_box->Location = System::Drawing::Point(805, 293);
			this->motor_tilt_box->Name = L"motor_tilt_box";
			this->motor_tilt_box->Size = System::Drawing::Size(290, 122);
			this->motor_tilt_box->TabIndex = 20;
			this->motor_tilt_box->TabStop = false;
			this->motor_tilt_box->Text = L"TILT-MOTOR";
			// 
			// label37
			// 
			this->label37->AutoSize = true;
			this->label37->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label37->ForeColor = System::Drawing::Color::White;
			this->label37->Location = System::Drawing::Point(12, 93);
			this->label37->Name = L"label37";
			this->label37->Size = System::Drawing::Size(30, 13);
			this->label37->TabIndex = 29;
			this->label37->Text = L"MAX";
			// 
			// label32
			// 
			this->label32->AutoSize = true;
			this->label32->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label32->ForeColor = System::Drawing::Color::White;
			this->label32->Location = System::Drawing::Point(12, 72);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(27, 13);
			this->label32->TabIndex = 28;
			this->label32->Text = L"MIN";
			// 
			// motor_tilt_max_val
			// 
			this->motor_tilt_max_val->Enabled = false;
			this->motor_tilt_max_val->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_tilt_max_val->Location = System::Drawing::Point(53, 94);
			this->motor_tilt_max_val->Name = L"motor_tilt_max_val";
			this->motor_tilt_max_val->Size = System::Drawing::Size(61, 20);
			this->motor_tilt_max_val->TabIndex = 26;
			// 
			// motor_tilt_min_val
			// 
			this->motor_tilt_min_val->Enabled = false;
			this->motor_tilt_min_val->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_tilt_min_val->Location = System::Drawing::Point(53, 72);
			this->motor_tilt_min_val->Name = L"motor_tilt_min_val";
			this->motor_tilt_min_val->Size = System::Drawing::Size(61, 20);
			this->motor_tilt_min_val->TabIndex = 25;
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label24->ForeColor = System::Drawing::Color::White;
			this->label24->Location = System::Drawing::Point(12, 51);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(29, 13);
			this->label24->TabIndex = 24;
			this->label24->Text = L"POS";
			// 
			// motor_tilt_status
			// 
			this->motor_tilt_status->Enabled = false;
			this->motor_tilt_status->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_tilt_status->Location = System::Drawing::Point(53, 23);
			this->motor_tilt_status->Name = L"motor_tilt_status";
			this->motor_tilt_status->Size = System::Drawing::Size(216, 20);
			this->motor_tilt_status->TabIndex = 23;
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label25->ForeColor = System::Drawing::Color::White;
			this->label25->Location = System::Drawing::Point(12, 26);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(35, 13);
			this->label25->TabIndex = 22;
			this->label25->Text = L"STAT";
			// 
			// motor_tilt_encoder
			// 
			this->motor_tilt_encoder->Enabled = false;
			this->motor_tilt_encoder->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_tilt_encoder->Location = System::Drawing::Point(53, 50);
			this->motor_tilt_encoder->Name = L"motor_tilt_encoder";
			this->motor_tilt_encoder->Size = System::Drawing::Size(60, 20);
			this->motor_tilt_encoder->TabIndex = 21;
			// 
			// motor_slide_box
			// 
			this->motor_slide_box->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->motor_slide_box->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->motor_slide_box->Controls->Add(this->label43);
			this->motor_slide_box->Controls->Add(this->motor_slide_uconv);
			this->motor_slide_box->Controls->Add(this->motor_slide_user);
			this->motor_slide_box->Controls->Add(this->motor_slide_rot);
			this->motor_slide_box->Controls->Add(this->motor_slide_tic);
			this->motor_slide_box->Controls->Add(this->label38);
			this->motor_slide_box->Controls->Add(this->label33);
			this->motor_slide_box->Controls->Add(this->motor_slide_max_val);
			this->motor_slide_box->Controls->Add(this->motor_slide_min_val);
			this->motor_slide_box->Controls->Add(this->label26);
			this->motor_slide_box->Controls->Add(this->motor_slide_status);
			this->motor_slide_box->Controls->Add(this->label27);
			this->motor_slide_box->Controls->Add(this->motor_slide_encoder);
			this->motor_slide_box->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->motor_slide_box->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->motor_slide_box->Location = System::Drawing::Point(805, 429);
			this->motor_slide_box->Name = L"motor_slide_box";
			this->motor_slide_box->Size = System::Drawing::Size(290, 122);
			this->motor_slide_box->TabIndex = 21;
			this->motor_slide_box->TabStop = false;
			this->motor_slide_box->Text = L"SLIDE-MOTOR";
			// 
			// label38
			// 
			this->label38->AutoSize = true;
			this->label38->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label38->ForeColor = System::Drawing::Color::White;
			this->label38->Location = System::Drawing::Point(12, 93);
			this->label38->Name = L"label38";
			this->label38->Size = System::Drawing::Size(30, 13);
			this->label38->TabIndex = 29;
			this->label38->Text = L"MAX";
			// 
			// label33
			// 
			this->label33->AutoSize = true;
			this->label33->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label33->ForeColor = System::Drawing::Color::White;
			this->label33->Location = System::Drawing::Point(12, 72);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(27, 13);
			this->label33->TabIndex = 28;
			this->label33->Text = L"MIN";
			// 
			// motor_slide_max_val
			// 
			this->motor_slide_max_val->Enabled = false;
			this->motor_slide_max_val->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_slide_max_val->Location = System::Drawing::Point(53, 94);
			this->motor_slide_max_val->Name = L"motor_slide_max_val";
			this->motor_slide_max_val->Size = System::Drawing::Size(61, 20);
			this->motor_slide_max_val->TabIndex = 26;
			// 
			// motor_slide_min_val
			// 
			this->motor_slide_min_val->Enabled = false;
			this->motor_slide_min_val->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_slide_min_val->Location = System::Drawing::Point(53, 72);
			this->motor_slide_min_val->Name = L"motor_slide_min_val";
			this->motor_slide_min_val->Size = System::Drawing::Size(61, 20);
			this->motor_slide_min_val->TabIndex = 25;
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label26->ForeColor = System::Drawing::Color::White;
			this->label26->Location = System::Drawing::Point(12, 51);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(29, 13);
			this->label26->TabIndex = 24;
			this->label26->Text = L"POS";
			// 
			// motor_slide_status
			// 
			this->motor_slide_status->Enabled = false;
			this->motor_slide_status->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_slide_status->Location = System::Drawing::Point(53, 23);
			this->motor_slide_status->Name = L"motor_slide_status";
			this->motor_slide_status->Size = System::Drawing::Size(216, 20);
			this->motor_slide_status->TabIndex = 23;
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label27->ForeColor = System::Drawing::Color::White;
			this->label27->Location = System::Drawing::Point(12, 26);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(35, 13);
			this->label27->TabIndex = 22;
			this->label27->Text = L"STAT";
			// 
			// motor_slide_encoder
			// 
			this->motor_slide_encoder->Enabled = false;
			this->motor_slide_encoder->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_slide_encoder->Location = System::Drawing::Point(53, 50);
			this->motor_slide_encoder->Name = L"motor_slide_encoder";
			this->motor_slide_encoder->Size = System::Drawing::Size(60, 20);
			this->motor_slide_encoder->TabIndex = 21;
			// 
			// motor_body_box
			// 
			this->motor_body_box->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->motor_body_box->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->motor_body_box->Controls->Add(this->label44);
			this->motor_body_box->Controls->Add(this->motor_body_uconv);
			this->motor_body_box->Controls->Add(this->motor_body_user);
			this->motor_body_box->Controls->Add(this->motor_body_rot);
			this->motor_body_box->Controls->Add(this->motor_body_tic);
			this->motor_body_box->Controls->Add(this->label39);
			this->motor_body_box->Controls->Add(this->label34);
			this->motor_body_box->Controls->Add(this->motor_body_max_val);
			this->motor_body_box->Controls->Add(this->motor_body_min_val);
			this->motor_body_box->Controls->Add(this->label28);
			this->motor_body_box->Controls->Add(this->motor_body_status);
			this->motor_body_box->Controls->Add(this->label29);
			this->motor_body_box->Controls->Add(this->motor_body_encoder);
			this->motor_body_box->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->motor_body_box->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->motor_body_box->Location = System::Drawing::Point(805, 565);
			this->motor_body_box->Name = L"motor_body_box";
			this->motor_body_box->Size = System::Drawing::Size(290, 122);
			this->motor_body_box->TabIndex = 22;
			this->motor_body_box->TabStop = false;
			this->motor_body_box->Text = L"BODY-MOTOR";
			// 
			// label39
			// 
			this->label39->AutoSize = true;
			this->label39->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label39->ForeColor = System::Drawing::Color::White;
			this->label39->Location = System::Drawing::Point(12, 93);
			this->label39->Name = L"label39";
			this->label39->Size = System::Drawing::Size(30, 13);
			this->label39->TabIndex = 29;
			this->label39->Text = L"MAX";
			// 
			// label34
			// 
			this->label34->AutoSize = true;
			this->label34->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label34->ForeColor = System::Drawing::Color::White;
			this->label34->Location = System::Drawing::Point(12, 72);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(27, 13);
			this->label34->TabIndex = 28;
			this->label34->Text = L"MIN";
			// 
			// motor_body_max_val
			// 
			this->motor_body_max_val->Enabled = false;
			this->motor_body_max_val->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_body_max_val->Location = System::Drawing::Point(53, 94);
			this->motor_body_max_val->Name = L"motor_body_max_val";
			this->motor_body_max_val->Size = System::Drawing::Size(61, 20);
			this->motor_body_max_val->TabIndex = 26;
			// 
			// motor_body_min_val
			// 
			this->motor_body_min_val->Enabled = false;
			this->motor_body_min_val->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_body_min_val->Location = System::Drawing::Point(53, 72);
			this->motor_body_min_val->Name = L"motor_body_min_val";
			this->motor_body_min_val->Size = System::Drawing::Size(61, 20);
			this->motor_body_min_val->TabIndex = 25;
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label28->ForeColor = System::Drawing::Color::White;
			this->label28->Location = System::Drawing::Point(12, 51);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(29, 13);
			this->label28->TabIndex = 24;
			this->label28->Text = L"POS";
			// 
			// motor_body_status
			// 
			this->motor_body_status->Enabled = false;
			this->motor_body_status->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_body_status->Location = System::Drawing::Point(53, 23);
			this->motor_body_status->Name = L"motor_body_status";
			this->motor_body_status->Size = System::Drawing::Size(216, 20);
			this->motor_body_status->TabIndex = 23;
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label29->ForeColor = System::Drawing::Color::White;
			this->label29->Location = System::Drawing::Point(12, 26);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(35, 13);
			this->label29->TabIndex = 22;
			this->label29->Text = L"STAT";
			// 
			// motor_body_encoder
			// 
			this->motor_body_encoder->Enabled = false;
			this->motor_body_encoder->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_body_encoder->Location = System::Drawing::Point(53, 50);
			this->motor_body_encoder->Name = L"motor_body_encoder";
			this->motor_body_encoder->Size = System::Drawing::Size(60, 20);
			this->motor_body_encoder->TabIndex = 21;
			// 
			// pcb326_panel
			// 
			this->pcb326_panel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->pcb326_panel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->pcb326_panel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->pcb326_panel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->pcb326_panel->Location = System::Drawing::Point(596, 433);
			this->pcb326_panel->Name = L"pcb326_panel";
			this->pcb326_panel->Size = System::Drawing::Size(194, 122);
			this->pcb326_panel->TabIndex = 23;
			this->pcb326_panel->TabStop = false;
			this->pcb326_panel->Text = L"PCB326 - OBSTACLE";
			// 
			// pcb303_panel
			// 
			this->pcb303_panel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(59)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->pcb303_panel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->pcb303_panel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			this->pcb303_panel->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->pcb303_panel->Location = System::Drawing::Point(10, 565);
			this->pcb303_panel->Name = L"pcb303_panel";
			this->pcb303_panel->Size = System::Drawing::Size(194, 122);
			this->pcb303_panel->TabIndex = 24;
			this->pcb303_panel->TabStop = false;
			this->pcb303_panel->Text = L"PCB303 -COLLIMATOR";
			// 
			// motor_vertical_tic
			// 
			this->motor_vertical_tic->AutoSize = true;
			this->motor_vertical_tic->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_vertical_tic->Location = System::Drawing::Point(130, 53);
			this->motor_vertical_tic->Name = L"motor_vertical_tic";
			this->motor_vertical_tic->Size = System::Drawing::Size(39, 17);
			this->motor_vertical_tic->TabIndex = 29;
			this->motor_vertical_tic->TabStop = true;
			this->motor_vertical_tic->Text = L"tic";
			this->motor_vertical_tic->UseVisualStyleBackColor = true;
			// 
			// motor_vertical_rot
			// 
			this->motor_vertical_rot->AutoSize = true;
			this->motor_vertical_rot->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_vertical_rot->Location = System::Drawing::Point(175, 53);
			this->motor_vertical_rot->Name = L"motor_vertical_rot";
			this->motor_vertical_rot->Size = System::Drawing::Size(40, 17);
			this->motor_vertical_rot->TabIndex = 30;
			this->motor_vertical_rot->TabStop = true;
			this->motor_vertical_rot->Text = L"rot";
			this->motor_vertical_rot->UseVisualStyleBackColor = true;
			// 
			// motor_vertical_user
			// 
			this->motor_vertical_user->AutoSize = true;
			this->motor_vertical_user->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_vertical_user->Location = System::Drawing::Point(218, 53);
			this->motor_vertical_user->Name = L"motor_vertical_user";
			this->motor_vertical_user->Size = System::Drawing::Size(49, 17);
			this->motor_vertical_user->TabIndex = 31;
			this->motor_vertical_user->TabStop = true;
			this->motor_vertical_user->Text = L"user";
			this->motor_vertical_user->UseVisualStyleBackColor = true;
			// 
			// label30
			// 
			this->label30->AutoSize = true;
			this->label30->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label30->ForeColor = System::Drawing::Color::White;
			this->label30->Location = System::Drawing::Point(12, 72);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(27, 13);
			this->label30->TabIndex = 32;
			this->label30->Text = L"MIN";
			// 
			// label35
			// 
			this->label35->AutoSize = true;
			this->label35->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label35->ForeColor = System::Drawing::Color::White;
			this->label35->Location = System::Drawing::Point(12, 93);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(30, 13);
			this->label35->TabIndex = 33;
			this->label35->Text = L"MAX";
			// 
			// motor_arm_user
			// 
			this->motor_arm_user->AutoSize = true;
			this->motor_arm_user->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_arm_user->Location = System::Drawing::Point(218, 53);
			this->motor_arm_user->Name = L"motor_arm_user";
			this->motor_arm_user->Size = System::Drawing::Size(49, 17);
			this->motor_arm_user->TabIndex = 34;
			this->motor_arm_user->TabStop = true;
			this->motor_arm_user->Text = L"user";
			this->motor_arm_user->UseVisualStyleBackColor = true;
			// 
			// motor_arm_rot
			// 
			this->motor_arm_rot->AutoSize = true;
			this->motor_arm_rot->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_arm_rot->Location = System::Drawing::Point(175, 53);
			this->motor_arm_rot->Name = L"motor_arm_rot";
			this->motor_arm_rot->Size = System::Drawing::Size(40, 17);
			this->motor_arm_rot->TabIndex = 33;
			this->motor_arm_rot->TabStop = true;
			this->motor_arm_rot->Text = L"rot";
			this->motor_arm_rot->UseVisualStyleBackColor = true;
			// 
			// motor_arm_tic
			// 
			this->motor_arm_tic->AutoSize = true;
			this->motor_arm_tic->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_arm_tic->Location = System::Drawing::Point(130, 53);
			this->motor_arm_tic->Name = L"motor_arm_tic";
			this->motor_arm_tic->Size = System::Drawing::Size(39, 17);
			this->motor_arm_tic->TabIndex = 32;
			this->motor_arm_tic->TabStop = true;
			this->motor_arm_tic->Text = L"tic";
			this->motor_arm_tic->UseVisualStyleBackColor = true;
			// 
			// motor_tilt_user
			// 
			this->motor_tilt_user->AutoSize = true;
			this->motor_tilt_user->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_tilt_user->Location = System::Drawing::Point(220, 53);
			this->motor_tilt_user->Name = L"motor_tilt_user";
			this->motor_tilt_user->Size = System::Drawing::Size(49, 17);
			this->motor_tilt_user->TabIndex = 34;
			this->motor_tilt_user->TabStop = true;
			this->motor_tilt_user->Text = L"user";
			this->motor_tilt_user->UseVisualStyleBackColor = true;
			// 
			// motor_tilt_rot
			// 
			this->motor_tilt_rot->AutoSize = true;
			this->motor_tilt_rot->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_tilt_rot->Location = System::Drawing::Point(177, 53);
			this->motor_tilt_rot->Name = L"motor_tilt_rot";
			this->motor_tilt_rot->Size = System::Drawing::Size(40, 17);
			this->motor_tilt_rot->TabIndex = 33;
			this->motor_tilt_rot->TabStop = true;
			this->motor_tilt_rot->Text = L"rot";
			this->motor_tilt_rot->UseVisualStyleBackColor = true;
			// 
			// motor_tilt_tic
			// 
			this->motor_tilt_tic->AutoSize = true;
			this->motor_tilt_tic->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_tilt_tic->Location = System::Drawing::Point(132, 53);
			this->motor_tilt_tic->Name = L"motor_tilt_tic";
			this->motor_tilt_tic->Size = System::Drawing::Size(39, 17);
			this->motor_tilt_tic->TabIndex = 32;
			this->motor_tilt_tic->TabStop = true;
			this->motor_tilt_tic->Text = L"tic";
			this->motor_tilt_tic->UseVisualStyleBackColor = true;
			// 
			// motor_slide_user
			// 
			this->motor_slide_user->AutoSize = true;
			this->motor_slide_user->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_slide_user->Location = System::Drawing::Point(220, 53);
			this->motor_slide_user->Name = L"motor_slide_user";
			this->motor_slide_user->Size = System::Drawing::Size(49, 17);
			this->motor_slide_user->TabIndex = 34;
			this->motor_slide_user->TabStop = true;
			this->motor_slide_user->Text = L"user";
			this->motor_slide_user->UseVisualStyleBackColor = true;
			// 
			// motor_slide_rot
			// 
			this->motor_slide_rot->AutoSize = true;
			this->motor_slide_rot->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_slide_rot->Location = System::Drawing::Point(177, 53);
			this->motor_slide_rot->Name = L"motor_slide_rot";
			this->motor_slide_rot->Size = System::Drawing::Size(40, 17);
			this->motor_slide_rot->TabIndex = 33;
			this->motor_slide_rot->TabStop = true;
			this->motor_slide_rot->Text = L"rot";
			this->motor_slide_rot->UseVisualStyleBackColor = true;
			// 
			// motor_slide_tic
			// 
			this->motor_slide_tic->AutoSize = true;
			this->motor_slide_tic->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_slide_tic->Location = System::Drawing::Point(132, 53);
			this->motor_slide_tic->Name = L"motor_slide_tic";
			this->motor_slide_tic->Size = System::Drawing::Size(39, 17);
			this->motor_slide_tic->TabIndex = 32;
			this->motor_slide_tic->TabStop = true;
			this->motor_slide_tic->Text = L"tic";
			this->motor_slide_tic->UseVisualStyleBackColor = true;
			// 
			// motor_body_user
			// 
			this->motor_body_user->AutoSize = true;
			this->motor_body_user->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_body_user->Location = System::Drawing::Point(220, 53);
			this->motor_body_user->Name = L"motor_body_user";
			this->motor_body_user->Size = System::Drawing::Size(49, 17);
			this->motor_body_user->TabIndex = 34;
			this->motor_body_user->TabStop = true;
			this->motor_body_user->Text = L"user";
			this->motor_body_user->UseVisualStyleBackColor = true;
			// 
			// motor_body_rot
			// 
			this->motor_body_rot->AutoSize = true;
			this->motor_body_rot->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_body_rot->Location = System::Drawing::Point(177, 53);
			this->motor_body_rot->Name = L"motor_body_rot";
			this->motor_body_rot->Size = System::Drawing::Size(40, 17);
			this->motor_body_rot->TabIndex = 33;
			this->motor_body_rot->TabStop = true;
			this->motor_body_rot->Text = L"rot";
			this->motor_body_rot->UseVisualStyleBackColor = true;
			// 
			// motor_body_tic
			// 
			this->motor_body_tic->AutoSize = true;
			this->motor_body_tic->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_body_tic->Location = System::Drawing::Point(132, 53);
			this->motor_body_tic->Name = L"motor_body_tic";
			this->motor_body_tic->Size = System::Drawing::Size(39, 17);
			this->motor_body_tic->TabIndex = 32;
			this->motor_body_tic->TabStop = true;
			this->motor_body_tic->Text = L"tic";
			this->motor_body_tic->UseVisualStyleBackColor = true;
			// 
			// motor_vertical_uconv
			// 
			this->motor_vertical_uconv->Enabled = false;
			this->motor_vertical_uconv->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_vertical_uconv->Location = System::Drawing::Point(188, 78);
			this->motor_vertical_uconv->Name = L"motor_vertical_uconv";
			this->motor_vertical_uconv->Size = System::Drawing::Size(79, 20);
			this->motor_vertical_uconv->TabIndex = 34;
			// 
			// label40
			// 
			this->label40->AutoSize = true;
			this->label40->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label40->ForeColor = System::Drawing::Color::White;
			this->label40->Location = System::Drawing::Point(127, 79);
			this->label40->Name = L"label40";
			this->label40->Size = System::Drawing::Size(55, 13);
			this->label40->TabIndex = 35;
			this->label40->Text = L"ROT-User";
			// 
			// label41
			// 
			this->label41->AutoSize = true;
			this->label41->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label41->ForeColor = System::Drawing::Color::White;
			this->label41->Location = System::Drawing::Point(129, 77);
			this->label41->Name = L"label41";
			this->label41->Size = System::Drawing::Size(55, 13);
			this->label41->TabIndex = 37;
			this->label41->Text = L"ROT-User";
			// 
			// motor_arm_uconv
			// 
			this->motor_arm_uconv->Enabled = false;
			this->motor_arm_uconv->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_arm_uconv->Location = System::Drawing::Point(190, 76);
			this->motor_arm_uconv->Name = L"motor_arm_uconv";
			this->motor_arm_uconv->Size = System::Drawing::Size(79, 20);
			this->motor_arm_uconv->TabIndex = 36;
			// 
			// label42
			// 
			this->label42->AutoSize = true;
			this->label42->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label42->ForeColor = System::Drawing::Color::White;
			this->label42->Location = System::Drawing::Point(131, 78);
			this->label42->Name = L"label42";
			this->label42->Size = System::Drawing::Size(55, 13);
			this->label42->TabIndex = 37;
			this->label42->Text = L"ROT-User";
			// 
			// motor_tilt_uconv
			// 
			this->motor_tilt_uconv->Enabled = false;
			this->motor_tilt_uconv->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_tilt_uconv->Location = System::Drawing::Point(192, 77);
			this->motor_tilt_uconv->Name = L"motor_tilt_uconv";
			this->motor_tilt_uconv->Size = System::Drawing::Size(79, 20);
			this->motor_tilt_uconv->TabIndex = 36;
			// 
			// label43
			// 
			this->label43->AutoSize = true;
			this->label43->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label43->ForeColor = System::Drawing::Color::White;
			this->label43->Location = System::Drawing::Point(131, 77);
			this->label43->Name = L"label43";
			this->label43->Size = System::Drawing::Size(55, 13);
			this->label43->TabIndex = 37;
			this->label43->Text = L"ROT-User";
			// 
			// motor_slide_uconv
			// 
			this->motor_slide_uconv->Enabled = false;
			this->motor_slide_uconv->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_slide_uconv->Location = System::Drawing::Point(192, 76);
			this->motor_slide_uconv->Name = L"motor_slide_uconv";
			this->motor_slide_uconv->Size = System::Drawing::Size(79, 20);
			this->motor_slide_uconv->TabIndex = 36;
			// 
			// label44
			// 
			this->label44->AutoSize = true;
			this->label44->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label44->ForeColor = System::Drawing::Color::White;
			this->label44->Location = System::Drawing::Point(129, 77);
			this->label44->Name = L"label44";
			this->label44->Size = System::Drawing::Size(55, 13);
			this->label44->TabIndex = 37;
			this->label44->Text = L"ROT-User";
			// 
			// motor_body_uconv
			// 
			this->motor_body_uconv->Enabled = false;
			this->motor_body_uconv->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold));
			this->motor_body_uconv->Location = System::Drawing::Point(190, 76);
			this->motor_body_uconv->Name = L"motor_body_uconv";
			this->motor_body_uconv->Size = System::Drawing::Size(79, 20);
			this->motor_body_uconv->TabIndex = 36;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->ClientSize = System::Drawing::Size(1114, 819);
			this->ControlBox = false;
			this->Controls->Add(this->pcb303_panel);
			this->Controls->Add(this->pcb326_panel);
			this->Controls->Add(this->motor_body_box);
			this->Controls->Add(this->motor_slide_box);
			this->Controls->Add(this->motor_tilt_box);
			this->Controls->Add(this->motor_arm_box);
			this->Controls->Add(this->motor_vertical_box);
			this->Controls->Add(this->pcb315_panel);
			this->Controls->Add(this->groupBox6);
			this->Controls->Add(this->pcb301_panel);
			this->Controls->Add(this->pcb304_panel);
			this->Controls->Add(this->pcb302_panel);
			this->DoubleBuffered = true;
			this->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(239)), static_cast<System::Int32>(static_cast<System::Byte>(189)),
				static_cast<System::Int32>(static_cast<System::Byte>(190)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"MainForm";
			this->ShowIcon = false;
			this->Text = L"Cybele Input Simulator";
			this->infoPanel->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rotation_led))->EndInit();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->x_lmp2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->x_lmp1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->x_led))->EndInit();
			this->pcb302_panel->ResumeLayout(false);
			this->pcb302_panel->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->calib_ena))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->cmp_ena))->EndInit();
			this->pcb304_panel->ResumeLayout(false);
			this->pcb304_panel->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grid_on))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->grid_in))->EndInit();
			this->pcb301_panel->ResumeLayout(false);
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->vdc48_sw))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->vdc48_stat))->EndInit();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			this->pcb315_panel->ResumeLayout(false);
			this->pcb315_panel->PerformLayout();
			this->groupBox7->ResumeLayout(false);
			this->groupBox7->PerformLayout();
			this->groupBox11->ResumeLayout(false);
			this->groupBox11->PerformLayout();
			this->motor_vertical_box->ResumeLayout(false);
			this->motor_vertical_box->PerformLayout();
			this->motor_arm_box->ResumeLayout(false);
			this->motor_arm_box->PerformLayout();
			this->motor_tilt_box->ResumeLayout(false);
			this->motor_tilt_box->PerformLayout();
			this->motor_slide_box->ResumeLayout(false);
			this->motor_slide_box->PerformLayout();
			this->motor_body_box->ResumeLayout(false);
			this->motor_body_box->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;

	private: System::Void onStartupTimeout(Object^ source, System::Timers::ElapsedEventArgs^ e)
	{
		SendMessageA(window, WM_USER + 1, 0, 0);
	}
	


	// Keypad Activation
	private: System::Void keypad_up_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.keypad_up = true;
	}
	private: System::Void keypad_up_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.keypad_up = false;
	}
	private: System::Void keypad_down_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.keypad_down = true;
	}
	private: System::Void keypad_down_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.keypad_down = false;
	}
	private: System::Void keypad_cw_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.keypad_cw = true;
	}
	private: System::Void keypad_cw_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.keypad_cw = false;
	}
	private: System::Void keypad_ccw_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.keypad_ccw = true;
	}
	private: System::Void keypad_ccw_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.keypad_ccw = false;
	}

	// Pedalboard activation
	private: System::Void pedalboard_up_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.pedalboard_up = true;
	}
	private: System::Void pedalboard_up_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.pedalboard_up = false;
	}
	private: System::Void pedalboard_down_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.pedalboard_down = true;
	}
	private: System::Void pedalboard_down_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.pedalboard_down = false;
	}
	private: System::Void pedalboard_cmp_up_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.pedalboard_cmp_up = true;
	}
	private: System::Void pedalboard_cmp_up_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.pedalboard_cmp_up = false;
	}
	private: System::Void pedalboard_cmp_down_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.pedalboard_cmp_down = true;
	}
	private: System::Void pedalboard_cmp_down_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.pedalboard_cmp_down = false;
	}

	private: System::Void pedalboard_cmp_unlk_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.pedalboard_cmp_down = true;
		PCB301::board->inputs.pedalboard_cmp_up = true;

	}
	private: System::Void pedalboard_cmp_unlk_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.pedalboard_cmp_down = false;
		PCB301::board->inputs.pedalboard_cmp_up = false;
	}

	// X-RAY Button
	private: System::Void xray_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.xray_button_input = true;
	}
	private: System::Void xray_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.xray_button_input = false;
	}

	// Power On Button
	private: System::Void poweron_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.power_off_req = true;
	}
	private: System::Void poweron_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB301::board->inputs.power_off_req = false;
	}

	// Compressor Paddle Activation
	private: System::Void paddle_up_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB302::inputs.paddle_up = true;
	}
	private: System::Void paddle_up_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB302::inputs.paddle_up = false;
	}
	private: System::Void paddle_down_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB302::inputs.paddle_down = true;
	}
	private: System::Void paddle_down_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		PCB302::inputs.paddle_down = false;
	}

	
	private: System::Void exit_simulator_Click(System::Object^ sender, System::EventArgs^ e) {
		Application::Exit();
	}



private: System::Void bulb_high_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
}
};

}