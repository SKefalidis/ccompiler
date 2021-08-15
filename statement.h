#ifndef STATEMENT_H
#define STATEMENT_H

#include "node.h"
#include "expression.h"
#include "visitor.h"


class Statement : public Node
{
public:
    Statement(Expression* expr);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    Expression* expr;
};

#endif // STATEMENT_H
