#ifndef _CADATADICSERVICEINTERFACE_H__
	#define _CADATADICSERVICEINTERFACE_H__

#include "aDataDicInterface.h"

namespace R2CP
{

	typedef struct{
		unsigned char 	PowerKW;
		unsigned char 	MaxKvp;
		unsigned char 	MinKvp;
		unsigned char 	Max_mA_data_0_7;
		unsigned char 	Max_mA_data_8_15;
		unsigned char 	Min_mA_data_0_7;
		unsigned char 	Min_mA_data_8_15;
		unsigned char 	Max_mAs1000_data_0_7;
		unsigned char 	Max_mAs1000_data_8_15;
		unsigned char 	Max_mAs1000_data_16_23;
		unsigned char 	Max_mAs1000_data_24_31;
		unsigned char 	Min_mAs1000_data_0_7;
		unsigned char 	Min_mAs1000_data_8_15;
		unsigned char 	Min_mAs1000_data_16_23;
		unsigned char 	Min_mAs1000_data_24_31;
		unsigned char 	Max_ms_data_0_7;
		unsigned char 	Max_ms_data_8_15;
		unsigned char 	Min_ms_data_0_7;
		unsigned char 	Min_ms_data_8_15;
		unsigned char 	Max_PPS;
		bool	High_SpeedEnabled;
		bool	AEC_Enabled;
		bool	Tomography_Enabled;
		bool	DualEnergy_Enabled;
		bool	R20Scale_Enabled;
		bool	RF_Enabled;
		bool	TrackingFormula0P_Enabled;
		bool	FluoroCurves_Enabled;
		bool	DSI_Enabled;
		bool	DigitalEnd_Enabled;
		bool	DSA_Enabled;
		bool	Cine_Enabled;
		bool	HCF_Enabled;
		bool	Scan_Enabled;
		unsigned char 	PowerSupply;
		bool	BatteriesPSU_Enabled;
		bool	BatteriesStationary_Enabled;
		bool	Capacitors_Enabled;
		bool	StarterBooster_Enabled;
		unsigned char 	ACFilter;
		unsigned char 	HighDutyCycle;
		unsigned char 	Mamography;
		unsigned char 	InterfaceOptions_data_0_7;
		unsigned char 	InterfaceOptions_data_8_15;
		bool	StarterEnabled;
		unsigned char 	Spare46;
		unsigned char 	Spare47;
		unsigned char 	Spare48;
		bool	Pikachu_Bucky_Enabled;
		bool	Pikachy_SerialPort_Enabled;
		bool	DemoMode_Enabled;
		unsigned char 	StarterType;
		unsigned char 	Spare53;
		unsigned char 	Spare54;
		unsigned char 	Spare55;
		unsigned char 	Spare56;
		unsigned char 	Spare57;
		unsigned char 	Spare58;
		unsigned char 	Spare59;
		unsigned char 	Spare60;
		unsigned char 	Spare61;
		unsigned char 	Spare62;
		unsigned char 	Spare63;
		unsigned char 	Spare64;
	}tStandardLicense;
		                
	typedef struct{
		unsigned char 	System;
		unsigned char 	Generator;
		unsigned char 	Client;
		unsigned char 	ImageSystem;
		unsigned char 	StartupMode;
		bool	Stitching_Enabled;
		bool	Image_Preview_Enabled;
		bool	Tomosynthesis_Enabled;
		unsigned char 	DAP;
		bool	AutoCollimation_Enabled;
		bool	DigitalInterface_Enabled;
		bool	DetectorAligmentAssistance_Enabled;
		bool	RemoteExposureControl_Enabled;
		unsigned char 	FluoromAStation;
		unsigned char 	isCyphered;
		unsigned char 	ServiceModeProtected;
		unsigned char 	Spare17;
		unsigned char 	Spare18;
		unsigned char 	Spare19;
		unsigned char 	Spare20;
		unsigned char 	Spare21;
		unsigned char 	Spare22;
		unsigned char 	Spare23;
		unsigned char 	Spare24;
		unsigned char 	Spare25;
		unsigned char 	Spare26;
		unsigned char 	Spare27;
		unsigned char 	Spare28;
		unsigned char 	Spare29;
		unsigned char 	Spare30;
		unsigned char 	Spare31;
		unsigned char 	Spare32;
		unsigned char 	Spare33;
		unsigned char 	Spare34;
		unsigned char 	Spare35;
		unsigned char 	Spare36;
		unsigned char 	Spare37;
		unsigned char 	Spare38;
		unsigned char 	Spare39;
		unsigned char 	Spare40;
		unsigned char 	Spare41;
		unsigned char 	Spare42;
		unsigned char 	Spare43;
		unsigned char 	Spare44;
		unsigned char 	Spare45;
		unsigned char 	Spare46;
		unsigned char 	Spare47;
		unsigned char 	Spare48;
		unsigned char 	Spare49;
		unsigned char 	Spare50;
		unsigned char 	Spare51;
		unsigned char 	Spare52;
		unsigned char 	Spare53;
		unsigned char 	Spare54;
		unsigned char 	Spare55;
		unsigned char 	Spare56;
		unsigned char 	Spare57;
		unsigned char 	Spare58;
		unsigned char 	Spare59;
		unsigned char 	LicenseRevision;
		unsigned char 	Spare61;
		unsigned char 	Spare62;
		unsigned char 	Spare63;
		unsigned char 	LicenseVersion;
	}tExtendedLicense;
	
	typedef struct{
		tStandardLicense StandardLicense;
		tExtendedLicense ExtendedLicense;
	}tR2CPLicense;
	
	typedef struct{
		unsigned char 				SourceNode;
		unsigned char 				ErrorCategory;
		unsigned long long 	Date;
		char 				Info[255];
	}tServiceLog;
	
	class CaDataDicServiceInterface
	{
		
	public:
		
		/*!
		 * \brief Class Destructor
		 */
		virtual ~CaDataDicServiceInterface(){}
		
		/*
		 * \brief	The intended use of this message is to read the license options. 
		 *			The device which needs to know the license options should ask for them to the generator sending the GET message. 
		 *			The generator will answer sending the EVENT message to the device which asked for them.
		 */
		virtual unsigned char II_Service_SS_StandardLicense(void) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets the Extended License
		 */
		virtual unsigned char II_Service_SS_ExtendedLicense(void) { return Cp_MessageNotAvailable; }
	};
};

#endif
