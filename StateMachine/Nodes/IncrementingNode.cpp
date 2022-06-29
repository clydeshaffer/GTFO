#include "IncrementingNode.h"

#include <cstring>
#include <cstdlib>
#include <stdio.h>

IncrementingNode::IncrementingNode(AbstractStateNode& nextNode, char* fmt, int length) : AbstractStateNode(nextNode), _fmtString{fmt} {
    msgTemplate.skipLines = 2;
    msgTemplate.length = length;
}

SerialMessage IncrementingNode::onEnter() {
    SerialMessage newMessage = msgTemplate;
    if(newMessage.length && count) {
        newMessage.data = (char*) calloc(newMessage.length, sizeof(char));
        sprintf(newMessage.data, _fmtString, 
            params[0].val,
            params[1].val,
            params[2].val,
            params[3].val,
            params[4].val,
            params[5].val);
        count--;
    }
    for(int i = 0; i < INCREMENTER_MAX_PARAMS; i++) {
        params[i].val += params[i].increment;
    }
    return newMessage;
}

AbstractStateNode& IncrementingNode::nextNode(SerialMessage msg) {
    if(count) {
        return AbstractStateNode::nextNode(msg);
    } else {
        return _nextNode;
    }
}