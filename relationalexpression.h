#ifndef RELATIONALEXPRESSION_H
#define RELATIONALEXPRESSION_H

#include "node.h"
#include "visitor.h"
#include "token.h"


class RelationalExpression : public Node
{
public:
    RelationalExpression(AdditiveExpression* expr);
    RelationalExpression(AdditiveExpression* expr, Token op, RelationalExpression* tail);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    AdditiveExpression* expr;
    Token op;
    RelationalExpression* tail;
};

#endif // RELATIONALEXPRESSION_H
