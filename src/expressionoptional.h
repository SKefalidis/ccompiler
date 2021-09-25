#ifndef EXPRESSIONOPTIONAL_H
#define EXPRESSIONOPTIONAL_H

#include "node.h"
#include "visitor.h"

class ExpressionOptional : public Node
{
public:
    ExpressionOptional(Expression* expr);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    bool is_empty();

    Expression* expr;
};

#endif // EXPRESSIONOPTIONAL_H
