#ifndef FACTOR_H
#define FACTOR_H

#include <string>
#include "visitor.h"
#include "expression.h"
#include "literal.h"
#include "unaryoperator.h"


class Factor
{
public:
    Factor(Literal* literal);
    Factor(UnaryOperator* op, Factor* factor);
    Factor(Expression* expr);

    void accept(Visitor* v);

    Literal* value;
    UnaryOperator* unary_op;
    Factor* inner_factor;
    Expression* expr;
};

#endif // FACTOR_H
