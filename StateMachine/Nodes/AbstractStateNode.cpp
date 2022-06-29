#include "AbstractStateNode.h"

AbstractStateNode::AbstractStateNode() : _nextNode{*this} {
}

AbstractStateNode::AbstractStateNode(AbstractStateNode& nextNode) : _nextNode{nextNode} {
}

void AbstractStateNode::connect(string response, AbstractStateNode* next) {
    _links.insert(pair<string, AbstractStateNode*>(response, next));
}

AbstractStateNode& AbstractStateNode::nextNode(SerialMessage msg) {
    if(msg.shouldWait && msg.length) {
#ifdef MATCHER_DEBUG        
        char *cur = msg.data;
        while(*cur) {
            printf("%x ", *cur);
            cur++;
        }
        printf("\n");
#endif
        std::string s(msg.data, msg.length);
        map<string, AbstractStateNode*>::iterator i = _links.find(s);
        if(i != _links.end()) {
            return *(i->second);
        }
    }
    return _nextNode;
}

SerialMessage AbstractStateNode::onEnter() {
    SerialMessage newMessage = msgTemplate;
    return newMessage;
}