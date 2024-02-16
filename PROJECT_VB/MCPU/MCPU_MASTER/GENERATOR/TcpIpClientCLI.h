#pragma once

#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include "winuser.h"



#pragma comment (lib, "User32.lib")

using namespace System;
using namespace System::Threading;
using namespace System::Diagnostics;
using namespace System::Net;
using namespace System::Net::Sockets;

#define TcpClientCLI_DISPATCH_MESSAGE (WM_USER + 1)
#define TcpClientCLI_DISCONNECTION_MESSAGE (WM_USER + 2)
#define TcpClientCLI_CONNECTION_MESSAGE (WM_USER + 3)

#define TcpClientCLI_RX_BUFFER_SIZE (4096)
#define TcpClientCLI_TX_BUFFER_SIZE (4096)
#define TcpClientCLI_DISPATCH_BUFFER_SIZE (100)

ref class TcpClientCLI
{
public:

	delegate void rxData_slot(cli::array<Byte>^ receiveBuffer, int rc);//!< This is the delegate of the reception handler
	event rxData_slot^ rxData_event; //!< Reception event 

	delegate void connection_slot(bool status); //!< This is the delegate of the connection handler
	event connection_slot^ connection_event; //!< Connection event

	TcpClientCLI(String^ ip, int port); //!< This is the class constructor
	void send(cli::array<Byte>^ buffer);//!< This is function to send data in the BUS

	/// <summary>
	/// Returns the current connection sattus
	/// </summary>
	/// <param name=""></param>
	/// <returns>true if the socket is connected </returns>
	public: bool isConnected(void) { return connection_status; }
	protected: void threadWork(void);//!< This is the worker thread for the connection management
	

protected:
	void startConnection(void) { running_thread->Start();}
	virtual void rxData(cli::array<Byte>^ receiveBuffer, int rc);

private:

	Thread^ running_thread;//!< This is the worker thread handler
	Socket^ clientSocket;//!< This is the connected client socket handler
	IPAddress^ serverAddress ;
	IPEndPoint^ remoteEndpoint;
	String^ ipserver;
	int     portserver;

	HANDLE ghSemaphore;//!< This is the semaphore used to protect the received buffer during the buffer dispatching
	bool connection_status;//!< This is the current connection status
	cli::array<Byte>^ rxBuffer;//!< This is the reception buffer
	
};

