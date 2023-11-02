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
#include "CaDataDicGen.h"


/******************************************************************************************************************/
//												GENERATOR
/******************************************************************************************************************/

namespace R2CP
{


    void CaDataDicGen::Generator_DataBank_ExposureAcceptance( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo )
    {
        if(MessageInfo == nullptr)  return;
        if(!m_p_RadInterface_) return;
        if(Access != DATADIC_ACCESS_ANSWER_EVENT) return;
        if(nData != 3) return;

    }

    void CaDataDicGen::Generator_DataBank_DefineProcedure( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo )
    {
        if(MessageInfo == nullptr)  return;
        if(!m_p_RadInterface_) return;
        if(Access != DATADIC_ACCESS_ANSWER_EVENT) return;
        unsigned char id = 0;
        if( MessageInfo->SubIndex == GENERATOR_DATA_BANK_DEFINE_PROCEDURE_V5){
            id = m_p_RadInterface_->MET_Generator_Update_ProcedureV5(pData);
        }else id = m_p_RadInterface_->MET_Generator_Update_ProcedureV6(pData);

    }


    void CaDataDicGen::Generator_ExposureManagement_GeneratorStatus( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo )
    {
        if(MessageInfo == nullptr)  return;
        if(!m_p_RadInterface_) return;
        if(Access != DATADIC_ACCESS_ANSWER_EVENT) return;

        if( MessageInfo->SubIndex == GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V5){
            m_p_RadInterface_->MET_Generator_Set_StatusV5(pData);
        }else m_p_RadInterface_->MET_Generator_Set_StatusV6(pData);
        
    }

    void CaDataDicGen::Generator_ExposureManagement_RadPostExposure( tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo ){
        if(MessageInfo == nullptr)  return;
        if(!m_p_RadInterface_) return;
        if(Access != DATADIC_ACCESS_ANSWER_EVENT) return;


        float kV = ((float) pData[4] * 256. + (float) pData[5]) / 10.;
        float mA = ((float) pData[6] * 65536. + (float) pData[7] * 256. + (float) pData[8]) / 100.;
        float ms = ((float) pData[9] * 65536. + (float) pData[10] * 256. + (float) pData[11]) / 100.;
        float mAs = ms*mA/1000;
        //float mAs = ((float) pData[12] * 65536. + (float) pData[13] * 256. + (float) pData[14]) / 1000.;

        std::string focus ;
        if(pData[15]) focus = "LARGE";
        else focus = "SMALL";

        std::string result;

        switch(pData[19]){
        case 0: result = "EXPOSURE NOT-STARTED"; break;
        case 1: result = "EXPOSURE COMPLETED"; break;
        case 2: result = "EXPOSURE COMPLETED AEC"; break;
        case 3: result = "EXPOSURE AEC TIMEOUT"; break;
        case 4: result = "EXPOSURE PUSH BUTTON RELEASE"; break;
        case 5: result = "EXPOSURE ABORTED WITH ERROR"; break;
        case 6: result = "EXPOSURE ABORTED BEFORE READY"; break;
        case 7: result = "EXPOSURE ABORTED PANEL NOT READY"; break;
        case 8: result = "EXPOSURE ABORTED BECAUSE INHIBIT"; break;
        case 9: result = "EXPOSURE ABORTED BECAUSE OF EXPWIN OUT"; break;
        }

        /*
        qDebug() << " POST EXPOSURE: " << (pData[0] * 256 + pData[1]) << " ----------------- ";
        qDebug() << " Procedure: " << pData[2];
        qDebug() << " DB Number: " << pData[3];
        qDebug() << " EXIT: " << result;
        qDebug() << " Focus: " << focus;
        qDebug() << " kV: " << kV;
        qDebug() << " mAs: " << mAs;
        qDebug() << " mA: " << mA;
        qDebug() << " mS: " << ms;
        */

        /* TBD
        unsigned char foc = exposureManager::_FOCUS_SMALL;
        if(pData[15]) foc = exposureManager::_FOCUS_LARGE;

        COMMUNICATION->PostExposureEvent(pData[3], foc, kV, mAs,mA,ms,pData[19]);
        */
    }




    // Get Functions
    void CaDataDicGen::Generator_Get_StatusV5(void){
        (void)m_Type_-> Get(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                GENERATOR_COMMANDS_ENTRY,
                                GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V5,
                                0,
                                nullptr);

    }

    void CaDataDicGen::Generator_Get_StatusV6(void){
        (void)m_Type_-> Get(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                GENERATOR_COMMANDS_ENTRY,
                                GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V6,
                                0,
                                nullptr);

    }


    void CaDataDicGen::Generator_Set_SkipPulse_Databank(unsigned char dbId, unsigned char nskip){
        unsigned char pData[27];

        pData[0] = dbId;// Databank Id
        pData[1] = nskip; // Plse to be skip

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                GENERATOR_COMMANDS_ENTRY,
                                GENERATOR_LOAD_SKIP_PULSE_DB,
                                2,
                                pData);
    }

    void CaDataDicGen::Generator_Assign_SkipPulse_Databank(unsigned char procedureId, unsigned char dbId){
        unsigned char pData[27];

        pData[0] = procedureId;// Databank Id
        pData[1] = dbId; // Plse to be skip

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                GENERATOR_COMMANDS_ENTRY,
                                GENERATOR_ASSIGN_SKIP_PULSE_DB,
                                2,
                                pData);
    }


    void CaDataDicGen::Generator_Set_2D_Databank(unsigned char dbId, unsigned char focus, float KV, float MAS, unsigned long tmo){
        if((dbId < 1) || (dbId >= DB_LastId)) return;

        unsigned char pData[27];

        pData[0] = dbId; // Databank Id
        pData[1] = DB_LastId; // Image Identifier
        pData[2] = DB_Tech_2 | DB_Tech_AutoMode_NotModify | DB_Tech_AAdjustParam_NotModify;

        // kV * 10
        unsigned short kV = (unsigned short) (KV * 10.0);
        pData[3] = (unsigned char ) ((kV >> 8) & 0xFF);
        pData[4] = (unsigned char ) ((kV >> 0) & 0xFF);

        // mAs * 1000
        unsigned long mAs = (unsigned long) ( MAS * 1000.0);
        pData[5] = (unsigned char ) ((mAs >> 16) & 0xFF);
        pData[6] = (unsigned char ) ((mAs >> 8) & 0xFF);
        pData[7] = (unsigned char ) ((mAs >> 0) & 0xFF);

        // mA * 100
        unsigned long mA = ((unsigned long) 250 * 100.0);
        pData[8] = (unsigned char ) ((mA >> 16) & 0xFF);
        pData[9] = (unsigned char ) ((mA >> 8) & 0xFF);
        pData[10] = (unsigned char ) ((mA >> 0) & 0xFF);

        // mS * 100
        unsigned long mS = ((unsigned long) 5000 * 100.0);
        pData[11] = (unsigned char ) ((mS >> 16) & 0xFF);
        pData[12] = (unsigned char ) ((mS >> 8) & 0xFF);
        pData[13] = (unsigned char ) ((mS >> 0) & 0xFF);

        // MinIntg. Time
        unsigned short  mInt= 1;
        pData[14] = (unsigned char ) (((mInt * 1) >> 8) & 0xFF);
        pData[15] = (unsigned char ) (((mInt * 1) >> 0) & 0xFF);

        // MaxIntg. Time
        unsigned short  MInt= tmo;
        pData[16] = (unsigned char ) (((MInt * 1) >> 8) & 0xFF);
        pData[17] = (unsigned char ) (((MInt * 1) >> 0) & 0xFF);

        // Focal spot

        /*TBD 
        if(focus == exposureManager::_FOCUS_LARGE) pData[18] = 1;
        else pData[18] = 0;
        */

        pData[19] = 0; // NA
        pData[20] = 0; // NA
        pData[21] = 0; // NA
        pData[22] = 0; // NA

        // FPS * 10
        unsigned short  FPS = 0;
        pData[23] = (unsigned char ) (((FPS * 10) >> 8) & 0xFF);
        pData[24] = (unsigned char ) (((FPS * 10) >> 0) & 0xFF);


        pData[25] = 0; // NA
        pData[26] = 0; // NA

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                GENERATOR_COMMANDS_ENTRY,
                                GENERATOR_RAD_DATA_BANK_LOAD_V6,
                                27,
                                pData);
    }

    void CaDataDicGen::Generator_Set_Ms_Databank(unsigned char dbId, float MS){
        if((dbId < 1) || (dbId >= DB_LastId)) return;

        unsigned char pData[5];
        pData[0] = dbId; // Databank Id

        // mS * 100
        unsigned long mS = ((unsigned long) MS * 100.0);
        pData[1] = (unsigned char ) ((mS >> 16) & 0xFF);
        pData[2] = (unsigned char ) ((mS >> 8) & 0xFF);
        pData[3] = (unsigned char ) ((mS >> 0) & 0xFF);
        pData[4] = 0;

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                GENERATOR_COMMANDS_ENTRY,
                                GENERATOR_RAD_EXPOSURE_PARAMETER_MS,
                                5,
                                pData);

    }


    void CaDataDicGen::Generator_Get_Databank(unsigned char dbId){
        if((dbId < 1) || (dbId >= DB_LastId)) return;
        unsigned char pData[1];

        pData[0] = dbId;
        (void)m_Type_-> Get(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                GENERATOR_COMMANDS_ENTRY,
                                GENERATOR_RAD_DATA_BANK_LOAD_V6,
                                1,
                                pData);
    }

    void CaDataDicGen::Generator_Set_3D_Databank(unsigned char dbId, unsigned char focus, float KV, float MA, float MS, float MT){
        if((dbId < 1) || (dbId >= DB_LastId)) return;

        unsigned char pData[27];

        pData[0] = dbId; // Databank Id
        pData[1] = DB_LastId; // Image Identifier
        pData[2] = DB_Tech_3 | DB_Tech_AutoMode_NotModify | DB_Tech_AAdjustParam_NotModify;

        // kV * 10
        unsigned short kV = (unsigned short) (KV * 10.0);
        pData[3] = (unsigned char ) ((kV >> 8) & 0xFF);
        pData[4] = (unsigned char ) ((kV >> 0) & 0xFF);

        // mAs * 1000
        unsigned long mAs = (unsigned long) ( 640 * 1000.0);
        pData[5] = (unsigned char ) ((mAs >> 16) & 0xFF);
        pData[6] = (unsigned char ) ((mAs >> 8) & 0xFF);
        pData[7] = (unsigned char ) ((mAs >> 0) & 0xFF);

        // mA * 100
        unsigned long mA = (unsigned long)( MA * 100.0);
        pData[8] = (unsigned char ) ((mA >> 16) & 0xFF);
        pData[9] = (unsigned char ) ((mA >> 8) & 0xFF);
        pData[10] = (unsigned char ) ((mA >> 0) & 0xFF);

        // mS * 100
        unsigned long mS = (unsigned long) ( MS * 100.0);
        pData[11] = (unsigned char ) ((mS >> 16) & 0xFF);
        pData[12] = (unsigned char ) ((mS >> 8) & 0xFF);
        pData[13] = (unsigned char ) ((mS >> 0) & 0xFF);

        // MinIntg. Time
        unsigned short  mInt= 1;
        pData[14] = (unsigned char ) (((mInt * 1) >> 8) & 0xFF);
        pData[15] = (unsigned char ) (((mInt * 1) >> 0) & 0xFF);

        // MaxIntg. Time
        unsigned short  MInt= MT;
        pData[16] = (unsigned char ) (((MInt * 1) >> 8) & 0xFF);
        pData[17] = (unsigned char ) (((MInt * 1) >> 0) & 0xFF);

        // Focal spot
        // TBD if(focus == exposureManager::_FOCUS_LARGE) pData[18] = 1;
        // else pData[18] = 0;

        pData[19] = 0; // NA
        pData[20] = 0; // NA
        pData[21] = 0; // NA
        pData[22] = 0; // NA

        // FPS * 10
        unsigned short  FPS = 0;
        pData[23] = (unsigned char ) (((FPS * 10) >> 8) & 0xFF);
        pData[24] = (unsigned char ) (((FPS * 10) >> 0) & 0xFF);


        pData[25] = 0; // NA
        pData[26] = 0; // NA

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                GENERATOR_COMMANDS_ENTRY,
                                GENERATOR_RAD_DATA_BANK_LOAD_V6,
                                27,
                                pData);
    }

    void CaDataDicGen::Generator_AssignDbToProc(unsigned char db, unsigned char proc, unsigned char index){

        unsigned char pData[3];

        pData[0] = proc; // Procedure Id
        pData[1] = index; // Exposure Index
        pData[2] = db; // Databank id

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                GENERATOR_COMMANDS_ENTRY,
                                GENERATOR_DATA_BANK_ASSIGN_EXPOSURE,
                                3,
                                pData);

    }


    void CaDataDicGen::Generator_verifyDbToProc(unsigned char proc, unsigned char index){

        unsigned char pData[2];

        pData[0] = proc; // Procedure Id
        pData[1] = index; // Exposure Index

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                GENERATOR_COMMANDS_ENTRY,
                                GENERATOR_DATA_BANK_EXPOSURE_ACCEPTANCE,
                                2,
                                pData);

    }

    void CaDataDicGen::Generator_startExposure(void){

        unsigned char pData[1];

        pData[0] = 1; // Start

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                GENERATOR_COMMANDS_ENTRY,
                                GENERATOR_EXPOSURE_MANAGEMENT_START_STOP_EXPOSURE,
                                1,
                                pData);

    }

    void CaDataDicGen::Generator_stopExposure(void){

        unsigned char pData[1];

        pData[0] = 0; // Stop

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                GENERATOR_COMMANDS_ENTRY,
                                GENERATOR_EXPOSURE_MANAGEMENT_START_STOP_EXPOSURE,
                                1,
                                pData);

    }

    void CaDataDicGen::Generator_RadDataBank_Load(tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
    {
        if(MessageInfo == nullptr) 	return;
        if( Access != DATADIC_ACCESS_ANSWER_EVENT) return;
        if( MessageInfo->SubIndex != GENERATOR_RAD_DATA_BANK_LOAD_V6) return;
        if((pData[0] < 1) || (pData[0] >= DB_LastId)) return;
        if(pData[1] != DB_LastId) return;

        m_p_RadInterface_->DbDefinitions[pData[0]].DatabankId = pData[0];
        m_p_RadInterface_->DbDefinitions[pData[0]].ImagingSystemProtocolId = pData[1];
        m_p_RadInterface_->DbDefinitions[pData[0]].TechMode.value = pData[2];
        m_p_RadInterface_->DbDefinitions[pData[0]].kV10.value = pData[3] << 8 | pData[4];
        m_p_RadInterface_->DbDefinitions[pData[0]].mAs1000.value = (int)(pData[5] <<16 | (int)pData[6] <<8 | pData[7]);
        m_p_RadInterface_->DbDefinitions[pData[0]].mA100.value = (int)(pData[8] <<16 | (int)pData[9] <<8 | pData[10]);
        m_p_RadInterface_->DbDefinitions[pData[0]].ms100.value = (int)(pData[11]<<16 | (int)pData[12]<<8 | pData[13]);
        m_p_RadInterface_->DbDefinitions[pData[0]].MinIntegrationTime.value	= (unsigned short )(pData[14]<<8 | pData[15]);
        m_p_RadInterface_->DbDefinitions[pData[0]].MaxIntegrationTime.value	= (unsigned short )(pData[16]<<8 | pData[17]);
        m_p_RadInterface_->DbDefinitions[pData[0]].FocalSpot = pData[18];
        m_p_RadInterface_->DbDefinitions[pData[0]].TargetDose.Fields.AECSensitivity_DoseTarget 	= pData[19];
        m_p_RadInterface_->DbDefinitions[pData[0]].TargetDose.Fields.AECDensity					= pData[20];
        m_p_RadInterface_->DbDefinitions[pData[0]].IonChamber.value								= pData[21];
        m_p_RadInterface_->DbDefinitions[pData[0]].Spare_1										= pData[22];
        m_p_RadInterface_->DbDefinitions[pData[0]].FPS10.value									= (unsigned short )( pData[ 23 ] << 8 | pData[ 24 ]);
        m_p_RadInterface_->DbDefinitions[pData[0]].TrakingId									= pData[25];
        m_p_RadInterface_->DbDefinitions[pData[0]].Spare_2										= pData[26];

        
    }

    void CaDataDicGen::Generator_RadDataBank_Load_Ms(tDataDicAccess Access, unsigned char *pData, unsigned short  nData,  tInfoMessage *MessageInfo)
    {
        
        if(MessageInfo == nullptr) 	return;
        if( Access != DATADIC_ACCESS_ANSWER_EVENT) return;
        if( MessageInfo->SubIndex != GENERATOR_RAD_EXPOSURE_PARAMETER_MS) return;
        if((pData[0] < 1) || (pData[0] >= DB_LastId)) return;
        if(pData[1] != DB_LastId) return;

        if(pData[1] == 0){            
            return;
        }
        m_p_RadInterface_->DbDefinitions[pData[0]].ms100.value = (int)(pData[2]<<16 | (int)pData[3]<<8 | pData[4]);

        

    }



}//namespace R2CP




