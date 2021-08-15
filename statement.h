#ifndef STATEMENT_H
#define STATEMENT_H

#include "node.h"
#include "additiveexpression.h"
#include "visitor.h"


class Statement : public Node
{
public:
    Statement(AdditiveExpression* expr);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    AdditiveExpression* expr;
};

#endif // STATEMENT_H
