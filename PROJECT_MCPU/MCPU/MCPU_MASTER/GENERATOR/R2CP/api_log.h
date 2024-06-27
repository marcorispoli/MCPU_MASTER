#ifndef _UTIL_LOG_2015_06_08_H_
	#define _UTIL_LOG_2015_06_08_H_

/*
* Operating System.
*/


  namespace Api
  {
	  typedef enum
	  {
		  Debug = 0,
		  Info,
		  Error,
		  Warning,
		  Critical
	  }Log_level;

	  typedef enum
	  {
		  Serial_Port = 0,
		  Internal_uC_Memory,
		  SdCard,
		  NvRam,
		  Ethernet
	  }Output;

	  class Log
	  {
	  public:
		  
		  static void Trace( const char *string );
		  
		  static void DebugPrint( const char *string, ... );
		  
		  static void Print( Log_level error_level, const char *string, ...);
		  
		  static void Set_System_Level(Log_level new_level = Log_level::Info , Output output = Output::Serial_Port , void (*LogByEthernet)(unsigned char , char*) = nullptr);
		  
	  private:
		  static Log_level system_level_;
		  static Output log_output_;
		  static void (*fcb_LogByEthernet_)(unsigned char , char*);
	  };
  }


#endif  
