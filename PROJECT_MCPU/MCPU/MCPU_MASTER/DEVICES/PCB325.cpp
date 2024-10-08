#include "PCB325.h"
#include "Notify.h"
#include "Log.h"
#include <thread>
#include "ConfigurationFiles.h"



/// <summary>
/// This is the configuration loop routine executed at the beginning of the 
/// device connection, before to execute the runningLoop() routine.
/// 
/// 
/// </summary>
/// <param name=""></param>
/// <returns>true if the configuration success</returns>
bool PCB325::configurationLoop(void) {
    LogClass::logInFile("PCB325: CONFIGURED");
    return true;
}


void PCB325::runningLoop(void) {
   
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return;
}

