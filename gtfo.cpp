using namespace std;

#include "serialib.h"
#include "StateMachine/SerialStateMachine.h"

#include "StateMachine/Nodes/SendStringNode.h"
#include "StateMachine/Nodes/NopNode.h"

#include <stdio.h>

#define SERIAL_PORT "COM3"

int main(int argc, char** argv) {

    serialib serial;

    char errorOpen = serial.openDevice(SERIAL_PORT, 9600);

    if(errorOpen != 1) return errorOpen;

    printf("Connected to %s\n", SERIAL_PORT);

    SerialStateMachine machine(serial);

    NopNode success(machine.endNode(), "SUCCESS");
    NopNode fail(machine.endNode(), "FAIL");
    success.msgTemplate.shouldWait = false;
    fail.msgTemplate.shouldWait = false;

    SendStringNode askVersion(fail, "version\r");
    askVersion.connect("\nGTCP2-0.0.2\r", &success);

    NopNode wakeupMsg(askVersion);
    wakeupMsg.msgTemplate.delimiter = '!';

    machine.run(wakeupMsg);

    serial.closeDevice();

    return 0;
}