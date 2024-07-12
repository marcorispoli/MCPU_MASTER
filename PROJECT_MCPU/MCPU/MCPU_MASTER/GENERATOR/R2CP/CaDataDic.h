
#ifndef _CADATADIC_H
#define _CADATADIC_H


/*!
 * \brief Defines the broadcast node for the r2cp protocol
 */
#define BROADCAST_NODE_ID	0


/*!
 * \brief Enums kind of access to dictionary parameters. It can be a combination of each of them
*/
typedef enum {
	DATADIC_ACCESS_SET				= 0x01,
	DATADIC_ACCESS_GET				= 0x02,
	DATADIC_ACCESS_SET_GET			= 0x03,
	DATADIC_ACCESS_ANSWER			= 0x04,
	DATADIC_ACCESS_ANSWER_EVENT		= 0x04,
	DATADIC_ACCESS_EVENT			= 0x08,
	DATADIC_ACCESS_NOT_AVAILABLE 	= 1 << 6,
	DATADIC_ACCESS_NOT_MISMATCH		= 1 << 7,
	DATADIC_ACCESS_ALL_MEMBERS		= 0xFF
}tDataDicAccess;

class CaDataDic;

/*!
 *\brief Message Aditional Information  
 */
typedef struct{
	unsigned char 			Node_dest;
	unsigned char 			Node_own;
	unsigned char 			Sequence;
	unsigned char 			Function;
	unsigned char 			Priority;
	unsigned char 			SubIndex;
	CaDataDic		*pDataDic;
}tInfoMessage;

/*!
 * \brief       Prototype function for an entry of a dictionary object.
 * \param [in]	Function  Kind of access to the parameter/entry.
 * \param [in]  pData     Data.									 
 * \param [in]  pData     Data Size.
*/
typedef void (*pfvoidDataDic) (tDataDicAccess, unsigned char *, unsigned short  , tInfoMessage *);

/*!
 * \brief Defines lenght for entries which lenght is variable.
*/
#define UNDEFINED_LENGHT        (unsigned char )-1 
#define SUBINDEX_NOT_FOUND		-1

/*!
 * \brief 
 */
#define DATADIC_HEARTBEAT_KEYWORD			0x55

/*!
 * \brief Struct which defines an entry/parameter of a dictionary object.
*/
typedef struct {
	unsigned char            SubIndex;
	pfvoidDataDic   pFunction;		/*!< Function pointer to deal the access */
	tDataDicAccess	AccessType;		/*!< Kind of access allowed*/
	unsigned char            GetDataSize;	/*!< Data size of the GET access */
	unsigned char            SetDataSize;	/*!< Data size of the SET access */
	unsigned char            AnswerDataSize;	/*!< Data size of the ANSWER access */
	unsigned char            EventDataSize;	/*!< Data size of the EVENT access */
} tEntry;

/*!
 * \brief Struct which defines a row for an object dictionary.
*/
typedef struct {
	unsigned char            Index;
	unsigned char            nSubIndexes;
	const tEntry	*pEntries;
} tRow;

/*!
 * \brief Enums R2CP status.
*/

typedef enum{
  SLAVE_INITIALIZING,	/*!< Initializing. */
  SLAVE_READY,			/*!< PCB already initialized and without errors. */
}eStatus;

/*!
 * \brief Struct to define Hardware version field.  Format: A3616-01-0
*/
typedef struct {
	char Revision;	/*!< Hw revision. */
	unsigned char Version;	/*!< Hw version. */
	unsigned short  Model;		/*!< Hw model version. */
} tHwVersion;

/*!
* \brief Struct to define Software version field. Format: V1R01.1
*/
typedef struct{
	unsigned char Subrevision;	/*!< Sw subrevision. */
	unsigned char Revision;		/*!< Sw revision. */
	unsigned char Version;		/*!< Sw version. */
} tSwVersion;

/*!
 * \brief Struct which defines an object dictionary.
*/
typedef struct {
	unsigned char 			nIndexes;
	const tRow		*pRows;
} tTable;

/*!
 * \brief Struct to define Hardware version field.  Format: A3616-01-0
*/
typedef struct {
	unsigned short  Model;		/*!< Hw model version. */
	unsigned char Version;	/*!< Hw version. */
	char Revision;	/*!< Hw revision. */
} tDataDicHwVersion;

/*!
* \brief Struct to define Software version field. Format: V1R01.1
*/
typedef struct{
	unsigned char Version;		/*!< Sw version. */
	unsigned char Revision;		/*!< Sw revision. */
	unsigned char Subrevision;	/*!< Sw subrevision. */
} tDataDicSwVersion;

/*!
 * \brief Struct to define protocol version field. Format: V1.1 R A
*/
typedef struct{
  unsigned char Version;
  unsigned char SubVersion;
  char Revision;
} tDataDicProtocolVersion;

/*!
 * \brief Enums states.
*/
typedef enum{
  DATADIC_STATUS_INITIALIZING,	/*!< Initializing. */
  DATADIC_STATUS_READY			/*!< Already initialized and without errors. */
}eDataDicStatus;

/*!
 * \brief Enums common working modes.
*/
typedef enum{
  DATADIC_MODE_NORMAL,	/*!< Normal Operation Mode. */
  DATADIC_MODE_SAFETY,	/*!< Safety Mode. */
  DATADIC_MODE_SERVICE,	/*!< Service Mode. */
  DATADIC_MODE_NONE
}eDataDicModes;

/*!
 * \brief Defines status register.
*/
typedef struct
{
   unsigned char status				:1;
   unsigned char modes				:2;
   unsigned char unused				:1;
   unsigned char heartbeatFlag		:1;
   unsigned char errorFlag			:1;
   unsigned char bootingType			:1;
   unsigned char reserved			:1;
}tDataDicStatusRegister;



	
/*!
 * \brief Class to handle any R2CP dictionary
*/
class CaDataDic
{
	
	public:
		/*!
		 * \brief Constructor of the class.
		*/
		CaDataDic( unsigned char Node );
		
		/*!
		 * \brief Destructor of the class.
		*/
    	virtual ~CaDataDic();
				
		/*!
		 * \brief Stores Hardware version field.  Format: A3616-01-0
		*/
		tDataDicHwVersion mHwVersion;
		
		/*!
		 * \brief Stores Software version field. Format: V1R01.1
		*/
		tDataDicSwVersion mSwVersion;
		
		/*!
		 * \brief Stores Bootloader version field. Format: V1R01.1
		*/
		tDataDicSwVersion mBootVersion;
		
		/*!
		 * \brief Stores R2CP status register.
		*/
		tDataDicStatusRegister mStatusRegister;
		
		/*!
		 * \brief Stores protocol version field. Format: V1.1 R A
		*/
		tDataDicProtocolVersion mProtocolVersion;
		
		/*!
		 * \brief Stores protocol version field. Format: V1.1 R A
		*/
		char *m_pPackageVersion;
		
		/*!
		 * \brief Stores pipe node id.
		*/				  
		unsigned char mPipeSubNodeId;
		
		/*!
		 * \brief Stores pipe interface number.
		*/				  
		unsigned char mPipeInterfaceNumber;
		
		/*!
		 * \brief Stores serial number of the node/equipment.
		*/				  
		char *m_pSerialNumber;
		/*!
		 * \brief Stores a short description of a node.
		*/				  
		char *m_pDescription;
		
		/*!
		 * \brief Stores heartbeat time out. Units (mS).
		*/
		unsigned short  mHeartBeatTime;
		
		/*!
		 * \brief Stores R2CP node id of the dictionary. 0 is reserved for broadcast and 4 for HCS12 bootloader.
		*/				  
		unsigned char mNodeId;
		
		/*!
		 * \brief Checks access to the dictionary and calls related function according to parameters passed.
		 * \param [in]	Index		Index of the dictionary entry.												 
		 * \param [in]	SubIndex	Subindex of the dictionary entry.
		 * \param [in]	Node		Node Id of the dictionary.
		 * \param [in]	pData		Data for the access.
		 * \param [in]	Size		Data size for the access.
		 * \param [in]  infomessage In case any extra param has to be passed to the dictionary
		 * \return      - false   If it was succesful
		 *              - true    If not
		*/
		unsigned char DataDicCheckAndCall( unsigned char Index,  unsigned char SubIndex, unsigned char Node, tDataDicAccess Access, unsigned char *pData, unsigned short  Size, tInfoMessage *infomessage = nullptr );
		
		/*!
		 * \brief Function to know if the message is available for the current state
		 * \param[in]	Index		Dictionary Index
		 * \param[in]   SubIndex	Dictionary SubIndex
		 * \return 		Accessible True; Otherwise False
		 */
		virtual bool Check_NotAvailable_Message_vs_Status(unsigned short  index, unsigned short  subindex, tInfoMessage *MessageInfo) { return true; }

		/*!
		 * \brief Called by R2CP protocol after getting HEARTBEAT function message..
		 * \param [in]	Node		Node Id of the dictionary.
		 * \param [in]	keyWord		Keyword of the HEARTBEAT message got.
		 * \param [in]	status		Status of the node which is carried on the HEARTBEAT message got.
		 * \return      - false   If it was succesful
		 *              - true    If not
		*/
		virtual bool HeartbeatGot(unsigned char node, unsigned char keyWord, unsigned char status)=0;
	
		/*!
		 * \brief
		 * \param [in] target_id	Keyword to know if the firmware is adjusted to the target board
		 * \param [in]	pData		Data for the access.
		 * param [in]	Size		Data size for the access.
		 * \return      - false   If it was succesful
		 *              - true    If not	
		 */
		virtual bool DownloadGot( unsigned short  target_id , unsigned char *pData , unsigned short  Size ) { return false; }
				
		/*!
		 * \brief 
		 * \param 
		 */
		//virtual bool ReservedGot( unsigned char Priority, unsigned char Node , unsigned char Index , unsigned char SubIndex , unsigned char *pData , unsigned short  Size ) = 0;		
			
	protected:
		/*!
		 * \brief Mutex pointer
		 */
        //OS::mutex_handle_t datadic_mutex;
		/*!
		 * \brief Stores entries of the dictionary.
		*/				  
		const tTable *pDataDictionary;
};

#endif
