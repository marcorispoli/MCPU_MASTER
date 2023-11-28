/*!
 * \file      CaDataDicGen.cpp
 * \brief     Definition of the local object dictionary.
 * \author   
 * \bug       Unknown bugs
 * \copyright SEDECAL S.A.
 * \ingroup   R2CPModule
*/

#include <stdio.h>
#include "CaDataDicGen.h"
#include "R2CP_Eth.h"

namespace R2CP
{
	/******************************************************************************************************************/
	//												NETWORK
	/******************************************************************************************************************/
	
	void CaDataDicGen::Network_HeartBeat(tDataDicAccess Access, unsigned char *pData, unsigned short  nData, tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		unsigned char data[2];
		
		data[0] = MessageInfo->Node_own;
		data[1] = static_cast<unsigned char >(~pData[1]);
	
		(void)m_Type_->Event_Answer(	static_cast<eEthPriorities>(MessageInfo->Priority),
										MessageInfo->Node_dest, 
										m_p_instance_->mNodeId, 
										NETWORK_COMMANDS_ENTRY, 
										NETWORK_HEARTBEAT,
										0x02, 
										data);
	}
	
	/// <summary>
	/// This function is called whenever a CONNECTION-CHANGED EVENT is received
	/// 
	/// </summary>
	/// <param name="Access"></param>
	/// <param name="pData"></param>
	/// <param name="nData"></param>
	/// <param name="MessageInfo"></param>
	void CaDataDicGen::Network_ConnectionChanged(tDataDicAccess Access, unsigned char *pData, unsigned short  nData, tInfoMessage *MessageInfo)
	{
        if(MessageInfo == nullptr) return;
        
        if(nData !=2) return;
        if(pData[0] == APPLICATION_NODE_ID){
			if (pData[1] == 1) m_Type_-> smartHubConnected = true;
            return;
        }

        if(pData[0] == GENERATOR_NODE_ID){
			if (pData[1] == 1) m_Type_->generatorConnected = true;
            else m_Type_->generatorConnected = false;
            return;
        }

	}
	
	void CaDataDicGen::Network_GetApplicationNodeStatus(void)
	{
		unsigned char node_iss = APPLICATION_NODE_ID;
		unsigned char node_dest = SH_NODE_ID;
		unsigned char node_get = APPLICATION_NODE_ID;
		
		(void)m_Type_->Get(ETH_LOWEST_PRIORITY,
			node_dest,
			node_iss,
			NETWORK_COMMANDS_ENTRY,
			NETWORK_NODESTATUS,
			1,
			&node_get);
	}

	void CaDataDicGen::Network_GetGeneratorNodeStatus(void)
	{
		unsigned char node_iss = APPLICATION_NODE_ID;
		unsigned char node_dest = SH_NODE_ID;
		unsigned char node_get = GENERATOR_NODE_ID;

		(void)m_Type_->Get(ETH_LOWEST_PRIORITY,
			node_dest,
			node_iss,
			NETWORK_COMMANDS_ENTRY,
			NETWORK_NODESTATUS,
			1,
			&node_get);
	}



	/// <summary>
	/// This function is called when a network NODE-STATUS ANSWER/EVENT is received.
	/// 
	/// </summary>
	/// <param name="Access"></param>
	/// <param name="pData"></param>
	/// <param name="nData"></param>
	/// <param name="MessageInfo"></param>
	void CaDataDicGen::Network_NodeStatus(tDataDicAccess Access, unsigned char *pData, unsigned short  nData, tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		
		if (pData[0] == APPLICATION_NODE_ID) {
			if(pData[1]) m_Type_->smartHubConnected = true;
			else m_Type_->smartHubConnected = false;
		}

		if (pData[0] == GENERATOR_NODE_ID) {
			if (pData[1]) m_Type_->generatorConnected = true;
			else m_Type_->generatorConnected = false;
		}

	}
	
	void CaDataDicGen::Network_ConnectionRequest(tDataDicAccess Access, unsigned char *pData, unsigned short  nData, tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		if(m_p_NetworkInteface_)
			m_p_NetworkInteface_->II_Network_SS_NetworkConnectionRequest(pData[0]);
	}
	
    void CaDataDicGen::Network_ConnectionRequest_Event(void)
	{
        unsigned char node_iss = APPLICATION_NODE_ID;
        unsigned char node_dest = SH_NODE_ID;

		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
                                        node_dest,
                                        node_iss,
										NETWORK_COMMANDS_ENTRY, 
										NETWORK_CONNECTION_REQUEST, 
										sizeof(unsigned char ), 
										&node_iss);
	}
	
	

	void CaDataDicGen::Network_IpConfig(tDataDicAccess Access, unsigned char *pData, unsigned short  nData, tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		unsigned char pDataCp[] = {  MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_NetworkInteface_)
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
					{
						m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
	
						udword IpAddress,MaskSubNetwork,Gateway,ShIpAdress;
						
						IpAddress.value 	  = (pData[0] << 24)	|  (pData[1] << 16) | (pData[2] << 8)  | pData[3];
						MaskSubNetwork.value  = (pData[4] << 24)	|  (pData[5] << 16) | (pData[6] << 8)  | pData[7];
						Gateway.value		  = (pData[8] << 24)	|  (pData[9] << 16) | (pData[10] << 8) | pData[11];
						ShIpAdress.value	  = (pData[12]<< 24)    |  (pData[13]<< 16) | (pData[14] << 8) | pData[15];
	
						pDataCp[1] = m_p_NetworkInteface_->II_Network_SS_IpConfig(	IpAddress,
																					MaskSubNetwork,
																					Gateway,
																					ShIpAdress);
					}
					break;
				case DATADIC_ACCESS_GET:
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_NetworkInteface_->II_Network_SS_IpConfig();
					break;
				default:
					break;
			}
		}
				
		(void)m_Type_->Processed(	static_cast<eEthPriorities>(MessageInfo->Priority),
									m_p_instance_->GetNodeEvent(true),
									m_p_instance_->mNodeId,
									NETWORK_COMMANDS_ENTRY,
									NETWORK_CONFIGURATION,
									sizeof(pDataCp),
									(pDataCp));
	}
	
	void CaDataDicGen::Network_FileConfigUpdate	( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo )
	{	
		if(m_p_NetworkInteface_)
			m_p_NetworkInteface_->II_Network_SS_FileConfigUpdate();
	}
	
	void CaDataDicGen::Network_SnapShot	( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo )
	{
		/*Deprecated*/
	}
	
	void CaDataDicGen::Network_BackupSnapShot ( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		unsigned char pDataCp[] = {  MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_NetworkInteface_)
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					tBackupAndSnapShot BackupAndSnapShot;
					
					BackupAndSnapShot.Log.Value 					= pData[0];
					BackupAndSnapShot.ConfigAndCalibration.Value	= pData[1];
					
					pDataCp[1] = m_p_NetworkInteface_->II_Network_SS_BackupSnapShot(BackupAndSnapShot);
					
				break;
			}
		}
				
		(void)m_Type_->Processed(	static_cast<eEthPriorities>(MessageInfo->Priority),
									m_p_instance_->GetNodeEvent(true),
									m_p_instance_->mNodeId,
									NETWORK_COMMANDS_ENTRY,
									NETWORK_BACKUP_SNAPSHOT,
									sizeof(pDataCp),
									(pDataCp));

	}
	
	void CaDataDicGen::Network_Restore ( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		unsigned char pDataCp[] = {  MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_NetworkInteface_)
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					tRestore Restore;					
					Restore.Value = pData[0];
					pDataCp[1] = m_p_NetworkInteface_->II_Network_SS_Restore(Restore);
					
				break;
			}
		}
				
		(void)m_Type_->Processed(	static_cast<eEthPriorities>(MessageInfo->Priority),
									m_p_instance_->GetNodeEvent(true),
									m_p_instance_->mNodeId,
									NETWORK_COMMANDS_ENTRY,
									NETWORK_RESTORE,
									sizeof(pDataCp),
									(pDataCp));
	}	
	
	void CaDataDicGen::Network_CalConfigUpdate( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
	{
		if(m_p_NetworkInteface_)
		{
			m_p_NetworkInteface_->II_Network_SS_FileConfigUpdate();
		}
	}
	/************************************ EVENTS **********************************************/
	void CaDataDicGen::Network_IpConfig_Event(udword IpAddress, udword MaskSubNetwork , udword Gateway , udword ShIpAdress)
	{
		unsigned char data[16];

		data[0] = IpAddress.Fields.data_24_31;
		data[1] = IpAddress.Fields.data_16_23;
		data[2]=  IpAddress.Fields.data_8_15;
		data[3]=  IpAddress.Fields.data_0_7;
	
		data[4] = MaskSubNetwork.Fields.data_24_31;
		data[5] = MaskSubNetwork.Fields.data_16_23;
		data[6]=  MaskSubNetwork.Fields.data_8_15;
		data[7]=  MaskSubNetwork.Fields.data_0_7;

		data[8] = Gateway.Fields.data_24_31;
		data[9] = Gateway.Fields.data_16_23;
		data[10]= Gateway.Fields.data_8_15;
		data[11]= Gateway.Fields.data_0_7;
	
		data[12] = ShIpAdress.Fields.data_24_31;
		data[13] = ShIpAdress.Fields.data_16_23;
		data[14] = ShIpAdress.Fields.data_8_15;
		data[15] = ShIpAdress.Fields.data_0_7;
	
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
										GetNodeEvent(),
										m_p_instance_->mNodeId,
										NETWORK_COMMANDS_ENTRY,
										NETWORK_CONFIGURATION,
										sizeof(data),
										data);
	}
	
	void CaDataDicGen::Network_BackupSnapShot_Event ( tBackupSnapShotStatus Status )
	{
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
										m_p_instance_->GetNodeEvent(),
										m_p_instance_->mNodeId,
										NETWORK_COMMANDS_ENTRY,
										NETWORK_BACKUP_SNAPSHOT,
										sizeof(unsigned char ),
										(unsigned char *)&Status);
	}
	
	void CaDataDicGen::Network_Restore_Event ( tRestoreStatus Status )
	{
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
										m_p_instance_->GetNodeEvent(),
										m_p_instance_->mNodeId,
										NETWORK_COMMANDS_ENTRY,
										NETWORK_RESTORE,
										sizeof(unsigned char ),
										(unsigned char *)&Status);
	}
	
	
	
}//namespace R2CP
