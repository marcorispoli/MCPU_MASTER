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
#include <thread>


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
    generator->send(buf);
    return datalength;
}


Generator::Generator():TcpClientCLI( SH_IP_ADDRESS, SH_PORT) 
{
    
    // CR2CP Initialization
    R2CP_Eth = new CR2CP_Eth((pSendR2CP_callback) sendCR2CPData, 0, (CaDataDic*) R2CP::CaDataDicGen::GetInstance(), 0, 0);
    R2CP::CaDataDicGen::GetInstance()->Initialitation();
    R2CP::CaDataDicGen::GetInstance()->SetCommunicationForm(R2CP_Eth);

    R2CP_Eth->smartHubConnected = false;
    R2CP_Eth->generatorConnected = false;
  

    // Start the reception thread
    running_thread = gcnew Thread(gcnew ThreadStart(this, &Generator::threadWork));
    running_thread->Name = "Loop Generator Workflow ";
    running_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
    running_thread->Start();


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
    if (!generator->isConnected()) return false;
    return R2CP_Eth->smartHubConnected;
}
bool Generator::isGeneratorConnected(void) {
    if (!generator->isConnected()) return false;
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
        Debug::WriteLine("Try to connect the Smart Hub and Generator!\n");
        Notify::activate(Notify::messages::ERROR_GENERATOR_ERROR_CONNECTION, false);
        Notify::activate(Notify::messages::WARNING_GENERATOR_INIT, false);
        Notify::activate(Notify::messages::WARNING_GENERATOR_NOT_READY, false);

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
        Debug::WriteLine("Generator Connected!\n");
        
        // Inits of the generator
        if(!generatorInitialization()) continue;
        Debug::WriteLine("Generator Initialized!\n");

        // Clear the system messages
        if (!clearSystemMessages()) continue;
        Debug::WriteLine("System Message erased!\n");

        // Generator setup
        if (!generatorSetup()) continue;
        Debug::WriteLine("Generator Setup Completed!\n");
        setup_completed = true;

        

        // Disables the Rx Message
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_SetDisableRx(true);
        handleCommandProcessedState(nullptr);
        Debug::WriteLine("Generator Rx Disabled!\n");

        Debug::WriteLine("Generator In Idle\n");
        idle_status = true;
        Notify::deactivate(Notify::messages::WARNING_GENERATOR_INIT);

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
            Debug::WriteLine("CommandProcessedError: Timeout");
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
    Debug::WriteLine("GENERATOR: Setup procedure\n");
    if (!connectionTest()) return false;

    // Setup the 2D Test (no grid)  procedure
    Debug::WriteLine("GENERATOR: Setup 2D test no grid procedure \n");
    R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Standard_Test, 0);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the 2D Test (with grid)  procedure
    Debug::WriteLine("GENERATOR: Setup 2D test with grid procedure \n");
    R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Standard_Test_with_grid, 0);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the 2D Mammography  procedure
    Debug::WriteLine("GENERATOR: Setup 2D Mammography no AEC procedure \n");
    R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Standard_Mammography_2D, 0);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the 2D Mammography with AEC  procedure
    Debug::WriteLine("GENERATOR: Setup 2D Mammography with AEC procedure \n");
    R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Aec_Mammography_2D, 0);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the 3D Mammography procedure
    Debug::WriteLine("GENERATOR: Setup 3D Mammography  procedure \n");
    R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Standard_Mammography_3D, 25);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the 3D Mammography procedure
    Debug::WriteLine("GENERATOR: Setup 3D Mammography  with AEC procedure \n");
    R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(R2CP::ProcId_Aec_Mammography_3D, 25);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the Databank for pre pulse
    Debug::WriteLine("GENERATOR: Setup Databank for Pre pulse \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pre, 1, 20, 10, 5000);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the Databank for pulse
    Debug::WriteLine("GENERATOR: Setup Databank for Pulse \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(R2CP::DB_Pulse, 1, 20, 10, 5000);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Setup the 3D Databank for Tomo skip pulses
    Debug::WriteLine("GENERATOR: Setup Databank for Tomo Skip pulses \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_Set_SkipPulse_Databank(R2CP::DB_SkipPulse, 0);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pulse Databank to the Index 1 of the Standard test procedure
    Debug::WriteLine("GENERATOR: Assignes Pulse Databank to Index 1 of the  ProcId_Standard_Test \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Standard_Test, 1);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pulse Databank to the Index 1 of the Standard test with grid procedure
    Debug::WriteLine("GENERATOR: Assignes Pulse Databank to Index 1 of the  ProcId_Standard_Test_with_grid \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Standard_Test_with_grid, 1);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pulse Databank to the Index 1 of the Standard Mammography procedure
    Debug::WriteLine("GENERATOR: Assignes Pulse Databank to Index 1 of the  ProcId_Standard_Mammography_2D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Standard_Mammography_2D, 1);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pre-pulse Databank to the Index 1 of the Standard Mammography with AEC procedure
    Debug::WriteLine("GENERATOR: Assignes Pre-Pulse Databank to Index 1 of the  ProcId_Aec_Mammography_2D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pre, R2CP::ProcId_Aec_Mammography_2D, 1);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pulse Databank to the Index 2 of the Standard Mammography with AEC procedure
    Debug::WriteLine("GENERATOR: Assignes Pulse Databank to Index 2 of the  ProcId_Aec_Mammography_2D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Aec_Mammography_2D, 2);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pulse Databank to the Index 1 of the Standard 3D Mammography procedure
    Debug::WriteLine("GENERATOR: Assignes Pulse Databank to Index 1 of the  ProcId_Standard_Mammography_3D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Standard_Mammography_3D, 1);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Skip-Pulse Databank to the Standard 3D Mammography procedure
    Debug::WriteLine("GENERATOR: Assignes Skip Databank to the  ProcId_Standard_Mammography_3D \n");
   // R2CP::CaDataDicGen::GetInstance()->Generator_Assign_SkipPulse_Databank(R2CP::DB_SkipPulse, R2CP::ProcId_Standard_Mammography_3D);
   // if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pre-pulse Databank to the Index 1 of the Standard 3D Mammography with AEC procedure
    Debug::WriteLine("GENERATOR: Assignes Pre-Pulse Databank to Index 1 of the  ProcId_Aec_Mammography_3D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pre, R2CP::ProcId_Aec_Mammography_3D, 1);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Pulse Databank to the Index 2 of the Standard 3D Mammography with AEC procedure
    Debug::WriteLine("GENERATOR: Assignes Pulse Databank to Index 2 of the  ProcId_Aec_Mammography_3D \n");
    R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(R2CP::DB_Pulse, R2CP::ProcId_Aec_Mammography_3D, 2);
    if (!handleCommandProcessedState(nullptr)) return false;

    // Assignes the Skip-Pulse Databank to the  Standard 3D Mammography with AEC procedure
    Debug::WriteLine("GENERATOR: Assignes Skip Databank to the  ProcId_Aec_Mammography_3D \n");
   // R2CP::CaDataDicGen::GetInstance()->Generator_Assign_SkipPulse_Databank(R2CP::DB_SkipPulse, R2CP::ProcId_Aec_Mammography_3D);
   // if (!handleCommandProcessedState(nullptr)) return false;

    Debug::WriteLine("GENERATOR: Setup successfully terminated \n");
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
        Debug::WriteLine("GENERATOR: removing message: " + id.ToString() + "\n");
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_Clear_Message(id);
        if (!handleCommandProcessedState(nullptr)) return false;
    }

    // Read again the messages
    R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();
    if (!handleCommandProcessedState(nullptr)) return false;

    // Test if the messages have been removed
    if (R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size()) {
        Debug::WriteLine("GENERATOR: system messages still present! \n");        
    }else Debug::WriteLine("GENERATOR: No more system messages are present! \n");

    return true;
}

bool Generator::generatorIdle(void) {
    static unsigned char generator_status = 0;


    while (true) {
        
        // Ethernet disconnection detected
        if (!connectionTest()) return false;

        // Request the Generator Node status
        R2CP::CaDataDicGen::GetInstance()->Network_GetGeneratorNodeStatus();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (!R2CP_Eth->generatorConnected) return false;

        // Gets the current generator status
        R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();
        if (!handleCommandProcessedState(nullptr)) return false;

        if (generator_status != R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus) {
            generator_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
            Debug::WriteLine("GENERATOR: Current generator status: " + generator_status.ToString() + "\n");

            if (generator_status == 2) {
                Notify::deactivate(Notify::messages::WARNING_GENERATOR_NOT_READY);
                ready_for_exposure = true;
            }
            else {
                Notify::activate(Notify::messages::WARNING_GENERATOR_NOT_READY, false);
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

        // Evaluates if there is a request for the X-Ray exposure
        bool procedure_ret_code = false;
        bool dose_is_released =false;
        xray_exposure_error = exposure_completed_errors::XRAY_NO_ERRORS;

        if (xray_processing) {
            
            // Resets of the exposed pulses for all data-bank
            // The Exposure seqeunce fills the Data Bank used if the generated pulses real values.
            // NOTE: a single Pulse can be a sum of several partial pulses due to the Grid interruption.
            // See the function Generator_ExposureManagement_RadPostExposure in CaDataDicGen_GenIndex.cpp file
            R2CP::CaDataDicGen::GetInstance()->resetExecutedPulse();

            // Every Xray procedure return True if the seqeunce is completed if success.
            // In case of false returned (error condition), the   xray_exposure_error is set properly
            switch (xray_exposure_type) {
            case ExposureModule::exposure_type_options::MAN_2D: procedure_ret_code = man_2d_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::AEC_2D: procedure_ret_code = aec_2d_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::MAN_3D: procedure_ret_code = man_3d_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::AEC_3D: procedure_ret_code = aec_3d_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::MAN_COMBO: procedure_ret_code = man_combo_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::AEC_COMBO: procedure_ret_code = aec_combo_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::MAN_AE: procedure_ret_code = man_ae_exposure_procedure(); break;
            case ExposureModule::exposure_type_options::AEC_AE: procedure_ret_code = aec_ae_exposure_procedure(); break;
            default:         
                procedure_ret_code = false;
                xray_exposure_error = exposure_completed_errors::XRAY_INVALID_PROCEDURE;
            }

            // Fills the ExposureModule::exposed[] with the effettive pulse data retrived from the proper Databanks
            // The function returns true if a dose has been released.
            bool dose_is_released = setExecutedPulse(xray_exposure_type);

            // Debug Prints
            if(procedure_ret_code)  Debug::WriteLine("GENERATOR EXPOSURE: Successfully completed \n");
            else  if(!dose_is_released)    Debug::WriteLine("GENERATOR EXPOSURE ERROR (NO DOSE): Error code = " + xray_exposure_error.ToString() + "\n");
            else  Debug::WriteLine("GENERATOR EXPOSURE ERROR (PARTIAL DOSE): Error code = " + xray_exposure_error.ToString() + "\n");

            // Removes the X-RAY ena signal 
            PCB301::setXrayEnable(false);

            // De-synch the grid device
            PCB304::synchGridWithGenerator(false);

            // Unlock the compressor if requested
            if (ExposureModule::getCompressorMode() == ExposureModule::compression_mode_option::CMP_RELEASE) PCB302::setCompressorUnlock();
            
            // The X-Ray procedure termines here: the complete event is generated
            // In case of sequence not completed (partial or total) is App to the application 
            // retrive the error code (xray_exposure_error).
            if(procedure_ret_code)  xray_complete_event(exposure_completed_options::XRAY_SUCCESS);
            else if(dose_is_released) xray_complete_event(exposure_completed_options::XRAY_PARTIAL_DOSE);
            else xray_complete_event(exposure_completed_options::XRAY_NO_DOSE);

            // Waits for the X-RAY button release
            if (PCB301::isXrayButton()) {
                Debug::WriteLine("GENERATOR EXPOSURE WAITING BUTTON RELEASE\n");
                while (PCB301::isXrayButton()) std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            
            xray_processing = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return true;
}

/// <summary>
/// This function assignes the kV and mAs of the Generator executed pulses 
/// to the ExposureModule::exposed[] pulses.
/// 
/// The Generator sequence cumulates the exposure data of the partial
/// pulses into the array of the data bank used.
/// 
/// Based on the exposure type, this function takes the proper Data Bank pulse data 
/// and reverses its content into the ExposureModule pulse array.
/// 
/// </summary>
/// <param name="xray_exposure_type"></param>
/// <returns></returns>
bool Generator::setExecutedPulse(ExposureModule::exposure_type_options xray_exposure_type) {
    return false;
    
    ExposureModule::exposure_pulse^ exposure_pulse;
    double kV;
    double mAs;
    PCB315::filterMaterialCodes filter;
    bool is_released_dose = false;

    // reset of the pulses
    ExposureModule::setExposedPulse(0, gcnew ExposureModule::exposure_pulse()); 
    ExposureModule::setExposedPulse(1, gcnew ExposureModule::exposure_pulse()); 
    ExposureModule::setExposedPulse(2, gcnew ExposureModule::exposure_pulse()); 
    ExposureModule::setExposedPulse(3, gcnew ExposureModule::exposure_pulse());
    
    switch (xray_exposure_type) {

    case ExposureModule::exposure_type_options::MAN_2D:

        // set the Pulse 0 with the R2CP::DB_Pulse databank

        exposure_pulse = ExposureModule::getExposurePulse(0);
        filter = exposure_pulse->getFilter();
        kV = (double) R2CP::CaDataDicGen::GetInstance()->executed_pulses[R2CP::DB_Pulse].kV;
        mAs = (double)R2CP::CaDataDicGen::GetInstance()->executed_pulses[R2CP::DB_Pulse].mAs;
        if (mAs > 1) is_released_dose = true;
        
        ExposureModule::setExposedPulse(0, gcnew ExposureModule::exposure_pulse(kV, mAs, filter)); // This is the only used

        break;
    case ExposureModule::exposure_type_options::AEC_2D:

        break;
    case ExposureModule::exposure_type_options::MAN_3D:  break;
    case ExposureModule::exposure_type_options::AEC_3D:  break;
    case ExposureModule::exposure_type_options::MAN_COMBO:  break;
    case ExposureModule::exposure_type_options::AEC_COMBO:  break;
    case ExposureModule::exposure_type_options::MAN_AE:  break;
    case ExposureModule::exposure_type_options::AEC_AE:  break;
    default:
        break;
    }

    return is_released_dose;
    
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
    Notify::activate(Notify::messages::WARNING_GENERATOR_MESSAGE, current_error_list_id, false);
    Debug::WriteLine("GENERATOR: system messages present:" + error_list_id + "\n");

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));


        // Get the error list
        R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();
        if (!handleCommandProcessedState(nullptr)) return false;

        if (R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size() == 0) {
            Notify::deactivate(Notify::messages::WARNING_GENERATOR_MESSAGE);
            Debug::WriteLine("GENERATOR: system messages removed \n");
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
            Notify::activate(Notify::messages::WARNING_GENERATOR_MESSAGE, current_error_list_id, false);
            Debug::WriteLine("GENERATOR: system messages present:" + error_list_id + "\n");
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

bool Generator::startExposure(void) {
    
    if (!ready_for_exposure) return false; // Only in Stand-by mode can be initiated
    if (!xray_processing) return false; // A X-RAY procedure is processing (busy condition)

    // Test if the pulse 0 is actually valid
    if (!ExposureModule::getExposurePulse(0)->validated) return false;

    // test if the Exposure mode is correctly set
    if (ExposureModule::getExposureMode() == ExposureModule::exposure_type_options::EXP_NOT_DEFINED) return false;

    // Sets the exposure type according with the requested by the ExposureModule
    xray_exposure_type = ExposureModule::getExposureMode();
    xray_exposure_error = exposure_completed_errors::XRAY_NO_ERRORS;
    xray_processing = true;
    

    return true;
}