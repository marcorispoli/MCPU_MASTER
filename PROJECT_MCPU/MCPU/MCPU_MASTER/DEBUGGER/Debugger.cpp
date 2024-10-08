#include "Debugger.h"
#include "Log.h"
#include "PCB301.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB304.h"
#include "PCB326.h"
#include "TiltMotor.h"
#include "ArmMotor.h"
#include "VerticalMotor.h"
#include "BodyMotor.h"
#include "SlideMotor.h"




using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Threading;
using namespace System::Diagnostics;

DebuggerCLI::DebuggerCLI(System::String^ ip, int port)
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
	running_thread = gcnew Thread(gcnew ThreadStart(this, &DebuggerCLI::threadWork));
	running_thread->Name = "Loop Debugger thread";
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
void DebuggerCLI::threadWork(void) {

	while (true) {
		serverSocket->Listen(5);
		clientSocket = serverSocket->Accept();
		LogClass::logInFile("Debugger Client Connected!\n");
		connection_status = true;

		// Notifies the connection status
		connection_event(true);
		
		while (true) {
			try{
				rx_rc = clientSocket->Receive(rxBuffer);
				if (rx_rc == 0) break; // Shutdown
				rxHandler();
			 }
			catch (...) {
				break;
			}

		}
		connection_status = false;

		// The Client closed the connection
		LogClass::logInFile("Debugger Shutdown!\n");
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
void DebuggerCLI::send(cli::array<System::Byte>^ buffer) {
	if (!connection_status) return;
	if (clientSocket == nullptr) return;

	clientSocket->Send(buffer);

}



void DebuggerCLI::rxHandler(void){

	// Decode with Unicode the byte array into a string
	System::String^ sFrame = System::Text::UTF8Encoding::UTF8->GetString(rxBuffer, 0, rx_rc);

	System::String^ result_string = "";

	// Can Statistics
	if (sFrame->Contains("CANSTAT")) {
		result_string += "PCB301: " + PCB301::device->getCanCommunicationMonitorString() + "\n\r";
		result_string += "PCB302: " + PCB302::device->getCanCommunicationMonitorString() + "\n\r";
		result_string += "PCB303: " + PCB303::device->getCanCommunicationMonitorString() + "\n\r";
		result_string += "PCB304: " + PCB304::device->getCanCommunicationMonitorString() + "\n\r";		
		result_string += "PCB326: " + PCB326::device->getCanCommunicationMonitorString() + "\n\r";

		if (!TiltMotor::device->isSimulatorMode()) result_string += "TiltMotor: " + TiltMotor::device->getCanCommunicationMonitorString() + "\n\r";
		if (!ArmMotor::device->isSimulatorMode()) result_string += "ArmMotor: " + ArmMotor::device->getCanCommunicationMonitorString() + "\n\r";
		if (!VerticalMotor::device->isSimulatorMode()) result_string += "VerticalMotor: " + VerticalMotor::device->getCanCommunicationMonitorString() + "\n\r";
		if (!BodyMotor::device->isSimulatorMode()) result_string += "BodyMotor: " + BodyMotor::device->getCanCommunicationMonitorString() + "\n\r";
		if (!SlideMotor::device->isSimulatorMode()) result_string += "SlideMotor: " + SlideMotor::device->getCanCommunicationMonitorString() + "\n\r";
		send(System::Text::Encoding::Unicode->GetBytes(result_string));
		return;
	}

	if (sFrame->Contains("COMPRESSOR")) {
		result_string += "PCB302> RAW POS:" + PCB302::device->getRawPosition().ToString() + "\n\r";
		result_string += "PCB302> RAW FORCE:" + PCB302::device->getRawForce().ToString() + "\n\r";
		result_string += "PCB302> PADDLE POS:" + PCB302::device->getPaddlePosition().ToString() + "\n\r";
		result_string += "PCB302> PADDLE FORCE:" + PCB302::device->getPaddleForce().ToString() + "\n\r";
		send(System::Text::Encoding::Unicode->GetBytes(result_string));
	}

}

