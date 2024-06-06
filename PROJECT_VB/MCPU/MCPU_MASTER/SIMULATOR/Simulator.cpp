
#include <thread>
#include "Simulator.h"
#include "mutex"
#include "Log.h"

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Threading;
using namespace System::Diagnostics;

static std::mutex send_mutex;

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
Simulator::Simulator(String^ ip, int port)
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
	rxBuffer = gcnew cli::array<System::Byte>(4096);


	// Create the server socket
	serverSocket = gcnew Socket(localAddress->AddressFamily, sockType, sockProtocol);
	serverSocket->Bind(localEndPoint);

	// Start the reception thread
	running_thread = gcnew Thread(gcnew ThreadStart(this, &Simulator::threadWork));
	running_thread->Name = "Loop Simulator Server";
	running_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
	running_thread->Start();

}

void Simulator::threadWork(void) {
	DWORD dwWaitResult;

	while (true) {
		serverSocket->Listen(5);
		clientSocket = serverSocket->Accept();
		LogClass::logInFile("Simulator Server: Connected!\n");
		connection_status = true;

		// Notifies the connection status
		connection_event(true);

		while (true) {
			try {
				rx_rc = clientSocket->Receive(rxBuffer);
				if (rx_rc == 0) break; // Shutdown
				handleBuffer();
				
			}
			catch (...) {
				break;
			}

		}
		connection_status = false;

		// The Client closed the connection
		LogClass::logInFile("Simulator Server: Shutdown!\n");
		clientSocket->Shutdown(SocketShutdown::Send);
		clientSocket->Close();

		// Notifies the disconnection status
		connection_event(false);
	}

}

void Simulator::handleBuffer(void) {

	
	// Scroll for possible concatenated frames
	for (int j = 0; j < rx_rc; j++) {
		
		if (rx_rc - j < 4) continue;

		// Start byte
		if (rxBuffer[j + 0] != 0x3) continue;

		// Lenght of frame
		int len = rxBuffer[j + 1];
		if (len > rx_rc - j) continue;

		// End of frame
		if (rxBuffer[j + len - 1] != 0x2) continue;

		// Destination
		if		(rxBuffer[j + 2] == PCB301_DEVID) pcb301_rx_event(rxBuffer, j, len);
		else if (rxBuffer[j + 2] == PCB302_DEVID) pcb302_rx_event(rxBuffer, j, len);
		else if (rxBuffer[j + 2] == PCB303_DEVID) pcb303_rx_event(rxBuffer, j, len);
		else if (rxBuffer[j + 2] == PCB304_DEVID) pcb304_rx_event(rxBuffer, j, len);
		else if (rxBuffer[j + 2] == PCB315_DEVID) pcb315_rx_event(rxBuffer, j, len);
		else if (rxBuffer[j + 2] == PCB326_DEVID) pcb326_rx_event(rxBuffer, j, len);
	}
}


void Simulator::send(cli::array<System::Byte>^ buffer) {
	if (!connection_status) return;
	if (clientSocket == nullptr) return;


	clientSocket->Send(buffer);
}

