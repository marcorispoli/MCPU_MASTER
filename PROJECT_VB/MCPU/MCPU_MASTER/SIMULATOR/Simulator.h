#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include "winuser.h"

#pragma comment (lib, "User32.lib")

#define Simulator_RX_BUFFER_SIZE (4096)
#define Simulator_TX_BUFFER_SIZE (4096)

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
	#define PCB301_DEVID			1
	#define PCB302_DEVID			2
	#define PCB303_DEVID			3
	#define PCB304_DEVID			4
	#define PCB315_DEVID			5
	#define PCB326_DEVID			6


	delegate void rxData_slot(cli::array<System::Byte>^ receiveBuffer, int index, int rc);
	static event rxData_slot^ pcb301_rx_event; //!< Reception event 
	static event rxData_slot^ pcb302_rx_event; //!< Reception event 
	static event rxData_slot^ pcb303_rx_event; //!< Reception event 
	static event rxData_slot^ pcb304_rx_event; //!< Reception event 
	static event rxData_slot^ pcb315_rx_event; //!< Reception event 
	static event rxData_slot^ pcb326_rx_event; //!< Reception event 

	delegate void connection_slot(bool status); //!< This is the delegate of the connection handler
	event connection_slot^ connection_event; //!< Connection event

	Simulator(void); //!< This is the class constructor
	static Simulator^ device = gcnew Simulator();

	void startSimulator(void);	
	void send(cli::array<System::Byte>^ buffer);//!< This is function to send data in the BUS


private:
	static bool started = false;
	static bool connection_status = false;//!< This is the current connection status
	
	void threadWork(void);//!< This is the worker thread for the connection management	
	System::Threading::Thread^ running_thread;//!< This is the worker thread handler
	System::Net::Sockets::Socket^ serverSocket;//!< This is the server socket handler
	System::Net::Sockets::Socket^ clientSocket;//!< This is the connected client socket handler



	static int rx_rc;//!< This is the number of the received bytes
	static cli::array<System::Byte>^ rxBuffer;//!< This is the reception buffer
	static void handleBuffer(void);

	
};

