#include "Motors.h"
#include "TiltMotor.h"
#include "pd4_od.h"
#include "canInterface.h"
#include <thread>
#include "mutex"


TiltMotor::TiltMotor(void):Motors(0x3) {

	

}
