#ifndef __ADADICRADINTERFACE_H__
	#define __ADADICRADINTERFACE_H__

#include "aDataDicInterface.h"

namespace R2CP
{	
	typedef struct
	{
			unsigned char DatabankId;
			unsigned char ImagingSystemProtocolId;
			union RadDB_Tech_{
				struct{
					unsigned char TechniqueSel:		6;
					unsigned char AutoTechMode:		1;
					unsigned char AdjustParamAuto:	1; 
				}Fields;
				unsigned char value;
			}TechMode;
			union RadDB_kV_{
				struct{
					unsigned short  data_0_7	 : 8;
					unsigned short  data_8_15	 : 8;
				}Fields;
				unsigned short  value;
			}kV10;
			union RadDB_mAs1000_{
				struct{
					int data_0_7	 : 8;
					int data_8_15	 : 8;
					int data_16_23 : 8;
					int data_24_31 : 8;
				}Fields;
				int value;
			}mAs1000;
			union Rad_mA100_{
				struct{
					int data_0_7	 : 8;
					int data_8_15	 : 8;
					int data_16_23 : 8;
					int data_24_31 : 8;
				}Fields;
				int value;
			}mA100;
			union Rad_ms100_{
				struct{
					int data_0_7	 : 8;
					int data_8_15	 : 8;
					int data_16_23 : 8;
					int data_24_31 : 8;
				}Fields;
				int value;
			}ms100;
			union Rad_MinIntegrationTime_{
				struct{
					unsigned short  data_0_7	 : 8;
					unsigned short  data_8_15	 : 8;
				}Fields;
				unsigned short  value;
			} MinIntegrationTime;
			union Rad_MaxIntegrationTime_{
				struct{
					unsigned short  data_0_7	 : 8;
					unsigned short  data_8_15	 : 8;
				}Fields;
				unsigned short  value;
			}MaxIntegrationTime;
			unsigned char FocalSpot;
			union Rad_TargetDose
			{
				struct
				{
					unsigned short  AECSensitivity_DoseTarget 	: 8;
					unsigned short  AECDensity					: 8;
				}Fields;
				unsigned short  value;
			}TargetDose;
			union Rad_IonChamber_{
				struct{
					unsigned char LeftAECStatus		: 1;
					unsigned char CenterAECStatus	: 1;
					unsigned char RightAECStatus		: 1;
					unsigned char AECNotUsed			: 4;
					unsigned char Internal_External	: 1;
				}Fields;
				unsigned char value;
			}IonChamber;
			unsigned char Spare_1;
			union Rad_FPS10_{
				struct{
					unsigned short  data_0_7	 : 8;
					unsigned short  data_8_15	 : 8;
				}Fields;
				unsigned short  value;
			}FPS10;
			unsigned char TrakingId;
			unsigned char Spare_2;
	}tRadDb;
	
	typedef struct{
		union{
			struct{
				unsigned short  data_0_7	 : 8;
				unsigned short  data_8_15	 : 8;
			}Fields;
			unsigned short  value;
		} MinkV10;
		union{
			struct{
				unsigned short  data_0_7	 : 8;
				unsigned short  data_8_15	 : 8;
			}Fields;
			unsigned short  value;
		}MaxKv10;
		union{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		} MinmAs1000;
		union{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		} MaxmAs1000;
		union{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		} MinmA100;
		union{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		}  MaxmA100;
		union{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		}  Minms100;
		union{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		}  Maxms100;
		unsigned char     mAScale;
	}tRadExpParamRange;

	typedef struct{
		uword ExposureNumber;
		unsigned char  ProcedureId;
		unsigned char  ExpSeqNumber;
		union{
			struct{
				unsigned short  data_0_7	 : 8;
				unsigned short  data_8_15	 : 8;
			}Fields;
			unsigned short  value;
		} kV10;
		union{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		}mA100;
		union{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		} ms100;
		union{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		} mAs1000;
		unsigned char 	FocalSpot;
		union Rad_TargetDose
		{
			struct
			{
				unsigned short  AECSensitivity_DoseTarget 	: 8;
				unsigned short  AECDensity					: 8;
			}Fields;
			unsigned short  value;
		}TargetDose;
		union{
			struct{
				unsigned char LeftAECStatus		: 1;
				unsigned char CenterAECStatus	: 1;
				unsigned char RightAECStatus		: 1;
				unsigned char Spare				: 4;
				unsigned char Internal_External	: 1;
			}Fields;
			unsigned char value;
		}IonChamber;
		unsigned char 	RadEndOfExposure;
		unsigned char IonChamberOrientation;
	}tRadPostExpPostCondition;


	
	typedef enum
	{
		Aec_Rotation_Unknown	= 0,
		Aec_Rotation_0Degree,
		Aec_Rotation_90Degree,
		Aec_Rotation_180Degree,
		Aec_Rotation_270Degree
	}tAecRotation;
	
	typedef struct
	{
		unsigned short  PreparationStatus_HARDWIRED 	: 1;
		unsigned short  ExposureStatus_HARDWIRED		: 1;
		unsigned short  FluoroStatus_HARDWIRED			: 1;
		unsigned short  PreparationStatus_BLUETOOTH 	: 1;
		unsigned short  ExposureStatus_BLUETOOTH		: 1;
		unsigned short  FluoroStatus_BLUETOOTH			: 1;
		unsigned short  PreparationStatus_IR 			: 1;
		unsigned short  ExposureStatus_IR				: 1;
		unsigned short  FluoroStatus_IR				: 1;
		unsigned short  ServiceOnlyExposureDeadman 	: 1;
	}tExposureSwitchesActivation;
	
	typedef enum{
		INJECTOR_STOP = 0,
		INJECTOR_PREPARE,
		INJECTOR_INJECT
	}tInjectorAcction;
	
	typedef enum{
		INJECTOR_STOPPED = 0,
		INJECTOR_READY,
		INJECTOR_RUNNING
	}tInjectorStatus;
	
	typedef enum{
		R2CP_Procedure_Status_None = 0,
		R2CP_Procedure_Status_SingelShot,
		R2CP_Procedure_Status_SerialRAD,
		R2CP_Procedure_Status_ContinousFL,
		R2CP_Procedure_Status_ContinousFL_HLC,
		R2CP_Procedure_Status_Pulsed,
		R2CP_Procedure_Status_PulsedFL_HLC
	}tProcedureExposureType;
	
	
	const unsigned short  	MAX_KVP10_ALLOWED 		= 150 * 10;
	const unsigned short  	MIN_KVP10_ALLOWED 		= 20  * 10;
	const int MAX_MAS1000_ALLOWED		= 32000 * 1000;
	const int	MIN_MAS1000_ALLOWED		= 0.01 * 1000;
	const int MAX_MA100_ALLOWED		= 1000 * 100;
	const int MIN_MA100_ALLOWED		= 1 * 100;
	const int MAX_MS100_ALLOWED		= 20000 * 100;
	const int MIN_MS100_ALLOWED		= 1 * 100;
	const unsigned short   MAX_MIT_ALLOWED			= 20000;
	const unsigned short   MIN_MIT_ALLOWED			= 1;
	const unsigned short   MAX_MAT_ALLOWED			= 20000;
	const unsigned short   MIN_MAT_ALLOWED			= 1;
	const unsigned short   MAX_FPS10_ALLOWED		= 120 * 10;
	
	typedef enum
	{
		AecInternalReferenceBySensitivity = 0, 	//AEC internal reference. Sensitivity can be Low/Medium/High values (0, 1, 2)
		AecInternalReference,					//AEC internal reference. Sensitivity is calculated with a correction factor (%) that multiplies the calibrated reference value.
		AecExternalReference					//AEC external reference Generator console or imaging system calculate the AEC reference
	}tTypeAecReference;
	
	typedef struct
	{
		union MinRadDB_kV_{
			struct{
				unsigned short  data_0_7	 : 8;
				unsigned short  data_8_15	 : 8;
			}Fields;
			unsigned short  value;
		}MinkV10;
		union MaxRadDB_kV_{
			struct{
				unsigned short  data_0_7	 : 8;
				unsigned short  data_8_15	 : 8;
			}Fields;
			unsigned short  value;
		}MaxkV10;
		union MinRadDB_mAs1000_{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		}MinmAs1000;
		union MaxRadDB_mAs1000_{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		}MaxmAs1000;
		union MinRadDB_mA100_{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		}MinmA100;
		int MinmA100SmallFocus;
		int MinmA100LargeFocus;
		union MaxRadDB_mA100_{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		}MaxmA100;
		int MaxmA100SmallFocus;
		int MaxmA100LargeFocus;
		union MinRadDB_ms100_{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		}Minms100;
		union MaxRadDB_ms100_{
			struct{
				int data_0_7	 : 8;
				int data_8_15	 : 8;
				int data_16_23 : 8;
				int data_24_31 : 8;
			}Fields;
			int value;
		}Maxms100;
		union MinRadDB_PPS10_{
			struct{
				unsigned short  data_0_7	 : 8;
				unsigned short  data_8_15	 : 8;
			}Fields;
			unsigned short  value;
		}MinPPS10;
		union MaxRadDB_PPS10_{
			struct{
				unsigned short  data_0_7	 : 8;
				unsigned short  data_8_15	 : 8;
			}Fields;
			unsigned short  value;
		}MaxPPS10;
		unsigned char FocalSpot;
		unsigned char DatabankId;
	}tRadParameterRanges;
	
	typedef enum
	{
		GEN_SCALE_R10	= 0,
		GEN_SCALE_R20,
		GEN_SCALE_LINEAL
	}tScale;
	
	typedef struct
	{
		unsigned short  SmallFocus_mA_vs_Demand_Status : 1;
		unsigned short  LargeFocus_mA_vs_Demand_Status	: 1;
		unsigned short  kVpGainStatus					: 1;
		unsigned short  FluoroGainStatus				: 1;
		unsigned short  LowGainStatus					: 1;
		unsigned short  HighGainStatus					: 1;
		unsigned short  SmallFocusFilamentDemandStatus : 1;
		unsigned short  LargeFocusFilamentDemandStatus : 1;
	}tCalibrationStatus;

    // __________________________________________ GENERATORE STATUS

    /// This id the Enumeration code of the GEnerator Internal Status
    typedef enum{
	   Stat_Invalid = 0,		//!< [0] Invalid or wrong communication
       Stat_Initialization = 1, //!< [1] The Generator is Initializing
       Stat_Standby,            //!< [2] The Generator is in Standby mode
       Stat_Preparation,        //!< [3] The Generator is in Exposure preparation
       Stat_Ready,              //!< [4] The Generator is ready to activate X-RAYs
       Stat_ExpReq,             //!< [5] The Generator received the Exzposure request
       Stat_ExpInProgress,      //!< [6] The Generator is executing the exposure
       Stat_WaitFootRelease,    //!< [7] The Generator is waiting the XRAY signal release
       Stat_GoigToShutdown,     //!< [8] The Generator is in shutdown
       Stat_Error,              //!< [9] The Generator is in error condition
       Stat_Service,            //!< [10] The Generator is in Service Mode
	   Stat_Unknown				
    } tGenStatus_Stat;

    typedef enum{
        Stat_SystemMessageActive_NotActive = 0,
        Stat_SystemMessageActive_Active,
    }tGenStatus_SystemMessageActive;

    typedef enum{
        Stat_SystemMessageInhibit_NotActive = 0,
        Stat_SystemMessageInhibit_Active,
    }tGenStatus_SystemMessageInhibit;

    typedef enum{
        Stat_ProcedureStat_NotActive = 0,
        Stat_ProcedureStat_Active,
        Stat_ProcedureStat_Paused,
        Stat_ProcedureStat_Finished
    }tGenStatus_ProcedureStat;

    typedef enum{
        tGenStatus_ExposureType_None=0,
        tGenStatus_ExposureType_SingleShot,
        tGenStatus_ExposureType_SerialRad,
        tGenStatus_ExposureType_ContFl,
        tGenStatus_ExposureType_ContFl_HLC,
        tGenStatus_ExposureType_PulsedFl,
        tGenStatus_ExposureType_PulsedFl_HLC
    }tGenStatus_ExposureType;

    typedef struct
    {
        unsigned char GeneratorStatus;
        union
        {
            struct
            {
                unsigned char Active		:4;
                unsigned char Inhibit	:4;
            }Fields;
            unsigned char value;
        }SystemMessage;
        unsigned char ProcedureId;
        unsigned char ExposureType;
        unsigned char ExposureDatabankSeqNumber;
        union
        {
            struct
            {
                unsigned char data_0_7;
                unsigned char data_15_8;
            } Fields;
            unsigned short  value;
        } FluoroTime;
        union
        {
            struct
            {
                unsigned char Alarm_5minute      	: 1;
                unsigned char Warning_5minute	  	: 1;
                unsigned char Spare_1			  	: 1;
                unsigned char LockInFrameReached 	: 1;
                unsigned char Alarm_10minute	  		: 1;
                unsigned char Warning_10minute	  	: 1;
                unsigned char Spare_2			  	: 1;
                unsigned char HCF_ON			  		: 1;
            } Fields;
            unsigned char value;
        } FluoroStatus;
        unsigned char AccumulatedAnodeHU;
        unsigned char AccumultatedhousingHU;
        unsigned char AccumulatedGenHU;
        union
        {
            struct
            {
                unsigned char PrepSignalStatus		  :1;
                unsigned char ExpsignalStatus          :1;
                unsigned char FlSignalStatus			  :1;
                unsigned char Spare					  :4;
                unsigned char FilStatus				  :1;
            } Fields;
            unsigned char value;
        }ExposureSwitches;
        union
        {
            struct
            {
                unsigned char Mode_1   : 1;
                unsigned char Mode_2	  : 1;
                unsigned char Spare	  : 6;
            } Fields;
            unsigned char value;
        }DynModeStatus;
        unsigned char CurrentRotorSpeed;
    }tGeneratorStatusV6;

    typedef struct
    {
        unsigned char GeneratorStatus;
        union
        {
            struct
            {
                unsigned char Active		:4;
                unsigned char Inhibit	:4;
            }Fields;
            unsigned char value;
        }SystemMessage;
        unsigned char ProcedureId;
        unsigned char ProcedureStatus;
        unsigned char ExposureDatabankSeqNumber;
        union
        {
            struct
            {
                unsigned char data_0_7;
                unsigned char data_15_8;
            } Fields;
            unsigned short  value;
        } FluoroTime;
        union
        {
            struct
            {
                unsigned char Alarm_5minute      	: 1;
                unsigned char Warning_5minute	  	: 1;
                unsigned char Spare_1			  	: 1;
                unsigned char LockInFrameReached 	: 1;
                unsigned char Alarm_10minute	  		: 1;
                unsigned char Warning_10minute	  	: 1;
                unsigned char Spare_2			  	: 1;
                unsigned char HCF_ON			  		: 1;
            } Fields;
            unsigned char value;
        } FluoroStatus;
        unsigned char AccumulatedAnodeHU;
        unsigned char AccumultatedhousingHU;
        unsigned char AccumulatedGenHU;
        union
        {
            struct
            {
                unsigned char PrepSignalStatus		  :1;
                unsigned char ExpsignalStatus          :1;
                unsigned char FlSignalStatus			  :1;
                unsigned char Spare					  :4;
                unsigned char FilStatus				  :1;
            } Fields;
            unsigned char value;
        }ExposureSwitches;
        union
        {
            struct
            {
                unsigned char Mode_1   : 1;
                unsigned char Mode_2	  : 1;
                unsigned char Spare	  : 6;
            } Fields;
            unsigned char value;
        }DynModeStatus;
        unsigned char CurrentRotorSpeed;
    }tGeneratorStatusV5;

    // __________________________________________ / GENERATOR STATUS
    //___________________________________________ PROCEDURE DEFINITION
    typedef enum{        
		ProcId_2D_NoDet_NoGrid=1,
		ProcId_2D_NoDet_WithGrid,
		ProcId_2D_WithDet_NoGrid,
		ProcId_2D_WithDet_WithGrid,
        ProcId_2D_Aec_NoGrid,
		ProcId_2D_Aec_WithGrid,
        ProcId_Standard_Mammography_3D, // no grid
        ProcId_Aec_Mammography_3D, // no grid		
        ProcId_Last
    }tProcedureId;

    typedef enum{
		ProcType_Not_Defined = 0,
        ProcType_StandardRAD=1,
		ProcType_MultiEnergy = 4,
        ProcType_StandardRADMamo=50,
        ProcType_Aec_Mammography_2D=51,
        ProcType_Standard_Mammography_3D=10,
        ProcType_Aec_Mammography_3D=52,
    }tProcedureType;


    typedef enum{
        Proc_SoftwareActivation =0,
        Proc_HandswitchActivation,
    }tProcedureActivation;

    typedef enum{
        Proc_DatabankSeq_ND =0,
        Proc_DatabankSeq_Next,
        Proc_DatabankSeq_Software
    }tProcedureDatabankSeq;

    typedef struct
    {
        unsigned char ProcedureId;
        unsigned char GeneratorProcedureType;
        unsigned char Handswitch;
        unsigned char ActivationMode;
        unsigned char WorkstationId;
        unsigned char numDatabank;
        unsigned char databankSeqMode;
        unsigned char numberOfExposure;
        unsigned char initialized;
    }tProcedureDefinition;
    // __________________________________________ / PROCEDURE DEFINITION

    // __________________________________________ DATABANK DEFINITION
    typedef enum{
        DB_Pre = 1,
        DB_Pulse,
        DB_SkipPulse,
        DB_LastId
    }tProcedureDbId;

    typedef enum{
        DB_Focus_Small = 0,
        DB_Focus_Large = 1,
    }tProcedureDbFocus;

    typedef enum{
        DB_Tech_0= 0,
        DB_Tech_1,
        DB_Tech_2,
        DB_Tech_3,
        DB_Tech_2_FallingLoad,
        DB_Tech_FullAuto,
        DB_Tech_2_max_ms,
    }tDbTech;

    typedef enum{
        DB_Tech_AutoMode_NotModify= 0,
        DB_Tech_AutoMode_Modify = 0x40,
    }tDbTechAutoMode;

    typedef enum{
        DB_Tech_AAdjustParam_NotModify= 0,
        DB_Tech_AdjustParam_Modify = 0x80,
    }tDbAdjustParam;



    // __________________________________________ / DATABANL DEFINITION



	class CaDataDicRadInterface
	{
	public:
        CaDataDicRadInterface(void){
            Initialitation();
        }


        /*!
         *	\brief Once the CaDataDicGen is instanced this method is called
         */
        void Initialitation(void) {
            for(int i=1; i< ProcId_Last; i++){
                procedureDefinitions[i-1].initialized = 0;
            }

            for(int i=1; i< DB_LastId; i++){
                DbDefinitions[i-1].ImagingSystemProtocolId = 0;
            }

        };


        void MET_Generator_Set_StatusV5(unsigned char * data) { generatorStatusV5 =  *(tGeneratorStatusV5*) data; }


        void MET_Generator_Set_StatusV6(unsigned char * data) {
            generatorStatusV6.GeneratorStatus = data[0];
            generatorStatusV6.SystemMessage.value = data[1];
            generatorStatusV6.ProcedureId = data[2];
            generatorStatusV6.ExposureType = data[3];
            generatorStatusV6.ExposureDatabankSeqNumber = data[4];
            generatorStatusV6.FluoroTime.value = data[5]*256 + data[6];
            generatorStatusV6.FluoroStatus.value = data[7];
            generatorStatusV6.AccumulatedAnodeHU = data[8];
            generatorStatusV6.AccumultatedhousingHU = data[9];
            generatorStatusV6.AccumulatedGenHU = data[10];
            generatorStatusV6.ExposureSwitches.value = data[11];
            generatorStatusV6.CurrentRotorSpeed = data[13];


           // generatorStatusV6 =  *(tGeneratorStatusV6*) data;
        }

        unsigned char MET_Generator_Update_ProcedureV5(unsigned char * data) {
            if(data ==nullptr) return 0;

            unsigned char id = data[0];
            if( id >= ProcId_Last) return 0;
            if( id < 1) return 0;
            tProcedureDefinition* tProc = &procedureDefinitions[id-1];

            tProc->ProcedureId = data[0];
            tProc->GeneratorProcedureType = data[1];
            tProc->Handswitch = data[2];
            tProc->ActivationMode = data[3];
            tProc->WorkstationId = data[4];
            tProc->numDatabank = data[5];
            tProc->databankSeqMode = data[6];
            tProc->numberOfExposure = 0;
            tProc->initialized = 1;
            return id;
        }

        unsigned char MET_Generator_Update_ProcedureV6(unsigned char * data) {
            if(data ==nullptr) return 0;

            unsigned char id = data[0];
            if( id >= ProcId_Last) return 0;
            if( id < 1) return 0;
            tProcedureDefinition* tProc = &procedureDefinitions[id-1];

            tProc->ProcedureId = data[0];
            tProc->GeneratorProcedureType = data[1];
            tProc->Handswitch = data[2];
            tProc->ActivationMode = data[3];
            tProc->WorkstationId = data[4];
            tProc->numDatabank = data[5];
            tProc->databankSeqMode = data[6];
            tProc->numberOfExposure = data[8] + 256 * data[7];
            tProc->initialized = 1;
            return id;
        }

        public:
         tGeneratorStatusV5 generatorStatusV5;
         tGeneratorStatusV6 generatorStatusV6;
         tProcedureDefinition procedureDefinitions[ProcId_Last];
         bool isProcInitialized(unsigned char i){return ( i >= ProcId_Last) ? false : procedureDefinitions[i-1].initialized;}

         tRadDb DbDefinitions[DB_LastId];
         bool isDbInitialized(unsigned char i){return ( i >= DB_LastId) ? false : ( (DbDefinitions[i].ImagingSystemProtocolId == DB_LastId) ? true : false );}

	};//class
};//namesapce
#endif
