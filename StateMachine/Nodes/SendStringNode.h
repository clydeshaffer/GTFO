#pragma once
#include "AbstractStateNode.h"
#include <map>
#include <string>

class SendStringNode: public AbstractStateNode {
    private:
        AbstractStateNode& _nextNode;
        char* _cmdString;
        std::map <string, AbstractStateNode*> _links;
    public:
        SendStringNode(AbstractStateNode& next, char* cmd);
        SerialMessage onEnter();
        AbstractStateNode& nextNode(SerialMessage msg);
        void connect(string response, AbstractStateNode* next);
};