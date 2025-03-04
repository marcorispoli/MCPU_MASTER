#include "gantry_global_status.h"
#include "mainForm.h"
#include "PCB301/PCB301.h"
#include "PCB302/PCB302.h"
#include "PCB303/PCB303.h"
#include "PCB304/PCB304.h"
#include "PCB325/PCB325.h"
#include "PCB326/PCB326.h"
#include "MOTORS/VerticalMotor.h"
#include "MOTORS/ArmMotor.h"
#include "MOTORS/TiltMotor.h"
#include "MOTORS/BodyMotor.h"
#include "MOTORS/SlideMotor.h"
#include "canInterface.h"

using namespace CppCLRWinFormsProject;


#define STARTUP_IMAGE Image::FromFile(Gantry::applicationResourcePath + "Icons\\Cybele.PNG")

#define COLOR_OFF Color::Red
#define COLOR_ON Color::Yellow

void MainForm::MainFormInitialize(void) {

	// Initialize the position of the form
	//this->Left = Gantry::monitor_X0;
	//this->Top = Gantry::monitor_Y0;

	// PCB301 Panel Items initialization
	main_ac->Checked = true;
	batt_ena->Checked = true;
	door_input->Checked = true;
	cabinet_input->Checked = true;
	emergency_input->Checked = false;
	burning_input->Checked = false;

	motor_vertical_rot->Checked = true;
	motor_arm_rot->Checked = true;
	motor_tilt_rot->Checked = true;
	motor_slide_rot->Checked = true;
	motor_body_rot->Checked = true;

	batt_v1->Text = "12";
	batt_v2->Text = "12";

	x_led->BackColor = COLOR_OFF;
	x_lmp1->BackColor = COLOR_OFF;
	x_lmp2->BackColor = COLOR_OFF;
	vdc48_stat->BackColor = COLOR_OFF;
	vdc48_sw->BackColor = COLOR_OFF;



	// PCb302 Panel items Initializtion
	cmp_ena->BackColor = COLOR_OFF;
	calib_ena->BackColor = COLOR_OFF;
	paddle_list->Text = "NOT_DETECTED";
	component_list->Text = "NOT_DETECTED";

	startupFase = 0;
	startupSubFase = 0;
	startupCompleted = false;
	startupError = false;

	// Connects the configuration word
	canInterface::canrx_configuration_event += gcnew canInterface::rxData_slot(this, &MainForm::configurationCallback);

	// Starts the client
	PCB301::initialize();
	PCB302::initialize();
	PCB303::initialize();
	PCB304::initialize();
	PCB325::initialize();
	PCB326::initialize();
	VerticalMotor::initialize();
	ArmMotor::initialize();
	TiltMotor::initialize();
	BodyMotor::initialize();
	SlideMotor::initialize();

	canInterface::activateConnection();

	// Start the startup session
	startupTimer = gcnew System::Timers::Timer(100);
	startupTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &MainForm::onStartupTimeout);
	startupTimer->Start();


}

void MainForm::configurationCallback(void) {
	unsigned char config_command = canInterface::canId & 0x00FF;
	unsigned char buffer[8];

	if (config_command == canInterface::SIMUL_CONFIG_ACTIVE_DEVICES) {
		if (canInterface::canDataBuffer[0] & 0x1) PCB301::board->active = true;
		else PCB301::board->active = false;
		PCB301::board->operating = true;

		if (canInterface::canDataBuffer[0] & 0x2) PCB302::board->active = true;
		else PCB302::board->active = false;
		PCB302::board->operating = true;

		if (canInterface::canDataBuffer[0] & 0x4) PCB303::board->active = true;
		else PCB303::board->active = false;
		PCB303::board->operating = true;

		if (canInterface::canDataBuffer[0] & 0x8) PCB304::board->active = true;
		else PCB304::board->active = false;
		PCB304::board->operating = true;

		if (canInterface::canDataBuffer[0] & 0x10) PCB325::board->active = true;
		else PCB325::board->active = false;
		// PCB325::board->operating = true; --> impostato dal pannello di Main

		if (canInterface::canDataBuffer[0] & 0x20) PCB326::board->active = true;
		else PCB326::board->active = false;
		PCB326::board->operating = true;

		if (canInterface::canDataBuffer[1] & 0x1) VerticalMotor::device->active = true;
		else VerticalMotor::device->active = false;
		PCB301::board->operating = true;

		if (canInterface::canDataBuffer[1] & 0x2) BodyMotor::device->active = true;
		else BodyMotor::device->active = false;

		if (canInterface::canDataBuffer[1] & 0x4) ArmMotor::device->active = true;
		else ArmMotor::device->active = false;

		if (canInterface::canDataBuffer[1] & 0x8) TiltMotor::device->active = true;
		else TiltMotor::device->active = false;

		if (canInterface::canDataBuffer[1] & 0x10) SlideMotor::device->active = true;
		else SlideMotor::device->active = false;
		
		// Updates configuration
		SendMessageA(window, WM_USER + 2, 0, 0);
		return;
	}

	// Tilt Rotation conversion data
	if (config_command == canInterface::SIMUL_CONFIG_MOTOR_CONFIG_TRX) {
		for (int i = 0; i < 8; i++) buffer[i] = canInterface::canDataBuffer[i];
		TiltMotor::device->rot_to_unit = *reinterpret_cast<double*>(buffer);		
		return;
	}

	// Arm
	if (config_command == canInterface::SIMUL_CONFIG_MOTOR_CONFIG_ARM) {
		for (int i = 0; i < 8; i++) buffer[i] = canInterface::canDataBuffer[i];
		ArmMotor::device->rot_to_unit = *reinterpret_cast<double*>(buffer);
		return;
	}

	// Slide
	if (config_command == canInterface::SIMUL_CONFIG_MOTOR_CONFIG_SLIDE) {
		for (int i = 0; i < 8; i++) buffer[i] = canInterface::canDataBuffer[i];
		SlideMotor::device->rot_to_unit = *reinterpret_cast<double*>(buffer);
		return;
	}

	// Body
	if (config_command == canInterface::SIMUL_CONFIG_MOTOR_CONFIG_BODY) {
		for (int i = 0; i < 8; i++) buffer[i] = canInterface::canDataBuffer[i];
		BodyMotor::device->rot_to_unit = *reinterpret_cast<double*>(buffer);
		return;
	}

	// Vertical
	if (config_command == canInterface::SIMUL_CONFIG_MOTOR_CONFIG_VERTICAL) {
		for (int i = 0; i < 8; i++) buffer[i] = canInterface::canDataBuffer[i];
		VerticalMotor::device->rot_to_unit = *reinterpret_cast<double*>(buffer);
		return;
	}

	// Filter Material assignment 
	if (config_command == canInterface::SIMUL_CONFIG_FILTERS) {
		for (int i = 0; i < 8; i++) buffer[i] = canInterface::canDataBuffer[i];
		for(int i = 0; i< 5; i++) PCB303::filter_slot_assignment[i] = "";
		
		int i = 0;
		System::String^ mat = "Rh";
		if (buffer[i] < 5) {
			if(PCB303::filter_slot_assignment[buffer[i]] == "")	PCB303::filter_slot_assignment[buffer[i]] = mat;
			else PCB303::filter_slot_assignment[buffer[i]] += "\/" + mat;
		}

		i = 1;
		mat = "Ag";
		if (buffer[i] < 5) {
			if (PCB303::filter_slot_assignment[buffer[i]] == "")	PCB303::filter_slot_assignment[buffer[i]] = mat;
			else PCB303::filter_slot_assignment[buffer[i]] += "\/" + mat;
		}

		i = 2;
		mat = "Al";
		if (buffer[i] < 5) {
			if (PCB303::filter_slot_assignment[buffer[i]] == "")	PCB303::filter_slot_assignment[buffer[i]] = mat;
			else PCB303::filter_slot_assignment[buffer[i]] += "\/" + mat;
		}

		i = 3;
		mat = "Cu";
		if (buffer[i] < 5) {
			if (PCB303::filter_slot_assignment[buffer[i]] == "")	PCB303::filter_slot_assignment[buffer[i]] = mat;
			else PCB303::filter_slot_assignment[buffer[i]] += "\/" + mat;
		}

		i = 4;
		mat = "Mo";
		if (buffer[i] < 5) {
			if (PCB303::filter_slot_assignment[buffer[i]] == "")	PCB303::filter_slot_assignment[buffer[i]] = mat;
			else PCB303::filter_slot_assignment[buffer[i]] += "\/" + mat;
		}

		i = 5;
		mat = "Ld";
		if (buffer[i] < 5) {
			if (PCB303::filter_slot_assignment[buffer[i]] == "")	PCB303::filter_slot_assignment[buffer[i]] = mat;
			else PCB303::filter_slot_assignment[buffer[i]] += "\/" + mat;
		}

	}

}

void MainForm::pcb301Simulator(void) {

	// Assignes the external signals coming from the simulated boards
	PCB301::board->inputs.compression_detected = PCB302::outputs.compression_detected;
	
	PCB301::board->inputs.powerdown = (main_ac->Checked) ? 0 : 1;
	PCB301::board->inputs.battery_enable = (batt_ena->Checked) ? 1 : 0;
	PCB301::board->inputs.burning_jumper = (burning_input->Checked) ? 1 : 0;
	PCB301::board->inputs.closed_door = (door_input->Checked) ? 1 : 0;
	PCB301::board->inputs.cabinet_safety = (cabinet_input->Checked) ? 0 : 1;
	PCB301::board->inputs.emergency = (emergency_input->Checked) ? 1 : 0;


	// 48VDC Power supply
	if(PCB301::outputs.power_48VDC_stat) vdc48_stat->BackColor = COLOR_ON;
	else vdc48_stat->BackColor = COLOR_OFF;

	// 48 SW 
	if (PCB301::board->outputs.power_48SW_stat) vdc48_sw->BackColor = COLOR_ON;
	else vdc48_sw->BackColor = COLOR_OFF;
	
	// X-led
	if (PCB301::board->outputs.xray_led) x_led->BackColor = COLOR_ON;
	else x_led->BackColor = COLOR_OFF;

	// X-lamp1
	if (PCB301::board->outputs.xray_lamp1) x_lmp1->BackColor = COLOR_ON;
	else x_lmp1->BackColor = COLOR_OFF;

	// X-lamp2
	if(PCB301::board->outputs.xray_lamp2) x_lmp2->BackColor = COLOR_ON;
	else x_lmp2->BackColor = COLOR_OFF;

	// Compressor Enable
	if (PCB301::board->outputs.compression_ena) cmp_ena->BackColor = COLOR_ON;
	else cmp_ena->BackColor = COLOR_OFF;

	// Calibration Enable
	if (PCB301::board->outputs.calibration_ena) calib_ena->BackColor = COLOR_ON;
	else calib_ena->BackColor = COLOR_OFF;

	// Rotation Led
	if (PCB301::board->outputs.rotation_led) rotation_led->BackColor = COLOR_ON;
	else rotation_led->BackColor = COLOR_OFF;


}
void MainForm::pcb302Simulator(void) {
	
	paddle_list->Enabled = true;

	// Enable/Disable the component: only without pad can nbe selected a component
	//if (paddle_list->Text == "NOT_DETECTED") component_list->Enabled = true;
	//else component_list->Enabled = false;

	 
	// Target Force 
	compression_force->Text = PCB302::protocol.data_register.target_compression.ToString();
	
	// Target Thickness 
	compression_thickness->Text = PCB302::device.breast_thickness.ToString();
	
	
	// Assignes the external signals coming from the simulated boards
	if (PCB301::board->active) {
		PCB302::inputs.compression_ena = PCB301::outputs.compression_ena;
		PCB302::inputs.calibration_ena = PCB301::outputs.calibration_ena;
		PCB302::inputs.compression_down = PCB301::outputs.pedalboard_compression_down;
		PCB302::inputs.compression_up = PCB301::outputs.pedalboard_compression_up;

	}
	else {
		PCB302::inputs.compression_ena = true;
		PCB302::inputs.calibration_ena = false;
		PCB302::inputs.compression_down = false;
		PCB302::inputs.compression_up = false;
	}

	
	// Compressor Enable
	if (PCB302::inputs.compression_ena) cmp_ena->BackColor = COLOR_ON;
	else cmp_ena->BackColor = COLOR_OFF;

	// Calibration Enable
	if (PCB302::inputs.calibration_ena) calib_ena->BackColor = COLOR_ON;
	else calib_ena->BackColor = COLOR_OFF;



	// Paddle detected
	PCB302::device.current_paddle_tag = (PCB302::paddleTags) (paddle_list->SelectedIndex + 1);
	PCB302::device.current_component_code = component_list->SelectedIndex;

	// Current compression force
	current_force->Text = PCB302::device.current_force.ToString();

	// Current compression Thickness
	current_thickness->Text = PCB302::device.paddle_holder_position.ToString();

}
void MainForm::pcb303Simulator(void) {

	// Format Panel Setting
	colliIndex->Text = PCB303::device.format_index.ToString();
	if (PCB303::device.format_action_command == PCB303::ProtocolStructure::StatusRegister::action_code::STAT_UNDEFINED) {
		leftBlade->Text = "####";
		rightBlade->Text = "####";
		frontBlade->Text = "####";
		backBlade->Text = "####";
		trapBlade->Text = "####";
	}
	else if (PCB303::device.format_action_command == PCB303::ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING) {
		leftBlade->Text = "...";
		rightBlade->Text = "...";
		frontBlade->Text = "...";
		backBlade->Text = "...";
		trapBlade->Text = "...";
	}
	else {
		colliIndex->Text = PCB303::device.format_index.ToString();
		leftBlade->Text = PCB303::device.format_data.left.ToString();
		rightBlade->Text = PCB303::device.format_data.right.ToString();
		frontBlade->Text = PCB303::device.format_data.front.ToString();
		backBlade->Text = PCB303::device.format_data.back.ToString();
		trapBlade->Text = PCB303::device.format_data.trap.ToString();
	}
	
	// Filter Panel Setting
	
	if (PCB303::device.filter_index < 5) {
		filterMat->Text = PCB303::filter_slot_assignment[PCB303::device.filter_index];
		filterIndex->Text = PCB303::device.filter_index.ToString();
	}
	else {
		filterIndex->Text = "####";
		filterMat->Text = "####";
	}

	if (PCB303::device.filter_action_command == PCB303::ProtocolStructure::StatusRegister::action_code::STAT_UNDEFINED) {
		filterPosition->Text = "####";
	}
	else if (PCB303::device.filter_action_command == PCB303::ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING) {
		filterPosition->Text = "...";
	}
	else {
		filterPosition->Text = PCB303::device.filter_position.ToString();
	}


	// Mirror Panel Setting
	if(PCB303::device.mirror_index == PCB303::ProtocolStructure::StatusRegister::mirror_target_code::IN_FIELD)
		mirrorIndex->Text = "FIELD";
	else mirrorIndex->Text = "OUT";

	if (PCB303::device.mirror_action_command == PCB303::ProtocolStructure::StatusRegister::action_code::STAT_UNDEFINED) {
		mirrorPosition->Text = "####";
	}
	else if (PCB303::device.mirror_action_command == PCB303::ProtocolStructure::StatusRegister::action_code::STAT_POSITIONING) {
		mirrorPosition->Text = "...";
	}
	else {
		mirrorPosition->Text = PCB303::device.mirror_position.ToString();
	}

	// Light Panel
	lightTimer->Text = PCB303::device.power_light_timer.ToString();
	if (PCB303::device.power_light == PCB303::ProtocolStructure::StatusRegister::light_target_code::LIGHT_ON) lightPicture->BackColor = COLOR_ON;
	else lightPicture->BackColor = COLOR_OFF;

	// Tube Panel
	if (statorTemp->Text == "") statorTemp->Text = "0";
	if (bulbTemp->Text == "") bulbTemp->Text = "0";

	try {
		int val = System::Convert::ToUInt16(statorTemp->Text);		
		if (val > 100) val = 100;
		else if (val < 0) val = 0;
		
		PCB303::device.temp_stator = val;
		statorTemp->Text = val.ToString();
	}
	catch (...) {
		statorTemp->Text = "0";
		PCB303::device.temp_stator = 0;
	}

	try {
		int val = System::Convert::ToUInt16(bulbTemp->Text);
		if (val > 100) val = 100;
		else if (val < 0) val = 0;

		PCB303::device.temp_bulb = val;
		bulbTemp->Text = val.ToString();
	}
	catch (...) {
		bulbTemp->Text = "0";
		PCB303::device.temp_bulb = 0;
	}

}
void MainForm::pcb304Simulator(void) {

	// Display Section -------------------------------------------------------------------------
	
	// Display Intensity
	DisplayIntensity->Value = PCB304::device.display_intensity;

	// Display Value 
	if (!PCB304::device.display_on) {
		DisplayVal->Text = "";
	}
	else {
		DisplayVal->Text = PCB304::device.display_value.ToString();
	}

	// Grid Section -------------------------------------------------------------------------
	if (PCB304::device.grid_InOut_stat == PCB304::hardware_device::INOUT_UNDEF) {
		UndefField->Checked = true;
	}else if (PCB304::device.grid_InOut_stat == PCB304::hardware_device::INOUT_RUN) {
		RunField->Checked = true;
	}
	else if (PCB304::device.grid_InOut_stat == PCB304::hardware_device::IN_FIELD) {
		InField->Checked = true;
	}
	else if (PCB304::device.grid_InOut_stat == PCB304::hardware_device::OUT_FIELD) {
		OutField->Checked = true;
	}

	if (PCB304::device.grid_Translation_stat == PCB304::hardware_device::TRANSLATION_UNDEF) {
		UndefTrasv->Checked = true;
	}else if (PCB304::device.grid_Translation_stat == PCB304::hardware_device::TRANSLATION_HOME) {
		HomeTrasv->Checked = true;
	}
	else if (PCB304::device.grid_Translation_stat == PCB304::hardware_device::TRANSLATION_CENTER) {
		CenterTrasv->Checked = true;
	}
	else if (PCB304::device.grid_Translation_stat == PCB304::hardware_device::TRANSLATION_RUN) {
		RunTrasv->Checked = true;
	}

}

void MainForm::pcb325Simulator(void) {
	static bool biopOnBk = true;


	if (biopOnBk != biopsyOn->Checked) {
		biopOnBk = biopsyOn->Checked;
		PCB325::device.init();
		crashLabel->Hide();
		

		// Connection management
		if (biopsyOn->Checked) {
			biopsyOn->Checked = true;
			PCB325::device.connected = true;
			PCB325::board->operating = true;
			XScrollFrame->Show();
			YFrame->Show();
			positionFrame->Show();
			pointerFrame->Show();
			keyFrame->Show();
			workingMode->Show();
		}
		else {	
			biopsyOff->Checked = true;
			PCB325::device.connected = false;
			PCB325::board->operating = false;
			XScrollFrame->Hide();
			YFrame->Hide();
			positionFrame->Hide();
			pointerFrame->Hide();
			keyFrame->Hide();
			workingMode->Hide();
		}

		// Init items here
		if (PCB325::device.XScroll == PCB325::ProtocolStructure::StatusRegister::xscroll::SCROLL_CENTER) xCenter->Checked = true;
		else if (PCB325::device.XScroll == PCB325::ProtocolStructure::StatusRegister::xscroll::SCROLL_LEFT) xLeft->Checked = true;
		else if (PCB325::device.XScroll == PCB325::ProtocolStructure::StatusRegister::xscroll::SCROLL_RIGHT) xRight->Checked = true;
		else xUndef->Checked = true;

		
		if (PCB325::device.Yup_stat) yUp->Checked = true;
		else yDown->Checked = true;

		
		// Pointer Group
		if (PCB325::device.pointer_present) {
			pointerOn->Checked = true;
			slider->Show();
			needleForm->Show();

			if (PCB325::device.Needle == PCB325::ProtocolStructure::StatusRegister::needle::NEEDLE_A) needleA->Checked = true;
			else if (PCB325::device.Needle == PCB325::ProtocolStructure::StatusRegister::needle::NEEDLE_B) needleB->Checked = true;
			else if (PCB325::device.Needle == PCB325::ProtocolStructure::StatusRegister::needle::NEEDLE_C) needleC->Checked = true;
			else noNeedle->Checked = true;

		}
		else {
			pointerOff->Checked = true;
			slider->Hide();
			needleForm->Hide();

			// When the pointer is not mounted the needle is detected as C
			noNeedle->Checked = true;
			PCB325::device.Needle == PCB325::ProtocolStructure::StatusRegister::needle::NEEDLE_C;
		}
		slider->Value = PCB325::device.Slider;
				

		// Position
		xPos->Text = "X:" + PCB325::device.Xposition.ToString();
		yPos->Text = "Y:" + PCB325::device.Yposition.ToString();
		zPos->Text = "Z:" + PCB325::device.Zposition.ToString();

		// Key mode
		keyMode->Text = "NORMAL";

		return;
	}

	if (!PCB325::device.connected) return;

	// Working mode
	workingMode->Text = PCB325::device.motor_working_mode.ToString();


	if (xCenter->Checked) PCB325::device.XScroll = PCB325::ProtocolStructure::StatusRegister::xscroll::SCROLL_CENTER;
	else if (xLeft->Checked) PCB325::device.XScroll = PCB325::ProtocolStructure::StatusRegister::xscroll::SCROLL_LEFT;
	else if (xRight->Checked) PCB325::device.XScroll = PCB325::ProtocolStructure::StatusRegister::xscroll::SCROLL_RIGHT;
	else PCB325::device.XScroll = PCB325::ProtocolStructure::StatusRegister::xscroll::SCROLL_UNDEF;
	
	if (yUp->Checked) PCB325::device.Yup_stat = true;
	else PCB325::device.Yup_stat = false;

	if (pointerOn->Checked) {
		PCB325::device.pointer_present = true;
		slider->Show();
		needleForm->Show();
		PCB325::device.Slider = (int) slider->Value;
	
		if (needleA->Checked) PCB325::device.Needle = PCB325::ProtocolStructure::StatusRegister::needle::NEEDLE_A;
		else if (needleB->Checked) PCB325::device.Needle = PCB325::ProtocolStructure::StatusRegister::needle::NEEDLE_B;
		else if (needleC->Checked) PCB325::device.Needle = PCB325::ProtocolStructure::StatusRegister::needle::NEEDLE_C;
		else PCB325::device.Needle = PCB325::ProtocolStructure::StatusRegister::needle::NEEDLE_NOT_PRESENT;

	
	}
	else {
		slider->Hide();
		needleForm->Hide();
		PCB325::device.pointer_present = false;
		PCB325::device.Needle = PCB325::ProtocolStructure::StatusRegister::needle::NEEDLE_C;
				
	}
	

	// Keyboard
	if (PCB325::device.keystep_mode) keyMode->Text = "STEP";
	else keyMode->Text = "NORMAL";
	
	// Position
	xPos->Text = "X:" + PCB325::device.Xposition.ToString();
	yPos->Text = "Y:" + PCB325::device.Yposition.ToString();
	zPos->Text = "Z:" + PCB325::device.Zposition.ToString();

	// Motor enable flag
	if (PCB325::device.power_switch_stat)  biopMotEna->BackColor = COLOR_ON;
	else biopMotEna->BackColor = COLOR_OFF;

	// Crash Event
	if (PCB325::device.crash_event) crashLabel->Show();
	else crashLabel->Hide();
}

void MainForm::pcb326Simulator(void) {

}


void MainForm::clientConnectionStatus(bool status) {

}



void MainForm::StartupProcedure(void) {
	

	if(PCB301::board->active)  pcb301Simulator();
	if (PCB302::board->active) pcb302Simulator();
	if (PCB303::board->active) pcb303Simulator();
	if (PCB304::board->active) pcb304Simulator();
	if (PCB325::board->active) pcb325Simulator();
	
	// Motor power supply assignment
	int motor_power_voltage;
	int motor_power_safe_voltage;

	if (PCB301::board->active) {
		if (PCB301::outputs.power_48VDC_stat) motor_power_safe_voltage = 48000;
		else motor_power_safe_voltage = 0;
	}else motor_power_safe_voltage = 48000;

	if (PCB301::board->active) {
		if ((PCB301::outputs.power_48VDC_stat) && (PCB301::outputs.power_48SW_stat)) motor_power_voltage = 48000;
		else motor_power_voltage = 0;
	}
	else motor_power_voltage = 48000;

	if (VerticalMotor::device->active) {
		VerticalMotor::device->voltage_supply_mV = motor_power_safe_voltage;

		if (motor_vertical_tic->Checked) {
			motor_vertical_encoder->Text = VerticalMotor::device->getEncoder().ToString();
			motor_vertical_min_val->Text = ((int)VerticalMotor::device->object_dictionary[93]->data).ToString();
			motor_vertical_max_val->Text = ((int)VerticalMotor::device->object_dictionary[94]->data).ToString();
		}
		else if (motor_vertical_rot->Checked) {
			motor_vertical_encoder->Text = Motors::getRotationFromEncoder(VerticalMotor::device->getEncoder()).ToString();
			motor_vertical_min_val->Text = Motors::getRotationFromEncoder((int)VerticalMotor::device->object_dictionary[93]->data).ToString();
			motor_vertical_max_val->Text = Motors::getRotationFromEncoder((int)VerticalMotor::device->object_dictionary[94]->data).ToString();
		}
		else {
			motor_vertical_encoder->Text = VerticalMotor::device->getUserUnitFromEncoder(VerticalMotor::device->getEncoder()).ToString();
			motor_vertical_min_val->Text = VerticalMotor::device->getUserUnitFromEncoder((int)VerticalMotor::device->object_dictionary[93]->data).ToString();
			motor_vertical_max_val->Text = VerticalMotor::device->getUserUnitFromEncoder((int)VerticalMotor::device->object_dictionary[94]->data).ToString();
		}
		
		motor_vertical_uconv->Text = VerticalMotor::device->rot_to_unit.ToString();
		motor_vertical_status->Text = VerticalMotor::device->getStatus().ToString();
		
	}

	if (ArmMotor::device->active) {
		ArmMotor::device->voltage_supply_mV = motor_power_safe_voltage;
		
		if (motor_arm_tic->Checked) {
			motor_arm_encoder->Text = ArmMotor::device->getEncoder().ToString();
			motor_arm_min_val->Text = ((int)ArmMotor::device->object_dictionary[93]->data).ToString();
			motor_arm_max_val->Text = ((int)ArmMotor::device->object_dictionary[94]->data).ToString();
		}
		else if (motor_arm_rot->Checked) {
			motor_arm_encoder->Text = Motors::getRotationFromEncoder(ArmMotor::device->getEncoder()).ToString();
			motor_arm_min_val->Text = Motors::getRotationFromEncoder((int)ArmMotor::device->object_dictionary[93]->data).ToString();
			motor_arm_max_val->Text = Motors::getRotationFromEncoder((int)ArmMotor::device->object_dictionary[94]->data).ToString();
		}
		else {
			motor_arm_encoder->Text = ArmMotor::device->getUserUnitFromEncoder(ArmMotor::device->getEncoder()).ToString();
			motor_arm_min_val->Text = ArmMotor::device->getUserUnitFromEncoder((int)ArmMotor::device->object_dictionary[93]->data).ToString();
			motor_arm_max_val->Text = ArmMotor::device->getUserUnitFromEncoder((int)ArmMotor::device->object_dictionary[94]->data).ToString();
		}
		
		motor_arm_uconv->Text = ArmMotor::device->rot_to_unit.ToString();
		motor_arm_status->Text = ArmMotor::device->getStatus().ToString();
	}

	if (TiltMotor::device->active) {
		TiltMotor::device->voltage_supply_mV = motor_power_voltage;
		
		if (motor_tilt_tic->Checked) {
			motor_tilt_encoder->Text = TiltMotor::device->getEncoder().ToString();
			motor_tilt_min_val->Text = ((int)TiltMotor::device->object_dictionary[93]->data).ToString();
			motor_tilt_max_val->Text = ((int)TiltMotor::device->object_dictionary[94]->data).ToString();
		}
		else if (motor_tilt_rot->Checked) {
			motor_tilt_encoder->Text = Motors::getRotationFromEncoder(TiltMotor::device->getEncoder()).ToString();
			motor_tilt_min_val->Text = Motors::getRotationFromEncoder((int)TiltMotor::device->object_dictionary[93]->data).ToString();
			motor_tilt_max_val->Text = Motors::getRotationFromEncoder((int)TiltMotor::device->object_dictionary[94]->data).ToString();
		}
		else {
			motor_tilt_encoder->Text = TiltMotor::device->getUserUnitFromEncoder(TiltMotor::device->getEncoder()).ToString();
			motor_tilt_min_val->Text = TiltMotor::device->getUserUnitFromEncoder((int)TiltMotor::device->object_dictionary[93]->data).ToString();
			motor_tilt_max_val->Text = TiltMotor::device->getUserUnitFromEncoder((int)TiltMotor::device->object_dictionary[94]->data).ToString();
		}
		motor_tilt_uconv->Text = TiltMotor::device->rot_to_unit.ToString();
		motor_tilt_status->Text = TiltMotor::device->getStatus().ToString();
	}

	if (SlideMotor::device->active) {
		SlideMotor::device->voltage_supply_mV = motor_power_safe_voltage;
		
		if (motor_slide_tic->Checked) {
			motor_slide_encoder->Text = SlideMotor::device->getEncoder().ToString();
			motor_slide_min_val->Text = ((int)SlideMotor::device->object_dictionary[93]->data).ToString();
			motor_slide_max_val->Text = ((int)SlideMotor::device->object_dictionary[94]->data).ToString();
		}
		else if (motor_slide_rot->Checked) {
			motor_slide_encoder->Text = Motors::getRotationFromEncoder(SlideMotor::device->getEncoder()).ToString();
			motor_slide_min_val->Text = Motors::getRotationFromEncoder((int)SlideMotor::device->object_dictionary[93]->data).ToString();
			motor_slide_max_val->Text = Motors::getRotationFromEncoder((int)SlideMotor::device->object_dictionary[94]->data).ToString();
		}
		else {
			motor_slide_encoder->Text = SlideMotor::device->getUserUnitFromEncoder(SlideMotor::device->getEncoder()).ToString();
			motor_slide_min_val->Text = SlideMotor::device->getUserUnitFromEncoder((int)SlideMotor::device->object_dictionary[93]->data).ToString();
			motor_slide_max_val->Text = SlideMotor::device->getUserUnitFromEncoder((int)SlideMotor::device->object_dictionary[94]->data).ToString();
		}
		motor_slide_uconv->Text = SlideMotor::device->rot_to_unit.ToString();
		motor_slide_status->Text = SlideMotor::device->getStatus().ToString();
	}

	if (BodyMotor::device->active) {
		BodyMotor::device->voltage_supply_mV = motor_power_safe_voltage;

		if (motor_body_tic->Checked) {
			motor_body_encoder->Text = BodyMotor::device->getEncoder().ToString();
			motor_body_min_val->Text = ((int)BodyMotor::device->object_dictionary[93]->data).ToString();
			motor_body_max_val->Text = ((int)BodyMotor::device->object_dictionary[94]->data).ToString();
		}
		else if (motor_body_rot->Checked) {
			motor_body_encoder->Text = Motors::getRotationFromEncoder(BodyMotor::device->getEncoder()).ToString();
			motor_body_min_val->Text = Motors::getRotationFromEncoder((int)BodyMotor::device->object_dictionary[93]->data).ToString();
			motor_body_max_val->Text = Motors::getRotationFromEncoder((int)BodyMotor::device->object_dictionary[94]->data).ToString();
		}
		else {
			motor_body_encoder->Text = BodyMotor::device->getUserUnitFromEncoder(BodyMotor::device->getEncoder()).ToString();
			motor_body_min_val->Text = BodyMotor::device->getUserUnitFromEncoder((int)BodyMotor::device->object_dictionary[93]->data).ToString();
			motor_body_max_val->Text = BodyMotor::device->getUserUnitFromEncoder((int)BodyMotor::device->object_dictionary[94]->data).ToString();
		}
		motor_body_uconv->Text =  BodyMotor::device->rot_to_unit.ToString();
		motor_body_status->Text = BodyMotor::device->getStatus().ToString();
	}
	

}

void MainForm::WndProc(System::Windows::Forms::Message% m) 
{
	switch (m.Msg) {

	case (WM_USER + 1): // onStartupTimeout		
		StartupProcedure();
		break;

	case (WM_USER + 2): 
		// Set the  view of active panels
		(PCB301::board->active) ? pcb301_panel->Show() : pcb301_panel->Hide();
		(PCB302::board->active) ? pcb302_panel->Show() : pcb302_panel->Hide();
		(PCB303::board->active) ? pcb303_panel->Show() : pcb303_panel->Hide();
		(PCB304::board->active) ? pcb304_panel->Show() : pcb304_panel->Hide();
		(PCB325::board->active) ? pcb325_panel->Show() : pcb325_panel->Hide();
		(PCB326::board->active) ? pcb326_panel->Show() : pcb326_panel->Hide();

		(VerticalMotor::device->active) ? motor_vertical_box->Show() : motor_vertical_box->Hide();
		(ArmMotor::device->active) ? motor_arm_box->Show() : motor_arm_box->Hide();
		(TiltMotor::device->active) ? motor_tilt_box->Show() : motor_tilt_box->Hide();
		(SlideMotor::device->active) ? motor_slide_box->Show() : motor_slide_box->Hide();
		(BodyMotor::device->active) ? motor_body_box->Show() : motor_body_box->Hide();
		break;

	case (WM_USER + 3): 
		
		break;

	case (WM_USER + 4): 
		
		break;

	case (WM_USER + 5): 
		
		break;
	}


	Form::WndProc(m);
}
