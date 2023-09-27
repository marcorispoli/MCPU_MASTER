/*!
 * \defgroup  Ethenet Module.
 */

/*!
 * \file      Socket.h
 * \brief     It allows to use TCP socket type in both client or server configuration.
 *  		  Provides async events notification for:
 *   			- Data receive
 *   			- Closed connections
 *   			- Accepted clients.
 * 				
 * \author    Juan Miguel Andujar Morgado <juanmiguel.andujar@sedecal.com>.
 * \bug       Unknown bugs
 * \version
 *  \htmlonly
 *		<A HREF="https://svn.sedecal.com/software/Embedded Drivers/Kinetis/ETH/branches/">V1R01.1</A>
 *	\endhtmlonly
 * \date      18/11/2014
 * \remarks   Steps to create and use a socket are:
 *            - Define ACCEPT_TASK (Settings.h) which identifies the task template.
 *            - Define READ_TASK (Settings.h) which identifies the task template.
 *            - Create the class with the callback function defined into MySocket.h, calling to Create function. 
 *            - If you want to make a client call Connect function. However, if you want a server call Accept function.
 *            - To close the socket call to disconnect function.
 *            - When a message is received, it is sent to the callback function defined. 
 *         
 * \pre      The task template for the tasks have to be defined in Settings.h.  
 *            
 * \attention ...
 * \warning   ... 
 * \copyright SEDECAL S.A.
 * \ingroup   Socket Module.
*/

namespace API
{
	namespace RTCS
	{
		/*!
		 * \brief Return value in case of something is wrong
		 */
		#define CASOCKET_ERROR				(-1)
		
		/*!
		 * \brief Default timeout to wait
		 */
		#define TIME_WAIT_CONNECT			15000
		#define TIME_WAIT_ACCEPT			20000
		#define TIME_WAIT_RECEIVE_AND_LOST	20000
		/*!
		 * \brief Default buffer size 
		 */
		#define BUF_SIZE					500
		/*!
		 * \brief Default number of attemps to connect with a remote server 
		 */
		#define NCONNECTATTEMPS				5
		
		/*!
		 * \brief Sockect state
		 */
		enum esocketState{
			NONE=0, 
			DISCONNECTING, 
			CREATED, 
			CONNECTED, 
			ACCEPTING,
			CONNECTED_ACCP, 
		};
		/*!
		 * \brief Possible event when a thread is closed
		 */
		enum easyncEvents {
			EVN_CONNCLOSED=0, 
			EVN_CONNLOST, 
			EVN_SERVERDOWN
		};
		/*!
		 * \brief Threads state
		 */
		enum ethreadState{
			CLOSED=0, 
			CLOSING, 
			RUNNING,
			SLEEPING
		};
		/*!
		 * \brief Class to connect with a remote server and exchange message with external devices using ethernet connectios
		 */
		class CaSocket {
		public:
			
			/*!
			 *  \Brief Type of possible connections
			 *  \remark In case of a standard conenction wants to be done, recomended NONE_DECODER options. 
			 *			Maximum buffer size will be 500 bytes
						However, if R2CP connection is going to be done, use R2CP_DECOCER. Maximum buffer size is 50
			 */
			typedef enum {NONE_DECODER = 0,R2CP_DECODER} etype_decode;
			
			/*!
			 * \Brief		Class Constructor 
			 * \param[in]	deco_type: Type of connection to be done
			 */
			CaSocket( etype_decode deco_type = NONE_DECODER);
			
			/*!
			 * \brief Class Destructor
			 */
			virtual ~CaSocket();
			
			/*!
			 * \brief       Create a TCP socket.
			 * \return	    (true)   if created successfully.
			 * \return      (false)  if an error occurs.
			*/
			short Create(void);
			
			/*! 
			 * \brief	Accepts an incoming connection request.
			 * 			To make a server first call Create and then Accept. This will bind the socket to a local port
			 *  		waiting for connections. TCP will trigger OnAccept giving the
			 *  		possibility to accept or refuse the connection. If accepted you must
			 *          create a new (subclassed) CaSocket class and pass it the service socket
			 *  		with AcceptServiceSocket. The new class will be already connected and
			 *  		can be used to send and receive data.
			 *   		After receiving this data you can use the same socket to send
			 *       	data to the remote client.
			 * \param [in] localPort Local port to bind for incoming requests.
			 * \param [in] maxConn   Maximum length of the queue of pending connections.
			 * \return	    (true)   if created successfully.
			 * \return      (false)  if an error occurs.
			 */
			short Accept(unsigned short localPort, unsigned char maxConn,short timeout_rcv = 0);
			
			/*! 
			 * \brief	Connect to a remote host.
			 * \param[in]  remotePort Remote host port to connect to.
			 * \param [in] remoteaddr  Remote host address. It should be an IP number.
			 */
			short Connect(unsigned short remotePort,int remoteaddr,int rx_Size, int tx_Size,unsigned char nattemps = NCONNECTATTEMPS);
			/*!
			 * \brief Shutdown
			 */
			void Shutdown( void );
			/*!
			 * \brief	Disconnects the socket.
			*/	
			short Disconnect(void);
			/*!
			 * \brief	Send data to a remote host.
			 * \param[in] buf Contains the data to send.
			 * \param[in] len Length of the buffer.
			 * \return Number of bytes sent or SOCKET_ERROR if an error occurs.
			 */
			short Send(unsigned char *buf,unsigned short  len);
			
			/*!
			 * \brief Attaches a connected socket.
			 *  Use this function in conjunction with OnAccept to receive remote client
			 *  connections requests.
			 *  \param[in] serviceSocket This is the socket connected with the remote
			 *                           client received from OnAccept notification.
			 */
			bool 	AcceptServiceSocket( Socket_t serviceSocket = NULL);
			
			/*! 
			 * \brief	Received data notification. If you refused the first OnReceive notification (probably because
			 *  		you didn't implemented it) you'll receive this call. Now you can
			 *  		access to the data through a data access function.
			 * \param[in] buf Contains the data to send.
			 * \param[in] len Length of the buffer.
			 * \return	    (true)   if created successfully.
			 * \return      (false)  if an error occurs. 
			 */
			virtual void OnReceive(unsigned char * buf, int len) = 0;
			
			/*!
			 * \brief	Accepts an incoming connection request.
			 * \param[in] serviceSocket This is the socket that was connected with
			 *              the remote client. Pass it to AcceptServiceSocket.
			 * \return Return FALSE if you don't accept the connection.
			 */
			virtual bool OnAccept( Socket_t serviceSocket ){return false;};
			
			/*!
			 * \brief The socket lost connection or failed.
			 *  You receive this notification if something goes wrong:
			 *
			 *   - You are a client and, for some reason, the read thread failed:
			 *      - TCP: closeEvent = EVN_CONNCLOSED. The socket is disconnected. You must call
			 *        create and connect to open another connection.
			 *   - You are a client and the server whom you are sending data disconnected:
			 *   	- TCP: closeEvent = EVN_CONNCLOSED. The socket is disconnected. You must call
			 *        create and connect to open another connection.
			 *   - You are a server and, for some reasone, accept (TCP) thread failed:
			 *      - TCP: closeEvent = EVN_SERVERDOWN. The socket is disconnected. You must call
			 *        create and accept to start another server.
			 *   - You are a server and the client disconnected:
			 *      - TCP: closeEvent = EVN_CONNCLOSED. Service socket disconnected.
			 *
			 *	 - You will receive OnClose when the remote client disconnected. 
			 *	 This allows you to stop sending data. TCP server never receives OnClose.
			 */
			virtual void OnClose(/*asyncEvents*/unsigned char closeEvent) = 0;
			/*
			 *\brief Notify error during:
			 *	- The bytes to read is greater than buffer size configured
			 */
			virtual void OnError(void) { };
			/*!
			 * \brief	Socket state
			 */
			esocketState	m_socketState;
							
		private:

			/*!
			 * \brief	Private variables and methods
			 */

			#ifdef __MQX__
			/*!
			 * \brief Accept task to work as server
			 */
			friend void AcceptTask(int pParam);
			/*!
			 *	\brief Read Task to parser the incoming messages 
			 */
			friend void ReadTask(int pParam);
			#elif	WIN32
				friend DWORD AcceptTask(LPVOID pParam);
				friend DWORD ReadTask(LPVOID pParam);
			#else
				friend void AcceptTask( void * pParam );
				friend void ReadTask( void *pParam );
			#endif
					
			/*!
			 * \brief Internal socket 
			 */
			Socket_t 				m_sock;
			/*!
			 * \brief Private socket
			 */
			Socket_t 				m_socket_private;
			/*!
			 * \brief Accept task status
			 */
			int 				m_acceptThreadState;
			/*!
			 * \brief Remote address values
			 */
			//OS::sockaddr_str 	m_remoteAddress;
			/*!
			 * \brief Local Address values
			 */
			//OS::sockaddr_str 	m_localAddress;
			/*!
			 * \brief Remote port to listen incoming connections
			 */
			int				m_socket_remote_port;
			/*!
			 * \brief Read thread status
			 */
			ethreadState 		m_readThreadState;
			/*!
			 * \brief Timeout to wait incoming message
			 */
			short				m_timeout_rcv;

			/*!
			 * \brief Decoder types
			 */
			etype_decode		m_type_decoder;
			/*!
			 * \brief Max sizeof incoming messages
			 */
			unsigned short 				m_sizeof_buffer;
			/*!
			 * \brief Handler for read thread
			 */
			OS::thread_handle_t m_read_handle;	/*Accept task*/
#ifdef SOCKET_WATCHDOG_TASK
			/*!
			 * \brief Watchdog Index
			 */
			unsigned char m_Wdog_index;
#endif
			/*!
			 * \brief Allocate pointer for incoming messages
			 */
			unsigned char *mBuffer;
		};
	}//RTCS
}//API

#endif /* SOCKET_H_ */
