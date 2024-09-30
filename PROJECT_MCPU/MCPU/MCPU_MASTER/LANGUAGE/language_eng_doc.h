 /** \addtogroup MessageNotifyDescription
 This is the message translation of the language: language_eng
 | ID | MESSAGE | Description |
 |:--|:--|:--|
|E00000| Compilation Error<br> Invalid module condition detected. See the log file|The application risen a non handled exception.<br>This is a possible bug scenario not tested or not investigated yet.<br>See the Gantry.log file for details.|
|E00001| AWS Communication<br> Fatal error in opening the socket. Check the network parameters.|An error has been detected during the connection with the AWS.<br>The problem may be due to a wrong address set into the SystemIni.cnf file <br>for the Command socket and/or Event socket.<br>See the AWS protocol documentation for details.<br>|
|E00002| PCB301 Communication<br> The Board has been reset|The PCB301 is the service power board. <br>The board signalled a reset event (the microcontroller is restarted).<br>The Application can handle this event restoring the current device status properly.<br>However this event may suggest a problem with the cabling (power cable) or a board issue.<br>Frequent events like that cannot be accepted as normal behavior.|
|E00003| PCB302 Communication<br> The Board has been reset|The board signalled a reset event (the microcontroller is restarted).<br>The Application can handle this event restoring the current device status properly.<br>However this event may suggest a problem with the cabling (power cable) or a board issue.<br>Frequent events like that cannot be accepted as normal behavior.<br>|
|E00004| PCB303 Communication<br> The Board has been reset|The board signalled a reset event (the microcontroller is restarted).<br>The Application can handle this event restoring the current device status properly.<br>However this event may suggest a problem with the cabling (power cable) or a board issue.<br>Frequent events like that cannot be accepted as normal behavior.<br>|
|E00005| PCB304 Communication<br> The Board has been reset|The board signalled a reset event (the microcontroller is restarted).<br>The Application can handle this event restoring the current device status properly.<br>However this event may suggest a problem with the cabling (power cable) or a board issue.<br>Frequent events like that cannot be accepted as normal behavior.<br>|
|E00006| PCB325 Communication<br> The Board has been reset|-|
|E00007| PCB326 Communication<br> The Board has been reset|The board signalled a reset event (the microcontroller is restarted).<br>The Application can handle this event restoring the current device status properly.<br>However this event may suggest a problem with the cabling (power cable) or a board issue.<br>Frequent events like that cannot be accepted as normal behavior.<br>|
|E00008| PCB301 Communication<br> The board no longer communicate with the MCPU|-|
|E00009| PCB302 Communication<br> The board no longer communicate with the MCPU|-|
|E00010| PCB303 Communication<br> The board no longer communicate with the MCPU|-|

<div style="page-break-after: always;"></div>

 | ID | MESSAGE | Description |
 |:--|:--|:--|
|E00011| PCB304 Communication<br> The board no longer communicate with the MCPU|-|
|E00012| PCB325 Communication<br> The board no longer communicate with the MCPU|-|
|E00013| PCB326 Communication<br> The board no longer communicate with the MCPU|-|
|E00014| Xray push button fault<br> Xray push button locked On|-|
|E00015| Motor Pedals Fault<br> Up/Down pedals locked On|-|
|E00016| Motor Pedals Fault<br> Compressor pedals locked On|-|
|E00017| Motor Buttons Fault<br> C-Arm rotation buttons locked On|-|
|E00018| Motor Buttons Fault<br> Up/Down buttons locked On|-|
|E00019| Software Configuration Fault<br> Wrong software or firmware revision detected|-|
|E00020| System Power Monitoring<br> Power down condition detected!|-|

<div style="page-break-after: always;"></div>

 | ID | MESSAGE | Description |
 |:--|:--|:--|
|E00021| System Power Monitoring<br> Emergency button activation detected!|-|
|E00022| System Safety Monitoring<br> Safety Cabinet activation detected!|-|
|E00023| System Power Monitoring<br> Battery voltage monitoring error! The battery voltage level is too low.|-|
|E00024| Generator Status<br> Generator device not connected|-|
|E00025| Collimator Monitoring<br> The Collimator selector seams to be blocked|-|
|E00026| Filter Monitoring<br> The Filter selector seams to be blocked|-|
|E00027| Tube Temperature Monitoring<br> The Tube Stator sensor connection input seams to be broken|-|
|E00028| Tube Temperature Monitoring<br> The Tube Stator sensor connection input seams to be shorted|-|
|E00029| Tube Temperature Monitoring<br> The Tube Bulb sensor connection input seams to be broken|-|
|E00030| Tube Temperature Monitoring<br> The Tube Bulb sensor connection input seams to be shorted|-|

<div style="page-break-after: always;"></div>

 | ID | MESSAGE | Description |
 |:--|:--|:--|
|E00031| Body Motor Monitoring<br> The brake device is in Fault condition|-|
|E00032| Tilt Motor Monitoring<br> The brake device is in Fault condition|-|
|E00033| Vertical Motor Monitoring<br> The encoder is not initialized|-|
|E00034| ARM Motor Monitoring<br> The encoder is not initialized|-|
|E00035| Slide Motor Monitoring<br> The encoder is not initialized|-|
|E00036| Tilt Motor Monitoring<br> The encoder is not initialized|-|
|E00037| Body Motor Monitoring<br> The encoder is not initialized|-|
|E00038| Body Motor Monitoring<br> The limit switch is active|-|
|E00039| Body Motor Monitoring<br> Unable to read the potentiometer input|-|
|E00040| Vertical Motor Monitoring<br> The limit switch is active|-|

<div style="page-break-after: always;"></div>

 | ID | MESSAGE | Description |
 |:--|:--|:--|
|E00041| Generator Status Monitoring<br> The Generator configuration is failed|-|
|E00042| Generator Messages<br> There are error messages|-|
|E00043| Potter Messages<br> The Grid device is in fault condition|-|
|E00900| X-RAY Sequence Error<br> X-Ray push button early released|-|
|E00901| X-RAY Sequence Error<br> Lower Anodic Current|-|
|E00902| X-RAY Sequence Error<br> Lower kV than expected|-|
|E00903| X-RAY Sequence Error<br> Exposure Timeout|-|
|E00904| X-RAY Sequence Error<br> HS Starter Error|-|
|W00001| Compressor Monitoring<br> Missing compression force.|-|
|W00002| Component Monitoring<br> Undetected patient protection component.|-|

<div style="page-break-after: always;"></div>

 | ID | MESSAGE | Description |
 |:--|:--|:--|
|W00003| Arm Position Monitoring<br> C-ARM angle out of range. Repeat the C-ARM positioning.|-|
|W00004| Compressor Monitoring<br> The position is not calibrated.|-|
|W00005| Compressor Monitoring<br> The force is not calibrated.|-|
|W00006| Compressor Monitoring<br> A valid paddle is not detected.|-|
|W00007| Compressor Monitoring<br> Wrong Paddle model detected.|-|
|W00008| Exposure Parameters Monitoring<br> Missing exposure mode selection.|-|
|W00009| Exposure Parameters Monitoring<br> Missing exposure data selection.|-|
|W00010| X-Ray Button Monitoring<br> The X-Ray button is not enabled.|-|
|W00011| Safety Monitoring<br> The Study's door is detected Open.|-|
|W00012| Generator Status Monitoring<br> The Generator is not ready for exposure.|-|

<div style="page-break-after: always;"></div>

 | ID | MESSAGE | Description |
 |:--|:--|:--|
|W00013| X-Ray Tube Monitoring<br> The X-Ray Tube cumulated energy is too high|-|
|W00014| Collimator Light Command Failed<br> The Collimation Light cannot be activated now. Try again.|-|
|W00015| Tube Temperature Monitoring<br> The Tube Stator temperature is too high|-|
|W00016| Tube Temperature Monitoring<br> The Tube Bulb temperature is too high|-|
|W00017| Tube Temperature Monitoring<br> The Anode temperature is too high|-|
|W00018| Filter Monitoring<br> The Filter selector is not in a valid position|-|
|W00019| Collimator Monitoring<br> An invalid collimation format is detected|-|
|W00020| Generator Monitoring<br> The Generator is in Service Mode|-|
|W00021| Potter Monitoring<br> The grid is not in the expected position|-|
|W00022| Potter Monitoring<br> The grid activation is momentary disabled|-|

<div style="page-break-after: always;"></div>

 | ID | MESSAGE | Description |
 |:--|:--|:--|
|I00001| System Power Monitoring<br> The backup batteries are detected Disabled!|-|
|I00002| Projection Confirmation Action<br> The C-ARM may be activated|-|
|I00003| Abort Projection Request<br> Proceed to Abort the current projection?|-|
|I00004| Motor Slide Activation<br> Proceed with the Slide activation?|-|
|I00005| Motor Body Activation<br> Proceed with the Body activation?|-|
|I00006| Motor Arm Activation<br> Proceed with the Arm activation?|-|
|I00007| Motor Tilt Activation<br> Proceed with the Tilt activation?|-|
|I00008| Motor Slide Activation<br> Invalid activation conditions|-|
|I00009| Motor Body Activation<br> Invalid activation conditions|-|
|I00010| Motor Arm Activation<br> Invalid activation conditions|-|

<div style="page-break-after: always;"></div>

 | ID | MESSAGE | Description |
 |:--|:--|:--|
|I00011| Motor Tilt Activation<br> Invalid activation conditions|-|
|I00012| Motor Power Supply Monitoring<br> The safety line is off|-|
|I00013| Motor Power Supply Monitoring<br> The power supply is off|-|
|I00014| Power Service Monitoring<br> The Burning Jumper is present|-|
|I00015| Power Service Monitoring<br> The power supply lock is active|-|
|I00016| Manual Motor Activation<br> The manual activation is disabled in this contest|-|
|I00017| Manual Motor Activation<br> The manual activation is disabled for safety|-|
|I00018| Manual Motor Activation<br> The manual activation is disabled for pending errors|-|
|I00019| Body Motor Monitoring<br> The motor driver is in error condition|-|
|I00020| Vertical Motor Monitoring<br> The motor driver is in error condition|-|

<div style="page-break-after: always;"></div>

 | ID | MESSAGE | Description |
 |:--|:--|:--|
|I00021| Tilt Motor Monitoring<br> The motor driver is in error condition|-|
|I00022| Arm Motor Monitoring<br> The motor driver is in error condition|-|
|I00023| Slide Motor Monitoring<br> The motor driver is in error condition|-|
|I00024| Power Off Request<br> Proceed with the System Power Off?|-|
|I00025| Potter Monitoring<br> The Grid positioning is set to manual mode (test mode)|-|
|L00001| Error Window Panel<br> -|-|
|L00002| Projection Selection<br> -|-|
|L00003| Zero Setting Panel<br> -|-|
|L00004| Service Panel<br> -|-|
|L00005| Calibration Panel<br> -|-|

<div style="page-break-after: always;"></div>

 | ID | MESSAGE | Description |
 |:--|:--|:--|
|L00006| Rotation Tool Panel<br> -|-|
|L00007| ERR<br> -|-|
|L00008| WRN<br> -|-|
|L00009| INF<br> -|-|
|L00010| Standby<br> -|-|
|L00011| Ready<br> -|-|
|L00012| X-Ray Executing<br> -|-|
|L00013| Command successfully completed<br> -|-|
|L00014| Command manually terminated<br> -|-|
|L00015| Obstacle detected error<br> -|-|

<div style="page-break-after: always;"></div>

 | ID | MESSAGE | Description |
 |:--|:--|:--|
|L00016| Motor busy error<br> -|-|
|L00017| Command initialization error<br> -|-|
|L00018| Unexpected internal status error<br> -|-|
|L00019| Limit switches activation error<br> -|-|
|L00020| Brake device activation error<br> -|-|
|L00021| Timeout activation error<br> -|-|
|L00022| Driver internal error<br> -|-|
|L00023| Driver access register error<br> -|-|
|L00024| Missing zero setting error<br> -|-|
|L00025| Motor activation disabled error<br> -|-|

<div style="page-break-after: always;"></div>

 | ID | MESSAGE | Description |
 |:--|:--|:--|
|L00026| Motor activation aborted<br> -|-|
|L00027| Compression Activated<br> -|-|
|L00028| Arm Motor Activated<br> -|-|
|L00029| Body Motor Activated<br> -|-|
|L00030| Vertical Motor Activated<br> -|-|
|L00031| Slide Motor Activated<br> -|-|
|L00032| Tilt Motor Activated<br> -|-|
|L00033| Xray Tube Calibration Panel<br> -|-|
 */
