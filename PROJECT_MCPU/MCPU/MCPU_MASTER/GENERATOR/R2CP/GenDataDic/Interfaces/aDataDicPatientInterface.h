#ifndef _ADADADICPATIENTINTERFACE__
	#define _ADADADICPATIENTINTERFACE__

#include "aDataDicInterface.h"

namespace R2CP
{		
		typedef enum eApplicationMode {
			ConsoleUserMode = 0,
			PositionerConsoleCalibrationMode,
			GeneratorConsoleCalibrationMode,
			ConsoleServiceMode,
			ConsoleInitialitation = 0xFF
		}tApplicationMode;
		
		typedef struct {
			unsigned char 	ProcedureId;
			unsigned char 	GeneratorProcedureType;
			unsigned char 	HorFSwitchId;
			unsigned char 	ActiveWhenHorFSwichIsPressed;
			unsigned char 	WorkstationId;
			unsigned char 	TotalNumOfExpDBinProcedure;
			unsigned char 	GenDataBankSequencing;
			uword	TotalNumOfExpInProcedure;
			unsigned char 	SubIndex;
		}tProcedure;
		
		typedef enum
		{
			PowerOffType_PowerOff	= 0,
			PowerOffType_Recycling,
		}tPowerOffType;
		
		class CaDataDicPatientInterface
		{
		public:
		
		/*!
		 * \brief Default Constructor
		 */
		CaDataDicPatientInterface(){};
			
		/*!
		 * \brief Destructor
		 */
		virtual ~CaDataDicPatientInterface(){};
		
		enum{
			Cp_II_Patient_SS_PDProcedureIdOutOfRangeGen					= 1, ///generator does not accept the requested procedure ID
			Cp_II_Patient_SS_PDProcedureTypeNotSupportedGen				= 2, ///generator does not implement the requested functionality (license)
			Cp_II_Patient_SS_PDHandswitchFootswitchIndexOutOfRangeGen	= 10, ///assigned exposure switch is out of range
			Cp_II_Patient_SS_PDNonExistentWorkstationGen				= 12, ///workstation not defined on the generator
			Cp_II_Patient_SS_PDIncorrectNumberExposureDatabanks			= 13, ///assigned databank not defined
			Cp_II_Patient_SS_PDExposureDatabankSequencingNotPossible	= 14,
			Cp_II_Patient_SS_PDExposureDatabankSequencingNotSupported	= 15,
			Cp_II_Patient_SS_PDWorkstationOutOfRange					= 16,
			Cp_II_Patient_SS_PDHandswitchFootswitchIndexNotDefined		= 17,
			Cp_II_Patient_SS_PDProcedurePrevioslyCreated				= 18
		};
	
		/*!
		 * \brief	Defines a procedure. Message to be sent to all positioners and generators in the room. 
		 *			Each positioner and generator will reply with a Positioner Procedure and Generator Procedure ANSWER-EVENT
		 */
		virtual unsigned char II_Patient_SS_ProcedureDefinition( tProcedure *pProcedure ) { return Cp_MessageNotAvailable; } 
	
		/*!
		 * \brief Gets procedure Id Information
		 */
		virtual unsigned char II_Patient_SS_ProcedureDefinition( unsigned char ProcedureId ) { return Cp_MessageNotAvailable; } 
			
		enum{
			Cp_II_Patient_SS_PAProcedureIdNotDefinedGen = 1,
			Cp_II_Patient_SS_PAProcedureIdOutOfRangeGen = 2,
			Cp_II_Patient_SS_PAExposureSequenceNumberOutOfRange = 3,
			Cp_II_Patient_SS_PAExposureDbNotAssigned = 4,
			Cp_II_Patient_SS_PAProcedureInvalidActivation = 5,
			Cp_II_Patient_SS_PAProcedureInvalidDeactivation = 6,
			Cp_II_Patient_SS_PAExposureSequenceNumberInvalid = 7
		};
		
		/*!
		 * \brief	Activates the specified Procedure, Exposure/Position/Collimator FOV/Collimator Data Bank Sequence Numbers.
		 *			There can only be one procedure active.
		 *			All masters will reply with a Positioner or Generator Activate Procedure and Data Banks ANSWER.
		 *			If activation is driven by handswitch/footswitch, masters will send EVENT messages.
		 */
		virtual unsigned char II_Patient_SS_ProcedureActivateDatabank(unsigned char ProcedureId , unsigned char Command , unsigned char ExpDBSeqNum )  { return Cp_MessageNotAvailable; } 
	
		/*!
		 *	\brief Gets Current Status of Procedure & Databank
		 */
		virtual unsigned char II_Patient_SS_ProcedureActivateDatabank(unsigned char ProcedureId )  { return Cp_MessageNotAvailable; } 
	
		enum{
			Cp_II_Patient_SS_PDefaultProcedureIdNotDefinedGen = 1,
			Cp_II_Patient_SS_PDefaultProcedureIdOutOfRangeGen = 2,
			Cp_II_Patient_SS_PDefaultExposureSequenceNumberOutOfRange = 3,
			Cp_II_Patient_SS_PDefaultExposureDbNotAssigned = 4
		};
	
		/*!
		 * \brief For procedures activated from handswitches/footswitches, defines the procedure that is activated when no handswitch/footswitch is pressed. 
		 *		  be used to always be ready for the most commonly used procedure, typically fluoro.
		 *		  If no procedure is defined, default procedure 0 is used when there is no active handswitch/footswitch.
		 */
		virtual unsigned char II_Patient_SS_Procedure_Default( unsigned char ProcedureId , unsigned char Command , unsigned char ExpDBSeqNum ) { return Cp_MessageNotAvailable; } 
	
		/*!
		 * \brief Gets Default Procedure
		 */
		virtual unsigned char II_Patient_SS_Procedure_Default( void ) { return Cp_MessageNotAvailable; }
	
		/*!
		 *	\brief Clears all Procedures. All masters will reply with a Positioner Clear All Procedures or Generator Clear All Procedures EVENT.
		 */
		virtual unsigned char II_Patient_SS_Procedure_ClearAll (void) { return Cp_MessageNotAvailable; }
	
		enum{
			Cp_II_Patient_SS_PRProcedureIdNotDefinedGen			= 1,
			Cp_II_Patient_SS_PRProcedureIdOutOfRangeGen			= 2,
			Cp_II_Patient_SS_PRProcedureActivationNotSupported	= 11
		};
		/*!
		 * \brief  Redefines Handswitch/Footswitch Procedure Activation Field. 
		 *		   Message to be sent to all positioners and generators in the room. 
		 *		   Each positioner and generator will reply with a Positioner Procedure and Generator Procedure ANSWER-EVENT.
		 */
		virtual unsigned char II_Patient_SS_Patient_Redefine_HandFootswitchActivation( unsigned char ProcedureId , unsigned char ActiveWhenHorFSwitchIsPressed ) { return Cp_MessageNotAvailable; }
	
		enum{
			Cp_II_Patient_SS_PWProcedureIdNotDefinedGen = 1,
			Cp_II_Patient_SS_PWProcedureIdOutOfRangeGen = 2,
			Cp_II_Patient_SS_PWNonExistentWorkstation 	= 12,
			Cp_II_Patient_SS_PWDifferentTubeDefinition	= 13
		};
		/*!
		 * \brief Redefines Workstation Field. 
		 *		  Message to be sent to all positioners and generators in the room. 
		 *		  Each positioner and generator will reply with a Positioner Procedure and Generator Procedure ANSWER-EVENT.
		 */
		virtual unsigned char II_Patient_SS_Redefine_Workstation(unsigned char ProcedureId , unsigned char WorkstationId ) { return Cp_MessageNotAvailable; }
	
		/************************************************** MISCELLANEOUS ********************************************/
		/*!
		 * \brief This message is sent to positioner request a complete update of the current positioner status. 
				  This includes defined procedures and data banks.
				  This message is sent to generator request a complete update of the current generator status. 
				  This includes defined procedures and data banks.
		 */
		virtual unsigned char II_Patient_SS_SyncUp( void ) { return Cp_MessageNotAvailable; }
	
		/*!
		 * \brief Sets new Working Mode
		 */
		virtual unsigned char II_Patient_SS_WorkingMode(unsigned char WorkingMode , unsigned char NodeId) { return Cp_MessageNotAvailable; }
			
		/*!
		 *\brief Gets Current Working MOde
		 */
		virtual unsigned char II_Patient_SS_WorkingMode(void) { return Cp_MessageNotAvailable; };
		
		enum{
			Cp_II_Patient_SS_Pos_Cal_Not_Available 	= 1,
			Cp_II_Patient_SS_Pos_Cal_No_Switching	= 2,
			Cp_II_Patient_SS_Pos_Cal_Wait_To_SendCp
		};
		
		/*!
		 * \brief 	This message is sent to generator and positioner when there are more than one positioner configured in the room, 
		 			to distiguish wich positioner (tube) is about to be calibrated. 
					If this message is not sent, the devices will assume that positioner 1 is calibrated.

		 */
		
		virtual unsigned char II_Positioner_Calibration_Number( unsigned char Pos_Id , unsigned char Sequence ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets Positioner Calibration Number
		 */
		virtual unsigned char II_Positioner_Calibration_Number( void ) { return Cp_MessageNotAvailable; }
	
		/*!
		 *\brief	This event is fired when a power off sequence is requested. 
		 *			It could be started from the RCC (Remote Control Console), 
		 *			or any other device (i.e Console, generator power off key, …).
		 *			Power Recycling is available available on ChallengeX, 
		 *			and consists on a power on restart after a regular power off sequence.
		 *\param[in]PowerOffType => 0: Power off  1: Power Recycling	
		 */
		virtual unsigned char II_Patient_SS_PowerOffRequest( tPowerOffType PowerOffType ){}
	
		enum{
			Cp_II_Patient_SS_POOutOfRange	= 1,
			Cp_II_Patient_SS_PONotPossible	= 2
		};
		/*!
		 * \brief	Set message should be sent after Power Off Confirm .
					This message is sent to the generator to indicate the number of seconds to wait before the power removal (WaitTimeBeforePowerOff property). 
					Normally, this time should be long enough to allow the PC workstations to do a clean shutdown.
					Once the generator receives this request, notifies with an event to all devices that a power off will be done in the indicated time interval. There is no possibility to cancel power off at this point.
					Maximum selectable time: 10 minutes (600 seconds).
		 */ 
		virtual unsigned char II_Patient_SS_PowerOff( unsigned short WaitTimeBeforePowerOff ){ return Cp_MessageNotAvailable; } 
		/*!
		 * \brief Sets a Power Off   
		 */
		virtual void II_Patient_SS_PowerOff(void){ } 
	
		/*!
		 *	\brief This message is sent when a console cancels the power off sequence. It should be sent before the “Power Off” set.
		 */
		virtual unsigned char II_Patient_SS_PowerOffCancel(void) { return Cp_MessageNotAvailable; } 
	
		/*!
		 * \brief This message should sent to the positioner before the Power Off to confirm the Power Off Request event (power off or reboot).
		 * \param[in]  PowerOffConfirmType => 0: Power off 	1: Power Recycling.
		 */
		virtual unsigned char II_Patient_SS_PowerOffConfirm( tPowerOffType PowerOffConfirmType) { return Cp_MessageNotAvailable; }
	};
};//namesapce
	
#endif
