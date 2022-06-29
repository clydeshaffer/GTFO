#pragma once
#include "AbstractStateNode.h"

class NopNode: public AbstractStateNode {
    private:
        char* _logString;
    public:
        NopNode();
        NopNode(AbstractStateNode& next);
        NopNode(char* logString);
        NopNode(AbstractStateNode& next, char* logString);
        SerialMessage onEnter() override;
        AbstractStateNode& nextNode(SerialMessage msg) override;
};