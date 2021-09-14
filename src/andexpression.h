#ifndef ANDEXPRESSION_H
#define ANDEXPRESSION_H

#include "node.h"
#include "visitor.h"
#include "token.h"


class AndExpression : public Node
{
public:   
    AndExpression(EqualityExpression* expr);
    AndExpression(EqualityExpression* expr, Token op, AndExpression* tail);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    EqualityExpression* expr    { nullptr };
    Token op;
    AndExpression* tail         { nullptr };
};

#endif // ANDEXPRESSION_H
