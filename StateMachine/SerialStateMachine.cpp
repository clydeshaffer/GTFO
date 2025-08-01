#include "SerialStateMachine.h"

#include <cstdlib>
#include <stdio.h>
#include <typeinfo>
#include <stdexcept>

SerialStateMachine::SerialStateMachine(serialib& c) : _conn{c} , _endNode{new NopNode("EXIT")} {
    _endNode->msgTemplate.shouldWait = false;
    _endNode->msgTemplate.expected_response = 0;
    _conn = c;
}

AbstractStateNode& SerialStateMachine::endNode() {
    return *_endNode;
}

int throwIfNegative(int x) {
    if(x < 0) {
        throw std::range_error("invalid number of bytes read");
    }
    return x;
}

void SerialStateMachine::run(AbstractStateNode& start) {
    SerialMessage msg;
    AbstractStateNode* currentNode = &start;
    while(currentNode != _endNode) {
        //Add verbose flag?
        //printf("running %s\n", typeid(*currentNode).name());
        msg = currentNode->onEnter();

        unsigned int totalWrittenBytes = 0;
        unsigned int lastWrittenBytes = 0;
        while((msg.length > 0) && (totalWrittenBytes < msg.length)) {
            _conn.writeBytes(msg.data+totalWrittenBytes, msg.length - totalWrittenBytes, &lastWrittenBytes);
            totalWrittenBytes += lastWrittenBytes;
        }


        if(msg.data != nullptr) {
            free(msg.data);
            msg.data = nullptr;
        }

        if(msg.shouldWait) {
            msg.data = (char*) calloc(msg.expected_response, sizeof(char));
            if(msg.waitForDelimiter) {
                for(unsigned int i = 0; i < msg.skipLines; i++) {
                    char byte = 0;
                    do {
                        if(_conn.readChar(&byte, msg.timeoutMs) != 1)
                            break;
                        printf("%c", byte);
                    } while (byte != msg.delimiter);
                }
                msg.length = throwIfNegative(_conn.readString( msg.data, msg.delimiter, msg.expected_response, msg.timeoutMs));
                if(msg.length < msg.expected_response) {
                    printf("%s\n", msg.data);
                }
            } else {
                msg.length = throwIfNegative(_conn.readBytes(msg.data, msg.expected_response, msg.timeoutMs, 100));
                printf("Received %d raw bytes\n", msg.length);
            }
        }

        currentNode->ranOnce = true;
        currentNode = &currentNode->nextNode(msg);
        if(msg.data != nullptr) {
            free(msg.data);
        }
    }
}