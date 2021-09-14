#ifndef NODE_H
#define NODE_H

#include <string>
#include "visitor.h"


class Node
{
public:
    virtual void accept(Visitor* v) = 0;
    virtual void print_node(int tabs) const = 0;
    static std::string tabs_string(int i);
};

#endif // NODE_H
