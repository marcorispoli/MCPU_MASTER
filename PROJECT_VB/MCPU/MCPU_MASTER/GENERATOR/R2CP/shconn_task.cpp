#include "pch.h" 
#include <stdio.h>

#include "../Platform/Interface_OS/os_time.h"

#include "../Api/api_log.h"
#include "../Api/api_swtimer.h"

#include "../R2CP/R2CP_Eth.h"
#include "../R2CP/CSystemMessage.h"

#include "../DataDic/GenDataDic/CaDataDicGen.h"

#include "../HdlUpdateServer.h"

#include "../GeneratorStateMachine/GenMasterEvent_task.h"

#include "decode_task.h"

#include "../master/PosMasterEvent_task.h"

#include "shconn_task.h"

#define WAITING_TIME_FOR_EVENT_IF_NO_SERVER_RUNNING 1500
#define WAITING_TIME_FOR_FPGA_100MS_TIMEOUT			100
#define WAITING_TIME_FOR_EVENT_DEFAULT				OS::EVENT_WAIT_FOREVER

extern CR2CP_Eth *R2CP_Eth;

CaSh_Connection *CaSh_Connection::m_p_instance = nullptr;

tsmarthubinfo CaSh_Connection::m_sh_info = { 0 };

short SendEthMsg(unsigned char *msg, unsigned short  n_msg)
{	
	if( CaSh_Connection::m_p_instance->Get_SHConnectionStatus() == SH_Connected  && msg && n_msg )
	{
		return CaSh_Connection::m_p_instance->Send(msg,n_msg);
	}
	else{
		return -1;
	}
}


void smarthub_connection_task( void * temp )
{
	OS::event_flags_t syncStatus= 0;

	CaSh_Connection::GetInstance();
			
	/*Create synchro event*/
	CaSh_Connection::m_p_instance->event_socket = OS::Event::Create();
	/*****************************************************************************************************************/
	
	while(1)
	{
		/* Check ETHERNET Interface */
    	if( CaSh_Connection::m_p_instance->Get_SHConnectionStatus() == SH_DisConnected || CaSh_Connection::m_p_instance->Get_SHConnectionStatus() == SH_ConnectionLost )
    	{
			if( CaSh_Connection::m_p_instance->TryConnect( &CaSh_Connection::m_sh_info ) )
			{
				Api::Log::Print(Api::Log_level::Debug, "____ SmartHub Connected ____");
								
				if( CaSh_Connection::m_p_instance->m_IsHdlUpdatingEnabled )
					CaSh_Connection::m_p_instance->m_waiting_time_for_event = WAITING_TIME_FOR_FPGA_100MS_TIMEOUT;
				else
					CaSh_Connection::m_p_instance->m_waiting_time_for_event = WAITING_TIME_FOR_EVENT_DEFAULT;

				
				CaSh_Connection::m_p_instance->m_SHConnectionStatus 	= SH_Waiting2SendRqstConn;
				CaSh_Connection::m_p_instance->SendConnectionRequest();
			}
			else
			{
				OS::Time::Delay_ms(WAITING_TIME_FOR_EVENT_IF_NO_SERVER_RUNNING);
			}
    	}
    	else if( OS::Event::Wait( CaSh_Connection::m_p_instance->event_socket, MASTER_ETHR2CP_CLOSE_SOCKET_EVENT | MASTER_ETHR2CP_RESTART_CONNECTION_EVENT , false , CaSh_Connection::m_p_instance->m_waiting_time_for_event ,&syncStatus) )
		{    		
			if( syncStatus == MASTER_ETHR2CP_CLOSE_SOCKET_EVENT )
			{
				CaSh_Connection::m_p_instance->m_SHConnectionStatus = SH_ConnectionLost;
				
				//Enviamos a la maquina de estados del posicionador desconexion
				if( g_pCanR2cpMasterEventsHandle )
				{
					OS::Event::Set(g_pCanR2cpMasterEventsHandle, MASTER_CANR2CP_SMARTHUB_EVENT );
				}
				
				printf("\n\r ___ SmartHub DisConnected ___");				
			}
	    	else if( CaSh_Connection::m_p_instance->m_IsHdlUpdatingEnabled )
	    	{
	    		CHdlUpdateServer::GetInstance()->CheckTasks_100mS( );	    		
	    	}
		}
	}
}

CaSh_Connection::CaSh_Connection(void):	API::RTCS::CaSocket( R2CP_DECODER ), m_SHConnectionStatus(SH_DisConnected)
{
	m_p_instance 				= this;
	m_IsHdlUpdatingEnabled		= false;
}

CaSh_Connection::~CaSh_Connection(){
	
}

CaSh_Connection* CaSh_Connection::GetInstance( void )
{
	if( m_p_instance == nullptr )
	{
		m_p_instance = new CaSh_Connection();
	}
	return m_p_instance;
}

bool CaSh_Connection::TryConnect( tsmarthubinfo *pShInfo )
{	
	if( pShInfo )
	{
		if( OS::RTCS::get_link_active() )
		{
			if( Connect( pShInfo->base_port, pShInfo->sh_ip, pShInfo->rx_size, pShInfo->tx_size, 1 ) == true )
			{
				// Try to connect with the server
				if(AcceptServiceSocket() == false)
				{
					Disconnect();
					return false;
				}
				
				return true;
			}
		}
		else
		{
			Api::Log::Print( Api::Log_level::Info, "[ Sh_Connection ] NO ETH LINK ");
		}
	}
	return false;
}


/*event_socket
* 0x01---> CallbackAccept
* 0x02---> CallbackDisconnect
* */
	
void CaSh_Connection::Abort_Connection( void )
{
	if( m_p_instance == nullptr )
		return;
	
	m_p_instance->Disconnect();
}

//This method is employed to restart the connection from outside
void CaSh_Connection::Restart_Connection(void)
{
	OS::Event::Set( event_socket , MASTER_ETHR2CP_RESTART_CONNECTION_EVENT);
}

void CaSh_Connection::EnableHdlUpdating(bool Enable)
{
	m_IsHdlUpdatingEnabled = Enable;
	
	if( Enable )
	{
		m_waiting_time_for_event = WAITING_TIME_FOR_FPGA_100MS_TIMEOUT;
		//Una vez que esta actualizando, poner un sistem message para evitar dar rayos
		R2CP::AddSystemMessagefromGenerator( R2CP::SMType_UserConfirm_InbRx_PhaseStandby_NoAbort_Save , R2CP::XRAY_DISABLED_BY_NODE_UPGRATING , true);
	}
	else
	{
		m_waiting_time_for_event = WAITING_TIME_FOR_EVENT_DEFAULT;
	}

	Restart_Connection();
}

void CaSh_Connection::OnClose(unsigned char closeEvent)
{
	OS::Event::Set( event_socket , MASTER_ETHR2CP_CLOSE_SOCKET_EVENT );
		
	if( g_pGenMasterEventsHandle )
	{
		OS::Event::Set( g_pGenMasterEventsHandle , GENMASTER_SH_DISCONNECTION_EVENT );
	}
	
	printf("\n\rON CLOSE");
}

void CaSh_Connection::OnError(unsigned char errorEvent)
{
	//Enviar system message
}

void CaSh_Connection::SendConnectionRequest(void)
{
	m_SHConnectionStatus = SH_Connected;
	R2CP::CaDataDicGen::GetInstance()->Network_ConnectionRequest_Event(m_sh_info.local_shid, m_sh_info.local_id);
}

bool CaSh_Connection::OnAccept(int serviceSocket)
{
	return true;
}

void CaSh_Connection::OnReceive(unsigned char * buf, int len)
{	
	if( R2CP_Eth )
	{
		R2CP_Eth->ProcessMessage(buf);
	}
}
