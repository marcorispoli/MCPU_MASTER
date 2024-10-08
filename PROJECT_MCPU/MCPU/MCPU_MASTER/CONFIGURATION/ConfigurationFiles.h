#pragma once



/**
    \defgroup ConfigurationFilesModule Configuration Files Implementation
    \ingroup APPIMPL
    \internal

    This section describes the configuration file modules implementation.
*/


/**
\defgroup ConfigurationFilesDescription Application Configuration File Description
\ingroup APPDOC

   # Abstract
   
   This section describes the configuration files used in the Application.

   # Index

   \ref SystemConfig \n
   \ref MotorConfig \n
   \ref PaddleConfig \n
   \ref CollimatorConfig \n
   \ref FilterConfig \n
   \ref TomoConfig \n
   \ref DetectorConfig \n

*
* # Overview
*
* The Application makes use of several configuration files in order to change its behavior
* and/or its interface.
* 
* The Application autogenerates all the missing configuration files, initializing them with 
* the default values. 
* 
* All the configuration files share the same structure, derived from a common base-class.
* 
* The configuration files are stored into the same directory of the
* Application executable file and cannot be removed from there.
* 
* The basic characteristics are :
*
* + A Readable Text file format;
* + A robust data structure and access data interface, preventing data corruption;
* + Comment strings inside the file describe the meaning of every field;
* + Etherogeneous data content : all data are encoded in string format;
* + File revision management: every configuration file is assigned to a revision code;
*
* # Structure of a configuration file
* 
* There are three kind of raw in the file:
* + The Comment (unformatted) raw: this is any raw not containing the special characters "<" and ">";
* + The Data (formatted) raw: this is the raw containing Data to be decoded, contained into the special characters: "<" and ">".
* + The Revision raw: this is a special Data raw, providing the revision of the configuration file.
* 
* ## Data Raw Structure
* 
* The Data raw structure is a string delimited with the special character "<" and ">" containing an arbitrary number of parameters.
* 
* The structure is the following:
* 
* <DATA_IDENTIFIER,data1, dat2, .., datan>
* + DATA_IDENTIFIER: is a String identifying a given parameter in the file;
* + data1,..,datan: are an arbitrary array of strings, encoding etherogeneus parameters;
  + The number of parameters is different for any Data raw;

* ## Revision Data Raw
* 
* The revision data raw is a special Data raw, containing the current configuration file revision number.
* 
* <REVISION, xx>\n
* + xx: is the revision number in integer format.
* 
* The revision code is verified by the application in order to 
* identify a wrong revision number.
* 
* In case a configuration file should contains a different revision code than expected, \n
  the Application may replace it with a new one or may try to restructure it:
  + The behavior may dependent by the specific configuration file and/or the given revision.
* 
* ## Configuration file example
* Follows a tipical content of a configuration file:
* 
\verbatim
* <REVISION, xx>
* 
* "comment to the param_name1"
* <"PARAM_NAME1", "data-1", "data-2", "...", "data-j">
*
*"comment to the param_name2"
* <"PARAM_NAME2", "data-1", "data-2", "...", "data-k">
*
* "comment to the param_name_nn"
* <"PARAM_NAME_nn", "data-1", "data-2", "...", "data-x">
* 
* \endverbatim
* 
*/



#include "ConfigFile.h"
#include "SystemConfig.h"
#include "PaddleConfig.h"
#include "MotorConfig.h"
#include "DetectorConfig.h"
#include "TomoConfig.h"
#include "CollimatorConfig.h"