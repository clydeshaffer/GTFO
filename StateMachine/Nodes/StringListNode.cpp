#include "StringListNode.h"

#include <cstring>
#include <cstdlib>
#include <stdio.h>

StringListNode::StringListNode(AbstractStateNode& nextNode, char* fmt, int length) : AbstractStateNode(nextNode), _fmtString{fmt} {
    msgTemplate.skipLines = 1;
    msgTemplate.length = length;
}

SerialMessage StringListNode::onEnter() {
    SerialMessage newMessage = msgTemplate;
    if(it!=cmds.end()) {
        newMessage.length = strlen(it->c_str())+1;
        newMessage.data = (char*) calloc(newMessage.length, sizeof(char));
        strcpy(newMessage.data, it->c_str());
        ++it;
    }
    return newMessage;
}

AbstractStateNode& StringListNode::nextNode(SerialMessage msg) {
    if(it!=cmds.end()) {
        return AbstractStateNode::nextNode(msg);
    } else {
        return _nextNode;
    }
}