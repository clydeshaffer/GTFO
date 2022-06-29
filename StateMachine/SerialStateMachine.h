#pragma once
using namespace std;

#include "serialib.h"
#include "Nodes/AbstractStateNode.h"
#include "Nodes/NopNode.h"

class SerialStateMachine {
    private:
        serialib& _conn;
        NopNode* _endNode;
        char buf[4096];
    public:
        SerialStateMachine(serialib& c);
        void run(AbstractStateNode& start);
        AbstractStateNode& endNode();
};