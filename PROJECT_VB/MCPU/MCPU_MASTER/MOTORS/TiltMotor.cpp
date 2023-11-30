#include "TiltMotor.h"

#define GEAR_RATIO (double) 352.2

/*      Versione con attesa iniziale del segnale EXP WIN
 *      ad un livello IDLE e inizializzazione a zero degli
 *      angoli di scansione
 *
 */
static const unsigned char nanojTrxProgram[] = {
    0x4e, 0x56, 0x4d, 0x4d, 0x0, 0x2, 0x0, 0x0, 0x1,
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

TiltMotor::TiltMotor(void) :CANOPEN::CanOpenMotor((unsigned char)CANOPEN::MotorDeviceAddresses::TILT_ID, L"MOTOR_TILT", GEAR_RATIO)
{
    setNanoJPtr(nanojTrxProgram, sizeof(nanojTrxProgram));
  
}