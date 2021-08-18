#ifndef EQUALITYEXPRESSION_H
#define EQUALITYEXPRESSION_H

#include "node.h"
#include "visitor.h"
#include "token.h"


class EqualityExpression : public Node
{
public:
    EqualityExpression(RelationalExpression* expr);
    EqualityExpression(RelationalExpression* expr, Token op, EqualityExpression* tail);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    RelationalExpression* expr;
    Token op;
    EqualityExpression* tail;
};

#endif // EQUALITYEXPRESSION_H
