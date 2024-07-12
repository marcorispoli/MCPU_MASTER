#pragma once

using namespace System;


ref class Motors
{
public:

	bool active;

	ref class ODRegister {
	public:
		

	
		enum class SDOCommandCodes {
			WRCMD = 0x20,	//!< SDO Write command for frame to the remote device
			WRANSW = 0x60,  //!< SDO Write command ack code from the remote device
			RDCMD = 0x40,   //!< SDO Read command for frame to the remote device
			RDANSW = 0x40,	//!< SDO Read command ack code from the remote device
			ERRACK = 0x80,  //!< SDO Error ack code from the remote device
		};

		enum class SDODataDimension {
			R0B = 0,	//!< SDO null dimension (for nanoj upload procedure)
			R1B = 0x0F, //!< SDO 1 Byte data dimension
			R2B = 0x0B, //!< SDO 2 Byte data dimension
			R3B = 0x07, //!< SDO 3 Byte data dimension
			R4B = 0x03, //!< SDO 4 Byte data dimension
		};

		
		inline static unsigned short  getIndex(unsigned short idx, unsigned char sub, SDODataDimension dim, unsigned int dt) { return idx; }
		inline static unsigned char  getSub(unsigned short idx, unsigned char sub, SDODataDimension dim, unsigned int dt) { return sub; }
		inline static SDODataDimension  getDim(unsigned short idx, unsigned char sub, SDODataDimension dim, unsigned int dt) { return dim; }
		inline static unsigned int  getType(unsigned short idx, unsigned char sub, SDODataDimension dim, unsigned int dt) { return dt; }



		/// <summary>
		/// Instance of a initialized register.
		/// 
		/// This constructor creates an object register 
		/// pointingto a phisical register (with address, data data type) 
		/// and initalizes its data value.
		/// 
		/// </summary>
		/// <param name="index">index of the register</param>
		/// <param name="subidx">sub index of the register</param>
		/// <param name="dim">dimension of the data</param>
		/// <param name="dt">default value</param>
		ODRegister(unsigned short idx, unsigned char sub, SDODataDimension dim, unsigned int def) {
			index = idx;
			subindex = sub;
			data_dim = dim;
			data = def;
			default_data = def;
			valid = false;
		}

		/// <summary>
		/// Instance of a non initialized register
		/// 
		/// This constructor creates an object register 
		/// pointing to a phisical register (with address, data data type) 
		/// but without initializing its content.
		/// 
		/// </summary>
		/// <param name="index">index of the register</param>
		/// <param name="subidx">sub index of the register</param>
		/// <param name="dim">dimension of the data</param>
		ODRegister(unsigned short idx, unsigned char sub, SDODataDimension dim) {
			index = idx;
			subindex = sub;
			data_dim = dim;
			valid = false;
			data = 0;
			default_data = 0;
		}


		static inline unsigned int  getVal(cli::array<Byte>^ frame) {

			switch (frame[0] & 0x0F) {
			case (unsigned char)SDODataDimension::R1B:
				return (unsigned int)frame[4];
			case (unsigned char)SDODataDimension::R2B:
				return ((unsigned int)frame[4] + 0x100 * (unsigned int)frame[5]);
			case (unsigned char)SDODataDimension::R3B:
				return (unsigned int)frame[4] + 0x100 * (unsigned int)frame[5] + 0x10000 * (unsigned int)frame[6];
			case (unsigned char)SDODataDimension::R4B:
				return (unsigned int)frame[4] + 0x100 * (unsigned int)frame[5] + 0x10000 * (unsigned int)frame[6] + 0x1000000 * (unsigned int)frame[7];
			}
			return 0;
		}


		SDOCommandCodes cmd;		//!< Command Code assigned to a register for a pending Read or Write command
		SDODataDimension data_dim;	//!< Data dimension assigned for write commands or received by read commands
		unsigned short index;	//!< Register Index address
		unsigned char subindex;	//!< Regiter SubIndex address 
		unsigned int  data;		//!< 4 Bytes Data content register: assigned ffor write commands, received by read commands
		unsigned int default_data;
		bool valid;				//!< Result of a validation process (in the reception can frame evaluation)

	};
	
	enum class _CiA402Status {
		CiA402_NotReadyToSwitchOn = 0,
		CiA402_SwitchOnDisabled,
		CiA402_ReadyToSwitchOn,
		CiA402_SwitchedOn,
		CiA402_OperationEnabled,
		CiA402_QuickStopActive,
		CiA402_FaultReactionActive,
		CiA402_Fault,
	};

	Motors(unsigned char id);
	cli::array<ODRegister^>^ object_dictionary;
	ODRegister^ findRegister(unsigned short index, unsigned char sub_index);
	int findIndex(unsigned short index, unsigned char sub_index);
	int getEncoder(void) { return current_encoder_position; }
	_CiA402Status getStatus(void) { return CiA_current_status; }
	static float getRotationFromEncoder(int encoder);
	float getUserUnitFromEncoder(int encoder);

	// External inputs
	int voltage_supply_mV;
	double rot_to_unit;

private:
		void threadWork(void);//!< This is the worker thread for the connection management	
		System::Threading::Thread^ running_thread;//!< This is the worker thread handler
		unsigned char devId;
		bool reset_event;

		void canrx_canopen_sdo_event(void);
		void canrx_canopen_nmt_event(void);
		void handle_connection(bool stat);
		void send_bootup_message(void);
		
		_CiA402Status CiA_current_status;     //!< CiA current detected status
		void not_ready_to_switch_on_callback();
		void switched_on_disabled_callback();
		void ready_to_switch_on_callback();
		void switched_on_callback();
		void operation_enabled_callback();
		void quick_stop_active_callback();
		void fault_reaction_active_callback();
		void fault_active_callback();

		void profile_positioning_management();
		bool profile_positioning_init;

		void profile_homing_management();
		bool profile_homing_init;


		bool fault_condition;
		int current_encoder_position;
		unsigned int od_6040_reg;
		bool position_started;
		bool profile_started;
		int target_position;
		bool increment_direction;
		bool target_ok = false;
		int  delta_count;

		


		
};
