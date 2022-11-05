using namespace std;

#include "lib/cxxopts/include/cxxopts.hpp"

#include "lib/serialib/lib/serialib.h"
#include "StateMachine/SerialStateMachine.h"

#include "StateMachine/Nodes/SendStringNode.h"
#include "StateMachine/Nodes/NopNode.h"
#include "StateMachine/Nodes/IncrementingNode.h"
#include "StateMachine/Nodes/StreamNode.h"
#include "StateMachine/Nodes/StringListNode.h"

#include <stdio.h>
#include <fstream>
#include <string>

#define SERIAL_PORT "/dev/ttyACM0"

int checkBankExtension(const char* str) {
    string s(str);
    s = s.substr(s.length()-7);
    string s2 = s.substr(0, 5);
    if(s2.compare(string(".bank")) == 0) {
        return strtol(s.substr(5).c_str(), nullptr, 16);
    }
    return -1;
}

string getBankNum(const char* str) {
    string s(str);
    return s.substr(s.length()-2);
}

int main(int argc, char** argv) {

    serialib serial;
    bool usingBankFiles = false;

    cxxopts::Options options("GTFO", "GameTank Flashing Overhauled");
    options.add_options()
        ("filenames", "The filename(s) to process", cxxopts::value<std::vector<string>>())
        ("p", "Serial port", cxxopts::value<string>()->default_value(SERIAL_PORT));
    options.parse_positional({"filenames"});
    auto cmdLineResults = options.parse(argc, argv);
    char errorOpen = serial.openDevice(cmdLineResults["p"].as<string>().c_str(), 115200);

    if(cmdLineResults.count("filenames") < 1) {
        printf("need at least one filename");
        return 0;
    }

    auto filenames = cmdLineResults["filenames"].as<vector<string>>();

    if(checkBankExtension(filenames.front().c_str()) != -1) {
        usingBankFiles = true;
    }  

    if(errorOpen != 1) {
        printf("Couldn't connect to %s\n", cmdLineResults["p"].as<string>().c_str());
        return errorOpen;
    }

    printf("Connected to %s\n", SERIAL_PORT);

    SerialStateMachine machine(serial);

    NopNode success(machine.endNode(), "SUCCESS");
    NopNode fail(machine.endNode(), "FAIL");
    success.msgTemplate.shouldWait = false;
    fail.msgTemplate.shouldWait = false;

    AbstractStateNode* shifter;

    //For full-chip flashing
    IncrementingNode incShifter(success, "shift %x\r", 12);
    incShifter.params[0].val = 0;
    incShifter.params[0].increment = 1;
    incShifter.count = 129;

    //For partial flashing
    StringListNode listShifter(success);

    StreamNode sendData(success, 4096);
    sendData.msgTemplate.skipLines = 0;

    if(usingBankFiles) {
        shifter = &listShifter;
        for(vector<string>::iterator fname = filenames.begin(); fname != filenames.end(); ++fname) {
            listShifter.cmds.push_back("shift " + getBankNum(fname->c_str()) + "\r");
            ifstream* stream = new ifstream();
            stream->open(fname->c_str(), ios::binary);
            sendData.streams.push_back(stream);
        } 
        listShifter.it = listShifter.cmds.begin();
    } else {
        ifstream* romFile = new ifstream();
        romFile->open(filenames.front().c_str(), ios::binary);
        sendData.streams.push_back(romFile);
        shifter = &incShifter;
    }

    shifter->msgTemplate.skipLines = 0;
    shifter->msgTemplate.slice = 5;
    sendData.it = sendData.streams.begin();

    IncrementingNode writeMulti(sendData, "writeMulti %x 1000\r", 32);
    writeMulti.msgTemplate.shouldWait = true;
    writeMulti.msgTemplate.skipLines = 0;
    writeMulti.params[0].val = 0;
    writeMulti.params[0].increment = 4096;
    writeMulti.count = 5;
    writeMulti.loop = true;

    writeMulti.loopLink = shifter;
    sendData.connect("ACK4096", &writeMulti);
    shifter->connect("> shi", &writeMulti);
    shifter->connect("shift", &writeMulti);

    SendStringNode eraseChip(fail, "eraseChip\r");
    eraseChip.msgTemplate.timeoutMs = 30000;
    eraseChip.msgTemplate.skipLines = 2;
    eraseChip.connect("Done", shifter);

    SendStringNode askVersion(fail, "version\r");
    askVersion.connect("GTCP2-0.0.2", &eraseChip);
    askVersion.msgTemplate.skipLines = 2;

    NopNode wakeupMsg(askVersion, "wakeupMsg");
    wakeupMsg.msgTemplate.delimiter = '!';

    machine.run(wakeupMsg);

    serial.closeDevice();

    return 0;
}
