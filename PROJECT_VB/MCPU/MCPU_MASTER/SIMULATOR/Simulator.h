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
ref class Simulator
{
public:
	static const Byte STX = 0x3;
	static const Byte ETX = 0x4;

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

	delegate void rxData_slot(void);
	static event rxData_slot^ canrx_canopen_sdo_event; //!< CanOpen SDO received buffer
	static event rxData_slot^ canrx_canopen_reset_event;
	static event rxData_slot^ canrx_device_event; //!< Metaltronica Buffer

	delegate void connection_slot(bool status); //!< This is the delegate of the connection handler
	event connection_slot^ connection_event; //!< Connection event

	Simulator(void); //!< This is the class constructor
	static Simulator^ device = gcnew Simulator();

	void startSimulator(void);	
	static bool send(unsigned short canId, unsigned char* data, unsigned char len);//!< This is function to send data in the BUS


private:
	static bool started = false;
	static bool connection_status = false;//!< This is the current connection status
	
	void threadWork(void);//!< This is the worker thread for the connection management	
	System::Threading::Thread^ running_thread;//!< This is the worker thread handler
	System::Net::Sockets::Socket^ serverSocket;//!< This is the server socket handler
	System::Net::Sockets::Socket^ clientSocket;//!< This is the connected client socket handler


	static unsigned char datalen;
	static unsigned short canId;

	static int rx_rc;//!< This is the number of the received bytes
	static cli::array<System::Byte>^ rxBuffer;//!< This is the reception buffer
	static void handleBuffer(void);

	
};

