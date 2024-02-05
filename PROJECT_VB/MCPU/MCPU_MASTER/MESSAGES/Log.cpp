#include "Log.h"
#include <Windows.h>
#include "SystemConfig.h"
#include <thread>

using namespace System::IO;


/// <summary>
/// This is the class constructor
/// </summary>
/// 
/// The constructor:
/// + checks the existance of the Lod Directory stored into the SystemConfig file;
/// + if the directory should not exists, it is created (or the log is disabled in case the creation fails);
/// + If a previous session shouldn't have close the log file, it is retrived and stored with the last modification date and time;
/// + A new Gantry.log file is created.
/// 
/// 
LogClass::LogClass() {

    // Enabled /Not enabled
    // If the SystemConfiguration should disable the Logs, no more logs will be stored.
    enabled = false;
    if (SystemConfig::Configuration->getParam(SystemConfig::PARAM_LOG_DIRECTORY)[SystemConfig::PARAM_ENA] == "OFF") return;


    // Program Path 
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    System::String^ stringa = gcnew System::String(buffer);
    int pos = stringa->LastIndexOf(L"\\");
    path = stringa->Substring(0, pos) + SystemConfig::Configuration->getParam(SystemConfig::PARAM_LOG_DIRECTORY)[SystemConfig::PARAM_DIR];

    
    // Verifies if the directory exists: in case it should not exist it is created
    if (!Directory::Exists(path)) {

        // Creates the directory before to continue
        try{
            Directory::CreateDirectory(path);
        }
        catch (...) {
            stream = nullptr;
            enabled = false;
            return;
        }
        
    }

    // Name of the run-time log file
    gantry_log = path  + "\\" + "Gantry.log";

    // Recovery of a previous non closed file
    if (File::Exists(gantry_log)) {
        System::DateTime date_recovery = File::GetLastWriteTime(gantry_log);
        System::String^ file_recovery_string = path + "\\" + + date_recovery.Day + "_" + date_recovery.Month + "_" + date_recovery.Year + "_" + date_recovery.Hour + "_" + date_recovery.Minute + "_" + date_recovery.Second + ".log";
        if(File::Exists(file_recovery_string)) File::Delete(file_recovery_string);
        File::Copy(gantry_log, file_recovery_string);
        File::Delete(gantry_log);

    }

    // Opens the new gantry Log file
    stream = gcnew StreamWriter(gantry_log);
    ((StreamWriter^)stream)->AutoFlush = true;
    enabled = true;
}

/// <summary>
/// This function closes the Gantry.log file and moves it into the current date and time.
/// 
/// </summary>
/// <param name=""></param>
void LogClass::logClose(void) {
    if (!enabled) return;
    ((StreamWriter^)stream)->Close();

    System::DateTime date;
    date = System::DateTime::Now;
    System::String^ date_string = date.Day + "_" + date.Month + "_" + date.Year + "_" + date.Hour + "_" + date.Minute + "_" + date.Second;
    
    // Copy the current log into the destination log
    File::Copy(gantry_log, path + "\\" + date_string + ".log");

    // Delete the Gantry.log
    File::Delete(gantry_log);
    return;
}

/// <summary>
/// Print the string into the log file.
/// </summary>
/// 
/// The message is prefixed with the delay in ms from the last log event, and the current Date and Time.
/// 
/// <param name="logstr"></param>
void LogClass::logInFile(System::String^ logstr) {
    static double oldt = clock();
    if (!enabled) return;
   
    // Calcs the delay time in ms from the last Log event
    double now = clock();
    double delay_ms = 1000 * (now - oldt) / (double)CLOCKS_PER_SEC;
    oldt = now;

    // Creates the lg string with the delay time and the date and time as prefix of the string message.
    System::DateTime date = System::DateTime::Now;    
    System::String^ date_string = date.Hour + "h:" + date.Minute + "m:" + date.Second + "s:" + date.Millisecond ;
    ((StreamWriter^)stream)->WriteLine("[" + delay_ms.ToString() + "][" + date_string + "] " + logstr);
    
}