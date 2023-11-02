
#include "CaDataDic.h"

#include "Api_log.h"

#define OK_ACCESS		0
#define WRONG_ACCESS	203
#define NOT_AVAILABLE	202


CaDataDic::CaDataDic( unsigned char Node ):/*datadic_mutex( nullptr ) ,*/ mNodeId( Node )
{
					
	pDataDictionary = nullptr;
	
	mHwVersion.Revision = '0';
	mHwVersion.Version = 0;
	mHwVersion.Model = 0;
	
	mSwVersion.Subrevision = 0;
	mSwVersion.Revision = 0;
	mSwVersion.Version = 0;
	
	mBootVersion.Subrevision = 0;
	mBootVersion.Revision = 0;
	mBootVersion.Version = 0;
	
	mProtocolVersion.Revision = '0';
	mProtocolVersion.SubVersion = 0;
	mProtocolVersion.Version = 0;

	m_pPackageVersion = (char*)nullptr;
	mPipeSubNodeId = -1;
	mPipeInterfaceNumber = -1;
	m_pSerialNumber = (char*)nullptr;
	m_pDescription = (char*)nullptr;
	
	mStatusRegister.status = DATADIC_STATUS_INITIALIZING;
	mStatusRegister.modes = DATADIC_MODE_SAFETY;
	mStatusRegister.heartbeatFlag = 0;
	mStatusRegister.errorFlag = 0;	   
	mStatusRegister.bootingType = 0;
	mStatusRegister.reserved = 0;
	mStatusRegister.unused = 0;
	
	mHeartBeatTime = 0;
}

CaDataDic::~CaDataDic()
{

}

unsigned char CaDataDic::DataDicCheckAndCall(unsigned char Index,  unsigned char SubIndex, unsigned char Node, tDataDicAccess Access, unsigned char *pData, unsigned short  Size,tInfoMessage *infomessage)
{

	unsigned char rowPosition = 0;
	short entryPosition = SUBINDEX_NOT_FOUND;
	unsigned char return_value = WRONG_ACCESS;
    
    if( mNodeId == Node )
    {
    	
	    for( rowPosition = 0; rowPosition < pDataDictionary->nIndexes; rowPosition++)
	    {
	        if( pDataDictionary->pRows[rowPosition].Index == Index )
            {

	        	///
	        	///The binary searching works if the DATADIC is sorted
	        	///
	        	short low = 0; 														// low end of the search area
	        	short high = pDataDictionary->pRows[rowPosition].nSubIndexes - 1; 	// high end of the search area
	        	unsigned short  middle = ( low + high + 1 ) / 2; 									// middle element
	        	do // loop to search for element
	        	{
	        		// if the element is found at the middle
	        		if ( SubIndex == pDataDictionary->pRows[rowPosition].pEntries[middle].SubIndex  )
	        			entryPosition = middle; 															// location is the
	        		else if ( SubIndex < pDataDictionary->pRows[rowPosition].pEntries[middle].SubIndex ) 	// middle is too high
	        			high = middle - 1; 																	// eliminate the higher half
	        		else // middle element is too low
	        			low = middle + 1; 					// eliminate the lower half
	        		middle = ( low + high + 1 ) / 2; 		// recalculate the middle
	        	} while ( ( low <= high ) && ( entryPosition == SUBINDEX_NOT_FOUND ) );
	        	
	        	if(entryPosition != SUBINDEX_NOT_FOUND)
	        	{
	        		if(!Check_NotAvailable_Message_vs_Status(Index,SubIndex,infomessage))
	        		{
						return_value = NOT_AVAILABLE;
						break;
	        		}
	        			//return OK_ACCESS;
					if( pDataDictionary->pRows[rowPosition].pEntries[entryPosition].pFunction != nullptr )
					{
						if( Access & pDataDictionary->pRows[rowPosition].pEntries[entryPosition].AccessType )
						{
							unsigned char dataSize = 0;
							switch( Access )
							{
								case DATADIC_ACCESS_GET:
									dataSize = pDataDictionary->pRows[rowPosition].pEntries[entryPosition].GetDataSize;
									break;
								case DATADIC_ACCESS_SET:
									dataSize = pDataDictionary->pRows[rowPosition].pEntries[entryPosition].SetDataSize;
									break;
								case DATADIC_ACCESS_ANSWER:
									dataSize = pDataDictionary->pRows[rowPosition].pEntries[entryPosition].AnswerDataSize;
									break;
								case DATADIC_ACCESS_EVENT:
									dataSize = pDataDictionary->pRows[rowPosition].pEntries[entryPosition].EventDataSize;
									break;
								default:
									break;
							}
							if( dataSize == Size || dataSize == UNDEFINED_LENGHT )
							{
								// Call the function
								if( pDataDictionary )
								{
									pDataDictionary->pRows[rowPosition].pEntries[entryPosition].pFunction(Access, pData, Size, infomessage);
									return_value = OK_ACCESS;
									break;
								}
							}
							else{
								break;
							}
						}
						else{
							break;
						}
					}
					else{
						break;
					}
	        	}
	        	else{
	        		break;
	        	}
	        }
	    }
    }

    /*
    if(datadic_mutex)
    {
    	(void)OS::Mutex::UnLock(datadic_mutex);
    }*/
    
    //_io_printf("\n-MT_%d",Node);
    	
    return return_value;
}
