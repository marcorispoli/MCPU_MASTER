#include "pch.h"
#include "ConfigFile.h"

using namespace System::IO;

ConfigFile::ConfigFile(const String^ file, int rev, array<paramItemT^>^ descriptor)
{
    warning = false;
    warning_string = "";

    filename = getConfigPath() + "/" + (String^) file;
    revision = rev;
    loaded_revision = revision;

    data = descriptor;

    // If the file is not present it is created
    // If the load file fails then create a default value
    if (!File::Exists(filename)) {
        warning = true;
        warning_string += "configuration file not exist: default is created.\n";
        createDefaultFile();
    }
    else {
        if (!loadFile()) {
            warning = true;
            warning_string += "default configuration file is created.\n";
            createDefaultFile();
        }

    }

}

String^ ConfigFile::getConfigPath(void) {

    // Path identification
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    String^ stringa = gcnew String(buffer);
    int pos = stringa->LastIndexOf(L"\\");
    return stringa->Substring(0, pos);
}

bool ConfigFile::decodeLine(String^ rline)
{
    
    String^ sval;

    // Exclude a non formatted line
    if (!rline->Contains("<")) return false;
    if (!rline->Contains(">")) return false;

    // Replace spaces and formatted symbols
    rline = rline->Replace(" ", "");
    rline = rline->Replace("<", "");
    rline = rline->Replace(">", "");

    // Gets the Tag 
    int p = rline->IndexOf(",");
    if (p < 0) return false;
    String^ tag = rline->Substring(0, p);

    if (tag == "REVISION") {
        rline = rline->Substring(p + 1);
        loaded_revision = Convert::ToInt16(rline);
        return true;
    }

    int index = getTagPosition(tag);
    if (index < 0) return false;
    
    for (int i = 0; i < data[index]->values->Length; i++) {
        rline = rline->Substring(p + 1);
        p = rline->IndexOf(",");
        if (p < 0) {
            if (i < data[index]->values->Length - 1) return false;
            data[index]->values[i] = rline;
            return true;
        }
        sval = rline->Substring(0, p);
        data[index]->values[i] = sval;
    }
    
    return false;
}

int ConfigFile::getTagPosition(String^ tag) {
    String^ stringa;
    for (int i = 0; i < data->Length; i++) {
        stringa = (String^)data[i]->tag;
        if (stringa == tag)  return i;
    }

    return -1;
}


bool ConfigFile::loadFile(void){
    

    StreamReader^ din = File::OpenText(filename);

    String^ str;
    int count = 0;
    while ((str = din->ReadLine()) != nullptr)
    {
        count++;
        decodeLine(str);
        if (loaded_revision != revision) {
            warning = true;
            warning_string += "configuration file revision changed.\n";
            din->Close();
            return revisionChangeCallback();
        }
        
    }

    din->Close();
    return true;
}

void ConfigFile::storeFile(void){
    String^ comment;
    String^ item;

    StreamWriter^ sw = gcnew StreamWriter(filename);

    comment = "File Revision Code";
    item = "<REVISION," + revision.ToString() + ">";
    sw->WriteLine(comment);
    sw->WriteLine(item);
    sw->WriteLine("");

    for (int i = 0; i < data->Length; i++) {
        comment = (String^)data[i]->comment;
        item = "<" + (String^)data[i]->tag;
        for (int j = 0; j < data[i]->defaults->Length; j++) {
            item += "," + (String^)data[i]->values[j];
        }
        item += ">";
        sw->WriteLine(comment);
        sw->WriteLine(item);
        sw->WriteLine("");
    }

    sw->Close();

}

void ConfigFile::createDefaultFile(void){
    String^ comment;
    String^ item;

    // Reload the default values
    for (int i = 0; i < data->Length; i++) {
        data[i]->values = gcnew array<String^>(data[i]->defaults->Length);
        for (int j = 0; j < data[i]->defaults->Length; j++) data[i]->values[j] = (String^)data[i]->defaults[j];
    }

    StreamWriter^ sw = gcnew StreamWriter(filename);

    comment = "File Revision Code";
    item = "<REVISION," + revision.ToString() + ">";
    sw->WriteLine(comment);
    sw->WriteLine(item);
    sw->WriteLine("");

    for (int i = 0; i < data->Length; i++) {
        comment =  (String^) data[i]->comment;
        item = "<" + (String^)data[i]->tag;
        for (int j = 0; j < data[i]->defaults->Length; j++) {
            item += "," + (String^)data[i]->defaults[j];
        }
        item += ">";
        sw->WriteLine(comment);
        sw->WriteLine(item);
        sw->WriteLine("");
    }

    sw->Close();
    return;   
}
