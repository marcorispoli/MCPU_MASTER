
/**
	\defgroup MOTIMPL Motor Modules Implementation
	\ingroup APPIMPL
	\internal

	This section describes the implementation of the modules controlling the system Motors

*/

/**
	\defgroup MOTDESC Motor Management Description
	\ingroup APPDOC
	

	# Index

	# Abstract 

	This section describes the implementation of the modules controlling the Gantry Motors

	# Overview

	The Gantry system controls five motorizations in order to accomplish with the system requirements:
	+ Motor controlling the C-ARM Vertical position;
	+ Motor controlling the Body position;
	+ Motor controlling the C-ARM rotation (projections);
	+ Motor controlling the C-ARM sliding;
	+ Motor controlling the Tube-Arm (Tilting) rotation;

	All the Motors are based on the same controlling interface:
	+ CANOpen standard interface.

	The Application implements a Base shared module that all the controller 
	inherit. 
	
	This shared module, implement the common features that all the activation requires:
	+ Implements the CANOpen protocol stack:
		+ Handles the connection with the motor device;
		+ Handles the CiA404 status machine;
		+ Handles the control of the motor register parameters;
		+ Handles the control of the motor activation; 
		+ Handles the Fault management;

	+ Implements the basic activation functions;
		+ Manual Activation mode;
		+ Automatic Activation mode;
		+ Automatic Zero Setting procedure;

	+ Implements the basic safety functions;
		+ Manage the activation timeouts;
		+ Manage the obstacle detection;
		+ Manage the diagnostic on the position sensor;
		
	The communication with the Motor Drivers makes use of the 
	USB to CAN converter device, that the application handles 
	with a dedicated internal process, providing an application 
	common interface to send and receive frames from the CAN bus.

	# Common Features Description

	## Motor Internal Status

	The Motor internal status machine is managed so that the Application
	can have a feedback on the current working status.
	
	A common status description helps the Application to provide
	a standard front-end when requesting to activate one of the available motors.

	Actually the standard internal status flags are (\ref status_options):
	- NOT_CONNECTED: the dirver is not connected to the Can Bus (maybe off, disconnected);
	- MOTOR_CONFIGURATION: this is the startup fase;
	- MOTOR_READY: the motor is ready to process activations;
	- MOTOR_BUSY: an activation is pending;
	- MOTOR_FAULT: the Motor is in Fault condition

		NOTE: The motor fault condition it is not necessarily a System fault. 
		For example, during a compression the motor power supply is switched off for safety reason, and the motor device 
		activate amn internal fault due to the missing power supply.

	## Motor Device Connection

	The initial startup fase is commonly handled by the Base module
	in order to establish a valid connection with the target motor device.

	During the startup fase:
	- The Can connection is established;
	- The Motor is configured (see the Motor Device Configuration section) with a set of common registers;
	- Every Motor is configured with a subset of registers that depend by the target motor;
	- The current motor position is initialized (either if it is an external sensor or the device internal incremental encoder);

	The Startup fase never ends until the previous steps are not successfully completed.

	When the startup fase completes successfully, 
	the application base module handle the internal status machine so that it is automatically set 
	to Ready status.

	In the event of fault condition, the application tries automatically to reset the fault as soon as the 
	fault event disappear.

	## Motor Device Configuration
	
	There are basically three kind of motor configurations:
	+ the motor control setup; 
	+ the basic motor configuration;
	+ the performance of the motor activations.

	### Motor Control Setup

	The motor can be configured to use an external sensor or the internal Encoder:
	+ If the motor is configured with the internal encoder:
		+ the position is initialized reading the actual position stored into the \ref MotorConfig; 
		+ the current position is then totally internally managed be the motor device;
	+ If the motor is configured with the external sensor:
		+ the position is initialized reading the external sensor and initializing the internal encoder with the sensor;
		+ external sensor (potentiometer) shall be calibrated;
		+ the calibrated parameters are stored into the  \ref MotorConfig;

		See the Position contro section for detail.

	During the startup fase every Motor can upload (if needed) a special program (Nano-J application) running inside the device 
	for special functions.

	See the detail of every motor description.

	### Basic Motor Configurations

	Every motor sets its own subset of parameters to tune the reaction of that motor propery, with respect of the
	mechanical asset and its required performances.
	
	The parameters that usually are set into this fase are:

	- The Maximum Current per fase;
	- The Maximum speed;
	- The Maximum acceleration;
	- The Position limits;
	- The PID parameters;
	- The GPIO configuration;

	During the Startup fase, every implemented motor controller is requested to upload
	its subset of registers.

	### Performances

	Every motor can set its own performances reading the content of the \ref MotorConfig.

	The following parameters can dinamically be configured:
	+ The Automatic activation speed, acceleration, deceleration  parameters;
	+ The Manual activation speed, acceleration, deceleration  parameters;
	+ The Automatic (if necessary) Zero setting speed and acceleration;
	
	Those parameters can be changed runtime after the application is compiled.

	## Position Handling

	Every motor control can be configured to use only the internal incremental encoder 
	or an external potentiometer, connected to the Analog Input of the motor device.

	See the \ref MotorConfig description for details. 

	### Internal Incremental Encoder Mode
	
	When the motor control setup is set to use only the internal encoder, the actual position 
	is handled internally by the motor device. 

	Because the internal position is reset after every power off, in order to keep a valid absolute position,
	after every activation completion, the actual position is stored into the \ref MotorConfig:
	- During the startup fase, the actual position is then retrived from the configuration file.

	In case of file corruption, first istallation, or whenever the position should be considered no more valid, 
	a special automatic or manual zero setting procedure can be requested:
	- a new valid actual position is then acquiored and stored into the configuration file.

	See the Zero Setting Management section for details.
	

	### External Position Sensor

	If the motor control is configured to make use of an external position sensor as a reference, 
	this sensor is used in the startup fase in order to initialize the internal encoder with the actual absolute position.
	
	The position then will be handled as in the Internal Incremental Encoder Mode.

	In order to be used, however, the external sensor position shall be calibrated:
	+ the calibration consists to store the sensor readout value corresponding to the virtual zero position.

	The calibration is executed with a proper panel in the Service Tool (see the related documentation).

	Without a valid calibration, the motor module sets a proper Warning message, preventing 
	any operative activation (but enabling the Service Activation for service purpose).

	
	## Fault Management

	Every motor control module handles the Faults signalled by the Motor Device.

	An active Fault status always prevents to activate the given motorization.

	The Application reads the current fault code from the remote motor device, 
	logging it into the Gantry.log file:
	+ A generic application Warning message will be activated and displayed in the Message Panel Window;

	The application automatically will retry to exits from the Fault status:
	+ in the case the fault condition should clear, the normal motor operation will be restored.

	If a Fault condition should happen during any activation mode, the activation will be quickly 
	aborted.


	## Obstacle Detection

	Every motor controller share the same mechanism to handle the obstacle detection:
	- the Application, based on the current operating mode, assignes the obstacle triggers 
	to the current motor ativated. 

	When the obstacle is triggered, if a motor is activated immediatelly stops.

	The Obstacle detection is active only for the Automatic activations.

		NOTE: the manual activation, for its intrinsic safety, doesn't require the obstacle detection.

	See the specific Motor function section for detailed description about the obstacle detection. 

	## Manual Activation Button management

	Every motor control module inherits a shared mechanism to check the activation 
	of system IOs that the Application, in a similar way as it mades for the obstacle triggers, 
	assignes to be a manual activation triggers.

	During every operating status, the motor control checks the status of the manual triggers,
	and if they are enabled by the application, they causes the following reaction:
	+ If the motor is in Idle, a manual activation is started;
	+ If the motor is running with an automatic activation, the activation stops;

	The Application may enable or disable the manual triggers individually for any motor
	based on the current operating contest.

	See the description of the given motor control for details.

	## Customization preparation and termination procedures

	Every motor controller module inherits a shared method to customize the activation
	preparation and termination:

	+ During the preparation of the motor activation, just after having applied the torque, the controller module 
	of the activating motor receives a call to a special function in order to let that module 
	to prepare for the activation: for example in case of stationary brake, it has to be released before to start.
	+ The similar method is used to terminates an activation: just before to remove the torque, the controller of the terminating motor 
	receives a call to a specific termination procedure: in the case of the brakes, for example, the stationary brake shall be activated.

	See the specific motor function implementation for details.

	## Automatic Position Activation

	Every motor inherits this basic activation mode.

	The Automatic activation allows the motor control module to set a particolar target,
	to set the activation parameters (speed, acceleration, deceleration)  and to wait 
	for the command completion.

	During the activation:
	+ if the obstacle is detected, the activation quickly stops;
	+ if the manual activation is triggered, the activation quickly stops;
	+ If a fault condition is detected, the activation quickly stops;
	
	The activation successfully termines when:
	- the motor device detect the target position;
	- the control module detects that the current position is in an acceptable target range for almost one second;

		NOTE: although the motor device should handle the activation completion, the module always check the current position
		to prevent a system lock if the motor driver should fail in monitoring the target range.

	There are two type of automatic activations:
	+ an automatic starting activation: the motor starts running after motor preparation;
	+ a nano-j start: the motor is prepared for the activation, and a special program is loaded into the 
	motor. The program, should activate the motor when the start condition are detected. 

	See the specific motor function implementation for details.


	## Manual Position Activation

	Every motor controller inherits the the Manual position activation mode.

	The manual position activate the motor when the manual activation triggers are detected active.

	The direction of motor rotation is assigned by the application to the triggers.

	The motor will continue to run until:
	- the limit position is detected;
	- the manual trigger are released;
	- the manual triggers of the opposed direction is activated;

	
	## Zero Setting Management

	The controller modules inherit the procedure to initialize the absolute position 
	to a known mechanical position.

	There are two zero setting procedures:
	+ the zero setting procedure with the external trigger;
	+ the zero setting istant procedure, without motor rotation;

	### Zero setting automatic procedure with external trigger

	This procedure makes use of an external trigger, usually a photocell, 
	in order to detect the mechanical zero position.

	When the procedure is activated, the motor starts rotating until 
	detect the trigger activation. The activation immediatelly termines and the 
	actual position is updated with the real mechanical position. 
	The current position is then stored into the \ref MotorConfig file.

	During the zero setting activation:
	- the obstacle detection is monitored: the zero setting is aborted in the case they should detected;
	- the manual activatin triggers are monitored: the zero setting is aborted in the case they should detected;

		NOTE: during the zero setting procedure, the limit position register is ignored.


	### Zero setting istant procedure

	This procedure can be activated when the current absolute position is known:
	the encoder is istantly initialized with the actual position, without to 
	start the rotation. 

	This procedure is internally used by the innher module of every motor controller 
	in case it is using an external position sensor: in this case, at the startup,
	the actual position is provided by the current value of the sensor.






*/

