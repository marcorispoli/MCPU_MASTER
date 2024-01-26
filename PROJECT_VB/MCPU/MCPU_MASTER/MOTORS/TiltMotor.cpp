#include "CalibrationConfig.h"
#include "Notify.h"
#include "TiltMotor.h"
#include "PCB301.h"
#include "pd4_od.h"
#include <thread>


#define GEAR_RATIO (double) 352.2

#define BRAKE_INPUT_MASK(x) (x & 0x00080000) //!< Not in the Special region [00II][0000]
#define OUPUT1_OUT_MASK     0x00010000 //!< Not in the Special region [00II][0000]
#define OUPUT2_OUT_MASK     0x00020000 //!< Not in the Special region [00II][0000]

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
  
}

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
}
/// <summary>
/// The TiltMotor override this function in order to initialize specific motor registers
/// 
/// </summary>
/// 
/// 
/// <param name=""></param>
/// <returns>true if the initialization termines successfully</returns>

bool TiltMotor::initializeSpecificObjectDictionaryCallback(void) {


    return true;
}


/// <summary>
/// The TiltMotor class override this function in order to handle the IDLE activities
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
TiltMotor::MotorCompletedCodes TiltMotor::idleCallback(void) {
    
    /*
    // With the brake alarm present, no more action can be executed
    if (brake_alarm) {

        // Alarm condition
        blocking_writeOD(OD_60FE_01, 0); // Set All outputs to 0
        return false;
    }

    // Check if the BRAKE input is OFF. In case it should be ON, a relevant alarm shall be activated
    if (!blocking_readOD(OD_60FD_00)) return false;
    if (BRAKE_INPUT_MASK(rxSdoRegister->data)) {

        brake_alarm = true;
        Debug::WriteLine("TiltMotor: Failed test brake input in IDLE");
        Notify::activate(Notify::messages::ERROR_TILT_MOTOR_BRAKE_FAULT, false);
        blocking_writeOD(OD_60FE_01, 0); // Set All outputs to 0
        return false;
    }

    // Handle the Safety condition 

    */
    return MotorCompletedCodes::COMMAND_PROCEED;

}

CanOpenMotor::MotorCompletedCodes TiltMotor::automaticHomingPreparationCallback(void) {
    return automaticPositioningPreparationCallback();
}


CanOpenMotor::MotorCompletedCodes  TiltMotor::automaticPositioningRunningCallback(void) {
    return MotorCompletedCodes::COMMAND_PROCEED;
}
/// <summary>
/// The TiltMotor class override this function in order to update the current position
/// and the command termination
/// 
/// </summary>
/// <param name=error></param>
void TiltMotor::automaticPositioningCompletedCallback(MotorCompletedCodes error) {

    // Lock the brake device
    // The control of the brake status is done in the IDLE status
    blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (isEncoderInitialized()) {
        MotorConfig::Configuration->setParam(MotorConfig::PARAM_TILT, MotorConfig::PARAM_CURRENT_POSITION, device->getCurrentEncoderUposition().ToString());
        MotorConfig::Configuration->storeFile();
    }

    // Notify the command termination event
    device->command_completed_event(getCommandId(), (int)error);

    return;
}

void TiltMotor::manualPositioningCompletedCallback(MotorCompletedCodes error) {

    // Lock the brake device
    // The control of the brake status is done in the IDLE status
    blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Notify the command termination event
    device->command_completed_event((int)0, (int)error);

    return;
}

/// <summary>
/// The TiltMotor class override this function in order to 
///handle the homing completion process.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
void TiltMotor::automaticHomingCompletedCallback(MotorCompletedCodes error) {

    // Lock the brake device
    // The control of the brake status is done in the IDLE status
    blocking_writeOD(OD_60FE_01, OUPUT2_OUT_MASK);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

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
}





/// <summary>
/// This function activates the Automatic Homing procedure
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool TiltMotor::startHoming(void) {

    // Gets the Speed and Acceleration from the configuration file
    int speed = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_HOME_SPEED]);
    int acc = System::Convert::ToInt16(MotorConfig::Configuration->getParam(MotorConfig::PARAM_TILT)[MotorConfig::PARAM_HOME_ACC]);
    return device->activateAutomaticHoming(HOMING_ON_METHOD, HOMING_OFF_METHOD, speed, acc);
}



/// <summary>
/// The BodyMotor class override this function in order to handle the manual activation process.
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
CanOpenMotor::MotorCompletedCodes  TiltMotor::manualPositioningRunningCallback(void) {

    // Handles the enable condition
    if (!manual_activation_enabled) {
        return MotorCompletedCodes::ERROR_COMMAND_DISABLED;
    }

    // Handle the limit switches

    // Handle the safety

    // handle the manual hardware inputs
    

    // Proceeds with the manual activation
    return MotorCompletedCodes::COMMAND_PROCEED;
}

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

    // Invalidate the position: if the command should completes the encoder position will lbe refresh 
    // with the current valid position
    MotorConfig::Configuration->setParam(MotorConfig::PARAM_TILT, MotorConfig::PARAM_CURRENT_POSITION, MotorConfig::MOTOR_UNDEFINED_POSITION);
    MotorConfig::Configuration->storeFile();
    return MotorCompletedCodes::COMMAND_PROCEED;
}

/// <summary>
/// This function is called at the beginning of the automatic activation
/// </summary>
/// 
/// See the automaticPositioningPreparationCallback()
/// 
/// <param name=""></param>
/// <returns></returns>
TiltMotor::MotorCompletedCodes TiltMotor::manualPositioningPreparationCallback(void) {
    return automaticPositioningPreparationCallback();
}