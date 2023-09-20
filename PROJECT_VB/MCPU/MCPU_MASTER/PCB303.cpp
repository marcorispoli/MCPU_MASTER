#include "pch.h"
#include "PCB303.h"

#include <thread>

void PCB303::runningLoop(void) {

    while (!send(GET_STATUS_SYSTEM_REGISTER));


    std::this_thread::sleep_for(std::chrono::microseconds(10000));

    return;
}
