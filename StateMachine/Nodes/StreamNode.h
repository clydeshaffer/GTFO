#pragma once
#include "AbstractStateNode.h"

#include <vector>
#include <istream>

class StreamNode: public AbstractStateNode {
    size_t _stride;
    public:
        std::vector<istream*> streams;
        std::vector<istream*>::iterator it;
        StreamNode(AbstractStateNode& next, size_t stride);
        SerialMessage onEnter() override;
        AbstractStateNode& nextNode(SerialMessage msg) override;
};