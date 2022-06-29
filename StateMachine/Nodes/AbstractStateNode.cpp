#include "AbstractStateNode.h"

const std::string WHITESPACE = " \n\r\t\f\v";
 
std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

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
        s = trim(s);
        std::string s2;
        if(msg.slice > 0) {
            s2 = s.substr(0, msg.slice);
        } else if (msg.slice < 0) {
            s2 = s.substr((s.length() >= -msg.slice) ? s.length() + msg.slice : 0);
        } else {
            s2 = s;
        }
        map<string, AbstractStateNode*>::iterator i = _links.find(s2);
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