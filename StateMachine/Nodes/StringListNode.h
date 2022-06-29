#pragma once
#include "AbstractStateNode.h"

#include <vector>
#include <string>

class StringListNode: public AbstractStateNode {
    public:
        std::vector<string> cmds;
        std::vector<string>::iterator it;
        StringListNode(AbstractStateNode& next);
        SerialMessage onEnter() override;
        AbstractStateNode& nextNode(SerialMessage msg) override;
};