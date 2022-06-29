#pragma once
#include "AbstractStateNode.h"

#define INCREMENTER_MAX_PARAMS 6

typedef struct Counter {
    int val;
    int increment;
} Counter;

class IncrementingNode: public AbstractStateNode {
    private:
        char* _fmtString;
    public:
        Counter params[INCREMENTER_MAX_PARAMS];
        unsigned int count;
        IncrementingNode(AbstractStateNode& next, char* fmt, int length);
        SerialMessage onEnter() override;
        AbstractStateNode& nextNode(SerialMessage msg) override;
};