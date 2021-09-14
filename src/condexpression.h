#ifndef CONDEXPRESSION_H
#define CONDEXPRESSION_H

#include "node.h"
#include "visitor.h"


class CondExpression : public Node
{
public:
    CondExpression(OrExpression* expr, Expression* if_expr, CondExpression* else_expr);
    void accept(Visitor *v) override;
    void print_node(int tabs) const override;

    OrExpression* expr          { nullptr };
    Expression* if_expr         { nullptr };
    CondExpression* else_expr   { nullptr };
};

#endif // CONDEXPRESSION_H
