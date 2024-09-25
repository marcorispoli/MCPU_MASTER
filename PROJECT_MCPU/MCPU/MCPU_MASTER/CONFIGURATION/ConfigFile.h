#pragma once

using namespace System::Collections::Generic;




/**
 * @brief This class handles the configuration file management
 * \ingroup ConfigurationFilesModule
 * \internal
 * 
 * # USAGE
 *
 * The application shall subclass this class in order to create a
 * specific configuration file.
 *
 * In the constructor of the subclass the application shall pass a static structure
 * defining the configFile::fileDescriptorT of the configuration file.
 * The fileDescriptor content defines:
 * - The list of available parameter TAGS;
 * - The default content of every parameter;
 * - The description string assigned to every parameter;
 *
 * This is a tipical valid declaration:
 \verbatim
    class yourConfigClass: public configFile
    {
        public:


        #define REVISION     1  // This is the revision code
        #define CONFIG_FILENAME     "C:\\Users\\.....\\your_config_file" // This is the configuration file name and path

        // This section defines labels helping the param identification along the application
        #define NAME_TAG_1   "NAME_TAG_1"
        #define NAME_TAG_2   "NAME_TAG_2"
        ....
        #define NAME_TAG_K   "NAME_TAG_K"

        // your class constructor
        yourConfigClass():configFile( (const configFile::fileDescriptorT)
            {
                CONFIG_FILENAME, REVISION,
                {{
                    { NAME_TAG_1,     {{ VAL_1_1, ... , VAL_1_N }},  "Description string tag 1"},
                    { NAME_TAG_2,     {{ VAL_2_1, ... , VAL_2_O }},  "Description string tag 2"},
                    { NAME_TAG_K,     {{ VAL_K_1, ... , VAL_K_P }},  "Description string tag K"},
                }}
            })
        {
            // Your constructor code ...
            this->loadFile();
        };


    }; // End class definition
\endverbatim
 *  Override the configFile::revisionChangeCallback() function to customize the\n
    default change revision callback
    \verbatim
     bool yourConfigClass::changeRevisionCallback(int file_rev, int config_rev){

     // your code here .....

     return true; // if you want to store the file at the end
     return false; // to prevent the file storing
    }
 \endverbatim
 *
 * As the previous code example shows, the class declaration
 * needs a "descriptor" structure, configFile::fileDescriptorT, that uniquelly defines the
 * content of the configuration file.
 *
 * When the configuration file will be instantiated for the first time,
 * a configuration file called "your_config_file" will be created with
 * the default values taken from the configFile::fileDescriptor.
 *
 * Further instantiation of the class will not modifies the content of the file.
 *
 * After instantiation, the memory content will be filled with the default values.\n
 * In order to update the memory content with the current file content, \n
 * the application shall explicitely call the configFile::loadFile() method.
 *
 * In case the file content should have some wrong formatted parameter line,
 * or some of the tag should not be present,\n
 * the wrong formatted lines will be removed from the file and the missing parameters\n
 * will be replaced with the default values.
 * The file then will be rigenerated with the corrected format.
 *
 * To access the configuration data content, the application shall use the following
 * methods:
 * \verbatim
 *
   // class instantiation in some part of the code
   yourConfigClass config;

   // Optionally the application shall load the content in memory
   // (if this as not been done in the yourConfigClass constructor yet)
   config.loadFile();

   // Access to a whole value list of a parameter
   paramItemT param = getParam("NAME_TAG_1");
     // param.tag: is the tag of the parameter;
     // param.values.at(i): is the i-value of the value list in string format

   // Access to the first element of the value's list of a parameter
   int val = getParam<int>("NAME_TAG_1", 0);


   // Setting the n-value value of a given param:
   setParam<int>("NAME_TAG_1",index); // For integers
   setParam<float>("NAME_TAG_1",index); // For floats
   .....
   setParam<QString>("NAME_TAG_1",index); // For strings
 \endverbatim

 * In all of the previous access methods, the application shall\n
 * check the valid data access in order to proceed. See configFile::isAccess()
 *
 * The class takes a copy of the loaded file.
 * After content modifications with the setParam() method,
 * the application can restore the original content
 * using the public method configFile::restoreContent().
 * This methode doesn't change the file in the file system, but
 * restore te content in memory.
 * In order to restore the file the application should call
 * both methods:
 * + configFile::restoreContent();
 * + configFile::storeFile();
 *
 * The application can load the file, store the file, reset to default
 * value the file with the following public methods:
 * - configFile::loadFile();
 * - configFile::storeFile();
 * - configFile::setDefaultFile();
 *
 *
 *
 */
ref class ConfigFile
{
    

public:

    
    #define CONFIG_FILE_DESCRIPTOR  gcnew cli::array<ConfigFile::paramItemT^>
    #define CONFIG_FILE_ITEM        gcnew ConfigFile::paramItemT
    #define CONFIG_FILE_DEFAULT     gcnew cli::array<System::String^>
    
    ref class paramItemT 
    {
        public:

        paramItemT(System::String^ tg, System::String^ cmt, cli::array<System::String^>^ def) {
            tag = tg;
            defaults = def;
            values = gcnew cli::array<System::String^>(defaults->Length);
            for (int i = 0; i < defaults->Length; i++) values[i] = (System::String^)defaults[i] ;
            comment = cmt;
        }
        System::String^ tag;        //!< Tag name of the parameter
        cli::array<System::String^>^ defaults; //!< Default Value list of a parameter
        System::String^ comment;    //!< comment
        cli::array<System::String^>^ values;   //!< Value list of a parameter
        
    };
   

    //! Configuration file descriptor
    ref class fileDescriptorT
    {
        public:

        System::String^ filename;   //!< Configuration filename
        int revision;       //!< Configuration revision code
        cli::array<paramItemT^>^ descriptor; //!< Parameter descriptor
    };


    ConfigFile(System::String^ filename, int revision, cli::array<paramItemT^>^ descriptor);

    //! Load the configuration file in memory
    bool loadFile(void);

    //! Stores the memory content of the configuration file into the file.
    void storeFile(void);

    //! Override thecurrent file with the default values
    inline void setDefaultFile(void){ createDefaultFile(); }
    inline System::String^ getFilename(void) { return filename; }
    inline bool isWarning(void) { return warning; }
    inline System::String^ getWarningString(void) { return warning_string; }
    
    cli::array<System::String^>^ getParam(System::String^ tag){
        int i = getTagPosition((System::String^) tag);
        if(i < 0)    return nullptr;        
        return data[i]->values;
    }


    bool setParam(System::String^ tag,  int index, System::String^ val){
        int i = getTagPosition(tag);
        if( (i < 0) || (index >= data[i]->values->Length)) return false;
        
        data[i]->values[index] = val;
        return true;
    }

protected:
    
    virtual bool revisionChangeCallback(void){

        return false;
    }


 private:
    
    cli::array<paramItemT^>^ data; //!< config file content in memory    
    System::String^ filename;   //!< Configuration filename
    int revision;       //!< Configuration revision code    
    int loaded_revision;//!< Revision read from the config file
    
    bool warning;
    System::String^ warning_string;
    

    /**
     * @brief getTagPosition
     * Get the index position of a given tag in the memory structure
     * @param tag
     * tag name
     * @return
     */
    int getTagPosition( System::String^ tag);
    System::String^ getConfigPath(void);
    void createDefaultFile(void); //!< Creates the default file based on the template
    bool decodeLine(System::String^ rline);

};


