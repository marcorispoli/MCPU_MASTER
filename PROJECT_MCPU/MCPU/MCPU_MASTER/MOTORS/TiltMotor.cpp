#include "ConfigurationFiles.h"
#include "Notify.h"
#include "TiltMotor.h"
#include "SlideMotor.h"

#include "PCB301.h"
#include "pd4_od.h"
#include "..\gantry_global_status.h"
#include <thread>
#include "Log.h"


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

#define NANOJ_PROGRAM

#ifdef NANOJ_PROGRAM
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
#else
static const unsigned char nanojTrxProgram[] = { 0 };
#endif

/// <summary>
/// This is the Tilt Motor class constructor
/// 
/// </summary>
/// 
/// The constructor initializes the Base class with:
/// + the motor address;
/// + the motor gear ratio; 
/// + the rotation direction;
/// + the binary array of the special program to be uploaded into the motor driver for the automatic tomo activation mode; 
/// + the actual mechanical Tilt position stored into the \ref MotorConfig after the last acivation, in order to initialize the motor internal encoder;
/// + the target acceptable range set to +/- 0.02°;
/// 
/// <param name=""></param>
TiltMotor::TiltMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::TILT_ID, L"MOTOR_TILT", MotorConfig::PARAM_TILT, Notify::messages::ERROR_TILT_MOTOR_HOMING, MIN_ROTATION_ANGLE, MAX_ROTATION_ANGLE, GEAR_RATIO, 1, false)
{
    setNanoJPtr(nanojTrxProgram, sizeof(nanojTrxProgram));

    // Sets +/- 0.02 ° as the acceptable target range
    setTargetRange(2, 2);
     
    idle_positioning = false;
    tomo_scan = false;
    pending_target = target_options::UNDEF;
    current_target = target_options::UNDEF;
}

/// <summary>
/// This is the method to be called by the Application to activate the Tilt chained Idle command.
/// </summary>
/// 
/// The Idle command position is a System procedure that involves
/// the Tilt motor, the Slide motor and finally the Arm motor in an activation daisy chain:
/// + The Tilt is the first activation in the sequence;
/// + The Slide activation starts after the Tilt completion;
/// + Finally the Arm starts as the last motor in the chain;
/// 
/// Scope of this activation sequence is to implement a System position called "Idle Position"
/// that corresponds to a Gantry status when the Gantry is set in Idle mode.
/// 
/// \note 
/// The application may initiate the sequence arbitrarily with any motor of the chain:
/// in this case the activated motors will be only those that follows the caller in the chain.
/// 
/// Being the first motor in the chain, when the Application calls this method 
/// all the motors will be subsequently activated in the proper order.
/// 
bool TiltMotor::setIdlePosition(void) {
    idle_positioning = true;
    pending_target = target_options::SCOUT;
    return device->activateAutomaticPositioning(0, 0, true);
}

/// <summary>
/// 
/// </summary>
/// 
/// The module provides this method to let the Application to move the Tilt motor to a 
/// predefined set of targets (\ref target_options)
///  
/// <param name="tg">standard target name (see \ref target_options) </param>
/// <param name="id">
/// + AWS identifier command: if this code is greater then 0, at the command completion the 
/// module will send the \ref EVENT_Executed
/// 
/// </param>
/// <returns></returns>
bool TiltMotor::setTarget(target_options tg, int id) {
    int angle = 0;
    int speed, acc, dec;
    idle_positioning = false;
    pending_target = target_options::UNDEF;

    switch (tg) {
    case target_options::SCOUT:
        angle = 0;        
        break;

    case target_options::BP_R:
        angle = 1500;        
        break;
    case target_options::BP_L:
        angle = -1500;        
        break;

    case target_options::TOMO_H:
        if (!Exposures::getTomoExposure()->valid) return false;       
        if ((!SlideMotor::isAllowedPosition())) {
            LogClass::logInFile("TiltMotor::serviceAutoPosition() - command: error, slide not in allowed position ");
            return false;
        }        

        angle = Exposures::getTomoExposure()->tomo_home;        
        break;

    case target_options::TOMO_E:
        if ((!SlideMotor::isAllowedPosition())) {
            LogClass::logInFile("TiltMotor::serviceAutoPosition() - command: error, slide not in allowed position ");
            return false;
        }

        if (!Exposures::getTomoExposure()->valid) return false;
        angle = Exposures::getTomoExposure()->tomo_end;
        speed = Exposures::getTomoExposure()->tomo_speed;
        acc = Exposures::getTomoExposure()->tomo_acc;
        dec = Exposures::getTomoExposure()->tomo_dec;
        pending_target = tg;
        return device->activateAutomaticPositioning(id, angle, speed, acc, dec, true);
        break;

    default:
        return false;

    }

    // Activate the command        
    pending_target = tg;
    return device->activateAutomaticPositioning(id, angle,  true);
}

    /// <summary>
    /// 
    /// </summary>
    /// 
    /// The Application uses this method during for service purpose.
    /// 
    /// <param name="pos">target rotation position in 0.01° units</param>
    /// <returns>
    /// + true: the command has been executed successfully;
    /// + false: the command cannot start for some reason (CanOpenMotor::getCommandCompletedCode() for gets the error code) 
    /// </returns>
bool TiltMotor::serviceAutoPosition(int pos) {
    idle_positioning = false;
    // Test on the Slide position
    if ((pos > 15) || (pos < -15)) {

        if ((!SlideMotor::isAllowedPosition())) {
            LogClass::logInFile("TiltMotor::serviceAutoPosition() - command: error, slide not in allowed position ");
            return false;
        }
    }

    return device->activateAutomaticPositioning(0, pos * 100, true);

}

    /// <summary>
    /// 
    /// 
    /// </summary>
    /// 
    /// The Application calls this procedure when the Til is prepositioned 
    /// into the starting To scan position.
    /// 
    /// When this method is called, the silent program running into the motor
    /// is waken up. The motor will start the rotation as soon as its gpio input
    /// connected with the detector sinchro signal detects a transition level from
    /// not active to active.
    /// 
    /// 
    /// <param name="pos">this is the final tomo position in 0.01° units</param>
    /// <param name="speed">this is the travel speed in 0,01°/s units</param>
    /// <param name="acc">this is the acceleration rate in 0.01°/s^2</param>
    /// <param name="dec">this is the deceleration rate in 0.01°/s^2</param>
     /// <returns>
    /// + true: the command has been executed successfully;
    /// + false: the command cannot start for some reason (CanOpenMotor::getCommandCompletedCode() for gets the error code) 
    /// </returns>
bool TiltMotor::activateTomoScan(int pos, int speed, int acc, int dec) { 
    idle_positioning = false;

    // never allowed when slide is out of position
    if ((!SlideMotor::isAllowedPosition())) {
        LogClass::logInFile("activateTomoScan::serviceAutoPosition() - command: error, slide not in allowed position ");
        return false;
    }

    pending_target = target_options::TOMO_E;
    tomo_scan = TiltMotor::device->activateAutomaticPositioning(0, pos, speed, acc, dec, false);
    return tomo_scan;
}

/// <summary>
/// 
/// </summary>
/// 
/// In case the motor should reset, the base class of this module
/// restart the initialization process.
/// 
/// When the initialization termines this callback is called.
/// 
/// The module resets the current targets to set the actual target as unkown target.
///  
/// <param name=""></param>
void TiltMotor::resetCallback(void) {

    pending_target = target_options::UNDEF;
    current_target = target_options::UNDEF;
}

/// <summary>
/// 
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
#define TILT_OD_CODE 0x0001
unsigned short TiltMotor::initializeSpecificObjectDictionaryCallback(void) {

    // Motor Drive Parameter Set
    while (!blocking_writeOD(OD_3210_01, 10000)); // 50000 Position Loop, Proportional Gain (closed Loop)
    while (!blocking_writeOD(OD_3210_02, 5));	 // 10  Position Loop, Integral Gain (closed Loop)

    
    // Sets the Output setting
    if (!blocking_writeOD(OD_3250_02, 0)) return 0; // Output control not inverted
    if (!blocking_writeOD(OD_3250_03, 0)) return 0; // Force Enable = false
    if (!blocking_writeOD(OD_3250_08, 0)) return 0; // Routing Enable = false
    if (!blocking_writeOD(OD_60FE_01, 0)) return 0; // Set All outputs to 0

    // Set the input setting
    if (!blocking_writeOD(OD_3240_01, 0x4)) return 0; // Input control special: I3 = HOMING
    if (!blocking_writeOD(OD_3240_02, 0)) return 0;   // Function Inverted: not inverted
    if (!blocking_writeOD(OD_3240_03, 0)) return 0;   // Force Enable = false
    if (!blocking_writeOD(OD_3240_06, 0)) return 0;   // Input Range Select: threshold = 5V;

  

    // Test unlock brake
    if (!unlockBrake()) {
        LogClass::logInFile("TiltMotor: Failed test unlock brake");
        Notify::activate(Notify::messages::ERROR_TILT_MOTOR_BRAKE_FAULT);
        brake_alarm = true;
        return TILT_OD_CODE;
    }

    // Test lock brake
    if (!lockBrake()) {
        LogClass::logInFile("TiltMotor: Failed test lock brake");
        Notify::activate(Notify::messages::ERROR_TILT_MOTOR_BRAKE_FAULT);
        brake_alarm = true;
        return TILT_OD_CODE;
    }

    return TILT_OD_CODE;
}



/// <summary>
/// 
/// 
/// </summary>
/// 
/// The automatic zero setting procedure activates the motor rotation 
/// until a photocell dedicated to the zero setting procedure intercepts 
/// the mechanical zero setting point.
/// 
/// This method should be called by the application for service,
/// in case a automatic zero setting should be executed.
/// 
/// This methods is not a blocking method for the caller thread:
/// + the encoder reset may takes time after this method returns.
/// 
/// \note:
/// + the Application shall call the method CanOpenMotor::isRunning() to checks if the 
/// manual command is terminated;
/// + the application shall call the CanOpenMotor::getCommandCompletedCode() to get the result 
/// of the command as soon as the CanOpenMotor::isRunning() should return false.
/// 
/// <param name=""></param>
/// <returns>
/// + true: the zero setting process is actually started;
/// + false: the zero setting cannot start for some reason (CanOpenMotor::getCommandCompletedCode() for gets the error code) 
/// </returns>
bool TiltMotor::startAutoHoming(void) {
    pending_target = target_options::UNDEF;
    current_target = pending_target;

    // Gets the Speed and Acceleration from the configuration file
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_HOME_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_HOME_ACC]);
    return device->activateAutomaticHoming(HOMING_ON_METHOD, HOMING_OFF_METHOD, speed, acc);
}

    /// <summary>
    /// 
    /// 
    /// </summary>
    /// 
    /// The manual zero setting procedure is a procedure that doesn't require
    /// any motor activation. The motor sets its internal encoder to the value
    /// manually passed by this module.
    /// 
    /// This method should be called by the application for service,
    /// in case a position manual zero setting should be executed.
    /// 
    /// This methods is not a blocking method for the caller thread:
    /// + the encoder reset may takes time after this method returns.
    /// 
    /// \note:
    /// + the Application shall call the method CanOpenMotor::isRunning() to checks if the 
    /// manual command is terminated;
    /// 
    /// + the application shall call the CanOpenMotor::getCommandCompletedCode() to get the result 
    /// of the command as soon as the CanOpenMotor::isRunning() should return false.
    /// 
    /// <param name="target_position">This is the current effective position in 0.01 degrees units</param>
    /// <returns>
    /// + true: the zero setting process is actually started;
    /// + false: the zero setting cannot start
    /// </returns>
    /// + true: the command successfully started
    /// + false: a condition prevents to activate this command.
    /// 
bool TiltMotor::startManualHoming(int target_position) {
    pending_target = target_options::UNDEF;
    current_target = pending_target;

    if (device->isPositionFromExternalSensor()) return device->activateExternalHoming(target_position);
    else return device->activateManualHoming(target_position);
}

/// <summary>
/// This function is internally used to command the brake to unlock.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool TiltMotor::unlockBrake(void) {
    if (simulator_mode) return true;

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
    if (simulator_mode) return true;

    // Sets the OUTPUT 1 properly   
    if (!blocking_writeOD(OD_60FE_01, LOCK_BRAKE_OUT_MASK)) {        
        return false;
    }

    // Test the input feedback to detect the effective deactivation
    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!blocking_readOD(OD_60FD_00)) continue; // Reads the Inputs
        if (!UNLOCK_BRAKE_INPUT_MASK(getRxReg()->data)) return true;
    }

    // Error in detecting the brake activation    
    brake_alarm = true;
    return false;
}

// ______________________ BASE CLASS CALLBACKS _________________________________________________

/// <summary>
/// 
/// </summary>
/// 
/// Usually when the activation termines, before to release the motor power torque 
/// the brake device shall lock the shaft.
/// 
/// <param name=""></param>
/// <returns>true if the brake correctly locks</returns>
bool TiltMotor::brakeCallback(void) {

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


    // Unlock the Brake device
    if (!unlockBrake()) {
        LogClass::logInFile("TiltMotor: Activation failed to unlock the brake device");
        return false;
    }

    return true;
}

// IDLE STATUS ----------------------------------------------------------------------

/// <summary>
/// 
/// 
/// </summary>
///
/// In Idle status the module monitor the current status of the stationary brake.
/// In the case it should result unlocked the module set out of service the Tilt
/// and a relevant error will be generated.
/// 
/// \note
/// No more activation can be performed with the active brake fault condition.
/// 
/// <param name=""></param>
/// <returns>
/// + MotorCompletedCodes::COMMAND_PROCEED: no fault condition;
/// + MotorCompletedCodes::ERROR_BRAKE_DEVICE in case of fault condition; 
/// </returns>
TiltMotor::MotorCompletedCodes TiltMotor::idleCallback(void) {
    MotorCompletedCodes ret_code = MotorCompletedCodes::COMMAND_PROCEED;
    
    if (!device->simulator_mode) {

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
                LogClass::logInFile("TiltMotor: brake detected unlocked in IDLE");
                ret_code = MotorCompletedCodes::ERROR_BRAKE_DEVICE;
            }
        }

    } // simulator_mode

   

    return ret_code;

}

/// <summary>
/// This callback is called during the activation preparation fase.
/// </summary>
/// 
/// The module oveeride this callback in order to start the special command 
/// on the motor driver, in case the activation requested should be for a Tomo sequence.
/// 
/// <param name="current_command">the base class passes the code of the current command that is excuting</param>
/// <param name="current_position">the base class passes the current position</param>
/// <param name="target_position">the base class passes the target command position</param>
/// <returns>
/// + MotorCompletedCodes::COMMAND_PROCEED: the command can proceed;
/// + MotorCompletedCodes::ERROR_STARTING_NANOJ: the command shall abort due to error in starting the special program;
/// </returns>
TiltMotor::MotorCompletedCodes TiltMotor::preparationCallback(MotorCommands current_command, int current_position, int target_position) {
    if (device->simulator_mode)  return MotorCompletedCodes::COMMAND_PROCEED;
    if (current_command != MotorCommands::MOTOR_AUTO_POSITIONING) return MotorCompletedCodes::COMMAND_PROCEED;

    //starts the nano-j program for tomo activation
    if (tomo_scan) {
        if (!startNanoj()) return MotorCompletedCodes::ERROR_STARTING_NANOJ;
    }

    return MotorCompletedCodes::COMMAND_PROCEED;
}

/// <summary>
/// The module overrides this callback iin order to update the module status at the command completion.
/// </summary>
/// 
/// When a command completes the module shall executes the following steps:
/// + in case of a tomo sequence shall stop the special program running into the motor driver;
/// + assignes the current target code;
/// + In case it should be the special Idle activation mode, the module shall start the Slide motor to the Idle position;
/// + if the activation has been initiated by the AWS (id >0) the module will send the \ref EVENT_Executed to the AWS;
/// 
/// <param name="id">the identifier of the AWS command;</param>
/// <param name="current_command">the current executed command code;</param>
/// <param name="current_position">the final Tilt position</param>
/// <param name="term_code">the command_completed code of the activation;</param>
void TiltMotor::completedCallback(int id, MotorCommands current_command, int current_position, MotorCompletedCodes term_code) {
    if (current_command == MotorCommands::MOTOR_AUTO_POSITIONING) {
        if (!simulator_mode) {
            if (tomo_scan) stopNanoj();
        }
        tomo_scan = false;

        // Assignes the current target
        if (term_code == MotorCompletedCodes::COMMAND_SUCCESS) current_target = pending_target;
        else  current_target = target_options::UNDEF;
        

        // Next step of the Idle positioning
        if (idle_positioning) {
            idle_positioning = false;
            if (term_code == MotorCompletedCodes::COMMAND_SUCCESS)  SlideMotor::setIdlePosition();
            return;
        }

        // Notify the command termination event
        device->command_completed_event(id, (int) term_code);
    }
    else {
        device->command_completed_event(id, (int) term_code);
        current_target = target_options::UNDEF;
    }
}

