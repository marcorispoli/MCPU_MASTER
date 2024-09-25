
#include <thread>
#include "CanSimulator.h"
#include "mutex"
#include "Log.h"
#include "ConfigurationFiles.h"

#include "PCB301.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB304.h"
#include "PCB315.h"
#include "PCB326.h"
#include "ArmMotor.h"
#include "TiltMotor.h"
#include "SlideMotor.h"
#include "BodyMotor.h"
#include "VerticalMotor.h"


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
CanSimulator::CanSimulator(void)
{
	// Gets the Handler of the Form parent class, to be used for the message exchange
	connection_status = false;
	started = false;

	canDataBuffer = gcnew cli::array<Byte>(8);

	// Sets the Tcp/Ip data for the socket
	IPAddress^ localAddress = IPAddress::Parse(SystemConfig::Configuration->getParam(SystemConfig::PARAM_SYM_MODE)[SystemConfig::SYM_MODE_IP]);
	SocketType sockType = SocketType::Stream;
	ProtocolType sockProtocol = ProtocolType::Tcp;
	serverSocket = nullptr;


	IPEndPoint^ localEndPoint = gcnew IPEndPoint(localAddress, System::Convert::ToInt16(SystemConfig::Configuration->getParam(SystemConfig::PARAM_SYM_MODE)[SystemConfig::SYM_MODE_PORT]));
	IPEndPoint^ senderAddress = gcnew IPEndPoint(localAddress, 0);

	clientSocket = nullptr;
	rxBuffer = gcnew cli::array<System::Byte>(4096);
	rx_rc = 0;

	// Create the server socket
	serverSocket = gcnew Socket(localAddress->AddressFamily, sockType, sockProtocol);
	serverSocket->Bind(localEndPoint);

	// Start the reception thread
	running_thread = gcnew Thread(gcnew ThreadStart(&CanSimulator::threadWork));
	running_thread->Name = "Loop Simulator Server";
	running_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
	running_thread->Start();
}

void CanSimulator::threadWork(void) {
	DWORD dwWaitResult;

	while (true) {
		serverSocket->Listen(5);
		clientSocket = serverSocket->Accept();
		LogClass::logInFile("Simulator Server: Connected!\n");
		connection_status = true;

		// Notifies the connection status
		connection_event(true);

		// Sends the system configuration to the simulator application
		sendConfiguration();

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
void CanSimulator::sendConfiguration() {
	cli::array<Byte>^ buffer = gcnew cli::array<Byte>((int)can_buf_struct::BUFLEN);

	buffer[(Byte)can_buf_struct::STX] = CanSimulator::STX;
	buffer[(Byte)can_buf_struct::LENGHT] = (Byte)can_buf_struct::BUFLEN;
	buffer[(Byte)can_buf_struct::CAN_IDL] = 0x00;
	buffer[(Byte)can_buf_struct::CAN_IDH] = 0xFF;
	buffer[(Byte)can_buf_struct::ETX] = CanSimulator::ETX;
	buffer[(Byte)can_buf_struct::DATALEN] = 8;
	
	// Board Simulation bit
	buffer[(Byte)can_buf_struct::D0] = 0;
	if (Gantry::isPcb301Demo()) buffer[(Byte)can_buf_struct::D0] |= 1;
	if (Gantry::isPcb302Demo()) buffer[(Byte)can_buf_struct::D0] |= 2;
	if (Gantry::isPcb303Demo()) buffer[(Byte)can_buf_struct::D0] |= 4;
	if (Gantry::isPcb304Demo()) buffer[(Byte)can_buf_struct::D0] |= 8;
	if (Gantry::isPcb315Demo()) buffer[(Byte)can_buf_struct::D0] |= 0x10;
	if (Gantry::isPcb326Demo()) buffer[(Byte)can_buf_struct::D0] |= 0x20;

	// Motor Simulation Bit
	buffer[(Byte)can_buf_struct::D1] = 0;
	if (Gantry::isMotorVerticalDemo()) buffer[(Byte)can_buf_struct::D1] |= 1;
	if (Gantry::isMotorBodyDemo()) buffer[(Byte)can_buf_struct::D1] |= 2;
	if (Gantry::isMotorArmDemo()) buffer[(Byte)can_buf_struct::D1] |= 4;
	if (Gantry::isMotorTiltDemo()) buffer[(Byte)can_buf_struct::D1] |= 8;
	if (Gantry::isMotorSlideDemo()) buffer[(Byte)can_buf_struct::D1] |= 0x10;
	
	try {
		device->clientSocket->Send(buffer);
	}
	catch (...) {
		LogClass::logInFile("Simulator Server: failed sending the configuration word !\n");
	}

	return;
}

void CanSimulator::sendMotorRotConfiguration(unsigned char devId, double rot_convertion) {
	
	cli::array<System::Byte>^ buffer = gcnew cli::array<System::Byte>((int)can_buf_struct::BUFLEN);
	char* byteArray = reinterpret_cast<char*>(&rot_convertion);
	for (int i = 0; i < 8; i++) buffer[(Byte)can_buf_struct::D0 + i] = byteArray[i];

	buffer[(System::Byte)can_buf_struct::STX] = CanSimulator::STX;
	buffer[(System::Byte)can_buf_struct::LENGHT] = (Byte)can_buf_struct::BUFLEN;
	buffer[(System::Byte)can_buf_struct::CAN_IDL] = devId;
	buffer[(System::Byte)can_buf_struct::CAN_IDH] = 0xFF;
	buffer[(System::Byte)can_buf_struct::ETX] = CanSimulator::ETX;
	buffer[(Byte)can_buf_struct::DATALEN] = 8;

	try {
		device->clientSocket->Send(buffer);
	}
	catch (...) {
		LogClass::logInFile("Simulator Server: failed sending the motor configuration word !\n");
	}

	return;
}
void CanSimulator::handleBuffer(void) {

	
	// Scroll for possible concatenated frames
	for (int j = 0; j < rx_rc; j++) {

		if (rx_rc - j < 4) continue;

		// Start byte
		if (rxBuffer[j + (int)can_buf_struct::STX] != (int)CanSimulator::STX)  continue;

		// Lenght of frame
		int len = rxBuffer[j + (int)can_buf_struct::LENGHT];
		if (len > rx_rc - j) continue;

		// End of frame
		if (rxBuffer[j + (int)can_buf_struct::ETX] != (int)CanSimulator::ETX) continue;

		// can Data Len
		datalen = (unsigned char)rxBuffer[j + (int)can_buf_struct::DATALEN];
		if (datalen > 8) continue;

		// Destination address
		canId = (unsigned short)rxBuffer[j + (int)can_buf_struct::CAN_IDL] + 256 * (unsigned short)rxBuffer[j + (int)can_buf_struct::CAN_IDH];

		// Destination buffer
		for (int i = 0; i < datalen; i++) canDataBuffer[i] = rxBuffer[j + (int)can_buf_struct::D0 + i];

		if ((canId >= 0x100) && (canId <= 0x17F)) {
			canrx_device_event();
			continue;
		}

		if ((canId >= 0x580) && (canId <= 0x5FF)) {

			canrx_canopen_sdo_event();
			continue;
		}

		if ((canId >= 0x700) && (canId <= 0x707)) {
			canrx_canopen_bootup_event();
			continue;
		}

	}
}


bool CanSimulator::send(unsigned short canId, unsigned char* data, unsigned char len) {
	
	if (!connection_status) return false;

	if (len > 8) len = 8;

	cli::array<Byte>^ buffer = gcnew cli::array<Byte>((int) can_buf_struct::BUFLEN);
	buffer[(Byte)can_buf_struct::STX] = CanSimulator::STX;
	buffer[(Byte)can_buf_struct::LENGHT] = (Byte)can_buf_struct::BUFLEN;
	buffer[(Byte)can_buf_struct::CAN_IDL] = (Byte) (canId & 0xff);
	buffer[(Byte)can_buf_struct::CAN_IDH] = (Byte)((canId>>8) & 0xff);
	buffer[(Byte)can_buf_struct::DATALEN] = len;
	for (unsigned char i = 0; i < len; i++) buffer[(Byte)can_buf_struct::D0 + i] = data[i];	
	buffer[(Byte)can_buf_struct::ETX] = CanSimulator::ETX;

	try{
		device->clientSocket->Send(buffer);
	}
	catch (...) {
		return false;
	}
	
	return true;
	
}

