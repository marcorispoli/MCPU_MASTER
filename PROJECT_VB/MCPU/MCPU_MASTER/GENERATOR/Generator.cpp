#include "R2CP_Eth.h"
#include "CaDataDicGen.h"
#include "SystemConfig.h"
#include "Generator.h"
#include "gantry_global_status.h"
#include <thread>


static CR2CP_Eth* R2CP_Eth;

static int16_t sendCR2CPData(unsigned char* pMessage, unsigned short  datalength) {
    if (pGENERATOR == nullptr) return 0;

    cli::array<Byte>^ buf = gcnew cli::array<Byte>(datalength);
    for (int i = 0; i < datalength; i++) buf[i] = pMessage[i]; 
    pGENERATOR->send(buf);
    return datalength;
}

Generator::Generator():TcpClientCLI( SH_IP_ADDRESS, SH_PORT) 
{
    
    // CR2CP Initialization
    R2CP_Eth = new CR2CP_Eth(sendCR2CPData, 0, (CaDataDic*) R2CP::CaDataDicGen::GetInstance(), 0, 0);
    R2CP::CaDataDicGen::GetInstance()->Initialitation();
    R2CP::CaDataDicGen::GetInstance()->SetCommunicationForm(R2CP_Eth);

    // Start the reception thread
    running_thread = gcnew Thread(gcnew ThreadStart(this, &Generator::threadWork));
    running_thread->Name = "Loop Generator Workflow ";
    running_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
    running_thread->Start();


}

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
    if (!isConnected()) return false;
    return R2CP_Eth->smartHubConnected;
}
bool Generator::isGeneratorConnected(void) {
    if (!isConnected()) return false;
    return R2CP_Eth->generatorConnected;
}

bool Generator::connectionTest(void) {
    if (!isConnected()) return false;
    if (!R2CP_Eth->smartHubConnected)  return false;
    if (!R2CP_Eth->generatorConnected)  return false;
    return true;

}
void Generator::threadWork(void) {
   // while(true) std::this_thread::sleep_for(std::chrono::milliseconds(10));

    while (true) {
        R2CP_Eth->smartHubConnected = false;
        R2CP_Eth->generatorConnected = false;
        generator_setup_completed = false;
        generator_idle = false;

        Notify::activate("GENERATOR_ERROR_SH", false);
        Notify::activate("GENERATOR_ERROR_GEN", false);
        Notify::deactivate("GENERATOR_INIT_WARNING");

        // Waits for the server connection
        while (!isConnected()) std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        

        // Try to connect the Smart Hub
        Debug::WriteLine("Try to connect the Smart Hub!\n");
        while (!R2CP_Eth->smartHubConnected) {
            R2CP::CaDataDicGen::GetInstance()->Network_ConnectionRequest_Event();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        Notify::deactivate("GENERATOR_ERROR_SH");

        Debug::WriteLine("Smart Hub Connected!\n");
        Debug::WriteLine("Wait Generator connection..\n");

        
        // Waits for the generator
        while (!R2CP_Eth->generatorConnected) {
            if (!isConnected()) break;
            if (!R2CP_Eth->smartHubConnected) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Repeats the connection sequence
        if (!isConnected()) continue;
        if (!R2CP_Eth->smartHubConnected) continue;

        Notify::deactivate("GENERATOR_ERROR_GEN");
        Debug::WriteLine("Generator Connected!\n");

        Notify::activate("GENERATOR_INIT_WARNING", false);
        
        // Inits of the generator
        if(!generatorInitialization()) continue;
        Debug::WriteLine("Generator Initialized!\n");

        // Generator setup
        if (!generatorSetup()) continue;
        Debug::WriteLine("Generator Setup Completed!\n");
        generator_setup_completed = true;

        // Clear the system messages
        if (!clearSystemMessages()) continue;
        Debug::WriteLine("System Message erased!\n");

        // Disables the Rx Message
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_SetDisableRx(true);
        handleCommandProcessedState(nullptr);
        Debug::WriteLine("Generator Rx Disabled!\n");

        Debug::WriteLine("Generator In Idle\n");
        generator_idle = true;
        Notify::deactivate("GENERATOR_INIT_WARNING");

        // Handles the Idle mode
        if (!generatorIdle()) continue;

    }
       
}

bool  Generator::handleCommandProcessedState(unsigned char* cd) {
    static int cp_timeout = 100;

    while (!R2CP_Eth->isCommandProcessed()) {
        cp_timeout--;

        if (!cp_timeout) {
            cp_timeout = 100;
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    cp_timeout = 100;

    unsigned char code = R2CP_Eth->getCommandProcessedResult();
    if (cd) *cd = code;

    if (code) {       
        Debug::WriteLine("CommandProcessedError:" + gcnew String(R2CP_Eth->getCommandProcessedString().c_str()));
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
    Debug::WriteLine("Wait the generator initialization process\n");

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
    if (!connectionTest()) return false;
    return true;
}

bool Generator::clearSystemMessages(void) {
    if (!connectionTest()) return false;
    return true;
}

bool Generator::generatorIdle(void) {

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (!connectionTest()) return false;
    }

    return true;
}

