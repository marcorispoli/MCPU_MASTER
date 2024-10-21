#pragma once

/**
	\defgroup DEVIMPL Device Modules Implementation
	\ingroup APPIMPL
	\internal

	This section describes the implementation of the modules controlling the Gantry internal boards.

*/

/**
	\defgroup DEVDESC Devices Management Description
	\ingroup APPDOC
	

	# Index

	\ref PCB301DESC
	\ref PCB302DESC
	\ref PCB303DESC
	\ref PCB304DESC
	\ref PCB325DESC
	\ref PCB326DESC

	# Abstract 

	This section describes the implementation of the modules controlling the Gantry boards

	# Overview

	The Gantry is equipped with several inbternal boards with microcontroller
	in order to execute specific realtime jobs.

	The board are following listed:
	+ PCB301 board: this board controls the power supply and several system GPIOs ;
	+ PCB302 board: this board controls the compressor device activities;
	+ PCB303 board: this board controls the collimation device activities;
	+ PCB304 board: this board controls the potter-grid device activities;
	+ PCB325 board: this board controls the Biopsy ativities;
	+ PCB326 board: this board controls the Obstacle detection activities;


	All the devices supports the same protocol engine based on the Can Bus communication
	and a defined frame data exchange.

	The protocol is desribed in the specific documentation.
	
	This application implement a Base class (\ref CanDeviceProtocol class) that 
	implements all the communication mechanisms: each specific device module 
	inherit this class as a base class. 

		IMPORTANT: the protocol is common for all the devices in order to be a robust 
		and well tested part of the module iplementation.

	The basic features that all the modules inherit from this base class are:
	+ Communication startup management:
		+ Connection fase;
		+ Remote Bootloader management; 
		+ Revision acquisition;
		+ Device Register Configuration;
		+ Board reset handling;

	+ High level routines to get and set the device registers;
	+ High level routines to handle the command execution;



*/

#include "PCB301.h"
#include "PCB302.h"
#include "PCB303.h"
#include "PCB304.h"
#include "PCB325.h"
#include "PCB326.h"
