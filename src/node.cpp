#include "node.h"

std::string Node::tabs_string(int i)
{
    std::string tabs = "";
    for (int j = 0; j < i; j++) {
        tabs += "  ";
    }
    return tabs;
}
