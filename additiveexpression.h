#ifndef ADDITIVEEXPRESSION_H
#define ADDITIVEEXPRESSION_H

#include <string>
#include "visitor.h"
#include "node.h"
#include "token.h"


class AdditiveExpression : public Node
{
public:
    AdditiveExpression(Term* term);
    AdditiveExpression(Term* term, Token op, AdditiveExpression* tail);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    Term* term;
    Token op;
    AdditiveExpression* tail;
};

#endif // ADDITIVEEXPRESSION_H
