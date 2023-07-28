#include "pch.h"
#include "CanOpenMotor.h"
#include "CanDriver.h"
#include "pd4_od.h"
#include <thread>

using namespace CANOPEN;

// Those defines SHALL be defined here!!!
#define MOTOR_GEAR 452.8


void CanOpenMotor::createObjectDictionary(void) {
   DECLARE_INIT_REGISTER(OD_4013_01,1); // Hardware configuration : 1 = EXTERNAL VCC LOGIC ON
    
   // NMT Behavior in case of fault
   DECLARE_INIT_REGISTER(OD_1029_01, 0);
   DECLARE_INIT_REGISTER(OD_1029_02,1 );
   DECLARE_INIT_REGISTER(OD_2031_00,5000 ); // Peak current
   DECLARE_INIT_REGISTER(OD_2032_00, 5000); // Maximum Speed
   DECLARE_INIT_REGISTER(OD_2033_00,0 );	// Plunger Block
   DECLARE_INIT_REGISTER(OD_2034_00,51500 );// Upper Voltage Warning Level
   DECLARE_INIT_REGISTER(OD_2035_00,21000 );// ****************************************Lower Voltage Warning Level
   DECLARE_INIT_REGISTER(OD_2036_00,2000 ); 	// Open Loop Current Reduction Idle Time
   DECLARE_INIT_REGISTER(OD_2037_00,(-50) );	// Open Loop Current Reduction Value/factor

   // I2t Parameters
   DECLARE_INIT_REGISTER(OD_203B_01,5000 );	// Nominal Current
   DECLARE_INIT_REGISTER(OD_203B_02,1000 );	// Maximum Duration Of Peak Current
   DECLARE_INIT_REGISTER(OD_203B_03,0 );	// Threshold
   DECLARE_INIT_REGISTER(OD_203B_04,0 );	// CalcValue
   DECLARE_INIT_REGISTER(OD_203B_05,5000 ); 	// LimitedCurrent
   DECLARE_INIT_REGISTER(OD_2056_00,500 );	// Limit Switch Tolerance Band

   // user units
   DECLARE_INIT_REGISTER(OD_2061_00,1 );	// Velocity Numerator
   DECLARE_INIT_REGISTER(OD_2062_00,SPEED_DENOMINATOR );  // Velocity Denominator        ***********************
   DECLARE_INIT_REGISTER(OD_2063_00,1 );	// Acceleration Numerator
   DECLARE_INIT_REGISTER(OD_2064_00,SPEED_DENOMINATOR ); 	// Acceleration Denominator    ***********************
   DECLARE_INIT_REGISTER(OD_2065_00,1 );	// Jerk Numerator
   DECLARE_INIT_REGISTER(OD_2066_00,60 );	// Jerk Denominator
   DECLARE_INIT_REGISTER(OD_3202_00,9 ); 	// Motor Drive Submode Select: 6:BLDC 3:CurRed 2:Brake 1:VoS 0:CLOOP/OLOOP

   // Motor Drive Sensor Display Closed Loop
   DECLARE_INIT_REGISTER(OD_320B_01,0 );  // Commutation
   DECLARE_INIT_REGISTER(OD_320B_02,0 );	// Torque
   DECLARE_INIT_REGISTER(OD_320B_03,1 );	// Velocity
   DECLARE_INIT_REGISTER(OD_320B_04,1 ); 	// Position

   // Motor Drive Parameter Set
   DECLARE_INIT_REGISTER(OD_3210_01,50000 ); // Position Loop, Proportional Gain (closed Loop)
   DECLARE_INIT_REGISTER(OD_3210_02,10 );	 // Position Loop, Integral Gain (closed Loop)

   // Analogue Inputs Control
   DECLARE_INIT_REGISTER(OD_3221_00,0 );     // 0 , Voltage, 1, Current

   // Digital Input Capture
   DECLARE_INIT_REGISTER(OD_3241_01,0 );    // Control (0:off, 1:RE, 2:FE, 3:RE+FE)

   // Following Error Option Code
   DECLARE_INIT_REGISTER(OD_3700_00,(-1) ); // -1 No reaction;0 Immediate stop; 1 Braking with "slow down ramp"; 2 Braking with "quick stop ramp"
        

    // Quick Stop Option Code
   DECLARE_INIT_REGISTER(OD_605A_00,2 );   // 0 Immediate stop  // 1 Braking with "slow down ramp"  // 2 Braking with "quick stop ramp"

    // Shutdown Option Code
   DECLARE_INIT_REGISTER(OD_605B_00,0 );   // 0 Immediate stop  // 1 Braking with "slow down ramp"

    // Disable Option Code
   DECLARE_INIT_REGISTER(OD_605C_00,0 );   // 0 Immediate stop  // 1 Braking with "slow down ramp"

    // Halt Option Code
   DECLARE_INIT_REGISTER(OD_605D_00,0 );  // 0 Immediate stop  // 1 Braking with "slow down ramp"  // 2 Braking with "quick stop ramp"

    // Fault Option Code
   DECLARE_INIT_REGISTER(OD_605E_00,0 );  // 0 Immediate stop  // 1 Braking with "slow down ramp"   // 2 Braking with "quick stop ramp"

    // Following Error Window and time
   DECLARE_INIT_REGISTER(OD_6065_00,256 ); 	// Window
   DECLARE_INIT_REGISTER(OD_6066_00,100 );	// Time (ms)

   // Position Window + time
   DECLARE_INIT_REGISTER(OD_6067_00,10 );	// Window
   DECLARE_INIT_REGISTER(OD_6068_00,100 );	// Time


   // Position Range Limit
   DECLARE_INIT_REGISTER(OD_607B_01,0 ); 	// Min Position Range Limit
   DECLARE_INIT_REGISTER(OD_607B_02,0 );	// Max Position Range Limit

   // Software Position Limit
   DECLARE_INIT_REGISTER(OD_607D_01, convert_cGRAD_TO_EncoderUnit(-2700));	// Min Position Limit
   DECLARE_INIT_REGISTER(OD_607D_02, convert_cGRAD_TO_EncoderUnit(2700) );	// Max Position Limit

   // Polarity
   DECLARE_INIT_REGISTER(OD_607E_00,0 );	// b7:1-> inverse rotaion


   // Position Encoder Resolution: EncInc/MotRev
   DECLARE_INIT_REGISTER(OD_608F_01,2000 );	// Encoder Increments
   DECLARE_INIT_REGISTER(OD_608F_02,1 ); 	// Motor Revolutions
   DECLARE_INIT_REGISTER(OD_60F2_00,0x0002 ); // Absolute positionning

   // Gear Ratio
   DECLARE_INIT_REGISTER(OD_6091_01,1 ); 	// Motor Revolutions
   DECLARE_INIT_REGISTER(OD_6091_02,1 );	// Shaft Revolutions

   // Max Absolute Acceleration and Deceleration
   DECLARE_INIT_REGISTER(OD_60C5_00,5000 );// Max Acceleration
   DECLARE_INIT_REGISTER(OD_60C6_00,5000 );// Max Deceleration

   // Homing registers
   DECLARE_INIT_REGISTER(OD_6098_00,21 );  // Homing method 21
   DECLARE_INIT_REGISTER(OD_607C_00, 0);
}

void CanOpenMotor::createWorkingDictionary(void) {
    DECLARE_NOINIT_REGISTER(OD_6041_00); // CiA 402 status word
    DECLARE_NOINIT_REGISTER(OD_6040_00); // CiA Control Word & Status change flags
    DECLARE_NOINIT_REGISTER(OD_6060_00); // OPERATING MODE CONROL WORD
    DECLARE_NOINIT_REGISTER(OD_6064_00); // Actual position encoder value (user units)
    DECLARE_NOINIT_REGISTER(OD_1001_00); // ERROR REGISTER
    DECLARE_NOINIT_REGISTER(OD_1003_01); // SPECIFIC ERROR REGISTER
    DECLARE_NOINIT_REGISTER(OD_1010_01); // Save All Parameters To Non-volatile Memory
    DECLARE_NOINIT_REGISTER(OD_2700_01); // User data control word: SET to 1 to save User content. reset autom. to 0 when finished
    DECLARE_NOINIT_REGISTER(OD_2700_02); // USER #0 - RESERVED FOR THE CONFIG VECTOR CONFIGURATION CODE
    DECLARE_NOINIT_REGISTER(OD_2700_03); // USER #1 - RESERVED FOR THE NANOJ CONFIGURATION CODE
    DECLARE_NOINIT_REGISTER(OD_2700_04); // USER #2 - RESERVED FOR RESET SENSING
    
}

int CanOpenMotor::findOd(unsigned short index, unsigned short sub) {
    if (object_dictionary->Count == 0) return -1;

    for (int i = 0; i < object_dictionary->Count; i++) {
        if ((object_dictionary[i]->index == index) && (object_dictionary[i]->subindex == sub)) return i;
    }
    return -1;

}

int CanOpenMotor::convert_cGRADsec_TO_ROT_min(int x) {
    return (int)((double) x * gear_ratio * (double) SPEED_DENOMINATOR / (double) 36000.);
}

int CanOpenMotor::convert_cGRAD_TO_EncoderUnit(int x) {
    return (int)((double) x * gear_ratio * (double) 10. / (double) 180.);
}

int CanOpenMotor::convert_EncoderUnit_TO_cGRAD(int x) {
    return (int)((double) x * (double) 180.0 / (gear_ratio * (double) 10.));
}

CanOpenMotor::CanOpenMotor(unsigned char devid, double gear) {

    // Gets the handler of the class instance to be used for the Post/Send message functions.
    hwnd = static_cast<HWND>(Handle.ToPointer());    
    device_id = devid;
    gear_ratio = gear;

    // Creates the Object dictionaries
    object_dictionary = gcnew List<Register^> {};
    createWorkingDictionary();
    createObjectDictionary();
 
    rxEvent = CreateEvent(
        NULL,               // default security attributes
        TRUE,               // manual-reset event
        FALSE,              // initial state is nonsignaled
        TEXT("RxEvent")  // object name
    );
    rxRegister = gcnew Register();

    // Start the reception thread
    startup_thread = gcnew Thread(gcnew ThreadStart(this, &CanOpenMotor::startupThread));
    startup_thread->Name = "Motor Startup";
    startup_thread->IsBackground = true; // Important!!! This is necessary to allow the thread to exit when the program exits !!!
    startup_thread->Start();

    CanDriver::canrx_canopen_sdo_event += gcnew CanDriver::delegate_can_rx_frame(this, &CanOpenMotor::thread_canopen_rx_sdo_callback);
}


void CanOpenMotor::thread_canopen_rx_sdo_callback(unsigned short canid, unsigned char* data, unsigned char len) {
    // Be careful!! This call is runing ino the program thread pull, out of the class thread.
    
    if (canid != device_id + 0x580) return; // Belongs to other devices
    if (len < 8) return; // Invalid message length

    rxRegister->update(data, len);
    if (rxRegister->index != wait_index) return;
    if (rxRegister->subindex != wait_subindex) return;
    rxRegister->valid = true;

    SetEvent(rxEvent);
    wait_index = 0;
    wait_subindex = 0;

}

bool CanOpenMotor::blocking_writeOD(unsigned short index, unsigned char sub, unsigned char dim, int val) {
    unsigned char buffer[8];
    DWORD dwWaitResult;

    buffer[0] = Register::WRCMD | dim;
    buffer[1] = (unsigned char) index & 0xFF;
    buffer[2] = (unsigned char)( index >> 8) & 0xFF;
    buffer[3] = sub;

    unsigned int dval = (unsigned int) val;
    buffer[4] = (unsigned char)(dval & 0xFF); dval = dval >> 8;
    buffer[5] = (unsigned char)(dval & 0xFF); dval = dval >> 8;
    buffer[6] = (unsigned char)(dval & 0xFF); dval = dval >> 8;
    buffer[7] = (unsigned char)(dval & 0xFF); 

    wait_index = index;
    wait_subindex = sub;  
    rxRegister->valid = false;
    CanDriver::multithread_send(0x600 + device_id, buffer, 8);
    dwWaitResult = WaitForSingleObject( rxEvent, 100);  
    if (dwWaitResult == WAIT_OBJECT_0) return true;
    return false;
   
}

bool CanOpenMotor::blocking_readOD(unsigned short index, unsigned char sub, unsigned char dim) {
    unsigned char buffer[8];
    DWORD dwWaitResult;

    buffer[0] = Register::RDCMD ;
    buffer[1] = (unsigned char)index & 0xFF;
    buffer[2] = (unsigned char)(index >> 8) & 0xFF;
    buffer[3] = sub;

    buffer[4] = 0;
    buffer[5] = 0;
    buffer[6] = 0;
    buffer[7] = 0;

    wait_index = index;
    wait_subindex = sub;
    rxRegister->valid = false;
    CanDriver::multithread_send(0x600 + device_id, buffer, 8);
    dwWaitResult = WaitForSingleObject(rxEvent, 5000);
    if (dwWaitResult == WAIT_OBJECT_0) return true;
    return false;
}

void CanOpenMotor::startupThread(void) {
    unsigned char status = 0;

    while (1) {
        switch (status) {
        case 0: 
            Debug::WriteLine("Motor device: startup initilize!\n");
            // Read The Status word
            if(blocking_readOD(OD_6041_00)) Debug::WriteLine("Received Status Word!\n");
            else Debug::WriteLine("TMO!\n");
            std::this_thread::sleep_for(std::chrono::microseconds(1000000));
        }
        
        
    }
}



