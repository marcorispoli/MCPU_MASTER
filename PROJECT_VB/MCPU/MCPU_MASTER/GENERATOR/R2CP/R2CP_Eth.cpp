/*!
 * \brief     Implementations for the class R2CP.
 * \author    Francisco Javier Sanchez Rabadán <francisco.sanchez@sedecal.com>.
 * \ingroup   R2CPModule
*/
#include "R2CP_Eth.h"
#include "R2CP_Index.h"

#include <string>

#define HB(x) (unsigned char) (x >> 8)
#define LB(x) (unsigned char) (x & 0xFF)
#define BIGENDIAN(h,l) (unsigned short) ((unsigned short) h * 256 + (unsigned short) l) 
/*!
 * 
 */
unsigned char CR2CP_Eth::m_token_granted;


///
///TIME TOKEN & HEARTBEAT
///
#define TIME_RELEASE_TOKEN		10000//token_release

///
///WORKFLOW SUBINDEX
///
#define II_WORKFLOW_SS_TOKEN	30

///
///TOKEN WORKFLOW SUBINDEX
///
#define TOKEN_REQUEST_ID	0	
#define	TOKEN_RELEASE_ID	1
#define TOKEN_REFRESH_ID	2


CR2CP_Eth::CR2CP_Eth(	short (*cbSendMsgEth)( unsigned char *pMessage , unsigned short  datalength ), 
						CaDataDic* ( *cbSearchDatadicSlave )(unsigned char Node),
						CaDataDic* pEthDataDic,
						void (*cbJumpToBootloader)(void) ,
						unsigned short  targetId)
{
	mCbSendMsgEth 				= 	cbSendMsgEth;
	mCbJumpToBootloader 		= 	cbJumpToBootloader; 
	mCbSearchDatadicSlave		= 	cbSearchDatadicSlave;
	m_token_granted 			= 	R2CP_ETH_BROADCAST_NODE_ID;
	mTargetId 					= 	targetId;
	mCbAuxDataDicToDownloader 	= 	nullptr;
	mpEthDatadic				= 	pEthDataDic;
			
	mBlockManager.nBlockDataExpected = 0;
	mBlockManager.nCurrentBlockData  = 0;
	mBlockManager.BlockDataSequenceExpected = 0;
	mBlockManager.pBlockData = nullptr;
	mBlockManager.BlockAccess = 0;
	mBlockManager.infomessage.Node_dest=0;
	mBlockManager.infomessage.Node_own=0;
	mBlockManager.infomessage.Sequence=0;
	mBlockManager.infomessage.Function=0;
	mBlockManager.infomessage.Priority=0;
	smartHubConnected = false;
	generatorConnected = false;
	serviceToolConnected = false;

}

CR2CP_Eth::~CR2CP_Eth()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: ProcessMessage
// Decode a message
//
void CR2CP_Eth::ProcessMessage(void *pMessage)
{
	if( mpEthDatadic == nullptr )
		return;
		
	tSocket_Msg_ptr pEthMsg = reinterpret_cast< tSocket_Msg_ptr >( pMessage );
	
	switch(pEthMsg->header.function)
	{
	case ETH_RESERVED_FNC:
		return;
		break;
	case ETH_DOWNLOAD_START:
		return;
		break;
    case ETH_MSG_PROCESSED_FNC:

            if(BIGENDIAN(pEthMsg->header.lenh, pEthMsg->header.lenl) != 2) return;
            if(pEthMsg->data[0] != assignedSequence) return;

            commandProcessed = true;
            commandProcessedResult = pEthMsg->data[1];
            if(commandProcessedResult) evalCommadProcessedResult();

            return;
        break;
	default:
		break;
	}
	
	switch(pEthMsg->header.index)
	{
		case II_Workflow:
			this->ProcessWorkflow(static_cast<eEthPriorities>(	pEthMsg->header.priority ),
																pEthMsg->header.iss_node,
																pEthMsg->header.dest_node,
																pEthMsg->header.index,
																pEthMsg->header.subindex,
																pEthMsg->header.function,
																(unsigned char) BIGENDIAN(pEthMsg->header.lenh, pEthMsg->header.lenl),
																pEthMsg->data);
			return;
			break;
		default:
			break;
	}
	
	if( m_token_granted == R2CP_ETH_BROADCAST_NODE_ID ||  m_token_granted == pEthMsg->header.iss_node )//ELSE The token is given to another node
	{		
		if( pEthMsg->header.dest_node == R2CP_ETH_BROADCAST_NODE_ID )
			pEthMsg->header.dest_node = mpEthDatadic->mNodeId;
		
		// CHECKING FUNCTION   
		switch(pEthMsg->header.function)
		{
			case ETH_SET_FNC:
			case ETH_GET_FNC:
			case ETH_EVENT_ANSWER_FNC:
			{	//crosses initialization
				///
				///Give an id in case a thread is launched by the dictionary
				///This id is placed in the last argument of the data message
				///
				tInfoMessage infomessage={ pEthMsg->header.iss_node , pEthMsg->header.dest_node , pEthMsg->header.sequence , pEthMsg->header.function , pEthMsg->header.priority , pEthMsg->header.subindex , nullptr };	
				// Checking right access and calling dictionary
				unsigned char rval = mpEthDatadic->DataDicCheckAndCall(	pEthMsg->header.index,
																pEthMsg->header.subindex,
																pEthMsg->header.dest_node,
																(tDataDicAccess)(0x01<<(pEthMsg->header.function - 1)),
																pEthMsg->data,
																BIGENDIAN(pEthMsg->header.lenh, pEthMsg->header.lenl),// NEED TO EXCHANGE THE BYTES	
																&infomessage);
				if( rval == 0)
				{
					return; // Right access
				}
				//Si recibimos un evento, no enviar nunca un cp wrong access
				if( pEthMsg->header.function == ETH_EVENT_ANSWER_FNC )
					break;
				
				unsigned char data[]={ pEthMsg->header.sequence , rval};
				(void)Processed(	static_cast<eEthPriorities>(pEthMsg->header.priority),
									pEthMsg->header.iss_node,
									pEthMsg->header.dest_node,
									pEthMsg->header.index,
									pEthMsg->header.subindex,
									0x02,
									data);
			}
			
			break;
			case ETH_BLOCK_FNC:
			{
				tInfoMessage infomessage;

		    	infomessage.Node_dest	= pEthMsg->header.dest_node;
		    	infomessage.Node_own	= pEthMsg->header.iss_node;
		    	infomessage.Sequence	= pEthMsg->header.sequence;
		    	infomessage.Function	= pEthMsg->header.function;
		    	infomessage.Priority	= pEthMsg->header.priority;
			    
			    // Making the block
			    if( ProcessBlock( mpEthDatadic , pEthMsg->header.index, pEthMsg->header.subindex, BIGENDIAN(pEthMsg->header.lenh, pEthMsg->header.lenl), pEthMsg->data, &infomessage) )
			    {
					unsigned char data[]={pEthMsg->header.sequence,ERROR_MESSAGE_NOT_AVAILABLE};
					(void)Processed(static_cast<eEthPriorities>(pEthMsg->header.priority),
									pEthMsg->header.iss_node,
									pEthMsg->header.dest_node,
									pEthMsg->header.index,
									pEthMsg->header.subindex,
									0x02,
									data);
			    }
			}
			break;
			default:
			break;
		}
	}
	else{
		Processed(	static_cast<eEthPriorities>(pEthMsg->header.priority), 
					pEthMsg->header.iss_node, 
					pEthMsg->header.dest_node,
					pEthMsg->header.index, 
					pEthMsg->header.subindex, 
					0x01, 
					reinterpret_cast<unsigned char *>(ERROR_CODE_TOKEN_ALREADY_GRANTED));
		return;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Reception of blocks. 
//
bool CR2CP_Eth::ProcessBlock(CaDataDic * pDataDic, unsigned char index, unsigned char subindex, unsigned char dataLength, unsigned char *pdata, tInfoMessage * pInfoMessage)
{
	bool result = false;

	if( dataLength == 0 || pdata == nullptr )
	{
        mBlockManager.nBlockDataExpected = 0;
        mBlockManager.nCurrentBlockData = 0;
        mBlockManager.BlockDataSequenceExpected = 0;
        mBlockManager.BlockAccess = 0;
    	mBlockManager.infomessage.Node_dest=0;
    	mBlockManager.infomessage.Node_own=0;
    	mBlockManager.infomessage.Sequence=0;
    	mBlockManager.infomessage.Function=0;
    	mBlockManager.infomessage.Priority=0;
    	
        if( mBlockManager.pBlockData != nullptr )
        {

          memset( mBlockManager.pBlockData , 0 , MAX_BLOCK_ETH_SIZE );
        }
        return true;
	}
		
	switch( pdata[0] )
	{
		case 0xFE:
			
			result = true;	
			if( dataLength == 4 )
			{
				mBlockManager.nBlockDataExpected = (unsigned short )pdata[1]<<8 | (unsigned short )pdata[2];
				
				// Checking function command   
				switch(	pdata[3] )
				{
					case ETH_SET_FNC:
					case ETH_GET_FNC:
					case ETH_EVENT_ANSWER_FNC:
				    	mBlockManager.infomessage.Node_dest=0;
				    	mBlockManager.infomessage.Node_own=0;
				    	mBlockManager.infomessage.Sequence=0;
				    	mBlockManager.infomessage.Function=0;
				    	mBlockManager.infomessage.Priority=0;
				    	
						mBlockManager.BlockAccess = (tDataDicAccess)(0x01<<(pdata[3]-1));
						if( mBlockManager.pBlockData != nullptr )
						{

							memset( mBlockManager.pBlockData , 0 , MAX_BLOCK_ETH_SIZE );
						}
						if( mBlockManager.pBlockData == nullptr )
						{
							mBlockManager.pBlockData = new unsigned char [ MAX_BLOCK_ETH_SIZE ];
						}
						
						if( mBlockManager.pBlockData != nullptr )
						{
							result = false;
						}
						else
						{
							//TODO: Gpio.AddError( ERROR_MEMORY);
						}
						break;
				}
			}
			
			if( result )
			{         
				//(void)WrongAccess(pDataDic->mNodeId,index,subindex,0, (unsigned char *)pNULL);
			}
			mBlockManager.nCurrentBlockData = 0;
			mBlockManager.BlockDataSequenceExpected = 0;
			break;
			
		case 0xFF:
			if( dataLength == 1 && mBlockManager.nBlockDataExpected == mBlockManager.nCurrentBlockData )
			{
				mBlockManager.infomessage = *pInfoMessage;
				// Checking right access and calling dictionary
				if( pDataDic )
				{
					bool result = pDataDic->DataDicCheckAndCall(		index,
							subindex,
							mBlockManager.infomessage.Node_dest,
							(tDataDicAccess)mBlockManager.BlockAccess,
							mBlockManager.pBlockData,
							mBlockManager.nCurrentBlockData,
							&mBlockManager.infomessage);
					if( !result )
					{
						mBlockManager.nBlockDataExpected = 0;
						mBlockManager.nCurrentBlockData = 0;
						mBlockManager.BlockDataSequenceExpected = 0;
						mBlockManager.BlockAccess = 0;
						if( mBlockManager.pBlockData != nullptr )
						{

						  memset( mBlockManager.pBlockData , 0 , MAX_BLOCK_ETH_SIZE );
						}
						mBlockManager.infomessage.Node_dest=0;
						mBlockManager.infomessage.Node_own=0;
						mBlockManager.infomessage.Sequence=0;
						mBlockManager.infomessage.Function=0;
						mBlockManager.infomessage.Priority=0;
						return false;
					}
				}
			}
	        //(void)WrongAccess(pDataDic->mNodeId,index,subindex,0, (unsigned char *)pNULL);
			result = true;
			break;
			
		default:
			if( dataLength > 1 &&
				pdata[0] == mBlockManager.BlockDataSequenceExpected &&
				mBlockManager.nCurrentBlockData < mBlockManager.nBlockDataExpected )
			{
				(void)memcpy(&mBlockManager.pBlockData[mBlockManager.nCurrentBlockData],&pdata[1],dataLength - 1);
				mBlockManager.nCurrentBlockData += dataLength - 1;
				mBlockManager.BlockDataSequenceExpected++;
			}
			else{
				result = true;
			} 
			break;
	}
    
	if( result )
	{
        mBlockManager.nBlockDataExpected = 0;
        mBlockManager.nCurrentBlockData = 0;
        mBlockManager.BlockDataSequenceExpected = 0;
        mBlockManager.BlockAccess = 0;
        if( mBlockManager.pBlockData != nullptr )
        {

          memset( mBlockManager.pBlockData , 0 , MAX_BLOCK_ETH_SIZE );
        }
    	mBlockManager.infomessage.Node_dest=0;
    	mBlockManager.infomessage.Node_own=0;
    	mBlockManager.infomessage.Sequence=0;
    	mBlockManager.infomessage.Function=0;
    	mBlockManager.infomessage.Priority=0;
	}
  
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: ProtFunction
// Makes and sends CAN message
//
bool CR2CP_Eth::ProtFunction( eEthPriorities prio ,unsigned char node_dest ,unsigned char node_iss , unsigned char index, unsigned char subindex, tEthFunction fnc, unsigned short  dataLength, unsigned char *pdata)
{
    /*
	if( mCbSendMsgEth == nullptr )
	{
		return true;
	} 
    */
	if( dataLength > MAX_LENGTH_BUFFER_ETH )
	{

		return true;
	}
	
	tSocket_Msg Eth_Msg = { 0 };
	
    Eth_Msg.header.sequence 	= Get_Sequence();
	Eth_Msg.header.priority 	= prio;
	Eth_Msg.header.dest_node 	= node_dest;
	Eth_Msg.header.iss_node		= node_iss;
	Eth_Msg.header.index		= index;
	Eth_Msg.header.subindex		= subindex;
	Eth_Msg.header.function		= fnc;
	Eth_Msg.header.lenh			= HB(dataLength);
	Eth_Msg.header.lenl			= LB(dataLength);

    if((fnc == ETH_SET_FNC) || (fnc == ETH_GET_FNC)){
        assignedSequence = Eth_Msg.header.sequence;
        commandProcessed = false;
        commandProcessedResult = 0;
        commandProcessedIndex = index;
        commandProcessedSubIndex = subindex;

        if(fnc == ETH_GET_FNC) isCpGet = true;
        else isCpGet = false;
    }

 	if( pdata )
 	{
 		memcpy( Eth_Msg.data , pdata , dataLength );
 	}

 	return mCbSendMsgEth( reinterpret_cast<unsigned char *>(&Eth_Msg), HEADER_SIZEOF + dataLength );
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Set
// Sends SET message
//
bool CR2CP_Eth::Set(eEthPriorities prio , unsigned char node_dest, unsigned char node_iss, unsigned char index, unsigned char subindex, unsigned char dataLength, unsigned char *pdata)
{  
	return ProtFunction(prio, node_dest, node_iss , index, subindex, ETH_SET_FNC , dataLength, pdata);  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Get
// Sends GET message
//
bool CR2CP_Eth::Get(eEthPriorities prio , unsigned char node_dest, unsigned char node_iss, unsigned char index, unsigned char subindex, unsigned char dataLength, unsigned char *pdata)
{   
  return ProtFunction(prio, node_dest, node_iss , index, subindex, ETH_GET_FNC , dataLength, pdata);  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Event_Answer
// Sends EVENT and Answer message
//
bool CR2CP_Eth::Event_Answer(eEthPriorities prio , unsigned char node_dest, unsigned char node_iss, unsigned char index ,unsigned char subindex, unsigned short  dataLength, unsigned char *pdata)
{
	return ProtFunction(prio, node_dest , node_iss , index , subindex , ETH_EVENT_ANSWER_FNC , dataLength , pdata);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Processed
// SEND PROCESSED MESSAGE
//
bool CR2CP_Eth::Processed(eEthPriorities prio , unsigned char node_dest, unsigned char node_iss, unsigned char index, unsigned char subindex, unsigned char dataLength, unsigned char *pdata)
{
	return ProtFunction( prio, node_dest , node_iss , index , subindex , ETH_MSG_PROCESSED_FNC , dataLength , pdata );	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: WrongAccess
//
bool CR2CP_Eth::WrongAccess(eEthPriorities prio , unsigned char node_dest, unsigned char node_iss, unsigned char index, unsigned char subindex, unsigned char dataLength, unsigned char *pdata){
	unsigned char data[] = {pdata[0],ERROR_MESSAGE_NOT_AVAILABLE};
	return Processed(prio , node_dest, node_iss, index, subindex, 0x02, data);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Reserved
//
bool CR2CP_Eth::Reserved(eEthPriorities prio , unsigned char node_dest, unsigned char node_iss, unsigned char index, unsigned char subindex, unsigned char dataLength, unsigned char *pdata)
{
	return ProtFunction( prio, node_dest , node_iss , index , subindex , ETH_RESERVED_FNC , dataLength , pdata );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: SendRawData
//
bool CR2CP_Eth::SendRawData(eEthPriorities prio , unsigned char node_dest, unsigned char node_iss, unsigned char index ,unsigned char subindex, unsigned char function ,unsigned short  dataLength, unsigned char *pdata)
{
	reinterpret_cast<tSocket_Header_ptr>(pdata)->sequence   = this->Get_Sequence();
	reinterpret_cast<tSocket_Header_ptr>(pdata)->priority	= prio;
	reinterpret_cast<tSocket_Header_ptr>(pdata)->dest_node	= node_dest;
	reinterpret_cast<tSocket_Header_ptr>(pdata)->iss_node	= node_iss;
	reinterpret_cast<tSocket_Header_ptr>(pdata)->index	    = index;
	reinterpret_cast<tSocket_Header_ptr>(pdata)->subindex  	= subindex;
	reinterpret_cast<tSocket_Header_ptr>(pdata)->function  	= function;
	reinterpret_cast<tSocket_Header_ptr>(pdata)->lenh       = HB( dataLength );
	reinterpret_cast<tSocket_Header_ptr>(pdata)->lenl		= LB(dataLength);
	
	if( mCbSendMsgEth == nullptr )
	{
		return true;
	} 
	
 	return mCbSendMsgEth( pdata, dataLength + HEADER_SIZEOF);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: ProcessWorkflow
//
bool CR2CP_Eth::ProcessWorkflow(unsigned char priority, unsigned char node_dest, unsigned char node_iss, unsigned char index, unsigned char subindex, unsigned char function, unsigned char dataLength, unsigned char *pdata){
	switch(function){
	case ETH_SET_FNC:
		switch(pdata[1]){
		case TOKEN_REQUEST_ID:
			if(m_token_granted == R2CP_ETH_BROADCAST_NODE_ID || m_token_granted == pdata[0]){
				m_token_granted = pdata[0];
				
				ProtFunction( 	static_cast<eEthPriorities>(priority) , 
								R2CP_ETH_BROADCAST_NODE_ID , 
								node_iss , 
								II_Workflow , 
								II_WORKFLOW_SS_TOKEN , 
								ETH_EVENT_ANSWER_FNC, 
								0x01, 
								&m_token_granted);
				Processed(	static_cast<eEthPriorities>(priority), 
							node_iss,
							node_dest, 
							index, 
							subindex, 
							0x01, 
							reinterpret_cast<unsigned char *>(TOKEN_IS_GRANTED));
				
				//m_p_timer_token_request->Load(m_TimerNodeToken_assigned,TIME_RELEASE_TOKEN);
			}
			else
				Processed(	static_cast<eEthPriorities>(priority),
							node_iss,
							node_dest, 
							index, 
							subindex, 
							0x01, 
							reinterpret_cast<unsigned char *>(ERROR_CODE_TOKEN_ALREADY_GRANTED));
			break;
		case TOKEN_RELEASE_ID:
			if( m_token_granted == pdata[0]){
				m_token_granted = R2CP_ETH_BROADCAST_NODE_ID;
				ProtFunction( 	static_cast<eEthPriorities>(priority) , 
								R2CP_ETH_BROADCAST_NODE_ID , 
								node_iss , 
								II_Workflow , 
								II_WORKFLOW_SS_TOKEN , 
								ETH_EVENT_ANSWER_FNC, 
								0x01, 
								&m_token_granted);
				
				Processed(	static_cast<eEthPriorities>(priority), 
							node_iss,
							node_dest, 
							index, 
							subindex, 
							0x01, 
							reinterpret_cast<unsigned char *>(TOKEN_IS_GRANTED));
			}
			else
				Processed(	static_cast<eEthPriorities>(priority), 
							node_iss,
							node_dest, 
							index, 
							subindex, 
							0x01, 
							reinterpret_cast<unsigned char *>(ERROR_CODE_TOKEN_ALREADY_GRANTED));
			break;
		case TOKEN_REFRESH_ID:
			if( m_token_granted == pdata[0]){
				ProtFunction( 	static_cast<eEthPriorities>(priority) , 
								R2CP_ETH_BROADCAST_NODE_ID , 
								node_iss , 
								II_Workflow , 
								II_WORKFLOW_SS_TOKEN , 
								ETH_EVENT_ANSWER_FNC, 
								0x01, 
								&m_token_granted);
				
				Processed(	static_cast<eEthPriorities>(priority), 
							node_iss,
							node_dest, 
							index, 
							subindex, 
							0x01, 
							reinterpret_cast<unsigned char *>(TOKEN_IS_GRANTED));
				//m_p_timer_token_request->Load(m_TimerNodeToken_assigned,TIME_RELEASE_TOKEN);
			}
			else					
				Processed(	static_cast<eEthPriorities>(priority), 
							node_iss,
							node_dest, 
							index, 
							subindex, 
							0x01, 
							reinterpret_cast<unsigned char *>(ERROR_CODE_TOKEN_ALREADY_GRANTED));			
			break;
		default:
			break;
		}
		break;
		case ETH_GET_FNC:
			ProtFunction( 	static_cast<eEthPriorities>(priority) , 
							R2CP_ETH_BROADCAST_NODE_ID , 
							node_iss , 
							II_Workflow , 
							II_WORKFLOW_SS_TOKEN , 
							ETH_EVENT_ANSWER_FNC, 
							0x01, 
							&m_token_granted);
			
			Processed(	static_cast<eEthPriorities>(priority), 
						node_iss,
						node_dest, 
						index, 
						subindex, 
						0x01, 
						(m_token_granted != R2CP_ETH_BROADCAST_NODE_ID) ? 
								reinterpret_cast<unsigned char *>(TOKEN_IS_GRANTED) : 
								reinterpret_cast<unsigned char *>(ERROR_CODE_TOKEN_ALREADY_GRANTED));						
			break;
		default:
		break;
	}

	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Get_Sequence 
//
unsigned char CR2CP_Eth::Get_Sequence(void){
	if(m_sequence > 0x3F)
		m_sequence = 1;
	return m_sequence++;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Get_Sequence
// CALLBACK IN CASE SWTIMER EXPIRED
//
void CR2CP_Eth::Callback_token_expired(unsigned char node){
	m_token_granted = R2CP_ETH_BROADCAST_NODE_ID;
}

void CR2CP_Eth::evalCommadProcessedResult(void){

    if(isCpGet){
        switch(commandProcessedIndex){
            case GENERATOR_COMMANDS_ENTRY: evalGetGeneratorProcessedResult();
                break;
            case PATIENT_COMMANDS_ENTRY: evalGetPatientProcessedResult();
                break;
            case SYSTEM_COMMANDS_ENTRY: evalGetSystemProcessedResult();
                break;
        }

    }else{
        switch(commandProcessedIndex){
            case GENERATOR_COMMANDS_ENTRY: evalSetGeneratorProcessedResult();
                break;
            case PATIENT_COMMANDS_ENTRY: evalSetPatientProcessedResult();
                break;
            case SYSTEM_COMMANDS_ENTRY: evalSetSystemProcessedResult();
                break;
        }

    }
}




void CR2CP_Eth::evalSetGeneratorProcessedResult(void){

    std::string stringa = "GENERATOR COMMAND PROCESS ERROR :";
    switch(commandProcessedSubIndex){

    case GENERATOR_DATA_BANK_ASSIGN_EXPOSURE:
        stringa += "(GENERATOR_DATA_BANK_ASSIGN_EXPOSURE) ";
        switch(commandProcessedResult){
            case 1: stringa += "Procedure ID not defined";break;
            case 2: stringa += "Procedure ID out of range";break;
            case 3: stringa += "Procedure ID already active, assign not possible";break;
            case 4: stringa += "Exposure Sequence Number out of range";break;
            case 5: stringa += "Exposure Data Bank ID not defined";break;
            case 6: stringa += "Exposure Data Bank ID out of range";break;
            case 7: stringa += "Exposure DB incompatible with Procedure Type";break;
            case 8: stringa += "Exposure Sequence Number already assigned";break;
            default:
                stringa +=" ?? " + std::to_string(commandProcessedResult);
        }

        break;

    case GENERATOR_DATA_BANK_EXPOSURE_ACCEPTANCE:
        stringa += "(GENERATOR_DATA_BANK_EXPOSURE_ACCEPTANCE) ";
        switch(commandProcessedResult){
            case 1: stringa += "Procedure ID not defined";break;
            case 2: stringa += "Procedure ID out of range";break;
            case 3: stringa += "Exposure Sequence Number out of range";break;
            case 4: stringa += "No Exposure DB assigned";break;
            default:
                stringa +=" ?? " + std::to_string(commandProcessedResult);

        }

        break;

    case GENERATOR_RAD_DATA_BANK_ACCEPTANCE:
        stringa += "(GENERATOR_RAD_DATA_BANK_ACCEPTANCE) ";
        switch(commandProcessedResult){
            case 1: stringa += "Procedure ID out of range";break;
            case 2: stringa += "Procedure ID not defined";break;
            case 3: stringa += "Procedure not RAD";break;
            default:
                stringa +=" ?? " + std::to_string(commandProcessedResult);

        }

        break;

    case GENERATOR_RAD_DATA_BANK_LOAD_V6:
        stringa += "(GENERATOR_RAD_DATA_BANK_LOAD_V6) ";
        switch(commandProcessedResult){
            case 1: stringa += "Procedure ID out of range";break;
            case 2: stringa += "Procedure ID not defined";break;
            case 4: stringa += "Pediatric code out of range";break;
            case 5: stringa += "Technique mode code out of range";break;
            case 6: stringa += "Technique mode not supported";break;
            case 7: stringa += "kVp value out of range";break;
            case 8: stringa += "mAs value out of range";break;
            case 9: stringa += "mA value out of range";break;
            case 10: stringa += "ms value out of range";break;
            case 11: stringa += "Maximum integration time value out of range";break;
            case 12: stringa += "Focal spot code out of range";break;
            case 13: stringa += "Focal spot not supported";break;
            case 14: stringa += "AEC sensitivity/Dose Target code out of range";break;
            case 15: stringa += "AEC density code out of range";break;
            case 16: stringa += "NA";break;
            case 17: stringa += "Tube power limit out of range";break;
            case 18: stringa += "FPS value out of range";break;
            case 19: stringa += "Minimum integration time value out of range";break;
            case 20: stringa += "NA";break;
            case 21: stringa += "Tracking not supported";break;
            case 22: stringa += "NA";break;
            case 23: stringa += "NA";break;
            case 24: stringa += "DataBank type doesn?t match";break;
            case 25: stringa += "NA";break;
            case 202: stringa += "Message not available";break;

        default:
            stringa +=" ?? " + std::to_string(commandProcessedResult);
        }

        break;

    case GENERATOR_EXPOSURE_MANAGEMENT_START_STOP_EXPOSURE:
        stringa += "(GENERATOR_EXPOSURE_MANAGEMENT_START_STOP_EXPOSURE) ";
        switch(commandProcessedResult){
            case 1: stringa += "Feature not supported by the detector interface";break;
            case 2: stringa += "Request Value out of range";break;

            default:
                stringa +=" ?? " + std::to_string(commandProcessedResult);

        }

        break;

    default:
        stringa +=" ?? SUBIDX=" + std::to_string(commandProcessedSubIndex) + " CPCODE=" + std::to_string(commandProcessedResult);
    }

    
    commandProcessedString = stringa;
}






void CR2CP_Eth::evalSetPatientProcessedResult(void){

    std::string stringa = "PATIENT COMMAND PROCESS ERROR :";
    switch(commandProcessedSubIndex){

    case PATIENT_PROCEDURE_CLEAR_ALL:
        stringa += "(PATIENT_PROCEDURE_DEFINITION_V6) ";
        switch(commandProcessedResult){
            default:
                stringa +=" ?? " + std::to_string(commandProcessedResult);
        }

        break;

    case PATIENT_PROCEDURE_DEFINITION_V6:
        stringa += "(PATIENT_PROCEDURE_DEFINITION_V6) ";
        switch(commandProcessedResult){
            case 1: stringa += "Procedure ID out of range";break;
            case 2: stringa += "Procedure Type not supported";break;
            case 10: stringa += "Handswitch/footswitch index out of range";break;
            case 12: stringa += "Non existent workstation";break;
            case 13: stringa += "Incorrect number of exposure DBs";break;
            case 14: stringa += "Exposure DB sequencing not possible (exposure DB=1)";break;
            case 15: stringa += "Exposure DB sequencing not supported";break;
            case 16: stringa += "Workstation out of range";break;
            case 17: stringa += "Handswitch/footswitch Index not defined";break;
            case 18: stringa += "Procedure Already Created";break;
            default:
                stringa +=" ?? " + std::to_string(commandProcessedResult);
        }

        break;

    case PATIENT_PROCEDURE_ACTIVATE:
        stringa += "(PATIENT_PROCEDURE_ACTIVATE) ";
        switch(commandProcessedResult){
            case 1: stringa += "Procedure ID not defined";break;
            case 2: stringa += "Procedure ID out of range";break;
            case 3: stringa += "Exposure Sequence Number out of range";break;
            case 4: stringa += "Exposure DB not assigned";break;
            case 5: stringa += "Procedure invalid activation";break;
            case 6: stringa += "Procedure invalid deactivation";break;
            case 7: stringa += "Exposure Sequence Number invalid";break;

            default:
                stringa +=" ?? " + std::to_string(commandProcessedResult);
        }

        break;
    default:
        stringa +=" ?? SUBIDX=" + std::to_string(commandProcessedSubIndex) + " CPCODE=" + std::to_string(commandProcessedResult);
    }

    commandProcessedString = stringa;
}

void CR2CP_Eth::evalSetSystemProcessedResult(void){

	commandProcessedString = "SYSTEM COMMAND PROCESS ERROR, SUBINDEX:" + std::to_string(commandProcessedSubIndex) + " CODE:" + std::to_string(commandProcessedResult);

}



void CR2CP_Eth::evalGetGeneratorProcessedResult(void){}

void CR2CP_Eth::evalGetPatientProcessedResult(void){}


void CR2CP_Eth::evalGetSystemProcessedResult(void){
	std::string stringa = "SYSTEM COMMAND PROCESS ERROR :";
    switch(commandProcessedSubIndex){

    case SYSTEM_ALL_SYSTEM_MESSAGES:
        stringa += "(SYSTEM_ALL_SYSTEM_MESSAGES) ";
        switch(commandProcessedResult){
            default:
                stringa +=" ?? " + std::to_string(commandProcessedResult);
        }

        break;

    default:
		stringa += " ?? SUBIDX=" + std::to_string(commandProcessedSubIndex) + " CPCODE=" + std::to_string(commandProcessedResult);
    }
   
    commandProcessedString = stringa;
}
