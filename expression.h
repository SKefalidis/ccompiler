#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include "visitor.h"
#include "literal.h"
#include "unaryoperator.h"


class Expression
{
public:
    Expression(Literal* literal);
    Expression(UnaryOperator* op, Expression* expr);
    void accept(Visitor* v);

    Literal* value;
    UnaryOperator* unary_op;
    Expression* expr;
};

#endif // EXPRESSION_H
