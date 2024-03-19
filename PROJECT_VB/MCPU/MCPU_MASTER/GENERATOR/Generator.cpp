#include "R2CP_Eth.h"
#include "CaDataDicGen.h"
#include "SystemConfig.h"
#include "Generator.h"
#include "gantry_global_status.h"
#include "ExposureModule.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB304.h"
#include "Notify.h"
#include "awsProtocol.h"
#include "ArmMotor.h"
#include <thread>
#include "Log.h"



typedef short (*pSendR2CP_callback)(unsigned char*, unsigned short) ; //!< This is the type used to pass the reception callback to the R2CP module

/// <summary>
/// This is the callback called by the R2CP module \n
/// whenever a valid R2CP frame shall be sent to the smart hub
/// 
/// </summary>
/// <param name="pMessage"></param>
/// <param name="datalength"></param>
/// <returns></returns>
int16_t Generator::sendCR2CPData(unsigned char* pMessage, unsigned short  datalength) {
    cli::array<Byte>^ buf = gcnew cli::array<Byte>(datalength);
    for (int i = 0; i < datalength; i++) buf[i] = pMessage[i]; 
    device->send(buf);
    return datalength;
}


Generator::Generator(void):TcpClientCLI( SH_IP_ADDRESS, SH_PORT) 
{
    

}

void Generator::startNormalMode(void) {
    device->simulator_mode = false;
    
    // Starts the client to communicate with smart Hub
    device->startConnection();

    // CR2CP Initialization
    R2CP_Eth = new CR2CP_Eth((pSendR2CP_callback)sendCR2CPData, 0, (CaDataDic*)R2CP::CaDataDicGen::GetInstance(), 0, 0);
    R2CP::CaDataDicGen::GetInstance()->Initialitation();
    R2CP::CaDataDicGen::GetInstance()->SetCommunicationForm(R2CP_Eth);

    R2CP_Eth->smartHubConnected = false;
    R2CP_Eth->generatorConnected = false;
    R2CP_Eth->serviceToolConnected = false;

    // Start the reception thread
    device->running_thread = gcnew Thread(gcnew ThreadStart(device, &Generator::threadWork));
    device->running_thread->Name = "Loop Generator Workflow ";
    device->running_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
    device->running_thread->Start();

}

void Generator::startSimulatorMode(void) {
    device->simulator_mode = true;

    device->running_thread = gcnew Thread(gcnew ThreadStart(device, &Generator::simulatorWork));
    device->running_thread->Name = "Loop Generator Simulator Workflow ";
    device->running_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
    device->running_thread->Start();
}

/// <summary>
/// This is the ethernet reception buffer callback.
/// 
/// The routine detect multiple nested frames and passes every frame \n
/// to the R2CP module to be properly processed.
/// 
/// </summary>
/// <param name="receiveBuffer"></param>
/// <param name="rc"></param>
void Generator::rxData(cli::array<Byte>^ receiveBuffer, int rc) {
    
    int block_size = rc;
    int framelen;
    int bufindex = 0;

    while (1) {
        block_size = rc - bufindex;
        if (block_size < 8) return;
        framelen = receiveBuffer[6+bufindex] * 256 + receiveBuffer[7 + bufindex];

        // Pass the managed array to unmanaged function
        pin_ptr<unsigned char> pp = &receiveBuffer[bufindex];
        R2CP_Eth->ProcessMessage(pp);

        if (block_size > framelen + 8) bufindex += framelen + 8;        
        else return;        
    }

    return;
}

bool Generator::isSmartHubConnected(void) {
    if (!device->isConnected()) return false;
    return R2CP_Eth->smartHubConnected;
}
bool Generator::isGeneratorConnected(void) {
    if (!device->isConnected()) return false;
    return R2CP_Eth->generatorConnected;
}
bool Generator::isServiceToolConnected(void) {
    if (device->simulator_mode) return false;
    return R2CP_Eth->serviceToolConnected;
}


bool Generator::connectionTest(void) {
    if (!isConnected()) return false;
    if (!R2CP_Eth->smartHubConnected)  return false;
    if (!R2CP_Eth->generatorConnected)  return false;
    return true;

}

void Generator::simulatorWork(void) {
    LogClass::logInFile("Generator Simulator Version!\n");

    while (true) {
        setup_completed = true;
        idle_status = true;
        ready_for_exposure = true;
        generatorSimulatorIdle();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Generator::errorLoop(void) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
void Generator::serviceToolLoop(void) {
    LogClass::logInFile("Service tool detected!\n");
    
    while (isServiceToolConnected()) {
        //Activates the XRAY-ENA to allows the service tool activities
        PCB301::set_xray_ena(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    LogClass::logInFile("Service tool exited\n");
    PCB301::set_xray_ena(false);

}
void Generator::threadWork(void) {
  
    while (true) {
        LogClass::logInFile("Try to connect the Smart Hub and Generator!\n");
        Notify::activate(Notify::messages::ERROR_GENERATOR_ERROR_CONNECTION);                
        Notify::activate(Notify::messages::WARNING_GENERATOR_NOT_READY);

        R2CP_Eth->smartHubConnected = false;
        R2CP_Eth->generatorConnected = false;
        setup_completed = false;
        idle_status = false;
        ready_for_exposure = false;

        // Waits the Application and Generator connected with the smart hub
        // Every repetition wait 1s befor to proceed to prevent over communication
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));        
        while ((!R2CP_Eth->smartHubConnected) || (!R2CP_Eth->generatorConnected) ) {

            // If the ethernet should drop repeats
            if (!isConnected()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                R2CP_Eth->smartHubConnected = false;
                R2CP_Eth->generatorConnected = false;
            }
            
            // Notify the connection
            R2CP::CaDataDicGen::GetInstance()->Network_ConnectionRequest_Event();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            // Request the Application Node status
            R2CP::CaDataDicGen::GetInstance()->Network_GetApplicationNodeStatus();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            // Request the Generator Node status
            R2CP::CaDataDicGen::GetInstance()->Network_GetGeneratorNodeStatus();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

        }       
        Notify::deactivate(Notify::messages::ERROR_GENERATOR_ERROR_CONNECTION);
        
        // If the service Tool has been detected the procedure enters here
        // until the service tool exits
        if (isServiceToolConnected()) serviceToolLoop();

        // Inits of the generator
        LogClass::logInFile("Generator connected: Initialization\n");
        if (!generatorInitialization()) {
            if (!connectionTest()) continue;

            LogClass::logInFile("Generator Iitialization Failed!\n");
            Notify::activate(Notify::messages::ERROR_GENERATOR_SETUP);

            setup_completed = true;
            errorLoop();
            Notify::deactivate(Notify::messages::ERROR_GENERATOR_SETUP);
            continue;
        }
        if (!connectionTest()) continue;
        if (isServiceToolConnected()) continue;

        // Clear the system messages
        LogClass::logInFile("Clear System Messages\n");
        while (!clearSystemMessages()) {
            if (!connectionTest()) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        if (!connectionTest()) continue;
        if (isServiceToolConnected()) continue;
        
        // Generator setup
        if (!generatorSetup()) {
            if (!connectionTest()) continue;

            LogClass::logInFile("Generator Setup Failed!\n");
            Notify::activate(Notify::messages::ERROR_GENERATOR_SETUP);

            setup_completed = true;
            errorLoop();
            Notify::deactivate(Notify::messages::ERROR_GENERATOR_SETUP);
            continue;
        }
              

        // Disables the Rx Message
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_SetDisableRx(true);
        handleCommandProcessedState(nullptr);
        
        LogClass::logInFile("Generator Setup Completed: Idle mode activated.\n");
        setup_completed = true;       
        idle_status = true;        
        Notify::deactivate(Notify::messages::WARNING_GENERATOR_NOT_READY);

        // Handles the Idle mode
        generatorIdle();
        
        

    }
       
}

bool  Generator::handleCommandProcessedState(unsigned char* cd) {
    static int cp_timeout = 100;

    while (!R2CP_Eth->isCommandProcessed()) {
        cp_timeout--;

        if (!cp_timeout) {
            cp_timeout = 100;
            LogClass::logInFile("CommandProcessedError: Timeout");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    cp_timeout = 100;

    unsigned char code = R2CP_Eth->getCommandProcessedResult();
    if (cd) *cd = code;

    if (code) {       
        LogClass::logInFile("CommandProcessedError:" + gcnew String(R2CP_Eth->getCommandProcessedString().c_str()));
        return false;
    }
    
    return true;
}

bool Generator::generatorInitialization(void) {

    // Sets the version 6 of the protocol
    while (true) {
       
        // Get the protocol
        while (!R2CP::CaDataDicGen::GetInstance()->isVersionUpdated()) {
            R2CP::CaDataDicGen::GetInstance()->Protocol_Get_Version();
            handleCommandProcessedState(nullptr);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (!connectionTest()) return false;
        }

        if (R2CP::CaDataDicGen::GetInstance()->isProtoV6()) break;

        // Set the version 6 of the protocol            
        R2CP::CaDataDicGen::GetInstance()->Protocol_Set_Version6();
        handleCommandProcessedState(nullptr);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (!connectionTest()) return false;

    }
    
    // Get the current generator status
    LogClass::logInFile("Wait the generator initialization process\n");

    while (true) {
        R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
        if (handleCommandProcessedState(nullptr)) {
            if (R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus != R2CP::Stat_Initialization) break;
        }        
        if (!connectionTest()) return false;
    }

    return true;
}


bool Generator::generatorSetup(void) {
    LogClass::logInFile("GENERATOR: Setup procedure\n");
    
    // Setup the 2D Test (no grid)  procedure
    LogClass::logInFile("GENERATOR: Setup 2D test no grid procedure \n");
    R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Standard_Test, 0);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the 2D Test (with grid)  procedure
    LogClass::logInFile("GENERATOR: Setup 2D test with grid procedure \n");
    R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Standard_Test_with_grid, 0);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the 2D Mammography  procedure
    LogClass::logInFile("GENERATOR: Setup 2D Mammography no AEC procedure \n");
    R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Standard_Mammography_2D, 0);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the 2D Mammography with AEC  procedure
    LogClass::logInFile("GENERATOR: Setup 2D Mammography with AEC procedure \n");
    R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Aec_Mammography_2D, 0);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the 3D Mammography procedure
    LogClass::logInFile("GENERATOR: Setup 3D Mammography  procedure \n");
    R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Standard_Mammography_3D, 25);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the 3D Mammography procedure
    LogClass::logInFile("GENERATOR: Setup 3D Mammography  with AEC procedure \n");
    R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Aec_Mammography_3D, 25);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the Databank for pre pulse
    LogClass::logInFile("GENERATOR: Setup Databank for Pre pulse \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pre, 1, 20, 10, 5000);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the Databank for pulse
    LogClass::logInFile("GENERATOR: Setup Databank for Pulse \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pulse, 1, 20, 10, 5000);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the 3D Databank for Tomo skip pulses
    LogClass::logInFile("GENERATOR: Setup Databank for Tomo Skip pulses \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_SkipPulse_Databank(R2CP::DB_SkipPulse, 0);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pulse Databank to the Index 1 of the Standard test procedure
    LogClass::logInFile("GENERATOR: Assignes Pulse Databank to Index 1 of the  ProcId_Standard_Test \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Standard_Test, 1);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pulse Databank to the Index 1 of the Standard test with grid procedure
    LogClass::logInFile("GENERATOR: Assignes Pulse Databank to Index 1 of the  ProcId_Standard_Test_with_grid \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Standard_Test_with_grid, 1);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pulse Databank to the Index 1 of the Standard Mammography procedure
    LogClass::logInFile("GENERATOR: Assignes Pulse Databank to Index 1 of the  ProcId_Standard_Mammography_2D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Standard_Mammography_2D, 1);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pre-pulse Databank to the Index 1 of the Standard Mammography with AEC procedure
    LogClass::logInFile("GENERATOR: Assignes Pre-Pulse Databank to Index 1 of the  ProcId_Aec_Mammography_2D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pre, R2CP::ProcId_Aec_Mammography_2D, 1);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pulse Databank to the Index 2 of the Standard Mammography with AEC procedure
    LogClass::logInFile("GENERATOR: Assignes Pulse Databank to Index 2 of the  ProcId_Aec_Mammography_2D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Aec_Mammography_2D, 2);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pulse Databank to the Index 1 of the Standard 3D Mammography procedure
    LogClass::logInFile("GENERATOR: Assignes Pulse Databank to Index 1 of the  ProcId_Standard_Mammography_3D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Standard_Mammography_3D, 1);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Skip-Pulse Databank to the Standard 3D Mammography procedure
    LogClass::logInFile("GENERATOR: Assignes Skip Databank to the  ProcId_Standard_Mammography_3D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_Assign_SkipPulse_Databank(R2CP::ProcId_Standard_Mammography_3D, R2CP::DB_SkipPulse);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pre-pulse Databank to the Index 1 of the Standard 3D Mammography with AEC procedure
    LogClass::logInFile("GENERATOR: Assignes Pre-Pulse Databank to Index 1 of the  ProcId_Aec_Mammography_3D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pre, R2CP::ProcId_Aec_Mammography_3D, 1);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pulse Databank to the Index 2 of the Standard 3D Mammography with AEC procedure
    LogClass::logInFile("GENERATOR: Assignes Pulse Databank to Index 2 of the  ProcId_Aec_Mammography_3D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Aec_Mammography_3D, 2);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Skip-Pulse Databank to the  Standard 3D Mammography with AEC procedure
    LogClass::logInFile("GENERATOR: Assignes Skip Databank to the  ProcId_Aec_Mammography_3D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_Assign_SkipPulse_Databank(R2CP::ProcId_Aec_Mammography_3D, R2CP::DB_SkipPulse);
    if (!handleCommandProcessedState(nullptr)) return false;

    LogClass::logInFile("GENERATOR: Setup successfully terminated \n");
    return true;
}

bool Generator::clearSystemMessages(void) {

    // Gets all the active messages present in the generator
    R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();
    if (!handleCommandProcessedState(nullptr)) return false;

    // No active messages are present
    if (R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size() == 0) return true;

    // Reset all the errors
    for (int i = 0; i < R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size(); i++) {
        unsigned int id = R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList[i];
        LogClass::logInFile("GENERATOR: removing message: " + id.ToString() + "\n");
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_Clear_Message(id);
        if (!handleCommandProcessedState(nullptr)) return false;
    }

    // Read again the messages
    R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();
    if (!handleCommandProcessedState(nullptr)) return false;

    // Test if the messages have been removed
    if (R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size()) {
        LogClass::logInFile("GENERATOR: system messages still present! \n");        
    }else LogClass::logInFile("GENERATOR: No more system messages are present! \n");

    return true;
}

bool Generator::startExposure(void) {

    // test if the Exposure mode is correctly set
    if (ExposureModule::getExposureMode() == ExposureModule::exposure_type_options::EXP_NOT_DEFINED) return false;

    // Test if the pulse 0 is actually valid
    if (!ExposureModule::getExposurePulse(0)->validated) return false;

    if (!idle_status) return false; // Only in Stand-by mode can be initiated
    if (!ready_for_exposure) return false; // Only in Stand-by mode can be initiated
    if (xray_processing) return false; // A X-RAY procedure is processing (busy condition)
    xray_processing = true;
    ExposureModule::clearXrayCompleted();
    
    // Invalidates the Pulse 0 because it is consumed.
    ExposureModule::getExposurePulse(0)->validated = false;
    return true;
}

bool Generator::generatorIdle(void) {
    static unsigned char generator_status = 0;
    ExposureModule::exposure_completed_errors exposure_err_code;

    while (true) {
        
        // Ethernet disconnection detected
        if (!connectionTest()) return false;
        if (isServiceToolConnected()) return false;

        // Request the Generator Node status
        R2CP::CaDataDicGen::GetInstance()->Network_GetGeneratorNodeStatus();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (!R2CP_Eth->generatorConnected) return false;

        // Gets the current generator status
        R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
        if (!handleCommandProcessedState(nullptr)) return false;

        if (generator_status != R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus) {
            generator_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
            LogClass::logInFile("GENERATOR: Current generator status: " + generator_status.ToString() + "\n");

            if (generator_status == 2) {
                Notify::deactivate(Notify::messages::WARNING_GENERATOR_NOT_READY);
                ready_for_exposure = true;
            }
            else {
                Notify::activate(Notify::messages::WARNING_GENERATOR_NOT_READY);
                ready_for_exposure = false;
            }
        }

        // Get the error list
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();
        if (!handleCommandProcessedState(nullptr)) return false;

        if (R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size()) {
            if (!generatorErrorMessagesLoop()) return false;
            continue;
        }
        
        // If not in Standby cannot be processed a request for an exposure sequence
        if (generator_status != 2) continue;

        
        if (xray_processing) {
            
            // Resets of the exposed pulses for all data-bank
            // The Exposure sequence fills the Data Bank used if the generated pulses real values.
            // NOTE: a single Pulse can be a sum of several partial pulses due to the Grid interruption.
            // See the function Generator_ExposureManagement_RadPostExposure in CaDataDicGen_GenIndex.cpp file
            R2CP::CaDataDicGen::GetInstance()->resetExecutedPulse();

            // reset of the pulses
            ExposureModule::setExposedPulse(0, gcnew ExposureModule::exposure_pulse());
            ExposureModule::setExposedPulse(1, gcnew ExposureModule::exposure_pulse());
            ExposureModule::setExposedPulse(2, gcnew ExposureModule::exposure_pulse());
            ExposureModule::setExposedPulse(3, gcnew ExposureModule::exposure_pulse());

            // Every Xray procedure return True if the seqeunce is completed if success.
            // In case of false returned (error condition), the   xray_exposure_error is set properly
            switch (ExposureModule::getExposureMode()) {
            case ExposureModule::exposure_type_options::TEST_2D: exposure_err_code = test_exposure_procedure();  break;
            case ExposureModule::exposure_type_options::MAN_2D: exposure_err_code = man_2d_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::AEC_2D: exposure_err_code = aec_2d_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::MAN_3D: exposure_err_code = man_3d_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::AEC_3D: exposure_err_code = aec_3d_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::MAN_COMBO: exposure_err_code = man_combo_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::AEC_COMBO: exposure_err_code = aec_combo_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::MAN_AE: exposure_err_code = man_ae_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::AEC_AE: exposure_err_code = aec_ae_exposure_procedure(); break;
            default:         
                exposure_err_code = ExposureModule::exposure_completed_errors::XRAY_INVALID_PROCEDURE;
            }

            // Removes the X-RAY ena signal 
            PCB301::set_xray_ena(false);
            
            ExposureModule::setCompletedError(exposure_err_code);

            // The X-Ray procedure termines here: the complete event is generated
            // In case of sequence not completed (partial or total) is App to the application 
            // retrive the error code (xray_exposure_error).
            if (exposure_err_code == ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS)  ExposureModule::setCompletedCode(ExposureModule::exposure_completed_options::XRAY_SUCCESS);// xray_complete_event(exposure_completed_options::XRAY_SUCCESS);
            else if (ExposureModule::getExposedPulse(0)->getmAs()) ExposureModule::setCompletedCode(ExposureModule::exposure_completed_options::XRAY_PARTIAL_DOSE);
            else ExposureModule::setCompletedCode(ExposureModule::exposure_completed_options::XRAY_NO_DOSE);
            LogClass::logInFile("GENERATOR EXPOSURE RESULT:" + ExposureModule::getExposureCompletedCode().ToString() + "-" + exposure_err_code.ToString());

            
            // De-synch the grid device
            PCB304::synchGridWithGenerator(false);

            // Only in operating mode
            if (ExposureModule::getExposureMode() != ExposureModule::exposure_type_options::TEST_2D) {

                // Unlock the compressor if requested
                if (ExposureModule::getCompressorMode() == ExposureModule::compression_mode_option::CMP_RELEASE) PCB302::setCompressorUnlock();

                // Notify the AWS about the XRAY completed event
                awsProtocol::EVENT_XraySequenceCompleted();

                // Disable the Xray Button
                ExposureModule::enableXrayPushButtonEvent(false);

                // Invalidate the current projection
                ArmMotor::abortTarget();
            }
            

            // Waits for the X-RAY button release
            if (PCB301::getXrayPushButtonStat()) {
                LogClass::logInFile("GENERATOR EXPOSURE WAITING BUTTON RELEASE");
                while (PCB301::getXrayPushButtonStat()) std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            LogClass::logInFile("GENERATOR EXPOSURE COMPLETED");
            xray_processing = false;
            ExposureModule::setXrayCompletedFlag(); // The Exposure is completed and the data are available
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return true;
}

bool Generator::generatorErrorMessagesLoop(void) {


    String^ error_list_id = "";
    String^ current_error_list_id = "";

    // Notify themessages presents
    for (int i = 0; i < R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size(); i++) {
        unsigned int id = R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList[i];
        current_error_list_id += id.ToString() + " ";        
    }
    error_list_id = current_error_list_id;
    Notify::activate(Notify::messages::WARNING_GENERATOR_MESSAGE, current_error_list_id);
    LogClass::logInFile("GENERATOR: system messages present:" + error_list_id + "\n");

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));


        // Get the error list
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();
        if (!handleCommandProcessedState(nullptr)) return false;

        if (R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size() == 0) {
            Notify::deactivate(Notify::messages::WARNING_GENERATOR_MESSAGE);
            LogClass::logInFile("GENERATOR: system messages removed \n");
            return true;
        }
        
        // Notify the messages presents
        current_error_list_id = "";
        for (int i = 0; i < R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size(); i++) {
            unsigned int id = R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList[i];
            current_error_list_id += id.ToString() + " ";            
        }

        if (current_error_list_id != error_list_id) {
            error_list_id = current_error_list_id;
            Notify::deactivate(Notify::messages::WARNING_GENERATOR_MESSAGE);
            Notify::activate(Notify::messages::WARNING_GENERATOR_MESSAGE, current_error_list_id);
            LogClass::logInFile("GENERATOR: system messages present:" + error_list_id + "\n");
        }

        
        // Reset all the errors
        for (int i = 0; i < R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size(); i++) {
            unsigned int id = R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList[i];
            R2CP::CaDataDicGen::GetInstance()->SystemMessages_Clear_Message(id);
            if (!handleCommandProcessedState(nullptr)) return false;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        
        
    }
}


bool Generator::logCurrentStatus(unsigned char status) {
    static unsigned char old_status = 255;

    // init status
    if (status == 255) {
        old_status = 255;
        return true;
    }

    if (old_status == status) return false;
    old_status = status;

    switch (status) {
    case R2CP::Stat_Standby: LogClass::logInFile("GENERATOR STATUS: Stat_Standby"); break;
    case R2CP::Stat_Error:LogClass::logInFile("GENERATOR STATUS: Stat_Error"); break;
    case R2CP::Stat_WaitFootRelease:LogClass::logInFile("GENERATOR STATUS: Stat_WaitFootRelease"); break;
    case R2CP::Stat_GoigToShutdown:LogClass::logInFile("GENERATOR STATUS: Stat_GoigToShutdown"); break;
    case R2CP::Stat_Service:LogClass::logInFile("GENERATOR STATUS: Stat_Service"); break;
    case R2CP::Stat_Initialization:LogClass::logInFile("GENERATOR STATUS: Stat_Initialization"); break;
    case R2CP::Stat_ExpInProgress:LogClass::logInFile("GENERATOR STATUS: Stat_ExpInProgress"); break;
    case R2CP::Stat_Ready:LogClass::logInFile("GENERATOR STATUS: Stat_Ready"); break;
    default: LogClass::logInFile("GENERATOR STATUS: Unknown status");
    }

    return true;
}

void Generator::setExposedData(unsigned char databank_index, unsigned char pulse_seq, PCB315::filterMaterialCodes ft, unsigned char fc) {
    if (R2CP::CaDataDicGen::GetInstance()->executed_pulses[databank_index].samples) {
        ExposureModule::setExposedPulse(pulse_seq, gcnew ExposureModule::exposure_pulse(
            R2CP::CaDataDicGen::GetInstance()->executed_pulses[databank_index].kV,
            R2CP::CaDataDicGen::GetInstance()->executed_pulses[databank_index].mAs,
            ft,
            R2CP::CaDataDicGen::GetInstance()->executed_pulses[databank_index].mA,
            R2CP::CaDataDicGen::GetInstance()->executed_pulses[databank_index].ms,
            fc,
            R2CP::CaDataDicGen::GetInstance()->executed_pulses[databank_index].samples
        ));
    }
}