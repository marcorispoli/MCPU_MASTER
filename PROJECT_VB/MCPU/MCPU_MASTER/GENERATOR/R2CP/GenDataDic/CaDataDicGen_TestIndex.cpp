/*!
 * \file      CaDataDicGen.cpp
 * \brief     Definition of the local object dictionary.
 * \author   
 * \bug       Unknown bugs
 * \version
 *  \htmlonly
 *		<A HREF="">V1R01.1</A>
 *	\endhtmlonly
 * \date      01/08/2012
 * \copyright SEDECAL S.A.
 * \ingroup   R2CPModule
*/
#include "pch.h"
#include "CaDataDicGen.h"

/******************************************************************************************************************/
//												GENERATOR
/******************************************************************************************************************/
namespace R2CP
{

	
	void CaDataDicGen::Generator_Test_Error ( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
		
		unsigned char pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
		
		if( m_p_TestInterface_ ) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:									
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_TestInterface_->II_Test_SS_Error( pData[ 0 ] << 24 |  pData[ 1 ] << 16 | pData[ 2 ] << 8 | pData[ 3 ] /*Id*/, pData[ 4 ] /*Status*/ , pData[ 5 ]/*Type*/ );
					break;
			}
		}
	
		(void)m_Type_->Processed( ETH_LOWEST_PRIORITY ,
								   m_p_instance_->GetNodeEvent(true), 
								   m_p_instance_->mNodeId, 
								   GENERATOR_TEST_ENTRY, 
								   GENERATOR_TEST_ERRORS, 
								   sizeof(pDataCp), 
								   (pDataCp));

	}
	
	void CaDataDicGen::Generator_Test_PowerControl ( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
		
		unsigned char pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
		
		if( m_p_TestInterface_ ) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:									
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_TestInterface_->II_Test_SS_PowerControl( pData , nData );
					break;
			}
		}
	
		(void)m_Type_->Processed( ETH_LOWEST_PRIORITY ,
								   m_p_instance_->GetNodeEvent(true), 
								   m_p_instance_->mNodeId, 
								   GENERATOR_TEST_ENTRY, 
								   GENERATOR_TEST_POWER_CONTROL, 
								   sizeof(pDataCp), 
								   (pDataCp));

	}
	
	void CaDataDicGen::Generator_Test_Uarc ( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
		
		unsigned char pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
		
		if( m_p_TestInterface_ ) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:									
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_TestInterface_->II_Test_SS_Uarc( pData , nData );
					break;
			}
		}
	
		(void)m_Type_->Processed( ETH_LOWEST_PRIORITY ,
								   m_p_instance_->GetNodeEvent(true), 
								   m_p_instance_->mNodeId, 
								   GENERATOR_TEST_ENTRY, 
								   GENERATOR_TEST_UARC, 
								   sizeof(pDataCp), 
								   (pDataCp));
		
	}
		
	
	void CaDataDicGen::Generator_Test_Enable_Log ( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
		
		unsigned char pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
		
		if( m_p_TestInterface_ ) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:									
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_TestInterface_->II_Test_SS_Enable_Log( pData[ 0 ] , pData[ 1 ] );
					break;
			}
		}
	
		(void)m_Type_->Processed( ETH_LOWEST_PRIORITY ,
								   m_p_instance_->GetNodeEvent( true ), 
								   m_p_instance_->mNodeId, 
								   GENERATOR_TEST_ENTRY, 
								   GENERATOR_TEST_LOG_STATUS, 
								   sizeof(pDataCp), 
								   (pDataCp));
		
		
	}


}//namespace R2CP
