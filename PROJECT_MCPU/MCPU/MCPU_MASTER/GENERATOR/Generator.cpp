#include "R2CP_Eth.h"
#include "CaDataDicGen.h"
#include "SystemConfig.h"
#include "Generator.h"
#include "gantry_global_status.h"
#include "Notify.h"
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



/// <summary>
/// This is the class constructor
/// </summary>
/// 
/// The Constructor instances the base class with the Smart Hub IP and Port address
/// <param name=""></param>
Generator::Generator(void) :TcpClientCLI( SH_IP, SH_PORT)
{
    // gets the instance pointer for the non static functions
    device = this;
 
}

/// <summary>
/// This function starts the Generator Module in Normal (non simulated) mode
/// </summary>
/// 
/// <param name=""></param>
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

/// <summary>
/// This function starts the Generator Module in Simulation mode
/// </summary>
/// 
/// The simulation mode provides a transparent Application interface
/// that doesn't make use of the Generator device but allowing the 
/// application to activate the exposure workflows in a simulated way.
/// 
/// <param name=""></param>
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

/// <summary>
/// This method provides the current smart hub connection status
/// </summary>
/// <param name=""></param>
/// <returns>True if a valid communication with the Smat Hub is present </returns>
bool Generator::isSmartHubConnected(void) {
    if (!device->isConnected()) return false;
    return R2CP_Eth->smartHubConnected;
}

/// <summary>
/// This method provides the current Generator connection status
/// </summary>
/// <param name=""></param>
/// <returns>True if a valid communication with the Generator device is present</returns>
bool Generator::isGeneratorConnected(void) {
    if (!device->isConnected()) return false;
    return R2CP_Eth->generatorConnected;
}

/// <summary>
/// This method provides the current Service Tool detection status
/// </summary>
/// 
/// If the External Service Tool software should be executed 
/// the Generator module stop working unil the software should be 
/// closed.
/// 
/// The Application can monitor this particolar running mode using 
/// this method
/// <param name=""></param>
/// <returns>True if the Service Tool is running on the Gantry PC</returns>
bool Generator::isServiceToolConnected(void) {
    if (device->simulator_mode) return false;
    return R2CP_Eth->serviceToolConnected;
}

/// <summary>
/// This function tests if the whole communication with the Generator System is present
/// </summary>
/// <param name=""></param>
/// <returns>True if the connection with the Smart hub and the Generator device is present</returns>
bool Generator::connectionTest(void) {
    if (!isConnected()) return false;
    if (!R2CP_Eth->smartHubConnected)  return false;
    if (!R2CP_Eth->generatorConnected)  return false;
    return true;

}

/// <summary>
/// This is the Simulation Mode Working thread
/// </summary>
/// 
/// The Working Thread of the simulation mode 
/// sets all the internal status in ready for exposure.
/// 
/// the exposureManagementLoop() is called every 100ms with the demo mode flag set to true.
/// 
/// <param name=""></param>
void Generator::simulatorWork(void) {
    LogClass::logInFile("Generator Simulator Version!\n");

    while (true) {
        idle_status = true;
        ready_for_exposure = true;
        exposureManagementLoop(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

/// <summary>
///  If the initial configuration of the Generator device should fail 
///  the main thread call this routine every 1000 ms forever
/// </summary>
/// <param name=""></param>
void Generator::errorLoop(void) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return;
    }
}

/// <summary>
/// If the Service Tool Software should be detected the module main thread 
/// calls this loop routine until the Service Tool software is closed.
/// 
/// </summary>
/// 
/// 
/// <param name=""></param>
void Generator::serviceToolLoop(void) {
    LogClass::logInFile("Service tool detected!\n");
    
    while (isServiceToolConnected()) {
        //Activates the XRAY-ENA to allows the service tool activities
        setXrayEnable(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    LogClass::logInFile("Service tool exited\n");
    setXrayEnable(false);

}

/// <summary>
/// This is the Normal Mode working thread
/// </summary>
/// 
/// The Main Thread starts at the beginning of the program when in Normal Mode.
/// 
/// The Main thread executes:
/// - Initial connection with the generator and smart hub;
/// - Generator Setup;
/// - Service Tool monitoring;
/// 
/// If all the previous condition temrines properly 
/// the generatorIdleLoop() is called.
/// 
/// If the generatorIdleLoop() should return, the main thread repeats 
/// all the previous steps, reinitializing the Generator and the Module.
/// 
/// <param name=""></param>
void Generator::threadWork(void) {
  
    while (true) {
        LogClass::logInFile("Try to connect the Smart Hub and Generator!\n");
        Notify::activate(Notify::messages::ERROR_GENERATOR_ERROR_CONNECTION);                
        Notify::activate(Notify::messages::WARNING_GENERATOR_NOT_READY);

        R2CP_Eth->smartHubConnected = false;
        R2CP_Eth->generatorConnected = false;
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

            errorLoop();
            Notify::deactivate(Notify::messages::ERROR_GENERATOR_SETUP);
            continue;
        }
              

        // Activates the Disables Rx Message         
        setDisableRxMessage(true);
        
        LogClass::logInFile("Generator Setup Completed: Idle mode activated.\n");       
        idle_status = true;        
        Notify::deactivate(Notify::messages::WARNING_GENERATOR_NOT_READY);

        // Handles the Idle mode
        generatorIdleLoop();
        
        

    }
       
}

/// <summary>
/// This function processes the returned code of the last R2CP command.
/// </summary>
/// 
/// Every command of the R2CP protocol returns a special code 
/// at the command completion. 
/// The code can be used by the module to determine if the command 
/// has been properly executed.
/// 
/// The missing of the returned code usually should cause 
/// a general communication error with the module. 
/// 
/// <param name="cd"></param>
/// <returns></returns>
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

/// <summary>
/// This routine is used to initialize the communication with the generator
/// </summary>
/// 
/// The procedure initialize the Generator communication setting the proper protocol version 
/// to V6.
/// 
/// <param name=""></param>
/// <returns></returns>
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


bool Generator::updateGeneratorStatus(void) {

    // Gets the current generator status
    R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
    if (!handleCommandProcessedState(nullptr)) return false;

    if (current_generator_status == R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus) return true;
    current_generator_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    switch (current_generator_status) {
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
    default: LogClass::logInFile("GENERATOR STATUS: (unknown name) > " + current_generator_status.ToString());
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
        unsigned char curstat = current_generator_status;
        updateGeneratorStatus();

        if (curstat != current_generator_status) {
            
            // Status changed
            if (current_generator_status == R2CP::Stat_Standby) {
                Notify::deactivate(Notify::messages::WARNING_GENERATOR_NOT_READY);
                ready_for_exposure = true;
            }
            else {
                Notify::activate(Notify::messages::WARNING_GENERATOR_NOT_READY);
                ready_for_exposure = false;                
            }
        }

        // If not in Standby cannot be processed a request for an exposure sequence
        if (current_generator_status != R2CP::Stat_Standby) continue;
        exposureManagementLoop(false);
        
        // ActivateDisables the safety disable RX message
        if (!disable_rx_message) setDisableRxMessage(true);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }


    // Unexpected status
    return false;
}



bool Generator::setDisableRxMessage(bool stat) {
    R2CP::CaDataDicGen::GetInstance()->SystemMessages_SetDisableRx(stat);
    if (!handleCommandProcessedState(nullptr)) {
        disable_rx_message = false; // set to false even if in an unknown state
        return false;
    }
    
    disable_rx_message = stat;
    return true;    
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
Generator::generator_errors Generator::generatorExecutePulseSequence(System::String^ ExpName, int ms_timeout) {
    int time_interval = 50;
    int timeout_tic = ms_timeout / time_interval;

    // Disables the safety disable RX message
    if (!setDisableRxMessage(false)) {
        LogClass::logInFile(ExpName + "SystemMessages_SetDisableRx error");
        return generator_errors::GEN_COMMUNICATION_ERROR;
    }

    setXrayEnable(true);

    // Wait to exit from the generator standby status
    int timeout = timeout_tic;
    while (timeout--) {
        std::this_thread::sleep_for(std::chrono::milliseconds(time_interval));
        
        // Brake as soon a non standby condition is detected
        updateGeneratorStatus();        
        if (current_generator_status != R2CP::Stat_Standby) break;

        // Checks for the XRAY button
        if (!getXrayPushButton()) {
            LogClass::logInFile(ExpName + "push button release error");
            setXrayEnable(false);
            return generator_errors::GEN_BUTTON_RELEASE;
        }

        // Checks for error messages
        if (R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active) {
            LogClass::logInFile(ExpName + "generator system messages during exposure error");
            setXrayEnable(false);
            return generator_errors::GEN_INTERNAL_ERROR;
        }

        
    }
    
    if (!timeout) {
        LogClass::logInFile(ExpName + "timeout waiting generator executing the xray sequence");
        setXrayEnable(false);
        return generator_errors::GEN_INVALID_STATUS;
    }
    
    // The sequence will completes when generator returns in Standby or Stat_WaitFootRelease.
    // The sequence termines as sooon a wrong expected status should be detected
    timeout = timeout_tic;
    unsigned char curstat;
    while (timeout--) {
        
        curstat = current_generator_status;
        updateGeneratorStatus();

        // Checks for the XRAY button
        if (!getXrayPushButton()) {
            LogClass::logInFile(ExpName + "push button release error");
            setXrayEnable(false);
            return generator_errors::GEN_BUTTON_RELEASE;
        }

        // Checks for error messages
        if (R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active) {
            LogClass::logInFile(ExpName + "generator system messages during exposure error");
            setXrayEnable(false);
            return generator_errors::GEN_INTERNAL_ERROR;
        }
        
        if (curstat != current_generator_status) {
        
            // The Stat is not a valid stat
            if (current_generator_status >= R2CP::Stat_Unknown) {
                LogClass::logInFile(ExpName + current_generator_status.ToString() + " Unknown Start");
                setXrayEnable(false);
                return generator_errors::GEN_INVALID_STATUS;
            }

            // Waits for the Exposure complete event
            switch (current_generator_status) {
            
            case R2CP::Stat_Standby:
            case R2CP::Stat_WaitFootRelease:                    
                setXrayEnable(false);
                return generator_errors::GEN_NO_ERRORS;

            case R2CP::Stat_Error:
                LogClass::logInFile(ExpName + "generator R2CP::Stat_Error error ");
                setXrayEnable(false);
                return generator_errors::GEN_INTERNAL_ERROR;

            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:            
                LogClass::logInFile(ExpName + current_generator_status.ToString() + " status error ");
                setXrayEnable(false);
                return generator_errors::GEN_INVALID_STATUS;                

            default:
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(time_interval));
    }

    LogClass::logInFile(ExpName + "generator sequence timeout error");
    setXrayEnable(false);
    return generator_errors::GEN_TIMEOUT;
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
Generator::generator_errors Generator::generatorSet3PointDatabank(unsigned char dbId, bool large_focus, float KV, float MAS, int n_pulse, int min_pulse, int max_pulse) {
    
    unsigned short mas;
    // Rounding to the next integer value of the requested mAs 
    if ((float)((unsigned short) MAS) != MAS) mas = ((unsigned short) MAS + 1);
    else mas = ((unsigned short) MAS);

    // Sets the 2D databank to force the generator to select the mA and mS(R10 tab) for an exposure with the requested mAs
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(dbId, large_focus, KV, mas, max_pulse);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile("set3PointDatabank() - error");
        return generator_errors::GEN_COMMUNICATION_ERROR;
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
        return generator_errors::GEN_INVALID_PARAMS;
    }

    // Recalculate the Anodic current that can fit with the R10 time slot
    float mA = (MAS * 1000) / pulse_mS;

    // Uses a 3 point procedure to better fit with the requested mAs
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_3D_Databank(dbId, large_focus, KV, mA, pulse_mS, max_pulse);
    if (!handleCommandProcessedState(nullptr)) {
        LogClass::logInFile("set3PointDatabank() - Generator_Set_3D_Databank() error");
        return generator_errors::GEN_COMMUNICATION_ERROR;
    }

    // Gets the Anodic current imposed by the generator
    float pulse_mA = ((float) R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[dbId].mA100.value) / 100;
    float err_100_mAs = (abs((pulse_mA * pulse_mS / 1000) - MAS) * 100 ) / MAS;

    // Max allowed error is 1%
    if (err_100_mAs > 1.0) {
        LogClass::logInFile("set3PointDatabank(): ERROR! pulse_mA:" + pulse_mA + "; pulse_mS:" + pulse_mS + "; mAs Error (\%):" + err_100_mAs);
        return  generator_errors::GEN_INVALID_PARAMS;
    }
    else {
        LogClass::logInFile("set3PointDatabank() -> pulse_mA:" + pulse_mA + "; pulse_mS:" + pulse_mS + "; mAs Error (\%):" + err_100_mAs);
    }
    
    return  generator_errors::GEN_NO_ERRORS;
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
    static bool sqlite_enabled = true;

    // If any issue in accessing the database should disable the access, no more request will be processed
    if (!sqlite_enabled) return "";

    SQLiteConnection conn("Data Source=" + SYTEM_MESSAGE_DATABASE + "; ");
    //    SQLiteConnection conn("Data Source=C:\\OEM\\AppData\\system_messages.sqlite;");
    
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
        sqlite_enabled = false;
        LogClass::logInFile("Generator: Invalid access to the System Messages Database");
        return "";
    }
    
}

Generator::generator_errors Generator::generator2DPulsePreparation(System::String^ exp_name, float kV, float mAs, bool islargefocus, bool det_sync, bool grid_sync, int exp_time) {
   
    selected_anode_current = 150;

    // Load Data Bank For pulse    
    if (!simulator_mode) {
        R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pulse, islargefocus, kV, mAs, exp_time);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(exp_name + "Generator_Set_2D_Databank error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }

        selected_anode_current = ((float)R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pulse].mA100.value) / 100;
    }
   

    System::String^ exposure_data_str;   
    exposure_data_str = "kV:" + kV; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "mAs:" + mAs; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Anodic-mA:" + selected_anode_current; LogClass::logInFile(exposure_data_str);
    
    if (islargefocus) { exposure_data_str = "Focus: LARGE";  LogClass::logInFile(exposure_data_str); }
    else { exposure_data_str = "Focus: SMALL";  LogClass::logInFile(exposure_data_str); }

    if (!simulator_mode) {
        if (det_sync) grid_sync = true; // Procedure activation 
        R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DProcedurePulse(det_sync, grid_sync);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(exp_name + "Patient_Activate2DProcedurePulse error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }

        // Clear the active system messages
        if (!clearSystemMessages()) {
            LogClass::logInFile(exp_name + "SystemMessages_Clear_Message error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }

        // Status not in StandBy
        updateGeneratorStatus();
        if (current_generator_status != R2CP::Stat_Standby) {
            LogClass::logInFile(exp_name + "generator not in standby error");
            return generator_errors::GEN_INVALID_STATUS;
        }

    }

    // Preparation completed
    return generator_errors::GEN_NO_ERRORS;
}

Generator::generator_errors Generator::generator2DAecPrePulsePreparation(System::String^ exp_name, float kV, float mAs, bool islargefocus, int exp_time){
    selected_anode_current = 150;

    // Load Data Bank For pulse    
    if (!simulator_mode) {
        R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pre, islargefocus, kV, mAs, exp_time);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(exp_name + "Generator_Set_2D_Databank error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }

        selected_anode_current = ((float)R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pre].mA100.value) / 100;
    }


    System::String^ exposure_data_str;
    exposure_data_str = "kV:" + kV; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "mAs:" + mAs; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Anodic-mA:" + selected_anode_current; LogClass::logInFile(exposure_data_str);

    if (islargefocus) { exposure_data_str = "Focus: LARGE";  LogClass::logInFile(exposure_data_str); }
    else { exposure_data_str = "Focus: SMALL";  LogClass::logInFile(exposure_data_str); }

    if (!simulator_mode) {

        // Procedure activation
        LogClass::logInFile(exp_name + "procedure activation");
        R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DAecProcedurePre();
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(exp_name + "Patient_Activate2DAecProcedurePre error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }

        // Clear the active system messages
        if (!clearSystemMessages()) {
            LogClass::logInFile(exp_name + "SystemMessages_Clear_Message error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }

        // Status not in StandBy
        updateGeneratorStatus();
        if (current_generator_status != R2CP::Stat_Standby) {
            LogClass::logInFile(exp_name + "generator not in standby error");
            return generator_errors::GEN_INVALID_STATUS;
        }

    }

    // Preparation completed
    return generator_errors::GEN_NO_ERRORS;
}

Generator::generator_errors Generator::generator2DAecPulsePreparation(System::String^ exp_name, float kV, float mAs, bool islargefocus, int exp_time) {

    selected_anode_current = 150;

    // Load Data Bank For pulse    
    if (!simulator_mode) {
        R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pulse, islargefocus, kV, mAs, exp_time);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(exp_name + "Generator_Set_2D_Databank error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }

        selected_anode_current = ((float)R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pulse].mA100.value) / 100;
    }


    System::String^ exposure_data_str ;
    exposure_data_str = "kV:" + kV; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "mAs:" + mAs; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Anodic-mA:" + selected_anode_current; LogClass::logInFile(exposure_data_str);

    if (islargefocus) { exposure_data_str = "Focus: LARGE";  LogClass::logInFile(exposure_data_str); }
    else { exposure_data_str = "Focus: SMALL";  LogClass::logInFile(exposure_data_str); }

    if (!simulator_mode) {
        
        R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DAecProcedurePulse();
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(exp_name + "Patient_Activate2DAecProcedurePulse error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }

    }

    // Preparation completed
    return generator_errors::GEN_NO_ERRORS;
}

Generator::generator_errors Generator::generator3DPulsePreparation(System::String^ exp_name, float kV, float mAs, int tomo_samples, int tomo_skip, bool islargefocus, int min_exp_time, int max_exp_time) {
    selected_anode_current = 200;
    generator_errors error;

    if (!simulator_mode) {

        //-------- Setup the 3D pulse using the 3 point approach procedure ---------------------------------------------           
        error = generatorSet3PointDatabank(R2CP::DB_Pulse, true, kV, mAs / ((float) tomo_samples), tomo_samples, min_exp_time, max_exp_time);
        if (error != generator_errors::GEN_NO_ERRORS) {
            LogClass::logInFile(exp_name + "generatorSet3PointDatabank() error");
            return error;
        }

        // Gets the Anodic current selected
        selected_anode_current = ((float)R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pulse].mA100.value) / 100;
    }
   
    System::String^ exposure_data_str;
    exposure_data_str = "kV:" + kV; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "mAs:" + mAs; LogClass::logInFile(exposure_data_str);
    exposure_data_str = "Anodic-mA:" + selected_anode_current; LogClass::logInFile(exposure_data_str);

    if (islargefocus) { exposure_data_str = "Focus: LARGE";  LogClass::logInFile(exposure_data_str); }
    else { exposure_data_str = "Focus: SMALL";  LogClass::logInFile(exposure_data_str); }

    if (!simulator_mode) {
        // Setup the 3D Databank for Tomo skip pulses
        R2CP::CaDataDicGen::GetInstance()->Generator_Set_SkipPulse_Databank(R2CP::DB_SkipPulse, tomo_skip);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(exp_name + "Generator_Set_SkipPulse_Databank error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }

        // Setup the 3D Mammography procedure    
        R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Standard_Mammography_3D, tomo_samples);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(exp_name + "Patient_SetupProcedureV6 error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }

        // Assignes the Skip-Pulse Databank to the Standard 3D Mammography procedure    
        R2CP::CaDataDicGen::GetInstance()->Generator_Assign_SkipPulse_Databank(R2CP::ProcId_Standard_Mammography_3D, R2CP::DB_SkipPulse);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(exp_name + "Generator_Assign_SkipPulse_Databank error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }

        // Assignes the Pulse Databank to the Index 2 of the Standard 3D Mammography with AEC procedure    
        R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Standard_Mammography_3D, 1);
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(exp_name + "Generator_AssignDbToProc error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }

        // Procedure activation
        LogClass::logInFile(exp_name + " procedure activation");
        R2CP::CaDataDicGen::GetInstance()->Patient_Activate3DProcedurePulse();
        if (!handleCommandProcessedState(nullptr)) {
            LogClass::logInFile(exp_name + "Patient_Activate3DProcedurePulse error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }

        // Clear the active system messages
        if (!clearSystemMessages()) {
            LogClass::logInFile(exp_name + "SystemMessages_Clear_Message error");
            return generator_errors::GEN_COMMUNICATION_ERROR;
        }



    }

    // Preparation completed
    return generator_errors::GEN_NO_ERRORS;
}