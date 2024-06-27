#ifndef __ADADICHDLNINTERFACE__
	#define __ADADICHDLNINTERFACE__

#include "aDataDicInterface.h"

namespace R2CP
{
	class CaDataDicHdlInterface
	{
	public:
		
		/*!
		 * \brief Initialitation
		 */
		virtual void Initialitation(void) { }
		
		/**************************** HDL Index *****************************************/
		/*!
		 * \brief Gets the fpga version.
		 */
		virtual unsigned char II_HDL_SS_FPGAVersion(void) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Sets a Fpga reboot.
		 */
		virtual unsigned char II_HDL_SS_FPGAReset(void) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Gets FPFA DNA
		 */
		virtual unsigned char II_HDL_SS_FPGADna(void) { return Cp_MessageNotAvailable; }

		/*!
		 *
		 */
		virtual unsigned char II_HDL_SS_FpgaConnections(void) { return Cp_MessageNotAvailable; }			
	
		enum {
			Cp_II_HDL_SS_FPGA_Nok = 1
		};
		/*!
		 *	\brief Starts Updating Status
		 *	\param[in]	Request	=> 0 : StartProcess		1 : Abort
		 */
		virtual unsigned char II_HDL_SS_FPGAUpdateStatus(unsigned char Request) { return Cp_MessageNotAvailable; }	

		/*!
		 * \brief Gets Current Status
		 */
		 virtual unsigned char II_HDL_SS_FPGAUpdateStatus(void) { return Cp_MessageNotAvailable; }	

		/*!
		 * \brief Get Update Length
		 */
		virtual unsigned char II_HDL_SS_FpgaUpdateLenght(int Length) { return Cp_MessageNotAvailable; }
	
		/*!
		 * \brief Erase Sector
		 */
		virtual unsigned char II_HDL_SS_FpgaUpdateEraseSector_Received(void) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Erase Sector
		 */
		virtual unsigned char II_HDL_SS_FpgaUpdateEraseSector_MaxSector(void) { return Cp_MessageNotAvailable; }

		/*!
		 *	\brief Set Update Prog Page
		 */
		virtual unsigned char II_HDL_SS_FpgaUpdateProgPage(int pageNumber, unsigned char * pData, unsigned short  nData) { return Cp_MessageNotAvailable; }	

		/*!
		 * \brief Gets Updata Prog Page
		 */
		virtual unsigned char II_HDL_SS_FpgaUpdateProgPage(void) { return Cp_MessageNotAvailable; }	

		/*!
		 *	\brief Sets Fpga Checksum
		 */
		virtual unsigned char II_HDL_SS_FpgaUpdateChecksum(unsigned char Chk) { return Cp_MessageNotAvailable; }
	};
};//namespace R2CP

#endif
