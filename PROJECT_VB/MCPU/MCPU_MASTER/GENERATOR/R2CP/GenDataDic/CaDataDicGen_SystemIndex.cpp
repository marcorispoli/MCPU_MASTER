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



namespace R2CP
{
	/******************************************************************************************************************/
	//												System
	/******************************************************************************************************************/

	void CaDataDicGen::System_SystemMessage( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo )
	{
        if(MessageInfo == nullptr) return;

		if(m_p_SystemInterface_) 
		{
			switch( Access )
			{
                case DATADIC_ACCESS_EVENT:
                case DATADIC_ACCESS_ANSWER_EVENT:
                {
                    if(!m_p_SystemInterface_) return;

                    tSystemMessage SystemMessage;
                    //Se cambia el Issuing Node por el del nodo que hace el set
                    SystemMessage.Message_Info.IssuingNode				= /*pData[0]*/MessageInfo->Node_dest;
                    SystemMessage.Id									= (int)pData[1]<<24 | (int)pData[2]<<16 | (int)pData[3]<<8 | pData[4];
                    SystemMessage.Active = (pData[5] == 1) ? true: false;
                    SystemMessage.Message_Info.InhibitRX				= pData[6];
                    SystemMessage.Message_Info.InhibitGeneratorPhase 	= pData[7];
                    SystemMessage.Message_Info.InhibitMovement			= pData[8];
                    SystemMessage.Message_Info.InhibitPositionerPhase	= pData[9];
                    m_p_SystemInterface_->handleMessage(&SystemMessage);
                }

			}
		}

	}




	/************************************************************************************************************************/
	/*********************************** Service Mode************************************************************************/
	/************************************************************************************************************************/
	void CaDataDicGen::System_LoadDesktop ( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo )
	{
	}
	void CaDataDicGen::System_DesktopDisplay ( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
	{
	}
	void CaDataDicGen::System_NumericPropertySetup(tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
	{
	}
	void CaDataDicGen::System_NumericPropertyValue(tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
	{
	}
	void CaDataDicGen::System_StringProperty(tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
	{
	
	}
	void CaDataDicGen::Sytem_NumericValueTable(tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
	{
	
	}
	void CaDataDicGen::Sytem_NumericValueTableItem(tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
	{
	
	}
	void CaDataDicGen::System_StringTable(tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
	{
	}
	void CaDataDicGen::System_SingleUserRequest(tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
	{
	}
	void CaDataDicGen::System_UserRequestTable(tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
	{
	}

    // Get Functions
    void CaDataDicGen::SystemMessages_Get_AllMessages(void){
        m_p_SystemInterface_->messageList.clear();
        m_p_SystemInterface_->rx_disable_msg = false;;


        (void)m_Type_-> Get(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                SYSTEM_COMMANDS_ENTRY,
                                SYSTEM_ALL_SYSTEM_MESSAGES,
                                0,
                                nullptr);
    }

    void CaDataDicGen::SystemMessages_SetDisableRx(bool state){

        unsigned char pData[12];
        unsigned long msgId = GENERAL_RX_DISABLE_MSG_ID;

        pData[0] = mNodeId;
        pData[1] = (unsigned char ) ((msgId & 0xFF000000) >> 24);
        pData[2] = (unsigned char ) ((msgId & 0x00FF0000) >> 16);
        pData[3] = (unsigned char ) ((msgId & 0x0000FF00) >> 8);
        pData[4] = (unsigned char ) ((msgId & 0x000000FF));
        if(state) pData[5] = 1; // Active
        else pData[5] = 0; // Not Active
        pData[6] = 1;      // Inhibit
        pData[7] = 2;      // Standby
        pData[8] = 0;
        pData[9] = 0;
        pData[10]= 0;
        pData[11]= 0;

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                SYSTEM_COMMANDS_ENTRY,
                                SYSTEM_SYSTEM_MESSAGE,
                                10,
                                pData);

        return ;

    }




    bool CaDataDicGen::SystemMessages_Clear_Message(unsigned int ID){

        if(ID == GENERAL_RX_DISABLE_MSG_ID) return false;
        unsigned char pData[12];


        pData[0] = GENERATOR_NODE_ID;
        pData[1] = (unsigned char ) ((ID & 0xFF000000) >> 24);
        pData[2] = (unsigned char ) ((ID & 0x00FF0000) >> 16);
        pData[3] = (unsigned char ) ((ID & 0x0000FF00) >> 8);
        pData[4] = (unsigned char ) ((ID & 0x000000FF));
        pData[5] = 0;
        pData[6] = 1;
        pData[7] = 2;
        pData[8] = 0;
        pData[9] = 0;
        pData[10]= 0;
        pData[11]= 0;

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                SYSTEM_COMMANDS_ENTRY,
                                SYSTEM_SYSTEM_MESSAGE,
                                10,
                                pData);

        return true;
    }


}//namespace
