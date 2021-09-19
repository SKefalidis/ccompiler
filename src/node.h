#ifndef NODE_H
#define NODE_H

#include <string>
#include "visitor.h"


enum class NodeType {
    ADDITIVE_EXPRESSION,
    AND_EXPRESSION,
    BLOCK_ITEM,
    COND_EXPRESSION,
    DECLARATION,
    EQUALITY_EXPRESSION,
    EXPRESSION,
    FACTOR,
    FUNCTION_CALL,
    FUNCTION_DECLARATION,
    GOAL,
    OR_EXPRESSION,
    RELATIONAL_EXPRESSION,
    STATEMENT,
    TERM,
    UNARY_OPERATOR,

    NONE
};

class Node
{
public:
    Node(NodeType type);

    virtual void accept(Visitor* v) = 0;
    virtual void print_node(int tabs) const = 0;
    static std::string tabs_string(int i);

    NodeType type { NodeType::NONE };
};

#endif // NODE_H
