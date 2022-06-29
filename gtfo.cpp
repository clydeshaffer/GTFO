using namespace std;

#include "serialib.h"
#include "StateMachine/SerialStateMachine.h"

#include "StateMachine/Nodes/SendStringNode.h"
#include "StateMachine/Nodes/NopNode.h"
#include "StateMachine/Nodes/IncrementingNode.h"
#include "StateMachine/Nodes/StreamNode.h"

#include <stdio.h>
#include <fstream>

#define SERIAL_PORT "COM3"

int main(int argc, char** argv) {

    serialib serial;

    char errorOpen = serial.openDevice(SERIAL_PORT, 115200);

    if(argc < 2) {
        printf("need at least one filename");
        return 0;
    }

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
    shifter.count = 129;
    shifter.msgTemplate.slice = 5;

    StreamNode sendData(success, 4096);
    ifstream romFile;
    romFile.open(argv[1], ios::binary);
    sendData.streams.push_back(&romFile);
    sendData.it = sendData.streams.begin();
    sendData.msgTemplate.skipLines = 0;

    IncrementingNode writeMulti(sendData, "writeMulti %x 1000\r", 32);
    writeMulti.msgTemplate.shouldWait = true;
    writeMulti.msgTemplate.skipLines = 0;
    writeMulti.params[0].val = 0;
    writeMulti.params[0].increment = 4096;
    writeMulti.count = 5;
    writeMulti.loop = true;

    writeMulti.loopLink = &shifter;
    sendData.connect("ACK4096", &writeMulti);
    shifter.connect("> shi", &writeMulti);
    shifter.connect("shift", &writeMulti);

    SendStringNode eraseChip(fail, "eraseChip\r");
    eraseChip.msgTemplate.timeoutMs = 30000;
    eraseChip.msgTemplate.skipLines = 2;
    eraseChip.connect("Done", &shifter);

    SendStringNode askVersion(fail, "version\r");
    askVersion.connect("GTCP2-0.0.2", &eraseChip);
    askVersion.msgTemplate.skipLines = 2;

    NopNode wakeupMsg(askVersion, "wakeupMsg");
    wakeupMsg.msgTemplate.delimiter = '!';

    machine.run(wakeupMsg);

    serial.closeDevice();
    romFile.close();

    return 0;
}