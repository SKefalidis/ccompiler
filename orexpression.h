#ifndef OREXPRESSION_H
#define OREXPRESSION_H

#include "node.h"
#include "visitor.h"
#include "token.h"


class OrExpression : public Node
{
public:  
    OrExpression(AndExpression* expr);
    OrExpression(AndExpression* expr, Token op, OrExpression* tail);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    AndExpression* expr;
    Token op;
    OrExpression* tail;
};

#endif // OREXPRESSION_H
