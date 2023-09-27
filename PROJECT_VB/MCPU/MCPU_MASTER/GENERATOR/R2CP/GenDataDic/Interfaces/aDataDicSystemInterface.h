#ifndef __ADATADICSYSTEMMESSAGEINTERFACE__H_
 #define __ADATADICSYSTEMMESSAGEINTERFACE__H_

#include "aDataDicInterface.h"
#include <vector>

namespace R2CP
{
// #define GENERAL_RX_DISABLE_MSG_ID ((unsigned long) 500074)
 const unsigned long  GENERAL_RX_DISABLE_MSG_ID		= 500074;

	typedef struct{
		unsigned char IssuingNode;
		unsigned char InhibitGeneratorPhase;
		unsigned char InhibitPositionerPhase;//Cambiar
		bool InhibitRX;
		bool InhibitMovement;
		bool ClearByIssuingNode;
	}tSystemMessageInfo;
	
	typedef struct{
		int 				Id;
        bool                Active;
		tSystemMessageInfo  Message_Info;
	}tSystemMessage;

	class CaDataDicSystemInterface
	{
	public:
		
		/*!
		 * \brief
		 */
		virtual ~CaDataDicSystemInterface() { }
	
        bool handleMessage(tSystemMessage* pItem){
            if(pItem->Id == R2CP::GENERAL_RX_DISABLE_MSG_ID){
                if(pItem->Active) rx_disable_msg = true;
                else rx_disable_msg = false;
                return true;
            }

            if(pItem->Active){
                for(int i=0; i< messageList.size(); i++){
                    if(messageList[i] == pItem->Id) return false; // Already active
                }
                messageList.push_back(pItem->Id);                
                return true;
            }else{
                if(messageList.size() == 0) return false;
                for(int i=0; i< messageList.size(); i++){
                    if(messageList[i] == pItem->Id){
                        messageList.erase(messageList.begin()+i);
                        return true;
                    }
                }
                return false;
            }
            return false;
        }

        std::vector<unsigned long> messageList;
        bool rx_disable_msg;

	};



};
#endif
