 /** \addtogroup MessageNotifyDescription
 
\section MSGDESC Messages Description Table 
 
In this section the meaning of every message will be described in detail.
 
 | MESSAGE CODE | TYPE | Description |
 |:--|:--|:--|
|E00000|ERROR|The application risen a non handled exception.<br>This is a possible bug scenario not tested or not investigated yet.<br>See the Gantry.log file for details.|
|E00001|ERROR|An error has been detected during the connection with the AWS.<br>The problem may be due to a wrong address set into the SystemIni.cnf file <br>for the Command socket and/or Event socket.<br>See the AWS protocol documentation for details.<br>|
|E00002|ERROR|The PCB301 is the service power board. <br>The board signalled a reset event (the microcontroller is restarted).<br>The Application can handle this event restoring the current device status properly.<br>However this event may suggest a problem with the cabling (power cable) or a board issue.<br>Frequent events like that cannot be accepted as normal behavior.|
|E00003|ERROR|The PCB302 is the compressor device. <br>The board signalled a reset event (the microcontroller is restarted).<br>The Application can handle this event restoring the current device status properly.<br>However this event may suggest a problem with the cabling (power cable) or a board issue.<br>Frequent events like that cannot be accepted as normal behavior.<br>|
|E00004|ERROR|The PCB303 is the collimator device. <br>The board signalled a reset event (the microcontroller is restarted).<br>The Application can handle this event restoring the current device status properly.<br>However this event may suggest a problem with the cabling (power cable) or a board issue.<br>Frequent events like that cannot be accepted as normal behavior.<br>|
|E00005|ERROR|The PCB304 is the grid controller device. <br>The board signalled a reset event (the microcontroller is restarted).<br>The Application can handle this event restoring the current device status properly.<br>However this event may suggest a problem with the cabling (power cable) or a board issue.<br>Frequent events like that cannot be accepted as normal behavior.<br>|
|E00006|ERROR|The PCB325 is the biopsy detection device. <br>The board signalled a reset event (the microcontroller is restarted).<br>The Application can handle this event restoring the current device status properly.<br>However this event may suggest a problem with the cabling (power cable) or a board issue.<br>Frequent events like that cannot be accepted as normal behavior.<br><br>|
|E00007|ERROR|The PCB326 is the obstacle detection device. <br>The board signalled a reset event (the microcontroller is restarted).<br>The Application can handle this event restoring the current device status properly.<br>However this event may suggest a problem with the cabling (power cable) or a board issue.<br>Frequent events like that cannot be accepted as normal behavior.<br>|
|E00008|ERROR| PCB301 Communication<br> The board no longer communicate with the MCPU|
|E00009|ERROR| PCB302 Communication<br> The board no longer communicate with the MCPU|
|E00010|ERROR| PCB303 Communication<br> The board no longer communicate with the MCPU|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TYPE | Description |
 |:--|:--|:--|
|E00011|ERROR| PCB304 Communication<br> The board no longer communicate with the MCPU|
|E00012|ERROR| PCB325 Communication<br> The board no longer communicate with the MCPU|
|E00013|ERROR| PCB326 Communication<br> The board no longer communicate with the MCPU|
|E00014|ERROR| Xray push button fault<br> Xray push button locked On|
|E00015|ERROR| Motor Pedals Fault<br> Up/Down pedals locked On|
|E00016|ERROR| Motor Pedals Fault<br> Compressor pedals locked On|
|E00017|ERROR| Motor Buttons Fault<br> C-Arm rotation buttons locked On|
|E00018|ERROR| Motor Buttons Fault<br> Up/Down buttons locked On|
|E00019|ERROR| Software Configuration Fault<br> Wrong software or firmware revision detected|
|E00020|ERROR| System Power Monitoring<br> Power down condition detected!|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TYPE | Description |
 |:--|:--|:--|
|E00021|ERROR| System Power Monitoring<br> Emergency button activation detected!|
|E00022|ERROR| System Safety Monitoring<br> Safety Cabinet activation detected!|
|E00023|ERROR| System Power Monitoring<br> Battery voltage monitoring error! The battery voltage level is too low.|
|E00024|ERROR| Generator Status<br> Generator device not connected|
|E00025|ERROR| Collimator Monitoring<br> The Collimator selector seams to be blocked|
|E00026|ERROR| Filter Monitoring<br> The Filter selector seams to be blocked|
|E00027|ERROR| Tube Temperature Monitoring<br> The Tube Stator sensor connection input seams to be broken|
|E00028|ERROR| Tube Temperature Monitoring<br> The Tube Stator sensor connection input seams to be shorted|
|E00029|ERROR| Tube Temperature Monitoring<br> The Tube Bulb sensor connection input seams to be broken|
|E00030|ERROR| Tube Temperature Monitoring<br> The Tube Bulb sensor connection input seams to be shorted|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TYPE | Description |
 |:--|:--|:--|
|E00031|ERROR| Body Motor Monitoring<br> The brake device is in Fault condition|
|E00032|ERROR| Tilt Motor Monitoring<br> The brake device is in Fault condition|
|E00033|ERROR| Vertical Motor Monitoring<br> The encoder is not initialized|
|E00034|ERROR| ARM Motor Monitoring<br> The encoder is not initialized|
|E00035|ERROR| Slide Motor Monitoring<br> The encoder is not initialized|
|E00036|ERROR| Tilt Motor Monitoring<br> The encoder is not initialized|
|E00037|ERROR| Body Motor Monitoring<br> The encoder is not initialized|
|E00038|ERROR| Body Motor Monitoring<br> The limit switch is active|
|E00039|ERROR| Body Motor Monitoring<br> Unable to read the potentiometer input|
|E00040|ERROR| Vertical Motor Monitoring<br> The limit switch is active|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TYPE | Description |
 |:--|:--|:--|
|E00041|ERROR| Generator Status Monitoring<br> The Generator configuration is failed|
|E00042|ERROR| Generator Messages<br> There are error messages|
|E00043|ERROR| Potter Messages<br> The Grid device is in fault condition|
|E00900|ERROR| X-RAY Sequence Error<br> X-Ray push button early released|
|E00901|ERROR| X-RAY Sequence Error<br> Lower Anodic Current|
|E00902|ERROR| X-RAY Sequence Error<br> Lower kV than expected|
|E00903|ERROR| X-RAY Sequence Error<br> Exposure Timeout|
|E00904|ERROR| X-RAY Sequence Error<br> HS Starter Error|
|W00001|WARNING| Compressor Monitoring<br> Missing compression force.|
|W00002|WARNING| Component Monitoring<br> Undetected patient protection component.|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TYPE | Description |
 |:--|:--|:--|
|W00003|WARNING| Arm Position Monitoring<br> C-ARM angle out of range. Repeat the C-ARM positioning.|
|W00004|WARNING| Compressor Monitoring<br> The position is not calibrated.|
|W00005|WARNING| Compressor Monitoring<br> The force is not calibrated.|
|W00006|WARNING| Compressor Monitoring<br> A valid paddle is not detected.|
|W00007|WARNING| Compressor Monitoring<br> Wrong Paddle model detected.|
|W00008|WARNING| Exposure Parameters Monitoring<br> Missing exposure mode selection.|
|W00009|WARNING| Exposure Parameters Monitoring<br> Missing exposure data selection.|
|W00010|WARNING| X-Ray Button Monitoring<br> The X-Ray button is not enabled.|
|W00011|WARNING| Safety Monitoring<br> The Study's door is detected Open.|
|W00012|WARNING| Generator Status Monitoring<br> The Generator is not ready for exposure.|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TYPE | Description |
 |:--|:--|:--|
|W00013|WARNING| X-Ray Tube Monitoring<br> The X-Ray Tube cumulated energy is too high|
|W00014|WARNING| Collimator Light Command Failed<br> The Collimation Light cannot be activated now. Try again.|
|W00015|WARNING| Tube Temperature Monitoring<br> The Tube Stator temperature is too high|
|W00016|WARNING| Tube Temperature Monitoring<br> The Tube Bulb temperature is too high|
|W00017|WARNING| Tube Temperature Monitoring<br> The Anode temperature is too high|
|W00018|WARNING| Filter Monitoring<br> The Filter selector is not in a valid position|
|W00019|WARNING| Collimator Monitoring<br> An invalid collimation format is detected|
|W00020|WARNING| Generator Monitoring<br> The Generator is in Service Mode|
|W00021|WARNING| Potter Monitoring<br> The grid is not in the expected position|
|W00022|WARNING| Potter Monitoring<br> The grid activation is momentary disabled|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TYPE | Description |
 |:--|:--|:--|
|I00001|INFORMATION| System Power Monitoring<br> The backup batteries are detected Disabled!|
|I00002|INFORMATION| Projection Confirmation Action<br> The C-ARM may be activated|
|I00003|INFORMATION| Abort Projection Request<br> Proceed to Abort the current projection?|
|I00004|INFORMATION| Motor Slide Activation<br> Proceed with the Slide activation?|
|I00005|INFORMATION| Motor Body Activation<br> Proceed with the Body activation?|
|I00006|INFORMATION| Motor Arm Activation<br> Proceed with the Arm activation?|
|I00007|INFORMATION| Motor Tilt Activation<br> Proceed with the Tilt activation?|
|I00008|INFORMATION| Motor Slide Activation<br> Invalid activation conditions|
|I00009|INFORMATION| Motor Body Activation<br> Invalid activation conditions|
|I00010|INFORMATION| Motor Arm Activation<br> Invalid activation conditions|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TYPE | Description |
 |:--|:--|:--|
|I00011|INFORMATION| Motor Tilt Activation<br> Invalid activation conditions|
|I00012|INFORMATION| Motor Power Supply Monitoring<br> The safety line is off|
|I00013|INFORMATION| Motor Power Supply Monitoring<br> The power supply is off|
|I00014|INFORMATION| Power Service Monitoring<br> The Burning Jumper is present|
|I00015|INFORMATION| Power Service Monitoring<br> The power supply lock is active|
|I00016|INFORMATION| Manual Motor Activation<br> The manual activation is disabled in this contest|
|I00017|INFORMATION| Manual Motor Activation<br> The manual activation is disabled for safety|
|I00018|INFORMATION| Manual Motor Activation<br> The manual activation is disabled for pending errors|
|I00019|INFORMATION| Body Motor Monitoring<br> The motor driver is in error condition|
|I00020|INFORMATION| Vertical Motor Monitoring<br> The motor driver is in error condition|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TYPE | Description |
 |:--|:--|:--|
|I00021|INFORMATION| Tilt Motor Monitoring<br> The motor driver is in error condition|
|I00022|INFORMATION| Arm Motor Monitoring<br> The motor driver is in error condition|
|I00023|INFORMATION| Slide Motor Monitoring<br> The motor driver is in error condition|
|I00024|INFORMATION| Power Off Request<br> Proceed with the System Power Off?|
|I00025|INFORMATION| Potter Monitoring<br> The Grid positioning is set to manual mode (test mode)|
|L00001|LABEL| Error Window Panel<br> -|
|L00002|LABEL| Projection Selection<br> -|
|L00003|LABEL| Zero Setting Panel<br> -|
|L00004|LABEL| Service Panel<br> -|
|L00005|LABEL| Calibration Panel<br> -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TYPE | Description |
 |:--|:--|:--|
|L00006|LABEL| Rotation Tool Panel<br> -|
|L00010|LABEL| Standby<br> -|
|L00011|LABEL| Ready<br> -|
|L00012|LABEL| X-Ray Executing<br> -|
|L00013|LABEL| Command successfully completed<br> -|
|L00014|LABEL| Command manually terminated<br> -|
|L00015|LABEL| Obstacle detected error<br> -|
|L00016|LABEL| Motor busy error<br> -|
|L00017|LABEL| Command initialization error<br> -|
|L00018|LABEL| Unexpected internal status error<br> -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TYPE | Description |
 |:--|:--|:--|
|L00019|LABEL| Limit switches activation error<br> -|
|L00020|LABEL| Brake device activation error<br> -|
|L00021|LABEL| Timeout activation error<br> -|
|L00022|LABEL| Driver internal error<br> -|
|L00023|LABEL| Driver access register error<br> -|
|L00024|LABEL| Missing zero setting error<br> -|
|L00025|LABEL| Motor activation disabled error<br> -|
|L00026|LABEL| Motor activation aborted<br> -|
|L00027|LABEL| Compression Activated<br> -|
|L00028|LABEL| Arm Motor Activated<br> -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TYPE | Description |
 |:--|:--|:--|
|L00029|LABEL| Body Motor Activated<br> -|
|L00030|LABEL| Vertical Motor Activated<br> -|
|L00031|LABEL| Slide Motor Activated<br> -|
|L00032|LABEL| Tilt Motor Activated<br> -|
|L00033|LABEL| Xray Tube Calibration Panel<br> -|
\section MSGTRANS Messages Translation Tables 
 
In this section, a table for every translated language is showed.
 
 
 ## Translation  eng table:
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00000| Compilation Error| Invalid module condition detected. See the log file|
|E00001| AWS Communication| Fatal error in opening the socket. Check the network parameters.|
|E00002| PCB301 Communication| The Board has been reset|
|E00003| PCB302 Communication| The Board has been reset|
|E00004| PCB303 Communication| The Board has been reset|
|E00005| PCB304 Communication| The Board has been reset|
|E00006| PCB325 Communication| The Board has been reset|
|E00007| PCB326 Communication| The Board has been reset|
|E00008| PCB301 Communication| The board no longer communicate with the MCPU|
|E00009| PCB302 Communication| The board no longer communicate with the MCPU|
|E00010| PCB303 Communication| The board no longer communicate with the MCPU|
|E00011| PCB304 Communication| The board no longer communicate with the MCPU|
|E00012| PCB325 Communication| The board no longer communicate with the MCPU|
|E00013| PCB326 Communication| The board no longer communicate with the MCPU|
|E00014| Xray push button fault| Xray push button locked On|
|E00015| Motor Pedals Fault| Up/Down pedals locked On|
|E00016| Motor Pedals Fault| Compressor pedals locked On|
|E00017| Motor Buttons Fault| C-Arm rotation buttons locked On|
|E00018| Motor Buttons Fault| Up/Down buttons locked On|
|E00019| Software Configuration Fault| Wrong software or firmware revision detected|
|E00020| System Power Monitoring| Power down condition detected!|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00021| System Power Monitoring| Emergency button activation detected!|
|E00022| System Safety Monitoring| Safety Cabinet activation detected!|
|E00023| System Power Monitoring| Battery voltage monitoring error! The battery voltage level is too low.|
|E00024| Generator Status| Generator device not connected|
|E00025| Collimator Monitoring| The Collimator selector seams to be blocked|
|E00026| Filter Monitoring| The Filter selector seams to be blocked|
|E00027| Tube Temperature Monitoring| The Tube Stator sensor connection input seams to be broken|
|E00028| Tube Temperature Monitoring| The Tube Stator sensor connection input seams to be shorted|
|E00029| Tube Temperature Monitoring| The Tube Bulb sensor connection input seams to be broken|
|E00030| Tube Temperature Monitoring| The Tube Bulb sensor connection input seams to be shorted|
|E00031| Body Motor Monitoring| The brake device is in Fault condition|
|E00032| Tilt Motor Monitoring| The brake device is in Fault condition|
|E00033| Vertical Motor Monitoring| The encoder is not initialized|
|E00034| ARM Motor Monitoring| The encoder is not initialized|
|E00035| Slide Motor Monitoring| The encoder is not initialized|
|E00036| Tilt Motor Monitoring| The encoder is not initialized|
|E00037| Body Motor Monitoring| The encoder is not initialized|
|E00038| Body Motor Monitoring| The limit switch is active|
|E00039| Body Motor Monitoring| Unable to read the potentiometer input|
|E00040| Vertical Motor Monitoring| The limit switch is active|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00041| Generator Status Monitoring| The Generator configuration is failed|
|E00042| Generator Messages| There are error messages|
|E00043| Potter Messages| The Grid device is in fault condition|
|E00900| X-RAY Sequence Error| X-Ray push button early released|
|E00901| X-RAY Sequence Error| Lower Anodic Current|
|E00902| X-RAY Sequence Error| Lower kV than expected|
|E00903| X-RAY Sequence Error| Exposure Timeout|
|E00904| X-RAY Sequence Error| HS Starter Error|
|W00001| Compressor Monitoring| Missing compression force.|
|W00002| Component Monitoring| Undetected patient protection component.|
|W00003| Arm Position Monitoring| C-ARM angle out of range. Repeat the C-ARM positioning.|
|W00004| Compressor Monitoring| The position is not calibrated.|
|W00005| Compressor Monitoring| The force is not calibrated.|
|W00006| Compressor Monitoring| A valid paddle is not detected.|
|W00007| Compressor Monitoring| Wrong Paddle model detected.|
|W00008| Exposure Parameters Monitoring| Missing exposure mode selection.|
|W00009| Exposure Parameters Monitoring| Missing exposure data selection.|
|W00010| X-Ray Button Monitoring| The X-Ray button is not enabled.|
|W00011| Safety Monitoring| The Study's door is detected Open.|
|W00012| Generator Status Monitoring| The Generator is not ready for exposure.|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|W00013| X-Ray Tube Monitoring| The X-Ray Tube cumulated energy is too high|
|W00014| Collimator Light Command Failed| The Collimation Light cannot be activated now. Try again.|
|W00015| Tube Temperature Monitoring| The Tube Stator temperature is too high|
|W00016| Tube Temperature Monitoring| The Tube Bulb temperature is too high|
|W00017| Tube Temperature Monitoring| The Anode temperature is too high|
|W00018| Filter Monitoring| The Filter selector is not in a valid position|
|W00019| Collimator Monitoring| An invalid collimation format is detected|
|W00020| Generator Monitoring| The Generator is in Service Mode|
|W00021| Potter Monitoring| The grid is not in the expected position|
|W00022| Potter Monitoring| The grid activation is momentary disabled|
|I00001| System Power Monitoring| The backup batteries are detected Disabled!|
|I00002| Projection Confirmation Action| The C-ARM may be activated|
|I00003| Abort Projection Request| Proceed to Abort the current projection?|
|I00004| Motor Slide Activation| Proceed with the Slide activation?|
|I00005| Motor Body Activation| Proceed with the Body activation?|
|I00006| Motor Arm Activation| Proceed with the Arm activation?|
|I00007| Motor Tilt Activation| Proceed with the Tilt activation?|
|I00008| Motor Slide Activation| Invalid activation conditions|
|I00009| Motor Body Activation| Invalid activation conditions|
|I00010| Motor Arm Activation| Invalid activation conditions|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|I00011| Motor Tilt Activation| Invalid activation conditions|
|I00012| Motor Power Supply Monitoring| The safety line is off|
|I00013| Motor Power Supply Monitoring| The power supply is off|
|I00014| Power Service Monitoring| The Burning Jumper is present|
|I00015| Power Service Monitoring| The power supply lock is active|
|I00016| Manual Motor Activation| The manual activation is disabled in this contest|
|I00017| Manual Motor Activation| The manual activation is disabled for safety|
|I00018| Manual Motor Activation| The manual activation is disabled for pending errors|
|I00019| Body Motor Monitoring| The motor driver is in error condition|
|I00020| Vertical Motor Monitoring| The motor driver is in error condition|
|I00021| Tilt Motor Monitoring| The motor driver is in error condition|
|I00022| Arm Motor Monitoring| The motor driver is in error condition|
|I00023| Slide Motor Monitoring| The motor driver is in error condition|
|I00024| Power Off Request| Proceed with the System Power Off?|
|I00025| Potter Monitoring| The Grid positioning is set to manual mode (test mode)|
|L00001| Error Window Panel| -|
|L00002| Projection Selection| -|
|L00003| Zero Setting Panel| -|
|L00004| Service Panel| -|
|L00005| Calibration Panel| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|L00006| Rotation Tool Panel| -|
|L00010| Standby| -|
|L00011| Ready| -|
|L00012| X-Ray Executing| -|
|L00013| Command successfully completed| -|
|L00014| Command manually terminated| -|
|L00015| Obstacle detected error| -|
|L00016| Motor busy error| -|
|L00017| Command initialization error| -|
|L00018| Unexpected internal status error| -|
|L00019| Limit switches activation error| -|
|L00020| Brake device activation error| -|
|L00021| Timeout activation error| -|
|L00022| Driver internal error| -|
|L00023| Driver access register error| -|
|L00024| Missing zero setting error| -|
|L00025| Motor activation disabled error| -|
|L00026| Motor activation aborted| -|
|L00027| Compression Activated| -|
|L00028| Arm Motor Activated| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|L00029| Body Motor Activated| -|
|L00030| Vertical Motor Activated| -|
|L00031| Slide Motor Activated| -|
|L00032| Tilt Motor Activated| -|
|L00033| Xray Tube Calibration Panel| -|
 
 ## Translation  esp table:
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00000| -| -|
|E00001| -| -|
|E00002| -| -|
|E00003| -| -|
|E00004| -| -|
|E00005| -| -|
|E00006| -| -|
|E00007| -| -|
|E00008| -| -|
|E00009| -| -|
|E00010| -| -|
|E00011| -| -|
|E00012| -| -|
|E00013| -| -|
|E00014| -| -|
|E00015| -| -|
|E00016| -| -|
|E00017| -| -|
|E00018| -| -|
|E00019| -| -|
|E00020| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00021| -| -|
|E00022| -| -|
|E00023| -| -|
|E00024| -| -|
|E00025| -| -|
|E00026| -| -|
|E00027| -| -|
|E00028| -| -|
|E00029| -| -|
|E00030| -| -|
|E00031| -| -|
|E00032| -| -|
|E00033| -| -|
|E00034| -| -|
|E00035| -| -|
|E00036| -| -|
|E00037| -| -|
|E00038| -| -|
|E00039| -| -|
|E00040| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00041| -| -|
|E00042| -| -|
|E00043| -| -|
|E00900| -| -|
|E00901| -| -|
|E00902| -| -|
|E00903| -| -|
|E00904| -| -|
|W00001| -| -|
|W00002| -| -|
|W00003| -| -|
|W00004| -| -|
|W00005| -| -|
|W00006| -| -|
|W00007| -| -|
|W00008| -| -|
|W00009| -| -|
|W00010| -| -|
|W00011| -| -|
|W00012| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|W00013| -| -|
|W00014| -| -|
|W00015| -| -|
|W00016| -| -|
|W00017| -| -|
|W00018| -| -|
|W00019| -| -|
|W00020| -| -|
|W00021| -| -|
|W00022| -| -|
|I00001| -| -|
|I00002| -| -|
|I00003| -| -|
|I00004| -| -|
|I00005| -| -|
|I00006| -| -|
|I00007| -| -|
|I00008| -| -|
|I00009| -| -|
|I00010| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|I00011| -| -|
|I00012| -| -|
|I00013| -| -|
|I00014| -| -|
|I00015| -| -|
|I00016| -| -|
|I00017| -| -|
|I00018| -| -|
|I00019| -| -|
|I00020| -| -|
|I00021| -| -|
|I00022| -| -|
|I00023| -| -|
|I00024| -| -|
|I00025| -| -|
|L00001| -| -|
|L00002| -| -|
|L00003| -| -|
|L00004| -| -|
|L00005| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|L00006| -| -|
|L00010| -| -|
|L00011| -| -|
|L00012| -| -|
|L00013| -| -|
|L00014| -| -|
|L00015| -| -|
|L00016| -| -|
|L00017| -| -|
|L00018| -| -|
|L00019| -| -|
|L00020| -| -|
|L00021| -| -|
|L00022| -| -|
|L00023| -| -|
|L00024| -| -|
|L00025| -| -|
|L00026| -| -|
|L00027| -| -|
|L00028| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|L00029| -| -|
|L00030| -| -|
|L00031| -| -|
|L00032| -| -|
|L00033| -| -|
 
 ## Translation  fra table:
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00000| -| -|
|E00001| -| -|
|E00002| -| -|
|E00003| -| -|
|E00004| -| -|
|E00005| -| -|
|E00006| -| -|
|E00007| -| -|
|E00008| -| -|
|E00009| -| -|
|E00010| -| -|
|E00011| -| -|
|E00012| -| -|
|E00013| -| -|
|E00014| -| -|
|E00015| -| -|
|E00016| -| -|
|E00017| -| -|
|E00018| -| -|
|E00019| -| -|
|E00020| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00021| -| -|
|E00022| -| -|
|E00023| -| -|
|E00024| -| -|
|E00025| -| -|
|E00026| -| -|
|E00027| -| -|
|E00028| -| -|
|E00029| -| -|
|E00030| -| -|
|E00031| -| -|
|E00032| -| -|
|E00033| -| -|
|E00034| -| -|
|E00035| -| -|
|E00036| -| -|
|E00037| -| -|
|E00038| -| -|
|E00039| -| -|
|E00040| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00041| -| -|
|E00042| -| -|
|E00043| -| -|
|E00900| -| -|
|E00901| -| -|
|E00902| -| -|
|E00903| -| -|
|E00904| -| -|
|W00001| -| -|
|W00002| -| -|
|W00003| -| -|
|W00004| -| -|
|W00005| -| -|
|W00006| -| -|
|W00007| -| -|
|W00008| -| -|
|W00009| -| -|
|W00010| -| -|
|W00011| -| -|
|W00012| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|W00013| -| -|
|W00014| -| -|
|W00015| -| -|
|W00016| -| -|
|W00017| -| -|
|W00018| -| -|
|W00019| -| -|
|W00020| -| -|
|W00021| -| -|
|W00022| -| -|
|I00001| -| -|
|I00002| -| -|
|I00003| -| -|
|I00004| -| -|
|I00005| -| -|
|I00006| -| -|
|I00007| -| -|
|I00008| -| -|
|I00009| -| -|
|I00010| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|I00011| -| -|
|I00012| -| -|
|I00013| -| -|
|I00014| -| -|
|I00015| -| -|
|I00016| -| -|
|I00017| -| -|
|I00018| -| -|
|I00019| -| -|
|I00020| -| -|
|I00021| -| -|
|I00022| -| -|
|I00023| -| -|
|I00024| -| -|
|I00025| -| -|
|L00001| -| -|
|L00002| -| -|
|L00003| -| -|
|L00004| -| -|
|L00005| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|L00006| -| -|
|L00010| -| -|
|L00011| -| -|
|L00012| -| -|
|L00013| -| -|
|L00014| -| -|
|L00015| -| -|
|L00016| -| -|
|L00017| -| -|
|L00018| -| -|
|L00019| -| -|
|L00020| -| -|
|L00021| -| -|
|L00022| -| -|
|L00023| -| -|
|L00024| -| -|
|L00025| -| -|
|L00026| -| -|
|L00027| -| -|
|L00028| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|L00029| -| -|
|L00030| -| -|
|L00031| -| -|
|L00032| -| -|
|L00033| -| -|
 
 ## Translation  ita table:
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00000| -| -|
|E00001| -| -|
|E00002| -| -|
|E00003| -| -|
|E00004| -| -|
|E00005| -| -|
|E00006| -| -|
|E00007| -| -|
|E00008| -| -|
|E00009| -| -|
|E00010| -| -|
|E00011| -| -|
|E00012| -| -|
|E00013| -| -|
|E00014| -| -|
|E00015| -| -|
|E00016| -| -|
|E00017| -| -|
|E00018| -| -|
|E00019| -| -|
|E00020| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00021| -| -|
|E00022| -| -|
|E00023| -| -|
|E00024| -| -|
|E00025| -| -|
|E00026| -| -|
|E00027| -| -|
|E00028| -| -|
|E00029| -| -|
|E00030| -| -|
|E00031| -| -|
|E00032| -| -|
|E00033| -| -|
|E00034| -| -|
|E00035| -| -|
|E00036| -| -|
|E00037| -| -|
|E00038| -| -|
|E00039| -| -|
|E00040| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00041| -| -|
|E00042| -| -|
|E00043| -| -|
|E00900| -| -|
|E00901| -| -|
|E00902| -| -|
|E00903| -| -|
|E00904| -| -|
|W00001| -| -|
|W00002| -| -|
|W00003| -| -|
|W00004| -| -|
|W00005| -| -|
|W00006| -| -|
|W00007| -| -|
|W00008| -| -|
|W00009| -| -|
|W00010| -| -|
|W00011| -| -|
|W00012| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|W00013| -| -|
|W00014| -| -|
|W00015| -| -|
|W00016| -| -|
|W00017| -| -|
|W00018| -| -|
|W00019| -| -|
|W00020| -| -|
|W00021| -| -|
|W00022| -| -|
|I00001| -| -|
|I00002| -| -|
|I00003| -| -|
|I00004| -| -|
|I00005| -| -|
|I00006| -| -|
|I00007| -| -|
|I00008| -| -|
|I00009| -| -|
|I00010| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|I00011| -| -|
|I00012| -| -|
|I00013| -| -|
|I00014| -| -|
|I00015| -| -|
|I00016| -| -|
|I00017| -| -|
|I00018| -| -|
|I00019| -| -|
|I00020| -| -|
|I00021| -| -|
|I00022| -| -|
|I00023| -| -|
|I00024| -| -|
|I00025| -| -|
|L00001| -| -|
|L00002| -| -|
|L00003| -| -|
|L00004| -| -|
|L00005| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|L00006| -| -|
|L00010| -| -|
|L00011| -| -|
|L00012| -| -|
|L00013| -| -|
|L00014| -| -|
|L00015| -| -|
|L00016| -| -|
|L00017| -| -|
|L00018| -| -|
|L00019| -| -|
|L00020| -| -|
|L00021| -| -|
|L00022| -| -|
|L00023| -| -|
|L00024| -| -|
|L00025| -| -|
|L00026| -| -|
|L00027| -| -|
|L00028| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|L00029| -| -|
|L00030| -| -|
|L00031| -| -|
|L00032| -| -|
|L00033| -| -|
 
 ## Translation  por table:
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00000| -| -|
|E00001| -| -|
|E00002| -| -|
|E00003| -| -|
|E00004| -| -|
|E00005| -| -|
|E00006| -| -|
|E00007| -| -|
|E00008| -| -|
|E00009| -| -|
|E00010| -| -|
|E00011| -| -|
|E00012| -| -|
|E00013| -| -|
|E00014| -| -|
|E00015| -| -|
|E00016| -| -|
|E00017| -| -|
|E00018| -| -|
|E00019| -| -|
|E00020| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00021| -| -|
|E00022| -| -|
|E00023| -| -|
|E00024| -| -|
|E00025| -| -|
|E00026| -| -|
|E00027| -| -|
|E00028| -| -|
|E00029| -| -|
|E00030| -| -|
|E00031| -| -|
|E00032| -| -|
|E00033| -| -|
|E00034| -| -|
|E00035| -| -|
|E00036| -| -|
|E00037| -| -|
|E00038| -| -|
|E00039| -| -|
|E00040| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00041| -| -|
|E00042| -| -|
|E00043| -| -|
|E00900| -| -|
|E00901| -| -|
|E00902| -| -|
|E00903| -| -|
|E00904| -| -|
|W00001| -| -|
|W00002| -| -|
|W00003| -| -|
|W00004| -| -|
|W00005| -| -|
|W00006| -| -|
|W00007| -| -|
|W00008| -| -|
|W00009| -| -|
|W00010| -| -|
|W00011| -| -|
|W00012| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|W00013| -| -|
|W00014| -| -|
|W00015| -| -|
|W00016| -| -|
|W00017| -| -|
|W00018| -| -|
|W00019| -| -|
|W00020| -| -|
|W00021| -| -|
|W00022| -| -|
|I00001| -| -|
|I00002| -| -|
|I00003| -| -|
|I00004| -| -|
|I00005| -| -|
|I00006| -| -|
|I00007| -| -|
|I00008| -| -|
|I00009| -| -|
|I00010| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|I00011| -| -|
|I00012| -| -|
|I00013| -| -|
|I00014| -| -|
|I00015| -| -|
|I00016| -| -|
|I00017| -| -|
|I00018| -| -|
|I00019| -| -|
|I00020| -| -|
|I00021| -| -|
|I00022| -| -|
|I00023| -| -|
|I00024| -| -|
|I00025| -| -|
|L00001| -| -|
|L00002| -| -|
|L00003| -| -|
|L00004| -| -|
|L00005| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|L00006| -| -|
|L00010| -| -|
|L00011| -| -|
|L00012| -| -|
|L00013| -| -|
|L00014| -| -|
|L00015| -| -|
|L00016| -| -|
|L00017| -| -|
|L00018| -| -|
|L00019| -| -|
|L00020| -| -|
|L00021| -| -|
|L00022| -| -|
|L00023| -| -|
|L00024| -| -|
|L00025| -| -|
|L00026| -| -|
|L00027| -| -|
|L00028| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|L00029| -| -|
|L00030| -| -|
|L00031| -| -|
|L00032| -| -|
|L00033| -| -|
 
 ## Translation  rus table:
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00000| -| -|
|E00001| -| -|
|E00002| -| -|
|E00003| -| -|
|E00004| -| -|
|E00005| -| -|
|E00006| -| -|
|E00007| -| -|
|E00008| -| -|
|E00009| -| -|
|E00010| -| -|
|E00011| -| -|
|E00012| -| -|
|E00013| -| -|
|E00014| -| -|
|E00015| -| -|
|E00016| -| -|
|E00017| -| -|
|E00018| -| -|
|E00019| -| -|
|E00020| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00021| -| -|
|E00022| -| -|
|E00023| -| -|
|E00024| -| -|
|E00025| -| -|
|E00026| -| -|
|E00027| -| -|
|E00028| -| -|
|E00029| -| -|
|E00030| -| -|
|E00031| -| -|
|E00032| -| -|
|E00033| -| -|
|E00034| -| -|
|E00035| -| -|
|E00036| -| -|
|E00037| -| -|
|E00038| -| -|
|E00039| -| -|
|E00040| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|E00041| -| -|
|E00042| -| -|
|E00043| -| -|
|E00900| -| -|
|E00901| -| -|
|E00902| -| -|
|E00903| -| -|
|E00904| -| -|
|W00001| -| -|
|W00002| -| -|
|W00003| -| -|
|W00004| -| -|
|W00005| -| -|
|W00006| -| -|
|W00007| -| -|
|W00008| -| -|
|W00009| -| -|
|W00010| -| -|
|W00011| -| -|
|W00012| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|W00013| -| -|
|W00014| -| -|
|W00015| -| -|
|W00016| -| -|
|W00017| -| -|
|W00018| -| -|
|W00019| -| -|
|W00020| -| -|
|W00021| -| -|
|W00022| -| -|
|I00001| -| -|
|I00002| -| -|
|I00003| -| -|
|I00004| -| -|
|I00005| -| -|
|I00006| -| -|
|I00007| -| -|
|I00008| -| -|
|I00009| -| -|
|I00010| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|I00011| -| -|
|I00012| -| -|
|I00013| -| -|
|I00014| -| -|
|I00015| -| -|
|I00016| -| -|
|I00017| -| -|
|I00018| -| -|
|I00019| -| -|
|I00020| -| -|
|I00021| -| -|
|I00022| -| -|
|I00023| -| -|
|I00024| -| -|
|I00025| -| -|
|L00001| -| -|
|L00002| -| -|
|L00003| -| -|
|L00004| -| -|
|L00005| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|L00006| -| -|
|L00010| -| -|
|L00011| -| -|
|L00012| -| -|
|L00013| -| -|
|L00014| -| -|
|L00015| -| -|
|L00016| -| -|
|L00017| -| -|
|L00018| -| -|
|L00019| -| -|
|L00020| -| -|
|L00021| -| -|
|L00022| -| -|
|L00023| -| -|
|L00024| -| -|
|L00025| -| -|
|L00026| -| -|
|L00027| -| -|
|L00028| -| -|
 
<div style="page-break-after: always;"></div>
 
 | MESSAGE CODE | TITLE | CONTENT |
 |:--|:--|:--|
|L00029| -| -|
|L00030| -| -|
|L00031| -| -|
|L00032| -| -|
|L00033| -| -|
 */
