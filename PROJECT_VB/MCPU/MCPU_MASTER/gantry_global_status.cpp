#include "gantry_global_status.h"
#include "WINDOWS/IdleForm.h"
#include "WINDOWS/OperatingForm.h"
#include "Notify.h"
#include <mutex>

using namespace System;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;

Gantry::Gantry() {
 
        // Sets the resource path
        applicationResourcePath = System::IO::Directory::GetCurrentDirectory() + "\\RESOURCES\\";

        // Monitor Identification
        int monitors = Screen::AllScreens->Length;

        // Gets the Monitors position
        for (int i = 0; i < monitors; i++) {
            
            int H = Screen::AllScreens[i]->Bounds.Height;
            int W = Screen::AllScreens[i]->Bounds.Width;

            if (W == 600) {
                monitor_X0 = Screen::AllScreens[i]->Bounds.Left;
                monitor_Y0 = Screen::AllScreens[i]->Bounds.Top;                
            }
        }
        Debug::WriteLine("SELECTED MONITOR: X0 = " + monitor_X0.ToString() + ", Y0 = " + monitor_Y0.ToString());
        
        // Set the current language for messages and GUI
        Notify::setLanguage("ENG");        

        // Creates the status Windows
        pIdleForm = gcnew IdleForm();
        pOperatingForm = gcnew OperatingForm();
    
}

void Gantry::setIdle() {
    if (current_operating_status == operating_status_options::GANTRY_IDLE) return;
    if (current_operating_status == operating_status_options::GANTRY_OPEN_STUDY) ((OperatingForm^)pOperatingForm)->close();

    current_operating_status = operating_status_options::GANTRY_IDLE;
    ((IdleForm^)pIdleForm)->open();
}

void Gantry::setOperating() {
    if (current_operating_status == operating_status_options::GANTRY_OPEN_STUDY) return;
    if (current_operating_status == operating_status_options::GANTRY_IDLE) ((IdleForm^)pIdleForm)->close();

    current_operating_status = operating_status_options::GANTRY_OPEN_STUDY;
    ((OperatingForm^)pOperatingForm)->open();
}

void Gantry::setStartup(void) {
    current_operating_status = operating_status_options::GANTRY_STARTUP;
}

bool Gantry::setOpenStudy(System::String^ patient) {
    if (current_operating_status != operating_status_options::GANTRY_IDLE) return false;
    patient_name = patient;
    setOperating();
    return true;
}

bool Gantry::setCloseStudy(void) {
    if (current_operating_status != operating_status_options::GANTRY_OPEN_STUDY) return false;
    patient_name = "";
    setIdle();
    return true;
}