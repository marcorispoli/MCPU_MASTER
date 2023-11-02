#pragma once

#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include "winuser.h"



#pragma comment (lib, "User32.lib")


#define TcpIpServerCLI_DISPATCH_MESSAGE (WM_USER + 1)
#define TcpIpServerCLI_DISCONNECTION_MESSAGE (WM_USER + 2)
#define TcpIpServerCLI_CONNECTION_MESSAGE (WM_USER + 3)

#define TcpIpServerCLI_RX_BUFFER_SIZE (4096)
#define TcpIpServerCLI_TX_BUFFER_SIZE (4096)
#define TcpIpServerCLI_DISPATCH_BUFFER_SIZE (100)

ref class TcpIpServerCLI: public System::Windows::Forms::Form
{
public:

	delegate void rxData_slot(cli::array<System::Byte>^ receiveBuffer, int rc);//!< This is the delegate of the reception handler
	event rxData_slot^ rxData_event; //!< Reception event 

	delegate void connection_slot(bool status); //!< This is the delegate of the connection handler
	event connection_slot^ connection_event; //!< Connection event

	TcpIpServerCLI(System::String^ ip, int port); //!< This is the class constructor

	void send(cli::array<System::Byte>^ buffer);//!< This is function to send data in the BUS

	/// <summary>
	/// Returns the current connection sattus
	/// </summary>
	/// <param name=""></param>
	/// <returns>true if the socket is connected </returns>
	public: bool isConnected(void) { return connection_status; }

	protected: void threadWork(void);//!< This is the worker thread for the connection management
	protected:  virtual void WndProc(System::Windows::Forms::Message% m) override;//!< This is the function handling the Window's messages

private:

	HWND window; //!< This is Form's class handler
	System::Threading::Thread^ running_thread;//!< This is the worker thread handler
	System::Net::Sockets::Socket^ serverSocket;//!< This is the server socket handler
	System::Net::Sockets::Socket^ clientSocket;//!< This is the connected client socket handler
	HANDLE ghSemaphore;//!< This is the semaphore used to protect the received buffer during the buffer dispatching

	bool connection_status;//!< This is the current connection status

	int rx_rc;//!< This is the number of the received bytes
	cli::array<System::Byte>^ rxBuffer;//!< This is the reception buffer

	int dispatch_rc;//!< This is the number of the byte to be dispatched
	cli::array<System::Byte>^ dispatchBuffer;//!< This is the buffer to be dispatched
	
	
};

