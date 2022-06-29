#pragma once
#include "AbstractStateNode.h"

#define INCREMENTER_MAX_PARAMS 6

typedef struct Counter {
    int val;
    int increment;
    int _initial;
} Counter;

class IncrementingNode: public AbstractStateNode {
    private:
        char* _fmtString;
        unsigned int _startCount;
        bool _didLoop = false;
    public:
        AbstractStateNode* loopLink = nullptr;
        Counter params[INCREMENTER_MAX_PARAMS];
        unsigned int count;
        bool loop = false;

        IncrementingNode(AbstractStateNode& next, char* fmt, int length);
        SerialMessage onEnter() override;
        AbstractStateNode& nextNode(SerialMessage msg) override;
};