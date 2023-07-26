#include "pch.h"
// #include "Form1.h"

using namespace CppCLRWinFormsProject;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace GantryStatusRegisters;

void Form1::closeStudyRegisters(void) {
	ExposureModeRegister::armMode->Value->setCode(ArmModeOption::options::ARM_ENA);
	ExposureModeRegister::compressorMode->Value->setCode(CompressionModeOption::options::CMP_KEEP);
	ExposureModeRegister::protectionMode->Value->setCode(PatientProtectionMode::options::PROTECTION_ENA);
	
	ArmStatusRegister::validate(false);
	ArmStatusRegister::getProjections()->Value->clearList();
	XrayPushButtonRegister::setDisable();

	groupBoxReady->Hide();

}

void Form1::openStudyRegisters(void) {
	ExposureModeRegister::armMode->Value->setCode(ArmModeOption::options::ARM_ENA);
	ExposureModeRegister::compressorMode->Value->setCode(CompressionModeOption::options::CMP_KEEP);
	ExposureModeRegister::protectionMode->Value->setCode(PatientProtectionMode::options::PROTECTION_ENA);
	ArmStatusRegister::validate(false);
	ArmStatusRegister::getProjections()->Value->clearList();
	XrayPushButtonRegister::setDisable();
	groupBoxReady->Show();
}

void Form1::InitSimulatorRegisters(void) {
	OperatingStatusRegister::setCloseStudy();
	OperatingStatusRegister::setIdle();

	OperatingStatusRegister::operating_status_change_event += gcnew delegate_void_callback(this, &Form1::openStudyChange);
	OperatingStatusRegister::operating_status_change_event += gcnew delegate_void_callback((awsProtocol^)GlobalObjects::pAws, &awsProtocol::gantryStatusChangeCallback);
	CompressorRegister::thickness_change_event += gcnew delegate_void_callback((awsProtocol^)GlobalObjects::pAws, &awsProtocol::compressorDataChangeCallback);

	CompressorRegister::getPaddle()->Value->value_change_event += gcnew delegate_void_callback((awsProtocol^)GlobalObjects::pAws, &awsProtocol::componentDataChangeCallback);
	ComponentRegister::Value->value_change_event += gcnew delegate_void_callback((awsProtocol^)GlobalObjects::pAws, &awsProtocol::componentDataChangeCallback);
	CollimatorComponentRegister::Value->value_change_event += gcnew delegate_void_callback((awsProtocol^)GlobalObjects::pAws, &awsProtocol::componentDataChangeCallback);


	ArmStatusRegister::activation_completed_event += gcnew delegate_activation_completed((awsProtocol^)GlobalObjects::pAws, &awsProtocol::activationCompletedCallback);
	TrxStatusRegister::activation_completed_event += gcnew delegate_activation_completed((awsProtocol^)GlobalObjects::pAws, &awsProtocol::activationCompletedCallback);
	TrxStatusRegister::position_change_event += gcnew delegate_void_callback(this, &Form1::updateTrxPosition);
	ReadyForExposureRegister::ready_change_event += gcnew delegate_void_callback((awsProtocol^)GlobalObjects::pAws, &awsProtocol::exposureReadyChangeCallback);


	ArmStatusRegister::getProjections()->Value->value_change_event += gcnew delegate_void_callback(this, &Form1::ProjectionChange);
	ArmStatusRegister::getProjections()->Value->lista_change_event += gcnew delegate_void_callback(this, &Form1::ProjectionUpdateList);
	ArmStatusRegister::projection_request_event += gcnew delegate_string_callback((awsProtocol^)GlobalObjects::pAws, &awsProtocol::selectProjectionCallback);
	ArmStatusRegister::abort_projection_request_event += gcnew delegate_void_callback((awsProtocol^)GlobalObjects::pAws, &awsProtocol::abortProjectionCallback);
	ArmStatusRegister::validate_change_event += gcnew delegate_void_callback(this, &Form1::ArmValidateChg);
	ArmStatusRegister::position_change_event += gcnew delegate_void_callback(this, &Form1::updateArmPosition);
	ExposureModeRegister::compressorMode->Value->value_change_event += gcnew delegate_void_callback(this, &Form1::updateCompressionMode);
	ExposureModeRegister::exposureType->Value->value_change_event += gcnew delegate_void_callback(this, &Form1::updateExposureType);
	ExposureModeRegister::protectionMode->Value->value_change_event += gcnew delegate_void_callback(this, &Form1::updateProtectionMode);
	ExposureModeRegister::armMode->Value->value_change_event += gcnew delegate_void_callback(this, &Form1::updateArmMode);
	TomoConfigRegister::selection_change_event += gcnew delegate_void_callback(this, &Form1::updateTomoConfig);
	XrayPushButtonRegister::Value->value_change_event += gcnew delegate_void_callback(this, &Form1::updateXrayPushEna);

	
}
void Form1::InitFormItems(void) {

	groupBoxReady->Hide();

	comboPaddle->SelectedIndex = 11;
	comboComponents->SelectedIndex = 5;
	comboColliComponent->SelectedIndex = 4;
	comboProjections->SelectedIndex = 0;
	textForce->Text = "0";
	textThickness->Text = "0";
	labelArmAngle->Text = "-";
	labelArmTarget->Text = "-";
	labelArmLow->Text = "-";
	labelArmHigh->Text = "-";
	checkBoxArmValid->Checked = false;
	labelArmProjection->Text = "-";

	readyTimer = gcnew System::Timers::Timer(1000);
	readyTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &Form1::onReadyTimeout);
	readyTimer->Start();

	ArmStatusRegister::target_change_event += gcnew delegate_void_callback(this, &Form1::ArmUpdateData);
	armTimer = gcnew System::Timers::Timer(3000);
	armTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &Form1::onArmTimeout);
	armTimer->Stop();

	labelTrxTarget->Text = "SCOUT";
	TrxStatusRegister::target_change_event += gcnew delegate_void_callback(this, &Form1::TrxUpdateData);
	trxTimer = gcnew System::Timers::Timer(3000);
	trxTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &Form1::onTrxTimeout);
	trxTimer->Stop();

	
}