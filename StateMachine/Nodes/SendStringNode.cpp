#include "SendStringNode.h"

#include <cstring>
#include <cstdlib>
#include <stdio.h>

SendStringNode::SendStringNode(AbstractStateNode& nextNode, char* cmd) : _nextNode{nextNode}, _cmdString{cmd} {
    msgTemplate.skipLines = 2;
}

SerialMessage SendStringNode::onEnter() {
    SerialMessage newMessage = msgTemplate;
    newMessage.length = strlen(_cmdString)+1;
    newMessage.data = (char*) calloc(newMessage.length, sizeof(char));
    strcpy(newMessage.data, _cmdString);
    printf("Sending: %s\n", newMessage.data);
    return newMessage;
}

AbstractStateNode& SendStringNode::nextNode(SerialMessage msg) {
    if(msg.length) {
        char *cur = msg.data;
        while(*cur) {
            printf("%x ", *cur);
            cur++;
        }
        printf("\n");
        std::string s(msg.data, msg.length);
        map<string, AbstractStateNode*>::iterator i = _links.find(s);
        if(i != _links.end()) {
            return *(i->second);
        }
    }
    return _nextNode;
}

void SendStringNode::connect(string response, AbstractStateNode* next) {
    _links.insert(pair<string, AbstractStateNode*>(response, next));
}