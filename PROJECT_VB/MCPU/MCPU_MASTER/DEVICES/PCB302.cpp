
#include "PCB302.h"

#include <thread>

void PCB302::runningLoop(void) {

    while (!send(GET_STATUS_SYSTEM_REGISTER));

    std::this_thread::sleep_for(std::chrono::microseconds(10000));

    return;
}
