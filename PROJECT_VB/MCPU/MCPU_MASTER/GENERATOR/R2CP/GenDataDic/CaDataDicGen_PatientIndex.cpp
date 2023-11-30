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


namespace R2CP
{
	


    void CaDataDicGen::Patient_ClearAllProcedures(void){
        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                PATIENT_COMMANDS_ENTRY,
                                PATIENT_PROCEDURE_CLEAR_ALL,
                                0,
                                nullptr);
    }

    void CaDataDicGen::Patient_SetupProcedureV6(unsigned char procId, unsigned char param){

        if((procId < 1) || (procId >= ProcId_Last)) return;
        unsigned char pData[16];



        if(procId == ProcId_Standard_Test){ // Text Exposure, no grid no synch

            pData[0] = procId; // Proc Id
            pData[1] = ProcType_Standard_test;
            pData[2] = 0;   // Positioner Proc Type = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 0;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = DIRECT_WORKSTATION_ID;   // 1
            pData[6] = 1;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = 0;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq
            pData[14] = 0;  // H Num Exposures
            pData[15] = 0;  // L Num Exposures


        }else if(procId == ProcId_Standard_Test_with_grid){ // Text Exposure, no grid no synch

            pData[0] = procId; // Proc Id
            pData[1] = ProcType_Standard_Mammography_2D;   // Generator Proc Type = 50
            pData[2] = 0;   // Positioner Proc Type = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 0;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = DIRECT_WORKSTATION_ID;   // 1
            pData[6] = 1;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = Proc_DatabankSeq_Software;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq
            pData[14] = 0;  // H Num Exposures
            pData[15] = 0;  // L Num Exposures


        }else if(procId == ProcId_Standard_Mammography_2D){ // Standard 2D
            pData[0] = procId; // Proc Id
            pData[1] = ProcType_Standard_Mammography_2D;   // Generator Proc Type = 50
            pData[2] = 0;   // Positioner Proc Type = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 0;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = DETECTOR_WORKSTATION_ID;   // 1
            pData[6] = 1;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = Proc_DatabankSeq_Software;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq
            pData[14] = 0;  // H Num Exposures
            pData[15] = 0;  // L Num Exposures


        }else if(procId == ProcId_Aec_Mammography_2D){ // AEC 2D
            pData[0] = procId; // Proc Id
            pData[1] = ProcType_Aec_Mammography_2D;   // Proc Type = standard Rad
            pData[2] = 0;   // Positioner = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 0;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = DETECTOR_WORKSTATION_ID;
            pData[6] = 2;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = 1; // Proc_DatabankSeq_Software;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq
            pData[14] = 0;  // H Num Exposures
            pData[15] = 0;  // L Num Exposures

        }else if(procId == ProcId_Standard_Mammography_3D){ // Tomo
            pData[0] = procId; // Proc Id
            pData[1] = ProcType_Standard_Mammography_3D;   // Proc Type = Tomo
            pData[2] = 0;   // Positioner = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 0;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = DETECTOR_WORKSTATION_ID;   // Workstation
            pData[6] = 1;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = 0;// Proc_DatabankSeq_Software;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq
            pData[14] = 0;  // H Num Exposures
            pData[15] = param;  // L Num Exposures

        }else if(procId == ProcId_Aec_Mammography_3D){ // Tomo AEC
            pData[0] = procId; // Proc Id
            pData[1] = ProcType_Aec_Mammography_3D;   // Proc Type = Tomo
            pData[2] = 0;   // Positioner = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 0;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = DETECTOR_WORKSTATION_ID;   // Workstation
            pData[6] = 2;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = 0;  //Proc_DatabankSeq_Software;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq
            pData[14] = 0;  // H Num Exposures
            pData[15] = param;  // L Num Exposures
        }

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                PATIENT_COMMANDS_ENTRY,
                                PATIENT_PROCEDURE_DEFINITION_V6,
                                16,
                                pData);

    }

    void CaDataDicGen::Patient_SetupProcedureV5(unsigned char num){

        if((num < 1) || (num >= ProcId_Last)) return;

        unsigned char pData[14];


        if(num == ProcId_Standard_Mammography_2D){ // Standard 2D
            pData[0] = num; // Proc Id
            pData[1] = ProcType_Standard_Mammography_2D;   // Generator Proc Type = standard Rad
            pData[2] = 0;   // Positioner Proc Type = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 1;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = DETECTOR_WORKSTATION_ID;   // Workstation: 1 = Detector, 5 = Direct
            pData[6] = 1;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = Proc_DatabankSeq_Software;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq


        }else if(num == ProcId_Aec_Mammography_2D){ // AEC 2D
            pData[0] = num; // Proc Id
            pData[1] = ProcType_Aec_Mammography_2D;   // Proc Type = standard Rad
            pData[2] = 0;   // Positioner = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 1;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = DETECTOR_WORKSTATION_ID;   // Workstation: 1 = Detector, 5 = Direct
            pData[6] = 2;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = Proc_DatabankSeq_Software;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq

        }else if(num == ProcId_Standard_Mammography_3D){ // Tomo
            pData[0] = num; // Proc Id
            pData[1] = ProcType_Standard_Mammography_3D;   // Proc Type = Tomo
            pData[2] = 0;   // Positioner = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 1;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = DETECTOR_WORKSTATION_ID;   // Workstation: 1 = Detector, 5 = Direct
            pData[6] = 1;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = Proc_DatabankSeq_Software;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq


        }else if(num == ProcId_Aec_Mammography_3D){ // Tomo AEC
            pData[0] = num; // Proc Id
            pData[1] = ProcType_Aec_Mammography_3D;   // Proc Type = Tomo
            pData[2] = 0;   // Positioner = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 1;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = DETECTOR_WORKSTATION_ID;   // Workstation: 1 = Detector, 5 = Direct
            pData[6] = 2;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = Proc_DatabankSeq_Software;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq

        }

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                PATIENT_COMMANDS_ENTRY,
                                PATIENT_PROCEDURE_DEFINITION_V5,
                                14,
                                pData);

    }



    void CaDataDicGen::Patient_Activate2DProcedurePulse(bool detector, bool grid){


        unsigned char pData[6];

        if(!grid) pData[0] = ProcId_Standard_Test;
        else if(!detector) pData[0] = ProcId_Standard_Test_with_grid;
        else pData[0] = ProcId_Standard_Mammography_2D; // Proc Id

        pData[1] = 1;   // Procedure activation
        pData[2] = 1;   // Databank Index to be activated
        pData[3] = 0;   // Position DB activation
        pData[4] = 0;   // Collimator DB activation
        pData[5] = 0;   // Filter DB activaition


        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                PATIENT_COMMANDS_ENTRY,
                                PATIENT_PROCEDURE_ACTIVATE,
                                6,
                                pData);

    }



    void CaDataDicGen::Patient_Activate2DAecProcedurePulse(void){


        unsigned char pData[6];

        pData[0] = ProcId_Aec_Mammography_2D; // Proc Id
        pData[1] = 1;   // Procedure activation
        pData[2] = 2;   // Databank Index to be activated
        pData[3] = 0;   // Position DB activation
        pData[4] = 0;   // Collimator DB activation
        pData[5] = 0;   // Filter DB activaition


        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                PATIENT_COMMANDS_ENTRY,
                                PATIENT_PROCEDURE_ACTIVATE,
                                6,
                                pData);

    }


    void CaDataDicGen::Patient_Activate2DAecProcedurePre(void){


        unsigned char pData[6];

        pData[0] = ProcId_Aec_Mammography_2D; // Proc Id
        pData[1] = 1;   // Procedure activation
        pData[2] = 1;//EXP_DB_IDX_PRE;   // Databank Index to be activated
        pData[3] = 0;   // Position DB activation
        pData[4] = 0;   // Collimator DB activation
        pData[5] = 0;   // Filter DB activaition


        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                PATIENT_COMMANDS_ENTRY,
                                PATIENT_PROCEDURE_ACTIVATE,
                                6,
                                pData);

    }

    void CaDataDicGen::Patient_Activate3DProcedurePulse(void){


        unsigned char pData[6];

        pData[0] = ProcId_Standard_Mammography_3D; // Proc Id

        pData[1] = 1;   // Procedure activation
        pData[2] = 1;   // Databank Index to be activated
        pData[3] = 0;   // Position DB activation
        pData[4] = 0;   // Collimator DB activation
        pData[5] = 0;   // Filter DB activaition


        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                PATIENT_COMMANDS_ENTRY,
                                PATIENT_PROCEDURE_ACTIVATE,
                                6,
                                pData);

    }

    void CaDataDicGen::Patient_Activate3DAecProcedurePulse(void){


        unsigned char pData[6];

        pData[0] = ProcId_Aec_Mammography_3D; // Proc Id
        pData[1] = 1;   // Procedure activation
        pData[2] = 2;   // Databank Index to be activated
        pData[3] = 0;   // Position DB activation
        pData[4] = 0;   // Collimator DB activation
        pData[5] = 0;   // Filter DB activaition


        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                PATIENT_COMMANDS_ENTRY,
                                PATIENT_PROCEDURE_ACTIVATE,
                                6,
                                pData);

    }


    void CaDataDicGen::Patient_Activate3DAecProcedurePre(void){


        unsigned char pData[6];

        pData[0] = ProcId_Aec_Mammography_3D; // Proc Id
        pData[1] = 1;   // Procedure activation
        pData[2] = 1;//EXP_DB_IDX_PRE;   // Databank Index to be activated
        pData[3] = 0;   // Position DB activation
        pData[4] = 0;   // Collimator DB activation
        pData[5] = 0;   // Filter DB activaition


        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                PATIENT_COMMANDS_ENTRY,
                                PATIENT_PROCEDURE_ACTIVATE,
                                6,
                                pData);

    }


}//namesapce R2Cp	
