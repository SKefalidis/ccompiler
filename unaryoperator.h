#ifndef UNARYOPERATOR_H
#define UNARYOPERATOR_H

#include "visitor.h"
#include "node.h"
#include "token.h"


class UnaryOperator : public Node
{
public:
    UnaryOperator(Token t);
    UnaryOperator(Token t, std::string id);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    Token op;
    std::string id { "" };
};

#endif // UNARYOPERATOR_H
