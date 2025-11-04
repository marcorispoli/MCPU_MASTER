/*!
 * \defgroup  R2CPModule R2CP Protocol Communication Module.
 */

/*!
 * \file      R2CP.h
 * \brief     Declarations and definitions for the class R2CP.
 *            Class defined to handle a R2CP Protocol.
 * \bug       Unknown bugs
 * \date      01/08/2012
 * \remarks   Steps to create and use R2CP:
 *            - Define local dictionary
 *            - Define the node id of the device 
 *            - Define the target id of the project
 *            - Create the class giving as a callback the funtion to use when R2CP requires sending messages.
 *            - Provide messages got from CAN bus to \ref ProcessMessage function.
 *            - Optionally load \ref HeartBeatTime with a configured value to run Heartbeat functionality.
 *            - Call rest of funtions to send commands.
 * \attention This driver is based on a can physical layer.
 * \copyright SEDECAL S.A.
 * \ingroup   R2CPModule
*/
#ifndef R2CP_ETH_H_
#define R2CP_ETH_H_

#include "CaDataDic.h"

#include <string>

#define SH_IP  "192.6.1.201"
#define SH_PORT  10000
#define SYTEM_MESSAGE_DATABASE  "C:\\OEM\\AppData\\system_messages.sqlite"

static const unsigned short  SH_NODE_ID = 1; //!< Node ID of the Smart Hub
static const unsigned short  GENERATOR_NODE_ID = 6; //!< Generator Node ID
static const unsigned short  SERVICE_TOOL_NODE_ID = 9; //!< Service Tool Node ID

static const unsigned short  APPLICATION_NODE_ID = 17; //!< Node ID of the Application
static const unsigned short  DIRECT_WORKSTATION_ID = 1; 
static const unsigned short  DYNAMIC_WORKSTATION_ID = 2; 


///
///NETWORK SUBINDEX
///
#define II_NETWORK_SS_CONNECTION_CHANGED       1
#define II_NETWORK_SS_NODE_STATUS              2
#define II_NETWORK_SS_HEARTBEAT                3
#define II_NETWORK_SS_CONNECTION_REQUEST	   4

///
///ERROR SUBINDEX COMMAND PROCESSED
///
#define TOKEN_IS_GRANTED						0
#define	ERROR_CODE_TOKEN_ALREADY_GRANTED		201
#define ERROR_MESSAGE_NOT_AVAILABLE				202
#define ERROR_ACCESS_MISMATCH					203

/*!
 * \brief Defines broadcast Node Id according to 
 *		  https://svn.sedecal.com/software/R2CP/Radiological Room Control Protocol - Appendix B - Node IDs.docx
*/
#define	R2CP_ETH_BROADCAST_NODE_ID			0x00

/*!
 * \brief Defines Bootloader Node Id for answers according to 
 *		  https://svn.sedecal.com/software/R2CP/Radiological Room Control Protocol - Appendix B - Node IDs.docx
*/
#define	R2CP_ETH_BOOTLOADER_NODE_ID			0x04

/*!
 * 
 */
#define R2CP_ETH_SMARTHUB_NODE_ID			0x01

/*!
 * \brief Enums kind of protocol functions supported.
*/
typedef enum
{
 	ETH_RESERVED_FNC,
 	ETH_SET_FNC = 1,        
 	ETH_GET_FNC,        
 	ETH_EVENT_ANSWER_FNC,	
 	ETH_BLOCK_FNC = 5,
 	ETH_DOWNLOAD_START		=  9, 
 	ETH_MSG_PROCESSED_FNC 	= 10   
}tEthFunction;


typedef enum{
	II_Generator               = 32,				//(addr = 0X20) 
    II_Positioner              = 64,    		    //(addr = 0x40)
    II_Dosimeter               = 96,               	//(addr = 0x60)
    II_System                  = 128,              	//(add  = 0x80)
    II_Network                 = 160,              	//(addr = 0xA0)        
    II_Service                 = 192,              	//(addr = 0xC0)
    II_PatientWorkflow         = 224,              	//(addr = 0xE0) 
    II_Workflow                = 240,              	//(add =  0xF0)
}tIndex;


typedef struct socket_header{
	unsigned char sequence 	: 6;
	unsigned char priority 	: 2;
	unsigned char dest_node	: 8;
	unsigned char iss_node 	: 8;
	unsigned char index 		: 8;
	unsigned char subindex 	: 8;
	unsigned char function 	: 8;
	unsigned char lenh 		: 8; // Big Endian on Ethernet
    unsigned char lenl      : 8;
}tSocket_Header,*tSocket_Header_ptr;

/*!
 * \brief Enums kind of message priorities supported.
*/
typedef enum
{
	ETH_LOWEST_PRIORITY,
	ETH_LOW_PRIORITY,
	ETH_HIGH_PRIORITY,
 	ETH_HIGHEST_PRIORITY
}eEthPriorities;

#define HEADER_SIZEOF			8
#define MAX_LENGTH_BUFFER_ETH	100


#define	MAX_SEQ_ALLOCATED	10

typedef struct{		
	tSocket_Header 	header;
	unsigned char 	data[ MAX_LENGTH_BUFFER_ETH ];
}tSocket_Msg,*tSocket_Msg_ptr;


#define MAX_BLOCK_ETH_SIZE	300



/*!
 * \brief Class to handle the protocol
*/
class CR2CP_Eth
{
  public:
    /*!
     * \brief       Constructor of the class.
     * \param [in]  cbSendMsgEth			Callback to send eth messages. It must return:
     *										- false			If succesful
	 *              						- true			If not 
     * \param [in]  cbGetDictionary			Function to get the right dictionary according to the node  It must return:
     *										- pNULL			If not available
	 *              						- CaDataDic*	Pointer to the dictionary
    */												   
  	CR2CP_Eth(	short (*cbSendMsgEth)( unsigned char *pMessage , unsigned short  datalength ), 
  				CaDataDic* ( *cbSearchDatadicSlave )(unsigned char Node),
				CaDataDic* pEthDataDic,
				void (*cbJumpToBootloader)(void) ,
				unsigned short  targetId );
   	/*!
     * \brief       Destructor of the class.
    */
  	~CR2CP_Eth();
  	
    bool smartHubConnected;
    bool generatorConnected;
    bool serviceToolConnected;
  	
  	
   	/*!
     * \brief       Scans/Processes a message given. It will check the format according to protocol defination.
     * \param [in]  pMessage        Can message.
    */
    void ProcessMessage(void *pMessage);
    
   	/*!
     * \brief       Sends a SET command.
     * \param [in]  nodeId        	Node id.
     * \param [in]	index           Index field of the dictionary object.
     * \param [in]	subindex        SubIndex field of the dictionary object.
     * \param [in]	dataLength      Data lenght.
     * \param [in]	pdata           Datas of the dictionary object.
	 * \return      - false			If succesful
	 *              - true			If not
    */
    bool Set(eEthPriorities prio , unsigned char node_dest, unsigned char node_iss, unsigned char index, unsigned char subindex, unsigned char dataLength, unsigned char *pdata);
   	/*!
     * \brief       Sends a GET command.
     * \param [in]  nodeId        	Node id.
     * \param [in]	index           Index field of the dictionary object.
     * \param [in]	subindex        SubIndex field of the dictionary object.
     * \param [in]	dataLength      Data lenght.
     * \param [in]	pdata           Datas of the dictionary object.
	 * \return      - false			If succesful
	 *              - true			If not
    */
    bool Get(eEthPriorities prio , unsigned char node_dest, unsigned char node_iss, unsigned char index, unsigned char subindex, unsigned char dataLength, unsigned char *pdata);
   	/*!
     * \brief       Sends a EVENT command.
     * \param [in]  nodeId          Node id.
     * \param [in]	index           Index field of the dictionary object.
     * \param [in]	subindex        SubIndex field of the dictionary object.
     * \param [in]	dataLength      Data lenght.
     * \param [in]	pdata           Datas of the dictionary object.
	 * \return      - false			If succesful
	 *              - true			If not
    */
    bool Event_Answer(eEthPriorities prio , unsigned char node_dest, unsigned char node_iss, unsigned char index ,unsigned char subindex, unsigned short  dataLength, unsigned char *pdata);
    
    /*!
     * \brief       Sends a MSG_PROCESSED_FNC command.
     * \param [in]  nodeId          Node id.
     * \param [in]	index           Index field of the dictionary object.
     * \param [in]	subindex        SubIndex field of the dictionary object.
     * \param [in]	dataLength      Data lenght.
     * \param [in]	pdata           Datas of the dictionary object.
	 * \return      - false			If succesful
	 *              - true			If not
    */
    bool Processed(eEthPriorities prio , unsigned char node_dest , unsigned char node_iss, unsigned char index, unsigned char subindex, unsigned char dataLength, unsigned char *pdata );
    /*!
     * 
     */
    bool WrongAccess(eEthPriorities prio , unsigned char node_dest, unsigned char node_iss, unsigned char index, unsigned char subindex, unsigned char dataLength, unsigned char *pdata);    
    
    /*!
     * \param [in]  prio            Priority field of frame id.
     * \param [in]	ack             Handshake field of frame id.
     * \param [in]	node            Node id field of frame id.
     * \param [in]	index           Index field of frame id.
     * \param [in]	subindex        SubIndex field of frame id.
     * \param [in]	dataLength      Data lenght.
     * \param [in]	pdata           Datas of the frame.
	 * \return      - false			If succesful
	 *              - true			If n
    */
    bool Reserved(eEthPriorities prio , unsigned char node_dest, unsigned char node_iss, unsigned char index, unsigned char subindex, unsigned char dataLength, unsigned char *pdata);
    
    /*!
     * \brief		Use this method when the data size is higher than 50 bytes to avoid dynamic memory allocation
     * \param [in]  prio            Priority field of frame id.
     * \param [in]	node_dest       Node id Destine field of frame id.
     * \param [in]	node_iss       	Node id Origin field of frame id.
     * \param [in]	index           Index field of frame id.
     * \param [in]	subindex        SubIndex field of frame id.
     * \param [in]  function		Function field of frame id.
     * \param [in]	dataLength      Data lenght.
     * \param [in]	pdata           Data pointer of the frame.
	 * \return      - false			If succesful
	 *              - true			If n
    */
    bool SendRawData(eEthPriorities prio , unsigned char node_dest, unsigned char node_iss, unsigned char index ,unsigned char subindex, unsigned char function ,unsigned short  dataLength, unsigned char *pdata);
    

    static void Callback_token_expired(unsigned char Node);

    bool isCommandProcessed(){return commandProcessed;};
    unsigned char getAssignedSequence(){return assignedSequence;};
    unsigned char getCommandProcessedResult(){return commandProcessedResult;};
    std::string getCommandProcessedString(void) {return commandProcessedString;};
    void    evalCommadProcessedResult(void);
    void    evalSetGeneratorProcessedResult(void);
    void    evalSetPatientProcessedResult(void);
    void    evalSetSystemProcessedResult(void);
    void    evalGetGeneratorProcessedResult(void);
    void    evalGetPatientProcessedResult(void);
    void    evalGetSystemProcessedResult(void);

  private:
    /*!
     * \brief
     */
    unsigned char Get_Sequence(void);
         	  
    /*!
     * \brief       Stores callback given in constructor to send can messages.
    */
  	short (*mCbSendMsgEth)(unsigned char * pMessage, unsigned short  datalength);		
    
  	/*!
     * \brief       Stores callback given in constructor to jump to bootloader when an update is required.
    */
    void (*mCbJumpToBootloader)(void);
    
    /*!
     * \brief	Stores callback given in constructor to get the slave dicctionary
     */
    CaDataDic* (*mCbSearchDatadicSlave)( unsigned char Node );

   	/*!
     * \brief       Assembles the protocol frame and sends it.
     * \param [in]  prio            Priority field of frame id.
     * \param [in]	ack             Handshake field of frame id.
     * \param [in]	node            Node id field of frame id.
     * \param [in]	index           Index field of frame id.
     * \param [in]	subindex        SubIndex field of frame id.
     * \param [in]	fnc             Function field of frame id.
     * \param [in]	dataLength      Data lenght.
     * \param [in]	pdata           Datas of the frame.
	 * \return      - false			If succesful
	 *              - true			If not
	 */             
    bool ProcessWorkflow(unsigned char priority, unsigned char node_dest, unsigned char node_iss, unsigned char index, unsigned char subindex, unsigned char function, unsigned char dataLength, unsigned char *pdata);
 	/*!
     * \brief       Assembles the protocol frame and sends it.
     * \param [in]  prio            Priority field of frame id.
     * \param [in]	ack             Handshake field of frame id.
     * \param [in]	node            Node id field of frame id.
     * \param [in]	index           Index field of frame id.
     * \param [in]	subindex        SubIndex field of frame id.
     * \param [in]	fnc             Function field of frame id.
     * \param [in]	dataLength      Data lenght.
     * \param [in]	pdata           Datas of the frame.
	 * \return      - false			If succesful
	 *              - true			If not
    */
    bool ProtFunction( eEthPriorities prio ,unsigned char node_dest ,unsigned char node_iss , unsigned char index, unsigned char subindex, tEthFunction fnc, unsigned short  dataLength, unsigned char *pdata);
    
   	/*!
     * \brief       Processes BLOCK command received.
     *              It stores pieces of the block until completing it. After that, provide object to dictionary.
     * \param [in]  sender          Node id.
     * \param [in]	index           Index field of the dictionary object.
     * \param [in]	subindex        SubIndex field of the dictionary object.
     * \param [in]	dataLength      Data lenght.
     * \param [in]	pdata           Datas of the dictionary object.
	 * \return      - false			If succesful
	 *              - true			If not
    */
    bool ProcessBlock( CaDataDic * pDataDic, unsigned char index, unsigned char subindex, unsigned char dataLength, unsigned char *pdata, tInfoMessage *infomessage = nullptr ); 
    
    /*!
     * \brief
     */
	unsigned char m_TimerNodeToken_assigned;
    
	/*!
	 * 
	 */
	static unsigned char m_token_granted;
	/*!
	 * 
	 */
	unsigned char m_sequence;

    /*!
     *
     */
    unsigned char assignedSequence;
    bool    commandProcessed;
    bool    isCpGet;
    unsigned char commandProcessedResult;
    unsigned char commandProcessedIndex;
    unsigned char commandProcessedSubIndex;
    std::string commandProcessedString;


	/*!
	 * 
	 */
	unsigned short  mTargetId;
	/*!
	 * 
	 */
	CaDataDic *mCbAuxDataDicToDownloader;
	/*!
	 * 
	 */
	CaDataDic* mpEthDatadic;
	
    /*!
     * \brief       Stores params to manage a download of a block.
    */
    struct
    {
		unsigned short  nBlockDataExpected;
		unsigned short  nCurrentBlockData;
		unsigned char BlockDataSequenceExpected;
		unsigned char *pBlockData;
		unsigned char BlockAccess;
		tInfoMessage infomessage;
    }mBlockManager;
};

#endif /* R2CP_ETH_H_ */
