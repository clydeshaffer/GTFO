#pragma once
#include "AbstractStateNode.h"

class NopNode: public AbstractStateNode {
    private:
        AbstractStateNode& _nextNode;
        char* _logString;
    public:
        NopNode();
        NopNode(AbstractStateNode& next);
        NopNode(char* logString);
        NopNode(AbstractStateNode& next, char* logString);
        SerialMessage onEnter();
        AbstractStateNode& nextNode(SerialMessage msg);
};