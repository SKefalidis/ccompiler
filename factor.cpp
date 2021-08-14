#include "factor.h"

Factor::Factor(Literal* literal)
    : value(literal)
{
    ;
}

Factor::Factor(UnaryOperator* op, Factor* factor)
    : unary_op(op), inner_factor(factor)
{
    ;
}

Factor::Factor(Expression* expr)
    : expr(expr)
{
    ;
}

void Factor::accept(Visitor* v)
{
    v->visit(this);
}
