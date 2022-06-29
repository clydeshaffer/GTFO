#pragma once

using namespace std;

typedef struct SerialMessage {
    char* data;
    unsigned int length;
    bool shouldWait;
    bool waitForDelimiter;
    unsigned int expected_response;
    char delimiter;
    unsigned int skipLines;
    SerialMessage() {
        data = nullptr;
        length = 0;
        shouldWait = true;
        waitForDelimiter = true;
        expected_response = 4096;
        delimiter = '\r';
        skipLines = 0;
    }
} SerialMessage;

class AbstractStateNode {
    public:
        virtual SerialMessage onEnter() =0;
        virtual AbstractStateNode& nextNode(SerialMessage msg) =0;
        SerialMessage msgTemplate;
};