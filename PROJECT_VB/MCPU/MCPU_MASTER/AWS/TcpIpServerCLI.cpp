#include "TcpIpServerCLI.h"

using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Threading;
using namespace System::Diagnostics;


/// <summary>
/// This is the Class constructor 
/// 
/// The constructor creates all the data necessaries to create the server socket.
/// 
/// After the data initialization the constructor runs the worker thread 
/// for the Listening and data reception. The thread is self destroying when the 
/// program closes.
/// 
/// 
/// </summary>
/// <param name="ip">This is a String for the IP address</param>
/// <param name="port">This is an integer value for thr server port</param>
TcpIpServerCLI::TcpIpServerCLI(System::String^ ip, int port) 
{

	// Gets the Handler of the Form parent class, to be used for the message exchange
	connection_status = false;

	
	// Sets the Tcp/Ip data for the socket
	IPAddress^ localAddress = IPAddress::Parse(ip);
	SocketType sockType = SocketType::Stream;
	ProtocolType sockProtocol = ProtocolType::Tcp;
	serverSocket = nullptr;

	IPEndPoint^ localEndPoint = gcnew IPEndPoint(localAddress, port);
	IPEndPoint^ senderAddress = gcnew IPEndPoint(localAddress, 0);

	clientSocket = nullptr;
	rxBuffer = gcnew cli::array<System::Byte>(TcpIpServerCLI_RX_BUFFER_SIZE);
	

	// Create the server socket
	serverSocket = gcnew Socket(localAddress->AddressFamily, sockType, sockProtocol);
	serverSocket->Bind(localEndPoint);

	// Start the reception thread
	running_thread = gcnew Thread(gcnew ThreadStart(this, &TcpIpServerCLI::threadWork));
	running_thread->Name = "Loop TcpIp Server";
	running_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
	running_thread->Start();

}

/// <summary>
/// This is the worker thread for the connection management.
/// 
/// There are a two nexted loops: 
/// - The first is for the listening and connection management;\n
/// - The second is for the data reception.
/// 
/// When a conmnection is closed, the first loop automatically enters in listening
/// in order to wait for a further connection.
/// Only one connection at a time is allowed!!
/// 
/// Whenever a connection status changes, the worjer thread 
/// update the Applicaition rising an event for the connection notify.
///  
/// When a buffer is received, a semaphore is used to lock the thread 
/// until the previous received buffer has been dispatched.
/// When the semaphore is ready, the received buffer is copied into the dispatch buffer 
/// in order to be safely sent to the destination.
/// 
/// The buffer is then dispatched in two steps:
/// - 1: a message with the data buffer from the worker thread is Posted to the main thread's message queue, where this class resides;
/// - 2: the message is self-received by the class that can safely rise an event handler;
/// - 3: the event handler with the received buffer can now be safely (thread safe) consumed by any class or Form residing into the Applicaiton Main Thread. 
/// 
/// </summary>
/// <param name=""></param>
void TcpIpServerCLI::threadWork(void) {
	DWORD dwWaitResult;

	while (true) {
		serverSocket->Listen(5);
		clientSocket = serverSocket->Accept();
		Debug::WriteLine("Client Connected!\n");
		connection_status = true;

		// Notifies the connection status
		connection_event(true);
		
		while (true) {
			rx_rc = clientSocket->Receive(rxBuffer);
			if (rx_rc == 0) break; // Shutdown
			rxData_event(rxBuffer, rx_rc);

		}
		connection_status = false;

		// The Client closed the connection
		Debug::WriteLine("Client Shutdown!\n");
		clientSocket->Shutdown(SocketShutdown::Send);
		clientSocket->Close();
		
		// Notifies the disconnection status
		connection_event(false);
	}

}


/// <summary>
/// This function sends a buffer on the bus.
/// 
/// </summary>
/// <param name="buffer">This is the data buffer to be sent</param>
void TcpIpServerCLI::send(cli::array<System::Byte>^ buffer) {
	if (!connection_status) return;
	if (clientSocket == nullptr) return;

	clientSocket->Send(buffer);

}