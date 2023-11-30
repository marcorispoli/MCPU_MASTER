#ifndef _ADATADICINTERFACE_H__
	#define _ADATADICINTERFACE_H__



namespace R2CP
{
	enum{
		Cp_Ok = 0,
		Cp_Nok,
		Cp_MessageNotAvailable = 202
	};
	
	typedef union
	{
		struct{
			int data_0_7	 : 8;
			int data_8_15	 : 8;
			int data_16_23 : 8;
			int data_24_31 : 8;
		}Fields;
		int value;
	}udword;
	
	typedef union
	{
		struct{
			unsigned short  data_0_7	 : 8;
			unsigned short  data_8_15	 : 8;
		}Fields;
		unsigned short  value;
	}uword;
	
	typedef union
	{
		struct{
			unsigned char data_0_3	: 4;
			unsigned char data_4_7	: 4;
		}Fields;
		unsigned char value;
	}ubyte;
		
	class CaDataDicInterface
	{
	public:
				
		/*!
		 * \brief Default Destructor
		 */
		virtual ~CaDataDicInterface(){ }
	
		/*!
		 * \brief Initialitation
		 */
		virtual void Initialitation(void){ }

		/*!
		 * \brief According to the generator status some messages cannot be replied.
		 */
		virtual bool CheckNotAvailableMessages_vs_Status( unsigned short  Index , unsigned short  SubIndex ) { return true; }

	};
};
#endif
	
