#ifndef __METRADINTERFACE_H__
    #define __METRADINTERFACE_H__

#include "aDataDicRadInterface.h"

namespace R2CP
{	
		
    class metDataDicRadInterface: public CaDataDicRadInterface
	{

		
		/*!
		 *	\brief Once the CaDataDicGen is instanced this method is called
		 */
		void Initialitation(void) {};

public:
		/******************************* STATUS AND EXPOSURE MANAGEMENT *********************************************/
		/*!
		 * \brief   Generator provides information about its current internal status and if a transition to a different status is in progress.
		 *			If Status=2, Status transition from=2 and Status transition to=3, 
		 *			it means that generator is preparing to make an exposure after prep signal has been pressed.
		 */
        unsigned char generatoreUpdateStatus(tGeneratorStatus* lastStatus) {
            status = *lastStatus;
            return Cp_Ok;
        }

    private:
        tGeneratorStatus status;


	};//class
};//namesapce
#endif
