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
#include <string>

#include "CaDataDicGen.h"

namespace R2CP
{
	const unsigned char R2CP_VERSION_VERSION 		= 0x05;
	const unsigned char R2CP_VERSION_SUBVERSION		= 0x05;
	const unsigned char R2CP_VERSION_REVISION		= '0';
	
	bool	CaDataDicGen::m_EnableLogInfo = false;
	CR2CP_Eth * CaDataDicGen::m_Type_ = nullptr;
	CaDataDicGen *CaDataDicGen::m_p_instance_ = nullptr;
	void (*CaDataDicGen::m_fcb_log_) ( unsigned char LogLevel , const char *string, ... ) = nullptr;
	
	/*Interface Pointers*/
	CaDataDicInterface		  	*CaDataDicGen::m_p_DataDicInterface_	= nullptr;
    CaDataDicNetworkInterface   *CaDataDicGen::m_p_NetworkInteface_		= nullptr;
    CaDataDicRadInterface	  	*CaDataDicGen::m_p_RadInterface_		= nullptr;
	//CaDataDicR2CPInterface 	  	*CaDataDicGen::m_p_R2CPInterface_		= nullptr;
	CaDataDicSystemInterface  	*CaDataDicGen::m_p_SystemInterface_		= nullptr;
	CaDataDicPatientInterface 	*CaDataDicGen::m_p_PatientInterface_	= nullptr;
	CaDataDicServiceInterface	*CaDataDicGen::m_p_ServiceInterface_	= nullptr;
	
	/*!
	 * \brief Defines lenght for entries which lenght is variable (blocks).
	*/
	#define UNDEFINED_LENGHT        (unsigned char )-1   
	
	const tEntry Row_00HEX[] = { // R2CP
	//	SubIndex,					Func,					tDataDicAccess,								GS,			SS,		    AS		ES
        {R2CP_PROTOCOL_VERSION,		CaDataDicGen::R2CP_ProtocolVersion,		(tDataDicAccess)DATADIC_ACCESS_ANSWER_EVENT, 0,			3,			3,		3},
	};
	

	
	const tEntry Row_20HEX[] = { // GENERATOR
        {GENERATOR_DATA_BANK_DEFINE_PROCEDURE_V5,                       CaDataDicGen::Generator_DataBank_DefineProcedure,                   DATADIC_ACCESS_ANSWER_EVENT,		0,		0,      7,      7},
        {GENERATOR_DATA_BANK_EXPOSURE_ACCEPTANCE,                       CaDataDicGen::Generator_DataBank_ExposureAcceptance,                DATADIC_ACCESS_ANSWER_EVENT,		0,		0,		3,		3},
        {GENERATOR_DATA_BANK_DEFINE_PROCEDURE_V6,                       CaDataDicGen::Generator_DataBank_DefineProcedure,                   DATADIC_ACCESS_ANSWER_EVENT,		0,		0,      9,      9},
        {GENERATOR_RAD_DATA_BANK_LOAD_V6,								CaDataDicGen::Generator_RadDataBank_Load,							DATADIC_ACCESS_ANSWER_EVENT,	    0,		0,		27,		27},
        {GENERATOR_RAD_EXPOSURE_PARAMETER_MS,							CaDataDicGen::Generator_RadDataBank_Load_Ms,					    DATADIC_ACCESS_ANSWER_EVENT,	    0,		0,		UNDEFINED_LENGHT,		UNDEFINED_LENGHT},
        {GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V5,				CaDataDicGen::Generator_ExposureManagement_GeneratorStatus,			DATADIC_ACCESS_ANSWER_EVENT,		0,		0,		14,		14},
        {GENERATOR_EXPOSURE_MANAGEMENT_RAD_POST_EXPOSURE,				CaDataDicGen::Generator_ExposureManagement_RadPostExposure,			DATADIC_ACCESS_ANSWER_EVENT,        0,		0,		21,		21},
        {GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V6,				CaDataDicGen::Generator_ExposureManagement_GeneratorStatus,			DATADIC_ACCESS_ANSWER_EVENT,		0,		0,		14,		14},
	};
	
	
	const tEntry Row_80HEX[] = { // SYSTEM MESSAGE
	//	SubIndex,														Func,												tDataDicAccess,			GetSize,	SetSize
        {SYSTEM_SYSTEM_MESSAGE,											CaDataDicGen::System_SystemMessage,			DATADIC_ACCESS_ANSWER_EVENT,		0,		0,		UNDEFINED_LENGHT,UNDEFINED_LENGHT},
        //{SYSTEM_ALL_SYSTEM_MESSAGES,									CaDataDicGen::System_AllSystemMessages,		DATADIC_ACCESS_ANSWER_EVENT,		0,		0,		0,		0},
		{SYSTEM_VD_LOAD_DESKTOP,										CaDataDicGen::System_LoadDesktop,			DATADIC_ACCESS_ANSWER_EVENT,		0,		0,	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_DESKTOP_DISPLAY,										CaDataDicGen::System_DesktopDisplay,		DATADIC_ACCESS_ANSWER_EVENT,		0,		0,	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_NUMERIC_PROPERTY_SETUP,								CaDataDicGen::System_NumericPropertySetup, 	DATADIC_ACCESS_GET,					9,		0, 	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_NUMERIC_PROPERTY_VALUE,								CaDataDicGen::System_NumericPropertyValue,	DATADIC_ACCESS_ANSWER_EVENT, 		0, 		0, 	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_STRING_PROPERTY,										CaDataDicGen::System_StringProperty,		DATADIC_ACCESS_SET_GET,				9, 		0, 	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_NUMERIC_VALUE_TABLE,									CaDataDicGen::Sytem_NumericValueTable,		DATADIC_ACCESS_SET_GET, 			9, 		0, 	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_NUMERIC_VALUE_TABLE_ITEM,							CaDataDicGen::Sytem_NumericValueTableItem,	DATADIC_ACCESS_SET_GET, 			10,		0, 	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_STRING_TABLE,										CaDataDicGen::System_StringTable,			DATADIC_ACCESS_SET_GET, 			9, 		0, 	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_SINGLE_USER_REQUEST,									CaDataDicGen::System_SingleUserRequest,		DATADIC_ACCESS_ANSWER_EVENT,		0,		0,	0x05 , 0x05},
		{SYSTEM_VD_USER_REQUEST_TABLE,									CaDataDicGen::System_UserRequestTable,		DATADIC_ACCESS_ANSWER_EVENT,		0,		0,	0x05,  0x05}	
	};
	
	const tEntry Row_A0HEX[] = { // NETWORK
		//	SubIndex,													Func,										tAccess,										GetSize,	SetSize
		{ NETWORK_CONNECTION_CHANGED,									CaDataDicGen::Network_ConnectionChanged,	(tDataDicAccess)(DATADIC_ACCESS_ANSWER_EVENT),	0,		  	0,		2,		2 },
		{ NETWORK_NODESTATUS,											CaDataDicGen::Network_NodeStatus,			(tDataDicAccess)(DATADIC_ACCESS_ANSWER_EVENT),	0,		  	0,		2,		2 },
		{ NETWORK_HEARTBEAT,											CaDataDicGen::Network_HeartBeat,			(tDataDicAccess)(DATADIC_ACCESS_GET),			2,		 	0,		0,		0 },
		{ NETWORK_CONNECTION_REQUEST,									CaDataDicGen::Network_ConnectionRequest,	(tDataDicAccess)(DATADIC_ACCESS_ANSWER_EVENT),	0,		  	0,		1,		1 },
		{ NETWORK_CONFIGURATION,										CaDataDicGen::Network_IpConfig,				(tDataDicAccess)(DATADIC_ACCESS_SET_GET),		0,		  	16,		0,		0 }, 
		{ NETWORK_FILECONFIGUPDATE,										CaDataDicGen::Network_FileConfigUpdate,		(tDataDicAccess)(DATADIC_ACCESS_ANSWER_EVENT),	0,		  	0,		0,		0 },
		{ NETWORK_SNAPSHOT,												CaDataDicGen::Network_SnapShot,				(tDataDicAccess)(DATADIC_ACCESS_ANSWER_EVENT),	0,			0,		1,		1 },	
		{ NETWORK_BACKUP_SNAPSHOT,										CaDataDicGen::Network_BackupSnapShot,		(tDataDicAccess)(DATADIC_ACCESS_SET),			0,			2,		0,		0 },	
		{ NETWORK_RESTORE,												CaDataDicGen::Network_Restore,				(tDataDicAccess)(DATADIC_ACCESS_SET),			0,			1,		0,		0 },
		{ NETWORK_CALIBRATION_UPDATE,									CaDataDicGen::Network_CalConfigUpdate,		(tDataDicAccess)(DATADIC_ACCESS_ANSWER_EVENT),	0,			0,		0,		0 },
	};
	
	/**/
	const tEntry Row_E0HEX[] = { // PATIENT WORKFLOW
	//	SubIndex,														Func,															tAccess,				GetSize,	  SetSize
        
        {PATIENT_PROCEDURE_ACTIVATE,									nullptr,												DATADIC_ACCESS_SET_GET,					1,		  	6,		0,		0},
		/*
		{PATIENT_PROCEDURE_DEFAULT,										CaDataDicGen::Patient_Procedure_Default,				DATADIC_ACCESS_SET_GET,					0,		  	6,		0,		0},
		{PATIENT_PROCEDURE_CLEAR_ALL,									CaDataDicGen::Patient_Procedure_ClearAll,				DATADIC_ACCESS_SET,						0,			0,		0,		0},
		{PATIENT_REDEFINE_HANDFOOTSWITCH_ACTIVATION,					CaDataDicGen::Patient_Redefine_HandFootswitchActivation,DATADIC_ACCESS_SET,						0,			2,		0,		0},
		{PATIENT_REDEFINE_WORKSTATION,									CaDataDicGen::Patient_Redefine_Workstation,				DATADIC_ACCESS_SET,						0,			2, 		0,		0},
		{PATIENT_MISCELLANEOUS_SYNC_UP,									CaDataDicGen::Patient_Miscellaneous_Sync_Up,			DATADIC_ACCESS_GET,						0,			0, 		0,		0},
		{PATIENT_MISCELLANEOUS_POWEROFF,								CaDataDicGen::Patient_Miscellaneous_PowerOff,			DATADIC_ACCESS_ALL_MEMBERS,				0,			2 ,		2,		2},
		{PATIENT_MISCELLANEOUS_POWEROFF_CANCEL,							CaDataDicGen::Patient_Miscellaneous_PowerOffCancel,		DATADIC_ACCESS_SET,						0,			0,		0,		0},
		{PATIENT_MISCELLANEOUS_POWEROFF_CONFIRM,						CaDataDicGen::Patient_Miscellaneous_PowerOffConfirm,	DATADIC_ACCESS_SET,						0,			1,		0,		0},
		{PATIENT_MISCELLANEOUS_WORKINGMODE,								CaDataDicGen::Patient_Miscellaneous_WorkingMode,		DATADIC_ACCESS_SET_GET,					0,			2,		0,		0},
		{PATIENT_MISCELLANEOUS_POSITIONER_CALIBRATION_NUMBER,			CaDataDicGen::Patient_Miscellaneous_Pos_Cal_Number,		DATADIC_ACCESS_SET_GET,					0,			1,		0,		0},
        */
	};
	
	const tEntry Row_C0HEX[] = {
	//	SubIndex,														Func,														tAccess,					GetSize,	  SetSize
		{SERVICE_STANDARD_LICENSE,										CaDataDicGen::Service_StandardLicense,						DATADIC_ACCESS_GET,			0,			0,		0,		0},
		{SERVICE_EXTENDED_LICENSE,										CaDataDicGen::Service_ExtendedLicense,						DATADIC_ACCESS_GET,			0,			0,		0,		0},
	};

	
	const tRow GenDictionary[] = {
		{NETWORK_COMMANDS_ENTRY,			(sizeof(Row_A0HEX) / sizeof(tEntry)),	Row_A0HEX },	// NETWORK
		{R2CP_COMMANDS_ENTRY,       		(sizeof(Row_00HEX)/sizeof(tEntry)),		Row_00HEX },	// R2CP
		{GENERATOR_COMMANDS_ENTRY,			(sizeof(Row_20HEX)/sizeof(tEntry)),		Row_20HEX },	// GENERATOR
        {SYSTEM_COMMANDS_ENTRY,				(sizeof(Row_80HEX)/sizeof(tEntry)),		Row_80HEX },	// SYSTEM MESSAGE
		{PATIENT_COMMANDS_ENTRY,			(sizeof(Row_E0HEX)/sizeof(tEntry)),		Row_E0HEX },	// PATIENT WORKFLOW
		{SERVICE_COMMANDS_ENTRY,			(sizeof(Row_C0HEX)/sizeof(tEntry)),		Row_C0HEX }		// SERVICE
	};
	
	/*!
	 * \brief Object dictionary defined, ready to export.
	*/
	const tTable GeneratorDataDictionary = {(sizeof(GenDictionary)/sizeof(tRow)) ,GenDictionary};
		
	
	
	CaDataDicGen *CaDataDicGen::GetInstance(void)
	{
			
		if (m_p_instance_ == nullptr) 
            m_p_instance_ = new CaDataDicGen(APPLICATION_NODE_ID);
		return m_p_instance_;
	}
	
	CaDataDicGen::CaDataDicGen(unsigned char Node) : CaDataDic( Node )
	{
		/* Initialize mutex attributes */
        // datadic_mutex = OS::Mutex::Create();
		m_Current_Node_Dest_	= 0;
		m_IsGetRequest_Pending_ = false;
		
		pDataDictionary 			= &GeneratorDataDictionary;
		
		mProtocolVersion.Version	= R2CP_VERSION_VERSION;
		mProtocolVersion.SubVersion	= R2CP_VERSION_SUBVERSION;
		mProtocolVersion.Revision	= R2CP_VERSION_REVISION;
		
		m_fcb_log_					= nullptr;
	}
	
	CaDataDicGen::~CaDataDicGen()
	{
		
	}
	

	
	void CaDataDicGen::Initialitation(void)
	{
		if(m_p_DataDicInterface_)
			m_p_DataDicInterface_->Initialitation();
		
		//if(m_p_R2CPInterface_)
		//	m_p_R2CPInterface_->Initialitation();

        m_p_RadInterface_ = &radInterface;
        m_p_SystemInterface_ = &systemInterface;
        protocolVersion.Version = 0;
        protocolVersion.SubVersion = 0;

	}
	
	void CaDataDicGen::SetCommunicationForm( CR2CP_Eth *Type)
	{
		m_Type_ =  Type;
	}
	
	unsigned char CaDataDicGen::GetNodeEvent(bool IsFromCp)
	{
		unsigned char Node =  m_Current_Node_Dest_;

		if(!m_IsGetRequest_Pending_ && !IsFromCp)//Pendiente un set y solicita un evento
			Node = BROADCAST_NODE_ID;
		
		if(IsFromCp)
			m_IsGetRequest_Pending_ = false;
		
		return Node;
	}
	
	void CaDataDicGen::SetNodeEvent(unsigned char Node, bool IsFromGet)
	{
		if(IsFromGet)
			m_IsGetRequest_Pending_ = true;
		else
			m_IsGetRequest_Pending_ = false;
	
		m_Current_Node_Dest_ = Node;
	}
	
	void CaDataDicGen::Lock_DataDicGenMutex(void)
	{
        //OS::Mutex::Lock(datadic_mutex);
	}
	
	void CaDataDicGen::UnLock_DataDicGenMutex(void)
	{
        //OS::Mutex::UnLock(datadic_mutex);
	}

	
	///////////////////////////////////  R2CP   ///////////////////////////////////
	void CaDataDicGen::R2CP_ProtocolVersion( tDataDicAccess Access , unsigned char *pData , unsigned short  nData , tInfoMessage *MessageInfo ) 
	{
        if(	MessageInfo == nullptr ) return;
        m_p_instance_->protocolVersion = *(tDataDicProtocolVersion*) pData;
        m_p_instance_->protocolUpdated = true;
        //qDebug() << "PROTOCOL:"  << m_p_instance_->protocolVersion.Version <<  "." << m_p_instance_->protocolVersion.SubVersion << "." << m_p_instance_->protocolVersion.Revision ;
	}
	
	
	
	bool CaDataDicGen::Check_NotAvailable_Message_vs_Status( unsigned short  index, unsigned short  subindex, tInfoMessage *MessageInfo )
	{
		if(m_p_DataDicInterface_)
			return m_p_DataDicInterface_->CheckNotAvailableMessages_vs_Status( index , subindex );
		return true;	
	}
	
	/*
	//Por defecto lo enviamos al powerControl
	bool CaDataDicGen::ReservedGot( unsigned char Priority, unsigned char Node , unsigned char Index , unsigned char SubIndex , unsigned char *pData , unsigned short  Size )
	{
		if(m_p_R2CPInterface_)
			m_p_R2CPInterface_->ReservedGot( Priority , Node , Index , SubIndex , pData , Size );

		return true;
	}*/
	

    void CaDataDicGen::Protocol_Get_Version(void){

         // Initialize to 0 so it can be checked
         m_p_instance_->protocolUpdated = false;

        (void)m_Type_-> Get(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                R2CP_COMMANDS_ENTRY,
                                R2CP_PROTOCOL_VERSION,
                                0,
                                nullptr);
    }

    void CaDataDicGen::Protocol_Set_Version6(void){

        unsigned char pData[3];
        pData[0] =  6;
        pData[1] =  0;
        pData[2] =  'A';
        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                R2CP_COMMANDS_ENTRY,
                                R2CP_PROTOCOL_VERSION,
                                3,
                                pData);
    }

    void CaDataDicGen::Protocol_Set_Version5(void){

        unsigned char pData[3];
        pData[0] =  5;
        pData[1] =  5;
        pData[2] =  0;
        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                R2CP_COMMANDS_ENTRY,
                                R2CP_PROTOCOL_VERSION,
                                3,
                                pData);
    }



}//namespace R2CP
