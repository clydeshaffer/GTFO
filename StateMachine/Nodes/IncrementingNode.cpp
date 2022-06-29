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

    if(loop && !ranOnce) {
        _startCount = count;
        for(int i = 0; i < INCREMENTER_MAX_PARAMS; i++) {
            params[i]._initial = params[i].val;
        }
    }

    if(newMessage.length && count) {
        newMessage.data = (char*) calloc(newMessage.length, sizeof(char));
        newMessage.length = sprintf(newMessage.data, _fmtString, 
            params[0].val,
            params[1].val,
            params[2].val,
            params[3].val,
            params[4].val,
            params[5].val);
        count--;
    }
    if(!count) {
        if(loop) {
            count = _startCount;
            _didLoop = true;
            for(int i = 0; i < INCREMENTER_MAX_PARAMS; i++) {
                params[i].val = params[i]._initial;
            }
            newMessage.length = 0;
            newMessage.shouldWait = false;
        }
    } else {
        for(int i = 0; i < INCREMENTER_MAX_PARAMS; i++) {
            params[i].val += params[i].increment;
        }
    }

    if(!newMessage.shouldWait && newMessage.length) {
        printf("SENDING %s\n", newMessage.data);
    }

    return newMessage;
}

AbstractStateNode& IncrementingNode::nextNode(SerialMessage msg) {
    if(count) {
        if(_didLoop) {
            _didLoop = false;
            if(loopLink != nullptr)
                return *loopLink;
        }
        return AbstractStateNode::nextNode(msg);
    } else {
        return _nextNode;
    }
}