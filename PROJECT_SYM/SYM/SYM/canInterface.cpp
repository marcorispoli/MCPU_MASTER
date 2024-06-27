
#include <thread>
#include "canInterface.h"
#include "mutex"


static std::mutex send_mutex;
#define BUFFER_SIZE 1000

canInterface::canInterface(void)
{

	ipserver = "127.0.0.1";
	portserver = 21000;

	// Gets the Handler of the Form parent class, to be used for the message exchange
	connection_status = false;
	rxBuffer = gcnew cli::array<Byte>(BUFFER_SIZE);
	canDataBuffer = gcnew cli::array<Byte>(8);
	txBuffer = gcnew cli::array<Byte>((int) can_buf_struct::BUFLEN);
	
	txBuffer[(int)can_buf_struct::STX] = (Byte)canInterface::STX;
	txBuffer[(int)can_buf_struct::ETX] = (Byte)canInterface::ETX;
	txBuffer[(int)can_buf_struct::LENGHT] = (Byte)can_buf_struct::BUFLEN;

	
	serverAddress = IPAddress::Parse(ipserver);
	remoteEndpoint = gcnew IPEndPoint(serverAddress, portserver);
	

	// Start the reception thread
	running_thread = gcnew Thread(gcnew ThreadStart(&canInterface::threadWork));
	running_thread->Name = "Can Driver TcpIp Client";
	running_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
	running_thread->Start();
}

void canInterface::threadWork(void) {
	
	SocketType sockType = SocketType::Stream;
	ProtocolType sockProtocol = ProtocolType::Tcp;
	
	while(!activate) std::this_thread::sleep_for(std::chrono::milliseconds(100));

	while (true) {			
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

		// Connection loop
		while (true) {
			
			try {
				rc = clientSocket->Receive(rxBuffer);// From(rxBuffer, castFromEndPoint);

				if (rc == 0) {					
					clientSocket->Close();					
					connection_status = false;
					connection_event(false);
					break;
				}

				// Dispatches the received buffer
				handleBuffer();
			}
			catch (SocketException^ err) {				
				clientSocket->Close();				
				connection_status = false;
				connection_event(false);
				break;
			}


		}

	}

}

void canInterface::handleBuffer() {
	
	// Scroll for possible concatenated frames
	for (int j = 0; j < rc; j++) {				
		
		if (j + (int) can_buf_struct::BUFLEN >= BUFFER_SIZE) return;
	
		
		// Start byte
		if (rxBuffer[j + (int)can_buf_struct::STX] != (int) canInterface::STX)  continue;

		// Lenght of frame
		int len = rxBuffer[j + (int)can_buf_struct::LENGHT];
		if (len > rc - j) continue;

		// End of frame
		if (rxBuffer[j + (int)can_buf_struct::ETX] != (int)canInterface::ETX) continue;

		// can Data Len
		datalen = (unsigned char) rxBuffer[j + (int)can_buf_struct::DATALEN];
		if (datalen > 8) continue;

		// Destination address
		canId = (unsigned short) rxBuffer[j + (int)can_buf_struct::CAN_IDL] + 256 * (unsigned short)rxBuffer[j + (int)can_buf_struct::CAN_IDH];

		// Destination buffer
		for (int i = 0; i < datalen; i++) canDataBuffer[i] = rxBuffer[j + (int)can_buf_struct::D0 + i];

		j += (len-1);
		
		// Configuration word
		if ((canId & 0xFF00) == 0xFF00) {
			canrx_configuration_event();
			continue;
		}

		if ((canId >= 0x100) && (canId <= 0x17F)) {
			canrx_device_event();
			continue;
		}

		if ((canId >= 0x600) && (canId <= 0x6FF)) {

			canrx_canopen_sdo_event();
			continue;
		}
		
		if (canId == 0) {
			canrx_canopen_nmt_event();
			continue;
		}

	}
}


void canInterface::send(unsigned short canId, unsigned char dlen, cli::array<Byte>^ buffer) {
	
	// Protect the multi thread send commands
	const std::lock_guard<std::mutex> lock(send_mutex);


	if (!connection_status) return;
	if (clientSocket == nullptr) return;

	try {
		if (buffer->Length < dlen) return;
		if (dlen > 8) return;

		txBuffer[(int)can_buf_struct::CAN_IDL] = (Byte) (canId & 0xFF);
		txBuffer[(int)can_buf_struct::CAN_IDH] = (Byte)((canId >> 8) & 0xFF);
		txBuffer[(int)can_buf_struct::DATALEN] = (Byte)dlen;
		
		// Copies the can data byte
		for (int i = 0; i < dlen; i++) txBuffer[(int)can_buf_struct::D0 + i] = buffer[i];

		clientSocket->Send(txBuffer);
	}
	catch (SocketException^ err) {

	}
	
}
