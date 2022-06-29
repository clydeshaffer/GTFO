#pragma once
#include "AbstractStateNode.h"

class SendStringNode: public AbstractStateNode {
    private:
        char* _cmdString;
    public:
        SendStringNode(AbstractStateNode& next, char* cmd);
        SerialMessage onEnter() override;
};