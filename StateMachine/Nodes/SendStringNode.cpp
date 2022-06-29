#include "SendStringNode.h"

#include <cstring>
#include <cstdlib>
#include <stdio.h>

SendStringNode::SendStringNode(AbstractStateNode& nextNode, char* cmd) : AbstractStateNode(nextNode), _cmdString{cmd} {
    msgTemplate.skipLines = 1;
}

SerialMessage SendStringNode::onEnter() {
    SerialMessage newMessage = msgTemplate;
    newMessage.length = strlen(_cmdString)+1;
    newMessage.data = (char*) calloc(newMessage.length, sizeof(char));
    strcpy(newMessage.data, _cmdString);
    return newMessage;
}