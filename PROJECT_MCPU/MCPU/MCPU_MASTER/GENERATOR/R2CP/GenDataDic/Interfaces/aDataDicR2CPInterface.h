#ifndef __ADATADICR2CPINTERFACE_H__
	#define	__ADATADICR2CPINTERFACE_H__

#include "aDataDicInterface.h"

namespace R2CP
{
	typedef struct
	{
		unsigned short  Hardware_Model;	/*!< Hw model version. */
		unsigned char Hardware_Version;	/*!< Hw version. */
		char Hardware_Revision;	/*!< Hw revision. */
		unsigned char SwVersion;			/*!< Sw version. */
		unsigned char SwRevision;		/*!< Sw revision. */
		unsigned char SwSubrevision;		/*!< Sw subrevision. */
		unsigned char BootVersion;		/*!< Boot version. */
		unsigned char BootRevision;		/*!< Boot revision. */
		unsigned char BootSubrevision;	/*!< Boot subrevision. */
		unsigned char HdlVersion;		/*!< Hdl version. */
		unsigned char HdlRevision;		/*!< Hdl revision. */
		unsigned char HdlSubrevision;	/*!< Hdl subrevision. */
		unsigned char GoldenVersion;		/*!< Golden version. */
		unsigned char GoldenRevision;	/*!< Golden revision. */
		unsigned char GoldenSubrevision;	/*!< Golden subrevision. */
		unsigned char Index;
		union 
		{
			struct
			{
				unsigned char HwVersion		:1;
				unsigned char SwVersion		:1;
				unsigned char BootVersion	:1;
				unsigned char HdlVersion		:1;
				unsigned char GolderVersion	:1;
			} Fields;
			unsigned char value;
		} FieldToApply;
	}tSystemVersion;
	
	class CaDataDicR2CPInterface
	{
	public:
		/*!
		 *\brief Constructor 
		 */
		CaDataDicR2CPInterface() { }
		
		/*!
		 * \brief Destructor
		 */
		virtual ~CaDataDicR2CPInterface() { }
		
		/*!
		 * \brief Class Initialitation
		 */
		virtual void Initialitation(void) { }

		/*!
		 * \brief Cleans Last Error 
		 */
		virtual unsigned char II_R2CP_SS_Clean_Error(void) {return Cp_MessageNotAvailable;}

		/*!
		 * \brief Gets all Error
		 */
		virtual unsigned char II_R2CP_SS_Get_Error(void) { return Cp_MessageNotAvailable;}

		/*!
		 * \brief Sets a Board Reset
		 */
		virtual unsigned char II_R2CP_SS_Reset(void) { return Cp_MessageNotAvailable;}

		/*!
		 * \brief Sets MasterLiveTime
		 */
		virtual unsigned char II_R2CP_SS_MasterLiveTime(unsigned short  Time2Configure) { return Cp_MessageNotAvailable;}

		/*!
		 * \brief Send Package Version
		 */
		virtual unsigned char II_R2CP_SS_PackageVersion(char* pPackageVersion) { return Cp_MessageNotAvailable;}
		
		/*!
		 * \brief Sets SerialNumber
		 */
		virtual unsigned char II_R2CP_SS_SerialNumber( unsigned char *pSerialNumber, unsigned short  SizeSerialNumber) { return Cp_MessageNotAvailable;}
		
		/*!
		 * \brief Gets System Versions
		 */
		virtual unsigned char II_R2CP_SS_SystemVersion(  void  ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Reverved Got
		 */
		virtual bool ReservedGot( unsigned char Priority, unsigned char Node , unsigned char Index , unsigned char SubIndex , unsigned char *pData , unsigned short  Size ){ return Cp_MessageNotAvailable;}
	};
};

#endif
