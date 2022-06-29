using namespace std;

#include "serialib.h"
#include "StateMachine/SerialStateMachine.h"

#include "StateMachine/Nodes/SendStringNode.h"
#include "StateMachine/Nodes/NopNode.h"
#include "StateMachine/Nodes/IncrementingNode.h"

#include <stdio.h>

#define SERIAL_PORT "COM3"

int main(int argc, char** argv) {

    serialib serial;

    char errorOpen = serial.openDevice(SERIAL_PORT, 9600);

    if(errorOpen != 1) {
        printf("Couldn't connect to %s\n", SERIAL_PORT);
        return errorOpen;
    }

    printf("Connected to %s\n", SERIAL_PORT);

    SerialStateMachine machine(serial);

    NopNode success(machine.endNode(), "SUCCESS");
    NopNode fail(machine.endNode(), "FAIL");
    success.msgTemplate.shouldWait = false;
    fail.msgTemplate.shouldWait = false;

    IncrementingNode shifter(success, "shift %x\r", 12);
    shifter.msgTemplate.skipLines = 0;
    shifter.params[0].val = 0;
    shifter.params[0].increment = 1;
    shifter.count = 16;
    shifter.msgTemplate.slice = 5;

    SendStringNode checksum(fail, "checksum 0 4000\r");
    checksum.msgTemplate.slice = 6;
    checksum.connect("CRC32:", &shifter);

    shifter.connect("shift", &checksum);

    SendStringNode eraseChip(fail, "eraseChip\r");
    eraseChip.msgTemplate.timeoutMs = 30000;
    eraseChip.connect("Done", &shifter);

    SendStringNode askVersion(fail, "version\r");
    askVersion.connect("GTCP2-0.0.2", &shifter);

    NopNode wakeupMsg(askVersion, "wakeupMsg");
    wakeupMsg.msgTemplate.delimiter = '!';

    machine.run(wakeupMsg);

    serial.closeDevice();

    return 0;
}