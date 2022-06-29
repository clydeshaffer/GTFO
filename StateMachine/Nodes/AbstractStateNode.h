#pragma once

using namespace std;

#include <map>
#include <string>
#include <stdio.h>

typedef struct SerialMessage {
    char* data;
    unsigned int length;
    bool shouldWait;
    bool waitForDelimiter;
    unsigned int expected_response;
    char delimiter;
    unsigned int skipLines;
    unsigned long timeoutMs;
    SerialMessage() {
        data = nullptr;
        length = 0;
        shouldWait = true;
        waitForDelimiter = true;
        expected_response = 4096;
        delimiter = '\r';
        skipLines = 0;
        timeoutMs = 10000;
    }
} SerialMessage;

class AbstractStateNode {
    protected:
        std::map <string, AbstractStateNode*> _links;
        AbstractStateNode& _nextNode;

    public:
        SerialMessage msgTemplate;

        AbstractStateNode();
        AbstractStateNode(AbstractStateNode& nextNode);
        virtual AbstractStateNode& nextNode(SerialMessage msg);

        virtual SerialMessage onEnter();
        
        void connect(string response, AbstractStateNode* next);
};