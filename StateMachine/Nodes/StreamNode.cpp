#include "StreamNode.h"

#include <cstring>
#include <cstdlib>
#include <stdio.h>

StreamNode::StreamNode(AbstractStateNode& nextNode, size_t stride) : AbstractStateNode(nextNode), _stride{stride} {
    msgTemplate.skipLines = 1;
}

SerialMessage StreamNode::onEnter() {
    SerialMessage newMessage = msgTemplate;
    if(it!=streams.end()) {
        newMessage.length = 0;
        do {
            newMessage.data = (char*) malloc(_stride);
            (*it)->read(newMessage.data, _stride);
            newMessage.length = (*it)->gcount();
            if((*it)->eof()) {
                ++it;
            }
        } while((newMessage.length == 0) && it!=streams.end());
    }
    return newMessage;
}

AbstractStateNode& StreamNode::nextNode(SerialMessage msg) {
    if(it!=streams.end()) {
        return AbstractStateNode::nextNode(msg);
    } else {
        return _nextNode;
    }
}