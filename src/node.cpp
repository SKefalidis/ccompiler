#include "node.h"

Node::Node(NodeType type)
    : type(type)
{
    ;
}

std::string Node::tabs_string(int i)
{
    std::string tabs = "";
    for (int j = 0; j < i; j++) {
        tabs += "  ";
    }
    return tabs;
}
