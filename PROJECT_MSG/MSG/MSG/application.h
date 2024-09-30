#pragma once


using namespace System::Collections::Generic;
using namespace System::Drawing;

ref class configurationClass {
public:
	configurationClass(int len) {
		tags = gcnew cli::array<System::String^>(len);
		descriptions = gcnew cli::array<System::String^>(len);
	}

	cli::array<System::String^>^ tags;
	cli::array<System::String^>^ descriptions;
};

ref class msgItem
{
public: enum class item_type {
	ERRMSG = 0,
	WRNMSG,
	INFMSG,
	LABMSG,
	INVALID
};

	  msgItem(System::String^ stringa);

	  static System::String^ decodeTypeString(System::String^ stringa);
	  static System::String^ decodeTagString(System::String^ stringa);
	  static System::String^ decodeTitleString(System::String^ stringa);
	  static System::String^ decodeContentString(System::String^ stringa);

	  item_type tp;
	  System::String^ msg_code;
	  System::String^ msg_tag;
	  System::String^ msg_title;
	  System::String^ msg_content;
	  System::String^ msg_description;
	  bool changed;
	  bool translated;
};

ref class msgApp
{
public:
    
    msgApp();
    static msgApp^ handle = gcnew msgApp();
    static void initialize(void);

   
    // Monitor coordinates
    static int monitor_X0;//!< Pointer to the Monitor X0 position
    static int monitor_Y0;//!< Pointer to the Monitor Y0 position
    static System::String^ applicationResourcePath; //!< This is the current application resource path
    

    
};
