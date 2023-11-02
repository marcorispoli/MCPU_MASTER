
#include <thread>
#include "TcpIpClientCLI.h"


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
TcpClientCLI::TcpClientCLI(String^ ip, int port) 
{

	ipserver = ip;
	portserver = port;

	// Gets the Handler of the Form parent class, to be used for the message exchange
	connection_status = false;
	rxBuffer = gcnew cli::array<Byte>(TcpClientCLI_RX_BUFFER_SIZE);

	//IPEndPoint^ localEndPoint = gcnew IPEndPoint(localAddress, port);
	serverAddress = IPAddress::Parse(ip);
	remoteEndpoint = gcnew IPEndPoint(serverAddress, port);
	

	// Start the reception thread
	running_thread = gcnew Thread(gcnew ThreadStart(this, &TcpClientCLI::threadWork));
	running_thread->Name = "Loop TcpIp Client";
	running_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
	running_thread->Start();

}

void TcpClientCLI::threadWork(void) {
	DWORD dwWaitResult;
	SocketType sockType = SocketType::Stream;
	ProtocolType sockProtocol = ProtocolType::Tcp;
	
	int rc;

	
	while (true) {
		Debug::WriteLine("Try to connect server to: " + ipserver + "\n");		
		connection_status = false;

		// Try to connect with the server
		clientSocket = gcnew Socket(serverAddress->AddressFamily, sockType, sockProtocol);
		try {
			clientSocket->Connect(remoteEndpoint);			
		}
		catch (SocketException^ err)
		{
			clientSocket->Close();
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			connection_status = false;
			continue;
		}
		
		// The server is now connected
		connection_status = true;
		connection_event(true);
		Debug::WriteLine("Client connected to: " + ipserver + "\n");

		// Connection loop
		while (true) {
			//IPEndPoint^ fromEndPoint = gcnew IPEndPoint(remoteEndpoint->Address, 0);
			//EndPoint^ castFromEndPoint = (EndPoint^)fromEndPoint;
			try {
				rc = clientSocket->Receive(rxBuffer);// From(rxBuffer, castFromEndPoint);

				if (rc == 0) {
					Debug::WriteLine("Client disconnected from: " + ipserver + "\n");
					clientSocket->Close();					
					connection_status = false;
					connection_event(false);
					break;
				}

				// Dispatches the received buffer
				rxData(rxBuffer, rc);
			}
			catch (SocketException^ err) {
				Debug::WriteLine("Client disconnected from: " + ipserver + "\n");
				clientSocket->Close();				
				connection_status = false;
				connection_event(false);
				break;
			}


		}

	}

}

void TcpClientCLI::rxData(cli::array<Byte>^ receiveBuffer, int rc){

	// As default the event is sent
	rxData_event(rxBuffer, rc);
}

/// <summary>
/// This function sends a buffer on the bus.
/// 
/// </summary>
/// <param name="buffer">This is the data buffer to be sent</param>
void TcpClientCLI::send(cli::array<Byte>^ buffer) {
	if (!connection_status) return;
	if (clientSocket == nullptr) return;

	try {
		clientSocket->Send(buffer);
	}
	catch (SocketException^ err) {

	}
	

}
