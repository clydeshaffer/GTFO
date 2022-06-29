#pragma once
#include "AbstractStateNode.h"

#include <vector>
#include <string>

typedef struct Counter {
    int val;
    int increment;
} Counter;

class StringListNode: public AbstractStateNode {
    private:
        char* _fmtString;
    public:
        std::vector<string> cmds;
        std::vector<string>::iterator it;
        StringListNode(AbstractStateNode& next, char* fmt, int length);
        SerialMessage onEnter() override;
        AbstractStateNode& nextNode(SerialMessage msg) override;
};