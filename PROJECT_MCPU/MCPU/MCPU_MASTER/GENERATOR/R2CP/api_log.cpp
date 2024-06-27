#include <Windows.h>
#include <string.h>
#include <stdio.h>

#include "api_log.h"

namespace Api
{
	  #define SIZEOF_STRING 	150
	  #define MAX_STRING		100
	  static char 	  cadena_final_[ SIZEOF_STRING ];
	  static bool	  LogMutex = false;

	  Log_level Log::system_level_	= Log_level::Debug;  
	  Output    Log::log_output_   	= Output::Serial_Port;			
	  void 		(*Log::fcb_LogByEthernet_)(unsigned char ,char*) = nullptr;
	  	  
	  void Log::Print(Log_level error_level, const char *string , ...)
	  {
			if( LogMutex )
			  return;
			
			LogMutex = true;
			  
			if (error_level < system_level_)
			{
				LogMutex = false;
				return;
			}
			
			if( strlen( string ) >  MAX_STRING )
			{
				LogMutex = false;
				return;
			}
			
			memset( cadena_final_ , 0 , SIZEOF_STRING );
			
			va_list			arglist;
			va_start( arglist, string );
			vsnprintf(cadena_final_, SIZEOF_STRING , string , arglist);
			va_end( arglist );
			
			if( log_output_ == Output::Serial_Port)
			{
				printf("\n\r");
				printf("%s", cadena_final_);
			}
			else if( log_output_ == Output::Ethernet && fcb_LogByEthernet_)
			{
				fcb_LogByEthernet_( static_cast<unsigned char >(error_level) , cadena_final_ );
			}
			
			LogMutex = false;
	  }
	
	  void Log::Set_System_Level( Log_level new_level , Output output , void (*LogByEthernet)(unsigned char , char*))
	  {
		  if( new_level > Log_level::Critical )
			  new_level = Log_level::Debug;
		  
		  if( output != Output::Serial_Port && output != Output::Ethernet )
			  output = Output::Serial_Port;
		  
		  system_level_ 		= new_level;
		  log_output_ 			= output;
		  fcb_LogByEthernet_	= LogByEthernet;
	  }
}
