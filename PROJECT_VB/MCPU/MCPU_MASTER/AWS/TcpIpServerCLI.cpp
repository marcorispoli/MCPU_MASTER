#include "pch.h"
#include "TcpIpServerCLI.h"

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
TcpIpServerCLI::TcpIpServerCLI(String^ ip, int port) 
{

	// Gets the Handler of the Form parent class, to be used for the message exchange
	window = static_cast<HWND>(Handle.ToPointer());
	connection_status = false;

	// Creates the semaphore to protect the reception buffer and the dispatched buffer, residing in different threads
	ghSemaphore = CreateSemaphore(
		NULL,           // default security attributes
		1,  // initial count
		1,  // maximum count
		NULL);

	// Sets the Tcp/Ip data for the socket
	IPAddress^ localAddress = IPAddress::Parse(ip);
	SocketType sockType = SocketType::Stream;
	ProtocolType sockProtocol = ProtocolType::Tcp;
	serverSocket = nullptr;

	IPEndPoint^ localEndPoint = gcnew IPEndPoint(localAddress, port);
	IPEndPoint^ senderAddress = gcnew IPEndPoint(localAddress, 0);

	clientSocket = nullptr;
	dispatchBuffer = gcnew cli::array<Byte>(TcpIpServerCLI_DISPATCH_BUFFER_SIZE);
	rxBuffer = gcnew cli::array<Byte>(TcpIpServerCLI_RX_BUFFER_SIZE);
	

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
		SendNotifyMessageA(window, TcpIpServerCLI_CONNECTION_MESSAGE, 0, 0);

		while (true) {
			rx_rc = clientSocket->Receive(rxBuffer);
			if (rx_rc == 0) break; // Shutdown

			// Protects with the Semaphore
			dwWaitResult = WaitForSingleObject(
				ghSemaphore,   // handle to semaphore
				1000);         // one-second time-out interval

			//  When the semaphore is unlocked can proceed with the event propagation in the main thread
			if (dwWaitResult == WAIT_OBJECT_0) {

				// Copies the received buffer into the buffer to be dispatched
				int i;
				for (i = 0; i < rx_rc; i++) dispatchBuffer[i] = rxBuffer[i];
				dispatchBuffer[i] = '\0';
				dispatch_rc = rx_rc;

				SendNotifyMessageA(window, TcpIpServerCLI_DISPATCH_MESSAGE, 0, 0);// rxData_event(receiveBuffer, rc); // Rise the event rxData
			}
			else {
				// In case of Semaphore timeout, the data is lost and the semaphore is reset
				Debug::WriteLine("Timeout Reception: data lost!\n");

				// Release the semaphore to prevent lock
				ReleaseSemaphore(
					ghSemaphore,  // handle to semaphore
					1,            // increase count by one
					NULL);
			}


		}
		connection_status = false;

		// The Client closed the connection
		Debug::WriteLine("Client Shutdown!\n");
		clientSocket->Shutdown(SocketShutdown::Send);
		clientSocket->Close();
		
		// Notifies the disconnection status
		SendNotifyMessageA(window, TcpIpServerCLI_DISCONNECTION_MESSAGE, 0, 0);

	}

}

/// <summary>
/// This is the ovverride function for the Window's messages
/// 
/// The function shall receive the messages generated by the worker thread 
/// in order to rise events for the connection status and the data received.
/// 
/// When the data is received (message TcpIpServerCLI_DISPATCH_MESSAGE) 
/// the rxData_event() is generated.
/// 
/// When the client gets a connection with the server  (message TcpIpServerCLI_CONNECTION_MESSAGE) 
/// the connection_event(true) is generated.
/// 
/// When the client losts a connection with the server  (message TcpIpServerCLI_DISCONNECTION_MESSAGE) 
/// the connection_event(false) is generated.
/// 
/// 
/// 
/// </summary>
/// <param name="m"></param>
void TcpIpServerCLI::WndProc(System::Windows::Forms::Message% m)
{


	//Do stuff in here
	switch (m.Msg) {

	case TcpIpServerCLI_DISPATCH_MESSAGE:
		Debug::WriteLine("User Dispach Message!\n");

		// Dispatches the received buffer
		rxData_event(dispatchBuffer, dispatch_rc);

		// Release the semaphore because has not to lock 
		ReleaseSemaphore(
			ghSemaphore,  // handle to semaphore
			1,            // increase count by one
			NULL);
		break;

	case TcpIpServerCLI_DISCONNECTION_MESSAGE:
		Debug::WriteLine("User Disconnection Message!\n");
		connection_event(false);
		break;

	case TcpIpServerCLI_CONNECTION_MESSAGE:
		Debug::WriteLine("User Connection Message!\n");		
		connection_event(true);
		break;
	
	}

	Form::WndProc(m);
}

/// <summary>
/// This function sends a buffer on the bus.
/// 
/// </summary>
/// <param name="buffer">This is the data buffer to be sent</param>
void TcpIpServerCLI::send(cli::array<Byte>^ buffer) {
	if (!connection_status) return;
	if (clientSocket == nullptr) return;

	clientSocket->Send(buffer);

}