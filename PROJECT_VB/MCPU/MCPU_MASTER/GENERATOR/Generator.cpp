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


#using "SQLITE\\System.Data.SQLite.dll" // put this file in the project directory
using namespace System::Data::SQLite;

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
        return;
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

        // Sets the version 6 of the protocol
        LogClass::logInFile("Setting protocol version\n");        
        
        while (!R2CP::CaDataDicGen::GetInstance()->isVersionUpdated()) {
            R2CP::CaDataDicGen::GetInstance()->Protocol_Get_Version();
            handleCommandProcessedState(nullptr);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (!connectionTest()) break;
        }
        if (!connectionTest()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }
     
        while (true) {

            // If the version is already the 6 finish here
            if (R2CP::CaDataDicGen::GetInstance()->isProtoV6()) break;

            // Set the version 6 of the protocol            
            R2CP::CaDataDicGen::GetInstance()->Protocol_Set_Version6();
            handleCommandProcessedState(nullptr);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (!connectionTest()) break;

        }

        if (!connectionTest()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }

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
        generatorIdleLoop();
        
        

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
        LogClass::logInFile("CommandProcessedError:" + code.ToString() + " >" +  gcnew String(R2CP_Eth->getCommandProcessedString().c_str()));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            if (!connectionTest()) return false;
        }

        // If the version is already the 6 finish here
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
        if (!handleCommandProcessedState(nullptr)) return false; 
        if (R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus != R2CP::Stat_Initialization) break;                
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

bool Generator::getGeneratorStatus(void) {

    // Gets the current generator status
    R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
    if (!handleCommandProcessedState(nullptr)) return false;
    if (current_status == R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus) return false;
    current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    switch (current_status) {
    case R2CP::Stat_Standby: LogClass::logInFile("GENERATOR STATUS: Stat_Standby"); break;
    case R2CP::Stat_Preparation: LogClass::logInFile("GENERATOR STATUS: Stat_Preparation"); break;
    case R2CP::Stat_ExpReq: LogClass::logInFile("GENERATOR STATUS: Stat_ExpReq"); break;
    case R2CP::Stat_Error:LogClass::logInFile("GENERATOR STATUS: Stat_Error"); break;
    case R2CP::Stat_WaitFootRelease:LogClass::logInFile("GENERATOR STATUS: Stat_WaitFootRelease"); break;
    case R2CP::Stat_GoigToShutdown:LogClass::logInFile("GENERATOR STATUS: Stat_GoigToShutdown"); break;
    case R2CP::Stat_Service:LogClass::logInFile("GENERATOR STATUS: Stat_Service"); break;
    case R2CP::Stat_Initialization:LogClass::logInFile("GENERATOR STATUS: Stat_Initialization"); break;
    case R2CP::Stat_ExpInProgress:LogClass::logInFile("GENERATOR STATUS: Stat_ExpInProgress"); break;
    case R2CP::Stat_Ready:LogClass::logInFile("GENERATOR STATUS: Stat_Ready"); break;
    default: LogClass::logInFile("GENERATOR STATUS: (unknown name) > " + current_status.ToString());
    }

    return true;
}

/// <summary>
/// This is the Loop procedure handling the presence of active Generator System Messages.
/// 
/// </summary>
/// 
/// The presence of internal system messages in the generator device means that 
/// the generator is facing with internal anomalies or invalid status that prevent
/// a correct exposure activation.
/// 
/// The Loop procedure calls a Module routine (R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages() )  
/// to get all the active messages from the device. \n The routine internally filter out a special message 
/// that is used to prevent an unwanted exposure procedure and, consequently, is not part of actual malfunctions.
/// 
/// The Loop remains alive until no system messages are detected active.
/// 
/// The Generator module in this execution status cannot activate any exposure procedure.
/// 
/// The loop activates a dedicated error message in the application:
/// + The descriptiono of the current error (errors) in string format is retrived by the
///   sqlite database of the generator software tool set.
/// 
/// 
/// <param name=""></param>
/// <returns>
/// + False: in case of error in communication with the generator device
/// + True: in case of no system messages are present and communication ok;
/// </returns>
bool Generator::generatorErrorMessagesLoop(void) {

    String^ error_list_id = "";
    String^ current_error_list_id = "";
    String^ description_list = "";
    static bool alarm_active = true;

    while (true) {

        // Get the error list
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();
        if (!handleCommandProcessedState(nullptr)) {
            alarm_active = true;
            return false;
        }

        // No more messages are presents. Successfully Exits
        if (R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size() == 0) {

            // If an alram has benn activated it is deactivated
            if (alarm_active)  Notify::deactivate(Notify::messages::ERROR_GENERATOR_SYS_MESSAGE);
            alarm_active = false;
            return true;
        }

        alarm_active = true;

        // Compile the list of errors and retrive the error description from the sqlite database
        current_error_list_id = "";
        description_list = "";
        for (int i = 0; i < R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size(); i++) {
            unsigned int id = R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList[i];
            current_error_list_id += id.ToString() + " ";
            description_list += "[" + id.ToString() + "] " + getGeneratorErrorString(id.ToString()) + "\n";
        }

        // If the current error list is changed, the log and the Application errors are updated
        if (current_error_list_id != error_list_id) {
            error_list_id = current_error_list_id;
            Notify::deactivate(Notify::messages::ERROR_GENERATOR_SYS_MESSAGE);
            Notify::activate(Notify::messages::ERROR_GENERATOR_SYS_MESSAGE, description_list);
            LogClass::logInFile("GENERATOR: system messages present:" + description_list);
        }


        // Try to reset all the errors and repeat the loop
        for (int i = 0; i < R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size(); i++) {
            unsigned int id = R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList[i];
            R2CP::CaDataDicGen::GetInstance()->SystemMessages_Clear_Message(id);
            if (!handleCommandProcessedState(nullptr)) {
                alarm_active = true;
                return false;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }

    // Unexpected status here
    alarm_active = true;
    return false;
}

/// <summary>
/// This function is called in the Generator Idle status in order to handle an Exposure procedure.
/// 
/// </summary>
/// 
/// The Procedure handle the activation and the completion of any exposure procedure:
/// 
/// The application should have used the dedicated API routine to activate an exposure sequence:
/// the ExposureModule class provides all the API to prrepare and select the further exposure procedure.
/// 
/// This function:
/// + Resets the internal and external Exposed-Pulse data structures:
///     + Those structure will be filled, after the exposure completion,  with the actual exposed parameters like kV, mAs,..
/// + Checks for the XRAY-push button activation;
/// + Checks for the Tube temperature validity;
/// + Pre-select the current filter that will be used;
/// + Starts the dedicated exsposure procedure;
/// + Evaluates the exposure completion code;
/// + If requested, interact with the Acquisition software to signal the exposure completion event;
/// + Handles the final status of the compressor device (keep compression or release);
/// 
/// 
/// <param name=""></param>
void Generator::handleExposureActivation(void) {

    ExposureModule::exposure_completed_errors exposure_err_code;

    // Resets of the Internal Generator Module exposed pulses for all data-banks    
    R2CP::CaDataDicGen::GetInstance()->resetExecutedPulse();

    // Reset of the Application Exposure Pulses
    ExposureModule::setExposedPulse(0, gcnew ExposureModule::exposure_pulse());
    ExposureModule::setExposedPulse(1, gcnew ExposureModule::exposure_pulse());
    ExposureModule::setExposedPulse(2, gcnew ExposureModule::exposure_pulse());
    ExposureModule::setExposedPulse(3, gcnew ExposureModule::exposure_pulse());

    // Common preparation Tests
    exposure_err_code = ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS;

    if (!PCB301::getXrayPushButtonStat()) {
        // Checks if the X-RAY push button is pressed
        exposure_err_code = ExposureModule::exposure_completed_errors::XRAY_BUTTON_RELEASE;
    }
    else if (PCB315::isTubeAlarm()) {
        // Checks for the Tube temperature
        exposure_err_code = ExposureModule::exposure_completed_errors::XRAY_TUBE_TEMPERATURE;
    }
    else {

        // Preliminary selection of the filter without waiting the command completion. The given seqeunce will check for completion
        PCB315::setFilterAutoMode(ExposureModule::getExposurePulse(0)->filter, false);

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

/// <summary>
/// This function loop handles the Generator Idle status.
/// </summary>
/// 
/// The Generator Idle status is a module status where an exposure procedure can be initiated.
/// 
/// The Generator Idle Status is activated as soon after the System Startup and 
/// the first Generator Setup procedure.
/// 
/// During the Idle Status:
/// + The correct connection with the smart hub is checked;
/// + The correct connection with the generator device is checked;
/// + The absence of the external Generator Service Tool software is checked;
/// 
/// If any of the previous condition should fail this routine exits 
/// and the main thread routine with a new startup sequence will be 
/// activated.
/// 
/// During the Idle Status, the presence of System Messages is checked:
/// + in case of system message presence, the Loop calls the generatorErrorMessagesLoop()
/// in order to handle the messages.
/// 
/// Finally, only if no system messages are present and only if 
/// the Generator internal status should be the R2CP::Stat_Standby,\n
/// a possible exposure activation procedure can be evaluated (xray_processing variable).
/// 
/// 
/// 
/// <param name=""></param>
/// <returns></returns>
bool Generator::generatorIdleLoop(void) {    
    

    while (true) {
        
        // Ethernet disconnection detected
        if (!connectionTest()) return false;
        if (isServiceToolConnected()) return false;

        // Request the Generator Node status
        R2CP::CaDataDicGen::GetInstance()->Network_GetGeneratorNodeStatus();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (!R2CP_Eth->generatorConnected) return false;

        // Handles the generator system error messages
        if (!generatorErrorMessagesLoop()) return false;
    

        // Handles the Application warning message related to the actual ready status
        if (getGeneratorStatus()) {
            
            // Status changed
            if (current_status == R2CP::Stat_Standby) {
                Notify::deactivate(Notify::messages::WARNING_GENERATOR_NOT_READY);
                ready_for_exposure = true;
            }
            else {
                Notify::activate(Notify::messages::WARNING_GENERATOR_NOT_READY);
                ready_for_exposure = false;                
            }
        }

        // If not in Standby cannot be processed a request for an exposure sequence
        if (current_status != R2CP::Stat_Standby) continue;
        if (xray_processing) handleExposureActivation();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }


    // Unexpected status
    return false;
}


/// <summary>
/// This function gets the data pulses received by the generator during the single Pulse sequence
/// and store it into the exposure pulse sequence number data structure.
/// </summary>
/// 
/// Every Exposure sequence can be composed with a max of four singlePulses.
/// Those pulses are referred in the application as pulse-0 to pulse-3.
/// 
/// The application gets the executed pulse exposure data (kv, mAs, ..) reading 
/// the data in the class ExposureModule::exposed after the exposure completed.
/// 
/// The actual exposed data are collected internally by the genberator module
/// into the  R2CP::CaDataDicGen::GetInstance()->executed_pulses[i] vector 
/// where i is the databank index passed to the Procedure definition.
/// 
/// This function is called after every single-pulse sequence 
/// to properly upload the Application data structure with the internal collected
/// exposure data.
/// 
/// 
/// <param name="databank_index">
/// This is the index assigned to the current data-bank in the procedure\n
/// The index is not the number of the databank (i.e. R2CP::DB_Pre or R2CP::DB_Pulse)\n
/// The index is the number assigned to it when the databank is assigne to a procedure
/// 
/// Example:\n
/// R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Standard_Mammography_2D, 1);
///     - in this example 1 is the index to be used!    
/// 
/// </param>
/// <param name="pulse_seq">This is the current pulse sequence (0 to 3) </param>
/// <param name="ft">Filter used in the exposure</param>
/// <param name="fc">This is the focus used in the exposure</param>
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

/// <summary>
/// This function controls the generator execution of a single exposure pulse. 
/// 
/// </summary>
/// 
/// The Single Pulse is any pulse composing a complete exposure sequence:\n
/// - The Manual 2D sequence is composed by only one pulse-Sequence;
/// - The AEC 2D sequence is composed by two pulse-Sequences (the pre-pulse and main-pulse);
/// - The 3D manual Exposure is composed by only one pulse-Sequence, where the sequence is a multi kv pulse output;
/// - The 3D AEC Exposure is composed by two pulse-Sequence ( the pre-pulse and the main train of kv pulses);
/// - And so on...
/// 
/// The procedure:
/// - waits to exit from the stand-by before to follow the genratore sequence;
/// - follows the generator status changes until the Stand-By or the WaitFootRelease status is detected;
/// 
/// The procedure will fail always when:
/// - an invalid status is detected;
/// - the procedure timeout expires;
/// - the x-ray enable signal is released (x-ray push button early release);
/// 
/// <param name="ExpName">A string used to log the name of the current exposure sequence</param>
/// <param name="ms_timeout">the timeout assigned to the execution of a pulse in ms</param>
/// <returns>The procedure returns the ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS  if csuccessfully completes</returns>
ExposureModule::exposure_completed_errors Generator::pulseSequence(System::String^ ExpName, int ms_timeout) {
    int time_interval = 50;
    int timeout_tic = ms_timeout / time_interval;

    // Wait to exit from the generator standby status
    int timeout = timeout_tic;
    while (timeout--) {

        // Checks for the XRAY button
        if (!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus) {
            LogClass::logInFile(ExpName + "push button release error");
            return ExposureModule::exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }

        // Checks for error messages
        if (R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active) {
            LogClass::logInFile(ExpName + "generator system messages during exposure error");
            return ExposureModule::exposure_completed_errors::XRAY_GENERATOR_ERROR;
        }

        // Brake as soon a non standby condition is detected
        getGeneratorStatus();
        if (current_status != R2CP::Stat_Standby) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(time_interval));
    }
    
    if (!timeout) {
        LogClass::logInFile(ExpName + "timeout waiting generator executing the xray sequence");
        return ExposureModule::exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
    }

    // The sequence will completes when generator returns in Standby or Stat_WaitFootRelease.
    // The sequence termines as sooon a wrong expected status should be detected
    timeout = timeout_tic;
    while (timeout--) {

        // Checks for the XRAY button
        if (!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus) {
            LogClass::logInFile(ExpName + "push button release error");
            return ExposureModule::exposure_completed_errors::XRAY_BUTTON_RELEASE;
        }

        // Checks for error messages
        if (R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active) {
            LogClass::logInFile(ExpName + "generator system messages during exposure error");
            return ExposureModule::exposure_completed_errors::XRAY_GENERATOR_ERROR;
        }
       
        if (getGeneratorStatus()) {
            // Waits for the Exposure complete event
            switch (current_status) {

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_Standby:               
                return ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS;                

            case R2CP::Stat_Error:
                LogClass::logInFile(ExpName + "generator R2CP::Stat_Error error ");
                return ExposureModule::exposure_completed_errors::XRAY_GENERATOR_ERROR;               

            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                LogClass::logInFile(ExpName + current_status.ToString() + " status error ");
                return ExposureModule::exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
                

            default:
                return ExposureModule::exposure_completed_errors::XRAY_INVALID_GENERATOR_STATUS;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(time_interval));
    }

    LogClass::logInFile(ExpName + "generator sequence timeout error");
    return ExposureModule::exposure_completed_errors::XRAY_TIMEOUT;
}

/// <summary>
/// This function load a databank with a 3 point tech to fit the finest mAs required value.
/// </summary>
/// 
/// When a decimal value is needed for the mAs (i.e. 10.5 mAs instead of 10 or 11)
/// the 2 point tech cannot be used because this tech can set only integer values.
/// 
/// In the case where the decimal part of the mAs should be important (i.e. in Tomo Exposures)
/// a different approach shall be followed:
/// 
/// The 3 point method allow to set the kV, the Anodic m Amps and the milliseconds of exposure.
/// The Anodic current can be controlle with enough accuracy, wheathe the Exposure time can be selected in a discrete 
/// range of values in the R10 table.
/// 
/// The method consist of the following procedure:
/// - The 2 point databank is uploaded into the generator with the mAs in integer format:\n
///     this step is necessary to know what is the available anodic current for the kV and mAs range selected;
/// - The generator then will assignes the proper anodic current and the integration time requested;
/// - The procedure select a new integration time bigger than the one selected by the genrator (in the R10 scale) so that \n
///   it will be possible to use a lower anodic current (a bigger value could not be usable because of Tube limitations);
/// - A new anodic current is calculated based on the requested mAs and integration time;
/// - A databank with 3 point tech is then uploaded with the new calculated data.
/// 
/// 
/// <param name="dbId">Databank index</param>
/// <param name="large_focus">true for large focuse, false for small focus</param>
/// <param name="KV">kV value</param>
/// <param name="MAS">mAs value</param>
/// <param name="n_pulse">number of pulses (1 in case of databank for a 2D procedure)</param>
/// <param name="min_pulse">minimum time for pulse in ms</param>
/// <param name="max_pulse">maximum pulse time (limited usually by the Max integration time of the Detector )</param>
/// <returns>ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS for success</returns>
ExposureModule::exposure_completed_errors Generator::set3PointDatabank(unsigned char dbId, bool large_focus, float KV, float MAS, int n_pulse, int min_pulse, int max_pulse) {
    
    unsigned short mas;
    // Rounding to the next integer value of the requested mAs 
    if ((float)((unsigned short) MAS) != MAS) mas = ((unsigned short) MAS + 1);
    else mas = ((unsigned short) MAS);

    // Sets the 2D databank to force the generator to select the mA and mS(R10 tab) for an exposure with the requested mAs
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(dbId, large_focus, KV, mas, n_pulse * max_pulse);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile("set3PointDatabank() - error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Gets the R10 time imposed by the generator
    float pulse_mS = R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[dbId].ms100.value / 100;   

    // Gets the R10 value corresponding to the minimum pulse time acceptable
    float min_r10_mS = getR10Time(min_pulse, true);

    // if the assigned pulse time is less than the minimum, the minimum is used
    if ((pulse_mS < min_pulse) && (min_r10_mS < max_pulse)) pulse_mS = min_r10_mS;

    // The maximum anodic current selected by Generator cannot fit into the Integration Window
    if (pulse_mS > max_pulse) {
        LogClass::logInFile("set3PointDatabank() - Invalid integration time for the current tomo pulse");
        return ExposureModule::exposure_completed_errors::XRAY_INVALID_TOMO_mAs;
    }

    // Recalculate the Anodic current that can fit with the R10 time slot
    float mA = (MAS * 1000) / pulse_mS;

    // Uses a 3 point procedure to better fit with the requested mAs
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_3D_Databank(dbId, large_focus, KV, mA, pulse_mS, max_pulse);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile("set3PointDatabank() - Generator_Set_3D_Databank() error");
        return ExposureModule::exposure_completed_errors::XRAY_COMMUNICATION_ERROR;
    }

    // Gets the Anodic current imposed by the generator
    float pulse_mA = ((float) R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[dbId].mA100.value) / 100;
    float err_100_mAs = (abs((pulse_mA * pulse_mS / 1000) - MAS) * 100 ) / MAS;

    // Max allowed error is 1%
    if (err_100_mAs > 1.0) {
        LogClass::logInFile("set3PointDatabank(): ERROR! pulse_mA:" + pulse_mA + "; pulse_mS:" + pulse_mS + "; mAs Error (\%):" + err_100_mAs);
        return ExposureModule::exposure_completed_errors::XRAY_INVALID_TOMO_mAs;
    }
    else {
        LogClass::logInFile("set3PointDatabank() -> pulse_mA:" + pulse_mA + "; pulse_mS:" + pulse_mS + "; mAs Error (\%):" + err_100_mAs);
    }
    
    return ExposureModule::exposure_completed_errors::XRAY_NO_ERRORS;
}

/**
* @brief This function returns the closed lower or higher time from the reinard R10 scale
*
* The Function returns the next R10 value or the Previous R10 value \n
* from the requested parameter.
*
* @param ms: this is the nominal time value
* @param next: true = next value, false = previous value
* @return the requested time of the R10 scale
* @return 0: value not found
*/
float Generator::getR10Time(float ms, bool next) {
    static float r10[] = { 1,1.25,1.6,2,2.5,3.2,4,5,6.3,8,10 };
    float mult = 1;

    if (ms >= 10000) mult = 10000;
    else if (ms >= 1000) mult = 1000;
    else if (ms >= 100) mult = 100;
    else if (ms >= 10) mult = 10;
    ms = ms / mult;

    for (int i = 9; i >= 0; i--) {
        if (ms == r10[i]) return r10[i] * mult;
        else if (ms > r10[i]) {
            if (next) return r10[i + 1] * mult;
            else return r10[i] * mult;
        }
    }
    return 0;
}


/// <summary>
/// This function returns a description strings of the System Message Error.
/// 
/// </summary>
/// 
/// The whole set of the system messages are stored into an SQLite database file
/// in the OEM/AppData/system_messages.sqlite file.
/// 
/// This function makes use of the message id code to retrive the message description
/// in a readable string format. 
/// 
/// <param name="errstr">This is the message identifier code in string format</param>
/// <returns>A desciption string of the related system message</returns>
System::String^ Generator::getGeneratorErrorString(System::String^ errstr) {

    SQLiteConnection conn("Data Source=C:\\OEM\\AppData\\system_messages.sqlite;");
    
    try {
        conn.Open();
        System::String^ result;

        SQLiteCommand sqlCmd("SELECT ID, DESC FROM SystemMessage WHERE ID=" + errstr, % conn);
        SQLiteDataReader^ sqReader = sqlCmd.ExecuteReader();

        if (sqReader->Read()) result = sqReader->GetString(1);
        else result = "";

        sqReader->Close();
        conn.Close();

        return result;
    }
    catch (...) {
        return "";
    }
    
}
