/*!
 * \file      CaDataDicGen.cpp
 * \brief     Definition Of R2CP Generator Datatadic.
 * \author   
 * \bug       Unknown bugs
 * \version
 *  \htmlonly
 *		
 *	\endhtmlonly
 * \date      01/08/2012
 * \copyright SEDECAL S.A.
 * \ingroup   R2CPModule
*/

#ifndef __CADATADICGEN__
    #define __CADATADICGEN__

#include "R2CP_Eth.h"
#include "CaDataDic.h"
#include "R2CP_Index.h"
#include "Interfaces/aDataDicRadInterface.h"
#include "Interfaces/aDataDicNetworkInterface.h"
#include "Interfaces/aDataDicPatientInterface.h"
#include "Interfaces/aDataDicSystemInterface.h"
#include "Interfaces/aDataDicR2CPInterface.h"
#include "Interfaces/aDataDicServiceInterface.h"



#define PROTOCOL_VERSION_6	6
#define PROTOCOL_VERSION_5	5

const tDataDicProtocolVersion ProtocolVersionSupported[] =
{
	{ PROTOCOL_VERSION_5 , 5 , 'A' },
	{ PROTOCOL_VERSION_6 , 1 , 'A' },
};

namespace R2CP
{


    class CaDataDicGen : public CaDataDic
    {

	public:
		/*!
		 * Generator Datadic SingleTone
		 */
		static CaDataDicGen *GetInstance(void);
	
		/*!
		 *	\brief DataDic Initialitation
		 */
		void Initialitation(void);
	
		/*!
		 * \brief Set Communication Channel
		 */
		void SetCommunicationForm( CR2CP_Eth *Type);
	
		/*!
		 * \brief Inject Interfaces
		 */
	
		
		/*!
		 * \brief Checks messages versus geneator status
		 */
		virtual bool Check_NotAvailable_Message_vs_Status( unsigned short  index, unsigned short  subindex, tInfoMessage *MessageInfo );

		/*!
		 * \brief Lock Generator Datadic
		 */
		void Lock_DataDicGenMutex(void);

		/*!
		 * \brief Unlock Generator Datadic
		 */
		void UnLock_DataDicGenMutex(void);
	
	
		///////////////////////////////////  R2CP  DATADIC ///////////////////////////////////
        static void R2CP_ProtocolVersion	(tDataDicAccess Access, unsigned char *pData, unsigned short  nData, tInfoMessage *MessageInfo = nullptr);

		
        ////////////////////////////////////GENERATOR INDEX////////////////////////////////////////////////////////////
        static void Generator_DataBank_ExposureAcceptance				( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
        static void Generator_DataBank_DefineProcedure                  ( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
        static void Generator_RadDataBank_Load							( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
        static void Generator_RadDataBank_Load_Ms   					( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
        static void Generator_ExposureManagement_GeneratorStatus		( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
        static void Generator_ExposureManagement_RadPostExposure		( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);


		//////////////////////////////////SYSTEM INDEX////////////////////////////////////////////////////////////////
		static void System_SystemMessage			( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_AllSystemMessages		( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_LoadDesktop				( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_DesktopDisplay			( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_NumericPropertySetup 	( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_NumericPropertyValue 	( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);	
		static void System_StringProperty		 	( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);	
		static void Sytem_NumericValueTable			( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void Sytem_NumericValueTableItem 	( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_StringTable				( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr); 
		static void System_SingleUserRequest		( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_UserRequestTable			( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
					
		
		//////////////////////////////////NETWORK INDEX////////////////////////////////////////////////////////////////
		static void Network_HeartBeat				( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_ConnectionChanged		( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_NodeStatus				( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_ConnectionRequest		( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_IpConfig				( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_FileConfigUpdate		( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_SnapShot				( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_BackupSnapShot			( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_Restore					( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_CalConfigUpdate			( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
	
		//Events
		void Network_IpConfig_Event(udword IpAddress, udword MaskSubNetwork , udword Gateway , udword ShIpAdress);
        void Network_ConnectionRequest_Event(void);
		void Network_GetApplicationNodeStatus(void);
		void Network_GetGeneratorNodeStatus(void);
		void Network_BackupSnapShot_Event ( tBackupSnapShotStatus Status );
		void Network_Restore_Event ( tRestoreStatus Status );
		
		//////////////////////////////////SERVICE INDEX//////////////////////////////////////////////////////////////////
		static void Service_StandardLicense			( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		static void Service_ExtendedLicense			( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);
		
        //////////////////////////////// IMAGE SYSTEM////////////////////////////////////////////////////////////////////
        //static void ImageSystem_CurrentLsb			( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo = nullptr);

        //Events
        void Service_Log_Event(unsigned char *ServiceLog , unsigned short  SizeofLog);
        void Service_StandardLicense_Event (tStandardLicense *StandardLicense);
        void Service_ExtendedLicense_Event (tExtendedLicense *ExtendedLicense);
		
		//Heartbeat 
		bool HeartbeatGot(unsigned char node, unsigned char keyWord, unsigned char status){return true;}
		//bool ReservedGot( unsigned char Priority, unsigned char Node , unsigned char Index , unsigned char SubIndex , unsigned char *pData , unsigned short  Size );
	
		static bool	m_EnableLogInfo;
		
		/*!
		 * \brief Log function Callback
		 */
		static void (*m_fcb_log_) ( unsigned char LogLevel , const char *string, ... );
		

        // Get Functions
        void Protocol_Get_Version(void);
        void Protocol_Set_Version6(void);
        void Protocol_Set_Version5(void);


        void Generator_Get_StatusV5(void);
        void Generator_Get_StatusV6(void);
        void SystemMessages_Get_AllMessages(void);
        void SystemMessages_SetDisableRx(bool state);
		void SystemMessages_SetTestMessage(unsigned long msg);

        bool SystemMessages_Clear_Message(unsigned int ID);


        void Patient_SetupProcedureV5(unsigned char num);
        void Patient_SetupProcedureV6(unsigned char procId, unsigned char param);
        void Patient_ClearAllProcedures(void);
        void Generator_Get_Databank(unsigned char i);
        void Generator_Set_2D_Databank(unsigned char i, bool large_focus, float kV, float mAs, unsigned long tmo);
        void Generator_Set_3D_Databank(unsigned char i, bool large_focus, float kV, float MA, float MS, float MT);
        void Generator_Set_Ms_Databank(unsigned char dbId, float MS);

        void Generator_Set_SkipPulse_Databank(unsigned char dbId, unsigned char nskip);
        void Generator_Assign_SkipPulse_Databank(unsigned char procedureId, unsigned char dbId);

        void Patient_Activate2DProcedurePulse(bool detector, bool grid);

        void Patient_Activate2DAecProcedurePulse(bool grid);
        void Patient_Activate2DAecProcedurePre(bool grid);

        void Patient_Activate3DProcedurePulse(void);
        void Patient_Activate3DAecProcedurePulse(void);
        void Patient_Activate3DAecProcedurePre(void);

        void Generator_AssignDbToProc(unsigned char db, unsigned char proc, unsigned char index);
        void Generator_verifyDbToProc(unsigned char proc, unsigned char index);
        void Generator_startExposure(void);
        void Generator_stopExposure(void);



	private:
		
		/*!
		 * \brief Constructor
		 */
		CaDataDicGen(unsigned char Node);
		/*!
		 * \brief Destructor
		 */
		~CaDataDicGen();
	
		/*!
		 * \brief  Gets Node to send 
		 */
		unsigned char GetNodeEvent(bool IsFromCp = false);
	
		/*!
		 *	\brief Sets Node 
		 */
		void SetNodeEvent(unsigned char Node, bool IsFromGet = false);
		
		/*!
		 * \brief Objet SingleTone Pointer
		 */
		static CaDataDicGen *m_p_instance_;	   
	
		/*!
		 * \brief Current Node pending message
		 */
		unsigned char m_Current_Node_Dest_;
	
		/*!
		 * \brief Get Request Pending
		 */
		bool m_IsGetRequest_Pending_;
	
		/*!
		 * \brief Template Communication Type pointer
		 */
		static CR2CP_Eth * m_Type_;
		
		/*Interface Pointers*/
		static CaDataDicInterface		  	*m_p_DataDicInterface_;
		static CaDataDicNetworkInterface    *m_p_NetworkInteface_;
		//static CaDataDicR2CPInterface 	  	*m_p_R2CPInterface_;		
        static CaDataDicRadInterface	  	*m_p_RadInterface_;
        static CaDataDicSystemInterface  	*m_p_SystemInterface_;
		static CaDataDicPatientInterface 	*m_p_PatientInterface_;		
		static CaDataDicServiceInterface	*m_p_ServiceInterface_;

		typedef struct {
			unsigned char result_code;
			unsigned char focus;
			float kV;
			float mAs;
			float ms;
			float mA;
			unsigned int samples;
		}databank_executed_t;


    public:
		databank_executed_t executed_pulses[10]; //!< Sets the array of per Databank received pulses
		void resetExecutedPulse(void); //!< Resets all the databank exposed pulses

        CaDataDicRadInterface       radInterface;
        CaDataDicSystemInterface    systemInterface;
        tDataDicProtocolVersion     protocolVersion;
        bool protocolUpdated;

        bool isVersionUpdated(void){return protocolUpdated;}
        bool isProtoV6(void){return (protocolVersion.Version == 6) ? true: false;}        
        bool isProtoV5(void){return (protocolVersion.Version == 5) ? true: false;}

	};
};//namespace R2CP

#endif
