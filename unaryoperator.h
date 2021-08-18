#ifndef UNARYOPERATOR_H
#define UNARYOPERATOR_H

#include "visitor.h"
#include "node.h"
#include "token.h"


class UnaryOperator : public Node
{
public:
    UnaryOperator(Token t);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    Token op;
};

#endif // UNARYOPERATOR_H
