#include "NopNode.h"

#include <stdio.h>

NopNode::NopNode() : _nextNode{*this}, _logString{nullptr} {
}

NopNode::NopNode(AbstractStateNode& nextNode) : _nextNode{nextNode}, _logString{nullptr} {
}

NopNode::NopNode(char* logString) : _nextNode{*this}, _logString{logString} {
}

NopNode::NopNode(AbstractStateNode& nextNode, char* logString) : _nextNode{nextNode}, _logString{logString} {
}

SerialMessage NopNode::onEnter() {
    SerialMessage newMessage = msgTemplate;
    if(_logString != nullptr) {
        printf("%s\n", _logString);
    }
    return newMessage;
}

AbstractStateNode& NopNode::nextNode(SerialMessage msg) {
    return _nextNode;
}