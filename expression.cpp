#include "expression.h"

Expression::Expression(Literal* literal) : value(literal)
{
    ;
}

Expression::Expression(UnaryOperator* op, Expression* expr)
    : unary_op(op), expr(expr)
{
    ;
}

void Expression::accept(Visitor* v)
{
    v->visit(this);
}
