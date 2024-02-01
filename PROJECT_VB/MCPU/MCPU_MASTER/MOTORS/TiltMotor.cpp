#include "CalibrationConfig.h"
#include "Notify.h"
#include "TiltMotor.h"
#include "PCB301.h"
#include "pd4_od.h"
#include "..\gantry_global_status.h"
#include <thread>


// The User units are 0.01°
#define GEAR_RATIO ((double) 1 / (double) 120) //!< 1 turn == 1.2°

#define EXPWIN_INPUT_MASK(x) (x & 0x00010000)               //!< INPUT 1 == expwin input signal
#define UNLOCK_BRAKE_INPUT_MASK(x) (x & 0x00020000)         //!< INPUT 2 == 1 is unlock condition
#define ZEROSETTING_INPUT_MASK(x) (x & 0x00040000)          //!< INPUT 3 Zero setting input photocell
#define SAFETY_ACTIVATION_INPUT_MASK(x) (x & 0x00080000)    //!< INPUT 4 == 1 is unlock condition

#define LOCK_BRAKE_OUT_MASK     0x00010000                  //!< OUTPUT 1 = 1
#define UNLOCK_BRAKE_OUT_MASK   0x00000000                  //!< OUTPUT 1 = 0

#define MAX_ROTATION_ANGLE 2700
#define MIN_ROTATION_ANGLE -2700
#define HOMING_ON_METHOD 19
#define HOMING_OFF_METHOD 20


/*      Versione con attesa iniziale del segnale EXP WIN
 *      ad un livello IDLE e inizializzazione a zero degli
 *      angoli di scansione
 *
 */
static const unsigned char nanojTrxProgram[] = {
    0x4e, 0x56, 0x4d, 0x4d, 0x0, 0x2, 0x0, 0x0, 0x0,
    0x2, 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x2, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0x50,
    0x3, 0x0, 0x0, 0x20, 0x0, 0xfd, 0x60, 0x20, 0x0, 0x64,
    0x60, 0x10, 0x0, 0x40, 0x60, 0x20, 0x2, 0x40, 0x32, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x10, 0xb1, 0xbd, 0xe8, 0xff, 0x9f, 0xff,
    0xdf, 0x0, 0xf0, 0x2, 0xf9, 0xff, 0xdf, 0x0, 0xbf, 0x2d,
    0xe9, 0xff, 0x5f, 0xfe, 0xdf, 0x70, 0x47, 0x3, 0x46, 0x18,
    0x46, 0xe1, 0xdf, 0x70, 0x47, 0x3, 0x46, 0xa, 0x46, 0x18,
    0x46, 0x11, 0x46, 0x1, 0xdf, 0x3, 0x46, 0x18, 0x46, 0x70,
    0x47, 0x3, 0x46, 0xa, 0x46, 0x18, 0x46, 0x11, 0x46, 0x2,
    0xdf, 0x3, 0x46, 0x18, 0x46, 0x70, 0x47, 0x3, 0x46, 0xa,
    0x46, 0x18, 0x46, 0x11, 0x46, 0x3, 0xdf, 0x3, 0x46, 0x18,
    0x46, 0x70, 0x47, 0x3, 0x46, 0xa, 0x46, 0x18, 0x46, 0x11,
    0x46, 0x4, 0xdf, 0x3, 0x46, 0x18, 0x46, 0x70, 0x47, 0x3,
    0x46, 0x18, 0x46, 0x9, 0xdf, 0x3, 0x46, 0x18, 0x46, 0x70,
    0x47, 0x3, 0x46, 0x18, 0x46, 0xa, 0xdf, 0x3, 0x46, 0x18,
    0x46, 0x70, 0x47, 0x3, 0x46, 0x18, 0x46, 0x5, 0xdf, 0x3,
    0x46, 0x18, 0x46, 0x70, 0x47, 0x3, 0x46, 0x18, 0x46, 0x6,
    0xdf, 0x3, 0x46, 0x18, 0x46, 0x70, 0x47, 0x3, 0x46, 0x18,
    0x46, 0x7, 0xdf, 0x3, 0x46, 0x18, 0x46, 0x70, 0x47, 0x3,
    0x46, 0x18, 0x46, 0x8, 0xdf, 0x3, 0x46, 0x18, 0x46, 0x70,
    0x47, 0x3, 0x46, 0x18, 0x46, 0xb, 0xdf, 0x3, 0x46, 0x18,
    0x46, 0x70, 0x47, 0x3, 0x46, 0x18, 0x46, 0xc, 0xdf, 0x3,
    0x46, 0x18, 0x46, 0x70, 0x47, 0x3, 0x46, 0x18, 0x46, 0xd,
    0xdf, 0x3, 0x46, 0x18, 0x46, 0x70, 0x47, 0x30, 0xb4, 0x3,
    0x46, 0xc, 0x46, 0x15, 0x46, 0x18, 0x46, 0x21, 0x46, 0x2a,
    0x46, 0x21, 0xdf, 0x30, 0xbc, 0x70, 0x47, 0x3, 0x46, 0xa,
    0x46, 0x18, 0x46, 0x11, 0x46, 0x20, 0xdf, 0x3, 0x46, 0x18,
    0x46, 0x70, 0x47, 0x30, 0xb4, 0xb, 0x46, 0x4, 0x46, 0x15,
    0x46, 0x20, 0x46, 0x19, 0x46, 0x2a, 0x46, 0x30, 0xdf, 0x3,
    0x46, 0x18, 0x46, 0x30, 0xbc, 0x70, 0x47, 0x3, 0x46, 0xa,
    0x46, 0x18, 0x46, 0x11, 0x46, 0x31, 0xdf, 0x70, 0x47, 0x3,
    0x46, 0xa, 0x46, 0x18, 0x46, 0x11, 0x46, 0x32, 0xdf, 0x3,
    0x46, 0x18, 0x46, 0x70, 0x47, 0x2, 0x46, 0x10, 0x46, 0xd0,
    0xdf, 0x3, 0x46, 0x59, 0x7, 0x7, 0xd5, 0x0, 0x21, 0x8,
    0x46, 0xe1, 0xdf, 0x10, 0x46, 0xd0, 0xdf, 0x3, 0x46, 0x58,
    0x7, 0xf8, 0xd4, 0xd3, 0xf1, 0x1, 0x0, 0x38, 0xbf, 0x0,
    0x20, 0x70, 0x47, 0x0, 0xbf, 0x2, 0x46, 0x10, 0x46, 0xd1,
    0xdf, 0x3, 0x46, 0x59, 0x7, 0x7, 0xd5, 0x0, 0x21, 0x8,
    0x46, 0xe1, 0xdf, 0x10, 0x46, 0xd1, 0xdf, 0x3, 0x46, 0x58,
    0x7, 0xf8, 0xd4, 0xd3, 0xf1, 0x1, 0x0, 0x38, 0xbf, 0x0,
    0x20, 0x70, 0x47, 0x0, 0xbf, 0x2, 0x46, 0x10, 0x46, 0xd2,
    0xdf, 0x3, 0x46, 0x59, 0x7, 0x7, 0xd5, 0x0, 0x21, 0x8,
    0x46, 0xe1, 0xdf, 0x10, 0x46, 0xd2, 0xdf, 0x3, 0x46, 0x58,
    0x7, 0xf8, 0xd4, 0xd3, 0xf1, 0x1, 0x0, 0x38, 0xbf, 0x0,
    0x20, 0x70, 0x47, 0x0, 0xbf, 0x2, 0x46, 0x10, 0x46, 0xd3,
    0xdf, 0x3, 0x46, 0x59, 0x7, 0x7, 0xd5, 0x0, 0x21, 0x8,
    0x46, 0xe1, 0xdf, 0x10, 0x46, 0xd3, 0xdf, 0x3, 0x46, 0x58,
    0x7, 0xf8, 0xd4, 0x18, 0x1c, 0x18, 0xbf, 0x1, 0x20, 0x70,
    0x47, 0x2, 0x46, 0x10, 0x46, 0xd4, 0xdf, 0x3, 0x46, 0x59,
    0x7, 0x7, 0xd5, 0x0, 0x21, 0x8, 0x46, 0xe1, 0xdf, 0x10,
    0x46, 0xd4, 0xdf, 0x3, 0x46, 0x58, 0x7, 0xf8, 0xd4, 0xd3,
    0xf1, 0x1, 0x0, 0x38, 0xbf, 0x0, 0x20, 0x70, 0x47, 0x0,
    0xbf, 0x2, 0x46, 0x10, 0x46, 0xd5, 0xdf, 0x3, 0x46, 0x59,
    0x7, 0x7, 0xd5, 0x0, 0x21, 0x8, 0x46, 0xe1, 0xdf, 0x10,
    0x46, 0xd5, 0xdf, 0x3, 0x46, 0x58, 0x7, 0xf8, 0xd4, 0xd3,
    0xf1, 0x1, 0x0, 0x38, 0xbf, 0x0, 0x20, 0x70, 0x47, 0x0,
    0xbf, 0x80, 0xb5, 0x84, 0xb0, 0x0, 0xaf, 0x0, 0x23, 0xfb,
    0x73, 0x1, 0x23, 0xfb, 0x73, 0xb, 0xe0, 0xfb, 0x7b, 0x4f,
    0xf4, 0x14, 0x50, 0x19, 0x46, 0x0, 0x22, 0xff, 0xf7, 0x4f,
    0xff, 0xff, 0xf7, 0xef, 0xfe, 0xfb, 0x7b, 0x1, 0x33, 0xfb,
    0x73, 0xfb, 0x7b, 0x20, 0x2b, 0xf0, 0xd9, 0x40, 0xf6, 0x80,
    0x63, 0xc2, 0xf2, 0x0, 0x3, 0xd3, 0xf8, 0x2, 0x30, 0xbb,
    0x60, 0xbb, 0x68, 0x43, 0xf0, 0x1, 0x2, 0x40, 0xf6, 0x80,
    0x63, 0xc2, 0xf2, 0x0, 0x3, 0xc3, 0xf8, 0x2, 0x20, 0xff,
    0xf7, 0xd7, 0xfe, 0x1, 0xe0, 0xff, 0xf7, 0xd4, 0xfe, 0x40,
    0xf6, 0x0, 0x63, 0xc2, 0xf2, 0x0, 0x3, 0x1b, 0x68, 0x3,
    0xf4, 0x80, 0x33, 0x0, 0x2b, 0xf4, 0xd1, 0x32, 0x20, 0xff,
    0xf7, 0xcc, 0xfe, 0x1, 0xe0, 0xff, 0xf7, 0xc5, 0xfe, 0x40,
    0xf6, 0x0, 0x63, 0xc2, 0xf2, 0x0, 0x3, 0x1b, 0x68, 0x3,
    0xf4, 0x80, 0x33, 0x0, 0x2b, 0xf4, 0xd0, 0x40, 0xf6, 0x80,
    0x63, 0xc2, 0xf2, 0x0, 0x3, 0x0, 0x22, 0x42, 0xf0, 0x2f,
    0x2, 0x1a, 0x70, 0x0, 0x22, 0x5a, 0x70, 0xff, 0xf7, 0xb0,
    0xfe, 0x40, 0xf6, 0x80, 0x63, 0xc2, 0xf2, 0x0, 0x3, 0x0,
    0x22, 0x42, 0xf0, 0x3f, 0x2, 0x1a, 0x70, 0x0, 0x22, 0x5a,
    0x70, 0xff, 0xf7, 0xa4, 0xfe, 0x64, 0x20, 0xff, 0xf7, 0xa5,
    0xfe, 0x1, 0x23, 0xfb, 0x73, 0x1, 0xe0, 0xff, 0xf7, 0x9c,
    0xfe, 0x40, 0xf6, 0x0, 0x63, 0xc2, 0xf2, 0x0, 0x3, 0x1b,
    0x68, 0x3, 0xf4, 0x80, 0x33, 0x0, 0x2b, 0xf4, 0xd1, 0x64,
    0x20, 0xff, 0xf7, 0x94, 0xfe, 0x1, 0xe0, 0xff, 0xf7, 0x8d,
    0xfe, 0x40, 0xf6, 0x0, 0x63, 0xc2, 0xf2, 0x0, 0x3, 0x1b,
    0x68, 0x3, 0xf4, 0x80, 0x33, 0x0, 0x2b, 0xf4, 0xd0, 0x40,
    0xf6, 0x0, 0x63, 0xc2, 0xf2, 0x0, 0x3, 0x5b, 0x68, 0x7b,
    0x60, 0xfa, 0x7b, 0x7b, 0x68, 0x4f, 0xf4, 0x14, 0x50, 0x11,
    0x46, 0x1a, 0x46, 0xff, 0xf7, 0xd4, 0xfe, 0xff, 0xf7, 0x74,
    0xfe, 0xfb, 0x7b, 0x1, 0x33, 0xfb, 0x73, 0xfb, 0x7b, 0x20,
    0x2b, 0x7, 0xd9, 0x4f, 0xf4, 0xc, 0x50, 0x0, 0x21, 0x0,
    0x22, 0xff, 0xf7, 0xc6, 0xfe, 0xff, 0xf7, 0x66, 0xfe, 0x64,
    0x20, 0xff, 0xf7, 0x67, 0xfe, 0x0, 0xbf, 0xc4, 0xe7, 0x0,
    0xbf, 0x4d, 0x45, 0x54, 0x41, 0x1, 0x0, 0x0, 0x0, 0x40,
    0x0, 0x0, 0x0, 0x14, 0xa, 0x4, 0xd, 0x1, 0xe, 0x50,
    0x4e, 0x44, 0x53, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x72, 0x5f, 0xdd, 0xa5


};

/// <summary>
/// This is the Tilt Motor class constructor
/// 
/// </summary>
/// 
/// The constructor initializes the Base class with the motor address, gear ratio and the direction logic.
/// 
/// The Tilt motor makes use of the nanoj program for the Tomo exposures so 
/// the constructor passes the nano-j program vector pointer to the base class
/// in order to be uploaded at the startup.
/// 
/// The Encoder is initialized with the stored value in the configuration file:
/// - in case the position should be invalid, the error Notify::messages::ERROR_TILT_MOTOR_HOMING is activated;
/// - in case of error, the zero setting shall be executed before any activation may take place.
/// 
/// 
/// <param name=""></param>
TiltMotor::TiltMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::TILT_ID, L"MOTOR_TILT", GEAR_RATIO, false)
{
    setNanoJPtr(nanojTrxProgram, sizeof(nanojTrxProgram));

    // Gets the initial position of the encoder. If the position is a valid position the oming is not necessary
    bool homing_initialized = false;
    int  init_position = 0;

    if (MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_CURRENT_POSITION] != MotorConfig::MOTOR_UNDEFINED_POSITION) {
        homing_initialized = true;
        init_position = System::Convert::ToInt32(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_CURRENT_POSITION]);
    }

    setEncoderInitStatus(homing_initialized);
    setEncoderInitialUvalue(init_position);
    

    // Activate a warning condition is the motor should'n be initialized
    if (!isEncoderInitialized()) Notify::activate(Notify::messages::ERROR_TILT_MOTOR_HOMING);
  
    tomo_scan = false;
    pending_target = target_options::UNDEF;
    current_target = target_options::UNDEF;
}

/// <summary>
/// This function activates the Tomo Scan rotation mode.
/// </summary>
/// 
/// The Tomo scan rotation mode starts when the Digital Input-1 is triggered high.
/// 
/// The Digital Input 1 is assigned to the Exp-Win signal coming from the detector
/// 
/// <param name="pos">Target position in User units</param>
/// <param name="speed">Speed in user units</param>
/// <param name="acc">Acceleration in User units</param>
/// <param name="dec">Deceleration in user units</param>
/// <returns>true if the command successfully started</returns>
bool TiltMotor::activateTomoScan(int pos, int speed, int acc, int dec) { 
    pending_target = target_options::TOMO_E;
    tomo_scan = TiltMotor::device->activateAutomaticPositioning(0, pos, speed, acc, dec, false);
    return tomo_scan;
}

/// <summary>
/// This is the override callback called in case of Motor Device reset event.
/// 
/// </summary>
/// 
/// In case the device should reset after the initialization, the initialization process shall 
/// restart again.
///  
/// <param name=""></param>
void TiltMotor::resetCallback(void) {

    // Gets the initial position of the encoder. If the position is a valid position the oming is not necessary
    bool homing_initialized = false;
    int  init_position = 0;

    if (MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_CURRENT_POSITION] != MotorConfig::MOTOR_UNDEFINED_POSITION) {
        homing_initialized = true;
        init_position = System::Convert::ToInt32(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_CURRENT_POSITION]);
    }

    setEncoderInitStatus(homing_initialized);
    setEncoderInitialUvalue(init_position);

    // Activate a warning condition is the motor should'n be initialized
    if (!isEncoderInitialized()) Notify::activate(Notify::messages::ERROR_TILT_MOTOR_HOMING);

    // Activates the configuration of the device
    activateConfiguration();
    pending_target = target_options::UNDEF;
    current_target = target_options::UNDEF;
}

/// <summary>
/// This is the override callback called during the initialization fase.
/// 
/// </summary>
/// 
/// The Tilt motor device initializes here the specific device's register.
/// 
/// The module, after register initialization, set the brake device in order 
/// to evaluates the correct behavior:
/// - Executes an unlock procedure;
/// - Executes a lock procedure;
/// - in case of wrong locking status the Notify::messages::ERROR_TILT_MOTOR_BRAKE_FAULT error is generated.
/// 
/// <param name=""></param>
/// <returns>true if the initialization termines successfully</returns>
bool TiltMotor::initializeSpecificObjectDictionaryCallback(void) {

    // Motor Drive Parameter Set
    while (!blocking_writeOD(OD_3210_01, 10000)); // 50000 Position Loop, Proportional Gain (closed Loop)
    while (!blocking_writeOD(OD_3210_02, 5));	 // 10  Position Loop, Integral Gain (closed Loop)

    // Position Range Limit
    while (!blocking_writeOD(OD_607B_01, convert_User_To_Encoder(MIN_ROTATION_ANGLE - 200))); 	// Min Position Range Limit
    while (!blocking_writeOD(OD_607B_02, convert_User_To_Encoder(MAX_ROTATION_ANGLE + 200)));	// Max Position Range Limit

    // Software Position Limit
    if (!blocking_writeOD(OD_607D_01, convert_User_To_Encoder(MIN_ROTATION_ANGLE))) return false;	// Min Position Limit
    if (!blocking_writeOD(OD_607D_02, convert_User_To_Encoder(MAX_ROTATION_ANGLE))) return false;	// Max Position Limit

    // Sets the Output setting
    if (!blocking_writeOD(OD_3250_02, 0)) return false; // Output control not inverted
    if (!blocking_writeOD(OD_3250_03, 0)) return false; // Force Enable = false
    if (!blocking_writeOD(OD_3250_08, 0)) return false; // Routing Enable = false
    if (!blocking_writeOD(OD_60FE_01, 0)) return false; // Set All outputs to 0

    // Set the input setting
    if (!blocking_writeOD(OD_3240_01, 0x4)) return false; // Input control special: I3 = HOMING
    if (!blocking_writeOD(OD_3240_02, 0)) return false;   // Function Inverted: not inverted
    if (!blocking_writeOD(OD_3240_03, 0)) return false;   // Force Enable = false
    if (!blocking_writeOD(OD_3240_06, 0)) return false;   // Input Range Select: threshold = 5V;


    // Test unlock brake
    if (!unlockBrake()) {
        Debug::WriteLine("TiltMotor: Failed test unlock brake");
        Notify::activate(Notify::messages::ERROR_TILT_MOTOR_BRAKE_FAULT);
        brake_alarm = true;
        return true;
    }

    // Test lock brake
    if (!lockBrake()) {
        Debug::WriteLine("TiltMotor: Failed test lock brake");
        Notify::activate(Notify::messages::ERROR_TILT_MOTOR_BRAKE_FAULT);
        brake_alarm = true;
        return true;
    }

    return true;
}



/// <summary>
/// This function activates the Automatic Homing procedure
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool TiltMotor::startHoming(void) {
    pending_target = target_options::UNDEF;
    current_target = pending_target;

    // Gets the Speed and Acceleration from the configuration file
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_HOME_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_HOME_ACC]);
    return device->activateAutomaticHoming(HOMING_ON_METHOD, HOMING_OFF_METHOD, speed, acc);
}

/// <summary>
/// This function is internally used to command the brake to unlock.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool TiltMotor::unlockBrake(void) {

return true;

    // Sets the OUTPUT 1 properly
    if (!blocking_writeOD(OD_60FE_01, UNLOCK_BRAKE_OUT_MASK)) {
        blocking_writeOD(OD_60FE_01, LOCK_BRAKE_OUT_MASK);
        return false;
    }

    // Test the input feedback to detect the effective unlock condition
    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!blocking_readOD(OD_60FD_00)) continue; // Reads the Inputs

        // Unlock OK
        if (UNLOCK_BRAKE_INPUT_MASK(getRxReg()->data)) return true;
    }

    // Error in detecting the brake activation
    blocking_writeOD(OD_60FE_01, LOCK_BRAKE_OUT_MASK);
    brake_alarm = true;
    return false;
}

/// <summary>
/// This function is internally used to command the brake to lock.
/// 
/// </summary>
/// 
/// <param name=""></param>
/// <returns></returns>
bool  TiltMotor::lockBrake(void) {

return true;
    // Sets the OUTPUT 1 properly   
    if (!blocking_writeOD(OD_60FE_01, LOCK_BRAKE_OUT_MASK)) {        
        return false;
    }

    // Test the input feedback to detect the effective deactivation
    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!blocking_readOD(OD_60FD_00)) continue; // Reads the Inputs
        if (!UNLOCK_BRAKE_OUT_MASK(getRxReg()->data)) return true;
    }

    // Error in detecting the brake activation    
    brake_alarm = true;
    return false;
}

// ______________________ BASE CLASS CALLBACKS _________________________________________________

/// <summary>
/// This callback is called by the base class when the brake device shall be locked.
/// </summary>
/// 
/// Usually when the activation termines, before to release the motor power torque 
/// the brake device shall lock the shaft.
/// 
/// <param name=""></param>
/// <returns>true if the brake correctly locks</returns>
bool TiltMotor::brakeCallback(void) {
return true;
    return lockBrake();
}

/// <summary>
/// This callback is called by the base class when the brake device shall be unlocked.
/// </summary>
/// 
/// The base class calls this callback after the motor torque is applied and before 
/// to start the rotation.
/// 
/// The callback tests the effective unlocking condition.
///  
/// <param name=""></param>
/// <returns>true if the brake device is correctly unlocked</returns>
bool TiltMotor::unbrakeCallback(void) {
return true;
    // Unlock the Brake device
    if (!unlockBrake()) {
        Debug::WriteLine("TiltMotor: Activation failed to unlock the brake device");
        return false;
    }

    return true;
}

// IDLE STATUS ----------------------------------------------------------------------

/// <summary>
/// The module overrides this function in order to handle the IDLE activities.
/// 
/// </summary>
/// 
/// In Idle status the module:
/// - test the brake device activity;
/// - monitors the safety conditions;
/// - monitors the manual activation inputs;
/// 
/// <param name=""></param>
/// <returns>MotorCompletedCodes::COMMAND_PROCEED in case of ready conditon </returns>
TiltMotor::MotorCompletedCodes TiltMotor::idleCallback(void) {
    MotorCompletedCodes ret_code = MotorCompletedCodes::COMMAND_PROCEED;
    int speed, acc, dec;

return MotorCompletedCodes::COMMAND_PROCEED;

    // With the brake alarm present, no more action can be executed
    if (brake_alarm) {

        // Keeps the alarm ON
        Notify::activate(Notify::messages::ERROR_TILT_MOTOR_BRAKE_FAULT);

        // Alarm condition
        lockBrake();
        ret_code = MotorCompletedCodes::ERROR_BRAKE_DEVICE;
    }
    else {
        // reads the Motor GPIO inputs
        if (!blocking_readOD(OD_60FD_00)) return MotorCompletedCodes::ERROR_ACCESS_REGISTER;
        if (UNLOCK_BRAKE_INPUT_MASK(getRxReg()->data)) {
            brake_alarm = true;
            Debug::WriteLine("TiltMotor: brake detected unlocked in IDLE");
            ret_code = MotorCompletedCodes::ERROR_BRAKE_DEVICE;
        }
    }

    // If the safety condition should prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::TILT_ID)) {
        ret_code = MotorCompletedCodes::ERROR_SAFETY;
    }

    // Handle a Manual activation mode    
    bool man_increase = Gantry::getManualRotationIncrease((int)CANOPEN::MotorDeviceAddresses::TILT_ID);
    bool man_decrease = Gantry::getManualRotationDecrease((int)CANOPEN::MotorDeviceAddresses::TILT_ID);
    if (man_increase || man_decrease) {
        if (!manual_activation_enabled) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_MANUAL_DISABLE);
        else if (ret_code == MotorCompletedCodes::ERROR_SAFETY) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_SAFETY_DISABLE);
        else if (ret_code != MotorCompletedCodes::COMMAND_PROCEED) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
        else {
            pending_target = target_options::UNDEF;
            current_target = target_options::UNDEF;
            speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_MANUAL_SPEED]);
            acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_MANUAL_ACC]);
            dec = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_MANUAL_DEC]);

            if (man_increase) {
                manual_increment_direction = true;
                if (!device->activateManualPositioning(MAX_ROTATION_ANGLE, speed, acc, dec)) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
            }
            else {
                manual_increment_direction = false;
                if (!device->activateManualPositioning(MIN_ROTATION_ANGLE, speed, acc, dec)) Notify::instant(Notify::messages::INFO_ACTIVATION_MOTOR_ERROR_DISABLE);
            }
        }
    }

    return MotorCompletedCodes::COMMAND_PROCEED;

}

// AUTO PROCEDURES -------------------------------------------------------------------

/// <summary>
/// This function is called at the beginning of the automatic activation
/// </summary>
/// 
/// The function invalidate the current encoder position in the case, 
/// during the activation, the software should be killed before to update the current encoder position.
/// 
/// <param name=""></param>
/// <returns></returns>
TiltMotor::MotorCompletedCodes TiltMotor::automaticPositioningPreparationCallback(void) {

    // If the tomo scan has been activated, the nanoj program shall be activated
    if (tomo_scan) {
        if (!startNanoj()) return MotorCompletedCodes::ERROR_STARTING_NANOJ;
    }

    // Invalidate the position: if the command should completes the encoder position will lbe refresh 
    // with the current valid position
    MotorConfig::Configuration->setParam(MotorConfig::PARAM_TILT, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
    MotorConfig::Configuration->storeFile();
    return MotorCompletedCodes::COMMAND_PROCEED;
}

CanOpenMotor::MotorCompletedCodes  TiltMotor::automaticPositioningRunningCallback(void) {

    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::TILT_ID)) {
        return MotorCompletedCodes::ERROR_SAFETY;
    }

    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getObstacleRotationStatus((int)CANOPEN::MotorDeviceAddresses::TILT_ID)) {
        return MotorCompletedCodes::ERROR_OBSTACLE_DETECTED;
    }

    return MotorCompletedCodes::COMMAND_PROCEED;
}
/// <summary>
/// The TiltMotor class override this function in order to update the current position
/// and the command termination
/// 
/// </summary>
/// <param name=error></param>
void TiltMotor::automaticPositioningCompletedCallback(MotorCompletedCodes error) {

    if (tomo_scan) stopNanoj();
    tomo_scan = false;

    if (isEncoderInitialized()) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_TILT, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
    }

    // Notify the command termination event
    device->command_completed_event(getCommandId(), (int)error);

    // Assignes the current target
    if (error == MotorCompletedCodes::COMMAND_SUCCESS) {
        current_target = pending_target;
    }
    else {
        current_target = target_options::UNDEF;
    }

    return;
}

// MANUAL PROCEDURES -------------------------------------------------------------------


/// The BodyMotor class override this function in order to handle the manual activation process.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
CanOpenMotor::MotorCompletedCodes  TiltMotor::manualPositioningRunningCallback(void) {

    // If the safety condition prevent the command execution it is immediatelly aborted
    if (Gantry::getSafetyRotationStatus((int)CANOPEN::MotorDeviceAddresses::TILT_ID)) {
        return MotorCompletedCodes::ERROR_SAFETY;
    }

    // handle the manual hardware inputs
    bool man_increase = Gantry::getManualRotationIncrease((int)CANOPEN::MotorDeviceAddresses::TILT_ID);
    bool man_decrease = Gantry::getManualRotationDecrease((int)CANOPEN::MotorDeviceAddresses::TILT_ID);
    if ((!man_increase) && (!man_decrease)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    if ((!man_increase) && (manual_increment_direction)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;
    if ((!man_decrease) && (!manual_increment_direction)) return MotorCompletedCodes::COMMAND_MANUAL_TERMINATION;

    // Proceeds with the manual activation
    return MotorCompletedCodes::COMMAND_PROCEED;
}

void TiltMotor::manualPositioningCompletedCallback(MotorCompletedCodes error) {

    if (isEncoderInitialized()) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_TILT, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
    }

    device->command_completed_event((int)0, (int)error); 
    current_target = target_options::UNDEF;

    return;
}



// HOMING PROCEDURES -------------------------------------------------------------------

/// <summary>
/// The TiltMotor class override this function in order to 
///handle the homing completion process.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
void TiltMotor::automaticHomingCompletedCallback(MotorCompletedCodes error) {

    if (isEncoderInitialized()) {
        // Set the position in the configuration file and clear the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_TILT, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
        Notify::deactivate(Notify::messages::ERROR_TILT_MOTOR_HOMING);
    }
    else {
        // Reset the position in the configuration file and reactivate the alarm
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_TILT, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
        MotorConfig::Configuration->storeFile();
        Notify::activate(Notify::messages::ERROR_TILT_MOTOR_HOMING);
    }

    // Notify the command termination event
    device->command_completed_event((int)0, (int)error);
    current_target = target_options::UNDEF;
}
