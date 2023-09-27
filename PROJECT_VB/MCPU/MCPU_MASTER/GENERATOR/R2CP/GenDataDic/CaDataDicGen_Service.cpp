/*!
 * \file      CaDataDicGen.cpp
 * \brief     Definition of the local object dictionary.
 * \author   
 * \bug       Unknown bugs
 * \copyright SEDECAL S.A.
 * \ingroup   R2CPModule
*/
#include "pch.h" 
#include "CaDataDicGen.h"

namespace R2CP
{
	/******************************************************************************************************************/
	//												SERVICE
	/******************************************************************************************************************/

	void CaDataDicGen::Service_StandardLicense	( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
		
		unsigned char pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
		
		if(m_p_ServiceInterface_)
		{
			if(Access == DATADIC_ACCESS_GET)
			{
				if(m_EnableLogInfo && m_fcb_log_)
				{
					m_fcb_log_(1,"[ R2CP DATADIC ] --> STANDARD LICENSE GET");		
				}
				
				m_p_instance_->SetNodeEvent( MessageInfo->Node_dest , true );
				pDataCp[1] = m_p_ServiceInterface_->II_Service_SS_StandardLicense();
			}
		}
		
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									SERVICE_COMMANDS_ENTRY, 
									SERVICE_STANDARD_LICENSE, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Service_ExtendedLicense ( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
		
		unsigned char pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
		
		if(m_p_ServiceInterface_)
		{
			if(Access == DATADIC_ACCESS_GET)
			{
				if(m_EnableLogInfo && m_fcb_log_)
				{
					m_fcb_log_(1,"[ R2CP DATADIC ] --> EXTENDED LICENSE GET");		
				}
				
				m_p_instance_->SetNodeEvent( MessageInfo->Node_dest , true );
				pDataCp[1] = m_p_ServiceInterface_->II_Service_SS_ExtendedLicense();
				
			}
		}
		
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									SERVICE_COMMANDS_ENTRY, 
									SERVICE_EXTENDED_LICENSE, 
									sizeof(pDataCp), 
									(pDataCp));
	}

	//Events
	void CaDataDicGen::Service_Log_Event(unsigned char *ServiceLog , unsigned short  SizeofLog)
	{

		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
										0,
										mNodeId,
										SERVICE_COMMANDS_ENTRY,
										SERVICE_LOG,
										SizeofLog,
										ServiceLog);

	}
	
	void CaDataDicGen::Service_StandardLicense_Event (tStandardLicense *StandardLicense)
	{
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY ,
										GetNodeEvent(), 
										mNodeId, 
										SERVICE_COMMANDS_ENTRY, 
										SERVICE_STANDARD_LICENSE, 
										sizeof(tStandardLicense), 
										(unsigned char *)(StandardLicense));
	}
	void CaDataDicGen::Service_ExtendedLicense_Event (tExtendedLicense *ExtendedLicense)
	{
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY ,
										GetNodeEvent(), 
										mNodeId, 
										SERVICE_COMMANDS_ENTRY, 
										SERVICE_EXTENDED_LICENSE, 
										sizeof(tExtendedLicense), 
										(unsigned char *)(ExtendedLicense));
	}
}
