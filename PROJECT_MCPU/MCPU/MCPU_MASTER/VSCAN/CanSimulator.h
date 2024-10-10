#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include "winuser.h"

#pragma comment (lib, "User32.lib")


/// <summary>
/// This Class implements the Tcp/Ip Client connection with the SmartHub server.
/// 
/// </summary>
/// 
/// 
///	
/// \ingroup Exposure_Module  
ref class CanSimulator
{
public:
	static const System::Byte STX = 0x3;
	static const System::Byte ETX = 0x4;

	enum class can_buf_struct {
		STX = 0,
		LENGHT,
		CAN_IDL,
		CAN_IDH,
		DATALEN,
		D0,
		D1,
		D2,
		D3,
		D4,
		D5,
		D6,
		D7,
		ETX,
		BUFLEN
	};

	// Configuration address (low part)
	literal unsigned char SIMUL_CONFIG_ACTIVE_DEVICES	= 0;
	literal unsigned char SIMUL_CONFIG_MOTOR_CONFIG_TRX	= 3;
	literal unsigned char SIMUL_CONFIG_MOTOR_CONFIG_ARM = 4;
	literal unsigned char SIMUL_CONFIG_MOTOR_CONFIG_SLIDE = 5;
	literal unsigned char SIMUL_CONFIG_MOTOR_CONFIG_BODY = 6;
	literal unsigned char SIMUL_CONFIG_MOTOR_CONFIG_VERTICAL = 7;
	literal unsigned char SIMUL_CONFIG_FILTERS = 10;

	delegate void rxData_slot(void);
	static event rxData_slot^ canrx_canopen_sdo_event; //!< CanOpen SDO received buffer
	static event rxData_slot^ canrx_canopen_bootup_event;
	static event rxData_slot^ canrx_device_event; //!< Metaltronica Buffer

	delegate void connection_slot(bool status); //!< This is the delegate of the connection handler
	static event connection_slot^ connection_event; //!< Connection event

	CanSimulator(void); //!< This is the class constructor
	static CanSimulator^ device = gcnew CanSimulator();

		
	static bool send(unsigned short canId, unsigned char* data, unsigned char len);//!< This is function to send data in the BUS
	static unsigned char datalen;
	static unsigned short canId;
	static cli::array<System::Byte>^ canDataBuffer;//!< This is the data buffer decoded
	static bool connection_status = false;//!< This is the current connection status
	
	static void sendMotorRotConfiguration(unsigned char devId, double rot_convertion);
	static void sendFilterConfiguration(void);

private:
	static bool started = false;
	
	
	static void threadWork(void);//!< This is the worker thread for the connection management	
	static System::Threading::Thread^ running_thread;//!< This is the worker thread handler
	static System::Net::Sockets::Socket^ serverSocket;//!< This is the server socket handler
	static System::Net::Sockets::Socket^ clientSocket;//!< This is the connected client socket handler

	

	static int rx_rc;//!< This is the number of the received bytes
	static cli::array<System::Byte>^ rxBuffer;//!< This is the reception buffer
	static void handleBuffer(void);
	static void sendConfiguration(void);
	

	
};

