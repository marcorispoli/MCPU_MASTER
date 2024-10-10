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


ref class canInterface
{
public:
	static const Byte STX = 0x3;
	static const Byte ETX = 0x4;

	enum class can_buf_struct {
		STX = 0,
		LENGHT,
		CAN_IDL,
		CAN_IDH,
		DATALEN,
		D0,
		D1,
		D2,
		D3,
		D4,
		D5,
		D6,
		D7,		
		ETX,
		BUFLEN
	};

	// Configuration address (low part)
	literal unsigned char SIMUL_CONFIG_ACTIVE_DEVICES = 0;
	literal unsigned char SIMUL_CONFIG_MOTOR_CONFIG_TRX = 3;
	literal unsigned char SIMUL_CONFIG_MOTOR_CONFIG_ARM = 4;
	literal unsigned char SIMUL_CONFIG_MOTOR_CONFIG_SLIDE = 5;
	literal unsigned char SIMUL_CONFIG_MOTOR_CONFIG_BODY = 6;
	literal unsigned char SIMUL_CONFIG_MOTOR_CONFIG_VERTICAL = 7;
	literal unsigned char SIMUL_CONFIG_FILTERS = 10;
	
	delegate void rxData_slot(void);
	static event rxData_slot^ canrx_canopen_sdo_event; //!< CanOpen SDO received buffer
	static event rxData_slot^ canrx_canopen_nmt_event;
	static event rxData_slot^ canrx_device_event; //!< Metaltronica Buffer
	static event rxData_slot^ canrx_configuration_event; //!< Configuration buffer

	
	delegate void connection_slot(bool status); //!< This is the delegate of the connection handler
	static event connection_slot^ connection_event; //!< Connection event

	// Auto create the can interface at the program start
	canInterface(void); //!< This is the class constructor
	static canInterface^ device = gcnew canInterface();

	static bool isConnected(void) { return connection_status; }
	static void send(unsigned short canId, unsigned char dlen, cli::array<Byte>^ buffer);//!< This is function to send data in the BUS	
	static unsigned short canId;
	static unsigned char datalen;
	static cli::array<Byte>^ canDataBuffer;//!< This is the data buffer decoded

	static void activateConnection(void) { activate = true; }
private:
	static bool activate = false;
	static void threadWork(void);//!< This is the worker thread for the connection management	
	static Thread^ running_thread;//!< This is the worker thread handler
	static Socket^ clientSocket;//!< This is the connected client socket handler
	static IPAddress^ serverAddress ;
	static IPEndPoint^ remoteEndpoint;
	static String^ ipserver;
	static int     portserver;
	static void handleBuffer(void);
	static cli::array<Byte>^ rxBuffer;//!< This is the reception buffer	
	static cli::array<Byte>^ txBuffer;//!< This is the reception buffer	
	static int rc;
	static bool connection_status;//!< This is the current connection status
	

};

