#ifndef __ADADICRFINTERFACE_H__
	#define __ADADICRFINTERFACE_H__

#include "aDataDicInterface.h"

namespace R2CP
{	
	typedef struct
	{
			udword 	mA100;
			udword 	ms100;
			uword  	kV10;
			uword 	MaxIntegrationTime;
			uword   AbcUpdateTime;
			uword 	TargeteLsb;
			uword 	PPS10;
			uword 	DoseLevelId;
			union{
				struct{
					unsigned char 	ABCEnabled		: 1;
					unsigned char 	Fl_LockIn		: 1;
					unsigned char 	HighDoseEnabled : 1;
					unsigned char 	Reserved		: 5;
				}Fields;
				unsigned char value;
			} DoseCtrl;
			
			unsigned char 	FocalSpot;
			unsigned char 	DatabankId;
			unsigned char 	ImagingSystemProtocol;
			unsigned char 	CurveId;
			unsigned char 	FilterId;
			unsigned char 	QbyPSS;
	}tFlDb;
	
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
				unsigned short  data_0_7	 : 8;
				unsigned short  data_8_15	 : 8;
			}Fields;
			unsigned short  value;
		}  MinmA100;
		union{
			struct{
				unsigned short  data_0_7	 : 8;
				unsigned short  data_8_15	 : 8;
			}Fields;
			unsigned short  value;
		}  MaxmA100;
	}tFlExpParamRange;
	
	typedef struct
	{
		uword ExposureNumber;
		unsigned char  ProcedureId;
		unsigned char  ExpDbSeqNumber;
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
		unsigned char 	FocalSpot;
		unsigned char 	FlEndOfExposure;
	}tFlPostExpPostCondition;
	
	const unsigned short  	MAX_FLKVP10_ALLOWED	= 125 * 10;
	const unsigned short  	MIN_FLKVP10_ALLOWED	= 40  * 10;
	const int	MAX_FLMA100_ALLOWED	= 1000 * 100;
	const unsigned short  	MIN_FLMA100_ALLOWED	= 1;//0.01 * 100
	const int	MAX_FLMS100_ALLOWED	= 1000 * 100;
	const int MIN_FLMS100_ALLOWED = 1 * 100;
	const unsigned short   MAX_FLMAT_ALLOWED	= 1000;
	const unsigned short   MIN_FLMAT_ALLOWED	= 1;
	const unsigned short   MAX_PPS10_ALLOWED	= 120 * 10;
	const unsigned short   MIN_PPS10_ALLOWED	= 0;
	
	
	class CaDataDicRfInterface
	{
	public:
		/*!
		 * 
		 */
		CaDataDicRfInterface() {}
		
		/*!
		 * \brief Destructor
		 */
		virtual ~CaDataDicRfInterface() {}
		
		/*!
		 *	\brief Once the CaDataDicGen is instanced this method is called
		 */
		void Initialitation(void) {};

		enum{
			Cp_II_GEN_SS_Databank_IdOutOfRange			= 1,
			Cp_II_GEN_SS_Databank_NotDefined			= 2,
			Cp_II_GEN_SS_IncorrectCommandType			= 10,
		};
		
		/************************************ Generator Index ***************************************/
		enum{
			Cp_II_GEN_SS_FDBkVpValueoutOfRange	= 6,
			Cp_II_GEN_SS_FDBmAValueoutofRange 	= 8,
			Cp_II_GEN_SS_FDBmsValueoutofRange,
			Cp_II_GEN_SS_FDBMaxIntTime,
			Cp_II_GEN_SS_FDBPPSValueoutOfRange,
			Cp_II_GEN_SS_FDBSettingPPSNotSupported,
			Cp_II_GEN_SS_FDBContinuousFluoroscopyNotSupported,
			Cp_II_GEN_SS_FDBCurveIDoutofRange,
			Cp_II_GEN_SS_FDBCurveNotSupported,
			Cp_II_GEN_SS_FDBDataBankTypeNoMatch,
			Cp_II_GEN_SS_FDFocalSpotCodeOutofRange,
			Cp_II_GEN_SS_FDFocalSpotNotSupported,
			Cp_II_GEN_SS_FDFDataBankTypeNotMatch			= 24,
		};
		
		/*!
		 * \brief Sets a FL Data Bank.
		 */
		virtual unsigned char II_Generator_SS_RFDLoad( tFlDb *pFlDb )  { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets a FL Data Bank.
		 */
		virtual unsigned char II_Generator_SS_RFDLoad(unsigned char Databank_Id) { return Cp_MessageNotAvailable; }
		
		enum{
			Cp_II_GEN_SS_FDBACCProcedureNoFluoro				= 3,
			Cp_II_GEN_SS_FDBACCNoExposureDbAssigned				= 4,
			Cp_II_GEN_SS_FDBACCExposureDbIdNotDefined			= 5
		};
		
		/*!
		 * \brief Message defined to keep RAD and FL procedure messages consistent.
		 */
		virtual unsigned char II_Generator_SS_RFDAcceptance(unsigned char Procedure_Id) { return Cp_MessageNotAvailable; }
		
		enum{
			Cp_II_GEN_SS_FDBKVPkVpValueoutofRange		= 3,
		};

		/*!
		 * \brief Sets FL kV value to a FL Data Bank.
		 */
		virtual unsigned char II_GEN_SS_FDBKVP( unsigned char Databank_Id, short int kVp10orStep , bool CommandType) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets FL kV value to a FL Data Bank.
		 */
		virtual unsigned char II_GEN_SS_FDBKVP(unsigned char Databank_Id) { return Cp_MessageNotAvailable; }
		
		enum{
			Cp_II_GEN_SS_FDBMAmAValueOutofRange		= 3,
		};

		/*!
		 * \brief Sets FL mA value to a FL Data Bank.
		 */			 
		virtual unsigned char II_GEN_SS_FDBMA( unsigned char Databank_Id, long int mA100orStep , bool CommandType ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets FL mA value to a FL Data Bank.
		 */
		virtual unsigned char II_GEN_SS_FDBMA( unsigned char Databank_Id ) { return Cp_MessageNotAvailable; } 
		
		enum{
			Cp_II_GEN_SS_FDBMSmsValueoutofRange		= 3,
		};

		/*!
		 * \brief Sets FL ms value (pulse width) to a FL Data Bank.
		 */
		virtual unsigned char II_GEN_SS_FDBMS(unsigned char Databank_Id, int ms100orStep , bool CommandType) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets FL ms value (pulse width) to a FL Data Bank.
		 */
		virtual unsigned char II_GEN_SS_FDBMS(unsigned char Databank_Id) { return Cp_MessageNotAvailable; }
		
		enum{
			Cp_II_GEN_SS_FDBMaxIntTimeValue = 3,
		};
		/*!
		 * \brief Sets maximum integration time in a FL Data Bank.
		 */
		virtual unsigned char II_Generator_SS_FDBMaxIntTime( unsigned char DatabankId , unsigned short  MaxIntTime ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets maximum integration time in a FL Data Bank.
		 */
		virtual unsigned char II_Generator_SS_FDBMaxIntTime( unsigned char DatabankId ) { return Cp_MessageNotAvailable; }
		
		enum{
			Cp_II_GEN_SS_FDB_PPSValueOutofRange = 3,
			CP_II_GEN_SS_FDB_SettingPPSNotSupported,
			CP_II_GEN_SS_FDB_ContinuousFluoroscopyNotSupported
		};

		/*!
		 * \brief Sets PPS in a FL Data Bank.
		 */
		virtual unsigned char II_Generator_SS_FDBPPS(unsigned char Databank_Id, unsigned short  PPS10  ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets PPS in a FL Data Bank.
		 */
		virtual unsigned char II_Generator_SS_FDBPPS(unsigned char Databank_Id) { return Cp_MessageNotAvailable; };
		
		/*!
		 * \brief Sets Abc , Lock In and High Dose 
		 */
		virtual unsigned char II_Generator_SS_FDAbcHighDoseLockIn( unsigned char DatabankId , bool IsAbc , bool IsFlLocking , bool HighDose ){ return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets Abc , Lock In and High Dose 
		 */
		virtual unsigned char II_Generator_SS_FDAbcHighDoseLockIn( unsigned char DatabankId ) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Set QbyPPS
		 */
		virtual unsigned char II_Generator_SS_FDQbyPPS( unsigned char DatabankId , bool QbyPPS ) { return Cp_MessageNotAvailable; };
		
		/*!
		 * \brief Get QbyPPS 
		 */
		virtual unsigned char II_Generator_SS_FDQbyPPS( unsigned char DatabankId ) { return Cp_MessageNotAvailable; };
		
		/*!
		 * \brief Sets Target Lsb 
		 */
		virtual unsigned char II_Generator_SS_FDTargetLsb( unsigned char DatabankId , unsigned short  TargetLsb ) { return Cp_MessageNotAvailable; } 
		/*!
		 * \brief Gets Target Lsb
		 */
		virtual unsigned char II_Generator_SS_FDTargetLsb( unsigned char DatabankId ) { return Cp_MessageNotAvailable; }
		
		enum
		{
			CP_II_GEN_SS_FSSFocalSpotCodeOutOfRange = 3,
			CP_II_GEN_SS_FSSFocalSpotNotSupported
		};
		/*!
		 * \brief Sets Target Lsb 
		 */
		virtual unsigned char II_Generator_SS_FDFocalSpot( unsigned char DatabankId , unsigned char FocalSpot ) { return Cp_MessageNotAvailable; }
		/*!
		 * \brief Gets Target Lsb
		 */
		virtual unsigned char II_Generator_SS_FDFocalSpot( unsigned char DatabankId ) { return Cp_MessageNotAvailable; }
		
		
		/*!
		 * \brief Sets Abc Update Time
		 */
		virtual unsigned char II_Generator_SS_FDAbcUpdateTime( unsigned char DatabankId , unsigned short  AbcUpdateTime ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Sets Abc Update Time
		 */
		virtual unsigned char II_Generator_SS_FDAbcUpdateTime( unsigned char DatabankId ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Sets Dose Level ID in a FL Data Bank. 
		 */
		virtual unsigned char II_Generator_SS_FDBDoseLevelID( unsigned char DatabankId, unsigned short  DoseLevelId ) { return Cp_MessageNotAvailable; }	
		
		/*!
		 * \brief Gets Dose Level ID in Fl Databank
		 */
		virtual unsigned char II_Generator_SS_FDBDoseLevelID( unsigned char DatabankId ) { return Cp_MessageNotAvailable; }	
		
		/*!
		 * \brief Sets New Filter Id
		 */
		virtual unsigned char II_Generator_SS_FDBFilterId( unsigned char DatabankId, unsigned char DoseLevelId ) { return Cp_MessageNotAvailable; }	
		
		/*!
		 * \brief Gets Filter Id
		 */
		virtual unsigned char II_Generator_SS_FDBFilterId( unsigned char DatabankId ) { return Cp_MessageNotAvailable; }
		
		enum
		{
			CP_II_GEN_SS_FDCCurveIDOutOfRange = 3,
			CP_II_GEN_SS_FDCurveNotSupported
		};
		/*!
		*	\brief 	Sets Curve ID in a FL Data Bank.
		*/
		virtual unsigned char II_Generator_SS_FDBCurveId (unsigned char DatabankId, unsigned char CurveId ) { return Cp_MessageNotAvailable; }	

		/*!
		*	\brief 	Gets Curve ID in a FL Data Bank.
		*/
		virtual unsigned char II_Generator_SS_FDBCurveId (unsigned char DatabankId ) { return Cp_MessageNotAvailable; }	
		
		/****************************** CURRENT FLUORO EXPOSURE DATA BANK *********************************/
		/*!
		 * \brief Current Fluoro Exposure Data Bank
		 */
		virtual unsigned char II_Generator_SS_FDBCurrentFlDataBank(void) { return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank kVp value.
		*/
		virtual unsigned char II_Generator_SS_FDBCurrentkVp(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank mA value.
		*/
		virtual unsigned char II_Generator_SS_FDBCurrentmA(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank ms value.
		*/
		virtual unsigned char II_Generator_SS_FDBCurrentms(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank MinIntegrationTime value.
		*/
		virtual unsigned char II_Generator_SS_FDBCurrentMaxIntegrationTime(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank FPS.
		*/
		virtual unsigned char II_Generator_SS_FDBCurrentPPS(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank ABC setting.
		*/
		virtual unsigned char II_Generator_SS_FDBCurrentABC(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank High Dose value.
		*/
		virtual unsigned char II_Generator_SS_FDBCurrentHighDose(void){ return Cp_MessageNotAvailable; }

		/*!
		 * \brief Current Fluoro Data Bank kV Scan
		 */
		virtual unsigned char II_Generator_SS_FDBCurrentkVScan(void){ return Cp_MessageNotAvailable; }

		/*!
		 * \brief Current Fluoro Data Bank QbyPPS setting.
		 */
		virtual unsigned char II_Generator_SS_FDBCurrentQbyPPS(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank Dose Level ID.
		*/
		virtual unsigned char II_Generator_SS_FDBCurrentDoseLevelId(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank Curve ID.
		*/
		virtual unsigned char II_Generator_SS_FDBCurrentCurveId(void){ return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Resets fluoro time.
		 */
		virtual unsigned char II_Generator_SS_FDBFluoroTimeReset(void){ return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Resets 5-minute fluoro alarm.
		 */
		virtual unsigned char II_Generator_SS_FDBFiveMinuteFluoroAlarmReset(void){ return Cp_MessageNotAvailable; }

		/*!
		 * \brief FlPost Exposure Fl Post Condition
		 */
		virtual unsigned char II_Generator_SS_FlPostExposure(void){ return Cp_MessageNotAvailable; }
		
		 /*!
		  *	\brief This message contains the information about the FLUORO exposure parameter ranges.
		  */
		 virtual unsigned char II_Generator_SS_FlExpParamRanges(void) { return Cp_MessageNotAvailable; }
		 /*!
		  * \brief This message constains the information about current Lsb measured by Image System
		  */
		virtual unsigned char II_ImageSystem_CurrentLsb( unsigned short  ExpNumber , unsigned short  CurrentLsb ) { return Cp_MessageNotAvailable; }
			
	};//class
};//namesapce
#endif
