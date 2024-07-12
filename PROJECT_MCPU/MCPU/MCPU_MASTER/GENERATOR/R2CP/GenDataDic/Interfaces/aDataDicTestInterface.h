#ifndef _ADATADICTEST_H__
	#define _ADATADICTEST_H__

#include "Typedef.h"

namespace R2CP
{		
	class CaDataDicTestInterface
	{
	public:
				
		/*!
		 * \brief Default Destructor
		 */
		virtual ~CaDataDicTestInterface(){}
	
		/*!
		 * \brief Initialitation
		 */
		virtual void Initialitation(void){}
		
		/*!
		 * \brief
		 */
		virtual unsigned char II_Test_SS_HandSwitch_FootSwitch( bool PrepOrder , bool ExpOrder , bool FlOrder , bool DynMode_1 , bool DynMode_2 ){ return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief
		 */
		virtual unsigned char II_Test_SS_HandSwitch_FootSwitch( void );
				
		/*!
		 * \brief
		 */
		virtual unsigned char II_Test_SS_Error( int Id , bool Status , unsigned char Type ){ return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief
		 */
		virtual unsigned char II_Test_SS_PowerControl( unsigned char *pData , unsigned char nData ){ return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief
		 */
		virtual unsigned char II_Test_SS_Uarc( unsigned char *pData , unsigned char nData ){ return Cp_MessageNotAvailable; }

		/*!
		 * \brief
		 */
		virtual unsigned char II_Test_SS_Enable_Log( unsigned char LogId , bool Status ){ return Cp_MessageNotAvailable; }
		
	};
};
#endif
	
